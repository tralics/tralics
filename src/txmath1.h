#pragma once
// -*- C++ -*-
// TRALICS, copyright (C)INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Helper for finding start and end of <mrow>
class MathQ {
    int start; // position of the item
    int end;   // type of the item
public:
    // no default Ctor, but two specialised ones
    MathQ(int i, int t) : start(i), end(t) {}
    void get_both(int &i, int &t) const {
        i = start;
        t = end;
    }
    void print(std::ostream &fp) const { fp << start << ", " << end << "; "; }
};

// Helper for finding big small etc
class MathPAux {
    int        pos;  // position of the item
    math_types type; // type of the item
public:
    // no default Ctor, but two specialised ones
    MathPAux(int i, math_types t) : pos(i), type(t) {}
    [[nodiscard]] auto get_pos() const -> int { return pos; }
    [[nodiscard]] auto get_type() const -> math_types { return type; }
    void               get_both(int &i, math_types &t) const {
        i = pos;
        t = type;
    }
    void               print(std::ostream &fp) const;
    [[nodiscard]] auto is_small() const -> bool { return type == mt_flag_small_l || type == mt_flag_small_r || type == mt_flag_small_m; }
};

using MathPList             = std::list<MathPAux>;
using const_math_p_iterator = std::list<MathPAux>::const_iterator;
using math_p_iterator       = std::list<MathPAux>::iterator;

class MathP {
    MathPList value;

public:
    friend auto        operator<<(std::ostream &fp, const MathP &X) -> std::ostream &;
    void               clear() { value.clear(); }
    [[nodiscard]] auto empty() const -> bool { return value.empty(); }
    auto               find_big(int &k) -> MathP;
    auto               is_lbr(int &seen_d1, int &seen_d2) const -> bool;
    auto               is_lbr2(int &seen_d1, int &seen_d2) const -> bool;
    auto               find_paren_rec(MathQList &res) const -> bool;
    void               find_paren2(int start, MathQList &res, bool verbose);
    void               push_back(MathPAux N) { value.push_back(N); }
    void               remove_binrel();
    [[nodiscard]] auto has_small() const -> bool;
    [[nodiscard]] auto find_paren_matched1() const -> bool;
    void               find_paren_matched2(MathQList &res) const;

private:
    [[nodiscard]] auto analyse1(bool w) const -> bool;
    auto               find_relbin(int &k) -> MathP;
    void               find_paren1(int start1, int end1, MathQList &res, bool verbose);
};

// Helper for finding start and end of <mrow>
class MathF {
    bool      state{true};
    bool      t_big;
    int       next_change{-1};
    int       next_finish{-1};
    MathQList aux;
    MathList  res;
    Xml *     t;

public:
    MathF();
    [[nodiscard]] auto in_mrow() const -> bool { return state; }
    [[nodiscard]] auto is_next_change(int i) const -> bool { return i == next_change; }
    auto               get_aux() -> MathQList & { return aux; }
    void               change_state();
    void               make_t_big() { t_big = true; }
    void               handle_t();
    void               push_in_t(Xml *x);
    void               push_in_res(MathElt x) { res.push_back(x); }
    void               push_in_res(Xml *x) { res.push_back(MathElt(x, -1, mt_flag_small)); }
    void               reset() {
        t     = nullptr;
        state = true;
        t_big = false;
    }
    void finish(MathList &value);
    void dump_aux();
    void pop_last(Xml *xval);
};
