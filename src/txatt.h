// -*- C++ -*-
// $Id: txatt.h,v 2.5 2008/10/03 08:07:55 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This is an attribute list.
class AttPair {
 public:
  Istring name;
  Istring value;
 AttPair(Istring a, Istring b) :name(a), value(b) {}
};

class AttList {
  // vector of attribute, like colspan=4, width=3.4cm
  // name (first) and val (second) are pointers into the SH table
public:

  vector<AttPair> val; // the value

  Istring get_val(int i) { return val[i].value; }
  void reset() { val = vector<AttPair>(); }
  bool empty() { return val.size()==0; }
  void push_back_empty(Istring n);
  void push_back(name_positions n, Istring v);
  void push_back(name_positions n, name_positions v);
  void push_back(name_positions n, name_positions v,bool);
  void push_back(Istring n, Istring v);
  void push_back(Istring n, Istring v, bool f);
  int has_value(Istring x) const;
  void delete_att(name_positions);
  void destroy();
  void print(ostream&);
};

// This is a table of AttList. We should use a vector instead
class AttListTable {
  AttList * table; // the table
  Xml*  xtable;
  int len; // the length of the table
  int size; // the size of the table
 public:
  AttListTable(): table(0), len(0), size (0) {}
  void destroy();
  void resize();
  void init();
  AttList& operator [](int k) { return table[k]; }
  AttList get(int n) { 
    if(n<len) return table[n]; 
    else return AttList();}
  void push_back(AttList X);
  void push_back();
  void push_back(Istring,Istring);
  void push_back(name_positions,name_positions);
  int get_size() const {return size; }
};
