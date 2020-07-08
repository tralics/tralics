#pragma once
#include "TexFont.h"

struct TexFonts : public std::vector<TexFont> {
    TexFonts() { emplace_back("nullfont", 0, 0); }

    auto is_valid(long k) -> bool;
    auto name(long k) -> std::string;
    void full_name(Buffer &B, long k);
    auto get_int_param(long ft, int pos) -> long;
    auto get_dimen_param(long ft, long pos) -> ScaledInt;
    void set_int_param(long ft, int pos, long v);
    void set_dimen_param(long ft, long p, ScaledInt v);
    auto find_font(const std::string &n, long a, long s) -> size_t;
    auto define_a_new_font(const std::string &n, long a, long s) -> size_t;
};