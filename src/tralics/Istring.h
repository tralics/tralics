#pragma once
#include <string>

class LabelInfo;

struct Istring : public std::string {
    size_t id{0};

    Istring() = default;
    Istring(const std::string &s);
    explicit Istring(size_t N);

    operator bool() const { return id != 0; }

    [[nodiscard]] auto labinfo() const -> LabelInfo *;
};

inline Istring hlinee_above, hlinee_width, hlinee_below;
