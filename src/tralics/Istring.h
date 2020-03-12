#pragma once
#include "enums.h"
#include <array>
#include <string>

class Buffer;

struct Istring {
    size_t value{0};

    Istring() = default;
    explicit Istring(name_positions N);
    explicit Istring(const Buffer &X);
    explicit Istring(size_t N) : value(N) {}
    explicit Istring(const std::string &s);
    explicit Istring(String s);

    [[nodiscard]] auto null() const -> bool { return value == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return value == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return value == 2; } // ""
    [[nodiscard]] auto only_space() const -> bool;
    [[nodiscard]] auto only_space_spec() const -> bool;
    [[nodiscard]] auto c_str() const -> String;
    [[nodiscard]] auto p_str() const -> String;

    auto operator==(Istring X) const -> bool { return value == X.value; }
    auto operator!=(Istring X) const -> bool { return value != X.value; }
};

extern std::array<Istring, np_last> the_names;
