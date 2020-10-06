#pragma once
#include <list>
#include <ostream>

using MathQ     = std::pair<int, int>;
using MathQList = std::list<MathQ>;

inline auto operator<<(std::ostream &os, const MathQ &m) -> std::ostream & { return os << m.first << ", " << m.second << "; "; }

inline auto operator<<(std::ostream &fp, const MathQList &X) -> std::ostream & {
    for (const auto &q : X) fp << q;
    return fp;
}
