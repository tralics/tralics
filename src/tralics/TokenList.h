#pragma once
#include "Token.h"

struct TokenList : public std::list<Token> {};

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
