#pragma once
#include "Parser.h"
#include "globals.h"

template <typename T> class Saver {
    T &target;
    T  saved;

public:
    Saver(T &t, T nt) : target(t), saved(t) { t = nt; }
    ~Saver() { target = saved; }
};

inline auto InLoadHandler() { return Saver(global_in_load, true); }
inline auto InUrlHandler() { return Saver(global_in_url, true); }
inline auto SaveCatcode(char32_t c, long nc) { return Saver(the_parser.eqtb_int_table[c].val, nc); }
inline auto SaveErrTok(Token t) { return Saver(the_parser.err_tok, t); }
inline auto SaveLongState(l_state c) { return Saver(the_parser.long_state, c); }
inline auto SaveScannerStatus(scan_stat c) { return Saver(the_parser.scanner_status, c); }
