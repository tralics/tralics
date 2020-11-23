#include "tralics/Symcode.h"

auto Symcode::call(subtypes c) -> std::optional<bool> {
    if (action) return (*action)(c);
    return {};
}

auto Symcode::get() -> automap & {
    static automap m;
    return m;
}
