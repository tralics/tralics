#pragma once
#include "Parser.h"
#include "util.h"
#include <variant>

// This is a virtual class for saving an object that is in an EQTB table
// In general we have a level field, a position field and a value field
// TODO: this needs some overhaul...
class SaveAuxBase {
public:
    save_type type; // the type of the real thing
    int       line; // current line number at start
    long      level;

    SaveAuxBase(save_type t, long l) : type(t), line(the_parser.get_cur_line()), level(l) {}
    virtual ~SaveAuxBase() = default;
};

// A boundary object is created when we see an open brace, or something
// like that.
class SaveAuxBoundary : public SaveAuxBase {
public:
    boundary_type val; // explains why we opened a new group

    explicit SaveAuxBoundary(boundary_type v) : SaveAuxBase(st_boundary, 0), val(v) {}
    ~SaveAuxBoundary() override;

    void dump(int n);
};

// This restores an integer value.
class SaveAuxInt : public SaveAuxBase {
    size_t pos; // the position in eqbt_int_table
    long   val; // the value to be restored
public:
    SaveAuxInt(long l, size_t a, long b) : SaveAuxBase(st_int, l), pos(a), val(b) {}
    ~SaveAuxInt() override;
};

// This restores a dimension
class SaveAuxDim : public SaveAuxBase {
    size_t    pos; // the position in eqbt_dim_table
    ScaledInt val; // the value to be restored
public:
    SaveAuxDim(long l, size_t a, ScaledInt b) : SaveAuxBase(st_int, l), pos(a), val(b) {}
    ~SaveAuxDim() override;
};

// data structure for restoring a command
class SaveAuxCmd : public SaveAuxBase {
    size_t cs;  // ths position in eqtb to be restored
    CmdChr val; // the CmdChr to be restored
public:
    SaveAuxCmd(size_t a, const EqtbCmdChr &X) : SaveAuxBase(st_cmd, X.level), cs(a), val(X.val) {}
    ~SaveAuxCmd() override;
};

// data structure fopr restoring a box
class SaveAuxBox : public SaveAuxBase {
    size_t pos; // the position in box_table to be restored
    Xml   *val; // the value to be restored
public:
    SaveAuxBox(long l, size_t a, Xml *b) : SaveAuxBase(st_box, l), pos(a), val(b) {}
    ~SaveAuxBox() override;
};

// case of \setbox0=\hbox{...} , remember the number and the box
class SaveAuxBoxend : public SaveAuxBase {
    size_t pos; // the box number
    Xml   *val; // the value of the box
public:
    SaveAuxBoxend(size_t a, Xml *b) : SaveAuxBase(st_box_end, 0), pos(a), val(b) {}
    ~SaveAuxBoxend() override;
};

// data structure for restoring a token list
class SaveAuxToken : public SaveAuxBase {
    size_t    pos; // pthe position in toks_registers
    TokenList val; // the value to be restored
public:
    SaveAuxToken(long l, size_t p, TokenList v) : SaveAuxBase(st_token, l), pos(p), val(std::move(v)) {}
    ~SaveAuxToken() override;
};

// data structure for restoring glue
class SaveAuxGlue : public SaveAuxBase {
    size_t pos; // the position in glue_table
    Glue   val; // the value to be restored
public:
    SaveAuxGlue(long l, size_t p, const Glue &g) : SaveAuxBase(st_glue, l), pos(p), val(g) {}
    ~SaveAuxGlue() override;
};

// data structure for restoring glue
class SaveAuxString : public SaveAuxBase {
    size_t      pos; // the position in glue_table
    std::string val; // the value to be restored
public:
    SaveAuxString(long l, size_t p, std::string s) : SaveAuxBase(st_string, l), pos(p), val(std::move(s)) {}
    ~SaveAuxString() override;
};

// data structure for a \begin{something}
class SaveAuxEnv : public SaveAuxBase {
public:
    std::string oldname;
    std::string name;
    int         lline;
    Token       token;
    CmdChr      cc;

    SaveAuxEnv(std::string a, std::string aa, int ll, Token b, CmdChr c)
        : SaveAuxBase(st_env, 0), oldname(std::move(a)), name(std::move(aa)), lline(ll), token(b), cc(c) {}
    ~SaveAuxEnv() override { the_parser.set_cur_env_name(oldname, lline); };
};

// data structure for a font change
class SaveAuxFont : public SaveAuxBase {
    long        val;   // the value to be restored
    std::string color; // the color to restore
public:
    SaveAuxFont(long l, long v, std::string c) : SaveAuxBase(st_font, l), val(v), color(std::move(c)) {}
    ~SaveAuxFont() override;
};

// This pops a token at the end of the group. Does not depend on a level
class SaveAuxAftergroup : public SaveAuxBase {
    Token val; // the token to pop
public:
    explicit SaveAuxAftergroup(Token v) : SaveAuxBase(st_save, 0), val(v) {}
    ~SaveAuxAftergroup() override;
};

using SaveAux = std::variant<SaveAuxBoundary, SaveAuxInt, SaveAuxDim, SaveAuxCmd, SaveAuxBox, SaveAuxBoxend, SaveAuxToken, SaveAuxGlue,
                             SaveAuxString, SaveAuxEnv, SaveAuxFont, SaveAuxAftergroup>;
