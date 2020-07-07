#pragma once
#include "Parser.h"
#include "util.h"
#include <variant>

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
// \todo this needs some overhaul...
class SaveAuxBase {
public:
    Parser &  P;
    save_type type; // the type of the real thing
    int       line; // current line number at start
    long      level;

    SaveAuxBase(Parser &p, save_type t, long l) : P(p), type(t), line(P.get_cur_line()), level(l) {}
    virtual ~SaveAuxBase() = default;
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAuxBase {
public:
    boundary_type val; // explains why we opened a new group

    SaveAuxBoundary(Parser &p, boundary_type v) : SaveAuxBase(p, st_boundary, 0), val(v) {}
    ~SaveAuxBoundary() override;

    void dump(int n);
};

// This restores an integer value.
class SaveAuxInt : public SaveAuxBase {
    size_t pos; // the position in eqbt_int_table
    long   val; // the value to be restored
public:
    SaveAuxInt(Parser &p, long l, size_t a, long b) : SaveAuxBase(p, st_int, l), pos(a), val(b) {}
    ~SaveAuxInt() override;
};

// This restores a dimension
class SaveAuxDim : public SaveAuxBase {
    size_t    pos; // the position in eqbt_dim_table
    ScaledInt val; // the value to be restored
public:
    SaveAuxDim(Parser &p, long l, size_t a, ScaledInt b) : SaveAuxBase(p, st_int, l), pos(a), val(b) {}
    ~SaveAuxDim() override;
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAuxBase {
    size_t cs;  // ths position in eqtb to be restored
    CmdChr val; // the CmdChr to be restored
public:
    SaveAuxCmd(Parser &p, size_t a, const Equivalent &X) : SaveAuxBase(p, st_cmd, X.level), cs(a), val({X.cmd, X.chr}) {}
    ~SaveAuxCmd() override;
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAuxBase {
    size_t pos; // the position in box_table to be restored
    Xml *  val; // the value to be restored
public:
    SaveAuxBox(Parser &p, long l, size_t a, Xml *b) : SaveAuxBase(p, st_box, l), pos(a), val(b) {}
    ~SaveAuxBox() override;
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAuxBase {
    size_t pos; // the box number
    Xml *  val; // the value of the box
public:
    SaveAuxBoxend(Parser &p, size_t a, Xml *b) : SaveAuxBase(p, st_box_end, 0), pos(a), val(b) {}
    ~SaveAuxBoxend() override;
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAuxBase {
    size_t    pos; // pthe position in toks_registers
    TokenList val; // the value to be restored
public:
    SaveAuxToken(Parser &P, long l, size_t p, TokenList v) : SaveAuxBase(P, st_token, l), pos(p), val(std::move(v)) {}
    ~SaveAuxToken() override;
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAuxBase {
    size_t pos; // the position in glue_table
    Glue   val; // the value to be restored
public:
    SaveAuxGlue(Parser &P, long l, size_t p, Glue g) : SaveAuxBase(P, st_glue, l), pos(p), val(g) {}
    ~SaveAuxGlue() override;
};

// data structure for restoring glue
class SaveAuxString : public SaveAuxBase {
    size_t      pos; // the position in glue_table
    std::string val; // the value to be restored
public:
    SaveAuxString(Parser &P, long l, size_t p, std::string s) : SaveAuxBase(P, st_string, l), pos(p), val(std::move(s)) {}
    ~SaveAuxString() override;
};

// data structure for a \begin{something}
class SaveAuxEnv : public SaveAuxBase {
public:
    std::string oldname;
    std::string name;
    int         line;
    Token       token;
    CmdChr      cc;

    SaveAuxEnv(Parser &p, std::string a, std::string aa, int ll, Token b, CmdChr c)
        : SaveAuxBase(p, st_env, 0), oldname(std::move(a)), name(std::move(aa)), line(ll), token(b), cc(c) {}
    ~SaveAuxEnv() override { P.set_cur_env_name(oldname, line); };
};

// data structure for a font change
class SaveAuxFont : public SaveAuxBase {
    long        val;   // the value to be restored
    std::string color; // the color to restore
public:
    SaveAuxFont(Parser &p, long l, long v, std::string c) : SaveAuxBase(p, st_font, l), val(v), color(std::move(c)) {}
    ~SaveAuxFont() override;
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAuxBase {
    Token val; // the token to pop
public:
    SaveAuxAftergroup(Parser &p, Token v) : SaveAuxBase(p, st_save, 0), val(v) {}
    ~SaveAuxAftergroup() override;
};

using SaveAux = std::variant<SaveAuxBoundary, SaveAuxInt, SaveAuxDim, SaveAuxCmd, SaveAuxBox, SaveAuxBoxend, SaveAuxToken, SaveAuxGlue,
                             SaveAuxString, SaveAuxEnv, SaveAuxFont, SaveAuxAftergroup>;
