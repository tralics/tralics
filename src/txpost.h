// -*- C++ -*-
// $Id: txpost.h,v 2.3 2008/02/21 17:45:59 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


// for modules
class ModChecker {
  Istring id;    // the id of the module or section
  bool is_mod;   // is this a module?
  bool has_info; // does this have some infos ?
 public:
  ModChecker() : id(0), is_mod(false), has_info(false) {}
  ModChecker(Istring I, bool mod) : id (I), is_mod(mod), has_info(false) {}
  void set(Istring I) { if(id==I) has_info = true; }
  void check(int*T) const {
    if(is_mod) { if(has_info) T[0]++; else T[1]++;  } 
    else { if(has_info) T[2]++; else T[3]++; } 
  }
};


// temporary for recursion.
class XmlAction {
  Istring match;   // the name of the object to match
  recur_type what; // the type of the action to perform
  int int_val;     // number of results, or xid
  Xmlp xml_val;    // input or output xml value
  Istring string_val; // name of element ot work on
 public:
  XmlAction(Istring M, recur_type w): match(M), what(w), int_val(0),xml_val(0), string_val(Istring()) {}
  XmlAction(Istring M, recur_type w, Xmlp X): match(M), what(w), int_val(0),xml_val(X), string_val(Istring()) {}
  XmlAction(Istring M, recur_type w, Istring X): match(M), what(w), int_val(0),xml_val(0), string_val(X) {}
  recur_type get_what() const { return what; }
  void incr_int_val() { int_val ++; }
  void mark_found() { int_val = 1; }
  bool is_ok() { return int_val != 0; }
  Xmlp get_xml_val() const { return xml_val; }
  int get_int_val() const { return int_val; }
  Istring get_string_val() const { return string_val; }
  void set_string_val(Istring s) { string_val = s; }
  void set_xml_val(Xmlp s) { xml_val = s; }
  void set_int_val(int s) { int_val = s; }
  Istring get_match() const { return match; }
};



// A class to count words...
class WordList {
  WordList* next;
  String name;
  int hash;
  int freq;
public:
  void set_next(WordList*n) { next = n;}
  bool next_empty() const { return next==0; }
  WordList(String s, int h,WordList*N) :next(N), name(s), hash(h), freq(1) {} 
  bool is_here(String s, int h) const { return hash==h && strcmp(name,s)==0;}
  void incr_freq() { freq++; }
  int get_freq() const { return freq; }
  WordList* get_next() const { return next; }
  bool dump(fstream*X,int i) {
    if(freq==i){
      (*X) << freq << " " << name << "\n"; 
      return true;
    }
    return false;
  } 
};
