#pragma once
#include "../txtokenlist.h"
#include "Macro.h"
#include <memory>
#include <vector>

struct rc_mac {
    std::unique_ptr<Macro> ptr;
    size_t                 ref{0};
};

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
struct Mactab : std::vector<rc_mac> {
    void incr_macro_ref(size_t c) { at(c).ref++; }
    void delete_macro_ref(size_t i);
    auto get_macro(size_t k) -> Macro & { return *at(k).ptr; }
    auto mc_new_macro(Macro *s) -> size_t;
};
