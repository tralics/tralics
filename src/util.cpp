#include "tralics/util.h"
#include "tralics/Parser.h"
#include <sstream>
#include <utf8.h>

namespace {
    auto counter_val(int k) -> int { return (0 <= k && k < to_signed(nb_registers)) ? k : -1; }
} // namespace

auto only_space(const std::string &s) -> bool {
    for (size_t i = 0; i < s.length(); ++i) {
        if (is_space(s[i])) continue;
        if (s[i] == static_cast<char>(0xC2) && s[i + 1] == static_cast<char>(0xA0)) // UTF-8 nbsp
            i += 1;
        else
            return false;
    }
    return true;
}

auto without_end_spaces(std::string s) -> std::string {
    size_t k = 0, l = s.size();
    while (is_space(s[k])) ++k;
    while (l > k && is_space(s[l - 1])) --l;
    return s.substr(k, l - k);
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
    size_t i = 0;
    while ((s[i] != 0) && (s[i] != '=')) i++;
    std::string key = without_end_spaces(std::string(s).substr(0, i));
    std::string val = (s[i] == '=') ? without_end_spaces(std::string(s).substr(i + 1)) : "";
    return {key, val};
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
    Token t  = the_parser.hash_table.locate(B);
    auto  cs = t.eqtb_loc();
    if (the_parser.hash_table.eqtb[cs].val.cmd != assign_int_cmd) return -1;
    return counter_val(the_parser.hash_table.eqtb[cs].val.chr - count_reg_offset);
}

auto only_digits(const std::string &s) -> bool {
    return std::all_of(s.begin(), s.end(), [](char v) { return is_digit(v); });
}

auto bt_to_string(boundary_type v) -> std::string { // \todo std::optional<std::string>
    switch (v) {
    case bt_brace: return "brace";
    case bt_cell: return "cell";
    case bt_local: return "local";
    case bt_semisimple: return "\\begingroup";
    case bt_esemisimple: return "\\endgroup";
    case bt_env: return "environment";
    case bt_tpa: return "titlepage argument";
    case bt_math: return "math";
    default: return "impossible";
    }
}

auto save_string_name(size_t n) -> String {
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
