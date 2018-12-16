// -*- C++ -*-
// $Id: txclasses.h,v 2.10 2009/01/05 10:35:32 grimm Exp $
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008


// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This holds the translation of \DeclareOption{foo}{bar}
// or \usepackage[foo=bar]{pack} 
//  name foo, val bar


// In the case of \DeclareOption{foo}{bar}, then name=full_name =foo
// and val is bar or empty. In the case \usepackage, this is different
class KeyAndVal {
private:
  string full_name; // In case 'key = {\val,etc}' holds all chars
  string name; // name of the option, e.g. 'key'
  TokenList val; // value of the option, eg '={\val,etc}'.
  bool used; // set to true when option is used
public:
  KeyAndVal(){}
  KeyAndVal(string a, TokenList b,string all) :full_name(all), name(a), val(b),used(false) {}
  const string& get_name ()const { return name; }
  const string& get_full_name ()const { return full_name; }
  const TokenList& get_val ()const { return val; }
  bool has_name(const string& x) const { return name==x; }
  bool has_full_name(const string& x) const { return full_name==x; }
  bool has_name(String x) const { return name==x; }
  void dump(Buffer&B) const { B << full_name; };
  void use(TokenList&L) const;
  void kill() { val = TokenList(); used = true;}
  void use_and_kill(TokenList& L,KeyAndVal&U,bool X);
  TokenList to_list() const;
  bool is_used() const { return used; }
  void mark_used() { used = true; }
  void mark_un_used() { used = false; }
};

typedef vector<KeyAndVal> OptionList;

// This defines what is in a package or a class
class LatexPackage {
public:
  string name; // name, with prefix P or C
  string date; // date and version 
  string req_date; // date argument in the call
  OptionList Uoptions; // options of \usepackage
  OptionList Poptions; // options defined by the package
  bool has_a_default; // is there a default error handler ?
  TokenList default_handler; // the token lists of the default handler
  TokenList hook; // for \AtEndOfPackage
  bool seen_process; // have we seen a \ProcessOptions 
  bool checked; // set by \usepakage (for re-use)
public:
  LatexPackage(string A); 
  void add_to_hook(TokenList&L) { hook.splice(hook.end(),L); }
  void add_options(const OptionList& L);
  int find_option(const string& name);
  bool is_class() const  { return name[0] == 'C'; }
  String real_name() const { return name.c_str()+1; } 
  string full_name() const { return name; } 
  String pack_or_class() const { return is_class() ? "class "  : "package "; }
  bool has_name(const string & s) const { return name==s; }
  void print_options();
  void check_global_options(TokenList& res, bool X);
  void check_local_options(TokenList& res, bool X);
  void check_all_options(TokenList& res,TokenList&, int X);
  void reload();
};

class ClassesData {
public:
  vector<LatexPackage*> packages;
  OptionList unused_options; // is \@unusedoptionlist in latex
  OptionList global_options; // is \@classoptionslist is latex
  TokenList documentclass_hook; // single hook for all classes
  bool seen_document_class; // have we seen a \documentclass command
  bool using_default_class; // inhibits warning
public:
  ClassesData();
  LatexPackage* cur_pack();
  int find_package(const string & name,bool,bool);
  void new_unused_global_option(const KeyAndVal& s)
  { unused_options.push_back(s); }
  void remove_from_unused(const string& name);
  void show_unused();
};

// The  date parser
class FormatDate
{
  int field1, field2, field3;
  bool scan_a_field(Buffer&,int&);
  bool scan_a_month(Buffer&,int&);
  bool scan_next(Buffer&,int&);
  bool sort();
  int next_format_char(Buffer&);
  bool parse_format(Buffer&);
  bool parse(Buffer&);
public:
  void interpret(const string&);
  int get_year() const { return field3; }
  int get_month() const { return field1; }
  int get_day() const { return field2; }
};

