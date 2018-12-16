// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


const char* txtitlepage_rcsid =
  "$Id: txtitlepage.C,v 2.37 2011/06/28 08:15:25 grimm Exp $";


#include "tralics.h"
#include "txtitlepage.h"


namespace{
  int init_file_pos = 0;   // position in init file
  Buffer docspecial; // Buffer for document special things
  Buffer tp_main_buf;
  Buffer local_buf;  // some local buffer
  Buffer ssa2;
  TitlePage Titlepage; // title page info
  TitlePageFullLine tpfl;
  TpiOneItem Toi;
}

namespace tpage_ns {
  void init_error();
  bool begins_with(string A, String B);
  bool scan_item(Buffer&in,Buffer&out,char del);
  tpi_vals next_item(Buffer&in,Buffer&out);
  int see_an_assignment(Buffer&in, Buffer&key,Buffer& val);
  bool see_main_a(Buffer&in, Buffer&key, Buffer&val);
  void after_conf_assign(vector<string>& V);
}

// needed elsewhere, because we have to call add_language_add.
bool tralics_ns::titlepage_is_valid()
{
  return Titlepage.is_valid();
}

// needed in txmain.
void tralics_ns::Titlepage_start(bool verbose)
{
  Titlepage.start_thing(verbose);
}

// This is called in case of trouble.
void tpage_ns::init_error()
{
  main_ns::log_and_tty << "Syntax error in init file (line " << init_file_pos << ")\n";
}

// return tl_end if seen End, tl_empty if empty (or comment), 
// tl_normal otherwise
tpa_line Buffer::tp_fetch_something()
{
  ptr = 0;
  if(strncmp(buf,"End",3)==0) return tl_end;
  skip_sp_tab();
  if(is_special_end()) 
    return tl_empty;
  if(ptr==0) {
    tpage_ns::init_error();
    main_ns::log_and_tty << buf;
    main_ns::log_and_tty << "Wanted End, or line starting with space\n";
    return tl_empty;
  }
  return tl_normal;
}

// This is the function that creates the title page data
// from a list of lines
void tralics_ns::Titlepage_create(LinePtr& lines)
{
  if(lines.is_empty()) return;
  Titlepage.make_valid();
  for(;;) {
    tp_main_buf.reset();
    int line = lines.get_next(tp_main_buf);
    if(line<0) return;
    init_file_pos = line;
    tpa_line k = tp_main_buf.tp_fetch_something();
    if(k==tl_empty) continue;
    if(k==tl_end) break;
    int w0 = tpfl.read();
    tpi_vals w = tpfl.classify(w0,Titlepage.state);
    if(w==tpi_err) {
      tpage_ns::init_error();
      continue;
    }
    TitlePageAux tpa(tpfl);
    bool res = tpa.classify(w,Titlepage.state);
    if(!res) { 
      tpage_ns::init_error(); 
      continue; 
    }
    Titlepage.bigtable.push_back(tpa);
  }
}


// Example of usage
// CESS  \makeRR <rrstart> "myatt" "docatt"
// ACSS  alias \makeRT "myTatt" "docTatt"
// CES   \title <title> "notitle"
// CEES  \author +<autlist> <au> "default"
// E     <UR> -
// CE    \URsop ?+ <UR>
// CEE   \Paris ?<UR> <Rocq>
// CCS   \myself \author "JG"
// AC    alias \foo

// CES \abstract e<abstract> "abstract"  % no par
// CES \abstract E<abstract> "abstract"  % with par


// Gets next non space. return false if EOL
bool Buffer::tp_next_char(char& res)
{
  skip_sp_tab();
  if(is_special_end()) 
    return false;
  res = head();
  advance();
  return true;
}

// Returns an item that start with del
// result is in local_buf, return value is true if OK.
bool tpage_ns::scan_item(Buffer&in,Buffer&out,char del)
{
  out.reset();
  if(del=='\\') {// scan a command
    while(is_letter(in.head())) out.push_back(in.next_char());
    return true;
  }
  if(del=='<') del = '>'; // else del is ""
  while(in.head() != del) {
    if(!in.head()) {
      tpage_ns::init_error(); 
      main_ns::log_and_tty << "could not find end delimiter\n";
      out << bf_reset << "notfound";
      return false;
    }
    out.push_back(in.next_char());
  }
  in.advance();
  return true;
}

// Scans an item: prefix plus value. Result in Toi
// returns the type of the thing to create.
tpi_vals tpage_ns::next_item(Buffer&in,Buffer&out)
{
  Toi.reset();
  char c;
  if(!in.tp_next_char(c)) return tpi_noval;
  if(c=='a' && in.is_here("lias"))
    return tpi_alias;
  if(c=='e' && in.is_here("xecute"))
    return tpi_execute;
  if(c=='a' && in.is_here("ction"))
    return tpi_execute;
  if(!is_tp_delim(c)) {
    Toi.set_p1(c);
    if(!in.tp_next_char(c)) return tpi_noval;
    if(!is_tp_delim(c)) {
      Toi.set_p2(c);
      if(!in.tp_next_char(c)) return tpi_noval;
      if(!is_tp_delim(c))
	return tpi_err;
    }
  }
  if(!tpage_ns::scan_item(in,out,c)) return tpi_err;
  Toi.set_value(out);
  if(c=='<') return tpi_elt;
  else if(c=='\\') return tpi_cmd;
  else return tpi_str;
}

// This reads up to four items.
// returns a number that tells what types have been read.
int TitlePageFullLine::read()
{
  item1.reset();
  item2.reset();
  item3.reset();
  item4.reset();
  int res =0;
  tpi_vals w= tpage_ns::next_item(tp_main_buf,local_buf);
  if(w==tpi_err) return -1;
  item1 = Toi;
  item1.set_v(w);
  res += 1000*(w-1);
  if(w==tpi_noval) return res;
  w = tpage_ns::next_item(tp_main_buf,local_buf);
  if(w==tpi_err) return -1;
  item2 = Toi;
  item2.set_v(w);
  res += 100*(w-1);
  if(w==tpi_noval) return res;
  w = tpage_ns::next_item(tp_main_buf,local_buf);
  if(w==tpi_err) return -1;
  item3 = Toi;
  item3.set_v(w);
  res += 10*(w-1);
  if(w==tpi_noval) return res;
  w = tpage_ns::next_item(tp_main_buf,local_buf);
  if(w==tpi_err) return -1;
  item4 = Toi;
  item4.set_v(w);
  res += (w-1);
  if(w==tpi_noval) return res;
  char c;
  if(tp_main_buf.tp_next_char(c)) return -1;
  return res;
}

// First attempt at classification 
// E=1 C=2 S=3 A=4 e=5
tpi_vals TitlePageFullLine::classify(int w,int state)
{
  if(w==-1) return tpi_err; // forget about this
  flags = 0;
  if(item1.has_a_char() || item4.has_a_char()) return tpi_err;
  if(w==0) return tpi_zero; // ignore this item
  if(w==2133) { if (state) return tpi_err; }
  else { if(!state) return tpi_err; }
  if(w==1000) 
    return item2.only_dash() ? tpi_E : tpi_err;
  else if(w==3000)  return tpi_S;
  else if(w==4200 || w==4233) {
    if(item2.has_a_char() || item3.has_a_char()) return tpi_err;
    if(state==1 && w==4233) return tpi_ACSS;
    if(state==2 && w==4200) return tpi_AC;
  } else if(w==2230 || w==2133) {
    if(item2.has_a_char() || item3.has_a_char()) return tpi_err;
    return w==2230 ? tpi_CCS : tpi_CESS;
  } else if (w==2100)
    return item2.quest_plus() && !item3.has_a_char() ? tpi_CE : tpi_err;
  else if(w==5200) 
    return !item2.has_a_char() ? tpi_EC : tpi_err;
  else if(w==2110)  
    return item2.question() && !item3.has_a_char() ? tpi_CEE : tpi_err;
  else if(w==2113)
    return item2.plus() && !item3.has_a_char() ? tpi_CEES : tpi_err;
  if(w != 2130) return tpi_err;
  if(!encode_flags(item2.get_p1(),item3.get_p1()))
    return tpi_err;  
  return tpi_CES;
}

// More classification. This allocates memory, and modifies state
bool TitlePageAux::classify(tpi_vals w,int& state)
{
  switch(w) {
  case tpi_CESS: // \makeRR <rrstart> "ok" "ok"
    state = 1; 
    idx = 0;
    type = tpi_rt_tp; 
    return true;
  case tpi_ACSS: // alias \makeRT "ok" "ok"
    T1 = T2; T2 = Titlepage.bigtable[0].get_T2();
    type = tpi_rt_tp;
    return true; // state is still 1
  case tpi_CES: // \title <title> "notitle"
    state = 2; 
    type = tpi_rt_normal;
    T4 = T3; T3 = T2; 
    idx = Titlepage.increase_data();
    if(get_flags2()==tp_A_flag) {
      Buffer&B = local_buf;
      B << bf_reset << "\\" << T1; 
      B.push_back_braced(T4); 
      B.push_back("%\n");
      the_main->add_to_from_config(1,B);
    }
    return true;
  case tpi_CEES: // \author +<aulist> <au> "default"
    state = 2; 
    type = tpi_rt_list; 
    idx = Titlepage.increase_data();
    return true;
  case tpi_E: //  <UR> -
    state = 3;  // next line cannot be alias
    type = tpi_rt_urlist; 
    idx = Titlepage.increase_data();
    T2 = T1;
    return true;
  case tpi_CE: // case \URsop ?+ <UR>
    {
      int k = Titlepage.find_UR(T2,T1);
      if(k==0) 	return false;
      state = 2; 
      type = tpi_rt_ur; 
      idx = k;
      T2 = local_buf.to_string(); 
      return true;
    }
  case tpi_S: // case "<foo/>"
    type = tpi_rt_constant; 
    idx = Titlepage.increase_data();
    return true;
  case tpi_EC: // case execute \foo 
    type = tpi_rt_exec; 
    idx = Titlepage.increase_data();
    return true;
  case tpi_CEE: // \Paris ?<UR> <Rocqu>
    {
      int k = Titlepage.find_UR(T2,"");
      if(k==0) 
	return false;
      state = 2;
      type = tpi_rt_ur; 
      idx = k;
      T2 = T3; T3 = "";
      return true;
    }
  case tpi_CCS: // case \myself \author "JG"
    {
      int k = Titlepage.find_cmd(T2);
      if(k==0) return false;
      TitlePageAux& R =Titlepage.bigtable[k]; 
      state = 2; 
      idx = R.idx;
      xflags = k; // hack...
      T2 = T3; T3 = R.T3;
      type = R.type ==tpi_rt_normal ? tpi_rt_normal_def : tpi_rt_list_def;
    }
    return true;
  case tpi_AC:
    type = tpi_rt_alias;
    T1 = T2; T2 = Titlepage.bigtable.back().get_T1();
    return true;
  default:
    return false;
  }
}

// Evaluates at start of translation
void TitlePageAux::exec_start(int k)
{
  if(type==tpi_rt_urlist) { // no command here
    Titlepage[idx] = convert(2,new Xml(the_names[cst_nl]));
    return;
  }
  if(type==tpi_rt_alias) { // \let\this\that
    the_parser.hash_table.eval_let(T1.c_str(), T2.c_str());
    return;
  }
  // in all other cases define this as (titlepage,k)
  the_parser.hash_table.primitive(T1.c_str(), titlepage_cmd, subtypes(k));
  if(type==tpi_rt_list) { // initialise the list
    Titlepage[idx] = convert(2,convert(3,Istring(T4)));
    return;
  }
  // remaining types are:  tp, ur, normal_def, list_def, and normal
  if(type != tpi_rt_normal)
    return;
  int fl = get_flags2(); // this is 0,1, 2 or 3
  if(fl==0) {
    Titlepage[idx] = convert(2,Istring(T4));
    return;
  }
  Buffer&B = local_buf;
  B << bf_reset << "\\" << T1; 
  B.push_back_braced(T4); 
  Titlepage[idx] = new Xml(Istring("empty"));
  B.push_back("%\n");
  if(fl==tp_A_flag) {} // already done
  //    the_main->add_to_from_config(1,B);
  else if(fl==tp_B_flag) 
    the_parser.add_buffer_to_document_hook(B,"(tpa init)");
  else T4 = B.to_string();
}

// This is executed when we see the \Titlepage cmd
void TitlePageAux::exec_post()
{
  if(type==tpi_rt_constant)
    Titlepage[idx] = new Xml(Istring(T1));
  if(type==tpi_rt_exec)
    Titlepage[idx] = the_parser.tpa_exec(T2.c_str());
  if(type != tpi_rt_normal)
    return;
  if(get_flags2() == tp_C_flag) 
    the_parser.evaluate_now(T4.c_str(),T1);
  if(!has_plus_flags()) return;
  if(has_u_flags()) return;
  the_parser.parse_error("No value given for command \\", T1, "");
}

// This is executed when the user asks for a titlepage command.
void TitlePageAux::exec(int v,bool vb)
{
  if(vb) 
    the_log << lg_startbrace << "\\titlepage " << v << "=\\"<< T1<<lg_endbrace;
  if(type==tpi_rt_tp) { 
    the_parser.T_titlepage_finish(v);
    return;
  }
  if(type==tpi_rt_ur) { // easy case
    Titlepage[idx]->add_last_nl(convert(2));
    return;
  }
  Xmlp R=0;
  if(type==tpi_rt_normal_def ||type==tpi_rt_list_def)
    R = convert(3,Istring(T2));
  else { // we have to read the argument.
    R = the_parser.special_tpa_arg(T1.c_str(),T3.c_str(), 
				   has_p_flags(),has_e_flags(),has_q_flags());
  }
  bool replace = (type==tpi_rt_normal || type==tpi_rt_normal_def);
  if(increment_flag()) {
    if(!replace)
      Titlepage[idx]->remove_last();
  }
  if(replace)
    Titlepage[idx] = R;
  else 
    Titlepage[idx]->add_last_nl(R);
}

// Finds the real flag to increment.
// If 0, increments it and returns true, otherwise return false.
// In the case of =tpi_rt_*_def, the flag is a pointer !
bool TitlePageAux::increment_flag()
{
  if(type==tpi_rt_normal_def ||type==tpi_rt_list_def)
    return Titlepage.bigtable[xflags].increment_flag();
  if(!has_u_flags()) {
    xflags ++;
    return true;
  }
  return false;
}

// Thew string S, a sequence of a='b', is converted to attributes of this.
void Xid::add_special_att(string S)
{
  if(S.length()==0) return;
  Buffer& B=local_buf;
  B.reset();
  B.push_back(S);
  B.reset_ptr();
  B.push_back_special_att(*this);
}


// This is executed when we see the \titlepage command,
// After that, no more titlepage ...
void Parser::T_titlepage_finish (int v)
{
  int kmax = Titlepage.bigtable.size();
  for(int k=0;k<kmax;k++) 
    Titlepage.bigtable[k].exec_post();
  add_language_att();
  TitlePageAux& tpa = Titlepage.bigtable[v];
  string tmp = tpa.get_T4();
  bool finished = false;
  bool also_bib = false;
  if(strstr(tmp.c_str(),"'only title page'")) finished = true;
  if(strstr(tmp.c_str(),"'translate also bibliography'")) also_bib = true;
  Xid(1).add_special_att(tmp);
  Xmlp res = tpa.convert(2);
  res->get_id().add_special_att(tpa.get_T3());
  kmax = Titlepage.get_len2();
  for(int k=1;k<kmax;k++)
    res->add_last_nl(Titlepage[k]);
  the_stack.pop_if_frame(the_names[cst_p]);
  the_stack.add_nl();
  the_stack.add_last(res);
  the_stack.add_nl();
  ileave_v_mode();
  Titlepage.make_invalid();
  if(the_main->get_tpa_mode() ==1) return;
  else if(the_main->get_tpa_mode() ==2) finished = true;
  else if(the_main->get_tpa_mode() ==0) {
    if(main_ns::nb_errs > 0)
      finished = true;
  }
  if(finished && also_bib) {
    tralics_ns::bibtex_set_nocite();
    tralics_ns::bibtex_insert_jobname();
  }
  if(finished) {
    main_ns::log_and_tty << "Translation terminated after title page\n";
    x_input(end_all_input_code);
  }
}

void Parser::T_titlepage (int v)
{
  if(tracing_commands()) 
    the_log << lg_startbrace << "\\titlepage " << v <<lg_endbrace;
  if(!Titlepage.is_valid()) {
    main_ns::log_and_tty << "No title page info, bug?\n";
    return; // why ?
  }
  if(v<0 || v>= int(Titlepage.bigtable.size())) {
    main_ns::log_and_tty << "T_titlepage strange\n" << lg_fatal;
    abort();
  }
  Titlepage.bigtable[v].exec(v,tracing_commands());
}

// This is executed when the translation begins.
void TitlePage::start_thing(bool verbose)
{
  if(!valid) return;
  Data = new Xmlp [len2];
  for(unsigned int k=0;k<bigtable.size();k++) {
    if(verbose) bigtable[k].dump(k);
    bigtable[k].exec_start(k);
  }
}

// clears a TPI
void TpiOneItem::reset()
{
  p1 = 0; 
  p2 = 0;
  value = "";
  v = tpi_noval;
}

// For the case CEE, \Paris ?<UR flags> <Rocq>
// s is the string without attribs, and n is the length
int TitlePageAux::find_UR (String s, int n) const
{
  if(type != tpi_rt_urlist) return 0;
  String w = T2.c_str();
  if(strncmp(w,s,n)==0) return idx;
  else return 0;
}

// For the case CE, \URsop ?+ <UR myflags> 
// We put `URsop myflags' in the buffer local_buf in case of success.
int TitlePage::find_UR (const string& s, string name) const
{
  Buffer& B = local_buf;
  B << bf_reset << s;
  int j=0;
  while(B[j] && !is_space(B[j])) j++;
  bool have_space  = B[j]!=0;
  B.kill_at(j);
  String match = B.c_str();
  int res = 0;
  for(unsigned int k=0;k<bigtable.size();k++) {
    res = bigtable[k].find_UR(match,j);
    if(res) break;
  }
  if(!res) return 0;
  if(!name.empty()) {
    string w = B.to_string(j+1);
    B << bf_reset << name;
    if(have_space) B << ' ' << w;
  }
  return res;
}

// true if this OK for CCS and the nams is right.
bool TitlePageAux::find_cmd (const string& s) const
{
  if(type != tpi_rt_normal && type != tpi_rt_list) return false;
  if(T1 != s) return false;
  return true;
}

// Returns the index for a CCS.
int TitlePage::find_cmd (const string& s) const
{
  for(unsigned int k=0;k<bigtable.size();k++) {
    if(bigtable[k].find_cmd(s))
      return k;
  }
  return 0;
}

// Ctor from a TitlePageFullLine. It's just a copy.
TitlePageAux::TitlePageAux(TitlePageFullLine& X) : idx(0), type(tpi_zero)
{
  T1 = X.item1.get_value();
  T2 = X.item2.get_value();
  T3 = X.item3.get_value();
  T4 = X.item4.get_value();
  xflags = X.get_flags();
}

// True if B is an initial substring of A
bool tpage_ns::begins_with(string A, String B)
{
  unsigned int n = strlen(B);
  if(A.length()<n) return false;
  return strncmp(A.c_str(),B,n) == 0;
}

// True if current line starts with x.
bool Clines::starts_with(String x) const
{
  return tpage_ns::begins_with(get_chars(),x);
}

// This compares a Begin line with the string s.
// Returns : 0 not a begin; 1 not this type; 2 not this object
// 3 this type; 4 this object; 5 this is a type
int Buffer::is_begin_something(String s)
{
  if(strncmp("Begin",buf,5) != 0) return 0;
  if(strncmp("Type",buf+5,4)==0) {
    ptr = 9;
    skip_sp_tab();
    if(ptr==9) return 2; // bad
    set_ptr1();
    skip_letter();
    if(ptr==ptr1) return 2; // bad
    kill_at(ptr); // what follows the type is a comment
    if(!s) return 5; // s=0 for type lookup
    if(strcmp(buf+ptr1,s)==0) return 3;
    return 1;
  }
  if(!s) return 0;
  ptr = 5;
  set_ptr1();
  skip_letter();
  if(ptr==ptr1) return 2;
  kill_at(ptr);
  if(strcmp(buf+ptr1,s)==0) return 4;
  return 2;
}

// We remove everything that is not of type S.
void LinePtr::parse_and_extract_clean(String s)
{
  LinePtr res;
  int b =0;
  Buffer& B = local_buf;
  bool keep=true;
  bool cv=true; // unused. We assume that the line is always converted
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  line_iterator_const W = value.begin();
  while(C != E) {
    B.reset();
    int n = C->to_buffer(B,cv);
    W = C;
    ++C;  
    int open = B.see_config_env();
    b += open;
    if(b<0) { b=0; continue; } // ignore bogus lines
    if(b==0 && open == -1) { // cur env has closed
      keep = true;
      continue;
    } else if (b==1 && open==1) { // something new started
      int v = B.is_begin_something(s);
      if(v==1) {  keep = false;  continue; }
      else if(v==3) { keep = true;  continue; }
    }
    if(keep) res.insert(n,B.to_string(),cv); //res.value.push_back(*W);
  }
  clear();
  splice_first(res);
}

// Returns all line in a begin/end block named s
LinePtr LinePtr::parse_and_extract(String s) const
{
  LinePtr res;
  int b =0;
  Buffer& B = local_buf;
  bool keep= false;
  bool cv; // unused.
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  line_iterator_const W = value.begin();
  while(C != E) {
    B.reset();
    C->to_buffer(B,cv);
    W = C;
    ++C;  
    int open = B.see_config_env();
    b += open;
    if(open) keep = false; // status changed
    if(b<0) { b=0; continue; } // ignore bogus lines
    if (b==1  && open==1) { // something new started
      if(B.is_begin_something(s) == 4) keep = true;
      continue;
    }
    if(keep) res.value.push_back(*W);
  }
  return res;
}

// Execute all lines that are not in an block via see_main_a
void LinePtr::parse_conf_toplevel() const
{
  int b =0;
  bool cv; // unused. We assume that the line is always converted
  Buffer B;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    B.reset();
    init_file_pos = C->to_buffer(B,cv);
    ++C;  
    int open = B.see_config_env();
    b += open;
    if(b==0) tpage_ns::see_main_a(B,ssa2,local_buf);
  }
}

// Converts two characters into a flag.
bool TitlePageFullLine::encode_flags(char c1, char c2)
{
  flags = 0;
  if(c1=='p') flags = tp_p_flag;
  else if(c1=='e')  flags = tp_e_flag;
  else if(c1=='E')  flags = tp_p_flag +tp_e_flag;
  else if(c1=='q') flags = tp_q_flag;
  else if(c1 !=0) return false;
  if(c2=='+') flags += tp_plus_flag ;
  else if(c2 =='A') flags += tp_A_flag;
  else if(c2 =='B') flags += tp_B_flag;
  else if(c2 =='C') flags += tp_C_flag;
  else if(c2!=0) return false;
  return true;
}

// This prints the flags, in a symbolic way.
void TitlePageAux::decode_flags()
{
  int f1 = 0, f2 = 0;
  f2 = xflags/16;
  f1 = xflags%16;
  f1 = f1/2;
  if(f1==0 && f2==0) return;
  the_log << " (flags";
  if(f1==1) the_log << " +par";
  if(f1==2) the_log << " +env";
  if(f1==3) the_log << " +par +env";
  if(f1==4) the_log << " -par";
  if(f2==1) the_log << " +list";
  if(f2==2) the_log << " +A";
  if(f2==4) the_log << " +B";
  if(f2==6) the_log << " +C";
  the_log << ")";
}

// This prints a slot.
void TitlePageAux::dump(int k)
{
  tpi_vals t = get_type();
  if(t==tpi_rt_alias) {
    the_log << lg_start << "Defining \\" << T1
	    << " as alias to \\" << T2 << "\n";
    return;
  }
  if(t==tpi_rt_constant) {
    the_log << lg_start << "Inserting the string " << get_T1() << "\n";
    return;
  }
  if(t==tpi_rt_exec) {
    the_log << lg_start << "Inserting the command \\" << get_T2()   << "\n";
    return;
  }
  the_log << lg_start << "Defining \\" << get_T1() 
	  << " as \\TitlePageCmd " << k << "\n";
  if(t==tpi_rt_normal)
    the_log << "   usual <" << T3 << "/>";
  else if(t==tpi_rt_normal_def)
    the_log << "   usual? <" << T2 << "/>" << " " << T3;
  else if(t==tpi_rt_list)
    the_log << "   list <" << T2 << "/> and <" << T3 << "/>";
  else if(t==tpi_rt_list_def)
    the_log << "   list? <" << T2 << "/> and <" << T3 << "/>";
  else if(t==tpi_rt_urlist)
    the_log << "   ur_list <" << T2  << "/>";
  else if(t==tpi_rt_ur)
    the_log << "   ur <" << T2  << "/>";
  else if(t==tpi_rt_tp)
    the_log << "   main <" << T2 << " " << T3 << " -- "<< T4 <<"/>"; //   id??
  else the_log << "   random";
  if(t != tpi_rt_normal_def && t != tpi_rt_list_def)
    decode_flags(); // otherwise flags are meaningless
  the_log << "\n";
}

// Converts one of the strings into an empty XML element
Xmlp TitlePageAux::convert(int i)
{
  string s = i==1 ? T1 : i==2 ? T2 : i== 3 ? T3:T4;
  return local_buf.xml_and_attrib(s);
}

// Converts one of the strings into a XML element containing R.
Xmlp TitlePageAux::convert(int i, Xmlp r)
{
  string s = i==1 ? T1 : i==2 ? T2 : i== 3 ? T3:T4;
  Xmlp res = local_buf.xml_and_attrib(s);
  res->push_back(r);
  return res;
}

// See foo=bar. Returns bar.
// If c is true, stop at white space or comment.
// Otherwise, just remove trailing space.

String Buffer::see_config_kw(String s, bool c)
{
  if(!see_equals(s)) return 0;
  if(c) {
    int k = ptr;
    while(buf[k] && buf[k] != '%' && buf[k] != '#') k++;
    wptr = k;
    buf[k] = 0;
  }
  remove_space_at_end();
  return buf+ptr;
}

// This find a toplevel attributes. Real job done by next function.
void LinePtr::find_top_atts (Buffer& B)
{
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    B << bf_reset << C->get_chars();
    B.find_top_atts();
    C = skip_env(C,B);
  }
}


// A loop to find all types  and put them in res.
void LinePtr::find_all_types (vector<string>& res)
{
  Buffer& B= local_buf;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    init_file_pos = C->get_number();
    B << bf_reset <<  C->get_chars();
    B.find_one_type(res);
    C = skip_env(C,B);
  }
}


// This find a toplevel value.
string LinePtr::find_top_val (String s,bool c)
{
  Buffer& B = local_buf;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    B << bf_reset << C->get_chars();
    String res = B.see_config_kw(s,c);
    if(res) return res;
    C = skip_env(C,B);
  }
  return "";
}

// This does something with DocAttribs.
void Buffer::find_top_atts()
{
  if(!see_equals("DocAttrib")) return;
  ptr1 = ptr;
  skip_letter();
  string a = substring();
  skip_sp_tab();
  if(head() != '\\' && head() != '\"') return;
  remove_space_at_end();
  if(buf[ptr]=='\"' && buf[wptr-1] == '\"' && ptr<wptr-1)
    buf[wptr-1] = 0;
  if(buf[ptr]=='\"') {
    Istring as = Istring(a);
    Istring bs = Istring(to_string(ptr+1));
    Xid(1).add_attribute(as,bs);
  }
  else if(strcmp(buf+ptr,"\\specialyear")==0) {
    Istring as = Istring(a);
    Istring bs = Istring(the_main->get_year_string());
    Xid(1).add_attribute(as,bs);
  } else if(strcmp(buf+ptr,"\\tralics")==0) {
    Istring as = Istring(a);
    reset();
    push_back("Tralics version ");
    push_back(the_main->get_version());
    Istring bs = Istring(to_string());
    Xid(1).add_attribute(as,bs);
  } else {
    docspecial << bf_reset << "\\addattributestodocument{"
	       << a << "}{" << (buf+ptr) << "}";
    the_main->add_to_from_config(init_file_pos,docspecial);
  }
}

// Returns +1 if begin, -1 if end, 0 otherwise.
int Buffer::see_config_env() const
{
  if(strncmp(buf,"Begin",5) ==0) return 1;
  if(strncmp(buf,"End",3) ==0) return -1;
  return 0;
}

// This skips over an environment.
line_iterator_const LinePtr::skip_env(line_iterator_const C, Buffer&B)
{
  ++C;
  int b = B.see_config_env();
  if(b!=1)  return C;
  line_iterator_const E = value.end();
  while( C != E) {
    B << bf_reset << C->get_chars();
    ++C;
    b += B.see_config_env();
    if(b==0) return C;
  }
  return C;
}

// This finds one type. prints on the log. Remembers it.
void Buffer::find_one_type(vector<string>& S)
{
  if(is_begin_something(0)==5) {
    string s = to_string(ptr1);
    S.push_back(s);
    the_log << "Defined type: " << s << "\n";
  }
}

// This is called for all lines, outside groups.
// Either calls the_parser.shell_a.assign
bool tpage_ns::see_main_a(Buffer&in, Buffer&key, Buffer&val)
{
  key.reset();
  val.reset();
  int k = tpage_ns::see_an_assignment(in,key,val);
  if(k==1) {
    bool res = config_ns::assign(key,val);
    if(res) {
      if(!ssa2.empty())
	the_log << key << "="  << val.convert_to_log_encoding() << "\n";
      return true;
    }
  }
  return false;
}

void tpage_ns::after_conf_assign(vector<string>& V)
{
  int n = V.size();
  int i =0;
  for(;;) {
    if(i>=n) return;
    ssa2 << bf_reset << V[i];
    i++;
    local_buf << bf_reset << V[i];
    i++;
    bool res = config_ns::assign(ssa2,local_buf);
    if(res) 
      the_log << ssa2.c_str() << "="  << local_buf.c_str() << "\n";
  }
}


// Returns 0, unless we see A="B", fills the buffers A and B.
// return 2 if there is a space in A, 1 otherwise.
int tpage_ns::see_an_assignment(Buffer&in, Buffer&key,Buffer& val)
{
  if(in.tp_fetch_something() != tl_normal) return 0;
  for(;;) {
    if(in.is_special_end()) return 0;
    if(in.head() == '=') break;
    key.push_back(in.head());
    in.advance();
  }
  in.advance();
  in.skip_sp_tab();
  if(in.head() != '\"') return 0;
  in.advance();
  tpage_ns::scan_item(in,val,'\"');
  key.reset_ptr();
  key.remove_space_at_end();
  for(;;) {
    if(key.head()==0) return 1;
    if(is_space(key.head())) return 2;
    key.advance();
  }
}


// Find one aliases in the config file.
bool Buffer::find_alias(const vector<string>& SL, string&res)
{
  ptr = 0;
  if(strncmp(buf,"End",3)==0) return false;
  skip_sp_tab();
  if(is_special_end())
    return false;
  if(ptr==0) return false;
  set_ptr1();
  advance_letter_dig();
  if(ptr1==ptr) return ""; // this is bad
  string pot_res = substring();
  bool local_potres = false;
  if(tralics_ns::exists(SL,pot_res))
    local_potres = true;
  for(;;) {
    skip_sp_tab();
    if(is_special_end())
      break;
    set_ptr1();
    advance_letter_dig();
    if(ptr1==ptr) break;
    string a = substring();
    bool ok = a==res;
    if(ok) {
      the_log << "Potential type " << res << " aliased to " << pot_res <<"\n";
      if(!local_potres) 
	local_potres = the_main->check_for_tcf(pot_res);
      if(local_potres) {
	res = pot_res;
	return true;
      }
      the_log << "Alias " << pot_res << " undefined\n";
      return false;
    }
  }
  the_log << "Alias " << pot_res << " does not match " << res << "\n";
  return false;
} 

// Find all aliases in the config file.
bool LinePtr::find_aliases (const vector<string>& SL, string& res)
{
  Buffer& B = local_buf;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  bool in_alias = false;
  while(C != E) {
    B << bf_reset << C->get_chars();
    if(in_alias) {
      if(B.find_alias(SL,res))
	return true;
    }
    if(strncmp("End",B.c_str(),3) ==0)
      in_alias = false;
    else if(strncmp("BeginAlias",B.c_str(),10) ==0) {
      in_alias = true;
      ++C;
      continue;
    }
    C = skip_env(C,B);
  }
  return false;
}

// This converts ra2003 to ra.
string Buffer::remove_digits(string s)
{
  reset();
  push_back(s);
  int k = wptr;
  while(k>0 &&is_digit(buf[k-1])) k--;
  if(k !=wptr) {
    buf[k] = 0;
    return to_string();
  }
  return "";
}

// This gets the DTD.
void Buffer::extract_dtd(String a, string&b, string&c)
{
  if(!a) return;
  reset();
  push_back(a);
  ptr = 0;
  advance_letter_dig_dot();
  if(ptr==0) return;
  ptr1 = 0;
  b = substring();
  skip_sp_tab();
  if(head()=='-' || head()=='+' || head()==',')  { advance(); skip_sp_tab(); }
  ptr1 = ptr;
  advance_letter_dig_dot_slash();
  if(ptr1==ptr) return;
  c = substring();
}
