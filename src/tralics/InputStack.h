#pragma once
#include "LineList.h"
#include "enums.h"

struct InputStack {
    std::vector<char32_t> line;
    states                state;     // copy of scanner state
    LineList              L;         // the lines
    int                   line_no;   // the current line number
    TokenList             TL;        // saved token list
    std::string           name;      // name of the current file
    long                  at_val;    // catcode of @ to restore
    long                  file_pos;  // file position to restore
    size_t                line_pos;  // position in B
    bool                  every_eof; // True if every_eof_token can be inserted
    bool                  eof_outer; // True if eof is outer

    InputStack(std::string N, int l, states S, long cfp, bool eof, bool eof_o)
        : state(S), line_no(l), name(std::move(N)), at_val(-1), file_pos(cfp), line_pos(0), every_eof(eof), eof_outer(eof_o) {}

    void destroy() {
        TL.clear();
        line.clear();
        L.clear();
    };

    void set_line_ptr(LineList &X) {
        L.clear_and_copy(X);
        X.file_name = "";
        L.set_interactive(X.interactive);
    }

    void get_line_ptr(LineList &X) {
        X.clear_and_copy(L);
        X.set_interactive(L.interactive);
    }
};
