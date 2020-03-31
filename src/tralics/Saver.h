#pragma once
#include "../txparser.h"

template <typename T> class Saver {
    T &target;
    T  saved;

public:
    Saver(T &t, T nt) : target(t), saved(t) { t = nt; }
    ~Saver() { target = saved; }

    Saver(const Saver &S) = delete;
    Saver(Saver &&S)      = delete;
    auto operator=(const Saver &S) = delete;
    auto operator=(Saver &&S) = delete;
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
