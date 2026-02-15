#pragma once
#include "LineList.h"
#include "util.h"
#include <fstream>
#include <string>

class Bbl {
private:
    std::ofstream file;

public:
    bool        too_late{false};
    std::string name;
    LineList    lines;
    std::string buffer;

    void flush();
    void open() {
        if (!file.is_open()) file = open_file(name, true);
    }
};

inline Bbl bbl;
