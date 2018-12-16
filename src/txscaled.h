// -*- C++ -*-
// $Id: txscaled.h,v 2.14 2008/11/12 09:43:57 grimm Exp $
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This defines the classes: ScaledInt, Glue, RealNumber,
// SthInternal ScanSlot TexRule

// this is a wrapper around an int
class ScaledInt {
  int value; // the integer, considered as a scaled number
 public:
  ScaledInt() : value(0) {}
  void set_value(int i) { value = i; }
  ScaledInt (int v): value (v) {}
  int get_value() const { return value; }
  ScaledInt operator-()const { return ScaledInt(-value); }
  void operator += (ScaledInt X) {value += X.value; }
  void add_dim(ScaledInt x);
  bool null() const { return value == 0; }
  void neg() { value = - value; }
  void divide(int);
  void quotient(int);
  void scale(int,int,int);
  bool scale(int,int,int,int,bool&);
  void mult_scaled(int);
  void mult_integer(int);
  void times_10_18();
  void times_18_10();
  void ovf30();
  void ovf31();
};

inline bool operator==(const ScaledInt& a, const ScaledInt&b)
{
  return a.get_value() == b.get_value();
}


// a glue like \hskip=2.3pt plus 4.5pt minus 6.7fill
// the three ints should be ScaledInt.
class Glue {
  ScaledInt width; // natural width (2.3)
  ScaledInt shrink; // shrink (6.7)
  ScaledInt stretch; // stretch (4.5)
  glue_spec shrink_order; // fill, symbolically
  glue_spec stretch_order; // pt, symbolically
 public:
  Glue(): shrink_order(glue_spec_pt), stretch_order(glue_spec_pt) {}
  ScaledInt get_width()const  { return width; }
  ScaledInt get_shrink()const { return shrink; }
  ScaledInt get_stretch()const { return stretch; }
  glue_spec get_shrink_order()const { return shrink_order; }
  glue_spec get_stretch_order()const { return stretch_order; }
  void set_width(ScaledInt x)  { width = x; }
  void set_shrink(ScaledInt x) { shrink = x; }
  void set_stretch(ScaledInt x) { stretch = x; }
  void set_shrink_order(glue_spec x) { shrink_order = x; }
  void set_stretch_order(glue_spec x) { stretch_order = x; }
  void kill();
  void normalise();
  void negate();
  void add(const Glue &x);
  void multiply(int v);
  void divide(int v);
  void incr_width(int x) { width += x; }
  void scale(int n, int f);
  void quotient(int f);
  void check_overflow();
  void expr_mul(int f);
  void add_ovf(const Glue&);
  void zdv();
};


// The value of the number is sign*(i+f/2^16);
class RealNumber {
  bool negative; // true if negative
  int ipart;     // fractional part
  int fpart;     // integer part, is <2^16
 public:
  void convert_decimal_part(int k, int*table);
  void set_ipart(int x) { ipart = x; }
  void set_fpart(int x) { fpart = x; }
  int get_ipart()const  { return ipart; }
  int get_fpart()const  { return fpart; }
  void change_sign() { negative = !negative; }
  void set_negative(bool x) { negative = x; }
  RealNumber() : negative(false), ipart(0), fpart(0) {}
  void set_neg() { negative = true ; }
  void change_sign_i() { ipart = -ipart; }
  bool get_negative() { return negative; };
  void from_int(int x);
};

// This is a union of different things
// it contains the result of scan_something_internal
class SthInternal {
  Glue glue_val;  // value if it is a glue
  ScaledInt int_val;    // value if it is a dimension or an integer
  TokenList token_val; // value if it is a token list
  internal_type type; // this says what the object is.
 public:
  SthInternal(): int_val(0), type(it_int) {}
  bool is_int()const { return type == it_int; }
  bool is_mu()const { return type == it_mu; }
  bool is_glue()const { return type == it_glue; }
  bool is_dimen()const { return type == it_dimen; }
  internal_type get_type()const { return type; }
  void kill() { int_val = 0; glue_val.kill(); type=it_int; }
  void set_type(internal_type X) { type=X; }
  int get_int_val()const { return int_val.get_value(); } 
  ScaledInt get_dim_val()const { return int_val; } 
  ScaledInt& get_scaled() { return int_val; } 
  TokenList get_token_val()const { return token_val; } 
  void set_int_val(int k) { int_val = k; } 
  void set_scaled_val(ScaledInt k) { int_val = k; } 
  const Glue& get_glue_val()const { return glue_val; } 
  int get_glue_width()const { return glue_val.get_width().get_value(); } 
  void initialise(internal_type t);
  void copy(const SthInternal& x);
  void negate(){
    if(type==it_glue||type==it_mu) glue_val.negate(); else int_val=-int_val;
  }
  void fast_negate() { int_val=-int_val; }
  void attach_fraction(RealNumber f);
  void attach_sign(bool negative);
  void set_int(int a) { int_val = a; type = it_int;}
  void set_dim(int a) { int_val = a; type = it_dimen;}
  void set_dim(ScaledInt a) { int_val = a.get_value(); type = it_dimen;}
  void set_glue(Glue a) { glue_val = a; type = it_glue;}
  void set_mu(Glue a) { glue_val = a; type = it_mu;}
  void set_toks(TokenList a) { token_val = a; type = it_tok;}
  void change_level(internal_type);
  void glue_to_mu(){if(type>=it_glue) int_val=glue_val.get_width().get_value();}
  void add(const SthInternal&);
  void incr_int(int x) { int_val += x; }
  void incr_dim(ScaledInt x) { int_val += x.get_value(); }
  void incr_glue(Glue x) { glue_val.add(x); }
  void glue_multiply(int v) { glue_val.multiply(v); }
  void glue_divide(int v) { glue_val.divide(v);  } 
  void set_glue_val(Glue x) { glue_val = x; }
  void cv_dim_to_mu();
  void cv_mu_to_glue();
  void cv_glue_to_mu();
  void get_info(subtypes);
  void scale(int,int);
  void quotient(int);
  void check_overflow(scan_expr_t t);
  void normalise();
  void expr_mul(int f);
  void add_ovf(const SthInternal &r);
};


class ScanSlot
{
public:
  internal_type expr_type;
  SthInternal expr_so_far;
  SthInternal term_so_far;  
  scan_expr_t expr_state;
  scan_expr_t term_state;
  int numerator;
public:
  ScanSlot(internal_type L, SthInternal E,SthInternal T, 
	    scan_expr_t R,scan_expr_t S, int N): 
    expr_type(L), expr_so_far(E),term_so_far(T),expr_state(R), term_state(S),
    numerator(N) {}
  ScanSlot(): expr_type(it_int), expr_state(se_none), term_state(se_none),
    numerator(0){}
  internal_type get_next_type() const 
  { return term_state==se_none ? expr_type: it_int; }
  void kill();
  void compute_term(scan_expr_t& next_state, SthInternal f,char&);
  void add_or_sub(scan_expr_t& next_state, SthInternal f,char&);
};


namespace arith_ns {
  int nx_plus_y(int n, int x, int y);
  ScaledInt n_times_x(int n, ScaledInt x);
  void scaled_div(ScaledInt& x, int n);
  int xn_over_d(int x, int n, int d, int& remainder);
  int quotient(int n, int d);
  int add_ovf(ScaledInt x, ScaledInt y);
};

 class TexRule {
public:
  ScaledInt rule_w, rule_h,rule_d;
  void reset();
  void convert(AttList&);
  void init_vrule() { rule_w = 26214; }
  void init_hrule() { rule_h = 26214; rule_d = 0; }
  void adjust() { rule_h += rule_d; rule_d.neg(); }
};
