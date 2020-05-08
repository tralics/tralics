#include "tralics/Mactab.h"
#include "tralics/Parser.h"

// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

auto Mactab::mc_new_macro(Macro *s) -> size_t {
    the_parser.my_stats.one_more_macro();
    push_back({s, 0});
    return size() - 1;
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
    }
}
