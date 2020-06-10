#pragma once
#include "Buffer.h"

// A bibtex macro, like @string(foo="bar")
struct BibMacro {
    size_t      h{}; // hash code of the name \todo deprecate?
    std::string name, value;

    BibMacro() = default;
    BibMacro(size_t hash, const Buffer &s1) : h(hash), name(s1) {}
    BibMacro(size_t hash, const std::string &s1, const std::string s2) : h(hash), name(s1), value(s2) {}

    auto is_same(size_t hash, const std::string &s) -> bool { return hash == h && name == s; } // \todo operator==
};
