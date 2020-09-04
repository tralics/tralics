#pragma once
#include "Buffer.h"

// Whenever Tralics reads a file, it puts the result in a structure like this
struct Line : public std::string {
    int  number;    // the number of the current line
    bool converted; // true if line is already converted

    Line(int n, std::string c = "", bool C = true) : std::string(std::move(c)), number(n), converted(C) {}
};

using line_iterator_const = std::list<Line>::const_iterator;
using line_iterator       = std::list<Line>::iterator;
