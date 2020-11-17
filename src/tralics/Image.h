#pragma once
#include "Buffer.h"

struct Image {
    std::string name;     // name
    int         occ{0};   // number of occurences
    int         flags{0}; // existence flags

    Image() = default;
    Image(std::string N, int oc_val) : name(std::move(N)), occ(oc_val) {}

    void check_existence();
    void check(Buffer &B1, Buffer &B2) const;
};

auto operator<<(std::ostream &X, const Image &Y) -> std::ostream &;
