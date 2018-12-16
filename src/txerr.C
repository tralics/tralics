// Tralics, a LaTeX to XML translator.
// Error handler for tralics
// Copyright INRIA/apics/marelle (Jose Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


const char* txerr_rcsid =
  "$Id: txerr.C,v 2.51 2015/01/22 14:12:18 grimm Exp $";

#include "tralics.h"

namespace err_ns {
  Buffer local_buf;
  Istring convert_to_string(Token t);
  void convert_to_string(const TokenList& t);
  void signal_error(String s,int);
  void unexpected_char(String s, int k);
  void fatal_error (String s);
} 
using err_ns::local_buf;

void err_ns::fatal_error (String s)
{ 
  cout << "Fatal error " << s  << "\n"; 
  abort();
}


// This is executed if we say the_log << lg_fatal.
void Logger::abort()
{ 
  cout << "Fatal_error for " << the_parser.get_job_name() << "\n"; 
  if(!!*fp) {
    *fp << "Fatal_error for " << the_parser.get_job_name() << "\n"; 
    (*fp).flush();  
    (*fp).close(); 
  }
}


// The error mechanism is as follows: we put the error message in a buffer
// then use the following function to signal it. 
// If w=0 and the option -noxmlerror is not given, 
// then an error element with value s is insert in the XML result
// (We have w=2 if the error is signalled by txcheck)

void err_ns::signal_error(String s,int w)
{
  Parser&P = the_parser;
  int line =P.get_cur_line();
  string file = P.get_cur_filename();
  main_ns::nb_errs++;
  P.flush_buffer();
  the_log << lg_start;
  main_ns::log_and_tty << "Error signaled at line " << line;
  if(!file.empty()) main_ns::log_and_tty << " of file " << file ;
  main_ns::log_and_tty << ":\n"; 
  main_ns::log_and_tty << local_buf; 
  main_ns::log_and_tty << ".\n";
  the_log << lg_flush;  cout.flush();
  if(main_ns::nb_errs>5000) {
    main_ns::log_and_tty << "Translation aborted: Too many errors.\n"; 
    main_ns::log_and_tty.finish(main_ns::nb_errs);
    tralics_ns::close_file(main_ns::log_and_tty.L.fp);
    exit(1);
  }
  if(main_ns::no_xml_error) return;
  if(w) return;
  Istring str = Istring(s);
  P.the_stack.add_newid0(np_error);
  Istring cmd = err_ns::convert_to_string(P.get_name_for_error());
  P.the_stack.add_att_to_last(np_letter_c,str);
  P.the_stack.add_att_to_last(np_letter_l, P.cur_line_to_istring());
  P.the_stack.add_att_to_last(np_letter_n,cmd);
}


// Converts t into a character string, removes a final space 
Istring err_ns::convert_to_string(Token T)
{
  Buffer &B = local_buf;
  B.reset();
  uint x = T.get_val();
  if(x==0 ) {
    T = invalid_token;
    x = T.get_val();
  }
  if(x<cs_token_flag+single_base  && x != cs_token_flag+null_cs) {
    // We simplify the algorithm by printing the character as is
    Utf8Char c = T.char_val();
    if(c.is_null()) B.push_back("^^@");
    else B.push_back_real_utf8(c);
  } else {
    B.push_back('\\');
    if(x==cs_token_flag+null_cs)
      B.push_back("\\csname\\endcsname"); 
    else if(x<cs_token_flag+bad_cs) {
      Utf8Char c = T.char_val();
      if(c.is_null()) B.push_back("^^@");
      else B.push_back_real_utf8(c);
    } else {
      String s =  the_parser.hash_table[x-cs_offset];
      int n = strlen(s);
      bool ok = true;
      for(int i = 0; i<n;i++) {
	uchar c = s[i];
	if(c=='<' || c=='>' || c=='&' || c<32) { ok = false; break; }
      }
      if(ok)
	B.push_back(s);
      else for(int i = 0; i<n;i++) B.push_back_xml_char(uchar(s[i]));
    }
  }
  return Istring(B);
}

void err_ns::convert_to_string(const TokenList& L)
{
  Buffer &B = local_buf;
  static Buffer buffer_for_log;
  const_token_iterator C = L.begin();
  const_token_iterator E = L.end();
  while(C != E) {
    buffer_for_log.reset();
    if(buffer_for_log.push_back(*C))
      buffer_for_log << ' ';
    B << buffer_for_log.c_str();
    ++C;
  }
}

// The simpliest error function (adds nothing to the XML)
void Parser::signal_error(String s)
{
  local_buf << bf_reset << s;
  err_ns::signal_error(s,1);
}

// Some generic errors (may add s to the XML)
void Parser::parse_error(const string& s)
{
  local_buf << bf_reset << s;
  err_ns::signal_error(s.c_str(),0);
}
void Parser::parse_error(String s)
{
  local_buf << bf_reset << s;
  err_ns::signal_error(s,0);
}

void Parser::parse_error(const string& s1, const string& s2)
{
  local_buf << bf_reset << s1;
  err_ns::signal_error(s2.c_str(),0);
}


void Parser::parse_error(const string& s, TokenList& L)
{
  local_buf << bf_reset << s;
  err_ns::convert_to_string(L);
  err_ns::signal_error(s.c_str(),0);
}

void Parser::parse_error(const string& s1, const string& s2, const string& s3)
{
  local_buf << bf_reset << s1 << s2;
  err_ns::signal_error(s3.c_str(),0);
}


/// Useful error. Here s2 is a token.
void Parser::parse_error(const string& s1, Token s2, const string& s3,const string& s4)
{
  local_buf << bf_reset << s1 << s2.tok_to_str() << s3; 
  err_ns::signal_error(s4.c_str(),0);
}

// --------------------------------------------------
// Error from txcheck, without a line number.
void MainClass::bad(string s, string s2)
{
  local_buf << bf_reset <<  s << s2;
  err_ns::signal_error("",2);
}

// Other error from txcheck.
void MainClass::bad1(string s, string s2)
{
  local_buf << bf_reset << s <<  
    "\nerror signaled while scanning " <<  s2;
  err_ns::signal_error("",2);
}

// Error signaled when txcheck sees a bad environment
void MainClass::bad_end_env(string env, int c)
{
  local_buf << bf_reset << "Environment " << env << " is not closed"
	    << "\nstart of environment at line " << c;
  err_ns::signal_error("",2);
}

// Error signaled when txcheck sees a bad end of file
void MainClass::unexpected_eof(string s2, int c)
{
  if(c) main_ns::log_and_tty << "\nError may be at line " << c << ".\n";
  local_buf << bf_reset << "unexpected end of file\n" 
	    << "error signaled while scanning " <<  s2;
  err_ns::signal_error("",2);
  main_ns::log_and_tty << lg_fatal;
  exit(1);
}

// Error from txcheck in the case of \foo bar {gee}
void MainClass::bad_char_before_brace(int k, String s, String info)
{
  local_buf << bf_reset << "Characters ignored before open brace\n" 
	    << "error signaled while scanning " << info << "\n";
  err_ns::unexpected_char(s,k);
  err_ns::signal_error("",2);
}

// Error from txcheck in the case where chars are in excess 
// at the end of a line
void MainClass::bad_ignore_char(int k, String s)
{
  local_buf << bf_reset << "Characters ignored at end of line\n";
  err_ns::unexpected_char(s,k);
  err_ns::signal_error("",2);
}

// Helper function for bad_ignore_char
void err_ns::unexpected_char(String s, int k)
{
  Buffer B = local_buf;
  B << "current line is " << s;
  int n = strlen(s);
  if(n==0 || s[n-1] !='\n') B << "\n";
  B << "error is at position " << k << " at character " << s[k] 
	<< " (ascii " << (int) s[k] << ")";
}

// --------------------------------------------------

// May be signaled in case \end{verbatim} is missing
void Parser::bad_end_env(int cl)
{
  local_buf << bf_reset 
	    << "End of data reached while scanning end of environment "
	    <<  get_cur_env_name() << "\nscanning started line " << cl;
  err_ns::signal_error("Bad end env",0);
}

// Error signaled if no closing brace is found
void Parser::missing_close_brace(int cl)
{
  local_buf << bf_reset 
	    << "End of data reached while scanning a group;\n" 
	    << "scanning started line " << cl
	    << "\nscanning argument of " << name_for_error.tok_to_str();
  err_ns::signal_error("Missing close brace",0);
}

// Error signaled if EOF found while scanning a delimited argument
void Parser::bad_delimited(int cl, Token x)
{
  local_buf << bf_reset 
	    << "End of data reached while scanning argument of "
	    << name_for_error.tok_to_str()
	    << "\nscanning started at line " << cl;
  if(!x.is_null()) err_ns::local_buf << "\nexpected " << x.tok_to_str();
  err_ns::signal_error("bad macro",0);
}

void Parser::runaway(int cl)
{
  local_buf << bf_reset;
  if(cl==-1)
    local_buf << "End of data reached while scanning start of argument";
  else {
    local_buf << "Runaway argument?\n"
	      << "Paragraph ended while scanning an argument of "
	      << name_for_error.tok_to_str();
    if(cl&& cl != get_cur_line())
      local_buf <<  ";\nscanning started on line " << cl;
  }
  err_ns::signal_error("Runaway argument",0);
}

// Case of \newcommand{\foo\bar}. The error message will be
// cannot define \invalid. This is bad...
void Parser::get_new_command_aux(const TokenList& a)
{
  local_buf << bf_reset << "Attempt to define: ";
  err_ns::convert_to_string(a);
  err_ns::signal_error("Cannot define",0);
}

// May be signaled in the case of \hbox \foo.
void Parser::missing_open_brace()
{
  local_buf << bf_reset 
	    << "A left brace was mandatory here, so I've put one in.\n"
	    << "Argument was replaced by {}";
  err_ns::signal_error("Missing open brace",0);
}

// Error signaled in case of \end{\foo}, or \includegraphics[\bar]{}
void Parser::bad_group_char (bool test)
{
  String s1 = test ? 
    "Non-character found in environment name:\n" :
    "Non-character found in \\includegraphics:\n";
  local_buf << bf_reset << s1 << cur_tok.tok_to_str()
	    << "\nArgument was replaced by {}";
  if(test) local_buf  << "\nThe unread closing brace may signal an error";
  err_ns::signal_error("bad char",0);
}

// Error signaled when pass_test sees end of data.
void Parser::lost_if(int l)
{
  local_buf << bf_reset 
	    << "End of data reached while scanning \\fi"
	    << "\nfor condition started on line " << l;
  err_ns::signal_error("lostif",0);
}

void Parser::math_only()
{
  if(is_pos_par(nomath_code)) {
    LC();
    unprocessed_xml << cur_tok;
    return;
  }
  parse_error("Math only command ", cur_tok, "","Mathonly command");
  static bool first_time=true;
  if(first_time) {
    main_ns::log_and_tty << "(Contrarily to TeX, Tralics does not switch to math mode in such a case.)\n";
    first_time = false;
  }
}

void Parser::bad_nbargs(int k)
{
  String S = "";
  if(k==-1) S = "Only one token allowed";
  if(k==-2) S = "Commands are not allowed";
  if(k==-3) S = "Argument should be a digit";
  local_buf << bf_reset 
	    << "Bad syntax in optional argument\n"
	    << S << "\nI will assume that the command takes no argument";
  err_ns::signal_error("Bad optional in \\newcommand",0);
}

void Parser::extra_fi_or_else()
{
  String S = cur_cmd_chr.token_fiorelse_name();
  local_buf << bf_reset 
	    << "Extra \\" << S 
	    << "\nI'm ignoring this; it doesn't match any \\if";
  err_ns::signal_error("extra fi or else",0);
}

// Case of \def\foo#2{}
void Parser::bad_definition(int nb, Token name)
{
  local_buf << bf_reset
	    << "Error while scanning definition of "
	    << name.tok_to_str();
  local_buf << "\ngot #" << cur_tok.tok_to_str() << ", expected #" << nb+1; 
  err_ns::signal_error("bad char after #",0);
}

void Parser::bad_csname()
{
  if(cur_tok.is_invalid())
    local_buf << bf_reset <<"End of data reached while expecting \\endcsname";
  else {
    back_input();
    local_buf << bf_reset << "Invalid token " << cur_tok.tok_to_str()
	      << "\nfound between \\csname and \\endcsname";
  }
  err_ns::signal_error("missing endcsname",0);
}

void Parser::bad_csname(String S)
{
  local_buf << bf_reset << "Invalid token " << cur_tok.tok_to_str() 
	    << " found while scanning " << S;
  err_ns::signal_error("bad csname",0);
}

void Parser::bad_counter0()
{
  local_buf << bf_reset <<  "Invalid token " << cur_tok.tok_to_str()
	    << "\nfound in counter name";
  err_ns::signal_error("invalid cmd",0);
}

void Parser::bad_counter1(const Buffer& B,Equivalent&E)
{
  local_buf << bf_reset 
	     << (E.is_undefined() ? "Unknown counter `" : "Invalid counter `")
	     << B.c_str(2) << "'";
  err_ns::signal_error("bad counter",0);
}

// This is a hack. In some cases, we define the command after signaling 
// the error. The hack is that no <error/> element must be created.
void Parser::undefined_mac()
{
  bool noxml = the_main->get_no_undef_mac();
  local_buf << bf_reset << "Undefined command " 
	    << cur_tok.tok_to_str();
  if(!cur_cmd_chr.is_undef())
    local_buf << "; command code = " << cur_cmd_chr.get_cmd();
  err_ns::signal_error("Undefined command",noxml ? 1 : 0);
  if(noxml) {
    eq_define(cur_tok.eqtb_loc(),CmdChr(self_insert_cmd,zero_code),true);
    back_input(cur_tok);
  }
}

void Parser::undefined_env(const string& s)
{
  local_buf << bf_reset << "Undefined environment " << s;
  if(!cur_cmd_chr.is_undef())
    local_buf << "; command code = " << cur_cmd_chr.get_cmd();
  err_ns::signal_error("Undefined environment",0);
}

// This is executed when vertical mode is assumed. Current paragraph is ended
void Parser::wrong_mode(String s)
{
  local_buf << bf_reset << s
	    << "\nThe following command is not in outer vertical mode: " 
	    << cur_tok.tok_to_str();
  err_ns::signal_error("Not in outer vertical",0);
  leave_h_mode();
}

// This is called in case of \global\hbox
void Parser::prefix_error(bool b_global,symcodes K)
{
  local_buf << bf_reset << "You cannot use the prefix ";
  local_buf.dump_prefix(true, b_global,K);
  local_buf << " before " << cur_tok.tok_to_str();
  err_ns::signal_error("bad prefix",0);
}

void Parser::improper_alpha()
{
  parse_error("Improper alphabetic constant ", cur_tok,"", "bad alph");
}

void Parser::wrong_pop(String a, String b)
{
  local_buf << bf_reset << "Wrong goup delimiter\n"
	    << "got `" << a  << "', expected `" << b << "'";
  err_ns::signal_error("wrong pop",0);
}

void Parser::extra_close_brace(int cl)
{
  local_buf << bf_reset << "Extra closing brace\n"
	    << "scanning started at line " << cl 
	    << "\nfor argument to " << name_for_error.tok_to_str();
  err_ns::signal_error("extra close brace",0);
}

void Parser::bad_macro_prefix(Token x, Token C)
{
  local_buf << bf_reset <<"Use of " << name_for_error.tok_to_str();
  local_buf << " doesn't match its definition;\ngot " 
	    << x.tok_to_str() << ", expected ";
  local_buf << C.tok_to_str();
  err_ns::signal_error("bad prefix",0);
}

void Parser::invalid_key(string key, const TokenList& val)
{
  local_buf << bf_reset
	    << "Invalid key in \\includegraphics " << key << " = " << val;
  err_ns::signal_error("invalid key",0);
}


void Parser::verb_error(bool which, int msg)
{
  String s = 0;
  switch(msg) {
  case 0: s = "in argument of a command"; break;
  case 1: s = "at end of line"; break;
  case 2: s = "terminated by end of line"; break;
  default: s = ": no delimiter found";
  }
  parse_error(which? "\\verb ": "\\SaveVerb ", s, "bad \\verb");
}

void Parser::missing_equals()
{
  String s = "Missing = inserted for comparison ";
  parse_error(s, cur_tok,"",s);
  back_input(cur_tok);
}

void Parser::short_verb_error(Token t, int x)
{
  local_buf << bf_reset <<
    (x&1 ? "\\UndefineShortVerb: " : "\\DefineShortVerb: ");
  if(x==3) local_buf << "Not a short verb " << t;
  else if(x==2) local_buf << "Already a short verb " << t;
  else if(t.is_null()) local_buf <<  "One argument required";
  else local_buf << "A one-char control sequence is needed instead of " << t;
  err_ns::signal_error("Bad argument to Define/Undefine ShortVerb",0);
}


void Parser::fp_parse_error(Token a,Token b)
{
  local_buf << bf_reset  << "Non digit found " << a.tok_to_str();
  local_buf << "\nwhile scanning argument of " << b.tok_to_str();
  err_ns::signal_error("nondigit",0);
}

void Parser::counter_overflow(int n, int nmax)
{
  local_buf<<bf_reset << "Illegal counter value " << n << " for " 
	   << cur_tok.tok_to_str() <<"\n";
  if(n<=0) local_buf << "Value must be positive";
  else  local_buf << "Value must be at most " << nmax;
  err_ns::signal_error("counter overflow",0);
}

void Parser::bad_redefinition(int rd,Token T)
{
  local_buf << bf_reset << name_for_error.tok_to_str(); 	
  local_buf << ": Cannot define " << T.tok_to_str() 
	    << "; token is "
	    << (rd ==1 ? "undefined" : rd==0 ? "already defined" : "not a command");
  err_ns::signal_error("bad definition",0);
}

void Parser::multiple_label(String name, int l, string f)
{
  local_buf << bf_reset << "Label '" << name 
	    << "' multiply defined (first use line " 
	    << l << " file " << f <<")";
  err_ns::signal_error("already defined label",0);
}

// Called when unprocessed_xml is not empty.
void Parser::missing_flush()
{
  int k = unprocessed_xml.size();
  while (k > 0 &&  unprocessed_xml.is_spaceh (k-1)) k--;
  if(k==0) return;
  local_buf << bf_reset << "In " << name_for_error.tok_to_str() <<
    ", non-empty buffer \n" <<
    unprocessed_xml.to_string() <<
    "\nSome text may be lost";
  err_ns::signal_error("non-empty buffer",0);
}
