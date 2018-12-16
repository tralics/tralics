// -*- C++ -*-
// $Id: txstack.h,v 2.33 2015/07/28 16:21:21 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008


// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file holds the stack and related stuff


// Array management
class ArrayInfo
{
  Xid id; // the id of the table
  int cell_no; // current cell number, first on row is zero
  vector<AttList> attribs; // positions attributes for each row
  vector<TokenList> u_table, v_table;
  int size,repeat_count, first_repeat; // for repeated patterns
 public:
  ArrayInfo(Xid a1) : id(a1), cell_no(0), size(0), repeat_count(0), first_repeat(0) {}
  bool its_me(Xid a) { return id==a; }
  int get_cell_no() const { return cell_no; }
  void set_cell_no(int k) { cell_no = k; }
  AttList get_cell_atts(int k);
  TokenList get_u_or_v(bool,int);
  void add_uv(TokenList& u, TokenList& v,AttList);
  int get_size() { return size; }
  void del_array_info();
};

// This is the stack
class Stack {
  class StackSlot{
    Xmlp obj;
    int line;
    Istring frame;
    mode md;
    Istring uid;
    bool omit_cell;
    void dump();
    void fulldump(int);
    StackSlot(Xmlp a, int b, Istring c, mode M, Istring u) 
      : obj(a), line(b), frame(c), md(M), uid(u), omit_cell(false) {}
    friend class Stack;
  };
  vector <StackSlot> Table;
 private:
  int last_xid;  // id of the last
  int xid_boot;
  Istring cur_lid;    // the id to be pushed on uids[]
  vector<AttList> attributes; // the main table of attributes
  vector<Xml*> enames; // the main table of element names
  Buffer mybuffer; // a buffer
  vector<ArrayInfo> AI;// the attributes for the current TeX arrays
  mode cur_mode;// the mode to be pushed on modes[]
 public:
  Stack();
  Xmlp newline_xml;

  void add_anchor(const string&);
  void add_att_to_last(Istring,Istring,bool);
  void add_att_to_last(Istring,Istring);
  void add_att_to_last(name_positions,name_positions);
  void add_att_to_last(name_positions,Istring);
  void add_att_to_cur(Istring A, Istring B);
  void add_att_to_cur(Istring A, Istring B,bool);
  void add_border(int a, int b);
  void add_borders(int a, int b);
  void add_center_to_p();
  void add_last(Xmlp);
  void add_last_string (const Buffer& B);
  void add_new_anchor();
  void add_new_anchor_spec();
  void add_nl();
  AttList& add_newid0(name_positions);
  void add_sp_to_p (int pid, int vid);
  void check_font();
  Xid cur_xid() { return top_stack()->get_id(); }
  void delete_table_atts();
  void dump();
  void dump_xml_table();
  Xmlp document_element() { return Table[0].obj; }
  Xmlp elt_from_id(int n) { return enames[n]; }
  void end_module ();
  Xmlp fetch_by_id (int n);
  ArrayInfo* find_cell_props(Xid id);
  void find_cid_rid_tid(Xid&cid,Xid&rid,Xid&tid);
  int find_ctrid(subtypes);
  Xmlp find_parent (Xmlp);
  void finish_cell(int w);
  Istring first_frame() const;
  const StackSlot& first_non_empty() const;
  void fonts0 (name_positions x);
  Xmlp fonts1 (name_positions x);
  AttList& get_att_list(int k) { return attributes[k]; }
  Istring get_cur_id() const  { return cur_lid; } 
  Xmlp get_cur_par();
  Xmlp get_father();
  mode get_mode() const { return cur_mode; }
  ArrayInfo* get_my_table(Xid&);
  Xid get_top_id() { return top_stack()->get_id(); }
  TokenList get_u_or_v(bool u_or_v);
  Xid get_xid() { return last_xid; } 
  void hack_for_hanl();
  void implement_cit(string b1, Istring b2, string a, string c);
  bool in_v_mode() const { return get_mode() == mode_v; }
  bool in_h_mode() const { return get_mode() == mode_h; }
  bool in_no_mode() const { return get_mode() == mode_none; }
  bool in_bib_mode() const { return get_mode() == mode_bib; }
  bool in_array_mode() const { return get_mode() == mode_array; }
  void init_all(string a);
  void ipush(Istring, Xmlp);
  bool is_float();
  bool is_frame(name_positions) const;
  bool is_frame2(name_positions) const;
  bool is_omit_cell() { return Table.back().omit_cell; }
  AttList& last_att_list() { return get_xid().get_att(); }
  void mark_omit_cell();
  ArrayInfo& new_array_info(Xid i);
  Xid next_xid(Xml*);
  void para_aux(int x);
  void pop (Istring a);
  void pop (name_positions a);
  void pop_if_frame(Istring x);
  void push(Istring a, Xmlp b);
  void push1(Istring name,name_positions x);
  void push1(name_positions x);
  Xmlp push_hbox(Istring);
  void push_pop_cell(int);
  void push_trace();
  Xid push_par(int);
  Xmlp remove_last();
  void remove_last_space();
  void set_arg_mode() { cur_mode = mode_argument; }
  void set_array_mode() { cur_mode = mode_array; }
  void set_bib_mode() { cur_mode = mode_bib; }
  void set_cur_id(Istring k) { cur_lid = k; } 
  void set_h_mode() { cur_mode = mode_h; }
  void set_m_mode() { cur_mode = mode_math; }
  void set_mode(mode x) { cur_mode = x; }
  void set_no_mode() { cur_mode = mode_none; }
  void set_v_mode() { cur_mode = mode_v; }
  void set_xid_boot () { xid_boot = last_xid; }
  void T_ampersand();
  void T_hline ();
  Xmlp temporary();
  Xmlp top_stack() { return Table.back().obj; }
  void trace_pop(bool);
  void trace_stack();
  void unbox(Xmlp);
  Xmlp xml2_space(Istring a, Xmlp c, Xmlp d);
  Xmlp xml2_space(Istring a, Istring, Istring, Xmlp c, Xmlp d);
  Xmlp xml2_space(Istring a, Istring, Xmlp c, Xmlp d);

};

