#include "tralics/Converter.h"

// If an error is signaled on current line, we do not signal again
// We mark current char as invalid
auto Converter::new_error() -> bool {
    if (global_error) return true;
    bad_lines++;
    global_error = true;
    return false;
}

// Action when starting conversion of line l of current file
void Converter::start_convert(int l) {
    cur_file_line = l;
    global_error  = false;
}
