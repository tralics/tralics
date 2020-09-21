#pragma once
#include "AttList.h"
#include "TokenList.h"
#include "Xid.h"

// Token classes and numbers for preamble tokens.
// c     0   0    ---    Start 4  -
// l     0   1           @-arg 5  -
// r     0   2           !     6  -
// m-arg 0   3           @     7
// p-arg 0   4           <     8
// b-arg 0   5           >     9
// |     1   0           m    10 3
// !-arg 1   1           p    10 4
// <-arg 2   -           b    10 5
// >-arg 3   -

class Parser;
class ArrayInfo;

// Class used to implement array.sty
class NewArray {
    std::array<bool, nb_newcolumn>  nct_bool{}; // for \newcolumntype
    std::array<Token, nb_newcolumn> nct_tok;    // for \newcolumntype
    int                             nct_size{}; // number of slots in the nct table
    array_class_type                ch_class{}, last_ch_class{};
    array_class_number              ch_num{}; // for array management
    TokenList                       preamble;
    bool                            have_token_list{};
    Token                           current_token;
    TokenList                       current_list;
    Parser *                        P{};
    ArrayInfo *                     AI{};
    Xid                             id;
    TokenList                       u_list, v_list;
    AttList                         attribs;
    int                             ac_cell_no{};
    bool                            first_bar{};
    std::string                     cur_h_pos;

public:
    [[nodiscard]] auto has_a_nct() const -> bool { return nct_size > 0; }

    auto dump_slot() -> String;
    void boot(Parser *Q);
    void add_a_type(uchar c, Token t);
    void remove_a_type(uchar c);
    auto nct_exists(uchar c) -> bool { return nct_bool[c]; }
    auto nct_token(uchar c) -> Token { return nct_tok[c]; }
    void test_pach();
    void ac_maybe_finish();
    void ac_maybe_finish_multi(bool &seen);
    void run(Xid /*ID*/, bool main_fct);
    auto ac_next() -> bool;
};

inline NewArray new_array_object;
