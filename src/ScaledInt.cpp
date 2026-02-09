#include "tralics/ScaledInt.h"
#include "tralics/Buffer.h"

namespace arith_ns {
    void start_err(std::optional<std::string_view> s);
    void end_err();
} // namespace arith_ns

using arith_ns::end_err;
using arith_ns::start_err;

ScaledInt::operator std::string() const {
    Buffer B;
    B.push_back(*this, glue_spec_pt);
    return std::move(B);
}

auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream & {
    Buffer B;
    B.push_back(x, glue_spec_pt);
    return fp << B;
}

// This makes sure that n fits on 30 bits
void ScaledInt::ovf30() {
    if (value > max_dimension || -value > max_dimension) {
        start_err(std::nullopt);
        err_buf.format(", threshold={}, cur val=", max_dimension, value);
        end_err();
        value = max_dimension;
    }
}

// This makes sure that n fits on 31 bits
// On a 32 bit machine, only -(max_integer+1) can overflow
void ScaledInt::ovf31() {
    if (value > max_integer || value < -max_integer) {
        start_err(std::nullopt);
        err_buf.format(", threshold={}, cur val={}", max_integer, value);
        end_err();
        value = 0;
    }
}

// This is called x_over_n in TeX
// Divides this by n.
// No overflow possible. Division by zero may happen.
void ScaledInt::divide(long n) {
    auto x = value;
    if (n == 0) {
        // value = 0;
        start_err(std::nullopt);
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

// Returns floor(xn/d +1/2)
// This is used when you say \numexpr X*Y/Z
void ScaledInt::scale(long n, long d, long max_answer) {
    auto x        = value;
    bool negative = false;
    if (scale(x, n, d, max_answer, negative)) {
        start_err(std::nullopt);
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

// Converson between mu and normal units, assuming one em =10pt
// The Tex algorithm is more complicated because we have to multiply by a
// non integer. Whose sees the difference.
void ScaledInt::times_10_18() {
    divide(18);
    mult_scaled(10);
}
