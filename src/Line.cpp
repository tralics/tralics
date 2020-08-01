#include "tralics/Line.h"
#include "tralics/Converter.h"
#include "txio.h"

// This converts a line of a file
void Line::convert_line(size_t wc) {
    Buffer B;
    B.append(*this);
    converted = true;
    the_converter.start_convert(number);
    auto [o, s] = B.convert_line0(wc);
    if (o) return;
    the_converter.lines_converted++;
    std::string::operator=(s);
}
