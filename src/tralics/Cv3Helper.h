#pragma once
#include "Math.h"

class Cv3Helper : public Math {
    Xml       *index{};
    Xml       *exponent{};
    Xml       *p{};
    long       ploc{};
    math_types ptype{};
    math_types prefix_type{};
    int        special{0}; // Sum or product

public:
    int  state{};
    Math res;

    explicit Cv3Helper(Math X) : Math(std::move(X)) {}

    void reinit();
    void non_script();
    void find_kernel();
    void find_index(math_style cms);
    void add_kernel(math_style cms);
    auto find_operator(math_style cms) -> std::string;
};
