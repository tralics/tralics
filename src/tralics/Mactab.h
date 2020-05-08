#pragma once
#include "../txtokenlist.h"
#include <cstddef>
#include <vector>

struct rc_mac {
    Macro *ptr{nullptr};
    size_t ref{0};
};

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
class Mactab : std::vector<rc_mac> {
private:
    size_t next{0}; // pointer to the first free position

public:
    void incr_macro_ref(size_t c) { at(c).ref++; }
    void delete_macro_ref(size_t i);
    auto get_macro(size_t k) -> Macro & { return *at(k).ptr; }
    auto mc_new_macro(Macro *s) -> size_t;
};
