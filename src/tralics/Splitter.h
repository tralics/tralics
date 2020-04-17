#pragma once
#include "types.h"
#include <string>
#include <vector>

// This class returns foo then bar then gee from `foo,bar,gee' \todo free
// standing function returning a vector of strings would be simpler?

class Splitter : public std::vector<std::string> {
    size_t index{0};

public:
    Splitter(const std::string &S);

    [[nodiscard]] auto at_end() const -> bool { return index == size(); }
    auto               get_next() -> std::string { return at_end() ? "" : at(index++); };

    static auto split(std::string T) -> std::pair<std::string, std::string>;
};
