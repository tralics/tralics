#pragma once
#include <string>

struct Istring : public std::string {
    Istring() = default;
    Istring(const std::string &s) : std::string(s) {} // \todo remove special values

    [[deprecated]] operator bool() const { return !empty(); }
};

inline Istring hlinee_above, hlinee_width, hlinee_below;
