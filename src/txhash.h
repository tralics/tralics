#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txeqtb.h"

// This is the main hash table. If a token like \foo has hashcode p,
// then foo is in Text[p], or in Text[Next[p]] or in Text[Next[Next[p]]] ...
// The data structure holds the location of tokens like \par
// that are known only after bootstrap
class Hashtab {
public:
    Token par_token, OB_token, CB_token, dollar_token;
    Token verb_token, noindent_token, small_token, textvisiblespace_token;
    Token end_token, frozen_dont_expand, relax_token, frozen_relax_token, sendgroup_token, textbf_token, frozen_endcsname, begin_token,
        eof_token;
    Token frozen_undef_token, equals_token, char_token;
    Token number_token, roman_numeral_token, Roman_numeral_token;
    Token advance_token, global_token, atalph_token, atAlph_token, fnsymbol_token;
    Token roman_token, Roman_token, alph_token, Alph_token;
    Token iftrue_token, iffalse_token, let_token, urlfont_token;
    Token fi_token, iterate_token, repeat_token, expandafter_token, endcsname_token;
    Token csname_token, xkv_cc_token;
    Token isodd_token, lengthtest_token, boolean_token, equal_token;
    Token or_token, and_token, not_token, ratio_token, real_token, calc_token;
    Token OR_token, AND_token, NOT_token, isundefined_token;
    Token frozen_protection, prime_token, elt_token, killcounter_token;
    Token math_OB_token, math_CB_token, text_OB_token, text_CB_token;
    Token real_end_token, error_token, tt_token, dagger_token, ddagger_token;
    Token ensuremath_token, vbar_token, atbd_token, notprerr_token;
    Token hat_token, zero_token, one_token, comma_token, dot_token, star_token;
    Token plus_token, minus_token, tilda_token, space_token, newline_token;
    Token verbatim_number_font, verbatim_font, temp_token, unitlength_token;
    Token verb_prefix_token, verbatim_prefix_token, citeone_token, citesimple_token;
    Token end_natcite_token, at_one_token, at_zero_token, at_align_token;
    Token alt_rapers_token, alt_pers_token, crwithargs_token, of_token;
    Token ifdim_token, ifnum_token, iwhilenum_token, iwhiledim_token, iwhilesw_token, do_token, bth_token, eth_token, arabic_token,
        gobble_token, gobbletwo_token, else_token, endv_token, cr_token, refstepcounter_token, hspace_token, footcite_sep_token,
        footcite_pre_token, cite_punct_token, operatorname_token, itemlabel_token, vspace_token;
    Token xscale_token, yscale_token, xscaley_token, yscalex_token, refname_token;
    Token cite_type_token, cite_prenote_token, lbrace_token, rbrace_token;
    Token backslash_token, headercr_token, mathversion_token, tag_token, qquad_token, xtag_token, ytag_token, xtag1_token, ytag1_token,
        apostrophe_token, cite_e_token, cite_i_token, ref_i_token, label_i_token, ensuremath_i_token, nocase_e_token, nocase_i_token,
        ref_token, label_token, infalse_token, intrue_token, nil_token, CurrentOption_token, xkv_for_i_token, CurrentOptionKey_token,
        CurrentOptionValue_token, forloop_token, doubleat_token, def_token, iforloop_token, tforloop_token;
    Token footnote_token, dblarg_token;
    Token mmlprescripts_token, mmlmultiscripts_token, mmlnone_token, mathop_token, limits_token, begingroup_token, endgroup_token,
        allowbreak_token;
    Token increqnum_token, makelabel_token, stepcounter_token;
    Token nomathsw0_token, nomathsw1_token;
    Token xkv_resa_token, xkv_warn_token, xkv_prefix_token, xkv_fams_token, xkv_na_token, xkv_rm_token, xkv_tfam_token, usevalue_token,
        xkv_header_token, xkv_tkey_token, gsavevalue_token, savevalue_token, empty_token, composite_token;
    Token ExplFileName_token, ExplFileDate_token, ExplFileVersion_token, ExplFileDescription_token;
    Token last_tok;

    std::array<Token, 15>             my_mathfont_table;
    std::array<Token, 5>              genfrac_mode;
    std::array<Equivalent, eqtb_size> eqtb;

private:
    std::array<String, hash_size> Text;       // the strings
    std::array<int, hash_size>    Next;       // points to next
    Buffer                        B;          // internal buffer
    int                           hash_used;  // all places above this one are used
    int                           hash_usage; // number of commands in the table
    int                           hash_bad;   // number of items not at hash position
private:
    auto find_empty(String s) -> int; // find an empty slot
    auto find_aux(int p, String name) -> int;

public:
    Hashtab();
    auto               my_buffer() -> Buffer & { return B; }
    auto               locate(String s) -> Token;             // used by primitive, etc
    auto               locate(const std::string &s) -> Token; // used by primitive, etc
    auto               locate(const Buffer &b) -> Token;      // used by primitive, etc
    [[nodiscard]] auto get_hash_usage() const -> int { return hash_usage; }
    [[nodiscard]] auto get_hash_bad() const -> int { return hash_bad; }
    auto               hash_find(const Buffer &b, String name) -> int;
    auto               hash_find() -> int;
    auto               primitive(String s, symcodes c, subtypes v = zero_code) -> Token;
    auto               nohash_primitive(String a, CmdChr b) -> Token;
    void               eval_let(String a, String b);
    auto               eval_letv(String a, String b) -> Token;
    void               eval_let_local(String a, String b);
    auto               operator[](size_t k) const { return Text[k]; }
    void               dump();
    void               boot_fancyhdr();
    void               boot_etex();
    void               load_latex3();
    void               boot_keyval();
    auto               is_defined(const Buffer &b) -> bool;
};

// This is an association table. We could use a standard C++ class here
class SpecialHash {
    size_t                   size;  // number of terms
    std::vector<std::string> key;   // the keys
    std::vector<std::string> value; // the values
public:
    SpecialHash(const std::string &s) : size(0) { create(s.c_str()); }
    void               create(String s);
    auto               find(String x) const -> std::string;
    [[nodiscard]] auto get_size() const { return size; }
    void               get_pair(size_t k, std::string &a, std::string &b) {
        if (k < size) {
            a = key[k];
            b = value[k];
        } else {
            a = "";
            b = "";
        }
    }
    auto        find_true_false(String s) -> int;
    static auto counter_val(int k) -> int;
    auto        find_counter() -> int;
};

// Data structure for label and references.
class LabelInfo {
    bool        used{false};    // is this ID used ?
    bool        defined{false}; // is this ID defined ?
    Istring     id;             // value of the ID
    Istring     name;           // name of the ID (pointer into SH)
    int         lineno{0};      // line of definition
    std::string file_name;      // file of definition
public:
    LabelInfo() : file_name("") {}
    LabelInfo(Istring k) : name(k), file_name("") {}
    [[nodiscard]] auto is_used() const -> bool { return used; }
    [[nodiscard]] auto is_defined() const -> bool { return defined; }
    auto               set_used() -> bool {
        if (used) return true;
        used = true;
        return false;
    }
    auto set_defined() -> bool {
        if (defined) return true;
        defined = true;
        return false;
    }
    void               set_undefined() { defined = false; }
    [[nodiscard]] auto get_id() const -> Istring { return id; }
    [[nodiscard]] auto get_name() const -> Istring { return name; }
    void               set_id(Istring i) { id = i; }
    [[nodiscard]] auto get_lineno() const -> int { return lineno; }
    [[nodiscard]] auto get_filename() const -> std::string { return file_name; }
    void               set_lineno(int i) { lineno = i; }
    void               set_filename(std::string i) { file_name = std::move(i); }
};

// This class returns foo then bar then gee from `foo,bar,gee'
class Splitter {
    std::string S;   // the string to split
    size_t      pos; // current position
    size_t      size;

public:
    Splitter(std::string w) : S(std::move(w)), pos(0) { size = S.size(); }
    [[nodiscard]] auto at_end() const -> bool { return pos == size; }
    [[nodiscard]] auto count() const -> int;
    auto               get_next_raw() -> String;
    auto               get_next() -> String;
    void               extract_keyval(std::string &key, std::string &val);
};

// This is used in order to extract things from \documentclass[]{}
// and \usepackage[]{}
// Not used anymore
class PackMatcher {
    std::string attrib;           // attribute to set
    std::string value;            // value of attribute
    std::string package;          // package name
    std::string options;          // option
    bool        wild_pack{false}; // wild card in package
    bool        att_star{false};  // one star in attribute
    bool        match_document;   // usepackage? or \documentclass ?
    bool        att_plus{false};  //
    bool        used{false};      // did we use this rule ?
public:
    PackMatcher(bool c = false) : match_document(c) {}
    auto ok_for_match(bool c) -> bool { return c == match_document && !used; }
    void set_match_doc(bool X) { match_document = X; }
    auto set_wild_pack(bool X) -> int {
        wild_pack = X;
        return X ? 1 : 0;
    }
    void set_package(std::string X) { package = std::move(X); }
    void set_options(std::string X) { options = std::move(X); }
    void print();
    auto get_attrib() -> std::string { return attrib; }
    auto find_att(std::string cur_pack, std::string cur_options) -> bool;
    auto find_att_val(std::string cur_pack, std::string cur_options) -> Istring;
    void initialise(std::string A, std::string B);
};
