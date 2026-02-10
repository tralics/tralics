#pragma once
#include "Glue.h"
#include "RealNumber.h"
#include "TokenList.h"

// This is a union of different things
// it contains the result of scan_something_internal

struct SthInternal {
    Glue          glue_val;     // value if it is a glue
    ScaledInt     int_val{0};   // value if it is a dimension or an integer
    TokenList     token_val;    // value if it is a token list
    internal_type type{it_int}; // this says what the object is.

    [[nodiscard]] auto is_int() const -> bool { return type == it_int; }
    [[nodiscard]] auto is_mu() const -> bool { return type == it_mu; }
    [[nodiscard]] auto is_glue() const -> bool { return type == it_glue; }
    [[nodiscard]] auto is_dimen() const -> bool { return type == it_dimen; }
    [[nodiscard]] auto get_dim_val() const -> ScaledInt { return int_val; }
    [[nodiscard]] auto get_glue_width() const -> ScaledInt { return glue_val.width; }
    [[nodiscard]] auto get_int_val() const -> long { return int_val.value; }
    void add(const SthInternal &r);
    void attach_fraction(RealNumber x);
    void attach_sign(bool negative);
    void change_level(internal_type level);
    void check_overflow(scan_expr_t t);
    void copy(const SthInternal &x);
    void expr_mul(long n);
    void fast_negate() { int_val = -int_val; }
    void get_info(subtypes m);
    void glue_divide(long v) { glue_val.divide(v); }
    void glue_multiply(long v) { glue_val.multiply(v); }
    void incr_dim(ScaledInt x) { int_val += x; }
    void incr_glue(Glue x) { glue_val.add(x); }
    void incr_int(long x) { int_val += ScaledInt{x}; }
    void initialise(internal_type t);
    void normalise();
    void quotient(long f);
    void scale(long n, long d);
    void set_glue_val(const Glue &x) { glue_val = x; }

    void kill() {
        int_val = ScaledInt{0};
        glue_val.kill();
        type = it_int;
    }

    void negate() {
        if (type == it_glue || type == it_mu)
            glue_val.negate();
        else
            int_val = -int_val;
    }

    void set_int(long a) {
        int_val = ScaledInt{a};
        type    = it_int;
    }

    void set_dim(long a) {
        int_val = ScaledInt{a};
        type    = it_dimen;
    }

    void set_dim(ScaledInt a) {
        int_val = a;
        type    = it_dimen;
    }

    void set_glue(const Glue &a) {
        glue_val = a;
        type     = it_glue;
    }

    void set_mu(const Glue &a) {
        glue_val = a;
        type     = it_mu;
    }

    void set_toks(TokenList a) {
        token_val = std::move(a);
        type      = it_tok;
    }

    void glue_to_mu() {
        if (type >= it_glue) int_val = glue_val.width;
    }
};

auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream &;
