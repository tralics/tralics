#pragma once
// -*- C++ -*-
// Copyright INRIA/apics (Jose' Grimm) 2002-2004, 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txscaled.h"
#include "txstring.h"
#include "txtokenlist.h"
#include <utility>

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
    int    level; // level at which this is defined
    CmdChr value; // the modifier
public:
    [[nodiscard]] auto get_level() const -> int { return level; }
    [[nodiscard]] auto get_cmdchr() const -> CmdChr { return value; }
    [[nodiscard]] auto get_cmd() const -> symcodes { return value.get_cmd(); }
    [[nodiscard]] auto get_chr() const -> subtypes { return value.get_chr(); }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
    void               reset() {
        level = level_zero;
        value.reset();
    }
    void special_prim(CmdChr b) {
        value = b;
        if (!b.is_undef()) level = level_one;
    }
    void primitive(CmdChr b) {
        value = b;
        level = level_one;
    }
    void set(CmdChr c, int lvl) {
        value = c;
        level = lvl;
    }
    void               setnl(CmdChr c) { value = c; }
    [[nodiscard]] auto is_undefined() const -> bool { return value.is_undef(); }
    [[nodiscard]] auto is_user() const -> bool { return value.is_user(); }
    [[nodiscard]] auto is_undef_or_relax() const -> bool { return value.is_undef_or_relax(); }
};

class RestoreVbSpace {
    Token   value; // value to restore
    Parser *P;     // this object has a pointer to the parser
public:
    RestoreVbSpace(Parser *p);
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

class SaveErrTok {
private:
    Token val;

public:
    SaveErrTok(Token t);
    ~SaveErrTok();
};

class InLoadHandler {
    bool in_load; // a boolean value to restore
public:
    static bool global_in_load;
    InLoadHandler() : in_load(global_in_load) { global_in_load = true; }
    ~InLoadHandler() { global_in_load = in_load; }
};

class InUrlHandler {
    bool in_url; // a boolean value to restore
public:
    static bool global_in_url;
    InUrlHandler() : in_url(global_in_url) { global_in_url = true; }
    ~InUrlHandler() { global_in_url = in_url; }
};

// this saves two values
class SaveState {
    TokenList L;          // the token list to be restored
    bool      restricted; // the restricted flag to be restored
public:
    void copy_and_reset(TokenList &X) {
        L.clear();
        L.splice(L.begin(), X);
    }
    void restore(TokenList &X) {
        X.clear();
        X.splice(X.begin(), L);
    }
    void               set_restricted(bool b) { restricted = b; }
    [[nodiscard]] auto get_restricted() const -> bool { return restricted; }
};

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
class SaveAux {
public:
    save_type type;    // the type of the real thing
    int       line_no; // current line number at start
public:
    virtual void unsave(bool, Parser &) = 0;
    virtual ~SaveAux()                  = default;
    SaveAux(save_type t) : type(t), line_no(0) {}
    void               set_line(int l) { line_no = l; }
    [[nodiscard]] auto get_line() const -> int { return line_no; }
    void               unsave_trace_aux(String s, int pos, bool rt);
    void               restore_or_retain(bool, String) const;
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAux {
    boundary_type val; // explains why we opened a new group
public:
    [[nodiscard]] auto get_val() const -> boundary_type { return val; }
    void               unsave(bool, Parser &) override;
    SaveAuxBoundary(boundary_type v) : SaveAux(st_boundary), val(v) {}
    ~SaveAuxBoundary() override = default;
    void dump(int);
};

// This restores an integer value.
class SaveAuxInt : public SaveAux {
    int level; // the level that was active when this was pushed
    int pos;   // the position in eqbt_int_table
    int val;   // the value to be restored
public:
    SaveAuxInt(int l, int a, int b) : SaveAux(st_int), level(l), pos(a), val(b) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxInt() override = default;
};

// This restores a dimension
class SaveAuxDim : public SaveAux {
    int       level; // the level that was active when this was pushed
    int       pos;   // the position in eqbt_dim_table
    ScaledInt val;   // the value to be restored
public:
    SaveAuxDim(int l, int a, ScaledInt b) : SaveAux(st_int), level(l), pos(a), val(b) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxDim() override = default;
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAux {
    int    level; // the level that was active when this was pushed
    int    cs;    // ths position in eqtb to be restored
    CmdChr val;   // the CmdChr to be restored
public:
    SaveAuxCmd(int a, Equivalent X) : SaveAux(st_cmd), level(X.get_level()), cs(a), val(X.get_cmdchr()) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxCmd() override = default;
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAux {
    int  level; // the level that was active when this was pushed
    int  pos;   // the position in box_table to be restored
    Xml *val;   // the value to be restored
public:
    SaveAuxBox(int l, int a, Xml *b) : SaveAux(st_box), level(l), pos(a), val(b) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxBox() override = default;
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAux {
    int  pos; // the box number
    Xml *val; // the value of the box
public:
    SaveAuxBoxend(int a, Xml *b) : SaveAux(st_box_end), pos(a), val(b) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxBoxend() override = default;
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAux {
    int       level; // the level that was active when this was pushed
    int       pos;   // pthe position in toks_registers
    TokenList val;   // the value to be restored
public:
    SaveAuxToken(int l, int p, TokenList v) : SaveAux(st_token), level(l), pos(p), val(std::move(v)) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxToken() override = default;
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAux {
    int  level; // the level that was active when this was pushed
    int  pos;   // the position in glue_table
    Glue val;   // the value to be restored
public:
    SaveAuxGlue(int l, int p, Glue g) : SaveAux(st_glue), level(l), pos(p), val(g) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxGlue() override = default;
};

// data structure for restoring glue
class SaveAuxString : public SaveAux {
    int    level;
    int    pos; // the position in glue_table
    string val; // the value to be restored
public:
    SaveAuxString(int l, int p, string s) : SaveAux(st_string), level(l), pos(p), val(std::move(s)) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxString() override = default;
};

// data structure for a \begin{something}
// had an unused field: int val;
class SaveAuxEnv : public SaveAux {
    string oldname;
    string newname;
    int    line;
    Token  T;
    CmdChr cc;

public:
    void               set_line(int x) { line = x; }
    [[nodiscard]] auto get_val() const -> CmdChr { return cc; }
    [[nodiscard]] auto get_token() const -> Token { return T; }
    auto               get_name() -> string { return newname; }
    void               unsave(bool, Parser &) override;
    SaveAuxEnv(string a, string aa, int ll, Token b, CmdChr c)
        : SaveAux(st_env), oldname(std::move(a)), newname(std::move(aa)), line(ll), T(b), cc(c) {}
    ~SaveAuxEnv() override = default;
};

// data structure for a font change
class SaveAuxFont : public SaveAux {
    int     level; // the level that was active when this was pushed
    int     value; // the value to be restored
    Istring color; // the color to restore
public:
    void unsave(bool, Parser &) override;
    SaveAuxFont(int l, int v, Istring c) : SaveAux(st_font), level(l), value(v), color(c) {}
    ~SaveAuxFont() override = default;
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAux {
    Token value; // the token to pop
public:
    SaveAuxAftergroup(Token v) : SaveAux(st_save), value(v) {}
    void unsave(bool, Parser &) override;
    ~SaveAuxAftergroup() override = default;
};

// EQBT entry for an integer
// Every eqtb entry has a level. Level_zero means undefined
// Level_one is the outer level. The old value must be saved in case
// the current level is different fron the old one, and is greater than one.
// These objects are defined at level_one
class EqtbInt {
    int val{0};           // value of the object
    int level{level_one}; // level at which this is defined
public:
    EqtbInt() = default;
    [[nodiscard]] auto get_val() const -> int { return val; }
    void               set_val(int x) { val = x; }
    void               set_level(int x) { level = x; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    void               val_and_level(int a, int b) {
        val   = a;
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};

class EqtbString {
    string val;              // value of the object
    int    level{level_one}; // level at which this is defined
public:
    EqtbString() : val("") {}
    [[nodiscard]] auto get_val() const -> string { return val; }
    void               set_val(string x) { val = std::move(x); }
    void               set_level(int x) { level = x; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    void               val_and_level(string a, int b) {
        val   = std::move(a);
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};

// EQBT entry for a dimension.
class EqtbDim {
    ScaledInt val;              // value of the object
    int       level{level_one}; // level at which this is defined
public:
    EqtbDim() : val(0) {}
    [[nodiscard]] auto get_val() const -> ScaledInt { return val; }
    void               set_val(ScaledInt x) { val = x; }
    void               set_level(int x) { level = x; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    void               val_and_level(ScaledInt a, int b) {
        val   = a;
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};

// EQTB entry for a glue
class EqtbGlue {
    Glue val;              // value of the object
    int  level{level_one}; // level at which this is defined
public:
    EqtbGlue() = default;
    [[nodiscard]] auto get_level() const -> int { return level; }
    [[nodiscard]] auto get_val() const -> Glue { return val; }
    void               val_and_level(Glue a, int b) {
        val   = a;
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};

// EQTB entry for a token list
class EqtbToken {
    TokenList val;              // value of the object
    int       level{level_one}; // level at which this is defined
public:
    EqtbToken() = default;
    [[nodiscard]] auto get_val() const -> TokenList { return val; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    void               val_and_level(TokenList a, int b) {
        val   = std::move(a);
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};

// EQTB entry for a box
class EqtbBox {
    int  level{level_one}; // level at which this is defined
    Xml *val;              // value of the object
public:
    void               set_val(Xml *X) { val = X; }
    [[nodiscard]] auto get_val() const -> Xml * { return val; }
    [[nodiscard]] auto get_level() const -> int { return level; }
    EqtbBox() = default;
    void val_and_level(Xml *a, int b) {
        val   = a;
        level = b;
    }
    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > level_one; }
};
