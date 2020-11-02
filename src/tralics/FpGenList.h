#pragma once
#include "TokenList.h"

// This is for fp
class FpGenList : public TokenList {
public:
    FpGenList(const TokenList &o) : TokenList(o) {}

    void add_last_space(String S);
    void add_last_space(TokenList &W, String S);
    auto find_str(int &n) const -> Token;
    void fp_gen_add();
    void fp_gen_app();
    void fp_gen_exp();
    void fp_gen_komma();
    void fp_gen_mul();
    void remove_first_n(int n);
    void remove_spaces();
    void split_after(int n, TokenList &z);
    auto split_at(Token x, Token x2, TokenList &z) -> Token;
    void split_after(TokenList::iterator X, TokenList &z);
    auto split_at_p(TokenList &A, TokenList &B) -> bool;
    void to_postfix();
    void fp_check_paren();
};
