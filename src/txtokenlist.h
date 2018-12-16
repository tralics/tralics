
// -*- C++ -*-
// $Id: txtokenlist.h,v 2.13 2008/02/21 17:46:00 grimm Exp $
// Copyright INRIA/apics (Jose' Grimm)  2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)



class Hashtab;

typedef std::list<Token> TokenList;
typedef std::list<Token>::const_iterator const_token_iterator;
typedef std::list<Token>::iterator token_iterator;

namespace token_ns {
  void add_env(TokenList&L, String name);
  void add_par_noindent(TokenList&,const Hashtab&);
  void add_verbatim_number(TokenList&,const Hashtab&, int);
  int block_size(const TokenList&);
  void expand_nct(TokenList&);
  bool expand_nct(TokenList&L,Token T,int n, uchar c,int&);
  void expand_star(TokenList&);
  TokenList fast_get_block(TokenList&);
  void fast_get_block(TokenList&,TokenList&w);
  TokenList get_a_param(TokenList&,bool);
  TokenList get_block(TokenList&);
  Token get_unique(TokenList& L);
  void get_unique(TokenList& L,Token&t1,Token&t2);
  bool has_a_single_token(const TokenList&);
  bool has_a_single_token(const TokenList&, Token);
  TokenList posint_to_list (int n);
  void push_back_i (TokenList&,int n);
  void remove_block(TokenList&);
  void remove_ext_braces(TokenList&);
  void remove_initial_spaces(TokenList&);
  void remove_first_last_space(TokenList&);
  void replace(TokenList& A, Token x1, Token x2); 
  int replace_space(TokenList& A, Token x2, Token x3);
  void show(const TokenList&);
  bool split_at(Token x1, Token x2, Token x3,TokenList&L, TokenList& z,bool);
  TokenList string_to_list (Istring s);
  TokenList string_to_list (String s,bool);
  TokenList string_to_list (const string& s,bool);
  void double_hack(TokenList& key);
  bool split_at(Token m,TokenList&L, TokenList& z);
  bool is_sublist(token_iterator A, token_iterator B, int);
  bool is_in(TokenList& A, TokenList&B, bool remove);
  void normalise_list(char c, TokenList &L);
  void sanitize_one(TokenList &L, uchar c);
  void sanitize_one(TokenList &L, TokenList& s, int n);
  void sanitize_one(TokenList &L);
  bool check_brace(Token,int&);
  bool compare(const TokenList& A, const TokenList&B);
}




// This is for fp
class FpGenList 
{
public:
  TokenList value;
public:
  FpGenList (TokenList A): value(A) {}
  void  add_last_space(String);
  void  add_last_space(TokenList&,String);
  Token find_str(int&n) const;
  void  fp_gen_add();
  void  fp_gen_app();
  void  fp_gen_exp();
  void  fp_gen_komma();
  void  fp_gen_mul();
  void  push_back(Token L) { value.push_back(L); }
  void  push_back(TokenList& L) { value.splice(value.end(), L); }
  void  remove_first_n (int n);
  void  remove_spaces();
  void  split_after(int n,TokenList& z);
  Token split_at(Token x, Token y,TokenList& z);
  void  split_after(token_iterator X,TokenList& z);
  bool  split_at_p(TokenList&,TokenList& z);
  void  to_postfix();
  void  fp_check_paren();
};


// 
class FpStack 
{
  TokenList value;
public:
  void clear() { value.clear(); }
  bool empty() const { return value.empty(); }
  void pop_upn(FpNum&);
  void pop_upn(TokenList&);
  void push_front(Token L) { value.push_front(L); }
  void push_front(TokenList& L) { value.splice(value.begin(), L); }
  void push_upn(TokenList&);
  void push_upn(FpNum);
};


// This represents the value of a user-defined command
class Macro 
{
  int nbargs; // number of arguments
  def_type type; // type of macro
  TokenList delimiters[10]; // deleimiters bewtween arguments
  TokenList body; // the body
 public:
  Macro() : nbargs(0), type(dt_normal) {}
  Macro(TokenList L) : nbargs(0), type(dt_normal), body(L) {correct_type();}
  // other methods
  def_type get_type () const { return type; }
  int get_nbargs() const { return nbargs; }
  TokenList& get_body()  { return body; }
  const TokenList& get_body() const { return body; }
  void set_nbargs(int n) { nbargs = n; }
  void set_type(def_type n) { type = n; }
  bool is_same(const Macro&) const;
  const TokenList& operator[](int n) const { return delimiters[n]; }
  void set_delimiters(int k, TokenList L) { delimiters[k] = L; }
  void correct_type();
};

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
class Mactab{
 private:
  Macro** table;   // this contains the table
  int* rc_table;  // this contains the reference counts
  int cur_rc_mac_len;   // size of the table.
  int ptr;              // pointer to the first free position
 private:  
  void rc_mac_realloc();
 public:
  Mactab(): table(0), rc_table(0), cur_rc_mac_len(0), ptr(-1){};
  void incr_macro_ref(int c) { rc_table[c]++; }
  void delete_macro_ref(int i);
  Macro& get_macro(int k) { return *table[k]; }
  subtypes new_macro (Macro* s);
};
