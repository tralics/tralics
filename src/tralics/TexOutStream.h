#pragma once
#include "enums.h"
#include <array>
#include <fstream>
#include <string>

// From TeX:
// The sixteen possible \write streams are represented by the |write_file|
// array. The |j|th file is open if and only if |write_open[j]=true|. The last
// two streams are special; |write_open[16]| represents a stream number
// greater than 15, while |write_open[17]| represents a negative stream number,
// and both of these variables are always |false|.
// Since \write18 is special, we added another slot in write_open

class TexOutStream : public std::array<std::ofstream, nb_output_channels> {
public:
    void close(size_t chan);
    void open(size_t chan, const std::string &fn);
    void write(size_t chan, const std::string &s) { at(chan) << s; }
};
