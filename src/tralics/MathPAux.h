#pragma once
#include "enums.h"
#include <ostream>

struct MathPAux {
    int        pos;  // position of the item
    math_types type; // type of the item

    [[nodiscard]] auto is_small() const -> bool { return type == mt_flag_small_l || type == mt_flag_small_r || type == mt_flag_small_m; }
};

auto operator<<(std::ostream &fp, const MathPAux &x) -> std::ostream &;
