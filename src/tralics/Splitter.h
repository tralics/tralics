#pragma once
#include "types.h"
#include <string>

// This class returns foo then bar then gee from `foo,bar,gee' \todo free
// standing function returning a vector of strings would be simpler?

class Splitter {
    std::string S;   // the string to split
    size_t      pos; // current position

public:
    Splitter(std::string w) : S(std::move(w)), pos(0) {}
    [[nodiscard]] auto at_end() const -> bool { return pos == S.size(); }
    auto               get_next() -> std::string;

    static auto split(std::string s) -> std::pair<std::string, std::string>;
};
