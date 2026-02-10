#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <spdlog/fmt/fmt.h>

namespace arith_ns {
    void start_err(std::optional<std::string_view> s);
    void end_err();
} // namespace arith_ns

using arith_ns::end_err;
using arith_ns::start_err;

// This prepares for an arithmetic error.
void arith_ns::start_err(std::optional<std::string_view> s) {
    err_buf = "Arithmetic overflow";
    if (s) err_buf += fmt::format(", threshold={}", *s);
}

// Signals the error in the buffer.
void arith_ns::end_err() { the_parser.signal_error(the_parser.err_tok, "Arithmetic overflow"); }

// --------------------------------------------------
// Division

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

void SthInternal::expr_mul(long n) {
    if (type == it_glue || type == it_mu)
        glue_val.multiply(n);
    else if (type == it_dimen)
        int_val.mult_scaled(n);
    else
        int_val.mult_integer(n);
}

void SthInternal::normalise() {
    if (type == it_glue || type == it_mu) glue_val.normalise();
}

// Converts a RealNumber into a a TeX scaled number.
// Overflow is 4096pt
// Inlined call to attach sign
void SthInternal::attach_fraction(RealNumber x) {
    int_val = ScaledInt{x.ipart};
    long f  = x.fpart;
    if (int_val.value >= (1 << 14)) {
        start_err("2^{14}");
        err_buf.format("\nfor {}", int_val.value);
        end_err();
        int_val = ScaledInt{max_dimension};
    } else
        int_val = ScaledInt{(int_val.value << 16) + f};
    if (x.negative) int_val = -int_val;
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
    int_val = ScaledInt{0};
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
            glue_val.width = x.int_val;
        } else {
            glue_val.kill();
            glue_val.width = x.int_val;
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
        int_val = ScaledInt{x + y};
        return;
    }
    if (x <= 0 && y >= -mx - x) {
        int_val = ScaledInt{x + y};
        return;
    }
    start_err(std::string_view(type == it_int ? "2^{31}" : "2^{30}"));
    err_buf.format("\nin {}+{}", x, y);
    end_err();
    int_val = ScaledInt{mx};
}

void SthInternal::get_info(subtypes m) {
    if (type != it_glue) {
        set_int(0);
        return;
    }
    Glue q = glue_val;
    if (m == gluestretch_code)
        set_dim(q.stretch);
    else if (m == glueshrink_code)
        set_dim(q.shrink);
    else if (m == gluestretchorder_code)
        set_int(q.stretch_order);
    else if (m == glueshrinkorder_code)
        set_int(q.shrink_order);
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
