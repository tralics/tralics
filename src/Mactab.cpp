#include "tralics/Mactab.h"
#include "tralics/Parser.h"

// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

auto Mactab::new_macro(Macro *s) -> subtypes {
    the_parser.my_stats.one_more_macro();
    if (ptr < 0) rc_mac_realloc();
    auto w      = ptr;
    ptr         = rc_table[w];
    rc_table[w] = 0;
    table[w]    = s;
    return subtypes(w);
}

// Initially cur_rc_mac_len=0, tables are empty
// Note that the table always contains a valid pointer
// \todo use std::vector instead
void Mactab::rc_mac_realloc() {
    auto         k         = cur_rc_mac_len;
    auto         ns        = k + 400;
    static auto *empty_mac = new Macro;
    auto *       T1        = new Macro *[ns];
    auto *       T2        = new long[ns];
    for (size_t i = 0; i < k; i++) {
        T1[i] = table[i];
        T2[i] = rc_table[i];
    }
    delete[] table;
    delete[] rc_table;
    table    = T1;
    rc_table = T2;
    for (size_t i = k; i < ns; i++) {
        table[i]    = empty_mac;
        rc_table[i] = to_signed(i + 1);
    }
    ptr              = k == 0 ? 1 : to_signed(k);
    rc_table[ns - 1] = -1;
    cur_rc_mac_len   = ns;
}

// Decrements the reference count. If it is zero, we kill the macro.
void Mactab::delete_macro_ref(int i) {
    if (rc_table[i] <= 0) {
        spdlog::critical("Fatal: macro reference count negative");
        abort();
    }
    rc_table[i]--;
    if (rc_table[i] == 0) {
        the_parser.my_stats.one_less_macro();
        delete table[i];
        rc_table[i] = ptr;
        ptr         = i;
    }
}
