#pragma once
#include "TokenList.h"
#include <array>
#include <ostream>

// This represents the value of a user-defined command
class Macro {
    std::array<TokenList, 10> delimiters; // delimiters between arguments
public:
    size_t    nbargs{0};       // number of arguments
    def_type  type{dt_normal}; // type of macro
    TokenList body;            // the body

    Macro() = default;
    explicit Macro(TokenList L) : body(std::move(L)) { correct_type(); }

    [[nodiscard]] auto is_same(const Macro &aux) const -> bool;
    [[nodiscard]] auto operator[](size_t n) const -> const TokenList & { return delimiters[n]; }

    void set_nbargs(size_t n) { nbargs = n; } // \todo remove
    void set_type(def_type n) { type = n; }
    void set_delimiters(size_t k, TokenList L) { delimiters[k] = std::move(L); }
    void correct_type();
};

auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream &;
