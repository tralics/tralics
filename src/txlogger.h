// -*- C++ -*-
// $Id: txlogger.h,v 2.20 2012/05/15 17:14:30 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003,  2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This include file holds some declarations for printing objects
// and the classes that allow us to print either on the tty, the log file 
// or both.

extern ostream& operator<< (ostream&fp, const Glue&x);
extern ostream& operator<< (ostream&fp, const Istring&x);
extern ostream& operator<< (ostream&fp, const Macro&x);
extern ostream& operator<< (ostream&fp, const TokenList& L);
extern ostream& operator<< (ostream&fp, const FontInfo& L);
extern ostream& operator<< (ostream&fp, const SthInternal&x);
extern ostream& operator<< (ostream&fp, Token x);
extern ostream& operator<< (ostream&fp, Xid);
extern ostream& operator<< (ostream&fp, const Utf8Char&);
extern ostream& operator<< (ostream&fp, const Xml*);
extern ostream& operator<< (ostream&fp, const ScaledInt&x);
extern ostream& operator<< (ostream&fp, const boundary_type&x);

inline ostream& operator<<(ostream&fp, const Buffer& L)
{ 
  return fp << L.c_str(); 
}

class Logger;
typedef void logger_fn(Logger&);


class Logger{
  bool finished; // if false, we are printing a character sequence and
                 // a newline is required 
  string filename; // the name of the log file
 public:
  fstream* fp; // the stream to which we print
  void finish_seq();
  void out_single_char(Utf8Char c);
  void dump (String s);
  void dump0 (String s);
  void set_finished() { finished = true; }
  void set_file_name(string x) { filename = x; }
  void abort();
  string get_filename() const { return filename; }
  Logger& operator<<(logger_fn f) { f(*this); return *this; }
  // some trivial functions
  Logger& operator<<(String s) { (*fp) << s; return *this; }
  Logger& operator<<(const Buffer& s) { (*fp) << s; return *this;}
  Logger& operator<<(int s) { (*fp)<< s; return *this;}
  Logger& operator<<(Istring s) { (*fp)<< s; return *this;}
  Logger& operator<<(char c)  { *(fp)<< c; return *this;}
  Logger& operator<<(unsigned char c)  { *(fp)<< c; return *this;}
  Logger& operator<<(const string& s)   { *(fp)<< s; return *this;}
  Logger& operator<<(const TokenList& s)   { *(fp)<< s; return *this;}
  Logger& operator<<(const boundary_type& s)   { *(fp)<< s; return *this;}
  Logger& operator<<(const Xml* s)   { *(fp)<< s; return *this;}
};

class FullLogger {
 public:
  Logger L;
  bool verbose;
  void abort() { L.abort(); }
  FullLogger& operator<<(logger_fn f) { f(L); return *this; }
  void finish(int);
  void init(string, bool);
  void unexpected_char(String s, int k);
};

class HalfLogger
{
 public:
  Logger L;
  bool verbose;
  HalfLogger& operator <<(logger_fn f) { f(L); return *this; }
  HalfLogger() { verbose = true; }
};

// if X is of type logger, then X << lg_start; is the same as
// lg_start(X); and hence as  X.finish_seq();
// if Y is of type FullLogger, Y<< lg_start; is the same as
// lg_start(Y.L), hence Y.L.finish_seq();


inline void lg_flush(Logger& L) { (*(L.fp)).flush(); }
inline void lg_start(Logger& L) { L.finish_seq(); }
inline void lg_startstack(Logger& L) { L.finish_seq(); *(L.fp) << "+stack: "; }
inline void lg_startbrace(Logger& L) { L.finish_seq(); *(L.fp) << "{"; }
inline void lg_startcond(Logger& L) { L.finish_seq(); *(L.fp) << "+"; }
inline void lg_startif(Logger& L) { L.finish_seq(); *(L.fp) << "{ifthenelse "; }
inline void lg_startcalc(Logger& L) { L.finish_seq(); *(L.fp) << "{calc "; }
inline void lg_startbracebs(Logger& L) { L.finish_seq(); *(L.fp) << "{\\"; }
void lg_end(Logger& L);
inline void lg_endsentence(Logger& L) { *(L.fp) << ".\n"; }
inline void lg_endbrace(Logger& L) { *(L.fp) << "}\n"; }
inline void lg_arrow(Logger&L) { *(L.fp) << "->"; }
inline void lg_fatal(Logger&L) { L.abort(); }


Logger& operator <<(Logger& X,  const ScaledInt&x);
Logger& operator <<(Logger& X,  const Glue&x);
Logger& operator <<(Logger& X,  const Macro&x);
Logger& operator <<(Logger& X,  const SthInternal&x);
Logger& operator <<(Logger& X,  const FontInfo&x);
Logger& operator <<(Logger& X,  Token x);
Logger& operator <<(Logger& X,  const Utf8Char& x);
FullLogger& operator <<(FullLogger& X,  const Utf8Char& x);
FullLogger& operator <<(FullLogger& X, Token x);
FullLogger& operator <<(FullLogger& X, const Macro& x);
FullLogger& operator <<(FullLogger& X, const ScaledInt&x);
FullLogger& operator <<(FullLogger& X, String s);
FullLogger& operator <<(FullLogger& X, Istring s);
FullLogger& operator <<(FullLogger& X, int s);
FullLogger& operator <<(FullLogger& X, const string& s);
FullLogger& operator <<(FullLogger& X, char s);
FullLogger& operator <<(FullLogger& X, unsigned char s);
FullLogger& operator <<(FullLogger& X, const Buffer& s);
FullLogger& operator <<(FullLogger& X, const TokenList& s);
FullLogger& operator <<(FullLogger& X, const Xml* s);
HalfLogger& operator <<(HalfLogger& X, String s);
HalfLogger& operator <<(HalfLogger& X, Istring s);
HalfLogger& operator <<(HalfLogger& X, int s);
HalfLogger& operator <<(HalfLogger& X, const string& s);
