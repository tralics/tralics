#include "tralics/util.h"
#include "tralics/codepoint.h"

auto only_space(const std::string &s) -> bool {
    size_t i = 0, l = s.length();
    for (; i < l;) {
        if (is_space(s[i]))
            i++;
        else if (s[i] == static_cast<char>(194) && i + 1 < l && s[i + 1] == static_cast<char>(160))
            i += 2;
        else
            return false;
    }
    return true;
}
