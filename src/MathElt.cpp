#include "tralics/MathElt.h"
#include "tralics/Buffer.h"
#include "tralics/util.h"
#include "txmath.h"
#include <fmt/ostream.h>

// This prints a math element
void MathElt::print() const {
    if (cmd == nomath_cmd) {
        Trace.format("only for {}\n", chr == zero_code ? "math" : "nomath");
        return;
    }
    Trace.format("ME {} - ", cmd);
    if (32 < chr && chr < 128)
        Trace.format("char {}", uchar(chr));
    else
        Trace.format("{}", chr);
    // is this secure ???
    //  if(cmd>16) Trace << " - " <<  Token(get_font());
    if (cmd == mathfont_cmd || is_m_font(symcodes(cmd)))
        Trace.format(" - {}\n", Token(get_font()));
    else
        Trace.format(" - {}\n", get_font());
    if (cmd == math_list_cmd || cmd == special_math_cmd) get_list().print(); // recurse
}
