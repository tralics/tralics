// -*- C++ -*-
// $Id: txfp.h,v 2.3 2008/02/21 17:45:58 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// for the fp package. this implements fixed point arithmetic
class FpNum {
 public:
  bool sign; // sign of the number
  unsigned int data[4]; // the digits

  FpNum(Digit a, Digit b, Digit c, Digit d)
    { data[0]=a; data[1]=b; data[2] = c; data[3] =d; sign=true; } 
  FpNum(): sign(true) {data[0]=data[1]=data[2]=data[3]=0;}
  void init(Digit a, Digit b, Digit c, Digit d)
    { data[0]=a; data[1]=b; data[2] = c; data[3] =d;} 
  bool is_zero() const ;
  void reset() { data[0] = data[1] = data[2] = data[3] = 0; }
  void correct_sign();
  bool create(Buffer&);
  TokenList to_list() const;
  void add(FpNum);
  void set_xmax(Digit* x, int & xmin, int&xmax);
  void prop_carry(Digit* z);
  void finish_mul(bool xs, Digit*z);
  void unsplit_mul4(const Digit*);
  void add_int(FpNum);
  void div(int n);
  void sin0();
  void cos0();
  void sincos_loop(int n, FpNum x, FpNum p);
  void add_abs(FpNum);
  void sub_abs(FpNum);
  void neg_sub_abs(FpNum);
  void mod2pi();
  int count_times(FpNum);
  String to_string() const;
  void neg();
  void mul_split(unsigned int*) const;
  void mul_by_10();
  void mul_by_2();
  void mul(FpNum X);
  void div(FpNum X);
  void div_by_10();
  void add_digit(int);
  void sgn();
  void truncate(int);
  void round(int);
  void round0();
  void exec_ln();
  void exec_ln_a();
  void exec_exp();
  bool large_exp();
  void mul(FpNum,FpNum);
  void mul(FpNum,int);
  void div(FpNum,FpNum);
  void pow(FpNum,FpNum,subtypes);
  void divide2();
  int octand();
  void sin1(int);
  void cos1(int);
  void mean(FpNum a, FpNum b);
  bool sincos_transform();
  bool arcsincos_loop();
  void trigo_xy(int);
  void trigo_inv();
  void trigo_sqrt();
  void arctan();
  void arctan1();
  void sqrt();
  void croot();
  void sub(FpNum,FpNum);
  void add(FpNum,FpNum);
  void cosine();
  void random();
};
