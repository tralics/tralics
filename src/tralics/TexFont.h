#pragma once
#include "ScaledInt.h"
#include <string>
#include <vector>

struct TexFont : std::vector<ScaledInt> {
    std::string name;
    long        at_val;
    long        scaled_val;
    long        hyphen_char{0};
    long        skew_char{0};

    /// In TeX, only one of at_value and scaled can be given. Unused in Tralics.
    TexFont(std::string n, long a, long s) : name(std::move(n)), at_val(a), scaled_val(s) {};

    [[nodiscard]] auto operator==(const TexFont &o) const { return name == o.name && at_val == o.at_val && scaled_val == o.scaled_val; };
};
