// -*- C++ -*-
// $Id: txchars.h,v 2.8 2012/04/27 15:10:05 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002,2004,2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Declarations for UTF8 characters


class Utf8Char {
private :
  unsigned int value;
public :
  Utf8Char(unsigned int x) : value(x) {}
public:
  void make_invalid() { value = 0xFFFF; } // Not a Unicode char
  bool is_invalid()const { return value == 0xFFFF; } 
  uint get_value() const { return value; }
  uchar ascii_value() const { return uchar(value); }
  bool is_ascii() const { return value<128; }
  bool is_delete() const { return value==127; }
  bool is_control() const { return value<32; }
  bool is_null() const { return value == 0; }
  bool is_small() const { return value < 256; }
  bool is_big() const { return value > 65535; }
  bool is_verybig() const { return value > 0x1FFFF; }
  bool non_null() const { return value != 0; }
  uchar char_val() const { return uchar(value); }
  bool is_digit() const { return '0'<= value && value <= '9'; }
  int  val_as_digit() const { return value -'0'; } 
  bool is_hex() const { return 'a'<= value && value <= 'f'; }
  int  val_as_hex() const { return value -'a' +10; } 
  bool is_Hex() const { return 'A'<= value && value <= 'F'; }
  int  val_as_Hex() const { return value -'A' +10; } 
  bool is_letter() const { return is_ascii() && ::is_letter(value); }
  bool is_upper_case() const {  return  'A' <= value && value <= 'Z';}
  bool is_lower_case() const {  return  'a' <= value && value <= 'z';}
  bool is_space() const { return value==' ' || value =='\t' || value =='\n'; }
  Utf8Char to_lower() const {  
    if('A' <= value && value <= 'Z') return Utf8Char(value + ('a'-'A')); 
    else return *this;
  }
};


inline bool operator ==(const Utf8Char& a, const Utf8Char& b) 
{ 
  return a.get_value() == b.get_value(); 
}

inline bool operator !=(const Utf8Char& a, const Utf8Char& b) 
{ 
  return a.get_value() != b.get_value(); 
}


inline bool operator ==(const Utf8Char& a, const unsigned char& b) 
{ 
  return a.get_value() == uint(b); 
}

inline bool operator !=(const Utf8Char& a, const unsigned char& b) 
{ 
  return a.get_value() != uint(b); 
}

