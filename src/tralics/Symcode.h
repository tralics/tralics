#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

class Symcode {
    explicit Symcode(symcodes v) : val(v) {}

public:
    symcodes val;

    std::unordered_map<subtypes, std::string>           name_sub;
    std::optional<std::function<std::string(subtypes)>> name_fun;
    std::optional<std::string>                          name_str;

    std::optional<std::function<bool(subtypes)>> action; // \todo add a map like name_sub

    operator symcodes() const { return val; }

    static auto        get(symcodes v) -> Symcode &;
    [[nodiscard]] auto call(subtypes c) const -> std::optional<bool>;
    [[nodiscard]] auto name(subtypes c) const -> std::optional<std::string>;
};
