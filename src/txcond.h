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
  auto get_limit() const -> int { return if_limit; }
  auto get_type() const -> int { return cur_if; }
  void set_limit(int x) { if_limit =x; }
  auto get_line() const -> int { return if_line; }
  auto get_serial() const -> int { return serial; }
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
  auto push(int) -> uint;
  void wait_for_fi(){ D.back().set_limit(fi_code); }
  void terminate();
  auto top_serial() const -> int {
    return D.empty() ? -1 : D.back().get_serial();
  }
  auto top_branch() const -> int;
  auto top_type() const -> int;
  auto top_level() const -> int { return D.size(); };
  auto top_limit() const -> int { return D.empty() ? 0 : D.back().get_limit(); }
  auto top_line() const -> int { return D.empty() ? 0 : D.back().get_line(); }
  void set_limit(int n, int l) { D[n-1].set_limit(l); }
  auto is_this_if(uint n) const -> bool { return n == D.size(); }
  auto empty() const -> bool { return D.empty(); }
  void dump() const;
};


