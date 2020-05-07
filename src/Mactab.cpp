#include "tralics/Mactab.h"
#include "tralics/Parser.h"

// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

auto Mactab::new_macro(Macro *s) -> subtypes {
    the_parser.my_stats.one_more_macro();
    if (ptr < 0) rc_mac_realloc();
    auto w      = to_unsigned(ptr);
    ptr         = rc_table[w];
    rc_table[w] = 0;
    table[w]    = s;
    return subtypes(w);
}

void Mactab::rc_mac_realloc() {
    auto k  = rc_table.size();
    auto ns = k + 400;
    table.resize(ns);
    rc_table.resize(ns);
    for (size_t i = k; i < ns; i++) { rc_table[i] = to_signed(i + 1); }
    ptr              = k == 0 ? 1 : to_signed(k);
    rc_table[ns - 1] = -1;
}

// Decrements the reference count. If it is zero, we kill the macro.
void Mactab::delete_macro_ref(size_t i) {
    if (rc_table[i] <= 0) {
        spdlog::critical("Fatal: macro reference count negative");
        abort();
    }
    rc_table[i]--;
    if (rc_table[i] == 0) {
        the_parser.my_stats.one_less_macro();
        delete table[i];
        rc_table[i] = ptr;
        ptr         = to_signed(i);
    }
}
