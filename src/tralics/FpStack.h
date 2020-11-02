#pragma once
#include "TokenList.h"

struct FpNum;

class FpStack : public TokenList {
public:
    void pop_upn(FpNum &x);
    void pop_upn(TokenList &L);
    void push_upn(TokenList &L);
    void push_upn(const FpNum &x);
};
