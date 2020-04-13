// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2003-2011
// Functions on fonts for Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txinline.h"
#include "txparser.h"
#include <fmt/format.h>

// Font info stuff.

// This returns the position of the name of a font attribute
// for use as an XML attribute.
// This function deals with the size.
auto FontInfo::size_change() const -> name_positions {
    if (tsize == fi_small_size) return np_font_small;
    if (tsize == fi_big_size) return np_font_large;
    if (tsize == 0) return cst_empty;
    if (tsize == fi_small_size1) return np_font_small1;
    if (tsize == fi_small_size2) return np_font_small2;
    if (tsize == fi_small_size3) return np_font_small3;
    if (tsize == fi_small_size4) return np_font_small4;
    if (tsize == fi_small_size5) return np_font_small5;
    if (tsize == fi_small_size6) return np_font_small6;
    if (tsize == fi_big_size1) return np_font_large1;
    if (tsize == fi_big_size2) return np_font_large2;
    if (tsize == fi_big_size3) return np_font_large3;
    if (tsize == fi_big_size4) return np_font_large4;
    if (tsize == fi_big_size5) return np_font_large5;
    if (tsize == fi_big_size6) return np_font_large6;
    return cst_empty;
    // maybe np_font_normalsize
}

// Idem. This function deals with the shape.
auto FontInfo::shape_change() const -> name_positions {
    if (shape == fi_it_shape) return np_font_it;
    if (shape == fi_sl_shape) return np_font_slanted;
    if (shape == fi_sc_shape) return np_font_sc;
    return cst_empty;
    // could be np_font_upright
}

// Idem. This function deals with the family.
auto FontInfo::family_change() const -> name_positions {
    if (family == fi_sf_family) return np_font_sansserif;
    if (family == fi_tt_family) return np_font_tt;
    return cst_empty;
    // could be np_font_roman
}

// Idem. This function deals with the series.
auto FontInfo::series_change() const -> name_positions {
    if (series == fi_bf_series) return np_font_bold;
    if (series == fi_bx_series) return np_font_boldextended;
    if (series == fi_sb_series) return np_font_semibold;
    if (series == fi_c_series) return np_font_condensed;
    return cst_empty; // could be np_font_medium
}

// This function returns a font attribute, as a LaTeX command.

auto FontInfo::size_name() const -> String {
    if (tsize == fi_small_size) return "\\small";
    if (tsize == fi_small_size1) return "\\small";
    if (tsize == fi_big_size) return "\\large";
    if (tsize == fi_big_size1) return "\\large";
    if (tsize == fi_small_size2) return "\\footnotesize";
    if (tsize == fi_small_size3) return "\\scriptsize";
    if (tsize == fi_small_size4) return "\\tiny";
    if (tsize == fi_small_size5) return "\\Tiny";
    if (tsize == fi_small_size6) return "\\TINY";
    if (tsize == fi_big_size2) return "\\Large";
    if (tsize == fi_big_size3) return "\\LARGE";
    if (tsize == fi_big_size4) return "\\huge";
    if (tsize == fi_big_size5) return "\\Huge";
    if (tsize == fi_big_size6) return "\\HUGE";
    return nullptr;
}

// Idem. This function deals with the shape.
auto FontInfo::shape_name() const -> String {
    if (shape == fi_it_shape) return "\\itshape";
    if (shape == fi_sl_shape) return "\\slshape";
    if (shape == fi_sc_shape) return "\\scshape";
    return nullptr;
}

// Idem. This function deals with the family.
auto FontInfo::family_name() const -> String {
    if (family == fi_sf_family) return "\\sffamily";
    if (family == fi_tt_family) return "\\ttfamily";
    return nullptr;
}

// Idem. This function deals with the series. (the function is inline).
auto FontInfo::series_name() const -> String {
    if (series == fi_bf_series) return "\\bfseries";
    if (series == fi_bx_series) return "\\boldextendedseries";
    if (series == fi_sb_series) return "\\semiboldseries";
    if (series == fi_c_series) return "\\condensedseries";
    return nullptr;
}

// This prints everything.
auto operator<<(std::ostream &fp, const FontInfo &L) -> std::ostream & {
    String s = L.size_name();
    if (s != nullptr) fp << s;
    s = L.shape_name();
    if (s != nullptr) fp << s;
    s = L.family_name();
    if (s != nullptr) fp << s;
    s = L.series_name();
    if (s != nullptr) fp << s;
    return fp;
}

auto operator<<(Logger &X, const FontInfo &x) -> Logger & {
    *(X.fp) << x;
    return X;
}

// This unpacks the font.
void FontInfo::unpack() {
    size   = packed & fi_size_mask;
    tsize  = packed & fi_tsize_mask;
    shape  = packed & fi_shape_mask;
    family = packed & fi_family_mask;
    series = packed & fi_series_mask;
}

// This implements a command like \small.
void FontInfo::change_size(long c) {
    if (c < 0) c = 0;
    if (c > 12) c = 12;
    size = c << 11;
    if (!the_main->use_all_sizes) {
        if (c < 6)
            tsize = fi_small_size;
        else if (c > 6)
            tsize = fi_big_size;
        else
            tsize = 0;
        return;
    }
    switch (c) {
    case 0: tsize = fi_small_size6; return;
    case 1: tsize = fi_small_size5; return;
    case 2: tsize = fi_small_size4; return;
    case 3: tsize = fi_small_size3; return;
    case 4: tsize = fi_small_size2; return;
    case 5: tsize = fi_small_size1; return;
    case 7: tsize = fi_big_size1; return;
    case 8: tsize = fi_big_size2; return;
    case 9: tsize = fi_big_size3; return;
    case 10: tsize = fi_big_size4; return;
    case 11: tsize = fi_big_size5; return;
    case 12: tsize = fi_big_size6; return;
    default: tsize = 0;
    }
}

// This implements a command like \rmfamily: one parameter is changed.
void FontInfo::see_font_change(subtypes c) {
    switch (c) {
    case rm_family_code: family = 0; return;
    case sf_family_code: family = fi_sf_family; return;
    case tt_family_code: family = fi_tt_family; return;
    case md_series_code: series = 0; return;
    case bf_series_code: series = fi_bf_series; return;
    case up_shape_code: shape = 0; return;
    case it_shape_code: shape = fi_it_shape; return;
    case sl_shape_code: shape = fi_sl_shape; return;
    case sc_shape_code: shape = fi_sc_shape; return;
    case em_code:
        if (shape != 0)
            shape = 0;
        else
            shape = fi_it_shape;
        return;
    default: kill(); // should be \normalfont
    }
}

// This implements \fontfamily etc
void FontInfo::ltfont(const std::string &s, subtypes c) {
    switch (c) {
    case fontencoding_code: return; // Output encoding is always Unicode
    case fontfamily_code:           // rm, sf, or tt
        if (s == "cmr" || s == "ptm")
            family = 0;
        else if (s == "cmss" || s == "phv")
            family = fi_sf_family;
        else if (s == "cmtt" || s == "pcr")
            family = fi_tt_family;
        else {
            family = 0;
            log_and_tty << lg_start << "Unknown font family " << s << lg_end;
        }
        return;
    case fontseries_code: // md bf
        if (s == "m")
            series = 0;
        else if (s == "b")
            series = fi_bf_series;
        else if (s == "bx")
            series = fi_bx_series;
        else if (s == "sb")
            series = fi_sb_series;
        else if (s == "c")
            series = fi_c_series;
        else {
            series = 0;
            log_and_tty << lg_start << "Unknown font series " << s << lg_end;
        }
        return;
    case fontshape_code: // it sl sc
        if (s == "n")
            shape = 0;
        else if (s == "it")
            shape = fi_it_shape;
        else if (s == "sl")
            shape = fi_sl_shape;
        else if (s == "sc")
            shape = fi_sc_shape;
        else {
            shape = 0;
            log_and_tty << lg_start << "Unknown font shape " << s << lg_end;
        }
        return;
    default: return; // impossible case
    }
}

// ------------------------------------------------------
// tex fonts

// Finds a font given by name and size, or creates one if needed
auto TexFonts::find_font(const std::string &n, long a, long s) -> size_t {
    for (unsigned i = 0; i < size(); i++)
        if (at(i) == TexFont{n, a, s}) return i;
    return define_a_new_font(n, a, s);
}

// This allocates a new slot in the font list.
auto TexFonts::define_a_new_font(const std::string &n, long a, long s) -> size_t {
    if (size() >= 256) { /// \todo Perhaps remove this artificial limitation
        the_parser.parse_error("fatal: font table overflow");
        return 0;
    }
    emplace_back(n, a, s);
    return size() - 1;
}

/// True if k is a valid font ID
/// \todo Replace that with size_t or std::optional<size_t>
auto TexFonts::is_valid(long k) -> bool { return (k >= 0) && (k < to_signed(size())); }

// Returns name of font
auto TexFonts::name(long k) -> std::string {
    if (!is_valid(k)) return "";
    return at(to_unsigned(k)).name;
}

// Returns name of font
void TexFonts::full_name(Buffer &B, long k) {
    if (!is_valid(k)) return;
    B.push_back(at(to_unsigned(k)).name);
    if (at(to_unsigned(k)).scaled_val != 0) B << fmt::format(" scaled {}", at(to_unsigned(k)).scaled_val);
    if (at(to_unsigned(k)).at_val != 0) {
        B.push_back(" at ");
        B.push_back(ScaledInt(at(to_unsigned(k)).at_val), glue_spec_pt);
    }
}

// Returns an integer parameter for a font
auto TexFonts::get_int_param(long ft, int pos) -> long {
    if (!is_valid(ft)) return -1;
    if (pos == 0) return at(to_unsigned(ft)).hyphen_char;
    return at(to_unsigned(ft)).skew_char;
}

// Returns a dimension parameter for a font
auto TexFonts::get_dimen_param(long ft, long pos) -> ScaledInt {
    if (!is_valid(ft)) return 0;
    if (pos < 0 || to_unsigned(pos) >= at(to_unsigned(ft)).size()) return 0;
    return at(to_unsigned(ft))[to_unsigned(pos)];
}

// Sets an integer parameter for a font
void TexFonts::set_int_param(long ft, int pos, long v) {
    if (!is_valid(ft)) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    if (pos == 0)
        at(to_unsigned(ft)).hyphen_char = v;
    else
        at(to_unsigned(ft)).skew_char = v;
}

// Sets a dimension parameter for a font
void TexFonts::set_dimen_param(long ft, long p, ScaledInt v) {
    if (!is_valid(ft) || p < 0 || p > 100000) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    auto up  = to_unsigned(p);
    auto uft = to_unsigned(ft);
    if (up >= at(uft).size()) at(uft).resize(up + 1);
    at(uft)[up] = v;
}
