#pragma once
#include "Token.h"

class Hashtab;

struct TokenList : public std::list<Token> {
    auto block_size() const -> int;

    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
    auto expand_nct(size_t n, uchar c, int &MX, TokenList &body) -> bool;
    void expand_star();
};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
