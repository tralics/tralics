#include "tralics/Macro.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

// Changes the type of a trivial macro.
void Macro::correct_type() {
    if (type == dt_empty) type = dt_normal; // Is this needed ?
    if (type == dt_normal && body.empty() && nbargs == 0) type = dt_empty;
    if (type == dt_optional && nbargs > 1) {
        const TokenList &L = delimiters[1];
        auto             A = L.begin();
        auto             B = L.end();
        if (A == B) return;
        if (*A != the_parser.hash_table.dblarg_token) return;
        ++A;
        if (A != B) return;
        type = dt_spec_opt;
    }
}

// compares two macros
auto Macro::is_same(const Macro &aux) const -> bool {
    if (nbargs != aux.nbargs) return false;
    if (type != aux.type) return false;
    if (!token_ns::compare(body, aux.body)) return false;
    for (size_t i = 0; i < 10; i++)
        if (!token_ns::compare(delimiters[i], aux.delimiters[i])) return false;
    return true;
}
