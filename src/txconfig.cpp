#include "tralics/Buffer.h"

namespace config_ns {
    // We add a final slash, or double slash, this makes parsing easier;
    // We also remove an initial slash (This is not done if the separator is a
    // space, case where s is empty).
    // An initial plus sign means: append the line to the vector, else reset.
    auto start_interpret(Buffer &B, std::string_view s) -> bool {
        bool ret_val = false;
        B.append(s);
        B.ptrs.b = 0;
        if (B.head() == '+')
            B.advance();
        else
            ret_val = true;
        if ((!s.empty()) && B.head() == '/') B.advance();
        return ret_val;
    }
} // namespace config_ns
