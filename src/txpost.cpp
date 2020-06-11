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
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txtrees.h"
#include <fmt/format.h>

namespace {
    std::vector<std::pair<int, Istring>> refindex_list; // list of all \ref to \index
} // namespace

namespace post_ns {
    void print_no_title(int i, String s);
    auto is_entity(const std::string &s) -> size_t;
} // namespace post_ns

// For finding words.
namespace all_words_ns {
    int                         nb_words = 0;
    std::array<WordList *, 100> WL0;
    std::ofstream               fp;
    void                        add_a_word(String s, size_t h);
    void                        dump_and_list(WordList *WL, int i);
    void                        dump_words(const std::string &name);
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

void Parser::create_label(const std::string &X, const Istring &S) {
    auto       m = Istring(X);
    LabelInfo *V = m.labinfo();
    if (V->set_defined()) {
        multiple_label(m.name, V->lineno, V->filename);
    } else {
        my_stats.one_more_label();
        V->id       = S;
        V->lineno   = get_cur_line();
        V->filename = get_cur_filename();
        defined_labels.emplace_back(S, V);
    }
}

// Implementation of \ref{foo}. We enter foo in the hashtab.
// and create/update the LabelInfo. We remember the ref in the ref_list.
void Xid::add_ref(const std::string &s) const { tralics_ns::add_ref(value, s, false); }

void tralics_ns::add_ref(long v, const std::string &s, bool idx) {
    the_parser.my_stats.one_more_ref();
    auto B = Istring(s);
    if (idx)
        refindex_list.emplace_back(v, B);
    else
        ref_list.emplace_back(v, B);
    LabelInfo *V = B.labinfo();
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
        int        E = i.first;
        Istring    V = i.second;
        LabelInfo *L = V.labinfo();
        if (!L->defined) {
            Logger::finish_seq();
            log_and_tty << "Error signaled in postprocessor\n"
                        << "undefined label `" << V << "' (first use at line " << L->lineno << " in file " << L->filename << ")";
            Xid(E).add_attribute(the_names["target"], V);
            Istring B = L->id;
            for (auto &removed_label : removed_labels) {
                if (removed_label.second == B) log_and_tty << "\n(Label was removed with `" << removed_label.first << "')";
                break;
            }
            log_and_tty << "\n";
            main_ns::nb_errs++;
        }
        Istring B = L->id;
        Xid(E).add_attribute(the_names["target"], B);
    }
}

//
void tralics_ns::find_index_labels(std::vector<std::string> &W) {
    for (auto &i : refindex_list) {
        auto       E = to_unsigned(i.first);
        Istring    V = i.second;
        LabelInfo *L = V.labinfo();
        if (!L->defined) continue; // should not happen
        Istring B = L->id;
        scbuf.clear();
        scbuf.push_back(W[E]);
        if (!scbuf.empty()) scbuf.push_back(' ');
        scbuf.push_back(B);
        W[E] = scbuf;
    }
}

// Puts element T with its attribute list in the buffer.
// If w=0, we print <foo att-list/>
// If w=1, we print <foo att-list>
// if w=2, we print </foo>
void Buffer::push_back_elt(const Istring &name, Xid id, int w) {
    push_back('<');
    if (w == 2) push_back('/');
    push_back(name.value);
    if (w != 2) push_back(id.get_att());
    if (w == 0) push_back('/');
    push_back('>');
}

void Buffer::finish_xml_print() {
    *cur_fp << data();
    clear();
}

// Adds all non-empty elements to res
void Xml::add_non_empty_to(Xml *res) {
    for (size_t k = 0; k < size(); k++) {
        Xml *T = at(k);
        if (T->is_xmlc() && only_space(T->name.name)) continue;
        res->push_back_unless_nullptr(T);
    }
}

// Postprocessor for <composition>
void Xml::compo_special() {
    XmlAction X(the_names["module"], rc_composition);
    recurse(X);
}

// This is used by sT_translate. It converts an XML element
// to a string, using scbuf as temporary. clears the object
auto Xml::convert_to_string() -> std::string {
    scbuf.clear();
    convert_to_string(scbuf);
    clear();
    return scbuf;
}

// This converts the content to a string. May be recursive
void Xml::convert_to_string(Buffer &b) {
    if (is_xmlc()) {
        b << name.name;
        return;
    }
    if (name.empty() || name == the_names["temporary"]) {
        auto len = size();
        for (size_t k = 0; k < len; k++) at(k)->convert_to_string(b);
        return;
    }
    err_buf.clear();
    if (id.is_font_change()) {
        Istring w = id.has_attribute(the_names["rend"]);
        if (!w.null()) {
            err_buf << "unexpected font change " << w;
            the_parser.unexpected_font();
            the_parser.signal_error();
            return;
        }
    }
    err_buf << "unexpected element " << name;
    the_parser.signal_error();
}

// Puts *this in the buffer B. Uses Internal Encoding
// Used to print the title of a section.
void Xml::put_in_buffer(Buffer &b) {
    for (size_t k = 0; k < size(); k++) {
        if (at(k)->is_xmlc())
            b << at(k)->name.name;
        else if (at(k)->has_name_of("hi"))
            at(k)->put_in_buffer(b);
        else
            b << '<' << at(k)->name << "/>";
    }
}

// Removes and returns the last element
auto Xml::remove_last() -> Xml * {
    if (empty()) return nullptr;
    Xml *res = back();
    pop_back();
    return res;
}

// True if this is empty, or contains only a <hi> element which is empty
auto Xml::par_is_empty() -> bool {
    if (empty()) return true;
    if (size() > 1) return false;
    if (at(0)->is_xmlc()) return false;
    if (at(0)->is_xmlc() || !at(0)->id.is_font_change()) return false;
    return at(0)->par_is_empty();
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

// Prints all words with frequency i. Removes them from the list
void all_words_ns::dump_and_list(WordList *WL, int i) {
    WordList *L       = WL->get_next();
    WordList *first   = WL;
    int       printed = 0;
    while (L != nullptr) {
        WordList *N = L->get_next();
        if (L->dump(fp, i)) {
            printed++;
            delete L;
        } else {
            first->set_next(L);
            first = L;
        }
        L = N;
    }
    first->set_next(nullptr);
    if (printed != 0) { scbuf << fmt::format("{}={}, ", i, printed); }
}

// Finish dumping the words
void all_words_ns::dump_words(const std::string &name) {
    auto *    WL = new WordList("", 0, nullptr);
    WordList *W  = WL;
    for (auto *L : WL0) {
        if (L == nullptr) continue;
        while (W->get_next() != nullptr) W = W->get_next();
        W->set_next(L);
    }
    if (WL->get_next() == nullptr) return;
    auto wf = tralics_ns::get_out_dir("words");

    auto f = std::ofstream(wf);
    if (!name.empty()) f << "Team " << name << "\n";
    scbuf.clear();
    int i = 0;
    while (WL->get_next() != nullptr) {
        i++;
        dump_and_list(WL, i);
    }
    f << "Total " << nb_words << "  ";
    scbuf.remove_last(); // space
    scbuf.remove_last(); // comma
    scbuf.push_back(".\n");
    f << scbuf;
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
    all_words_ns::nb_words++;
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

const std::array<String, 6> entities = {"&nbsp;", "&ndash;", "&mdash;", "&ieme;", "&gt;", "&lt;"};

// This is static. If s is &foo;bar, returns the length
// of the &foo; part. Returns 0 if this is not an entity.
auto post_ns::is_entity(const std::string &s) -> size_t {
    for (auto w : entities) {
        if (s.starts_with(w)) return strlen(w);
    }
    return 0;
}

// The scanner for all_the_words
void Xml::word_stats_i() {
    if (is_xmlc()) {
        auto str = name.name;
        for (size_t i = 0;; i++) {
            char c = str[i];
            if (c == 0) return;
            if (c == '&') {
                if (str.substr(i).starts_with("&oelig;")) {
                    i += 6;
                    scbuf << "oe";
                    continue;
                }
                if (str.substr(i).starts_with("&amp;")) {
                    i += 4;
                    scbuf << "&";
                    continue;
                }
                auto w = post_ns::is_entity(str.substr(i));
                if (w != 0) {
                    i += w - 1;
                    scbuf.new_word();
                    continue;
                }
            }
            if (c == ' ' || c == '`' || c == '\n' || c == ',' || c == '.' || c == '(' || c == ')' || c == ':' || c == ';' || c == '\253' ||
                c == '\273' || c == '\'' || c == '\"')
                scbuf.new_word();
            else
                scbuf << c;
        }
    } else {
        if (name == the_names["formula"]) return;
        for (size_t i = 0; i < size(); i++) at(i)->word_stats_i();
    }
}

void Xml::word_stats(const std::string &match) {
    scbuf.clear();
    word_stats_i();
    scbuf.new_word();
    all_words_ns::dump_words(match);
}
