// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains commands needed for the RA

#include "tralics/Parser.h"

static Buffer            Tbuf;
std::vector<std::string> module_list;

namespace ra_ns {
    void fnhack(TokenList &c, TokenList &d, TokenList &aux);
} // namespace ra_ns

auto Parser::next_arg_is_project() -> bool {
    TokenList L = read_arg();
    if (L.empty()) return true;
    return L.front().is_one_token();
}

// --------------------------------------------------

// If c has no \footnote, copies it in aux;
// else copies only what is before, what is after is appended to d
void ra_ns::fnhack(TokenList &c, TokenList &d, TokenList &aux) {
    Hashtab &H  = the_parser.hash_table;
    int      bl = 0;
    while (!c.empty()) {
        Token x = c.front();
        c.pop_front();
        token_ns::check_brace(x, bl);
        if (x == H.footnote_token && bl == 0) {
            if (!d.empty()) {
                d.push_front(H.space_token);
                d.push_front(H.comma_token);
            }
            d.splice(d.begin(), c);
        } else {
            aux.push_back(x);
        }
    }
}

// User function \fn@hack\foo\bar
void Parser::fnhack() {
    Token     A = get_r_token();
    Token     B = get_r_token();
    TokenList La, Lb = get_mac_value(B), Li = get_mac_value(A);
    ra_ns::fnhack(Li, Lb, La);
    new_macro(La, A, false);
    new_macro(Lb, B, false);
}

// User function associated to \UR in the RA
void Parser::interpret_rc() {
    TokenList L = read_arg();
    Tbuf << bf_reset << L;
    Xml *res = new Xml(np_rclist, nullptr);
    the_stack.add_last(res);
    config_ns::check_RC(Tbuf, res);
}

// --------------------------------------------------
// \end{RAsection} or \tralics@pop@section
Xid  compo_id = Xid(decltype(Xid::value)(-1));
void Parser::T_rasection_end() {
    Xml *in = the_stack.top_stack();
    if (in->id == compo_id) in->compo_special();
    the_stack.pop(cst_rasection);
}

// \begin{RAsection} or \tralics@push@section
void Parser::T_rasection() {
    std::string name     = sT_arg_nopar();
    Istring     elt_name = the_names[np_rasection];
    auto        iname    = Istring(name);
    leave_h_mode();
    the_stack.add_nl();
    Xml *cur = new Xml(elt_name.empty() ? iname : elt_name, nullptr);
    if (!elt_name.empty()) cur->id.add_attribute(the_names["name"], iname);
    if (iname == the_names[cst_composition]) compo_id = cur->id;
    the_stack.push(the_names[cst_rasection], cur);
    string_define(0, name, false);
    Istring id = the_stack.add_new_anchor();
    create_label("section:" + name, id);
}

void Parser::push_module() {
    std::string aux = sT_arg_nopar();
    push_module(aux);
}

void Parser::push_module(const std::string &aux) {
    if (the_stack.in_h_mode()) { parse_error("Invalid \\begin{module} in a paragraph"); }
    leave_h_mode();
    the_stack.add_nl();
    the_stack.push1(np_module);
    //  refstepcounter("module",false); Assume done by the package
    start_paras(8, aux, false);
}
