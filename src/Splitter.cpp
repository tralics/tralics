#include "tralics/Splitter.h"
#include "tralics/Buffer.h"
#include <sstream>

namespace {
    Buffer buf;

    auto without_end_spaces(std::string s) -> std::string {
        size_t k = 0, l = s.size();
        while (is_space(s[k])) ++k;
        while (l > k && is_space(s[l - 1])) --l;
        return s.substr(k, l - k);
    }
} // namespace

// This returns the number of keywords in the list.
auto Splitter::count() const -> size_t {
    return to_unsigned(std::count_if(S.begin(), S.end(), [](char c) { return c == ','; })) + 1;
}

// This returns the next keyword
auto Splitter::get_next_raw() -> std::string {
    auto p = pos;
    while (pos < size && S[pos] != ',') pos++;
    auto n = pos - p;
    if (pos < size && S[pos] == ',') pos++;
    if (n == 0) return "";
    buf.reset();
    buf.push_back(S.substr(p, n));
    return buf.c_str();
}

auto Splitter::get_next() -> std::string { return without_end_spaces(get_next_raw()); }

// Constructs the next pair. We first call next_for_splitter,
// Assume that this puts Key=Val in the buffer. We set key and val.
// If no equals sign is given, the string is the key, the value is empty.
auto Splitter::extract_keyval() -> std::pair<std::string, std::string> {
    auto   T = get_next_raw();
    size_t i = 0;
    while ((T[i] != 0) && (T[i] != '=')) i++;
    std::string key = without_end_spaces(std::string(T).substr(0, i));
    std::string val = (T[i] == '=') ? without_end_spaces(std::string(T).substr(i + 1)) : "";
    return {key, val};
}
