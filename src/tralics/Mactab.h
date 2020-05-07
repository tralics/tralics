#pragma once
#include "enums.h"
#include <cstddef>
#include <vector>

class Macro;

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
class Mactab {
private:
    std::vector<Macro *> table;    // this contains the table
    std::vector<long>    rc_table; // this contains the reference counts
    long                 ptr{-1};  // pointer to the first free position

    void rc_mac_realloc();

public:
    void incr_macro_ref(size_t c) { rc_table[c]++; }
    void delete_macro_ref(size_t i);
    auto get_macro(size_t k) -> Macro & { return *table[k]; }
    auto new_macro(Macro *s) -> subtypes;
};
