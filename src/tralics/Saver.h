#pragma once
#include "../txparser.h"

template <typename T> class Saver {
    T &target;
    T  saved;

public:
    Saver(T &t, T nt) : target(t), saved(t) { t = nt; }
    ~Saver() { target = saved; }
};

class SaveCatcode : public Saver<long> {
public:
    SaveCatcode(char32_t c, long nc) : Saver<long>(the_parser.eqtb_int_table[c].val, nc) {}
};

class SaveScannerStatus : public Saver<scan_stat> {
public:
    SaveScannerStatus(scan_stat c) : Saver<scan_stat>(the_parser.scanner_status, c) {}
};

class SaveLongState : public Saver<l_state> {
public:
    SaveLongState(l_state c) : Saver<l_state>(the_parser.long_state, c) {}
};
