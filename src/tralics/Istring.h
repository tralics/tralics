#pragma once
#include "enums.h"
#include <array>
#include <string>

class LabelInfo;

struct Istring {
    size_t      id{0};
    std::string name;

    Istring() = default;
    explicit Istring(size_t N);
    explicit Istring(const std::string &s);

    [[nodiscard]] auto null() const -> bool { return id == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return id == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return id == 2; } // "" \todo investigate because 2 is `" "`
    [[nodiscard]] auto labinfo() const -> LabelInfo *;

    auto operator==(const Istring &X) const -> bool { return id == X.id; }
    auto operator!=(const Istring &X) const -> bool { return id != X.id; }
};

inline auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream & { return fp << L.name; }
