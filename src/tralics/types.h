#pragma once
#include <cassert>
#include <cstddef>
#include <type_traits>

using uchar  = unsigned char;
using Digit = size_t;

template <typename T> auto to_signed(T x) -> std::enable_if_t<std::is_unsigned_v<T>, std::make_signed_t<T>> {
    return static_cast<std::make_signed_t<T>>(x);
}

template <typename T> auto to_unsigned(T x) -> std::enable_if_t<std::is_signed_v<T>, std::make_unsigned_t<T>> {
    assert(x >= 0); // NOLINT
    return static_cast<std::make_unsigned_t<T>>(x);
}
