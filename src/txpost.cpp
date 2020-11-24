// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Post processing for tralics

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/WordList.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/format.h>

namespace {
    std::vector<std::pair<int, std::string>> refindex_list; // list of all \ref to \index
} // namespace

namespace post_ns {
    void print_no_title(int i, String s);
} // namespace post_ns

void Parser::user_XML_swap(subtypes c) {
    if (c == zero_code)
        the_xmlB = the_xmlA;
    else if (c == one_code)
        the_xmlA = the_xmlB;
    else {
        Xml *cc  = the_xmlA;
        the_xmlA = the_xmlB;
        the_xmlB = cc;
    }
}

// puts in the_xmlA the element whose id is the argument of the command
void Parser::user_XML_fetch() {
    Token T  = cur_tok;
    auto  n  = scan_int(T);
    the_xmlA = (n <= 0) ? nullptr : the_stack.fetch_by_id(to_unsigned(n));
}

void Parser::user_XML_modify(subtypes c) {
    if (c == xml_setA_code) {
        scan_box(xmlA_location);
        return;
    }
    if (c == xml_setB_code) {
        scan_box(xmlB_location);
        return;
    }
    if (c == xml_parent_code) {
        the_xmlB = the_stack.find_parent(the_xmlA);
        return;
    }
    auto n = scan_int(cur_tok);
    switch (c) {
    case xml_get_code: the_xmlB = the_xmlA != nullptr ? the_xmlA->value_at(n) : nullptr; return;
    case xml_ins_code:
        if ((the_xmlA == nullptr) || n < 0) return;
        if (the_xmlB != nullptr) the_xmlA->insert_at(to_unsigned(n), the_xmlB->deep_copy());
        return;
    case xml_del_code:
        if ((the_xmlA == nullptr) || n < 0) return;
        the_xmlA->remove_at(n);
        return;
    case xml_set_code: // how to tell the user that it failed ?
        if ((the_xmlA == nullptr) || n < 0) return;
        if (the_xmlB != nullptr) the_xmlA->put_at(n, the_xmlB->deep_copy());
        return;
    default: return;
    }
}

// This implements \label{foo}; second argument is the anchor id
// We enter foo in the hashtab, and look at the LabelInfo value.
// If the label is undefined, we define it,

void Parser::create_label(const std::string &X, const std::string &S) {
    auto  m = std::string(X);
    auto *V = labinfo(m);
    if (V->set_defined()) {
        multiple_label(m, V->lineno, V->filename);
    } else {
        V->id       = S;
        V->lineno   = get_cur_line();
        V->filename = get_cur_filename();
        defined_labels.emplace_back(S, V);
    }
}

void tralics_ns::add_ref(long v, const std::string &s, bool idx) {
    auto B = std::string(s);
    if (idx)
        refindex_list.emplace_back(v, B);
    else
        ref_list.emplace_back(v, B);
    auto *V = labinfo(B);
    if (V->lineno == 0) V->lineno = the_parser.get_cur_line();
    if (V->filename.empty()) V->filename = the_parser.get_cur_filename();
}

//
void tralics_ns::find_index_labels(std::vector<std::string> &W) {
    for (auto &i : refindex_list) {
        auto        E = to_unsigned(i.first);
        std::string V = i.second;
        auto *      L = labinfo(V);
        if (!L->defined) continue; // should not happen
        std::string B = L->id;
        scbuf.clear();
        scbuf.append(W[E]);
        if (!scbuf.empty()) scbuf.push_back(' ');
        scbuf.append(encode(B));
        W[E] = scbuf;
    }
}

inline auto dig_char(char c) -> bool { return c == '-' || (std::isdigit(c) != 0); }
