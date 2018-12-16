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
  int end; // type of the item
public:
  // no default Ctor, but two specialised ones 
  MathQ(int i, int t) : start(i), end(t) {}
  void get_both(int& i, int& t) const { i = start; t = end; }
  void print(ostream&fp)const { fp << start << ", " << end << "; "; }
};


// Helper for finding big small etc
class MathPAux {
  int pos; // position of the item
  math_types type; // type of the item
public:
  // no default Ctor, but two specialised ones 
  MathPAux(int i, math_types t) : pos(i), type(t) {}
  int get_pos() const { return pos; }
  math_types get_type() const { return type; }
  void get_both(int& i, math_types& t) const { i = pos; t = type;  }
  void print(ostream&) const;
  bool is_small() const  { return type==mt_flag_small_l || type == mt_flag_small_r || type == mt_flag_small_m; }
};

typedef std::list<MathPAux> MathPList;
typedef std::list<MathPAux>::const_iterator const_math_p_iterator;
typedef std::list<MathPAux>::iterator math_p_iterator;


class MathPAuxType: public unary_function<MathPAux,bool> {
  math_types T;
public:
  explicit MathPAuxType(math_types TT) : T (TT) {}
  bool operator() (const MathPAux& m) { return m.get_type() == T; }
};

class MathPAuxSmall: public unary_function<MathPAux,bool> {
public:
  explicit MathPAuxSmall() {}
  bool operator() (const MathPAux& m) { 
    return m.is_small();
  }
};

class MathP {
  MathPList value;
 public:
  friend ostream& operator<<(ostream& fp, const MathP& X);
  void clear() { value.clear(); }
  bool empty() const { return value.empty(); }
  MathP find_big(int &);
  bool is_lbr(int&,int&) const;
  bool is_lbr2(int&,int&) const;
  bool find_paren_rec(MathQList& res) const;
  void find_paren2(int start, MathQList& res,bool);
  void push_back(MathPAux N) { value.push_back(N); } 
  void remove_binrel();
  bool has_small() const;
  bool find_paren_matched1() const;
  void find_paren_matched2(MathQList& res) const;

 private:
  bool analyse1(bool) const;
  MathP find_relbin(int &);
  void find_paren1(int start1, int end1,MathQList& res,bool);
};


// Helper for finding start and end of <mrow>
class MathF {
  bool state;
  bool t_big;
  int next_change;
  int next_finish;
  MathQList aux;
  MathList res;
  Xmlp t;
public:
  MathF(const Math&,bool);
  bool in_mrow() const { return state; }
  bool is_next_change(int i) const { return i == next_change; }
  MathQList& get_aux() { return aux; }
  void change_state();
  void make_t_big() { t_big = true; }
  void handle_t();
  void push_in_t(Xmlp x);
  void push_in_res(MathElt x) { res.push_back(x); }
  void push_in_res(Xmlp x) { res.push_back(MathElt(x,-1,mt_flag_small)); }
  void reset() { t = 0; state = true; t_big = false;}
  void finish(MathList&);
  void dump_aux();
  void pop_last(Xmlp);
};

