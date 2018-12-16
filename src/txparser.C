// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains the TeX parser of tralics
// 

#include "tralics.h"
const char* txparser_rcsid=
  "$Id: txparser.C,v 2.148 2015/07/29 16:10:40 grimm Exp $";

namespace {
  const TokenList empty_list;
  Buffer trace_buffer;
  Buffer Thbuf1, Thbuf2, fetch_name_res;
}

namespace token_ns {
  void strip_pt(TokenList& L);
}

Token invalid_token, runawayed_token;

inline String boolean(bool x)
{
  return x ? "true" : "false";
}

// --------------------------------------------------
// Reading arguments, groups, etc.

// There are two parameters that control what happens in case 
// an outer cmd or a \par is seen.
// If scanner_status is ss_normal then an outer command is valid
// Otherwise an error is signaled when the token is created or removed
// from TL. It will be replaced by runawayed_token. Error signaled by get_next


// If long_state is ls_normal then \par is OK, otherwise an error is signaled
// by scan_group0 or scan_group1 (defined below)
// They return true in case of error and scanning stops.

// Adjusts the brace counter b. Returns true if it becomes 0.
// The scanner returns only balanced lists.
bool Parser::check_brace(int& b)
{
  if(cur_tok.is_a_brace()){
    if(cur_tok.is_a_left_brace()) b++;
    else {
      b--; 
      if(b==0) return true ;
    }
  }
  return false;
}

// Assumes an open brace has been read. Reads in res everything up to the 
// matching close brace. Returns true in case of error (cl is start line). 
bool Parser::scan_group0 (TokenList&res, int cl)
{
  int b = 1; // brace counter, we start with 1
  for(;;) {
    if(get_token()) { missing_close_brace(cl); return true; } 
    if(check_brace(b)) return false;
    if(cur_tok==runawayed_token) return true;
    if(cur_tok==hash_table.par_token && long_state==ls_normal) {
      runaway(cl); 
      return true;
    }
    res.push_back(cur_tok);
  }
}

// Same as the function above, but with one argument less
void Parser::skip_group0 (TokenList& L)
{
  scan_group0(L,get_cur_line());
}

// Same as above, tokens may be expanded  if exp is true
// We are reading a macro body with N arguments, so that #1 must be interpreted
// Paragraphs are allowed
void Parser::scan_group3 (TokenList&res, int cl,bool exp, int N)
{
  int b = 1;
  for(;;) {
    bool stop = exp ? edef_aux(res) : get_token();
    if(stop) { missing_close_brace(cl); return; }
    if(check_brace(b)) return;
    if(cur_cmd_chr.is_parameter()) after_parameter(exp,N);
    res.push_back(cur_tok);
  }
}

// As above, but there is no closing brace: we read until end of data.
// Thus if check_brace() should not yield true 
// there are no arguments, and we expand

void Parser::scan_group4 (TokenList&res, int cl)
{
  int b = 1;
  for(;;) {
    if(edef_aux(res)) { if(b!=1) missing_close_brace(cl);  return; }
    if(check_brace(b)) { parse_error("Too many closing braces"); return; }
    if(cur_cmd_chr.is_parameter()) after_parameter(true,0);
    res.push_back(cur_tok);
  }
}

// Assumes an open brace has been read. Like skip_group0, returns nothing.
void Parser::ignore_group0 ()
{
  int b = 1;
  int cl = get_cur_line();
  for(;;) {
    if(get_token()) { missing_close_brace(cl); return; }
    if(check_brace(b)) return;
    if(cur_tok==runawayed_token) return;
    if(cur_tok==hash_table.par_token && long_state==ls_normal) {
      runaway(cl); 
      return;
    }
  }
}

// Contribute a group or a single token to the list res.
// Quantity b is 0 is res is formed of a single group, and -1 if empty
// We increment b if we read a group, set b=2 if we read a token.
// Returns true in case of error.
bool Parser::scan_group1(TokenList& res, int& b, int cl)
{
  Token aux = cur_tok;
  if(aux.is_OB_token()) {
    res.push_back(cur_tok); // Insert the opening brace
    bool retval = scan_group0(res,cl); // This may fail
    res.push_back(hash_table.CB_token); // Insert the closing brace
    b++;
    return retval;
  } else if(aux.is_CB_token()) {
    extra_close_brace(cl);
    return true;
  } else { 
    if(aux==hash_table.par_token && long_state==ls_normal) {
      runaway(cl);
      return true;
    }
    b = 2;
    res.push_back(aux); 
    return false;
  }
}

// In the case of \foo {x} and \foo x, this command reads an optional space
// reduces the second case to the first one, and reads an open brace.
// Returns true in case of error. Is used in math mode. Normal arguments
// are read via 
//    if(!before_mac_arg()) skip_group0(L);
// or optimised version: scan_group2(L) 

bool Parser::before_mac_arg(Token T)
{
  for(;;) {
    if(get_token()) {
      runaway(-1);
      return true; 
    }
    if(cur_tok.is_space_token()) continue;
    if(cur_tok.is_OB_token()) return false;
    if(cur_tok.is_CB_token()) {
      parse_error("Missing { inserted before unexpected } in argument of ",
		  T,"","extra}");
      back_input(cur_tok);
      return true;
    }
    back_input(hash_table.CB_token);
    back_input(cur_tok);
    return false;
  }
}

// Optimised version of mac_arg. This is the command that reads an argument;
// note that outer braces disappear
void Parser::scan_group2(TokenList&L)
{
  int cl = get_cur_line();
  for(;;) {
    if(get_token()) {
      runaway(-1);
      return; 
    }
    if(cur_tok.is_space_token()) continue;
    if(cur_tok.is_OB_token()) {
      scan_group0(L,cl);
      return;
    }
    if(cur_tok.is_CB_token()) {
      parse_error("Missing { inserted before unexpected } in argument of ",
		  name_for_error,"","extra}");
      return;
    }
    L.push_back(cur_tok);
    return;
  }
}

// Special case of read_delimited, in case a single token is wanted.
// Gathers all tokens until finding x
// If this is a group (b==0), then outer braces will be removed;
void Parser::read_until(TokenList&res, Token x)
{
  int cl = get_cur_line();
  int b = -1;
  for(;;) {
    if(get_token()) { bad_delimited(cl,x); break; }
    if(cur_tok==x) break;
    if(cur_tok==hash_table.newline_token && x==hash_table.space_token) break;
    if(cur_tok==runawayed_token) break;
    if(scan_group1(res,b,cl)) break;
  }
  if(b==0) { // Remove external braces
    res.pop_back();
    res.pop_front();
  }
}

// Internal code of read_delimited: reads all tokens until finding L
void Parser::scan_group_del (TokenList&res,const TokenList& L)
{
  int cl = get_cur_line();
  int b = -1;
  for(;;) {
    if(is_delimiter(L)) break;
    if(get_token()) { bad_delimited(cl,Token(0)); break; }
    if(cur_tok==runawayed_token) break;
    if(scan_group1(res,b,cl)) break;
  }
  if(b==0) { // Remove external braces
    res.pop_back();
    res.pop_front();
  }
}

// Reads a LaTeX optional argument, enclosed in brackets
// Initial spaces are discarded. Return false in no bracket found
bool Parser::scan_group_opt(TokenList& L)
{
  bool retval = false;
  for(;;) {
    if(get_token()) break; // end of data
    if(cur_tok.is_space_token()) continue;
    if(cur_tok.is_open_bracket())
      retval = true;
    else back_input(); 
    break;
  }
  if(retval) read_until(L,Token(other_t_offset,']'));
  return retval;
}

// Note: scan_group_opt, scan_group2, scan_group_del, 
// are used by expand_mac that temporarily resets scanner_status
// Functions below, in general, set it to ss_matching at the start, 
// and normal at the end.

bool Parser::next_optarg(TokenList& L)
{
  scanner_status = ss_matching;
  bool retval= scan_group_opt(L);
  scanner_status = ss_normal;
  return retval;
}

void Parser::ignore_next_optarg()
{
  TokenList L;
  SaveLongState sth(ls_normal); // \par forbidden
  next_optarg(L);
}

// The three next commands set locally long_state to normal
// Reads an optional argument in L, true if found
bool Parser::next_optarg_long(TokenList& L)
{
  SaveLongState sth(ls_normal);
  return next_optarg(L);
}

// Read a normal argument
TokenList Parser::mac_arg_long ()
{
  SaveLongState sth(ls_normal);
  return mac_arg();
}

// Read an argument delimited by a token
TokenList Parser::read_until_long(Token x)
{  
  SaveLongState sth(ls_normal);
  return read_until(x);
}

// This reads all tokens before x. The scanner status is set to matching
// and to normal at the end.
TokenList Parser::read_until(Token x)
{
  TokenList res;
  scanner_status = ss_matching;
  read_until(res,x);
  scanner_status = ss_normal;
  return res;
}

// Argument of macro. A single token, or a brace delimited non-expanded list
TokenList Parser::mac_arg ()
{
  TokenList L;
  scanner_status = ss_matching;
  scan_group2(L);
  scanner_status = ss_normal;
  return L;
}

// In a case like \def\bar#1foo#2{...}, this returns the value of #1
// given the token list foo as argument. 
TokenList Parser::read_delimited (const TokenList& L)
{
  TokenList res;
  scanner_status = ss_matching;
  scan_group_del(res,L);
  scanner_status = ss_normal;
  return res;
}

// Reads the body of a \def ou \xdef. A space is skipped, 
// and an open brace is read first.
void Parser::read_mac_body(TokenList& L, bool exp, int N)
{
  scanner_status = ss_defining;
  skip_initial_space();
  if(cur_cmd_chr.is_open_brace()) 
    scan_group3(L,get_cur_line(),exp,N);
  else missing_open_brace(); 
  scanner_status = ss_normal;
}

// --------------------------------------------------

// Adds the content of the buffer to the document-hook token list.
void Parser::add_buffer_to_document_hook(Buffer& b,const string& name)
{
  add_buffer_to_list(b,document_hook,name);
}

// Evaluates now a token string. First action is to put chars in a buffer
// (because we add a '\n' at the end of the string).
void Parser::evaluate_now(String s, const string& cmd)
{
  mac_buffer << bf_reset << s;
  evaluate_now(mac_buffer, "(tpa post " + cmd + ")");
}

// This tokenizes the buffer and evaluates it.
void Parser::evaluate_now(Buffer& b, const string& s)
{
  TokenList L = empty_list;
  add_buffer_to_list(b,L,s);
  T_translate(L);
}

// Puts in cur_tok the next non-expandable token.
bool Parser::get_x_token()
{
  for(;;) {
    if(get_token()) return true;
    if(cur_cmd_chr.is_expandable()) expand();
    else return false;
  }
}

// Returns true if the next line contains \end{env}
// In this case, removes the \end{env}
bool Parser::is_verbatim_end ()
{
  main_ns::file_ended = false;
  if(at_eol()) get_a_new_line();
  if(main_ns::file_ended) return false;
  bool res = input_buffer.contains_env(get_cur_env_name().c_str());
  if(res) {
    if(!input_buffer.head())
      kill_line();
    else {
      int n = input_line.size();
      for(int i=0;i<n;i++) if(input_line[i]=='}') { input_line_pos=i+1; break;}
    }
  }
  return res;
}

// Interprets some characters in verbatim mode (sometimes \catcode 11)
// result is something like {\tt foo}, added at the beginning or end of L.
// Sets ok to false in case of error. 
// Stops reading at EOL or character test.

void Parser::vb_tokens(bool& ok, Utf8Char test, TokenList& L, bool before)
{
  TokenList res;
  state = state_M;
  for(;;) {
    if(at_eol()) { ok = false; break; }
    Utf8Char c = input_line[input_line_pos++];
    if(c== test) { ok = true; break; } 
    res.push_back(verbatim_chars[c.get_value()]);
  }
  if(res.empty() && !before) res.push_back(hash_table.tilda_token);
  res.push_front(hash_table.verbatim_font);
  brace_me(res);
  if(before) res.push_front(hash_table.verb_prefix_token);
  else res.push_front(hash_table.verbatim_prefix_token);
  if(before) L.splice(L.begin(),res);
  else L.splice(L.end(),res);
}

// Adds \par\noindent to the token list
void token_ns::add_par_noindent(TokenList&L, const Hashtab& H)
{
  L.push_back(H.par_token);
  L.push_back(H.noindent_token);
}

// Case of \begin{verbatim} and others
void Parser::expand_verbatim (int my_number, Token style, Token pre,Token post)
{
  TokenList res;
  kill_line();
  int cc = eqtb_int_table[endlinechar_code].get_val();
  eqtb_int_table[endlinechar_code].set_val('\r');

  bool ok = true;
  int n = 0;
  bool want_number = false;
  if(my_number>=0) {
    n = eqtb_int_table[my_number+ count_reg_offset].get_val();
    want_number = true;
  }
  token_ns::add_par_noindent(res,hash_table);
  int cl = get_cur_line();
  for(;;) {
    if(is_verbatim_end()) break;
    if(main_ns::file_ended) {
      ok = false;
      break;
    }
    n++;
    if(want_number) { 
      word_define(my_number+ count_reg_offset,n,true);
      token_ns::add_verbatim_number(res,hash_table,n);
    } 
    vb_tokens(ok,'\r',res,false);
    if(!ok) break;
    token_ns::add_par_noindent(res,hash_table);
  }
  if(!ok) bad_end_env(cl);
  eqtb_int_table[endlinechar_code].set_val(cc);
  pop_level(true,bt_env);
  remove_initial_space_and_back_input();
  bool hack_par = false; // true in case empty line after verbatim
  if(cur_cmd_chr.get_cmd() == par_cmd) {
    // If \end{verbatim} is followed by \par, remove it and the \noindent.
    get_token();
    res.pop_back();
    hack_par = true;
  }
  if(style != hash_table.relax_token) res.push_front(style);
  if(pre != hash_table.relax_token) res.push_front(pre);
  if(post != hash_table.relax_token) {
    // move \par\noindent after the block
    if(!hack_par) res.pop_back();
    res.pop_back();
    res.push_back(post);
  }
  if(post != hash_table.relax_token) {
    res.push_back(hash_table.par_token);
    if(!hack_par)  res.push_back(hash_table.noindent_token);
  }
  if(tracing_commands())
    the_log << lg_startbrace << "Verbatim tokens: " << res << lg_endbrace;
  back_input(res);
}

// A number N gives: {\verbatimnumberfont{N}}\space
void token_ns::add_verbatim_number(TokenList&L, const Hashtab& H, int n)
{
  L.push_back(H.OB_token);
  L.push_back(H.verbatim_number_font);
  L.push_back(H.OB_token);
  push_back_i(L,n);
  L.push_back(H.CB_token);
  L.push_back(H.CB_token);
  L.push_back(H.space_token);
}

// This is not complete...

void Parser::special_verbatim()
{
  bool extended = cur_cmd_chr.get_chr()== one_code;
  bool noparse = cur_cmd_chr.get_chr()== two_code;
  bool optional = false;
  if(extended || noparse) {
    // do not read the next line (unless it's a %)
    for(;;) {
      if(get_token()) break;
      if(cur_tok.is_open_bracket()) {
	optional = true;
	back_input();
	break;
      }
      if(cur_tok.is_only_space_token()) continue; // ok for space, not newline.
      else { back_input(); break; }
    }
  }
  // Now, we know if we have an optional argument.
  TokenList largs;
  if(optional) next_optarg(largs);
  string hook = get_cur_env_name() + "@hook";
  if (noparse) {
    Token t1 = hash_table.locate(hook);
    new_macro(largs, t1, true); // definition is outside env
    Token t2 = hash_table.locate("FV@style@lst");
    Token t3 = hash_table.locate("FV@pre@lst");
    Token t4 = hash_table.locate("FV@post@lst");
    expand_verbatim(-1,t2,t3,t4);
    return;
  }
  TokenList lopt = get_mac_value(hook);
  largs.push_back(hash_table.comma_token);
  largs.splice(largs.end(),lopt);
  string args = exp_token_list_to_string(largs);
  bool want_number = false;
  SpecialHash S(args);
  //  Check if numbering wanted
  if(!S.find("numbers").empty()) want_number = true;
  if (!extended&&numbered_verbatim) want_number = true;
  bool reset = true;
  if(!extended) reset = false;
  // Check if a register number is given
  int reg_number = S.find_counter();
  if(reg_number>=0) reset = false;
  // Check if a start number is given
  int n = 0;
  string w = S.find("firstnumber");
  if(w.empty()) n = 1;
  else if(w=="last") { reset = false; want_number = true; }   
  else if(tralics_ns::only_digits(w)) 
    { reset = true; n = atoi(w.c_str()); want_number = true;}
  else { reset = true; n = 0; }
  if(want_number && reg_number<0) reg_number = 21; // hardcoded
  if(reset) 
    word_define(reg_number+ count_reg_offset,n-1,true);
  Token t2 = hash_table.relax_token; 
  Token t3 = hash_table.relax_token; 
  Token t4 = hash_table.relax_token; 
  string w2 = S.find("style");
  string w3 = S.find("pre");
  if(!w2.empty()) t2 = hash_table.locate("FV@style@"+w2);
  if(!w3.empty()) t3 = hash_table.locate("FV@pre@"+w3);
  if(!w3.empty()) t4 = hash_table.locate("FV@post@"+w3);
  expand_verbatim(reg_number,t2,t3,t4);
}

// Still incomplete
void Parser::special_fvset()
{
  flush_buffer();
  string args = sT_next_arg();
  {
    static const uint loc = uchar(' ');
    SpecialHash S(args);
    string s = S.find("showspaces");
    if(s.empty()) return;
    if(s == "true") verbatim_chars[loc] = hash_table.textvisiblespace_token;
    else if(s == "false") verbatim_chars[loc] = hash_table.tilda_token;
    else return;
  }
}


// We use RestoreVbSpace for changing temporarily expansion of space
inline RestoreVbSpace::RestoreVbSpace(Parser*p) 
{
  P = p;
  value = P->verbatim_chars[uchar(' ') ]; 
}

inline RestoreVbSpace::~RestoreVbSpace() 
{
  P->verbatim_chars[uchar(' ')] = value; 
}

inline void RestoreVbSpace::operator()(Token T) 
{ 
  P->verbatim_chars[uchar(' ')] = T; 
}

// In the case of \verb+foo+, reads and returns the + character.
// Sets special_space to true if spaces have to be treaded specially
// sets ok to false in case of trouble
Utf8Char Parser::delimiter_for_verb(bool& special_space, bool& ok)
{
  Utf8Char t = input_line[input_line_pos++];
  if(!(t =='*'))  return t;
  if(at_eol()) { ok = false; return t; }
  special_space = true;
  return  input_line[input_line_pos++];
}

// In the case of \SaveVerb{ok}+foo+, reads and returns the + character.
// sets ok to false in case of trouble
Utf8Char Parser::delimiter_for_saveverb(bool& ok)
{
  ok = false;
  for(;;) {
    if(at_eol()) return 0;
    Utf8Char c = input_line[input_line_pos++];
    if(c==0) return c;
    if(c.is_big()) return c;
    if(fast_catcode(c.get_value())== space_catcode) continue;
    if(fast_catcode(c.get_value())==16) ok = true;
    return c;
  }
}

// Case of the \verb command (t=0) or |foo|, where t is the char
void Parser::expand_verb0 (Utf8Char t)
{
  if(!TL.empty()) { 
    verb_error(true,0); 
    return;
  }
  if(at_eol ()) { verb_error(true, 1); return; }
  bool special_space=false, ok=true;
  if(t.is_null()) t = delimiter_for_verb(special_space,ok);
  if(!ok) { verb_error(true, 1); return; }
  RestoreVbSpace X(this);
  if(special_space)
    X(hash_table.textvisiblespace_token);
  vb_tokens(ok,t,TL,true);
  if(!ok) verb_error(true,2);
}

// Case of the \SaveVerb command
void Parser::expand_verb1 (TokenList& res)
{
  if(!TL.empty()) { verb_error(false, 0); return; }
  bool ok;
  Utf8Char t = delimiter_for_saveverb(ok);
  if(!ok) { verb_error(false,3); return; }
  if(at_eol ()) { verb_error(false,1); return; }
  vb_tokens(ok,t,res,true);
  if(!ok) verb_error(false,2);
}

// The env macro will read the \end{...}, so that we pop the stack.
// we make sure that reading is not from a token list
// Ignores everything, until end of environment.
// or take it verbatim
string Parser::ignore_env(bool want_result)
{
  kill_line();
  if(!TL.empty()) { 
    parse_error("Verbatim-like environment in argument : ", 
		get_cur_env_name(),
		"Verbatim-like environment in argument");
    pop_level(true,bt_env);
    return "";
  }
  mac_buffer.reset();
  int cl = get_cur_line();
  for(;;) {
    if(is_verbatim_end()) break; 
    if(main_ns::file_ended) {
      bad_end_env(cl);
      break;
    }
    if(want_result) {
      mac_buffer.push_back(input_buffer.c_str());
      mac_buffer.push_back('\n');
    }
    kill_line();
  }
  pop_level(true,bt_env);
  return want_result ? mac_buffer.to_string() : "";
}

// --------------------------------------------------
// Space hacking 

void Parser::back_input_unless_space()
{
  if(cur_tok.is_invalid()) return;
  if(cur_cmd_chr.is_space()) return;
  back_input();
}

void Parser::read_one_space()
{
  get_x_token();
  back_input_unless_space();
}

// This sets cur_tok to a token that has to be handled.
void Parser::remove_initial_space()
{
  for(;;) {
    get_x_token();
    if(!cur_cmd_chr.is_space()) return;
  }
}

void Parser::remove_initial_space_and_back_input()
{
  for(;;) {
    get_x_token();
    if(!cur_cmd_chr.is_space()) break;
  }
  if(!cur_tok.is_invalid()) back_input();
}

// This sets cur_tok to a token that has to be handled.
void Parser::remove_initial_space_relax()
{
  for(;;) {
    get_x_token();
    if(!cur_cmd_chr.is_space() && !cur_cmd_chr.is_relax()) return;
  }
}

// No expand here ...
// This sets cur_tok to a token that has to be handled.
void Parser::skip_initial_space()
{
  for(;;) {
    get_token();
    if(!cur_cmd_chr.is_space()) return;
  }
}

void Parser::skip_initial_space_and_back_input()
{
  skip_initial_space();
  back_input();
}

// Case of for instance: \catcode`A \space=2.
// Reads up to the equals sign
void Parser::scan_optional_equals()
{
  for(;;) {
    if(get_x_token()) return;
    if(!cur_cmd_chr.is_space()) break;
  }
  if(!cur_tok.is_equal_token())
    back_input();
}

// Scans the left brace in the case of \hbox{}.
void Parser::scan_left_brace()
{
  remove_initial_space_relax();
  if(!cur_cmd_chr.is_open_brace())  {
    parse_error("A left brace was mandatory here, so I've put one in",
		"Missing open brace");
    if(!cur_tok.is_invalid()) back_input();
  }
}

// This scans a brace. Emits an error in case of missing brace.
// No token is added.
void Parser::scan_left_brace_and_bi()
{
  scan_left_brace();
  back_input(hash_table.OB_token);
}

// Reads an optional *. Returns true if we have seen one.
bool Parser::remove_initial_star ()
{
  skip_initial_space();
  if(cur_tok.is_star_token())
     return true;
  if(!cur_tok.is_invalid()) back_input();
  return false;
}

// In the case of \edef, all tokens are expanded, unless they come from the
// \the command, these are appended to L by this procedure. 
// 
// Returns true if EOF.
bool Parser::edef_aux(TokenList& L)
{
  for(;;) {
    if(get_token()) return true;
    else if(!cur_cmd_chr.is_expandable()) return false;
    else if(cur_cmd_chr.get_cmd() >=userp_cmd) return false; // protected
    else if(cur_cmd_chr.get_cmd() != the_cmd) expand();
    else { 
      TokenList q = the_toks(); 
      if(tracing_commands()) 
	the_log << lg_start<< "\\the->" << q << "." << lg_end;
      L.splice(L.end(),q);
    }
  }
}

// This is done if we see a # while reading the body of a command.
// if exp is true, the token that follows the # is expanded,
void Parser::after_parameter(bool exp, int N)
{
  Token s = cur_tok;
  bool stop = exp ? get_x_token() : get_token();
  if(stop) {
    parse_error("end of data instead of a number after #");
    return;
  }
  if(cur_cmd_chr.is_parameter()) 
    {} //  ## gives #
  else {
    if(cur_tok.is_digit_token()) {
      int n = cur_cmd_chr.val_as_digit();
      if(n==0 || n>N) {
	parse_error("bad token (", cur_tok, ") after #", "bad after #"); 
	n=1;
      }
      cur_tok = Token(eol_t_offset+n);
    }
    else {
      parse_error("bad token (", cur_tok, ") after #", "bad after #"); 
      back_input(cur_tok);
      cur_tok = s;
    }
  }
}


// The resulting token list of \mark is unused.
void Parser::expand_mark(subtypes c)
{
  Token T = cur_tok;
  if(c==marks_code) scan_int(T);
  TokenList Lone = scan_general_text();
  scan_toks_edef(Lone);
}

// This feeds the list L to the body of an edef.
// The expansion of L will be in L. The loop stops at the end of L
// at brace_level zero (b=1)
void Parser::scan_toks_edef(TokenList& L)
{
  scanner_status = ss_absorbing; // Could be ss_defining;
  SaveState sv_state;
  save_the_state(sv_state); // now TL is empty
  restricted = true;
  TL.swap(L);// now L is empty
  scan_group4(L,get_cur_line()); // This fills L
  restore_the_state(sv_state);
  scanner_status = ss_normal;
}

// This is for \ifthenelse{\equal{\foo}{\bar}}. We have to fully expand
// the \foo and \bar.
void Parser::special_read_mac_body(TokenList& L)
{
  SaveState st_state;
  save_the_state(st_state);
  restricted = true;
  brace_me(L); // now L holds {\foo}
  TL.swap(L); 
  read_mac_body(L,true,0);
  restore_the_state(st_state); 
}

string Parser::get_env ()
{
  TokenList L = mac_arg();
  if(L.empty()) return "";
  SaveState st_state;
  save_the_state(st_state);
  restricted = true;
  L.push_back(hash_table.frozen_endcsname);
  TL.swap(L); 
  fetch_name2();
  restore_the_state(st_state); 
  return fetch_name_res.to_string();
}

// Argument of begin/end. We want only characters
string Parser::group_to_string ()
{
  skip_initial_space();
  if(!cur_cmd_chr.is_open_brace()) { 
    missing_open_brace(); 
    return ""; 
  }
  group_buffer.reset();
  for(;;) {
    get_token();
    symcodes S = cur_cmd_chr.get_cmd();
    if(S==close_catcode) return group_buffer.to_string();
    if(S==space_catcode) group_buffer.push_back(' '); 
    else if(S>2 && S< 16)
      group_buffer.push_back(cur_cmd_chr.char_val());
    else { 
      bad_group_char(true); 
      return ""; 
    }
  }
}

// Backinput \end{argument} (without the \end token).
void Parser::unskip_group(String s)
{
  TokenList L = token_ns::string_to_list(s,true);
  back_input(L);
}

// Here, we expand all tokens, and gather all non-expandable tokens
// For simplicity, we convert [foo] into {foo} and try again.
string Parser::exp_token_list_to_string(TokenList& L)
{
  special_read_mac_body(L);
  group_buffer.reset();
  while(!L.empty()) {
    group_buffer.push_back(L.front());
    L.pop_front();
  }
  return group_buffer.to_string();
}


// Reads a token that can be outer for \ifx \ifdefined or show
bool Parser::get_token_o()
{
  SaveScannerStatus tmp(ss_normal);
  return get_token();
}

// Scan_general_text is like scan_tokens(false,false).
// It can be recursive, hence scanner_status not reset to normal
TokenList Parser::scan_general_text()
{
  TokenList L;
  scan_left_brace();
  SaveLongState sth(ls_bad); // allows \par
  SaveScannerStatus aux(ss_absorbing);
  scan_group0(L,get_cur_line());
  return L;
}

// Pargraphs are forbidden in ignored arguments
// Use mac_arg_long() if \par allowed 
void Parser::ignore_next_arg ()
{
  SaveLongState sth(ls_normal);
  skip_initial_space();
  if(cur_tok.is_invalid()) runaway(-1);
  else if(cur_tok.is_OB_token()) ignore_group0();
}



// Returns 0 in case of error.
// Should we call scan_int here ??
int Parser::get_nbargs ()
{
  TokenList L;
  next_optarg(L); 
  if(!L.empty() && L.front().is_plus_token()) L.pop_front();
  while(!L.empty() && L.front().is_zero_token()) L.pop_front();
  if(L.empty()) return 0; // case of \newcommand\x[]{x}
  Token t = token_ns::get_unique(L);
  if(t.is_null()) { bad_nbargs(-1); return 0; }
  if(!t.is_a_char()) { bad_nbargs(-2); return 0; }
  if(t.cmd_val()==other_catcode) {
    int tt = t.val_as_digit();
    if(tt>= 0 && tt<= 9)
      return tt;
  }
  bad_nbargs(-3); 
  return 0;
}

// scans the `#1#2foo' part of \def\bar#1#2foo{}
void Parser::get_def_nbargs (Macro* X, Token name)
{
  def_type code = dt_normal;
  int nb = 0;
  TokenList L;
  scanner_status = ss_defining;
  for(;;) {
    if(get_token())
      break; // will signal an error later
    if(cur_cmd_chr.is_parameter()) {
      get_token();
      if(cur_cmd_chr.is_other() && cur_cmd_chr.get_chr()==nb+'1') {
	X->set_delimiters(nb,L);
	L.clear();
	nb ++;
      } else if(cur_cmd_chr.is_open_brace()) {
	code = dt_brace;
	L.push_back(cur_tok);
	break;
      } else bad_definition(nb,name);
      continue;
    } 
    if(!cur_tok.is_a_char())  // \bgroup will not match here
      { code = dt_delim ; L.push_back(cur_tok); continue; }
    if(cur_cmd_chr.get_cmd() == open_catcode) break; // start of body
    if(cur_cmd_chr.get_cmd() == close_catcode)
      break; // will signal an error later
    else { code = dt_delim; L.push_back(cur_tok); }
  }
  scanner_status = ss_normal;
  X->set_delimiters(nb,L);
  X->set_nbargs(nb);
  X->set_type(code);
}

// Changes the type of a trivial macro.
void Macro::correct_type()
{
  if(type==dt_empty) type = dt_normal; // Is this needed ?
  if(type==dt_normal && body.empty() && nbargs==0) 
    type = dt_empty;
  if(type==dt_optional &&nbargs>1) {
    const TokenList& L = delimiters[1];
    const_token_iterator A=L.begin();
    const_token_iterator B=L.end();
    if(A==B) return;
    if(*A != the_parser.hash_table.dblarg_token) return;
    ++A;
    if(A!=B) return;
    type=dt_spec_opt;
  }
}

// \DeclareMathOperator*{foo}{bar} is
// \newcommand{\foo}{\operatorname*{bar}}
// The star is optional
void Parser::declare_math_operator ()
{
  bool see_star = remove_initial_star();
  get_r_token(true); // retval ignored
  Token name = cur_tok;
  if(tracing_commands()) 
    the_log << lg_startbracebs << "DeclareMathOperator "
	    << name << lg_endbrace;
  Macro* X = new Macro;
  X->set_type(dt_normal);
  TokenList& L= X->get_body();
  read_mac_body(L,false,0);
  brace_me(L);
  if(see_star) L.push_front(hash_table.star_token);
  L.push_front(hash_table.operatorname_token);
  X->correct_type();
  mac_define(name,X,true,rd_if_undef,user_cmd);
}

// This implements \newcommand
void Parser::get_new_command (rd_flag redef, bool gbl)
{
  Token T =  name_for_error; 
  bool b_long = remove_initial_star();
  get_r_token(true);
  Token name = cur_tok;
  name_for_error = name; 
  if(tracing_commands()) 
    the_log << lg_startbracebs <<(b_long ? "newcommand* " :  "newcommand ")
	    << name << lg_endbrace;
  Macro* X = get_new_mac();
  symcodes what = b_long ? user_cmd : userl_cmd;
  if(redef==rd_never) { // case \CheckCommand
    bool is_same = true;
    CmdChr pq = hash_table.eqtb[name.eqtb_loc()].get_cmdchr();
    if(pq.get_cmd() != what) is_same = false;
    else {
      Macro& B = mac_table.get_macro(pq.get_chr());
      is_same =  X->is_same(B);
    }
    if(!is_same) {
      Thbuf1 << bf_reset << "Tralics Warning: Command " << name << 
	" has changed\n";
      out_warning(Thbuf1,mt_warning);
    }
  }
  name_for_error = T;
  mac_define(name,X,gbl,redef,what);
}


// Code of \newenvironment{foo}...
void Parser::get_new_env (rd_flag redef, bool gbl)
{
  Token nfe = name_for_error;
  bool b_long = remove_initial_star();
  symcodes what = b_long ? user_cmd : userl_cmd;
  string name = group_to_string();
  if(tracing_commands()) 
    the_log << lg_startbracebs << "newenvironment " << name << lg_endbrace;
  Token T = find_env_token(name,true);  // this is \foo
  Macro* X = get_new_mac();
  name_for_error = nfe;
  mac_define(T,X,gbl,redef,what);
  T = find_env_token(name, false);
  name_for_error = T;
  X = new Macro;
  read_mac_body(X->get_body(),false,0);
  X->correct_type();
  name_for_error = nfe;
  mac_define(T,X,gbl,redef,what);
}

// Common code for \newcommand and \newenv, constructs the macro
Macro* Parser::get_new_mac()
{
  Macro* X = new Macro;
  int n = get_nbargs();
  X->set_nbargs(n);
  TokenList op_arg;
  bool have_op_arg = next_optarg(op_arg);
  X->set_delimiters(1,op_arg);
  X->set_type(have_op_arg ? dt_optional : dt_normal);
  read_mac_body(X->get_body(),false,n);
  X->correct_type();
  return X;
}

void Parser::T_thm_aux(int c)
{
  TokenList L = mac_arg();
  token_list_define(c,L,false);
}

// \let \a\b, c is true if global
void Parser::eval_let(Token a, Token b, bool c)
{
  back_input(b);
  back_input(a);
  see_let(c);
}

// \newtheorem{name}{text}[c]    case 1
// \newtheorem{name}{text}       case 2
// \newtheorem{name}[c]{text}    case 3
// \newtheorem*{name}{text}      case 4
void Parser::see_new_thm ()
{
  bool star = remove_initial_star ();
  string name = group_to_string(); 
  if(tracing_commands()) 
    the_log << lg_startbracebs << "newtheorem " << name << lg_endbrace;
  TokenList ctr;
  int which_case = star ? 4 : 2;
  if(which_case ==2 && next_optarg_long(ctr)) which_case = 3;
  TokenList text = mac_arg();
  if(which_case ==2 && next_optarg_long(ctr)) // ctr is empty here
    which_case = 1;
  // We have now read the arguments.
  Token y = find_env_token(name,true);
  name_for_error = y; 
  if(!ok_to_define(y,rd_if_undef))
    return;
  find_env_token(name,false);
  eval_let(cur_tok, hash_table.eth_token,true); // defines \endname
  Token thename_cmd;
  brace_me(text);
  if(which_case ==4) {
    text.push_back(hash_table.OB_token);
    text.push_back(hash_table.CB_token); 
  } else {
    Thbuf1 << bf_reset << "the" << name;
    thename_cmd = hash_table.locate(Thbuf1);
    eval_let(thename_cmd,hash_table.relax_token,true); // in case of error
    text.push_back(thename_cmd);
  }
  TokenList aux = toks_registers[theorem_bodyfont_code].get_val();
  brace_me(aux);
  text.splice(text.end(),aux);
  aux = token_ns::string_to_list(name,true);
  text.splice(text.end(),aux);
  if(which_case==3) {
    aux = ctr;
    brace_me(aux);
  }
  else
    aux = token_ns::string_to_list(name,true);
  text.splice(text.end(),aux);
  aux = toks_registers[theorem_style_code].get_val();
  brace_me(aux);
  text.splice(text.end(),aux);
  text.push_front(hash_table.bth_token);
  new_macro(text,y,true); // This defines \name
  if(which_case==4) return;
  Thbuf1.reset();
  if(which_case==3) {
    if(!csname_aux("c@","",ctr,false,Thbuf1)) {
      bad_counter0();
      return;
    }
    if(!counter_check(Thbuf1,false,cur_tok)) return; // checks the counter
    Thbuf2 << bf_reset << "the" << (Thbuf1.c_str()+2);
    cur_tok = hash_table.locate(Thbuf2);
    TokenList R;
    R.push_back(cur_tok);
    new_macro(R,thename_cmd,true);
    return;
  }
  TokenList the_value = token_ns::string_to_list(name,true);
  TokenList V = the_value;
  the_value.push_front(hash_table.arabic_token);
  if(which_case==2) {
    back_input(hash_table.relax_token);
  } else {
    back_input(Token(other_t_offset,']'));
    TokenList ccopy = ctr;
    back_input(ccopy);
    back_input(Token(other_t_offset,'['));
    Thbuf2.reset();
    csname_aux("the","",ctr,false,Thbuf2);
    Token x = hash_table.locate(Thbuf2);
    the_value.push_front(Token(other_t_offset,'.'));
    the_value.push_front(x);
  }
  back_input(V);
  if(counter(true)) new_macro(the_value,thename_cmd,true);
}

void Parser::T_end_theorem()
{
  leave_h_mode();
  the_stack.pop(cst_theorem);
  the_stack.add_nl();
}

void Parser::T_start_theorem(int c)
{
  TokenList name = mac_arg();
  TokenList ctr = mac_arg(); // empty in case of theorem*
  TokenList font2 = mac_arg();
  TokenList me = mac_arg();
  TokenList mecounter = mac_arg();
  TokenList style = mac_arg();
  TokenList opt;
  bool was_o = next_optarg(opt);
  leave_h_mode();
  the_stack.push1(the_names[cst_theorem],np_theorem);
  the_stack.set_v_mode();
  bool noref = ctr.empty();
  if(!noref) {
    refstepcounter(mecounter,true);
    T_translate(mecounter);
  }
  if(c==0) {
    Xid id1=the_stack.get_xid();
    leave_v_mode();
    Xid id2=the_stack.get_xid();
    if(!(id1==id2) && the_names[np_theorem].empty()) id2.add_attribute(id1);
  }
  if(c==0) {
    if(!noref) name.push_back(hash_table.space_token);
    name.splice(name.end(),ctr);
    if(was_o) {
      name.push_back(hash_table.space_token);
      name.push_back(Token(other_t_offset,'('));
      name.splice(name.end(),opt);
      name.push_back(Token(other_t_offset,')'));
    }
    brace_me(name);
    TokenList font1 = toks_registers[theorem_headerfont_code].get_val();
    name.splice(name.begin(),font1);
    brace_me(name);
    name.push_back(hash_table.space_token);
    name.splice(name.end(),font2);
    back_input(name);

  } else {
    the_stack.set_arg_mode();
    the_stack.add_nl();
    back_input_braced(name);
    T_arg1(np_head);
    the_stack.add_nl();
    back_input_braced(me);
    Istring n = nT_next_arg();
    the_stack.add_att_to_cur(the_names[np_type],n);
    back_input_braced(style);
    n = nT_next_arg();
    the_stack.add_att_to_cur(the_names[np_style],n);
    if(was_o) {
      back_input_braced(opt);
      T_arg1(np_alt_head);
      the_stack.add_nl();
    }
    the_stack.set_v_mode();
    Xid res =  ileave_v_mode();
    res.add_attribute(Istring(0),Istring("")); 
    remove_initial_space_and_back_input();
  }
}

// case of \def... or \edef...
void Parser::see_new_def(bool edef, bool gbl, symcodes what)
{
  skip_initial_space();
  Token name = cur_tok;
  if(cur_tok.not_a_cmd()) bad_redefinition(2,name);
  name_for_error = name;
  Macro* X = new Macro;
  get_def_nbargs(X,name);
  back_input();
  read_mac_body(X->get_body(),edef,X->get_nbargs());
  X->correct_type();
  mac_define(name,X,gbl,rd_always,what);
}

// reads the tokens of the list in C
// If foo is wanted, and we see bar, then the b is read and an error 
// is signalled
void Parser::skip_prefix (const TokenList& L)
{
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    get_token(); 
    if(cur_tok != *C) { 
      bad_macro_prefix(cur_tok, *C);
      return;
    }
    ++C;
  }
}

// Returns true if the current tokens are L, for instance foo in
// \def\bar#1foo#2{...}. In case of a match, the tokens are read.
bool Parser::is_delimiter(const TokenList& L)
{
  TokenList backup;
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    Token x = *C;
    ++C;
    if(!get_token())
      backup.push_back(cur_tok);
    if(x==cur_tok) continue;
    if(cur_tok==hash_table.newline_token && x==hash_table.space_token) continue;
    back_input(backup);
    return false;
  }
  return true;
}


// Returns the token, until \end 
void Parser::grab_env(TokenList& v)
{
  int b = 1;
  int elevel = 0;
  for(;;) {
    if(get_token()) {
      parse_error("End of data reached while scanning content of ",
		  get_cur_env_name(), "bad grab env");
      return;
    }
    if(check_brace(b)) {
      parse_error("Spurious close brace removed");
      ++b; 
      continue;
    }
    if(b==1 && cur_cmd_chr.get_cmd()==begin_cmd) elevel++;
    if(b==1 && cur_cmd_chr.get_cmd()==end_cmd) {
      if(elevel==0) { back_input(); return ; }
      elevel --;
    }
    v.push_back(cur_tok);
  }
}

void Parser::grab_env(TokenList& v,bool stop_at_comma, bool& done)
{
  int b = 1;
  int elevel = 0;
  done = true;
  for(;;) {
    if(get_x_token()) {
      parse_error("End of data reached while scanning content of ",
		  get_cur_env_name(), "bad grab env");
      return;
    }
    if(check_brace(b)) {
      parse_error("Spurious close brace removed");
      ++b; 
      continue;
    }
    if(b==1 && stop_at_comma && elevel==0 && cur_tok.is_comma_token()) {
      done = false;
      return ;
    }
    if(b==1 && cur_cmd_chr.get_cmd()==begin_cmd) elevel++;
    if(b==1 && cur_cmd_chr.get_cmd()==end_cmd) {
      if(elevel==0) { back_input(); return ; }
      elevel --;
    }
    v.push_back(cur_tok);
  }
}

// Assumes that cur_tok holds the macro name, the argument the value.
// This is macro_call in TeX
void Parser::expand_mac (Macro& X)
{
  SaveScannerStatus sth(ss_matching);
  if(tracing_macros())
    the_log << lg_start <<" " << X << lg_end;
  def_type spec = X.get_type();
  if(spec == dt_empty) return;
  name_for_error = cur_tok;
  skip_prefix(X[0]);
  bool optional = spec ==dt_optional || spec ==dt_spec_opt;
  bool spec_optional = spec ==dt_spec_opt;
  int n = X.get_nbargs();
  TokenList arguments[10];
  for(int i=1; i<=n; i++) {
    if(spec==dt_delim || spec==dt_brace) {
      if(!X[i].empty()) 
	scan_group_del(arguments[i],X[i]);
      else scan_group2(arguments[i]);
    } else if(! optional) {
      scan_group2(arguments[i]);
    } else {
      optional = false;
      TokenList u;
      if(scan_group_opt(u))
	arguments[i] = u;
      else {
	if(spec_optional) {
	  scan_group2(arguments[1]);
	  i++;
	  arguments[2] = arguments[1];
	}
	else arguments[1] = X[1];
      }
    }
    if(tracing_macros())
      the_log << "#" << i << "<-" << arguments[i] << lg_end;
  }
  TokenList res = expand_mac_inner(X,arguments);
  if(spec==dt_brace) res.push_back(hash_table.OB_token);
  back_input(res);
}

// This is the code that replaces arguments by values in the body.
// note that Tex uses a completely different method (there is a stack with
// all arguments of all macros; here we have just one table).
TokenList Parser::expand_mac_inner(Macro& X, TokenList* arguments)
{
  TokenList res;
  TokenList W = X.get_body();
  while(!W.empty()) {
    Token x = W.front();
    W.pop_front();
    if(x.is_a_char() && x.cmd_val()==eol_catcode) {
      int k = x.chr_val();
      TokenList ww = arguments[k];
      res.splice(res.end(),ww);
   } else res.push_back(x);
  }
  
  return res;
}

// Special expansion for tables.
TokenList Parser::special_expand(TokenList* args)
{
  Macro& X = mac_table.get_macro(cur_cmd_chr.get_chr());
  return expand_mac_inner(X,args);
}

// Testing outer is non-trivial because of \protected
void Parser::check_outer_validity()
{
  int c = cur_cmd_chr.get_cmd();
  if(c== usero_cmd || c ==userlo_cmd ||
     c== userop_cmd || c== userlop_cmd)
    check_outer_validity0();
}

void Parser::check_outer_validity0()
{
  if(scanner_status == ss_normal) return;
  Buffer &B = err_ns::local_buf; B.reset();
  if(scanner_status == ss_skipping)
    B << "Incomplete \\if? missing \\fi inserted\n";
  else if(scanner_status == ss_defining)
    B << "Runaway definition?\n";
  else if(scanner_status == ss_absorbing)
    B << "Runaway text?\n";
  else // matching
    B << "Runaway argument?\n";
  if(cur_tok==hash_table.relax_token)
    B << "End of file";
  else
    B << "Forbidden control sequence " << cur_tok.tok_to_str();
  if(scanner_status == ss_skipping) {
    back_input(hash_table.fi_token);
    B << " in conditional started at line " << conditions.top_line();
  } else {
    B << " found while scanning ";
    if(scanner_status==ss_defining)
      B << "definition";
    else if(scanner_status == ss_absorbing)
      B << "text";
    else // matching
      B << "use";
    B << " of " << name_for_error.tok_to_str();
  }
  err_ns::signal_error("Runaway argument",0);
  // Insert special relax instead of this bad token
  cur_tok = runawayed_token;
  look_at_mac();
}



// This interprets \csname ... \endcsname
void Parser::csname()
{
  Buffer b; // Need a new buffer, 'cause of \csname x\csname etc
  if(tracing_commands()) 
    the_log.dump("csname");
  for(;;) {
    get_x_token();
    if(cur_tok.not_a_cmd()) b.push_back (cur_cmd_chr.char_val());
    else break;
  }
  if(cur_cmd_chr.get_cmd()!=endcsname_cmd) {
    bad_csname();
    return;
  }
  finish_csname(b,"\\csname");
}

// Internal command associated to fetch_name
void Parser::fetch_name2()
{
  Buffer b;
  for(;;) {
    if(get_x_token()) {
      bad_csname();
      break;
    }
    if(cur_cmd_chr.get_cmd() == endcsname_cmd) 
      break;
    if(cur_tok.not_a_cmd()) b.push_back (cur_cmd_chr.char_val());
    else {
      err_ns::local_buf << bf_reset << "Invalid token " << cur_tok.tok_to_str();
      err_ns::local_buf << " found while scanning " 
			<< name_for_error.tok_to_str();
      err_ns::signal_error("bad csname",0);
      break;
    }
  }
  fetch_name_res.reset();
  fetch_name_res.push_back(b);
}

String Parser::fetch_name0()
{
  TokenList L = mac_arg_long();
  return fetch_name1(L);
}

String Parser::fetch_name1(TokenList& L)
{
  if(L.empty()) return "";
  back_input(hash_table.frozen_endcsname);
  back_input(L);
  fetch_name2();
  return fetch_name_res.c_str();
}

// Use a non-long method
String Parser::fetch_name_opt()
{
  TokenList L;
  bool res = next_optarg_long(L);
  if(!res || L.empty())
    return "";
  else return fetch_name1(L);
}

// We have found the command in the buffer b, for the \csname-like 
// command s.
void Parser::finish_csname(const Buffer&b, String s)
{
  finish_csname(b);
  if(tracing_commands())     
    the_log << lg_startbrace << s << "->\\"<< b.convert_to_log_encoding() 
	    << lg_endbrace;
  back_input();
}

void Parser::finish_csname(const Buffer&b)
{
  cur_tok = hash_table.locate(b);
  int pos = cur_tok.eqtb_loc();
  if(hash_table.eqtb[pos].is_undefined())
    eq_define(pos,CmdChr(relax_cmd,relax_code),false);
}

// This is like \csname s1 L s2 \endcsname
// It returns false in case of error.
// If cs is true, we lowercase the thing.
bool Parser::my_csname(String s1, String s2, TokenList& L, String s,bool cs)
{
  if(tracing_commands()) the_log << lg_startbrace << s << lg_endbrace;
  Buffer b;
  if(!csname_aux(s1,s2, L,cs,b)) {
    bad_csname(s);
    return false;
  }
  finish_csname(b,s);
  return true;
}

// That's the real code.
// It is like \csname s1 L s2\endcsname, if cs is true, then L is lowercased
// This returns false in case of trouble. 
// Puts the chars in the buffer otherwise
bool Parser::csname_aux(String s1, String s2, TokenList& L, bool cs, Buffer& b)
{
  b.push_back(s1);
  bool r = csname_aux(L,cs,b);
  b.push_back(s2);
  return r;
}
bool Parser::csname_aux(TokenList& L, bool cs, Buffer& b)
{
  back_input(hash_table.endcsname_token);
  back_input(L);
  for(;;) {
    get_x_token();
    if(cur_tok.not_a_cmd()) {
      Utf8Char w = cur_cmd_chr.char_val();
      if(cs && w.is_upper_case())
	w = Utf8Char(w.get_value() + 'a'-'A');
      b.push_back(w);
    }
    else break;
  }
  return cur_cmd_chr.get_cmd()==endcsname_cmd;
}


// Implements \@ifundefined{cmd} {A}{B}
// token lists A and B are read by one_of_two. A command is undefined
// if its command code is relax.
void Parser::if_undefined()
{
  bool c = cur_cmd_chr.get_chr() != 0;
  Token T = cur_tok;
  // you lose in case \@ifundefined {a\par b}{c}{d}, pars
  TokenList a = mac_arg_long();
  bool undef = true;
  if(c) {
    back_input(hash_table.frozen_endcsname);
    back_input(a);
    fetch_name2();
    undef= hash_table.is_defined(fetch_name_res);
  } else {
    if(!my_csname("","",a,"command for \\@ifundefined",false))
      undef = true;
    else {
      get_token();
      undef = cur_cmd_chr.is_relax();
    }
  }
  if(tracing_macros()) 
    the_log << lg_startbrace << T << boolean(undef) << lg_endbrace;
  one_of_two(undef);
}

// Implements \@ifstar{A}{B}. If the next character is a star, it is read.
void Parser::if_star()
{
  TokenList a = mac_arg();
  TokenList b = mac_arg();
  bool ok = remove_initial_star();
  if(tracing_commands()) 
    the_log << lg_startbracebs << "@ifstar " << boolean(ok) << lg_endbrace;
  one_of_two(a,b,ok);
}

// Implements \@ifempty#1{A}{B} \@ifempty#1{A}{B}
void Parser::if_empty()
{
  Token T = cur_tok;
  subtypes c = cur_cmd_chr.get_chr();
  TokenList L = mac_arg();
  TokenList a = mac_arg();
  TokenList b = mac_arg();
  if(c) token_ns::remove_first_last_space(L);
  bool ok = L.empty();
  if(tracing_commands()) 
    the_log << lg_startbrace << T << " " << boolean(ok) << lg_endbrace;
  one_of_two(a,b,ok);
}

// Implements \@ifnextchar W{A}{B}. 
// If the next character is a W, it is not read.
void Parser::if_nextchar(bool c)
{
  Token T = cur_tok;
  TokenList D = mac_arg();
  Token d = token_ns::get_unique(D); // no error here..
  TokenList a = mac_arg();
  TokenList b = mac_arg();
  skip_initial_space();
  if(cur_tok.is_valid()) back_input();
  bool ok = cur_tok ==d;
  if(!c &&cur_tok.char_or_active() &&d.char_or_active() && cur_tok.chr_val()==d.chr_val())
    ok = true;
  if(tracing_commands()) 
    the_log <<lg_startbrace << T << boolean(ok) << lg_endbrace;
  one_of_two(a,b,ok);
}

void Parser::while_num(subtypes cc)
{
  Token T= cur_tok;
  TokenList A = read_until(cc== 2 ? hash_table.fi_token: hash_table.do_token);
  TokenList B = mac_arg();
  if(cc!=2)  A.push_back(hash_table.relax_token);
  A.splice(A.end(),B);
  TokenList res;
  if(cc==2) {
    res.push_back(hash_table.fi_token);
    res.push_back(hash_table.fi_token);
    TokenList C = A;
    brace_me(C);
    res.splice(res.begin(),C);
    res.push_front(hash_table.iwhilesw_token);
    res.splice(res.begin(),A);
  } else {
    if(cc==0) A.push_back(hash_table.relax_token);
    TokenList D = A;
    brace_me(D);
    res.push_front(hash_table.fi_token);
    res.splice(res.begin(),D);
    res.push_front(cc==1 ? hash_table.iwhiledim_token : hash_table.iwhilenum_token);
    res.splice(res.begin(),A);
    res.push_front(cc==1 ? hash_table.ifdim_token : hash_table.ifnum_token);
  }
  if(tracing_commands())
    the_log << lg_start << T << "<- " << res << lg_end;
  back_input(res);
}

void Parser::iwhile_num(subtypes cc)
{
  Token T = cur_tok;
  TokenList A = cc==2 ? read_until(hash_table.fi_token) : mac_arg();
  TokenList B = A;
  brace_me(B);
  TokenList w;
  if(cc==2) {
    w.push_front(hash_table.fi_token);
    w.splice(w.begin(),B);
    w.push_front(hash_table.fi_token);
    w.push_front(hash_table.gobbletwo_token);
    w.push_front(hash_table.else_token);
    w.push_front(T);
    w.push_front(hash_table.expandafter_token);
    w.splice(w.begin(),A);
  } else {
    w.splice(w.begin(),B);
    w.push_front(hash_table.fi_token);
    w.push_front (hash_table.gobble_token);
    w.push_front(hash_table.expandafter_token);
    w.push_front(hash_table.else_token);
    w.push_front(cc==1 ? hash_table.iwhiledim_token : hash_table.iwhilenum_token);
    w.push_front(hash_table.expandafter_token);
    w.splice(w.begin(),A);
    w.push_front(cc==1 ? hash_table.ifdim_token : hash_table.ifnum_token);
  }
  if(tracing_commands())
    the_log << lg_start << T << "<- " << w << lg_end;
  back_input(w);
}

// Case of  \loop....\repeat.
// We define \iterate as: \def\iterate{...\relax\expandafter\iterate\fi}
// then execute \iterate\let\iterate\relax
void Parser::T_loop()
{
  if(tracing_commands()) the_log.dump("loop");
  TokenList R = read_until(hash_table.repeat_token);
  R.push_back(hash_table.relax_token);
  R.push_back(hash_table.expandafter_token);
  R.push_back(hash_table.iterate_token);
  R.push_back(hash_table.fi_token);
  new_macro(R,hash_table.iterate_token,false);
  back_input(hash_table.relax_token);
  back_input(hash_table.iterate_token);
  back_input(hash_table.let_token);
  back_input(hash_table.iterate_token);  
}

// This takes an argument, say foo, and considers the command \c@foo.
// Returns false in case of error, true otherwise. If def is false we check 
// that foo is a counter i.e. \c@foo is assign_int. 
// Otherwise we define a counter
bool Parser::counter(bool def)
{
  Token T = cur_tok;
  Buffer b;
  TokenList L0 = mac_arg();
  if(!csname_aux("c@","",L0,false,b)) {
    bad_counter0();
    return false;
  }
  if(!counter_check(b,def,T)) return false;
  if(!def) { back_input(); return true; }
  return counter_aux(b.to_string(2),0,T);
}

// Used by the bootstrap phase to define a dependent counter
void Parser::counter_boot(String s, String aux)
{
  Token T = hash_table.relax_token;
  Buffer& b = Thbuf2;
  b << bf_reset <<  "c@" << s;
  if(!counter_check(b,true,T)) return; // should not happen
  counter_aux(s,aux,T);
}

// Given c@foo in the buffer b, creates the token \c@foo
// if def is false, check that \c@foo=2 is OK
// otherwise, checks that \c@foo is undefined.
// Returns true if OK, false otherwise
bool Parser::counter_check(Buffer&b, bool def,Token T)
{
  cur_tok = hash_table.locate(b);
  Equivalent& E = hash_table.eqtb[cur_tok.eqtb_loc()];
  // our buffer b holds c@foo, and cur_tok is \c@foo
  if(def) {
    if(!E.is_undef_or_relax()) {
      bad_redefinition(0,cur_tok);
      return false;
    }
  } else {
    if(E.get_cmd() != assign_int_cmd) {
      bad_counter1(b,E);
      return false;
    }
  }
  return true;
}


// Reads an optional argument bar; or takes it from the argument 
// creates the token \cl@bar in cur_tok
// Return 0 if no opt argument, or s empty, 1 if error, 2 if OK
int Parser::counter_read_opt(String s)
{
  if(s) {
    if(s[0]==0) return 0;
    Buffer&b = Thbuf2;
    b << bf_reset << "cl@" << s;
    finish_csname(b,"newcounter");
    return 2;
  }
  TokenList bar_list;
  int ne = main_ns::nb_errs;
  if(!next_optarg_long(bar_list)) return 0;
  if(ne != main_ns::nb_errs) return 1;
  if(!my_csname("cl@","",bar_list,"newcounter_opt",false))
    return 1;
  return 2;
}

// This defines a new counter. Arguments: a flag that says if we should
// read an an optional argument, the name of the counter, a token for error.
// cur_tok holds the token \c@foo
bool Parser::counter_aux(string name, String opt,Token T)
{
  Buffer&b = Thbuf1;
  back_input(); // keep the token somewhere
  // We are defining a counter now
  // We construct a macro without argument that expands to \number\c@foo
  TokenList L;
  L.push_front(cur_tok);
  L.push_front(hash_table.number_token);
 
  // evaluate : \global\let\cl@foo\@empty
  b << bf_reset << "cl@" << name;  // b is now cl@foo
  Token t1 = hash_table.locate(b);
  Token t2 = hash_table.empty_token;
  eq_define(t1.to_cs()-1, hash_table.eqtb[t2.to_cs()-1].get_cmdchr(),true);

  // thexx_cmd is the token associated to \thefoo
  b << bf_reset << "the" << name;  // B is now thefoo
  Token thecmd = hash_table.locate(b);
  define_new_counter(); // this will eat the \c@foo token
  new_macro(L,thecmd,true);
  switch(counter_read_opt(opt)) {
  case 0: return true;
  case 1: return false;
  default: break;
  } 
  // evaluate: \xdef\cl@bar{\cl@bar\noexpand\@elt{foo}}
  // We know that \cl@bar can be expanded only once.
  TokenList foo_list = token_ns::string_to_list(name,true);
  Token cl_token = cur_tok; // \cl@bar
  get_token(); // get the \cl@bar token
  T_cons(cl_token,foo_list);
  return true;
}


// \@addtreset{foo}{bar}
// evaluate: \@cons\cl@bar{{foo}}; i.e. T_cons(\cl@bar, {foo}).
void Parser::T_add_to_reset()
{
  int ne = main_ns::nb_errs;
  TokenList foo_list = mac_arg_long();
  TokenList L = mac_arg_long();
  if(ne != main_ns::nb_errs) return;
  brace_me(foo_list);
  if(!my_csname("cl@","",L,"\\@addtoreset",false)) return;
  Token cl_token = cur_tok; // \cl@bar
  get_token(); // get the \cl@bar token
  T_cons(cl_token,foo_list);
}

void Parser::trace_count_def(String S, CmdChr k)
{
  the_log << lg_startbracebs << S <<" " << cur_tok << "=\\" << k.name() <<lg_endbrace;
}

void Parser::define_new_something(subtypes c)
{
  switch(c) {
  case newtoks_code: define_new_toks(); return;
  case newcount_code: define_new_counter(); return;
  case newbox_code: define_new_box(); return;
  case newdimen_code: define_new_dimen(); return;
  case newlength_code: define_new_length(); return;
  case newmuskip_code:  define_new_muskip(); return;
  case newread_code: define_new_read(); return;
  case newwrite_code:  define_new_write(); return;
  case newlanguage_code:  define_new_language(); return;
  default:return;
  }
}

void Parser::define_new_something(String name,int max_val, int alloc_pos,symcodes c)
{
  Token T = cur_tok;
  alloc_pos += 10;
  int pos = get_r_token(true);
  int k = eqtb_int_table[alloc_pos+count_reg_offset].get_val();
  if(k>=max_val) {
    err_ns::local_buf << bf_reset
		      << "Overflow in " << T << "; max value is " << max_val;
    err_ns::signal_error("allocation overflow",0);
    return;
  }
  ++k;
  eqtb_int_table[alloc_pos+count_reg_offset].set_val(k); // No trace please
  if(c==assign_int_cmd) k += count_reg_offset;
  CmdChr R(c,subtypes(k));
  eq_define(pos,R, true);
  trace_count_def(name,R);
}


// Defines globally next token to be \count N, where N is incremented.
void Parser::define_new_counter()
{
  define_new_something("countdef",last_register,0,assign_int_cmd);
}

// Defines a new dimension
void Parser::define_new_dimen()
{
  define_new_something("dimendef",last_register,1,assign_dimen_cmd);
}

void Parser::define_new_length()
{
  define_new_something("skipdef",last_register,2,assign_glue_cmd);
}

void Parser::define_new_muskip()
{
  define_new_something("muskipdef",last_register,3,assign_mu_glue_cmd);
}

// If \foo is a counter, you can say \foo=0, but in the case of a 
// box, you say \setbox\bar={}, or \copy\bar, said otherwise, \bar is
// just a number (hence the \chardef)
void Parser::define_new_box()
{
  define_new_something("chardef",last_register,4,char_given_cmd);
}

void Parser::define_new_toks()
{
  define_new_something("toksdef",last_register,5,assign_toks_cmd);
}

void Parser::define_new_read()
{
  define_new_something("chardef",15,6,char_given_cmd);
}
void Parser::define_new_write()
{
  define_new_something("chardef",15,7,char_given_cmd);
}
void Parser::define_new_language()
{
  define_new_something("chardef",last_register,8,char_given_cmd);
}


// c is the code of \value, \stepcounter, \addtocounter, \setcounter, or
//  \@stpelt
// This may generate \calc{\global\advance\c@foo} for \addtocounter{foo}
// or \global\advance\c@foo 25\relax for \addtcounter{foo}{25}
// In the case of \stepcounter, we do
//  \global\advance\c@foo\@one\let\elt@stpelt\cl@foo
void Parser::counter_commands(int c)
{
  Token first = cur_tok;
  if(c==addtoreset_code) { T_add_to_reset(); return; }
  if(!counter(false)) return;
  if(c==value_code) return;
  get_token();
  Token t = cur_tok; // we have \c@foo
  TokenList L;
  if(!calc_loaded && c != stepcounter_code && c!= killcounter_code)
    L = mac_arg();
  L.push_front(t);
  if(c!=setcounter_code &&c!=killcounter_code)
    L.push_front(hash_table.advance_token);
  L.push_front(hash_table.global_token);
  if(c==stepcounter_code) {
    L.push_back(hash_table.at_one_token);
    L.push_back(hash_table.let_token);
    L.push_back(hash_table.elt_token);
    L.push_back(hash_table.killcounter_token);
    group_buffer << bf_reset << "cl@" << (hash_table[t.hash_loc()]+2);
    t = hash_table.locate(group_buffer);
    //  L.push_back(hash_table.OB_token);
    L.push_back(t);
    //    L.push_back(hash_table.CB_token);
  }
  else if(c==killcounter_code)
    L.push_back(hash_table.at_zero_token);
  else if(calc_loaded) {
    brace_me(L);
    L.push_front(hash_table.calc_token);
  } 
  else L.push_back(hash_table.relax_token);
  finish_counter_cmd(first,L);
}

// Backinputs the token list, plus some tracing info.
void Parser::finish_counter_cmd(Token first, TokenList& L)
{
  if(tracing_commands())
    the_log << lg_start << first << "->" << L << lg_end;
  back_input(L);
}

// c is 0 for \setlength, 1 for \addtolength
void Parser::setlength_command(int c)
{
  Token first = cur_tok;
  TokenList tmp = mac_arg();
  Token t = token_ns::get_unique(tmp);
  if(t.is_null()) {
    parse_error("The command ", first, " takes one token as argument",
		"onearg");
    return;
  }
  token_from_list(t);
  if(cur_tok.not_a_cmd()) { bad_redefinition(2,cur_tok); return; }
  if(cur_cmd_chr.is_undef()) { undefined_mac(); return; }
  TokenList L;
  if(calc_loaded) {
    L.push_back(t);
    if(c) L.push_front(hash_table.advance_token);
    brace_me(L);
    L.push_front(hash_table.calc_token);
  } else {
    L = mac_arg();
    L.push_back(hash_table.relax_token);
    L.push_front(t);
    if(c) L.push_front(hash_table.advance_token);
  }
  finish_counter_cmd(first,L);
}


// Implements \newif. Check that the command starts with the letters if
void Parser::newif()
{
  int pos = get_r_token();
  Token T = cur_tok;
  if(tracing_commands())
    the_log << lg_startbracebs <<"newif " << cur_tok << lg_endbrace;
  String S = hash_table[T.hash_loc()];
  if(S[0] !='i' || S[1] != 'f' || S[2] ==0) {
    parse_error("token list \\newif does not start with if");
    return;
  }
  S = S+2;
  eq_define(pos,CmdChr(if_test_cmd,if_false_code),false);
  newif_aux(T,S,true);
  newif_aux(T,S,false);
}

// If you say \newif\iffoo, this is the same as
// \def\footrue{\let\iffoo\iftrue} and \def\foofalse{\let\iffoo\iffalse} 
// this code does one or the other definitions, 
// given T=\iffoo, s=foo and b=true of false
void Parser::newif_aux(Token T, string S, bool b)
{
  TokenList L1;
  L1.push_front(hash_table.let_token);
  L1.push_back(T);
  L1.push_back(b? hash_table.iftrue_token : hash_table.iffalse_token);
  mac_buffer << bf_reset << S << boolean(b);
  make_token(mac_buffer.c_str());
  TL.pop_front();
  new_macro(L1,cur_tok);
}

// Given the string foo, this pushes back the token \foo
void Parser::make_token(String s)
{
  Buffer&b = Thbuf1;
  b.reset();
  b.push_back(s);
  cur_tok = hash_table.locate(b);
  back_input();
}

// Creates name as a macro without arguments with body L.
// Assumes token can be defined. Nothing appears in the transcript file.
void Parser::new_prim(Token name,TokenList& L)
{
  Macro* X = new Macro(L);
  subtypes B = mac_table.new_macro(X);
  eq_define(name.eqtb_loc(),CmdChr(user_cmd,B),false);
}

// Creates a macro without arguments with body {b}
void Parser::new_prim(Token a, Token b)
{
  TokenList L;
  L.push_back(b);
  new_prim(a,L);
}

// Creates a macro without arguments with body {b}
void Parser::new_primx(String a, String b)
{
  TokenList L;
  L.push_back(hash_table.locate(b));
  new_prim(hash_table.locate(a),L);
}

// Creates a macro without arguments, with body B
// body should contain only ASCII letters, spaces, or other tokens.
void Parser::new_prim(String a, String b)
{
  TokenList L;
  int n = strlen(b);
  for(int i=0; i<n;i++) {
    uchar c = b[i];
    if(c>128)  err_ns::fatal_error("internal error in make_macro");
    spec_offsets T= c==' ' ? space_t_offset : 
      is_letter(c)? letter_t_offset : other_t_offset;
    L.push_back(Token(T,Utf8Char(c)));
  }
  new_prim(hash_table.locate(a), L);
}

// Creates a macro without arguments with body b
void Parser::new_prim(String a, TokenList& b)
{
  new_prim(hash_table.locate(a), b);
}


// Implements  \DefineShortVerb and \UndefineShortVerb
void Parser::short_verb(int x)
{
  TokenList L = mac_arg();
  Token t = token_ns::get_unique(L);
  int T = t.get_val()-single_offset;
  if(0<T && T<int(nb_characters)) {
    if(x==0) {// define
      if(eqtb_int_table[T].get_val()==16) {
	short_verb_error(t,2);
	return;
      }
      if(T<int(nb_shortverb_values))
	old_catcode[T] = eqtb_int_table[T].get_val();
      eqtb_int_table[T].set_val(16);      
    } else { // undefine
      if(eqtb_int_table[T].get_val() != 16) {
	short_verb_error(t,3);
	return;
      }
      if(T<int(nb_shortverb_values))
	eqtb_int_table[T].set_val(old_catcode[T]);
      else
	eqtb_int_table[T].set_val(12);
    }
  } else short_verb_error(t,x);
}

// Returns the token \foo or \endfoo.
Token Parser::find_env_token(const string& name, bool beg)
{
  mac_buffer.reset();
  if(!beg) mac_buffer << "end";
  mac_buffer << name; 
  cur_tok = hash_table.locate(mac_buffer);
  look_at_mac();
  return cur_tok;
}

// Hack for \begin{foo}
SaveAuxEnv* Parser::env_helper (const string& s)
{
  find_env_token(s,true);
  int cl = get_cur_line();
  string cur_e_name = get_cur_env_name();
  if(cur_cmd_chr.is_undef()) {
    name_for_error = cur_tok;
    undefined_env(s);
  } else back_input();
  find_env_token(s,false);
  Token t = cur_tok;
  if(cur_cmd_chr.is_undef())
    return new SaveAuxEnv(cur_e_name,s,cl,t, CmdChr(relax_cmd,relax_code));
  else 
    return new SaveAuxEnv(cur_e_name,s,cl,t, cur_cmd_chr);
}


// This implements \begin{foo}
void Parser::T_begin(const string& s)
{
  SaveAuxEnv* X = env_helper(s);
  push_level(bt_env);
  push_save_stack(X);
  set_cur_env_name(s);
}

// This is executed when we pop a slot containing restore-foo-env
void SaveAuxEnv::unsave(bool trace, Parser&P)
{
  if(trace)
    the_log << lg_startstack << "ending environment " << get_name() 
	    << "; resuming "  << oldname << ".\n";
  P.set_cur_env_name(oldname,line);
}

// This is the code of \end{foo}. 
void Parser::T_end(const string& s)
{
  if(s == "document") // hack, because document is at outer level
    { T_begin(s); get_token(); }
  if(s == "tabular" || s == "tabular*") {
    if(false_end_tabular(s)) 
      return;
  }
  if(s=="") { parse_error("Illegal \\end{}");return; }
  SaveAuxEnv* X = is_env_on_stack(s);
  if(!X) {
    parse_error("cannot close environment ",s,"bad \\end");
    return;
  }
  if(first_boundary() ==bt_tpa) {
    pop_level(false,bt_tpa);
    pop_level(true,bt_env);
    cur_level ++;
    throw EndOfData();
  }
  back_input(hash_table.sendgroup_token);
  cur_tok = X->get_token();
  if(X->get_val().is_user()) {
    cur_cmd_chr = X->get_val();
    expand();
    return;
  } else {
    Token t = hash_table.temp_token;
    int k = t.eqtb_loc();
    hash_table.eqtb[k].setnl(X->get_val());
    back_input(t);
  }
}

// Given a string like foo, this evaluates to \expandafter{\foo}
// If \foo is undefined, the result is {}; if \foo expands to 'gee'
// the result is {gee}. The result can be obtained via mac_arg.
void Parser::expand_no_arg(const string& s)
{
  Token t = hash_table.locate(s);
  expand_no_arg0(t);
  back_input(hash_table.OB_token);
}

// auxiliary command used in the previous two functions
void Parser::expand_no_arg0(Token t)
{
  back_input(hash_table.CB_token);
  back_input(t);
  expand_when_ok(true);
}

void Parser::expand_when_ok(bool allow_undef)
{
  get_token(); 
  if(allow_undef &&cur_cmd_chr.is_undef())  {}
  else if(cur_cmd_chr.is_expandable()) expand(); 
  else back_input();
}

// Returns the value of the macro \foo;
// If \foo is not a user macro without args, returns empty list
TokenList Parser::get_mac_value(const string &s)
{
  return get_mac_value(hash_table.locate(s));
  
}

TokenList Parser::get_mac_value(Token t)
{
  TokenList res;
  if(t.not_a_cmd()) return res;  
  cur_tok = t;
  look_at_mac();
  subtypes c = cur_cmd_chr.get_chr();
  symcodes C = cur_cmd_chr.get_cmd();
  if(!(C== user_cmd || C== usero_cmd || C==userl_cmd || C== userlo_cmd
     || C==userp_cmd ||  C== userop_cmd || C== userlp_cmd || C== userlop_cmd))
    return res;
  Macro& M = mac_table.get_macro(c);
  if(M.get_type() != dt_normal) return res;
  if(M.get_nbargs() != 0) return res;
  return M.get_body();
}

void Parser::first_of_one(Token T)
{
  TokenList L = mac_arg();
  if(tracing_macros()) 
    the_log << lg_start << T << " #1->#1 " << lg_end
	    << "#1<-" << L << lg_end;
  back_input(L);
}


// Too bad we have to keep a copy of arith_ns::radix
void Parser::expand ()
{
  Token T = name_for_error;
  name_for_error = cur_tok;
  iexpand();
  name_for_error = T;
}

// converts 12.0pt in 12, and 12.3pt in 12.3
void token_ns::strip_pt(TokenList& L)
{
  if(L.back()!=Token(other_t_offset,'t')) return;
  L.pop_back();
  if(L.back()!=Token(other_t_offset,'p')) return;
  L.pop_back();
  if(L.back()==Token(other_t_offset,'.')) {
    L.pop_back();
    return;
  }
  if(L.back()==Token(other_t_offset,'0')) {
    L.pop_back();
    if(L.back()==Token(other_t_offset,'.')) {
      L.pop_back();
      return;
    }
    L.push_back(Token(other_t_offset,'0'));
  }
}

void Parser::iexpand ()
{
  subtypes c = cur_cmd_chr.get_chr();
  symcodes C = cur_cmd_chr.get_cmd();
  Token T = cur_tok;
  bool vb = tracing_macros();
  switch(C) {
  case a_cmd: accent_a(); return;
  case accent_cmd: accent(); return;
  case ifundefined_cmd: if_undefined(); return;
  case car_cmd: expand_car(c==zero_code); return;
  case first_of_one_cmd: first_of_one(cur_tok); return;
  case refstepcounter_cmd: refstepcounter(); return;
  case month_day_cmd: month_day(c); return;
  case scanupdown_cmd: scan_updown(); return;
  case sideset_cmd: scan_sideset(); return;
  case multispan_cmd: multispan(); return;
  case ifempty_cmd: if_empty(); return;
  case split_cmd: scan_split(); return;
  case useverb_cmd: T_save_use_verb(false); return;
  case counter_cmd: counter_commands(c); return;
  case setlength_cmd: setlength_command(c); return;
  case csname_cmd: csname(); return;
  case convert_cmd: convert();  return;
  case the_cmd: ins_the_toks(); return;
  case if_test_cmd: conditional(c,false); return;
  case fi_or_else_cmd: finish_fi(); return;
  case loop_cmd: T_loop(); return;
  case input_cmd: x_input(c); return;
  case xspace_cmd: T_xspace(); return;
  case ensuremath_cmd:  T_ensuremath(); return;
  case while_cmd: while_num(c); return;
  case iwhile_cmd: iwhile_num(c); return;
  case latex_ctr_cmd: latex_ctr(); return;
  case undef_cmd: undefined_mac();   return;
  case mathversion_cmd: mathversion(); return;
  case get_config_cmd: find_in_config(c); return;
  case random_cmd: T_random(); return;
  case ot2enc_cmd: T_parse_encoding(vb,c); return;
  case afterfi_cmd: T_afterfi();return;
  case afterelsefi_cmd: T_afterelsefi();return;
  case expandafter_cmd: if(c)  T_unless(); else T_expandafter(); return;

  case specchar_cmd:
    back_input(Token(other_t_offset,Utf8Char(c)));
    return;
  case user_cmd:
  case usero_cmd:
  case userl_cmd:
  case userlo_cmd: 
  case userp_cmd:
  case userop_cmd:
  case userlp_cmd:
  case userlop_cmd: 
    {
      if(vb) the_log << lg_start << cur_tok;
      name_for_error = cur_tok;
      SaveLongState sth(ls_normal);
      if(C == userl_cmd || C == userlo_cmd || C == userlp_cmd 
	 || C == userlop_cmd) long_state = ls_long;
      expand_mac(mac_table.get_macro(c)); 
      return;
    }
  case first_of_two_cmd: {
    if(vb) 
      the_log << lg_start << cur_tok << "#1#2->#" <<(c==1 ? "1" : "2")<<lg_end;
    TokenList L1 = mac_arg();
    TokenList L2 = mac_arg();
    if(tracing_macros()) {
      the_log << "#1<-" << L1 << lg_end;
      the_log << "#2<-" << L2 << lg_end;
    }
    one_of_two(L1,L2,c==1);
    return;
  }
  case zapspace_cmd: 
    if(c) { 
      TokenList a = mac_arg();
      token_ns::remove_first_last_space(a);
      back_input(a);
    } else zapspace(); 
    return;
  case stripprefix_cmd:
    // NOTE should this be long 
    read_until(Token(other_t_offset,'>')); return;
  case hexnumber_cmd: {
    TokenList L = mac_arg();
    back_input(L);
    int i = scan_int(cur_tok);
    if(i>=0 && i<10) back_input(Token(other_t_offset,char(i+'0')));
    if(i>=10 && i<16) back_input(Token(letter_t_offset,char(i+'A'-10)));
  }
    return;
  case strippt_cmd: {
    Token T = cur_tok;
    TokenList L = the_toks();
    token_ns::strip_pt(L);
    if (vb) the_log << lg_start<< T << lg_arrow << L << "." << lg_end;
    back_input(L);
    return;
  }
  case obracket_cmd:
    if(vb) the_log <<lg_start << cur_tok << lg_arrow << "$$" << lg_end;
    back_input(hash_table.dollar_token);
    back_input(hash_table.dollar_token);
    return;
  case oparen_cmd:
    if(vb) the_log << lg_start << cur_tok << lg_arrow << "$" << lg_end;
    back_input(hash_table.dollar_token);
    return;
  case gobble_cmd: {
    int n = c;
    while(n>0) {
      if(vb)
	the_log << lg_start << "gobble argument: " << mac_arg() << lg_end;
      else ignore_next_arg();
      --n;
    }
    return;
  }
  case noexpand_cmd: { 
    SaveScannerStatus sth(ss_normal);
    get_token();
    Token t = cur_tok;
    if(vb) the_log << lg_startbracebs << "noexpand " << t << lg_endbrace;
    back_input(t);
    if(!cur_tok.not_a_cmd()) back_input(hash_table.frozen_dont_expand);
    return;
  }
  case top_bot_mark_cmd: // no marks...
    if(c>=topmarks_code) scan_int(T);
    return;
  default: 
    parse_error("unexpandable command in expand? ",T,"","bad");
    return;
  }
}

void Parser::T_afterfi() 
{
  Token T =hash_table.fi_token;
  TokenList L = read_until(T);
  if(tracing_macros()) {
    the_log << lg_start << cur_tok << "#1\\fi->\\fi#1"  << lg_end;
    the_log << "#1<-" <<  L<< lg_end;
  }
  back_input(L);
  back_input(T);
}

void Parser::T_afterelsefi() 
{
  Token T1 =hash_table.else_token;
  Token T2 =hash_table.fi_token;
  TokenList L1 = read_until(T1);
  TokenList L2 = read_until(T2);
  if(tracing_macros()) {
    the_log << lg_start << cur_tok << "#1\\else#2\\fi->\\fi#1"  << lg_end;
    the_log << "#1<-" <<  L1;
    the_log << "#2<-" <<  L2;
  }
  back_input(L1);
  back_input(T2);
}

// \expandafter\foo\bar
void Parser::T_expandafter()
{
  get_token();
  Token T = cur_tok; 
  get_token();
  if(tracing_macros()) {
    the_log << lg_startbracebs << "expandafter ";
    the_log << T << " ";
    the_log << cur_tok << lg_endbrace;
  }
  if(cur_cmd_chr.is_expandable()) expand() ; else back_input();
  back_input(T);
}

//  Produces a random number
void Parser::T_random()
{
  Token T = cur_tok;
  int t =scan_int(T);
  int u = std::rand();
  if(t>0) u = u%t;
  Buffer B; B.push_back_int(u); 
  TokenList L = B.str_toks(true);
  if(tracing_macros())  the_log << lg_start << T << t << "->" << L << lg_end;
  back_input(L);
}

// converts T1/ OT2 into a Unicode character (expandable command)
// what=1 is OT2, everything else is T1
void Parser::T_parse_encoding(bool vb, subtypes what)
{
  Token T = cur_tok;
  int c = scan_braced_int(T);
  int r = 0;
  if(what == 1) {
    switch(c) {
    case 0: r = 0x40A; break; // 0-15
    case 1: r = 0x409; break;
    case 2: r = 0x40F; break;
    case 3: r = 0x42D; break;
    case 4: r = 0x406; break;
    case 5: r = 0x404; break;
    case 6: r = 0x402; break;
    case 7: r = 0x40B; break;
    case 8: r = 0x45A; break;
    case 9: r = 0x459; break;
    case 10: r = 0x45F; break;
    case 11: r = 0x44D; break;
    case 12: r = 0x456; break;
    case 13: r = 0x454; break;
    case 14: r = 0x452; break;
    case 15: r = 0x45B; break;
    case 16: r = 0x42E; break; // 16-31
    case 17: r = 0x416; break;
    case 18: r = 0x419; break;
    case 19: r = 0x401; break;
    case 20: r = 0x474; break;
    case 21: r = 0x472; break;
    case 22: r = 0x405; break;
    case 23: r = 0x42F; break;
    case 24: r = 0x44E; break;
    case 25: r = 0x436; break;
    case 26: r = 0x439; break;
    case 27: r = 0x451; break;
    case 28: r = 0x475; break;
    case 29: r = 0x473; break;
    case 30: r = 0x455; break;
    case 31: r = 0x44F; break;
    case 32: r = 0x308; break; // 32 - 47
    case 33: r = '!'; break;
    case 34: r = 0x201D; break; // right double quotation mark
    case 35: r = 0x462; break;
    case 36: r = 0x306; break;
    case 37: r = '%'; break;
    case 38: r = 0x301; break;
    case 39: r = 0x2019; break;  // right single quotation mark
    case 40: r = '('; break;
    case 41: r = ')'; break;
    case 42: r =  '*'; break;
    case 43: r = 0x463; break;
    case 44: r = ','; break;
    case 45: r = '-'; break;
    case 46: r = '.'; break;
    case 47: r =  '/';break;
    case 48: r = '0'; break; // 48 - 63
    case 49: r = '1'; break;
    case 50: r = '2'; break;
    case 51: r = '3'; break;
    case 52: r = '4'; break;
    case 53: r = '5'; break;
    case 54: r = '6'; break;
    case 55: r = '7'; break;
    case 56: r = '8'; break;
    case 57: r = '9'; break;
    case 58: r = ':'; break;
    case 59: r = ';'; break;
    case 60: r = 0xAB; break;
    case 61: r = 0xB9; break;
    case 62: r = 0xBB; break;
    case 63: r = '?';break;
    case 64: r = 0x306; break; // 64 -79
    case 65: r = 0x410; break;
    case 66: r = 0x411; break;
    case 67: r = 0x426; break;
    case 68: r = 0x414; break;
    case 69: r = 0x415; break;
    case 70: r = 0x424; break;
    case 71: r = 0x413; break;
    case 72: r = 0x425; break;
    case 73: r = 0x418; break;
    case 74: r = 0x408; break;
    case 75: r = 0x41A; break;
    case 76: r = 0x41B; break;
    case 77: r = 0x41C; break;
    case 78: r = 0x41D; break;
    case 79: r = 0x41E; break;
    case 80: r = 0x41F; break; // 80 - 95
    case 81: r = 0x427; break;
    case 82: r = 0x420; break;
    case 83: r = 0x421; break;
    case 84: r = 0x422; break;
    case 85: r = 0x423; break;
    case 86: r = 0x412; break;
    case 87: r = 0x429; break;
    case 88: r = 0x428; break;
    case 89: r = 0x42B; break;
    case 90: r = 0x417; break;
    case 91: r = '['; break;
    case 92: r = 0x201C; break; //  left double quotation mark
    case 93: r = ']'; break;
    case 94: r = 0x42C; break;
    case 95: r = 0x42A; break;
    case 96: r = 0x2018; break; // 96 - 111 left single quotation mark
    case 97: r = 0x430; break;
    case 98: r = 0x431; break;
    case 99: r = 0x446; break;
    case 100: r = 0x434; break;
    case 101: r = 0x435; break;
    case 102: r = 0x444; break;
    case 103: r = 0x433; break;
    case 104: r = 0x445; break;
    case 105: r = 0x438;break;
    case 106: r = 0x458;break;
    case 107: r = 0x43A; break;
    case 108: r = 0x43B; break;
    case 109: r = 0x43C; break;
    case 110: r = 0x43D; break;
    case 111: r = 0x43E; break;
    case 112: r = 0x43F; break; //112 - 127
    case 113: r = 0x447; break;
    case 114: r = 0x440; break;
    case 115: r = 0x441; break;
    case 116: r = 0x442; break;
    case 117: r = 0x443; break;
    case 118: r = 0x432; break;
    case 119: r = 0x449; break;
    case 120: r = 0x448; break;
    case 121: r = 0x44B; break;
    case 122: r = 0x437; break;
    case 123: r = '-'; break;
    case 124: r = 0x2014; break; // em dash
    case 125: r = 0x2116; break; // numero sign
    case 126: r = 0x44C; break;
    case 127: r = 0x44A; break;
    }
  } else if(what == 2) { // OT1 128 bits, parttly implemented
    if(33 <=c && c <= 123)  r = c ;
    switch(c) {
    case 0: r = 0x393; break; // Gamma
    case 1: r = 0x394; break; // Delta
    case 2: r = 0x398; break; // Theta
    case 3: r = 0x39B; break; // Lamnda
    case 4: r = 0x39E; break; // Xi
    case 5: r = 0x3A0; break; // Pi 
    case 6: r = 0x3A3; break; // Sigma
    case 7: r = 0x3A5; break; // Upsilon
    case 8: r = 0x3A6; break; // Phi
    case 9: r = 0x3A8; break; // Psi
    case 10: r = 0x3A9; break; // Omega
    case 25: r = 0xDF; break; // ss
    case 26: r = 0xE6; break; // ae
    case 27: r = 0x153; break; // oe
    case 28: r = 0xF8; break; // \o
    case 29: r = 0xC6; break; // AE
    case 30: r = 0x152; break; // OE
    case 31: r = 0xD8; break; // \O
    case 60: r = 0xA1; break; // exclam
    case 62: r = 0xBF; break; // question
    case 92: case 94: case 95: r = 0; break;
    }
  } else { // T1
    if(c==0xFF) r = 0xDF; // \ss
    else if(c==0xDF) r = 0x0; // SS is not a char in Unicode
    else if(c==0xD7) r = 0x152; // \OE
    else if(c==0xF7) r = 0x153; //\oe
    else if(0xC0 <=c && c <= 255)  r = c ;
    else if(33 <=c && c <= 126)  r = c ;
    else if(c==32) r = 0x2423; // visible space
    else if(c==127) r = '-'; // is this ok ?
    // if c<32 we reject
    else switch(c) {
      case 128: r = 0x102; break; // A breve
      case 129: r = 0x104; break; // A ogonek
      case 130: r = 0x106; break; // C acute
      case 131: r = 0x10C; break; // C caron
      case 132: r = 0x10E; break; // D caron
      case 133: r = 0x11A; break; // E caron
      case 134: r = 0x118; break; // E ogonek
      case 135: r = 0x11E; break; // G breve
      case 136: r = 0x139; break; // L acute
      case 137: r = 0x13D; break; // L caron
      case 138: r = 0x141; break; // L stroke
      case 139: r = 0x147; break; // N acute
      case 140: r = 0x143; break; // N caron
      case 141: r = 0x14A; break; // Eng
      case 142: r = 0x150; break; // O double acute
      case 143: r = 0x154; break; // R acute
      case 144: r = 0x158; break; // R caron
      case 145: r = 0x15A; break; // S acute
      case 146: r = 0x160; break; // S caron
      case 147: r = 0x15E; break; // S cedilla
      case 148: r = 0x164; break; // T caron
      case 149: r = 0x162; break; // T cedilla
      case 150: r = 0x170; break; // U double acute
      case 151: r = 0x16E; break; // U ring
      case 152: r = 0x178; break; // Y diaeresis
      case 153: r = 0x179; break; // Z acute
      case 154: r = 0x17D; break; // Z caron
      case 155: r = 0x17B; break; // Z dot
      case 156: r = 0x132; break; // IJ 
      case 157: r = 0x130; break; // I dot
      case 158: r = 0x111; break; // d stroke
      case 159: r = 0xA7; break; // section
      case 160: r = 0x103; break; // a breve
      case 161: r = 0x105; break; // a ogonek
      case 162: r = 0x107; break; // c acute
      case 163: r = 0x10D; break; // c caron
      case 164: r = 0x10F; break; // d caron
      case 165: r = 0x11B; break; // e caron
      case 166: r = 0x119; break; // e ogonek
      case 167: r = 0x11F; break; // g breve
      case 168: r = 0x13A; break; // l acute
      case 169: r = 0x13E; break; // l caron
      case 170: r = 0x142; break; // l stroke
      case 171: r = 0x144; break; // n acure
      case 172: r = 0x148; break; // n caron
      case 173: r = 0x14B; break; // eng
      case 174: r = 0x151; break; // o double acute
      case 175: r = 0x155; break; // r acute
      case 176: r = 0x159; break; // r caron
      case 177: r = 0x15B; break; // s acute
      case 178: r = 0x161; break; // s caron
      case 179: r = 0x15F; break; // s cedilla
      case 180: r = 0x165; break; // t caron
      case 181: r = 0x163; break; // t cedilla
      case 182: r = 0x171; break; // u double acute
      case 183: r = 0x16F; break; // u ring
      case 184: r = 0xFF; break; // y diaeresis
      case 185: r = 0x17A; break; // z acute
      case 186: r = 0x17E; break; // z caron
      case 187: r = 0x17C; break; // z dot
      case 188: r = 0x133; break; // ij
      case 189: r = 0xA1; break; // inverted exclam
      case 190: r = 0xBF; break; // inverted question
      case 191: r = 0xA3; break; // pound
    }
  }
  if(vb) {
    Buffer& B = trace_buffer;
    B.reset(); B.push_back("-> \\char\""); B.push_back16(r);
    the_log << lg_start<< T << c << B << "." << lg_end;
  }
  if(!r) {
    Buffer& B = trace_buffer;
    B.reset(); B.push_back("Invalid chararacter specification ");
    B.push_back(T); B.push_back("{");B.push_back_int(c); B.push_back("}");
    parse_error(B.c_str());
    return;
  }
  Token t = Token(other_t_offset, Utf8Char(r));
  back_input(t);
}

// translates \ensuremath{foo} outside math mode
// Note that \ensuremath is a robust command, in LaTeX so that 
// \if$\ensuremath true \else false \fi is false.
// Thus, we can implement is as a non-macro in Tralics
// 
void Parser::T_ensuremath()
{
  TokenList L = mac_arg();
  if(the_stack.get_mode() != mode_math) {
    L.push_front(hash_table.relax_token);
    L.push_front(hash_table.dollar_token);
    L.push_back(hash_table.dollar_token);
  }
  if(tracing_macros())
    the_log << lg_start <<"\\ensuremath-> " << L << lg_end;
  back_input(L);
}


void Condition::dump() const
{
  int n = D.size();
  for(int i=n-1;i>=0;i--) D[i].dump(i);
}

void CondAux::dump(int i) const
{
  the_log << lg_start << "### level " << i << " serial " << serial << ": ";  
  int T = cur_if;
  if(T>= unless_code) {
    T -= unless_code;
    the_log << "\\unless";
  }
  CmdChr tmp(if_test_cmd,subtypes(T));
  the_log << "\\" << tmp.name();
  if(if_limit==fi_code) the_log << "\\else";
  the_log << " entered on line " << if_line;
  the_log << lg_end;
}

int Condition::top_branch() const
{
  int k = top_limit();
  if(k==or_code || k== else_code) return 1;
  else if(k==fi_code) return -1;
  else return 0;
}

int Condition::top_type() const
{
  if(D.empty()) return 0;
  int n =  D.back().get_type();
  if(n<unless_code) return n+1;
  else return -(n-unless_code+1);
}

// Prints trace for \if in pass_text
// If k=-1, this is \if, must print \unless,
// If k=-2, this is \if, must not print \unless,
void Parser::trace_if(int k)
{
  if(tracing_commands()) {
    the_log << lg_startcond;
    if(k==-1) the_log << "\\unless";
    the_log << cur_tok << conditions.top_serial();
    if(k>0) the_log << "(+" << k << ")";
    the_log << lg_end;
  }
}

void Parser::trace_if(String a, int k, String b)
{
  if(tracing_commands()) 
    the_log << lg_startcond << a << k << " " << b << lg_end;
}

// same code
void Parser::trace_if(String a, int k, int b)
{
  if(tracing_commands()) 
    the_log << lg_startcond << a << k << " " << b << lg_end;
}

// Find \fi, \else , \or at level zero.
void Parser::pass_text()
{
  int l = 0;
  SaveScannerStatus foo(ss_skipping);
  for(;;) {
    if(get_token()) {
      cur_tok = hash_table.fi_token;
      cur_cmd_chr  = CmdChr(fi_or_else_cmd, fi_code);
      lost_if(conditions.top_line());
      return;
    }
    if(cur_cmd_chr.get_cmd()==fi_or_else_cmd) {
      trace_if(l);
      if(l==0) return;
      if(cur_cmd_chr.get_chr() == fi_code) l--;
    } else if(cur_cmd_chr.get_cmd()==if_test_cmd ||
	      cur_cmd_chr.get_cmd()==fpif_cmd) {
      l++;
      trace_if(l);
    }
  }
}

// Pushes a new conditional
uint Condition::push(int chr)
{
  if_serial++;
  D.push_back(CondAux(if_code,chr,the_parser.get_cur_line(),if_serial));
  return D.size();
}

// Pops a conditional
void Condition::pop()
{
  if(D.empty()) 
    main_ns::log_and_tty << "Error: attempt to pop empty conditional stack\n";
  else D.pop_back();
}

// This pops all conditions, signaling errors.
void Condition::terminate()
{
  int n = D.size();
  while(n>0) {
    n--;
    main_ns::nb_errs++;
    main_ns::log_and_tty << "Unterminated \\if "<< top_serial()
			 << ", started at line "
			 << top_line() <<"\n";
    pop();
  }
}

// Insert \relax before current token. 
// for instance \input foo\input bar.
void Parser::insert_relax ()
{
  back_input();
  cur_tok = hash_table.frozen_relax_token;
  back_input();
}

// This is called when we see \fi \or \else, and don't expect it via
// pass_text. If it's an \else, we call pass_text to get the \fi.
// the codes are, in order: if_code,fi_code,else_code,or_code.

void Parser::finish_fi()
{
  trace_if(0);
  if(conditions.empty()) {
    extra_fi_or_else();
    return;
  }
  int K = conditions.top_limit();
  if(K==if_code) insert_relax();
  else if(cur_cmd_chr.get_chr() > K)
    extra_fi_or_else();
  else {
    while(cur_cmd_chr.get_chr() != fi_code) pass_text();
    conditions.pop();
  }
}

void Parser::T_unless()
{
  get_token();
  if(cur_cmd_chr.get_cmd() == if_test_cmd
     && cur_cmd_chr.get_chr() != if_case_code)
    conditional(cur_cmd_chr.get_chr(),true);
  else {
    parse_error("You cannot use \\unless before ", cur_tok, "","bad unless");
    back_input();
  }
}  

void Parser::conditional (subtypes test, bool negate)
{
  uint sz = conditions.push(negate? test+unless_code : test);
  int k = conditions.top_serial();
  trace_if(negate? -1 : -2);
  if(test==if_case_code) {
    int n = scan_int(cur_tok);
    trace_if("\\ifcase", k, n);
    while(n!=0) {
      pass_text();
      if(conditions.is_this_if(sz)) {
	if(cur_cmd_chr.get_chr()==or_code) n--;
	else if(cur_cmd_chr.get_chr() == fi_code) {
	  trace_if("\\ifcase", k, "failed");
	  conditions.pop();
	  return;
	} else {
	  trace_if("\\ifcase", k, "choose else");
	  conditions.wait_for_fi();
	  return;
	}
      } else if(cur_cmd_chr.get_chr() == fi_code) conditions.pop();
    }
    conditions.set_limit(sz,or_code);
    trace_if("\\ifcase",k,"found");
    return;
  }
  bool b = eval_condition(test);
  if(negate) b = !b;
  trace_if("iftest", k, boolean(b));
  if(b) { 
    conditions.set_limit(sz,else_code);
    return;
  }
  for(;;) {
    pass_text();
    if(conditions.is_this_if(sz)) {
      if(cur_cmd_chr.get_chr() != or_code) break;
      else extra_fi_or_else(); 
    } else 
      if(cur_cmd_chr.get_chr()==fi_code) conditions.pop();
  }
  if(cur_cmd_chr.get_chr()==fi_code) conditions.pop();
  else conditions.wait_for_fi();
}

// Returns the truth value of the next condition
bool Parser::eval_condition(subtypes test) 
{ 
  switch(test) {
  case if_true_code: return true;
  case if_false_code: return false;
  case ifx_code: return expand_ifx();
  case if_char_code :
  case if_cat_code :
    {
      get_x_token_or_active_char();
      CmdChr mn = cur_cmd_chr;
      get_x_token_or_active_char();
      if (test==if_char_code) 
	return mn.get_chr()==cur_cmd_chr.get_chr();
      else return mn.get_cmd()==cur_cmd_chr.get_cmd();
    }
  case if_odd_code : 
    {
      int k = scan_int(cur_tok); return (k&1) == 1;
    }
  case if_leapyear_code:
    return tralics_ns::is_leap_year(scan_braced_int(cur_tok));
  case if_int_code:
  case if_dim_code:
    {
      Token T = cur_tok;
      if(test==if_int_code) scan_int(T); else scan_dimen(false,T);
      int n = cur_val.get_int_val();
      remove_initial_space();
      int r = cur_tok.val_as_other();
      if(r=='=' || r=='<' || r=='>') {}
      else { r='='; missing_equals(); }
      if(test==if_int_code) scan_int(T); else scan_dimen(false,T);
      if(r=='<') return n<cur_val.get_int_val();
      if(r=='>') return n>cur_val.get_int_val();
      return n==cur_val.get_int_val();
    }
  case if_mmode_code: return the_stack.get_mode() == mode_math;
  case if_vmode_code: return the_stack.in_v_mode();
  case if_hmode_code: return the_stack.in_h_mode();
  case if_eof_code: return is_in_open();
  case if_csname_code: 
    fetch_name2();
    return hash_table.is_defined(fetch_name_res);
  case if_defined_code:
    {
      SaveScannerStatus tmp(ss_normal);
      get_token();
      return !cur_cmd_chr.is_undef();
    }
  case if_fontchar_code:
    {
      int k = scan_font_ident();
      scan_char_num();
      return k!= 0;
    }
  case if_void_code: {
    int n = scan_eight_bit_int();
    Xmlp x = box_table[n].get_val();
    if(!x) return true;
    return x->empty();
  }
  case if_hbox_code:
  case if_vbox_code:
    scan_eight_bit_int();
    return false;
  case if_inner_code:
    if( the_stack.get_mode() == mode_math)
      return is_inner_math();
    return false;
  default:
    return false;
  }
}

// Returns truth value of \ifx \A\B
bool Parser::expand_ifx ()
{
  if(get_token_o()) { parse_error("\\if at end of file"); return false; }
  CmdChr pq = cur_cmd_chr;
  if(get_token_o())  { parse_error("\\if at end of file"); return false; }
  if (cur_cmd_chr.get_cmd() != pq.get_cmd()) return false;
  if (cur_cmd_chr.get_cmd() >= user_cmd)
    {
      Macro& A = mac_table.get_macro(cur_cmd_chr.get_chr());
      Macro& B = mac_table.get_macro(pq.get_chr());
      return A.is_same(B);
    } else
    return (cur_cmd_chr.get_chr() == pq.get_chr());
}

// If the tokens that follows are that of the string S (upper or lower case)
// then they are read. Initial spaces are read. Full expansion.
bool Parser::scan_keyword(String s)
{
  int k = 0;
  TokenList L;
  for(;;) {
    if(s[k]==0) break;
    get_x_token();
    if(cur_tok.not_a_cmd() &&(cur_cmd_chr.char_val()==s[k] || 
		    cur_cmd_chr.char_val()==s[k]+'A'-'a')) {
      L.push_back(cur_tok);
      k++;
    }
    else if(L.empty() && cur_cmd_chr.is_space()) continue;
    else { back_input(); back_input(L); return false; }
  }
  return true;
}

// This handles \catcode, \lccode etc, in a SET context.
void Parser::assign_def_something(bool gbl)
{
  Token T = cur_tok;
  int n; 
  int offset=cur_cmd_chr.get_chr();
  if (offset==0) n = 15; // catcode
  else if (offset==math_code_offset)  n = 0100000;
  else if (offset==sf_code_offset)  n = 077777;
  else if (offset==del_code_offset) n = 077777777;
  else if (offset==lc_code_offset || offset==uc_code_offset) 
    n=nb_characters-1;
  else n = 0; // This should not happen
  int k = scan_char_num(); 
  offset += k;
  scan_optional_equals();
  k = scan_int(T);
  if((k<0 && offset<del_code_offset)|| k>n) {
    err_ns::local_buf << bf_reset << "Invalid code (out of bounds)\n",
    err_ns::local_buf << T.tok_to_str() << " wants 0<=N<=" << n 
		      << ", got N=" <<k;
    err_ns::signal_error("number too big",0);
    k = 0;
  }
  word_define(offset,k,gbl);
}

// The result of \the\count0 could be 123, a list of three tokens.
// Overloaded by eTex for unexpanded and detokenize
void Parser::ins_the_toks ()
{
  String s = cur_cmd_chr.name(); if(!s) s = "??";
  TokenList L = the_toks();
  if(tracing_commands()) 
    the_log << lg_start<< "\\" << s << "->" << L << "." << lg_end;
  back_input(L);
}

// This implements \def, \let, etc.
void Parser::define_something (int chr, bool gbl,symcodes what)
{
  switch(chr) {
  case xdef_code: see_new_def(true,true,what);  return;
  case def_code: see_new_def(false,gbl,what); return;
  case gdef_code: see_new_def(false,true,what); return;
  case edef_code: see_new_def(true,gbl,what); return;
  case let_code: see_let(gbl); return;
  case newcommand_code: get_new_command(rd_if_undef,false); return; 
  case checkcommand_code: get_new_command(rd_never,false); return; 
  case newthm_code: see_new_thm(); return;
  case newenv_code:  get_new_env(rd_if_undef,false); return;
  case renewenv_code:  get_new_env(rd_if_defined,false); return;
  case renew_code: get_new_command(rd_if_defined,false); return; 
  case provide_code: get_new_command(rd_skip,false); return; 
  case declare_math_operator_code: declare_math_operator(); return;
  case futurelet_code: see_future_let(gbl);
  }
}

// This implements \let.
void Parser::see_let(bool gbl)
{
  int pos = get_r_token();
  Token W = cur_tok;
  skip_initial_space();
  if(cur_tok.is_equal_token()) {
    get_token();
    if(cur_cmd_chr.is_space()) get_token();
  }
  if(tracing_commands()) {
    the_log << lg_startbracebs << "let " << W  << " ";
    the_log << cur_tok << lg_endbrace;
  }
  if(tracing_assigns()) {
    String action = gbl ? "globally " :  ""; 
    the_log << lg_startbrace << action << "changing " << W <<"=" ;
    token_show(hash_table.eqtb[pos].get_cmdchr());
    the_log << "}\n{into " << W << "=";
    token_show(cur_cmd_chr);
    the_log << lg_endbrace; 
  }
  eq_define(pos,cur_cmd_chr,gbl);
}

// This is like Knuth's get_r_token, except that we hack the case of an
// argument of \newcommand.
int Parser::get_r_token(bool br)
{
  for(;;) {
    get_token(); 
    if (!cur_tok.is_space_token())
      break;
  }
  if(!cur_tok.not_a_cmd()) 
    return cur_tok.eqtb_loc(); // that's OK
  if(br && cur_cmd_chr.is_open_brace()) {
    TokenList L;
    skip_group0(L);
    if(L.empty()) {
      parse_error("Empty argument list for ",name_for_error,
		  "","empty argument");
      cur_tok = hash_table.frozen_protection;
    } else if(L.size() != 1) {
      get_new_command_aux(L);
      parse_error("Only one token allowed in argument list of ",
		  name_for_error,"","bad argument");
      cur_tok = hash_table.frozen_protection;
    } else
      cur_tok = token_ns::get_unique(L);
  }
  if(cur_tok.not_a_cmd()) {
    bad_redefinition(2,cur_tok);
    cur_tok = hash_table.frozen_protection;
  }
  look_at_mac(); 
  return cur_tok.eqtb_loc();
}

// This implements \futurelet\A\B\C
void Parser::see_future_let(bool gbl)
{
  int p = get_r_token();
  Token W = cur_tok;
  get_token();
  Token q = cur_tok;
  get_token();
  if(tracing_commands()) {
    the_log << lg_startbracebs << "futurelet " << W << " ";
    the_log << q << " ";
    the_log << cur_tok << lg_endbrace;
  }
  eq_define(p,cur_cmd_chr,gbl);
  back_input();
  cur_tok = q;
  back_input();
}

// q is register, advance, multiply, divide
// We may have seen \count: here we read a number, the register number.
// We may have seen \advance, and read \count. Then we read the number.
// We may have seen \advance, then \foo, where \foo is defined by
// \countdef, so that we can return after seeing \foo.
// In any case, returns a position and sets p to the type.
int Parser::do_register_arg(int q, int&p, Token& tfe)
{
  if(q != register_cmd) {
    get_x_token();
    tfe = cur_tok;
    int m = cur_cmd_chr.get_chr();
    if(cur_cmd_chr.get_cmd()==assign_int_cmd) 
      { p = it_int;  return m; }
    if(cur_cmd_chr.get_cmd()==assign_dimen_cmd)
      { p = it_dimen; return m; }
    if(cur_cmd_chr.get_cmd()==assign_glue_cmd) 
      { p = it_glue; return m; }
    if(cur_cmd_chr.get_cmd()==assign_mu_glue_cmd) 
      { p = it_mu; return m; }
  }
  if(cur_cmd_chr.is_undef()) {
    undefined_mac();
    return 0;
  }
  if(cur_cmd_chr.get_cmd() != register_cmd) {
    parse_error("Invalid argument for ", name_for_error, "","bad register");
    cur_tok = invalid_token;
    return 0;
  }
  p = cur_cmd_chr.get_chr(); 
  int m = scan_eight_bit_int(); 
  if(p == it_int) return m+count_reg_offset;
  else if(p==it_dimen) return m+dimen_reg_offset;
  else if (p==it_glue) return m;
  else return m +muskip_reg_offset;
}

// This is a more complex function. 
// Handles \advance\skip0 by \skip1, \divide \skip0 by \count1

void Parser::do_register_command(bool gbl)
{
  Token T = cur_tok;
  int q =cur_cmd_chr.get_cmd();
  int p;
  int l = do_register_arg(q,p,T);
  if(cur_tok.is_invalid()) return; // was an error
  if (q==register_cmd) 
    scan_optional_equals ();
  else scan_keyword("by");
  if (q<multiply_cmd) { // advance or set.
    if(p<it_glue) { 
      if (p==it_int) scan_int(T); else scan_dimen(false,T);
      if (q==advance_cmd) {
	if(p==it_int)
	  cur_val.incr_int(eqtb_int_table[l].get_val());
	else
	  cur_val.incr_dim(eqtb_dim_table[l].get_val());
      }
    } else { 
      scan_glue(internal_type(p),T);
      if (q==advance_cmd) cur_val.incr_glue(glue_table[l].get_val());
    }
  } else { 
    int v = scan_int(T); // Here we need an integer.
    if (p<it_glue) {
      if(p==it_int)
	cur_val.set_int(eqtb_int_table[l].get_val());
      else
	cur_val.set_dim(eqtb_dim_table[l].get_val());
      ScaledInt&W =cur_val.get_scaled();
      if (q==multiply_cmd)
	if (p==it_int)  
	  W.mult_integer(v);
	else W.mult_scaled(v);
      else { // division
	W.divide(v);
      }
    } else { 
      Glue s = glue_table[l].get_val();
      if (q==multiply_cmd) s.multiply(cur_val.get_int_val()); 
      else s.divide(cur_val.get_int_val());
      cur_val.set_glue_val(s);
    }
  }  
  if (p==it_dimen) dim_define(l,cur_val.get_dim_val(),gbl);
  else if (p<it_glue) word_define(l,cur_val.get_int_val(),gbl);
  else glue_define(l,cur_val.get_glue_val(),gbl);
}

// Case of \everypar or \toks0
void Parser::read_a_TokenList(bool gbl)
{
  int p;
  if (cur_cmd_chr.get_cmd()==toks_register_cmd)
    p = scan_eight_bit_int(); 
  else p = cur_cmd_chr.get_chr(); // local into list
  scan_optional_equals();
  remove_initial_space_relax();
  if(cur_cmd_chr.get_cmd() != 1) { // not a brace...
    int q = 0;
    if(cur_cmd_chr.get_cmd()==toks_register_cmd) { 
      q = scan_eight_bit_int(); 
      cur_cmd_chr = CmdChr(assign_toks_cmd,subtypes(cur_val.get_int_val()));
    } 
    else q = cur_cmd_chr.get_chr();
    if(cur_cmd_chr.get_cmd()==assign_toks_cmd) { // everypar etc
      if(p==q) return;
      TokenList Q = toks_registers[q].get_val();
      token_list_define(p,Q,gbl);
      return;
    }
    parse_error("missing brace for token register ",name_for_error,"","missing brace");
    back_input();
  } 
  SaveScannerStatus tmp(ss_absorbing);
  TokenList Q;
  skip_group0(Q);
  token_list_define(p,Q,gbl);
}


// This implements \tracingall
// Last assignment should be \tracingassigns...
void Parser::tracing_all()
{
  word_define(tracingmacros_code,1,false);
  word_define(tracingoutput_code,1,false);
  word_define(tracingcommands_code,1,false);
  word_define(tracingrestores_code,1,false);
  word_define(tracingmath_code,1,false);
  word_define(tracingassigns_code,1,false);
}

// This implements \arabic, \@arabic, etc
void Parser::latex_ctr()
{
  int t = cur_cmd_chr.get_chr(); 
  Token T = cur_tok;
  int n = 0;
  TokenList res;
  if(t<at_number_code) {
    counter(false);
    get_token();
    res.push_back(cur_tok);
  }
  else n = scan_int(T);
  cur_tok = T;
  switch(t) {
  case arabic_code: 
    res.push_front(hash_table.number_token);
    break;
  case roman_code:
    res.push_front(hash_table.roman_numeral_token);
    break;
  case Roman_code:
    res.push_front(hash_table.Roman_numeral_token);
    break;
  case alph_code:
    res.push_front(hash_table.atalph_token);
    break;
  case Alph_code:
    res.push_front(hash_table.atAlph_token);
    break;
  case fnsymbol_code:
    res.push_front(hash_table.fnsymbol_token);
    break;
  case at_alph_code:
    if(n>=1 && n<=26)
      res.push_front(Token(letter_t_offset+'a'-1+n));
    else counter_overflow(n,26);
    break;
  case at_Alph_code:
    if(n>0 && n<=26)
      res.push_front(Token(letter_t_offset+'A'-1+n));
    else counter_overflow(n,26);
    break;
  case at_fnsymbol_code:
    {
      if(n<=0 || n>9) {
	counter_overflow(n,9);
	break;
      }
      if(n==1) { res.push_back(hash_table.star_token); break; }
      if(n==4) { res.push_back(Token(other_t_offset,uchar('\247'))); break; }
      if(n==5) { res.push_back(Token(other_t_offset,uchar('\266'))); break; }
      if(n==7) { 
	res.push_back(hash_table.star_token);
	res.push_back(hash_table.star_token); 
	break; 
      }
      if(n==2) { res.push_back(hash_table.dagger_token); }
      if(n==6) { res.push_back(hash_table.vbar_token); }
      if(n==3) { res.push_back(hash_table.ddagger_token); }
      if(n==8) { 
	res.push_back(hash_table.dagger_token);
	res.push_back(hash_table.dagger_token);
      }
      if(n==9) { 
	res.push_back(hash_table.ddagger_token);
	res.push_back(hash_table.ddagger_token);
      }
      brace_me(res);
      res.push_front(hash_table.ensuremath_token);
    }
  }
  if(tracing_commands()) 
    the_log << lg_start << T << "->"  << res << lg_end;
  back_input(res);
}

// Implementation of \ifthenelse

// \newboolean , \provideboolean
void Parser::new_boolean(subtypes c)
{
  TokenList L= mac_arg();
  if(!my_csname("if","",L,"newboolean",false))
    return;
  get_token();
  if(cur_cmd_chr.is_relax()) {
    back_input(); newif();
  }
  else if(c==1) return;
  else parse_error("boolean ",cur_tok, " already defined in \\newboolean",
		   "bad newboolean");
}

// \setboolean{foo}{true} is the same as \footrue.
// You can say TRUE instead of true.
void Parser::set_boolean()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  String s = "";
  Buffer b;
  csname_aux(B,true,b);
  if(b=="true") s = "true";
  else if(b == "false") s = "false";
  else { 
    parse_error("boolean value neither true nor false: "+ b.to_string()); 
    return;
  }
  if(!my_csname("",s,A,"setboolean",false)) 
    return;
  if(hash_table.eqtb[cur_tok.eqtb_loc()].get_cmdchr().is_relax()) {
    get_token();
    parse_error("Undefined boolean ",cur_tok,"","undefined boolean");
  }
}

// \( and \) serve the role of braces.
void Parser::skip_over_parens()
{
  int b = 1;
  for(;;) {
    if(get_token()) return; // no error ?
    if(cur_tok.is_bs_oparen())
      b++;
    if(cur_tok.is_bs_cparen())
      b--;
    if (b==0) return;
  }
}

inline String skip_or_continue(bool s)
{
  return s ?  "skipping" : "continuing";
}

// This returns true if the ifthenelse condition should be true.
// is recursive...
bool Parser::expand_ifthenelse(Token t)
{
  bool res = true;
  bool must_negate = false;
  for(;;) {
    if(get_token()) break; // seen the end
    Token x = cur_tok;
    if(x.is_space_token() ||cur_cmd_chr.is_relax()) continue;
    if(tracing_commands()) 
      the_log << lg_startif << x << lg_endbrace;
    if(x==hash_table.not_token || x==hash_table.NOT_token) {
      must_negate = !must_negate;
      continue;
    }
    if(x.is_bs_oparen()) {
      res = expand_ifthenelse (t);
      skip_over_parens();
    } else if(x==hash_table.isodd_token) {
      TokenList L = mac_arg();
      back_input(hash_table.relax_token);
      back_input(L);
      int n = scan_int(x);
      res = (n &1) != 0;
    } else if(x==hash_table.isundefined_token) {
      TokenList L = mac_arg();
      if(L.empty())
	res = false;
      else {
	Token target = L.front();
	token_from_list(target);
	res = cur_cmd_chr.is_undef();
      }
    } else if(x==hash_table.lengthtest_token) {
      TokenList L = mac_arg();
      back_input(hash_table.relax_token);
      back_input(L);
      cur_tok = t;
      res = eval_condition(if_dim_code);
    } else if(x==hash_table.boolean_token) {
      TokenList L = mac_arg();
      if(my_csname("if","",L,"boolean",false))
	get_token();
      if(cur_cmd_chr.get_cmd()==if_test_cmd && 
	 cur_cmd_chr.get_chr() ==if_true_code) res = true;
      else res = false;
    } else if(x==hash_table.equal_token) {
      TokenList L1 = mac_arg();
      TokenList L2 = mac_arg();
      cur_tok = t; special_read_mac_body(L1);
      cur_tok = t; special_read_mac_body(L2);
      res = L1 == L2; 
      if(tracing_commands()) 
	the_log << lg_startif<< "equal " << boolean(res) << lg_endbrace;
    } else {
      back_input();
      cur_tok = t;
      res = eval_condition(if_int_code);
    }
    break;
  }
  if(must_negate) res =  !res;
  for(;;) { // we have the value. Look at what remains.
    if(get_token()) break; // seen the end
    Token x = cur_tok;
    if(x.is_space_token() ||cur_cmd_chr.is_relax()) continue;
    if(x.is_bs_cparen()) {
      back_input(x);
      break;
    }
    if(x==hash_table.or_token || x==hash_table.OR_token){
      if(tracing_commands()) 
	the_log << lg_startif << "\\or " <<skip_or_continue(res) <<lg_endbrace;
      if(res) break;
      return expand_ifthenelse(t);
    }
    if(x==hash_table.and_token || x==hash_table.AND_token){
      if(tracing_commands()) 
	the_log << lg_startif<< "\\and " <<skip_or_continue(!res)<<lg_endbrace;
      if(!res) return false;
      return expand_ifthenelse(t);
    }
    break;
  }      
  if(tracing_commands()) 
    the_log << lg_startif<< "-> " << boolean(res) << lg_endbrace;
  return res;
}

// Evaluates one of the two token lists, depending on the flag.
void Parser::one_of_two(TokenList& A, TokenList& B, bool which)
{
  if(which) {
    back_input(A);
  } else {
    back_input(B);
  }
}

// Like above, but reads the lists first.
void Parser::one_of_two(bool which)
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  one_of_two(A,B,which);
}

// This is the \ifthenelse function
void Parser::ifthenelse ()
{
  Token t = cur_tok;
  TokenList A = mac_arg();
  cur_tok = t;
  bool b = ifthenelse(A);
  one_of_two(b);
}

// This evaluates a condition in an environment.
bool Parser::ifthenelse(TokenList& A)
{
  Token t = cur_tok;
  SaveState st_state;
  save_the_state(st_state);
  restricted = true;
  TL.swap(A);
  bool b = expand_ifthenelse(t);
  restore_the_state(st_state);
  return b;
}

void Parser::while_do()
{
  Token t = cur_tok;
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  cur_tok = t;
  TokenList AA = A;
  bool b = ifthenelse(AA);
  if(!b) return;
  TokenList BB = B;
  back_input_braced(BB);
  back_input_braced(A);
  back_input(t);
  back_input(B);
  return;
}

// implementation of the calc package.

// We want to multiply res by val.
void Parser::calc_spec_mul(RealNumber val,SthInternal& res)
{
  cur_val = res;
  cur_val.change_level(it_dimen);
  multiply_dim(val,cur_val.get_int_val());
  res.copy(cur_val);
}

// This computes num/den with 6 decimals (why not use C++ code ?)
// and constructs \pm(tmp+f/2^16)
// After that, we multiply.
void Parser::calc_ratio_eval(int num, int den,SthInternal& res)
{
  static const unsigned int my_max = ((1u<<31)-1);
  RealNumber val;
  if(num<0) { val.set_negative(true); num = -num; }
  if(den<0) { val.change_sign(); den = -den; }
  while(den>= int(my_max/10)) { den/=2; num /=2; }
  if(den==0) { parse_error("Division by zero"); return; }
  int A = num/den;
  val.set_ipart(A); // Integer part of the thing.
  int table[17];
  for(int k=0;k<10;k++) {
    num -= A*den;
    num *= 10;
    A =num/den;
    table[k] = A;
  }
  val.convert_decimal_part(10,table);
  calc_spec_mul(val,res);
}

// Multiplies res by what follows (we must remove braces...)
void Parser::calc_mul_aux (SthInternal&res)
{
  TokenList x = mac_arg();
  back_input(hash_table.space_token); // in case...
  back_input(x);
  RealNumber val;
  val.set_negative(scan_sign());
  back_input();
  scan_double(hash_table.calc_token,val);
  skip_initial_space_and_back_input();
  calc_spec_mul(val,res);
}


// Reads something, and puts it in A.
// Note: \widthof{box} is not handled. We return 0.
void Parser::calc_primitive(SthInternal&A)
{
  if(get_token()) {
    parse_error("eof in calc");
    return;
  }
  if(cur_tok.is_open_paren())
    calc_aux(A);
  else if(cur_cmd_chr.get_cmd() == unimp_cmd && 
	  (cur_cmd_chr.get_chr() == widthof_code ||
	   cur_cmd_chr.get_chr() == depthof_code ||
	   cur_cmd_chr.get_chr() ==heightof_code)) {
    ignore_next_arg(); 
    A.initialise(A.get_type()); // this puts 0 in A.
  }
  else {
    back_input();
    scan_optional_equals(); // can do no harm.
    if(A.is_dimen()) {
      scan_dimen(false,hash_table.calc_token); 
      A.set_int_val(cur_val.get_int_val());
    } else if (A.is_glue()) {
      scan_glue(it_glue,hash_table.calc_token);
      A.set_glue_val(cur_val.get_glue_val());
    } else {
      scan_int(hash_table.calc_token);
      A.set_int_val(cur_val.get_int_val());
    }
  }
  if(tracing_commands())
    the_log << lg_startbrace << "calc primitive =" << A << lg_endbrace;
}

// This is the big function
void Parser::calc_aux(SthInternal& A)
{
  skip_initial_space_and_back_input();
  calc_primitive(A);
  for(;;) {
    skip_initial_space();
    Token T = cur_tok; // what next ?
    if(T.is_exclam_token()) return;  // special end marker.
    if(T.is_close_paren()) return;
    if(T.is_plus_token() || T.is_minus_token()) {
      if(tracing_commands()) the_log << lg_startcalc << "+-" << lg_endbrace;
      SthInternal B;
      B.initialise(A.get_type());
      calc_primitive(B);
      if(T.is_minus_token()) B.negate();
      A.add(B);
      continue;
    }
    if(tracing_commands())  the_log << lg_startcalc << "*/" << lg_endbrace;
    if(!(T.is_star_token()||T.is_slash_token())) {
      parse_error("unexpected token in calc\n", T.tok_to_str(),
		  "unexpected in calc");
      return;
    }
    bool mul = T.is_star_token();
    skip_initial_space();
    T = cur_tok;
    if(T==hash_table.ratio_token) {  
      TokenList x = mac_arg();
      TokenList y = mac_arg();
      if(mul) ratio_evaluate(x,y,A);
      else ratio_evaluate(y,x,A);
      continue;
    }
    if(T==hash_table.real_token) { 
      if(!mul) { // special case 3 / \real{1.5} ...
	TokenList x = mac_arg(); 
	x.push_back(Token(other_t_offset,'p'));
	x.push_back(Token(other_t_offset,'t'));
	TokenList y; // empty
	ratio_evaluate(y,x,A);
	continue;
      } else { // case 3 * \real{1.5}
	calc_mul_aux(A);
	continue;
      }
    } else {
      // Could be anything, says (2+3)*(4+5);
      // Must be an integer...
      SthInternal aux;
      aux.initialise(it_int);
      back_input();
      calc_primitive(aux);
      int v = aux.get_int_val();
      if(mul) {
	if(A.is_glue()) A.glue_multiply(v);
	else if(A.is_int()) 
	  A.get_scaled().mult_integer(v);
      else  A.get_scaled().mult_scaled(v);
      } else {
	if(A.is_glue()) A.glue_divide(v);
	else  A.get_scaled().divide(v);
      }
    }
  }
}

// We want to multiply res by A/B.
void Parser::ratio_evaluate(TokenList& A, TokenList& B,SthInternal&res)
{
  SthInternal num,den;
  num.initialise(it_int);
  den.initialise(it_int);
  if(A.empty()) 
    num.set_int_val(1 <<16);  // Same as 1pt, but faster.
  else 
    calc_main(it_dimen,num,A);
  calc_main(it_dimen,den,B);
  if(tracing_commands())
    the_log << lg_startcalc << "\\ratio "<< num << "/" << den << lg_endbrace;
  calc_ratio_eval(num.get_int_val(),den.get_int_val(),res);
}

void Parser::calc_main(internal_type type,SthInternal& res, TokenList& B)
{
  SthInternal A;
  A.initialise(type);
  if(tracing_commands()) 
    the_log << lg_startcalc << "argument: " << B << lg_endbrace;
  back_input(Token(other_t_offset,'!'));
  back_input(B);
  calc_aux(A);
  if(tracing_commands()) {
    the_log << lg_startcalc << "res " <<  A << lg_endbrace;
  }
  res.set_type(type);
  res.copy(A);
}

void Parser::exec_calc()
{
  TokenList a = mac_arg();
  TokenList b = mac_arg();
  bool gbl = false;
  bool advance = false;
  if(a.front() ==hash_table.global_token) {gbl = true; a.pop_front(); }
  if(a.front() ==hash_table.advance_token) {advance = true; a.pop_front(); } 
  Token T = a.front();
  a.pop_front();
  token_from_list(T);
  internal_type p;
  int l = cur_cmd_chr.get_chr();
  if(cur_cmd_chr.get_cmd()==assign_int_cmd) p = it_int;
  else if(cur_cmd_chr.get_cmd()==assign_dimen_cmd) p = it_dimen;
  else p = it_glue;  // ok ? should add a test here....
  if(tracing_commands()) {
    String s = p==it_int ? "integer" : (p==it_dimen ? " dimension"   : "glue");
    the_log << lg_startcalc << "modifying " <<  s 
	    << " at position " << l << lg_endbrace;
  }  
  SthInternal res; 
  calc_main(p,res,b);
  cur_val = res;
  if(advance) {
    if(p==it_glue)
      cur_val.incr_glue(glue_table[l].get_val());
    else if(p==it_dimen)
      cur_val.incr_dim(eqtb_dim_table[l].get_val());
    else 
      cur_val.incr_int(eqtb_int_table[l].get_val());
  }
  if(p==it_glue)
    glue_define(l,cur_val.get_glue_val(),gbl);
  else if(p==it_dimen)
    dim_define(l,cur_val.get_dim_val(),gbl);
  else
    word_define(l,cur_val.get_int_val(),gbl);
}

// You say \strip@pt\dimen25, this gives 34.37 or 34 (no dot)
// Assume arg is a dimension


void Parser::dimen_from_list0(Token T,TokenList& L)
{
  back_input(hash_table.relax_token);
  back_input(L);  
  scan_dimen(false,T);
  read_until(hash_table.relax_token);
}

ScaledInt Parser::dimen_from_list(Token T,TokenList& L)
{
  if(!calc_loaded) dimen_from_list0(T,L);
  else calc_main(it_dimen,cur_val,L);
  return cur_val.get_dim_val();
}

// This interprets \countdef, and things like that
void Parser::shorthand_define(int cmd, bool gbl)
{
  Token t = cur_tok;
  int pos = get_r_token();
  Token tbd = cur_tok;
  if(cur_tok == hash_table.frozen_protection) return;
  eq_define(pos,CmdChr(relax_cmd,relax_code),gbl);
  scan_optional_equals();
  cur_tok = t; 
  int k; symcodes ncmd;
  String name = "unknown";
  switch(cmd) {
  case char_def_code: 
    name = "chardef";
    k = scan_27bit_int(); ncmd = char_given_cmd; break;
  case math_char_def_code: 
    name = "mathchardef";
    k = scan_fifteen_bit_int(); ncmd = math_given_cmd; break;
  case count_def_code:
    name = "countdef";
    k = scan_eight_bit_int(); 
    k += count_reg_offset;
    ncmd = assign_int_cmd; 
    break;
  case dimen_def_code: 
    name = "dimendef";
    k = scan_eight_bit_int(); 
    k += dimen_reg_offset;
    ncmd = assign_dimen_cmd;
    break;
  case skip_def_code: 
    name = "skipdef";
    k = scan_eight_bit_int(); ncmd = assign_glue_cmd; break;
  case mu_skip_def_code: 
    name = "muskipdef";
    k = scan_eight_bit_int(); 
    k += muskip_reg_offset;
    ncmd = assign_mu_glue_cmd; 
    break;
  case toks_def_code: 
    name = "tokesdef";
    k = scan_eight_bit_int(); ncmd = assign_toks_cmd; break;
  default:
    return;
  }
  CmdChr R(ncmd,subtypes(k));
  eq_define(pos,R,gbl);
  the_log << lg_startbracebs << name <<" " << tbd 
	  << "=\\" << R.name() <<lg_endbrace;
  return;
}

void Parser::shorthand_gdefine(int cmd, String sh, int k)
{
  symcodes ncmd;
  String name = "unknown";
  switch(cmd) {
  case char_def_code: 
    name = "chardef";
    ncmd = char_given_cmd; break;
  case math_char_def_code: 
    name = "mathchardef";
    ncmd = math_given_cmd; break;
  case count_def_code:
    name = "countdef";
    k += count_reg_offset;
    ncmd = assign_int_cmd; 
    break;
  case dimen_def_code: 
    name = "dimendef";
    k += dimen_reg_offset;
    ncmd = assign_dimen_cmd;
    break;
  case skip_def_code: 
    name = "skipdef";
    ncmd = assign_glue_cmd; break;
  case mu_skip_def_code: 
    name = "muskipdef";
    k += muskip_reg_offset;
    ncmd = assign_mu_glue_cmd; 
    break;
  case toks_def_code: 
    name = "tokesdef";
    ncmd = assign_toks_cmd; break;
  default:
    return;
  }
  CmdChr R(ncmd,subtypes(k));
  int p = hash_table.locate(sh).eqtb_loc();
  eq_define(p,R,true);
  the_log << lg_startbracebs << name <<" \\" << sh 
	  << "=\\" << R.name() <<lg_endbrace;
}

// Let M be the number of registers (1024)
// scan_box is used by \shipout, \leaders, etc with an argument > 2M+2,
// is used by \moveleft, etc, with 2M as argument,
// is used by \setbox with argument < M,
// to which M is added if the assignment is global.
// The command reads a box, and performs some action.
// Exception: you can say \cleaders\hrule. 

void Parser::scan_box(int bc)
{
  remove_initial_space_relax();
  if(cur_cmd_chr.get_cmd() == make_box_cmd) 
    begin_box(bc,cur_cmd_chr.get_chr());
  else if (bc>=leaders_location && cur_cmd_chr.get_cmd()==rule_cmd) {
    scan_rule(cur_cmd_chr.get_chr());
    box_end(the_stack.remove_last(),bc);
  }
  else parse_error("A box was assumed to be here, got ",
		   cur_tok,"","Missing box"); 
}

// In case of \leaders\glue or \leaders\hbox{} a box or a rule is fetched;
// It will be put in a element named <leaders>. In the case of \box
// we fill the box (numbered pos or pos-M).
// An error is signaled in the case of \shipout, the box is inserted otherwise.
void Parser::box_end(Xmlp res, int pos)
{
  if(pos<last_register) box_define(pos,res,false);
  else if(pos<=setbox_last) box_define(pos-setbox_offset,res,true);
  else if(pos==shipout_location) parse_error("\\shipout is undefined");
  else if(pos==xmlA_location) the_xmlA = res;
  else if(pos==xmlB_location) the_xmlB = res;
  else if(pos==leaders_location|| pos ==cleaders_location 
	  || pos==xleaders_location) {
    flush_buffer();
    name_positions p = pos== leaders_location ? np_leaders : 
      pos == cleaders_location ? np_cleaders : np_xleaders;
    Xmlp Y = new Xml(p,res); 
    the_stack.add_last(Y);
    
  } else {
    flush_buffer();
    the_stack.add_last(res);
  }
}

// This function is called from scan_box, see above, 
// or \box\copy\usebox\hbox etc, src is 2M in these cases
// In some cases, the box is already there, and the code is easy.
// Otherwise we push a continuation on the stack.

void Parser::begin_box(int src, subtypes c)
{
  Token T = cur_tok;
  int res;
  Xmlp cur_box=0;
  if(c==usebox_code) { // a variant of \copy with an argument
    leave_v_mode();
    TokenList L = mac_arg();
    back_input(hash_table.relax_token);
    back_input(L);
    c = copy_code;
  }
  if(c==box_code) {
    res = scan_eight_bit_int();
    cur_box = box_table[res].get_val();
    box_table[res].set_val(0);
    box_end(cur_box,src);
    return;
  } 
  if(c==copy_code) {
    res = scan_eight_bit_int();
    cur_box = box_table[res].get_val();
    box_end(cur_box,src);
    return;
  }
  if(c==lastbox_code) {
    flush_buffer();
    cur_box = the_stack.top_stack()->last_box();
    box_end(cur_box,src);
    return;
  }
  if(c==useboxA_code || c == useboxB_code ) {
    cur_box = (c== useboxA_code ? the_xmlA : the_xmlB);
    if(cur_box) cur_box = cur_box -> deep_copy ();
    else cur_box = new Xml (Istring (""), 0);
    box_end(cur_box,src);
    return;
  }
  if(c==useboxB_code) {
    cur_box = the_stack.top_stack()->last_box();
    box_end(cur_box,src);
    return;
  }
  if(c==vsplit_code) {
    scan_eight_bit_int();
    scan_keyword("to");
    scan_dimen(false,T);
    box_end(cur_box,src);
    return;
  } 
  flush_buffer();
  Istring box_name = Istring("");
  if(c==parbox_code) {
    // same arguments as minipage
    ignore_next_optarg(); // position
    ignore_next_optarg(); // height
    ignore_next_optarg(); // inner_pos
    scan_glue(it_glue,T,false); //width
  }
  else if(c==xbox_code) box_name = nT_next_arg();
  else if(c==marginpar_code) {
    ignore_next_optarg();
    box_name = the_names[np_marginpar];
  }
  else if(scan_keyword("to") || scan_keyword("spread"))
    scan_dimen(false,T);
  if(tracing_commands()) {
    the_log << lg_startbrace;
    if(box_name.empty())
      the_log << "Constructing an anonymous box" << lg_endbrace;
    else 
      the_log << "Constructing a box named " << box_name << lg_endbrace;
  }
  scan_left_brace();
  push_level(bt_brace);
  TokenList L;
  if(c==xbox_code) L= toks_registers[everyxbox_code].get_val();
  else if(c==vbox_code) L = toks_registers[everyvbox_code].get_val();
  else if(c==vtop_code) L = toks_registers[everyvbox_code].get_val();
  else if(c==hbox_code) L = toks_registers[everyhbox_code].get_val();
  if(!L.empty()) {
    if(tracing_commands()) 
      the_log << lg_startbrace  << "<every...> " << L << lg_endbrace;
    back_input(L);
  }
  Xmlp cur_boxa = the_stack.push_hbox(box_name);
  SaveAux *x = new SaveAuxBoxend(src,cur_boxa);
  push_save_stack(x);
  the_stack.set_arg_mode();
}

// Implements \show \showbox and \showthe
void Parser::xray(subtypes c)
{
  switch(c) {
  case show_code: 
    trace_buffer.reset();
    token_show(0,trace_buffer);
    return;
  case showbox_code: 
    {
      int k = scan_eight_bit_int();
      main_ns::log_and_tty << "Box " << k << ": ";
      show_box(box_table[k].get_val());
      return;
    }
  case show_xmlA_code: 
    main_ns::log_and_tty << "xmlA: ";
    show_box(the_xmlA);
    return;
  case show_xmlB_code: 
    main_ns::log_and_tty << "xmlB: ";
    show_box(the_xmlB);
    return;
  case showthe_code: {
    TokenList L = the_toks();
    main_ns::log_and_tty << "\\show: " << L << "\n";
    return;
  }
  case showgroups_code:
    dump_save_stack();
    return;
  case showifs_code: 
    conditions.dump();
    return;
  case showlists_code: 
    the_stack.dump();
    return;
  case showtokens_code: {
    TokenList L = scan_general_text();
    main_ns::log_and_tty << "\\show: " << L << "\n";
    return;
  }
  default: return;
  }
}

void Parser::show_box(Xmlp X)
{
  if(X) main_ns::log_and_tty << X <<"\n"; 
  else main_ns::log_and_tty << "empty.\n";
}

// save_verb or use_verb
void Parser::T_save_use_verb(bool save)
{
  string name = group_to_string();
  mac_buffer << bf_reset << "savedverb@" << name;
  Token t = hash_table.locate(mac_buffer);
  if(save) {
    TokenList body;
    expand_verb1(body);
    new_macro(body,t,true);
  } else { // case useverb
    back_input(t);
  }
}


// Code of \long, \def, \count, etc.
// real action done by prefixed_aux.
// handles \afterassignment.
void Parser::prefixed_command()
{
  int flags = 0;
  for(;;) {
    if(cur_cmd_chr.get_cmd() ==prefix_cmd) {
      flags |= cur_cmd_chr.get_chr();
      remove_initial_space_relax();
    }
    else break;
  }
  bool b_global = false;
  if(flags>=8) { flags -= 8; b_global = true; }
  symcodes K = symcodes(user_cmd + flags);
  if(cur_cmd_chr.get_cmd()<=max_non_prefixed_command) {
    if(b_global || flags) prefix_error(b_global, K);
    if(cur_tok.is_valid()) back_input(); // case of prefix at end of list
    return;
  }
  if(cur_cmd_chr.get_cmd() != def_cmd && flags) 
    prefix_error(b_global, K);
  // look at \globaldefs
  int gd =eqtb_int_table[globaldefs_code].get_val();
  if(gd>0) b_global = true;
  if(gd<0) b_global = false;
  if(tracing_commands() && (b_global || flags)) {
    the_log << lg_startbrace;
    trace_buffer.reset();
    trace_buffer.dump_prefix(true,b_global,K);
    the_log << trace_buffer << cur_tok << lg_endbrace;
  }
  prefixed_aux(b_global,K);
  Token aat = get_after_ass_tok();
  if(!aat.is_null()) {
    back_input(aat);
    if(tracing_commands())
      the_log << lg_startbrace << "after assignment: " << aat << lg_endbrace;
  }
}

void Parser::back_input_braced(TokenList& L)
{
  back_input(hash_table.CB_token);
  back_input(L);
  back_input(hash_table.OB_token);
}

// If the list C is |^x_y| or |_y^x|, sets a to x and b to y,
// If ^x is omitted, A is used, if _y is omitted B is used
// The case |'^x_y| is the same as |^{\prime x}_y
// Inserts {a}{b} into res.
void Parser::scan_updown(TokenList& A,TokenList& B,TokenList&C,TokenList&res)
{
  back_input(hash_table.relax_token); // end marker
  back_input(C);
  TokenList df_up = A; 
  TokenList df_down = B; 
  get_token();
  if(cur_cmd_chr.get_cmd()==12 && cur_cmd_chr.get_chr()=='\'') {
    scan_prime();
    get_token();
  }
  if(cur_cmd_chr.get_cmd()==hat_catcode) {
    df_up = mac_arg();
    get_token();
    if(cur_cmd_chr.get_cmd()==underscore_catcode) {
      df_down = mac_arg();
    } else back_input();
  } else  if(cur_cmd_chr.get_cmd()==underscore_catcode) {
    df_down = mac_arg();
    get_token();
    if(cur_cmd_chr.get_cmd()==12 && cur_cmd_chr.get_chr()=='\'') {
      scan_prime();
      get_token();
    }    
    if(cur_cmd_chr.get_cmd()==hat_catcode) {
      df_up = mac_arg();
    } else back_input();
  }  else back_input();
  read_until(hash_table.relax_token); // read all unused tokens
  brace_me(df_up);  
  brace_me(df_down);
  res.splice(res.end(), df_down);
  res.splice(res.end(), df_up);
}

// \scanupdown\foo{aa}{bb}{^x_y} expands to \foo{x}{y}
// If no x given, then aa is used instead
// If no y given then bb is used instead
// You can say {_y^x}, or {'^x_y} if you like

void Parser::scan_updown()
{
  TokenList cmd = mac_arg();
  TokenList df_up = mac_arg();
  TokenList df_down = mac_arg();
  TokenList w = mac_arg(); 
  scan_updown(df_up,df_down,w,cmd);
  if(tracing_commands()) 
    the_log << lg_start << "\\scanupdown ->" << cmd << lg_end;
  back_input(cmd);
}
 
void Parser::scan_sideset()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  TokenList C = mac_arg();
  TokenList none;
  none.push_back(hash_table.mmlnone_token);
  if(A.empty() && B.empty()) 
    {} // \mathop{C}\limits
  else {
    if(!B.empty())
      scan_updown(none,none,B,C);
    if(!A.empty()) {
      C.push_back(hash_table.mmlprescripts_token);
      scan_updown(none,none,A,C);
    }
    brace_me(C);
    C.push_front(hash_table.mmlmultiscripts_token);
  }
  brace_me(C);
  C.push_front(hash_table.mathop_token);
  C.push_back(hash_table.limits_token);
  if(tracing_commands()) 
    the_log << lg_start << "\\sideset ->" << C << lg_end;
  back_input(C);
}
