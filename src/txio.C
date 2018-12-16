// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Functions on files and characters;
// Handle also utf8 input output

#include "tralics.h"
#include <sstream>
const char* txio_rcsid =
  "$Id: txio.C,v 2.42 2012/05/15 17:14:30 grimm Exp $";

extern void readline(char*buffer,int screen_size);

static const int lmaxchar =256;
static const int max_encoding =34;


namespace {
  Buffer thebuffer;
  bool log_is_open = false;
  Buffer utf8_out; // Holds utf8 outbuffer
  Buffer utf8_in; // Holds utf8 inbuffer
  Converter the_converter;
  int custom_table[max_encoding-2][lmaxchar];
}


bool opening_main = false;

namespace main_ns {
  void register_file(LinePtr*);
  bool use_pool(LinePtr&L);
}

namespace io_ns {
  void print_ascii(ostream& fp, uchar c);
  int how_many_bytes(uchar);
  Utf8Char make_utf8char(uchar A, uchar B,uchar C,uchar D);
  String plural(int n);
  void check_for_encoding();
  void set_enc_param(int enc,int pos,int v);
  int get_enc_param(int enc,int pos);
  int find_encoding(String cl);
  void show_encoding (int wc, const string& name);
}

// ---------------------------------------------------------
 

FullLogger& operator <<(FullLogger& X, String s) 
{
   if(X.verbose)  cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, Istring s) 
{
   if(X.verbose)  cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, int s) 
{
   if(X.verbose)  cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, char s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, unsigned char s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, const Buffer& s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, const string& s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, const TokenList& s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator <<(FullLogger& X, const Xml* s) 
{
   if(X.verbose) cout << s; X.L << s; return X;
}

FullLogger& operator<<(FullLogger& X,  const ScaledInt& x)
{
   if(X.verbose) cout << x; X.L << x;  return X;
}

FullLogger& operator<<(FullLogger& fp, Token t)
{
  return fp << t.tok_to_str();
}



HalfLogger& operator <<(HalfLogger& X, String s) 
{
  if(X.verbose) cout << s; X.L << s; return X;
}

HalfLogger& operator <<(HalfLogger& X, Istring s) 
{
  if(X.verbose) cout << s; X.L << s; return X;
}

HalfLogger& operator <<(HalfLogger& X, int s) 
{
  if(X.verbose) cout << s; X.L << s; return X;
}
HalfLogger& operator <<(HalfLogger& X, const string& s) 
{
  if(X.verbose) cout << s; X.L << s; return X;
}


// Prints an att list on a buffer, then a stream.
void AttList::print(ostream& fp)
{
  thebuffer.reset();
  thebuffer.push_back(*this);
  fp << thebuffer;
}

// Prints an att list on a stream.
ostream& operator<<(ostream&fp, Xid X)
{
  X.get_att().print(fp);
  return fp;
}


// ---------------------------------------------------------
// Output methods for characters

// This prints a character in the form \230 it not ascii
// Can be used in case where encoding is strange

void io_ns::print_ascii(ostream& fp, uchar c)
{
  if(c<127) fp << c;
  else {
    uint z = c&7;
    uint y = (c>>3) &7;
    uint x = (c>>6) &7;
    fp << "\\" << uchar(x+'0') << uchar(y+'0') << uchar(z + '0'); 
  }
}

// returns true if only asci 7 bits in the buffer
bool Buffer::is_all_ascii() const
{
  for(int i=0;i<wptr; i++) {
    uchar c = buf[i];
    if(c>=128) return false;
    if(c<32 && c !='\t' && c!='\n') return false; 
  }
  return true;
}

// returns false if some unpritable characters appear
// Non ascii chars are printable
bool Buffer::is_good_ascii() const
{
  for(int i=0;i<wptr; i++) {
    uchar c = buf[i];
    if(c<32 && c !='\t' && c!='\n') return false; 
  }
  return true;
}

// ------------------------------------------------------------------------
// Functions that extract utf8 characters from streams and buffers

Converter::Converter() : cur_file_line(0), global_error(false),
			 local_error(false), line_is_ascii(false),
			 lines_converted(0),bad_lines(0),bad_chars(0)
{
  cur_file_name = "tty";
}

String io_ns::plural(int n)
{
  if(n>1) return "s"; else return "";
}

void Converter::stats()
{
  if(bad_lines) {
    main_ns::log_or_tty << "Input conversion errors: " 
			<< bad_lines << " line" << io_ns::plural(bad_lines)
			<< ", " 
			<< bad_chars << " char" << io_ns::plural(bad_chars)
			<< ".\n";
  }
  if(lines_converted)
    main_ns::log_or_tty << "Input conversion: " << lines_converted << 
      " line" << io_ns::plural(lines_converted) << " converted.\n";
}

void print_cv_stats()
{
  the_converter.stats();
}



// If an error is signaled on current line, we do not signal again
// We mark current char as invalid
bool Converter::new_error()
{
  local_error = true;
  if(global_error) return true;
  bad_lines ++;
  global_error = true;
  return false;
}

// Action when starting conversion of line l of current file
void Converter::start_convert(int l)
{
  cur_file_line = l;
  global_error = false;
  line_is_ascii = true;
}

// This is called in case of error in the utf8 parser
// In case of error, we print characters as \230
// If first is false, we look at the second byte; ptr is not yet incremented
// for the case second char is missing.
void Buffer::utf8_error(bool first)
{
  Converter &T = the_converter;
  T.bad_chars++;
  main_ns::log_and_tty << "UTF-8 parsing error (line " << T.cur_file_line 
		       << ", file " << T.cur_file_name 
		       << (first ? ", first byte" : ", continuation byte") 
		       << ")\n";
  main_ns::log_and_tty.L << "Position in line is " << ptr << "\n";
  if(T.new_error()) return; // signal only one error per line
  for(int i=0;i<wptr;i++) io_ns::print_ascii( *(the_log.fp), buf[i]);
  the_log << "\n";
}



void Buffer::utf8_ovf(int n)
{
  Converter &T = the_converter;
  thebuffer.reset(); 
  thebuffer.push_back16u (n);
  main_ns::log_and_tty << "UTF-8 parsing overflow (char " << thebuffer
		       << ", line " << T.cur_file_line 
		       << ", file " << T.cur_file_name << ")\n";
  T.bad_chars++;
  T.new_error();
}

// This reads the next byte. 
// We assume buf[wptr]=0. We leave ptr unchanged in case it is >=ptr
// As a consequence, buf[ptr] is valid after the call
uchar Buffer::next_utf8_byte ()
{
  uchar x = buf[ptr];
  if ((x >> 6) == 2) { ++ptr; return x&63; }
  utf8_error(false);
  if(ptr<wptr) ++ptr;
  return 0;
}

// This returns the number of bytes in a UTF8 character
// given the first byte. Returns 0 in case of error
int io_ns::how_many_bytes(uchar C)
{
  if(C<128) return 1; // ascii
  if((C>>5) == 6) return 2;  // 2 bytes
  if((C>>5) != 7) return 0;  // cannot start with 10
  uint y = C & 31;
  if((y&16)==0) return 3;
  if(y==16 || y==17 || y==18 || y==19) return 4;
  return 0; // overflow
}

// Creates a Unicode character from the bytes A, B, C and D.
// Return 0 if invalid. Return 0 if overflow
Utf8Char io_ns::make_utf8char(uchar A, uchar B, uchar C, uchar D)
{
  int n = io_ns::how_many_bytes(A);
  if(n==0) return 0;
  else if(n==1) return A;
  else if(n==2) return ((A&31) <<6) + (B&63);
  else if(n==3) return (C&63) +((B&63) <<6) + ((A&31)<<12); 
  else return (D&63) + ((C&63)<<6) + ((B&63)<<12) + ((A&3)<<18);
}

// Returns 0 at end of line or error
// may set local_error
Utf8Char Buffer::next_utf8_char_aux()
{
  unsigned char c = next_char();
  if(c==0) return 0; 
  int n = io_ns::how_many_bytes(c);
  
  if(n==0) {
    utf8_error(true);
    return 0;
  }
  if(n==1) return c;
  the_converter.line_is_ascii = false;
  if(n==2) {
    uint x = next_utf8_byte(); 
    return ((c&31) <<6) + x;
  } else if(n==3) {
    uint z = next_utf8_byte(); 
    uint x = next_utf8_byte();
    return x +(z <<6) + ((c&31)<<12); 
  } else { 
    uint z = next_utf8_byte(); 
    uint y = next_utf8_byte(); 
    uint x = next_utf8_byte(); 
    return (x) + (y<<6) + (z<<12) + ((c&3)<<18);
  }
}

// Returns 0 at end of line or error
Utf8Char Buffer::next_utf8_char()
{
  the_converter.local_error = false;
  Utf8Char res = next_utf8_char_aux();
  if(the_converter.local_error) return 0;
  else if(res.is_verybig()) {
    utf8_ovf(res.get_value());
    return 0;
  }
  else return res;
}

// If the buffer contains a unique character, return it
// Otherwise return 0. No error signaled
Utf8Char Buffer::unique_character() const
{
  uchar c= buf[0];
  int n = io_ns::how_many_bytes(c);
  if(n==0) return 0;
  if(n!=size()) return 0;
  if(n==1) return c;
  if(n==2) return io_ns::make_utf8char(buf[0],buf[1], 0,0);
  if(n==3) return io_ns::make_utf8char(buf[0],buf[1], buf[2],0);
  if(n==4) return io_ns::make_utf8char(buf[0],buf[1], buf[2],buf[3]);
  return 0;
}

// This converts a line in UTF8 format. Returns true if no conversion needed
// Otherwise, the result is in utf8_out.
bool Buffer::convert_line0 (int wc)
{
  Buffer& res = utf8_out;
  res.reset();
  reset_ptr();
  Utf8Char c =0;
  for(;;) {
    if(wc==0) c = next_utf8_char();
    else {
      uchar C = next_char();
      if(wc==1) 
	c = C;
      else
	c = custom_table[wc-2][C]; // Tbl[wc-2][C]
      if(!(c.is_ascii() && c==C)) the_converter.line_is_ascii = false;
    }
    if(c.non_null()) res.push_back(c); 
    if(at_eol()) break;
  }
  return the_converter.line_is_ascii;
}


// This converts a line in UTF8 format
// Result of conversion is pushed back in the buffer
void Buffer::convert_line (int l, int wc)
{
  the_converter.start_convert(l);
  if(convert_line0(wc)) return;
  the_converter.lines_converted++;
  reset();
  push_back(utf8_out.c_str());  
}

// This converts a line of a file 
void Clines::convert_line(int wc)
{
  utf8_in.reset();
  utf8_in.push_back(chars);
  converted = true;
  the_converter.start_convert(get_number());
  if(utf8_in.convert_line0(wc)) return;
  the_converter.lines_converted++;
  set_chars(utf8_out.c_str());
}


void io_ns::check_for_encoding()
{
  for(int i=0; i<max_encoding-2; i++)
    for(int j=0;j<lmaxchar;++j)
      custom_table[i][j] = j;
}

void io_ns::set_enc_param(int enc,int pos, int v)
{
  if(!(enc>=2 && enc< max_encoding) ) {
    thebuffer << bf_reset <<  "Illegal encoding " <<  enc ;
    the_parser.parse_error(thebuffer.c_str());
    return;
  }
  enc -= 2;
  if(!(pos>=0 && pos<lmaxchar)) {
    thebuffer << bf_reset << "Illegal encoding position "  << pos ;
    the_parser.parse_error(thebuffer.c_str());
    return;
  }
  if(0<v && v< int(nb_characters)) custom_table[enc][pos] = v;
  else custom_table[enc][pos] = pos;
}

int io_ns::get_enc_param(int enc,int pos)
{
  if(!(enc>=2 && enc< max_encoding) ) return pos;
  enc -= 2;
  if(!(pos>=0 && pos<lmaxchar)) return pos;
  return custom_table[enc][pos];
}

void MainClass::set_input_encoding(int wc) 
{ 
  if(wc>=0 && wc< max_encoding) {
    input_encoding = wc;
    the_log << "++ Default input encoding changed to " << wc << "\n";
  }
}

void LinePtr::change_encoding(int wc)
{
  if(wc>=0 && wc< max_encoding) {
    cur_encoding = wc;
    the_log << "++ Input encoding changed to " << wc << " for " << file_name << "\n";
  }
}

// -----------------------------------------------------------------
// Reading characters from files

int io_ns::find_encoding(String cl)
{
  if (strstr (cl, "-*-")) {
    if(strstr(cl,"coding: utf-8")) return 0;
    if(strstr(cl,"coding: utf8")) return 0;
    if(strstr(cl,"coding: latin1")) return 1;
    if(strstr(cl,"coding: iso-8859-1")) return 1;
  }
  if(strstr(cl,"iso-8859-1")) return 1;
  if(strstr(cl,"utf8-encoded")) return 0;
  if(strstr(cl,"%&TEX encoding = UTF-8")) return 1;
  String s = strstr(cl,"tralics-encoding:");
  if(!s) return -1;
  if(!is_digit(s[17])) return -1;
  int k = s[17]-'0';
  if(is_digit(s[18])) { k = 10* k + s[18]-'0';}
  if(k< max_encoding) return k;
  return -1;
}

void LinePtr::set_interactive()
{ 
  interactive = true;
  file_name = "tty";
  cur_encoding = the_main->get_input_encoding();
}

// interface with the line editor.
int LinePtr::read_from_tty(Buffer& B)
{
  static bool prev_line = false; // was previous line non-blank ?
  static char m_ligne [4096];
  readline(m_ligne,78);
  if(strcmp(m_ligne,"\\stop")==0) return -1;
  cur_line ++;
  B.reset0();
  B << m_ligne << "\n";
  if(B.length()==1) {
    if(!prev_line) cout << "Say \\stop when finished, <ESC>-? for help.\n";
    prev_line = false;
  } else prev_line = true;
  if(B[0]=='%') { // debug
    int k = io_ns::find_encoding(B.c_str());
    if(k>=0) cur_encoding = k;
  }
  return cur_line;
}

// inserts a copy of aux
void LinePtr::insert(const LinePtr& aux)
{
  cur_encoding = 0;
  line_iterator_const C = aux.value.begin();
  line_iterator_const E = aux.value.end();
  while(C!=E) {
    Clines L = *C;
    L.clear_converted();
    push_back(L);
    ++C;
  }
}


// This reads the file named x. 
// If spec is 0, we are reading the config file.
// If 2 it's a tex file, and the file is converted later.
// If 3, no conversion  done 
// If 4, its is the main file, log not yet open.
void tralics_ns::read_a_file (LinePtr& L,string x, int spec)
{
  L.reset(x);
  if(main_ns::use_pool(L)) return;
  fstream* fp = new fstream(x.c_str(),std::ios::in);
  if(!fp) return ;
  string old_name = the_converter.cur_file_name;
  the_converter.cur_file_name = x;
  Buffer B;
  int wc = the_main->get_input_encoding();
  if (spec == 4) wc = -1;
  bool converted = spec <2 ;
  L.set_encoding(the_main->get_input_encoding());
  int co_try = spec==3 ? 0 : 20;
  for(;;) {
    int c = fp->get();
    bool emit = false;
    if(c=='\r') { // pc or mac ?
      emit = true;
      c = fp->peek();
      if(c == '\n')  fp->ignore();
    }
    else if(c=='\n') emit = true;
    else if(c==EOF) {
      if(!B.empty()) emit = true;
      fp->close();
      delete fp;
      the_converter.cur_file_name = old_name;
    } else
      B.push_back(c);
    if (emit) {
      if(spec==0) emit = B.push_back_newline_spec();
      else B.push_back_newline();
      if(co_try) {
	co_try --;
	int k = io_ns::find_encoding(B.c_str());
	if(k>=0) {
	  wc = k;
          L.set_encoding (k);
	  co_try = 0; 
	  the_log  << "++ Input encoding number "<< k << 
	    " detected  at line " << L.get_cur_line()+1 << 
	    " of file " << x <<  "\n";
        }
      }
      if(converted) B.convert_line(L.get_cur_line() +1,wc);
      if(emit) L.insert(B.to_string(),converted);
      else L.incr_cur_line();
      B.reset();
    }
    if(c==EOF) break;
  }
}


void io_ns::show_encoding (int wc, const string& name)
{
  const string& wa =
    (wc== 0?  " (UTF8)" : (wc == 1 ?  " (iso-8859-1)" : " (custom)"));
  the_log << "++ Input encoding is " << wc << wa <<" for " << name << "\n";
}

// If a line ends with \, we take the next line, and append it to this one
void LinePtr::normalise_final_cr ()
{
  line_iterator C = value.begin();
  line_iterator E = value.end();
  if(C==E) return;
  Clines*prev=0;
  Clines*cur=0;
  for(;;) {
    if(C==E) return;
    cur = &*C;
    ++C;
    const string& s = cur->get_chars();
    int n = s.size();
    bool special = (n>=2 && s[n-2]=='\\' && s[n-1]=='\n');
    string normal = s;
    if(special) 
      normal = string(s,0,n-2);
    if(prev) {
      prev->set_chars(prev->get_chars()+ normal);
      cur->set_chars("\n");
    }
    if(special) {
      if(!prev) {
	prev = cur;
	prev->set_chars(normal);
      }
    }
    else
      prev = 0;
  }
}

// ------------------------------------------------------

// This puts x into the buffer in utf8 form
void Buffer::push_back(Utf8Char c)
{
  uint x = c.get_value();
  if(x<128) { push_back(uchar(x)); return; }
  int x1 = x >> 18;
  int x2 = (x>>12) & 63;
  int x3 = (x>>6) & 63;
  int x4 = x&63;
  if(x1>0 || x2>=16) {
    push_back(uchar(x1+128+64+32+16));
    push_back(uchar(x2+128));
    push_back(uchar(x3+128));
    push_back(uchar(x4+128));
  } else if(x2>0 || x3>=32) {
    push_back(uchar(x2+128+64+32));
    push_back(uchar(x3+128));
    push_back(uchar(x4+128));
  } else {
    push_back(uchar(x3+128+64));
    push_back(uchar(x4+128));
  }
}

inline void Buffer::push_back_hex (uint c)
{
  if(c<10) push_back(c+'0'); else push_back(c+'a'-10);
}

inline void Buffer::push_back_Hex (uint c)
{
  if(c<10) push_back(c+'0'); else push_back(c+'A'-10);
}

void Buffer::push_back16u(uint n)
{
  push_back("U+"); push_back16(n);
}

void Buffer::push_back16(uint n)
{ 
  static uint dig[9]; 
  int k = 0;
  for(;;) {  // construct the list of digits
    dig[k] = n%16; 
    n = n/16;
    k++;
    if(n==0) break;
  }
  while(k>0) { // print the list
    k--;
    push_back_Hex(dig[k]);
  }
}

// Converts number in lower hex form
// if hat is true, inserts hats before
void Buffer::push_back16l(bool hat,uint n)
{ 
  static uint dig[9]; 
  int k = 0;
  for(;;) {  // construct the list of digits
    dig[k] = n%16; 
    n = n/16;
    k++;
    if(n==0) break;
  }
  if (hat) {
    int res = k;
    while (k>0) { k--; push_back('^'); }
    if(res == 3) { push_back('^'); push_back ('0') ; }
    k = res;
  }
  while(k>0) { // print the list
    k--;
    push_back_hex(dig[k]);
  }
}

// This puts a 16bit char in the form ^^^^abcd in the buffer.
// Uses ^^ab notation if better
void Buffer::out_four_hats (Utf8Char ch)
{
  if(ch=='\n') { push_back('\n'); return; }
  if(ch=='\r') { push_back('\r'); return; }
  if(ch=='\t') { push_back('\t'); return; }
  uint c = ch.get_value();
  if(ch.is_control()) {
    push_back("^^"); 
    push_back(uchar(c+64)); 
  } else if(ch.is_delete()) push_back("^^?");
  else if(ch.is_ascii()) push_back(uchar(c));
  else push_back16l(true,c);
}


// This inserts &#xabc;
void Buffer::push_back_ent(Utf8Char ch)
{
  int c = ch.get_value();
  if(c == 65534 ||c==65535) return; // these chars are illegal
  push_back('&');
  push_back('#');
  push_back('x');
  push_back16(c);
  push_back(';');
}

// This handles the case of \char 1234, where the number is at least 2^16
// Uses hex representation.
void Buffer::process_big_char(uint n)
{
  push_back('&');
  push_back('#');
  push_back('x');
  push_back16(n); 
  push_back(';');
}

// This is the function that puts a character into the buffer to be as XML
// We must handle some character. We use entities in case of big values
// or control characters.

void Parser::process_char (Utf8Char c) 
{
  if(c.is_null()) unprocessed_xml.push_back(""); // may be required
  else if(c=='\n') unprocessed_xml.push_back('\n');
  else if(c=='\r') unprocessed_xml.push_back('\r');
  else if(c=='\t') unprocessed_xml.push_back('\t');
  else if(c=='<')  unprocessed_xml.push_back("&lt;");
  else if(c=='>')  unprocessed_xml.push_back("&gt;");
  else if(c=='&')  unprocessed_xml.push_back("&amp;");
  else if(c.is_control() || c.is_big()) unprocessed_xml.push_back_ent(c); 
  else unprocessed_xml.push_back(c);
}

void Buffer::push_back_real_utf8 (Utf8Char c) 
{
  if(c.is_null()) push_back(""); // may be required
  else if(c=='\n') push_back('\n');
  else if(c=='\r') push_back('\r');
  else if(c=='\t') push_back('\t');
  else if(c=='<')  push_back("&lt;");
  else if(c=='>')  push_back("&gt;");
  else if(c=='&')  push_back("&amp;");
  else if(c.is_control() || c.is_big()) push_back_ent(c); 
  else push_back(c);
}

// Assumes that c is not a special char
void Parser::process_char (uchar c)
{
  if(c<128) unprocessed_xml.push_back(c);
  else
    process_char(Utf8Char(c));
}


void Parser::process_char (uint c)
{
  process_char(Utf8Char(c));
}


// This dumps a single character using log method
void Buffer::out_log (Utf8Char ch,output_encoding_type T)
{
  if(ch=='\n') push_back('\n');
  else if(ch=='\r') push_back("^^M"); 
  else if(ch=='\t') push_back('\t'); 
  else if(ch.is_control()) out_four_hats(ch);
  else if(ch.is_ascii())  push_back(uchar(ch.get_value()));
  else if(T==en_utf8) push_back(ch);
  else if(ch.is_small() && T==en_latin) push_back(uchar(ch.get_value()));
  else out_four_hats(ch);
}

// Converts the buffer to the output encoding
// If conversion is trivial, returns string a.
String Buffer::convert_to_out_encoding(String a) const
{
  output_encoding_type T = the_main->get_output_encoding();
  if(T==en_boot || T== en_utf8) return a;
  if(is_all_ascii()) return a;
  return convert_to_latin1(T==en_latin);
}

// Convert to latin 1 or ASCII
String Buffer::convert_to_latin1(bool nonascii) const
{
  Buffer& I = utf8_in;
  Buffer& O = utf8_out;
  I.reset(); I.push_back(buf);
  the_converter.global_error= false;
  O.reset();
  I.reset_ptr();
  Utf8Char c =0;
  for(;;) {
    c = I.next_utf8_char();
    if(c.is_null() && I.at_eol()) break;
    if(c.is_null()) continue;
    if(c.is_ascii()) O.push_back(c.char_val());
    else if(c.is_small() && nonascii) O.push_back(c.char_val());
    else O.push_back_ent(c); 
  }
  return O.convert_to_str();
}

// Returns the buffer, converted into log encoding, to be printed
// on the transcript file.
String Buffer::convert_to_log_encoding() const 
{
  output_encoding_type T = the_main->get_log_encoding();
  if(is_all_ascii() || (T==en_utf8 && is_good_ascii()))
    return c_str();
  Buffer& I = utf8_in;
  I.reset(); 
  I.push_back(buf);
  the_converter.global_error= false;
  I.reset_ptr();
  Buffer& O = utf8_out;
  O.reset();
  for(;;) {
    Utf8Char c = I.next_utf8_char();
    if(c==0) {
      if(I.at_eol()) break;
      else O << "<null>";
    } else if(c=='\r') O << "^^M";
    else O.out_log(c,T);
  }
  return O.c_str();
}

void Buffer::extract_chars(vector<Utf8Char>& V)
{
  the_converter.start_convert(the_parser.get_cur_line());
  V.clear();
  reset_ptr();
  for(;;) {
    Utf8Char c = next_utf8_char();
    if(c==0 && at_eol()) return;
    V.push_back(c);
  }
}

// --------------------------------------------

// Replaces e-accent by e.
void Buffer::to_seven_bits()
{ 
  if(is_all_ascii()) return;
  Buffer& I = utf8_in;
  I.reset(); I.push_back(buf);
  the_converter.global_error= false;
  I.reset_ptr();
  reset();
  for(;;) {
    Utf8Char c = I.next_utf8_char();
    if(I.at_eol())   remove_space_at_end();return;
    int x = c.get_value();
    if(x<128) push_back(x); continue; 
    if(x>256) push_back('?'); continue; 
    switch(x) {
    case 0: continue;
    case '\300':  
    case '\301':  
    case '\302':  
    case '\303':  
    case '\304':  
    case '\305':  push_back('A'); continue; 
    case '\306':  push_back('A'); push_back('E'); continue; 
    case '\307':  push_back('C'); continue; 
    case '\310':  
    case '\311':  
    case '\312':  
    case '\313':  push_back('E'); continue;  
    case '\314': 
    case '\315':   
    case '\316':  
    case '\317': push_back('I'); continue;  
    case '\340':  
    case '\341':  
    case '\342':  
    case '\343':  
    case '\344':  
    case '\345':  push_back('a'); continue; 
    case '\346':  push_back('a'); push_back('e'); continue; 
    case '\347':  push_back('c'); continue; 
    case '\350':  
    case '\351':  
    case '\352':  
    case '\353':  push_back('e'); continue;  
    case '\354': 
    case '\355':   
    case '\356':  
    case '\357': push_back('i'); continue;  
    case '\361': push_back('n'); continue;  
    case '\321': push_back('N'); continue;  
    case '\362':
    case '\363':
    case '\364':
    case '\365':
    case '\366': 
    case '\370': push_back('o'); continue;  
    case '\322':
    case '\323':
    case '\324':
    case '\325':
    case '\326': 
    case '\330': push_back('o'); continue;  
    case '\371':
    case '\372':
    case '\373':
    case '\374': push_back('u'); continue;  
    case '\331':
    case '\332':
    case '\333':
    case '\334': push_back('u'); continue;  
    case '{': continue;
    case '}': continue;
    case '$': continue;
    case '\\':continue;
    default:
       push_back(x);
    }
  }
}

void FullLogger::finish(int n)
{
  log_is_open = false;
  if(n==0) *this << "No error found.\n";
  else if(n==1) *this << "There was one error.\n";
  else *this << "There were " << n << " errors.\n";
  *this << "(For more information, see transcript file " << L.get_filename() << ")\n";
}

void FullLogger::init(string name,bool status)
{
  L.set_file_name(name);
  L.fp = tralics_ns::open_file(L.get_filename(), true);
  L.set_finished();
  verbose = status;
  log_is_open = true;
}

// This can be used to check if the main file exists. In this case the
// transcript file is not yet open.
bool tralics_ns::file_exists(String name)
{
  FILE* f = fopen(name,"r");
  if (log_is_open)
    the_log << lg_start << "++ file " << name 
	    << (f? " exists" : " does not exist") << lg_endsentence;
  if(f) { fclose(f); return true; }
  return false;
}

bool tralics_ns::file_exists(string B)
{
  return tralics_ns::file_exists(B.c_str());
}

bool tralics_ns::file_exists(Buffer&B)
{
  return tralics_ns::file_exists(B.c_str());
}

// This exits if the file cannot be opened and argument is true
fstream* tralics_ns::open_file(String name, bool fatal)
{
  fstream* fp = new fstream(name,std::ios::out);
  if (log_is_open && !*fp)
    the_log << "Cannot open file " << name << " for output \n"; 
  if(fatal && !*fp) {
    cout << "Cannot open file " << name << " for output \n"; 
    exit(1);
  } 
  return fp;
}

// This takes a string as argument
fstream* tralics_ns::open_file(string name, bool f)
{
  return tralics_ns::open_file(name.c_str(),f) ;
}

// This is a static function
void tralics_ns::close_file(fstream* fp) 
{
  fp->close();
  delete fp;
}

// Insert a line at the end of the file
void LinePtr::insert (int n, string c,bool cv) 
{
  value.push_back(Clines(n,c,cv));
}

// Insert a line at the end of the file, incrementing the line no
void LinePtr::insert (string c, bool cv) 
{
  cur_line ++;
  value.push_back(Clines(cur_line,c,cv));
}

// Insert a line at the end of the file, incrementing the line no
// We assume that the const char* is ascii 7 bits
void LinePtr::insert (String c) 
{
  cur_line ++;
  value.push_back(Clines(cur_line,c,true));
}

// Like insert, but we do not insert an empty line after an empty line.
// Used by the raweb preprocessor, hence already converted
void LinePtr::insert_spec(int n, string c)
{
  if(!value.empty() && value.back().get_chars()[0]=='\n' && c[0]=='\n')
    return;
  insert(n,c,true);
}

// Inserts the buffer, with a newline at the end.
void LinePtr::add(int n, Buffer& b,bool cv)
{
  b.push_back("\n");
  insert(n,b.to_string(),cv);
}

// insert a file at the start
void LinePtr::splice_first (LinePtr & X)
{
  value.splice(value.begin(), X.value);
}

// insert at the end
void LinePtr::splice_end(LinePtr& X) 
{ 
  value.splice(value.end(), X.value);
}
 
// Copy X here,
void LinePtr::clear_and_copy(LinePtr& X) 
{ 
  value.clear();
  value.splice(value.begin(), X.value);
  cur_encoding = X.cur_encoding;
  set_file_name(X.get_file_name());
}

// Whenever a TeX file is opened for reading, we print this in the log
void LinePtr::after_open()
{
  the_log << lg_start << "++ Opened file " << file_name << "; it has " 
	  << (value.empty() ? 0 : get_last_line_no()) <<" lines\n";
}

// Whenever a TeX file is closed, we call this. If sigforce is true
// we say if this was closed by a \endinput command.
void LinePtr::before_close(bool sigforce)
{
  the_log << lg_start << "++ ";
  if(file_name.empty()) {
    the_log << "End of virtual file";
    if(sigforce && !value.empty()) the_log << " (forced by \\endinput)";
    the_log << ".\n";
  }
  else {
    the_log << "End of file ";
    if(sigforce && !value.empty()) the_log << "(forced by \\endinput) ";
    the_log << file_name <<"\n";
  }
}

// Puts in b the next line of input. 
// return -1 at EOF, the line number otherwise.
int LinePtr::get_next (Buffer& b)
{
  int n = -1;
  bool converted = false;
  if(interactive) {
    n = read_from_tty(b);
    if(n==-1) interactive = false;
  } else {
    if(value.empty()) return -1;
    n = value.front().to_buffer(b,converted);
    value.pop_front();
  } 
  if(!converted) {
    the_converter.cur_file_name = file_name;
    b.convert_line(n,cur_encoding);
  }
  return n;
} 

int LinePtr::get_next_cv (Buffer& b,int w)
{
  if(value.empty()) return -1;
  bool converted = false; // unused
  int n = value.front().to_buffer(b,converted);
  value.pop_front();
  if(w) {
    the_converter.cur_file_name = file_name;
    b.convert_line(n,w);
  }
  return n;
} 

// same as get_next, without conversion
int LinePtr::get_next_raw (Buffer& b)
{
  if(value.empty()) return -1;
  bool unused;
  int n = value.front().to_buffer(b,unused);
  value.pop_front();
  return n;
} 

// Puts the line in the string, instead of the buffer.
int LinePtr::get_next (string& b, bool&cv)
{
  if(value.empty()) return -1;
  int n = value.front().to_string(b,cv);
  value.pop_front();
  return n;
} 

/// This finds a line with documentclass in it
// uses B and the buffer.
string LinePtr::find_documentclass(Buffer& B) 
{
  line_iterator C = value.begin();
  line_iterator E = value.end();
  the_main->set_doc_class_pos(E);
  while(C != E) {
    B.reset();
    B.push_back(C->get_chars());
    Buffer&aux = thebuffer;
    bool s = B.find_documentclass(aux);
    if(s) {
      the_main->set_doc_class_pos(C);
      return aux.to_string();
    }
    ++C;
  }
  return "";
}

// This inserts B into this, before C
void LinePtr::add_buffer(LinePtr& B, line_iterator C)
{
  if(B.value.empty()) return; 
  value.splice(C, B.value);
}


// This inserts B into *this, after N lines.
// If N is invalid, B is inserted at the start.
// The idea is to insert text from the config file to the main file
// It is assume that the inserted line is already converted.
void LinePtr::add_buffer(Buffer& B, line_iterator C)
{
  line_iterator E = value.end();
  if(C==E) value.push_front(Clines(1,B.to_string(),true));
  else
    value.insert(C,Clines(1,B.to_string(),true));
}

// This finds a line with documentclass in it
// uses B and the buffer.
string LinePtr::find_configuration(Buffer& B) 
{
  int N = 0;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    B.reset();
    B.push_back(C->get_chars());
    Buffer&aux = thebuffer;
    bool s = B.find_configuration(aux);
    if(s) return aux.to_string();
    ++C;
    N++;
    if(N>100) break;
  }
  return "";
}
// This finds a line with document type in it
// uses B and the buffer.
void LinePtr::find_doctype(Buffer& B,string&res) 
{
  if(!res.empty()) return; // use command line option if given
  int N = 0;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    B.reset();
    B.push_back(C->get_chars());
    int k = B.find_doctype();
    if(k) { res = B.to_string(k); return; }
    ++C;
    N++;
    if(N>100) return;
  }
}

// Splits a string at \n, creates a list of lines with l as first
// line number. 
// This is used by \scantokens and \reevaluate, assumes UTF8 
void LinePtr::split_string (String x, int l)
{
  Buffer& B = thebuffer;
  LinePtr L;
  L.set_cur_line (l);
  int i = 0;
  B.reset();
  for(;;) {
    char c = x[i]; i++;
    bool emit = false;
    if(c=='\n') emit = true;
    else if(c==0) { if(!B.empty())  emit = true; }
    else B.push_back(c);
    if(emit) {
      B.push_back_newline();
      L.insert(B.to_string(),true);
      B.reset();
    }
    if(c==0) break;
  }
  splice_first(L);
}

// Prints in Latin1 format
void LinePtr::print1(fstream* outfile)
{
  Buffer temp;
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    const string& s = C->get_chars();
    temp.reset();
    temp.push_back(s);
    if(temp.is_all_ascii()) *outfile  << s;
    else *outfile  << temp.convert_to_latin1(true);
    ++C;
  }
}

void LinePtr::print(fstream* outfile)
{
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    *outfile << C->get_chars();
    ++C;
  }
}

// For debug
void LinePtr::print()
{
  line_iterator_const C = value.begin();
  line_iterator_const E = value.end();
  while(C != E) {
    cout << C->get_number() << "  " << C->get_chars();
    ++C;
  }
}

// This implements the filecontent environment.
// \begin{filecontents}{name} some lines of code \end{filecontents}
// spec=0 normal, =1 star, =2 plus
void Parser::T_filecontents(int spec)
{
  string filename;
  {
    flush_buffer(); 
    InLoadHandler somthing;
    filename = sT_next_arg(); 
  }
  int action= 0;
  fstream* outfile=0;
  LinePtr* res=0;
  bool is_encoded = true;
  if(spec==2 || spec==3) {
    action = 2;
    res = new LinePtr;
    res->reset(filename);
    main_ns::register_file(res);
    if(spec==3)  is_encoded = false;
  } else if(tralics_ns::find_in_path(filename)) {
    main_ns::log_and_tty << lg_start << "File `" << main_ns::path_buffer
			 << "' already exists on the system.\n"
			 << "Not generating it from this source\n";
  } else {
    String fn= tralics_ns::get_out_dir(filename);
    outfile = tralics_ns::open_file(fn,false);
    main_ns::log_and_tty << lg_start << "Writing file `" << fn << "'\n";
    if(!outfile) parse_error("unable to open file for writing");
    else {
      action= 1;
      if(spec==0) 
	(*outfile) << "%% LaTeX2e file `" << filename <<"' utf8-encoded\n"
		   << "%% generated by the `filecontents' environment\n"
		   << "%% from source `" << get_job_name() << "' on "
		   << the_main->get_short_date() << ".\n%%\n";
	  }
  }
  kill_line();
  for(;;) {
    if(is_verbatim_end()) break;
    if(at_eol()) {
      parse_error("bad end of filecontents env");
      break;
    }
    input_buffer.push_back_newline();
    if(action==1) *outfile << input_buffer;
    if(action==2) {
      int n = get_cur_line();
      res->insert(n,input_buffer.c_str(),is_encoded);
    }
    kill_line();
  }  
  kill_line(); // who knows
  if(action==1) tralics_ns::close_file(outfile);
  pop_level(true,bt_env);
} 
