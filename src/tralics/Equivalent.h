#pragma once
#include "CmdChr.h"

struct Equivalent : public CmdChr {
    long level{0}; ///< level at which this is defined

    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > 1; }

    void reset() {
        CmdChr::reset();
        level = 0;
    }
};
