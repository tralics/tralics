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

// Returns a new element named N, initialised with z (if not empty...)
Xml::Xml(Istring N, Xml *z) : name(std::move(N)) {
    id = the_main->the_stack->next_xid(this);
    if (z != nullptr) add_tmp(gsl::not_null{z});
}

// This adds x at the end the element
void Xml::push_back_unless_nullptr(Xml *x) {
    if (x != nullptr) push_back(gsl::not_null{x});
}

// True if last element on the tree is a string.
auto Xml::last_is_string() const -> bool { return !empty() && back()->id.value == 0; }

// Assume that last element is a string. This string is put in the internal
// buffer
void Xml::last_to_SH() const {
    shbuf.clear();
    shbuf.push_back(back()->name.name);
}

// This adds B at the end the element, via concatenation, if possible.
void Xml::add_last_string(const Buffer &B) {
    if (B.empty()) return;
    shbuf.clear();
    if (last_is_string()) {
        last_to_SH();
        pop_back();
        the_parser.my_stats.one_more_merge();
    }
    shbuf.push_back(B);
    push_back_unless_nullptr(new Xml(shbuf));
}

// This adds x and a \n at the end of this.
void Xml::add_last_nl(Xml *x) {
    if (x != nullptr) {
        push_back_unless_nullptr(x);
        push_back_unless_nullptr(the_main->the_stack->newline_xml);
    }
}

// Removes a trailing space on the tree.
void Xml::remove_last_space() {
    if (!last_is_string()) return;
    last_to_SH();
    auto k = shbuf.size();
    shbuf.remove_space_at_end();
    if (k != shbuf.size()) {
        pop_back();
        if (!shbuf.empty()) push_back_unless_nullptr(new Xml(shbuf));
    }
}

// This adds a NL to the end of the element
void Xml::add_nl() {
    if (!all_empty() && back_or_nullptr() == the_main->the_stack->newline_xml) return;
    push_back_unless_nullptr(the_main->the_stack->newline_xml);
}

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

// This returns the span of the current cell; -1 in case of trouble
// the default value is 1
auto Xml::get_cell_span() const -> long { // \todo std::optional<size_t>
    if (is_xmlc()) return 0;
    if (!has_name(the_names["cell"])) return -1;             // not a cell
    if (!shbuf.install_att(id, the_names["cols"])) return 1; // no property, default is 1
    auto o = shbuf.int_val();
    return o ? to_signed(*o) : -1;
}

auto Xml::tail_is_anchor() const -> bool { return !empty() && back()->is_anchor(); }
