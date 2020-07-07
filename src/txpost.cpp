// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Post processing for tralics

#include "txpost.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txtrees.h"
#include <fmt/format.h>

namespace {
    std::vector<std::pair<int, std::string>> refindex_list; // list of all \ref to \index
} // namespace

namespace post_ns {
    void print_no_title(int i, String s);
} // namespace post_ns

// For finding words.
namespace all_words_ns {
    void add_a_word(String s, size_t h);
} // namespace all_words_ns

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
        my_stats.one_more_label();
        V->id       = S;
        V->lineno   = get_cur_line();
        V->filename = get_cur_filename();
        defined_labels.emplace_back(S, V);
    }
}

void tralics_ns::add_ref(long v, const std::string &s, bool idx) {
    the_parser.my_stats.one_more_ref();
    auto B = std::string(s);
    if (idx)
        refindex_list.emplace_back(v, B);
    else
        ref_list.emplace_back(v, B);
    auto *V = labinfo(B);
    if (!V->set_used()) the_parser.my_stats.one_more_used_ref();
    if (V->lineno == 0) V->lineno = the_parser.get_cur_line();
    if (V->filename.empty()) V->filename = the_parser.get_cur_filename();
}

// In ref_list, we have  (e,v), (e,v), (e,v) etc
// where E is the xid of a <ref> element, and V is an entry in the
// hash table of the label. After the translation is complete,
// we know the value of the label, and can add the attribute target=xxx.
void Parser::check_all_ids() {
    for (auto &i : ref_list) {
        int         E = i.first;
        std::string V = i.second;
        auto *      L = labinfo(V);
        if (!L->defined) {
            Logger::finish_seq();
            log_and_tty << "Error signaled in postprocessor\n"
                        << "undefined label `" << V << "' (first use at line " << L->lineno << " in file " << L->filename << ")";
            Xid(E).add_attribute(the_names["target"], V);
            std::string B = L->id;
            for (auto &removed_label : removed_labels) {
                if (removed_label.second == B) log_and_tty << "\n(Label was removed with `" << removed_label.first << "')";
                break;
            }
            log_and_tty << "\n";
            main_ns::nb_errs++;
        }
        std::string B = L->id;
        if (!B.empty()) Xid(E).add_attribute(the_names["target"], B);
    }
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

// Puts element T with its attribute list in the buffer.
// If w=0, we print <foo att-list/>
// If w=1, we print <foo att-list>
// if w=2, we print </foo>
void Buffer::push_back_elt(const std::string &name, Xid id, int w) {
    push_back('<');
    if (w == 2) push_back('/');
    append(name.c_str()); // NOLINT c_str because there might be a 0 char in name, \todo fix that
    if (w != 2) push_back(id.get_att());
    if (w == 0) push_back('/');
    push_back('>');
}

void Buffer::finish_xml_print() {
    *cur_fp << data();
    clear();
}
//--------------------------- Word stats

// This is called when a new word is seen.
void all_words_ns::add_a_word(String s, size_t h) {
    auto      H = h % 100;
    WordList *L = WL0[H];
    while (L != nullptr) {
        if (L->is_here(s, h)) {
            L->incr_freq();
            return;
        }
        L = L->get_next();
    }
    WL0[H] = new WordList(s, h, WL0[H]);
}

inline auto dig_char(char c) -> bool { return c == '-' || is_digit(c); }

void Buffer::new_word() {
    if (empty()) return;
    if (size() == 1) at(0) = 'x';
    bool ok = true;
    for (unsigned i = 0; i < size(); i++) {
        if (!dig_char(at(i))) {
            ok = false;
            break;
        }
    }
    if (ok) {
        clear();
        return;
    }
    ++nb_words;
    ok = true;
    if (is_upper_case(at(0))) {
        for (size_t i = 1; i < size(); i++) {
            if (!is_lower_case(at(i))) ok = false;
        }
        if (ok) at(0) += 'a' - 'A';
    }
    all_words_ns::add_a_word(data(), hashcode(6397));
    clear();
}
