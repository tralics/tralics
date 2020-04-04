#pragma once
#include "enums.h"
#include "util.h"
#include <array>
#include <string>

class Buffer;

struct Istring {
    size_t id{0};

    Istring() = default;
    explicit Istring(name_positions N);
    explicit Istring(const Buffer &X);
    explicit Istring(size_t N) : id(N) {}
    explicit Istring(const std::string &s);
    explicit Istring(String s);

    [[nodiscard]] auto null() const -> bool { return id == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return id == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return id == 2; } // ""
    [[nodiscard]] auto c_str() const -> String;
    [[nodiscard]] auto p_str() const -> String;

    auto operator==(Istring X) const -> bool { return id == X.id; }
    auto operator!=(Istring X) const -> bool { return id != X.id; }
};

extern std::array<Istring, np_last> the_names;
