#include "tralics/Mactab.h"
#include "tralics/Parser.h"
#include <spdlog/spdlog.h>

// finds a slot for the macro
// Note: the reference count is 0. This is strange: the macro has to
// be killed or its reference count increased.

auto Mactab::mc_new_macro(Macro *s) -> size_t {
    push_back({std::unique_ptr<Macro>(s), 0});
    return size() - 1;
}

// Decrements the reference count. If it is zero, we kill the macro.
void Mactab::delete_macro_ref(size_t i) {
    if (at(i).ref == 0) {
        spdlog::critical("Fatal: macro reference count negative");
        abort();
    }
    at(i).ref--;
    if (at(i).ref == 0) at(i).ptr.reset(); // \todo shared_ptr or something RAII?
}
