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

[[nodiscard]] inline auto cur_centering() -> size_t { return to_unsigned(eqtb_int_table[incentering_code].val); }
[[nodiscard]] inline auto cur_lang_fr() -> bool { return eqtb_int_table[language_code].val == 1; }
[[nodiscard]] inline auto cur_lang_german() -> bool { return eqtb_int_table[language_code].val == 2; }
[[nodiscard]] inline auto get_catcode(size_t x) -> symcodes { return symcodes(eqtb_int_table[x].val); }
[[nodiscard]] inline auto is_pos_par(size_t k) { return eqtb_int_table[k].val > 0; }
[[nodiscard]] inline auto tracing_assigns() -> bool { return is_pos_par(tracingassigns_code); }
[[nodiscard]] inline auto tracing_io() -> bool { return is_pos_par(tracingoutput_code); }
[[nodiscard]] inline auto tracing_macros() -> bool { return is_pos_par(tracingmacros_code); }
[[nodiscard]] inline auto tracing_math() -> bool { return is_pos_par(tracingmath_code); }
[[nodiscard]] inline auto tracing_stack() -> bool { return is_pos_par(tracingrestores_code); }
[[nodiscard]] inline auto tracing_commands() -> bool { return is_pos_par(tracingcommands_code); }

inline void set_catcode(size_t x, long v) { eqtb_int_table[x].val = v; }
