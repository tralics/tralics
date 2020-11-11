#pragma once
#include "EQTB.h"
#include "ScaledInt.h"

// a glue like \hskip=2.3pt plus 4.5pt minus 6.7fill
struct Glue {
    ScaledInt width;                       // natural width (2.3)
    ScaledInt shrink;                      // shrink (6.7)
    ScaledInt stretch;                     // stretch (4.5)
    glue_spec shrink_order{glue_spec_pt};  // fill, symbolically
    glue_spec stretch_order{glue_spec_pt}; // pt, symbolically

    void kill();
    void normalise();
    void negate();
    void add(const Glue &r);
    void multiply(long n);
    void divide(long n);
    void incr_width(long x) { width += x; }
    void scale(long n, long d);
    void quotient(long f);
    void check_overflow();
    void expr_mul(long f);
    void add_ovf(const Glue &);
};

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream &;

using EqtbGlue = EQTB<Glue>;
