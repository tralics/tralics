#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txvars.h"
#include <string>

// Adress of attribute list of an xml object
// is a wrapper around an int
class Xid {
public:
    size_t value; // value of the id

    Xid(size_t v = 0) : value(v) {}

    [[nodiscard]] auto get_att() const -> AttList &;
    [[nodiscard]] auto is_font_change() const -> bool;

    void add_attribute(Istring A, Istring B);
    void add_attribute(Istring A, Istring B, bool f);
    void add_attribute(name_positions A, name_positions B);
    void add_attribute(name_positions A, name_positions B, bool c);
    void add_attribute(name_positions n, Istring v);
    void add_attribute(const AttList &L, bool f);
    void add_attribute_but_rend(Xid b);
    void add_attribute(Xid b);
    void add_ref(const std::string &s);
    void add_span(int n);
    void add_top_rule();
    void add_bottom_rule();
    auto has_attribute(Istring n) -> Istring;
    void add_special_att(const std::string &S);

    auto operator==(const Xid &X) const -> bool { return value == X.value; }
};
