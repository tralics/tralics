#include "tralics/Macro.h"
#include "tralics/Parser.h"

// Changes the type of a trivial macro.
void Macro::correct_type() {
    if (type == dt_empty) type = dt_normal; // Is this needed ?
    if (type == dt_normal && body.empty() && nbargs == 0) type = dt_empty;
    if (type == dt_optional && nbargs > 1) {
        const TokenList &L = delimiters[1];
        auto             A = L.begin();
        auto             B = L.end();
        if (A == B) return;
        if (*A != hash_table.dblarg_token) return;
        ++A;
        if (A != B) return;
        type = dt_spec_opt;
    }
}

// compares two macros
auto Macro::is_same(const Macro &aux) const -> bool {
    if (nbargs != aux.nbargs) return false;
    if (type != aux.type) return false;
    if (!body.same_tokens_as(aux.body)) return false;
    for (size_t i = 0; i < 10; i++)
        if (!delimiters[i].same_tokens_as(aux.delimiters[i])) return false;
    return true;
}
