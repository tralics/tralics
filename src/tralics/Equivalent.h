#pragma once
#include "CmdChr.h"

struct Equivalent : public CmdChr {
    int level{0}; ///< level at which this is defined

    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > 1; }
    void               reset() {
        CmdChr::reset();
        level = 0;
    }

    void setnl(CmdChr c) { CmdChr::operator=(c); }

    void special_prim(CmdChr b) {
        setnl(b);
        if (!is_undef()) level = 1;
    }

    void set(CmdChr c, int lvl) {
        setnl(c);
        level = lvl;
    }

    void primitive(CmdChr b) { set(b, 1); }
};
