#include "tralics/Symcode.h"

auto Symcode::call(subtypes c) -> std::optional<bool> {
    if (action) return (*action)(c);
    return {};
}

automap<symcodes, Symcode> &symcode_map() {
    static automap<symcodes, Symcode> m;
    return m;
}
