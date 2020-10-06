#pragma once
#include "MathPAux.h"
#include "MathQ.h"
#include <list>

class MathP {
    std::list<MathPAux> value; // \todo inherit

public:
    friend auto        operator<<(std::ostream &fp, const MathP &X) -> std::ostream &;
    void               clear() { value.clear(); }
    [[nodiscard]] auto empty() const -> bool { return value.empty(); }
    auto               find_big(int &k) -> MathP;
    auto               is_lbr(int &seen_d1, int &seen_d2) const -> bool;
    auto               is_lbr2(int &seen_d1, int &seen_d2) const -> bool;
    auto               find_paren_rec(MathQList &res) const -> bool;
    void               find_paren2(int start, MathQList &res, bool verbose);
    void               push_back(MathPAux N) { value.push_back(N); }
    void               remove_binrel();
    [[nodiscard]] auto has_small() const -> bool;
    [[nodiscard]] auto find_paren_matched1() const -> bool;
    void               find_paren_matched2(MathQList &res) const;

private:
    [[nodiscard]] auto analyse1(bool w) const -> bool;
    auto               find_relbin(int &k) -> MathP;
    void               find_paren1(int start1, int end1, MathQList &res, bool verbose);
};
