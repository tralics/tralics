#pragma once
#include "EQTB.h"
#include "enums.h"
#include <string>

struct ScaledInt {
    long value; // the integer, considered as a scaled number

    explicit ScaledInt(long v = 0) : value(v) {}
    explicit ScaledInt(subtypes v) : value(int(v)) {} // \todo This is a bit hackish, but it works (going through `long` fails).

    auto operator==(ScaledInt X) const { return value == X.value; }
    auto operator-() const -> ScaledInt { return ScaledInt(-value); }
    void operator+=(ScaledInt X) { value += X.value; }

    void add_dim(ScaledInt Y);
    void neg() { value = -value; }
    void divide(long n);
    void quotient(long d);
    void scale(long n, long d, long max_answer);
    auto scale(long x, long n, long d, long max_answer, bool &negative) -> bool;
    void mult_scaled(long x);
    void mult_integer(long x);
    void times_10_18();
    void ovf30();
    void ovf31();
    operator std::string() const;
};

auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream &;

using EqtbDim = EQTB<ScaledInt>;
