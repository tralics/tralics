#pragma once
#include "Buffer.h"

// Whenever Tralics reads a file, it puts the result in a structure like this
struct Clines : public std::string { // \todo rename to Line or something
    int  number;                     // the number of the current line
    bool converted;                  // true if line is already converted

    Clines(int n, std::string c = "", bool C = true) : std::string(std::move(c)), number(n), converted(C) {}

    auto to_string(std::string &C, bool &cv) const -> int {
        C  = *this;
        cv = converted;
        return number;
    }
    auto to_buffer(Buffer &b, bool &C) const -> int {
        b.append(*this);
        C = converted;
        return number;
    }
    void convert_line(size_t wc);
    void clear_converted() { converted = false; }
};

using line_iterator_const = std::list<Clines>::const_iterator;
using line_iterator       = std::list<Clines>::iterator;
