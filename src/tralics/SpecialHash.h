#pragma once
#include "types.h"
#include <string>
#include <vector>

// This is an association table. We could use a standard C++ class here
class SpecialHash {
    std::vector<std::pair<std::string, std::string>> kv;

public:
    SpecialHash(const std::string &S);

    [[nodiscard]] auto find(const std::string &x) const -> std::string;
};
