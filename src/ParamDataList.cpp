#include "tralics/Buffer.h"
#include "tralics/ConfigData.h"

namespace config_ns {
    auto start_interpret(Buffer &B, String s) -> bool;
} // namespace config_ns

void ParamDataList::interpret(const std::string &b) {
    Buffer B(b);
    if (config_ns::start_interpret(B, "//")) clear();
    for (;;) {
        std::string r1, r2;
        if (!B.slash_separated(r1)) return;
        if (!B.slash_separated(r2)) return;
        if (r1.empty()) continue;
        if (r2.empty()) r2 = r1;
        (*this)[r1] = r2;
    }
}
