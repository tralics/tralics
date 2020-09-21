#pragma once
#include "Token.h"

class Hashtab;

struct TokenList : public std::list<Token> {
    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
