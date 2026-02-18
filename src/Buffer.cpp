// This file contains a lot of stuff dealing with buffers.

#include "tralics/Bibtex.h"
#include "tralics/Glue.h"
#include "tralics/Parser.h"
#include "tralics/Xml.h"
#include "tralics/util.h"
#include <cctype>
#include <ctre.hpp>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <utf8.h>

namespace {
    // This returns the number of bytes in a UTF8 character
    // given the first byte. Returns 0 in case of error
    auto how_many_bytes(char c) -> size_t { return to_unsigned(utf8::internal::sequence_length(&c)); }

    // True if it is an accent character, like \^.
    auto is_accent_char(char c) -> bool { return c == '\'' || c == '`' || c == '^' || c == '"' || c == '~' || c == '.' || c == '='; }

    // Returns the current escape char (used for printing)
    auto current_escape_char() -> long { return eqtb_int_table[escapechar_code].val; }

    // Returns a temporary string, corresponding to the command with
    // an empty name, without initial escape char.
    auto null_cs_name() -> std::string {
        auto c = current_escape_char();
        if (c == '\\') return "csname\\endcsname";
        if (c > 0 && c < int(nb_characters)) {
            Buffer B = "csname";
            B.out_log(char32_t(c));
            B.append("endcsname");
            return std::move(B);
        }
        if (c == 0) return "csname^^@endcsname";
        return "csnameendcsname";
    }

    void err_in_name(std::string_view a, long i) {
        Bibtex::err_in_entry(a);
        spdlog::error("bad syntax in author or editor name.");
        spdlog::error("error occurred at character position {} in the string: {}.", i, fmt::streamed(name_buffer));
    }
} // namespace

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
        out_log(char32_t(char32_t(c)));
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
            out_log(c);
            out_log(c);
        } else
            out_log(c);
        return false;
    }
    if (!T.char_or_active()) insert_escape_char();
    if (T.active_or_single()) {
        out_log(T.char_val());
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
    constexpr std::array<std::string_view, 6> gptable{"pt", "fil", "fill", "filll", "", "mu"};
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

// Sets ptrs.a to the first non-space
// sets ptrs.b to the next equals sign. Returns false if no such sign exists
auto Buffer::find_equals() -> bool {
    skip_sp_tab_nl();
    ptrs.a = ptrs.b;
    while (ptrs.b < size() && head() != '=') advance();
    return ptrs.b < size() && head() == '=';
}

// If there is 'foobar' at ptrs.b, then puts ptrs.a to the char after
// the quote and ptrs.b on the closing quote.
// returns false in case of trouble.
auto Buffer::string_delims() -> bool {
    skip_sp_tab_nl();
    char c = head();
    if (c == 0) return false;
    advance();
    ptrs.a = ptrs.b;
    while (ptrs.b < size() && (head() != 0) && head() != c) advance();
    if (head() == 0 || head() != c) return false;
    return true;
}

// Assumes the buffer is of the form foo/bar/etc,
// with a final slash; returns the next item; Retval false if no string found

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
    if (substr(ptrs.a, ptrs.b - ptrs.a) != s) return false;
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
    if (substr(ptrs.a, ptrs.b - ptrs.a) != "\\end") return false;
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
    while (ptrs.b < size() && (std::isspace(uchar(head())) != 0)) ptrs.b++;
}

void Buffer::skip_letter_dig() {
    while ((std::isalpha(head()) != 0) || (std::isdigit(head()) != 0)) ptrs.b++;
}

auto Buffer::is_special_end() const -> bool { return head() == '\n' || head() == '#' || head() == '%'; }

// Returns true if there is a space. Leaves ptrs.b on the first space.
auto Buffer::look_at_space(const std::string &s) -> bool {
    clear();
    append(s);
    ptrs.a = 0;
    ptrs.b = 0;
    while (ptrs.b < size()) {
        if (head() == 0) break;
        if (std::isspace(uchar(head())) != 0) return true;
        advance();
    }
    return false;
}

// What follows is used for handling the configuration file.

// In the case of "foo" (no space), returns <foo>
// In the case of space, what follows the spaces is considered as
// attribute list.
auto Buffer::xml_and_attrib(const std::string &s) -> Xml * {
    bool has_spaces = look_at_space(s);
    if (!has_spaces) return new Xml(s, nullptr);
    auto *res = new Xml(substr(0, ptrs.b), nullptr);
    push_back_special_att(*res);
    return res;
}

// This converts the buffer in to an attribute list for x.
void Buffer::push_back_special_att(Xml &x) {
    for (;;) {
        if (!find_equals()) return;
        auto name_start = ptrs.a;
        auto name_end   = ptrs.b;
        while (name_end > name_start && is_spaceh(name_end - 1)) name_end--;
        if (name_end == name_start) return;
        advance();
        if (!string_delims()) return;
        std::string a = substr(name_start, name_end - name_start);
        std::string b = substr(ptrs.a, ptrs.b - ptrs.a);
        x.add_att(a, b);
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

// This replaces \c{c} by \char'347 in order to avoid some errors.
void Buffer::normalise_for_bibtex(std::string_view s) {
    clear();
    push_back(' '); // make sure we can always backup one char
    for (size_t i = 0; i < s.size(); ++i) {
        auto c = s[i];
        push_back(c);
        if (c != '\\') continue;
        auto rest = s.substr(i + 1);
        if (rest.starts_with("c{c}")) {
            pop_back();
            push_back(char32_t(0347U));
            i += 4;
        } else if (rest.starts_with("c{C}")) {
            pop_back();
            push_back(char32_t(0307U));
            i += 4;
        } else if (rest.starts_with("v{c}")) {
            pop_back();
            append("{\\v c}");
            i += 4;
            continue;
        } else if (rest.size() >= 2 && rest[0] == 'a' && is_accent_char(rest[1])) {
            i += 1;
        } else if (!rest.empty() && rest[0] == ' ') {
            pop_back();
        } // replace \space by space
    }
    ptrs.b = 0;
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

auto Buffer::find_and(const bchar_type *table) -> bool { // TODO: regexp "\s*[aA][nN][dD]\s"
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
