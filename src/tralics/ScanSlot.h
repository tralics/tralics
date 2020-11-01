#pragma once
#include "SthInternal.h"
#include "enums.h"

struct ScanSlot {
    internal_type expr_type{it_int};
    SthInternal   expr_so_far;
    SthInternal   term_so_far;
    scan_expr_t   expr_state{se_none};
    scan_expr_t   term_state{se_none};
    long          numerator{0};

    [[nodiscard]] auto get_next_type() const -> internal_type { return term_state == se_none ? expr_type : it_int; }

    void kill();
    void compute_term(scan_expr_t &next_state, SthInternal f, char &C);
    void add_or_sub(scan_expr_t &next_state, char &C);
};
