// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains the TeX parser of tralics It contains a lot of other
// things, but not the XML generator.

#include "tralics.h"
#include "txclasses.h"
#include "txparser2.h"

const char* txparser2_rcsid=
  "$Id: txparser2.C,v 2.38 2013/07/22 09:28:21 grimm Exp $";

bool xkv_patch = false;

namespace {
  Buffer local_buf,  mac_buf, buf_for_del;
  TokenList KV_list;
  bool xkv_is_global;
  bool xkv_is_save;
  string xkv_prefix = "";
  string xkv_header = "";
  TokenList xkv_action;
}

extern int is_in_skipped;

namespace xkv_ns {
  void find_aux(int c);
  void to_string(TokenList& s, String s1,String s2,String msg,Buffer&B);
  Token to_string(TokenList& s, String s1,String s2,String msg);
  string to_string(TokenList& s,String msg);
  string find_key_of(const TokenList&L,int type);
  void merge(TokenList&W, TokenList& L, int type);
  void remove(TokenList&W, TokenList& L, int type);
  void makehd(const string& L); 
  bool is_Gin(const TokenList& x);
}

namespace token_ns {
  void lower_case(TokenList& L);
  bool find_in(TokenList&A, TokenList&B, Token C, bool);
  void int_to_roman(Buffer&b, int n);
}

namespace classes_ns {
  TokenList cur_options(bool,TokenList&,bool);
  void register_key(const string&);
  void unknown_optionX(TokenList&cur_keyval, TokenList&action);
}

void Parser::new_macro(TokenList& L,Token name, bool gbl)
{
  Macro* X = new Macro(L);
  mac_define(name,X, gbl,rd_always,user_cmd);
}
void Parser::new_macro(TokenList& L,Token name)
{
  Macro* X = new Macro(L);
  mac_define(name,X, false,rd_always,user_cmd);
}

void Parser::new_macro(const string& s,Token name)
{
  mac_buf << bf_reset << s;
  TokenList L = mac_buf.str_toks11(false);
  Macro* X = new Macro(L);
  mac_define(name,X, false,rd_always,user_cmd);
}


// Optimised version, because \zap@space 12 3 {44} 5 6\@empty
// is wrong after all
void Parser::zapspace()
{
  TokenList result;
  Token t2 = hash_table.empty_token;
  for(;;) {
    TokenList L = read_until_long(hash_table.space_token);
    result.splice(result.end(),L);
    if(get_token()) break; else back_input(); // error ?
    if(cur_tok==t2) break;
  }
  back_input(result);
}

// Stuff for fancyheadings
void Parser::T_fancy(String s, TokenList& L)
{
  flush_buffer();
  push_level(bt_brace);
  hash_table.eval_let("thepage","inert@thepage");
  back_input(hash_table.CB_token);
  TokenList tmp = L; // make a copy of the list
  brace_me(tmp);
  back_input(tmp);
  TokenList sl = token_ns::string_to_list(s,true);
  back_input(sl);
  back_input(hash_table.locate("fancyinternal"));
}

void Parser::T_xfancy()
{
  Istring s = nT_next_arg();
  the_stack.push1(np_header);
  the_stack.add_att_to_last(the_names[np_type],s);
  the_stack.set_arg_mode();
  T_next_arg();
  flush_buffer();
  the_stack.pop(np_header);
}

void Parser::T_fancy()
{
  subtypes c = cur_cmd_chr.get_chr();
  int hf =0;
  if(c==fancy_head_code) hf=1;
  else if(c==fancy_foot_code) hf=2;
  else if(c==fancy_hf_code) hf=3;
  if(hf==0) {
    TokenList L1;
    bool o = next_optarg_long(L1);
    TokenList L2 = mac_arg();
    if(!o) L1 = L2;
    if(c==fancy_lhead_code) {
      T_fancy("elh",L1); T_fancy("olh",L2); return;
    } else if(c==fancy_chead_code) {
      T_fancy("ech",L1); T_fancy("och",L2); return;
    } else if(c==fancy_rhead_code) {
      T_fancy("erh",L1); T_fancy("orh",L2); return;
    } else if(c==fancy_lfoot_code) {
      T_fancy("elf",L1); T_fancy("olf",L2); return;
    } else if(c==fancy_cfoot_code) {
      T_fancy("ecf",L1); T_fancy("ocf",L2); return;
    } else if(c==fancy_rfoot_code) {
      T_fancy("erf",L1); T_fancy("orf",L2); return;
    }
  }
  TokenList L1; 
  next_optarg_long(L1);
  TokenList L = mac_arg();
  for(;;) {
    bool use_e = false, use_o = false;
    bool use_h = false, use_f = false;
    bool use_l = false, use_r = false, use_c = false;
    for(;;) {
      if(L1.empty()) break;
      Token t = L1.front();  L1.pop_front();
      if(t.is_comma_token()) break;
      if(t.no_case_letter('e')) { use_e = true;  }
      else if(t.no_case_letter('o')) { use_o = true;  }
      else if(t.no_case_letter('l')) { use_l = true;  }
      else if(t.no_case_letter('c')) { use_c = true;  }
      else if(t.no_case_letter('r')) { use_r = true;  }
      else if(t.no_case_letter('h')) { use_h = true;  }
      else if(t.no_case_letter('f')) { use_f = true;  }
      else continue;
    }
    if(!use_e && !use_o) { use_e = true; use_o = true; }
    if(hf==1) use_h = true;
    if(hf==2) use_f = true;
    if(!use_h && !use_f) { use_h = true; use_f = true; }
    if(!use_l && !use_c &&!use_r) { use_l = true; use_c = true; use_r =true;}
    if(use_h){
      if(use_e) {
	if(use_l) T_fancy("elh", L);
	if(use_c) T_fancy("ech", L);
	if(use_r) T_fancy("erh", L);
      }
      if(use_o) {
	if(use_l) T_fancy("olh", L);
	if(use_c) T_fancy("och", L);
	if(use_r) T_fancy("orh", L);
      }
    }
    if(use_f) {
      if(use_e) {
	if(use_l) T_fancy("elf", L);
	if(use_c) T_fancy("ecf", L);
	if(use_r) T_fancy("erf", L);
      }
      if(use_o) {
	if(use_l) T_fancy("olf", L);
	if(use_c) T_fancy("ocf", L);
	if(use_r) T_fancy("orf", L);
      }
    }
    if(L1.empty()) break;
  }
}


bool CmdChr::is_ok_for_xspace() const 
{
  if(cmd==other_catcode) {
    if(chr< int(nb_xspace_values)) return the_parser.ok_for_xspace[chr];
    return false;
  }
  if(cmd==2 || cmd==10) return true;
  if(cmd==1) return false; // doc says true; FIXME
  if(cmd==cst1_cmd && chr==space_code) return true;
  return false;
}

void Parser::T_xspace()
{
  if(get_token()) return;
  back_input();
  bool val = cur_cmd_chr.is_ok_for_xspace();
  if(tracing_commands()) 
    the_log << lg_start<< "\\xspace after " << cur_tok 
	    <<  (val ? " did nothing " : " added space") << lg_end;
  if(val) return;
  back_input(hash_table.space_token);
}

// arg2 ignored, in arg1 \ is special
string Parser::xmllatex()
{
  TokenList L = mac_arg();
  ignore_next_arg();
  mac_buffer.reset();
  while(!L.empty()) {
    Token x = L.front();
    L.pop_front();
    if(x.get_val()<cs_token_flag+bad_cs) mac_buffer.push_back(x.char_val());
    else if(x.get_val()>=cs_offset) { 
      int xx = x.hash_loc();
      mac_buffer.push_back(hash_table[xx]);
    }
  }
  if(tracing_commands()) 
    the_log << lg_startbrace  << "Rawxml: " << mac_buffer << lg_endbrace;
  return mac_buffer.to_string();
}

void Parser::find_in_config(int c)
{
  Token T = cur_tok;
  TokenList L1 = mac_arg();
  string resource = xkv_ns::to_string(L1,"Problem scanning resource name");
  string key;
  string res;
  if(c) {
    TokenList L2 = mac_arg();
    key = xkv_ns::to_string(L2,"Problem scanning key");
    res =config_ns::find_one_key(resource,key);
  } else res = config_ns::find_keys(resource);
  mac_buf << bf_reset << res;
  TokenList L = mac_buf.str_toks11(false);
  if(tracing_macros()) {
    the_log << lg_start << T << " #1=" << resource;
    if(c) the_log << " #2=" << key;
    the_log <<  " -> " << L << lg_end;
  }
  back_input(L);
}


void Parser::T_usefont()
{
  flush_buffer();
  cur_font.ltfont(sT_next_arg(),fontencoding_code);
  cur_font.ltfont(sT_next_arg(),fontfamily_code);
  cur_font.ltfont(sT_next_arg(),fontseries_code);
  cur_font.ltfont(sT_next_arg(),fontshape_code);
  font_has_changed();    // \selectfont
  remove_initial_space_and_back_input(); // \ignorespaces
}

void Parser::T_unimplemented_font(subtypes c)
{
  Token T = cur_tok;
  if(c==DeclareMathSizes_code) remove_initial_star();
  switch(c) {
  case TextSymbolUnavailable_code://1 arg
  case DeclareMathVersion_code:
  case DeclareMathDelimiter_code:
    ignore_next_arg();
    break;
  case ProvideTextCommand_code: //2 args
  case DeclareTextCommand_code:
  case DeclareTextCommandDefault_code: 
  case ProvideTextCommandDefault_code:
  case DeclareTextSymbolDefault_code:
  case UseTextSymbol_code:
  case UndeclareTextCommand_code:
  case DeclareFontEncodingDefaults_code:
  case DeclareSizeFunction_code:
  case DeclareSymbolFontAlphabet_code:
  case DeclareTextFontCommand_code:
    ignore_next_arg();
    ignore_next_arg();
    break;
  case DeclareTextAccent_code : //3 args
  case UseTextAccent_code:
  case DeclareTextSymbol_code:
  case DeclareFontFamily_code:
  case DeclareFontEncoding_code:
  case DeclareOldFontCommand_code:
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    break;
  case DeclareTextCompositeCommand_code : //4 args
  case DeclareTextComposite_code:
  case DeclareFontSubstitution_code:
  case DeclareMathAccent_code:
  case DeclareMathSymbol_code:
  case DeclareMathSizes_code:
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    break;
  case DeclarePreloadSizes_code : //5 args
  case DeclareErrorFont_code:
  case DeclareSymbolFont_code:
  case DeclareMathAlphabet_code:
  case DeclareMathRadical_code:
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    break;
  case DeclareFontShape_code: //6 args
  case DeclareFixedFont_code:
  case SetSymbolFont_code:
  case SetMathAlphabet_code:
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    ignore_next_arg();
    break;
  default: break;
  }  
  parse_error("Unimplemented NFSS command ",T,"","unimplemented");
  if(c==ProvideTextCommand_code || c==DeclareTextCommand_code) {
    back_input(hash_table.frozen_protection);
    get_new_command(rd_always,false);
  }
}


// raisebox{1cm}[2cm][3cm]{a box}

void Parser::T_raisebox()
{
  Istring A = nT_next_arg();
  Istring B = nT_next_optarg();
  Istring C = nT_next_optarg();
  leave_v_mode();
  the_stack.push1(np_raisebox);
  AttList& cur = last_att_list();
  cur.push_back(np_val,A);
  cur.push_back(np_height,B);
  cur.push_back(np_depth,C);
  T_next_arg_local();
  the_stack.pop(np_raisebox);
}

void Parser::T_isin()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  bool found = token_ns::is_in(A,B,false);
  back_input(found ? hash_table.intrue_token :hash_table.infalse_token);
}

void Parser::expand_car(bool first)
{
  Token T = cur_tok;
  TokenList L = mac_arg();
  TokenList M =  read_until_long(hash_table.nil_token);
  if(tracing_macros()) {
    the_log << lg_start << T << "#1#2\\@nil ->#" << (first ? "1" : "2")<<lg_end;
    the_log << "#1 <-" << L << lg_end;
    the_log << "#2 <-" << M << lg_end;
    the_log << lg_start << T << "<- " << (first? L:M) << lg_end;
  }
  if(first) back_input(L); else back_input(M);
}

// Translation of \foo is <foo/>
void Parser::T_ignoreA()
{
  Token T = cur_tok;
  String s= T.tok_to_str();
  if(s[0]=='\\') ++s;
  if(strcmp(s,"newpage")==0 || strcmp(s,"clearpage")==0 || 
     strcmp(s, "cleardoublepage")==0)
    leave_h_mode();
  flush_buffer();
  unprocessed_xml << "<" <<  s << "/>" ; 
  flush_buffer();
}

void Parser::T_line(subtypes c)
{
  name_positions k = np_center;
  switch(c) {
  case leftline_code: k= np_left; break;
  case rightline_code: k= np_right; break;
  case llap_code: k= np_llap; break;
  case rlap_code: k= np_rlap; break;
  default:;
  }
  if(c== leftline_code||c== centerline_code || c== rightline_code) {
    if(the_stack.is_float()) return;
    leave_v_mode();
  }
  the_stack.push1(np_lineC);
  AttList& cur = last_att_list();
  cur.push_back(np_rend,k);
  T_next_arg_local();
  the_stack.pop(np_lineC);
}

// Implements \typein[\foo]{bar},  or \typein{bar}. In the first case
// it is like \typeout{bar}\read0to\foo, otherwise backinputs what is read.
void Parser::T_typein()
{
  Token cmd = hash_table.relax_token;
  TokenList res;
  bool has_opt = false;
  if(next_optarg_long(res)) { // 
    Token Q = token_ns::get_unique(res);
    back_input(Q);
    get_r_token(false);
    cmd = cur_tok;
    has_opt = true;
  }
  main_ns::log_and_tty << tex_write(negative_out_slot); // \typeout next arg
  int cc = eqtb_int_table[endlinechar_code].get_val();
  eqtb_int_table[endlinechar_code].set_val(-1);
  TokenList L = read_from_file(0,false);
  eqtb_int_table[endlinechar_code].set_val(cc);
  if (has_opt)  new_macro(L,cmd);
  else  back_input(L);
}

// In the case of \@tfor \foo:=... reads the command name \foo 
// and skips the :=
Token Parser::read_for_variable()
{
  TokenList W; 
  W.push_back(Token(other_t_offset,':'));
  W.push_back(hash_table.equals_token);
  get_r_token();
  Token cmd =  cur_tok;
  read_delimited(W); // this should read an empty list
  return cmd;
}

// expand the first element of L, line in \expandafter{\foo...}
// the result is pushed back in L 
void Parser::expand_first(TokenList& L)
{
  back_input(hash_table.CB_token);
  back_input(L);
  expand_when_ok(true);
  back_input(hash_table.OB_token);
  TokenList res = mac_arg(); 
  L.swap(res);
}


// Implementation of some loops
void Parser::xkv_for(subtypes c)
{
  Token comma = hash_table.comma_token;
  Token nil = hash_table.nil_token;
  Token doubleat = hash_table.doubleat_token;
  Token T = cur_tok;
  TokenList res;
  switch(c) {
  case for_code: { // case \@for \A:= list \do code
    Token cmd =read_for_variable();
    TokenList L = read_until(hash_table.do_token);
    TokenList function = mac_arg();
    expand_first(L);
    if(L.empty()) break;
    brace_me(function);
    res.push_back(hash_table.forloop_token);
    res.splice(res.end(),L);
    res.push_back(comma); res.push_back(nil);
    res.push_back(comma); res.push_back(nil);
    res.push_back(doubleat);
    res.push_back(cmd);
    res.splice(res.end(),function);
    break;
  }
  case forloop_code: {
    TokenList entry=read_until(comma);
    TokenList next_entry = read_until(comma);
    TokenList remainder = read_until(doubleat);
    get_r_token();
    Token cmd = cur_tok;
    TokenList function = mac_arg();
    if(token_ns::has_a_single_token(entry,nil)) break;
    new_macro(entry,cmd);
    TokenList aux = function;
    res.splice(res.end(), aux);
    if(token_ns::has_a_single_token(next_entry,nil)) break;
    brace_me(next_entry);
    TokenList aux2 = function;
    brace_me(function);
    res.push_back(hash_table.def_token);
    res.push_back(cmd);
    res.splice(res.end(),next_entry);
    res.splice(res.end(),aux2);
    res.push_back(hash_table.iforloop_token);
    res.splice(res.end(),remainder);
    res.push_back(doubleat);
    res.push_back(cmd);
    res.splice(res.end(),function);
    break;
  }
  case tfor_code: {
    Token cmd = read_for_variable();
    back_input(hash_table.space_token);
    TokenList L = read_until(hash_table.do_token);
    TokenList function = mac_arg();
    if(token_ns::has_a_single_token(L,hash_table.space_token)) break;
    brace_me(function);
    res.push_back(hash_table.tforloop_token);
    res.splice(res.end(),L);
    res.push_back(nil);
    res.push_back(nil);
    res.push_back(doubleat);
    res.push_back(cmd);
    res.splice(res.end(),function);
    break;
  }
  case xkv_breaktfor_code: {
    // should we insert \fi here
    TokenList W;
    W.push_back(nil);W.push_back(comma);
    read_delimited(W);
    res.push_back(hash_table.fi_token);
    break;
  }
  case breaktfor_code:
    read_until(hash_table.doubleat_token);
    ignore_next_arg(); 
    ignore_next_arg();
    res.push_back(hash_table.fi_token);
    break;
  case iforloop_code:
  case tforloop_code:  {
    TokenList entry=c==iforloop_code ? read_until(comma) : mac_arg();; 
    TokenList remainder = read_until(doubleat);
    get_r_token();
    Token cmd = cur_tok;
    TokenList function = mac_arg();
    if(token_ns::has_a_single_token(entry,nil)) break;
    new_macro(entry,cmd);
    TokenList aux = function;
    res.splice(res.end(),aux);
    brace_me(function);
    res.push_back(hash_table.relax_token);
    res.push_back(c==iforloop_code ? hash_table.iforloop_token :
		  hash_table.tforloop_token);
    res.splice(res.end(),remainder);
    res.push_back(doubleat);
    res.push_back(cmd);
    res.splice(res.end(),function);
    break;
  }
  case xkv_for_n_code: 
  case xkv_for_o_code: 
  case xkv_for_en_code: 
  case xkv_for_eo_code: {
    TokenList L=mac_arg();  // the list to operate on
    if(c==xkv_for_o_code || c==xkv_for_eo_code) expand_first(L);
    TokenList cmd =mac_arg(); 
    TokenList function= mac_arg();
    if(L.empty() && (c==xkv_for_n_code || c==xkv_for_o_code)) break;
    brace_me(function);
    res.push_front(hash_table.xkv_for_i_token);
    res.splice(res.end(),cmd);
    res.splice(res.end(),function);
    res.splice(res.end(),L);
    res.push_back(comma);
    res.push_back(nil);
    res.push_back(comma);
    break;
  } 
  case xkv_for_i_code: {
    get_r_token(false); // the command
    Token cmd=cur_tok;
    TokenList function=mac_arg(); // the code
    TokenList entry=read_until(comma);
    if(token_ns::has_a_single_token(entry,nil)) break;
    new_macro(entry,cmd);
    TokenList aux = function;
    brace_me(function);
    res.splice(res.end(),aux); 
    res.push_back(hash_table.xkv_for_i_token);
    res.push_back(cmd);
    res.splice(res.end(),function);
    break;
    }
    //  case xkv_whilist_code:
    //    
  default:;
  }
  if(tracing_commands())
    the_log << lg_start << T << "<- " << res << lg_end;
  back_input(res);
}

// Implementation of \@cons
// Read a token T and a list L, calls the function below
void Parser::T_cons()
{
  get_r_token();
  Token cmd = cur_tok;
  TokenList L= mac_arg();
  if(tracing_commands()) 
    the_log << lg_startbrace << "\\@cons " << cmd << " + " << L << lg_endbrace;
  T_cons(cmd,L);
}

// \@cons\T{L} is \edef\T{\T\@elt L}, with \let\@elt\relax in a group
// Question: latex has \xdef, is a \edef useful ? 
void Parser::T_cons(Token cmd, TokenList& L)
{
  Token E = hash_table.elt_token;
  L.push_front(E);
  L.push_front(cmd);
  push_level(bt_brace);
  eval_let(E,hash_table.relax_token,false);
  Token nfe = name_for_error;
  name_for_error = cmd;
  scan_toks_edef(L);
  pop_level(false,bt_brace);
  new_macro(L,cmd,true);
  name_for_error = nfe;
}

// Implements \@testopt \A B as: if bracket, then \A, else \A[{B}]
void Parser::T_testopt()
{
  TokenList L= mac_arg();
  TokenList R = mac_arg();
  skip_initial_space();
  if(cur_tok.is_valid()) back_input();
  if(!cur_tok.is_open_bracket()) {
    brace_me(R);
    R.push_front(Token(other_t_offset,'['));
    R.push_back(Token(other_t_offset,']'));
    back_input(R);
  }
  back_input(L);
}


// Implements \tralics@addtolist@n{sep}{\cmd}{val}
// and \tralics@addtolist@o{sep}{\cmd}{val}
// In the second case, the first tken in val is expanded; 
// This puts val to the end of \cmd, with sep unless \cmd is empty
void Parser::T_addtomacro(bool exp)
{
  TokenList D = mac_arg(); // The separator 
  get_r_token(true);
  Token T = cur_tok;  // the macro to define
  TokenList L = mac_arg(); // 
  back_input(hash_table.CB_token);
  back_input(L);
  if(exp) expand_when_ok(true);
  TokenList W = get_mac_value(T);
  if(!W.empty()) back_input(D);
  back_input(W);
  L.clear();
  skip_group0(L);
  new_macro(L,T);
}


// Implements \@expandtwoargs\foo\bar\gee
// result is \foo{barval}{geeval}
void Parser::expand_twoargs()
{
  TokenList L = mac_arg();
  TokenList M = mac_arg();
  TokenList N = mac_arg();
  brace_me(M);
  brace_me(N);
  M.splice(M.end(),N);
  scan_toks_edef(M);
  back_input(M);
  back_input(L);
  
}


// ------------------------------------------------------------
// Special commands for xkeyval


void Parser::T_xkeyval(subtypes c)
{
  switch(c) {
  case boot_keyval_code: hash_table.boot_keyval(); return;
  case definekey_code: T_define_key(false); return;
  case xdefinekey_code: T_define_key(true); return;
  case define_cmdkey_code: 
  case define_cmdkeys_code:
    define_cmd_key(c); 
    return;
  case define_boolkey_code: 
  case define_boolkeys_code:
    define_bool_key(c); 
    return;
  case define_choicekey_code: define_choice_key(); return; 
  case define_cc_code: internal_choice_key(); return; 
  case disable_keys_code: disable_keys(); return;
  case key_ifundefined_code: key_ifundefined(); return;
  case save_keys_code: 
  case gsave_keys_code: 
    {
      xkv_save_keys_aux(false,0);
      TokenList A = mac_arg();
      xkv_merge(c==gsave_keys_code,0,A,true);
      return;
    }
  case delsave_keys_code: 
  case gdelsave_keys_code: 
    {
      bool bad = xkv_save_keys_aux(false,0);
      TokenList A = mac_arg();
      if(!bad) xkv_merge(c==gdelsave_keys_code,0,A,false);
      return;
    }
  case unsave_keys_code: 
  case gunsave_keys_code: 
    if(xkv_save_keys_aux(true,0)) return;
    eval_let(cur_tok,hash_table.frozen_undef_token,c==gunsave_keys_code);
    return;
  case preset_keys_code: 
  case gpreset_keys_code: 
    {
      xkv_save_keys_aux(false,1);
      TokenList A = mac_arg();
      TokenList B = mac_arg();
      xkv_merge(c==gpreset_keys_code,1,A,true);
      xkv_merge(c==gpreset_keys_code,2,B,true);
      return;
    }
  case delpreset_keys_code: 
  case gdelpreset_keys_code: 
    {
      bool bad = xkv_save_keys_aux(true,1);
      TokenList A = mac_arg();
      TokenList B = mac_arg();
      if(bad) return;
      xkv_merge(c==gdelpreset_keys_code,1,A,false);
      xkv_merge(c==gdelpreset_keys_code,2,B,false);
      return;
    }
  case unpreset_keys_code: 
  case gunpreset_keys_code: 
    {
      if(xkv_save_keys_aux(true,1)) return;
      eval_let(cur_tok,hash_table.frozen_undef_token,c==gunpreset_keys_code);
      xkv_ns::find_aux(2);
      cur_tok = hash_table.locate(local_buf);
      eval_let(cur_tok,hash_table.frozen_undef_token,c==gunpreset_keys_code);
      return;
    }
  case setrmkeys_code: setkeys(false); return;
  case setkeys_code: setkeys(true); return;
  case declare_optionsX_code: xkv_declare_option(); return;
  case process_optionsX_code: xkv_process_options(); return;
  case execute_optionsX_code: xkv_execute_options(); return;
  default: return;
  } 
}

// Converts the token list X into a string, adding s1 and s2
// May signal an error, used bad instead 
void xkv_ns::to_string(TokenList& x, String s1,String s2,String msg,Buffer&B)
{
  B << bf_reset << s1;
  if(!the_parser.csname_aux(x,false,B)) {
    the_parser.parse_error(msg,x);
    B << bf_reset << s1 << "bad";
  }
  B << s2;
}

string xkv_ns::to_string(TokenList& x,String msg)
{
  Buffer&B = local_buf;
  B.reset();
  if(!the_parser.csname_aux(x,false,B)) {
    the_parser.parse_error(msg,x);
    B << bf_reset << "bad";
  }
  return B.to_string();
}

// Same as above, converts the string into a token 
Token xkv_ns::to_string(TokenList& x, String s1,String s2,String msg)
{
  to_string(x,s1,s2,msg,local_buf);
  return the_parser.hash_table.locate(local_buf);
}

// Case \define@key{foo}{bar}{ETC}
// or   \define@key{foo}{bar}[gee]{ETC}
// Read foo and bar, construct KV@foo@bar, call it \cmd
// In the first case, we produce \def\cmd#1{ETC}
// Otherwise \def\KV@foo@bar@default{\cmd{gee}}, then \def\cmd#1{ETC}

void Parser::T_define_key(bool xkv)
{ 
  Buffer& B = local_buf;
  if(xkv) xkv_fetch_prefix_family();
  else {
    TokenList fam = mac_arg();
    xkv_ns::to_string(fam, "KV@","","Problem in define@key",B);
    B <<'@';
    xkv_header= B.to_string();
  } 
  TokenList key = mac_arg();
  Token T = xkv_ns::to_string(key, xkv_header.c_str(),"","bad key name");
  TokenList opt;
  if(next_optarg(opt)) internal_define_key_default(T,opt);
  internal_define_key(T);
}

// Implements \define@cmdkey, \define@cmdkeys
// We make the assumption that a key does not contain a comma 
void Parser::define_cmd_key(subtypes c)
{ 
  Buffer&B = local_buf;
  xkv_fetch_prefix_family(); // read prefix and family
  TokenList L;
  if(next_optarg_long(L)) {
    xkv_ns::to_string(L,"","","Problem scanning macro prefix",B);
  } else B << bf_reset << "cmd" << xkv_header;
  string mp = B.to_string();
  TokenList keytoks = mac_arg();
  TokenList dft;
  bool has_dft = next_optarg(dft); // \par ok here
  // construct the key or key list
  string Keys = xkv_ns::to_string(keytoks,"problem scanning key");
  Splitter S(Keys);
  for(;;) {
    if(S.at_end()) return;
    string Key = S.get_next();
    if(Key.empty()) continue;
    B << bf_reset << mp << Key;
    Token cmd = hash_table.locate(B);
    B << bf_reset << xkv_header << Key;
    Token T = hash_table.locate(B);
    if(has_dft) {
      TokenList D = dft;
      internal_define_key_default(T,D);
    }
    TokenList L;
    if(c==define_cmdkey_code)  {// case of cmdkey
      TokenList u = mac_arg();
      L.splice(L.end(),u);
    }
    L.push_front(hash_table.CB_token);
    L.push_front(Token(other_t_offset,'1'));
    L.push_front(make_char_token('#',6));
    L.push_front(hash_table.OB_token);
    L.push_front(cmd);
    L.push_front(hash_table.locate("def"));  
    brace_me(L);
    back_input(L);
    Macro* X = new Macro;
    X->set_nbargs(1);
    X->set_type(dt_normal);
    read_mac_body(X->get_body(),false,1);
    X->correct_type();
    mac_define(T,X,false,rd_always,user_cmd);
  }
}

// Implements \define@choicekey
void Parser::define_choice_key()
{
  bool if_star = remove_initial_plus(false);
  bool if_plus = remove_initial_plus(true);
  xkv_fetch_prefix_family(); 
  TokenList keytoks = mac_arg();
  Token T = xkv_ns::to_string(keytoks, xkv_header.c_str(),"","bad key name");
  TokenList storage_bin;
  next_optarg_long(storage_bin); 
  TokenList allowed = mac_arg();
  TokenList opt;
  if(next_optarg(opt)) internal_define_key_default(T,opt);
  TokenList F;
  if(if_plus) {
    TokenList x = mac_arg();
    TokenList y = mac_arg();
    brace_me(x);
    brace_me(y);
    F.splice(F.end(),x); F.splice(F.end(),y);
  } else {
    F = mac_arg();
    brace_me(F);
  }
  TokenList body;
  body.push_back(hash_table.xkv_cc_token);
  if(if_star) body.push_back(Token(other_t_offset,'*'));
  if(if_plus) body.push_back(Token(other_t_offset,'+'));
  storage_bin.push_front(Token(other_t_offset,'['));
  storage_bin.push_back(Token(other_t_offset,']'));
  body.splice(body.end(),storage_bin);
  TokenList argument;
  argument.push_back(make_char_token('#',6));
  argument.push_back(Token(other_t_offset,'1'));
  brace_me(argument);
  body.splice(body.end(),argument);
  brace_me(allowed);
  body.splice(body.end(),allowed);
  body.splice(body.end(),F);
  brace_me(body);
  back_input(body);
  internal_define_key(T);
}

// Implements \XKV@cc
void Parser::internal_choice_key()
{
  bool if_star = remove_initial_plus(false);
  bool if_plus = remove_initial_plus(true);
  TokenList bin;
  next_optarg_long(bin);
  TokenList input = mac_arg();
  TokenList allowed = mac_arg();
  TokenList ok_code = mac_arg();
  TokenList bad_code;
  if(if_plus) bad_code= mac_arg();
  if(if_star) {
    token_ns::lower_case(input);
    token_ns::lower_case(allowed);
  }
  Token relax = hash_table.relax_token;
  Token B1=relax,B2=relax;
  if(!bin.empty()) { B1 = bin.front(); bin.pop_front(); }
  if(!bin.empty()) { B2 = bin.front(); bin.pop_front(); }
  if(B1.not_a_cmd()) B1 = relax;
  if(B2.not_a_cmd()) B2 = relax;
  if(B1 != relax) {
    TokenList u = input;
    new_macro(u,B1);
  } 
  TokenList xinput=input;
  bool found = token_ns::find_in(xinput,allowed,hash_table.comma_token,false);
  if(B2!=relax) {
    local_buf << bf_reset << is_in_skipped;
    TokenList u = local_buf.str_toks(false);
    new_macro(u,B2);
  }
  if(found)
    back_input(ok_code);
  else if(if_plus) // invalid, but catched
    back_input(bad_code);
  else {
    parse_error("XKV: value is not allowed");
    main_ns::log_and_tty << " " << input << lg_end;
  }
}


void Parser::define_bool_key(subtypes c)
{ 
  Buffer&B = local_buf;
  remove_initial_plus(false);
  bool if_plus = remove_initial_plus(true);
  if(c!=define_boolkey_code) if_plus = false;
  xkv_fetch_prefix_family(); // read prefix and family
  TokenList L;
  if(next_optarg_long(L)) {
    xkv_ns::to_string(L,"","","Problem scanning macro prefix",B);
  } else B << bf_reset  << xkv_header;
  string mp = B.to_string();
  TokenList keytoks = mac_arg();
  TokenList dft;
  bool has_dft = next_optarg_long(dft);
  // construct the key or key list
  string Keys = xkv_ns::to_string(keytoks,"Problem scanning key");
  Splitter S(Keys);
  for(;;) {
    if(S.at_end()) break;
    string Key = S.get_next();
    if(Key.empty()) continue;
    B << bf_reset << mp << Key;
    TokenList u = B.str_toks11(false);
    B << bf_reset << "if" << mp << Key;
    back_input(hash_table.locate(B));
    newif();
    B << bf_reset << "true,false";
    TokenList v = B.str_toks11(false);
    B << bf_reset <<xkv_header << Key;
    Token T = hash_table.locate(local_buf);
    if(has_dft) {
      TokenList D  = dft;
      internal_define_key_default(T,D);
    }
    u.push_front(hash_table.csname_token);
    u.push_back(hash_table.xkv_resa_token);
    u.push_back(hash_table.endcsname_token);
    if(c==define_boolkey_code) {
      TokenList add1 = mac_arg();
      u.splice(u.end(),add1);
    }
    brace_me(u);
    TokenList L;
    L.splice(L.end(),u);
    if(if_plus) {
      TokenList add2 = mac_arg();
      brace_me(add2);
      L.splice(L.end(),add2);
    }
    brace_me(v);
    L.splice(L.begin(),v);
    L.push_front(hash_table.CB_token);
    L.push_front(Token(other_t_offset,'1'));
    L.push_front(make_char_token('#',6));
    L.push_front(hash_table.OB_token);
    L.push_front(Token(other_t_offset,']'));
    L.push_front(hash_table.xkv_resa_token);
    L.push_front(Token(other_t_offset,'['));
    if(if_plus) L.push_front(Token(other_t_offset,'+'));
    L.push_front(Token(other_t_offset,'*'));
    L.push_front(hash_table.xkv_cc_token);
    brace_me(L);
    back_input(L);
    Macro* X = new Macro;
    X->set_nbargs(1);
    X->set_type(dt_normal);
    read_mac_body(X->get_body(),false,1);
    X->correct_type();
    mac_define(T,X,false,rd_always,user_cmd);
  }
}

// Implements \key@ifundefined
void Parser::key_ifundefined()
{
  Buffer& B = local_buf;
  xkv_fetch_prefix();
  TokenList fams = mac_arg();
  bool undefined = true;
  TokenList key = mac_arg();
  string Key = xkv_ns::to_string(key,"problem scanning key");
  string Fams = xkv_ns::to_string(fams,"Problem with the families");
  string fam="";
  Splitter S (Fams);
  for(;;) {
    if(S.at_end()) break;
    fam = S.get_next();
    B.reset();
    B << xkv_prefix << fam;
    if(!fam.empty()) B.push_back('@');
    B.push_back(Key);
    if(hash_table.is_defined(B)) { undefined = false; break; }
  }
  new_macro(fam,hash_table.xkv_tfam_token);
  one_of_two(undefined);
}


// Implements \disable@keys
void Parser::disable_keys()
{
  Buffer&B = local_buf;
  xkv_fetch_prefix_family(); // read prefix and family
  TokenList keys = mac_arg();
  string Keys = xkv_ns::to_string(keys,"problem scanning keys");
  Splitter S (Keys);
  for(;;) {
    if(S.at_end()) break;
    string Key = S.get_next();
    B << bf_reset << xkv_header << Key;
    if(hash_table.is_defined(B)) {
      Token T = hash_table.last_tok;
      B.push_back("@default");
      if(hash_table.is_defined(B)) {
	TokenList L;
	brace_me(L);
	L.push_front(T);
	new_macro(L,hash_table.last_tok);
      }
      B << bf_reset << "Key `" << Key << "' has been disabled";
      TokenList L = B.str_toks(false);
      brace_me(L);
      L.push_front(hash_table.xkv_warn_token);
      Macro* X = new Macro(L);
      X->set_nbargs(1);
      X->set_type(dt_normal);
      mac_define(T,X,false,rd_always,user_cmd);
    } else parse_error("Undefined key cannot be disabled: ", Key,"");
  }
}



// Find saved or preset keys, depending on c2. If not found:
// signals a an error if c is true (creates otherwise), return true.
// Creates cur_tok if needed
bool Parser::xkv_save_keys_aux(bool c, int c2)
{
  xkv_fetch_prefix_family();
  xkv_ns::find_aux(c2);
  Buffer& B = local_buf;
  bool ret = !hash_table.is_defined(B);
  if(c && ret) {
    B << bf_reset << "No " << (c2 ? "presets" : " save keys") 
      << " defined for `" << xkv_header << "'";
    parse_error(B.to_string());
    return true;
  }
  cur_tok = hash_table.last_tok;
  return ret;
}


// The following function takes in L one item and puts the key in x.
// If type is 0, we are looking for \global, and there is no equals
// Otherwise we look for \savevalue or \gsavevalue, skip equals.
// We set some booleans
string xkv_ns::find_key_of(const TokenList&L,int type)
{
  Hashtab&H =the_parser.hash_table;
  Token equals = H.equals_token;
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  TokenList x; // will hold the tokens
  while(C != E) {
    if(*C==equals) break;
    x.push_back(*C);
    ++C;
  }
  xkv_is_save = false; xkv_is_global = false;
  if(x.empty()) return "";
  Token first=x.front();
  if(type==0) {
    if(first==H.global_token) {
      x.pop_front();
      xkv_is_global = true;
    }
  } else {
    if(first==H.savevalue_token) {
      x.pop_front();
      xkv_is_save = true;
    }
    else if(first==H.gsavevalue_token) {
      x.pop_front();
      xkv_is_save = true;
      xkv_is_global = true;
    }
  }
  token_ns::remove_ext_braces(x);
  return xkv_ns::to_string(x,"Invalid key name");
}


// Command savekeys and presetkeys use a common whose name is constructed
// here 
void xkv_ns::find_aux(int c)
{
  local_buf << bf_reset << "XKV@" << xkv_header;
  local_buf << (c==0 ? "save" : (c==1 ? "preseth" :  "presett")); 
}

// This merges L into W; both lists have the form \global{key}=value
// or, depenfing on the type, are lists of keys 
void xkv_ns::merge(TokenList&W, TokenList& L, int type)
{
  Token comma = the_parser.hash_table.comma_token;
  TokenList key;
  TokenList tmp;
  if(W.empty()) { 
    while(!L.empty()) {
      token_ns::split_at(comma,L,key);
      token_ns::remove_first_last_space(key);
      if(!W.empty()) W.push_back(comma);
      W.splice(W.end(),key);
    }
    return;
  }
  while(!L.empty()) {
    token_ns::split_at(comma,L,key);
    token_ns::remove_first_last_space(key);
    string key_name = find_key_of(key,type);
    bool found = false; // Was key in W ?
    TokenList z;
    W.swap(tmp);
    while(!tmp.empty()) {
      token_ns::split_at(comma,tmp,z);
      string zname = find_key_of(z,type);
      if(!W.empty()) W.push_back(comma);
      if(key_name==zname) {
	found = true; // replace in W old value z by new value 
	W.splice(W.end(),key);
      } else
	W.splice(W.end(),z);
    }
    if(!found) {
      if(!W.empty()) W.push_back(comma);
      W.splice(W.end(),key); // Insert key at the end
    }
  }
}

// This deletes L from W. Here L is a simple list of keys
void xkv_ns::remove(TokenList&W, TokenList& L, int type)
{
  Buffer&B = buf_for_del;
  Buffer&aux = local_buf;
  to_string(L,",",",","Invalid key name list",B);
  Token comma = the_parser.hash_table.comma_token;
  TokenList tmp;
  TokenList key;
  W.swap(tmp);
  while(!tmp.empty()) {
    token_ns::split_at(comma,tmp,key);
    string key_name = xkv_ns::find_key_of(key,type);
    aux << bf_reset << "," << key_name << ",";
    if(!B.contains(aux.c_str())) {
      if(!W.empty()) W.push_back(comma);
      W.splice(W.end(), key);
    }
  }
}


// This finds mykey in the list whose name is in the buffer
bool Parser::find_a_save_key(const string& mykey)
{
  Buffer&B = local_buf; 
  TokenList W = get_mac_value(hash_table.locate(B));
  TokenList key;
  while(!W.empty()) {
    token_ns::split_at(hash_table.comma_token,W,key);
    string key_name = xkv_ns::find_key_of(key,0);
    if(key_name==mykey) return true;
  }
  return false;
}


// Merges or deletes depending on mg globally if gbl is true
// the keys in L from the variable depending on type
// Implements preset or save depending on type
void Parser::xkv_merge(bool gbl,int type,TokenList& L,bool mg)
{
  token_ns::sanitize_one(L, '=');
  token_ns::sanitize_one(L, ',');
  xkv_ns::find_aux(type);
  Token T = hash_table.locate(local_buf);
  TokenList W = get_mac_value(T);
  if(mg) 
    xkv_ns::merge(W,L,type); 
  else xkv_ns::remove(W,L,type);
  new_macro(W,T,gbl);
}

// Assume local_buf contains the name of T without the extension
void Parser::internal_define_key_default(Token T,TokenList&L)
{
  brace_me(L);
  L.push_front(T);
  local_buf.push_back("@default");
  cur_tok = hash_table.locate(local_buf);
  new_macro(L,cur_tok);
}

// This is like \def\T#1, optimised
void Parser::internal_define_key(Token T)
{
  Macro* X = new Macro;
  X->set_nbargs(1);
  X->set_type(dt_normal);
  read_mac_body(X->get_body(),false,1);
  X->correct_type();
  mac_define(T,X,false,rd_always,user_cmd);
}

// Skips initial + or *, catcode irrelevant
bool Parser::remove_initial_plus (bool plus)
{
  skip_initial_space();
  if(cur_tok.is_a_char()) {
    if(cur_tok.char_val()== (plus ? '+' : '*')) return true;
  }
  if(!cur_tok.is_invalid()) back_input();
  return false;
}


// Reads optional prefix, and family, and handles them
void Parser::xkv_fetch_prefix()
{
  TokenList L;
  if(!next_optarg(L)) {
    xkv_prefix = "KV@";
    return;
  }
  Buffer&B = local_buf;
  B.reset();
  token_ns::remove_first_last_space(L);
  bool t = csname_aux(L,false,B);
  if(!t) {
    parse_error("Bad command ",cur_tok," in XKV prefix (more errors may follow)","bad kv prefix");
    B.reset();
  }
  if(B.is_equal("XKV")) {
    parse_error("xkeyval: `XKV' prefix is not allowed");
    B.reset();
  }
  if(!B.empty()) B.push_back('@');
  xkv_prefix = B.to_string();
}

// Creates the XKV header

void Parser::xkv_makehd(TokenList& L)
{
  token_ns::remove_first_last_space(L);
  Buffer&B = local_buf;
  B.reset();
  B << xkv_prefix;
  int k = B.size();
  if(!csname_aux(L,false,B)) {
    parse_error("Bad command ",cur_tok," in XKV family (more errors may follow)","bad kv family");
   B.set_last(k);
  }
  if(B.size() != k) B.push_back('@');
  xkv_header = B.c_str();
}

void xkv_ns::makehd(const string& fam)
{
  Buffer&B = local_buf;
  B << bf_reset << xkv_prefix << fam;
  if(!fam.empty())  B.push_back('@');
  xkv_header = B.c_str();
}

void Parser::xkv_fetch_prefix_family()
{
  xkv_fetch_prefix();
  TokenList M = mac_arg();
  xkv_makehd(M);
}

void token_ns::lower_case(TokenList& L)
{  
  token_iterator P = L.begin();
  token_iterator E = L.end();
  int offset = lc_code_offset;
  while (P != E) {
    Token a = *P;
    if(a.get_val()<cs_token_flag+single_base) {
      int b = a.chr_val();
      int cx = the_parser.eqtb_int_table[b+offset].get_val();
      if(cx)
	*P = Token(a.get_val()-b+cx);
    }
    ++P;
  }
}

void Parser::xkv_declare_option()
{
  // \let\@fileswith@pti@ns\@badrequireerror
  bool star = remove_initial_star();
  if(star) {
    T_declare_option_star();
    return;
  }
  xkv_fetch_prefix();
  TokenList FL = XKV_parse_filename();
  xkv_makehd(FL);
  TokenList key = mac_arg();
  TokenList xkey = key;
  string Key = xkv_ns::to_string(xkey,"Invalid option");
  classes_ns::register_key(Key);
  Token T = xkv_ns::to_string(key, xkv_header.c_str(),"","bad option name");
  TokenList opt;
  next_optarg(opt);
  internal_define_key_default(T,opt);
  internal_define_key(T);
}


// Implements ExecuteOptionsX
void Parser::xkv_execute_options()
{
  XkvSetkeys data(this);
  xkv_fetch_prefix();
  data.special_fams();
  data.fetch_keys(true);
  data.dump_keys();
  data.set_aux();
  data.finish();
}  


void XkvSetkeys::dump_keys()
{
  the_log << lg_startbrace << "Options to execute->" 
	  << keyvals << lg_endbrace;    
}

// Implements ProcessOptionsX
void Parser::xkv_process_options()
{
  XkvSetkeys data(this);
  //  data.no_err = remove_initial_plus(false);
  bool s = remove_initial_star();  // we should do something with this
  xkv_fetch_prefix();
  data.set_inpox();
  data.special_fams();
  TokenList uo;
  TokenList L = classes_ns::cur_options(s,uo,false);
  brace_me(L);
  back_input(L);
  data.fetch_keys(true);
  data.set_aux();
  data.finish();
}


// Removes A from B, result will be in C
void Parser::T_remove_element()
{
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  get_r_token(true);
  remove_element(A,B,cur_tok);
}

bool token_ns::find_in(TokenList&A, TokenList&B, Token t, bool sw)
{
  A.push_back(t);
  B.push_back(t);
  A.push_front(t);
  B.push_front(t);
  return token_ns::is_in(A,B,sw);
}

// We add commas around A and B, and must remove them later
void Parser::remove_element(TokenList& A, TokenList& B, Token C)
{
  Token t = hash_table.comma_token;
  token_ns::find_in(A,B,t,true);
  B.pop_front();
  if(!B.empty()) B.pop_back();
  new_macro(B,C);
}


// Redefinition of T_pass_options if xkvltxp is loaded
// The idea is to not expand the option.
// Pushes tokens into \opt@foo.cls
void Parser::xkv_pass_options(bool c) // true if a class
{
  TokenList opt = mac_arg();
  string name = rT_next_arg(); 
  name = "opt@" + name;
  name +=  (c ? ".cls" : ".sty");
  Token t = hash_table.CurrentOption_token;
  if(token_ns::has_a_single_token(opt,t))
    opt = get_mac_value(t);
  Token tname = hash_table.locate(name);
  TokenList res = get_mac_value(tname);
  if(!res.empty()) res.push_back(Token(other_t_offset,','));
  res.splice(res.end(),opt);
  new_macro(res,tname,true);
}


// If true sanitize on comma and equal, otherwise only comma
// Currently unused
void Parser::xkv_checksanitize(bool c)
{
  TokenList B = mac_arg();
  get_r_token(true);
  Token A = cur_tok;
  xkv_checksanitize(A,B,c);
}

void Parser::xkv_checksanitize(Token A, TokenList&B,bool c)
{
  if(c) token_ns::sanitize_one(B, '=');
  token_ns::sanitize_one(B, ',');
  new_macro(B,A);
}

void Parser::selective_sanitize()
{
  Token T = cur_tok;
  int n = 10000;
  TokenList nb;
  next_optarg_long(nb);
  if(!nb.empty())
    n = scan_int(nb,T);
  TokenList chars = mac_arg();
  token_ns::sanitize_one(chars);
  get_r_token(true);
  Token res = cur_tok;
  if(chars.empty())  return;
  TokenList L = get_mac_value(res);
  token_ns::sanitize_one(L,chars,n);
  new_macro(L,res);
}

// --------------------------------------------------
// Implementation of \setkeys
// We start with some auxiliary functions

// Ctor 
XkvSetkeys::XkvSetkeys(Parser*P) : P(P) 
{
  comma_token = P->hash_table.comma_token;
  equals_token = P->hash_table.equals_token;
  na_token = P->hash_table.xkv_na_token;
  fams_token = P->hash_table.xkv_fams_token;
  rm_token = P->hash_table.xkv_rm_token;
  savevalue_token = P->hash_table.savevalue_token;
  usevalue_token = P->hash_table.usevalue_token;
  in_pox = false;
  no_err = false;
  set_all = false;
}



// This reads and manages the list of families
void XkvSetkeys::fetch_fams()
{
  fams = P->mac_arg();
  extract_keys(fams,Fams);
}

// Special case of ExecuteOptions
void XkvSetkeys::special_fams()
{
  fams = P->XKV_parse_filename();
  extract_keys(fams,Fams);
}

// Reads the optional list of keys that should not be set
void XkvSetkeys::fetch_na()
{
  P->next_optarg_long(na); 
  extract_keys(na,Na);
}

void XkvSetkeys::fetch_keys(bool c)
{
  if(!c) keyvals = P->get_mac_value(rm_token); // case of \setrmkeys
  else keyvals = P->mac_arg();  
  if(the_parser.tracing_commands())
    the_log << lg_start << "setkeys -> "  << keyvals << lg_end;
  extract_keys(keyvals,Keys);
}

// Evaluate now everything
void XkvSetkeys::finish()
{
  P->new_macro(xkv_prefix,P->hash_table.xkv_prefix_token);
  P->new_macro(fams,fams_token);
  P->new_macro(na,na_token);
  if(!delayed.empty()) delayed.pop_back(); // remove trailing comma 
  if(the_parser.tracing_commands())
    the_log << lg_start << "setkeys <- "  << action << lg_end;
  P->new_macro(delayed,rm_token);
  P->back_input(action);
} 


// Extract the keys from a list, result in the vector R
// Will store a normalised version of the list in L 
void XkvSetkeys::extract_keys(TokenList& L, vector<string>& R)
{
  if(L.empty()) return;
  Token T = comma_token;
  TokenList res;
  TokenList z;
  int bl = 0;
  L.push_back(T); 
  for(;;) {
    if(L.empty()) break;
    Token x = L.front();
    L.pop_front();
    token_ns::check_brace(x,bl);
    if(bl==0 && x.is_a_char() && x.char_val()==',') {
      token_ns::remove_first_last_space(z);
      if(z.empty()) continue;
      string s = xkv_ns::find_key_of(z,1);
      R.push_back(s);
      res.splice(res.end(),z);
      res.push_back(T);
    }
    else z.push_back(x);
  }
  if(!res.empty()) res.pop_back(); // remove final comma
  L.swap(res);
}


// May set \XKV@prefix \XKV@fams \XKV@tfam \XKV@header \XKV@tkey \XKV@na
void Parser::setkeys(bool c)
{
  XkvSetkeys data(this);
  data.run(c);
}

bool xkv_ns::is_Gin(const TokenList& x)
{
  const_token_iterator C = x.begin();
  const_token_iterator E = x.end();
  if(C==E) return false;
  if(*C!=Token(letter_t_offset,'G')) return false;
  ++C;
  if(C==E) return false;
  if(*C!=Token(letter_t_offset,'i')) return false;
  ++C;
  if(C==E) return false;
  if(*C!=Token(letter_t_offset,'n')) return false;
  ++C;
  if(C==E) return true;
  return false;
}

void XkvSetkeys::run(bool c)
{
  no_err = P->remove_initial_plus(false);
  set_all = P->remove_initial_plus(true);
  P->xkv_fetch_prefix();
  fams = P->mac_arg();
  if(xkv_ns::is_Gin(fams)) {
    TokenList L = the_parser.mac_arg();
    L.push_back(comma_token);
    the_parser.new_macro(L,the_parser.hash_table.locate("Gin@keys"));
    return;
  }  
  extract_keys(fams,Fams);
  fetch_na();
  fetch_keys(c);
  check_preset("preseth");
  set_aux(keyvals,-1);
  check_preset("presett");
  finish();
}


void XkvSetkeys::check_preset(String s)
{
  Buffer& B = local_buf;
  int N = Fams.size();
  for(int i=0;i<N; i++) {
    xkv_ns::makehd(Fams[i]);
    B << bf_reset << "XKV@" << xkv_header << s;
    if(P->hash_table.is_defined(B)) {
      Token T = P->hash_table.locate(B);  
      TokenList W = P->get_mac_value(T);
      set_aux(W,i);
    }
  }
}
  
// If i=-1 this is the normal case, else
// we are in preset and i is the family index
void XkvSetkeys::set_aux(TokenList& W,int idx)
{
  TokenList val;
  XkvToken cur;
  for(;;) {
    if(W.empty()) return;
    token_ns::split_at(comma_token,W,val);
    token_ns::remove_initial_spaces(val);
    if(val.empty()) continue;
    cur.set_initial(val);
    cur.extract();
    if(cur.key_empty()) {
      if(!cur.val_empty()) P->parse_error("No key for a value");
      continue;
    }
    if(cur.ignore_this(Na)) continue; 
    if(idx>=0 && cur.ignore_this(Keys)) continue; 
    bool found = false;
    int N = Fams.size();
    // if idx>=0, execute the loop once
    int i = 0;
    if(idx>=0) { i = idx; N = idx+1; }
    for(; i<N;i++) {
      if(!cur.is_defined(Fams[i])) continue;
      Token T = P->hash_table.last_tok;
      found = true;
      run_key(T,cur,Fams[i]);
      if(!set_all && found) break;
    }
    if(!found) check_action(cur);
  }
}


// Splits key=val into pieces
void XkvToken::extract()
{
  TokenList key;
  value = initial;
  has_val = token_ns::split_at(Token(other_t_offset,'='),value,key);
  token_ns::remove_first_last_space(key);
  token_ns::remove_first_last_space(value);
  token_ns::remove_ext_braces(value);
  token_ns::remove_ext_braces(value);
  keyname = xkv_ns::find_key_of(key,1);
  has_save = xkv_is_save;
  is_global = xkv_is_global;
  token_ns::remove_first_last_space(value);
}

// True if the key is in the ignore list
bool XkvToken::ignore_this(vector<string>&igna)
{
  int n = igna.size();
  for(int i=0;i< n; i++) if(keyname==igna[i]) return true;
  return false;
}

// Constructs the header, in xkv_header
// Constructs the 3 macros in action
void XkvToken::prepare(const string& fam)
{
  Hashtab& H = the_parser.hash_table;
  // We start constructing the three macros
  Buffer& B = local_buf;
  action.push_back(H.def_token);
  action.push_back(H.xkv_tkey_token);
  B<< bf_reset << keyname;
  TokenList L = B.str_toks11(false);
  the_parser.brace_me(L);
  action.splice(action.end(),L);
  action.push_back(H.def_token);
  action.push_back(H.xkv_tfam_token);
  B<< bf_reset << fam;
  L = B.str_toks11(false);
  the_parser.brace_me(L);
  action.splice(action.end(),L);
  action.push_back(H.def_token);
  action.push_back(H.xkv_header_token);
  xkv_ns::makehd(fam); 
  L = B.str_toks11(false);
  the_parser.brace_me(L);
  action.splice(action.end(),L);
}

// Returns true if the key is defined
bool XkvToken::is_defined(const string& fam)
{
  xkv_ns::makehd(fam);
  local_buf << keyname;
  return the_parser.hash_table.is_defined(local_buf);
}

// Returns true if must be saved; may set xkv_is_global
bool XkvToken::check_save()
{
  if(has_save) { xkv_is_global = is_global; return true; }
  xkv_ns::find_aux(0); 
  if(!the_parser.hash_table.is_defined(local_buf)) return false;
  return the_parser.find_a_save_key(keyname);
}


// This is called if the value must be saved
void XkvSetkeys::save_key(const string& Key, TokenList& L)
{
  find_pointer(Key);
  Token T = P->hash_table.locate(local_buf);
  P->new_macro(L,T,xkv_is_global);
}


// This is called when the key is defined
// What happens if you say \savevalue{keya} = \usevalue{keyb} ? 
void XkvSetkeys::run_key(Token mac,XkvToken&cur, const string &fam)
{
  cur.prepare(fam);
  more_action(cur.get_action());
  bool s = cur.check_save();
  const string& Key = cur.get_name();
  TokenList L=cur.get_code();
  if(s) save_key(Key,L);
  if(cur.val_empty()) run_default(Key,mac,s);
  else {
    TokenList L=cur.get_code();
    replace_pointers(L);
    action.push_back(mac);
    P->brace_me(L);
    more_action(L);
  }
}

// Interprets \usevalue{foo} in the list L
void XkvSetkeys::replace_pointers(TokenList& L)
{
  Buffer& B = local_buf;
  TokenList res;
  int n =1000;
  for(;;) {
    if(L.empty()) break;
    Token t = L.front();
    if(t.is_a_left_brace()) {
      token_ns::fast_get_block(L,res);
      continue;
    }
    L.pop_front();
    if(t!=usevalue_token) {
      res.push_back(t);
      continue;
    }
    if(!L.empty()) t = L.front();
    if(!t.is_a_left_brace()) {
      P->parse_error("Invalid \\usevalue token","invalid usevalue");
      continue;
    }
    --n;
    if(n<0) {
      P->parse_error("Replace pointer aborted, (infinite loop?)","key loop");
      res.clear();
      break;
    }
    TokenList key = token_ns::fast_get_block(L);
    token_ns::remove_ext_braces(key);
    string Key = xkv_ns::to_string(key, "Argument of \\savevalue");
    find_pointer(Key);
    if(P->hash_table.is_defined(B)) {
      TokenList w = P->get_mac_value(P->hash_table.last_tok);
      L.splice(L.begin(),w);
    } else {
      B << bf_reset << "No value recorded for key `" << Key << "'; ignored";
      P->parse_error(B.c_str(),"no val recorded");
    }
  }
  L.swap(res);
}

// 
void XkvSetkeys::run_default(const string &Key,Token mac,bool s)
{
  Buffer&B = local_buf; 
  B << bf_reset << xkv_header << Key << "@default";
  if(!P->hash_table.is_defined(B)) {
    B << bf_reset << "No value specified for key `" << Key << "'";
    P->parse_error(B.c_str());
    return;
  }
  Token T = P->hash_table.locate(B);
  TokenList L = P->get_mac_value(T);
  if(L.empty() || L.front() != mac) {
    // no hack needed 
    action.push_back(T);      
    return;
  }
  L.pop_front();
  // We apply mac to L via setkeys and not the macro
  TokenList args;
  if(!L.empty()) {
    if(L.front().is_a_left_brace()) { // get the block without the braces
      token_ns::fast_get_block(L,args);
      args.pop_front();
      args.pop_back();
    } else { // otherwise get just a token
      args.push_back(L.front());
      L.pop_front();
    }
  }
  // We have now in args the real argument. 
  if(s) save_key(Key,args);
  replace_pointers(args);
  action.push_back(mac);
  P->brace_me(args);
  more_action(args);
}


// Puts in the buffer the name of the macro that holds the saved value
void XkvSetkeys::find_pointer(const string&Key)
{
  local_buf << bf_reset << "XKV@" << xkv_header << Key << "@value";
}


//
// This considers the case when \ProcessOptionsX has seen cur_opt
void XkvSetkeys::check_action(XkvToken& cur)
{
  TokenList cur_opt = cur.get_all();
  if(in_pox) {
    classes_ns::unknown_optionX(cur_opt,action);
    return;
  }
  // This is the normal case
  if(no_err) new_unknown(cur_opt);
  else
    P->parse_error("Undefined key: ",cur.get_name(),"undefined key");
}
void Parser::formatdate()
{
  flush_buffer();
  string s = sT_next_arg();
  FormatDate FP;
  FP.interpret(s);
  Xmlp X= new Xml(Istring("date"), 0);
  the_stack.add_last(X);
  AttList&AL = X->get_id().get_att();
  Buffer&B = local_buf;
  B << bf_reset << FP.get_year();
  AL.push_back(Istring("year"),Istring(B));
  int k = FP.get_month();
  if(k<0) k = -k;
  B << bf_reset << k;
  AL.push_back(Istring("month"),Istring(B));
  B << bf_reset << FP.get_day();
  AL.push_back(Istring("day"),Istring(B));
}


// True if ok, parse_error otherwise
bool FormatDate::sort()
{
  if(field1>field2) swap(field1,field2);
  if(field2>field3) { // else ok
    swap(field3,field2);
    if(field1>field2) swap(field1,field2);
  }
  if(field2>31) {
    the_parser.parse_error("Date has two fields with value>31");
    return false;
  }
  if(field2<0) {
      the_parser.parse_error("Date has two month fields");
    return false;
  }
  return true;
}

// True if ok, parse_error otherwise
// assumes current character is a digit.
bool FormatDate::scan_a_field(Buffer&B,int& res)
{
  res = 0;
  for(;;) {
    if(B.at_eol()) return true;
    uchar c = B.uhead();
    if(!is_digit(c)) { return true; }
    B.advance();
    res = 10*res + (c-'0');
    if(res>9999) {
      the_parser.parse_error("Too many digits in date field");
      return false;
    }
  }
}

bool Buffer::is_here_case(String s)
{
  int n = strlen(s);
  for(int i=0;i<n;i++) {
    char c = buf[ptr+i];
    if(is_upper_case(c)) c += 'a'-'A';
    if(c != s[i]) return false;
  }
  char c = buf[ptr+n];
  if(is_letter(c)) return false;
  ptr += n;
  return true;  
}


// True if ok. Assume cur char is non-space
bool FormatDate::scan_a_month(Buffer&B,int& res)
{
  if(B.is_here_case("janvier")) { res = -1; return true; }
  if(B.is_here_case("januar")) { res = -1; return true; }
  if(B.is_here_case("jan")) { res = -1; return true; }
  if(B.is_here_case("fevrier")) { res = -2; return true; } 
  if(B.is_here_case("f\303\251vrier")) { res = -2; return true; }
  if(B.is_here_case("februar")) { res = -2; return true; }
  if(B.is_here_case("feb")) { res = -2; return true; }
  if(B.is_here_case("fev")) { res = -2; return true; }
  if(B.is_here_case("mars")) { res = -3; return true; }
  if(B.is_here_case("march")) { res = -3; return true; }
  if(B.is_here_case("mar")) { res = -3; return true; }
  if(B.is_here_case("avril")) { res = -4; return true; }
  if(B.is_here_case("april")) { res = -4; return true; }
  if(B.is_here_case("apr")) { res = -4; return true; }
  if(B.is_here_case("avr")) { res = -4; return true; }
  if(B.is_here_case("mai")) { res = -5; return true; }
  if(B.is_here_case("may")) { res = -5; return true; }
  if(B.is_here_case("juin")) { res = -6; return true; }
  if(B.is_here_case("june")) { res = -6; return true; }
  if(B.is_here_case("jun")) { res = -6; return true; }
  if(B.is_here_case("juillet")) { res = -7; return true; }
  if(B.is_here_case("july")) { res = -7; return true; }
  if(B.is_here_case("jul")) { res = -7; return true; }
  if(B.is_here_case("aout")) { res = -8; return true; }
  if(B.is_here_case("ao\303\273t")) { res = -8; return true; }
  if(B.is_here_case("august")) { res = -8; return true; }
  if(B.is_here_case("aug")) { res = -8; return true; }
  if(B.is_here_case("septembre")) { res = -9; return true; }
  if(B.is_here_case("september")) { res = -9; return true; }
  if(B.is_here_case("sep")) { res = -9; return true; }
  if(B.is_here_case("octobre")) { res = -10; return true; }
  if(B.is_here_case("october")) { res = -10; return true; }
  if(B.is_here_case("oct")) { res = -10; return true; }
  if(B.is_here_case("novembre")) { res = -11; return true; }
  if(B.is_here_case("novembre")) { res = -11; return true; }
  if(B.is_here_case("nov")) { res = -11; return true; }
  if(B.is_here_case("decembre")) { res = -12; return true; }
  if(B.is_here_case("december")) { res = -12; return true; }
  if(B.is_here_case("dec")) { res = -12; return true; }
  if(B.is_here_case("d\303\251cembre")) { res = -12; return true; }
  return false;
}

// true if OK.
bool FormatDate::scan_next(Buffer&B,int& res)
{
  B.skip_sp_tab_nl();
  if(B.at_eol()) {
    the_parser.parse_error("Missing fields in date");
    return false;
  }
  uchar c = B.head();
  if(c=='-' || c== '/' || c==',' || c==';') {
    B.advance();
    B.skip_sp_tab_nl();
    if(B.at_eol()) {
      the_parser.parse_error("Missing fields in date");
      return false;
    }
    c = B.head();
  } 
  if(is_digit(c)) return scan_a_field(B,res);
  if(!scan_a_month(B,res)) {
    the_parser.parse_error("Expected digits or a month in letters");
    return false;
  }
  return true;
}

int FormatDate::next_format_char(Buffer&B)
{
  if(B.at_eol()) return 0;
  uchar c = B.next_char();
  if(c=='j' || c=='J' || c=='D' || c=='d') return 2;
  if(c=='M' || c=='m') return 1;
  if(c=='Y' || c=='y' || c=='A' || c=='a') return 3;
  return 0;
}

// this is called if a random character is after the date
bool FormatDate::parse_format(Buffer&B)
{
  int c1=0,c2=0,c3=0;
  bool ok = false;
  c1 = next_format_char(B);
  if(c1) c2 = next_format_char(B);
  if(c2) c3 = next_format_char(B);
  if(c3) {
    B.skip_sp_tab_nl();
    if(B.at_eol()) ok = true;
  }
  if(c1==c2 || c1==c3 || c2==c3)  ok = false;
  if(!ok) {
    the_parser.parse_error("Illegal date format");
    return false;
  }
  if(c1>c2) {swap (c1,c2); swap(field1,field2); }
  if(c2>c3) { // else ok
    swap(c2,c3);
    swap(field2,field3);
    if(c1>c2) { swap(c1,c2);swap(field1,field2); }
  }  
  // now c1=m c2=d c3=y
  // hence field1=month, field2 = day field3 =year
  if(field1>12) {
    the_parser.parse_error("Bad month field"); return false;
  }
  if(field2>31 || field2<0) {
    the_parser.parse_error("Bad day field"); return false;
  }
  if(field3<0) {
    the_parser.parse_error("Bad year field"); return false;
  }
  return true;
}

bool FormatDate::parse(Buffer&B)
{
  if(!scan_next(B,field1)) return false;
  if(!scan_next(B,field2)) return false;
  if(!scan_next(B,field3)) return false;
  B.skip_sp_tab_nl();
  if(!B.at_eol()) return parse_format(B);
  sort();
  if(field3>31) {
    if(field1<0) return true;
    if(field1==field2) return true;
    if(field2>12) return true;
  }
  the_parser.parse_error("Unable to distinguish between year day and month");
  return false;
}

// fills year month day
void FormatDate::interpret(const string& s)
{
  Buffer&B= local_buf;
  B.reset(); B.reset_ptr(); B<< s;
  if(!parse(B)) { field1=1;  field2=1; field3=2008; }
}

// \@dblarg\foo provides as optional argument of \foo the required one
void Parser::dbl_arg()
{
  TokenList L = mac_arg();
  skip_initial_space();
  if(cur_tok.is_valid()) back_input();
  if(!cur_tok.is_open_bracket()) {
    TokenList A = mac_arg();
    brace_me(A);
    TokenList B = A;
    B.push_front(Token(other_t_offset,'['));
    B.push_back(Token(other_t_offset,']'));
    B.splice(B.end(),A);
    back_input(B);
  }
  back_input(L);
}

// \numberwithin[\c]{foo}{bar}
// evaluate: \@cons\cl@bar{{foo}}; i.e. T_cons(\cl@bar, {foo}).
// and \def\thefoo{\thebar.\c{foo}}
void Parser::numberwithin()
{
  Token T = cur_tok;
  TokenList L;
  next_optarg(L);
  if(L.empty()) L.push_back(hash_table.arabic_token);
  cur_tok = T;
  TokenList foo_list = mac_arg();
  TokenList A = foo_list;
  TokenList bar_list = mac_arg();
  Buffer&b = local_buf;
  b.reset();
  if(!csname_aux("c@","",foo_list,false,b)) {  bad_counter0(); return; }
  if(!counter_check(b,false,T)) return;
  string fooname = b.c_str(2);
  b.reset();
  if(!csname_aux("c@","",bar_list,false,b)) { bad_counter0(); return; }
  if(!counter_check(b,false,T)) return;
  string barname = b.c_str(2);
  b << bf_reset << "cl@" << barname;  // B is now thefoo
  Token clbar_token = hash_table.locate(b);
  brace_me(A);
  TokenList B = A;
  T_cons(clbar_token,B);
  b << bf_reset << "the" << barname;  // B is now thefoo
  Token thebar = hash_table.locate(b);
  b << bf_reset << "the" << fooname;  // B is now thebar
  Token thefoo = hash_table.locate(b);
  L.splice(L.end(),A);
  L.push_front(Token(other_t_offset,'.'));
  L.push_front(thebar);
  new_macro(L,thefoo,true);
}

void Parser::refstepcounter(const string&s,bool star )
{
  Buffer&b = local_buf;
  b.reset();
  b.push_back(s);
  TokenList L = b.str_toks11(true);
  refstepcounter(L,star);
  T_translate(L);
}

void Parser::refstepcounter()
{
  bool star = remove_initial_star();  
  TokenList L = mac_arg();
  refstepcounter(L,star);
  back_input(L);
}

void Parser::refstepcounter(TokenList&L,bool star)
{
  brace_me(L);
  TokenList L1 = L;
  L.push_front(hash_table.stepcounter_token);
  if(star) L1.push_front(Token(other_t_offset,'*'));
  L1.push_front(hash_table.makelabel_token);
  L.splice(L.end(),L1);
  if(tracing_macros()) 
    the_log << lg_start <<"\\refstepcounter-> " << L << lg_end;
}

// \edef\@currentlabel{\csname\p@#1\endcsname\csnane the#1\endcsname}
void Parser::makelabel()
{
  Buffer&b = local_buf;
  b.reset();
  bool star = remove_initial_star();
  TokenList L = mac_arg();
  if(!csname_aux("","",L,false,b)) { bad_counter0(); return; }
  string s =b.to_string();
  TokenList res;
  b << bf_reset << "the" << s; 
  res.push_back(hash_table.locate(b));
  b << bf_reset << "p@" << s; 
  if(hash_table.is_defined(b))
    res.push_front(hash_table.last_tok);
  b.reset();
  if(!csname_aux("","",res,false,b)) { 
    parse_error("Illegal tokens in \\makelabel");
    return; 
  }
  if(tracing_commands()) 
    the_log << lg_start <<"\\@currlabel<- " << b << lg_end;
  string_define(0,b.to_string(),false);
  if(star) the_stack.add_new_anchor();
}

void Parser::ifdefinable() 
{
  bool is_ok = true;
  TokenList A = mac_arg();
  TokenList B = mac_arg();
  Token C = token_ns::get_unique(A);
  if(C.is_null()) { parse_error("Wrong syntax in \\@ifdefinable"); return; }
  if(C.not_a_cmd()) is_ok = false;
  else if(C==hash_table.relax_token) is_ok =false;
  else if(!hash_table.eqtb[C.eqtb_loc()].is_undef_or_relax()) is_ok = false;
  else {
     uint x = C.get_val();
     if(x>cs_token_flag+null_cs) {
       string s = hash_table[C.hash_loc()];
       if(s[0]=='e' && s[1]=='n' && s[2]=='d') is_ok=false;
     }
  }
  if(is_ok) back_input(B);
  else parse_error("Undefinable command ",C,"","bad");
}

// Interpret the optional argument of \enumerate. If it is
// [(i)], then executes \def\@itemlabel{(\theenumiv)}
// where \theenumiv is the last argument. 
// redefines \def\theenumiv{\arabic{enumiv}}
bool Parser::optional_enumerate(TokenList& L,String ctr)
{
  Hashtab& H= hash_table;
  TokenList res;
  int b = 0;
  Buffer& B= local_buf;
  B << bf_reset << "the" << ctr;
  Token the_ctr = H.locate(B);
  Token cmd = H.relax_token;
  for(;;) {
    if(L.empty()) break;
    Token T = L.front();
    L.pop_front();
    if(T.is_a_brace()){
      if(T.is_a_left_brace()) b++;
      else b--;
    } 
    if(b) { res.push_back(T); continue; }
    if(T==Token(other_t_offset,'1')) { cmd = H.arabic_token ;}
    else if(T==Token(letter_t_offset,'A')) { cmd = H.Alph_token; }
    else if(T==Token(letter_t_offset,'a')) { cmd = H.alph_token; }
    else if(T==Token(letter_t_offset,'i')) { cmd = H.roman_token; }
    else if(T==Token(letter_t_offset,'I')) { cmd = H.Roman_token; }
    else { res.push_back(T); continue; }
    res.push_back(the_ctr);
  }
  if(cmd==H.relax_token) return false; // Invalid argument, no warning ?
  Macro* X= new Macro(res);
  Token t = hash_table.itemlabel_token;
  mac_define(t,X,false,rd_always,user_cmd); 
  B << bf_reset << ctr;
  TokenList the_ctr_value = B.str_toks11(true);
  brace_me(the_ctr_value);
  the_ctr_value.push_front(cmd);  
  Macro* Y = new Macro(the_ctr_value);
  mac_define(the_ctr,Y,false,rd_always,user_cmd); 
  return true;
}

// This is \usecounter{foo} 
// remembers locally the name,  changes globally the counter to 0
// Caller checks that this is a counter
void Parser::T_use_counter(const string& s)
{
  string_define(1,s,false);
  Buffer&b = local_buf;
  b << bf_reset << "c@" << s;
  Token T  = hash_table.locate(b);
  Equivalent& E = hash_table.eqtb[T.eqtb_loc()];
  if(E.get_cmd() != assign_int_cmd) return;
  word_define(E.get_chr(),0,true);
}

void token_ns::int_to_roman(Buffer&b, int n)
{
  switch(n) {
  case 1: b << "i"; break;
  case 2: b << "ii"; break;
  case 3: b << "iii"; break;
  case 4: b << "iv"; break;
  case 5: b << "v"; break;
  case 6: b << "vi"; break;
  case 7: b << "vii"; break;
  case 8: b << "viii"; break;
  case 9: b << "ix"; break;
  default: b << "x";
  }
}

// Start of list, itemize, enumerate, or description

void Parser::T_listenv(symcodes x)
{
  leave_h_mode();
  bool is_enum = x==enumerate_cmd;
  int listdepth = eqtb_int_table[list_depth_code].get_val();
  listdepth += is_enum ? 100 : 1;
  word_define(list_depth_code,listdepth,false);
  int n=listdepth;
  if(is_enum) n = n/100;
  n = n%100;
  Buffer&b = local_buf;
  b << bf_reset << (is_enum ? "enum" : "Enum") ;
  token_ns::int_to_roman(b,n);
  string list_ctr = b.to_string();
  name_positions np= x==list_cmd? np_user_list :
    x==itemize_cmd ?np_simple:
    x==enumerate_cmd ? np_ordered : np_description;
  Token t = hash_table.itemlabel_token;
  eval_let(t, hash_table.relax_token,false);
  T_use_counter(list_ctr);
  TokenList L;
  if(x==enumerate_cmd) {
    next_optarg_long(L);
    // Token cmd = hash_table.relax_token;  unused (why ?)
    if(!optional_enumerate(L,list_ctr.c_str())){
      b << bf_reset << "labelenum";
      token_ns::int_to_roman(b,n);
      Token T = hash_table.locate(b); 
      eval_let(t, T,false);
    }
  }
  if(x==list_cmd) {
    TokenList L = mac_arg();
    Macro* X= new Macro(L);
    mac_define(t,X,false,rd_always,user_cmd);
    TokenList L2 = mac_arg();
    back_input(L2);
  } 
  if(x==enumerate_cmd) {
    b << bf_reset << "enum";
    token_ns::int_to_roman(b,n);
    b << "@hook"; 
    Token T = hash_table.locate(b);
    int pos = T.eqtb_loc();
    if(!hash_table.eqtb[pos].is_undefined()) back_input(T);
  }
  Xmlp res = new Xml(np_list,0);
  the_stack.push(the_names[np_list],res);

  res->get_id().add_attribute(np_type,np);
}



