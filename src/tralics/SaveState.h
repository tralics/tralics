#pragma once
#include "TokenList.h"

class SaveState {
    TokenList &LL;
    TokenList  L;
    bool &     bb;
    bool       b{true};

public:
    SaveState(TokenList &LL, bool &bb) : LL(LL), bb(bb) {
        LL.swap(L);
        std::swap(bb, b);
    }
    ~SaveState() {
        LL.swap(L);
        std::swap(bb, b);
    }
};
