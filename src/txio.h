// -*- C++ -*-
// $Id: txio.h,v 2.24 2013/07/22 09:28:21 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Implements:  FileForInput, LinePtr, TexOutStream
// InputStack converter

// Class used for conversion to and from UTF8

class Converter {
public:
  string cur_file_name; // file name to be converted
  int cur_file_line; // current line number
  bool global_error; // Is this line OK?
  bool local_error; // is this char OK ?
  bool line_is_ascii; // is this line ascii 7bit 
  int lines_converted; 
  int bad_lines;
  int bad_chars;
public:
  Converter();
  bool new_error();
  void start_convert(int);
  void stats();
};

// Whenever Tralics reads a file, it puts the result in a structure like this
class Clines  
{
  int number;    // the number of the current line
  string chars;  // the characters on the line
  bool converted; // true if line is already converted
 public:
  unsigned int next_byte(Buffer&B);
  Clines(int n) : number(n), chars(""), converted(true) {}
  Clines (int n, string c,bool C): number(n), chars(c),converted(C) {}
  int to_string(string& C,bool&cv) const { C = chars; cv=converted;return number; }
  int to_buffer(Buffer&b,bool&C) const { b.push_back(chars);C=converted; return number; }
  int get_number() const { return number; }
  const string& get_chars() const  { return chars; }
  void set_chars(string x) { chars = x; }
  bool starts_with(String) const;
  void convert_line(int);
  void clear_converted() { converted = false; }
};

typedef std::list<Clines>::const_iterator line_iterator_const; 
typedef std::list<Clines>::iterator line_iterator; 

class LinePtr {
private:
  std::list<Clines> value;
  int cur_line;   // current line number
  bool interactive; // is this file or a tty ?
  string file_name; // file name associated to the lines
  int cur_encoding; // current file encoding
public:
  void add(int n, Buffer& b,bool);
  void add_buffer(Buffer&,line_iterator);
  void add_buffer(LinePtr&,line_iterator);
  void after_open();
  void before_close(bool);
  void clear() { value.clear(); }
  void clear_and_copy(LinePtr& X) ;
  void change_encoding(int k);
  void find_tex_encoding();
  bool find_aliases (const vector<string>&, string&res);
  void find_all_types (vector<string>&);
  string find_configuration(Buffer&);
  void find_doctype(Buffer&,string&);
  string find_documentclass(Buffer&);
  void find_top_atts (Buffer& B);
  string find_top_val (String s,bool c);
  int  get_cur_line() const { return cur_line; }
  int get_encoding() const { return cur_encoding; }
  string get_file_name() const { return file_name; }
  bool get_interactive()const { return interactive; }
  void set_interactive(bool sw) { interactive=sw; }
  int  get_last_line_no() { return value.back().get_number(); } 
  int  get_next_raw (Buffer&b);
  int  get_next_cv (Buffer&b,int);
  int  get_next (Buffer&b);
  int  get_next (string&b,bool&cv);
  std::list<Clines>& get_value() { return value; }
  void incr_cur_line() { cur_line++; }
  void insert (int n, string c,bool);
  void insert (string c,bool) ;
  void insert (String c) ;
  void insert_spec (int n, string c);
  void insert(const LinePtr&);
  bool is_empty() const { return value.empty(); }
  void parse_and_extract_clean(String s);
  void parse_conf_toplevel() const;
  LinePtr parse_and_extract(String s) const;
  void print();
  void print(fstream*);
  void print1(fstream*);
  void push_front(Clines x) { value.push_front(x); }
  void push_back(Clines x) { value.push_back(x); }
  void reset(string x){cur_line=0; cur_encoding=1; interactive=false;clear(); file_name = x;}
  int read_from_tty(Buffer& b);
  void set_cur_line(int x) { cur_line = x; }
  void set_encoding(int k)  { cur_encoding=k; }
  void set_file_name(string s) { file_name = s; }
  void set_interactive();
  line_iterator_const skip_env(line_iterator_const C, Buffer&B);
  void splice_end(LinePtr & X);
  void splice_first(LinePtr & X);
  void split_string (String x, int l);
  LinePtr() :  cur_line(0), interactive(false), file_name(""),cur_encoding(1) {}
  void normalise_final_cr ();
};


// This allows us to temporarily read from elsewhere
class InputStack {
  vector<Utf8Char>B;
  states s;  // copy of scanner state
  LinePtr L; // the lines
  int line_no; // the current line number
  TokenList TL; // saved token list
  string name; // name of the current file
  // int level; unused 
  int restore_at; // catcode of @ to restore
  int file_type; // file type to restore
  int Bpos; // position in B
  bool seen_eof; // True if every_eof_token already included
 public:
  const string& get_name() const { return name; }
  int get_line_no()const { return line_no; }
  void set_state(states X) { s = X; }
  TokenList& get_TL() { return TL; }
  vector<Utf8Char>& get_B() { return B; }
  states get_state()const { return s; }
  int get_line_pos()const { return Bpos; }
  void set_line_pos(int x) { Bpos = x; }
  void set_line_vector(const vector<Utf8Char>& x) { B = x; }
  int get_at_val() const { return restore_at; }
  void set_at_val(int x) { restore_at = x; }
  int get_file_type() const { return file_type; }
  void set_file_type(int x) { file_type = x; }
  bool get_seen_eof() const { return seen_eof; }
  void destroy();
  void set_line_ptr(LinePtr& X) { 
    L.clear_and_copy(X); 
    X.set_file_name("");
    L.set_interactive(X.get_interactive());
  }
  void get_line_ptr(LinePtr& X) { 
    X.clear_and_copy(L);
    X.set_interactive(L.get_interactive());
  }
  InputStack(string N,int l, states S,int cfp,bool eof): 
    s(S), line_no(l), 
    name(N),restore_at(-1),file_type(cfp),seen_eof(eof)
  {}
};



// data structure associated to \input3=some_file. 
class FileForInput {
  bool is_open; // is this file active ?
  LinePtr the_lines; // the lines that not yet read by TeX
  Buffer cur_line; // this holds the current line
  int line_no;  // this holds the current line number
 public:
  void open(string,bool);
  void close();
  FileForInput() : is_open(false) {};
  bool is_valid() { return is_open; }
  LinePtr& get_lines() { return the_lines; }
  void set_lines(LinePtr X) { the_lines=X; }
  Buffer& get_buffer() { return cur_line; }
  int& get_line_no() { return line_no; }
};

// From TeX:
// The sixteen possible \write streams are represented by the |write_file|
// array. The |j|th file is open if and only if |write_open[j]=true|. The last
// two streams are special; |write_open[16]| represents a stream number
// greater than 15, while |write_open[17]| represents a negative stream number,
// and both of these variables are always |false|.
// Since \write18 is special, we added another slot in write_open
class TexOutStream {
  fstream* write_file[nb_input_channels];
  bool write_open[nb_output_channels];
 public:
  TexOutStream();
  void close(int);
  void open(int, string);
  bool is_open(int i)const { return write_open[i]; }
  void write(int chan, string s) { *(write_file[chan]) << s; }
};

