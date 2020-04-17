#include "tralics/Splitter.h"
#include "tralics/Buffer.h"

namespace {
    Buffer thebuffer;

    /// Remove initial and final spaces.
    auto without_end_spaces(std::string s) -> std::string {
        size_t k = 0, l = s.size() - 1;
        while (is_space(s[k])) ++k;
        while (l > k && is_space(s[l])) --l;
        return s.substr(k, l - k + 1);
    }
} // namespace

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
