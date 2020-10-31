#include "tralics/ScaledInt.h"
#include "tralics/Buffer.h"

ScaledInt::operator std::string() const {
    Buffer B;
    B.push_back(*this, glue_spec_pt);
    return std::move(B);
}

auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream & {
    Buffer B;
    B.push_back(x, glue_spec_pt);
    return fp << B;
}
