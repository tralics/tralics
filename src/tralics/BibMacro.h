#pragma once
#include <utility>

#include "Buffer.h"

// A bibtex macro, like @string(foo="bar")
struct BibMacro {
    size_t      h{}; // hash code of the name \todo deprecate?
    std::string name, value;

    BibMacro() = default;
    BibMacro(size_t hash, const Buffer &s1) : h(hash), name(s1) {}
    BibMacro(size_t hash, std::string s1, std::string s2) : h(hash), name(std::move(s1)), value(std::move(s2)) {}

    [[nodiscard]] auto is_same(size_t hash, const std::string &s) const -> bool { return hash == h && name == s; } // \todo operator==
};
