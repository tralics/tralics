#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

class Symcode {
    struct automap : public std::unordered_map<symcodes, Symcode> {
        Symcode &operator[](symcodes k) {
            try_emplace(k, Symcode{k});
            return at(k);
        }
    };

    friend class Dispatcher;

    Symcode(symcodes v) : val(v) {}
    static automap &get();

    symcodes                                     val;
    std::optional<std::function<bool(subtypes)>> action;

public:
    static Symcode &get(symcodes v) { return get()[v]; }

    operator symcodes() { return val; }

    auto call(subtypes c) -> std::optional<bool>;
};
