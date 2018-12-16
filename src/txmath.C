// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Tralics, math part I

#include "tralics.h"
#include "txmath.h"

const char* txmath_rcsid = 
  "$Id: txmath.C,v 2.141 2015/08/06 09:28:44 grimm Exp $";

namespace {
  Buffer math_buffer;
  Buffer special_buffer;
  Buffer math_buffer1;
  Buffer Trace;
  int old_pos=0; // pointer into trace
  MathHelper cmi; // Data structure holding some global values
  bool trace_needs_space; // bool  for: \frac\pi y
  bool old_need = false; // prev value of trace_needs_space
  Xmlp xmlspace; // Const, xml element containing a space
  Token fct_caller;
  string the_tag = "";
  vector <Xmlp> all_maths;
}

MathDataP math_data;

extern bool only_input_data;

using namespace math_ns;
namespace math_ns {
  void add_to_trace(Token x);
  void add_to_trace(char);
  void add_to_trace(const string& x);
  void remove_from_trace();
  void bad_math_warn(Buffer&);
  Xmlp finish_cv_special(bool isfrac, Istring s,int pos, Xmlp a, Xmlp b,
				 Istring sz,int numalign, int denalign,
			       int style,int open, int close);
};

// This duplicates the formula.
// It removes tokens preceeded by the special marker. 

subtypes Math::duplicate (bool nomath) const
{
  subtypes k = math_data.find_math_location(type);
  Math& cp =  math_data.get_list(k);
  int sp = 0, sm = 0;
  bool skipping = false;
  bool skip_next= false;
  cp.name = name;
  const_math_iterator C = value.begin();
  const_math_iterator E = value.end();
  while(C != E) {
    skip_next = skipping;
    if(C->get_cmd()== nomath_cmd) {
      int s = C->get_chr();
      ++C; if(C==E) break;
      if(s==0) skip_next = nomath;
      else if(s==1) skip_next = !nomath;
      else {
	if (s==2) sp++;
        else if (s == 3) { if (sp >0) sp --;sm++ ; }
        else if (s == 4) { if (sm >0) sm-- ; }
        else { sp = sm = 0; } // should not happen, clear the stack
	skipping = false;
 	if (sp> 0 && nomath) skipping = true;
	if (sm> 0 && !nomath) skipping = true;
	skip_next = skipping;
      }
    }
    if(skip_next) {} 
    else if(C->get_cmd()== math_list_cmd) {
      Math& v = C->get_list();
      subtypes k1 = v.duplicate(nomath);
      cp.push_back_list(k1,C->get_lcmd());
    } else if(C-> get_cmd() ==special_math_cmd) {
      Math& v = C->get_list();
      subtypes k1 = v.duplicate(nomath);
      cp.push_back(CmdChr(special_math_cmd,k1),C->get_font());
    } else cp.push_back (*C); 
    ++C;
  }
  return k;
}

void Parser::select_math_font()
{
  bool bold=is_pos_par(atmathversion_code);
  word_define(math_font_pos,bold? math_f_bold: math_f_normal,false); 
}


void math_ns::remove_from_trace()
{
  trace_needs_space = old_need;
  Trace.set_last(old_pos);
}

// Three functions that add something to the trace.
// We want spaces line this: \frac\pi x\bar\alpha

void math_ns::add_to_trace(Token T)
{
  old_need = trace_needs_space;
  old_pos = Trace.size();
  uint x = T.get_val();
  if(trace_needs_space && T.cmd_val()==letter_catcode) Trace.push_back(' ');
  if(T.is_a_char() && T.char_val()=='\n') 
    Trace.push_back("^^J");
  else
    Trace.push_back(T);
  trace_needs_space= false;
  if(x>=cs_token_flag) trace_needs_space=true;
}
void math_ns::add_to_trace(const string& x)
{
  Trace.push_back_braced(x); trace_needs_space= false;
}

void math_ns::add_to_trace(char x)
{
  Trace.push_back(x); trace_needs_space= false;
}



Xmlp math_ns::get_builtin(int p) 
{ 
  return math_data.get_builtin(p); 
}

// -----------------------------------------------------------------------

// Interprets the meta data of the list
ostream& operator <<(ostream& X,  math_list_type y)
{
  switch(y) {
  case invalid_cd: X << "Invalid list"; break;
  case math_open_cd: X << "simple group";  break;
  case math_dollar_cd: X << "Inline formula"; break;
  case math_ddollar_cd: X << "Display formula"; break;
  case math_env_cd: X << "Environment"; break;
  case math_LR_cd: X << "Left/right pair"; break;
  case math_hbox_cd: X << "Hbox"; break;
  case math_argument_cd: X << "Argument"; break;
  default: {
    CmdChr x(special_math_cmd,math_to_sub(y));
    String w = x.name();
    if(w) X << "Argument list for \\" << w;
    else X << "impossible " <<  int(y); break;
  }
  }
  return X;
}

Logger& operator <<(Logger& X,  math_list_type y)
{
  *(X.fp) << y;
  return X;
}

Buffer& operator <<(Buffer& X,  math_list_type y)
{
  switch(y) {
  case invalid_cd: X << "Invalid list"; break;
  case math_open_cd: X << "Simple group";  break;
  case math_dollar_cd: X << "Inline formula"; break;
  case math_ddollar_cd: X << "Display formula"; break;
  case math_env_cd: X << "Environment"; break;
  case math_LR_cd: X << "Left/right pair"; break;
  case math_hbox_cd: X << "Hbox"; break;
  case math_argument_cd: X << "Argument"; break;
  default: {
    CmdChr x(special_math_cmd,math_to_sub(y));
    String w = x.name();
    if(w) X << "Argument list for \\" << w;
    else X << "cimpossible " <<  int(y); break;
  }
  }
  return X;
}

// Prints a list into the Trace buffer
void Math::print() const
{
  Trace << "{" << type;
  if(name.empty()) Trace << "\n"; else Trace << " name= " << name << "\n";
  if(empty()) Trace << "empty";
  else {
    int k=0;
    const_math_iterator C = value.begin();
    const_math_iterator E = value.end();
    while (C != E) {
      k++; Trace << k  << " ";
      C->print();
      ++C;
    }
  }
  Trace <<"}\n";
}


// This prints a math element
void MathElt::print() const
{
  int cmd = get_cmd();
  int chr = get_chr();
  if(cmd==nomath_cmd) {
    Trace << "only for " << (chr==zero_code ? "math\n" : "nomath\n");
      return;
  }
  Trace << "ME " << cmd << " - "  ;
  if(32<chr && chr < 128) Trace << "char " <<  uchar(chr) ;
  else Trace << chr ;
  if(cmd==mathfont_cmd || is_m_font (symcodes(cmd)))
    Trace << " - " <<  Token(get_font()) << "\n";
  else Trace << " - " << int(get_font()) << "\n";
  if(cmd==math_list_cmd ||cmd==special_math_cmd) get_list().print(); // recurse
}

// -----------------------------------------------------------------------
// Basic code generator

// General fence around val.
Xmlp MathDataP::make_mfenced(int open, int close, Xmlp val)
{
  Xmlp res = new Xml(cst_mfenced,0);
  res->add_att(np_close,xml_lr_ptable[close]);
  res->add_att(np_open,xml_lr_ptable[open]);
  bool single_elt = val->size() == 1;
  if(the_names[np_separator]==the_names[cst_mrow]) {
    if(!single_elt) 
      val = new Xml(cst_mrow,val);
  } else if (the_names[np_separator].empty() && single_elt) {}
  else res->add_att(cst_separators,np_separator);
  res->add_tmp(val);
  return res;
} 


// This adds a style element above res.
Xmlp MathDataP::add_style(int lvl, Xmlp res)
{
  if(lvl<0) return res; // special case
  res = new Xml(cst_mstyle,res);
  if(lvl==0) {
    res->add_att(cst_displaystyle, np_true);
    res->add_att(cst_scriptlevel,cst_dig0);
  } else {
    res->add_att(cst_displaystyle, np_false);
    res->add_att(cst_scriptlevel,name_positions(cst_dig0+lvl-1));
  }
  return res;
}

// Implements \cellattribute
void MathHelper::add_attribute(Istring a, Istring b, subtypes c)
{
  Xid w;
  if(c==cell_attribute_code) w = cur_cell_id;
  else if(c==row_attribute_code) w =cur_row_id;
  else if(c==table_attribute_code) w =cur_table_id;
  else if(c==thismath_attribute_code) w = cur_math_id;
  else if(c==formula_attribute_code) w = cur_formula_id;
  else return;
  w.add_attribute(a,b,true);
} 

void math_ns::add_attribute_spec(Istring a, Istring b)
{
  cmi.get_tid().add_attribute(a,b,true);
}


// Adds a label to the formula X
void Parser::add_math_label (Xmlp res)
{
  Istring my_id = the_main->SH.next_label_id();
  Istring T = the_stack.get_cur_id();
  the_stack.set_cur_id(my_id);
  res->get_id().get_att().push_back(np_id,my_id);
  if(the_tag.empty()) {
    static int mid =0;
    mid++;
    math_buffer << bf_reset << "mid" << mid;
    the_tag = math_buffer.to_string(); 
  }
  res->get_id().get_att().push_back(np_idtext,Istring(the_tag));
  const string& label = cmi.get_label_val();
  if(!label.empty()) add_id(label);
  the_stack.set_cur_id(T);
}


// Generates <elt>first_arg second_arg</elt>
Xmlp math_ns::xml2sons(Istring elt, Xmlp first_arg, Xmlp second_arg)
{
  Xmlp tmp= new Xml(elt,0);
  tmp->add_tmp(first_arg);
  tmp->push_back(xmlspace);
  tmp->add_tmp(second_arg);
  return tmp;
}

// As above, but if B1 is not empty, adds b1 as attribute with value b2.
Xmlp Stack::xml2_space(Istring elt, Istring b1, Istring b2,Xmlp f_arg, Xmlp s_arg)
{
  Xmlp tmp = new Xml(elt,0);
  if(!b1.null()) tmp->add_att(b1,b2);
  tmp->add_tmp(f_arg);
  tmp->push_back(xmlspace);
  tmp->add_tmp(s_arg);
  return tmp;
}

// As above, but if B1 is not empty, adds b1 as attribute with value true.
Xmlp Stack::xml2_space(Istring elt, Istring b1, Xmlp first_arg, Xmlp second_arg)
{
  Xmlp tmp = new Xml(elt,0);
  if(!b1.null()) tmp->add_att(b1,the_names[np_true]);
  tmp->add_tmp(first_arg);
  tmp->push_back(xmlspace);
  tmp->add_tmp(second_arg);
  return tmp;
}

// -----------------------------------------------------------------------
// Some trivial functions on lists

// This reallocates the math_table
void MathDataP::realloc_list()
{
  int k = 2*lmath_size;
  Math* T = new Math[k];
  for(int i=0; i<lmath_size; i++) T[i] = math_table[i];
  delete[] math_table;
  math_table = T;
  lmath_size = k;
  the_log << "Realloc math table to " << k << "\n";
}

// Makes sure there is enough place for two copies
void MathDataP::realloc_list0()
{
  int n= lmath_pos+1;
  if(n+n+n> lmath_size) realloc_list();
  if(n+n+n> lmath_size) realloc_list();
}

// This reallocates the xml_math_table
void MathDataP::realloc_xml()
{
  int k = 2*xmath_size;
  Xmlp* T = new Xmlp[k]; 
  for(int i=0;i<k;i++) T[i] = 0;
  for(int i=0;i<xmath_size; i++) T[i] = xml_math_table[i];
  delete[] xml_math_table;
  xml_math_table = T;
  xmath_size = k;
  the_log << "Realloc xml math table to " << k << "\n";
}

// Appends the list X at the end, and destroys X.
void Math::push_front(Math& X)
{
  value.splice(value.begin(), X.value);
}

void Math::destroy()
{
  name = "";
  value.clear();
}


// Adds X to the end of *this, and kills X.
void Math::push_back(Math& X)
{
  value.splice(value.end(), X.value);
}

Math& MathElt::get_list() const
{
  return math_data.get_list(get_chr());
}

Math& Math::get_list(int w) const
{
  return math_data.get_list(w);
}

// Adds a token to the list
void Math::push_back(CmdChr X, subtypes c)
{
  push_back(MathElt(X,c));
}

void Math::push_back_list(subtypes X, math_list_type c)
{
  push_back(MathElt(CmdChr(math_list_cmd,X),subtypes(c)));
}

void Math::push_back_font(subtypes X, subtypes c)
{
  push_back(MathElt(CmdChr(math_font_cmd,X),c));
}

// Adds a token to the list at position k
void MathDataP::push_back(int k, CmdChr X, subtypes c)
{
  get_list(k).push_back(MathElt(X,c));
}

// Adds a token to the list
void Math::push_front(CmdChr X, subtypes c)
{
  value.push_front(MathElt(X,c));
}

// Adds a character (cmd+chr). Uses current math font.
void Math::push_back(CmdChr X)
{
  subtypes font = subtypes(the_parser.eqtb_int_table[math_font_pos].get_val());
  push_back(MathElt(X,font));
}

// Add an xml element. If b>=0, this element is at position b in the table.
void Math::push_back(Xmlp A, int b, math_types c)
{
  push_back(MathElt(A,b,c));
}

MathElt::MathElt(Xmlp A, int b, math_types c)
{
  subtypes pos;
  if(b>=0) pos = subtypes(b);
  else pos = math_data.find_xml_location(A);
  val = CmdChr(math_xml_cmd,pos);
  set_xmltype(c);
}

// Add an xml element, of type small, to the list.
void Math::push_back_small(Xmlp A)
{
  subtypes pos = math_data.find_xml_location(A);
  push_back(MathElt(pos,mt_flag_small));
}

// Return true if this is an environment that produces an array.
bool Math::is_array_env() const
{
  if(is_array_ext()) return true;
  if(is_split_ext()) return true;
  if(is_aligned_ext()) return true;
  if(is_matrix_ext()) return true; 
  if(is_matrix()) return true;
  if(is_bordermatrix()) return true;
  return false;
}

// Returns the next style in the list D,T, S, SS.
// D and T give S, S gives SS, and SS gives SS.
// Knuth uses: 2*(x/4)+1+1 for sub,  2*(x/4)+1+ (x mod2) for sub
math_style math_ns::next_math_style(math_style x)
{
  switch(x) {
  case ms_T: 
  case ms_D: return ms_S;
  default: return ms_SS;
  }
}

// Knuth uses: x+2-2*(x/6)  for numerator x*(x/2)+1+2-2*(x/6) for denominator
math_style math_ns::next_frac_style(math_style x)
{
  switch(x) {
  case ms_D: return ms_T;
  case ms_T:  return ms_S;
  default: return ms_SS;
  }
}

// Takes the code of a command like \textstyle, returns ms_T
math_style math_ns::style_level(subtypes tt)
{
  if(tt==displaystyle_code)  return ms_D;
  else if(tt==textstyle_code) return ms_T;
  else if(tt==scriptstyle_code) return ms_S;
  else if(tt==scriptscriptstyle_code) return ms_SS;
  else return ms_T;
}

// ------------------------------------------------------------------

// This creates the two tables math_table and xml_math_table
void MathDataP::boot_table()
{
  lmath_size = 10;
  math_table = new Math[lmath_size];
  xmath_size = 10;
  xml_math_table = new Xmlp[xmath_size];
  for(int i=0;i<xmath_size;i++) xml_math_table[i] = 0;
  xmath_pos = 0;
  lmath_pos = 0;
}

inline Xmlp MathElt::get_xml_val() const
{
  return math_data.get_xml_val(get_chr());
}

// --------------------------------------------------------------------

// This is called before every math formula.
void MathHelper::reset(bool dual)
{
  free_list = 0;
  current_mode = false;
  pos_att = cst_dig0;
  seen_label = false;
  warned_label = false;
  label_val = "";
  tag_list = TokenList();
  is_tag_starred = false;
  math_env_ctr = 0;
  all_env_ctr = 0;
  cur_math_id = the_main->the_stack->next_xid(0);
  cur_formula_id = the_main->the_stack->next_xid(0);
  if(dual) cur_texmath_id = the_main->the_stack->next_xid(0);
  else cur_texmath_id = cur_math_id;
}

// Sets the type (display or not), and prepares the pos attribute.
void MathHelper::set_type(bool b)
{
  current_mode = b;
  pos_att = b ? np_inline : np_display;
}

// This finds a free position in the table of math lists.
// Note: the return value is never zero.
subtypes MathDataP::find_math_location(math_list_type c)
{
  lmath_pos++;
  int res = lmath_pos;
  if(res>=lmath_size) realloc_list();
  math_table[res].set_type(c);
  return subtypes(res);
}

// This finds a free position in the xml table.
// Note that there is a free position in case of overflow (see next function)
subtypes MathDataP::find_xml_location()
{
  xmath_pos++;
  if(xmath_pos>=xmath_size) realloc_xml();
  return subtypes(m_offset+xmath_pos-1);
}

// This finds a free position, puts Y in it, and returns the position.
subtypes MathDataP::find_xml_location(Xmlp y)
{
  xml_math_table[xmath_pos] = y;
  return find_xml_location();
}

MathElt::MathElt(Xmlp x, math_types y) :
  val(CmdChr(math_xml_cmd,zero_code))
{ 
  set_xmltype(y);
  set_chr(math_data.find_xml_location(x));
}


// Destroys all math lists, and resets pointers, 
// for use with another math object
void MathHelper::finish_math_mem()
{
  math_data.finish_math_mem();
  free_list = 0;
}

// This kills the math elements
void MathDataP::finish_math_mem()
{
  for(int i=0;i<=lmath_pos;i++)
    math_table[i].destroy();
  lmath_pos = 0; 
  xmath_pos = 0;
}

// True if this is a digit with category 12
bool MathElt::is_digit() const 
{
  return get_cmd()==12 && val.is_digit();
}

// If we see something like {} in a math formula, say cur_math,
// we open a group, create a new mathlist res, add it to cur_math.
// Note that the type c appears both as type of res, and a field of cur_math

subtypes Parser::new_math_list(int cur_math, math_list_type c, string s)
{
  subtypes k = math_data.find_math_location(c);
  math_data.get_list(cur_math).push_back_list(k,c);
  math_data.get_list(k).set_name(s);
  scan_math3(k,c,true);
  return k;
}

// This reads a character (delimiter) after a \left or a \right.
del_pos Parser::math_lr_value()
{
  remove_initial_space_relax();
  add_to_trace(cur_tok);
  del_pos k = get_delimiter(cur_cmd_chr);
  if(k != del_invalid) return k;
  parse_error("Invalid character in \\left or \\right\n",
	      cur_tok,"","bad left/right");
  return del_dot;
}


// This is done to the main math list U when we start reading a formula.
// Second argument is -1 if a dollar has been read, 
// the subtype of the token otherwise;
// Note: \( and \[ expand to $$, in all other cases we have an environment
// for instance \begin{math} or \begin{equation}. We compute and insert
// the name of the environment. 
// Sets and returns the inline/non-inline flag

bool Parser::start_scan_math(Math&u, int type)
{
  if(type==-1) { // case of $...
    add_to_trace(cur_tok);
    get_token();// no expansion
    if(cur_cmd_chr.get_cmd()==3) {
      add_to_trace(cur_tok);
      u.set_display_type();
      return false;
    } else {
      if(cur_tok != hash_table.relax_token)
	back_input();
      u.set_nondisplay_type();
      return true;
    }
  } else { // case of \begin{...}
    u.set_type(math_env_cd);
    u.set_env_name(type);
    //  if(type==align_code) back_input(hash_table.at_align_token);
    return type==math_code;
  }
}

// This function returns true if an equation number is required
void MathHelper::check_for_eqnum(int type)
{
  need_tag = type==align_code ||  type==multline_code ||
    type==gather_code ||
    type == eqnarray_code ||type == Beqnarray_code || type == equation_code;
}

// This is called after parsing is complete. The argument is the
// same as above. We change the type, so that has_type(math_env_cd) returns 
// true only if the argument is in the list shown here, i.e.,
// if the math formula is a table.
void Math::hack_type(int type)
{
  if(type==align_code || type==align_star_code ||
       type==multline_code ||type==multline_star_code || 
       type==gather_code ||type==gather_star_code || 
       type == eqnarray_code || type==eqnarray_star_code ||
       type == Beqnarray_code || type==Beqnarray_star_code)
    return;
   set_type(math_open_cd);
}

// This piece of code is executed when the math formula has been read
// and translated. If the formula is inline, we execute \leavevmode
// Otherwise, it is a display math formula, we finish the current paragraph
// and look at the next (expanded) token. If it is no \par, we insert
// a \noindent.
void Parser::after_math (bool is_inline)
{
  cmi.finish_math_mem(); 
  if(is_inline)
    leave_v_mode();
  else if(the_stack.is_frame(np_fbox)) return;
  else { 
    bool w = the_stack.in_h_mode() || the_stack.in_v_mode();
    leave_h_mode();
    if (the_main->is_interactive_math()) return; 
    remove_initial_space_and_back_input();
    if(w&&cur_cmd_chr.get_cmd() != par_cmd)
      back_input(hash_table.noindent_token);
  }
}

// This is called if no MathML should be generated.
void Parser::finish_no_mathml(bool is_inline, int vp)
{
  Math& u = math_data.get_list(vp);
  Xid id = cmi.get_mid();
  string S = u.get_real_name();
  Istring s = Istring(S);
  if( S != u.get_name()) u.pop3();
  if(S.empty()) s = Istring(is_inline ? np_inline : np_display);
  id.add_attribute(np_type,cmi.get_pos_att());
  id.add_attribute(np_textype, s);
  Xmlp res = u.convert_math_noML(cmi.get_pos_att(),
				 eqtb_int_table[nomath_code].get_val()==-2);
  res->change_id(id);
  if (the_main->is_interactive_math())  cout << res << "\n";
  after_math(is_inline);
  the_stack.top_stack()->push_back(res);
  if(cmi.has_label()) add_math_label(res);
  if(!is_inline) the_stack.add_nl();
}


// Case of a trivial math formula that translates to res
// Always inline
void Parser::finish_trivial_math(Xmlp res)
{
  if(tracing_math()) 
    the_log << lg_start  << "formula was math" << lg_end;
  the_parser.my_stats.one_more_trivial();
  if(the_main->is_interactive_math())  cout << res << "\n"; 
  leave_v_mode();
  cmi.finish_math_mem(); 
  the_stack.top_stack()->add_tmp(res);
}


// Needed for implementation of \ifinner
bool Parser::is_inner_math() 
{
  return cmi.is_inline();
}

// Toplevel function. Reads and translates a formula. Argument as in
// start_scan_math
void Parser::T_math (int type)
{
  int nm = eqtb_int_table[nomath_code].get_val();
  cmi.reset(nm == -3);
  Trace.reset();
  trace_needs_space = false;
  Math&u1= math_data.get_list(0);
  bool is_inline = start_scan_math(u1,type);
  cmi.set_type(is_inline);
  cmi.check_for_eqnum(type);
  if(cmi.need_eqnum()) 
    back_input(hash_table.increqnum_token);
  // Insert the everymath token list
  int position = is_inline ? everymath_code : everydisplay_code;
  TokenList everymath = toks_registers[position].get_val();
  if(!everymath.empty()) {
    if(tracing_commands()) 
      the_log << lg_startbrace 
	      << (is_inline ? "<everymath> " : "<everydisplay> ")
	      << everymath << lg_endbrace;
    back_input(everymath);
  }
  select_math_font();
  scan_math3(0,math_data.get_list(0).get_type(),true);
  if(tracing_math())
    the_log << lg_start  << "Math: " << Trace << lg_end << lg_flush;
  //  if(cmi.need_eqnum()) u.remove_initial_group();
  if(tracing_math()) {
    Trace.reset();
    math_data.get_list(0).print();
    the_log << lg_start  << Trace << lg_flush;
  }
  // Test for the no-mathml mode
  math_data.realloc_list0();
  int loc_of_cp = 0;
  if(nm <0) {
    loc_of_cp = math_data.get_list(0).duplicate(true);
    if(nm != -3) {
      finish_no_mathml(is_inline,loc_of_cp);
      return;
     }
  }
  Xmlp alter = 0;
  string textype =   math_data.get_list(0).get_real_name();
  if(nm == -3) {
    Math& w = math_data.get_list(loc_of_cp);
  if(textype != w.get_name()) w.pop3();
    alter = w.convert_math_noML(cmi.get_pos_att(), false);
    alter->change_id(cmi.get_tid());
  }
  loc_of_cp = math_data.get_list(0).duplicate(false);
  Math& u = math_data.get_list(loc_of_cp);
  // Translate the formula into res
  Xmlp res=0;
  u.hack_type(type);
  if(u.has_type(math_env_cd)) { 
    res = u.M_array(is_inline? ms_T: ms_D);
  } else {
    u.remove_spaces();
    if(is_inline) {
      int k = eqtb_int_table[notrivialmath_code].get_val();
      k = k%8;
      res = u.trivial_math(k);
      if(res) { 
	finish_trivial_math(res);
	return;
      }
    }
    res = u.convert_math(is_inline? ms_T: ms_D);
  }
  after_math(is_inline);
  // Insert the result in the tree.
  Xmlp x = new Xml(cst_math,0);
  x->change_id(cmi.get_mid());
  x->add_att(cst_xmlns,cst_mathml);
  x->add_tmp(res);
  if(!is_inline) x->add_att(cst_mode,cst_display);
  Xmlp res1 = new Xml(np_formula,x);
     if (alter) res1-> push_back(alter);

  res1->change_id(cmi.get_fid());
  res1->add_att(np_type,cmi.get_pos_att());
  if(!textype.empty())
    res1->add_att(np_textype, Istring(textype));
  if(cmi.has_label()) add_math_label(res1);
  if(the_main->is_interactive_math()) {
    if(only_input_data)
      cout << x << "\n";
    else 
      cout << res1 << "\n";
  }
  the_stack.add_last(res1);
  if(!is_inline) the_stack.add_nl();
}

// --------------------------------------------------
// Scanning math

// x is index of result, t is type of math list, m is false in \hbox
void Parser::scan_math3(int x, math_list_type t, bool m)
{
  Token xfct_caller = fct_caller;
  fct_caller = hash_table.relax_token;
  mode om = the_stack.get_mode();
  if(t==math_open_cd) {} // do not change the mode
  else if(m)
    the_stack.set_m_mode();
  else
    the_stack.set_h_mode();
  scan_math2(x,t);
  the_stack.set_mode(om);
  fct_caller = xfct_caller;
}
// Like scan_math, but push and pop a level. When we read an array,
// we pop&push whenever a cell boundary is seen; in this case the push here
// will have type bt_cell, instead of type bt_math. 
void Parser::scan_math2(int U, math_list_type t)
{
  boundary_type aux = bt_math;
  Math u = math_data.get_list(U);
  if(u.is_array_ext() || u.is_split_ext() || u.is_matrix_ext()
     || u.is_matrix() || u.is_bordermatrix()
     || u.is_aligned_ext() || u.is_external_array())
    aux = bt_cell;
  push_level(aux);
  scan_math(U,t);
  pop_level(true,aux); // beurks
}

void MathHelper::add_tag (TokenList&L)
{
  if(!tag_list.empty()) tag_list.push_back(the_parser.hash_table.comma_token);
  tag_list.splice(tag_list.end(),L); 
}


void MathHelper::handle_tags()
{
  TokenList L = tag_list;
  token_ns::remove_first_last_space(L);
  the_parser.brace_me(L);
  L.push_front(is_tag_starred ?the_parser.hash_table.ytag1_token :
	       the_parser.hash_table.xtag1_token);
  the_parser.back_input(L);
  tag_list = TokenList();
}

// Reads a new token.
// Return  0 if end-of data, 1 if token should be ignored, 2 otherwise
int Parser::scan_math1(int res)
{
  get_x_token();
  if(cur_tok.is_invalid()) {
    parse_error("End of input reached");
    return 0;
  }
  symcodes T = cur_cmd_chr.get_cmd();
  if(T==nobreakspace_cmd) {
    back_input(Token(other_t_offset,'~'));
    return 1;
  }
  // replace \{ by \lbrace
  if(T==cst1_cmd &&cur_cmd_chr.get_chr()==rbrace_chr)  {
    back_input(hash_table.rbrace_token); 
    return 1;
  }
  if(T==cst1_cmd &&cur_cmd_chr.get_chr()==lbrace_chr)  {
    back_input(hash_table.lbrace_token); 
    return 1;
  }
  if(T==3) { // dollar sign hack a bit this
    if(cmi.has_tag()) {
      back_input(Token(dollar_t_offset,'$'));
      cmi.handle_tags();
      return 1;
    }
  }
  if(T==11 || T==12 || T==char_given_cmd) {
    if(the_stack.get_mode() == mode_math) {
      int c = cur_cmd_chr.get_chr();
      if(c>0 && c<int(nb_characters)) {
	int u = eqtb_int_table[c+math_code_offset].get_val();
	if(u==32768) {
	  cur_tok.from_cs(c+active_base);
	  look_at_mac();
	  back_input();
	return 1;
	}
      }
    }
  }
  if(is_m_font(T)) {
    if(eqtb_int_table[nomath_code].get_val()== -2) {
      math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
      return 1;
    }
    math_data.push_back(res,CmdChr(nomath_cmd,one_code),zero_code);
    math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
    TM_fonts();
    return 1;
  }
  add_to_trace(cur_tok);
  return 2;
}

// The function that reads a math formula. Read tokens until the end
// of the current group is seen. Fills the list number res, of type type
void Parser::scan_math(int res, math_list_type type)
{
  string s="";
  for(;;) {
    int w = scan_math1(res);
    if(w==0) return;
    if(w==1) continue;
    symcodes T = cur_cmd_chr.get_cmd();
    subtypes c = cur_cmd_chr.get_chr();
    Token t = cur_tok;
    switch(T) {
    case 1: // open brace, read a group
      new_math_list(res,math_open_cd,"");
      continue;
    case 2: // close brace, end group
      if(type==math_open_cd || type==math_argument_cd || type==math_hbox_cd) 
	return; 
      parse_error("Unexpected }");
      return;
    case 3: // dollar, open or close ?
      if(scan_math_dollar(res,type)) return;
      continue;
    case par_cmd:
      err_ns::local_buf << bf_reset << "Unexpected \\par";
      if(type==math_argument_cd) 
	err_ns::local_buf << " while scanning argument of "
			  << fct_caller.tok_to_str();
      err_ns::signal_error("Unexpected par",0);
      return;
    case eqno_cmd:
      scan_eqno(type);
      continue;
    case tag_cmd: 
      scan_math_tag(c);
      continue;
    case label_cmd:
      s = special_next_arg();
      cmi.new_label(s);
      continue;
    case ensuremath_cmd: 
      first_of_one(t);
      continue;
    case begingroup_cmd:
      if(c==0) push_level(bt_semisimple);
      else pop_level(false,bt_semisimple);
      continue;
    case begin_cmd:
    case end_cmd:
      if(scan_math_env(res,type)) return;
      continue;
    case left_cmd:
      {
	del_pos k = math_lr_value();
	int tmp = new_math_list(res,math_LR_cd,"");
	Math& w = math_data.get_list(tmp);
	w.push_front(CmdChr(T,subtypes(k)),zero_code);
	if(w.back().get_cmd() != right_cmd) {
	  parse_error("Missing \\right. inserted");
	  w.push_back(CmdChr(right_cmd,subtypes(del_dot)),zero_code);
	}
	continue;
      }
    case right_cmd:
      {
	del_pos k = math_lr_value();
	if(type == math_LR_cd) {
	  math_data.get_list(res).push_back(CmdChr(right_cmd,subtypes(k)));
	  return;
	} else {
	  parse_error("Unexpected \\right");
	  continue;
	}
      }
    case special_math_cmd:
      interpret_math_cmd(res,c);
      continue;
    case multicolumn_cmd:
      interpret_math_cmd(res,multicolumn_code);
      continue;
    case specmath_cmd: // Tralics commands like \mathmi
      interpret_math_cmd(res,c);
      continue;
    case make_box_cmd:
      if(c==hbox_code) scan_math_hbox(res);
      else 
	math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
      continue;
    case fbox_cmd:
      if(c==fbox_code) scan_math_hbox(res);
      else 
	math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
      continue;
    case box_cmd:
      scan_hbox(res);
      continue;
    case 4:    // case & and \\ in a table 
    case backslash_cmd:
      math_data.push_back(res,cur_cmd_chr,subtypes(t.get_val())); // ok ??
      pop_level(false,bt_cell); // start-end a group for the next cell
      push_level(bt_cell);
      continue;
    case mathfont_cmd:
      math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
      word_define(math_font_pos,c,false);
      continue;
    case undef_cmd:
      undefined_mac();
      continue;
    case mathrel_cmd:
      scan_math_rel(c,res);
      continue;
    case unimp_cmd:
      T_unimp(c);
      continue;
    case specimp_cmd:
      if(c==allowbreak_code) continue;
      T_specimp(c);
      continue;
    case kern_cmd:
    case scan_glue_cmd:
    case hspace_cmd:
      {
	ScaledInt val = scan_math_kern(T,c);
	CmdChr w = CmdChr(hspace_cmd,c);
	math_data.push_back(res,w,subtypes(val.get_value()));
	continue;
      }
    case start_par_cmd:
      // no error ?
      continue;
    case char_num_cmd: 
      { 
	int C = scan_27bit_int();
	CmdChr w  =CmdChr(char_given_cmd,subtypes(C));
	math_data.push_back(res,w,subtypes(0));
      }
      continue;
    case char_given_cmd:
    case math_given_cmd:
      math_data.push_back(res,cur_cmd_chr,subtypes(0));
      continue;
    case relax_cmd:
      math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
       continue;
    case mathord_cmd: //may be bold
      {
	subtypes F = subtypes(cur_tok.get_val());
	CmdChr R = cur_cmd_chr;
	if(math_loc(c)>first_inline_hack && math_loc(c)<last_inline_bhack 
	   &&is_pos_par(atmathversion_code))
	  R = CmdChr(mathordb_cmd,c);
	math_data.push_back(res,R,F);
	continue;
      }
    default:
      if(T==12 && c=='\'' && type != math_hbox_cd) {
	Trace.remove_last_quote();
	scan_prime();
	continue;
      }
      if(T>=first_mode_independent) {
	remove_from_trace();
	translate01();
	continue;
      }
      if(T<16 && cur_tok.not_a_cmd()) {
	// in the case \let\A=B, we replace \A by B (avoids core dump)
	cur_cmd_chr = CmdChr(T,c);
      }
      if(T<16) { // Case of a character
	subtypes font = subtypes(the_parser.eqtb_int_table[math_font_pos].get_val());
	math_data.push_back(res,cur_cmd_chr,font);
      }
      else
	math_data.push_back(res,cur_cmd_chr,subtypes(cur_tok.get_val()));
    }
  }
}


// We have seen \begin or \end
// return false if parsing continues, true otherwise (case of \end)
bool Parser::scan_math_env(int res, math_list_type type)
{
  symcodes T = cur_cmd_chr.get_cmd();
  bool at_start = T==begin_cmd;
  cmi.update_all_env_ctr(at_start);
  string s = get_env(); // name of the env
  add_to_trace(s);
  if(is_not_a_math_env(s.c_str())) { // Check if \endfoo defined
    if(at_start) {
      find_env_token(s.c_str(),true);
      back_input();
      back_input(hash_table.OB_token); // Insert OB
      cmi.update_math_env_ctr(true);
    } else { // end something
      if(cmi.get_math_env_ctr()>0) {
	back_input(hash_table.CB_token); // matches the OB above
	cmi.update_math_env_ctr(false);
      } else {
	T_end(s); return false;
      }
      find_env_token(s.c_str(),false);
      back_input();
      if(cmi.get_math_env_ctr() ==0 && s=="align") ;
    }
    return false;
  }
  if(at_start) {
    new_math_list(res,math_env_cd,s);
    return false;
  }
  bool  at_level_zero =
      (s== "@align" ? cmi.get_all_env_ctr()==-2 : cmi.get_all_env_ctr()==-1);
  //  bool  at_level_zero = cmi.get_all_env_ctr()==-1; old code
  if(at_level_zero && cmi.has_tag()) {
    TokenList L = token_ns::string_to_list(s,true);
    back_input(L);
    back_input(hash_table.end_token);
    cmi.handle_tags();
    return false;
  }
  if(type == math_env_cd &&  s==math_data.get_list(res).get_name()) {
    if(res==0) {
      the_tag = the_parser.eqtb_string_table[0].get_val();
    }
    return true;
  }
  parse_error("Bad \\end{" + s + "}");
  return true;
}

// We have seen a dollar sign, expected closing delimiter type
// Returns  true if Ok, false if parsing continues
bool Parser::scan_math_dollar(int res,math_list_type type)
{
  switch(type) {
  case math_dollar_cd:
    return true;
  case math_ddollar_cd: // We want $$ or equivalent
    get_x_token();  
    add_to_trace(cur_tok);
    if(cur_cmd_chr.get_cmd()!=3) 
      parse_error("Display math should end with $$");
    return true;
  case math_open_cd:
    if(!the_stack.in_h_mode()) {
      parse_error("Out of scope $ ignored, maybe a } is missing here","extra $");
      return false;  
    }
    // fall through
  case math_hbox_cd: {
    // it's a math formula inside a formula
    TokenList everymath = toks_registers[everymath_code].get_val();
    if(!everymath.empty()) {
      if(tracing_commands()) 
	the_log << lg_startbrace  << "<every...> " 
		<< everymath << lg_endbrace;
      back_input(everymath);
    }
    select_math_font();
    new_math_list(res,math_dollar_cd,"");
    return false;
  } 
  case math_LR_cd:
    back_input(); 
    parse_error("Missing \\right. inserted","missing \\right");
    math_data.get_list(res).push_back(CmdChr(right_cmd,subtypes(del_dot)));
    return true;
  case math_argument_cd:
    parse_error("Extra $ ignored while scanning argument of ",
			   fct_caller,"","extra $");
    return false;
  default:
    parse_error("Extra $ ignored...");
    return false;
  } 
}

// We have seen \tag, \@xtag or \@ytag
void Parser::scan_math_tag(subtypes c)
{
  bool is_star = false; 
  if(c==0) is_star = remove_initial_star();
  TokenList L = mac_arg();
  if(c==0) {
    brace_me(L);
    L.remove_if(MathIsDollar());
    L.push_front(is_star ? hash_table.ytag_token : hash_table.xtag_token);
    back_input(L);
  } else {
    if(c==2) cmi.starred_tag();
    cmi.add_tag(L);
  }
}

// Scans the arguments of \eqno; interprets \char`\^
void Parser::scan_eqno(math_list_type type)
{
  if(type != math_ddollar_cd) {
    parse_error("Command \\eqno allowed only in display math","bad \\eqno");
    return;
  }
  name_positions w = cur_cmd_chr.get_chr()==leqno_code? np_left : np_right;
  cmi.get_fid().add_attribute(np_eqnpos,w,true);
  TokenList L;
  int balance =0;
  for(;;) {
    get_x_token();  
    if(cur_tok.is_invalid()) {
      parse_error("End of input reached while scanning \\eqno");
      return;
    }
    if(cur_cmd_chr.get_cmd()== char_num_cmd) {
      int C = scan_char_num();
      cur_tok = Token(other_t_offset,C);
      back_input();
      continue;
    }
    if(cur_cmd_chr.get_cmd()==12 && cur_cmd_chr.get_chr() =='\'')
      cur_tok = hash_table.apostrophe_token;
    check_brace(balance);
    if(balance==0 && cur_cmd_chr.get_cmd() == 3) break;
    L.push_back(cur_tok);
  }
  back_input();
  brace_me(L);
  L.push_front(hash_table.ytag_token);
  back_input(L);
}
  
// Case of a \kern, or something like that. We scan the value, convert it
// in pt.
ScaledInt Parser::scan_math_kern(symcodes T, subtypes& c)
{
  ScaledInt value =0;
  bool is_mu = false;
  Token ct = cur_tok;
  if(T==kern_cmd) {
    is_mu = c==1;
    c=zero_code;
    scan_dimen(is_mu,ct);
    value = cur_val.get_int_val();
  } else if(T==hspace_cmd) {
    if(remove_initial_star()) c = subtypes(c+2);
    scan_glue(it_glue,ct,false);
    value = cur_val.get_glue_width();
  } else {
    is_mu = c==mskip_code;
    if(c==vskip_code) c = one_code; else c = zero_code; 
    scan_glue(is_mu ? it_mu: it_glue, ct);
    value = cur_val.get_glue_width();
  }
  if(is_mu) value.times_10_18();
  return value;
}

// Case of a \not\in read as \notin, \not= read as \ne
// What should we do in other cases?
void Parser::scan_math_rel(subtypes c, int res)
{
  subtypes w = subtypes(cur_tok.get_val());
  if(c ==subtypes(not_code)) {
    Token not_token = cur_tok;
    get_x_token();
    if(cur_tok.is_invalid()) {}
    else if(cur_tok.not_a_cmd() && cur_cmd_chr.get_chr()=='=') {
      add_to_trace(cur_tok);
      cur_cmd_chr.set_cmd_chr(mathrel_cmd,subtypes(ne_code));
      w = subtypes(not_token.get_val());
    } else if(cur_cmd_chr.get_cmd()==mathbin_cmd 
	      && cur_cmd_chr.get_chr()==subtypes(in_code)) {
      add_to_trace(cur_tok);
      cur_cmd_chr.set_cmd_chr(mathbin_cmd,subtypes(notin_code));
      w = subtypes(not_token.get_val());
    } else {
      back_input();
      cur_tok = not_token;
      cur_cmd_chr.set_cmd_chr(mathord_cmd,subtypes(not_code));
    }
  }
  math_data.push_back(res,cur_cmd_chr,w);
}

// Case of a \hbox; like \mbox, but inserts \everyhbox tokens
void Parser::scan_math_hbox(int res)
{
  TokenList L = toks_registers[everyhbox_code].get_val();
  if(!L.empty()) {
    if(before_mac_arg(cur_tok)) back_input(hash_table.CB_token);; 
    if(tracing_commands()) 
      the_log << lg_startbrace  << "<every...> " << L << lg_endbrace;
    back_input(L);
    back_input(hash_table.OB_token);
  }
  scan_hbox(res);
}

// Scans a mbox or a hbox
void Parser::scan_hbox(int ptr) 
{
  Buffer&B=math_buffer;
  B.reset(); B << cur_tok;
  if(B[0]!='\\') { B.reset(); B<< "\\hbox"; } 
  string s = B.c_str(1);
  if(before_mac_arg(cur_tok)) back_input(hash_table.CB_token);
  add_to_trace('{');
  subtypes k = math_data.find_math_location(math_hbox_cd);
  math_data.get_list(k).set_name(s);
  scan_math3(k,math_hbox_cd,false);
  math_data.push_back(ptr,CmdChr(math_list_cmd,k),subtypes(math_hbox_cd));
}

// Scans an argument of a procedure.
// Case w=0 or w = 1: creates a scope
// case w=1 or w = 3: scans a relax
// Same command with relax hacking
// Changes in 2.10.9: always create a scope
subtypes Parser::math_argument(int w,Token t) 
{
  Token xfct_caller = fct_caller;
  fct_caller = t;
  if(w&1) {
    remove_initial_space_relax();
    if(!cur_tok.is_invalid()) back_input();
  }
  if(before_mac_arg(t))  back_input(hash_table.CB_token);
  add_to_trace('{');
  subtypes k = math_data.find_math_location(math_argument_cd);
  boundary_type aux = bt_math;
  push_level(aux);
  scan_math(k,math_argument_cd);
  pop_level(true,aux);
  fct_caller = xfct_caller;
  return k;
}

// This reads a number and returns \displaystyle, \textstyle
// scriptstyle \scriptscriptstyle if the result is 0 1 2 3 
// It returns \relax otherwise. Should we call scanint ?
Token Parser::scan_style()
{
  TokenList L = mac_arg();
  Token t = token_ns::get_unique(L);
  int p = 4;
  if(t.cmd_val()==other_catcode) {
    int tt = t.val_as_digit();
    if(tt>= 0 && tt<= 3)
      p = tt;
  }
  return hash_table.genfrac_mode[p];
}

// This parses something like 
// \genfrac (){0pt}3{foo}{bar}
void Parser::interpret_genfrac_cmd(int res,subtypes k,CmdChr W) 
{
  Token ct = cur_tok;
  del_pos k1,k2;
  TokenList L1 = mac_arg();
  if(L1.empty())
    k1 = del_dot;
  else { back_input (L1); k1 = math_lr_value(); }
  TokenList L2 = mac_arg();
  if(L2.empty())
    k2 = del_dot;
  else { back_input (L2); k2 = math_lr_value(); }
  TokenList L3 = mac_arg();
  int dmres = 0;
  if(!L3.empty()) {
    back_input (L3);
    scan_dimen(false,ct); 
    dmres = cur_val.get_int_val();
    Buffer& B = the_main->SH.shbuf();
    B.reset();
    Trace.push_back(ScaledInt(dmres),glue_spec_pt);
    B.push_back(ScaledInt(dmres),glue_spec_pt);
    dmres = the_main->SH.hash_find();
  }
  Token m = scan_style();
  add_to_trace(m);
  subtypes r1 = math_argument(0,ct);
  subtypes r2 = math_argument(0,ct);
  //  subtypes k = math_data.find_math_location(sub_to_math(genfrac_code));
  Math& u = math_data.get_list(k);
  u.push_back(CmdChr(left_cmd,subtypes(k1)),zero_code);
  u.push_back(CmdChr(right_cmd,subtypes(k2)),zero_code);
  u.push_back(CmdChr(right_cmd,subtypes(dmres)),zero_code);

  token_from_list(m);
  cur_tok = m;
  u.push_back(cur_cmd_chr,subtypes(cur_tok.get_val()));
  u.push_back_list(r1,math_argument_cd);
  u.push_back_list(r2,math_argument_cd);
  math_data.push_back(res,W,subtypes(u.get_type()));
}

// Handles \mathmi[foo][bar][a][b]{etc} and friends
void Parser::scan_math_mi(int res,subtypes c,subtypes k,CmdChr W) 
{
  Token ct = cur_tok;
  string s = "";
  if(c ==mathbox_code) {
    s = group_to_string(); // name of the env (no expand here ?)
    add_to_trace(s); 
  }
  vector<MathElt> T; // the optional arguments
  for(;;) {
    remove_initial_space_and_back_input(); 
    if(!cur_tok.is_open_bracket()) break; 
    TokenList L; next_optarg(L);
    brace_me(L);
    back_input(L); 
    subtypes r1 = math_argument(0,ct); 
    T.push_back(MathElt(CmdChr(math_list_cmd,r1),subtypes(math_argument_cd)));
  }
  int n = T.size();
  n = n/2; n = n+n; // Ignore last if odd
  subtypes r1 = math_argument(0,ct); 
  Math& u = math_data.get_list(k);
  if(c==mathbox_code)  u.set_name(s);
  u.push_back_list(r1,math_argument_cd); 
  for(int i=0;i<n;i++) u.push_back(T[i]);
  math_data.push_back(res, W, subtypes(u.get_type()));
}

// Case of \mathchoice{}{}{}{}
void Parser::interpret_mathchoice_cmd(int res,subtypes k,CmdChr W) 
{
  Token ct = cur_tok;
  subtypes r1 = math_argument(1,ct);
  subtypes r2 = math_argument(1,ct);
  subtypes r3 = math_argument(1,ct);
  subtypes r4 = math_argument(1,ct);
  Math& u = math_data.get_list(k);
  u.push_back_list(r1,math_argument_cd);
  u.push_back_list(r2,math_argument_cd);
  u.push_back_list(r3,math_argument_cd);
  u.push_back_list(r4,math_argument_cd);
  math_data.push_back(res, W, subtypes(u.get_type()));
}

// This replaces [foo] by {foo}
void Parser::opt_to_mandatory()
{
  TokenList L;
  next_optarg(L);
  brace_me(L);
  back_input(L);
}

// Scans a command with some arguments.
void Parser::interpret_math_cmd(int res,subtypes c) 
{
  Token ct = cur_tok;
  subtypes k = math_data.find_math_location(sub_to_math(c));
  CmdChr W = CmdChr(special_math_cmd,k);
  switch(c) {
  case genfrac_code:
    interpret_genfrac_cmd(res,k,W);
    return;
  case mathchoice_code:
    interpret_mathchoice_cmd(res,k,W);
    return;
  case sqrt_code:
    remove_initial_space_and_back_input();
    if(cur_tok.is_open_bracket()) {
      c = root_code; 
      opt_to_mandatory();
    }
    break;
  case xleftarrow_code:
  case xrightarrow_code: 
    opt_to_mandatory();
    break;
  case smash_code:
  case cfrac_code:
    opt_to_mandatory();
    break;
  case mathmi_code: 
  case mathmo_code:
  case mathmn_code:
  case mathnothing_code:
  case mathci_code:
  case mathcn_code:
  case mathcsymbol_code:
  case multiscripts_code:
  case mathbox_code:
    scan_math_mi(res,c,k,W);
    return;
  case operatorname_code:
    if(remove_initial_star())
      c = operatornamestar_code;
    break;
  default:; // nothing special 
  }
  math_data.set_type(k,sub_to_math(c)); //may be needed in some cases
  // bool is_math_accent = c>=first_maccent_code && c<=last_maccent_code;
  // int first_spec = is_math_accent?1:0; 
  // if(c==sqrt_code) first_spec = 1;
  //  subtypes r1 = math_argument(first_spec,ct);
  subtypes r1 = math_argument(0,ct);
  subtypes r2 = zero_code;
  subtypes r3 = zero_code;
  if(c==cfrac_code || c==qopname_code || c==multicolumn_code) {
    r2 = math_argument(0,ct);
    r3 = math_argument(0,ct);
  } else if(c<=last_marg_code || c>last_maccent_code)
    r2 = math_argument(0,ct);
  else if(c==xleftarrow_code || c==xrightarrow_code || c==smash_code) 
    r2 = math_argument(0,ct);
  Math& u = math_data.get_list(k);
  u.push_back_list(r1,math_argument_cd);
  if(r2)
    u.push_back_list(r2,math_argument_cd);
  if(r3)
    u.push_back_list(r3,math_argument_cd);
  math_data.push_back(res, W, subtypes(u.get_type()));
}

// --------------------------
// Math array handling
void Math::skip_initial_space()
{
  while(!empty() && front().is_space()) pop_front();
}

// Creates a table for array specifications.
void StrHash::rlc_to_string(String s,vector<AttList>&res)
{
  int i = 0;
  while(s[i]) {
    char c = s[i];
    ++i;
    if(c!='r' && c!='l' && c!='c') continue;
    AttList L;
    if(c=='l') L.push_back(np_columnalign,np_left);
    else if(c=='r') L.push_back(np_columnalign,np_right);
    res.push_back(L);
  }
}

// Tries to find the name of an environment; especially in the case of @align.
string Math::get_real_name() const
{
  string u = get_name ();
  if(!has_type(math_env_cd)) return u;
  bool st = true;
  if(is_align_ext()) st = false;
  else if(is_align_ext_star()) st = true;
  else  return u;
  string a = remove_req_arg_noerr();
  if(a=="1") u  = "align";
  if(a=="2") u  = "flalign";
  if(a=="3") u  = "alignat";
  if(a=="4") u  = "xalignat";
  if(a=="5") u  = "xxalignat";
  if(st) u  = u + "*";
  return u;
}

// This pops two elements, since the optional star in now in the type
void Math::pop3()
{
  while(!empty() && front().is_space()) pop_front();
  if(!empty()) pop_front(); // the type
  // while(!empty() && front().is_space()) pop_front();
  //  if(!empty()) pop_front(); // optional star
  while(!empty() && front().is_space()) pop_front();
  if(!empty()) pop_front(); // number of arguments
}

// Constructs the attributes list for a table.
// eqnarray is RCL, align is RL, and matrix is C*.
// For aligned it is rlrlrlrl
// Otherwise we must fetch an argument.
void Math::fetch_rlc(vector<AttList>&table)
{
  string rlc = "rcl";
  if(is_aligned_ext()) {
    remove_opt_arg(false);
    rlc = "rlrlrlrlrl"; // C if more then 10 cols
  } else if(is_align_ext()  || is_align_ext_star()) {
    pop3();
    rlc = "rlrlrlrlrl";
  }
  else if(is_multline_ext()) rlc = "c";
  else if(is_gather_ext()) rlc = "c";
  else if(is_split_ext()) rlc = "rl";
  else if(is_matrix()) rlc = "";
  else if(is_bordermatrix()) rlc = "";
  else if(is_matrix_ext()) rlc = ""; // pmatrix, etc
  else if(is_array_ext())
    rlc = remove_req_arg();
  the_main->SH.rlc_to_string(rlc.c_str(),table);
}

// Converts a cell. Updates n, the index of the cell in the row.
Xmlp Math::convert_cell(int& n, vector<AttList>& table,math_style W)
{
  Xmlp res = new Xml(cst_mtd,0);
  if(empty()) {
    n++; // empty cell, no atts needed.
    return res;
  }
  Xid id = res->get_id();
  int tbl_align = 0;
  cmi.set_cid(id);
  Math args = *this;
  if(!(front().get_cmd()==special_math_cmd && 
       front().get_lcmd()==sub_to_math(multicolumn_code))) {
    int m = table.size();
    if(n<m) id.add_attribute(table[n],true);
    n++;
  } else {
    Math L = front().get_list();
    pop_front();
    skip_initial_space();
    if(!empty()) 
      the_parser.parse_error("Cell contains garbage after \\multicolumn",
			     "multicol garbage"); 
    L.get_arg1().convert_this_to_string(math_buffer); // get the span
    int k = atoi(math_buffer.c_str());
    if(k<=0) n++;
    else { 
      id.add_attribute(np_columnspan,Istring(the_main->SH.find(k)));
      n += k;
    }
    L.get_arg2().convert_this_to_string(math_buffer);
    char c = math_buffer.single_char();
    if(c == 'l') tbl_align = 1;
    if(c == 'r') tbl_align = 2;
    args = L.get_arg3();
  }
  int k = args.check_align();
  if(k) tbl_align = k;
  if(tbl_align==1) id.add_attribute(np_columnalign,np_left);
  else if(tbl_align==2) id.add_attribute(np_columnalign,np_right);
  else if(tbl_align==3) id.add_attribute(np_columnalign,np_center);
  Xmlp tmp = args.convert_math(W);
  res->add_tmp(tmp);
  return res;
}

// Converts an array.
Xmlp Math::split_as_array(vector<AttList>& table,bool is_multline,bool needs_dp,math_style W)
{
  Math cell;
  int n =0; // index of cell in row.
  Xmlp res = new Xml(cst_mtable,0);
  Xmlp row = new Xml(cst_mtr,0);
  Xid rid = cmi.get_rid(); // old rid, to be restored at the end
  Xid cid = cmi.get_cid();
  Xid taid = cmi.get_taid();
  cmi.set_taid(res->get_id());
  cmi.set_rid(row->get_id());
  if (needs_dp) W = ms_D;
  res->push_back(row);
  bool first_cell = is_multline; 
  while (!empty()) {
    symcodes cmd = front().get_cmd();
    if(cmd==alignment_catcode) { // finish cell
      pop_front();
      row->push_back(cell.convert_cell(n,table,W));
      cmi.set_cid(cid);
      cell.clear();
      first_cell = false;
    } else if(cmd == backslash_cmd) { // finish row and cell
      pop_front();
      remove_opt_arg(true);
      row->push_back(cell.convert_cell(n,table,W));
      if(first_cell) cmi.get_cid().add_attribute(np_columnalign,np_left);
      cmi.set_cid(cid);
      cell.clear();
      first_cell = false;
      n = 0;
      row = new Xml(cst_mtr,0);
      cmi.set_rid(row->get_id());
      res->push_back(row);
    } else if(cmd==space_catcode && cell.empty()) { pop_front(); }
    else { cell.push_back(front()); pop_front(); }
  }
  if(!cell.empty()) {
    row->push_back(cell.convert_cell(n,table,W));
    if(is_multline) cmi.get_cid().add_attribute(np_columnalign,np_right);
    cmi.set_cid(cid);
  }
  if(row->size()==0)  // kill the last empty row
    res->pop_back();
  Xid w = the_main->the_stack->next_xid(res);
  w.add_attribute(cmi.get_taid()); // move the attributes
  cmi.set_rid(rid);
  cmi.set_taid(taid);
  res->change_id(w);
  if(needs_dp) res->add_att(cst_displaystyle,np_true);
  return res;
}


// Converts an object that holds an array or something.
// eqnarray and align are outer objects, hence the finish_math.
Xmlp Math::M_array (math_style cms)
{
  if(is_array_ext())
    remove_opt_arg(false);
  vector<AttList> table;
  fetch_rlc(table);
  bool special = is_split_ext()||is_aligned_ext()|| is_external_array();
  Xmlp res = split_as_array(table,is_multline_ext(), special,cms);
  if(is_bordermatrix()) res -> bordermatrix();
  if(is_matrix_ext()) {
    int open,close;
    special_fence(name[0],open,close);
    res = new Xml(the_names[cst_mfenced], res);
    res->add_att(np_close,math_data.get_fence(close));
    res->add_att(np_open,math_data.get_fence(open));
  }
  return res;
}

void Xml::bordermatrix()
{
  int n = tree.size() -1;
  if(n <= 0) return;
  Xml* F = tree[0];
  if(F && ! F->is_xmlc() && F-> tree.size() > 1) {
    F -> insert_at (1, new Xml(cst_mtd,0));
  }
  Istring att = Istring("rowspan");
  Buffer& B = math_buffer; B.reset(); B << n;
  Istring attval = Istring(B);
  F = tree[1];
  if(F && ! F->is_xmlc() && F-> tree.size() > 1) {
    Xml* aux = new Xml(cst_mtd,math_data.mk_mo("("));
    aux -> add_att(att,attval);
    F -> insert_at (1, aux);
    aux = new Xml(cst_mtd,math_data.mk_mo(")"));
    aux -> add_att(att,attval);    
    F -> push_back(aux);
  }
}

// -----------------------------------------------------------


// We assume here that the formula has two tokens. The first is underscore 
// or caret, code in cmd. In the case of $_x$ or $_{x}$ or $^{y}$
// return a non-mathml expression in case the argument is OK.
Xmlp Math::trivial_math_index(symcodes cmd) 
{
  Buffer &B = math_buffer;
  B.reset();
  const_math_iterator L = value.begin();
  ++L;
  name_positions loc = cmd==underscore_catcode ? np_s_sub: np_s_sup;
  CmdChr w = L->get_cmd_chr();
  bool have_font = false;
  name_positions font_pos = cst_empty;
  if(w.is_letter() || w.is_other())
    B.push_back(w.char_val());
  else if(L->is_list()) {
    const Math& A=  L->get_list();
    const_math_iterator C = A.value.begin();
    const_math_iterator E = A.value.end();
    if(C==E) return 0;
    if(C->get_cmd() == mathfont_cmd) {
      have_font = true;
      subtypes s = C->get_chr();
      if(s==math_f_italic) font_pos = np_font_it;
      else if(s==math_f_upright) have_font= false;
      else if(s==math_f_sansserif) font_pos = np_font_sansserif;
      else if(s==math_f_monospace) font_pos = np_font_tt;
      else if(s==math_f_bold) font_pos = np_font_bold;
      else return 0;
      ++C;
    }
    while(C!=E) {
      if(C->is_char())
	B.push_back(C->get_char());
      else if(C->get_cmd() == mathfont_cmd) {
	++C;
	if(C==E) break;
	else return 0;
      }
      else return 0;
      ++C;
    }
  }
  else return 0;
  Xmlp tmp = the_main->the_stack->fonts1(loc);
  Xmlp xval = new Xml(Istring(B)); 
  if(have_font) {
    Xmlp tmp2 = the_main->the_stack->fonts1(font_pos);
    tmp2 ->push_back(xval);
    xval = tmp2;
  }
  tmp ->push_back(xval);
  return tmp;
}



// If this is a number, returns the number, with ok = 1;
// if it starts with a number, followed by a hat, returns the number,
// and reads the hat. 
// Sets ok=2 (if ok), 0 otherwise.

// If only digits are given the result is the formula as an XML elemet.
// Otherwise, we expect $1^{er}$, or things like that. 
// Only digits can appear before the hat. Only one token (or a list)
// can appear after the hat. This should be e, o or a special exponent.
// The result is as if you said: 1\textsuperscript{er} (without $).
// Some hacks
// Handles the case $1$, the case $x$.
// We handle also the case of $_{foo}$ that gives \textsubscript{foo}
// provided that a special switch is true.

Xmlp Math::trivial_math(int action)
{
  action = action%8;
  if(action==0) return 0;
  const_math_iterator L = value.begin();
  if(L== value.end()) return 0; // empty formula is never trivial
  int len = 1;
  ++L;
  if(L!= value.end()) {
    ++L;
    len = 2;
    if(L!= value.end()) len = 3;
  }
  symcodes cmd = front().get_cmd();
  Xmlp res = 0;
  if((action&4) && len ==2 && (cmd == underscore_catcode||cmd == hat_catcode))
    res = trivial_math_index(cmd);
  if(res) return res;
  if(action&1) res = special1();
  if(res) return res;
  if(len != 1) return 0;
  if(!(action&2)) return 0;
  if(front().is_digit()) {
    Istring sval = the_names[cst_dig0+front().val_as_digit()];
    return new Xml(sval);
  }
  if(front().is_letter_token()) {
    uint c = front().get_char().get_value();
    if(c<nb_simplemath)
      return math_data.get_simplemath_val(c);

  }
  if(front().is_other_token() && front().get_char()=='-')
    return new Xml(Istring("&#x2013;"));
  if(front().get_cmd() == mathord_cmd ||
     front().get_cmd() == mathordb_cmd ||
     front().get_cmd() == mathbin_cmd ||
     front().get_cmd() == mathop_cmd ||
     front().get_cmd() == mathopn_cmd ||
     front().get_cmd() == mathrel_cmd ||
     front().get_cmd() == mathinner_cmd ||
     front().get_cmd() == mathbetween_cmd ||
     front().get_cmd() == mathopen_cmd ||
     front().get_cmd() == mathclose_cmd) {
    int c = front().get_chr();
    if(first_inline_hack <= c && c <= last_inline_hack)
      return math_ns::get_builtin_alt(c);
  }
  return 0;
}

// Inserts the current font in the list
void Math::add_cur_font()
{
  int c = the_parser.eqtb_int_table[math_font_pos].get_val();
  push_back_font(subtypes(c),zero_code);
}

// Insert the font in the list and saves the font.
void Parser::TM_math_fonts(Math& x)
{
  subtypes cur_chr = cur_cmd_chr.get_chr();
  the_parser.word_define(math_font_pos,cur_chr,false);
  x.push_back_font(cur_chr,zero_code);
}

bool Parser::is_not_a_math_env(String s)
{
  find_env_token(s,false);
  if(cur_cmd_chr.is_user()) return true;
  return false;
}



// This is done when we see a label in a math formula. 
// Only one label is allowed.
void MathHelper::new_label(string s)
{
  if(warned_label) return;
  if(seen_label || current_mode) {
    warned_label = true;
    the_parser.parse_error("Label will be lost: ",s,"label-lost");
  } else 
    set_label(s);
}

// This removes the spaces.
void Math::remove_spaces()
{
  value.remove_if(MathIsSpace());
}

// Returns true if there is an \over or something like that in the list.
bool Math::has_over() const
{
  int ovr = count_if(value.begin(), value.end(), MathIsOver());
  if(ovr>1)
    the_parser.parse_error("Too many commands of type \\over");
  return ovr>0;
}

// Case of \mathop{\rm sin}. The this in the procedure is what follows the
// \mathop.
Xmlp MathElt::try_math_op()
{
  if(!is_list()) return 0;
  Math& X = get_list();
  if(X.empty()) return 0;
  if(!(X.front().get_cmd()==mathfont_cmd && X.front().get_chr() ==math_f_upright))
    return 0;
  if(!X.chars_to_mb2(math_buffer)) return 0;
  Xmlp s = new Xml(cst_mo, new Xml(Istring(math_buffer)));
  s->add_att(np_form,np_prefix);
  return s;
}

// This converts a character into a MathML object
MathElt MathElt::cv_char()
{
  uint c = get_chr();
  int a;
  math_types mt = mt_flag_small;
  int F = get_font();
  if(c>=nb_mathchars)
    return MathElt(math_ns::mk_mi(Utf8Char(c)),mt_flag_small);
  if(::is_digit(c))
    a = c-'0'+math_dig_loc;
  else if(::is_letter(c) && F<2) {
    a = math_char_normal_loc+F*nb_mathchars+c;
  } else if(::is_letter(c)) {
    int w = the_parser.eqtb_int_table[mathprop_ctr_code].get_val();
    if(w&(1<<F))
      return MathElt(math_ns::mk_mi(c,F),mt);
    else return MathElt(math_ns::make_math_char(c,F),mt);
  } else {
    a = c + math_c_loc;
    mt = math_data.get_math_char_type(c);
  }
  return MathElt(subtypes(a),mt);
}

// This converts a constant.
MathElt MathElt::cv_cst()
{
  subtypes c = get_chr();
  Xmlp s = math_constants(c);
  math_types mt = math_space_code(c) ? mt_flag_space : mt_flag_small;
  return MathElt(s, mt);
}

// This converts a list
MathElt MathElt::cv_list(math_style cms,bool ph)
{
  Math& X = get_list();
  if(get_lcmd()==math_open_cd) { // case of {x+y}
    XmlAndType res = X.M_cv(cms,ph?2:1);
    return MathElt(res.get_value(), res.get_type());
  }
  if(get_lcmd()==math_LR_cd) {   // case \left(x+y\right)
    int a = X.front().get_chr();
    int b = X.back().get_chr();
    X.pop_front(); 
    X.pop_back();
    XmlAndType res = X.M_cv(cms, 0);
    Xmlp res2 = math_data.make_mfenced(a,b,res.get_value());
    return MathElt(res2,mt_flag_big);
  }
  if(get_lcmd()==math_env_cd) { // case \begin{array}...
    if(X.is_array_env())
      return MathElt(X.M_array(cms),mt_flag_big);
    if(X.is_align_ext() || X.is_align_ext_star())
      return MathElt(X.M_array(cms),mt_flag_big);
    the_parser.parse_error("bad math env ", X.get_name(),"bad math env");
    return MathElt(CmdChr(error_cmd,zero_code),zero_code);
  }
  cv1_err();
  return MathElt(CmdChr(error_cmd,zero_code),zero_code);
}

// Return the name of the element associated to the command c.
name_positions math_ns::cv_special_string (int c)
{
  if(c>=first_maccent_code &&  c<=last_maccent_code)
    return c>=first_under_accent_code ? cst_munder : cst_mover;
  if(c==overline_code || c==overbrace_code|| c==stackrel_code ||
     c== overset_code || c==accentset_code)
    return cst_mover; 
  if(c==underline_code || c==underbrace_code || c==underset_code 
     || c==undertilde_code || c==underaccent_code)
    return cst_munder;
  if (c== root_code) 
    return cst_mroot;
  if(c==frac_code || c== dfrac_code || c== tfrac_code || c==genfrac_code
    || c==cfrac_code || c==binom_code||c==dbinom_code || c==tbinom_code) 
    return cst_mfrac;
  return np_unknown;
}

// Return 1 if the list is left aligned, 2 if right aligned, 0 if centered
int Math::check_align()
{
  int a=0,b=0;
  if(!empty() &&front().get_cmd() == hfill_cmd) {
    a = front().get_chr();
    pop_front();
  }
  if(!empty() &&back().get_cmd() == hfill_cmd) {
    b = back().get_chr();
    pop_back();
  }
  if(a==b && a==hfill_code) return 3; // clearly centered
  if(a==b) return 0; // clearly centered
  if(a==hfill_code) return 2;
  if(b==hfill_code) return 1;
  if(a==hfil_code) return 2;
  if(b==hfil_code) return 1;
  return 0; // remaining cases are \hss and \hfilneg
}


// Create <mi>...</mi> and friends
MathElt MathElt::cv_mi(math_style cms)
{
  Math& L = get_list();
  subtypes c = get_fml_subtype();
  const_math_iterator X = L.begin();
  const_math_iterator Y = L.end();
  Xmlp res = 0;
  if(c== mathbox_code) {
    Xmlp xs = X->get_list().M_cv(cms,0).get_value();
    res = new Xml(Istring(L.get_name()),xs);
  } else if (c== multiscripts_code) {
    Xmlp xs = X->get_list().M_cv(cms,0).get_value();  
    name_positions w = name_positions(c-mathmi_code +  cst_mi);
    res = new Xml(w,xs);
  } else {
  string s = X->get_list().convert_this_to_string(math_buffer);
    Xmlp xs = new Xml(Istring(s));
    name_positions w = name_positions(c-mathmi_code +  cst_mi);
    res = new Xml(w,xs);
  }
  ++X;
  for(;;) {
    if(X==Y) break;
    string s1 = X->get_list().convert_this_to_string(math_buffer);
    ++X;
    string s2 = "";
    if(X==Y) {} // Should we signal an error ?
    else {
      s2 = X->get_list().convert_this_to_string(math_buffer);
      ++X;
    }
    res->add_att(Istring(s1),Istring(s2));
  }
  return MathElt(res,mt_flag_small);
}


//  This converts commands.
MathElt MathElt::cv_special(math_style cms)
{
  subtypes c = get_fml_subtype();
  Math& L = get_list();
  switch(c) {
  case mathchoice_code:  {
    Math table[4];
    if(!L.empty()) { table[0] = L.get_arg1(); L.pop_front(); }
    if(!L.empty()) { table[1] = L.get_arg1(); L.pop_front(); }
    if(!L.empty()) { table[2] = L.get_arg1(); L.pop_front(); }
    if(!L.empty()) { table[3] = L.get_arg1(); L.pop_front(); }
    Math w = table[cms];
    return MathElt(w.convert_math(cms),mt_flag_small); // flag ok ?
  }
  case operatorname_code:
  case operatornamestar_code:  {
    string s = L.get_arg1().convert_opname();
    Xmlp xs = new Xml(Istring(s));
    Xmlp res = new Xml(cst_mo,xs);
    res->add_att(np_form,np_prefix);   
    return MathElt(res,c==operatornamestar_code ?mt_flag_opD : mt_flag_opN);
  }
  case qopname_code: {
    // arg 1 is currently ignored
    string s = L.get_arg3().convert_opname();
    string o = L.get_arg2().convert_opname();
    Xmlp xs = new Xml(Istring(s));
    Xmlp res = new Xml(cst_mo,xs);
    res->add_att(np_form,np_prefix);
    return MathElt(res,(o=="o") ? mt_flag_opN : mt_flag_opD);
  }
  case mathmi_code:
  case mathmo_code:
  case mathmn_code:
  case mathnothing_code:
  case mathci_code:
  case mathcn_code:
  case mathcsymbol_code:
  case multiscripts_code:
  case mathbox_code: 
    return cv_mi(cms);
  case math_attribute_code:
  case cell_attribute_code:
  case row_attribute_code:
  case table_attribute_code:
  case formula_attribute_code:
  case thismath_attribute_code: {
    string s1 = L.get_arg1().convert_this_to_string(math_buffer);
    string s2 = L.get_arg2().convert_this_to_string(math_buffer);
    Istring A = Istring(s1);
    Istring B = Istring(s2);
    if(c==math_attribute_code)
      the_main->the_stack->add_att_to_last(A,B,true);
    else cmi.add_attribute(A,B,c);
    return MathElt(CmdChr(error_cmd,zero_code),zero_code);
  }
  case boxed_code: {
    Xmlp x = L.get_arg1().M_cv(cms,0).get_value();
    x = new Xml(cst_mtd,x);
    x = new Xml(cst_mtr,x);
    x = new Xml(cst_mtable,x);
    x->add_att(Istring("frame"),Istring("solid"));
    return MathElt(x,mt_flag_small);
  }
  case phantom_code:
  case hphantom_code:
  case vphantom_code: {
    Xmlp A = L.get_arg1().M_cv(cms,0).get_value();
    if(c==hphantom_code || c==vphantom_code) {
      A = new Xml(cst_mpadded,A);
      if(c==vphantom_code) A->add_att(np_width,np_zerodim);
      if(c==hphantom_code) A->add_att(np_height,np_zerodim);
      if(c==hphantom_code) A->add_att(np_depth,np_zerodim);
    }
    Xmlp R = new Xml(cst_mphantom,A);
    return MathElt(R,mt_flag_small);
  }
  case smash_code: {
    Xmlp A = L.get_arg2().M_cv(cms,0).get_value();
    Xmlp R = new Xml(cst_mpadded,A);
    L.get_arg1().convert_this_to_string(math_buffer);
    char w= math_buffer[0];
    if(w!='b'&& w !='t' && w != 'w') w = 'c';
    if(w=='b' || w=='c') R->add_att(np_depth,np_zerodim);
    if(w=='t' || w=='c') R->add_att(np_height,np_zerodim);
    if(w=='w') R->add_att(np_width,np_zerodim);
    return MathElt(R,mt_flag_small);
  }
  default: return cv_special1(cms);
  }
}


MathElt MathElt::cv_special1(math_style cms)
{
  subtypes c = get_fml_subtype();
  int numalign = 0, denalign = 0;
  Math& L = get_list();
  if(c==cfrac_code) {
    L.get_arg1().convert_this_to_string(math_buffer); 
    L.pop_front();
    cms =ms_D;
    char w= math_buffer[0];
    if(w=='l') numalign = 1;
    if(w=='r') numalign = 2;
    c = frac_code;
  }
  int style = -1; // style to add to XML
  int open =del_dot, close=del_dot; // delimiters, in case
  Istring sz; // fraction rule width
  if(c==genfrac_code) {
    open = L.front().get_chr();
    L.pop_front();
    close = L.front().get_chr();
    L.pop_front();
    sz = Istring(L.front().get_chr());
    L.pop_front();
    if(L.front().get_cmd()==style_cmd) {
      cms = style_level(L.front().get_chr());
      style = cms;
    }
    L.pop_front();
  } else {
    if(c==dfrac_code) { cms = ms_D; style = 0; }
    if(c==tfrac_code) { cms = ms_T; style = 1; }
    if(c==dbinom_code) { cms = ms_D; style = 0; }
    if(c==tbinom_code) { cms = ms_T; style = 1; }
  }
  if(c==dbinom_code || c== tbinom_code || c==binom_code) {
    open = del_open_par; close = del_close_par;
    sz = Istring(np_zerodim);
  }
  if(c==frac_code || c==binom_code || c==genfrac_code || c==dfrac_code 
     || c==dbinom_code) {
    cms = next_frac_style(cms);
  } else 
    cms = next_math_style(cms);
  Math tmp = L.get_arg1();
  int k = tmp.check_align();
  if(!numalign) numalign = k;
  Xmlp A1 = tmp.convert_math(cms);
  if(c==sqrt_code)
    return MathElt(new Xml(cst_msqrt,A1),mt_flag_big);
  Xmlp A2=0;
  name_positions ns = cv_special_string(c);
  Istring s= the_names[ns]; 
  bool is_fraction = ns == cst_mfrac;
  bool is_mathop = false;
  int pos =0;
  if(c==xleftarrow_code || c==xrightarrow_code) {
    tmp = L.get_arg2();
    tmp.check_align();
    A2 = tmp.convert_math(cms);
    Xmlp A3 = get_builtin(c);
    if(L.get_arg1().empty()) {
      A1 = A3;
      s = the_names[cst_mover];
    } else if(L.get_arg2().empty()) {
      A2 = A1; A1 = A3; 
      s = the_names[cst_munder];
    } else {
      Xmlp tmp= new Xml(the_names[cst_munderover],0);
      tmp->add_tmp(A3);
      tmp->push_back(xmlspace);
      tmp->add_tmp(A1);
      tmp->push_back(xmlspace);
      tmp->add_tmp(A2);
      return MathElt(tmp,mt_flag_big);
    }
  }
  else if(c>=first_maccent_code &&  c<=last_maccent_code) {
    A2 = get_builtin(c);
    pos = c>=first_under_accent_code ? cst_accentunder : cst_accent;
  }
  else if(c==overline_code) {
    A2 = math_data.get_mc_table(1);
    pos = 0;
    //    pos = cst_accent;
  }
  else if(c==overbrace_code) {
    A2 = math_data.get_mc_table(2);
    pos = cst_accent;
    is_mathop = true;
  }
  else if(c==underline_code) {
    A2 = math_data.get_mc_table(3);
    pos = 0;
    // pos = cst_accentunder;
  }
  else if(c==undertilde_code) {
    A2 = math_data.get_mc_table(26);
    pos = cst_accentunder;
  } else if(c==underbrace_code) {
    A2 = math_data.get_mc_table(4);
    pos = cst_accentunder;
    is_mathop = true;
  } else {
    tmp = L.get_arg2();
    denalign = tmp.check_align();
    A2 = tmp.convert_math(cms);
    if(c==accentset_code) pos = cst_accent;
    if(c==underaccent_code) pos = cst_accentunder;
    if(c==stackrel_code||c==underset_code || c==overset_code || c== root_code
       || c==accentset_code || c==underaccent_code){
      Xmlp X = A1; A1 = A2; A2 = X;
      int xx= numalign; numalign = denalign; denalign = xx; // swap this also
    }
  }
  Xmlp res =  finish_cv_special(is_fraction,s, pos,A1,A2,sz,
			       numalign,denalign,style,open,close);
  return MathElt(res,is_mathop? mt_flag_opD: mt_flag_big);
}


// First pass: convert characters.
MathElt MathElt::cv1(math_style cms,bool ph)
{
  subtypes c = get_chr();
  switch(get_cmd()) {
  case space_catcode:
  case letter_catcode: 
  case other_catcode:
  case char_given_cmd:
  case math_given_cmd:
    return cv_char();
  case hat_catcode:
  case underscore_catcode:
  case math_xml_cmd: // ok ??
  case left_cmd:
  case right_cmd:
    return *this;
  case cst1_cmd:
    return cv_cst();
  case mathbin_cmd: 
   return MathElt(c,mt_flag_bin);
  case mathrel_cmd: 
   return MathElt(c,mt_flag_rel);
  case mathordb_cmd:
    return MathElt(subtypes(c-alpha_code+alpha_bcode),mt_flag_small);
  case mathord_cmd:
  case mathinner_cmd: 
   return MathElt(c,mt_flag_small);
  case mathbetween_cmd:
   return MathElt(c,mt_flag_small_m);
  case mathopen_cmd: 
   return MathElt(c,mt_flag_small_l);
  case mathclose_cmd:
   return MathElt(c,mt_flag_small_r);
  case mathop_cmd: 
    return MathElt(c,mt_flag_opD);
  case mathopn_cmd: 
    return MathElt(c,mt_flag_opN);
  case special_math_cmd:
    return cv_special(cms);
  case math_list_cmd:
    return cv_list(cms,ph);
  case mathspace_cmd:
    return MathElt(c,mt_flag_space);
  case relax_cmd:
    return MathElt(CmdChr(error_cmd,zero_code),zero_code);
  default:
    cv1_err(); 
    return MathElt(CmdChr(error_cmd,zero_code),zero_code);
  }
}

void MathElt::dump_for_err() const
{
  the_log << lg_start << int(get_cmd()) << " - " 
	  << int(get_chr()) << " - " << int(get_font()) 
	  << lg_end;
}

void MathElt::cv1_err()
{
  dump_for_err();
  main_ns::log_and_tty << lg_start << "--- " << Token(get_font()) << "\n";
  the_parser.signal_error("Bad math expression"); 
}

// Converts flags from small to large.
void MathElt::change_type(int t)
{
  if(get_cmd() != math_xml_cmd) return;
  math_types T = get_xmltype();
  if(T==mt_flag_small) {
    del_pos w = get_delimiter(get_chr());
    if(w==del_invalid) return;
    if(t==0) {
      if(w==del_open_ket) T = mt_flag_large_l;
      else if(w==del_close_ket) T = mt_flag_large_r;
      else return;
    } else if(t==1) T = mt_flag_large_l;
    else if(t==2) T = mt_flag_large_r;
    else T = mt_flag_large_m;
    set_xmltype(T);
    return;
  }
  if(!(T == mt_flag_small_l || T ==mt_flag_small_r || T == mt_flag_small_m))
    return;
  if(t==0)  T = math_types(T- mt_flag_small_l + mt_flag_large_l);
  else if(t==1) T = mt_flag_large_l;
  else if(t==2) T = mt_flag_large_r;
  else T = mt_flag_large_m;
  set_xmltype(T);
}

// We have an \over somewhere...
XmlAndType Math::M_cv0(math_style cms)
{
  Math A;
  subtypes c = subtypes(atopwithdelims_code+1);
  cms = next_frac_style(cms);
  while(!empty()) {
    if(front().get_cmd()==over_cmd) {
      c = front().get_chr();
      break;
    }
    A.push_back(front());
    pop_front();
  }
  pop_front();
  del_pos open = del_dot, close = del_dot;
  int numalign = 0, denalign = 0;
  if(c==atopwithdelims_code ||c==overwithdelims_code ||c==abovewithdelims_code){
    while(!empty())
      { if(front().get_cmd()==relax_cmd) pop_front(); else break; }
    if(empty())
      the_parser.parse_error("Problem finding opening delim");
    else {
      open = get_delimiter(front().get_cmd_chr());
      if(open==del_invalid)
	the_parser.parse_error("Invalid character in open","bad delims");
      pop_front();
    }
    while(!empty())
      { if(front().get_cmd()==relax_cmd) pop_front(); else break; }
    if(empty())
      the_parser.parse_error("Problem finding closing delim");
    else {
      close = get_delimiter(front().get_cmd_chr());
      if(close==del_invalid)
	the_parser.parse_error("Invalid character in close","bad delims");
      pop_front();
    }
  }
  Istring sz;
  if(c==above_code ||c==abovewithdelims_code) {
    if(empty()) the_parser.parse_error("Invalid width","bad delims");
    sz = chars_to_mb3();
  }
  if(c==atop_code ||c==atopwithdelims_code) sz = the_names[np_zerodim];
  numalign = A.check_align();
  denalign = check_align();
  Xmlp a = A.M_cv(cms,1).get_value();
  Xmlp b = M_cv(cms,1).get_value();
  Xmlp res = finish_cv_special(true,the_names[cst_mfrac],
			       0,a,b,sz,numalign,denalign,-1,open,close);
  return XmlAndType(res, mt_flag_big);
}

Xmlp math_ns::finish_cv_special(bool isfrac, Istring s, int pos, Xmlp a, Xmlp b,
			       Istring sz,int numalign, int denalign,
			       int style,int open, int close)
{
  Istring Pos;
  if(pos) Pos = the_names[pos];
  Xmlp R = the_main->the_stack->xml2_space(s,Pos, a,b);
  if(!sz.null()) R->add_att(the_names[np_linethickness],sz);
  if(isfrac) {
    if(numalign==1) R->add_att(cst_numalign,np_left);
    if(numalign==2) R->add_att(cst_numalign,np_right);
    if(denalign==1) R->add_att(cst_denalign,np_left);
    if(denalign==2) R->add_att(cst_denalign,np_right);
  }
  if(style>=0) R = math_data.add_style(style,R);
  if(!(open ==del_dot && close==del_dot))
    R = math_data.make_mfenced(open,close,R);
  return R;
}

XmlAndType Math::M_cv (math_style cms, int need_row)
{
  the_parser.my_stats.one_more_convert();
  if(has_over()) return M_cv0(cms); // there was an over somewhere
  Math res; 
  int i=0;
  bool prev_is_hat = false; //  must increment cms if true
  bool cur_is_hat = false;
  bool seen_style = false;
  while(!empty()) {
    i++;
    MathElt cur = front();
    pop_front();
    if(cur.is_space())
      continue; // ignore this
    prev_is_hat = cur_is_hat;
    symcodes cmd = cur.get_cmd();
    subtypes chr = cur.get_chr();
    if(cmd==hat_catcode || cmd==underscore_catcode) {
      cur_is_hat = true;
      res.push_back(cur);
      continue;
    }
    cur_is_hat = false;
    if(cur.is_hbox()) {
      cur.get_list().handle_mbox(res);
      continue;
    }
    if(cmd==hspace_cmd) {
      if(chr==one_code||chr==three_code) continue;
      Buffer& B = Trace;
      B.reset();
      int n = cur.get_font();
      B.push_back(ScaledInt(n),glue_spec_pt);
      Xmlp value = mk_space(B.c_str());
      res.push_back(MathElt(value,mt_flag_space));
      continue;
    }
    if(cmd==style_cmd) {
      cms = style_level(chr);
      seen_style = true;
      continue;
    }
    if(cmd==ref_cmd) { 
      res.push_back_small(M_ref()); 
      continue;
    }

    int next_action = 0;
    math_types new_type=mt_flag_rel;
    int t =0;
    if(cmd==nonscript_cmd) {
      if(cms==ms_T || ms_D) continue;
      res.push_back(cur);
      continue;
    }
    if(cmd==limits_cmd) {
      res.push_back(cur); 
      continue;
    } 
    if(cmd==math_comp_cmd && (chr==bin_noad ||chr==rel_noad) && !empty()) {
      next_action = 1;
      if(chr==bin_noad) new_type = mt_flag_bin;
      cur = front();
      pop_front();
    } else if(cmd==math_comp_cmd) {
      res.push_back(cur);
      continue;
    } else if(cmd==big_cmd && !empty()) {
      cur = front();
      pop_front();
      t = chr/4; //  empty, L, R, et M.
      next_action = 2;
    } else
      if( cmd==ensuremath_cmd || cmd==nonumber_cmd || cmd==nolinebreak_cmd||
	 cmd==mathfont_cmd)
      continue;
    bool next_is_hat = false;
    if(cmd==math_list_cmd) {
      while(!empty() && front().is_space()) pop_front();
      if(!empty() && (front().get_cmd()==hat_catcode 
		      || front().get_cmd()==hat_catcode))
	next_is_hat = true;
    }
    math_style cmss = cms;
    if(prev_is_hat)
      cmss =  next_math_style(cms);
    if(cur.maybe_seq()&& !empty() && front().maybe_seq(cur.get_font()) &&
	    next_action==0 &&!prev_is_hat)
      { res.push_back(convert_char_seq(cur)); }
    else {
      MathElt Cur = cur.maybe_iseq() ?
	convert_char_iseq(cur,!prev_is_hat) :
	cur.cv1(cmss,next_is_hat);
      if(next_action==1) Cur.set_xmltype(new_type);
      if(next_action==2) Cur.change_type(t);
      if(Cur.get_cmd()== error_cmd) continue;
      res.push_back(Cur);
    }
  }
  res.handle_cmd_Big(cms);
  Math res1 = res.M_cv3(cms); 
  math_types res_type = mt_flag_small;
  if(res1.finish_translate1(the_parser.tracing_math()))
    res_type =mt_flag_big;
  if(res1.length_one()) {
    Xmlp W = res1.front().get_xml_val();
    if(need_row==2) W = new Xml(cst_mrow,W);
    if(!seen_style)
      return XmlAndType(W,res_type);
    else {
      Xmlp res2 = math_data.add_style(cms,W);
      return XmlAndType(res2, res_type);      
    }
  }
  Xmlp tmp = new Xml(cst_temporary,0);
  res1.concat(tmp);
  Xmlp res22=0;
  if(need_row) res22 = new Xml(cst_mrow,tmp);
  else res22 = tmp;
  if(seen_style) res22 = math_data.add_style(cms,res22);
  return XmlAndType(res22,res_type);
}

// Translate \ref
Xmlp Math::M_ref()
{
  Math w = front().get_list();
  pop_front();
  string label = w.convert_opname();
  Xmlp X = new Xml(cst_mref,0);
  X->get_id().add_ref(label);
  return X;
}

// Case of a box in math.
// Gathers characters in the buffer. Return 0 in case of error
// 1 in case everything is OK, -N if a math formula has been seen,
// and 3 if we have seen a space. Return 2 in case of a font command 
// Sets f to the font 

static ScaledInt cur_math_space;

void math_ns::bad_math_warn (Buffer& B)
{
  the_log << lg_start << "Bad token in argument of \\text-like command\n";
  if(B.empty())
    the_log << "Error occured at start of list.\n";
  else
    the_log << "Error occured after scanning " << B << ".\n";
}

int Math::M_mbox1 (Buffer&B, subtypes&f)
{
  B.reset();
  while(!empty()) {
    symcodes cmd = front().get_cmd();
    int chr = front().get_chr();
    subtypes fn = front().get_font();
    MathElt old = front();
    pop_front();
    if(cmd == space_catcode || (cmd==other_catcode && chr== '~'))
      return 3;
    else if(cmd==cst1_cmd && math_space_code(chr)){ 
      switch(chr) {
      case quad_code: return 5;
      case qquad_code: return 6;
      case exclam_code: return 7;
      case comma_code: return 8;
      default: return 3;
      }
    } else if(cmd== 11 || cmd==12) {
      B.push_back_real_utf8(chr); 
      f = fn;
      continue;
    } else if(cmd==mathfont_cmd) return 2;
    else if(cmd==fontsize_cmd) return 2;
    else if(is_m_font(cmd)) continue;
    else if(cmd==ref_cmd) { 
      if(front().get_cmd()==math_list_cmd&&front().get_list().type==math_open_cd)
	return 4;
      else return 2;// Should signal an error
    } else if(cmd== char_given_cmd || cmd==math_given_cmd) {
      B.push_back_real_utf8(chr); 
      continue;
    } 
    else if(cmd == relax_cmd) continue;
    else if(cmd==mathspace_cmd) {
      if(chr==xml_thickmu_space_loc) return 10;
      if(chr==xml_thinmu_space_loc) return 7;
      if(chr==xml_medmu_space_loc) return 9;
      return 0; 
    }
    else if (cmd==kern_cmd || cmd == scan_glue_cmd || cmd == hspace_cmd)
      {
	cur_math_space = ScaledInt(old.get_font());
	return 11;
      }
    else if(cmd == special_math_cmd) {
      math_ns::bad_math_warn (B);
      math_list_type y = old.get_list().type;
      CmdChr x (special_math_cmd, math_to_sub(y));
      the_log << "Offending command = \\" << x.name() << "\n";
      return 0;
    }
    else if(cmd != math_list_cmd) {
      math_ns::bad_math_warn (B);
      the_log << "Offending command = \\" << old.get_cmd_chr().name() << "\n";
      return 0; 
    }
    else if(old.get_list().type==math_dollar_cd)
      return - int(chr);
    else if(old.get_list().type==math_open_cd)
      push_front(old.get_list()); // insert the sublist here
    else {
      math_ns::bad_math_warn (B);
      the_log << "Subformula is of type " << old.get_list().type << "\n";
      return 0;
    }
  }
  return 1;
}


void Math::handle_mbox(Math& res)
{
  while(!empty()) {
    subtypes font = math_f_normal;
    int ok = M_mbox1(math_buffer,font);
    if(ok == 0) { 
      the_parser.signal_error("bad hbox (see transcript file for details)"); 
      Xmlp Text = new Xml(cst_mtext,new Xml(Istring("bad hbox")));
      res.push_back_small(new Xml (Istring("merror"), Text));
      return;
    }
    if(!math_buffer.empty()) {
      String s = math_buffer.c_str();
      Xmlp Text = new Xml(cst_mtext,new Xml(Istring(s)));
      if(int(font)>1)
	Text->add_att(cst_mathvariant,name_positions(cstf_normal+font));
      res.push_back_small(Text);
      the_parser.my_stats.one_more_mbox();
    }
    if(ok==1) return;
    if(ok==2) continue;
    if(ok==4) { // This is a \ref
      res.push_back_small(M_ref());
    } else if(ok==3 ||ok==5 ||ok==6 ||ok==7 ||ok==8||ok==9||ok==10 ||ok==11) {
      Xmlp b;
      if(ok==5) b = math_data.get_mc_table(7);
      else if(ok==6) b = math_data.get_mc_table(8);
      else if(ok==7) b = math_data.get_mc_table(13);
      else if(ok==8) b = math_data.get_mc_table(14);
      else if(ok==9) b = get_builtin(xml_medmu_space_loc);
      else if(ok==10) b = get_builtin(xml_thickmu_space_loc);
      else if(ok==11) {
	Buffer& B = Trace;
	B.reset();
	B.push_back(cur_math_space,glue_spec_pt);
	b  = mk_space(B.c_str());
      } else {
	b = new Xml(cst_mspace,0);
	b->add_att(np_cst_width, np_halfem);
      }
      res.push_back_small(b);
    } else {
      Math u = math_data.get_list(-ok);
      res.push_back_small(u.convert_math(ms_T));
    }
  }
}
 
Xmlp MathElt::remove_prefix() const
{
  if(get_cmd() == math_xml_cmd) return get_xml_val();
  dump_for_err();
  main_ns::log_and_tty << "bad math token " << Token(get_font()) << int(right_cmd) <<"\n";
  return new Xml(Istring("BAD"));
}

void Cv3Helper::reinit()
{
  state = 2; 
  index= 0; 
  exponent = 0;
  p = 0;
  ploc = -1; 
  ptype = mt_flag_small;
  prefix_type = mt_flag_zero;
  special = 0;
}


// Case where current token is \nonscript. If the mode is D or T, the token
// is removed in the first pass. Otherwise we remove it in the second pass
// together with a space that follows
void Cv3Helper::non_script()
{
  pop_front();
  if(!empty()) {
    if(get_cmd() == math_xml_cmd && get_xmltype()==mt_flag_space)
      pop_front();
  }
}


void Cv3Helper::find_kernel()
{
  if(empty()) {
    state = 3;
    return;
  }
  symcodes C = get_cmd();
  if(C== hat_catcode || C == underscore_catcode) {
    p = new Xml(the_names[cst_mrow],0); // Insert <mrow/> before
    state = 1;
    return;
  }
  if(C==nonscript_cmd) { non_script(); return; }
  if(C==math_comp_cmd) {
    subtypes q = get_chr();
    switch(q) {
    case between_noad: prefix_type = mt_flag_small_m; break;
    case ord_noad: prefix_type = mt_flag_small; break;
    case op_noad: prefix_type = mt_flag_opD; break;
    case bin_noad: prefix_type = mt_flag_bin; break;
    case rel_noad: prefix_type = mt_flag_rel; break;
    case open_noad: prefix_type = mt_flag_small_l; break;
    case close_noad: prefix_type = mt_flag_small_r; break;
    default:  prefix_type = mt_flag_small; break;
    }
    pop_front();
    return;
  }
  if(C != math_xml_cmd) {
    the_parser.signal_error("unexpected obj");
    pop_front();
    return;
  }
  ploc = get_chr();
  ptype = get_xmltype();
  if(prefix_type !=0) ptype = prefix_type;
  if(ptype==mt_flag_opD) special = 1;
  if(ptype==mt_flag_opN) special = 2;
  p = object.front().get_xml_val();
  pop_front();
  state = 1;
}

void Cv3Helper::find_index(math_style cms)
{
  if(empty()) {
    state = 2;
    return;
  }
  symcodes C = get_cmd();
  if(C==limits_cmd) {
    int k = get_chr(); // displaylimits, nolimits, limits
    pop_front();
    if(index || exponent) return;   // allow x_2\limits^3
    if(special == 0) return;
    if(k==0) k = cms==ms_D? 2 : 1;
    special = special & 7; // clear flags
    if(k==2) special += 8;
    else special += 16;
    return;
  }
  if(C==nonscript_cmd) { non_script(); return; }
  if (C ==underscore_catcode && !index) {
    pop_front();
    if(empty()) {
      the_parser.parse_error("Math formula should not finish with _");
    } else if(get_cmd() == underscore_catcode || get_cmd() == hat_catcode) 
      the_parser.parse_error("Two consecutive _ or ^ characters");
    else { 
      index = object.front().remove_prefix ();
      pop_front();
    }
    return;
  }
  if(C == hat_catcode && !exponent) {
    pop_front();
    if(empty()) {
      the_parser.parse_error("Math formula should not finish with ^");
    } else if(get_cmd() == underscore_catcode || get_cmd() == hat_catcode) 
      the_parser.parse_error("Two consecutive _ or ^ characters");
    else {
      exponent = object.front().remove_prefix();
      pop_front();
    }
    return;
  }
  if (C ==underscore_catcode) {
    the_parser.parse_error("double subscript");
    pop_front();
    state = 2;
    return;
  }
  if(C == hat_catcode) {
    the_parser.parse_error("double superscript");
    pop_front();
    state = 2;
    return;
  }
  state = 2;
  return;
}

// x_1 and \sum_1 produce <msub> or <munder>
// This is a bit complicated
// Special is A+B, A=8(limits) or 16(nolimits)
// B=0 (not a big op), B=1 (\sum, \lim, \mathop), B=2(\int, \sin)
name_positions Cv3Helper::find_operator(math_style cms)
{
  int what =0;
  if(index) what ++;
  if(exponent) what += 2;
  name_positions bl = cst_msub;
  if(what==3) bl = cst_msubsup; 
  else if (what==2) bl = cst_msup;

  if(special >16) special = 0; //  explicit \nolimits
  else if(special ==2) special = 0; // implicit \nolimits
  else if(special==1 && cms != ms_D) special = 0; // implicit \displaylimits

  if(!special) return bl;
  if(special>8) { // \lim\limits_1 : define movablelimits='false'
    Xmlp q = p->spec_copy();
    if(q) {
      p = q;
      p->add_att(np_movablelimits,np_false);
    }
  }
  if(what==3) bl = cst_munderover;
  else if(what==2) bl = cst_mover;
  else bl = cst_munder;
  return bl;
}

void Cv3Helper::add_kernel(math_style cms)
{
  if(!index && !exponent) {
    res.push_back(p,ploc,ptype);
    return;
  }
  the_parser.my_stats.one_more_kernel();
  name_positions bl = find_operator(cms);
  if(the_main->get_prime_hack() &&
     exponent == math_data.get_mc_table(27) && bl == cst_msup) {
    bl = cst_mrow; exponent = get_builtin(varprime_code);
  }
  Xmlp tmp = new Xml(bl,0);
  // case a_b_c. If we do nothing, the mathml interpreter will barf
  if(p && !p->is_xmlc() && (p->has_name(cst_msup) || p->has_name(cst_msub)
			    || p->has_name(cst_msubsup)))
    p = new Xml(cst_mrow,p);
  if(ptype == mt_flag_small_l || ptype ==mt_flag_small_r 
     || ptype == mt_flag_small_m) {
    res.push_back(p,ploc,ptype);
    //      static int dmc =0;
    //      cout << "Dmc=== " <<  ++dmc << "\n";
    Xmlp q =p;    
    //      special_buffer << bf_reset << "<mn>DMC " << dmc << "</mn>";
    //      q = new Xml (Istring(special_buffer.c_str()));
    res.push_back(q,-1,mt_flag_dummy);
  }
  
  // case {\sum}_1
  tmp->add_tmp(p); 
  tmp->push_back(xmlspace);
  if(index) { tmp->add_tmp(index); tmp->push_back(xmlspace);}
  if(exponent) { tmp->add_tmp(exponent); tmp->push_back(xmlspace);}
  p = tmp;
  res.push_back(p,-1,mt_flag_big);
}

Math Math::M_cv3(math_style cms)
{
  Cv3Helper W(*this);
  for(;;) {
    W.reinit();
    for(;;) {
      W.find_kernel();
      if(W.state==3) return W.finish();
      if(W.state==2) continue;
      break;
    }
    for(;;) {  // if state = 1, I have a kernel p, otherwise looking for it
      W.find_index(cms);
      if(W.state==2) break;
    }
    W.add_kernel(cms);
  }
}

// returns the element with a new id, if it's a <mo> and has a np_movablelimits
// attributes; return null otherwise.
Xmlp Xml::spec_copy()
{
  if(name != Istring(cst_mo)) return 0;
  AttList&X = get_id().get_att();
  int i = X.has_value(the_names[np_movablelimits]);
  if(i<0) return 0;
  Xmlp res = new Xml(name,0);
  res->tree = tree;
  res->get_id().add_attribute(X,true);
  return res;
}

void Math::concat_space(Xmlp res)
{
  while(!empty()) {
    res->push_back(front().get_xml_val());
    pop_front();
    if(!empty()) res->push_back(xmlspace);
  }
}

void Math::concat(Xmlp res)
{
  while(!empty()) {
    res->push_back(front().get_xml_val());
    pop_front();
  }
}

del_pos MathElt::large2()
{
  return get_delimiter(get_chr());
}

// The list holds something like [a+b, et cl is ]
// returns <mfenced open=[ close=]>a+b</mfenced>
Xmlp Math::large1(MathElt& cl,math_style cms)
{
  the_parser.my_stats.one_more_large();
  int close = cl.large2(); // ok ??
  int open = front().large2(); // ok ??
  bool bad = false;
  if(open==del_invalid || close == del_invalid) {
    bad = true;
    push_back(cl);
  } else pop_front();
  Math res0 = M_cv3(cms);
  res0.finish_translate1(the_parser.tracing_math());
  Xmlp res1 = new Xml(cst_temporary,0);
  res0.concat_space(res1);
  if(bad) {
    Xmlp res = new Xml(cst_mrow,0);
    res->add_tmp(res1);
    return res;
  } else 
  return math_data.make_mfenced(open,close,res1);
}

// This piece of code tries to add some <mfenced> commands
// whenever we have a \big something
void Math::handle_cmd_Big (math_style cms)
{
  for(;;) {
    bool ok = handle_cmd_Big_aux(cms);
    if(!ok) return;
  }
}

bool Math::handle_cmd_Big_aux (math_style cms)
{
  MathList res;
  Math aux;
  bool state = false;
  bool ok = false; // true if we have added a fence
  bool try_again = false; // true if formula has unused \big
  while(!empty()) {
    int cmd = front().get_cmd();
    math_types t = front().get_xmltype();
    if(cmd == math_xml_cmd && t== mt_flag_large_l) {
      if(state) {
	try_again = true;
	res.splice(res.end(),aux.value); 
      }
      aux.push_back(front());
      state = true;
    } else if(cmd == math_xml_cmd && t == mt_flag_large_r) {
      if (!state) {
	try_again = true;
	res.push_back(front());
      } else {
	Xmlp x = aux.large1(front(),cms);
	res.push_back(MathElt(x,-1,mt_flag_big));
	aux.clear();
	ok = true;
	state = false;
      }
    }
    else if (!state) res.push_back(front());
    else aux.push_back(front());
    pop_front();
  }
  value.swap(res);
  value.splice(value.end(), aux.value);
  return ok && try_again;
}

Xmlp Math::convert_math(math_style k)
{
  return M_cv(k,1).get_value();
}

// Removes an an initial group that is the consequence of \refstepcounter
void Math::remove_initial_group()
{
  bool initial_relax = false;
  const_math_iterator B = value.begin();
  const_math_iterator E = value.end();
  if(B==E) return;
  if(B->get_cmd()==relax_cmd) { initial_relax=true; ++B;}
  if(B==E) return;
  if(B->get_cmd()!=math_list_cmd) return;
  if(B->get_lcmd()!=math_open_cd) return;
  Math& X = B->get_list();
  if(!X.value.empty()) return;
  if(initial_relax)
    value.pop_front();
  value.pop_front();
}

void tralics_ns::boot_math(bool mv)
{
  xmlspace = new Xml(Istring(" "));
  math_data.boot();
  if(mv) {
    uint w = (2<< 15)-1;
    the_parser.word_define (mathprop_ctr_code,w,true);
  }
}
