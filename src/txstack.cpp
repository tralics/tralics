// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file implements the XML stack for Tralics

#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <fmt/ostream.h>

auto Parser::last_att_list() -> AttList & { return the_stack.get_top_id().get_att(); }

// Adds a new <u>, <v>, and att-list slot
void ArrayInfo::add_uv(TokenList &u, TokenList &v, const AttList &At) {
    u_table.push_back(u);
    v_table.push_back(v);
    attribs.push_back(At);
}

// This gets u-part or v-part
auto ArrayInfo::get_u_or_v(bool u_or_v, size_t pos) const -> TokenList {
    if (pos >= attribs.size()) return TokenList();
    return u_or_v ? u_table[pos] : v_table[pos];
}

auto ArrayInfo::get_cell_atts(size_t k) const -> AttList {
    if (k >= attribs.size()) return AttList();
    return attribs[k];
}
