#include "tralics/Bibtex.h"

// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002,2004, 2007-2011
// This file contains the bibliographic part of Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/BibEntry.h"
#include "tralics/Bibliography.h"
#include "tralics/CitationKey.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txbib.h"
#include <algorithm>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {
    class Error {};

    Buffer CB;

    // This creates a <ref target='bidN'/> element. This is the REF that needs
    // to be solved later. In the case of \footcite[p.25]{Knuth},
    // the arguments of the function are foot and Knuth; the `p.25' will be
    // considered elsewhere.
    auto make_cit_ref(const Istring &type, const Istring &ref) -> Xml {
        auto    n  = *the_bibliography.find_citation_item(type, ref, true);
        Istring id = the_bibliography.citation_table[n].get_id();
        Xml     res(the_names["ref"], nullptr);
        res.id.add_attribute(the_names["target"], id);
        return res;
    }
} // namespace

namespace bib_ns {
    auto normalise_for_bib(Istring w) -> Istring;
    void bib_explain();
    auto is_noopsort(const std::string &s, size_t i) -> bool;
} // namespace bib_ns
using namespace bib_ns;

namespace io_ns {
    auto how_many_bytes(char) -> size_t;
} // namespace io_ns

// Main idea. The TeX file has commands like \cite , \nocite, which produce
// a CitationItem (new or old). There are stored in citation_table. They have
// a unique id, a Bid, and \cite creates a REF element with a reference to this
// the element having this bid as id.
// Such an element can be produced in the TeX source; we call it a solver
// of this bid.
// At the end we must construct a solver for each unsolved one. We use
// a function like dump_bibtex, this creates all_entries, a table of
// bibentry objects, one for each unsolved entry.
// we read some bibliography data bases, this fills the bibentry objects
// (maybe others are added). They are sorted, converted to TeX code
// and the result is translated.

// \cite[year][]{foo}is the same as \cite{foo}
// if distinguish_refer is false,  \cite[refer][]{foo} is also the same.
auto bib_ns::normalise_for_bib(Istring w) -> Istring {
    auto S = w.name;
    if (S == "year") return the_names["cst_empty"];
    if (!distinguish_refer)
        if (S == "refer") return the_names["cst_empty"];
    return w;
}

// Translation of \cite, \nocite, \footcite, \refercite, \yearcite
// Syntax: \cite{Knuth} or \cite[p.25]{Knuth} or \cite[bar]{p.25]{Knuth}
// \nocite{Knuth} or \nocite[foot]{Knuth}
// \footcite[Knuth} or \footcite[p.25]{Knuth}
// \refercite[Knuth} or \refercite[p.25]{Knuth}
// Case of \natcite[otherargs][see][p25]{Knuth}
// Translates as <cit type='otherargs'>see <ref target='xx'/> p25</cit>

// case of \omitcite{foo}. reads an argument, puts it in the omit mist
// and logs it
void Parser::T_omitcite() {
    flush_buffer();
    std::string s = sT_arg_nopar();
    omitcite_list.push_back(s);
    Logger::finish_seq();
    the_log << "{\\omitcite(" << int(omitcite_list.size());
    the_log << ") = " << s << "}\n";
}

// We start with a function that fetches optional arguments
// In prenote we put `p. 25' or nothing, in type one of year, foot, refer, bar
// as an istring, it will be normalised later.
void Parser::T_cite(subtypes sw, TokenList &prenote, Istring &type) {
    if (sw == footcite_code) {
        read_optarg_nopar(prenote);
        type = the_names["foot"];
    } else if (sw == yearcite_code) {
        read_optarg_nopar(prenote);
        type = the_names["cst_empty"]; // should be year here
    } else if (sw == refercite_code) {
        read_optarg_nopar(prenote);
        type = the_names["refer"];
    } else if (sw == nocite_code) {
        type = Istring(fetch_name_opt());
    } else if (sw == natcite_code) {
        read_optarg_nopar(prenote); // is really the post note
    } else {
        // we can have two optional arguments, prenote is last
        TokenList L1;
        if (read_optarg_nopar(L1)) {
            if (read_optarg_nopar(prenote))
                type = Istring(fetch_name1(L1));
            else
                prenote = L1;
        }
    }
}

// You can say \cite{foo,bar}. This is the same as
// \cite@one{}{foo}{}\cite@one{}{foo}{}, empty lists are replaced by the
// optional arguments.  The `prenote' applies only to the first entry,
// and the `type' to all entries. If you do not like like, do not use commas.
// The natbib package proposes a postnote, that applies only to the last.
// We construct here a token list, and push it back.

// The command is fully handled in the \nocite case. The `prenote' is ignored.
// For the special entry `*', the type is ignored too.
void Parser::T_cite(subtypes sw) {
    Token T         = cur_tok;
    bool  is_natbib = sw == natcite_code;
    if (sw == natcite_e_code) {
        flush_buffer();
        the_stack.pop(the_names["natcit"]);
        return;
    }
    if (is_natbib) {
        leave_v_mode();
        the_stack.push1(the_names["natcit"]);
    }
    TokenList res;
    TokenList prenote;
    auto      type = Istring("");
    if (is_natbib) {
        Istring x = nT_optarg_nopar();
        if (!x.empty()) the_stack.add_att_to_last(the_names["citetype"], x);
        read_optarg(res);
        if (!res.empty()) res.push_back(hash_table.space_token);
        res.push_front(hash_table.locate("NAT@open"));
    }
    cur_tok = T;
    T_cite(sw, prenote, type); // reads optional arguments
    type = normalise_for_bib(type);
    if (sw == footcite_code) res.push_back(hash_table.footcite_pre_token);
    Token sep = sw == footcite_code ? hash_table.footcite_sep_token
                                    : sw == natcite_code ? hash_table.locate("NAT@sep") : hash_table.cite_punct_token;
    cur_tok      = T;
    auto  List   = fetch_name0_nopar();
    int   n      = 0;
    Token my_cmd = is_natbib ? hash_table.citesimple_token : hash_table.citeone_token;
    for (const auto &cur : split_commas(List)) {
        if (cur.empty()) continue;
        if (sw == nocite_code) {
            if (cur == "*")
                the_bibliography.set_nocite();
            else
                the_bibliography.find_citation_item(type, Istring(cur), true);
        } else {
            if (n > 0) res.push_back(sep);
            TokenList tmp;
            res.push_back(my_cmd);
            if (!is_natbib) {
                tmp = token_ns::string_to_list(type.name, true);
                res.splice(res.end(), tmp);
            }
            tmp = token_ns::string_to_list(cur, true);
            res.splice(res.end(), tmp);
            if (!is_natbib) {
                res.push_back(hash_table.OB_token);
                if (n == 0) res.splice(res.end(), prenote);
                res.push_back(hash_table.CB_token);
            }
            n++;
        }
    }
    if (is_natbib) {
        if (!prenote.empty()) res.push_back(hash_table.locate("NAT@cmt"));
        res.splice(res.end(), prenote);
        res.push_back(hash_table.locate("NAT@close"));
        res.push_back(hash_table.end_natcite_token);
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "->" << res << ".\n";
    }
    back_input(res);
}

// \cite@one{foot}{Knuth}{p25}.
// Translates to <cit><ref target="xx">p25</ref></cit>
// and \cite@simple{Knuth} gives <ref target="xx"/>
void Parser::T_cite_one() {
    bool  is_simple = cur_cmd_chr.chr != 0;
    Token T         = cur_tok;
    flush_buffer();
    Istring type = is_simple ? Istring("") : Istring(fetch_name0_nopar());
    cur_tok      = T;
    auto ref     = Istring(fetch_name0_nopar());
    Xml *arg     = is_simple ? nullptr : xT_arg_nopar();
    // signal error after argument parsing
    if (bbl.is_too_late()) {
        parse_error("Citation after loading biblio?");
        return;
    }
    auto *res = new Xml(make_cit_ref(type, ref));
    if (is_simple) {
        flush_buffer();
        the_stack.add_last(res);
        return;
    }
    leave_v_mode();
    TokenList L     = get_mac_value(hash_table.cite_type_token);
    auto      xtype = Istring(fetch_name1(L));
    L               = get_mac_value(hash_table.cite_prenote_token);
    auto prenote    = Istring(fetch_name1(L));
    if (arg != nullptr) res->add_tmp(gsl::not_null{arg});
    the_stack.add_last(new Xml(the_names["cit"], res));
    if (!type.empty()) the_stack.add_att_to_last(the_names["rend"], type);
    if (!xtype.empty()) the_stack.add_att_to_last(the_names["citetype"], xtype);
    if (!prenote.empty()) the_stack.add_att_to_last(the_names["prenote"], prenote);
}

// This adds a marker, a location where to insert the bibliography
// The marker can be forcing or not. If it is not forcing, and a forcing
// marker has already been inserted, nothing happens.
void Parser::add_bib_marker(bool force) {
    Bibliography &T = the_bibliography;
    if (!force && T.location_exists()) return;
    Xml *mark = new Xml(Istring(""), nullptr);
    Xml *Foo  = new Xml(Istring(""), mark);
    the_stack.add_last(Foo);
    T.set_location(mark, force);
}

// Translation of \bibliographystyle{foo}
// We remember the style; we could also have a command, bibtex:something
// or program:something
void Parser::T_bibliostyle() {
    auto          Val = fetch_name0_nopar();
    Bibliography &T   = the_bibliography;
    if (Val.starts_with("bibtex:")) {
        if (Val[7] != 0) T.set_style(Val.substr(7));
        T.set_cmd("bibtex " + get_job_name());
    } else if (Val.starts_with("program:"))
        T.set_cmd(Val.substr(8) + " " + get_job_name() + ".aux");
    else
        T.set_style(Val);
}

// Translation of \bibliography{filename}. We add a marker.
// We have a list of file names, they are stored in the
// bibliography data structure
void Parser::T_biblio() {
    flush_buffer();
    auto list = fetch_name0_nopar();
    add_bib_marker(false);
    for (const auto &w : split_commas(list)) {
        if (w.empty()) continue;
        the_bibliography.push_back_src(w);
    }
}

// This creates the bbl file by running an external program.
void Parser::create_aux_file_and_run_pgm() {
    if (!the_main->shell_escape_allowed) {
        spdlog::warn("Cannot call external program unless using option -shell-escape");
        return;
    }
    Buffer &B = biblio_buf4;
    B.clear();
    bbl.reset_lines();
    Bibliography &T = the_bibliography;
    T.dump(B);
    if (B.empty()) return;
    T.dump_data(B);
    std::string auxname = tralics_ns::get_short_jobname() + ".aux";
    try {
        std::ofstream(auxname) << B;
    } catch (...) {
        spdlog::warn("Cannot open file {} for output, bibliography will be missing", auxname);
        return;
    }
    the_log << "++ executing " << T.cmd << ".\n";
    system(T.cmd.c_str());
    B << bf_reset << tralics_ns::get_short_jobname() << ".bbl";
    // NOTE: can we use on-the-fly encoding ?
    the_log << "++ reading " << B << ".\n";
    tralics_ns::read_a_file(bbl.lines, B, 1);
}

void Parser::after_main_text() {
    the_bibliography.stats();
    if (the_bibliography.has_cmd())
        create_aux_file_and_run_pgm();
    else {
        the_bibliography.dump_bibtex();
        the_bibtex->work();
    }
    init(bbl.lines);
    if (!lines.empty()) {
        Xml *res = the_stack.temporary();
        the_stack.push1(the_names["biblio"]);
        AttList &L = the_stack.get_att_list(3);
        the_stack.cur_xid().add_attribute(L, true);
        translate0();
        the_stack.pop(the_names["biblio"]);
        the_stack.pop(the_names["argument"]);
        the_stack.document_element()->insert_bib(res, the_bibliography.location);
    }
    finish_color();
    finish_index();
    check_all_ids();
}

// Translation of \end{thebibliography}
void Parser::T_end_the_biblio() {
    leave_h_mode();
    the_stack.pop(the_names["thebibliography"]);
}

// Translation of \begin{thebibliography}
// The bibliography contains \bibitem commands.
// Thus we can safely remove two optional arguments
// before and after the mandatory one.
void Parser::T_start_the_biblio() {
    ignore_optarg();
    ignore_arg(); // longest label, ignored
    ignore_optarg();
    TokenList L1;
    L1.push_back(hash_table.refname_token);
    auto a = fetch_name1(L1);
    the_stack.set_arg_mode();
    auto name = Istring(a);
    the_stack.set_v_mode();
    the_stack.push(the_names["thebibliography"], new Xml(name, nullptr));
}

// \cititem{foo}{bar} translates \cititem-foo{bar}
// If the command with the funny name is undefined then translation is
// <foo>bar</foo>
// Command has to be in Bib mode, argument translated in Arg mode.
void Parser::T_cititem() {
    auto    a = fetch_name0_nopar();
    Buffer &B = biblio_buf4;
    B << bf_reset << "cititem-" << a;
    finish_csname(B);
    see_cs_token();
    if (cur_cmd_chr.cmd != relax_cmd) {
        back_input();
        return;
    }
    mode m = the_stack.get_mode();
    need_bib_mode();
    the_stack.set_arg_mode();
    auto name = Istring(a);
    the_stack.push(name, new Xml(name, nullptr));
    T_arg();
    the_stack.pop(name);
    the_stack.set_mode(m);
    the_stack.add_nl();
}

// -------------------------------------------------------- //
// This flushes the buffer.
void Bbl::newline() {
    B.push_back("\n");
    file << B.convert_to_log_encoding();
    lines.insert(B, true);
    B.clear();
}

// \bpers[opt-full]{first-name}{von-part}{last-name}{jr-name}
// note that Tralics generates an empty von-part
void Parser::T_bpers() {
    int e              = main_ns::nb_errs;
    unexpected_seen_hi = false;
    Istring A          = nT_optarg_nopar();
    Istring a          = nT_arg_nopar();
    Istring b          = nT_arg_nopar();
    Istring c          = nT_arg_nopar();
    Istring d          = nT_arg_nopar();
    if (unexpected_seen_hi && e != main_ns::nb_errs) log_and_tty << "maybe you confused Publisher with Editor\n";
    need_bib_mode();
    the_stack.add_newid0("bpers");
    if (!(A.null() || A.empty())) the_stack.add_att_to_last(the_names["full_first"], A);
    if (!d.empty()) the_stack.add_att_to_last(the_names["junior"], d);
    the_stack.add_att_to_last(the_names["nom"], c);
    if (!b.empty()) the_stack.add_att_to_last(the_names["particule"], b);
    the_stack.add_att_to_last(the_names["prenom"], a);
}

void Stack::implement_cit(const std::string &b1, const Istring &b2, const std::string &a, const std::string &c) {
    add_att_to_last(the_names["userid"], Istring(b1));
    add_att_to_last(the_names["id"], b2);
    add_att_to_last(the_names["key"], Istring(a));
    add_att_to_last(the_names["from"], Istring(c));
}

// case \bibitem
void Parser::T_bibitem() {
    flush_buffer();
    leave_h_mode();
    leave_v_mode();
    solve_cite(true);
    skip_initial_space_and_back_input();
}

// Flag true for bibitem, \bibitem[opt]{key}
// false in the case of \XMLsolvecite[id][from]{key}
void Parser::solve_cite(bool user) {
    Token T    = cur_tok;
    bool  F    = true;
    auto  from = Istring("");
    long  n    = 0;
    if (user) {
        implicit_par(zero_code);
        the_stack.add_last(new Xml(the_names["bibitem"], nullptr));
        Istring ukey = nT_optarg_nopar();
        the_stack.get_xid().get_att().push_back(the_names["bibkey"], ukey);
        n = the_stack.get_xid().value;
    } else {
        F    = remove_initial_star();
        n    = to_signed(read_elt_id(T));
        from = Istring(fetch_name_opt());
    }
    from     = normalise_for_bib(from);
    cur_tok  = T;
    auto key = Istring(fetch_name0_nopar());
    if (user) insert_every_bib();
    if (n == 0) return;
    Xid           N  = Xid(n);
    Bibliography &B  = the_bibliography;
    size_t        nn = 0;
    if (F)
        nn = B.find_citation_star(from, key);
    else
        nn = *B.find_citation_item(from, key, true);
    CitationItem &CI = B.citation_table[nn];
    if (CI.is_solved()) {
        err_buf << bf_reset << "Bibliography entry already defined " << key.name;
        the_parser.signal_error(the_parser.err_tok, "bad solve");
        return;
    }
    AttList &AL    = N.get_att();
    auto     my_id = AL.lookup(the_names["id"]);
    if (my_id) {
        if (CI.id.empty())
            CI.id = AL.get_val(*my_id);
        else {
            err_buf << bf_reset << "Cannot solve (element has an Id) " << key.name;
            the_parser.signal_error(the_parser.err_tok, "bad solve");
            return;
        }
    } else
        AL.push_back(the_names["id"], CI.get_id());
    CI.solved = N;
}

// this is the same as \bibitem[]{foo}{}
void Parser::T_empty_bibitem() {
    flush_buffer();
    std::string w;
    std::string a;
    std::string b  = sT_arg_nopar();
    Istring     id = the_bibtex->exec_bibitem(w, b);
    if (id.empty()) return;
    leave_v_mode();
    the_stack.push1(the_names["citation"]);
    the_stack.implement_cit(b, id, a, w);
    the_stack.pop(the_names["citation"]);
}

// Translation of \citation{key}{userid}{id}{from}{type}[alpha-key]
void Parser::T_citation() {
    flush_buffer();
    std::string a  = sT_arg_nopar();
    std::string b1 = special_next_arg();
    std::string b2 = sT_arg_nopar();
    std::string c  = sT_arg_nopar();
    Istring     d  = nT_arg_nopar();
    the_stack.add_nl();
    the_stack.push1(the_names["citation"]);
    the_stack.add_att_to_last(the_names["type"], d);
    the_stack.implement_cit(b1, Istring(b2), a, c);
    the_stack.set_bib_mode();
    ignore_optarg();
    insert_every_bib();
}

void Parser::insert_every_bib() {
    TokenList everybib = toks_registers[everybibitem_code].val;
    if (everybib.empty()) return;
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{<everybibitem> " << everybib << "}\n";
    }
    back_input(everybib);
}

// ----------------------------------------------------------------------
//
// the bibtex parser: read the data base files
//
//

// Returns the buffer without initial and final space, if init is true.
// In any case, a tab is converted into a space, multiple space chars
// are replaced by single ones.
// We can safely assume that buffer is ASCII
auto Buffer::special_convert(bool init) -> std::string {
    ptrs.b = 0;
    if (init) skip_sp_tab_nl();
    biblio_buf1.clear();
    bool space = true;
    for (;;) {
        auto c = next_char();
        if (c == 0) break;
        if (is_space(c)) {
            if (!space) {
                biblio_buf1.push_back(' ');
                space = true;
            }
        } else {
            biblio_buf1.push_back(c);
            space = false;
        }
    }
    if (init && !biblio_buf1.empty() && biblio_buf1.back() == ' ') biblio_buf1.remove_last();
    return biblio_buf1;
}

// ------------------------------------------------------------
//
// Working with the entries

// In the bibliobraphy \url="foo bar"
// gives \href{foobar}{\url{foo\allowbreak bar}}
// We handle here the first string
auto Buffer::remove_space(const std::string &x) -> std::string {
    auto n = x.size();
    clear();
    for (size_t i = 0; i < n; i++)
        if (x[i] != ' ') push_back(x[i]);
    return *this;
}

// We create here the second string
auto Buffer::insert_break(const std::string &x) -> std::string {
    auto n = x.size();
    clear();
    push_back("{\\url{");
    for (size_t i = 0; i < n; i++) {
        if (x[i] == ' ' && main_ns::bib_allow_break) push_back("\\allowbreak");
        push_back(x[i]);
    }
    push_back("}}");
    return *this;
}

// In case of Lo{\"i}c, repeated calls will set head() to L, o, { and c.
// It works also in the case of non-ascii characters
void Buffer::next_bibtex_char() {
    auto c = head();
    if (c == 0) return;
    if (c == '\\') {
        ptrs.b++;
        c = head();
        if (c == 0) return;
        ptrs.b += io_ns::how_many_bytes(c);
        if (ptrs.b > size()) ptrs.b = size();
        return;
    }
    auto n = io_ns::how_many_bytes(c);
    if (n > 1) {
        ptrs.b += n;
        if (ptrs.b > size()) ptrs.b = size();
        return;
    }
    if (c != '{')
        ptrs.b++;
    else
        skip_over_brace();
}

// We assume that current char is an open brace.
// We can forget about utf8 here.
// Just want {\{\}\}\\} to be interpret correctly
void Buffer::skip_over_brace() {
    int bl = 0;
    for (;;) {
        auto c = head();
        if (c == 0) return;
        ptrs.b++;
        if (c == '\\') {
            if (head() == 0) return;
            ptrs.b++;
            continue;
        }
        if (c == '{')
            bl++;
        else if (c == '}') {
            bl--;
            if (bl == 0) return;
        }
    }
}

// True if \sortnoop, \SortNoop, \noopsort plus brace or space
// First char to test is at i+1
auto bib_ns::is_noopsort(const std::string &s, size_t i) -> bool {
    auto n = s.size();
    if (i + 10 >= n) return false;
    if (s[i + 10] != '{' && !is_space(s[i + 10])) return false;
    if (s[i + 1] != '\\') return false;
    if (s[i + 3] != 'o') return false;
    if (s[i + 7] != 'o') return false;
    if (s[i + 2] == 'n' && s[i + 4] == 'o' && s[i + 5] == 'p' && s[i + 6] == 's' && s[i + 8] == 'r' && s[i + 9] == 't') return true;
    if ((s[i + 2] == 's' || s[i + 2] == 'S') && s[i + 4] == 'r' && s[i + 5] == 't' && (s[i + 6] == 'n' || s[i + 6] == 'N') &&
        s[i + 8] == 'o' && s[i + 9] == 'p')
        return true;
    return false;
}

// removes braces in the case title="study of {$H^p}, part {I}"
// Brace followed by Uppercase or dollar
// Also, handles {\noopsort foo} removes the braces and the command.
void Buffer::special_title(std::string s) {
    int  level  = 0;
    int  blevel = 0;
    auto n      = s.size();
    for (size_t i = 0; i < n; i++) {
        char c = s[i];
        if (c == '\\') {
            push_back(c);
            i++;
            if (i < n) push_back(s[i]);
            continue;
        }
        if (c == '}') {
            if (level != blevel)
                push_back(c);
            else
                blevel = 0;
            if (level > 0) level--;
            continue;
        }
        if (c == '{') level++;
        if (c != '{' || i == n - 1 || (blevel != 0) || level != 1) {
            push_back(c);
            continue;
        }
        // c= is a brace at bottom level
        char cc = s[i + 1];
        if (is_upper_case(cc) || cc == '$') {
            blevel = level;
            continue;
        }
        if (is_noopsort(s, i)) {
            i += 10;
            while (i < n && is_space(s[i])) i++;
            i--;
            blevel = level;
        } else
            push_back(c);
    }
}

// This replaces \c{c} by \char'347 in order to avoid some errors.
void Buffer::normalise_for_bibtex(String s) {
    clear();
    push_back(' '); // make sure we can always backup one char
    for (;;) {
        auto c = *s;
        if (c == 0) {
            ptrs.b = 0;
            return;
        }
        push_back(c);
        s++;
        if (c != '\\') continue;
        if (std::string(s).starts_with("c{c}")) {
            remove_last();
            push_back(codepoint(0347U));
            s += 4;
        } else if (std::string(s).starts_with("c{C}")) {
            remove_last();
            push_back(codepoint(0307U));
            s += 4;
        } else if (std::string(s).starts_with("v{c}")) {
            remove_last();
            push_back("{\\v c}");
            s += 4;
            continue;
        } else if (*s == 'a' && is_accent_char(s[1])) {
            s++;
        } else if (*s == ' ') {
            remove_last();
        } // replace \space by space
    }
}

// For each character, we have its type in the table.
void Buffer::fill_table(bchar_type *table) {
    ptrs.b = 0;
    for (;;) {
        auto i = ptrs.b;
        auto c = head();
        if (c == 0U) {
            table[i] = bct_end;
            return;
        }
        ptrs.b++;
        if (static_cast<uchar>(c) >= 128 + 64) {
            table[i] = bct_extended;
            continue;
        }
        if (static_cast<uchar>(c) >= 128) {
            table[i] = bct_continuation;
            continue;
        }
        if (c == ' ') {
            table[i] = bct_space;
            continue;
        }
        if (c == '~') {
            table[i] = bct_tilde;
            continue;
        }
        if (c == '-') {
            table[i] = bct_dash;
            continue;
        }
        if (c == ',') {
            table[i] = bct_comma;
            continue;
        }
        if (c == '&') {
            the_bibtex->err_in_name("unexpected character `&' (did you mean `and' ?)", to_signed(i));
            table[i] = bct_bad;
            continue;
        }
        if (c == '}') {
            the_bibtex->err_in_name("too many closing braces", to_signed(i));
            table[i] = bct_bad;
            continue;
        }
        if (c != '\\' && c != '{') {
            table[i] = bct_normal;
            continue;
        }
        if (c == '\\') {
            c = head();
            if (!is_accent_char(c)) {
                table[i] = bct_bad;
                the_bibtex->err_in_name("commands allowed only within braces", to_signed(i));
                continue;
            }
            if (is_letter(at(ptrs.b + 1))) {
                table[i]     = bct_cmd;
                table[i + 1] = bct_continuation;
                table[i + 2] = bct_continuation;
                ptrs.b += 2;
                continue;
            }
            if (at(ptrs.b + 1) != '{') {
                table[i]     = bct_bad;
                table[i + 1] = bct_bad;
                ptrs.b++;
                the_bibtex->err_in_name("bad accent construct", to_signed(i));
                continue;
            }
            at(ptrs.b + 1) = c;
            at(ptrs.b)     = '\\';
            at(ptrs.b - 1) = '{';
            Logger::finish_seq();
            the_log << "+bibchanged " << data() << "\n";
        }
        int  bl  = 1;
        auto j   = i;
        table[i] = bct_brace;
        for (;;) {
            if (head() == 0) {
                the_bibtex->err_in_name("this cannot happen!", to_signed(j));
                resize(j);
                table[j] = bct_end;
                return;
            }
            c             = head();
            table[ptrs.b] = bct_continuation;
            ptrs.b++;
            if (c == '{') bl++;
            if (c == '}') {
                bl--;
                if (bl == 0) break;
            }
        }
    }
}

auto Buffer::find_and(const bchar_type *table) -> bool {
    for (;;) {
        char c = head();
        if (c == 0) return true;
        ptrs.b++;
        if (table[ptrs.b - 1] == bct_space && is_and(ptrs.b)) return false;
    }
}

// True if this is an `and'
auto Buffer::is_and(size_t k) const -> bool {
    char c = at(k);
    if (c != 'a' && c != 'A') return false;
    c = at(k + 1);
    if (c != 'n' && c != 'N') return false;
    c = at(k + 2);
    if (c != 'd' && c != 'D') return false;
    c = at(k + 3);
    return (c == ' ') || (c == '\t') || (c == '\n');
}

// In J.G. Grimm,only the first dot matches.
auto Buffer::insert_space_here(size_t k) const -> bool {
    if (k == 0) return false;
    if (at(k) != '.') return false;
    if (!is_upper_case(at(k + 1))) return false;
    if (!is_upper_case(at(k - 1))) return false;
    return true;
}

void Buffer::remove_spec_chars(bool url, Buffer &B) {
    ptrs.b = 0;
    B.clear();
    for (;;) {
        auto c = head();
        if (c == 0U) return;
        advance();
        if (c == '|') {
            B.push_back("\\vbar ");
            continue;
        } // bar is special
        if (c == '{' || c == '}') continue;
        if (c == '~') {
            B.push_back(url ? '~' : ' ');
            continue;
        }
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            B.push_back(' ');
            continue;
        }
        if (c == '-' && head() == '-') continue; // -- gives -
        if (c != '\\') {
            B.push_back(c);
            continue;
        }
        c = head();
        advance();
        if (c == '{' || c == '}') {
            B.push_back(c);
            continue;
        }
        if (c == '|') {
            B.push_back("\\vbar ");
            continue;
        } // bar is special
        if (c == 's' && head() == 'p' && !is_letter(at(ptrs.b + 1))) {
            advance();
            B.push_back('^');
            continue;
        }
        if (c == 's' && head() == 'b' && !is_letter(at(ptrs.b + 1))) {
            advance();
            B.push_back('_');
            continue;
        }
        if (is_accent_char(c)) {
            auto C = head();
            if (C == '{') {
                advance();
                C = head();
            }
            advance();
            if (c == '\'') {
                if (C == 'a') {
                    B.push_back(codepoint('\341'));
                    continue;
                }
                if (C == 'e') {
                    B.push_back(codepoint('\351'));
                    continue;
                }
                if (C == 'i') {
                    B.push_back(codepoint('\355'));
                    continue;
                }
                if (C == 'o') {
                    B.push_back(codepoint('\363'));
                    continue;
                }
                if (C == 'u') {
                    B.push_back(codepoint('\372'));
                    continue;
                }
                if (C == 'y') {
                    B.push_back(codepoint('\375'));
                    continue;
                }
                if (C == 'A') {
                    B.push_back(codepoint('\301'));
                    continue;
                }
                if (C == 'E') {
                    B.push_back(codepoint('\311'));
                    continue;
                }
                if (C == 'I') {
                    B.push_back(codepoint('\315'));
                    continue;
                }
                if (C == 'O') {
                    B.push_back(codepoint('\323'));
                    continue;
                }
                if (C == 'U') {
                    B.push_back(codepoint('\332'));
                    continue;
                }
                if (C == 'Y') {
                    B.push_back(codepoint('\335'));
                    continue;
                }
            }
            if (c == '`') {
                if (C == 'a') {
                    B.push_back(codepoint('\340'));
                    continue;
                }
                if (C == 'e') {
                    B.push_back(codepoint('\350'));
                    continue;
                }
                if (C == 'i') {
                    B.push_back(codepoint('\354'));
                    continue;
                }
                if (C == 'o') {
                    B.push_back(codepoint('\362'));
                    continue;
                }
                if (C == 'u') {
                    B.push_back(codepoint('\371'));
                    continue;
                }
                if (C == 'A') {
                    B.push_back(codepoint('\300'));
                    continue;
                }
                if (C == 'E') {
                    B.push_back(codepoint('\310'));
                    continue;
                }
                if (C == 'I') {
                    B.push_back(codepoint('\314'));
                    continue;
                }
                if (C == 'O') {
                    B.push_back(codepoint('\322'));
                    continue;
                }
                if (C == 'U') {
                    B.push_back(codepoint('\331'));
                    continue;
                }
            }
            if (c == '^') {
                if (C == 'a') {
                    B.push_back(codepoint('\342'));
                    continue;
                }
                if (C == 'e') {
                    B.push_back(codepoint('\352'));
                    continue;
                }
                if (C == 'i') {
                    B.push_back(codepoint('\356'));
                    continue;
                }
                if (C == 'o') {
                    B.push_back(codepoint('\364'));
                    continue;
                }
                if (C == 'u') {
                    B.push_back(codepoint('\373'));
                    continue;
                }
                if (C == 'A') {
                    B.push_back(codepoint('\302'));
                    continue;
                }
                if (C == 'E') {
                    B.push_back(codepoint('\312'));
                    continue;
                }
                if (C == 'I') {
                    B.push_back(codepoint('\316'));
                    continue;
                }
                if (C == 'O') {
                    B.push_back(codepoint('\324'));
                    continue;
                }
                if (C == 'U') {
                    B.push_back(codepoint('\333'));
                    continue;
                }
            }
            if (c == '\"') {
                if (C == 'a') {
                    B.push_back(codepoint('\344'));
                    continue;
                }
                if (C == 'e') {
                    B.push_back(codepoint('\353'));
                    continue;
                }
                if (C == 'i') {
                    B.push_back(codepoint('\357'));
                    continue;
                }
                if (C == 'o') {
                    B.push_back(codepoint('\366'));
                    continue;
                }
                if (C == 'u') {
                    B.push_back(codepoint('\374'));
                    continue;
                }
                if (C == 'y') {
                    B.push_back(codepoint('\377'));
                    continue;
                }
                if (C == 'A') {
                    B.push_back(codepoint('\304'));
                    continue;
                }
                if (C == 'E') {
                    B.push_back(codepoint('\313'));
                    continue;
                }
                if (C == 'I') {
                    B.push_back(codepoint('\317'));
                    continue;
                }
                if (C == 'O') {
                    B.push_back(codepoint('\326'));
                    continue;
                }
                if (C == 'U') {
                    B.push_back(codepoint('\334'));
                    continue;
                }
            }
            if (c == '~') {
                if (C == 'n') {
                    B.push_back(codepoint('\361'));
                    continue;
                }
                if (C == 'N') {
                    B.push_back(codepoint('\321'));
                    continue;
                }
            }
            B.push_back('\\');
            B.push_back(c);
            B.push_back(C);
            continue;
        }
        B.push_back('\\');
        B.push_back(c);
    }
}

// ----------------------------------------
// Boot strapping the system

void tralics_ns::bibtex_boot(std::string b, const std::string &dy, std::string no_year, bool inra, bool db) {
    distinguish_refer = db;
    bbl.name          = std::move(b);
    the_bibtex        = new Bibtex(dy);
    the_bibtex->boot(std::move(no_year), inra);
}

void tralics_ns::bibtex_set_nocite() { the_bibliography.set_nocite(); }

void tralics_ns::bibtex_insert_jobname() {
    if (the_bibliography.number_of_data_bases() == 0) { the_bibliography.push_back_src(tralics_ns::get_short_jobname()); }
}
