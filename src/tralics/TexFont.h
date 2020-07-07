#pragma once
#include "TeXChar.h"

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
