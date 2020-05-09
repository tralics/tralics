#include "tralics/Clines.h"
#include "txio.h"

// This converts a line of a file
void Clines::convert_line(size_t wc) {
    Buffer B;
    B.push_back(*this);
    converted = true;
    the_converter.start_convert(number);
    if (B.convert_line0(wc)) return;
    the_converter.lines_converted++;
    std::string::operator=(utf8_out.to_string());
}
