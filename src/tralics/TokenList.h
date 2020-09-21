#pragma once
#include "Token.h"

struct TokenList : public std::list<Token> {
    void add_env(const std::string &name);
};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
