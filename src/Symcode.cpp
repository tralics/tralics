#include "tralics/Symcode.h"

auto Symcode::call(subtypes c) -> std::optional<bool> {
    if (action) return (*action)(c);
    return {};
}

Symcode &Symcode::get(symcodes v) {
    static std::unordered_map<symcodes, Symcode> m;
    m.try_emplace(v, Symcode{v});
    return m.at(v);
}
