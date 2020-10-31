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

#include "tralics/Glue.h"
#include "tralics/RealNumber.h"
#include "tralics/ScaledInt.h"
#include "tralics/SthInternal.h"
#include "txtokenlist.h"

struct AttList;

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
