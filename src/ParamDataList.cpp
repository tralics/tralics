#include "tralics/Buffer.h"
#include "tralics/ConfigData.h"

namespace config_ns {
    auto start_interpret(Buffer &B, std::string_view s) -> bool;
} // namespace config_ns

void ParamDataList::interpret(const std::string &b) {
    Buffer B(b);
    if (config_ns::start_interpret(B, "//")) clear();
    for (;;) {
        auto r1 = B.slash_separated();
        if (!r1) return;
        auto r2 = B.slash_separated();
        if (!r2) return;
        if (r1->empty()) continue;
        if (r2->empty()) r2 = r1;
        (*this)[*r1] = *r2;
    }
}
