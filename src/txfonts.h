#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txstring.h"

// This is how Tralics interprets a font
class FontInfo {
    long    size;              // size, between 1 and 11 times 2048
    int     tsize{0};          // is fi_normal_size, etc
    int     shape{0};          // it, sl, sc, or normal
    int     family{0};         // tt, sf, or normal
    int     series{0};         // bf or normal
    long    packed{0};         // packed value of the font
    int     level{level_zero}; // the level, as for any EQTB object
    long    old{-1};           // previous value
    Istring color;             // current color
    Istring old_color;         // previous color
    bool    stackval{true};    // is the value on the stack ok ?
public:
    FontInfo() : size(6 * 2048) {}
    [[nodiscard]] auto shape_change() const -> name_positions;
    [[nodiscard]] auto shape_name() const -> String;
    [[nodiscard]] auto size_change() const -> name_positions;
    [[nodiscard]] auto size_name() const -> String;
    [[nodiscard]] auto family_change() const -> name_positions;
    [[nodiscard]] auto family_name() const -> String;
    [[nodiscard]] auto series_change() const -> name_positions;
    [[nodiscard]] auto series_name() const -> String;
    void               not_on_stack() { stackval = false; }
    void               is_on_stack() { stackval = true; }
    void               update_old() {
        old       = packed;
        old_color = color;
    }
    auto is_ok() -> bool { return (old & fi_data_mask) == (packed & fi_data_mask) && stackval && color == old_color; }
    void pack() { packed = tsize + shape + family + series + size; }
    void unpack();
    void change_size(long c);
    void kill() {
        shape  = 0;
        family = 0;
        series = 0;
    }
    void               see_font_change(subtypes c);
    auto               show_font() -> String;
    auto               get_level() -> int { return level; }
    auto               get_size() -> long { return size >> 11; }
    void               set_level(int k) { level = k; }
    void               set_packed(int k) { packed = k; }
    auto               get_old() -> long { return old; }
    auto               get_old_color() -> Istring { return old_color; }
    auto               get_packed() -> long { return packed; }
    void               set_old_from_packed() { old = packed; }
    void               set_color(Istring c) { color = c; }
    [[nodiscard]] auto get_color() const -> Istring { return color; }
    void               ltfont(const std::string &s, subtypes c);
};

class TeXChar {
public:
    short int width_idx;  // index in the width_table of this char
    short int height_idx; // index in the height_table of this char
    short int depth_idx;  // index in the depth_table of this char
    short int italic_idx; // index in the italics_table of this char
    short int tag;        // explains how to interpret remainder
    short int remainder;  // the remainder field
};

class TexFont {
public:
    int         smallest_char;
    int         largest_char;
    int         width_len;
    int         height_len;
    int         depth_len;
    int         italic_len;
    int         ligkern_len;
    int         kern_len;
    int         exten_len;
    int         param_len;
    TeXChar *   char_table;
    int *       width_table;
    int *       height_table;
    int *       depth_table;
    int *       italic_table;
    int *       ligkern_table;
    int *       kern_table;
    int *       exten_table;
    ScaledInt * param_table;
    int         hyphen_char;
    int         skew_char;
    std::string name;
    int         scaled_val;
    int         at_val;

    TexFont(const std::string &n, int a, int s);

    void               realloc_param(int p);
    [[nodiscard]] auto its_me(const std::string &n, int a, int s) const -> bool;
    void               make_null();
    void               load();
};

struct TexFonts : public std::vector<TexFont> {
    TexFonts() { emplace_back("nullfont", 0, 0); }

    auto is_valid(int k) -> bool;
    auto name(int k) -> std::string;
    void full_name(Buffer &B, int k);
    auto get_int_param(int ft, int pos) -> int;
    auto get_dimen_param(int ft, long pos) -> ScaledInt;
    void set_int_param(int ft, int pos, int v);
    void set_dimen_param(int ft, int p, ScaledInt v);
    auto find_font(const std::string &n, int a, int s) -> size_t;
    auto define_a_new_font(const std::string &n, int a, int s) -> size_t;
};
