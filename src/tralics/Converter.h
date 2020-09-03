#pragma once
#include <string>

class Converter {
public:
    std::string cur_file_name{"tty"}; // file name to be converted
    int         cur_file_line{0};     // current line number
    int         bad_chars{0};         // number of errors

    void start_convert(int l);
};

inline Converter the_converter;
