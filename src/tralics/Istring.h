#pragma once
#include "StrHash.h"
#include "enums.h"
#include "util.h"
#include <array>

class Buffer;
class ScaledInt;

struct Istring {
    std::string name;
    size_t      id{0};

    Istring(size_t N = 0) : name(SH[N].name), id(N) {}
    explicit Istring(const std::string &s);
    explicit Istring(const ScaledInt &i);

    [[nodiscard]] auto null() const -> bool { return id == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return id == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return id == 2; } // "" \todo investigate because 2 is `" "`
    [[nodiscard]] auto value() const -> std::string & { return SH[id].value; }
    [[nodiscard]] auto labinfo() const { return SH.labinfo(id); }

    auto operator==(const Istring &X) const -> bool { return id == X.id; }
    auto operator!=(const Istring &X) const -> bool { return id != X.id; }

    static inline StrHash SH;
};

auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream &;

inline std::array<Istring, np_last> the_names; // \todo static in Istring and make is a map from std::string
