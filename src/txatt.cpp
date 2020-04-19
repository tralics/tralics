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
#include <utility>

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
void AttList::push_back(const Istring& n, const Istring& v) { push_back(n, v, true); }

void AttList::push_back(name_positions N, name_positions V) { push_back(the_names[N], the_names[V], true); }

void AttList::push_back(name_positions N, const Istring& v) { push_back(the_names[N], v, true); }

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring& A, const Istring& B) const { get_att().push_back(A, B); }

// Add attribute named A value B to this id.
void Xid::add_attribute(const Istring& A, const Istring& B, bool f) const { get_att().push_back(A, B, f); }

// Add attribute named A value B to this id.
void Xid::add_attribute(name_positions A, name_positions B) const { get_att().push_back(A, B); }
void Xid::add_attribute(name_positions A, name_positions B, bool c) const { get_att().push_back(A, B, c); }

void Xid::add_attribute(name_positions n, const Istring& v) const { get_att().push_back(n, v); }

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
    if (i && (*i > 0)) val[*i].name = Istring(0UL);
}

// Puts in the buffer the value of the attribute M of element idx
// returns false if there is no such value.
auto Buffer::install_att(Xid idx, const Istring& m) -> bool {
    AttList &L = idx.get_att();
    auto     k = L.has_value(m);
    if (!k) return false;
    reset();
    push_back(L.get_val(*k).name);
    return true;
}

// Puts the attribute list in the buffer
// We can have Foo="bar'", printed as Foo='bar&apos;'
// The case Foo'bar="&gee" is not handled.
// nothing is printed in case the name starts with an apostrophe.
// Print in reverse order, because this was in the previous version

void Buffer::push_back(const AttList &Y) {
    auto n = Y.val.size();
    if (the_main->double_quote_att)
        for (auto i = n; i > 0; i--) push_back_alt(Y.val[i - 1]);
    else
        for (auto i = n; i > 0; i--) push_back(Y.val[i - 1]);
}

void Buffer::push_back(const AttPair &X) {
    const Istring &b = X.name;
    const Istring &a = X.value;
    if (a.null()) return;
    if (b.null()) return;
    auto B = b.value();
    auto A = a.value();
    if (B[0] == '\'') return;
    push_back(' ');
    push_back(B);
    push_back('=');
    push_back('\'');
    for (char c : A) {
        if (c == '\'')
            push_back("&apos;");
        else
            push_back(c);
    }
    push_back('\'');
}

// Use double quotes instead of single quotes
void Buffer::push_back_alt(const AttPair &X) {
    const Istring &b = X.name;
    const Istring &a = X.value;
    if (a.null()) return;
    if (b.null()) return;
    auto B = b.value();
    auto A = a.value();
    if (B[0] == '\'') return;
    push_back(' ');
    push_back(B);
    push_back('=');
    push_back('\"');
    for (char c : A) {
        if (c == '\"')
            push_back("&quot;");
        else
            push_back(c);
    }
    push_back('\"');
}

// Returns true if there is a space. Kills at the space. Advance
auto Buffer::look_at_space(const std::string &s) -> bool {
    reset();
    push_back(s);
    bool has_space = false;
    ptr            = 0;
    for (int i = 0;; i++) {
        if (head() == 0) break;
        if (is_space(head())) {
            has_space = true;
            at(ptr)   = 0;
            advance();
            break;
        }
        advance();
    }
    return has_space;
}

// What follows is used for handling the configuration file.

// In the case of "foo" (no space), returns <foo>
// In the case of space, what follows the spaces is considered as
// attribute list.
auto Buffer::xml_and_attrib(const std::string &s) -> Xml {
    bool has_spaces = look_at_space(s);
    if (!has_spaces) return {Istring(s), nullptr};
    Xml res{Istring(data()), nullptr};
    push_back_special_att(res.id);
    return res;
}

// This converts the buffer in to an attribute list for id.
void Buffer::push_back_special_att(Xid id) {
    for (;;) {
        if (!find_equals()) return;
        auto J = ptr1;
        if (!backup_space()) return;
        advance();
        if (!string_delims()) return;
        Istring a = Istring(to_string(J));
        Istring b = Istring(to_string(ptr1));
        id.add_attribute(a, b);
        advance();
    }
}

// Returns true if we see space, then s then space then equals then space.
// sets ptr to the char after this space
auto Buffer::see_equals(String s) -> bool {
    ptr = 0;
    skip_sp_tab();
    auto k = strlen(s);
    if (strncmp(data() + ptr, s, k) != 0) return false;
    ptr += k;
    skip_sp_tab();
    if (next_char() != '=') return false;
    skip_sp_tab();
    return true;
}
