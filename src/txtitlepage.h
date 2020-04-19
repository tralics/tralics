#pragma once
#include "tralics/Xml.h"
#include <utility>

// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002-2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains stuff for title-page element.

class TitlePageFullLine;

static const int tp_p_flag    = 2;
static const int tp_e_flag    = 4;
static const int tp_q_flag    = 8;
static const int tp_plus_flag = 16;
static const int tp_A_flag    = 32;
static const int tp_B_flag    = 64;
static const int tp_C_flag    = 96;

// data for a titlepage item
class TitlePageAux {
    std::string T1, T2, T3, T4; // the four strings
    size_t      idx{0};         // index into titlepage::Data
    size_t      xflags{0};      // flags associated to the object
    tpi_vals    type{tpi_zero}; // type of object

public:
    auto classify(tpi_vals w, int &state) -> bool;
    auto convert(int i) -> Xml *;
    auto convert(int i, Xml *r) -> Xml *;
    auto convert(int i, Istring s) -> Xml * { return convert(i, new Xml(std::move(s))); }
    void dump(size_t k);
    void exec_start(size_t k);
    void exec_post();
    void exec(size_t v, bool vb);
    void set_T1(std::string x) { T1 = std::move(x); }
    void set_T2(std::string x) { T2 = std::move(x); }
    void set_T3(std::string x) { T3 = std::move(x); }
    void set_T4(std::string x) { T4 = std::move(x); }

    auto               get_type() -> tpi_vals { return type; }
    auto               get_typeref() -> tpi_vals & { return type; }
    [[nodiscard]] auto get_flags2() const -> size_t { return 32U * (xflags / 32U); }
    [[nodiscard]] auto has_u_flags() const -> bool { return (xflags & 1) != 0; }
    [[nodiscard]] auto has_p_flags() const -> bool { return (xflags & tp_p_flag) != 0; }
    [[nodiscard]] auto has_e_flags() const -> bool { return (xflags & tp_e_flag) != 0; }
    [[nodiscard]] auto has_q_flags() const -> bool { return (xflags & tp_q_flag) != 0; }
    [[nodiscard]] auto has_plus_flags() const -> bool { return (xflags & tp_plus_flag) != 0; }
    TitlePageAux() = default;
    TitlePageAux(TitlePageFullLine &X);
    auto               find_UR(String s, size_t n) const -> size_t;
    auto               get_T1() -> std::string { return T1; }
    auto               get_T2() -> std::string { return T2; }
    auto               get_T3() -> std::string { return T3; }
    auto               get_T4() -> std::string { return T4; }
    [[nodiscard]] auto find_cmd(const std::string &s) const -> bool;
    void               move_T1T2(std::string x) {
        T1 = T2;
        T2 = std::move(x);
    }
    auto increment_flag() -> bool;
    void decode_flags() const;
};

// One item if a titlepage
class TpiOneItem {
    char        p1{};  // first character modifier
    char        p2{};  // second character modifier
    std::string value; // the value
    tpi_vals    v;     // the type (none, string, command, or XML element)
public:
    TpiOneItem() { reset(); }
    [[nodiscard]] auto has_a_char() const -> bool { return p1 != 0 || p2 != 0; }
    [[nodiscard]] auto noval() const -> bool { return v == tpi_noval; }
    [[nodiscard]] auto is_elt() const -> bool { return v == tpi_elt; }
    [[nodiscard]] auto is_str() const -> bool { return v == tpi_str; }
    [[nodiscard]] auto is_cmd() const -> bool { return v == tpi_cmd; }
    [[nodiscard]] auto is_alias() const -> bool { return v == tpi_alias; }
    [[nodiscard]] auto only_dash() const -> bool { return p1 == '-' && p2 == 0; }
    [[nodiscard]] auto question() const -> bool { return p1 == '?' && p2 == 0; }
    [[nodiscard]] auto plus() const -> bool { return p1 == '+' && p2 == 0; }
    [[nodiscard]] auto quest_plus() const -> bool { return p1 == '?' && p2 == '+'; }
    [[nodiscard]] auto second_char() const -> bool { return p2 != 0; }
    [[nodiscard]] auto get_v() const -> tpi_vals { return v; }
    [[nodiscard]] auto get_value() const -> std::string { return value; }
    [[nodiscard]] auto get_p1() const -> char { return p1; }
    void               set_p2(char c) { p2 = c; }
    void               set_p1(char c) { p1 = c; }
    //  void bad() { v = tpi_err; }
    void set_v(tpi_vals V) { v = V; }
    void set_value(const Buffer &b) { value = b.to_string(); }
    void reset();
};

// temporary class, will bew copied into a TitlePageAux
class TitlePageFullLine {
    TpiOneItem item1, item2, item3, item4; // the four items
    size_t     flags{};                    // the flags
public:
    friend class titlepage;
    friend class TitlePageAux;
    auto               read() -> int;
    void               kill();
    auto               classify(int w, int state) -> tpi_vals;
    [[nodiscard]] auto get_flags() const -> size_t { return flags; }
    auto               encode_flags(char c1, char c2) -> bool;
};

class TitlePage {
    size_t len2{1};       // size of bigtable and Data
    bool   valid{false};  // is this initialised and not killed ?
    int    size{0};       // allocated size of bigtable
    Xml ** Data{nullptr}; // the array of xml data
public:
    std::vector<TitlePageAux> bigtable; // the table
    int                       state{};  // current state of the parser
    TitlePage() = default;
    auto               operator[](size_t k) -> Xml *& { return Data[k]; }
    auto               get_bigtable(size_t k) -> TitlePageAux & { return bigtable[k]; }
    [[nodiscard]] auto get_len2() const -> size_t { return len2; }
    [[nodiscard]] auto is_valid() const -> bool { return valid; }
    void               make_invalid() { valid = false; }
    void               make_valid() { valid = true; }
    void               start_thing(bool verbose);
    void               parse();
    auto               increase_data() -> size_t { return len2++; }
    void               check_size();
    [[nodiscard]] auto find_UR(const std::string &s, const std::string &name) const -> size_t;
    [[nodiscard]] auto find_cmd(const std::string &s) const -> size_t;
};
