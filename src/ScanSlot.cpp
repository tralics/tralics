#include "tralics/ScanSlot.h"

// Kill everything but expr_type
void ScanSlot::kill() {
    expr_so_far.kill();
    term_so_far.kill();
    expr_state = se_none;
    term_state = se_none;
    numerator  = 0;
}

// In the case y+x*a*b/c, we handle the two terms y and x*a*b/c
// Consider the second term. When x is read, the state is none, we copy
// x in term_so_far. Quantities a,b,c and integers. When they are read
// we look at what follows (in next_state) and preceeds (in term_state)
// case *a* : we multiply
// case *b/ we remember b in numerator, change next_state to scale
// case /c?: we divide b c
// case \c? : we scale

void ScanSlot::compute_term(scan_expr_t &next_state, SthInternal f, char &C) {
    C = ' ';
    f.check_overflow(term_state);
    if (term_state == se_none) {
        term_so_far = f;
        if (next_state != se_none) term_so_far.normalise();
    } else if (term_state == se_mult) {
        if (next_state == se_div) {
            next_state = se_scale;
            numerator  = f.get_int_val();
        } else {
            C = '*';
            term_so_far.expr_mul(f.get_int_val());
        }
    } else if (term_state == se_div) {
        C = '/';
        term_so_far.quotient(f.get_int_val());
    } else if (term_state == se_scale) {
        C = '\\';
        term_so_far.scale(numerator, f.get_int_val());
    }
}

// This performs addition or subtraction, or finishes term evaluation
void ScanSlot::add_or_sub(scan_expr_t &next_state, char &C) {
    C = ' ';
    if (next_state > se_sub) {
        term_state = next_state;
        return;
    }
    term_state = se_none; // done with term
    if (expr_state == se_none) {
        C           = '=';
        expr_so_far = term_so_far; // convert term to expre
    } else {
        if (expr_state == se_sub) {
            C = '-';
            term_so_far.negate();
        } else
            C = '+';
        expr_so_far.add(term_so_far);
    }
    expr_state = next_state;
}
