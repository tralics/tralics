#pragma once
#include "types.h"
#include <optional>
#include <string>

class Xml;
struct AttList;
class Buffer;

class Xid {
public:
    size_t       value;
    mutable Xml *xml{nullptr}; ///< direct pointer to owning Xml (lazily resolved)

    Xid(size_t v = 0) : value(v) {}
    Xid(size_t v, Xml *p) : value(v), xml(p) {}

    auto resolve() const -> Xml *; ///< lazily resolve xml pointer via id_map

    [[nodiscard]] auto get_att() const -> AttList &;
    [[nodiscard]] auto is_font_change() const -> bool;

    void               add_attribute(const std::string &A, const std::string &B, bool force = true) const;
    void               add_attribute(const AttList &L, bool f) const;
    void               add_attribute_but_rend(Xid b) const;
    void               add_attribute(Xid b) const;
    void               add_ref(const std::string &s) const;
    void               add_span(long n) const;
    void               add_top_rule() const;
    void               add_bottom_rule() const;
    [[nodiscard]] auto has_attribute(const std::string &n) const -> std::string;
    void               add_special_att(const std::string &S, Buffer &B) const;
};

inline auto operator==(const Xid &a, const Xid &b) -> bool { return a.value == b.value; }
auto        operator<<(std::ostream &fp, Xid X) -> std::ostream &;

auto fetch_att(Xid idx, const std::string &m) -> std::optional<std::string>;
