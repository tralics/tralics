#include "tralics/Splitter.h"
#include "tralics/codepoint.h"

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
