// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains a lot of stuff dealing with buffers.
#include "tralics.h"

const char* txbuffer_rcsid =
  "$Id: txbuffer.C,v 2.92 2011/06/28 08:15:25 grimm Exp $";

namespace {
  Buffer thebuffer; // a scratch buffer
  Buffer substring_buf; // another buffer
  Buffer null_cs_buffer; // buffer for \csname\endcsname
  const String gptable[] = { "pt", "fil", "fill", "filll","","mu"};
}
static Buffer local_buf; // another buffer
extern int txgethostname(char *name, size_t len);


namespace buffer_ns {
  int current_escape_char();
  String null_cs_name();
  void dump_identification(String);
};


Buffer::~Buffer()
{ 
  // cout <<"delete buffer "<< asize << " " <<name <<"\n"; 
  delete [] buf;
}

Buffer::Buffer() :  wptr(0), asize(128), ptr(0), ptr1(0) 
{ 
  buf = new char [128];
  buf[0]=0;
}

String Buffer::convert(int k) const 
{ 
  return tralics_ns::make_string(buf+k); 
}

bool is_letter (char c)
{
  return is_upper_case(c) || is_lower_case(c);
}

// This returns a copy of the string
String tralics_ns::make_string (String a)
{
  int n = strlen(a);
  the_parser.my_stats.one_more_string(n+1);
  char* res = new char[n+1];
  strcpy(res,a);
  return res;
}

// Returns of copy of the buffer as a string.
string Buffer::to_string() const 
{ 
  int n = strlen(buf);
  the_parser.my_stats.one_more_string(n+1);
  return string(buf); 
}

// Returns a copy, starting at k.
string Buffer::to_string(int k) const
{ 
  int n = strlen(buf+k);
  the_parser.my_stats.one_more_string(n+1);
  return string(buf+k); 
}


// Replaces all \r by a space.
void Buffer::no_control_M()
{
  int k = wptr;
  for(int j=0;j<k;j++)
    if(buf[j]=='\r') buf[j] = ' ';
}

void Buffer::push_back_braced(String s)
{
  push_back_open_brace(); 
  push_back(s); 
  push_back_close_brace();
}

void Buffer::push_back_braced(string s)
{
  push_back_open_brace(); 
  push_back(s); 
  push_back_close_brace();
}

// Inserts a character in the buffer. Always adds a null after it.
void Buffer::push_back(char c) 
{
  if(wptr>=asize -2) resize();
  buf[wptr] = c; 
  wptr++;
  buf[wptr] = 0;
}

// Same code, but takes a 7 bit character as argument.
// It could also be a part of a utf8 characater. Is left unchanged then.
void Buffer::push_back_xml_char (uchar c) 
{
  if(c==0) buf[ptr]=0; // may be required
  else if(c==13) push_back('\n');
  else if(c=='<')  push_back("&lt;");
  else if(c=='>')  push_back("&gt;");
  else if(c=='&')  push_back("&amp;");
  else if(c<32) push_back_ent(c); 
  else push_back(c);
}

// Converts the buffer into a string.
String Buffer::convert_to_str() const
{
  the_parser.my_stats.one_more_string(wptr+1);
  char* aux = new char[wptr+1];
  strcpy(aux,buf);
  return aux;
}

// This doubles the size
void Buffer::resize()
{
  asize *= 2;
  realloc();
}

// This reallocates to the value of size.
void Buffer::realloc()
{
  the_parser.my_stats.one_more_buffer_realloc();
  char* aux = new char[asize];
  kill_at(wptr);
  strcpy(aux,buf);
  delete [] buf;
  buf = aux;
}

// Makes sure we can add n chars in the buffer
// Note: we have wptr+1<size, so that buf[wptr] can be assigned
void Buffer::alloc(int n)
{
  n = wptr+n+1;
  if(n>= asize) {
    while(n>=asize) asize *= 2;
    realloc();
  }
}

// Inserts a string. Always inserts a null character at the end.
void Buffer::push_back(String s)
{
  int n = strlen(s);
  if(n==0) { buf[wptr] = 0; return; }
  alloc(n);
  strcpy(buf+wptr,s); 
  wptr += n;
}

void Buffer::push_back_substring(String S, int n)
{
  alloc(n);
  strncpy(buf+wptr, S,n);
  wptr += n;
  buf[wptr] = 0;
}


void Buffer::push_back_substring(const string& S, int p,int n)
{
  alloc(n);
  for(int i=0;i<n;i++) buf[wptr+i] = S[p+i];
  wptr += n;
  buf[wptr] = 0;
}

// Inserts an integer (this is the print_int procedure from TeX)
// at least one digit is printed.
void Buffer::push_back_int(int n)
{
  static int dig[30];
  int k = 0;
  if(n<0) {
    push_back('-');
    if (n>-100000000) n = -n;
    else {
      int m = -1-n;
      n = m/10; 
      m = (m%10)+1; 
      k = 1;
      if(m<10) dig[0] = m;
      else { dig[0] = 0; n++; }
    }
  }
  for(;;) {  // construct the list of digits
    dig[k] = n%10; 
    n = n/10;
    k++;
    if(n==0) break;
  }
  while(k>0) { // print the list
    k--;
    push_back('0' + dig[k]);
  }
}


// In case of error, we add the current line number as attribute 
// via this function
Istring Parser::cur_line_to_istring()
{
  Buffer&  B = local_buf;
  B.reset();
  B.push_back_int(get_cur_line());
  return Istring(B.c_str());
}


// Returns 0, 1, 2 depending on whether there is \begin or \end in the buffer
// between ptr1 and ptr.
int Buffer::is_begin_end() const
{
  if(contains_here("\\begin")) return 1;
  if(contains_here("\\end")) return 2;
  return 0;
}

// Sets ptr1 to ptr, advances ptr to after a command, returns false in case
// of failure, either because cur char is not a \, or last char is \.
bool Buffer::after_slash()
{
  if(head()!='\\')  return false;
  set_ptr1();
  advance();
  if(!head()) return false;
  if(!is_letter(head())) advance();
  else while(is_letter(head())) advance();
  return true;
}

// Sets (ptr1,ptr) to the next macro. In case of failure, returns false.
bool Buffer::next_macro()
{
  if(at_eol()) return false;
  while(head() && head() != '\\') advance();
  return after_slash();
}

// Sets ptr to the start of the next macro. Ignores non-letter macros
// Handles comments and newlines. Returns false in case of failure.
bool Buffer::next_macro_spec()
{
  for(;;) {
    if(at_eol()) return false;
    if(head()=='%') {
      while(!at_eol() && head() != '\n') advance();
      if(at_eol()) return false;
    } else if(head() !='\\')  advance();
    else if(!is_letter(after_head())) advance(2);
    else return true;
  }
}

// Sets (ptr1,ptr) to the next macro, if it is \begin or \end.
// Returns false in case of failure.
bool Buffer::next_env_spec() 
{
  for(;;) {
    if(!next_macro_spec()) return false;
    set_ptr1();
    advance();
    skip_letter();
    if(is_begin_end()) return true;
  }
}

// Searches for \begin{motscle} or \end{motscle}.
// Sets ptr1 to location of \, ptr to after closing brace.
// Returns false in case of failure.
bool Buffer::next_kw()
{
  for(;;) {
    if(!next_env_spec()) return false;
    if(contains_braced("motscle")) return true;
  }
}

// Searches for a command in the NULL terminated table.
// returns the index if found. Sets (ptr1,ptr).
int Buffer::see_something(const String* Table)
{
  reset_ptr();
  for(;;) {
    if(!next_macro()) return -1;
    ptr1++; /// skip over the backslash
    for(int k = 0;; k++) {
      if(!Table[k]) break;
      if(contains_here(Table[k])) return k;
    }
  }
}

// Splits at \begin or \end. The return value says if it's \begin or \end.
int Buffer::see_begin_end(Buffer& before, Buffer& after)
{
  ptr = 0;
  int res = 0;
  for(;;) {
    if(!next_macro()) return 0;
    res = is_begin_end();
    if(res)
      break;
  }
  after.reset0();
  after.push_back(buf+ptr);
  kill_at(ptr1);
  before.push_back(buf);
  return res;
}

// Returns the strings between ptr1 and ptr, between ptr and the end.
void Buffer::extract_strs(Buffer& A, Buffer& B)
{
  buf[ptr-1] = 0;
  A.reset(); 
  B.reset();
  B.push_back(buf+ptr);
  A.push_back(buf+ptr1);
}

// Converts the entire Buffer to lower case
void Buffer::lowercase ()
{
  for(int j=0;j<wptr;j++) 
    if(is_upper_case(buf[j])) buf[j] += 'a'-'A';
}

// Converts the entire buffer to upper case
void Buffer::uppercase ()
{
  for(int j=wptr-1;j>=0;j--)
    if(is_lower_case(buf[j])) buf[j] += 'A'-'a';
}

// Replaces newline by space
void Buffer::no_newline ()
{
  for(int j=0;j<wptr;j++) 
    if(buf[j]=='\n') buf[j] = ' ';
}

// Single brace match. When we see a newline, it will later be replaced
// by some other character. Search starts at ptr.
// Returns 0, 1 or 2. Sets ptr to the position after the char.
int Buffer::find_brace()
{

  for(;;) {
    char c = head();
    if(c==0 || c=='\n') return 0;
    advance();
    if(c=='{') return 1;
    if(c=='}') return 2;
  }
}

// Same for [] and {}
int Buffer::find_bracket()
{
  for(;;) {
    char c = head();
    if(c==0 || c=='\n') return 0;
    advance();
    if(c=='{') return 1;
    if(c=='}') return 2;
    if(c=='[') return -1;
    if(c==']') return -2;
  }
}

// We assume that there is an open brace at ptr.
// Sets ptr to the matching closiung brace. Returns false in case of trouble.
bool Buffer::full_brace_match()
{
  int level = 0;
  for(;;) {
    uchar c = next_char();
    if(c=='{') level ++;
    else if(c=='}') { level --; if (level==0) return true; }
    else if(!c) return false;
  }
}

// Finds a non hidden closing bracket.
// Assumes that braces do match. [}{] would be OK.
bool Buffer::full_bracket_match()
{
  int level = 0;
  advance();
  for(;;) {
    uchar c = next_char();
    if(c=='{') level ++;
    else if(c=='}')  level --;
    else if(c==']' && level==0) return true;
    else if(!c)  return false;
  }
}

// True if letter (or digit, hyphen, but not first char).
// Is also ok in case of underscore (Laurent Pierron)
bool Buffer::is_letter_digit() const
{
  for(int j = 0; j<wptr; j++) {
    if(is_letter(buf[j])) continue;
    if(j==0) return false;
    if(buf[j]=='-') continue;
    if(buf[j]=='_') continue;  
    if(is_digit(buf[j])) continue; 
    return false;
  }
  return true;
}

// Find a command, but in some case sets a flag if we see a dollar sign
// Stops at a comment.  If it returns true, then we have a command
// between ptr1 and ptr
bool Buffer::next_macro_spec(bool incat, int& com_loc, bool& seen_dollar)
{
  com_loc = -1;
  for(;;) {
    char c = head();
    if(c==0) return false;
    if(c=='$' &&incat) seen_dollar = true;
    if(c=='%') { com_loc = ptr; return false;}
    if(c=='\\') { after_slash(); return true; }
    advance();
  }
}

// Returns the part of the buffer between ptr1 (included) and ptr (excluded).
string Buffer::substring()
{
  char c = buf[ptr];
  buf[ptr] = 0;
  string s = c_str(ptr1);
  buf[ptr] = c;
  return s;
}

// replaces the content of the buffer by the value of B
void Buffer::copy(const Buffer& B)
{
  reset0();
  push_back(B.buf);
  ptr = B.ptr;
}

// Replaces trailing cr-lf by lf.
void Buffer::push_back_newline()
{
  if(wptr>=1 && buf[wptr-1]=='\r') // for windows
    wptr--; 
  push_back('\n');
}

// This removes (returns false) for lines starting with #
// Adds a space at first position if needed.
// Assumes that *this is NOT local_buf !!
void buffer_ns::dump_identification(String s)
{
  main_ns::log_or_tty << "Configuration file identification: " << s;
}

bool Buffer::push_back_newline_spec()
{
  push_back('\n');
  if(wptr==1) return true; // keep empty lines
  if(buf[0]=='#') {
    const String  match = "## tralics ident rc=";
    if(strncmp(buf,match,20)==0) {
      int k=20;
      while (k<wptr-1 && buf[k]!='$') k++;
      if(buf[k] == '$') {
	char c = buf[k+1];
	buf[k+1] = 0;
	buffer_ns::dump_identification(buf+20);
	buf[k+1] = c;
	main_ns::log_or_tty <<" " << buf+k+1;
      }
      else
	buffer_ns::dump_identification(buf+20);
    }
    return false;
  }
  if(buf[0]=='%' || is_space(buf[0])) return true;
  if(strncmp(buf,"Begin",5)==0) return true;
  if(strncmp(buf,"End",3)==0) return true;
  local_buf.reset0(); // THIS is thebuffer, (be careful)
  local_buf.push_back(buf);
  reset0();
  push_back(' ');
  push_back(local_buf.c_str());
  return true;
}


// This returns the number of keywords in the list.
int Splitter::count() const
{
  int n = 1;
  for(int i=0;i<size;i++)
    if(S[i]==',') n++;
  return n;
}

// This returns the next keyword
String Splitter::get_next_raw() 
{
  int p = pos;
  while(pos<size && S[pos] != ',') pos++;
  int n = pos - p;
  if(pos<size && S[pos] == ',') pos++;
  if(n==0) return "";
  Buffer&B = thebuffer;
  B.reset();
  B.push_back_substring(S,p,n);
  return B.c_str();
}

// Returns the next keyword, or empty string if there is none.
// Uses next_for_splitter to see the start and end of the keyword
String Splitter::get_next ()
{
  String T = get_next_raw();
  while(T[0] && is_space(T[0])) T++;
  if(!T[0]) return "";
  Buffer&B= local_buf;
  B.reset();
  B.push_back(T);
  B.remove_space_at_end();
  return B.c_str();
} 
// Constructs the next pair. We first call next_for_splitter,
// Assume that this puts Key=Val in the buffer. We set key and val.
// If no equals sign is given, the string is the key, the value is empty.
void Splitter::extract_keyval(string& key, string& val)
{
  key = "";
  val = "";
  String T = get_next_raw();
  if(!T[0]) return;
  int i=0;
  while(T[i]&& T[i] != '=') i++;
  if(T[i]=='=') {
    thebuffer.kill_at(i);
    val = local_buf.without_end_spaces(T+i+1);
  }
  key = local_buf.without_end_spaces(T);
}

// Return the value associated to the key x, or empty string if not found.
string SpecialHash::find(String x) const
{
  for(int i=0;i<size;i++)
    if(key[i] == x) return value[i];
  return "";
}

// This splits the string S, then creates a hash table.
void SpecialHash::create(String s)
{
  Splitter S(s);
  int n = S.count();
  key.reserve(n);
  value.reserve(n);
  size = n;
  for(int j=0;j<size;j++) {
    string a,b;
    S.extract_keyval(a,b);
    key.push_back(a);
    value.push_back(b);
  }
}

// FIXME: utf8 space ok  here ?
// This removes one space or an &nbspace;
void Buffer::remove_last_space()
{
  if(wptr>0 && is_spaceh(wptr-1))
    wptr--;
  else if(wptr>=6 && strncmp(buf+wptr-6,"&nbsp;",6)==0) wptr -= 6;
  else if(wptr>=6 && strncmp(buf+wptr-6,"&#xA;",6)==0) wptr -= 6;
  kill_at(wptr);
}

// FIXME: utf8 space ok  here ?
// This removes all spaces, and terminates the string
void Buffer::remove_space_at_end()
{
  while(wptr>0 && is_spaceh(wptr-1))
    wptr--;
  kill_at(wptr);
}

// This removes initial and final spaces. The result is a temporary.
String Buffer::without_end_spaces(String T)
{
  while(is_space(T[0])) T++;
  if(!T[0]) return "";
  reset();
  push_back(T);
  while(wptr>0 && is_spaceh(wptr-1))
    wptr--;
  kill_at(wptr);
  return buf;
}

// Returns the current escape char (used for printing)
int buffer_ns::current_escape_char()
{
  return the_parser.eqtb_int_table[escapechar_code].get_val();
}

// Inserts the current escape char, unless zero or out of range.
// This is used for transcript only
void Buffer::insert_escape_char()
{
  int c = buffer_ns::current_escape_char();
  if(c>0 && c<int(nb_characters))
    out_log(Utf8Char(c),the_main->get_log_encoding());
}

/// This one is for \meaning
void Buffer::insert_escape_char_raw()
{
  int c = buffer_ns::current_escape_char();
  if(c>0 && c<int(nb_characters))
    push_back(Utf8Char(c));
}

// Returns a temporary string, corresponding to the command with
// an empty name, without initial escape char.
String buffer_ns::null_cs_name()
{
  int c = buffer_ns::current_escape_char();
  if(c=='\\') return "csname\\endcsname";
  if(c>0 && c< int(nb_characters)) {
    Buffer&B = null_cs_buffer;
    B << bf_reset << "csname";
    B.out_log(Utf8Char(c),the_main->get_log_encoding());
    B << "endcsname";
    return B.c_str();
  } else  return "\\csname\\endcsname"; // strange otherwise
}

// Returns a temporary string: the name of the token
String Token::tok_to_str() const
{
  local_buf.insert_token0(*this);
  return local_buf.c_str();
}

// Returns a temporary string: the name of the token
String Token::tok_to_str1() const
{
  local_buf.push_back(*this);
  return local_buf.c_str();
}

// This puts the name of the token in the buffer.
// This is used when printing the token 
void Buffer::insert_token0 (Token T)
{
  reset();
  if(T.get_val()<eqtb_offset) {
    int cat = T.cmd_val(); 
    if(cat==eol_catcode) { // not real chars
      push_back(T);
      return;
    }
    subtypes c = T.chr_val();
    bool good_cat = false;
    if(c>128 && cat==12) good_cat= true;
    if(c<128 && cat==11 && is_letter(c)) good_cat = true;
    if(good_cat) 
      out_log(Utf8Char(c),the_main->get_log_encoding());
    else {
      push_back("{Character ");
      out_log(Utf8Char(c),the_main->get_log_encoding());
      push_back(" of catcode ");
      push_back_int(cat);
      push_back("}");
      return;      
    }
  }
  else push_back(T);
}

// This puts the name of the token in the buffer.
// This is used when printing a token list

// returns true if a space could be added after the token
bool Buffer::push_back(Token T)
{
  static Buffer Tmp;
  output_encoding_type enc = the_main->get_log_encoding();
  uint x = T.get_val();
  if(x==0 ) {
    T = invalid_token;
    x = T.get_val();
  }
  if(x<eqtb_offset) {
    int cmd = T.cmd_val();
    Utf8Char c = T.char_val();
    if(cmd==eol_catcode) { 
      push_back('#'); 
      push_back(uchar(c.get_value() +'0'));  // parameter
    }
    else if(cmd==parameter_catcode) { out_log(c,enc); out_log(c,enc); }
    else out_log(c,enc);
    return false;
  } else {
    if(x>=cs_token_flag+single_base) 
      insert_escape_char();
    if(x<cs_token_flag+bad_cs) {
      out_log(T.char_val(),enc);
      return
	the_parser.has_letter_catcode(T.char_val().get_value());
    } else if(x==cs_token_flag+null_cs) {
      push_back(buffer_ns::null_cs_name()); 
      return true;
    } else {
      Tmp.reset();
      Tmp.push_back(the_parser.hash_table[T.hash_loc()]);
      push_back(Tmp.convert_to_log_encoding());
      return true;
    }
  }
}

// Note: if sw, a space is added after a command, assuming standard catcode
// otherwise we check catcodes; if sw, insert backslash else escape char
// Return + if opening brace, -1 if closing brace, 0 otherwise
int Buffer::push_back(Token T,bool sw)
{
  uint x = T.get_val();
  if(x==0 ) {
    T = invalid_token;
    x = T.get_val();
  }
  if(x<cs_token_flag+single_base 
     && x != cs_token_flag+null_cs) {
    Utf8Char c = T.char_val();
    if(x<cs_token_flag+active_base) {
      int cmd = T.cmd_val();
      if(cmd==eol_catcode) { 
	push_back('#'); 
	push_back(uchar(c.get_value() +'0'));  // parameter
      }
      else if (T.is_space_token()) push_back(' ');
      else if(cmd==parameter_catcode) { push_back(c); push_back(c); }
      else if(c.is_null()) push_back("^^@");
      else push_back(c);
    } else {
      if(c.is_null()) push_back("^^@");
      else push_back(c); // active character 
    }
    if(c=='{') return 1;
    if(c=='}') return -1;
  } else {
    if(sw) push_back('\\'); else insert_escape_char_raw();
    if(x==cs_token_flag+null_cs)
      push_back("csname\\endcsname ");  // FIXME. Can this be read again?
    else if(x<cs_token_flag+bad_cs) {
      Utf8Char c = T.char_val();
      if(c.is_null()) push_back("^^@");
      else push_back (c);
      bool need_space = sw ? c.is_letter() : 
	the_parser.has_letter_catcode(c.get_value());
      if(need_space) push_back(' ');
    } else {
      push_back(the_parser.hash_table[x-cs_offset]); 
      push_back(' ');
    }
  }
  return 0;
}

// Print the scaled int V as a floating point in the buffer.
// This is the algorithm of Knuth
void Buffer::push_back(ScaledInt V, glue_spec unit)
{
  const int unity = 1<< 16;
  int s = V.get_value();
  if(s<0) {
    push_back('-');
    s = -s;
  }
  push_back_int(s/unity);
  push_back('.');
  s = 10*(s % unity)+5; 
  int delta = 10;
  for(;;) {
    if(delta>unity)
      s =s+0100000-50000; // s + 2^16/2 -10^6/2
    push_back('0'+(s / unity));
    s =10*(s %unity); 
    delta=delta*10;
    if(s<=delta) break;
  }
  push_back(gptable[unit]);
}

// Useful function for scan_dimen.
String Buffer::trace_scan_dimen(Token T, ScaledInt v, bool mu)
{
  reset();
  push_back("+scandimen for ");
  push_back(T);
  if(is_spaceh(wptr-1)) wptr--;
  push_back("->");
  push_back(v,glue_spec_pt);
  if(mu) pt_to_mu();
  push_back('\n');
  return buf;
}

// Adds glue likes 3pt plus 3 fill
void Buffer::push_back(const Glue& x)
{
  push_back(x.get_width(), glue_spec_pt);
  if(!x.get_stretch().null()) {
    push_back(" plus ");
    push_back(x.get_stretch(),x.get_stretch_order());
  }
  if(!x.get_shrink().null()) {
    push_back(" minus ");
    push_back(x.get_shrink(),x.get_shrink_order());
  }
}

// Replaces all `pt' by `mu'
void Buffer::pt_to_mu()
{
  for(int i=0;;i++) {
    if(!buf[i]) return;
    if(buf[i] != 'p') continue;
    if(buf[i+1] =='t') { buf[i] = 'm'; buf[i+1] = 'u'; }
  }
}

// The \relax is a bit strange.
void Buffer::push_back(const SthInternal& x)
{
  switch(x.get_type()) {
  case it_int:
    push_back_int(x.get_int_val()); break;
  case it_dimen: 
    push_back(ScaledInt(x.get_int_val()),glue_spec_pt);
    break;
  case it_glue:
    push_back(x.get_glue_val());
    push_back("\\relax ");
    break;
  case it_mu:
    push_back(x.get_glue_val());
    pt_to_mu();
    push_back("\\relax ");
    break;
  default: push_back("??");
  }
}


Logger& operator<<(Logger& fp, Token t)
{
  return fp << t.tok_to_str();
}


ostream& operator<<(ostream& fp, Token x)
{
  return fp << x.tok_to_str();
}

ostream& operator<<(ostream& fp, const ScaledInt& x)
{
  thebuffer.reset();
  thebuffer.push_back(x,glue_spec_pt);
  return fp << thebuffer.c_str();
}

Logger& operator<<(Logger& X,  const ScaledInt& x)
{
  *(X.fp) << x;
  return X;
}

ostream& operator<<(ostream& fp, const Glue& x)
{
  thebuffer.reset();
  thebuffer.push_back(x);
  fp << thebuffer.c_str();
  return fp;
}

Logger& operator<<(Logger& X,  const Glue& x)
{
  *(X.fp) << x;
  return X;
}

ostream& operator<<(ostream& fp, const SthInternal& x)
{
  thebuffer.reset();
  thebuffer.push_back(x);
  fp << thebuffer.c_str();
  return fp;
}

Logger& operator<<(Logger& X, const SthInternal& x)
{
  thebuffer.reset();
  thebuffer.push_back(x);
  X << thebuffer.c_str();
  return X;
}

// We use internal encoding here. 
ostream& operator<<(ostream& fp, const Utf8Char& x)
{
  if(x.is_ascii()) fp << x.ascii_value();
  else {
    thebuffer.reset();
    thebuffer.push_back(x);
    fp << thebuffer.c_str();
  }
  return fp;
}
// Duplicate code...
FullLogger& operator<<(FullLogger& fp, const Utf8Char& x)
{
  if(x.is_ascii()) fp << x.ascii_value();
  else {
    thebuffer.reset();
    thebuffer.push_back(x);
    fp << thebuffer.c_str();
  }
  return fp;
}


// We use log encoding here. 
Logger& operator<<(Logger& fp, const Utf8Char& x)
{
  if(x.is_ascii()) fp << x.ascii_value();
  else {
    thebuffer.reset();
    thebuffer.out_log(x,the_main->get_log_encoding());
    fp << thebuffer.c_str();
  }
  return fp;
}


// Puts n in roman (in upper case roman first, the loewrcasify)
void Buffer::push_back_roman(int n)
{
  int k = wptr;
  push_back_Roman(n);
  for(int i=k;i<wptr;i++)
    buf[i] += 'a' -'A';
}

// Adds n as roman upper case.
void Buffer::push_back_Roman(int n)
{
  while(n>=1000) { push_back('M'); n -= 1000; }
  if(n<=0) return;
  if(n>=900) { push_back("CM"); n -= 900; }
  if(n>=500) { push_back('D'); n -= 500; }
  if(n>=400) { push_back("CD"); n -= 400; }
  while(n>=100) { push_back('C'); n -= 100; }
  if(n>=90) { push_back("XC"); n -= 90; }
  if(n>=50) { push_back('L'); n -= 50; }
  if(n>=40) { push_back("XL"); n -= 40; }
  while(n>=10) { push_back('X'); n -= 10; }
  if(n>=9) { push_back("IX"); n -= 9; }
  if(n>=5) { push_back('V'); n -= 5; }
  if(n>=4) { push_back("IV"); n -= 4; }
  while(n>0) { push_back('I'); n --; }
}

// True is s is at ptr. If so, updates ptr
bool Buffer::is_here(String s)
{
  int n = strlen(s);
  if(strncmp(buf+ptr,s,n) !=0) return false;
  ptr += n;
  return true;
}


// True if this line contains only a comment
bool Buffer::tex_comment_line() const
{
  int j = 0;
  for(;;) {
    uchar c = buf[j];
    if(c==0) return false;
    if(c=='%') return true;
    if(!is_space(c)) return false;
    j++;
  }
}

// returns the document class. value in aux
bool Buffer::find_documentclass(Buffer&aux)
{
  String cmd = "\\documentclass";
  String s = strstr(buf,cmd);
  if(!s) return false;
  int k = s-buf;
  for(int j = 0; j<k;j++)
    if(buf[j]=='%' &&buf[j+1]=='%') return false; // double comment
  push_back("{}");  //  make sure we have braces
  k += strlen(cmd); // skip command name
  while(buf[k]==' ') ++k; // skip spaces
  if(buf[k]=='[') {
    int cur_len = size()-2;
    int cur_k = k;
    while(k<cur_len && buf[k] !=']') ++k;
    if(buf[k] ==']') ++k;
    else k = cur_k;
  }
  while(buf[k] != '{') k++;
  int p = k;
  while(buf[p] != '}') p++;
  int len = p-k-1;
  if(len==0) return false;
  aux.reset();
  aux.push_back(buf+k+1);
  aux.buf[len]=0;
  for(int i=0;i<len;i++) // \documentclass{Jos\351} is invalid
    if(!is_letter(aux[i]) && !is_digit(aux[i])) 
      return false;
  return true;
}


// returns the configuration value in aux
bool Buffer::find_configuration(Buffer&aux)
{
  if (buf[0]!='%') return false;
  String s = strstr(buf,"ralics configuration file");
  if(!s) return false;
  int k = s-buf;
  while(buf[k] && buf[k] != '\'') k++;
  if(!buf[k]) return false;
  k++;
  int len = 0;
  aux.reset();
  for(;;) {
    if(!buf[k]) return false;
    if(buf[k]=='\'') break;
    aux.push_back(buf[k]);
    len++;
    k++;
  }
  aux.buf[len]=0;
  if(len==0) return false;
  return true;
}

// returns the configuration value in aux
int Buffer::find_doctype()
{
  if (buf[0]!='%') return 0;
  String S = "ralics DOCTYPE ";
  String s = strstr(buf,S);
  if(!s) return 0;
  int k = s-buf;
  k += strlen(S);
  while(buf[k] && (buf[k] == ' ' || buf[k] == '=')) k++;
  if(!buf[k]) return 0;
  return k;
}

// If the buffer holds a single char (plus space) returns it.
// Note: this returns 0 in case of a non-7bit character
char Buffer::single_char() const
{
  int j = skip_space(0);
  if(buf[j]==0) return 0;
  char c = buf[j];
  j++;
  j = skip_space(j);
  if(buf[j]!=0) return 0;
  return c;
}

// If the buffer contains a small positive number returns it.
// Otherwise returns -1;
int Buffer::get_int_val() const
{
  int n = 0;
  for(int p=0;;p++) {
    char c = buf[p];
    if(c==0) return n;
    if(!is_digit(c)) return -1;
    n = 10*n +(c-'0');
    if(n>1000000) return -1;
  }
}

// Copies b into this. Resets pointers.
void Buffer::init_from_buffer(Buffer& b)
{
  wptr = 0;
  ptr1 = 0;
  ptr = 0;
  if(!b.empty()) push_back(b.buf);
}

bool Buffer::find_char(char c)
{
  ptr = 0;
  while(head() && head() != c)
    advance();
  return head() == c;
}

// Sets ptr1 to the first non-space
// sets ptr to the next equals sign. Returns false if no such sign exists
bool Buffer::find_equals()
{
  skip_sp_tab_nl();
  set_ptr1();
  while(head() && head() != '=')
    advance();
  return head() == '=';
}

// Ignores character at ptr, and following ones.
// removes the spaces just before.
// puts a null char there.
// returns false in case of trouble (only spaces).
bool Buffer::backup_space()
{
  int j = ptr;
  while(j>ptr1 &&is_spaceh(j-1))
    j --;
  if(j==ptr1) return false;
  kill_at(j);
  return true;
}

// If there is 'foobar' at ptr, then puts ptr1 to the char after 
// the quote, replaces the second quote by a null.
// returns false in  case of trouble.
bool Buffer::string_delims()
{
  skip_sp_tab_nl();
  char c = head();
  if(!c) return false;
  advance();
  set_ptr1();
  while(head() && head() != c) advance();
  if(!head()) return false;
  kill_at(ptr);
  return true;
}

// Assumes the buffer is of the form foo/bar/etc, 
// with a final slash; returns the next item; Retval false if no string found

bool Buffer::slash_separated(string&a)
{
  static Buffer tmp;
  tmp.reset();
  int p = 0;
  skip_sp_tab();
  if(!head()) return false;
  for(;;) {
    char c = head();
    if(!c) return false;
    advance();
    if(c== '/') break;
    if(c=='\\') {
      if(!head()) return false;
      if(head()==' ') { p = tmp.size()+1; }
      c = head();
      advance();
    }
    tmp.push_back(c);
  }
  int b = tmp.size();
  while(b>p&&is_space(tmp.buf[b-1])) b--;
  tmp.kill_at(b);
  a = tmp.to_string();
  return true;
}

// Returns the substring between a and b. 
String Buffer::some_substring(int a, int b)
{
  int c = b-a;
  char* S = new char [c+1];
  for(int j=0;j<c;j++)
    S[j] = buf[a+j];
  S[c] = 0;
  return S;
}

// Returns the substring between a and b. 
// Assume this is not substring_buf
string Buffer::some_sub_string(int a, int b)
{
  int c = b-a;
  substring_buf.reset();
  substring_buf.alloc(c+1);
  for(int j=0;j<c;j++)
    substring_buf.push_back(buf[a+j]);
  return substring_buf.to_string();
}

void Buffer::push_back_unless_punct(char c)
{
  if(wptr>=6 && strncmp(buf+wptr-6,"&nbsp;",6)==0)
    return;
  if(wptr>0 && is_spaceh(wptr-1))
    return;
  if(wptr>0) {
    char test = buf[wptr-1];
    if(test=='(') return;
  }
  push_back(c);
}


// Tries to read an argument. Sets ptr1 to after the opening brace
// ptr to the closing brace.
bool Buffer::fetch_spec_arg()
{
  skip_sp_tab_nl();
  if(head() != '{') return false;
  advance();
  set_ptr1();
  for(;;) {
    uchar c = uhead();
    if(c==0 || c=='{' || c=='%') return false;
    if(c=='}') return true;
    advance();
  }
}

// Returns true if the string S is between ptr1 and ptr.
bool Buffer::contains_here(String s) const
{
  int k = ptr1,j=0;
  for(;;) {
    char c = s[j];
    if(c==0) return k==ptr;
    if(c!=buf[k]) return false;
    k++; j++;
  }
}

// Returns true if the buffer contains the string s with braces.
// After that, ptr1 is reset, ptr set to after the closing brace.
bool Buffer::contains_braced(String s)
{
  int k = get_ptr1();
  if(!fetch_spec_arg()) return false;
  if(!contains_here(s)) return false;
  set_ptr1(k);
  advance();
  return true;
}

// Returns true if the buffer contains only \end{env}
// (used for detecting the end of a verbatim environment).
bool Buffer::contains_env(String env) 
{
  skip_sp_tab_nl();
  ptr1 = ptr;
  ptr = ptr1+4;
  if(!contains_here("\\end")) return false;
  skip_sp_tab_nl();
  if(!(contains_braced(env))) return false;
  skip_sp_tab_nl();
  if(head()=='\r') ptr++;
  return true;
}

// returns true if the file exists with extension s.
bool Image::file_exists(String s)
{
  local_buf << bf_reset << name << '.' << s;
  return tralics_ns::file_exists(local_buf.c_str());
}

// This checks all possible extensions and remembers them
void Image::check_existence()
{
  flags = 0;
  if(file_exists("ps")) flags |= 1;
  if(file_exists("eps")) flags |= 2;
  if(file_exists("epsi")) flags |= 4;
  if(file_exists("epsf")) flags |= 8;
  if(file_exists("pdf")) flags |= 16;
  if(file_exists("png")) flags |= 32;
  if(file_exists("gif")) flags |= 64;
}

Buffer check_image1;
Buffer check_image2;
// This checks that there is a unique source for the image
void Image::check()
{
  int a = flags&1 ? 1 : 0;
  int b = flags&2 ? 1 : 0;
  int c = flags&4 ? 1 : 0;
  int d = flags&8 ? 1 : 0;
  int e = a+b+c+d;
  if(e>1) { 
    if(!check_image1.empty()) check_image1 << ", ";
    check_image1 <<  name;
  }
  if(flags==0) { 
    if(!check_image2.empty()) check_image2 << ", ";
    check_image2 <<  name;
  }
}

// Enter a new image file, if ok is false, do not increase the occ count
void Parser::enter_file_in_table(const string& nm, bool ok)
{
  int s = the_images.size();
  for(int i=0;i<s;i++) {
    Image& X = the_images[i];
    if(X.name==nm) {
      if(ok) X.occ++;
      return;
    }
  }
  the_images.push_back(Image(nm,ok?1:0));
}

void operator<< (fstream& X, const Image& Y)
{
  X << "see_image(\"" << Y.name << "\"," ;
  int k = Y.flags;
  if(!k) X << 0;
  else {
    bool first = true;
    if(k&1) { if(!first) X<< "+"; X<< 1; first = false; }
    if(k&2) { if(!first) X<< "+"; X<< 2; first = false; }
    if(k&4) { if(!first) X<< "+"; X<< 4; first = false; }
    if(k&8) { if(!first) X<< "+"; X<< 8; first = false; }
    if(k&16) { if(!first) X<< "+"; X<< 16; first = false; }
    if(k&32) { if(!first) X<< "+"; X<< 32; first = false; }
    if(k&64) { if(!first) X<< "+"; X<< 64; first = false; }
  }
  X <<"," << Y.occ << ");\n";
}

// finish handling the images,
void Parser::finish_images()
{
  int s = the_images.size();
  if(s==0) return;
  string name = tralics_ns::get_short_jobname() + ".img";
  String wn = tralics_ns::get_out_dir(name);
  fstream* fp = new fstream(wn,std::ios::out);
  if(!*fp) return;
  *fp << "# images info, 1=ps, 2=eps, 4=epsi, 8=epsf, 16=pdf, 32=png, 64=gif\n";
  check_image1.reset();
  check_image2.reset();
  for(int i=0;i<s;i++) {
    if(the_images[i].occ) {
      the_images[i].check_existence();
      the_images[i].check();
      *fp << the_images[i];
    }
  }
  fp->close();
  delete fp;
  if(!s)
    main_ns::log_or_tty << "There was no image.\n";
  else
    main_ns::log_or_tty << "There were " << s << " images.\n";
  if(!check_image1.empty())
    main_ns::log_or_tty << "Following images have multiple PS source: " 
			<< check_image1 << ".\n";
  if(!check_image2.empty()) 
    main_ns::log_or_tty << "Following images not defined: " 
			<< check_image2 << ".\n";
}

string Buffer::get_machine_name()
{
  reset();
  alloc(200);
  if(txgethostname(buf,199)!=0) push_back("unknown");
  buf[200] = 0;
  wptr = strlen(buf);
  int n = wptr;
  for(int i=1;i<n;i++) if(buf[i]=='.') kill_at(i);
  return to_string();
}

// Adds a slash at the end unless there is already one.
// 
void Buffer::optslash()
{
  if(wptr==0) return;
  else if(wptr==1 && buf[0]=='/') reset();
  else if(buf[wptr-1]=='/')  return;
  else push_back('/');
}

// returns location of last slash in the buffer
int Buffer::last_slash() const
{
  int k = -1;
  for(int i=0;i<wptr;i++) if (buf[i] == '/') k = i;
  return k;
}

// True if the string s is at the end of the buffer
bool Buffer::is_at_end(String s) const
{
  int n = strlen(s);
  return wptr>n && strcmp(buf+wptr-n,s)==0;
}

// Inserts the string s is at the end of the buffer unless there
void Buffer::put_at_end(String s)
{
  int n = strlen(s);
  if(wptr>n && strcmp(buf+wptr-n,s)==0) return;
  push_back(s);
}
