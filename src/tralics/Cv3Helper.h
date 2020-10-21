#pragma once
#include "Math.h"

class Cv3Helper : public Math {
    Math       res;
    Xml *      p{};
    int        ploc{};
    math_types ptype{};
    math_types prefix_type{};
    Xml *      index{}, *exponent{};
    int        special{0}; // Sum or product

    [[nodiscard]] auto get_cmd() const -> symcodes { return front().cmd; }
    [[nodiscard]] auto get_chr() const -> subtypes { return front().chr; }
    auto               get_xmltype() -> math_types { return front().get_xmltype(); }

public:
    Cv3Helper(Math X) : Math(std::move(X)) {}
    int  state{};
    void reinit();
    void non_script();
    void find_kernel();
    void find_index(math_style cms);
    void add_kernel(math_style cms);
    auto find_operator(math_style cms) -> std::string;
    auto finish() -> Math { return res; }
};
