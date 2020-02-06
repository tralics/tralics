// -*- C++ -*-
// $Id: txinline.h,v 2.30 2015/10/28 17:38:46 grimm Exp $
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
  extern bool bib_allow_break;
}


extern Logger& the_log;  // not in a namespace ?


namespace tralics_ns {
auto is_leap_year(int y) -> bool;
auto year_length(int y) -> int;
void make_names();
void boot_math(bool);
auto file_exists(String name) -> bool;
auto find_in_confdir(const string &s, bool retry) -> bool;
auto find_in_path(const string &s) -> bool;
auto find_no_path(const string &s) -> bool;
void read_a_file(LinePtr &, string x, int spec);
auto make_string(String) -> String;
auto titlepage_is_valid() -> bool;
auto file_exists(String name) -> bool;
auto file_exists(string name) -> bool;
auto file_exists(Buffer &) -> bool;
void bibtex_bootagain();
void bibtex_boot(String b, String, string, bool, bool);
void Titlepage_create(LinePtr &x);
void Titlepage_start(bool);
auto exists(const vector<string> &, string) -> bool;
void bibtex_set_nocite();
void bibtex_insert_jobname();
auto open_file(String name, bool) -> fstream *;
auto open_file(string name, bool) -> fstream *;
void close_file(fstream *);
auto only_digits(const string &s) -> bool;
auto get_out_dir(string) -> String;
auto get_short_jobname() -> string;
};

namespace err_ns {
  extern Buffer local_buf;
  void fatal_error (String s);
}

namespace config_ns {
auto find_one_key(const string &name, const string &key) -> string;
auto pers_rc(const string &rc) -> string;
void check_RC(Buffer &s, Xml *);
auto find_keys(const string &name) -> string;
auto start_interpret(Buffer &B, String s) -> bool;
void interpret_list(const string &, Buffer &B);
auto assign(Buffer &a, Buffer &b) -> bool;
auto assign_name(String A, String B) -> bool;
auto assign_att(String A, String B) -> bool;
}

inline auto operator==(const Buffer &B, String s) -> bool {
  return strcmp(B.c_str(),s)== 0;
}

inline auto is_vowel(char c) -> bool {
  return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

// True if it is an accent character, like \^.
inline auto is_accent_char(char c) -> bool {
  return c=='\'' || c=='`' || c=='^' || c=='"' || c=='~' || c== '.' || c=='=';
}

inline auto is_tp_delim(char c) -> bool {
  return c == '<' || c == '\\' || c == '"';
}

inline auto operator==(Token a, Token b) -> bool {
  return a.get_val() == b.get_val();
}

inline auto operator!=(Token a, Token b) -> bool {
  return a.get_val() != b.get_val();
}

inline Istring::Istring(const Buffer& X) :
 value(the_main->SH.find(X.c_str())) {}

inline Istring::Istring(String s) :
  value(the_main->SH.find(s)) {}

inline Istring::Istring(string s) :
  value(the_main->SH.find(s)) {}

inline auto Istring::c_str() const -> String { return the_main->SH[value]; }

inline auto Istring::p_str() const -> String {
  return the_main->SH.p_str(value);
}
