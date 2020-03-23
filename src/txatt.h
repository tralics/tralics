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
    [[nodiscard]] auto has_value(Istring x) const -> std::optional<size_t>;
    void               delete_att(name_positions a);
    void               print(std::ostream &fp);
};
