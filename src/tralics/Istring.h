#pragma once
#include <string>

class Istring : public std::string {
    bool defined{false};

public:
    Istring() = default;
    Istring(const std::string &s) : std::string(s), defined(true) {} // \todo remove special values

    [[deprecated]] operator bool() const { return defined; }
};

inline Istring hlinee_above, hlinee_width, hlinee_below;
