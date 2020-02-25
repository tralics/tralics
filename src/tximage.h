#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include <string>
#include <utility>

// data structure for file arguments of \includegraphics

class Image {
public:
    std::string name;     // name
    int         occ{0};   // number of occurences
    int         flags{0}; // existence flags
public:
    Image() = default;
    Image(std::string N, int oc_val) : name(std::move(N)), occ(oc_val) {}
    auto file_exists(String) -> bool;
    void check_existence();
    void check();
};
