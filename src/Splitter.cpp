#include "tralics/Splitter.h"
#include "tralics/codepoint.h"

namespace {
    auto without_end_spaces(std::string s) -> std::string {
        size_t k = 0, l = s.size();
        while (is_space(s[k])) ++k;
        while (l > k && is_space(s[l - 1])) --l;
        return s.substr(k, l - k);
    }
} // namespace

Splitter::Splitter(const std::string &S) {
    size_t pos = 0;
    while (pos != S.size()) {
        auto p = pos;
        while (pos != S.size() && S[pos] != ',') ++pos;
        auto n = pos - p;
        if (S[pos] == ',') ++pos;
        push_back(without_end_spaces(S.substr(p, n)));
    }
}

auto Splitter::split(std::string T) -> std::pair<std::string, std::string> {
    size_t i = 0;
    while ((T[i] != 0) && (T[i] != '=')) i++;
    std::string key = without_end_spaces(std::string(T).substr(0, i));
    std::string val = (T[i] == '=') ? without_end_spaces(std::string(T).substr(i + 1)) : "";
    return {key, val};
}
