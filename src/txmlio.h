#pragma once
#include "txio.h"

// -*- C++ -*-
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

enum x_type { xt_letter, xt_space, xt_digit, xt_ok_first, xt_other, xt_invalid };

class EntityDef {
    std::string name;
    std::string value;

public:
    EntityDef(std::string a, std::string b) : name(std::move(a)), value(std::move(b)) {}
    [[nodiscard]] auto has_name(const std::string &x) const -> bool { return x == name; }
    auto               get_val() -> std::string { return value; }
};

class XmlIO {
    Buffer                  B; // holds current element
    Buffer                  aux;
    Buffer                  line_buffer; // holds current line
    LinePtr                 lines;       // input file
    int                     cur_line{};
    std::array<x_type, 128> Type{};
    std::vector<codepoint>  input_line;  // current line
    std::vector<codepoint>  reread_list; // current line
    codepoint               cur_char;    // current character in some cases

    Xml *                  cur_xml{};
    std::vector<Xml *>     cur_stack;
    size_t                 cur_line_len{};
    size_t                 input_line_pos{};
    int                    enc{}; // 0=utf8, 1=latin1
    void                   error(const std::string &s) const;
    std::vector<EntityDef> entities;
    int                    nb_cond{};
    bool                   eof_ok{};

private:
    auto               peek_char() -> codepoint;
    void               skip_char();
    auto               next_char() -> codepoint;
    void               skip_space();
    void               next_line();
    [[nodiscard]] auto at_eol() const -> bool { return input_line_pos >= cur_line_len; }
    void               scan_name();
    void               scan_name(uchar c);
    void               scan_name(uchar c1, uchar c2);
    void               parse_lt();
    void               parse_end();
    void               parse_dec();
    void               parse_dec_comment();
    void               parse_dec_entity();
    void               parse_dec_element();
    void               parse_dec_attlist();
    void               parse_dec_doctype();
    void               parse_dec_cdata();
    void               parse_dec_conditional();
    void               parse_dec_notation();
    auto               parse_sys_pub() -> bool;
    void               expect(String s);
    void               parse_pi();
    void               parse_tag();
    void               parse_attributes();
    void               parse_att_val();
    void               parse_quoted();
    void               pop_this();
    void               flush_buffer();
    auto               expand_PEReference() -> bool;
    void               run();

public:
    XmlIO() : cur_char(' ') {}
    auto init(const std::string &name) -> bool;
    auto prun() -> Xml *;
};
