#pragma once
#include "../txvars.h"
#include <string>

class Istring {
    int value{0};

public:
    Istring() = default;
    Istring(name_positions N);
    Istring(const Buffer &X);
    explicit Istring(int N) : value(N) {}
    explicit Istring(const std::string &s);
    explicit Istring(String s);

    [[nodiscard]] auto null() const -> bool { return value == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return value == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return value == 2; } // ""
    [[nodiscard]] auto only_space() const -> bool;
    [[nodiscard]] auto starts_with_div() const -> uchar;
    [[nodiscard]] auto only_space_spec() const -> bool;
    auto               operator==(Istring X) const -> bool { return value == X.value; }
    auto               operator!=(Istring X) const -> bool { return value != X.value; }
    [[nodiscard]] auto get_value() const -> int { return value; }
    [[nodiscard]] auto c_str() const -> String;
    [[nodiscard]] auto p_str() const -> String;
};
