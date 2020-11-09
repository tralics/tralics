#include "tralics/FontInfo.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include <ostream>

// This returns the position of the name of a font attribute
// for use as an XML attribute.
// This function deals with the size.
auto FontInfo::size_change() const -> std::string {
    if (tsize == fi_small_size) return "font_small";
    if (tsize == fi_big_size) return "font_large";
    if (tsize == 0) return "cst_empty";
    if (tsize == fi_small_size1) return "font_small1";
    if (tsize == fi_small_size2) return "font_small2";
    if (tsize == fi_small_size3) return "font_small3";
    if (tsize == fi_small_size4) return "font_small4";
    if (tsize == fi_small_size5) return "font_small5";
    if (tsize == fi_small_size6) return "font_small6";
    if (tsize == fi_big_size1) return "font_large1";
    if (tsize == fi_big_size2) return "font_large2";
    if (tsize == fi_big_size3) return "font_large3";
    if (tsize == fi_big_size4) return "font_large4";
    if (tsize == fi_big_size5) return "font_large5";
    if (tsize == fi_big_size6) return "font_large6";
    return "cst_empty";
    // maybe np_font_normalsize
}

// Idem. This function deals with the shape.
auto FontInfo::shape_change() const -> std::string {
    if (shape == fi_it_shape) return "font_it";
    if (shape == fi_sl_shape) return "font_slanted";
    if (shape == fi_sc_shape) return "font_sc";
    return "cst_empty";
    // could be np_font_upright
}

// Idem. This function deals with the family.
auto FontInfo::family_change() const -> std::string {
    if (family == fi_sf_family) return "font_sansserif";
    if (family == fi_tt_family) return "font_tt";
    return "cst_empty";
    // could be np_font_roman
}

// Idem. This function deals with the series.
auto FontInfo::series_change() const -> std::string {
    if (series == fi_bf_series) return "font_bold";
    if (series == fi_bx_series) return "font_boldextended";
    if (series == fi_sb_series) return "font_semibold";
    if (series == fi_c_series) return "font_condensed";
    return "cst_empty"; // could be np_font_medium
}

// This function returns a font attribute, as a LaTeX command.

auto FontInfo::size_name() const -> std::string {
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
    return "";
}

// Idem. This function deals with the shape.
auto FontInfo::shape_name() const -> std::string {
    if (shape == fi_it_shape) return "\\itshape";
    if (shape == fi_sl_shape) return "\\slshape";
    if (shape == fi_sc_shape) return "\\scshape";
    return "";
}

// Idem. This function deals with the family.
auto FontInfo::family_name() const -> std::string {
    if (family == fi_sf_family) return "\\sffamily";
    if (family == fi_tt_family) return "\\ttfamily";
    return "";
}

// Idem. This function deals with the series.
auto FontInfo::series_name() const -> std::string {
    if (series == fi_bf_series) return "\\bfseries";
    if (series == fi_bx_series) return "\\boldextendedseries";
    if (series == fi_sb_series) return "\\semiboldseries";
    if (series == fi_c_series) return "\\condensedseries";
    return "";
}

// This prints everything.
auto operator<<(std::ostream &fp, const FontInfo &L) -> std::ostream & {
    return fp << L.size_name() << L.shape_name() << L.family_name() << L.series_name();
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
    if (!the_main.use_all_sizes) {
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
            Logger::finish_seq();
            log_and_tty << "Unknown font family " << s << "\n";
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
            Logger::finish_seq();
            log_and_tty << "Unknown font series " << s << "\n";
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
            Logger::finish_seq();
            log_and_tty << "Unknown font shape " << s << "\n";
        }
        return;
    default: return; // impossible case
    }
}
