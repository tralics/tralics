#pragma once
#include "Token.h"

class Hashtab;

struct TokenList : public std::list<Token> {
    [[nodiscard]] auto block_size() const -> int;

    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
    auto expand_nct(size_t n, uchar c, int &MX, TokenList &body) -> bool;
    void expand_star();
    auto fast_get_block() -> TokenList;
    void fast_get_block(TokenList &res); // \todo rename
};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
