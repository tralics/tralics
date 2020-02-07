// -*- C++ -*-
// $Id: txmath1.h,v 2.8 2008/10/28 15:41:59 grimm Exp $
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
    void print(ostream &fp) const { fp << start << ", " << end << "; "; }
};

// Helper for finding big small etc
class MathPAux {
    int        pos;  // position of the item
    math_types type; // type of the item
public:
    // no default Ctor, but two specialised ones
    MathPAux(int i, math_types t) : pos(i), type(t) {}
    auto get_pos() const -> int { return pos; }
    auto get_type() const -> math_types { return type; }
    void get_both(int &i, math_types &t) const {
        i = pos;
        t = type;
    }
    void print(ostream &) const;
    auto is_small() const -> bool { return type == mt_flag_small_l || type == mt_flag_small_r || type == mt_flag_small_m; }
};

using MathPList             = std::list<MathPAux>;
using const_math_p_iterator = std::list<MathPAux>::const_iterator;
using math_p_iterator       = std::list<MathPAux>::iterator;

class MathPAuxType : public unary_function<MathPAux, bool> {
    math_types T;

public:
    explicit MathPAuxType(math_types TT) : T(TT) {}
    auto operator()(const MathPAux &m) -> bool { return m.get_type() == T; }
};

class MathPAuxSmall : public unary_function<MathPAux, bool> {
public:
    explicit MathPAuxSmall() {}
    auto operator()(const MathPAux &m) -> bool { return m.is_small(); }
};

class MathP {
    MathPList value;

public:
    friend auto operator<<(ostream &fp, const MathP &X) -> ostream &;
    void        clear() { value.clear(); }
    auto        empty() const -> bool { return value.empty(); }
    auto        find_big(int &) -> MathP;
    auto        is_lbr(int &, int &) const -> bool;
    auto        is_lbr2(int &, int &) const -> bool;
    auto        find_paren_rec(MathQList &res) const -> bool;
    void        find_paren2(int start, MathQList &res, bool);
    void        push_back(MathPAux N) { value.push_back(N); }
    void        remove_binrel();
    auto        has_small() const -> bool;
    auto        find_paren_matched1() const -> bool;
    void        find_paren_matched2(MathQList &res) const;

private:
    auto analyse1(bool) const -> bool;
    auto find_relbin(int &) -> MathP;
    void find_paren1(int start1, int end1, MathQList &res, bool);
};

// Helper for finding start and end of <mrow>
class MathF {
    bool      state;
    bool      t_big;
    int       next_change;
    int       next_finish;
    MathQList aux;
    MathList  res;
    Xmlp      t;

public:
    MathF(const Math &, bool);
    auto in_mrow() const -> bool { return state; }
    auto is_next_change(int i) const -> bool { return i == next_change; }
    auto get_aux() -> MathQList & { return aux; }
    void change_state();
    void make_t_big() { t_big = true; }
    void handle_t();
    void push_in_t(Xmlp x);
    void push_in_res(MathElt x) { res.push_back(x); }
    void push_in_res(Xmlp x) { res.push_back(MathElt(x, -1, mt_flag_small)); }
    void reset() {
        t     = 0;
        state = true;
        t_big = false;
    }
    void finish(MathList &);
    void dump_aux();
    void pop_last(Xmlp);
};
