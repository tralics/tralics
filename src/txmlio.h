// -*- C++ -*-
// $Id: txmlio.h,v 2.5 2015/07/29 16:10:40 grimm Exp $
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008


// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

enum x_type {xt_letter,xt_space,xt_digit,xt_ok_first,xt_other,xt_invalid};


class EntityDef{
  string name;
  string value;
public:
  EntityDef(string a, string b) : name(a), value(b) {}
  bool has_name(const string& x) const { return x== name; }
  string get_val() { return value; }
};

class XmlIO {
  Parser*P;
  Buffer B; // holds current element
  Buffer aux;
  Buffer line_buffer; // holds current line
  LinePtr lines; // input file
  int cur_line;
  x_type Type[128];
  vector<Utf8Char> input_line; // current line
  vector<Utf8Char> reread_list; // current line
  Utf8Char cur_char; // current character in some cases
  
  Xml* cur_xml;
  vector<Xml*> cur_stack;
   int cur_line_len;
  int input_line_pos;
  int enc; // 0=utf8, 1=latin1
  void error(string);
  vector<EntityDef > entities;
  int nb_cond;
  bool eof_ok;
private:
  Utf8Char peek_char();
  void skip_char();
  Utf8Char next_char();
  void skip_space();
  void next_line();
   bool at_eol() { return input_line_pos>= cur_line_len; }
  void scan_name();
  void scan_name(uchar);
  void scan_name(uchar,uchar);
  void parse_lt();
  void parse_end();
  void parse_dec();
  void parse_dec_comment();
  void parse_dec_entity();
  void parse_dec_element();
  void parse_dec_attlist();
  void parse_dec_doctype();
  void parse_dec_cdata();
  void parse_dec_conditional();
  void parse_dec_notation();
  bool parse_sys_pub();
  void expect (String);
  void parse_pi();
  void parse_tag();
  void parse_attributes();  
  void parse_att_val();
  void parse_quoted();
  void pop_this();
  void flush_buffer();
  bool expand_PEReference();
  void run();
public:
  XmlIO(Parser*P) : cur_char(' ') {}
  bool init(const string&);
  Xmlp prun();
};
