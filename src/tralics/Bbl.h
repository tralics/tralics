#pragma once
#include "LineList.h"
#include "util.h"
#include <fstream>

class Bbl : public Buffer {
private:
    std::ofstream file;

public:
    bool        too_late{false};
    std::string name;
    LineList    lines;

    void flush();
    void open() {
        if (!file.is_open()) file = open_file(name, true);
    }
};

inline Bbl bbl;
