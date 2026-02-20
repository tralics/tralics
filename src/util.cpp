#include "tralics/util.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <ctre.hpp>
#include <cstdio>
#include <spdlog/spdlog.h>
#include <sstream>
#include <utf8.h>

namespace {
    auto counter_val(int k) -> int { return (0 <= k && k < to_signed(nb_registers)) ? k : -1; }

    auto split_at_char(const std::string &s, char sep) -> std::optional<std::pair<std::string, std::string>> {
        if (auto pos = s.find(sep); pos != std::string::npos) return {{s.substr(0, pos), s.substr(pos + 1)}};
        return {};
    }

    auto shell_quote(std::string_view s) -> std::string {
        std::string out;
        out.reserve(s.size() + 8);
        out.push_back('\'');
        for (char c : s) {
            if (c == '\'')
                out += "'\\''";
            else
                out.push_back(c);
        }
        out.push_back('\'');
        return out;
    }

    auto trim_eol(std::string s) -> std::string {
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
        return s;
    }
} // namespace

auto find_in_kpathsea(const std::string &name) -> std::optional<std::filesystem::path> {
    std::string cmd = "kpsewhich -- " + shell_quote(name) + " 2>/dev/null";
    FILE       *fp  = popen(cmd.c_str(), "r");
    if (fp == nullptr) return {};
    char              buffer[1024];
    std::string       out;
    while (fgets(buffer, sizeof(buffer), fp) != nullptr) out += buffer;
    [[maybe_unused]] int status = pclose(fp);
    out                         = trim_eol(out);
    if (out.empty()) return {};
    auto path = std::filesystem::path(out);
    if (!std::filesystem::exists(path)) return {};
    return path;
}

auto only_space(const std::string &s) -> bool {
    for (size_t i = 0; i < s.length(); ++i) {
        if (std::isspace(uchar(s[i])) != 0) continue;
        if (s[i] == static_cast<char>(0xC2) && s[i + 1] == static_cast<char>(0xA0)) // UTF-8 nbsp
            i += 1;
        else
            return false;
    }
    return true;
}

auto is_valid_xml_name(std::string_view s) -> bool {
    if (s.empty()) return false;
    auto is_start = [](unsigned char c) {
        return std::isalpha(c) != 0 || c == '_' || c == ':' || c >= 0x80;
    };
    auto is_name_char = [&](unsigned char c) {
        return is_start(c) || std::isdigit(c) != 0 || c == '-' || c == '.';
    };
    if (!is_start(static_cast<unsigned char>(s.front()))) return false;
    for (size_t i = 1; i < s.size(); ++i)
        if (!is_name_char(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

auto without_end_spaces(std::string s) -> std::string {
    size_t k = 0, l = s.size();
    while (std::isspace(s[k]) != 0) ++k;
    while (l > k && (std::isspace(s[l - 1]) != 0)) --l;
    return s.substr(k, l - k);
}

void append_unless_punct(std::string &s, char c) {
    if (s.size() >= 6 && s.ends_with("&nbsp;")) return;
    if (!s.empty() && (std::isspace(static_cast<unsigned char>(s.back())) != 0)) return;
    if (!s.empty() && s.back() == '(') return;
    s.push_back(c);
}

void append_roman_upper(std::string &s, long n) {
    while (n >= 1000) {
        s.push_back('M');
        n -= 1000;
    }
    if (n <= 0) return;
    if (n >= 900) {
        s.append("CM");
        n -= 900;
    }
    if (n >= 500) {
        s.push_back('D');
        n -= 500;
    }
    if (n >= 400) {
        s.append("CD");
        n -= 400;
    }
    while (n >= 100) {
        s.push_back('C');
        n -= 100;
    }
    if (n >= 90) {
        s.append("XC");
        n -= 90;
    }
    if (n >= 50) {
        s.push_back('L');
        n -= 50;
    }
    if (n >= 40) {
        s.append("XL");
        n -= 40;
    }
    while (n >= 10) {
        s.push_back('X');
        n -= 10;
    }
    if (n >= 9) {
        s.append("IX");
        n -= 9;
    }
    if (n >= 5) {
        s.push_back('V');
        n -= 5;
    }
    if (n >= 4) {
        s.append("IV");
        n -= 4;
    }
    while (n > 0) {
        s.push_back('I');
        n--;
    }
}

void append_roman_lower(std::string &s, long n) {
    auto k = s.size();
    append_roman_upper(s, n);
    for (size_t i = k; i < s.size(); i++) s[i] = char(s[i] + 'a' - 'A');
}

auto special_title(std::string_view s) -> std::string {
    std::string out;
    int         level  = 0;
    int         blevel = 0;
    auto        n      = s.size();
    for (size_t i = 0; i < n; i++) {
        char c = s[i];
        if (c == '\\') {
            out.push_back(c);
            i++;
            if (i < n) out.push_back(s[i]);
            continue;
        }
        if (c == '}') {
            if (level != blevel)
                out.push_back(c);
            else
                blevel = 0;
            if (level > 0) level--;
            continue;
        }
        if (c == '{') level++;
        if (c != '{' || i == n - 1 || (blevel != 0) || level != 1) {
            out.push_back(c);
            continue;
        }
        char cc = s[i + 1];
        if ((std::isupper(static_cast<unsigned char>(cc)) != 0) || cc == '$') {
            blevel = level;
            continue;
        }
        if (is_noopsort(s, i)) {
            i += 10;
            while (i < n && (std::isspace(static_cast<unsigned char>(s[i])) != 0)) i++;
            i--;
            blevel = level;
        } else
            out.push_back(c);
    }
    return out;
}

auto is_noopsort(std::string_view s, size_t i) -> bool {
    if (i + 1 >= s.size()) return false;
    static constexpr auto pattern = ctll::fixed_string{R"(^\\(?:noopsort|[sS]ort[nN]oop)(?:\{|[ \t\r\n]))"};
    return ctre::match<pattern>(s.substr(i + 1));
}

auto four_hats(char32_t ch) -> std::string {
    std::string out;
    if (ch == '\n') {
        out.push_back('\n');
        return out;
    }
    if (ch == '\r') {
        out.push_back('\r');
        return out;
    }
    if (ch == '\t') {
        out.push_back('\t');
        return out;
    }
    unsigned c = ch;
    if (ch < 32) {
        out.append("^^");
        out.push_back(static_cast<char>(c + 64));
    } else if (ch == 127)
        out.append("^^?");
    else if (ch < 128)
        out.push_back(static_cast<char>(ch));
    else {
        auto s = fmt::format("{:x}", c);
        for (size_t i = 0; i < s.size(); ++i) out.push_back('^');
        if (s.size() == 3) out.append("^0");
        out.append(s);
    }
    return out;
}

auto split_commas(const std::string &S) -> std::vector<std::string> {
    std::vector<std::string> res;
    size_t                   pos = 0;
    while (pos != S.size()) {
        auto p = pos;
        while (pos != S.size() && S[pos] != ',') ++pos;
        auto n = pos - p;
        if (S[pos] == ',') ++pos;
        res.push_back(without_end_spaces(S.substr(p, n)));
    }
    return res;
}

auto split_assign(std::string s) -> std::pair<std::string, std::string> {
    auto out = split_at_char(s, '=');
    if (!out) return {without_end_spaces(std::move(s)), ""};
    return {without_end_spaces(out->first), without_end_spaces(out->second)};
}

// Hack. Returns -1 in case no counter was given
// counter=12345 was given, this is out_of_range,  counter=foo is given
// but \c@foo is not a counter.
// Otherwise, returns the register number of the counter
auto find_counter(const std::string &s) -> int {
    if (s.empty()) return -1;
    if (only_digits(s)) return counter_val(std::stoi(s));
    static Buffer B;
    B        = "c@" + s;
    Token t  = hash_table.locate(B);
    auto  cs = t.eqtb_loc();
    if (Hashtab::the_eqtb()[cs].val.cmd != assign_int_cmd) return -1;
    return counter_val(int(Hashtab::the_eqtb()[cs].val.chr) - int(count_reg_offset));
}

auto only_digits(const std::string &s) -> bool {
    return std::all_of(s.begin(), s.end(), [](char v) { return std::isdigit(v); });
}

auto bt_to_string(boundary_type v) -> std::optional<std::string> {
    switch (v) {
    case bt_brace: return "brace";
    case bt_cell: return "cell";
    case bt_local: return "local";
    case bt_semisimple: return "\\begingroup";
    case bt_esemisimple: return "\\endgroup";
    case bt_env: return "environment";
    case bt_tpa: return "titlepage argument";
    case bt_math: return "math";
    default: return std::nullopt;
    }
}

auto save_string_name(size_t n) -> std::string_view {
    if (n == 0) return "current label";
    if (n == 1) return "current counter";
    return "unknown";
}

auto is_m_font(symcodes cmd) -> bool { return cmd == math_font_cmd || cmd == oldfont_cmd || cmd == argfont_cmd || cmd == noargfont_cmd; }

auto to_utf8(char32_t c) -> std::string {
    std::ostringstream o;
    utf8::append(c, std::ostream_iterator<char>(o));
    return o.str();
}

auto remove_digits(std::string s) -> std::string {
    while ((!s.empty()) && (std::isdigit(s.back()) != 0)) s.pop_back();
    return s;
}

auto single_non_space_char(std::string_view s) -> char {
    size_t j = 0;
    while (j < s.size() && std::isspace(static_cast<uchar>(s[j])) != 0) j++;
    if (j >= s.size()) return 0;
    char c = s[j++];
    while (j < s.size() && std::isspace(static_cast<uchar>(s[j])) != 0) j++;
    return (j == s.size()) ? c : 0;
}

auto insert_space_here(std::string_view s, size_t k) -> bool {
    if (k == 0 || k + 1 >= s.size()) return false;
    if (s[k] != '.') return false;
    if (std::isupper(static_cast<uchar>(s[k + 1])) == 0) return false;
    if (std::isupper(static_cast<uchar>(s[k - 1])) == 0) return false;
    return true;
}

auto special_convert_ascii_space(std::string_view s, bool trim_edges) -> std::string {
    size_t i = 0;
    if (trim_edges) {
        while (i < s.size() && std::isspace(static_cast<uchar>(s[i])) != 0) i++;
    }
    std::string out;
    bool        space = true;
    for (; i < s.size(); ++i) {
        auto c = s[i];
        if (std::isspace(static_cast<uchar>(c)) != 0) {
            if (!space) {
                out.push_back(' ');
                space = true;
            }
        } else {
            out.push_back(c);
            space = false;
        }
    }
    if (trim_edges && !out.empty() && out.back() == ' ') out.pop_back();
    return out;
}

auto codepoints(const std::string &s) -> std::vector<char32_t> {
    the_parser.cur_file_line = the_parser.get_cur_line();
    std::vector<char32_t> res;
    for (auto it = s.begin(); it != s.end() && *it != 0;) res.emplace_back(utf8::next(it, s.end()));
    return res;
}

auto convert_to_utf8(const std::string &s, size_t wc) -> std::string {
    if (wc == 0) return s; // Noop if utf8-encoded, but we never call the function in that case
    if (wc >= 2) {
        static bool warned_custom_input_encoding = false;
        if (!warned_custom_input_encoding) {
            warned_custom_input_encoding = true;
            spdlog::warn("Input encoding tables >1 are deprecated; prefer UTF-8 or ISO-8859-1 source files.");
        }
    }
    std::string res;
    for (auto ch : s) {
        auto C = static_cast<uchar>(ch);
        auto c = wc == 1 ? char32_t(C) : the_main.custom_table[wc - 2][C];
        if (c != 0) utf8::append(c, std::back_inserter(res));
    }
    return res;
}

// splits foo:bar into foo and bar
auto split_at_colon(const std::string &s) -> std::optional<std::pair<std::string, std::string>> {
    return split_at_char(s, ':');
}

// returns true if only ascii 7 bits in the buffer
auto is_all_ascii(const std::string &s) -> bool {
    auto wrong = [](char c) { return static_cast<uchar>(c) >= 128 || (c < 32 && c != '\t' && c != '\n'); };
    return std::none_of(s.begin(), s.end(), wrong);
}
