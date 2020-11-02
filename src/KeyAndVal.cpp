#include "tralics/KeyAndVal.h"
#include "tralics/Token.h"
#include "tralics/globals.h"
#include "txtokenlist.h"

// Reverse function
auto KeyAndVal::to_list() const -> TokenList {
    TokenList u = token_ns::string_to_list(name, false);
    if (val.empty()) return u;
    TokenList aux = val;
    u.splice(u.end(), aux);
    return u;
}

// Use the option, meaning execute the code associated to it.
// We put a copy of the value at the end of A, rather than putting it directly
// in the input stream
void KeyAndVal::use(TokenList &A) const {
    TokenList val_copy = val;
    A.splice(A.end(), val_copy);
}

// We execute key=val (from U), with code in this
// If X is false, val is unused, we insert code in L
// Otherwise, we insert key=val
// We mark this as used.
void KeyAndVal::use_and_kill(TokenList &L, KeyAndVal &U, bool X) {
    used = true;
    if (!X) {
        L.splice(L.end(), val); // insert action code
        return;
    }
    TokenList W = U.to_list();
    if (!L.empty()) L.push_back(Token(other_t_offset, ','));
    L.splice(L.end(), W); // insert key=val
}
