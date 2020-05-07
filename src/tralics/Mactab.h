#pragma once
#include "enums.h"
#include <cstddef>

class Macro;

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
class Mactab {
private:
    Macro **table{nullptr};    // this contains the table
    long *  rc_table{nullptr}; // this contains the reference counts
    size_t  cur_rc_mac_len{0}; // size of the table.
    long    ptr{-1};           // pointer to the first free position

    void rc_mac_realloc();

public:
    Mactab() = default;

    void incr_macro_ref(int c) { rc_table[c]++; }
    void delete_macro_ref(int i);
    auto get_macro(int k) -> Macro & { return *table[k]; }
    auto new_macro(Macro *s) -> subtypes;
};
