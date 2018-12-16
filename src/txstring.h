// -*- C++ -*-
// $Id: txstring.h,v 2.20 2008/10/03 08:07:55 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)



// This file holds the hash table, and XML.
class LabelInfo;
class AttListTable;


// The string_hash table. It is like the hashtable below (Text and Next)
// but it is extensible (maybe, we should make the hashtable extensible)
// Each item has a pointer to a LabelInfo element. If the pointer is not null
// we consider the slot of `foo', that has a \label{foo} or \ref{foo}
// At bootstrap, we fill the table with true, false, spacebefore, center,...
// This uses utf8 encoding. The token can be dumped via its Value
// which can be ascii, utf8 or latin1 (XML syntax)

class StrHash {
  String *Text;  // the Text table
  String *Value;  // the Value table
  int *Next;     // the Next table
  LabelInfo** Labinfo; // the LabelInfo
  int hash_len;   // size of the table
  int hash_used;  // number of slots used
  int hash_last;  // last slot used
  Buffer mybuf;   // local buffer
 public:
  StrHash();
  void re_alloc();
  int hash_find ();
  int find (String);
  int find (const string&);
  int find (int);
  String operator[](int k) const { return Text[k]; }
  String p_str(int k) const { return Value[k]; }
  Buffer& shbuf() { return mybuf; }
  LabelInfo* lab_val(Istring k);
  LabelInfo* lab_val_check(Istring k);
  Istring next_label_id();
  Istring next_top_label_id();
  void rlc_to_string(String s, vector<AttList>&);
  name_positions st_bool(bool x) const { return x ? np_true : np_false; };
  Istring find_scaled(ScaledInt);
  name_positions skip_val(int k) { return k==0 ? np_3pt : k == 1 ? np_6pt : np_12pt; }
};


class Istring {
  int value;
public:
  Istring() : value(0) {};
  Istring(name_positions N) : value(the_names[N].value) {}
  Istring(const Buffer& X);
  explicit Istring(int N) : value(N) {}
  explicit Istring(string s);
  explicit Istring(String s);


  bool null() const { return value == 0; }  // null string
  bool empty() const { return value == 1; } // ""
  bool spec_empty() const { return value == 2; } // ""
  bool only_space() const;
  uchar starts_with_div() const;
  bool only_space_spec() const;
  bool operator==(Istring X)const { return value==X.value; }
  bool operator!=(Istring X)const { return value!=X.value; }
  int get_value() const { return value; }
  String c_str()const;
  String p_str()const;
};

inline LabelInfo* StrHash::lab_val(Istring k) { return Labinfo[k.get_value()]; }

