// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txtoken_rcsid=
  "$Id: txtoken.C,v 2.65 2012/05/15 17:14:30 grimm Exp $";

// token lists.
namespace {
  Buffer buffer_for_log;
  // never use this outside function buffer::push_back(const macro&)
  Buffer buffer_for_log1;
  Buffer buffer_for_log2;
}

extern void print_cv_stats();
int is_in_skipped; // set by token_ns::is_in
namespace classes_ns {
  void dump_file_list();
}

namespace token_ns {
  int length_normalise(TokenList&);
  bool is_spacer(Token);
}


// Prints some statistics at enfd of run.
void Stats::token_stats ()
{
  main_ns::log_or_tty 
    << "Math stats: formulas "<< m_cv << ", kernels " 
    << m_k << ", trivial " << m_trivial 
    << ", \\mbox "<< m_spec_box << ", large " << m_large << ", small " 
    << m_small << ".\n";
  if(nb_hdr) main_ns::log_or_tty << "Number of HdR: " << nb_hdr << ".\n";
  main_ns::log_or_tty 
    << "Buffer realloc "<< stb_alloc << ", string " 
    << st_nb_string << ", size "<< str_length 
    << ", merge " << m_merge << "\n"
    << "Macros created " << nb_macros 
    << ", deleted " << nb_macros_del 
    << "; hash size " << the_parser.hash_table.get_hash_usage()
    << "; foonotes " << footnotes 
    <<  ".\n"
    << "Save stack +"<< level_up << " -" << level_down << ".\n"
    << "Attribute list search "<< sh_find << "("<< sh_boot
    << ") found " << sh_used << " in " 
    << the_main->the_stack->get_xid().value <<" elements (" 
    <<  nb_xboot << " at boot).\n"
    << "Number of ref " << nb_ref
    << ", of used labels " << nb_used_ref 
    << ", of defined labels "  << nb_label_defined 
    << ", of ext. ref. " << nb_href << ".\n";
  classes_ns::dump_file_list();
  print_cv_stats();
}


// Converts an integer into a token list, catcode 12
// Assumes n>0, otherwise result is empty.
TokenList token_ns::posint_to_list(int n)
{
  TokenList L;
  if(n<=0) return L;
  while(n != 0) {
    int k = n % 10;
    n = n/10;
    Token t = Token(other_t_offset, uchar (k+'0'));
    L.push_front(t);
  }
  return L;
}

// Adds an integer to the end of the token list.
// Uses a temporary list, since we get digits from right to left.
// Noting done if n<0. This is used for inserting verbatim line numbers.
void token_ns::push_back_i (TokenList& L,int n)
{
  TokenList tmp =  posint_to_list(n);
  L.splice(L.end(),tmp);
}

// Finds the first M, not escaped by E in the list L.
// If so, puts the prefix in z, removes it from L
// A pair EX is replaced by X in Z if S is false
bool token_ns::split_at(Token e, Token m, Token m1, TokenList&L, TokenList& z,bool s)
{
  z.clear();
  while(!L.empty()) {
    Token x = L.front();
    L.pop_front();
    if(x==e) {
      if(s) z.push_back(x);
      if(L.empty()) break;
      x = L.front();
      L.pop_front();
      z.push_back(x);
    } else if(x== m) return true;
    else if(x== m1) return true;
    else z.push_back(x);
  }
  swap(L,z);
  return false;
}

// Checks that x is a brace, increases or decreases brace level
// returns true if this is the toplevel closing brace
bool token_ns::check_brace(Token x, int& bl)
{
  if(x.is_a_brace()) {
    if(x.is_a_left_brace()) {
      bl++;
    } else {
      bl--;
      if(bl==0) return true;
    }
  }
  return false;
}

// Replace x1 by x2 at toplevel in the list a
void token_ns::replace(TokenList& A, Token x1, Token x2)
{
  token_iterator C = A.begin();
  token_iterator E = A.end();
  int bl = 0;
  while(C != E) {
    Token x = *C;
    check_brace(x,bl);
    if(bl==0 && x==x1) *C = x2;
    ++C;
  }
}

// Replace space by x2,x3  at brace-level zero.
// Two consecutive space tokens are replaced by
// a single occurence. 
int token_ns::replace_space(TokenList& A, Token x2, Token x3)
{
  remove_first_last_space(A);
  token_iterator C = A.begin();
  token_iterator E = A.end();
  int bl = 0;
  int n = 0;
  bool prev_is_space = false;
  while(C != E) {
    Token x = *C;
    check_brace(x,bl);
    if(bl==0 && x.is_space_token()) {
      if(!prev_is_space) {
	A.insert(C,x2);
	*C = x3;
	++n;
      }
      prev_is_space = true;
    } else prev_is_space = false;
    ++C;
  }
  return n;
}

// True if the Token is upper case or lower case x
bool Token::no_case_letter(char x) const
{
  if(cmd_val()!=letter_catcode) return false;
  int c = val_as_letter();
  if(c==x) return true;
  if(c== x+'A'-'a') return true;
  return false;
}

// the hash table  --------------------------------------------------

// Given a string s, with hash code p, we have p<hash_prime. Normally, 
// the string will be at position p. Otherwise we used slots with positions
// > hash_prime. If all these slots are used then table look-up will be
// slower.  Initially, hash_used is hash_size. We decrement until
// finding an empty slot. It will never increment!

// Finds an empty slot in the hash table; uses it with name s.
// Return value is hash_used
// The string s must be a permanent string
int Hashtab::find_empty(String s)
{
  for(;;) {
    if(hash_used<=1) {
      main_ns::log_and_tty << "Size of hash table is " << hash_size << "\n";
      main_ns::log_and_tty << "Value of has_prime is " << hash_prime << "\n";
      main_ns::log_and_tty << "hash table full\n" << lg_fatal; 
      abort();
    }
    hash_used--;
    if(!Text[hash_used]) break;
  }
  Text[hash_used] = s;
  hash_usage++;
  return hash_used;
}

// unused, kept for debug
// void Hashtab::dump()
// {
//   for(int i=0;i<hash_size;i++) {
//     if(Text[i]) cout << i << " " << Text[i] << "\n";
//   }
// }

// Returns the hash location of the name in the buffer. 
// If a new slot has to be created, uses the string name, if not empty.
// The string name must be a permanent string
int Hashtab::hash_find(const Buffer&b, String name)
{
  int p = b.hashcode(hash_prime);
  for(;;) {
    if(Text[p] && b == Text[p]) return p;
    if(Next[p]) p = Next[p];
    else break;
  }
  if(!name) name = b.convert_to_str();
  return find_aux(p,name);
}


// Finds the object in the buffer B.
int Hashtab::hash_find()
{
  int p = B.hashcode(hash_prime);
  for(;;) {
    if(Text[p] && B == Text[p]) return p;
    if(Next[p]) p = Next[p];
    else break;
  }
  return find_aux(p,B.convert_to_str());
}

// This inserts name in the hash table. If Texp[p] is empty, mark it
// non empty, use this position. Otherwise find an empty position,
// and set Next[p] to this position.
int Hashtab::find_aux(int p, String name)
{
  if(Text[p]) {
    find_empty(name);
    Next[p] = hash_used;
    return hash_used;
  } else {
    hash_usage++;
    Text[p] = name;
    return p;
  }
}

// Defines the command names a, but hash_find will not find it.
// The string a must be a permanent string
// This must be used at bootstap code: 
// if hash_used<hash_prime it overwrites memory
Token Hashtab::nohash_primitive(String a, CmdChr b)
{
  hash_used --;
  Text[hash_used] = a;
  int t = hash_used+hash_base;
  eqtb[t-1].special_prim(b);
  return Token(t+cs_token_flag);
}

// Returns the hashcode of the string in the buffer (assumed zero-terminated).
int Buffer::hashcode(int prime) const
{
  int j = 1;
  unsigned int h = (unsigned char) (buf[0]);
  if(h==0) return 0;
  for(;;) {
    unsigned char c = buf[j];
    if(c==0)  return h;
    h = (h+h+c) % prime;
    j++;
  }
}

// Returns the hash table location of the active character s,
// This is the hash loc of x+2^16.
Token Hashtab::locate_active(Utf8Char s)
{
  Utf8Char c = s.get_value() + nb_characters;
  B << bf_reset << c; 
  return Token(hash_find(B,0) + cs_offset);
}

// Returns the hash table location of the non-active character s,
// This is the hash loc of x+2^16.
Token Hashtab::locate_mono(Utf8Char c)
{
  if(c.is_ascii())
    return Token(uchar(c.get_value()) + single_offset);
  B << bf_reset << c; 
  return Token(hash_find(B,0) + cs_offset);
}


// Returns the hash table location of the string s.
// The string must be a permanent string
Token Hashtab::locate(String s)
{
  if(!s) return Token(find_empty("unnamed_command.") + cs_offset);
  if(!s[1]) return Token(uchar(s[0]) + single_offset);
  B << bf_reset << s; 
  return Token(hash_find(B,s) + cs_offset);
}

Token Hashtab::locate(const string& s)
{
  if(s.size()==1) return Token(uchar(s[0]) + single_offset);
  B << bf_reset << s; 
  return locate(B);
}

// This returns the token associate to the string in the buffer.
Token Hashtab::locate(const Buffer&b)
{
  if(b.size()==0) return Token(null_cs+cs_token_flag);
  Utf8Char c = b.unique_character();
  if(c.non_null()) return Token(c.get_value() + single_offset);
  return Token(hash_find(b,0) + cs_offset);
}

// This returns true if the token associated to the string in the buffer.
// exists in the hash table and is not undefined.
// Sets last_tok
bool Hashtab::is_defined(const Buffer&b)
{
  int T=0;
  if(b.size()==0) T = null_cs;
  else {
    Utf8Char c = b.unique_character();
    if(c.non_null()) T = c.get_value() + single_base;
    else  {
      int p = b.hashcode(hash_prime);
      for(;;) {
	if(Text[p] && b == Text[p]) break;
	if(Next[p]) p = Next[p];
	else return false;
      }
      T = p +hash_base;
    }
  }
  last_tok = Token(T+cs_token_flag);
  return !eqtb[T-1].is_undefined();
}

// Creates a primitive.
// The string s must be a permanent string
Token Hashtab::primitive (String s, symcodes c, subtypes v)
{
  Token res = locate(s);
  int w = res.eqtb_loc();
  eqtb[w].primitive(CmdChr(c,v));
  return res;
}

// \global\let\firststring = \secondstring 
// Both strings must be permanent strings
void Hashtab::eval_let(String a, String b)
{
  int A = locate(a).eqtb_loc();
  int Bval = locate(b).eqtb_loc();
  the_parser.eq_define(A,eqtb[Bval].get_cmdchr(),true);
}

// \let\firststring = \secondstring 
// Both strings must be permanent strings
void Hashtab::eval_let_local(String a, String b)
{
  int A = locate(a).eqtb_loc();
  int Bval = locate(b).eqtb_loc();
  the_parser.eq_define(A,eqtb[Bval].get_cmdchr(),false);
}

inline bool token_ns::is_spacer(Token t)
{
  return t == Token(newline_token_val) || t ==Token(space_token_val);
}


bool token_ns::compare(const TokenList& A, const TokenList&B)
{
  const_token_iterator C1 = A.begin();
  const_token_iterator E1 = A.end();
  const_token_iterator C2 = B.begin();
  const_token_iterator E2 = B.end();
  for(;;) {
    if(C1==E1 || C2==E2) 
      return C1==E1 && C2==E2;
    if(*C1 != *C2) {
      if(!is_spacer(*C1)) return false;
      if(!is_spacer(*C2)) return false;
    }
    ++C1;
    ++C2;
  } 
}


// compares two macros
bool Macro::is_same(const Macro& aux) const
{
  if(nbargs != aux.nbargs) return false;
  if(type != aux.type) return false;
  if(!token_ns::compare(body,aux.body)) return false;
  for(int i =0; i<10;i++)
    if(!token_ns::compare(delimiters[i],aux.delimiters[i])) return false;
  return true;
}

// Removes the external braces in {foo}, but not in {foo}{bar}.
void token_ns::remove_ext_braces(TokenList&L)
{
  if(L.empty()) return;
  if(!L.front().is_OB_token()) return;
  if(!L.back().is_CB_token()) return;
  token_iterator C = L.begin();
  token_iterator E = L.end();
  ++C;
  --E;
  int b = 0;
  while(C != E) {
    if(C->is_OB_token()) b++;
    else if(C->is_CB_token()) { b--; if(b<0) return; }
    ++C;
  }
  if(b!=0) return; // should not happen.
  L.pop_front();
  L.pop_back();
}

void token_ns::remove_initial_spaces(TokenList&L)
{
  while(!L.empty()) {
    if(!L.front().is_space_token()) return;
    L.pop_front();
  }
}

// If the token list is `{foo}bar{gee}'
// returns foo, leaves `bar{gee}' in the list
TokenList token_ns::get_block(TokenList&L)
{
  int bl = 0;
  TokenList res;
  remove_initial_spaces(L);
  while(!L.empty()) {
    Token t = L.front();
    L.pop_front();
    res.push_back(t);
    if(check_brace(t,bl)) {
      // Here we have {foo} in res
      remove_ext_braces(res);
      return res;
    }
  }
  return TokenList();
}

// Assumes that the list starts with a brace. 
// Returns the number of tokens in sublist with its braces.
// If the sublist is everything, returns -1.
// in case of problem, returns -2.
int token_ns::block_size(const TokenList&L)
{
  int res = 0;
  int bl = 0;
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    Token t = *C;
    ++C;
    ++res;
    if(check_brace(t,bl)) {
      if(C==E) return -1;
      return res;
    }
  }
  return -2;
}

// Assumes that the list starts with a brace. 
// Returns the sublist with its braces.
TokenList token_ns::fast_get_block(TokenList&L)
{
  int len = block_size(L);
  TokenList res;
  if(len==-2) {
    L.clear();
    return res;
  }
  if(len==-1) {
    L.swap(res);
    return res;
  }
  token_iterator C = L.begin();
  while(len>0) {  len--; ++C; }
  res.splice(res.begin(),L,L.begin(), C);
  return res;
}

// Assumes that the list L starts with a brace. 
// puts the first block to the end of res
void token_ns::fast_get_block(TokenList&L,TokenList&res)
{
  int len = block_size(L);
  if(len==-2) {
    L.clear();
    return;
  }
  if(len==-1) {
    res.splice(res.end(),L); 
    return;
  }
  token_iterator C = L.begin();
  while(len>0) {  len--; ++C; }
  res.splice(res.end(),L,L.begin(), C);
}

// Returns the first token, or the first token-list
// There are braces around the thing if br is true
TokenList token_ns::get_a_param(TokenList&L, bool br)
{
  TokenList res;
  while(!L.empty()) {
    Token t = L.front();
    if(t.is_a_left_brace()) {
      TokenList w = fast_get_block(L);
      if(!br) remove_ext_braces(w);
      return w;
    }
    L.pop_front();
    if(t.is_space_token()) continue;
    res.push_back(t);
    break;
  }
  if(br) the_parser.brace_me(res);
  return res;
}

// Like getblock, returns nothing
void token_ns::remove_block(TokenList&L)
{
  int bl = 0;
  remove_initial_spaces(L);
  while(!L.empty()) {
    Token t = L.front();
    L.pop_front();
    if(check_brace(t,bl)) return;
  }
}

// Interprets *{3}{abc} as 3 copies of abc
// This is used for expanding the header of a table
void token_ns::expand_star(TokenList&L)
{
  TokenList res;
  while(!L.empty()) {
    Token t = L.front();
    if(t.is_a_left_brace()) {
      fast_get_block(L,res);
    } else if (!t.is_star_token()) {
      L.pop_front();
      res.push_back(t);
    } else {
      L.pop_front();
      TokenList u = fast_get_block(L);
      TokenList v = fast_get_block(L);
      remove_ext_braces(u);
      remove_ext_braces(v);
      int n=0;
      while(!u.empty()) {
	Token q = u.front();
	u.pop_front();
	if(!q.is_digit_token()) break;
	n = 10*n+ q.val_as_digit();
	if(n>1000000) break; // bug?
      }
      while(n>0) {
	TokenList w = v;
	L.splice(L.begin(),w);
	n--;
      }
    }
  }
  L.swap(res);
}

// Converts the string in the buffer into a token list.
// Everything is of \catcode 12, except space.
// If the switch is true, \n is converted to space, otherwise newline
TokenList Buffer::str_toks (bool nl)
{
  Token SP = Token(space_token_val);
  Token NL = the_parser.hash_table.newline_token;
  TokenList L;
  reset_ptr();
  for(;;) {
    if(at_eol()) return L;
    Utf8Char c = next_utf8_char();
    if(c==0) {} // ignore bad chars
    else if(c == ' ') L.push_back(SP);
    else if(c == '\n') L.push_back(nl ? SP : NL);
    else L.push_back(Token(other_t_offset,c));
  }
}

// Use character code 11 whenever possible
TokenList Buffer::str_toks11 (bool nl)
{
  Token SP = Token(space_token_val);
  Token NL = the_parser.hash_table.newline_token;
  TokenList L;
  reset_ptr();
  for(;;) {
    if(at_eol()) return L;
    Utf8Char c = next_utf8_char();
    if(c==0) {} // ignore bad chars
    else if(c == ' ') L.push_back(SP);
    else if(c == '\n') L.push_back(nl ? SP : NL);
    else if(c.is_letter() || c=='@') L.push_back(Token(letter_t_offset,c));
    else L.push_back(Token(other_t_offset,c));
  }
}

// Converts a string to a token list. If b is true, we add braces.
TokenList token_ns::string_to_list (String s, bool b)
{
  Buffer& B = buffer_for_log;
  B << bf_reset << s; 
  TokenList L = B.str_toks(false);
  if(b) the_parser.brace_me(L);
  return L;
}

TokenList token_ns::string_to_list (const string& s, bool b)
{
  Buffer& B = buffer_for_log;
  B << bf_reset << s; 
  TokenList L = B.str_toks(false);
  if(b) the_parser.brace_me(L);
  return L;
}

// Converts a istring to a token list. 
// Special hack, because we insert the number, not the value
TokenList token_ns::string_to_list (Istring s)
{
  Buffer& B = buffer_for_log;
  B << bf_reset << s.get_value(); 
  return B.str_toks(false);
}

// Converts a Token list into a String. 
Buffer& Buffer::operator<<(const TokenList& L)
{ 
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    push_back(*C,false); 
    ++C;
  }
  return *this;
}

// Prints a token list.
// Note: conversion to log_encoding
ostream& operator<<(ostream&fp, const TokenList& L)
{
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    buffer_for_log.reset();
    if(buffer_for_log.push_back(*C))
      buffer_for_log << ' ';
    fp << buffer_for_log.c_str();
    ++C;
  }
  return fp;
}

// Prints a token list.
ostream& operator<<(ostream&fp, const Istring& L)
{
  return fp << L.c_str();
}

// Puts a macro into a buffer.
void Buffer::push_back(const Macro& x)
{
  *this << x[0];
  int K = x.get_nbargs();
  if(x.get_type() != dt_optional) {
    for(int i=0;i<K;i++){
      *this << '#' << i+1 << x[i+1];
    }
  } else {
    *this << x[1];
    for(int i=1;i<K;i++){
      *this << '#' << i+1; 
    }
  }
  if(wptr>0 && buf[wptr-1]=='{')
    buf[wptr-1]='#';
  *this << "->" << x.get_body();
}


// Puts a macro into a buffer.
// Sw is true when we want to print it
void Buffer::push_back(const Macro& x, bool sw)
{
  if(!sw)
    push_back(x);
  else {
    Buffer&B=buffer_for_log1; // Use some other buffer
    B.reset();
    B.push_back(x);
    push_back(B.convert_to_log_encoding());
  }    
}

Buffer& Buffer::operator<<(const Macro& x)
{
  push_back(x,false); 
  return *this; 
}


// Puts a macro definition in a file.
ostream& operator<<(ostream&fp, const Macro&x)
{
  Buffer&B = buffer_for_log;
  B << bf_reset;
  B.push_back(x,true);
  return fp << B.c_str();
}

FullLogger& operator<<(FullLogger&fp, const Macro&x)
{
  Buffer&B = buffer_for_log;
  B << bf_reset;
  B.push_back(x,true);
  return fp << B.c_str();
}



// This is the Ctor of strhash.
StrHash::StrHash()
{
  hash_len = hash_size;
  Text = new String[hash_len];
  Value = new String[hash_len];
  Next = new int [hash_len];
  Labinfo = new LabelInfo* [hash_len];
  for(int i=0;i<hash_len;i++) {
    Text[i] = 0;
    Next[i] = 0;
    Labinfo[i] = 0;
  }
  hash_last = hash_prime+1;
  Text[0] = ""; // make sure these are allocated.
  Text[1] = "";
  Text[2] = " ";
  Value[0] = ""; // make sure these are allocated.
  Value[1] = "";
  Value[2] = " ";
}

// This is called in case the table is too small.
void StrHash::re_alloc()
{
  int k = hash_len +10000;
  String* T1 = new String[k];
  int* T2 = new int[k];
  LabelInfo** T3 = new LabelInfo*[k];
  String* T4 = new String[k];
  for(int i=0;i<hash_len;i++) {
    T1[i] = Text[i];
    T2[i] = Next[i];
    T3[i] = Labinfo[i];
    T4[i] = Value[i];
  }
  delete[] Text; Text = T1;
  delete[] Next; Next = T2;
  delete[] Labinfo; Labinfo = T3;
  delete[] Value; Value = T4;
  for(int i=hash_len;i<k;i++) {
    Text[i] = 0;
    Next[i] = 0;
    Labinfo[i] =0;
    Value[i] =0;
  }
  hash_len = k;
  the_log << "Realloc str hash to " << k <<"\n";
}

// Find something in the StrHash table. The buffer mybuf holds the string 
// to search. result is never zero
int StrHash::hash_find()
{
  the_parser.my_stats.one_more_sh_find();
  if(!mybuf[0]) return 1;
  int p = mybuf.hashcode(hash_prime) + 3;
  for(;;) {
    if(Text[p] && mybuf == Text[p]) return p;
    if(Next[p]) p = Next[p];
    else break;
  }
  String name = mybuf.convert_to_str();
  String value = mybuf.convert_to_out_encoding(name);
  if(!Text[p]) {
    the_parser.my_stats.one_more_sh_used();
    Text[p] = name;
    Value[p] = value;
    return p;
  }
  if(hash_last>=hash_len) re_alloc();
  int k = hash_last;
  hash_last++;
  the_parser.my_stats.one_more_sh_used();
  Text[k] = name;
  Value[k] = value;
  Next[p] = k;
  return k;
}

// The string can be a temporary
int StrHash::find(String s) 
{
  mybuf << bf_reset << s;
  return hash_find();
}

// The string can be a temporary
int StrHash::find(const string& s) 
{
  mybuf << bf_reset << s;
  return hash_find();
}

// Converts s into a string and returns its hash table location.
int StrHash::find(int s) 
{
  mybuf << bf_reset << s;
  return hash_find();
}

// if s is the integer associard to 15pt, returns its hash location.
Istring StrHash::find_scaled(ScaledInt s)
{
  mybuf.reset();
  mybuf.push_back(s, glue_spec_pt);
  return Istring(hash_find());
}

void Buffer::push_back(const Istring& X)
{
  int v = X.get_value();
  if(!v) return;
  if(v==1) return;
  push_back(X.p_str());
}

// Prints a token list.
void token_ns::show (const TokenList&L)
{
  Buffer B;
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    B.insert_token0(*C); 
    cout << B.c_str() << " ";
    ++C;
  }
}

// True if L has a single token 
bool token_ns::has_a_single_token(const TokenList& L)
{
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  if(C==E) return false;
  ++C;
  if(C==E) return true;
  return false;
}

// True if L has a single token that is T
bool token_ns::has_a_single_token(const TokenList& L, Token t)
{
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  if(C==E) return false;
  if(*C != t) return false;
  ++C;
  if(C==E) return true;
  return false;
}

// Removes first and last spaces in a token list.
void token_ns::remove_first_last_space(TokenList&L)
{
  while(!L.empty() && L.front().is_space_token())
    L.pop_front();
  while(!L.empty() && L.back().is_space_token())
    L.pop_back();
}
// finishes a sequence of characters.
void Logger::finish_seq()
{
  if(!finished) { 
    *fp << buffer_for_log2.convert_to_log_encoding();
    *fp<< ".\n"; 
    finished = true; 
  }
}

// starts a sequence of characters if needed, adds character c
void Logger::out_single_char(Utf8Char c)
{
  if(finished) {
    finished = false;
    buffer_for_log2.reset();
    buffer_for_log2 << "Character sequence: "; 
  }
  buffer_for_log2 << c;
}

Logger& operator<<(Logger&X, const Macro&x)
{
  *(X.fp) << x;
  return X;
}

void Logger::dump (String s) 
{ 
  finish_seq(); 
  *fp << "{\\" << s << "}\n";
}

void Logger::dump0 (String s) 
{ 
  finish_seq(); 
  *fp << "{" << s << "}\n";
}


// This prints a control sequence value on the log file.
// Used when tracing a command (catcode not 11 nor 12)
// used in the case {\let\x\y}, after the closing brace.
// It it's not a char, it's a command, with a plain ASCII name.
void Parser::print_cmd_chr(CmdChr X)
{
  String a = X.special_name();
  String b = X.name();
  if(a&& b) { // print both values
    the_log << "\\" << b << " " << a;
    return;
  }
  if(a) { // chr 
    the_log << a;
    Utf8Char y =  X.get_chr();
    Buffer&B = buffer_for_log;
    B.reset();
    B.out_log(y,the_main->get_log_encoding());
    return;
  }
  if(b) the_log << "\\" << b;
  else the_log << "(Unknown)";
}

// ------------------------ token lists ----------------------------

// kill L. If it has 1 element, returns it, otherwise 0.
Token token_ns::get_unique(TokenList& L)
{
  if(L.empty()) return Token(0);
  Token x = L.front();
  L.pop_front();
  if(!L.empty()) x.kill();
  L.clear();
  return x;
}

// kill L. If it has 1,2 element, put in t1,t2
void token_ns::get_unique(TokenList& L,Token&t1, Token&t2)
{
  t1 = Token(0);
  t2 = Token(0);
  if(L.empty()) return;
  t1 = L.front();
  L.pop_front();
  if(L.empty()) return;
  t2 = L.front();
  L.pop_front();
  if(!L.empty()) t1.kill();
  L.clear();
}

// insert an open brace at the beginning, a close brace at the end.
void Parser::brace_me(TokenList&L)
{
  L.push_front(hash_table.OB_token);
  L.push_back(hash_table.CB_token); 
}

void token_ns::add_env(TokenList&L, String name)
{
  TokenList L1 = string_to_list(name,true);
  TokenList L2 = L1;
  TokenList res;
  res.push_back(the_parser.hash_table.begin_token);
  res.splice(res.end(),L1);
  res.splice(res.end(),L);
  res.push_back(the_parser.hash_table.end_token);
  res.splice(res.end(),L2);
  swap(L,res);
}

void Buffer::dump_prefix(bool err, bool gbl,symcodes K)
{
  if(gbl) push_back("\\global");
  if(K==user_cmd) return;
  if(K==userp_cmd || K==userlp_cmd ||  K==userop_cmd || K==userlop_cmd) {
    if(err) push_back("\\"); else insert_escape_char_raw();
    push_back("protected");
  }
  if(K==userl_cmd || K==userlo_cmd ||  K==userlp_cmd || K==userlop_cmd) {
    if(err) push_back("\\"); else insert_escape_char_raw();
    push_back("long");
  }
  if(K==usero_cmd || K==userlo_cmd ||  K==userop_cmd || K==userlop_cmd) {
    if(err) push_back("\\"); else insert_escape_char_raw();
    push_back("outer");
  }
  push_back(' ');
}


// --------------------------------------------------
// Finds the first m at brace level 0; before in Z, after in L
bool token_ns::split_at(Token m,TokenList&L, TokenList& z)
{
  z.clear();
  int bl = 0;
  while(!L.empty()) {
    Token x = L.front();
    L.pop_front();
    check_brace(x,bl);
    if(bl==0 && x== m) return true;
    z.push_back(x);
  }
  return false;
}

// For all level-zero characters c, use a category code 12 char instead
void token_ns::sanitize_one(TokenList &L, uchar c)
{
  Token T = Token(other_t_offset,c);
  token_iterator C = L.begin();
  token_iterator E = L.end();
  int bl = 0;
  while(C != E) {
    Token x = *C;
    check_brace(x,bl);
    if(bl==0 && x.is_a_char() && x.char_val()==c) *C = T;
    ++C;
  }
}

// Replace in L all character tokens by  category code 12 ones
// All other tokens are discarded
void token_ns::sanitize_one(TokenList &L)
{
  TokenList res;
  token_iterator C = L.begin();
  token_iterator E = L.end();
  while(C != E) {
    Token x = *C;
    if(x.is_a_char())  res.push_back(Token(other_t_offset, x.char_val()));
    ++C;
  }
  L.swap(res);
}


// For all characters c in s, at level at most n
// use a category code 12 char instead
void token_ns::sanitize_one(TokenList &L, TokenList& s, int n)
{
  token_iterator C = L.begin();
  token_iterator E = L.end();
  int bl = 0;
  while(C != E) {
    Token x = *C;
    check_brace(x,bl);
    if(bl<=n && x.is_a_char()) {
      Utf8Char c = x.char_val();
      token_iterator sC = s.begin();
      token_iterator sE = s.end();
      while(sC != sE) {
	if(sC->char_val() == c) *C = *sC;
	break;
	++sC;
      }
    }
    ++C;
  }
}

// The name of the command is misleading: there is a single hack
void token_ns::double_hack(TokenList& key)
{
  remove_first_last_space(key);
  remove_ext_braces(key);
}

// \tralics@split{L}\A\B{u=v,w} expands into
// \A{Lu}{v}\B{Lw}
void Parser::scan_split() 
{
  Token T = cur_tok;
  TokenList prefix = mac_arg();
  TokenList cmd = mac_arg();
  TokenList cmd_def = mac_arg();
  TokenList L= mac_arg();
  TokenList R;
  Token x1 = hash_table.equals_token;
  Token x2 = Token(other_t_offset,',');
  TokenList key,val;
  for(;;) {
    if(L.empty()) break;
    token_ns::split_at(x2,L,val);
    bool seen_val = token_ns::split_at(x1,val,key);
    token_ns::double_hack(key);  
    if(key.empty()) continue;
    { TokenList tmp = prefix;  key.splice(key.begin(), tmp); }
    brace_me(key);

    token_ns::double_hack(val);  
    if(seen_val) brace_me(val);
    TokenList tmp = seen_val ?cmd : cmd_def; 
    R.splice(R.end(), tmp);
    R.splice(R.end(), key);
    if(seen_val) R.splice(R.end(), val);
  }
   if(tracing_macros()) 
     the_log << lg_start << T << lg_arrow << R << lg_end;
   back_input(R);
}

int token_ns::length_normalise(TokenList& L)
{
  Token u =Token(space_t_offset+'\n');
  Token v =Token(space_t_offset+' ');
  token_iterator A = L.begin();
  token_iterator B = L.end();
  int n = 0;
  while(A !=B) {
    if(*A==u) *A=v;
    ++n;
    ++A;
  }
  return n;
}

bool token_ns::is_sublist(token_iterator A, token_iterator B, int n)
{
  while(n>0) {
    if(*A != *B) return false;
    ++A; ++B; --n;
  }
  return true;
}


// Returns true if A is in B. If the switch is true, the value is removed
// but the last token of B is not
// Counts the number of skipped commas.
bool token_ns::is_in(TokenList& A, TokenList&B, bool remove)
{
  int n= length_normalise(A);
  int m= length_normalise(B);
  int k= m-n;
  if(k<0) return false;
  token_iterator AA = A.begin();
  token_iterator BB = B.begin();
  bool found = false;
  int skipped = -1;
  Token to_skip = A.front();
  while(k>=0) {
    if(*BB==to_skip) ++skipped;
    if(is_sublist(AA,BB,n)) { 
      found = true; 
      break;
    }
    ++BB; 
    --k;
  }
  if(remove && found) {
    token_iterator CC=BB;
    --n;
    while(n>0) { ++CC; --n; }
    B.erase(BB,CC);
  }
  is_in_skipped = found ? skipped : -1;
  return found;
}

