#pragma once
#include "Dispatcher.h"
#include "enums.h"
#include <optional>
#include <unordered_map>

template <typename K, typename V> struct automap : public std::unordered_map<K, V> {
    V &operator[](K k) {
        std::unordered_map<K, V>::try_emplace(k, V{k});
        return std::unordered_map<K, V>::at(k);
    }
};

struct Symcode {
    symcodes                                     val;
    std::optional<std::function<bool(subtypes)>> action;

    Symcode(symcodes v) : val(v) {}
    operator symcodes() { return val; }

    auto call(subtypes c) -> std::optional<bool> {
        if (action) return (*action)(c);
        return Dispatcher::call(val, c);
    }
};

inline automap<symcodes, Symcode> &symcode_map() {
    static automap<symcodes, Symcode> m;
    return m;
}
