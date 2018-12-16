// -*- C++ -*-
// $Id: txtitlepage.h,v 2.9 2008/02/21 17:46:00 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002-2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains stuff for title-page element.

class TitlePageFullLine;
  static const int tp_p_flag=2;
  static const int tp_e_flag=4;
  static const int tp_q_flag=8;
  static const int tp_plus_flag=16;
  static const int tp_A_flag=32;
  static const int tp_B_flag=64;
  static const int tp_C_flag=96;

// data for a titlepage item
class TitlePageAux {
  string T1,T2,T3,T4; // the four strings
  int idx;    // index into titlepage::Data
  int xflags;  // flags associated to the object
  tpi_vals type; // type of object

 public:
  bool classify(tpi_vals,int&);
  Xmlp convert(int);
  Xmlp convert(int,Xmlp);
  Xmlp convert(int i,Istring s) { return convert(i, new Xml(s)); }
  void dump(int);
  void exec_start(int);
  void exec_post();
  void exec(int,bool);
  void set_T1(string x) { T1 = x; }
  void set_T2(string x) { T2 = x; }
  void set_T3(string x) { T3 = x; }
  void set_T4(string x) { T4 = x; }
  void set_flags(int f) { xflags = f; }

  tpi_vals get_type() { return type; }
  tpi_vals& get_typeref() { return type; }
  int get_idx() { return idx; }
  int get_flags2() const { return 32*(xflags/32); } 
  bool has_u_flags() const { return (xflags&1) != 0; }
  bool has_p_flags() const { return (xflags&tp_p_flag) != 0; }
  bool has_e_flags() const { return (xflags&tp_e_flag) != 0; }
  bool has_q_flags() const { return (xflags&tp_q_flag) != 0; }
  bool has_plus_flags() const { return (xflags&tp_plus_flag) != 0; }
  TitlePageAux(): idx(0),xflags(0), type(tpi_zero) {}
  TitlePageAux(TitlePageFullLine& X);
  int find_UR(String s, int n) const;
  string get_T1() { return T1; }
  string get_T2() { return T2; }
  string get_T3() { return T3; }
  string get_T4() { return T4; }
  bool find_cmd (const string& s) const;
  void move_T1T2(string x) { T1 = T2; T2 = x;}
  bool increment_flag();
  void decode_flags();
};

// One item if a titlepage
class TpiOneItem {
  char p1;  // first character modifier
  char p2;  // second character modifier
  string value; // the value
  tpi_vals v; // the type (none, string, command, or XML element)
 public:
  TpiOneItem() { reset(); }
  bool has_a_char() const { return p1 !=0 || p2 != 0; }
  bool noval() const { return v==tpi_noval; }
  bool is_elt() const { return v==tpi_elt; }
  bool is_str() const { return v==tpi_str; }
  bool is_cmd() const { return v==tpi_cmd; }
  bool is_alias() const { return v==tpi_alias; }
  bool only_dash() const { return p1=='-' && p2==0; }
  bool question() const { return p1=='?' && p2==0; }
  bool plus() const { return p1=='+' && p2==0; }
  bool quest_plus() const { return p1=='?' && p2=='+'; }
  bool second_char() const { return p2!=0; }
  tpi_vals get_v() const { return v; }
  string get_value() const { return value; }
  char get_p1() const { return p1; }
  void set_p2(char c) { p2 = c; }
  void set_p1(char c) { p1 = c; }
  //  void bad() { v = tpi_err; }
  void set_v(tpi_vals V) { v = V;}
  void set_value(const Buffer& b) { value = b.to_string(); }
  void reset();
};

// temporary class, will bew copied into a TitlePageAux
class TitlePageFullLine {
  TpiOneItem item1, item2, item3,item4; // the four items
  int flags; // the flags 
 public:
  friend class titlepage;
  friend class TitlePageAux;
  int read();
  void kill();
  tpi_vals classify(int,int state);
  int get_flags()const { return flags; }
  bool encode_flags(char, char);
};

class TitlePage {
  int len2; // size of bigtable and Data
  bool valid; // is this initialised and not killed ?
  int size;   // allocated size of bigtable
  Xmlp* Data;  // the array of xml data
 public:
  vector<TitlePageAux> bigtable; // the table
  int state;  // current state of the parser
  TitlePage() : len2(1),valid(false),size(0),Data(0) {}
  Xmlp& operator[] (int k) { return Data[k]; }
  TitlePageAux& get_bigtable(int k) { return bigtable[k] ;}
  int get_len2()const { return len2;}
  bool is_valid() { return valid; }
  void make_invalid() { valid = false; }
  void make_valid() { valid = true; }
  void start_thing(bool);
  void parse();
  int increase_data() { len2++; return len2-1;}
  void check_size();
  int find_UR(const string&,string) const;
  int find_cmd(const string&) const;
};

