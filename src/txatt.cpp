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

// Returns a pointer to the pair x=... if it exists, -1 otherwise
auto AttList::has_value(const Istring &x) const -> std::optional<size_t> {
    for (size_t i = 0; i < val.size(); ++i)
        if (val[i].name == x) return i;
    return {};
}

// Return att value if this id has attribute value n.
// Returns null string otherwise
auto Xid::has_attribute(const Istring &n) const -> Istring {
    AttList &X = get_att();
    auto     i = X.has_value(n);
    if (i) return X.get_val(*i);
    return Istring();
}

// Return true if this id has special attribute pair.
// (it is unprintable).
auto Xid::is_font_change() const -> bool {
    Istring n(the_names[cst_flaghi]);
    return static_cast<bool>(get_att().has_value(n));
}

// adds a=b, unless there is already an a in the list.
// In this case, if force is true, removes the old value,
// otherwise does nothing
// Does nothing if b is null (ok if b is empty).
void AttList::push_back(const Istring &a, const Istring &b, bool force) {
    if (b.null()) return;
    auto T = has_value(a);
    if (T) {
        if (force) val[*T].value = b;
        return;
    }
    val.push_back({a, b});
}

// Same function with a name_positions instead of an istring
// Note: istring(a) is the same as the_names[a];
void AttList::push_back(name_positions a, name_positions b, bool force) { push_back(the_names[a], the_names[b], force); }

// Same functions, without a third argument (default is force).
void AttList::push_back(const Istring &n, const Istring &v) { push_back(n, v, true); }

void AttList::push_back(name_positions N, name_positions V) { push_back(the_names[N], the_names[V], true); }

void AttList::push_back(name_positions N, const Istring &v) { push_back(the_names[N], v, true); }

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring &A, const Istring &B) const { get_att().push_back(A, B); }

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring &A, const Istring &B, bool f) const { get_att().push_back(A, B, f); }

// Add attribute named A value B to this id.
void Xid::add_attribute(name_positions A, name_positions B) const { get_att().push_back(A, B); }
void Xid::add_attribute(name_positions A, name_positions B, bool c) const { get_att().push_back(A, B, c); }

void Xid::add_attribute(name_positions n, const Istring &v) const { get_att().push_back(n, v); }

// Adds the list L to the attribute list of this id.

void Xid::add_attribute(const AttList &L, bool force) const {
    size_t   n = L.val.size();
    AttList &l = get_att();
    for (size_t i = 0; i < n; i++) l.push_back(L.val[i].name, L.val[i].value, force);
}

void Xid::add_attribute_but_rend(Xid b) const {
    AttList &L = b.get_att();
    size_t   n = L.val.size();
    AttList &l = get_att();
    for (size_t i = 0; i < n; i++)
        if (L.val[i].name != the_names[np_rend]) l.push_back(L.val[i].name, L.val[i].value, true);
}

// Add attribute list of element B to this id.
void Xid::add_attribute(Xid b) const {
    AttList &L = b.get_att();
    add_attribute(L, true);
}

// We should remove the slot....instead of replacing
void AttList::delete_att(name_positions a) {
    auto i = has_value(the_names[a]);
    if (i && (*i > 0)) val[*i].name = Istring();
}
