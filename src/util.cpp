#include "tralics/util.h"
#include "tralics/codepoint.h"
#include <cstring>

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

auto only_space_spec(const std::string &s) -> bool {
    size_t i  = 0;
    auto   ss = s.c_str();
    while (s[i] != 0) {
        if (is_space(s[i]))
            i++;
        else if ((strncmp(ss + i, "&nbsp;", 6) == 0) || (strncmp(ss + i, "&#xA0;", 6) == 0) || (strncmp(ss + i, "\302\240", 2) == 0))
            i += 6;
        else
            return false;
    }
    return true;
}
