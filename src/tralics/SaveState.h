#pragma once
#include "TokenList.h"

class SaveState {
    TokenList &LL;
    TokenList  L;
    bool &     bb;
    bool       b{true};

public:
    SaveState(TokenList &LLL, bool &bbb) : LL(LLL), bb(bbb) {
        LL.swap(L);
        std::swap(bb, b);
    }
    ~SaveState() {
        LL.swap(L);
        std::swap(bb, b);
    }
};
