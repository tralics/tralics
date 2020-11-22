#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

struct Symcode {
    symcodes val;

    Symcode(symcodes v) : val(v) {}
    operator symcodes() { return val; }

    auto call(subtypes c) -> std::optional<bool> { return Dispatcher::call(val, c); }
};

std::unordered_map<symcodes, Symcode> &symcode_map() {
    static std::unordered_map<symcodes, Symcode> m;
    return m;
}
