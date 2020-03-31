// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains a lot of stuff dealing with buffers.

#include "txinline.h"
#include "txparser.h"
#include <fmt/format.h>
#include <unistd.h>

namespace {
    Buffer thebuffer; // a scratch buffer

    /// Returns the current escape char (used for printing)
    auto current_escape_char() -> long { return the_parser.eqtb_int_table[escapechar_code].val; }

    void dump_identification(String s) { main_ns::log_or_tty << "Configuration file identification: " << s; }

    /// Returns a temporary string, corresponding to the command with
    /// an empty name, without initial escape char.
    auto null_cs_name() -> String {
        auto c = current_escape_char();
        if (c == '\\') return "csname\\endcsname";
        if (c > 0 && c < int(nb_characters)) {
            Buffer B;
            B << "csname";
            B.out_log(codepoint(char32_t(to_unsigned(c))), the_main->log_encoding);
            B << "endcsname";
            return B.c_str();
        }
        if (c == 0) return "csname^^@endcsname";
        return "csnameendcsname";
    }

    /// Remove initial and final spaces.
    auto without_end_spaces(String T) -> std::string {
        while (is_space(T[0])) T++;
        Buffer tmp;
        tmp.push_back(T);
        tmp.remove_space_at_end();
        return tmp.to_string();
    }
} // namespace

void Buffer::reset(size_t k) {
    wptr     = k;
    at(wptr) = 0;
}

auto Buffer::next_non_space(size_t j) const -> size_t {
    while (is_spaceh(j)) j++;
    return j;
}

// Returns a copy, starting at k.
auto Buffer::to_string(size_t k) const -> std::string {
    auto n = strlen(data() + k);
    the_parser.my_stats.one_more_string(n + 1);
    return std::string(data() + k);
}

void Buffer::push_back_braced(String s) {
    push_back_open_brace();
    push_back(s);
    push_back_close_brace();
}

void Buffer::push_back_braced(const std::string &s) {
    push_back_open_brace();
    push_back(s);
    push_back_close_brace();
}

// Inserts a character in the buffer. Always adds a null after it.
void Buffer::push_back(char c) {
    alloc(1);
    at(wptr) = c;
    wptr++;
    at(wptr) = 0;
}

void Buffer::push_back(uchar c) { push_back(static_cast<char>(c)); }

// Same code, but takes a 7 bit character as argument.
// It could also be a part of a utf8 characater. Is left unchanged then.
void Buffer::push_back_xml_char(uchar c) {
    if (c == 0)
        at(ptr) = 0; // may be required
    else if (c == 13)
        push_back('\n');
    else if (c == '<')
        push_back("&lt;");
    else if (c == '>')
        push_back("&gt;");
    else if (c == '&')
        push_back("&amp;");
    else if (c < 32)
        push_back_ent(codepoint(static_cast<char>(c)));
    else
        push_back(static_cast<char>(c));
}

void Buffer::alloc(size_t n) {
    resize(wptr + n + 1);
    at(wptr) = 0;
}

auto Buffer::convert_to_str() const -> String {
    the_parser.my_stats.one_more_string(wptr + 1);
    char *aux = new char[wptr + 1];
    memcpy(aux, data(), wptr + 1);
    return aux;
}

void Buffer::push_back(const std::string &s) {
    alloc(s.size());
    for (auto c : s) at(wptr++) = c;
    at(wptr) = 0;
}

void Buffer::push_back_substring(String S, size_t n) {
    alloc(n);
    strncpy(data() + wptr, S, n);
    wptr += n;
    at(wptr) = 0;
}

void Buffer::push_back_substring(const std::string &S, size_t p, size_t n) {
    alloc(n);
    for (size_t i = 0; i < n; i++) at(wptr + i) = S[p + i];
    wptr += n;
    at(wptr) = 0;
}

void Buffer::push_back(long n) { push_back(fmt::format("{}", n)); }

// In case of error, we add the current line number as attribute
// via this function
auto Parser::cur_line_to_istring() const -> Istring { return Istring(fmt::format("{}", get_cur_line())); }

// Sets ptr1 to ptr, advances ptr to after a command, returns false in case
// of failure, either because cur char is not a \, or last char is \.
auto Buffer::after_slash() -> bool {
    if (head() != '\\') return false;
    ptr1 = ptr;
    advance();
    if (head() == 0) return false;
    if (!is_letter(head()))
        advance();
    else
        while (is_letter(head())) advance();
    return true;
}

// Sets (ptr1,ptr) to the next macro. In case of failure, returns false.
auto Buffer::next_macro() -> bool {
    if (at_eol()) return false;
    while ((head() != 0) && head() != '\\') advance();
    return after_slash();
}

// Sets ptr to the start of the next macro. Ignores non-letter macros
// Handles comments and newlines. Returns false in case of failure.
auto Buffer::next_macro_spec() -> bool {
    for (;;) {
        if (at_eol()) return false;
        if (head() == '%') {
            while (!at_eol() && head() != '\n') advance();
            if (at_eol()) return false;
        } else if (head() != '\\')
            advance();
        else if (!is_letter(at(ptr + 1)))
            advance(2);
        else
            return true;
    }
}

// Sets (ptr1,ptr) to the next macro, if it is \begin or \end.
// Returns false in case of failure.
auto Buffer::next_env_spec() -> bool {
    for (;;) {
        if (!next_macro_spec()) return false;
        ptr1 = ptr;
        advance();
        skip_letter();
        if ((substring() == "\\begin") || (substring() == "\\end")) return true;
    }
}

// Converts the entire Buffer to lower case
void Buffer::lowercase() {
    for (size_t j = 0; j < wptr; j++)
        if (is_upper_case(at(j))) at(j) += 'a' - 'A';
}

// Converts the entire buffer to upper case
void Buffer::uppercase() {
    for (auto j = wptr; j > 0; j--)
        if (is_lower_case(at(j - 1))) at(j - 1) += 'A' - 'a';
}

// Replaces newline by space
void Buffer::no_newline() {
    for (size_t j = 0; j < wptr; j++)
        if (at(j) == '\n') at(j) = ' ';
}

// Returns the part of the buffer between ptr1 (included) and ptr (excluded).
auto Buffer::substring() const -> std::string { return std::string(begin() + to_signed(ptr1), begin() + to_signed(ptr)); }

auto Buffer::c_str(size_t k) const -> String { return data() + k; }

// Replaces trailing cr-lf by lf.
void Buffer::push_back_newline() {
    if (wptr >= 1 && at(wptr - 1) == '\r') // for windows
        wptr--;
    push_back('\n');
}

auto Buffer::push_back_newline_spec() -> bool {
    push_back('\n');
    if (wptr == 1) return true; // keep empty lines
    if (at(0) == '#') {
        const String match = "## tralics ident rc=";
        if (strncmp(data(), match, 20) == 0) {
            size_t k = 20;
            while (k + 1 < wptr && at(k) != '$') k++;
            if (at(k) == '$') {
                char c    = at(k + 1);
                at(k + 1) = 0;
                dump_identification(data() + 20);
                at(k + 1) = c;
                main_ns::log_or_tty << " " << data() + k + 1;
            } else
                dump_identification(data() + 20);
        }
        return false;
    }
    if (at(0) == '%' || is_space(at(0))) return true;
    if (strncmp(data(), "Begin", 5) == 0) return true;
    if (strncmp(data(), "End", 3) == 0) return true;
    Buffer B;
    B.push_back(data());
    reset();
    push_back(' ');
    push_back(B.c_str());
    return true;
}

// This returns the number of keywords in the list.
auto Splitter::count() const -> size_t {
    size_t n = 1;
    for (size_t i = 0; i < size; i++)
        if (S[i] == ',') n++;
    return n;
}

// This returns the next keyword
auto Splitter::get_next_raw() -> String {
    auto p = pos;
    while (pos < size && S[pos] != ',') pos++;
    auto n = pos - p;
    if (pos < size && S[pos] == ',') pos++;
    if (n == 0) return "";
    Buffer &B = thebuffer;
    B.reset();
    B.push_back_substring(S, p, n);
    return B.c_str();
}

// Returns the next keyword, or empty string if there is none.
// Uses next_for_splitter to see the start and end of the keyword
auto Splitter::get_next() -> std::string {
    String T = get_next_raw();
    while ((T[0] != 0) && is_space(T[0])) T++;
    if (T[0] == 0) return "";
    Buffer B;
    B.push_back(T);
    B.remove_space_at_end();
    return B.to_string();
}
// Constructs the next pair. We first call next_for_splitter,
// Assume that this puts Key=Val in the buffer. We set key and val.
// If no equals sign is given, the string is the key, the value is empty.
void Splitter::extract_keyval(std::string &key, std::string &val) {
    key      = "";
    val      = "";
    String T = get_next_raw();
    if (T[0] == 0) return;
    size_t i = 0;
    while ((T[i] != 0) && T[i] != '=') i++;
    if (T[i] == '=') {
        thebuffer.at(i) = 0;
        val             = without_end_spaces(T + i + 1);
    }
    key = without_end_spaces(T);
}

// Return the value associated to the key x, or empty string if not found.
auto SpecialHash::find(String x) const -> std::string {
    for (size_t i = 0; i < size; i++)
        if (key[i] == x) return value[i];
    return "";
}

// This splits the string S, then creates a hash table.
void SpecialHash::create(String s) {
    Splitter S(s);
    auto     n = S.count();
    key.reserve(n);
    value.reserve(n);
    size = n;
    for (size_t j = 0; j < size; j++) {
        std::string a, b;
        S.extract_keyval(a, b);
        key.push_back(a);
        value.push_back(b);
    }
}

void Buffer::remove_last(size_t n) {
    if (wptr >= n) {
        wptr -= n;
        at(wptr) = 0;
    }
}

void Buffer::remove_last_quote() {
    if (last_char() == '\'') remove_last();
}

// FIXME: utf8 space ok  here ?
// This removes one space or an &nbspace;
void Buffer::remove_last_space() {
    if (wptr > 0 && is_spaceh(wptr - 1))
        wptr--;
    else if (wptr >= 6 && strncmp(data() + wptr - 6, "&nbsp;", 6) == 0)
        wptr -= 6;
    else if (wptr >= 6 && strncmp(data() + wptr - 6, "&#xA;", 5) == 0)
        wptr -= 6;
    at(wptr) = 0;
}

// FIXME: utf8 space ok  here ?
// This removes all spaces, and terminates the string
void Buffer::remove_space_at_end() {
    while (wptr > 0 && is_spaceh(wptr - 1)) wptr--;
    at(wptr) = 0;
}

// Inserts the current escape char, unless zero or out of range.
// This is used for transcript only
void Buffer::insert_escape_char() {
    auto c = current_escape_char();
    if (c >= 0 && c < int(nb_characters))
        out_log(codepoint(char32_t(to_unsigned(c))), the_main->log_encoding);
    else if (c == 0)
        push_back("^^@");
}

void Buffer::insert_escape_char_raw() {
    auto c = current_escape_char();
    if (c > 0 && c < int(nb_characters))
        push_back(codepoint(char32_t(to_unsigned(c))));
    else if (c == 0)
        push_back("^^@");
}

// This is the TeX command \string ; if esc is false, no escape char is inserted
void Parser::tex_string(Buffer &B, Token T, bool esc) const {
    if (T.not_a_cmd())
        B.push_back(T.char_val());
    else {
        auto x = T.val;
        if (esc && x >= single_offset) B.insert_escape_char_raw();
        if (x >= hash_offset)
            B.push_back(hash_table[T.hash_loc()]);
        else if (x < first_multitok_val)
            B.push_back(T.char_val());
        else
            B.push_back(null_cs_name());
    }
}

// Returns a temporary string: the name of the token
// This is used for printing errors in the transcript file
// Uses the function below, except for characters
auto Token::tok_to_str() const -> std::string {
    Buffer B;
    if (!is_a_char() || cmd_val() == eol_catcode) {
        B.push_back(*this);
        return B.c_str();
    }
    int       cat      = cmd_val();
    codepoint c        = char_val();
    bool      good_cat = false;
    if (!c.is_ascii() && cat == 12) good_cat = true;
    if (c.is_letter() && cat == 11) good_cat = true;
    if (good_cat)
        B.out_log(c, the_main->log_encoding);
    else {
        B.push_back("{Character ");
        B.out_log(c, the_main->log_encoding);
        B.push_back(fmt::format(" of catcode {}}}", cat));
    }
    return B.c_str();
}

// This puts the name of the token in the buffer.
// This is used when printing a token list

// returns true if a space could be added after the token
auto Buffer::push_back(Token T) -> bool {
    static Buffer        Tmp;
    output_encoding_type enc = the_main->log_encoding;
    if (T.is_null()) {
        push_back("\\invalid.");
        return false;
    }
    if (T.is_a_char()) {
        int       cmd = T.cmd_val();
        codepoint c   = T.char_val();
        if (cmd == eol_catcode) {
            push_back('#');
            push_back(static_cast<char>(c.value + '0')); // parameter
        } else if (cmd == parameter_catcode) {
            out_log(c, enc);
            out_log(c, enc);
        } else
            out_log(c, enc);
        return false;
    }
    if (!T.char_or_active()) insert_escape_char();
    if (T.active_or_single()) {
        out_log(T.char_val(), enc);
        return the_parser.has_letter_catcode(T.char_val().value);
    }
    if (T.is_in_hash()) {
        Tmp.reset();
        Tmp.push_back(the_parser.hash_table[T.hash_loc()]);
        push_back(Tmp.convert_to_log_encoding());
        return true;
    }
    push_back(null_cs_name());
    return true;
}

// if sw is true, we assume that the list will be re-evaluated in a standard env
// otherwise use current escape char and catcodes.
void Buffer::insert_token(Token T, bool sw) {
    if (T.is_null()) {
        push_back("\\invalid.");
        return;
    }
    if (T.char_or_active()) {
        codepoint c = T.char_val();
        if (T.is_a_char()) {
            int cmd = T.cmd_val();
            if (cmd == eol_catcode) {
                push_back('#');
                push_back(static_cast<char>(c.value + '0')); // parameter
            } else if (T.is_space_token())
                push_back(' '); // space or newline
            else if (c.is_null())
                push_back("^^@"); // if cmd==parameter_catcode ??
            else if (cmd == parameter_catcode) {
                push_back(c);
                push_back(c);
            } else
                push_back(c);
        } else {
            if (c.is_null())
                push_back("^^@");
            else
                push_back(c); // active character
        }
        return;
    }
    if (sw)
        push_back('\\');
    else
        insert_escape_char_raw();
    if (T.active_or_single()) {
        codepoint c = T.char_val();
        if (c.is_null())
            push_back("^^@");
        else
            push_back(c);
        bool need_space = sw ? c.is_letter() : the_parser.has_letter_catcode(c.value);
        if (need_space) push_back(' ');
    } else if (T.is_in_hash()) { // multichar
        push_back(the_parser.hash_table[T.hash_loc()]);
        push_back(' ');
    } else { // empty or bad
        if (sw)
            push_back("csname\\endcsname");
        else
            push_back(null_cs_name());
        push_back(' ');
    }
}

// In case of error, this puts a token in the XML tree
auto Buffer::convert_for_xml_err(Token T) -> Istring {
    reset();
    if (T.is_null())
        push_back("\\invalid.");
    else if (T.char_or_active()) {
        // We simplify the algorithm by printing the character as is
        codepoint c = T.char_val();
        if (c.is_null())
            push_back("^^@");
        else
            push_back_real_utf8(c);
    } else {
        push_back('\\');
        if (T.active_or_single()) {
            codepoint c = T.char_val();
            if (c.is_null())
                push_back("^^@");
            else
                push_back_real_utf8(c);
        } else if (T.is_in_hash()) {
            String s  = the_parser.hash_table[T.hash_loc()];
            auto   n  = strlen(s);
            bool   ok = true;
            for (size_t i = 0; i < n; i++) {
                auto c = s[i];
                if (c == '<' || c == '>' || c == '&' || c < 32) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                push_back(s);
            else
                for (size_t i = 0; i < n; i++) push_back_xml_char(uchar(s[i]));
        } else
            push_back("csname\\endcsname");
    }
    return Istring(*this);
}

// Print the scaled int V as a floating point in the buffer.
// This is the algorithm of Knuth
void Buffer::push_back(ScaledInt V, glue_spec unit) {
    const int unity = 1 << 16;
    auto      s     = V.get_value();
    if (s < 0) {
        push_back('-');
        s = -s;
    }
    push_back(s / unity);
    push_back('.');
    s         = 10 * (s % unity) + 5;
    int delta = 10;
    for (;;) {
        if (delta > unity) s = s + 0100000 - 50000; // s + 2^16/2 -10^6/2
        push_back(static_cast<char>('0' + (s / unity)));
        s     = 10 * (s % unity);
        delta = delta * 10;
        if (s <= delta) break;
    }
    constexpr std::array<String, 6> gptable{"pt", "fil", "fill", "filll", "", "mu"};
    push_back(gptable[unit]);
}

// Useful function for scan_dimen.
auto Buffer::trace_scan_dimen(Token T, ScaledInt v, bool mu) -> String {
    reset();
    push_back("+scandimen for ");
    push_back(T);
    if (is_spaceh(wptr - 1)) wptr--;
    push_back("->");
    push_back(v, glue_spec_pt);
    if (mu) pt_to_mu();
    push_back('\n');
    return data();
}

// Adds glue likes 3pt plus 3 fill
void Buffer::push_back(const Glue &x) {
    push_back(x.get_width(), glue_spec_pt);
    if (!x.get_stretch().null()) {
        push_back(" plus ");
        push_back(x.get_stretch(), x.get_stretch_order());
    }
    if (!x.get_shrink().null()) {
        push_back(" minus ");
        push_back(x.get_shrink(), x.get_shrink_order());
    }
}

// Replaces all `pt' by `mu'
void Buffer::pt_to_mu() {
    for (size_t i = 0;; i++) {
        if (at(i) == 0) return;
        if (at(i) != 'p') continue;
        if (at(i + 1) == 't') {
            at(i)     = 'm';
            at(i + 1) = 'u';
        }
    }
}

// The \relax is a bit strange.
void Buffer::push_back(const SthInternal &x) {
    switch (x.get_type()) {
    case it_int: push_back(x.get_int_val()); break;
    case it_dimen: push_back(ScaledInt(x.get_int_val()), glue_spec_pt); break;
    case it_glue:
        push_back(x.get_glue_val());
        push_back("\\relax ");
        break;
    case it_mu:
        push_back(x.get_glue_val());
        pt_to_mu();
        push_back("\\relax ");
        break;
    default: push_back("??");
    }
}

auto operator<<(Logger &fp, Token t) -> Logger & { return fp << t.tok_to_str(); }

auto operator<<(std::ostream &fp, Token x) -> std::ostream & { return fp << x.tok_to_str(); }

auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream & {
    thebuffer.reset();
    thebuffer.push_back(x, glue_spec_pt);
    return fp << thebuffer.c_str();
}

auto operator<<(Logger &X, const ScaledInt &x) -> Logger & {
    *(X.fp) << x;
    return X;
}

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream & {
    thebuffer.reset();
    thebuffer.push_back(x);
    fp << thebuffer.c_str();
    return fp;
}

auto operator<<(Logger &X, const Glue &x) -> Logger & {
    *(X.fp) << x;
    return X;
}

auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream & {
    thebuffer.reset();
    thebuffer.push_back(x);
    fp << thebuffer.c_str();
    return fp;
}

auto operator<<(Logger &X, const SthInternal &x) -> Logger & {
    thebuffer.reset();
    thebuffer.push_back(x);
    X << thebuffer.c_str();
    return X;
}

// We use internal encoding here.
auto operator<<(std::ostream &fp, const codepoint &x) -> std::ostream & {
    if (x.is_ascii())
        fp << static_cast<uchar>(x.value);
    else {
        thebuffer.reset();
        thebuffer.push_back(x);
        fp << thebuffer.c_str();
    }
    return fp;
}
// Duplicate code...
auto operator<<(FullLogger &fp, const codepoint &x) -> FullLogger & {
    if (x.is_ascii())
        fp << static_cast<uchar>(x.value);
    else {
        thebuffer.reset();
        thebuffer.push_back(x);
        fp << thebuffer.c_str();
    }
    return fp;
}

// We use log encoding here.
auto operator<<(Logger &fp, const codepoint &x) -> Logger & {
    if (x.is_ascii())
        fp << static_cast<uchar>(x.value);
    else {
        thebuffer.reset();
        thebuffer.out_log(x, the_main->log_encoding);
        fp << thebuffer.c_str();
    }
    return fp;
}

// Puts n in roman (in upper case roman first, the loewrcasify)
void Buffer::push_back_roman(long n) {
    auto k = wptr;
    push_back_Roman(n);
    for (auto i = k; i < wptr; i++) at(i) += 'a' - 'A';
}

// Adds n as roman upper case.
void Buffer::push_back_Roman(long n) {
    while (n >= 1000) {
        push_back('M');
        n -= 1000;
    }
    if (n <= 0) return;
    if (n >= 900) {
        push_back("CM");
        n -= 900;
    }
    if (n >= 500) {
        push_back('D');
        n -= 500;
    }
    if (n >= 400) {
        push_back("CD");
        n -= 400;
    }
    while (n >= 100) {
        push_back('C');
        n -= 100;
    }
    if (n >= 90) {
        push_back("XC");
        n -= 90;
    }
    if (n >= 50) {
        push_back('L');
        n -= 50;
    }
    if (n >= 40) {
        push_back("XL");
        n -= 40;
    }
    while (n >= 10) {
        push_back('X');
        n -= 10;
    }
    if (n >= 9) {
        push_back("IX");
        n -= 9;
    }
    if (n >= 5) {
        push_back('V');
        n -= 5;
    }
    if (n >= 4) {
        push_back("IV");
        n -= 4;
    }
    while (n > 0) {
        push_back('I');
        n--;
    }
}

// True is s is at ptr. If so, updates ptr
auto Buffer::is_here(String s) -> bool {
    auto n = strlen(s);
    if (strncmp(data() + ptr, s, n) != 0) return false;
    ptr += n;
    return true;
}

// returns the document class. value in aux
auto Buffer::find_documentclass(Buffer &aux) -> bool {
    String cmd = "\\documentclass";
    auto   k   = to_string().find(cmd);
    if (k == std::string::npos) return false;
    for (size_t j = 0; j < k; j++)
        if (at(j) == '%' && at(j + 1) == '%') return false; // double comment
    push_back("{}");                                        //  make sure we have braces \todo [vb] at the end of the Buffer ??
    k += strlen(cmd);                                       // skip command name
    while (at(k) == ' ') ++k;                               // skip spaces
    if (at(k) == '[') {
        auto cur_len = size() - 2;
        auto cur_k   = k;
        while (k < cur_len && at(k) != ']') ++k;
        if (at(k) == ']')
            ++k;
        else
            k = cur_k;
    }
    while (at(k) != '{') k++;
    auto p = k;
    while (at(p) != '}') p++;
    auto len = p - k - 1;
    if (len == 0) return false;
    aux.reset();
    aux.push_back(data() + k + 1);
    aux[len] = 0;
    for (size_t i = 0; i < len; i++) // \documentclass{Jos\351} is invalid
        if (!is_letter(aux[i]) && !is_digit(aux[i])) return false;
    return true;
}

auto Buffer::find_configuration(Buffer &aux) const -> bool {
    if (at(0) != '%') return false;
    auto k = to_string().find("ralics configuration file");
    if (k == std::string::npos) return false;
    while ((at(k) != 0) && at(k) != '\'') k++;
    if (at(k) == 0) return false;
    k++;
    size_t len = 0;
    aux.reset();
    for (;;) {
        if (at(k) == 0) return false;
        if (at(k) == '\'') break;
        aux.push_back(at(k));
        len++;
        k++;
    }
    aux[len] = 0;
    return len != 0;
}

auto Buffer::find_doctype() const -> size_t {
    if (at(0) != '%') return 0;
    String S = "ralics DOCTYPE ";
    auto   k = to_string().find(S);
    if (k == std::string::npos) return 0;
    k += strlen(S);
    while ((at(k) != 0) && (at(k) == ' ' || at(k) == '=')) k++;
    if (at(k) == 0) return 0;
    return k;
}

// If the buffer holds a single char (plus space) returns it.
// Note: this returns 0 in case of a non-7bit character
auto Buffer::single_char() const -> char {
    auto j = next_non_space(0);
    if (at(j) == 0) return 0;
    char c = at(j);
    j      = next_non_space(j + 1);
    if (at(j) != 0) return 0;
    return c;
}

// If the buffer contains a small positive number returns it.
// Otherwise returns -1;
auto Buffer::int_val() const -> std::optional<size_t> {
    size_t n = 0;
    for (size_t p = 0;; p++) {
        auto c = at(p);
        if (c == 0) return n;
        if (!is_digit(c)) return {};
        n = 10 * n + to_unsigned(c - '0');
        if (n > 1000000) return {};
    }
}

auto Buffer::find_char(char c) -> bool {
    ptr = 0;
    while ((head() != 0) && head() != c) advance();
    return head() == c;
}

// splits foo:bar into foo and bar
auto Buffer::split_at_colon(std::string &before, std::string &after) -> bool {
    if (find_char(':')) {
        at(ptr) = 0;
        after   = to_string(ptr + 1);
        before  = to_string();
        return true;
    }
    before = to_string();
    after  = "";
    return false;
}

// Sets ptr1 to the first non-space
// sets ptr to the next equals sign. Returns false if no such sign exists
auto Buffer::find_equals() -> bool {
    skip_sp_tab_nl();
    ptr1 = ptr;
    while ((head() != 0) && head() != '=') advance();
    return head() == '=';
}

// Ignores character at ptr, and following ones.
// removes the spaces just before.
// puts a null char there.
// returns false in case of trouble (only spaces).
auto Buffer::backup_space() -> bool {
    size_t j = ptr;
    while (j > ptr1 && is_spaceh(j - 1)) j--;
    if (j == ptr1) return false;
    at(j) = 0;
    return true;
}

// If there is 'foobar' at ptr, then puts ptr1 to the char after
// the quote, replaces the second quote by a null.
// returns false in  case of trouble.
auto Buffer::string_delims() -> bool {
    skip_sp_tab_nl();
    char c = head();
    if (c == 0) return false;
    advance();
    ptr1 = ptr;
    while ((head() != 0) && head() != c) advance();
    if (head() == 0) return false;
    at(ptr) = 0;
    return true;
}

// Assumes the buffer is of the form foo/bar/etc,
// with a final slash; returns the next item; Retval false if no string found

auto Buffer::slash_separated(std::string &a) -> bool {
    static Buffer tmp;
    tmp.reset();
    size_t p = 0;
    skip_sp_tab();
    if (head() == 0) return false;
    for (;;) {
        char c = head();
        if (c == 0) return false;
        advance();
        if (c == '/') break;
        if (c == '\\') {
            if (head() == 0) return false;
            if (head() == ' ') { p = tmp.size() + 1; }
            c = head();
            advance();
        }
        tmp.push_back(c);
    }
    auto b = tmp.size();
    while (b > p && is_space(tmp[b - 1])) b--;
    tmp.at(b) = 0;
    a         = tmp.to_string();
    return true;
}

void Buffer::push_back_unless_punct(char c) {
    if (wptr >= 6 && strncmp(data() + wptr - 6, "&nbsp;", 6) == 0) return;
    if (wptr > 0 && is_spaceh(wptr - 1)) return;
    if (wptr > 0) {
        char test = at(wptr - 1);
        if (test == '(') return;
    }
    push_back(c);
}

// Tries to read an argument. Sets ptr1 to after the opening brace
// ptr to the closing brace.
auto Buffer::fetch_spec_arg() -> bool {
    skip_sp_tab_nl();
    if (head() != '{') return false;
    advance();
    ptr1 = ptr;
    for (;;) {
        auto c = head();
        if (c == 0 || c == '{' || c == '%') return false;
        if (c == '}') return true;
        advance();
    }
}

auto Buffer::contains_braced(const std::string &s) -> bool {
    auto k = ptr1;
    if (!fetch_spec_arg()) return false;
    if (substring() != s) return false;
    ptr1 = k;
    advance();
    return true;
}

// Returns true if the buffer contains \end{env} with spaces.
// (used for detecting the end of a verbatim environment).
auto Buffer::contains_env(const std::string &env) -> bool {
    skip_sp_tab_nl();
    ptr1 = ptr;
    ptr  = ptr1 + 4;
    if (substring() != "\\end") return false;
    skip_sp_tab_nl();
    if (!(contains_braced(env))) return false;
    skip_sp_tab_nl();
    return true;
}

// returns true if the file exists with extension s.
auto Image::file_exists(const std::string &s) const -> bool { return tralics_ns::file_exists(name + '.' + s); }

// This checks all possible extensions and remembers them
void Image::check_existence() {
    flags = 0;
    if (file_exists("ps")) flags |= 1;
    if (file_exists("eps")) flags |= 2;
    if (file_exists("epsi")) flags |= 4;
    if (file_exists("epsf")) flags |= 8;
    if (file_exists("pdf")) flags |= 16;
    if (file_exists("png")) flags |= 32;
    if (file_exists("gif")) flags |= 64;
}

Buffer check_image1;
Buffer check_image2;
// This checks that there is a unique source for the image
void Image::check() const {
    int a = (flags & 1) != 0 ? 1 : 0;
    int b = (flags & 2) != 0 ? 1 : 0;
    int c = (flags & 4) != 0 ? 1 : 0;
    int d = (flags & 8) != 0 ? 1 : 0;
    int e = a + b + c + d;
    if (e > 1) {
        if (!check_image1.empty()) check_image1 << ", ";
        check_image1 << name;
    }
    if (flags == 0) {
        if (!check_image2.empty()) check_image2 << ", ";
        check_image2 << name;
    }
}

// Enter a new image file, if ok is false, do not increase the occ count
void Parser::enter_file_in_table(const std::string &nm, bool ok) {
    auto s = the_images.size();
    for (size_t i = 0; i < s; i++) {
        Image &X = the_images[i];
        if (X.name == nm) {
            if (ok) X.occ++;
            return;
        }
    }
    the_images.emplace_back(nm, ok ? 1 : 0);
}

void operator<<(std::fstream &X, const Image &Y) {
    X << "see_image(\"" << Y.name << "\",";
    int k = Y.flags;
    if (k == 0)
        X << 0;
    else {
        bool first = true;
        if ((k & 1) != 0) {
            if (!first) X << "+";
            X << 1;
            first = false;
        }
        if ((k & 2) != 0) {
            if (!first) X << "+";
            X << 2;
            first = false;
        }
        if ((k & 4) != 0) {
            if (!first) X << "+";
            X << 4;
            first = false;
        }
        if ((k & 8) != 0) {
            if (!first) X << "+";
            X << 8;
            first = false;
        }
        if ((k & 16) != 0) {
            if (!first) X << "+";
            X << 16;
            first = false;
        }
        if ((k & 32) != 0) {
            if (!first) X << "+";
            X << 32;
            first = false;
        }
        if ((k & 64) != 0) {
            if (!first) X << "+";
            X << 64;
            // first = false;
        }
    }
    X << "," << Y.occ << ");\n";
}

// finish handling the images,
void Parser::finish_images() {
    auto s = the_images.size();
    if (s == 0) return;
    std::string name = tralics_ns::get_short_jobname() + ".img";
    String      wn   = tralics_ns::get_out_dir(name);
    auto *      fp   = new std::fstream(wn, std::ios::out);
    if (!*fp) return;
    *fp << "# images info, 1=ps, 2=eps, 4=epsi, 8=epsf, 16=pdf, 32=png, 64=gif\n";
    check_image1.reset();
    check_image2.reset();
    for (size_t i = 0; i < s; i++) {
        if (the_images[i].occ != 0) {
            the_images[i].check_existence();
            the_images[i].check();
            *fp << the_images[i];
        }
    }
    fp->close();
    delete fp;
    if (s == 0)
        main_ns::log_or_tty << "There was no image.\n";
    else
        main_ns::log_or_tty << fmt::format("There were {} images.\n", s);
    if (!check_image1.empty()) main_ns::log_or_tty << "Following images have multiple PS source: " << Istring(check_image1) << ".\n";
    if (!check_image2.empty()) main_ns::log_or_tty << "Following images not defined: " << Istring(check_image2) << ".\n";
}

auto Buffer::get_machine_name() -> std::string {
    reset();
    alloc(200);
    if (gethostname(data(), 199) != 0) push_back("unknown");
    at(200) = 0;
    wptr    = strlen(data());
    auto n  = wptr;
    for (size_t i = 1; i < n; i++)
        if (at(i) == '.') at(i) = 0;
    return to_string();
}

// Adds a slash at the end unless there is already one.
//
void Buffer::optslash() {
    if (wptr == 0) return;
    if (wptr == 1 && at(0) == '/')
        reset();
    else if (at(wptr - 1) == '/')
        return;
    else
        push_back('/');
}

// returns location of last slash in the buffer
auto Buffer::last_slash() const -> std::optional<size_t> {
    for (size_t i = wptr; i > 0; --i)
        if (at(i - 1) == '/') { return i - 1; }
    return {};
}

// True if the string s is at the end of the buffer
auto Buffer::is_at_end(String s) const -> bool {
    size_t n = strlen(s);
    return wptr > n && strcmp(data() + wptr - n, s) == 0;
}

// Inserts the string s is at the end of the buffer unless there
void Buffer::put_at_end(String s) {
    size_t n = strlen(s);
    if (wptr > n && strcmp(data() + wptr - n, s) == 0) return;
    push_back(s);
}

auto Buffer::contains(const std::string &s) const -> bool { return to_string().find(s) != std::string::npos; }

auto Buffer::is_spaceh(size_t j) const -> bool { return is_space(at(j)); }

auto Buffer::last_char() const -> char { return (wptr == 0) ? char(0) : at(wptr - 1); }

void Buffer::push_back(const TokenList &L) {
    for (auto &C : L) { insert_token(C, false); }
}

void Buffer::skip_letter() {
    while (is_letter(head())) ptr++;
}
void Buffer::skip_sp_tab() {
    while (at(ptr) == ' ' || at(ptr) == '\t') ptr++;
}
void Buffer::skip_sp_tab_nl() {
    while (is_space(at(ptr))) ptr++;
}
void Buffer::skip_sp_tab_comma() {
    while (at(ptr) == ' ' || at(ptr) == '\t' || at(ptr) == ',') ptr++;
}

void Buffer::skip_letter_dig() {
    while (is_letter(at(ptr)) || is_digit(at(ptr))) ptr++;
}
void Buffer::skip_letter_dig_dot() {
    while (is_letter(at(ptr)) || is_digit(at(ptr)) || at(ptr) == '.') ptr++;
}
void Buffer::skip_letter_dig_dot_slash() {
    while (is_letter(at(ptr)) || is_digit(at(ptr)) || at(ptr) == '.' || at(ptr) == '/') ptr++;
}

auto Buffer::is_special_end() const -> bool { return at(ptr) == '\n' || at(ptr) == '#' || at(ptr) == '%'; }

Buffer Txbuf, err_buf, ssa2;
