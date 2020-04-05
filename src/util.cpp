#include "tralics/util.h"
#include "tralics/codepoint.h"

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
