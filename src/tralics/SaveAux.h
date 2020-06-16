#pragma once
#include "Parser.h"
#include "enums.h"
#include "util.h"

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
// \todo this needs some overhaul...
class SaveAux {
public:
    Parser &  P;
    save_type type;    // the type of the real thing
    int       line{0}; // current line number at start

    SaveAux(Parser &p, save_type t) : P(p), type(t) {}
    virtual ~SaveAux() = default;

    virtual void unsave() = 0;
    static void  restore_or_retain(bool rt, String s); // \todo this is pure logging, does nothing
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAux {
public:
    boundary_type val; // explains why we opened a new group

    SaveAuxBoundary(Parser &p, boundary_type v) : SaveAux(p, st_boundary), val(v) {}

    void unsave() override;
    void dump(int n);
};

// This restores an integer value.
class SaveAuxInt : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in eqbt_int_table
    long   val;   // the value to be restored
public:
    SaveAuxInt(Parser &p, int l, size_t a, long b) : SaveAux(p, st_int), level(l), pos(a), val(b) {}

    void unsave() override;
};

// This restores a dimension
class SaveAuxDim : public SaveAux {
    int       level; // the level that was active when this was pushed
    size_t    pos;   // the position in eqbt_dim_table
    ScaledInt val;   // the value to be restored
public:
    SaveAuxDim(Parser &p, int l, size_t a, ScaledInt b) : SaveAux(p, st_int), level(l), pos(a), val(b) {}

    void unsave() override;
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t cs;    // ths position in eqtb to be restored
    CmdChr val;   // the CmdChr to be restored
public:
    SaveAuxCmd(Parser &p, size_t a, const Equivalent &X) : SaveAux(p, st_cmd), level(X.level), cs(a), val({X.cmd, X.chr}) {}

    void unsave() override;
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in box_table to be restored
    Xml *  val;   // the value to be restored
public:
    SaveAuxBox(Parser &p, int l, size_t a, Xml *b) : SaveAux(p, st_box), level(l), pos(a), val(b) {}

    void unsave() override;
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAux {
    size_t pos; // the box number
    Xml *  val; // the value of the box
public:
    SaveAuxBoxend(Parser &p, size_t a, Xml *b) : SaveAux(p, st_box_end), pos(a), val(b) {}

    void unsave() override;
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAux {
    int       level; // the level that was active when this was pushed
    size_t    pos;   // pthe position in toks_registers
    TokenList val;   // the value to be restored
public:
    SaveAuxToken(Parser &P, int l, size_t p, TokenList v) : SaveAux(P, st_token), level(l), pos(p), val(std::move(v)) {}

    void unsave() override;
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAux {
    int    level; // the level that was active when this was pushed
    size_t pos;   // the position in glue_table
    Glue   val;   // the value to be restored
public:
    SaveAuxGlue(Parser &P, int l, size_t p, Glue g) : SaveAux(P, st_glue), level(l), pos(p), val(g) {}

    void unsave() override;
};

// data structure for restoring glue
class SaveAuxString : public SaveAux {
    int         level;
    size_t      pos; // the position in glue_table
    std::string val; // the value to be restored
public:
    SaveAuxString(Parser &P, int l, size_t p, std::string s) : SaveAux(P, st_string), level(l), pos(p), val(std::move(s)) {}

    void unsave() override;
};

// data structure for a \begin{something}
class SaveAuxEnv : public SaveAux {
public:
    std::string oldname;
    std::string name;
    int         line;
    Token       token;
    CmdChr      cc;

    SaveAuxEnv(Parser &p, std::string a, std::string aa, int ll, Token b, CmdChr c)
        : SaveAux(p, st_env), oldname(std::move(a)), name(std::move(aa)), line(ll), token(b), cc(c) {}

    void unsave() override;
};

// data structure for a font change
class SaveAuxFont : public SaveAux {
    long    level; // the level that was active when this was pushed
    long    value; // the value to be restored
    Istring color; // the color to restore
public:
    SaveAuxFont(Parser &p, long l, long v, Istring c) : SaveAux(p, st_font), level(l), value(v), color(std::move(c)) {}

    void unsave() override;
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAux {
    Token value; // the token to pop
public:
    SaveAuxAftergroup(Parser &p, Token v) : SaveAux(p, st_save), value(v) {}

    void unsave() override;
};
