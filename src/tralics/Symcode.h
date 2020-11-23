#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

class Symcode {
    Symcode(symcodes v) : val(v) {}

public:
    symcodes                                     val;
    std::optional<std::function<bool(subtypes)>> action;

    operator symcodes() { return val; }

    static auto get(symcodes v) -> Symcode &;
    auto        call(subtypes c) -> std::optional<bool>;
};
