#pragma once
#include "types.h"
#include <string>
#include <vector>

// This is an association table. We could use a standard C++ class here
class SpecialHash {
    std::vector<std::string> key;   // the keys
    std::vector<std::string> value; // the values
public:
    size_t size{0}; // number of terms

    SpecialHash(const std::string &S);

    auto               find(String x) const -> std::string;
    [[nodiscard]] auto find_counter() const -> int;
};
