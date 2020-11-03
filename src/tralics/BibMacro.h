#pragma once
#include "Buffer.h"

// A bibtex macro, like @string(foo="bar")
struct BibMacro {
    std::string name, value;

    BibMacro() = default;
    BibMacro(std::string s1) : name(std::move(s1)) {}
    BibMacro(std::string s1, std::string s2) : name(std::move(s1)), value(std::move(s2)) {}

    [[nodiscard]] auto is_same(const std::string &s) const -> bool { return name == s; }
};
