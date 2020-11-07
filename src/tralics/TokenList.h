#pragma once
#include "Token.h"

class Hashtab;

struct TokenList : public std::list<Token> {
    [[nodiscard]] auto block_size() const -> int;

    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
    void append(TokenList &L) { splice(end(), L); }
    auto expand_nct(size_t n, uchar c, int &MX, TokenList &body) -> bool;
    void expand_star();
    auto fast_get_block() -> TokenList;
    void fast_get_block(TokenList &res); // \todo rename
    auto get_a_param() -> TokenList;
    void prepend(TokenList &L) { splice(begin(), L); }
    void url_hack();
    void latex_ctr_fnsymbol(long n);
};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;

using EqtbToken = EQTB<TokenList>;
