#pragma once
#include "types.h"
#include <string>

struct AttList;
class Buffer;

class Xid { // \todo this is just a size_t with tagged methods
public:
    size_t value;

    Xid(size_t v = 0) : value(v) {}

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
    void               add_special_att(const std::string &S, Buffer &B);
};

inline auto operator==(const Xid &a, const Xid &b) -> bool { return a.value == b.value; }
auto        operator<<(std::ostream &fp, Xid X) -> std::ostream &;
