#pragma once
#include "enums.h"
#include <array>
#include <string>

class LabelInfo;

struct Istring : public std::string {
    size_t id{0};

    Istring() = default;
    explicit Istring(size_t N);
    explicit Istring(const std::string &s);

    [[nodiscard]] auto null() const -> bool { return id == 0; }  // null string \todo operator bool
    [[nodiscard]] auto empty() const -> bool { return id == 1; } // ""
    [[nodiscard]] auto labinfo() const -> LabelInfo *;

    auto operator==(const Istring &X) const -> bool { return id == X.id; }
    auto operator!=(const Istring &X) const -> bool { return id != X.id; }
};
