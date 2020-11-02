#include "tralics/FpStack.h"
#include "tralics/FpNum.h"
#include "tralics/Parser.h"

// upn. Thes stack is a comm-separated list of tokens.
// This adds L in front.
void FpStack::push_upn(TokenList &L) {
    if (!empty()) push_front(the_parser.hash_table.comma_token);
    prepend(L);
}

// This adds a fp number in front of the stack.
void FpStack::push_upn(const FpNum &x) {
    if (!empty()) push_front(the_parser.hash_table.comma_token);
    TokenList xv = x.to_list();
    prepend(xv);
}

// This takes all tokens from this, until comma or end of list
// and puts them in L
void FpStack::pop_upn(TokenList &L) {
    if (empty()) {
        the_parser.parse_error("UPN stack empty");
        return;
    }
    for (;;) {
        if (empty()) return;
        Token x = front();
        pop_front();
        if (x.is_comma_token()) return;
        L.push_back(x);
    }
}

// This takes all tokens from this, until comma or end of list
// converts them into a FpNumber and puts the result in x.
void FpStack::pop_upn(FpNum &x) {
    x.reset();
    x.sign = true; // Now x is zero
    TokenList L;
    pop_upn(L);
    if (L.empty()) return;
    the_parser.fp_send_one_arg(L);
    the_parser.back_input(L);
    x = the_parser.fp_read_value();
}
