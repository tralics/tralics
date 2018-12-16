
// -*- C++ -*-
// $Id: txxml.h,v 2.19 2015/07/27 16:15:40 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


// a xml object
class Xml
{
  Xid id;  // id of the objet
  Istring name; // name of the element
  vector <Xmlp> tree; // the aux field
 public:
  Xml(Istring n) : id(0), name(n) {}
  Xml(const Buffer& n) : id(0), name(Istring(n.c_str())) {}
  Xml(StrHash&X) : id(0), name(Istring(X.hash_find())) {}
  Xml(Istring x, Xmlp z);
  Xml(name_positions x, Xmlp z);
  Xml(name_positions x, Xid n): id(n), name(Istring(x)) {} 
  Xml() : id(0), name(Istring(0)){}

  void change_id(Xid n) { id = n; } // use with care
  void add_att(Istring a, Istring b) { id.add_attribute(a,b); }
  void add_att(name_positions a, name_positions b) { id.add_attribute(a,b); }
  void add_first(Xmlp x);
  void add_ref(string s);
  void add_tmp(Xmlp val);
  void add_last_nl(Xmlp val);
  void add_last_string (const Buffer& B);
  void add_nl();
  void add_non_empty_to(Xmlp res);
  Xmlp back() { return tree.empty() ? 0 : tree.back(); }
  void change_name(Istring s) { name = s; }
  void change_name(name_positions s) { name = the_names[s]; }
  void compo_special();
  bool contains_env(Istring name);
  void convert_to_string(Buffer& B);
  string convert_to_string();
  Xid delete_one_env0(Istring name);
  Xmlp deep_copy ();
  bool empty() const;
  bool empty_name() const { return name.empty(); }
  Xmlp father(Xmlp X,int&);
  Xmlp figline(int&ctr,Xmlp junk);
  bool find_on_tree(Xmlp check, Xmlp*res) const;
  Xmlp first_lower(Istring src);
  int get_cell_span() const;
  Xmlp get_first_env(name_positions name);
  Xid  get_id() const { return id; }
  Istring get_name() const { return name; }
  bool has_name(Istring s)const { return name == s; }
  bool has_name(name_positions s)const { return name == the_names[s]; }
  int how_many_env(Istring name);
  void insert_at(int pos, Xmlp x);
  bool insert_at_ck(int pos, Xmlp x);
  void insert_bib (Xmlp bib, Xmlp match);
  bool is_anchor() const { return !is_xmlc() && name== the_names[np_anchor]; }
  bool is_child(Xmlp) const;
  bool is_empty() const;
  bool is_empty_p() const;
  bool is_empty_spec() const;
  bool is_xmlc() const { return id.value <=0; }
  void kill_name() { name = Istring(); }
  Xmlp last_addr() const;
  Xmlp last_box();
  bool last_is(name_positions) const;
  bool last_is_string() const;
  void last_to_SH();
  void make_hole(int pos);
  void move(Istring name,Xmlp res);
  void one_fig_tab(bool is_fig);
  bool only_hi() const;
  bool only_recur_hi() const;
  bool only_text() const;
  bool par_is_empty();
  void pop_back() { tree.pop_back(); }
  void postprocess_fig_table(bool);
  Xmlp prev_sibling(Xmlp);
  bool put_at(int n, Xmlp x);
  void put_in_buffer(Buffer& x);
  void push_back(Buffer&B) { push_back(new Xml(B)); }
  void push_back(Xmlp val);
  void push_back_list(Xmlp);
  int real_size() const;
  void recurse(XmlAction&X);
  void recurse0(XmlAction&X);
  void remove_empty_par();
  bool remove_at(int);
  Xmlp remove_last();
  void remove_last_empty_hi();
  void remove_last_space();
  void remove_par_bal_if_ok();
  void rename(Istring,Istring);
  void reset();
  void sans_titre();
  String sans_titre(Xmlp);
  void set_id(int i) {id = i; }
  Xmlp single_non_empty () const;
  Xmlp single_son() const;
  int  size() const { return tree.size(); }
  void subst_env0(Istring name,Xmlp value);
  void swap_x(Xmlp);
  bool tail_is_anchor() const;
  void to_buffer(Buffer&) const;
  bool total_span(int& res) const;
  bool try_cline(bool action);
  bool try_cline_again(bool);
  void unbox(Xmlp);
  Xmlp value_at(int);
  void word_stats(string);
  void word_stats_i();
  Xmlp spec_copy();
  void replace_first(Xmlp x) { if(tree.size()) tree[0] = x; }
  void bordermatrix();
}
;


class XmlAndType {
  Xmlp value;
  math_types type;
 public:
  XmlAndType(Xmlp X) : value(X), type(mt_flag_small) {}
  XmlAndType(Xmlp X,math_types t) : value(X), type(t) {}
  Xmlp get_value()const { return value; }
  math_types get_type()const { return type; }
};
