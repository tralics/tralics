// -*- C++ -*-
// $Id: txinline.h,v 2.27 2012/05/15 17:14:30 grimm Exp $
// TRALICS, copyright (C)INRIA/apics (Jose' Grimm) 2003,2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This file contains some inline functions and the tralics 
// namespace declarations

// We also declare some global variables here

// See comments in tralics.C where these variables are defined
namespace main_ns {
  extern FullLogger log_and_tty;
  extern HalfLogger log_or_tty;
  extern Buffer path_buffer;
  extern int nb_errs; 
  extern bool file_ended;
  extern bool no_xml_error;
  extern bool use_quotes;
}


extern Logger& the_log;  // not in a namespace ?


namespace tralics_ns {
  bool is_leap_year(int y);
  int year_length(int y);
  void make_names();
  void boot_math(bool);
  bool file_exists(String name);
  bool find_in_confdir(const string & s, bool retry);
  bool find_in_path(const string & s);
  bool find_no_path(const string & s);
  void read_a_file (LinePtr&,string x, int spec);
  String make_string(String);
  bool titlepage_is_valid();
  bool file_exists(String name);
  bool file_exists(string name);
  bool file_exists(Buffer&);
  void bibtex_bootagain();
  void bibtex_boot(String b,String,string,bool,bool);
  void Titlepage_create(LinePtr& x);
  void Titlepage_start(bool);
  bool exists(const vector<string>&, string);
  void bibtex_set_nocite();
  void bibtex_insert_jobname();
  fstream* open_file(String name,bool);
  fstream* open_file(string name,bool);
  void close_file(fstream*);
  bool only_digits(const string& s);
  String get_out_dir(string);
  string get_short_jobname();
};

namespace err_ns {
  extern Buffer local_buf;
  void signal_error(String s,int);
  void fatal_error (String s);
}

namespace config_ns {
  string find_one_key(const string& name,const string&key);
  string pers_rc(const string& rc);
  void check_RC(Buffer& s,Xml*);
  string find_keys(const string& name);
  bool start_interpret(Buffer&B, String s);
  void interpret_list(const string&, Buffer&B);
  bool assign(Buffer& a, Buffer& b);
  bool assign_name(String A,String B);
  bool assign_att(String A, String B);

}

inline bool operator==(const Buffer& B, String s) 
{ 
  return strcmp(B.c_str(),s)== 0;
}

inline bool is_vowel(char c) 
{
  return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

// True if it is an accent character, like \^. 
inline bool is_accent_char(char c)
{
  return c=='\'' || c=='`' || c=='^' || c=='"' || c=='~' || c== '.' || c=='=';
}

inline bool is_tp_delim (char c)
{
  return c == '<' || c == '\\' || c == '"';
}


inline bool operator ==(Token a, Token b) 
{ 
  return a.get_val() == b.get_val(); 
}

inline bool operator !=(Token a, Token b) 
{ 
  return a.get_val() != b.get_val(); 
}

inline Istring::Istring(const Buffer& X) :
 value(the_main->SH.find(X.c_str())) {}

inline Istring::Istring(String s) :
  value(the_main->SH.find(s)) {}

inline Istring::Istring(string s) :
  value(the_main->SH.find(s)) {}

inline String Istring::c_str()const 
{
  return the_main->SH[value]; 
}

inline String Istring::p_str()const 
{
  return the_main->SH.p_str(value); 
}
