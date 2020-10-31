// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002,2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Parser.h"
#include <fmt/format.h>

namespace arith_ns {
    void start_err(String s);
    void end_err();
} // namespace arith_ns

using arith_ns::end_err;
using arith_ns::start_err;

// This prepares for an arithmetic error.
void arith_ns::start_err(String s) {
    err_buf = "Arithmetic overflow";
    if (s != nullptr) err_buf += fmt::format(", threshold={}", s);
}

// Signals the error in the buffer.
void arith_ns::end_err() { the_parser.signal_error(the_parser.err_tok, "Arithmetic overflow"); }

// This makes sure that n fits on 30 bits
void ScaledInt::ovf30() {
    if (value > max_dimension || -value > max_dimension) {
        start_err(nullptr);
        err_buf.format(", threshold={}, cur val=", max_dimension, value);
        end_err();
        value = max_dimension;
    }
}

// This makes sure that n fits on 31 bits
// On a 32 bit machine, only -(max_integer+1) can overflow
void ScaledInt::ovf31() {
    if (value > max_integer || value < -max_integer) {
        start_err(nullptr);
        err_buf.format(", threshold={}, cur val={}", max_integer, value);
        end_err();
        value = 0;
    }
}

// If this is a TeX token representing a valid digit in base radix,
// returns the value,
// otherwise return -1.
auto Token::tex_is_digit(unsigned radix) const -> long {
    auto w = val_as_other();
    if ('0' <= w && w <= radix + '0' && w <= '9') // do not use is_digit...
        return to_signed(w) - '0';
    if (radix != 16) return -1;
    if (w <= 'F' && w >= 'A') return to_signed(w) - 'A' + 10;
    w = val_as_letter(); // allow category code 11 digits
    if (w <= 'F' && w >= 'A') return to_signed(w) - 'A' + 10;
    return -1;
}

// --------------------------------------------------
// Division

// Divide by zero.
void Glue::zdv() {
    start_err(nullptr);
    err_buf += ", division by 0";
    end_err();
}

// This is called x_over_n in TeX
// Divides this by n.
// No overflow possible. Division by zero may happen.
void ScaledInt::divide(long n) {
    auto x = value;
    if (n == 0) {
        // value = 0;
        start_err(nullptr);
        err_buf.format(", division by 0\nin {}/0", x);
        end_err();
        return;
    }
    if (n < 0) {
        x = -x;
        n = -n;
    }
    if (x >= 0)
        value = x / n;
    else
        value = -((-x) / n);
}

// computes xn/d , returns quotient and remainder
// Assumes n>0 and d>0.  Quotient is < 2^30
// This is used for instance when converting 38cm into 38*n/d pt
auto arith_ns::xn_over_d(long x, long n, long d, long &remainder) -> long {
    const int two_fifteen = 1 << 15;
    bool      positive    = true;
    if (x < 0) {
        positive = false;
        x        = -x;
    }
    auto t = (x % two_fifteen) * n;
    auto u = (x / two_fifteen) * n + (t / two_fifteen);
    auto v = (u % d) * two_fifteen + (t % two_fifteen);
    auto w = u / d;
    if (w >= two_fifteen) {
        start_err("2^{30}");
        err_buf.format("\nin {}*{}/{}", x, n, d);
        end_err();
        u = 1;
    } else
        u = two_fifteen * w + (v / d);
    remainder = positive ? (v % d) : -(v % d);
    return positive ? u : -u;
}

// Returns floor(xn/d +1/2)
// This is used when you say \numexpr X*Y/Z
void ScaledInt::scale(long n, long d, long max_answer) {
    auto x        = value;
    bool negative = false;
    if (scale(x, n, d, max_answer, negative)) {
        start_err(nullptr);
        err_buf.format("\nin {}*{}/{}", x, n, d);
        end_err();
    }
    if (negative) value = -value;
}

// Return true if overflow.
auto ScaledInt::scale(long x, long n, long d, long max_answer, bool &negative) -> bool {
    value = 0;
    if (x == 0) return false;
    if (n == 0) return false;
    if (d < 0) {
        d        = -d;
        negative = true;
    }
    if (x < 0) {
        x        = -x;
        negative = !negative;
    }
    if (n < 0) {
        n        = -n;
        negative = !negative;
    }
    long t = n / d;
    if (t > max_answer / x) return true;
    auto a = t * x;
    n      = n - t * d; // answer is a + xn/d, with n<d
    if (n == 0) {
        value = a;
        return false;
    }
    t = x / d;
    if (t > (max_answer - a) / n) return true;
    a = a + t * n;
    x = x - t * d; // answer is a + xn/d, with n<d and x<d
    if (x == 0) {
        value = a;
        return false;
    }
    if (x < n) {
        t = x;
        x = n;
        n = t;
    }
    // Invariants -d<=r<0<n<=x<d
    // f+ floor[(xn+(r+d))/d]  = floor(x0n0/d+1/2)
    long f = 0;
    long r = (d / 2) - d;
    auto h = -r;
    for (;;) {
        if ((n & 1) != 0) {
            r = r + x;
            if (r >= 0) {
                r = r - d;
                ++f;
            }
        }
        n = n / 2;
        if (n == 0) break;
        if (x < h)
            x = x + x;
        else {
            t = x - d;
            x = t + x;
            f = f + n;
            if (x == 0) break;
            if (x < n) {
                t = x;
                x = n;
                n = t;
            }
        }
    }
    if (f > (max_answer - a)) return true;
    value = a + f;
    return false;
}

// Computes x/n, rounded
// No overflow possible. Division by zero must be checked by caller.
// This is used when you say \dimexpr 2pt/3
void ScaledInt::quotient(long d) {
    bool negative = false;
    auto n        = value;
    if (n < 0) {
        n        = -n;
        negative = !negative;
    }
    if (d < 0) {
        d        = -d;
        negative = !negative;
    }
    auto a = n / d;
    n      = n - a * d;
    d      = n - d;
    if (d + n >= 0) ++a;
    if (negative) a = -a;
    value = a;
}

// Three of the four functions defined above apply to glue.
void Glue::quotient(long f) {
    width.quotient(f);
    stretch.quotient(f);
    shrink.quotient(f);
    normalise();
}

void Glue::scale(long n, long d) {
    width.scale(n, d, max_dimension);
    stretch.scale(n, d, max_dimension);
    shrink.scale(n, d, max_dimension);
    normalise();
}

void Glue::divide(long n) {
    if (n == 0) {
        zdv();
        return;
    }
    width.divide(n);
    stretch.divide(n);
    shrink.divide(n);
    normalise();
}

// Two of the four functions defined above apply to everything
// (\numexpr, etc).
void SthInternal::quotient(long f) {
    if (f == 0) {
        the_parser.parse_error("Division by zero");
        return;
    }
    if (type == it_glue || type == it_mu)
        glue_val.quotient(f);
    else
        int_val.quotient(f);
}

void SthInternal::scale(long n, long d) {
    if (d == 0) {
        the_parser.parse_error("Division by zero");
        return;
    }
    if (type == it_glue || type == it_mu)
        glue_val.scale(n, d);
    else if (type == it_int)
        int_val.scale(n, d, max_integer);
    else
        int_val.scale(n, d, max_dimension);
}

// --------------------------------------------------

// Computes nx+y. Overflow is 2^30.
// We assume |y|< 2^{30};  so that overflow_threshold \pm y is OK
auto arith_ns::nx_plus_y(long n, long x, long y) -> long {
    if (n < 0) {
        x = -x;
        n = -n;
    }
    if (n == 0) return y;
    if (x <= (max_dimension - y) / n && -x <= (max_dimension + y) / n) return n * x + y;
    start_err("2^{30}");
    err_buf.format("\nin {}*{}+{}", n, x, y);
    end_err();
    return 0;
}

// Was arith_ns::mult_integers
// Multiplies the scaled number considered as an int by the integer.
void ScaledInt::mult_integer(long x) {
    auto n = value;
    if (n < 0) {
        x = -x;
        n = -n;
    }
    if (n == 0) return;
    if (x <= max_integer / n && -x <= max_integer / n)
        value = n * x;
    else {
        start_err("2^{31}");
        err_buf.format("\nin {}*{}", n, x);
        end_err();
        value = 0;
    }
}

// Was arith_ns::nx_plus_y with 2 args
// Multiplies the scaled number considered as a dimension by the integer.
void ScaledInt::mult_scaled(long x) {
    auto n = value;
    if (n < 0) {
        x = -x;
        n = -n;
    }
    if (n == 0) return;
    if (x <= max_dimension / n && -x <= max_dimension / n)
        value = n * x;
    else {
        start_err("2^{30}=16384pt");
        err_buf.format("\nin {}*{}", n, x);
        end_err();
        value = 0;
    }
}

void SthInternal::expr_mul(long n) {
    if (type == it_glue || type == it_mu)
        glue_val.multiply(n);
    else if (type == it_dimen)
        int_val.mult_scaled(n);
    else
        int_val.mult_integer(n);
}

// Multiplies a glue by an integer n
// What is the difference with void Glue::expr_mul(int n) ??
void Glue::multiply(long n) {
    if (n == 0) {
        kill();
        return;
    }
    width.mult_scaled(n);
    stretch.mult_scaled(n);
    shrink.mult_scaled(n);
}

// This negates a Glue.
void Glue::negate() {
    width   = -width;
    shrink  = -shrink;
    stretch = -stretch;
}

// Adds y to this, assuming we have dimensions.
void ScaledInt::add_dim(ScaledInt Y) {
    auto x = value, y = Y.value;
    auto mx = max_dimension;
    if ((x >= 0 && y <= mx - x) || (x <= 0 && y >= -mx - x))
        value = x + y;
    else {
        value = max_dimension;
        start_err("2^{30}");
        err_buf.format("\nin {}+{}\n", x, y);
        end_err();
    }
}

void Glue::normalise() {
    if (stretch == 0) stretch_order = glue_spec_pt;
    if (shrink == 0) shrink_order = glue_spec_pt;
}

void SthInternal::normalise() {
    if (type == it_glue || type == it_mu) glue_val.normalise();
}

// Add a glue to glue.
void Glue::add(const Glue &r) {
    width.add_dim(r.width);
    if (stretch_order == r.stretch_order) {
        stretch.add_dim(r.stretch);
        if (stretch == 0) stretch_order = glue_spec_pt;
    } else if (stretch_order < r.stretch_order && !(r.stretch == 0)) {
        stretch       = r.stretch;
        stretch_order = r.stretch_order;
    }
    if (shrink_order == r.shrink_order) {
        shrink.add_dim(r.shrink);
        if (shrink == 0) shrink_order = glue_spec_pt;
    } else if (shrink_order < r.shrink_order && !(r.shrink == 0)) {
        shrink       = r.shrink;
        shrink_order = r.shrink_order;
    }
}

// Converts a RealNumber into a a TeX scaled number.
// Overflow is 4096pt
// Inlined call to attach sign
void SthInternal::attach_fraction(RealNumber x) {
    int_val = x.get_ipart();
    long f  = x.get_fpart();
    if (int_val.value >= (1 << 14)) {
        start_err("2^{14}");
        err_buf.format("\nfor {}", int_val.value);
        end_err();
        int_val = max_dimension;
    } else
        int_val = (int_val.value << 16) + f;
    if (x.get_negative()) int_val = -int_val;
}

// This attaches the sign, and checks for overflow.
// Overflow is 4096pt (i.e. 2^30sp)
void SthInternal::attach_sign(bool negative) {
    int_val.ovf30();
    if (negative) int_val = -int_val;
}

// Initialises to zero, with a given type.
void SthInternal::initialise(internal_type t) {
    type    = t;
    int_val = 0;
    glue_val.kill();
}

// copies X into THIS.
void SthInternal::copy(const SthInternal &x) {
    if (type == it_int) {
        if (x.type == it_glue)
            int_val = x.get_glue_width();
        else
            int_val = x.int_val;
    } else if (type == it_dimen) {
        if (x.type == it_glue)
            int_val = x.get_glue_width();
        else if (x.type == it_dimen)
            int_val = x.int_val;
        else {
            int_val = x.int_val;
            the_parser.parse_error("Internal error: Bad Coerce int to dim");
        }
    } else {
        if (x.type == it_glue)
            glue_val = x.glue_val;
        else if (x.type == it_dimen) {
            glue_val.kill();
            glue_val.set_width(x.int_val);
        } else {
            glue_val.kill();
            glue_val.set_width(x.int_val);
            the_parser.parse_error("Internal error: Bad Coerce int to dim");
        }
    }
}

// Hope it's ok for the types...
// No overflow tests ?
void SthInternal::add(const SthInternal &r) {
    if (type != r.type) {
        the_parser.parse_error("Internal error in addition");
        return;
    }
    if (type == it_mu || type == it_glue) {
        glue_val.add(r.glue_val);
        return;
    }
    auto x = int_val.value, y = r.int_val.value;
    int  mx = type == it_int ? max_integer : max_dimension;
    if (x >= 0 && y <= mx - x) {
        int_val = x + y;
        return;
    }
    if (x <= 0 && y >= -mx - x) {
        int_val = x + y;
        return;
    }
    start_err(static_cast<String>(type == it_int ? "2^{31}" : "2^{30}"));
    err_buf.format("\nin {}+{}", x, y);
    end_err();
    int_val = mx;
}

// Converson between mu and normal units, assuming one em =10pt
// The Tex algorithm is more complicated because we have to multiply by a
// non integer. Whose sees the difference.
void ScaledInt::times_10_18() {
    divide(18);
    mult_scaled(10);
}

void SthInternal::get_info(subtypes m) {
    if (type != it_glue) {
        set_int(0);
        return;
    }
    Glue q = glue_val;
    if (m == gluestretch_code)
        set_dim(q.get_stretch());
    else if (m == glueshrink_code)
        set_dim(q.get_shrink());
    else if (m == gluestretchorder_code)
        set_int(q.get_stretch_order());
    else if (m == glueshrinkorder_code)
        set_int(q.get_shrink_order());
    else
        set_int(0);
}

// Initially arith_error was set to true;
// Signals an error
void SthInternal::check_overflow(scan_expr_t t) {
    if (type == it_int || (t == se_mult || t == se_div || t == se_scale)) {
        int_val.ovf31();
    } else if (type == it_dimen) {
        int_val.ovf30();
    } else
        glue_val.check_overflow();
}

void Glue::check_overflow() {
    width.ovf30();
    stretch.ovf30();
    shrink.ovf30();
}

// Reset to zero.
void Glue::kill() {
    width         = 0;
    shrink        = 0;
    stretch       = 0;
    shrink_order  = glue_spec_pt;
    stretch_order = glue_spec_pt;
}

// put x in this, but change the sign is *this<0
void RealNumber::from_int(long x) {
    if (negative) x = -x;
    if (x > 0) {
        ipart    = x;
        fpart    = 0;
        negative = false;
    } else {
        ipart    = -x;
        negative = true;
        fpart    = 0;
    }
}

// Assume that we have read x=0.142. i.e. k=3 digits  in the table.
// This computes y=x* 2^{17}, then (y+1)/2.
void RealNumber::convert_decimal_part(size_t k, const long *table) {
    long f = 0;
    while (k > 0) {
        k--;
        f = (f + (table[k] << 17)) / 10;
    }
    fpart = (f + 1) / 2;
}
