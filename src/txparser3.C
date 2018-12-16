// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains a part of the TeX parser of tralics


#include "tralics.h"
const char* txparser3_rcsid=
  "$Id: txparser3.C,v 2.15 2015/07/27 16:15:40 grimm Exp $";


namespace parser_ns {
  String to_string(boundary_type v);
}

namespace {
  vector<SaveAux*>the_save_stack;
  Xmlp the_box_to_end;
  int the_box_position = -1;
  Buffer Thbuf1;
}

// --------------------------------------------------

String to_string (save_type v)
{
  switch(v) {
  case st_boundary: return "boundary";
  case st_cmd: return "command";
  case st_int: return "integer";
  case st_dim : return "dimension";
  case st_glue: return "glue";
  case st_token: return "token";
  case st_save: return "save";
  case st_env: return "environment";
  case st_font: return "font";
  case st_box: return "box";
  case st_box_end: return "end of box";
  case st_string: return "string";
  default: return "unknown";
  }
}

SaveScannerStatus::SaveScannerStatus(scan_stat c) 
{
  code = the_parser.get_scanner_status();
  the_parser.set_scanner_status(c);
}


SaveScannerStatus::~SaveScannerStatus()
{ 
  the_parser.set_scanner_status(code); 
}

SaveLongState::SaveLongState(l_state c) 
{
  code = the_parser.get_long_state();
  the_parser.set_long_state(c);
}

SaveLongState::~SaveLongState()
{ 
  the_parser.set_long_state(code); 
}


Parser::Parser() :
  unexpected_seen_hi(false),  force_eof(false), 
  no_new_file(false), chapter_has_star(false),
  begin_env_line(0), 
  ra_year(1789), after_assignment_token(0), 
  default_language_num(0), cur_file_pos(0), cur_env_name("document"),
  //  input_buffer("main input buffer"),
  //  mac_buffer("mac buffer"),
  //  group_buffer("group_buffer"), unprocessed_xml("unprocessed xml"),
  verb_saved_char(0),input_line_pos(0), the_xmlA(0), the_xmlB(0)
{
  sectionning_offset=section_code;
  restricted = false;
  cur_level = level_one;
  calc_loaded = false;
  cur_in_chan = main_in_chan;
  long_state = ls_bad;
  scanner_status = ss_normal;
  list_files_p = false;
  eqtb_int_table[count_reg_offset+10].set_val(20); // counter
  eqtb_int_table[count_reg_offset+11].set_val(12); // dimen
  eqtb_int_table[count_reg_offset+12].set_val(10); 
  eqtb_int_table[count_reg_offset+13].set_val(10); 
  eqtb_int_table[count_reg_offset+14].set_val(10); 
  eqtb_int_table[count_reg_offset+15].set_val(10); 
  eqtb_int_table[count_reg_offset+16].set_val(0); // newread 
  eqtb_int_table[count_reg_offset+17].set_val(0); // newwrite 
  eqtb_int_table[count_reg_offset+18].set_val(10); 
  eqtb_int_table[count_reg_offset+19].set_val(10); 
  cur_font.pack();
  cur_font.set_old_from_packed();
}


// Saves in *this the catcode at position c, modifies it to be nc
SaveCatcode::SaveCatcode(int c, int nc) : character(c)
{ 
  code = the_parser.eqtb_int_table[character].get_val(); 
  the_parser.eqtb_int_table[character].set_val(nc);
}

// Undoes a catcode change
SaveCatcode::~SaveCatcode() 
{ 
  the_parser.eqtb_int_table[character].set_val(code);
} 



// saving and restoring things

// This converts a boundary_type to a String for printing
String parser_ns::to_string(boundary_type v)
{
  switch(v) {
  case bt_brace: return "brace";
  case bt_cell: return "cell";
  case bt_local: return "local";
  case bt_semisimple: return "\\begingroup";
  case bt_esemisimple: return "\\endgroup";
  case bt_env: return "environment";
  case bt_tpa: return "titlepage argument";
  case bt_math: return "math";
  default: return "impossible";
  }
}

int Parser::cur_group_type()
{
  boundary_type V = first_boundary();
  switch(V) {
  case bt_brace: return 1;
  case bt_cell: return 19;
  case bt_local: return 20;
  case bt_semisimple: return 14;
  case bt_esemisimple: return 17;
  case bt_env: return 18;
  case bt_tpa: return 21;
  case bt_math: return 9;
  case bt_impossible: return 0;
  default: return 17;
  }
}

ostream& operator<< (ostream&fp, const boundary_type&x)
{
  fp << parser_ns::to_string(x);
  return fp;
}

// This adds a new element to the save stack.
void Parser::push_save_stack(SaveAux *v)
{
  my_stats.one_more_up();
  v->set_line(get_cur_line());
  the_save_stack.push_back(v);
}

// This is done when we evaluate { or \begingroup.
void Parser::push_level(boundary_type v)
{
  push_save_stack(new SaveAuxBoundary(v));
  cur_level++;
  if(tracing_stack()) 
    the_log <<lg_startstack << "level + " << cur_level << " for " 
	    << v << " entered on line " << get_cur_line() << lg_end;
}

void Parser::push_tpa()
{
  push_save_stack(new SaveAuxBoundary(bt_tpa));
  if(tracing_stack()) 
    the_log <<lg_startstack << "level = " << cur_level 
	    << " for " << bt_tpa << lg_end;
}

// Defines something at position A, with type and subtype B and C.
// Definition is global if gbl is true. In the case of a user defined
// command, we must increase the reference count, and if we override
// a user defined command we must decrease its reference count (maybe kill).
// If the definition is local, the old definition is saved on the save stack.
void Parser::eq_define (int a, CmdChr bc, bool gbl)
{
  if(bc.is_user()) mac_table.incr_macro_ref(bc.get_chr());
  if(!gbl && hash_table.eqtb[a].must_push(cur_level)) 
    push_save_stack(new SaveAuxCmd(a,hash_table.eqtb[a]));
  else if(hash_table.eqtb[a].is_user()) 
    mac_table.delete_macro_ref(hash_table.eqtb[a].get_chr());
  if(gbl) hash_table.eqtb[a].primitive(bc);
  else hash_table.eqtb[a].set(bc,cur_level);
}

// This defines a user command, associated to the token A.
// Depending of redef, this may be valid or not 
// (implements \provideXX, \newXX and \renewXX)
// We either kill the macro, or increment its reference count.
void Parser::mac_define(Token a, Macro* b, bool gbl,rd_flag redef,symcodes what)
{
  if(ok_to_define(a,redef)) {
    subtypes B = mac_table.new_macro(b);
    if(tracing_assigns()) {
      String action = gbl ? "globally " :  "";
      the_log << lg_startbrace << action << "changing " <<  a <<"=" ;
      token_show(hash_table.eqtb[a.eqtb_loc()].get_cmdchr());
    }
    eq_define(a.eqtb_loc(),CmdChr(what,B),gbl);
    if(tracing_assigns()) {
      the_log << "}\n{into " << a << "=";
      token_show(hash_table.eqtb[a.eqtb_loc()].get_cmdchr());
      the_log << lg_endbrace; 
    }
  } else delete b;     
}

// For \newcommand, \renewcommand: we signal an error if we cannot define
// the thing. For \providecommand: the function returns true if we have
// to define, false otherwise.
bool Parser::ok_to_define(Token a, rd_flag redef)
{
  if(redef==rd_never) return false;
  if(a.not_a_cmd()) return false; // Should not happen
  if(a == hash_table.frozen_protection) return false;
  if(redef==rd_always) return true;
  int A = a.eqtb_loc();
  bool undef_or_relax = hash_table.eqtb[A].is_undef_or_relax();
  if(redef==rd_if_defined && undef_or_relax) {
    bad_redefinition(1,a);
    return false;
  }
  if(redef==rd_if_undef && !undef_or_relax) {
    bad_redefinition(0,a);
    return false;
  }
  if(redef==rd_skip && !undef_or_relax)
    return false;
  return true;
}

// Define for an integer quantity. Like eq_define without reference counts.
void Parser::word_define (int a, int c, bool gbl)
{
  EqtbInt& W = eqtb_int_table[a];
  bool reassign = !gbl && W.get_val()==c;
  if(tracing_assigns()) {
    CmdChr tmp(assign_int_cmd, subtypes(a));
    String action = gbl ? "globally changing" : 
      reassign ? "reassigning" : "changing";
    the_log << lg_startbrace << action << " \\" <<  tmp.name() <<"=" << 
      W.get_val();
    if(!reassign) the_log << " into \\" << tmp.name() << "=" << c;
    the_log << lg_endbrace; 
  }
  if(gbl)
    W.val_and_level(c, level_one);
  else if(reassign) return;
  else {
    if(W.must_push(cur_level))
      push_save_stack(new SaveAuxInt(W.get_level(),a,W.get_val()));
    W.val_and_level(c,cur_level);
  }
}
// Define for an string quantity. Like eq_define without reference counts.
void Parser::string_define (int a, const string& c, bool gbl)
{
  EqtbString& W = eqtb_string_table[a];
  bool reassign = !gbl && W.get_val()==c;
  if(tracing_assigns()) {
    String action = gbl ? "globally changing" : 
      reassign ? "reassigning" : "changing";
    the_log << lg_startbrace << action << " string" <<  a <<"='" << W.get_val() << "'";
    if(!reassign) the_log << " into '"  << c << "'";
    the_log << lg_endbrace; 
  }
  if(gbl)
    W.val_and_level(c, level_one);
  else if(reassign) return;
  else {
    if(W.must_push(cur_level))
      push_save_stack(new SaveAuxString(W.get_level(),a,W.get_val()));
    W.val_and_level(c,cur_level);
  }
}

// Define for a dimension quantity. Like eq_define without reference counts.
void Parser::dim_define (int a, ScaledInt c, bool gbl)
{
  EqtbDim& W = eqtb_dim_table[a];
  bool reassign = !gbl && W.get_val()==c;
  if(tracing_assigns()) {
    CmdChr tmp(assign_dimen_cmd, subtypes(a));
    String action = gbl ? "globally changing" : 
      reassign ? "reassigning" : "changing";
    the_log << lg_startbrace << action << " \\" <<  tmp.name() <<"=" << 
      W.get_val();
    if(!reassign) the_log << " into \\" << tmp.name() << "=" << c;
    the_log << lg_endbrace; 
  }
  if(gbl)
    W.val_and_level(c, level_one);
  else if(reassign) return;
  else {
    if(W.must_push(cur_level))
      push_save_stack(new SaveAuxDim(W.get_level(),a,W.get_val()));
    W.val_and_level(c,cur_level);
  }
}

bool operator==(const Glue& a, const Glue&b)
{
  return a.get_width() == b.get_width()  &&
    a.get_shrink() == b.get_shrink()  &&
    a.get_stretch() == b.get_stretch()  &&
    a.get_shrink_order() == b.get_shrink_order()  &&
    a.get_stretch_order() == b.get_stretch_order();
}


// Define for a glue quantity
void Parser::glue_define (int a, Glue c, bool gbl)
{
  EqtbGlue& W = glue_table[a];
  bool reassign = !gbl && W.get_val()==c;
  if(tracing_assigns()) {
    CmdChr tmp(assign_glue_cmd, subtypes(a));
    String action = gbl ? "globally changing" : 
      reassign ? "reassigning" : "changing";
    the_log << lg_startbrace << action << " \\" <<  tmp.name() <<"=" << 
      W.get_val();
    if(!reassign) the_log << " into \\" << tmp.name() << "=" << c;
    the_log << lg_endbrace; 
  }
  if(gbl)
    W.val_and_level(c, level_one);
  else if(reassign) return;
  else {
    if(W.must_push(cur_level)) 
      push_save_stack(new SaveAuxGlue(W.get_level(),a,W.get_val()));
    W.val_and_level(c, cur_level);
  }
}

// Define for a box quantity
void Parser::box_define (int a, Xmlp c, bool gbl)
{
  EqtbBox& W = box_table[a];
  if(tracing_assigns()) {
    String action = gbl ? "globally changing" :  "changing";
    the_log << lg_startbrace << action << " \\box" <<  a <<"=" << 
      W.get_val();
    the_log << " into \\box" << a << "=" << c;
    the_log << lg_endbrace; 
  }
  if(gbl)
    W.val_and_level(c, level_one);
  else {
    if(W.must_push(cur_level))
      push_save_stack(new SaveAuxBox(W.get_level(),a,W.get_val()));
    W.val_and_level(c, cur_level);
  }
}

// Same code for a token list.
void Parser::token_list_define(int p, TokenList& c, bool gbl)
{
  EqtbToken& W = toks_registers[p];
  bool reassign = !gbl && W.get_val()==c;
  if(tracing_assigns()) {
    CmdChr tmp(assign_toks_cmd, subtypes(p));
    String action = gbl ? "globally changing" : 
      reassign ? "reassigning" : "changing";
    the_log << lg_startbrace << action << " \\" <<  tmp.name() <<"=" << 
      W.get_val();
    if(!reassign) the_log << " into \\" << tmp.name() << "=" << c;
    the_log << lg_endbrace; 
  }
  if(gbl) {
    W.val_and_level(c,level_one);
  } else {
    if(W.must_push(cur_level))
      push_save_stack(new SaveAuxToken(W.get_level(), p, W.get_val()));
    W.val_and_level(c,cur_level);
  }
}

// This is called whenever a font has changed. 
// It pushes the value on the save stack if needed.
void Parser::save_font()
{
  if(tracing_commands())
    the_log << lg_startbrace << "font change " << cur_font << lg_endbrace;
  if(cur_font.get_level() == cur_level) return;
  push_save_stack(new SaveAuxFont(cur_font.get_level(),cur_font.get_old(),
				  cur_font.get_old_color()));
  cur_font.set_level(cur_level);
}

// When we pop a level, the current font may change. 
void SaveAuxFont::unsave(bool trace, Parser& P)
{
  P.flush_buffer(); 
  P.cur_font.set_old_from_packed();
  P.cur_font.set_level(level);
  P.cur_font.set_packed(value);
  P.cur_font.set_color(color);
  P.cur_font.unpack();
  if(trace)
    the_log <<lg_startbrace << "font restore " << P.cur_font << lg_endbrace;
  P.font_has_changed1();
  P.cur_font.update_old();
}


// If you say: \let\foo=1 \global\let\foo=2
// the first \let saves the old value on the stack. The \global makes this
// irrelevant.
void SaveAux::restore_or_retain(bool rt) const
{
  the_log << lg_startstack << (rt? "restoring " : "retaining ");
}

// Utility function for unsave_trace.
void SaveAux::unsave_trace_aux(String s, int pos, bool rt)
{
  restore_or_retain(rt);
  the_log <<  s << " value at position "  << pos << lg_end;
}

// This done when we restore an integer value 
void SaveAuxInt::unsave(bool trace, Parser& P)
{
  bool rt = P.eqtb_int_table[pos].get_level()!= level_one;
  if(trace) {
    restore_or_retain(rt);
    CmdChr tmp(assign_int_cmd, subtypes(pos));
    the_log <<  "integer value";
    if(rt) the_log << " " << val ;
    the_log << " for \\" << tmp.name() << lg_end;
  }
  if(rt) P.eqtb_int_table[pos].val_and_level(val,level);
}

// This done when we restore an integer value 
void SaveAuxString::unsave(bool trace, Parser& P)
{
  bool rt = P.eqtb_string_table[pos].get_level()!= level_one;
  if(trace) {
    restore_or_retain(rt);
    the_log <<  "string value";
    if(rt) the_log << " " << val ;
    the_log << " " << pos << lg_end;
  }
  if(rt) P.eqtb_string_table[pos].val_and_level(val,level);
}


// This done when we restore a dimension value
void SaveAuxDim::unsave(bool trace, Parser& P)
{
  bool rt = P.eqtb_dim_table[pos].get_level()!= level_one;
  if(trace) {
    restore_or_retain(rt);
    CmdChr tmp(assign_dimen_cmd, subtypes(pos));
    the_log << "dimension value";
    if(rt) the_log << " " << val ;
    the_log << " for \\" << tmp.name() << lg_end;
  }
  if(rt) P.eqtb_dim_table[pos].val_and_level(val,level);
}

// Restore command. We have to take care to free memory for user commands.
void SaveAuxCmd::unsave(bool trace, Parser& P)
{
  int lvl = P.hash_table.eqtb[cs].get_level();
  if(trace) {
    String S = lvl== level_one ? "retaining " 
      : (val.is_undef() ? "killing " : "restoring ");
    the_log << lg_startstack << S << Token(cs+1+cs_token_flag);
    if(lvl>level_one && val.is_user())
      the_log << P.mac_table.get_macro(val.get_chr());
    else if(val.is_undef()) {}
    else if(lvl>level_one) {
      the_log << "=";
      P.print_cmd_chr(val);
    }
    the_log << lg_end;
  }
  if(lvl==level_one) {
    if(val.is_user()) P.mac_table.delete_macro_ref(val.get_chr());
  } else {
    if(P.hash_table.eqtb[cs].is_user()) 
      P.mac_table.delete_macro_ref(P.hash_table.eqtb[cs].get_chr());
    P.hash_table.eqtb[cs].set(val,level);
  }
}

// Restore glue
void SaveAuxGlue::unsave(bool trace, Parser& P)
{
  bool rt = P.glue_table[pos].get_level() != level_one;
  if(trace) {
    restore_or_retain(rt);
    CmdChr tmp(assign_glue_cmd, subtypes(pos));
    if(pos>=thinmuskip_code) the_log <<"mu";
    the_log <<  "glue value for \\" << tmp.name() << lg_end;
  }
  if(rt) 
    P.glue_table[pos].val_and_level(val,level);
}

// Restore token list. 
void SaveAuxToken::unsave(bool trace, Parser& P)
{
  bool rt = P.toks_registers[pos].get_level() != level_one;
  if(trace) {
    restore_or_retain(rt); 
    CmdChr tmp(assign_toks_cmd, subtypes(pos));
    the_log << "token value for \\" << tmp.name() << lg_end;
  }
  if(rt)
    P.toks_registers[pos].val_and_level(val,level);
}

// Restore box. Called in the case {\setbox0=\hbox{...}} 
// when we see the last closing brace. This may restore box0.
void SaveAuxBox::unsave(bool trace, Parser& P)
{
  bool rt = P.box_table[pos].get_level() != level_one;
  if(trace)
    unsave_trace_aux("box", pos,rt);
  if(rt)
    P.box_table[pos].val_and_level(val,level);
}

// Restore box. Called in the case {\setbox0=\hbox{...}} 
// when we see the first closing brace. The box just created will be put in
// box0. 
void SaveAuxBoxend::unsave(bool trace, Parser& P)
{
  if(trace)
    the_log << lg_startstack << "finish a box of type " << pos << lg_end;
  P.flush_buffer();
  P.the_stack.pop(the_names[cst_hbox]);
  the_box_to_end = val;
  the_box_position = pos;
}

// \aftergroup\foo{}: When the group is finished, the token \foo is 
// pushed back into the input stream.
void SaveAuxAftergroup::unsave(bool trace, Parser& P)
{
  if(trace)
    the_log << lg_startstack << "after group " << value << lg_end;
  P.back_input(value); 
}

// Consider the case of {\setbox0=\hbox{...}} 
// We must call box_end, the function that fills box0 with the current box.
// The number of the box is set by SaveAuxBoxend into the_box_position
// But this function cannot call box_end (because we are still in the {...}
// group. Hence box_end must be called after cur_level is decremented.
void SaveAuxBoundary::unsave(bool trace, Parser& P)
{
  if(trace) the_log << lg_startstack << "level - "  << P.get_cur_level()
		    << " for " << val << " from line " << line_no << lg_end;
  P.decr_cur_level();
  if(the_box_position>=0) {
    if(the_box_to_end) the_box_to_end->remove_last_empty_hi();
    P.box_end(the_box_to_end,the_box_position);
    the_box_position = -1;
  }
}

boundary_type Parser::first_boundary()
{
  int n = the_save_stack.size();
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];    
    if(p->type!=st_boundary) continue;
    return  static_cast<SaveAuxBoundary*>(p)->get_val();
  }
  return bt_impossible;
}

void Parser::dump_save_stack()
{
  int L = cur_level-1;
  int n = the_save_stack.size();
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];    
    if(p->type!=st_boundary) continue;
    static_cast<SaveAuxBoundary*>(p)->dump(L);
    --L;
  } 
  the_log << "### bottom level\n";
}
void SaveAuxBoundary::dump(int n)
{
  String s= val== bt_semisimple ? "semi simple" : parser_ns::to_string(val);
  the_log << "### "<< s << " group (level " << n << ") entered at line "<< line_no << "\n";
}

// The function called when we see a closing brace or \endgroup.
void Parser::pop_level(bool allow_env, boundary_type v)
{
  bool trace = tracing_stack();
  bool must_throw = false;
  boundary_type w = first_boundary();
  for(;;) {
    if(the_save_stack.empty()) {
      parse_error("Extra }");
      return;
    }
    SaveAux* tmp = the_save_stack.back();
    if(!allow_env &&tmp->type==st_env) {
      if(v==bt_semisimple) v = bt_esemisimple;
      Thbuf1 << bf_reset << "Extra " <<  parser_ns::to_string(v) <<
	" found in unclosed env";
      parse_error(Thbuf1.c_str(),"extra brace");
      return;
    }
    the_save_stack.pop_back();
    bool ok = tmp->type==st_boundary;
    if(ok) {
      if(v==bt_brace && w ==bt_tpa)
	must_throw = true;
      else if(v != w)
	wrong_pop(parser_ns::to_string(v),parser_ns::to_string(w));
    }
    if(ok && must_throw) cur_level++; 
    tmp->unsave(trace,*this);
    my_stats.one_more_down();
    delete tmp;
    if(ok) {
      if(must_throw) {
	throw EndOfData(); 
      } else  return;
    }
  }
}

// Signals error for unclosed environments and braces
void Parser::pop_all_levels()
{
  pop_level(true,bt_env); // pop the end document
  bool started = false;
  string ename = "";
  Buffer& B = err_ns::local_buf; B.reset();
  for(;;) {
    if(the_save_stack.empty()) break;
    SaveAux* tmp = the_save_stack.back();
    cout << to_string(tmp->type) << " at " << tmp-> line_no << "\n";
    if(tmp->type==st_env) {
      SaveAuxEnv* q = static_cast<SaveAuxEnv*>(tmp);
      ename =  q->get_name();
    }
    if(tmp->type==st_boundary) {
      boundary_type w =static_cast<SaveAuxBoundary*>(tmp)->get_val();
      int l = tmp->get_line();
      if(started) { 
	B<< ".\n"; // finish prev line
	main_ns::nb_errs ++;
      }
      started = true;
      B <<  "Non-closed " <<  parser_ns::to_string(w);
      if(w==bt_env) B << " `" << ename << "'";
      B << " started at line " << l;
    }
    the_save_stack.pop_back();
    tmp->unsave(false,*this);
    my_stats.one_more_down();
    delete tmp;
  }
  if(started) {
    err_ns::signal_error("",1);
    B<< ".\n";
    out_warning(B, mt_none); // insert a warning in the XML if desired
  }
  push_level(bt_env);
}


// This is done when all is finished. 
// We try to pop a font command
void Parser::final_checks()
{
  conditions.terminate();
  int n = the_save_stack.size();
  if(n==1) {
    SaveAux* tmp = the_save_stack.back();
    if (tmp->type== st_font) {
      the_save_stack.pop_back();
      tmp->unsave(false,*this);
      my_stats.one_more_down();
      delete tmp;
      n = the_save_stack.size();
    }
  }
  if(n==0) return;
  the_log << "Number of items on the save stack: "<< n << "\n"; 
  Buffer& B = err_ns::local_buf; B.reset();
  Buffer& A = Thbuf1;
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];
    A.reset ();
    A << to_string(p->type) << " at " << p-> line_no;
    if (B.size () == 0) {
       B << "  " << A;
    } else if(B.size () + A.size () < 78) { 
      B << "; " << A;
    } else { the_log << B << "\n"; B.reset (); B << "  " << A; }
  } 
  the_log << B << ".\n";
}


// Returns the slot associated to the env S
SaveAuxEnv* Parser::is_env_on_stack(const string& s)
{
  int n = the_save_stack.size();
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];    
    if(p->type!=st_env) continue;
    SaveAuxEnv* q = static_cast<SaveAuxEnv*>(p);
    if(q->get_name()==s) 
      return q;
  }
  return 0;
}
// Returns the number of env named S
int Parser::nb_env_on_stack(const string& s)
{
  int n = the_save_stack.size();
  int k = 0;
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];    
    if(p->type!=st_env) continue;
    SaveAuxEnv* q = static_cast<SaveAuxEnv*>(p);
    if(q->get_name()==s) 
      ++k;
  }
  return k;
}

// Returns the number of environments
int Parser::nb_env_on_stack()
{
  int n = the_save_stack.size();
  int k = 0;
  for(int i=n-1;i>=0;i--) {
    SaveAux*p = the_save_stack[i];    
    if(p->type==st_env)  ++k;
  }
  return k;
}

// ----------------------------------------------------------------------
// TIPA macros
void Parser::T_ipa(subtypes c)
{
  if(c==0) tipa_star();
  if(c==1) tipa_semi();
  if(c==2) tipa_colon();
  if(c==3) tipa_exclam();
  if(c==4) tipa_normal();
}

void  Parser::mk_hi(String x, char c)
{
  the_parser.LC();
  the_parser.unprocessed_xml << "<hi rend='" << x << "'>" 
			     << c << "</hi>";
}

void Parser::tipa_star()
{
  if(get_token()) return; // should not happen 
  int cmd = cur_cmd_chr.get_cmd();
  if(cmd==12 || cmd==11) {
    int n = cur_cmd_chr.get_chr();
    if(n>0 && n <128) {
      if(n=='k') { extended_chars(0x29e); return; }
      if(n=='f') { mk_hi("turned",'f'); return; }
      if(n=='t') { extended_chars(0x287); return; }
      if(n=='r') { extended_chars(0x279); return; }
      if(n=='w') { extended_chars(0x28d); return; }
      if(n=='j') { extended_chars(0x25f); return; }
      if(n=='n') { extended_chars(0x272); return; }
      if(n=='h') { extended_chars(0x127); return; } // 0x45b ?
      if(n=='l') { extended_chars(0x26c); return; }
      if(n=='z') { extended_chars(0x26e); return; }
    }
  }
  back_input(cur_tok);
}

void Parser::tipa_semi()
{
  if(get_token()) return; // should not happen 
  int cmd = cur_cmd_chr.get_cmd();
  if(cmd==12 || cmd==11) {
    int n = cur_cmd_chr.get_chr();
    if(n>0 && n <128) {
      if(n=='E' || n=='J' || n=='A' || n=='U') {
	mk_hi("sc", char(n));
	return;
      }
      if(n=='H') { extended_chars(0x29c); return; }
      if(n=='L') { extended_chars(0x29f); return; }
      if(n=='B') { extended_chars(0x299); return; }
      if(n=='G') { extended_chars(0x262); return; }
      if(n=='N') { extended_chars(0x274); return; }
      if(n=='R') { extended_chars(0x280); return; }
    }
  }
  back_input(cur_tok);
}

void Parser::tipa_colon()
{
  if(get_token()) return; // should not happen 
  int cmd = cur_cmd_chr.get_cmd();
  if(cmd==12 || cmd==11) {
    int n = cur_cmd_chr.get_chr();
    if(n>0 && n <128) {
      if(n=='d') { extended_chars(0x256); return; }
      if(n=='l') { extended_chars(0x26d); return; }
      if(n=='n') { extended_chars(0x273); return; }
      if(n=='r') { extended_chars(0x27d); return; }
      if(n=='R') { extended_chars(0x27b); return; }
      if(n=='s') { extended_chars(0x282); return; }
      if(n=='t') { extended_chars(0x288); return; }
      if(n=='z') { extended_chars(0x290); return; }
    }
  }
  back_input(cur_tok);
}

void Parser::tipa_exclam()
{
  if(get_token()) return; // should not happen 
  int cmd = cur_cmd_chr.get_cmd();
  if(cmd==12 || cmd==11) {
    int n = cur_cmd_chr.get_chr();
    if(n>0 && n <128) {
      if(n=='G') { extended_chars(0x29b); return; }
      if(n=='b') { extended_chars(0x253); return; }
      if(n=='d') { extended_chars(0x257); return; }
      if(n=='g') { extended_chars(0x260); return; }
      if(n=='j') { extended_chars(0x284); return; }
      if(n=='o') { extended_chars(0x298); return; }
    }
  }
  back_input(cur_tok);
}

void Parser::tipa_normal()
{
  if(get_token()) return; // should not happen 
  int cmd = cur_cmd_chr.get_cmd();
  if(cmd==12 || cmd==11) {
    int n = cur_cmd_chr.get_chr();
    if(n>0 && n <128) {
      if(n=='0'){ extended_chars(0x289); return; }
      if(n=='1'){ extended_chars(0x268); return; }
      if(n=='2'){ extended_chars(0x28c); return; }
      if(n=='3'){ extended_chars(0x25c); return; }
      if(n=='4'){ extended_chars(0x265); return; }
      if(n=='5'){ extended_chars(0x250); return; }
      if(n=='6'){ extended_chars(0x252); return; }
      if(n=='7'){ extended_chars(0x264); return; }
      if(n=='8'){ extended_chars(0x275); return; }
      if(n=='9'){ extended_chars(0x258); return; }
      if(n=='@'){ extended_chars(0x259); return; }
      if(n=='A'){ extended_chars(0x251); return; }
      if(n=='B'){ extended_chars(0x3b2); return; } // beta
      if(n=='C'){ extended_chars(0x255); return; }
      if(n=='D'){ extended_chars(0x0f0); return; } // eth
      if(n=='E'){ extended_chars(0x25B); return; }
      if(n=='F'){ extended_chars(0x278); return; }
      if(n=='G'){ extended_chars(0x263); return; }
      if(n=='H'){ extended_chars(0x266); return; }
      if(n=='I'){ extended_chars(0x26A); return; }
      if(n=='J'){ extended_chars(0x29D); return; }
      if(n=='K'){ extended_chars(0x261); return; }
      if(n=='L'){ extended_chars(0x28E); return; }
      if(n=='M'){ extended_chars(0x271); return; }
      if(n=='N'){ extended_chars(0x14b); return; } // eng 
      if(n=='O'){ extended_chars(0x254); return; }
      if(n=='P'){ extended_chars(0x294); return; }
      if(n=='Q'){ extended_chars(0x295); return; }
      if(n=='R'){ extended_chars(0x27E); return; }
      if(n=='S'){ extended_chars(0x283); return; }
      if(n=='T'){ extended_chars(0x3b8); return; } // theta
      if(n=='U'){ extended_chars(0x28a); return; }
      if(n=='V'){ extended_chars(0x28b); return; }
      if(n=='W'){ extended_chars(0x26f); return; }
      if(n=='X'){ extended_chars(0x3c7); return; } // chi
      if(n=='Y'){ extended_chars(0x28f); return; }
      if(n=='Z'){ extended_chars(0x292); return; }
    }
  }
  back_input(cur_tok);
}


// 25a 25d 25e 
// 262 267 269 26b 
// 270 276 277 27a 27c 27f
// 281 285 286 
// 291 x293 296 297 29a 
// 2a0 2a1 2a2 2a3 2a4 2a5 2a6 2a7 2a8 2a9 2aa 2ab 2ac 2ad 2ae 2af

