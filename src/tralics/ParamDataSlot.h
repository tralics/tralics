#pragma once
#include <string>

// parameterised data
struct ParamDataSlot {
    std::string key, value;
    bool        is_used{false};

    [[nodiscard]] auto matches(const std::string &x) const -> bool { return is_used && x == key; }
    [[nodiscard]] auto no_topic() const -> bool { return !is_used; } // \todo deprecated?
};
