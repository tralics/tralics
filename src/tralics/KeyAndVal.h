#pragma once
#include "TokenList.h"

// This holds the translation of \DeclareOption{foo}{bar} or
// \usepackage[foo=bar]{pack} name foo, val bar

// In the case of \DeclareOption{foo}{bar}, then name=full_name =foo
// and val is bar or empty. In the case \usepackage, this is different

struct KeyAndVal {
    bool        used{false}; // set to true when option is used
    std::string name;        // name of the option, e.g. 'key'
    std::string full_name;   // In case 'key = {\val,etc}' holds all chars
    TokenList   val;         // value of the option, eg '={\val,etc}'.

    KeyAndVal() = default;
    KeyAndVal(std::string a, TokenList b, std::string all) : name(std::move(a)), full_name(std::move(all)), val(std::move(b)) {}

    [[nodiscard]] auto to_list() const -> TokenList;

    void use(TokenList &A) const;
    void use_and_kill(TokenList &L, const KeyAndVal &U, bool X);
};

inline auto operator<<(std::ostream &os, const KeyAndVal &kv) -> std::ostream & { return os << kv.full_name; }
