#pragma once
#include "Buffer.h"
#include <list>
#include <string>

// Whenever Tralics reads a file, it puts the result in a structure like this
struct Clines {            // \todo rename to Line or something
    int         number;    // the number of the current line
    std::string chars;     // the characters on the line
    bool        converted; // true if line is already converted

    Clines(int n, std::string c = "", bool C = true) : number(n), chars(std::move(c)), converted(C) {}

    auto to_string(std::string &C, bool &cv) const -> int {
        C  = chars;
        cv = converted;
        return number;
    }
    auto to_buffer(Buffer &b, bool &C) const -> int {
        b.push_back(chars);
        C = converted;
        return number;
    }
    auto starts_with(String x) const -> bool;
    void convert_line(size_t wc);
    void clear_converted() { converted = false; }
};

using line_iterator_const = std::list<Clines>::const_iterator;
using line_iterator       = std::list<Clines>::iterator;
