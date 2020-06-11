#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file holds the stack and related stuff

#include "tralics/AttList.h"
#include "tralics/Xml.h"

// Array management
class ArrayInfo {
    std::vector<AttList>   attribs; // positions attributes for each row
    std::vector<TokenList> u_table, v_table;

public:
    Xid    id;         // the id of the table
    size_t cell_no{0}; // current cell number, first on row is zero

    ArrayInfo(Xid a1) : id(a1) {}

    [[nodiscard]] auto get_cell_atts(size_t k) const -> AttList;
    [[nodiscard]] auto get_u_or_v(bool u_or_v, size_t pos) const -> TokenList;
    void               add_uv(TokenList &u, TokenList &v, const AttList &At);
};
