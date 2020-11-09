#pragma once
#include "enums.h"
#include <array>
#include <string>

// Every eqtb entry has a level. Level_zero means undefined
// Level_one is the outer level. The old value must be saved in case
// the current level is different fron the old one, and is greater than one.
// These objects are defined at 1
template <typename T> struct EQTB {
    T    val{};
    long level{};

    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > 1; }
};

using EqtbInt    = EQTB<long>;
using EqtbString = EQTB<std::string>;

inline std::array<EqtbInt, integer_table_size> eqtb_int_table;
