#pragma once
// -*- C++ -*-
// Copyright INRIA/apics (Jose' Grimm) 2002-2004, 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txstring.h"

class Parser;

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

struct Equivalent : public CmdChr {
    int level{}; ///< level at which this is defined

    [[nodiscard]] auto must_push(int l) const -> bool { return level != l && l > 1; }
    void               reset() {
        CmdChr::reset();
        level = 0;
    }

    void setnl(CmdChr c) { CmdChr::operator=(c); }

    void special_prim(CmdChr b) {
        setnl(b);
        if (!is_undef()) level = 1;
    }

    void set(CmdChr c, int lvl) {
        setnl(c);
        level = lvl;
    }

    void primitive(CmdChr b) { set(b, 1); }
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
    TokenList L; // the token list to be restored
public:
    bool restricted{}; // the restricted flag to be restored

    void copy_and_reset(TokenList &X) {
        L.clear();
        L.splice(L.begin(), X);
    }
    void restore(TokenList &X) {
        X.clear();
        X.splice(X.begin(), L);
    }
    void set_restricted(bool b) { restricted = b; }
};

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
// \todo this needs some overhaul...
class SaveAux {
public:
    save_type type;    // the type of the real thing
    int       line{0}; // current line number at start

    SaveAux(save_type t) : type(t) {}
    virtual ~SaveAux() = default;

    virtual void unsave(bool, Parser &) = 0;
    void         set_line(int l) { line = l; }
    void         unsave_trace_aux(String s, int pos, bool rt);
    static void  restore_or_retain(bool rt, String s);
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAux {
public:
    boundary_type val; // explains why we opened a new group

    SaveAuxBoundary(boundary_type v) : SaveAux(st_boundary), val(v) {}
    ~SaveAuxBoundary() override = default;

    void unsave(bool trace, Parser &P) override;
    void dump(int n);
};

// This restores an integer value.
class SaveAuxInt : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in eqbt_int_table
    long   val;   // the value to be restored
public:
    SaveAuxInt(int l, size_t a, long b) : SaveAux(st_int), level(l), pos(a), val(b) {}
    ~SaveAuxInt() override = default;

    void unsave(bool trace, Parser &P) override;
};

// This restores a dimension
class SaveAuxDim : public SaveAux {
    int       level; // the level that was active when this was pushed
    size_t    pos;   // the position in eqbt_dim_table
    ScaledInt val;   // the value to be restored
public:
    SaveAuxDim(int l, size_t a, ScaledInt b) : SaveAux(st_int), level(l), pos(a), val(b) {}
    ~SaveAuxDim() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t cs;    // ths position in eqtb to be restored
    CmdChr val;   // the CmdChr to be restored
public:
    SaveAuxCmd(size_t a, const Equivalent &X) : SaveAux(st_cmd), level(X.level), cs(a), val({X.cmd, X.chr}) {}
    ~SaveAuxCmd() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in box_table to be restored
    Xml *  val;   // the value to be restored
public:
    SaveAuxBox(int l, size_t a, Xml *b) : SaveAux(st_box), level(l), pos(a), val(b) {}
    ~SaveAuxBox() override = default;

    void unsave(bool trace, Parser &P) override;
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAux {
    size_t pos; // the box number
    Xml *  val; // the value of the box
public:
    SaveAuxBoxend(size_t a, Xml *b) : SaveAux(st_box_end), pos(a), val(b) {}
    ~SaveAuxBoxend() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAux {
    int       level; // the level that was active when this was pushed
    size_t    pos;   // pthe position in toks_registers
    TokenList val;   // the value to be restored
public:
    SaveAuxToken(int l, size_t p, TokenList v) : SaveAux(st_token), level(l), pos(p), val(std::move(v)) {}
    ~SaveAuxToken() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in glue_table
    Glue   val;   // the value to be restored
public:
    SaveAuxGlue(int l, size_t p, Glue g) : SaveAux(st_glue), level(l), pos(p), val(g) {}
    ~SaveAuxGlue() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure for restoring glue
class SaveAuxString : public SaveAux {
    int         level;
    size_t      pos; // the position in glue_table
    std::string val; // the value to be restored
public:
    SaveAuxString(int l, size_t p, std::string s) : SaveAux(st_string), level(l), pos(p), val(std::move(s)) {}
    ~SaveAuxString() override = default;

    void unsave(bool trace, Parser &P) override;
};

// data structure for a \begin{something}
class SaveAuxEnv : public SaveAux {
public:
    std::string oldname;
    std::string name;
    int         line;
    Token       token;
    CmdChr      cc;

    SaveAuxEnv(std::string a, std::string aa, int ll, Token b, CmdChr c)
        : SaveAux(st_env), oldname(std::move(a)), name(std::move(aa)), line(ll), token(b), cc(c) {}
    ~SaveAuxEnv() override = default;

    void set_line(int x) { line = x; }
    void unsave(bool trace, Parser &P) override;
};

// data structure for a font change
class SaveAuxFont : public SaveAux {
    int     level; // the level that was active when this was pushed
    long    value; // the value to be restored
    Istring color; // the color to restore
public:
    SaveAuxFont(int l, long v, Istring c) : SaveAux(st_font), level(l), value(v), color(c) {}
    ~SaveAuxFont() override = default;

    void unsave(bool trace, Parser &P) override;
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAux {
    Token value; // the token to pop
public:
    SaveAuxAftergroup(Token v) : SaveAux(st_save), value(v) {}
    ~SaveAuxAftergroup() override = default;

    void unsave(bool trace, Parser &P) override;
};
