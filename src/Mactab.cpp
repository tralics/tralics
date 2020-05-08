#include "tralics/Mactab.h"
#include "tralics/Parser.h"

// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

auto Mactab::mc_new_macro(Macro *s) -> size_t {
    the_parser.my_stats.one_more_macro();
    if (next == size()) {
        push_back({s, 0});
        return next++;
    } else {
        auto w = next;
        next   = at(w).ref;
        at(w)  = {s, 0};
        return w;
    }
}

// Decrements the reference count. If it is zero, we kill the macro.
void Mactab::delete_macro_ref(size_t i) {
    if (at(i).ref == 0) {
        spdlog::critical("Fatal: macro reference count negative");
        abort();
    }
    at(i).ref--;
    if (at(i).ref == 0) {
        the_parser.my_stats.one_less_macro();
        delete at(i).ptr;
        at(i).ref = next;
        next      = i;
    }
}
