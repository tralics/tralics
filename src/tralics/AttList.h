#pragma once
#include "Istring.h"
#include <optional>
#include <vector>

struct AttPair {
    Istring name, value;
};

struct AttList {
    std::vector<AttPair> val;

    [[nodiscard]] auto get_val(size_t i) const -> Istring { return val[i].value; }
    [[nodiscard]] auto empty() const -> bool { return val.empty(); }
    [[nodiscard]] auto has_value(const Istring &x) const -> std::optional<size_t>;

    void reset() { val = std::vector<AttPair>(); }
    void push_back_empty(Istring n);
    void push_back(name_positions n, const Istring &v);
    void push_back(name_positions n, name_positions v);
    void push_back(name_positions a, name_positions b, bool force);
    void push_back(const Istring &n, const Istring &v);
    void push_back(const Istring &a, const Istring &b, bool f);
    void delete_att(name_positions a);
    void print(std::ostream &fp) const;
};
