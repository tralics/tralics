// -*- C++ -*-
// $Id: txeqtb.h,v 2.12 2013/07/22 09:28:21 grimm Exp $
// Copyright INRIA/apics (Jose' Grimm) 2002-2004, 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Contains:
//   equivalent, restore_vb_space, save_catcode, InLoadHandler,
//  InUrlHandler, SaveState
//   SaveAux, and derived types
//     SaveAuxBoundary , SaveAuxInt , SaveAuxDim , SaveAuxCmd 
//     SaveAuxBox , SaveAuxBoxend, SaveAuxToken, SaveAuxGlue 
//     SaveAuxEnv , SaveAuxFont , SaveAuxAftergroup 
// the 5 types for eqtb:
//   eqtb_int eqtb_dim eqtb_glue eqtb_token eqtb_box

// Defined EQTB, the table of equivalents
// EQTB for commands. No Ctor(?) but reset() is called for every object.
// The values is a (symcodes, subtype) pair
class Equivalent {
  int level;      // level at which this is defined
  CmdChr value; // the modifier
 public:
  int get_level() const { return level; }
  CmdChr get_cmdchr() const { return value; }
  symcodes get_cmd() const { return value.get_cmd(); }
  subtypes get_chr() const { return value.get_chr(); }
  bool must_push(int l) const { return level !=l && l> level_one;}
  void reset() { level = level_zero; value.reset(); }
  void special_prim(CmdChr b) {
    value = b;
    if(!b.is_undef()) level = level_one;
  }
  void primitive (CmdChr b) {
    value = b;  level = level_one;
  }
  void set (CmdChr c,int lvl) {
    value = c;  level = lvl;
  }
  void setnl (CmdChr c) {
    value = c;
  }
  bool is_undefined() const { return value.is_undef();}
  bool is_user() const { return value.is_user(); }
  bool is_undef_or_relax() const { return is_undefined() || value.is_relax();}
};

class RestoreVbSpace {
  Token value;  // value to restore
  Parser* P ;   // this object has a pointer to the parser
 public:
  RestoreVbSpace(Parser*p);
  ~RestoreVbSpace();
  void operator()(Token T);
};

class SaveCatcode {
 private:
  int character; // a character
  int code;      // the code of the character to restore
 public:
  SaveCatcode(int c, int nc);
  ~SaveCatcode();
};

class SaveScannerStatus {
 private:
  scan_stat code;
 public:
  SaveScannerStatus(scan_stat c);
  ~SaveScannerStatus();
};


class SaveLongState {
 private:
  l_state code;
 public:
  SaveLongState(l_state c);
  ~SaveLongState();
};


class InLoadHandler {
  bool in_load; // a boolean value to restore
 public:
  static bool global_in_load;
  InLoadHandler(): in_load(global_in_load) {global_in_load= true;}
  ~InLoadHandler() {global_in_load= in_load;}
};

class InUrlHandler {
  bool in_url; // a boolean value to restore
 public:
  static bool global_in_url;
  InUrlHandler(): in_url(global_in_url) {global_in_url= true;}
  ~InUrlHandler() {global_in_url= in_url;}
};

// this saves two values
class SaveState {
  TokenList L;     // the token list to be restored
  bool restricted;  // the restricted flag to be restored
 public:
  void copy_and_reset(TokenList&X) { L.clear(); L.splice(L.begin(), X); }
  void restore(TokenList&X) { X.clear(); X.splice(X.begin(), L); }
  void set_restricted(bool b) { restricted = b;}
  bool get_restricted() const { return restricted; }
};

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
class SaveAux {
 public:
  save_type type; // the type of the real thing
  int line_no;   // current line number at start
 public:
  virtual void unsave(bool,Parser&)=0;
  virtual ~SaveAux() {}
  SaveAux(save_type t) : type(t), line_no(0) {}
  void set_line(int l) { line_no = l; }
  int get_line() const { return line_no; }
  void unsave_trace_aux(String s, int pos, bool rt);
  void restore_or_retain(bool) const;
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAux {
  boundary_type val; // explains why we opened a new group
 public:
  boundary_type get_val() const { return val; }
  void unsave(bool,Parser&);
  SaveAuxBoundary(boundary_type v) : SaveAux(st_boundary), val(v) {}
  ~SaveAuxBoundary() {}
  void dump(int);
};

// This restores an integer value.
class SaveAuxInt : public SaveAux {
  int level; // the level that was active when this was pushed
  int pos; // the position in eqbt_int_table
  int val; // the value to be restored
 public:
  SaveAuxInt(int l, int a, int b) :  SaveAux(st_int), level(l), pos(a),val(b) {}
  void unsave(bool,Parser&);
  ~SaveAuxInt() {}
};


// This restores a dimension
class SaveAuxDim : public SaveAux {
  int level; // the level that was active when this was pushed
  int pos; // the position in eqbt_dim_table
  ScaledInt val; // the value to be restored
 public:
  SaveAuxDim(int l, int a, ScaledInt b) :  SaveAux(st_int), level(l), pos(a),val(b) {}
  void unsave(bool,Parser&);
  ~SaveAuxDim() {}
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAux{
  int level; // the level that was active when this was pushed
  int cs; // ths position in eqtb to be restored
  CmdChr val; // the CmdChr to be restored
 public:
  SaveAuxCmd(int a, Equivalent X) : SaveAux(st_cmd),
    level(X.get_level()),cs(a),val(X.get_cmdchr()) {}
  void unsave(bool,Parser&);
  ~SaveAuxCmd() {}
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAux {
  int level; // the level that was active when this was pushed
  int pos;  // the position in box_table to be restored
  Xmlp val; // the value to be restored
 public:
  SaveAuxBox(int l, int a, Xmlp b) : SaveAux(st_box),level(l), pos(a),val(b) {}
  void unsave(bool,Parser&);
  ~SaveAuxBox() {}
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAux {
  int pos;  // the box number
  Xmlp val; // the value of the box
 public:
  SaveAuxBoxend(int a, Xmlp b) :  SaveAux(st_box_end),pos(a),val(b) {}
  void unsave(bool,Parser&);
  ~SaveAuxBoxend() {}
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAux {
  int level; // the level that was active when this was pushed
  int pos; // pthe position in toks_registers
  TokenList val; // the value to be restored
 public:
  SaveAuxToken(int l, int p, TokenList v) :
    SaveAux(st_token),level(l),pos(p), val(v) {}
  void unsave(bool,Parser&);  
  ~SaveAuxToken() {}
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAux {
  int level; // the level that was active when this was pushed
  int pos; // the position in glue_table
  Glue val; // the value to be restored
 public:
  SaveAuxGlue(int l, int p, Glue g) :  SaveAux(st_glue),level(l),pos(p), val(g) {}
  void unsave(bool,Parser&);
  ~SaveAuxGlue(){}
};

// data structure for restoring glue
class SaveAuxString : public SaveAux {
  int level;
  int pos; // the position in glue_table
  string val; // the value to be restored
 public:
  SaveAuxString(int l,int p, string s) :  SaveAux(st_string),level(l),pos(p), val(s) {}
  void unsave(bool,Parser&);
  ~SaveAuxString(){}
};

// data structure for a \begin{something}
// had an unused field: int val;
class SaveAuxEnv : public SaveAux {
  string oldname;
  string newname;
  int line;
  Token T;
  CmdChr cc;
 public:
  void set_line(int x) { line = x; }
  CmdChr get_val() const { return cc; }
  Token get_token() const { return T; }
  string get_name() { return newname; }
  void unsave(bool,Parser&);
  SaveAuxEnv(string a, string aa, int ll, Token b, CmdChr c) :
    SaveAux(st_env),oldname(a),newname(aa),line(ll), T(b), cc(c) {}
  ~SaveAuxEnv(){}
};

// data structure for a font change
class SaveAuxFont : public SaveAux {
  int level; // the level that was active when this was pushed
  int value; // the value to be restored
  Istring color; // the color to restore
  public :
    void unsave(bool,Parser&);
  SaveAuxFont(int l, int v,Istring c) : SaveAux(st_font), level(l), value (v),color(c) {}
  ~SaveAuxFont(){}
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAux {
  Token value; // the token to pop
  public :
  SaveAuxAftergroup(Token v) : SaveAux(st_save),value (v) {}
  void unsave(bool,Parser&);
  ~SaveAuxAftergroup(){}
};



// EQBT entry for an integer
// Every eqtb entry has a level. Level_zero means undefined
// Level_one is the outer level. The old value must be saved in case
// the current level is different fron the old one, and is greater than one.
// These objects are defined at level_one
class EqtbInt {
  int val; // value of the object
  int level; // level at which this is defined
 public:
  EqtbInt() :val(0), level(level_one) {}
  int get_val() const { return val; }
  void set_val(int x) { val = x; }
  void set_level(int x) { level = x; }
  int get_level()const { return level; }
  void val_and_level(int a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};

class EqtbString {
  string val; // value of the object
  int level; // level at which this is defined
 public:
  EqtbString() :val(""), level(level_one) {}
  string get_val() const { return val; }
  void set_val(string x) { val = x; }
  void set_level(int x) { level = x; }
  int get_level()const { return level; }
  void val_and_level(string a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};



// EQBT entry for a dimension.
class EqtbDim {
  ScaledInt val; // value of the object
  int level; // level at which this is defined
 public:
  EqtbDim() :val(0), level(level_one) {}
  ScaledInt get_val() const { return val; }
  void set_val(ScaledInt x) { val = x; }
  void set_level(int x) { level = x; }
  int get_level() const { return level; }
  void val_and_level(ScaledInt a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};

// EQTB entry for a glue
class EqtbGlue {
  Glue val;  // value of the object
  int level; // level at which this is defined
 public:
  EqtbGlue() : level(level_one) {}
  int get_level() const { return level; }
  Glue get_val() const { return val; }
  void val_and_level(Glue a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};

// EQTB entry for a token list
class EqtbToken {
  TokenList val;  // value of the object
  int level; // level at which this is defined
 public:
  EqtbToken(): level(level_one) {}
  TokenList get_val()const { return val; }
  int get_level() const { return level; }
  void val_and_level(TokenList a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};

// EQTB entry for a box
class EqtbBox {
  int level;  // level at which this is defined
  Xmlp val; // value of the object
 public:
  void set_val(Xmlp X) { val = X; }
  Xmlp get_val() const { return val; }
  int get_level() const { return level; }
  EqtbBox() : level(level_one) {}
  void val_and_level(Xmlp a, int b) { val = a; level = b; }
  bool must_push(int l) const { return level != l&& l>level_one;}
};

