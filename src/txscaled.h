#pragma once
// -*- C++ -*-
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This defines the classes: ScaledInt, Glue, RealNumber,
// SthInternal ScanSlot TexRule

#include "tralics/ScaledInt.h"
#include "txtokenlist.h"

struct AttList;

// a glue like \hskip=2.3pt plus 4.5pt minus 6.7fill
// the three ints should be ScaledInt.
class Glue {
    ScaledInt width;                       // natural width (2.3)
    ScaledInt shrink;                      // shrink (6.7)
    ScaledInt stretch;                     // stretch (4.5)
    glue_spec shrink_order{glue_spec_pt};  // fill, symbolically
    glue_spec stretch_order{glue_spec_pt}; // pt, symbolically
public:
    Glue() = default;
    [[nodiscard]] auto get_width() const -> ScaledInt { return width; }
    [[nodiscard]] auto get_shrink() const -> ScaledInt { return shrink; }
    [[nodiscard]] auto get_stretch() const -> ScaledInt { return stretch; }
    [[nodiscard]] auto get_shrink_order() const -> glue_spec { return shrink_order; }
    [[nodiscard]] auto get_stretch_order() const -> glue_spec { return stretch_order; }
    void               set_width(ScaledInt x) { width = x; }
    void               set_shrink(ScaledInt x) { shrink = x; }
    void               set_stretch(ScaledInt x) { stretch = x; }
    void               set_shrink_order(glue_spec x) { shrink_order = x; }
    void               set_stretch_order(glue_spec x) { stretch_order = x; }
    void               kill();
    void               normalise();
    void               negate();
    void               add(const Glue &r);
    void               multiply(long n);
    void               divide(long n);
    void               incr_width(long x) { width += x; }
    void               scale(long n, long d);
    void               quotient(long f);
    void               check_overflow();
    void               expr_mul(long f);
    void               add_ovf(const Glue &);

    static void zdv();
};

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream &;

// The value of the number is sign*(i+f/2^16);
class RealNumber {
    bool negative{false}; // true if negative
    long ipart{0};        // fractional part
    long fpart{0};        // integer part, is <2^16
public:
    void               convert_decimal_part(size_t k, const long *table);
    void               set_ipart(long x) { ipart = x; }
    void               set_fpart(long x) { fpart = x; }
    [[nodiscard]] auto get_ipart() const -> long { return ipart; }
    [[nodiscard]] auto get_fpart() const -> long { return fpart; }
    void               change_sign() { negative = !negative; }
    void               set_negative(bool x) { negative = x; }
    RealNumber() = default;
    void               set_neg() { negative = true; }
    void               change_sign_i() { ipart = -ipart; }
    [[nodiscard]] auto get_negative() const -> bool { return negative; };
    void               from_int(long x);
};

// This is a union of different things
// it contains the result of scan_something_internal
class SthInternal {
    Glue          glue_val;     // value if it is a glue
    ScaledInt     int_val;      // value if it is a dimension or an integer
    TokenList     token_val;    // value if it is a token list
    internal_type type{it_int}; // this says what the object is.
public:
    SthInternal() : int_val(0) {}
    [[nodiscard]] auto is_int() const -> bool { return type == it_int; }
    [[nodiscard]] auto is_mu() const -> bool { return type == it_mu; }
    [[nodiscard]] auto is_glue() const -> bool { return type == it_glue; }
    [[nodiscard]] auto is_dimen() const -> bool { return type == it_dimen; }
    [[nodiscard]] auto get_type() const -> internal_type { return type; }
    void               kill() {
        int_val = 0;
        glue_val.kill();
        type = it_int;
    }
    void               set_type(internal_type X) { type = X; }
    [[nodiscard]] auto get_int_val() const -> long { return int_val.value; }
    [[nodiscard]] auto get_dim_val() const -> ScaledInt { return int_val; }
    auto               get_scaled() -> ScaledInt & { return int_val; }
    [[nodiscard]] auto get_token_val() const -> TokenList { return token_val; }
    void               set_int_val(long k) { int_val = k; }
    void               set_scaled_val(ScaledInt k) { int_val = k; }
    [[nodiscard]] auto get_glue_val() const -> const Glue & { return glue_val; }
    [[nodiscard]] auto get_glue_width() const -> long { return glue_val.get_width().value; }
    void               initialise(internal_type t);
    void               copy(const SthInternal &x);
    void               negate() {
        if (type == it_glue || type == it_mu)
            glue_val.negate();
        else
            int_val = -int_val;
    }
    void fast_negate() { int_val = -int_val; }
    void attach_fraction(RealNumber x);
    void attach_sign(bool negative);
    void set_int(long a) {
        int_val = a;
        type    = it_int;
    }
    void set_dim(long a) {
        int_val = a;
        type    = it_dimen;
    }
    void set_dim(ScaledInt a) {
        int_val = a.value;
        type    = it_dimen;
    }
    void set_glue(Glue a) {
        glue_val = a;
        type     = it_glue;
    }
    void set_mu(Glue a) {
        glue_val = a;
        type     = it_mu;
    }
    void set_toks(TokenList a) {
        token_val = std::move(a);
        type      = it_tok;
    }
    void change_level(internal_type level);
    void glue_to_mu() {
        if (type >= it_glue) int_val = glue_val.get_width().value;
    }
    void add(const SthInternal &r);
    void incr_int(long x) { int_val += x; }
    void incr_dim(ScaledInt x) { int_val += x.value; }
    void incr_glue(Glue x) { glue_val.add(x); }
    void glue_multiply(long v) { glue_val.multiply(v); }
    void glue_divide(long v) { glue_val.divide(v); }
    void set_glue_val(Glue x) { glue_val = x; }
    void cv_dim_to_mu();
    void cv_mu_to_glue();
    void cv_glue_to_mu();
    void get_info(subtypes m);
    void scale(long n, long d);
    void quotient(long f);
    void check_overflow(scan_expr_t t);
    void normalise();
    void expr_mul(long n);
    void add_ovf(const SthInternal &r);
};

auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream &;

class ScanSlot {
public:
    internal_type expr_type{it_int};
    SthInternal   expr_so_far;
    SthInternal   term_so_far;
    scan_expr_t   expr_state{se_none};
    scan_expr_t   term_state{se_none};
    long          numerator{0};

    ScanSlot(internal_type L, SthInternal E, SthInternal T, scan_expr_t R, scan_expr_t S, int N)
        : expr_type(L), expr_so_far(std::move(E)), term_so_far(std::move(T)), expr_state(R), term_state(S), numerator(N) {}
    ScanSlot() = default;

    [[nodiscard]] auto get_next_type() const -> internal_type { return term_state == se_none ? expr_type : it_int; }
    void               kill();
    void               compute_term(scan_expr_t &next_state, SthInternal f, char &C);
    void               add_or_sub(scan_expr_t &next_state, char &C);
};

namespace arith_ns {
    auto nx_plus_y(long n, long x, long y) -> long;
    auto n_times_x(int n, ScaledInt x) -> ScaledInt;
    void scaled_div(ScaledInt &x, int n);
    auto xn_over_d(long x, long n, long d, long &remainder) -> long;
    auto quotient(int n, int d) -> int;
    auto add_ovf(ScaledInt x, ScaledInt y) -> int;
} // namespace arith_ns

class TexRule {
public:
    ScaledInt rule_w, rule_h, rule_d;
    void      reset();
    void      convert(AttList &res) const;
    void      init_vrule() { rule_w = 26214; }
    void      init_hrule() {
        rule_h = 26214;
        rule_d = 0;
    }
    void adjust() {
        rule_h += rule_d;
        rule_d.neg();
    }
};
