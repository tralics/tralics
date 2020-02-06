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
  auto hash_find() -> int;
  auto find(String) -> int;
  auto find(const string &) -> int;
  auto find(int) -> int;
  auto operator[](int k) const -> String { return Text[k]; }
  auto p_str(int k) const -> String { return Value[k]; }
  auto shbuf() -> Buffer & { return mybuf; }
  auto lab_val(Istring k) -> LabelInfo *;
  auto lab_val_check(Istring k) -> LabelInfo *;
  auto next_label_id() -> Istring;
  auto next_top_label_id() -> Istring;
  void rlc_to_string(String s, vector<AttList>&);
  auto st_bool(bool x) const -> name_positions {
    return x ? np_true : np_false;
  };
  auto find_scaled(ScaledInt) -> Istring;
  auto skip_val(int k) -> name_positions {
    return k == 0 ? np_3pt : k == 1 ? np_6pt : np_12pt;
  }
};


class Istring {
  int value{0};

public:
  Istring(){};
  Istring(name_positions N) : value(the_names[N].value) {}
  Istring(const Buffer& X);
  explicit Istring(int N) : value(N) {}
  explicit Istring(string s);
  explicit Istring(String s);

  auto null() const -> bool { return value == 0; }       // null string
  auto empty() const -> bool { return value == 1; }      // ""
  auto spec_empty() const -> bool { return value == 2; } // ""
  auto only_space() const -> bool;
  auto starts_with_div() const -> uchar;
  auto only_space_spec() const -> bool;
  auto operator==(Istring X) const -> bool { return value == X.value; }
  auto operator!=(Istring X) const -> bool { return value != X.value; }
  auto get_value() const -> int { return value; }
  auto c_str() const -> String;
  auto p_str() const -> String;
};

inline auto StrHash::lab_val(Istring k) -> LabelInfo * {
  return Labinfo[k.get_value()];
}
