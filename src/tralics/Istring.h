#pragma once
#include "enums.h"
#include <array>
#include <string>

class ScaledInt;
class LabelInfo;

struct Istring {
    std::string name;
    size_t      id{0};

    Istring() = default;
    explicit Istring(size_t N);
    explicit Istring(const std::string &s);
    explicit Istring(const ScaledInt &i);

    [[nodiscard]] auto null() const -> bool { return id == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return id == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return id == 2; } // "" \todo investigate because 2 is `" "`
    [[nodiscard]] auto value() const -> std::string;
    [[nodiscard]] auto labinfo() const -> LabelInfo *;

    auto operator==(const Istring &X) const -> bool { return id == X.id; }
    auto operator!=(const Istring &X) const -> bool { return id != X.id; }
};

auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream &;

inline std::array<Istring, np_last> the_names; // \todo static in Istring and make is a map from std::string
