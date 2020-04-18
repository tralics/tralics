#pragma once
#include "StrHash.h"
#include "enums.h"
#include "util.h"
#include <array>

class Buffer;
class ScaledInt;

struct Istring {
    size_t id{0};

    Istring(size_t N = 0) : id(N) {}
    explicit Istring(String s);
    explicit Istring(const Buffer &X);
    explicit Istring(const std::string &s);
    explicit Istring(const ScaledInt &i);

    [[nodiscard]] auto null() const -> bool { return id == 0; }       // null string
    [[nodiscard]] auto empty() const -> bool { return id == 1; }      // ""
    [[nodiscard]] auto spec_empty() const -> bool { return id == 2; } // ""
    [[nodiscard]] auto c_str() const -> String { return SH[id].name; }
    [[nodiscard]] auto p_str() const -> std::string & { return SH[id].value; }
    [[nodiscard]] auto labinfo() const { return SH.labinfo(id); }

    auto operator==(Istring X) const -> bool { return id == X.id; }
    auto operator!=(Istring X) const -> bool { return id != X.id; }

    static inline StrHash SH;
};

extern std::array<Istring, np_last> the_names;
