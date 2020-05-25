#pragma once
#include "Istring.h"
#include <optional>
#include <vector>

struct AttPair {
    Istring name, value;
};

struct AttList : public std::vector<AttPair> {
    [[nodiscard]] auto get_val(size_t i) const -> Istring { return at(i).value; }
    [[nodiscard]] auto lookup(const Istring &x) const -> std::optional<size_t>;

    using std::vector<AttPair>::push_back;
    void push_back(const Istring &name, const Istring &value, bool force = true);
};

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream &;
