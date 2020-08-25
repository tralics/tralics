#pragma once
#include "../txinline.h"
#include "LineList.h"
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
        if (!file.is_open()) file = tralics_ns::open_file(name, true);
    }
};

inline Bbl bbl;
