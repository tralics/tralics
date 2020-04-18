#pragma once
#include "tralics/Buffer.h"
#include "txeqtb.h"

// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Data structure for label and references.
class LabelInfo {
public:
    Istring     id;             // value of the ID
    Istring     name;           // name of the ID
    std::string filename;       // file of definition
    int         lineno{0};      // line of definition
    bool        used{false};    // is this ID used ?
    bool        defined{false}; // is this ID defined ?

    LabelInfo(Istring k = {}) : name(k) {}

    auto set_used() -> bool { return std::exchange(used, true); }
    auto set_defined() -> bool { return std::exchange(defined, true); }
};
