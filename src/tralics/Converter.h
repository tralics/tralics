#pragma once
#include <string>

class Converter {
public:
    std::string cur_file_name{"tty"}; // file name to be converted
    int         cur_file_line{0};     // current line number
    bool        global_error{false};  // Is this line OK?
    int         bad_lines{0};         // number of lines with errors
    int         bad_chars{0};         // number of errors

    auto new_error() -> bool;
    void start_convert(int l);
};

inline Converter the_converter;
