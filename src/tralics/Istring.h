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

    operator bool() const { return id != 0; }

    [[nodiscard]] auto labinfo() const -> LabelInfo *;
};
