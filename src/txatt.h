#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txstring.h"

// This is an attribute list.
struct AttPair {
    Istring name, value;
};

class AttList {
    // vector of attribute, like colspan=4, width=3.4cm
    // name (first) and val (second) are pointers into the SH table
public:
    std::vector<AttPair> val; // the value

    auto               get_val(size_t i) -> Istring { return val[i].value; }
    void               reset() { val = std::vector<AttPair>(); }
    auto               empty() -> bool { return val.empty(); }
    void               push_back_empty(Istring n);
    void               push_back(name_positions n, Istring v);
    void               push_back(name_positions n, name_positions v);
    void               push_back(name_positions a, name_positions b, bool force);
    void               push_back(Istring n, Istring v);
    void               push_back(Istring a, Istring b, bool f);
    [[nodiscard]] auto has_value(Istring x) const -> int;
    void               delete_att(name_positions a);
    void               destroy();
    void               print(std::ostream &fp);
};

// This is a table of AttList. We should use a vector instead
class AttListTable {
    AttList *table{nullptr}; // the table
    Xml *    xtable{nullptr};
    int      len{0};  // the length of the table
    int      size{0}; // the size of the table
public:
    void destroy();
    void resize();
    void init();
    auto operator[](int k) -> AttList & { return table[k]; }
    auto get(int n) -> AttList {
        if (n < len) return table[n];
        return AttList();
    }
    void               push_back(AttList X);
    void               push_back();
    void               push_back(Istring, Istring);
    void               push_back(name_positions, name_positions);
    [[nodiscard]] auto get_size() const -> int { return size; }
};
