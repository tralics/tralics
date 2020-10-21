#include "tralics/Math.h"

auto Math::has_two_elements() const -> bool {
    auto X = begin();
    if (X == end()) return false;
    ++X;
    return X != end();
}

auto Math::has_one_element() const -> bool {
    auto X = begin();
    if (X == end()) return false;
    ++X;
    return X == end();
}

auto Math::second_element() const -> const MathElt & {
    auto X = begin();
    ++X;
    return *X;
}

auto Math::third_element() const -> const MathElt & {
    auto X = begin();
    ++X;
    ++X;
    return *X;
}
