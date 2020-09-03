#include "tralics/Line.h"
#include "tralics/Converter.h"
#include "tralics/util.h"

// This converts a line of a file
void Line::convert_line(size_t wc) {
    converted = true;
    the_converter.start_convert(number);
    if (wc != 0) std::string::operator=(convert_to_utf8(*this, wc));
}
