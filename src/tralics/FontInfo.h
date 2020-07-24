#pragma once
#include "enums.h"
#include <string>
#include <utility>

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
    [[nodiscard]] auto shape_name() const -> std::string;
    [[nodiscard]] auto size_change() const -> std::string;
    [[nodiscard]] auto size_name() const -> std::string;
    [[nodiscard]] auto family_change() const -> std::string;
    [[nodiscard]] auto family_name() const -> std::string;
    [[nodiscard]] auto series_change() const -> std::string;
    [[nodiscard]] auto series_name() const -> std::string;
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
