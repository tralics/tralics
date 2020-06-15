#pragma once
#include "Buffer.h"

struct Image {
    std::string name;     // name
    int         occ{0};   // number of occurences
    int         flags{0}; // existence flags

    Image() = default;
    Image(std::string N, int oc_val) : name(std::move(N)), occ(oc_val) {}

    [[nodiscard]] auto file_exists(const std::string &s) const -> bool;
    void               check_existence();
    void               check(Buffer &B1, Buffer &B2) const;
};
