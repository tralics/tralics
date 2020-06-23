#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Logger.h"

// This is how Tralics interprets a font
class FontInfo {
    long tsize{0};       // is fi_normal_size, etc
    long shape{0};       // it, sl, sc, or normal
    long family{0};      // tt, sf, or normal
    long series{0};      // bf or normal
    bool stackval{true}; // is the value on the stack ok ?
public:
    long        level{0};  // the level, as for any EQTB object
    long        old{-1};   // previous value
    long        packed{0}; // packed value of the font
    long        size;      // size, between 1 and 11 times 2048
    std::string color;     // current color
    std::string old_color; // previous color

    FontInfo() : size(6 * 2048) {}
    [[nodiscard]] auto shape_change() const -> std::string;
    [[nodiscard]] auto shape_name() const -> String;
    [[nodiscard]] auto size_change() const -> std::string;
    [[nodiscard]] auto size_name() const -> String;
    [[nodiscard]] auto family_change() const -> std::string;
    [[nodiscard]] auto family_name() const -> String;
    [[nodiscard]] auto series_change() const -> std::string;
    [[nodiscard]] auto series_name() const -> String;
    void               not_on_stack() { stackval = false; }
    void               is_on_stack() { stackval = true; }
    void               update_old() {
        old       = packed;
        old_color = color;
    }
    [[nodiscard]] auto is_ok() const -> bool { return (old & fi_data_mask) == (packed & fi_data_mask) && stackval && color == old_color; }
    void               pack() { packed = tsize + shape + family + series + size; }
    void               unpack();
    void               change_size(long c);
    void               kill() {
        shape  = 0;
        family = 0;
        series = 0;
    }
    void               see_font_change(subtypes c);
    auto               show_font() -> String;
    [[nodiscard]] auto get_size() const -> long { return size / 2048; }
    void               set_level(long k) { level = k; }
    void               set_packed(long k) { packed = k; }
    void               set_old_from_packed() { old = packed; }
    void               set_color(std::string c) { color = std::move(c); }
    void               ltfont(const std::string &s, subtypes c);
};

auto operator<<(std::ostream &fp, const FontInfo &L) -> std::ostream &;

class TeXChar {
public:
    short int width_idx;  // index in the width_table of this char
    short int height_idx; // index in the height_table of this char
    short int depth_idx;  // index in the depth_table of this char
    short int italic_idx; // index in the italics_table of this char
    short int tag;        // explains how to interpret remainder
    short int remainder;  // the remainder field
};

struct TexFont : std::vector<ScaledInt> {
    std::string name;
    long        at_val{0};
    long        scaled_val{0};
    long        hyphen_char{0};
    long        skew_char{0};
    int         smallest_char{0};
    int         largest_char{0};
    int         width_len{0};
    int         height_len{0};
    int         depth_len{0};
    int         italic_len{0};
    int         ligkern_len{0};
    int         kern_len{0};
    int         exten_len{0};
    TeXChar *   char_table{nullptr};
    int *       width_table{nullptr};
    int *       height_table{nullptr};
    int *       depth_table{nullptr};
    int *       italic_table{nullptr};
    int *       ligkern_table{nullptr};
    int *       kern_table{nullptr};
    int *       exten_table{nullptr};

    /// In TeX, only one of at_value and scaled can be given. Unused in Tralics.
    TexFont(std::string n, long a, long s) : name(std::move(n)), at_val(a), scaled_val(s){};

    [[nodiscard]] auto operator==(const TexFont &o) const { return name == o.name && at_val == o.at_val && scaled_val == o.scaled_val; };
};

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
