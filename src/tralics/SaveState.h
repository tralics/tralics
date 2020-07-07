#pragma once
#include "Token.h"

class SaveState {
    TokenList L; // the token list to be restored
public:
    bool restricted{}; // the restricted flag to be restored

    void copy_and_reset(TokenList &X) {
        L.clear();
        L.splice(L.begin(), X);
    }
    void restore(TokenList &X) {
        X.clear();
        X.splice(X.begin(), L);
    }
    void set_restricted(bool b) { restricted = b; }
};
