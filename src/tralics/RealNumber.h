#pragma once
#include <stddef.h>

// The value of the number is sign*(i+f/2^16);

struct RealNumber {
    bool negative{false}; // true if negative
    long ipart{0};        // integer part
    long fpart{0};        // fractional part, is <2^16

    void convert_decimal_part(size_t k, const long *table);
    void from_int(long x);
};
