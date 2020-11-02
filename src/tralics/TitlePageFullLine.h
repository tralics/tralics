#pragma once
#include "TpiOneItem.h"

// temporary class, will bew copied into a TitlePageAux \todo do
struct TitlePageFullLine {
    TpiOneItem item1, item2, item3, item4; // the four items
    size_t     flags{};                    // the flags

    auto read() -> int;
    auto classify(int w, int state) -> tpi_vals;
    auto encode_flags(char c1, char c2) -> bool;
};
