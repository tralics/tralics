// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose Grimm) 2004,2007-2011.
// Functions on attributes for Tralics

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txinline.h"
#include "txstack.h"

// This returns the attribute list of this id.
// Uses the global variable the_stack.
auto Xid::get_att() const -> AttList & { return the_main->the_stack->get_att_list(to_unsigned(value)); }

// Return att value if this id has attribute value n.
// Returns null string otherwise
auto Xid::has_attribute(const Istring &n) const -> Istring {
    AttList &X = get_att();
    auto     i = X.lookup(n);
    if (i) return X.get_val(*i);
    return Istring();
}

// Return true if this id has special attribute pair.
// (it is unprintable).
auto Xid::is_font_change() const -> bool {
    Istring n(the_names["'hi_flag"]);
    return static_cast<bool>(get_att().lookup(n));
}

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring &A, const Istring &B) const { get_att().push_back(A, B); }

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring &A, const Istring &B, bool f) const { get_att().push_back(A, B, f); }

// Add attribute named A value B to this id.
void Xid::add_attribute(name_positions A, name_positions B) const { get_att().push_back(the_names[A], the_names[B]); }
void Xid::add_attribute(name_positions A, name_positions B, bool c) const { get_att().push_back(the_names[A], the_names[B], c); }

void Xid::add_attribute(name_positions n, const Istring &v) const { get_att().push_back(the_names[n], v); }

// Adds the list L to the attribute list of this id.

void Xid::add_attribute(const AttList &L, bool force) const {
    size_t   n = L.size();
    AttList &l = get_att();
    for (size_t i = 0; i < n; i++) l.push_back(L[i].name, L[i].value, force); // \todo push_back(L)
}

void Xid::add_attribute_but_rend(Xid b) const {
    AttList &L = b.get_att();
    size_t   n = L.size();
    AttList &l = get_att();
    for (size_t i = 0; i < n; i++)
        if (L[i].name != the_names["rend"]) l.push_back(L[i].name, L[i].value, true); // \todo push_back(L)
}

// Add attribute list of element B to this id.
void Xid::add_attribute(Xid b) const {
    AttList &L = b.get_att();
    add_attribute(L, true);
}
