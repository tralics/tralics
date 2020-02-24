#include <utility>

#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This is the unique identifier of a bibliography element
// from the bibtex point of view. If used, a unique id (an integer will be
// computed). For the moment being it has a cite key, and a prefix.
class CitationKey {
    string   cite_key;         // cite key, say Foo
    string   lower_cite_key;   // cite key, lower case, say foo
    bib_from cite_prefix;      // symbolic prefix, say from_foot
    string   full_key;         // something like footcite:Foo
    void     make_key(String); // creates the key.
public:
    [[nodiscard]] auto is_similar(const CitationKey &w) const -> bool { return cite_key == w.cite_key; }
    [[nodiscard]] auto is_similar_lower(const CitationKey &w) const -> bool { return lower_cite_key == w.lower_cite_key; }
    [[nodiscard]] auto is_same(const CitationKey &w) const -> bool { return is_similar(w) && cite_prefix == w.cite_prefix; }
    [[nodiscard]] auto is_same_old(const CitationKey &w) const -> bool { return full_key == w.full_key; }
    [[nodiscard]] auto is_same_lower(const CitationKey &w) const -> bool { return is_similar_lower(w) && cite_prefix == w.cite_prefix; }
    [[nodiscard]] auto is_same_lower_old(const CitationKey &w) const -> bool;
    CitationKey() = default;
    CitationKey(String, String);
    CitationKey(bib_from, String);
    [[nodiscard]] auto get_name() const -> string { return full_key; }
    [[nodiscard]] auto get_from() const -> bib_from { return cite_prefix; }
    [[nodiscard]] auto from_to_string() const -> String;
    void               move_to_year() { cite_prefix = from_year; } // leaves full_key unchanged
    void               move_to_refer() { cite_prefix = from_refer; }
};

class CitationItem {
    Istring key;
    Istring from;
    Istring bid;
    Xid     solved;

public:
    auto               get_bid() -> Istring;
    void               set_id(Istring x) { bid = x; }
    auto               has_empty_id() -> bool { return bid.empty(); }
    void               set_solved(Xid N) { solved = N; }
    void               dump(Buffer &);
    void               dump_bibtex();
    auto               match(Istring, Istring) -> bool;
    auto               match_star(Istring) -> bool;
    [[nodiscard]] auto is_solved() const -> bool { return solved.value != 0; }
    CitationItem(Istring A, Istring B) : key(A), from(B), bid(""), solved(Xid()) {}
};

class Bibliography {
private:
    string         bib_style;               // The bibliography style
    string         bib_cmd;                 // The bibliography command
    Xml *          biblio_loc{nullptr};     // location of biblio in the XML tree
    vector<string> biblio_src;              // database file names
    bool           nocite{false};           // have  we seen \nocite* ?
    bool           biblio_loc_force{false}; // force location
    int            last_bid{-1};            // current number for unique_bid
public:
    vector<CitationItem> citation_table;
    Bibliography();

public:
    void               dump(Buffer &);
    void               dump_bibtex();
    void               dump_data(Buffer &);
    auto               get_bid(int n) -> Istring { return citation_table[n].get_bid(); }
    auto               find_citation_item(Istring, Istring, bool) -> int;
    auto               find_citation_star(Istring, Istring) -> int;
    [[nodiscard]] auto get_cmd() const -> string { return bib_cmd; }
    [[nodiscard]] auto get_location() const -> Xml * { return biblio_loc; }
    [[nodiscard]] auto has_cmd() const -> bool { return !bib_cmd.empty(); }
    [[nodiscard]] auto location_exists() const -> bool { return biblio_loc_force; }
    auto               number_of_data_bases() -> int { return biblio_src.size(); }
    void               push_back_src(String x) { biblio_src.emplace_back(x); }
    [[nodiscard]] auto seen_nocite() const -> bool { return nocite; }
    void               set_cmd(string x) { bib_cmd = std::move(x); }
    void               set_location(Xml *X, bool f) {
        biblio_loc       = X;
        biblio_loc_force = f;
    }
    void set_nocite() { nocite = true; }
    void set_style(string x) { bib_style = std::move(x); }
    void stats();
    auto unique_bid() -> Istring;
};

// A bibtex macro, like @string(foo="bar")
class BibMacro {
private:
    int    h;     // hash code of the name
    string name;  // the name of the name (e.g. foo)
    string value; // the value of the macro (e.g. bar)
public:
    auto is_same(int hash, String s) -> bool { return hash == h && name == s; }
    void set_value(string v) { value = std::move(v); }
    auto get_value() -> string { return value; }
    void set_default_value() { value = name; }
    BibMacro() = default;
    BibMacro(int hash, Buffer &s1) : h(hash), name(s1.to_string()) {}
    BibMacro(int hash, String &s1, String s2) : h(hash), name(s1), value(s2) {}
};

// Consider for instance the name list :
// Carrasco, J. and MahEEvas, S. and Rubino, G. and Su\~nEE, V.
// (where EE stands for \'e on 8 bits)
// value will be \bpers{}\bpers{}\bpers{}\bpers (concatenation of 4 value)
// long_key will be the concatenation of 4?
// short_key will be CMRS
class BibtexName {
public:
    string value;
    string long_key;
    string short_key;
    string name_key;
    string cnrs_key;
};

class Bchar {
public:
    int         first; // index of first char
    int         last;  // index of one-after-last char
    bchar_type *table;

public:
    void init(int i, int j) {
        first = i;
        last  = j;
    }
    void init(bchar_type *T) {
        first = 0;
        last  = 0;
        table = T;
    }
    [[nodiscard]] auto empty() const -> bool { return first >= last; }
    void               print_first_name(Buffer &, Buffer &, Buffer &);
    void               make_key(bool, Buffer &);
    void               remove_junk();
    [[nodiscard]] auto find_a_lower() const -> int;
    [[nodiscard]] auto find_a_space() const -> int;
    void               invent_spaces();
    void               find_a_comma(int &, int &, int &) const;

private:
    void               make_key_aux(bool, Buffer &);
    auto               is_junk(int i) -> bool;
    [[nodiscard]] auto like_space(int i) const -> bool {
        bchar_type T = table[i];
        return T == bct_space || T == bct_tilde || T == bct_dash;
    }
    [[nodiscard]] auto like_special_space(int i) const -> bool {
        bchar_type T = table[i];
        return T == bct_space || T == bct_tilde || T == bct_dot;
    }
    [[nodiscard]] auto is_printable() const -> bool {
        bchar_type b = table[first];
        return b == bct_normal || b == bct_cmd || b == bct_brace || b == bct_extended;
    }
    auto is_name_start(int) -> bool;
    auto print_for_key(Buffer &) -> int;
    auto special_print(Buffer &, bool) -> int;
    auto print(Buffer &) -> int;
};

// In the case of a name like  `Carrasco, J.', num_tokens will be 2,
// num_commas will be 1. von_start, etc are token numbers.
// Here first_start is comma1, and is one (it's the second token).
class NameSplitter {
    bchar_type *table;
    Bchar       first_name;
    Bchar       last_name;
    Bchar       jr_name;
    Bchar       main_data;

public:
    NameSplitter(bchar_type *T) : table(T) {}
    void handle_the_names();
    void handle_one_name(bool ifn, bool iln, int serial);
    auto want_handle_key(int, bool) -> bool;
    auto is_this_other() -> bool;
};

// A bibliographic entry
class BibEntry {
    friend class Bibtex;

    BibEntry *  crossref{nullptr};      // In case of a crossref
    BibEntry *  crossref_from{nullptr}; // reverse crossref
    entry_type  type_int{type_unknown}; // the type of the entry
    CitationKey cite_key;               // the cite_key structure
    bib_creator why_me;                 // reason why this entry is considered
    string      all_fields[fp_unknown]; // all the fields
    BibtexName  author_data;            // already processed author data
    BibtexName  editor_data;            // already processed editor data

    string       label, sort_label, aux_label; // cite label and sort label
    int          extra_num{0};                 // extra char added to the label
    int          cur_year{0};                  // current year, if year field can be parsed
    string       lab1, lab2, lab3;             // two temporaries.
    int          id{0};
    Istring      unique_id;
    bool         explicit_cit{false};
    c_primaire   main_c_type;
    c_secondaire second_c_type;
    int          first_line{-1};
    string *     user_fields;
    int          is_extension;

public:
    BibEntry();
    [[nodiscard]] auto get_unique_id() const -> Istring { return unique_id; }
    [[nodiscard]] auto Sort_label() const -> const string & { return sort_label; }

private:
    void               find_cnrs_type(Buffer &);
    [[nodiscard]] auto from_to_string() const -> String { return cite_key.from_to_string(); };
    void               output_bibitem();
    void               out_something(field_pos p);
    void               out_something(field_pos p, string s);
    void               out_something(field_pos p, int w);
    void               set_explicit_cit() { explicit_cit = true; }
    [[nodiscard]] auto get_from() const -> bib_from { return cite_key.get_from(); }
    [[nodiscard]] auto get_name() const -> string { return cite_key.get_name(); }
    auto               is_empty(String s) -> bool;
    void               move_to_year() { cite_key.move_to_year(); }
    void               use_extra_num();
    void               numeric_label(int);
    void               call_type();
    void               call_type_all();
    void               call_type_special();
    void               format_series_etc(bool);
    void               sort_author(bool);
    void               sort_editor();
    void               sort_organization();
    void               sort_check(String);
    void               presort(int);
    void               sort_key();
    void               format_author(bool);
    void               forward_pass(string &previous_label, int &last_num);
    void               reverse_pass(int &next_extra);
    auto               find_all_names(String) -> BibtexName *;
    auto               format_lab_names(String s) -> String;
    auto               sort_format_names(String s) -> String;
    void               normalise();
    [[nodiscard]] auto ra_prefix() const -> String;
    void               handle_one_namelist(string &s, BibtexName &X);
    void               un_crossref();
    void               copy_from(BibEntry *);
    void               copy_from(BibEntry *, int);
    void               normalise_statut(Buffer &);
    void               one_cnrs_aux(Buffer &A, bool &nf, field_pos p, String aux);
    void               add_warning(int);
    auto               store_field(field_pos) -> bool;
    void               parse_crossref();
    void               work(int);
};

class Berror {};

class Bibtex {
private:
    Buffer             inbuf;          // contains a line of the bib file
    vector<codepoint>  input_line;     // line as Utf8Chars
    uint               input_line_pos; // position in input_line
    Buffer             token_buf;
    LinePtr            in_lines;     // contains the bibfile
    String             src_name;     // name of the bibfile
    int                cur_bib_line; // current line number
    int                last_ok_line; // start of current entry
    char               right_outer_delim;
    vector<BibMacro>   all_macros;
    vector<BibEntry *> all_entries;       // potential entries
    vector<BibEntry *> all_entries_table; // real entries
    vector<string>     user_model;
    bib_from           entry_prefix;
    bool               nocitestar;
    bool               normal_biblio;
    bool               refer_biblio;
    bool               in_ra;
    String             default_year;
    bool               want_numeric;
    string             cur_field_name;
    string             no_year;
    bool               noyearerror;
    bool               interactive;
    id_type            id_class[128];

public:
    Bibtex(String dy) : input_line_pos(0), nocitestar(false), normal_biblio(true), default_year(dy), interactive(false) {}
    auto               find_entry(const CitationKey &s) -> BibEntry *;
    auto               find_entry(String, const string &, bib_creator) -> BibEntry *;
    auto               find_entry(String, bool, bib_creator) -> BibEntry *;
    auto               make_new_entry(const CitationKey &a, bib_creator b) -> BibEntry *;
    void               make_entry(const CitationKey &, Istring);
    auto               auto_cite() -> bool;
    [[nodiscard]] auto default_prefix() const -> bib_from { return entry_prefix; }

private:
    [[nodiscard]] auto at_eol() const -> bool { return input_line_pos >= input_line.size(); }
    void               advance() { input_line_pos++; }
    auto               check_val_end() -> int;
    auto               check_entry_end() -> int;
    auto               check_entry_end(int) -> int;
    auto               check_field_end(int) -> int;
    auto               cur_char() -> codepoint { return input_line[input_line_pos]; }
    void               define_a_macro(String name, String value);
    auto               find_a_macro(Buffer &name, bool insert, String, String) -> int;
    auto               find_lower_case(const CitationKey &s, int &n) -> BibEntry *;
    auto               find_similar(const CitationKey &s, int &n) -> BibEntry *;
    void               forward_pass();
    auto               get_class(codepoint c) -> id_type { return id_class[c.value]; }
    void               handle_multiple_entries(BibEntry *);
    void               kill_the_lists();
    auto               look_at_macro(const Buffer &) -> int;
    auto               look_at_macro(int h, String name) -> int;
    void               mac_def_val(int X) { all_macros[X].set_default_value(); }
    void               mac_set_val(int X, string s) { all_macros[X].set_value(std::move(s)); }
    auto               make_entry(const CitationKey &a, bib_creator, Istring) -> BibEntry *;
    auto               next_char() -> codepoint { return input_line[input_line_pos++]; }
    void               next_line(bool);
    auto               not_start_or_end(int what, char c, bool s) -> bool;
    void               parse_a_file();
    void               parse_one_item();
    void               parse_one_field(BibEntry *X);
    void               read_one_field(bool);
    void               read_field(bool);
    auto               read2(bib_from pre) -> bool;
    void               reset_input() { input_line.clear(); }
    void               reverse_pass();
    void               scan_for_at();
    auto               scan_identifier(int) -> bool;
    auto               scan_identifier0(int) -> int;
    auto               see_new_entry(entry_type, int) -> BibEntry *;
    void               skip_space();
    auto               wrong_first_char(codepoint, int) -> int;

public:
    auto               get_an_entry(int i) -> BibEntry * { return all_entries[i]; }
    auto               find_field_pos(String s) const -> field_pos;
    auto               find_type(String s) -> entry_type;
    auto               exec_bibitem(const string &, const string &) -> Istring;
    void               nocitestar_true() { nocitestar = true; }
    auto               implement_cit(String x, string w) -> int;
    auto               is_year_string(const string &y, bib_from from) -> String;
    auto               wrong_class(int, const string &y, bib_from from) -> bool;
    void               work();
    void               read(String src, bib_from);
    auto               read0(Buffer &B, bib_from) -> bool;
    void               read1(const string &cur);
    void               read_ra();
    void               err_in_file(String s, bool last);
    void               err_in_entry(String a);
    void               err_in_name(String a, int i);
    void               bad_year(const string &given, String wanted);
    void               boot(string, bool);
    void               bootagain();
    [[nodiscard]] auto is_in_ra() const -> bool { return in_ra; }
    void               enter_in_table(BibEntry *x) { all_entries_table.push_back(x); }
};

class Bbl {
private:
    Buffer   B;
    fstream *file;
    string   name;
    LinePtr  ptr;
    bool     too_late;

public:
    friend class BibEntry;
    friend class BblAndTty;
    Bbl() {
        file     = nullptr;
        too_late = false;
    }
    auto get_lines() -> LinePtr & { return ptr; }

    void newline();
    void push_back(String s) { B.push_back(s); }
    void push_back(const string &s) { B.push_back(s); }
    void push_back_braced(String s) { B.push_back_braced(s); }
    void push_back_braced(string s) { B.push_back_braced(std::move(s)); }
    void push_back_cmd(String s) {
        B.push_back('\\');
        B.push_back(s);
    }
    auto non_empty_buf() -> bool { return !B.empty(); }
    void reset() { B.reset(); }
    void reset_lines() { ptr.clear(); }
    auto is_too_late() -> bool { return too_late; }
    void finish() {
        file->close();
        delete file;
        file     = nullptr;
        too_late = true;
    }
    void open() {
        if (file == nullptr) file = tralics_ns::open_file(name, true);
    }

    void install_file(String b) { name = b; }
};

class BblAndTty {
public:
    Bbl *  X;
    Buffer lb;
    void   init() { lb.reset(); }
    void   out_bar() {
        cout << "|";
        *(X->file) << "|";
    }
    void out_buffer(Buffer &B) {
        cout << B;
        *(X->file) << B;
    }
    void out_field(Buffer &B) {
        out_buffer(B);
        out_bar();
    }
    void out_field(String B) {
        cout << B;
        *(X->file) << B;
        out_bar();
    }
    void out_nl() {
        cout << "\n";
        *(X->file) << "\n";
    }
    void out_string(const string &s) {
        cout << s;
        *(X->file) << s;
    }
    void out_string(int s) {
        cout << s;
        *(X->file) << s;
    }
    void out_string(char s) {
        cout << s;
        *(X->file) << s;
    }
};
