#pragma once
#include "MathPAux.h"
#include "MathQ.h"
#include <list>

struct MathP : public std::list<MathPAux> {
    [[nodiscard]] auto analyse1(bool w) const -> bool;
    [[nodiscard]] auto find_paren_matched1() const -> bool;
    [[nodiscard]] auto has_small() const -> bool;
    auto               find_big(int &k) -> MathP;
    auto               find_paren_rec(MathQList &res) const -> bool;
    auto               find_relbin(int &k) -> MathP;
    auto               is_lbr(int &seen_d1, int &seen_d2) const -> bool;
    void               find_paren_matched2(MathQList &res) const;
    void               find_paren1(int start1, int end1, MathQList &res, bool verbose);
    void               find_paren2(int start, MathQList &res, bool verbose);
    void               remove_binrel();
};

auto operator<<(std::ostream &fp, const MathP &X) -> std::ostream &;
