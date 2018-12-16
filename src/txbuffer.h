// -*- C++ -*-
// $Id: txbuffer.h,v 2.74 2012/05/15 17:14:30 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This is the include file for the Buffer data structure


typedef void buffer_fn(Buffer&);

// a big structure
class Buffer {
 private:
  char* buf; // the characters
  int wptr;  // the write pointer
  int asize;  // allocated size
  int ptr;   // the read pointer
  int ptr1;  // a seconnd read pointer
 public:
 
  Buffer ();
  ~Buffer();
  char operator[] (int k) const { return buf[k]; }
  Buffer& operator<<(String s) { push_back(s); return *this;}
  Buffer& operator<<(char c) { push_back(c); return *this;}
  Buffer& operator<<(uchar c) { push_back(c); return *this;}
  Buffer& operator<<(int c) { push_back_int(c); return *this;}
  Buffer& operator<<(const TokenList&);
  Buffer& operator<<(const Macro&);
  Buffer& operator<<(const Utf8Char&b) { push_back(b); return *this;}
  Buffer& operator<<(const Buffer&b) { push_back(b); return *this;}
  Buffer& operator<<(Token b) { push_back(b); return *this;}
  Buffer& operator<<(const string&b) { push_back(b); return *this;}
  Buffer& operator<<(const Istring&b) { push_back(b); return *this;}
  Buffer& operator<<(const SthInternal&b) { push_back(b); return *this;}
  Buffer& operator<<(const Glue&b) { push_back(b); return *this;}
  Buffer& operator<<(buffer_fn f) { f(*this); return *this; }
  string add_with_space(String);
  void advance() { ptr++; }
  void advance(int k) { ptr += k; }
  void alloc(int);
  char after_head () const { return buf[ptr+1]; }
  uchar after_uhead () const { return buf[ptr+1]; }
  uchar after_after_uhead () const { return buf[ptr+2]; }
  int  append_checked_line(LinePtr&);
  bool at_eol() const { return wptr<=ptr; }
  bool backup_space();
  void bib_spec();
  void brace_match();
  void bracket_match();
  String c_str(int k) const { return buf+k; } 
  String c_str() const { return buf; } 
  void chars_to_buffer(Buffer&);
  void check_before_brace(String);
  string check_cat_perso (int,int,bool);
  bool contains(String s) const { return strstr(buf,s) != 0; }
  bool contains_braced(String env);
  bool contains_env(String env);
  bool contains_here(String) const;
  String convert(int k) const;
  string convert_dim(int,int);
  String convert_to_str() const;
  String convert_to_out_encoding(String) const;
  String convert_to_log_encoding() const;
  String convert_to_latin1(bool) const;
  void convert_custom(int l);
  bool convert_line0(int);
  void convert_line(int l,int);
  void copy(const Buffer&);
  void decr_wptr() { wptr--; };
  bool double_hat_aux(int);
  void dump_prefix(bool err, bool gbl,symcodes K);
  bool empty() const { return size() ==0; }
  void extract_cnrs_info();
  void extract_dtd(String a, string&b, string&c);
  void extract_strs(Buffer& A, Buffer&B);
  void extract_chars(vector<Utf8Char>&);
  int  fetch_beg_end_spec(bool k, bool incat,int&com_loc,
			 bool&seen_dollar, String&);
  bool fetch_citation(String,String&a,String&b);
  bool fetch_spec_arg ();
  string figline(int&ctr,string& junk);
  void figtable (String start, int id);
  void fill_table(bchar_type* table);
  bool find_alias(const vector<string>& SL,string&res);
  bool find_and(bchar_type* table);
  void find_bibtex_name_token(int);
  int  find_brace();
  int  find_bracket();
  bool find_configuration(Buffer&);
  int find_doctype();
  bool find_documentclass(Buffer&);
  bool find_equals();
  String find_one_bibtex_name();
  void find_one_type(vector<string>&);
  void find_top_atts();
  void finish_figure(String start,string junk,int w);
  void finish_xml_print();
  bool full_bracket_match();
  bool full_brace_match();
  int  get_int_val() const;
  string get_machine_name();
  int  get_ptr() const { return ptr; }
  int  get_ptr1() const { return ptr1; }
  int  hashcode(int hash) const;
  char head () const { return buf[ptr]; }
  bool is_not_char(int p,uchar x) { return uchar(buf[p]) != x; }
  int  holds_documentclass(Buffer&a,Buffer&b,Buffer&c);
  int  holds_env(String&a,String&b,String&c);
  unsigned int horner(int);
  int  how_many_bibtex_name_token();
  void kill_at(int p) { buf[p] = 0; }
  void init_from_buffer(Buffer &);
  void insert_and_kill(Buffer& a) { reset(); push_back(a.buf); a.reset(); }
  void insert_escape_char();
  void insert_escape_char_raw();
  String insert_fp(const FpNum&);
  bool insert_space_here(int) const;
  void insert_string (const Buffer& s);
  string insert_break (const string &);
  void insert_token0(Token c);
  bool install_att (Xid idx, Istring match);
  void interpret_aux(vector <Istring>&, vector <Istring>& );
  void interpret_bibtex_list();
  void interpret_bibtex_extension_list();
  bool is_and(int);
  int  is_begin_end() const;
  int  is_begin_something(String);
  int  is_begin_something(String, bool);
  bool is_equal(String x) const { return strcmp(buf,x)== 0; }
  bool is_at_end(String x) const;
  bool is_here(String s);
  bool is_here_case(String s);
  bool is_letter_digit() const;
  bool is_all_ascii() const;
  bool is_good_ascii() const;
  bool is_spaceh (int j) const { return is_space(buf[j]); }
  char last_char() const { if(wptr==0) return 0; else return buf[wptr-1]; }
  int last_slash() const;
  int  length() const { return wptr; }
  int  size() const { return wptr; }
  bool look_at_space(string s);
  void lowercase();
  void make_citation(String a, String b);
  string make_unique_bid(int);
  void modify(int p, char c) { buf[p] = c; }
  void new_keyword();
  void new_word();
  void next_bibtex_char();
  void next_bibtex_name_token();
  uchar next_char() { uchar c = buf[ptr];  ptr ++; return c; }
  bool next_env_spec();
  bool next_kw();
  bool next_macro();
  bool next_macro_spec(bool incat, int& com_loc,bool&seen_dollar);
  bool next_macro_spec();
  uchar next_utf8_byte();
  Utf8Char next_utf8_char();
  Utf8Char next_utf8_char_aux();
  void no_control_M();
  void no_newline();
  void no_double_dot();
  void normalise_for_bibtex(String s);
  void optslash();
  void out_four_hats(Utf8Char);
  void out_log (Utf8Char ch,output_encoding_type T);
  int  pack_or_class(Buffer& aux);
  void pt_to_mu();
  void process_big_char(unsigned int c);
  void purify();
  void purify(String s);
  void push_back (char c);
  void push_back(const Buffer& b) { push_back(b.buf); }
  void push_back(const string& b) { push_back(b.c_str()); }
  void push_back(const Istring&);
  void push_back(String s);
  void push_back(ScaledInt v, glue_spec unit);
  void push_back(const SthInternal&);
  void push_back(const Glue&);
  void push_back(const AttList&);
  void push_back(const AttPair&);
  void push_back(const Macro&);
  void push_back(const Macro&,bool);
  bool push_back(Token c);
  int  push_back(Token c,bool);
  void push_back_alt(const AttPair&);
  void push_back_braced(string);
  void push_back_braced(String);
  void push_back_def(String,string);
  void push_back_elt(Istring,Xid,int);
  void push_back_int(int);
  void push_back16(uint);
  void push_back16u(uint);
  void push_back16l(bool,uint);
  void push_back_ent(Utf8Char);
  void push_back_hex(uint);
  void push_back_Hex(uint);
  void push_back_math_token(const CmdChr&,bool);  
  void push_back_math_tag(const CmdChr&,int);  
  void push_back_math_tag(String,int);  
  void push_back_math_aux(String); 
  void push_back_newline();
  bool push_back_newline_spec();
  void push_back_open_brace() { push_back('{'); };
  void push_back_close_brace() { push_back('}'); };
  void push_back_roman(int);
  void push_back_Roman(int);
  void push_back_space() { push_back(' '); };
  void push_back_special_att(Xid id);
  void push_back_special_string(String s);
  void push_back_real_utf8 (Utf8Char c);
  void push_back_xml_char (uchar c);
  void push_back_substring(String, int);
  void push_back_substring(const string&, int, int);
  void push_back_unless_punct(char c);
  void push_back(Utf8Char c);
  void push_back3(unsigned int);
  void push_back9(unsigned int);
  void put_at_end(String x);
  string remove_digits(string);
  string remove_space(const string&);
  void rrl() { wptr--; buf[wptr] = 0; } // really remove last
  void remove_last() { if(wptr>0) rrl(); }
  void remove_last_n(int n) { if(wptr>=n) wptr-=n; buf[wptr] = 0; }
  void remove_last_comma() { if(wptr>0 && buf[wptr-1]==',') rrl(); }
  void remove_last_quote() { if(wptr>0 && buf[wptr-1]=='\'') rrl(); }
  void remove_last_space();
  void remove_space_at_end();
  void remove_spec_chars(bool, Buffer&);
  void reset0 () { wptr = 0; }
  void reset() { wptr = 0; buf[0] = 0; }
  void reset_ptr() { ptr = 0; }
  void resize();
  uint reverse_horner();
  int  see_begin_end(Buffer&before, Buffer&after);
  int  see_config_env() const;
  String see_config_kw(String s,bool c);
  bool see_equals(String s);
  int  see_something(const String* Table);
  int  see_year(const string& a, string&b);
  void set(String s) { reset0(); push_back(s); }
  void set_last(int k) { wptr =k; buf[wptr] = 0; }
  void set_ptr(int j) { ptr = j; }
  void set_ptr1(int j) { ptr1 = j; }
  void set_ptr1() { ptr1 = ptr; }
  void show_uncomplete(String);
  void skip_over_brace();
  void skip_over_comment();
  int  skip_space(int j) const {while(is_spaceh(j)) j++; return j; }
  void skip_letter() { while(is_letter(head())) ptr++; }
  void skip_sp_tab() { while(buf[ptr]==' ' || buf[ptr] == '\t') ptr ++; }
  void skip_sp_tab_nl() { while(is_space(buf[ptr])) ptr ++; }
  void skip_sp_tab_comma() { while(buf[ptr]==' ' || buf[ptr] == '\t'|| buf[ptr] == ',') ptr ++; }
  char single_char() const;
  bool slash_separated(string&);
  String some_substring(int a, int b);
  string some_sub_string(int a, int b);
  String sortify(String s);
  bool space_or_underscore() const { return buf[ptr]=='_' || buf[ptr]==' '; }
  string special_convert(bool);
  String special_exponent() const;
  void special_purify(String s, int& pos);
  void special_title(string s);
  TokenList str_toks(bool);
  TokenList str_toks11(bool);
  bool string_delims();
  string substring();
  bool tex_comment_line() const;
  void to_seven_bits();
  string to_string() const ;
  string to_string(int k) const;
  bool tp_next_char(char&);
  tpa_line tp_fetch_something();
  String trace_scan_dimen(Token,ScaledInt,bool);
  unsigned char uhead () const { return buf[ptr]; }
  void undo() { ptr--; }
  void unicode_char(int);
  Utf8Char unique_character() const;
  Utf8Char make_character() const;
  void uppercase();
  void utf8_error(bool);
  void utf8_ovf(int);
  Xmlp xml_and_attrib(string s);
  String without_end_spaces(String T);
  bool find_char(char c);
  bool is_special_end() const
    { return buf[ptr] == '\n' || buf[ptr]=='#' || buf[ptr]=='%'; }
 private:
  void realloc();
  bool after_slash();
  void advance_letter_dig() { while (is_letter(buf[ptr])||is_digit(buf[ptr])) ptr++; } 
  void advance_letter_dig_dot() {
   while(is_letter(buf[ptr])|| is_digit(buf[ptr])||buf[ptr]=='.') ptr++;
  }
  void advance_letter_dig_dot_slash() {
   while(is_letter(buf[ptr])|| is_digit(buf[ptr])||buf[ptr]=='.'|| buf[ptr]=='/') ptr++;
  }
};

inline void bf_reset(Buffer& B) { B.reset0(); }
inline void bf_optslash(Buffer& B) { B.optslash(); }
inline void bf_comma(Buffer& B) { if(!B.empty()) B.push_back(',');  }
