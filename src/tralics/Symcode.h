#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

class Symcode {
    Symcode(symcodes v) : val(v) {}

public:
    symcodes                                            val;
    std::optional<std::function<bool(subtypes)>>        action;
    std::optional<std::function<std::string(subtypes)>> name_fn;
    std::unordered_map<subtypes, std::string>           name_sub;

    operator symcodes() const { return val; }

    static auto get(symcodes v) -> Symcode &;
    auto        call(subtypes c) const -> std::optional<bool>;
    auto        name(subtypes c) const -> std::optional<std::string>;
};
