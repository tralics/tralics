#pragma once
#include "../txid.h"
#include "Buffer.h"
#include "Istring.h"

struct CitationItem {
    Istring key, from, id;
    Xid     solved;

    CitationItem(Istring A, Istring B) : key(std::move(A)), from(std::move(B)), id("") {}

    void dump(Buffer &b) const;

    [[nodiscard]] auto is_solved() const -> bool { return solved.value != 0; }
    [[nodiscard]] auto match(const Istring &A, const Istring &B) const -> bool;
    [[nodiscard]] auto match_star(const Istring &A) const -> bool;

    auto get_id() -> Istring;
    void dump_bibtex();
};
