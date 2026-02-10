#include "tralics/Xid.h"
#include "tralics/Buffer.h"
#include "tralics/NameMapper.h"
#include "tralics/Stack.h"
#include "tralics/Xml.h"
#include "tralics/globals.h"
#include "tralics/util.h"

// Resolve the xml pointer, lazily looking it up if not yet set.
auto Xid::resolve() const -> Xml * {
    if (xml == nullptr) xml = the_stack.elt_from_id(value);
    return xml;
}

void Xid::add_top_rule() const { resolve()->add_top_rule(); }
void Xid::add_bottom_rule() const { resolve()->add_bottom_rule(); }
void Xid::add_span(long n) const { resolve()->add_span(n); }

auto Xid::get_att() const -> AttList & { return resolve()->att; }

auto Xid::has_attribute(const std::string &n) const -> std::string { return resolve()->has_att(n); }

auto Xid::is_font_change() const -> bool { return resolve()->is_font_change(); }

void Xid::add_attribute(const std::string &A, const std::string &B, bool force) const { resolve()->add_att(A, B, force); }

void Xid::add_attribute(const AttList &L, bool force) const { resolve()->add_att(L, force); }

void Xid::add_attribute_but_rend(Xid b) const { resolve()->add_att_but_rend(b); }

void Xid::add_attribute(Xid b) const { resolve()->add_att(b); }

void Xid::add_ref(const std::string &s) const { resolve()->add_ref(s); }

void Xid::add_special_att(const std::string &S, Buffer &B) const { resolve()->add_special_att(S, B); }

auto operator<<(std::ostream &fp, Xid X) -> std::ostream & { return fp << X.resolve()->att; }

auto fetch_att(Xid idx, const std::string &m) -> std::optional<std::string> {
    if (auto *k = idx.resolve()->att.lookup(m)) return encode(*k);
    return {};
}
