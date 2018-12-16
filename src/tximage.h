// -*- C++ -*-
// $Id: tximage.h,v 2.3 2008/02/21 17:45:58 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// data structure for file arguments of \includegraphics

class Image {
 public: 
  string name; // name
  int occ;     // number of occurences
  int flags;   // existence flags
 public:
  Image() {}
  Image(string N, int oc_val) : name(N), occ(oc_val), flags(0) {}
  bool file_exists(String);
  void check_existence();
  void check();
};
