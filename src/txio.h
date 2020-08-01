#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics/marelle (Jose' Grimm) 2006 2015

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/LineList.h"
#include <filesystem>
#include <fstream>

void readline(char *buffer, size_t screen_size); ///< Read a line from standart input (readline.cpp)

// From TeX:
// The sixteen possible \write streams are represented by the |write_file|
// array. The |j|th file is open if and only if |write_open[j]=true|. The last
// two streams are special; |write_open[16]| represents a stream number
// greater than 15, while |write_open[17]| represents a negative stream number,
// and both of these variables are always |false|.
// Since \write18 is special, we added another slot in write_open
class TexOutStream {
    std::array<std::ofstream, nb_output_channels> write_file{}; // \todo array<ofstream>
    std::array<bool, nb_output_channels>          write_open{};

public:
    TexOutStream();
    void               close(size_t chan);
    void               open(size_t chan, const std::string &fn);
    [[nodiscard]] auto is_open(size_t i) const -> bool { return write_open[i]; }
    void               write(size_t chan, const std::string &s) { write_file[chan] << s; }
};
