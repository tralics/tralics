#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

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

#include "txarray.h"
#include "txatt.h"
#include "txstack.h"
#include "txtokenlist.h"

// Class used to implement array.sty
class NewArray {
    bool               nct_bool[nb_newcolumn]; // for \newcolumntype
    Token              nct_tok[nb_newcolumn];  // for \newcolumntype
    int                nct_size;               // number of slots in the nct table
    array_class_type   ch_class, last_ch_class;
    array_class_number ch_num; // for array management
    TokenList          preamble;
    bool               have_token_list;
    Token              current_token;
    TokenList          current_list;
    Parser *           P;
    ArrayInfo *        AI;
    Xid                id;
    TokenList          u_list, v_list;
    AttList            attribs;
    int                ac_cell_no;
    bool               first_bar;
    name_positions     cur_h_pos;

public:
    auto               dump_slot() -> String;
    void               boot(Parser *);
    void               add_a_type(uchar c, Token t);
    void               remove_a_type(uchar c);
    auto               nct_exists(uchar c) -> bool { return nct_bool[c]; }
    auto               nct_token(uchar c) -> Token { return nct_tok[c]; }
    [[nodiscard]] auto has_a_nct() const -> bool { return nct_size > 0; }
    void               test_pach();
    void               ac_maybe_finish();
    void               ac_maybe_finish_multi(bool &);
    void               ac_pre_cell();
    void               ac_pre_inter();
    void               ac_inter();
    void               run(Xid, bool);
    auto               ac_next() -> bool;
};
