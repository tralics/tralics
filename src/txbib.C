// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002,2004, 2007-2011
// This file contains the bibliographic part of Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txbib.h"
#include "txparser.h"
#include <algorithm>

namespace {
    String my_constant_table[3];
    class Error {};
    Bbl                      bbl;
    Buffer                   biblio_buf1, biblio_buf2, biblio_buf3, biblio_buf4, biblio_buf5, aux_name_buffer, name_buffer, field_buf;
    Buffer                   CB;
    Bibtex *                 the_bibtex;
    const int                not_found         = -1;
    bool                     distinguish_refer = false;
    std::string              cur_entry_name; // name of entry under construction.
    int                      cur_entry_line; // position of entry in source file
    std::vector<std::string> omitcite_list;

    BblAndTty    BAT;
    Bibliography the_bibliography;
    int          similar_entries;
    bool         old_ra      = false;
    bool         start_comma = true; // should we scan for an initial comma ?
} // namespace

namespace bib_ns {
    auto normalise_for_bib(Istring w) -> Istring;
    void boot_ra_prefix(String s);
    auto first_three(const std::string &s) -> std::string;
    auto last_chars(const std::string &s, int k) -> std::string;
    auto skip_dp(const std::string &str) -> std::string;
    void bib_explain();
    void handle_special_string(const std::string &s, Buffer &A, Buffer &B);
    auto type_to_string(entry_type x) -> name_positions;
    auto is_noopsort(const std::string &s, int i) -> bool;
    bool raw_bib = false;
} // namespace bib_ns
using namespace bib_ns;

namespace io_ns {
    auto how_many_bytes(uchar) -> int;
} // namespace io_ns

String bib_xml_name[] = {"bcrossref",     "bkey",         "baddress", "bauthors", "bbooktitle",   "bchapter", "bedition",      "beditors",
                         "bhowpublished", "binstitution", "bjournal", "bmonth",   "bnote",        "bnumber",  "borganization", "bpages",
                         "bpublisher",    "bschool",      "bseries",  "btitle",   "btype",        "burl",     "bvolume",       "byear",
                         "bdoi",          "bsubtype",     "bunite",   "bequipe",  "bidentifiant", "bunknown"};

// Ctor of the bibliography stuff.
Bibliography::Bibliography() : bib_style("plain"), bib_cmd("") {}

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

// This creates a unique ID, named bid1, bid2, etc.
auto Bibliography::unique_bid() -> Istring {
    last_bid++;
    Buffer &B = biblio_buf4;
    B << bf_reset << "bid" << last_bid;
    return Istring(B);
}

// This returns a bid. It may create one.
auto CitationItem::get_bid() -> Istring {
    if (bid.empty()) bid = the_bibliography.unique_bid();
    return bid;
}

// This creates a <ref target='bidN'/> element. This is the REF that needs
// to be solved later. In the case of \footcite[p.25]{Knuth},
// the arguments of the function are foot and Knuth; the `p.25' will be
// considered elsewhere.
auto Parser::make_cit_ref(Istring type, Istring ref) -> Xml * {
    int     n   = the_bibliography.find_citation_item(type, ref, true);
    Istring id  = the_bibliography.citation_table[n].get_bid();
    Xml *   res = new Xml(np_ref, nullptr);
    res->get_id().add_attribute(np_target, id);
    return res;
}

// \cite[year][]{foo}is the same as \cite{foo}
// if distinguish_refer is false,  \cite[refer][]{foo} is also the same.
auto bib_ns::normalise_for_bib(Istring w) -> Istring {
    String S = w.c_str();
    if (strcmp(S, "year") == 0) return the_names[cst_empty];
    if (!distinguish_refer)
        if (strcmp(S, "refer") == 0) return the_names[cst_empty];
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
    the_log << lg_startbrace << "\\omitcite(" << int(omitcite_list.size());
    the_log << ") = " << s << lg_endbrace;
}

// We start with a function that fetches optional arguments
// In prenote we put `p. 25' or nothing, in type one of year, foot, refer, bar
// as an istring, it will be normalised later.
void Parser::T_cite(subtypes sw, TokenList &prenote, Istring &type) {
    if (sw == footcite_code) {
        read_optarg_nopar(prenote);
        type = the_names[cst_foot];
    } else if (sw == yearcite_code) {
        read_optarg_nopar(prenote);
        type = the_names[cst_empty]; // should be year here
    } else if (sw == refercite_code) {
        read_optarg_nopar(prenote);
        type = the_names[cst_refer];
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
        the_stack.pop(np_natcit);
        return;
    }
    if (is_natbib) {
        leave_v_mode();
        the_stack.push1(np_natcit);
    }
    TokenList res;
    TokenList prenote;
    auto      type = Istring("");
    if (is_natbib) {
        Istring x = nT_optarg_nopar();
        if (!x.empty()) the_stack.add_att_to_last(the_names[np_cite_type], x);
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
    cur_tok       = T;
    String   List = fetch_name0_nopar();
    Splitter S(List);
    int      n      = 0;
    Token    my_cmd = is_natbib ? hash_table.citesimple_token : hash_table.citeone_token;
    for (;;) {
        if (S.at_end()) break;
        String cur = S.get_next();
        if (cur[0] == 0) continue;
        if (sw == nocite_code) {
            if (strcmp(cur, "*") == 0)
                the_bibliography.set_nocite();
            else
                the_bibliography.find_citation_item(type, Istring(cur), true);
        } else {
            if (n > 0) res.push_back(sep);
            TokenList tmp;
            res.push_back(my_cmd);
            if (!is_natbib) {
                tmp = token_ns::string_to_list(type.c_str(), true);
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
    if (tracing_commands()) the_log << lg_start << T << "->" << res << "." << lg_end;
    back_input(res);
}

// \cite@one{foot}{Knuth}{p25}.
// Translates to <cit><ref target="xx">p25</ref></cit>
// and \cite@simple{Knuth} gives <ref target="xx"/>
void Parser::T_cite_one() {
    bool  is_simple = cur_cmd_chr.get_chr() != 0;
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
    Xml *res = make_cit_ref(type, ref);
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
    res->add_tmp(arg);
    the_stack.add_last(new Xml(np_cit, res));
    if (!type.empty()) the_stack.add_att_to_last(the_names[np_rend], type);
    if (!xtype.empty()) the_stack.add_att_to_last(the_names[np_cite_type], xtype);
    if (!prenote.empty()) the_stack.add_att_to_last(the_names[np_prenote], prenote);
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
    String        Val = fetch_name0_nopar();
    Bibliography &T   = the_bibliography;
    if (strncmp(Val, "bibtex:", 7) == 0) {
        if (Val[7] != 0) T.set_style(std::string(Val + 7));
        T.set_cmd("bibtex " + get_job_name());
    } else if (strncmp(Val, "program:", 8) == 0)
        T.set_cmd(std::string(Val + 8) + " " + get_job_name() + ".aux");
    else
        T.set_style(std::string(Val));
}

// Translation of \bibliography{filename}. We add a marker.
// We have a list of file names, they are stored in the
// bibliography data structure
void Parser::T_biblio() {
    flush_buffer();
    String list = fetch_name0_nopar();
    add_bib_marker(false);
    Splitter S(list);
    while (!S.at_end()) {
        String w = S.get_next();
        if (w[0] == 0) continue;
        the_bibliography.push_back_src(w);
    }
}

// This prints an unsolved reference in a buffer, we will put it in
// the aux file.
void CitationItem::dump(Buffer &b) {
    if (is_solved()) return;
    b << "\\citation{" << key.c_str() << "}\n";
}

// Ctor via "foot" and "Knuth"
CitationKey::CitationKey(String a, String b) {
    if (strcmp(a, "foot") == 0)
        cite_prefix = from_foot;
    else if (strcmp(a, "refer") == 0)
        cite_prefix = from_refer;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Ctor via from_foot and "Knuth".
CitationKey::CitationKey(bib_from a, String b) {
    if (a == from_foot || a == from_refer)
        cite_prefix = a;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Common code of the Ctor. Argument is "Knuth", cite_prefix is OK.
void CitationKey::make_key(String s) {
    if (!distinguish_refer && cite_prefix == from_refer) cite_prefix = from_year;
    Buffer &B = biblio_buf2;
    B.reset();
    B.push_back(s);
    B.lowercase();
    cite_key       = s;
    lower_cite_key = B.to_string();
    B.reset();
    if (cite_prefix == from_foot)
        B.push_back("foot");
    else if (cite_prefix == from_refer && !old_ra)
        B.push_back("refer");
    B.push_back("cite:");
    B.push_back(s);
    full_key = B.to_string();
}

// This prints an unsolved reference for use by Tralics.
// Note that make_citation can produce the same result for two different
// entries, this is bad.
void CitationItem::dump_bibtex() {
    if (is_solved()) return;
    CitationKey ref(from.c_str(), key.c_str());
    BibEntry *  X = the_bibtex->find_entry(ref);
    if (X != nullptr) {
        err_ns::local_buf << bf_reset << "Conflicts with tralics bib" << ref.get_name();
        the_parser.signal_error(the_parser.err_tok, "bib");
        return;
    }
    the_bibtex->make_entry(ref, get_bid());
}

// This creates the full aux file, for use with bibtex.
void Bibliography::dump(Buffer &b) {
    if (seen_nocite()) b << "\\citation{*}\n";
    int n = citation_table.size();
    for (int i = 0; i < n; i++) citation_table[i].dump(b);
}

// This reads conditionally a file. Returns true if the file exists.
auto Bibtex::read0(Buffer &B, bib_from ct) -> bool {
    B.push_back(".bib");
    if (tralics_ns::find_in_path(B.c_str())) {
        read(main_ns::path_buffer.c_str(), ct);
        return true;
    }
    return false;
}

// This takes a file name. It handles the case where the file has a suffix
// like miaou+foot. Prints a warning if this is a bad name.
void Bibtex::read1(const std::string &cur) {
    Buffer &Tbuf = biblio_buf4;
    Tbuf.reset();
    Tbuf.push_back(cur);
    int n = Tbuf.length();
    if (read0(Tbuf, from_year)) return;
    String Str = Tbuf.c_str();
    if (n > 5 && strcmp(Str + n - 5, "+foot.bib") == 0) {
        Tbuf.set_last(n - 5);
        if (read0(Tbuf, from_foot)) return;
    }
    if (n > 5 && strcmp(Str + n - 5, "+year.bib") == 0) {
        Tbuf.set_last(n - 5);
        if (read0(Tbuf, from_year)) return;
    }
    if (n > 4 && strcmp(Str + n - 4, "+all.bib") == 0) {
        Tbuf.set_last(n - 4);
        if (read0(Tbuf, from_any)) return;
    }
    if (n > 6 && strcmp(Str + n - 6, "+refer.bib") == 0) {
        Tbuf.set_last(n - 6);
        if (read0(Tbuf, from_refer)) return;
    }
    if (n > 4 && strcmp(Str + n - 4, ".bib.bib") == 0) {
        Tbuf.set_last(n - 4);
        if (read0(Tbuf, from_year)) return;
    }
    log_and_tty << "Bibtex Info: no biblio file " << Tbuf << "\n";
}

// Handles one bib file for the raweb. Returns true if the file exists.
// Extension can be foot, year or refer. New in Tralics 2.9.3 it can be any
auto Bibtex::read2(bib_from pre) -> bool {
    Buffer &B = biblio_buf4;
    B.reset();
    B.push_back(no_year);
    if (pre == from_foot)
        B.push_back("_foot");
    else if (pre == from_refer)
        B.push_back("_refer");
    else if (pre == from_any)
        B.push_back("_all");
    B.push_back(default_year);
    return read0(B, pre);
}

// This loads all three files, if we are compiling the raweb. Otherwise,
// there is no default.
// We read apics2006_all.bib if this exists
void Bibtex::read_ra() {
    if (in_ra) {
        bbl.open();
        if (read2(from_any)) return;
        read2(from_year);
        read2(from_foot);
        read2(from_refer);
    }
}

// This dumps the whole biblio for use by Tralics.
void Bibliography::dump_bibtex() {
    if (seen_nocite()) the_bibtex->nocitestar_true();
    int n = citation_table.size();
    if (n != 0) bbl.open();
    for (int i = 0; i < n; i++) citation_table[i].dump_bibtex();
    n = biblio_src.size();
    if (n > 0) {
        bbl.open();
        for (int i = 0; i < n; i++) the_bibtex->read1(biblio_src[i]);
    } else
        the_bibtex->read_ra();
}

void Bibliography::stats() {
    int solved = 0, total = 0;
    int n = citation_table.size();
    for (int i = 0; i < n; i++) {
        total++;
        if (citation_table[i].is_solved()) solved++;
    }
    main_ns::log_or_tty << "Bib stats: seen " << total;
    if (solved != 0) main_ns::log_or_tty << "(" << solved << ")";
    main_ns::log_or_tty << " entries.\n";
}

// This dumps the whole biblio for use by bibtex.
void Bibliography::dump_data(Buffer &b) {
    int n = biblio_src.size();
    b << "\\bibstyle{" << bib_style.c_str() << "}\n";
    b << "\\bibdata{";
    if (n == 0) b << tralics_ns::get_short_jobname();
    for (int i = 0; i < n; i++) {
        if (i > 0) b.push_back(",");
        b.push_back(biblio_src[i]);
    }
    b << "}\n";
}

// This creates the bbl file by running an external program.
void Parser::create_aux_file_and_run_pgm() {
    if (!the_main->is_shell_escape_allowed()) {
        log_and_tty << "Cannot call external program unless using option -shell-escape\n";
        return;
    }
    Buffer &B = biblio_buf4;
    B.reset();
    bbl.reset_lines();
    Bibliography &T = the_bibliography;
    T.dump(B);
    if (B.empty()) return;
    T.dump_data(B);
    std::string auxname = tralics_ns::get_short_jobname() + ".aux";
    try {
        std::fstream fp(auxname.c_str(), std::ios::out);
        fp << B.c_str();
        fp.close();
    } catch (...) {
        log_and_tty << "Cannot open file " << auxname << " for output \n"
                    << "Bibliography will be missing\n";
        return;
    }
    the_log << "++ executing " << T.get_cmd().c_str() << ".\n";
    system(T.get_cmd().c_str());
    B << bf_reset << tralics_ns::get_short_jobname() << ".bbl";
    // NOTE: can we use on-the-fly encoding ?
    the_log << "++ reading " << B.c_str() << ".\n";
    tralics_ns::read_a_file(bbl.get_lines(), B.c_str(), 1);
}

void Parser::after_main_text() {
    the_bibliography.stats();
    if (the_bibliography.has_cmd())
        create_aux_file_and_run_pgm();
    else {
        the_bibliography.dump_bibtex();
        the_bibtex->work();
    }
    init(bbl.get_lines());
    if (!lines.is_empty()) {
        Xml *res = the_stack.temporary();
        the_stack.push1(np_biblio);
        AttList &L = the_stack.get_att_list(3);
        the_stack.cur_xid().add_attribute(L, true);
        translate0();
        the_stack.pop(np_biblio);
        the_stack.pop(cst_argument);
        the_stack.document_element()->insert_bib(res, the_bibliography.get_location());
    }
    finish_color();
    finish_index();
    check_all_ids();
}

// Translation of \end{thebibliography}
void Parser::T_end_the_biblio() {
    leave_h_mode();
    the_stack.pop(cst_biblio);
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
    String a = fetch_name1(L1);
    the_stack.set_arg_mode();
    auto name = Istring(a);
    the_stack.set_v_mode();
    the_stack.push(the_names[cst_biblio], new Xml(name, nullptr));
}

// Returns true if this is the same object.
// returns false for \cite{Knuth} and \footcite{Knuth}
auto CitationItem::match(Istring A, Istring B) -> bool { return key == A && from == B; }

// Case of solve{?}{Knuth}. We return true if the key is Knuth, whatever the
// from field, but only if the entry is unsolved.
auto CitationItem::match_star(Istring A) -> bool { return key == A && !is_solved(); }

// This finds a citation in the table. In the case \footcite{Kunth},
// the first two arguments are the Istrings associated to foot and Knuth.
// If not found, we may insert a new item (normal case),
// or return -1 (in case of failure)
auto Bibliography::find_citation_item(Istring from, Istring key, bool insert) -> int {
    int n = citation_table.size();
    for (int i = 0; i < n; i++)
        if (citation_table[i].match(key, from)) return i;
    if (!insert) return -1;
    citation_table.emplace_back(key, from);
    return n;
}

// This is like the function above. In the case \footcite{Kunth},
// the two arguments are the Istrings associated to foot and Knuth.
// If not found, we try \cite[?]{Kunth}, using any possibility for the first
// argument (this matches only unsolved references). In case of failure
// a new entry is added, of type FROM.
auto Bibliography::find_citation_star(Istring from, Istring key) -> int {
    int n = find_citation_item(from, key, false);
    if (n >= 0) return n;
    n = citation_table.size();
    for (int i = 0; i < n; i++)
        if (citation_table[i].match_star(key)) return i;
    citation_table.emplace_back(key, from);
    return n;
}

// \cititem{foo}{bar} translates \cititem-foo{bar}
// If the command with the funny name is undefined then translation is
// <foo>bar</foo>
// Command has to be in Bib mode, argument translated in Arg mode.
void Parser::T_cititem() {
    String  a = fetch_name0_nopar();
    Buffer &B = biblio_buf4;
    B << bf_reset << "cititem-" << a;
    finish_csname(B);
    see_cs_token();
    if (cur_cmd_chr.get_cmd() != relax_cmd) {
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
    *file << B.convert_to_log_encoding();
    ptr.insert(B.to_string(), true);
    B.reset();
}

// Returns the index of the macro named name if it exists,  not_found otherwise.
auto Bibtex::look_at_macro(const Buffer &name) -> int {
    int h = name.hashcode(bib_hash_mod);
    return look_at_macro(h, name.c_str());
}

auto Bibtex::look_at_macro(int h, String name) -> int {
    int n = all_macros.size();
    for (int i = 0; i < n; i++)
        if (all_macros[i].is_same(h, name)) return i;
    return not_found;
}

// This looks for a macro named name or xname. If the macro is not found
// and insert is true, we construct a new macro. If xname is true, then
// the macro is initialised to val.
// [ if xname true, we define a system macro,
//   otherwise we define/redefine user one]
auto Bibtex::find_a_macro(Buffer &name, bool insert, String xname, String val) -> int {
    if (xname != nullptr) name << bf_reset << xname;
    int h   = name.hashcode(bib_hash_mod);
    int res = look_at_macro(h, name.c_str());
    if (res >= 0 || !insert) return res;
    res = all_macros.size();
    if (xname != nullptr)
        all_macros.emplace_back(h, xname, val);
    else
        all_macros.emplace_back(h, name);
    return res;
}

// This defines a system macro.
void Bibtex::define_a_macro(String name, String value) { find_a_macro(biblio_buf1, true, name, value); }

// Return an integer associated to a field position.
auto Bibtex::find_field_pos(String s) const -> field_pos {
    if (s == nullptr) return fp_unknown;
    auto S = Istring(s);
    // Check is this has to be ignored
    std::vector<Istring> &Bib_s        = the_main->get_bibtex_fields_s();
    int                   additional_s = Bib_s.size();
    for (int i = 0; i < additional_s; i++)
        if (Bib_s[i] == S) return fp_unknown;

    // Check is this is standard
    if (S == cstb_address) return fp_address;
    if (S == cstb_author) return fp_author;
    if (S == cstb_booktitle) return fp_booktitle;
    if (S == cstb_chapter) return fp_chapter;
    if (S == cstb_doi) return fp_doi;
    if (S == cstb_edition) return fp_edition;
    if (S == cstb_editor) return fp_editor;
    if (S == cstb_howpublished) return fp_howpublished;
    if (S == cstb_institution) return fp_institution;
    if (S == cstb_isbn) return fp_isbn;
    if (S == cstb_issn) return fp_issn;
    if (S == cstb_isrn) return fp_isrn;
    if (S == cstb_journal) return fp_journal;
    if (S == cstb_key) return fp_key;
    if (S == cstb_month) return fp_month;
    if (S == cstb_langue) return fp_langue;
    if (S == cstb_language) return fp_langue;
    if (S == cstb_note) return fp_note;
    if (S == cstb_number) return fp_number;
    if (S == cstb_organization) return fp_organization;
    if (S == cstb_pages) return fp_pages;
    if (S == cstb_publisher) return fp_publisher;
    if (S == cstb_school) return fp_school;
    if (S == cstb_series) return fp_series;
    if (S == cstb_title) return fp_title;
    if (S == cstb_type) return fp_type;
    if (S == cstb_url) return fp_url;
    if (S == cstb_volume) return fp_volume;
    if (S == cstb_year) return fp_year;
    if (S == cstb_crossref) return fp_crossref;
    // Check is this is additional
    std::vector<Istring> &Bib        = the_main->get_bibtex_fields();
    int                   additional = Bib.size();
    for (int i = 0; i < additional; i++)
        if (Bib[i] == S) return field_pos(fp_unknown + i + 1);
    return fp_unknown;
}

// Finds the type of an entry (or comment, string, preamble).
auto Bibtex::find_type(String s) -> entry_type {
    if (s[0] == 0) return type_comment; // in case of error.
    auto S = Istring(s);

    std::vector<Istring> &Bib2        = the_main->get_bibtex_extensions_s();
    int                   additional2 = Bib2.size();
    for (int i = 0; i < additional2; i++)
        if (Bib2[i] == S) return type_comment;
    if (S == cstb_article) return type_article;
    if (S == cstb_book) return type_book;
    if (S == cstb_booklet) return type_booklet;
    if (S == cstb_conference) return type_conference;
    if (S == cstb_coursenotes) return type_coursenotes;
    if (S == cstb_comment) return type_comment;
    if (S == cstb_inbook) return type_inbook;
    if (S == cstb_incollection) return type_incollection;
    if (S == cstb_inproceedings) return type_inproceedings;
    if (S == cstb_manual) return type_manual;
    if (S == cstb_masterthesis) return type_masterthesis;
    if (S == cstb_mastersthesis) return type_masterthesis;
    if (S == cstb_misc) return type_misc;
    if (S == cstb_phdthesis) return type_phdthesis;
    if (S == cstb_proceedings) return type_proceedings;
    if (S == cstb_preamble) return type_preamble;
    if (S == cstb_techreport) return type_techreport;
    if (S == cstb_string) return type_string;
    if (S == cstb_unpublished) return type_unpublished;

    std::vector<Istring> &Bib        = the_main->get_bibtex_extensions();
    int                   additional = Bib.size();
    for (int i = 0; i < additional; i++)
        if (Bib[i] == S) return entry_type(type_extension + i + 1);
    return type_unknown;
}

// Dual function. Returns the name of the thing.
auto bib_ns::type_to_string(entry_type x) -> name_positions {
    switch (x) {
    case type_article: return cstb_article;
    case type_book: return cstb_book;
    case type_booklet: return cstb_booklet;
    case type_conference: return cstb_conference;
    case type_coursenotes: return cstb_coursenotes;
    case type_inbook: return cstb_inbook;
    case type_incollection: return cstb_incollection;
    case type_inproceedings: return cstb_inproceedings;
    case type_manual: return cstb_manual;
    case type_masterthesis: return cstb_masterthesis;
    case type_misc: return cstb_misc;
    case type_phdthesis: return cstb_phdthesis;
    case type_proceedings: return cstb_proceedings;
    case type_techreport: return cstb_techreport;
    case type_unpublished: return cstb_unpublished;
    default: return cstb_unknown;
    }
}

// This is a table of prefixes, for the RA only
String ra_pretable[8];

void bib_ns::boot_ra_prefix(String s) {
    char *tmp = new char[3 * 8];
    for (int i = 0; i < 8; i++) {
        int j          = i * 3;
        ra_pretable[i] = tmp + j;
        tmp[j]         = s[1];
        tmp[j + 1]     = 'A' + i;
        tmp[j + 2]     = 0;
    }
    tmp[0] = s[0];
    tmp[3] = s[2];
}

// This returns a prefix from ra_pretable, according to from and type_int.
auto BibEntry::ra_prefix() const -> String {
    if (get_from() == from_refer) return ra_pretable[0];
    if (get_from() == from_foot) return ra_pretable[1];
    switch (type_int) {
    case type_book:
    case type_booklet:
    case type_proceedings: return ra_pretable[2];
    case type_phdthesis: return ra_pretable[3];
    case type_article:
    case type_inbook:
    case type_incollection: return ra_pretable[4];
    case type_conference:
    case type_inproceedings: return ra_pretable[5];
    case type_manual:
    case type_techreport:
    case type_coursenotes: return ra_pretable[6];
    case type_masterthesis:
    case type_misc:
    case type_unpublished:
    default: return ra_pretable[7];
    }
}

// This finds a citation that matches exactly S
auto Bibtex::find_entry(const CitationKey &s) -> BibEntry * {
    int len = all_entries.size();
    if (old_ra) {
        for (int i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same_old(s)) return all_entries[i];
    } else {
        for (int i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same(s)) return all_entries[i];
    }
    return nullptr;
}

auto CitationKey::is_same_lower_old(const CitationKey &w) const -> bool {
    if (!is_similar_lower(w)) return false;
    if (cite_prefix == w.cite_prefix) return true;
    if (cite_prefix == from_foot || w.cite_prefix == from_foot) return false;
    return true;
}

// This finds a citation whose lowercase equivalent is S.
// Puts in N the number of citations found.
auto Bibtex::find_lower_case(const CitationKey &s, int &n) -> BibEntry * {
    n             = 0;
    int       len = all_entries.size();
    BibEntry *res = nullptr;
    if (old_ra) {
        for (int i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same_lower_old(s)) {
                res = all_entries[i];
                n++;
            }
    } else
        for (int i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same_lower(s)) {
                res = all_entries[i];
                n++;
            }
    return res;
}

// This command is used in the case of apics_all.
// Assume that the citation is Knuth
// Returns -2 if we have \cite{Knuth}, \footcite{Knuth}
// Returns -2 if we have \cite{knuth}, \footcite{knuth}
// Returns 2  if we have \cite{KNUTH}, \footcite{knuth} or other mismatch
// Returns 0 if nothing was found.
auto Bibtex::find_similar(const CitationKey &s, int &n) -> BibEntry * {
    n             = 0;
    int       len = all_entries.size();
    BibEntry *res = nullptr;
    for (int i = 0; i < len; i++)
        if (all_entries[i]->cite_key.is_similar(s)) {
            res = all_entries[i];
            n++;
        }
    if (res != nullptr) {
        n = -n;
        return res;
    }
    bool bad = false;
    for (int i = 0; i < len; i++)
        if (all_entries[i]->cite_key.is_similar_lower(s)) {
            n++;
            if (res == nullptr) {
                res = all_entries[i];
                continue;
            }
            if (!all_entries[i]->cite_key.is_similar(res->cite_key)) bad = true;
        }
    if (!bad) n = -n;
    return res;
}

// This makes a new entry.
auto Bibtex::make_new_entry(const CitationKey &a, bib_creator b) -> BibEntry * { return make_entry(a, b, the_bibliography.unique_bid()); }

// Copy from the biblio
void Bibtex::make_entry(const CitationKey &a, Istring myid) { make_entry(a, because_cite, myid); }

// Generic code
auto Bibtex::make_entry(const CitationKey &a, bib_creator b, Istring myid) -> BibEntry * {
    auto *X      = new BibEntry;
    X->cite_key  = a;
    X->why_me    = b;
    X->id        = all_entries.size();
    X->unique_id = myid;
    all_entries.push_back(X);
    return X;
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
    the_stack.add_newid0(np_bpers);
    if (!(A.null() || A.empty())) the_stack.add_att_to_last(np_full_first, A);
    if (!d.empty()) the_stack.add_att_to_last(np_junior, d);
    the_stack.add_att_to_last(np_nom, c);
    if (!b.empty()) the_stack.add_att_to_last(np_particle, b);
    the_stack.add_att_to_last(np_prenom, a);
}

void Stack::implement_cit(std::string b1, Istring b2, std::string a, std::string c) {
    add_att_to_last(np_userid, Istring(std::move(b1)));
    add_att_to_last(np_id, b2);
    add_att_to_last(np_key, Istring(std::move(a)));
    add_att_to_last(np_from, Istring(std::move(c)));
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
    int   n;
    if (user) {
        implicit_par(zero_code);
        the_stack.add_last(new Xml(np_bibitem, nullptr));
        Istring ukey = nT_optarg_nopar();
        the_stack.get_xid().get_att().push_back(np_bibkey, ukey);
        n = the_stack.get_xid().value;
    } else {
        F    = remove_initial_star();
        n    = read_elt_id(T);
        from = Istring(fetch_name_opt());
    }
    from     = normalise_for_bib(from);
    cur_tok  = T;
    auto key = Istring(fetch_name0_nopar());
    if (user) insert_every_bib();
    if (n == 0) return;
    Xid           N = Xid(n);
    Bibliography &B = the_bibliography;
    if (F)
        n = B.find_citation_star(from, key);
    else
        n = B.find_citation_item(from, key, true);
    CitationItem &CI = B.citation_table[n];
    if (CI.is_solved()) {
        err_ns::local_buf << bf_reset << "Bibliography entry already defined " << key.c_str();
        the_parser.signal_error(the_parser.err_tok, "bad solve");
        return;
    }
    AttList &AL    = N.get_att();
    int      my_id = AL.has_value(np_id);
    if (my_id >= 0) {
        if (CI.has_empty_id())
            CI.set_id(AL.get_val(my_id));
        else {
            err_ns::local_buf << bf_reset << "Cannot solve (element has an Id) " << key.c_str();
            the_parser.signal_error(the_parser.err_tok, "bad solve");
            return;
        }
    } else
        AL.push_back(the_names[np_id], CI.get_bid());
    CI.set_solved(N);
}

// this is the same as \bibitem[]{foo}{}
void Parser::T_empty_bibitem() {
    flush_buffer();
    std::string w  = "";
    std::string a  = "";
    std::string b  = sT_arg_nopar();
    Istring     id = the_bibtex->exec_bibitem(w, b);
    if (id.empty()) return;
    leave_v_mode();
    the_stack.push1(np_citation);
    the_stack.implement_cit(b, id, a, w);
    the_stack.pop(np_citation);
}

auto Bibtex::exec_bibitem(const std::string &w, const std::string &b) -> Istring {
    BibEntry *X = find_entry(b.c_str(), w, because_all);
    if (X->type_int != type_unknown) {
        the_parser.parse_error("Duplicate bibliography entry ignored");
        return Istring("");
    }
    X->type_int = type_article;
    X->set_explicit_cit();
    return X->get_unique_id();
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
    the_stack.push1(np_citation);
    the_stack.add_att_to_last(np_type, d);
    the_stack.implement_cit(b1, Istring(b2), a, c);
    the_stack.set_bib_mode();
    ignore_optarg();
    insert_every_bib();
}

void Parser::insert_every_bib() {
    TokenList everybib = toks_registers[everybibitem_code].get_val();
    if (everybib.empty()) return;
    if (tracing_commands()) the_log << lg_startbrace << "<everybibitem> " << everybib << lg_endbrace;
    back_input(everybib);
}

// ----------------------------------------------------------------------
//
// the bibtex parser: read the data base files
//
//

// Signals an error while reading the file.
// We do not use parse_error here
void Bibtex::err_in_file(String s, bool last) {
    main_ns::nb_errs++;
    log_and_tty << lg_start << "Error detected at line " << cur_bib_line << " of bibliography file " << in_lines.get_file_name() << "\n";
    if (!cur_entry_name.empty()) log_and_tty << "in entry " << cur_entry_name << " started at line " << last_ok_line << "\n";
    log_and_tty << s;
    if (last) log_and_tty << ".\n";
}

void Bibtex::err_in_entry(String a) {
    main_ns::nb_errs++;
    log_and_tty << "Error signaled while handling entry " << cur_entry_name;
    if (cur_entry_line >= 0) log_and_tty << " (line " << cur_entry_line << ")";
    log_and_tty << "\n" << a;
}

void Bibtex::err_in_name(String a, int i) {
    err_in_entry(a);
    log_and_tty << "\nbad syntax in author or editor name\n";
    log_and_tty << "error occurred at character position " << i << " in the string\n" << name_buffer.c_str() << ".\n";
}

// Returns next line of the .bib file. Error if EOF and what.
// Throws if EOF.
void Bibtex::next_line(bool what) {
    static Buffer scratch;
    scratch.reset();
    int n = in_lines.get_next(scratch);
    if (n > 0)
        cur_bib_line = n;
    else {
        if (what) err_in_file("Illegal end of bibtex database file", true);
        throw Berror();
    }
    the_parser.set_cur_line(n);
    inbuf.insert_string(scratch);
    inbuf.extract_chars(input_line);
    input_line_pos = 0;
}

// Skip over a space. Error in case of EOF.
// This is the only function that reads a new line.
void Bibtex::skip_space() {
    for (;;) {
        if (at_eol())
            next_line(true);
        else {
            if (cur_char().is_space())
                advance();
            else
                return;
        }
    }
}

// Reads until the next @ sign. This is the only function
// that accepts to read an EOF without error.
void Bibtex::scan_for_at() {
    for (;;) {
        if (at_eol())
            next_line(false);
        else {
            codepoint c = next_char();
            if (c == '@') return;
        }
    }
}

static const String scan_msgs[] = {
    "bad syntax for a field type",
    "bad syntax for an entry type",
    "bad syntax for a string name",
    "bad syntax for a field name",
    "\nremaining characters on current line will be ignored.\n", // 4
    "\nall characters up to next @ will be ignored.\n",          // 5
    "\nlet's assume that the brace ends the entry.\n",           // 6
    "\nall characters up to next `('  or `{' ignored.\n",        // 7
    "\nall characters up to next `=' ignored.\n"                 // 8
};

// Calls scan_identifier0, and interprets the return message.
// that should be 0, or at least 4 in absolute value
// If the retval of scan_identifier0 is <=0, but not -4
// Note: The error message must be output before skip_space,
// in case we are at EOF.
auto Bibtex::scan_identifier(int what) -> bool {
    int ret = scan_identifier0(what);
    if (ret != 0) log_and_tty << scan_msgs[ret > 0 ? ret : -ret];
    if (ret == 4 || ret == -4) {
        start_comma = false;
        reset_input();
        skip_space();
    }
    if (ret > 0) return true;
    return false;
}

// Scans an identifier. It will be in lower case in token_buf.
// Scans also something after it. Invariant: at_eol() is false on entry.
// it is also false on exit
auto Bibtex::scan_identifier0(int what) -> int {
    Buffer &B = token_buf;
    B.reset();
    codepoint c = cur_char();
    if (!c.is_ascii() || c.is_digit() || get_class(c) != legal_id_char) return wrong_first_char(c, what);
    for (;;) {
        if (at_eol()) break;
        c = next_char();
        if (!c.is_ascii() || get_class(c) != legal_id_char) {
            input_line_pos--; // c is to be read again
            break;
        }
        c = c.to_lower();
        B.push_back(c);
    }
    // a field part.
    if (what == 0) return check_val_end();
    if (at_eol() || c.is_space()) skip_space();
    if (what == 1) return check_entry_end(); // case of @foo
    return check_field_end(what);
}

// A bunch of functions called when we see the end of an identifier.
// We start with a function that complains if first character is wrong.
auto Bibtex::wrong_first_char(codepoint c, int what) -> int {
    err_in_file(scan_msgs[what], false);
    if (c.is_digit())
        log_and_tty << "\nit cannot start with a digit";
    else
        log_and_tty << "\nit cannot start with `" << c << "'";
    if (c == '%') log_and_tty << "\n(A percent sign is not a comment character in bibtex)";
    if (what == 1 || what == 2) return 5;
    if (what == 0) {
        if (c == '}') { // this brace might be the end of the entry
            return 6;
        }
    }
    return 4;
}

// We have seen @article, plus space, plus c. We want paren or brace.
// Return 0 if OK, a negative value otherwise.
// We read c; maybe additional characters in case of error
// @comment(etc) is ignored
auto Bibtex::check_entry_end() -> int {
    if (token_buf == "comment") return 0; // don't complain
    codepoint c = cur_char();
    if (c == '(' || c == '{') return check_entry_end(0);
    err_in_file(scan_msgs[1], false);
    log_and_tty << "\nexpected `('  or `{'";
    for (;;) {
        c = cur_char();
        if (c == '(' || c == '{') return check_entry_end(-7);
        advance();
        if (at_eol()) return -4;
    }
}

// We store in right_outer_delim the closing delimiter.
// associated to the current char. We skip over spaces.
auto Bibtex::check_entry_end(int k) -> int {
    right_outer_delim = cur_char() == '(' ? ')' : '}';
    advance();
    skip_space();
    return k;
}

// We have seen foo in foo=bar. We have skipped over spaces
// Returns 0 if OK. We try to read some characters on the current line
// in case of error
auto Bibtex::check_field_end(int what) -> int {
    if (cur_char() == '=') {
        advance();
        skip_space();
        return 0;
    }
    err_in_file(scan_msgs[what], false);
    log_and_tty << "\nexpected `=' sign";
    for (;;) {
        if (at_eol()) return what == 2 ? 5 : -4;
        if (cur_char() == '=') {
            advance();
            skip_space();
            return -8;
        }
        advance();
    }
}

// We have something like year=foo. After foo we can have a # (year is foo
// concatenated with something), a comma (there are more fields after foo)
// or brace/paren indicating the end of the entry. Of course spaces are allowed.
// Returns 0 ik OK, 4 otherwise.
auto Bibtex::check_val_end() -> int {
    if (at_eol()) return 0;
    codepoint c = cur_char();
    if (c.is_space() || c == '#' || c == ',' || c == right_outer_delim) return 0;
    err_in_file(scan_msgs[0], false);
    log_and_tty << "\nit cannot end with `" << c << "'\n"
                << "expecting `,', `#' or `" << right_outer_delim << "'";
    return 4;
}

// Returns the buffer without initial and final space, if init is true.
// In any case, a tab is converted into a space, multiple space chars
// are replaced by single ones.
// We can safely assume that buffer is ASCII
auto Buffer::special_convert(bool init) -> std::string {
    ptr = 0;
    if (init) skip_sp_tab_nl();
    biblio_buf1.reset();
    bool space = true;
    for (;;) {
        unsigned char c = next_char();
        if (c == 0U) break;
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
    if (init && biblio_buf1.last_char() == ' ') biblio_buf1.rrl();
    return biblio_buf1.to_string();
}

// This reads a field into field_buf.
// This can be "foo" # {bar} # 1234 # macro.
// If store is false, we do not look at the value of a macro
void Bibtex::read_field(bool store) {
    field_buf.reset();
    for (;;) {
        read_one_field(store);
        skip_space();
        if (cur_char() != '#') return;
        advance();
        skip_space();
    }
}

// This reads a single field
void Bibtex::read_one_field(bool store) {
    codepoint c = cur_char();
    if (c == '{' || c == '\"') {
        uchar delimiter = c == '{' ? '}' : '\"';
        advance(); // reads left delimiter
        int bl = 0;
        for (;;) {
            if (at_eol()) {
                field_buf.push_back(' ');
                next_line(true);
                continue;
            }
            c = cur_char();
            advance();
            if (c == '\\') {
                field_buf.push_back(c);
                if (at_eol()) {
                    field_buf.push_back(' ');
                    next_line(true);
                    continue;
                }
                field_buf.push_back(cur_char());
                advance();
                continue;
            }
            if (c == delimiter && bl == 0) return;
            if (c == '}' && bl == 0) {
                err_in_file("illegal closing brace", true);
                continue;
            }
            field_buf.push_back(c);
            if (c == '}') bl--;
            if (c == '{') bl++;
        }
    } else if (c.is_digit()) {
        for (;;) {
            if (at_eol()) return;
            c = cur_char();
            if (!c.is_digit()) return;
            field_buf.push_back(c);
            advance();
        }
    } else {
        bool k = scan_identifier(0);
        if (store && !k) {
            int macro = find_a_macro(token_buf, false, nullptr, nullptr);
            if (macro == not_found) {
                err_in_file("", false);
                log_and_tty << "undefined macro " << token_buf << ".\n";
            } else
                field_buf << all_macros[macro].get_value();
        }
    }
}

// Returns true if because of \nocite{*}
auto Bibtex::auto_cite() -> bool {
    if (refer_biblio) return true;
    if (normal_biblio && nocitestar) return true;
    return false;
}

// This finds entry named s, or case-equivalent.
// creates entry if not found. This is used by exec_bibitem
auto Bibtex::find_entry(String s, const std::string &prefix, bib_creator bc) -> BibEntry * {
    CitationKey key(prefix.c_str(), s);
    BibEntry *  X = find_entry(key);
    if (X != nullptr) return X;
    int n = 0;
    X     = find_lower_case(key, n);
    if (n > 1) err_in_file("more than one lower case key equivalent", true);
    if (X == nullptr) return make_new_entry(key, bc);
    return X;
}

// This finds entry named s, or case-equivalent. If create is true,
// creates entry if not found.
// Used by see_new_entry (create=auto_cite), or parse_crossref
// prefix can be from_year, from_refer, from_foot, from_any;
// If create is true, it is either a crossref, or \nocite{*}+from_year
// or from_refer, or bug; if prefix is from_any, we use from_year instead.
//

auto Bibtex::find_entry(String s, bool create, bib_creator bc) -> BibEntry * {
    bib_from prefix = default_prefix();
    if (create && prefix == from_any) prefix = from_year;
    CitationKey key(prefix, s);
    similar_entries = 1;
    int n           = 0;
    if (prefix == from_any) {
        BibEntry *X = find_similar(key, n);
        if (n > 1) err_in_file("more than one lower case key equivalent", true);
        if (n < 0) similar_entries = -n;
        return X;
    }
    BibEntry *X = find_entry(key);
    if (X != nullptr) return X;
    X = find_lower_case(key, n);
    if (n > 1) err_in_file("more than one lower case key equivalent", true);
    if ((X == nullptr) && create) return make_new_entry(key, bc);
    return X;
}

// This command is called when we have see @foo{bar,... on line lineno
// The type of FOO is in cn, the value BAR in cur_entry_name.
// the entry is ignored if teh name is in the omit list (with the same case)
// This does not create a new entry, complains if the entry exists
// and is not empty. If OK, we start to fill the entry.

auto Bibtex::see_new_entry(entry_type cn, int lineno) -> BibEntry * {
    for (const auto &i : omitcite_list)
        if (i == cur_entry_name) {
            the_log << lg_start << "bib: Omitting " << cur_entry_name << lg_end;
            return nullptr;
        }
    BibEntry *X = find_entry(cur_entry_name.c_str(), auto_cite(), because_all);
    if (X == nullptr) return X;
    if (X->type_int != type_unknown) {
        err_in_file("duplicate entry ignored", true);
        return nullptr;
    }
    if (old_ra && default_prefix() == from_refer) X->cite_key.move_to_refer();
    int ext = int(cn) - int(type_extension);
    if (ext > 0) {
        X->is_extension = ext;
        cn              = type_extension;
    } else
        X->is_extension = 0;
    X->type_int   = cn;
    X->first_line = lineno;
    return X;
}

// Reads someththing like foo=bar, and stores the result in X if ok
// The first field is preceded by the key and a comma.
// Thus, we start reading the comma (unless previous fiels has a syntax err).
// A null pointer means an entry to be discarded
void Bibtex::parse_one_field(BibEntry *X) {
    if (start_comma) {
        if (cur_char() != ',')
            err_in_file("expected comma before a field", true);
        else
            advance();
    }
    start_comma = true;
    skip_space();
    if (cur_char() == right_outer_delim) return;
    if (scan_identifier(3)) return;
    field_pos where = fp_unknown;
    bool      store = false;
    if (X != nullptr) { // if X null, we just read, but store nothing
        cur_field_name = token_buf.to_string();
        where          = find_field_pos(token_buf.c_str());
        if (where != fp_unknown) store = true;
    }
    read_field(store);
    if (!store) return;
    bool ok = X->store_field(where);
    if (!ok) {
        err_in_file("", false);
        log_and_tty << "duplicate field `" << cur_field_name << "' ignored.\n";
        return;
    }
    if (where != fp_crossref) return;
    X->parse_crossref(); // special action
}

// This is non trivial, because we have a fixed-sized array and a varying size
// return true if the field is not already filled.
auto BibEntry::store_field(field_pos where) -> bool {
    if (int(where) < fp_unknown) {
        if (all_fields[where].empty()) {
            all_fields[where] = field_buf.special_convert(true);
            return true;
        }
        return false;
    }
    int k = int(where) - fp_unknown - 1;
    if (user_fields[k].empty()) {
        user_fields[k] = field_buf.special_convert(true);
        return true;
    }
    return false;
}

// This parses an @something.
void Bibtex::parse_one_item() {
    cur_entry_name = "";
    cur_entry_line = -1;
    scan_for_at();
    last_ok_line = cur_bib_line;
    skip_space();
    bool k = scan_identifier(1);
    if (k) return;
    entry_type cn = find_type(token_buf.c_str());
    if (cn == type_comment) return;
    if (cn == type_preamble) {
        read_field(true);
        bbl.push_back(field_buf.to_string());
    } else if (cn == type_string) {
        k = scan_identifier(2);
        if (k) return;
        int X = find_a_macro(token_buf, true, nullptr, nullptr);
        mac_def_val(X);
        read_field(true);
        mac_set_val(X, field_buf.special_convert(false));
    } else {
        cur_entry_line = cur_bib_line;
        Buffer &A      = biblio_buf1;
        A.reset();
        skip_space();
        for (;;) {
            if (at_eol()) break;
            codepoint c = cur_char();
            if (c == ',' || c.is_space()) break;
            A << c;
            next_char();
        }
        skip_space();
        cur_entry_name = A.to_string();
        BibEntry *X    = see_new_entry(cn, last_ok_line);
        int       m    = similar_entries;
        while (cur_char() != ')' && cur_char() != '}') parse_one_field(X);
        if (m > 1) handle_multiple_entries(X);
    }
    codepoint c = cur_char();
    if (c == ')' || c == '}') advance();
    if (c != right_outer_delim) err_in_file("bad end delimiter", true);
}

// When we see crossref=y, we expect to see entry Y later
// We make Y to be cited, and link X to Y and Y to X.

void BibEntry::parse_crossref() {
    String name = all_fields[fp_crossref].c_str();
    if (name[0] == 0) return;
    bib_creator bc = the_bibtex->auto_cite() ? because_all : because_crossref;
    BibEntry *  Y  = the_bibtex->find_entry(name, true, bc);
    if (this == Y) return; /// should not happen
    crossref = Y;
    if (Y->crossref_from == nullptr) Y->crossref_from = this;
}

// Assume that X has a crossref to Y. Then all empty fields will be
// replaced by the fields of Y, except crossref and key.
void BibEntry::un_crossref() {
    if (crossref == nullptr) return;
    copy_from(crossref, 2);
}

void BibEntry::copy_from(BibEntry *Y) {
    if (type_int != type_unknown) {
        the_bibtex->err_in_file("duplicate entry ignored", true);
        return;
    }
    the_log << "Copy Entry " << Y->cite_key.get_name() << " into " << cite_key.get_name() << "\n";
    is_extension = Y->is_extension;
    type_int     = Y->type_int;
    first_line   = Y->first_line;
    copy_from(Y, 0);
}

void BibEntry::copy_from(BibEntry *Y, int k) {
    if (Y->type_int == type_unknown) {
        log_and_tty << "Unknown reference in crossref " << Y->cite_key.get_name() << "\n";
        return; // Should signal an error
    }
    for (int i = k; i < fp_unknown; i++) {
        //   std::cout<< bib_xml_name[i] << all_fields[i].empty() << Y->all_fields[i] <<"\n";
        if (all_fields[i].empty()) all_fields[i] = Y->all_fields[i];
    }
    int n = the_main->get_bibtex_fields().size();
    for (int i = 0; i < n; i++)
        if (user_fields[i].empty()) user_fields[i] = Y->user_fields[i];
}

void Bibtex::handle_multiple_entries(BibEntry *Y) {
    CitationKey s   = Y->cite_key;
    int         len = all_entries.size();
    for (int i = 0; i < len; i++)
        if (all_entries[i]->cite_key.is_similar(s)) {
            BibEntry *X = all_entries[i];
            if (X == Y) continue;
            X->copy_from(Y);
        }
}

void Bibtex::parse_a_file() {
    last_ok_line = 0;
    reset_input();
    try {
        for (;;) parse_one_item();
    } catch (Berror x) {}
}

// ------------------------------------------------------------
//
// Working with the entries

auto xless(BibEntry *A, BibEntry *B) -> bool { return A->Sort_label() < B->Sort_label(); }

// This is the main function.
void Bibtex::work() {
    int n = all_entries.size();
    if (n == 0) return;
    if (bbl.non_empty_buf()) bbl.newline();
    boot_ra_prefix("ABC");
    all_entries_table.reserve(n);
    for (int i = 0; i < n; i++) all_entries[i]->un_crossref();
    for (int i = 0; i < n; i++) all_entries[i]->work(i);
    int nb_entries = all_entries_table.size();
    main_ns::log_or_tty << "Seen " << nb_entries << " bibliographic entries.\n";
    // Sort the entries
    std::sort(all_entries_table.begin(), all_entries_table.end(), xless);
    std::string previous_label = "";
    int         last_num       = 0;
    for (int i = 0; i < nb_entries; i++) all_entries_table[i]->forward_pass(previous_label, last_num);
    int next_extra = 0;
    for (int i = nb_entries - 1; i >= 0; i--) all_entries_table[i]->reverse_pass(next_extra);
    if (want_numeric)
        for (int i = 0; i < nb_entries; i++) all_entries_table[i]->numeric_label(i + 1);
    for (int i = 0; i < nb_entries; i++) all_entries_table[i]->call_type();
    bbl.finish();
}

// Some entries are discarded, others are pushed in all_entries_table.
// For these we call normalise and presort
void BibEntry::work(int serial) {
    cur_entry_line = -1;
    cur_entry_name = cite_key.get_name();
    if (type_int == type_unknown) {
        the_bibtex->err_in_entry("undefined reference.\n");
        if (crossref_from != nullptr) log_and_tty << "This entry was crossref'd from " << crossref_from->cite_key.get_name() << "\n";
        return;
    }
    if (explicit_cit) return;
    if (why_me == because_crossref) return;
    the_bibtex->enter_in_table(this);
    cur_entry_line = first_line;
    normalise();
    presort(serial);
}

// This computes the extra_num field. It's k if this is the k-th entry
// with the same label (0 for the first entry).
void BibEntry::forward_pass(std::string &previous_label, int &last_num) {
    if (label == previous_label) {
        last_num++;
        extra_num = last_num;
    } else {
        last_num       = 0;
        previous_label = label;
        extra_num      = 0;
    }
}

// Uses the extra-num value : 0 gives a, 1 gives b, etc.
// -1 gives nothing.
void BibEntry::use_extra_num() {
    if (extra_num == -1) return;
    Buffer &B = biblio_buf1;
    B << bf_reset << label;
    if (extra_num <= 25)
        B << char('a' + extra_num);
    else
        B << extra_num;
    label = B.to_string();
}

// This must be done after the forward pass. We know if an extra num of 0
// must be converted to -1. We can compute the full label.
void BibEntry::reverse_pass(int &next_extra) {
    if (extra_num == 0 && next_extra != 1) {
        extra_num  = -1;
        next_extra = 0;
    } else {
        next_extra = extra_num;
        use_extra_num();
    }
}

// Creates a numeric version of the label, and (optional) alpha one.
void BibEntry::numeric_label(int i) {
    Buffer &B = biblio_buf1;
    B << bf_reset << '[' << label << ']';
    aux_label = B.to_string();
    B << bf_reset << i;
    label = B.to_string();
}

BibEntry::BibEntry() : label(""), sort_label(""), lab1(""), lab2(""), lab3(""), unique_id("") {
    for (auto &all_field : all_fields) all_field = "";
    std::vector<Istring> &Bib = the_main->get_bibtex_fields();
    int                   n   = Bib.size();
    if (n != 0) {
        user_fields = new std::string[n];
        for (int i = 0; i < n; i++) user_fields[i] = "";
    }
}

// -----------------------------------------------------------------------
// printing the bbl.

void BibEntry::out_something(field_pos p, std::string s) {
    bbl.push_back_cmd("cititem");
    bbl.push_back_braced(bib_xml_name[p]);
    bbl.push_back_braced(std::move(s));
    bbl.newline();
}

// output a generic field as \cititem{k}{value}
// If no value, and w>0, a default value will be used.
void BibEntry::out_something(field_pos p, int w) {
    std::string s = all_fields[p];
    if (s.empty()) s = my_constant_table[w - 1];
    out_something(p, s);
}

void BibEntry::out_something(field_pos p) {
    std::string s = all_fields[p];
    if (s.empty()) return;
    out_something(p, s);
}

// Outputs a part of the thing.
void BibEntry::format_series_etc(bool pa) {
    out_something(fp_series);
    out_something(fp_number);
    out_something(fp_volume);
    if (pa) {
        out_something(fp_publisher);
        out_something(fp_address);
    }
}

// Outputs author or editor.
void BibEntry::format_author(bool au) {
    field_pos p = au ? fp_author : fp_editor;
    if (all_fields[p].empty()) return;
    std::string data = au ? author_data.value : editor_data.value;
    bbl.push_back_cmd(bib_xml_name[p]);
    bbl.push_back_braced(data);
    bbl.newline();
}

auto CitationKey::from_to_string() const -> String {
    if (cite_prefix == from_year) return "year";
    if (cite_prefix == from_refer) return "refer";
    return "foot";
}

void BibEntry::call_type() {
    bbl.reset();
    bbl.push_back("%");
    bbl.newline();
    //  bbl.push_back("%%%");bbl.push_back(sort_label); bbl.newline();
    bbl.push_back_cmd("citation");
    bbl.push_back_braced(label);
    bbl.push_back_braced(cite_key.get_name());
    bbl.push_back_braced(unique_id.c_str());
    bbl.push_back_braced(from_to_string());
    String my_name = nullptr;
    if (is_extension != 0)
        my_name = the_main->get_bibtex_extensions()[is_extension - 1].c_str();
    else
        my_name = the_names[type_to_string(type_int)].c_str();
    bbl.push_back_braced(my_name);
    bbl.push_back(aux_label);
    bbl.newline();
    if (type_int == type_extension || bib_ns::raw_bib)
        call_type_all();
    else
        call_type_special();

    out_something(fp_doi);
    std::string s = all_fields[fp_url];
    if (!s.empty()) {
        Buffer &B = biblio_buf1;
        if (strncmp(s.c_str(), "\\rrrt", 5) == 0)
            bbl.push_back(s);
        else {
            bbl.push_back("\\csname @href\\endcsname");
            //    string S = hack_bib_space(s);
            bbl.push_back_braced(B.remove_space(s));
            bbl.push_back(B.insert_break(s));
        }
        bbl.newline();
    }
    std::vector<Istring> &Bib        = the_main->get_bibtex_fields();
    int                   additional = Bib.size();
    for (int i = 0; i < additional; i++) {
        std::string s = user_fields[i];
        if (!s.empty()) {
            bbl.push_back_cmd("cititem");
            bbl.push_back_braced(Bib[i].c_str());
            bbl.push_back_braced(s);
            bbl.newline();
        }
    }
    out_something(fp_note);
    bbl.push_back_cmd("endcitation");
    bbl.newline();
}

void BibEntry::call_type_all() {
    format_author(true);
    out_something(fp_title);
    format_author(false);
    out_something(fp_journal);
    out_something(fp_edition);
    format_series_etc(true);
    out_something(fp_howpublished);
    out_something(fp_booktitle);
    out_something(fp_organization);
    out_something(fp_pages);
    out_something(fp_chapter);
    if (type_int == type_masterthesis)
        out_something(fp_type, 3);
    else if (type_int == type_phdthesis)
        out_something(fp_type, 1);
    else if (type_int == type_techreport)
        out_something(fp_type, 2);
    else
        out_something(fp_type);
    out_something(fp_school);
    out_something(fp_institution);
    out_something(fp_month);
    out_something(fp_year);
}

void BibEntry::call_type_special() {
    if (type_int != type_proceedings) format_author(true);
    if (type_int == type_book || type_int == type_inbook) format_author(false);
    out_something(fp_title);
    if (type_int == type_proceedings || type_int == type_incollection) format_author(false);
    switch (type_int) {
    case type_article:
        out_something(fp_journal);
        out_something(fp_number);
        out_something(fp_volume);
        break;
    case type_book:
    case type_inbook:
        out_something(fp_edition);
        format_series_etc(true);
        break;
    case type_booklet:
        out_something(fp_howpublished);
        out_something(fp_address);
        break;
    case type_incollection:
        out_something(fp_booktitle);
        format_series_etc(true);
        break;
    case type_inproceedings:
    case type_conference:
        out_something(fp_booktitle);
        format_series_etc(false);
        out_something(fp_organization);
        out_something(fp_publisher);
        format_author(false);
        out_something(fp_pages);
        out_something(fp_address);
        break;
    case type_manual:
        out_something(fp_organization);
        out_something(fp_edition);
        out_something(fp_address);
        break;
    case type_masterthesis:
    case type_coursenotes:
    case type_phdthesis:
        if (type_int == type_coursenotes)
            out_something(fp_type);
        else if (type_int == type_phdthesis)
            out_something(fp_type, 1);
        else
            out_something(fp_type, 3);
        out_something(fp_school);
        out_something(fp_address);
        break;
    case type_misc:
        out_something(fp_howpublished);
        format_author(false);
        out_something(fp_booktitle);
        format_series_etc(true);
        break;
    case type_proceedings:
        out_something(fp_organization);
        format_series_etc(true);
        break;
    case type_techreport:
        out_something(fp_type, 2);
        out_something(fp_number);
        out_something(fp_institution);
        out_something(fp_address);
        break;
    default:;
    }
    out_something(fp_month);
    out_something(fp_year);
    if (type_int == type_inbook || type_int == type_incollection) out_something(fp_chapter);
    switch (type_int) {
    case type_inproceedings:
    case type_conference: break;
    default: out_something(fp_pages);
    }
}

// In the bibliobraphy \url="foo bar"
// gives \href{foobar}{\url{foo\allowbreak bar}}
// We handle here the first string
auto Buffer::remove_space(const std::string &x) -> std::string {
    int n = x.size();
    reset();
    for (int i = 0; i < n; i++)
        if (x[i] != ' ') push_back(x[i]);
    return to_string();
}

// We create here the second string
auto Buffer::insert_break(const std::string &x) -> std::string {
    int n = x.size();
    reset();
    push_back("{\\url{");
    for (int i = 0; i < n; i++) {
        if (x[i] == ' ' && main_ns::bib_allow_break) push_back("\\allowbreak");
        push_back(x[i]);
    }
    push_back("}}");
    return to_string();
}

// Handle author or editor (depending on au), for sort key.
void BibEntry::sort_author(bool au) {
    if (!lab1.empty()) return;
    if (au && !all_fields[fp_author].empty()) {
        lab1 = author_data.short_key;
        lab2 = author_data.long_key;
        lab3 = author_data.name_key;
    }
    if (!au && !all_fields[fp_editor].empty()) {
        lab1 = editor_data.short_key;
        lab2 = editor_data.long_key;
        lab3 = editor_data.name_key;
    }
}

// In case of Lo{\"i}c, repeated calls will set head() to L, o, { and c.
// It works also in the case of non-ascii characters
void Buffer::next_bibtex_char() {
    uchar c = head();
    if (c == 0) return;
    if (c == '\\') {
        ptr++;
        c = head();
        if (c == 0) return;
        ptr += io_ns::how_many_bytes(c);
        if (ptr > wptr) ptr = wptr;
        return;
    }
    int n = io_ns::how_many_bytes(c);
    if (n > 1) {
        ptr += n;
        if (ptr > wptr) ptr = wptr;
        return;
    }
    if (c != '{')
        ptr++;
    else
        skip_over_brace();
}

// We assume that current char is an open brace.
// We can forget about utf8 here.
// Just want {\{\}\}\\} to be interpret correctly
void Buffer::skip_over_brace() {
    int bl = 0;
    for (;;) {
        uchar c = head();
        if (c == 0U) return;
        ptr++;
        if (c == '\\') {
            if (head() == 0) return;
            ptr++;
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

// In the case of `Lo{\"i}c', returns  `Lo{\"i}'.
auto bib_ns::first_three(const std::string &s) -> std::string {
    Buffer &B = biblio_buf1;
    B.reset();
    B.push_back(s);
    B.reset_ptr();
    if (B.head() == '\\') return s;
    B.next_bibtex_char();
    if (B.head() == '\\') return s;
    B.next_bibtex_char();
    if (B.head() == '\\') return s;
    B.next_bibtex_char();
    if (B.head() == 0) return s;
    B.set_last(B.get_ptr());
    return B.to_string();
}

// In the case of `Lo{\"i}c', returns  `{\"i}c' for k=2.
// In the case of `Lo\"i c', returns the whole string.
auto bib_ns::last_chars(const std::string &s, int k) -> std::string {
    Buffer &B = biblio_buf1;
    B.reset();
    B.push_back(s);
    B.reset_ptr();
    int n = -k;
    while (B.head() != 0) {
        n++;
        B.next_bibtex_char();
    }
    if (n <= 0) return s;
    B.reset_ptr();
    while (n > 0) {
        n--;
        B.next_bibtex_char();
    }
    return B.to_string(B.get_ptr());
}

// Signals an error if the year is invalid in the case of refer.
auto Bibtex::wrong_class(int y, const std::string &Y, bib_from from) -> bool {
    if (from != from_refer) return false;
    if (old_ra) return false;
    int ry = the_parser.get_ra_year();
    if (y <= 0 || y > ry || (!distinguish_refer && y == ry)) {
        the_bibtex->err_in_entry("");
        log_and_tty << "entry moved from refer to year because\n";
        if (y == 0)
            log_and_tty << "the year field of this entry is missing.\n";
        else if (y < 0)
            log_and_tty << "the year field of this entry is invalid";
        else if (y == ry)
            log_and_tty << "it is from this year";
        else
            log_and_tty << "it is unpublished";
        if (!Y.empty()) log_and_tty << " (year field is `" << Y << "').\n";
        return true;
    }
    return false;
}

void BibEntry::add_warning(int dy) {
    if (get_from() != from_year) return;
    int y = cur_year;
    if (y <= dy) return;
    if (!all_fields[fp_note].empty()) return;
    all_fields[fp_note] = "To appear";
    log_and_tty << "Warning signaled while handling entry " << cur_entry_name << "\nTo appear added in note field\n";
}

// Converts cite:foo into foo, with some heuristic tests.
auto bib_ns::skip_dp(const std::string &str) -> std::string {
    String s = str.c_str();
    int    i = 0;
    while ((s[i] != 0) && s[i] != ':') i++;
    if ((s[i] != 0) && (s[i + 1] != 0)) return s + i + 1;
    return str;
}

void Bibtex::bad_year(const std::string &given, String wanted) {
    the_bibtex->err_in_entry("");
    log_and_tty << "the year field of this entry should be " << wanted << ", ";
    if (given.empty())
        log_and_tty << "it is missing.\n";
    else
        log_and_tty << "it is `" << given << "'.\n";
}

void BibEntry::presort(int serial) {
    Buffer &B = biblio_buf1;
    sort_author(type_int != type_proceedings);
    sort_author(type_int == type_proceedings);
    if (lab1.empty()) {
        std::string s = all_fields[fp_key];
        if (s.empty()) s = skip_dp(cite_key.get_name());
        lab1 = first_three(s);
        lab2 = s;
        lab3 = s;
    }
    std::string y = all_fields[fp_year];
    // Removed in version 2.13.2.
    //   if(the_main->in_ra() && !the_main->get_no_bibyearmodify()) {
    //     if(the_bibtex->wrong_class(cur_year,y,get_from()))
    //       move_to_year();
    //     add_warning(the_parser.get_ra_year());
    //   }
    //   String yy = the_bibtex->is_year_string(y,get_from());
    //   if(yy)
    //     the_bibtex->bad_year(y,yy);
    {
        std::string s = last_chars(y, 2);
        if (!s.empty()) {
            if (!is_digit(s[0])) s = "";
        }
        label = lab1 + s;
    }
    B.reset();
    if (the_main->in_ra()) B << ra_prefix() << lab3;
    B << label << lab2 << "    " << y << "    ";
    B.special_title(all_fields[fp_title]);
    B.lowercase();
    if (serial < 10) B << '0';
    if (serial < 100) B << '0';
    if (serial < 1000) B << '0';
    if (serial < 10000) B << '0';
    B << serial;
    sort_label = B.to_string();
}

// True if \sortnoop, \SortNoop, \noopsort plus brace or space
// First char to test is at i+1
auto bib_ns::is_noopsort(const std::string &s, int i) -> bool {
    int n = s.size();
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
    int level  = 0;
    int blevel = 0;
    int n      = s.size();
    for (int i = 0; i < n; i++) {
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

void BibEntry::handle_one_namelist(std::string &src, BibtexName &X) {
    if (src.empty()) return;
    biblio_buf1.reset();
    biblio_buf2.reset();
    biblio_buf3.reset();
    biblio_buf4.reset();
    biblio_buf5.reset();
    name_buffer.normalise_for_bibtex(src.c_str());
    int          n     = name_buffer.length() + 1;
    auto *       table = new bchar_type[n];
    NameSplitter W(table);
    name_buffer.fill_table(table);
    W.handle_the_names();
    delete[] table;
    X.value     = biblio_buf1.to_string();
    X.long_key  = biblio_buf2.to_string();
    X.name_key  = biblio_buf4.to_string();
    X.short_key = biblio_buf3.to_string();
    X.cnrs_key  = biblio_buf5.to_string();
}

// This replaces \c{c} by \char'347 in order to avoid some errors.
void Buffer::normalise_for_bibtex(String s) {
    reset();
    push_back(' '); // make sure we can always backup one char
    for (;;) {
        unsigned char c = *s;
        if (c == 0) {
            reset_ptr();
            return;
        }
        push_back(c);
        s++;
        if (c != '\\') continue;
        if (strncmp(s, "c{c}", 4) == 0) {
            wptr--;
            push_back(codepoint(0347));
            s += 4;
        } else if (strncmp(s, "c{C}", 4) == 0) {
            wptr--;
            push_back(codepoint(0307));
            s += 4;
        } else if (strncmp(s, "v{c}", 4) == 0) {
            s += 4;
            wptr--;
            push_back("{\\v c}");
            continue;
        } else if (*s == 'a' && is_accent_char(s[1])) {
            s++;
        } else if (*s == ' ') {
            wptr--;
            at(wptr) = 0;
        } // replace \space by space
    }
}

// For each character, we have its type in the table.
void Buffer::fill_table(bchar_type *table) {
    ptr = 0;
    for (;;) {
        int   i = ptr;
        uchar c = head();
        if (c == 0U) {
            table[i] = bct_end;
            return;
        }
        ptr++;
        if (c >= 128 + 64) {
            table[i] = bct_extended;
            continue;
        }
        if (c >= 128) {
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
            the_bibtex->err_in_name("unexpected character `&' (did you mean `and' ?)", i);
            table[i] = bct_bad;
            continue;
        }
        if (c == '}') {
            the_bibtex->err_in_name("too many closing braces", i);
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
                the_bibtex->err_in_name("commands allowed only within braces", i);
                continue;
            }
            if (is_letter(at(ptr + 1))) {
                table[i]     = bct_cmd;
                table[i + 1] = bct_continuation;
                table[i + 2] = bct_continuation;
                ptr += 2;
                continue;
            }
            if (at(ptr + 1) != '{') {
                table[i]     = bct_bad;
                table[i + 1] = bct_bad;
                ptr++;
                the_bibtex->err_in_name("bad accent construct", i);
                continue;
            }
            at(ptr + 1) = c;
            at(ptr)     = '\\';
            at(ptr - 1) = '{';
            the_log << lg_start << "+bibchanged " << data() << lg_end;
        }
        int bl   = 1;
        int j    = i;
        table[i] = bct_brace;
        for (;;) {
            if (head() == 0) {
                the_bibtex->err_in_name("this cannot happen!", j);
                at(j)    = 0;
                table[j] = bct_end;
                wptr     = j;
                return;
            }
            c          = head();
            table[ptr] = bct_continuation;
            ptr++;
            if (c == '{') bl++;
            if (c == '}') {
                bl--;
                if (bl == 0) break;
            }
        }
    }
}

// This handles all the names in the list.
void NameSplitter::handle_the_names() {
    bool is_first_name = true;
    int  serial        = 1;
    int  pos           = 1; // there is a space at position 0
    main_data.init(table);
    for (;;) {
        name_buffer.set_ptr(pos);
        bool is_last_name = name_buffer.find_and(table);
        int  k            = name_buffer.get_ptr();
        main_data.init(pos, k);
        handle_one_name(is_first_name, is_last_name, serial);
        if (is_last_name) return;
        is_first_name = false;
        serial++;
        pos = k + 3;
    }
}

auto Buffer::find_and(const bchar_type *table) -> bool {
    for (;;) {
        char c = head();
        if (c == 0) return true;
        ptr++;
        if (table[ptr - 1] == bct_space && is_and(ptr)) return false;
    }
}

// True if this is an `and'
auto Buffer::is_and(int k) -> bool {
    char c = at(k);
    if (c != 'a' && c != 'A') return false;
    c = at(k + 1);
    if (c != 'n' && c != 'N') return false;
    c = at(k + 2);
    if (c != 'd' && c != 'D') return false;
    c = at(k + 3);
    if (c != ' ' && c != '\t' && c != '\n') return false;
    return true;
}

auto operator<<(Buffer &X, const Bchar &Y) -> Buffer & {
    int i = Y.first;
    int j = Y.last;
    for (int k = i; k < j; k++)
        if (Y.table[k] != bct_bad) X.push_back(name_buffer[k]);
    return X;
}

// Very complicated function. Assume that the names are Alpha,
// Bravo, Charlie, Delta, usw. The key will be
// Alp, AB, ABC, ABCD, ABC+, if there are 1,2,3,4,... authors.
// The name `others' is handled specially if last, not first.

void NameSplitter::handle_one_name(bool ifn, bool iln, int serial) {
    first_name.init(table);
    last_name.init(table);
    jr_name.init(table);
    main_data.remove_junk();
    int F  = main_data.first;
    int L  = main_data.last;
    int fc = 0, lc = 0, hm = 0;
    main_data.find_a_comma(fc, lc, hm);
    if (hm >= 2) {
        last_name.init(F, fc);
        jr_name.init(fc + 1, lc);
        first_name.init(lc + 1, L);
        if (hm > 2) {
            the_bibtex->err_in_entry("");
            log_and_tty << "too many commas (namely " << hm << ") in name\n" << name_buffer.c_str() << ".\n";
        }
    } else if (hm == 1) {
        first_name.init(fc + 1, L);
        last_name.init(F, fc);
    } else { // no comma
        int k = main_data.find_a_lower();
        if (k == L) {
            k = main_data.find_a_space();
            if (k == L) {
                main_data.invent_spaces();
                k = main_data.find_a_space();
            }
        }
        if (k == L)
            last_name.init(F, L);
        else {
            first_name.init(F, k);
            last_name.init(k + 1, L);
        }
    }
    first_name.remove_junk();
    last_name.remove_junk();
    jr_name.remove_junk();
    if (first_name.empty() && last_name.empty() && jr_name.empty()) {
        the_bibtex->err_in_entry("empty name in\n");
        log_and_tty << name_buffer.c_str() << ".\n";
        return;
    }
    bool handle_key = want_handle_key(serial, iln);
    bool is_other   = is_this_other();
    if (!ifn) biblio_buf5.push_back("; ");
    if (is_other) {
        if (iln && !ifn) {
            biblio_buf1.push_back("\\cititem{etal}{}");
            biblio_buf2.push_back("etal");
            biblio_buf4.push_back("etal");
            biblio_buf5.push_back("etal");
            if (handle_key) biblio_buf3.push_back("+");
        } else {
            biblio_buf1.push_back("\\bpers{}{}{others}{}");
            biblio_buf2.push_back("others");
            biblio_buf4.push_back("others");
            biblio_buf5.push_back("others");
            if (handle_key) biblio_buf3.push_back(iln ? "oth" : "o");
        }
        return;
    }
    if (handle_key) last_name.make_key(!(ifn && iln), biblio_buf3);
    biblio_buf1.push_back("\\bpers[");
    biblio_buf1 << first_name;
    biblio_buf1.push_back("]{");
    biblio_buf2.push_back(" ");
    biblio_buf5 << last_name << " " << jr_name << " ";
    first_name.print_first_name(biblio_buf1, biblio_buf2, biblio_buf5);
    biblio_buf1 << "}{}{" << last_name << "}{" << jr_name << "}";
    biblio_buf2 << " " << last_name << " " << jr_name << " ";
    biblio_buf4 << last_name << " " << jr_name << " ";
}

void Bchar::find_a_comma(int &first_c, int &second_c, int &howmany) const {
    for (int i = first; i < last; i++)
        if (table[i] == bct_comma) {
            if (howmany == 0) first_c = i;
            if (howmany == 1) second_c = i;
            howmany++;
        }
}

auto Bchar::find_a_space() const -> int {
    for (int i = last - 1; i >= first; i--)
        if (like_special_space(i)) return i;
    return last;
}

//
auto Bchar::find_a_lower() const -> int {
    for (int i = first; i < last - 1; i++) {
        if (!like_space(i)) continue;
        char c = '0';
        if (table[i + 1] == bct_extended) continue; // too complicated a case
        if (table[i + 1] == bct_normal) c = name_buffer[i + 1];
        if (table[i + 1] == bct_cmd) c = name_buffer[i + 3];
        if (is_lower_case(c)) return i;
    }
    return last;
}

void Bchar::invent_spaces() {
    for (int i = first; i < last; i++)
        if (table[i] == bct_normal && name_buffer.insert_space_here(i)) table[i] = bct_dot;
}

// In J.G. Grimm,only the first dot matches.
auto Buffer::insert_space_here(int k) const -> bool {
    if (k <= 0) return false;
    if (at(k) != '.') return false;
    if (!is_upper_case(at(k + 1))) return false;
    if (!is_upper_case(at(k - 1))) return false;
    return true;
}

// Returns true if character can be removed (between names)
auto Bchar::is_junk(int i) -> bool {
    bchar_type b = table[i];
    if (b == bct_comma) {
        the_bibtex->err_in_entry("misplaced comma in bibtex name\n");
        log_and_tty << "you should say \"{},{},foo\", instead of  \",,foo\" in \n" << name_buffer.c_str() << ".\n";
    }
    if (b == bct_space || b == bct_tilde || b == bct_dash || b == bct_comma) return true;
    if (b == bct_bad || b == bct_continuation) return true;
    return false;
}

// Remove initial and final junk (space, dash, tilde).
void Bchar::remove_junk() {
    if (empty()) return;
    for (int i = first; i < last; i++)
        if (is_junk(i))
            first = i + 1;
        else
            break;
    for (int j = last - 1; j >= first; j--) {
        if (table[j] == bct_continuation || table[j] == bct_bad) continue;
        if (is_junk(j))
            last = j;
        else
            break;
    }
}

auto NameSplitter::want_handle_key(int s, bool last) -> bool {
    if (s < 4) return true;
    if (s > 4) return false;
    if (last) return true;
    biblio_buf3.push_back("+");
    return false;
}

auto NameSplitter::is_this_other() -> bool {
    if (!first_name.empty()) return false;
    if (!jr_name.empty()) return false;
    int a = last_name.first;
    int b = last_name.last;
    if (b - a == 6 && strncmp(name_buffer.c_str() + a, "others", 6) == 0) return true;
    return false;
}

// We use the fact that first cannot be zero
void Bchar::make_key(bool sw, Buffer &B) {
    int        oldfirst = first;
    int        w        = first - 1;
    bchar_type old      = table[w];
    table[w]            = bct_space;
    make_key_aux(sw, B);
    table[w] = old;
    first    = oldfirst;
}

void Bchar::make_key_aux(bool sw, Buffer &B) {
    int k = 0;
    for (int i = first; i < last; i++)
        if (is_name_start(i)) k++;
    if (k >= 3 || sw) {
        while (first < last) {
            if (is_name_start(first)) { print_for_key(B); }
            first++;
        }
    } else {
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
    }
}

auto Bchar::is_name_start(int i) -> bool {
    bchar_type A = table[i - 1];
    bchar_type B = table[i];
    if (A != bct_space && A != bct_dash && A != bct_tilde) return false;
    if (B != bct_normal && B != bct_cmd && B != bct_brace) return false;
    return true;
}

auto Bchar::print_for_key(Buffer &X) -> int {
    int i = first;
    while (i < last && table[i] == bct_bad) i++;
    if (i >= last) return i;
    if (table[i] == bct_brace && is_letter(name_buffer[i + 1])) {
        X.push_back(name_buffer[i + 1]);
        i++;
        while (table[i] == bct_continuation) i++;
        return i;
    }
    if (table[i] == bct_cmd) {
        X.push_back(name_buffer[i + 2]);
        return i + 3;
    }
    return print(X);
}

auto Bchar::print(Buffer &X) -> int {
    int i = first;
    while (i < last && table[i] == bct_bad) i++;
    if (i >= last) return i;
    X.push_back(name_buffer[i]);
    i++;
    while (i < last && table[i] == bct_continuation) {
        X.push_back(name_buffer[i]);
        i++;
    }
    return i;
}

auto Bchar::special_print(Buffer &X, bool sw) -> int {
    int i = print(X);
    if (sw) X.push_back('.');
    X.no_double_dot();
    return i;
}

void Buffer::no_double_dot() {
    if (wptr > 1 && at(wptr - 2) == '.' && at(wptr - 1) == '.') {
        rrl();
        return;
    }
    if (wptr > 2 && at(wptr - 2) == '}' && at(wptr - 3) == '.' && at(wptr - 1) == '.') {
        rrl();
        return;
    }
}

void Bchar::print_first_name(Buffer &B1, Buffer &B2, Buffer &B3) {
    bool print_next = true;
    while (first < last) {
        bchar_type c = table[first];
        if (c == bct_space || c == bct_tilde || c == bct_dash || c == bct_dot) {
            int i = special_print(B1, false);
            special_print(B2, false);
            special_print(B3, false);
            print_next = true;
            first      = i;
        } else if (print_next) {
            int i = special_print(B1, true);
            special_print(B2, true);
            special_print(B3, true);
            print_next = false;
            first      = i;
        } else
            first++;
    }
}

void BibEntry::normalise() {
    handle_one_namelist(all_fields[fp_author], author_data);
    handle_one_namelist(all_fields[fp_editor], editor_data);
    std::string y = all_fields[fp_year];
    int         n = y.size();
    if (n == 0) return;
    cur_year = -1;
    int res  = 0;
    for (int i = 0; i < n; i++) {
        char c = y[i];
        if (!is_digit(c)) return;
        res = res * 10 + (c - '0');
        if (res > 10000) return;
    }
    if (n == 2) {
        if (res > 50) {
            res += 1900;
            all_fields[fp_year] = "19" + y;
        } else {
            res += 2000;
            all_fields[fp_year] = "20" + y;
        }
    }
    cur_year = res;
}

void Buffer::remove_spec_chars(bool url, Buffer &B) {
    reset_ptr();
    B.reset();
    for (;;) {
        unsigned char c = uhead();
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
        c = uhead();
        advance();
        if (c == '{' || c == '}') {
            B.push_back(c);
            continue;
        }
        if (c == '|') {
            B.push_back("\\vbar ");
            continue;
        } // bar is special
        if (c == 's' && uhead() == 'p' && !is_letter(after_uhead())) {
            advance();
            B.push_back('^');
            continue;
        }
        if (c == 's' && uhead() == 'b' && !is_letter(after_uhead())) {
            advance();
            B.push_back('_');
            continue;
        }
        if (is_accent_char(c)) {
            unsigned char C = uhead();
            if (C == '{') {
                advance();
                C = uhead();
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

// Faire un hack: s'il y a un extrabib, et normal_biblio
// le rajouter a la fin, et virer le extrabib
// Plus le parser propement

void Bibtex::read(String src, bib_from ct) {
    bbl.push_back("% reading source ");
    bbl.push_back(src);
    bbl.newline();
    entry_prefix  = ct;
    normal_biblio = ct == from_year ? true : false;
    refer_biblio  = ct == from_refer ? true : false;
    tralics_ns::read_a_file(in_lines, src, 1);
    interactive = false;
    parse_a_file();
    if (bbl.non_empty_buf()) // the buffer contains a preamble
    {
        bbl.push_back("%");
        bbl.newline();
    }
}

/// -------------------------   a virer

void bib_ns::handle_special_string(const std::string &s, Buffer &A, Buffer &B) {
    if (s.empty()) {
        B.reset();
        return;
    }
    A << bf_reset << s;
    A.remove_spec_chars(false, B);
}

// ----------------------------------------
// Boot strapping the system

void tralics_ns::bibtex_boot(String b, String dy, std::string no_year, bool inra, bool db) {
    distinguish_refer = db;
    bbl.install_file(b);
    the_bibtex = new Bibtex(dy);
    the_bibtex->boot(std::move(no_year), inra);
}

void tralics_ns::bibtex_bootagain() { the_bibtex->bootagain(); }

void tralics_ns::bibtex_set_nocite() { the_bibliography.set_nocite(); }

void tralics_ns::bibtex_insert_jobname() {
    if (the_bibliography.number_of_data_bases() == 0) { the_bibliography.push_back_src(tralics_ns::get_short_jobname().c_str()); }
}

void Bibtex::boot(std::string S, bool inra) {
    no_year      = std::move(S);
    in_ra        = inra;
    want_numeric = false;
    if (the_main->in_ra()) want_numeric = true;
    for (auto &id_clas : id_class) id_clas = legal_id_char;
    for (int i = 0; i < 32; i++) id_class[i] = illegal_id_char;
    id_class[(unsigned char)' ']  = illegal_id_char;
    id_class[(unsigned char)'\t'] = illegal_id_char;
    id_class[(unsigned char)'"']  = illegal_id_char;
    id_class[(unsigned char)'#']  = illegal_id_char;
    id_class[(unsigned char)'%']  = illegal_id_char;
    id_class[(unsigned char)'\''] = illegal_id_char;
    id_class[(unsigned char)'(']  = illegal_id_char;
    id_class[(unsigned char)')']  = illegal_id_char;
    id_class[(unsigned char)',']  = illegal_id_char;
    id_class[(unsigned char)'=']  = illegal_id_char;
    id_class[(unsigned char)'{']  = illegal_id_char;
    id_class[(unsigned char)'}']  = illegal_id_char;
    bootagain();
}

void Bibtex::bootagain() {
    old_ra = the_parser.get_ra_year() < 2006;

    if (the_parser.cur_lang_fr()) { // french
        define_a_macro("jan", "janvier");
        define_a_macro("feb", "f\\'evrier");
        define_a_macro("mar", "mars");
        define_a_macro("apr", "avril");
        define_a_macro("may", "mai");
        define_a_macro("jun", "juin");
        define_a_macro("jul", "juillet");
        define_a_macro("aug", "ao\\^ut");
        define_a_macro("sep", "septembre");
        define_a_macro("oct", "octobre");
        define_a_macro("nov", "novembre");
        define_a_macro("dec", "d\\'ecembre");
        my_constant_table[0] = "th\\`ese de doctorat";
        my_constant_table[1] = "rapport technique";
        my_constant_table[2] = "m\\'emoire";
    } else if (the_parser.cur_lang_german()) { //	      german
        define_a_macro("jan", "Januar");
        define_a_macro("feb", "Februar");
        define_a_macro("mar", "M\\\"arz");
        define_a_macro("apr", "April");
        define_a_macro("may", "Mai");
        define_a_macro("jun", "Juni");
        define_a_macro("jul", "Juli");
        define_a_macro("aug", "August");
        define_a_macro("sep", "September");
        define_a_macro("oct", "Oktober");
        define_a_macro("nov", "November");
        define_a_macro("dec", "Dezember");
        my_constant_table[0] = "Ph. D. Thesis";
        my_constant_table[1] = "Technical report";
        my_constant_table[2] = "Masters thesis";
    } else { // non french, assume english
        define_a_macro("jan", "January");
        define_a_macro("feb", "February");
        define_a_macro("mar", "March");
        define_a_macro("apr", "April");
        define_a_macro("may", "May");
        define_a_macro("jun", "June");
        define_a_macro("jul", "July");
        define_a_macro("aug", "August");
        define_a_macro("sep", "September");
        define_a_macro("oct", "October");
        define_a_macro("nov", "November");
        define_a_macro("dec", "December");
        my_constant_table[0] = "Ph. D. Thesis";
        my_constant_table[1] = "Technical report";
        my_constant_table[2] = "Masters thesis";
    }
}
