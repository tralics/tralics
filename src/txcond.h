// -*- C++ -*-
// $Id: txcond.h,v 2.3 2008/02/21 17:45:58 grimm Exp $
// Copyright INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

class CondAux {
  int if_limit; // specifies the largest code of a fi_or_else command
  int cur_if; // is the name of the current type of conditional
  int if_line; // is the line number at which it began
  int serial; // is the serial number
public:
  int get_limit() const { return if_limit; }
  int get_type() const { return cur_if; }
  void set_limit(int x) { if_limit =x; }
  int get_line() const { return if_line; }
  int get_serial()const { return serial; }
  void dump(int) const;
  CondAux(int a,int b, int c, int d): 
    if_limit(a),cur_if(b), if_line(c),serial(d) {}
};


// This is used to implement \if, \else, \fi
class Condition {
  int if_serial;
  vector<CondAux> D;  
 public:
  Condition()  { if_serial=0; }
  void pop();
  uint push(int);
  void wait_for_fi(){ D.back().set_limit(fi_code); }
  void terminate();
  int top_serial() const { return D.empty()? -1:  D.back().get_serial(); }
  int top_branch() const;
  int top_type() const;
  int top_level() const { return D.size(); };
  int top_limit() const { return D.empty()? 0 : D.back().get_limit(); }
  int top_line() const { return D.empty()? 0 : D.back().get_line(); }
  void set_limit(int n, int l) { D[n-1].set_limit(l); }
  bool is_this_if(uint n) const { return n==D.size(); }
  bool empty() const { return D.empty(); }
  void dump() const;
};


