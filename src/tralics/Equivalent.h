#pragma once
#include "CmdChr.h"

struct Equivalent {
    CmdChr val{};
    long   level{};

    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > 1; }

    void reset() {
        val.reset();
        level = 0;
    }
};
