#include "tralics/Glue.h"
#include "tralics/Buffer.h"

namespace arith_ns {
    void start_err(std::optional<std::string_view> s);
    void end_err();
} // namespace arith_ns

using arith_ns::end_err;
using arith_ns::start_err;

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
        start_err(std::nullopt);
        err_buf += ", division by 0";
        end_err();
        return;
    }
    width.divide(n);
    stretch.divide(n);
    shrink.divide(n);
    normalise();
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

void Glue::normalise() {
    if (stretch == ScaledInt{0}) stretch_order = glue_spec_pt;
    if (shrink == ScaledInt{0}) shrink_order = glue_spec_pt;
}

// Add a glue to glue.
void Glue::add(const Glue &r) {
    width.add_dim(r.width);
    if (stretch_order == r.stretch_order) {
        stretch.add_dim(r.stretch);
        if (stretch == ScaledInt{0}) stretch_order = glue_spec_pt;
    } else if (stretch_order < r.stretch_order && !(r.stretch == ScaledInt{0})) {
        stretch       = r.stretch;
        stretch_order = r.stretch_order;
    }
    if (shrink_order == r.shrink_order) {
        shrink.add_dim(r.shrink);
        if (shrink == ScaledInt{0}) shrink_order = glue_spec_pt;
    } else if (shrink_order < r.shrink_order && !(r.shrink == ScaledInt{0})) {
        shrink       = r.shrink;
        shrink_order = r.shrink_order;
    }
}

// Reset to zero.
void Glue::kill() {
    width         = ScaledInt{0};
    shrink        = ScaledInt{0};
    stretch       = ScaledInt{0};
    shrink_order  = glue_spec_pt;
    stretch_order = glue_spec_pt;
}

void Glue::check_overflow() {
    width.ovf30();
    stretch.ovf30();
    shrink.ovf30();
}
