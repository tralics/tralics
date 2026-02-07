// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains a lot of stuff dealing with buffers.

#include "tralics/Bibtex.h"
#include "tralics/Glue.h"
#include "tralics/Parser.h"
#include "tralics/Xml.h"
#include "tralics/util.h"
#include <cctype>
#include <ctre.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <utf8.h>

namespace {
    // This returns the number of bytes in a UTF8 character
    // given the first byte. Returns 0 in case of error
    auto how_many_bytes(char c) -> size_t { return to_unsigned(utf8::internal::sequence_length(&c)); }

    // True if it is an accent character, like \^.
    auto is_accent_char(char c) -> bool { return c == '\'' || c == '`' || c == '^' || c == '"' || c == '~' || c == '.' || c == '='; }

    // True if \sortnoop, \SortNoop, \noopsort plus brace or space
    // First char to test is at i+1 \todo regexp
    auto is_noopsort(const std::string &s, size_t i) -> bool {
        auto n = s.size();
        if (i + 10 >= n) return false;
        if (s[i + 10] != '{' && (std::isspace(s[i + 10]) == 0)) return false;
        if (s[i + 1] != '\\') return false;
        if (s[i + 3] != 'o') return false;
        if (s[i + 7] != 'o') return false;
        if (s[i + 2] == 'n' && s[i + 4] == 'o' && s[i + 5] == 'p' && s[i + 6] == 's' && s[i + 8] == 'r' && s[i + 9] == 't') return true;
        if ((s[i + 2] == 's' || s[i + 2] == 'S') && s[i + 4] == 'r' && s[i + 5] == 't' && (s[i + 6] == 'n' || s[i + 6] == 'N') &&
            s[i + 8] == 'o' && s[i + 9] == 'p')
            return true;
        return false;
    }

    /// Returns the current escape char (used for printing)
    auto current_escape_char() -> long { return eqtb_int_table[escapechar_code].val; }

    /// Returns a temporary string, corresponding to the command with
    /// an empty name, without initial escape char.
    auto null_cs_name() -> std::string {
        auto c = current_escape_char();
        if (c == '\\') return "csname\\endcsname";
        if (c > 0 && c < int(nb_characters)) {
            Buffer B = "csname";
            B.out_log(char32_t(c), the_main.log_encoding);
            B.append("endcsname");
            return std::move(B);
        }
        if (c == 0) return "csname^^@endcsname";
        return "csnameendcsname";
    }

    void err_in_name(String a, long i) {
        Bibtex::err_in_entry(a);
        spdlog::error("\nbad syntax in author or editor name\n");
        spdlog::error("error occurred at character position {} in the string\n{}.", i, fmt::streamed(name_buffer));
    }
} // namespace

auto Buffer::next_non_space(size_t j) const -> size_t {
    while (is_spaceh(j)) j++;
    return j;
}

void Buffer::push_back(uchar c) { push_back(static_cast<char>(c)); }

// Same code, but takes a 7 bit character as argument.
// It could also be a part of a utf8 characater. Is left unchanged then.
void Buffer::push_back_xml_char(char c) {
    if (c == 0) return;
    if (c == '\n')
        push_back('\n');
    else if (c == '<')
        append("&lt;");
    else if (c == '>')
        append("&gt;");
    else if (c == '&')
        append("&amp;");
    else if (c > 0 && c < 32)
        format("&#x{:X};", size_t(c));
    else
        push_back(c);
}

void Buffer::append_with_xml_escaping(char32_t c) {
    if (c == 0) return;
    if (c == '\n')
        push_back('\n');
    else if (c == '\r')
        push_back('\r');
    else if (c == '\t')
        push_back('\t');
    else if (c == '<')
        append("&lt;");
    else if (c == '>')
        append("&gt;");
    else if (c == '&')
        append("&amp;");
    else if (c < 32 || c > 65535)
        format("&#x{:X};", size_t(c));
    else
        utf8::append(c, std::back_inserter(*this));
}

// Converts the entire Buffer to lower case
auto Buffer::lowercase() -> Buffer & {
    std::transform(begin(), end(), begin(), [](uchar c) { return std::tolower(c); });
    return *this;
}

// Returns the part of the buffer between ptrs.a (included) and ptrs.b (excluded).
auto Buffer::substring() const -> std::string { return substr(ptrs.a, ptrs.b - ptrs.a); }

// Replaces trailing cr-lf by lf.
void Buffer::push_back_newline() {
    if (!empty() && back() == '\r') pop_back();
    push_back('\n');
}

auto Buffer::push_back_newline_spec() -> bool {
    if (empty()) return true;
    push_back('\n');
    if (at(0) == '#') return false;
    if (at(0) == '%' || (std::isspace(at(0)) != 0)) return true;
    if (starts_with("Begin")) return true;
    if (starts_with("End")) return true;
    insert(begin(), ' ');
    return true;
}

void Buffer::remove_last(size_t n) {
    if (size() >= n) resize(size() - n);
}

// FIXME: utf8 space ok  here ?
// This removes one space or an &nbspace;
void Buffer::remove_last_space() {
    if (!empty() && (std::isspace(uchar(back())) != 0))
        pop_back();
    else if (ends_with("&nbsp;"))
        remove_last(6);
    else if (ends_with("&#xA;"))
        remove_last(5);
}

// FIXME: utf8 space ok  here ?
// This removes all spaces, and terminates the string
void Buffer::remove_space_at_end() {
    while (!empty() && (std::isspace(uchar(back())) != 0)) pop_back();
}

// Inserts the current escape char, unless zero or out of range.
// This is used for transcript only
void Buffer::insert_escape_char() {
    auto c = current_escape_char();
    if (c >= 0 && c < int(nb_characters))
        out_log(char32_t(char32_t(c)), the_main.log_encoding);
    else if (c == 0)
        append("^^@");
}

void Buffer::insert_escape_char_raw() {
    auto c = current_escape_char();
    if (c > 0 && c < int(nb_characters))
        push_back(char32_t(char32_t(c)));
    else if (c == 0)
        append("^^@");
}

// This puts the name of the token in the buffer.
// This is used when printing a token list

// returns true if a space could be added after the token
auto Buffer::push_back(Token T) -> bool {
    output_encoding_type enc = the_main.log_encoding;
    if (T.is_null()) {
        append("\\invalid.");
        return false;
    }
    if (T.is_a_char()) {
        unsigned cmd = T.cmd_val();
        char32_t c   = T.char_val();
        if (cmd == eol_catcode) {
            push_back('#');
            push_back(static_cast<char>(c + '0')); // parameter
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
        return get_catcode(T.char_val()) == letter_catcode;
    }
    if (T.is_in_hash()) {
        append(Buffer{hash_table[T.hash_loc()]}.convert_to_log_encoding());
        return true;
    }
    append(null_cs_name());
    return true;
}

// if sw is true, we assume that the list will be re-evaluated in a standard env
// otherwise use current escape char and catcodes.
void Buffer::insert_token(Token T, bool sw) {
    if (T.is_null()) {
        append("\\invalid.");
        return;
    }
    if (T.char_or_active()) {
        char32_t c = T.char_val();
        if (T.is_a_char()) {
            auto cmd = T.cmd_val();
            if (cmd == eol_catcode) {
                push_back('#');
                push_back(static_cast<char>(c + '0')); // parameter
            } else if (T.is_space_token())
                push_back(' '); // space or newline
            else if (c == 0)
                append("^^@"); // if cmd==parameter_catcode ??
            else if (cmd == parameter_catcode) {
                push_back(c);
                push_back(c);
            } else
                push_back(c);
        } else {
            if (c == 0)
                append("^^@");
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
        char32_t c = T.char_val();
        if (c == 0)
            append("^^@");
        else
            push_back(c);
        bool need_space = sw ? (std::isalpha(static_cast<int>(c)) != 0) : (get_catcode(c) == letter_catcode);
        if (need_space) push_back(' ');
    } else if (T.is_in_hash()) { // multichar
        append(hash_table[T.hash_loc()]);
        push_back(' ');
    } else { // empty or bad
        if (sw)
            append("csname\\endcsname");
        else
            append(null_cs_name());
        push_back(' ');
    }
}

// Print the scaled int V as a floating point in the buffer.
// This is the algorithm of Knuth
void Buffer::push_back(const ScaledInt &V, glue_spec unit) {
    const int unity = 1 << 16;
    auto      s     = V.value;
    if (s < 0) {
        push_back('-');
        s = -s;
    }
    format("{}.", s / unity);
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
    append(gptable[unit]);
}

// Adds glue likes 3pt plus 3 fill
void Buffer::push_back(const Glue &x) {
    push_back(x.width, glue_spec_pt);
    if (!(x.stretch == ScaledInt{0})) {
        append(" plus ");
        push_back(x.stretch, x.stretch_order);
    }
    if (!(x.shrink == ScaledInt{0})) {
        append(" minus ");
        push_back(x.shrink, x.shrink_order);
    }
}

// Replaces all `pt' by `mu'
void Buffer::pt_to_mu() {
    for (size_t i = 0;; i++) {
        if ((*this)[i] == 0) return;
        if (at(i) != 'p') continue;
        if (at(i + 1) == 't') {
            at(i)     = 'm';
            at(i + 1) = 'u';
        }
    }
}

// The \relax is a bit strange.
void Buffer::push_back(const SthInternal &x) {
    switch (x.type) {
    case it_int: format("{}", x.get_int_val()); break;
    case it_dimen: push_back(ScaledInt(x.get_int_val()), glue_spec_pt); break;
    case it_glue:
        push_back(x.glue_val);
        append("\\relax ");
        break;
    case it_mu:
        push_back(x.glue_val);
        pt_to_mu();
        append("\\relax ");
        break;
    default: append("??");
    }
}

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream & {
    Buffer B;
    B.push_back(x);
    return fp << B;
}

auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream & {
    Buffer B;
    B.push_back(x);
    return fp << B;
}

// We use internal encoding here.
auto operator<<(std::ostream &fp, const char32_t &x) -> std::ostream & { return fp << to_utf8(x); }

// Puts n in roman (in upper case roman first, the loewrcasify)
void Buffer::push_back_roman(long n) {
    auto k = size();
    push_back_Roman(n);
    for (auto i = k; i < size(); i++) at(i) += 'a' - 'A';
}

// Adds n as roman upper case.
void Buffer::push_back_Roman(long n) {
    while (n >= 1000) {
        push_back('M');
        n -= 1000;
    }
    if (n <= 0) return;
    if (n >= 900) {
        append("CM");
        n -= 900;
    }
    if (n >= 500) {
        push_back('D');
        n -= 500;
    }
    if (n >= 400) {
        append("CD");
        n -= 400;
    }
    while (n >= 100) {
        push_back('C');
        n -= 100;
    }
    if (n >= 90) {
        append("XC");
        n -= 90;
    }
    if (n >= 50) {
        push_back('L');
        n -= 50;
    }
    if (n >= 40) {
        append("XL");
        n -= 40;
    }
    while (n >= 10) {
        push_back('X');
        n -= 10;
    }
    if (n >= 9) {
        append("IX");
        n -= 9;
    }
    if (n >= 5) {
        push_back('V');
        n -= 5;
    }
    if (n >= 4) {
        append("IV");
        n -= 4;
    }
    while (n > 0) {
        push_back('I');
        n--;
    }
}

// True is s is at ptrs.b. If so, updates ptrs.b
auto Buffer::skip_string(const std::string &s) -> bool {
    if (!substr(ptrs.b).starts_with(s)) return false;
    ptrs.b += s.size();
    return true;
}

auto Buffer::skip_word_ci(const std::string &s) -> bool {
    for (size_t i = 0; i < s.size(); i++) {
        char c = (*this)[ptrs.b + i];
        if (std::isupper(uchar(c)) != 0) c += 'a' - 'A';
        if (c != s[i]) return false;
    }
    char c = (*this)[ptrs.b + s.size()];
    if (std::isalpha(uchar(c)) != 0) return false;
    ptrs.b += s.size();
    return true;
}

// If the buffer holds a single char (plus space) returns it.
// Note: this returns 0 in case of a non-7bit character
auto Buffer::single_char() const -> char {
    auto j = next_non_space(0);
    if (at(j) == 0) return 0;
    char c = at(j);
    j      = next_non_space(j + 1);
    if ((*this)[j] != 0) return 0;
    return c;
}

// If the buffer contains a small positive number returns it.
auto Buffer::int_val() const -> std::optional<size_t> {
    size_t n = 0;
    for (size_t p = 0;; p++) {
        auto c = (*this)[p];
        if (c == 0) return n;
        if (std::isdigit(c) == 0) return {};
        n = 10 * n + to_unsigned(c - '0');
        if (n > 1000000) return {};
    }
}

// Sets ptrs.a to the first non-space
// sets ptrs.b to the next equals sign. Returns false if no such sign exists
auto Buffer::find_equals() -> bool {
    skip_sp_tab_nl();
    ptrs.a = ptrs.b;
    while ((head() != 0) && head() != '=') advance();
    return head() == '=';
}

// Ignores character at ptrs.b, and following ones.
// removes the spaces just before.
// puts a null char there.
// returns false in case of trouble (only spaces).
auto Buffer::backup_space() -> bool {
    size_t j = ptrs.b;
    while (j > ptrs.a && is_spaceh(j - 1)) j--;
    if (j == ptrs.a) return false;
    at(j) = 0; // \todo Not replaceable by reset(j) ? Because the string after ptrs.b is still there
    return true;
}

// If there is 'foobar' at ptrs.b, then puts ptrs.a to the char after
// the quote, replaces the second quote by a null.
// returns false in  case of trouble.
auto Buffer::string_delims() -> bool {
    skip_sp_tab_nl();
    char c = head();
    if (c == 0) return false;
    advance();
    ptrs.a = ptrs.b;
    while ((head() != 0) && head() != c) advance();
    if (head() == 0) return false;
    at(ptrs.b) = 0; // \todo Not replaceable by reset(j) ?
    return true;
}

// Assumes the buffer is of the form foo/bar/etc,
// with a final slash; returns the next item; Retval false if no string found

auto Buffer::slash_separated() -> std::optional<std::string> {
    std::string res;
    size_t      p = 0;
    skip_sp_tab();
    if (head() == 0) return {};
    for (;;) {
        char c = head();
        if (c == 0) return {};
        advance();
        if (c == '/') break;
        if (c == '\\') {
            if (head() == 0) return {};
            if (head() == ' ') { p = res.size() + 1; }
            c = head();
            advance();
        }
        res.push_back(c);
    }
    auto b = res.size();
    while (b > p && (std::isspace(res[b - 1]) != 0)) b--;
    res.resize(b);
    return res;
}

void Buffer::push_back_unless_punct(char c) {
    if (ends_with("&nbsp;")) return;
    if (!empty() && (std::isspace(back()) != 0)) return;
    if (!empty() && back() == '(') return;
    push_back(c);
}

// Tries to read an argument. Sets ptrs.a to after the opening brace
// ptrs.b to the closing brace.
auto Buffer::fetch_spec_arg() -> bool {
    skip_sp_tab_nl();
    if (head() != '{') return false;
    advance();
    ptrs.a = ptrs.b;
    for (;;) {
        auto c = head();
        if (c == 0 || c == '{' || c == '%') return false;
        if (c == '}') return true;
        advance();
    }
}

auto Buffer::contains_braced(const std::string &s) -> bool {
    auto k = ptrs.a;
    if (!fetch_spec_arg()) return false;
    if (substring() != s) return false;
    ptrs.a = k;
    advance();
    return true;
}

// Returns true if the buffer contains \end{env} with spaces.
// (used for detecting the end of a verbatim environment).
auto Buffer::contains_env(const std::string &env) -> bool {
    skip_sp_tab_nl();
    ptrs.a = ptrs.b;
    ptrs.b = ptrs.a + 4;
    if (substring() != "\\end") return false;
    skip_sp_tab_nl();
    if (!(contains_braced(env))) return false;
    skip_sp_tab_nl();
    return true;
}

auto Buffer::is_spaceh(size_t j) const -> bool { return std::isspace((*this)[j]) != 0; }

void Buffer::push_back(const TokenList &L) {
    for (const auto &C : L) { insert_token(C, false); }
}

void Buffer::skip_letter() {
    while (std::isalpha(head()) != 0) ptrs.b++;
}
void Buffer::skip_sp_tab() {
    while (ptrs.b < size() && (head() == ' ' || head() == '\t')) ptrs.b++;
}
void Buffer::skip_sp_tab_nl() {
    while (std::isspace(head()) != 0) ptrs.b++;
}

void Buffer::skip_letter_dig() {
    while ((std::isalpha(head()) != 0) || (std::isdigit(head()) != 0)) ptrs.b++;
}

auto Buffer::is_special_end() const -> bool { return head() == '\n' || head() == '#' || head() == '%'; }

// Returns true if there is a space. Kills at the space. Advance
auto Buffer::look_at_space(const std::string &s) -> bool {
    clear();
    append(s);
    bool has_space = false;
    ptrs.b         = 0;
    while (true) {
        if (head() == 0) break;
        if (std::isspace(head()) != 0) {
            has_space  = true;
            at(ptrs.b) = 0;
            advance();
            break;
        }
        advance();
    }
    return has_space;
}

// What follows is used for handling the configuration file.

// In the case of "foo" (no space), returns <foo>
// In the case of space, what follows the spaces is considered as
// attribute list.
auto Buffer::xml_and_attrib(const std::string &s) -> Xml {
    bool has_spaces = look_at_space(s);
    if (!has_spaces) return {s, nullptr};
    Xml res{*this, nullptr};
    push_back_special_att(res.id);
    return res;
}

// This converts the buffer in to an attribute list for id.
void Buffer::push_back_special_att(const Xid &id) {
    for (;;) {
        if (!find_equals()) return;
        auto J = ptrs.a;
        if (!backup_space()) return;
        advance();
        if (!string_delims()) return;
        std::string a = substr(J).c_str(); // NOLINT \todo this cuts at first null char, fix
        std::string b = substr(ptrs.a);
        id.add_attribute(a, b);
        advance();
    }
}

// Returns true if we see space, then s then space then equals then space.
// sets ptrs.b to the char after this space
auto Buffer::see_equals(const std::string &s) -> bool {
    ptrs.b = 0;
    skip_sp_tab();
    if (!substr(ptrs.b).starts_with(s)) return false;
    ptrs.b += s.size();
    skip_sp_tab();
    if (next_char() != '=') return false;
    skip_sp_tab();
    return true;
}

// Returns the buffer without initial and final space, if init is true.
// In any case, a tab is converted into a space, multiple space chars
// are replaced by single ones.
// We can safely assume that buffer is ASCII
auto Buffer::special_convert(bool init) -> std::string {
    ptrs.b = 0;
    if (init) skip_sp_tab_nl();
    std::string bb1;
    bool        space = true;
    for (;;) {
        auto c = next_char();
        if (c == 0) break;
        if (std::isspace(uchar(c)) != 0) {
            if (!space) {
                bb1.push_back(' ');
                space = true;
            }
        } else {
            bb1.push_back(c);
            space = false;
        }
    }
    if (init && !bb1.empty() && bb1.back() == ' ') bb1.pop_back();
    return bb1;
}

// In case of Lo{\"i}c, repeated calls will set head() to L, o, { and c.
// It works also in the case of non-ascii characters
void Buffer::next_bibtex_char() {
    auto c = head();
    if (c == 0) return;
    if (c == '\\') {
        ptrs.b++;
        c = head();
        if (c == 0) return;
        ptrs.b += how_many_bytes(c);
        if (ptrs.b > size()) ptrs.b = size();
        return;
    }
    auto n = how_many_bytes(c);
    if (n > 1) {
        ptrs.b += n;
        if (ptrs.b > size()) ptrs.b = size();
        return;
    }
    if (c != '{')
        ptrs.b++;
    else
        skip_over_brace();
}

// We assume that current char is an open brace.
// We can forget about utf8 here.
// Just want {\{\}\}\\} to be interpret correctly
void Buffer::skip_over_brace() {
    int bl = 0;
    for (;;) {
        auto c = head();
        if (c == 0) return;
        ptrs.b++;
        if (c == '\\') {
            if (head() == 0) return;
            ptrs.b++;
            continue;
        }
        if (c == '{')
            bl++;
        else if (c == '}') {
            bl--;
            if (bl == 0) return;
        }
    }
}

// removes braces in the case title="study of {$H^p}, part {I}"
// Brace followed by Uppercase or dollar
// Also, handles {\noopsort foo} removes the braces and the command.
void Buffer::special_title(std::string s) {
    int  level  = 0;
    int  blevel = 0;
    auto n      = s.size();
    for (size_t i = 0; i < n; i++) {
        char c = s[i];
        if (c == '\\') {
            push_back(c);
            i++;
            if (i < n) push_back(s[i]);
            continue;
        }
        if (c == '}') {
            if (level != blevel)
                push_back(c);
            else
                blevel = 0;
            if (level > 0) level--;
            continue;
        }
        if (c == '{') level++;
        if (c != '{' || i == n - 1 || (blevel != 0) || level != 1) {
            push_back(c);
            continue;
        }
        // c= is a brace at bottom level
        char cc = s[i + 1];
        if ((std::isupper(cc) != 0) || cc == '$') {
            blevel = level;
            continue;
        }
        if (is_noopsort(s, i)) {
            i += 10;
            while (i < n && (std::isspace(s[i]) != 0)) i++;
            i--;
            blevel = level;
        } else
            push_back(c);
    }
}

// This replaces \c{c} by \char'347 in order to avoid some errors.
void Buffer::normalise_for_bibtex(String s) {
    clear();
    push_back(' '); // make sure we can always backup one char
    for (;;) {
        auto c = *s;
        if (c == 0) {
            ptrs.b = 0;
            return;
        }
        push_back(c);
        s++;
        if (c != '\\') continue;
        if (std::string(s).starts_with("c{c}")) {
            pop_back();
            push_back(char32_t(0347U));
            s += 4;
        } else if (std::string(s).starts_with("c{C}")) {
            pop_back();
            push_back(char32_t(0307U));
            s += 4;
        } else if (std::string(s).starts_with("v{c}")) {
            pop_back();
            append("{\\v c}");
            s += 4;
            continue;
        } else if (*s == 'a' && is_accent_char(s[1])) {
            s++;
        } else if (*s == ' ') {
            pop_back();
        } // replace \space by space
    }
}

// For each character, we have its type in the table.
void Buffer::fill_table(std::vector<bchar_type> &table) {
    ptrs.b = 0;
    for (;;) {
        auto i = ptrs.b;
        auto c = head();
        if (c == 0U) {
            table[i] = bct_end;
            return;
        }
        ptrs.b++;
        if (static_cast<uchar>(c) >= 128 + 64) {
            table[i] = bct_extended;
            continue;
        }
        if (static_cast<uchar>(c) >= 128) {
            table[i] = bct_continuation;
            continue;
        }
        if (c == ' ') {
            table[i] = bct_space;
            continue;
        }
        if (c == '~') {
            table[i] = bct_tilde;
            continue;
        }
        if (c == '-') {
            table[i] = bct_dash;
            continue;
        }
        if (c == ',') {
            table[i] = bct_comma;
            continue;
        }
        if (c == '&') {
            err_in_name("unexpected character `&' (did you mean `and' ?)", to_signed(i));
            table[i] = bct_bad;
            continue;
        }
        if (c == '}') {
            err_in_name("too many closing braces", to_signed(i));
            table[i] = bct_bad;
            continue;
        }
        if (c != '\\' && c != '{') {
            table[i] = bct_normal;
            continue;
        }
        if (c == '\\') {
            c = head();
            if (!is_accent_char(c)) {
                table[i] = bct_bad;
                err_in_name("commands allowed only within braces", to_signed(i));
                continue;
            }
            if (std::isalpha(at(ptrs.b + 1)) != 0) {
                table[i]     = bct_cmd;
                table[i + 1] = bct_continuation;
                table[i + 2] = bct_continuation;
                ptrs.b += 2;
                continue;
            }
            if (at(ptrs.b + 1) != '{') {
                table[i]     = bct_bad;
                table[i + 1] = bct_bad;
                ptrs.b++;
                err_in_name("bad accent construct", to_signed(i));
                continue;
            }
            at(ptrs.b + 1) = c;
            at(ptrs.b)     = '\\';
            at(ptrs.b - 1) = '{';
            spdlog::trace("+bibchanged {}", fmt::streamed(data()));
        }
        int  bl  = 1;
        auto j   = i;
        table[i] = bct_brace;
        for (;;) {
            if (head() == 0) {
                err_in_name("this cannot happen!", to_signed(j));
                resize(j);
                table[j] = bct_end;
                return;
            }
            c             = head();
            table[ptrs.b] = bct_continuation;
            ptrs.b++;
            if (c == '{') bl++;
            if (c == '}') {
                bl--;
                if (bl == 0) break;
            }
        }
    }
}

auto Buffer::find_and(const bchar_type *table) -> bool { // \todo regexp "\s*[aA][nN][dD]\s"
    for (;;) {
        char c = head();
        if (c == 0) return true;
        ptrs.b++;
        if (table[ptrs.b - 1] != bct_space) continue;
        c = (*this)[ptrs.b];
        if (c != 'a' && c != 'A') continue;
        c = (*this)[ptrs.b + 1];
        if (c != 'n' && c != 'N') continue;
        c = (*this)[ptrs.b + 2];
        if (c != 'd' && c != 'D') continue;
        c = (*this)[ptrs.b + 3];
        if ((c == ' ') || (c == '\t') || (c == '\n')) return false;
    }
}

// In J.G. Grimm,only the first dot matches.
auto Buffer::insert_space_here(size_t k) const -> bool {
    if (k == 0) return false;
    if (at(k) != '.') return false;
    if (std::isupper(at(k + 1)) == 0) return false;
    if (std::isupper(at(k - 1)) == 0) return false;
    return true;
}

// This is the TeX command \string ; if esc is false, no escape char is inserted
void Buffer::tex_string(Token T, bool esc) {
    if (T.not_a_cmd())
        push_back(T.char_val());
    else {
        auto x = T.val;
        if (esc && x >= single_offset) insert_escape_char_raw();
        if (x >= hash_offset)
            append(hash_table[T.hash_loc()]);
        else if (x < first_multitok_val)
            push_back(T.char_val());
        else
            append(null_cs_name());
    }
}
