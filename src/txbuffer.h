#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This is the include file for the Buffer data structure

#include "txid.h"
#include "txscaled.h"
#include <cstring>
#include <vector>

using buffer_fn = void(Buffer &);

/// a big structure \todo This is kind of a messy class, would be better off
/// using `std::string` as much as possible but we can't because of all the
/// zero-char manipulations.

class Buffer : public std::vector<char> {
public:
    size_t wptr{0}; // the write pointer
    size_t ptr{0};  // the read pointer
    size_t ptr1{0}; // a second read pointer

    Buffer() : std::vector<char>(128, 0){};

    auto operator<<(String s) -> Buffer & {
        push_back(s);
        return *this;
    }
    auto operator<<(char c) -> Buffer & {
        push_back(c);
        return *this;
    }
    auto operator<<(uchar c) -> Buffer & {
        push_back(static_cast<char>(c));
        return *this;
    }
    auto operator<<(int c) -> Buffer & {
        push_back_int(c);
        return *this;
    }
    auto operator<<(long c) -> Buffer & {
        push_back_int(c);
        return *this;
    }
    auto operator<<(const TokenList &L) -> Buffer &;
    auto operator<<(const Macro &x) -> Buffer &;
    auto operator<<(const codepoint &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(const Buffer &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(Token b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(const std::string &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(const Istring &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(const SthInternal &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(const Glue &b) -> Buffer & {
        push_back(b);
        return *this;
    }
    auto operator<<(buffer_fn f) -> Buffer & {
        f(*this);
        return *this;
    }
    auto               add_with_space(String s) -> std::string;
    void               advance() { ptr++; }
    void               advance(size_t k) { ptr += k; }
    void               alloc(size_t n);
    [[nodiscard]] auto after_head() const -> char { return at(ptr + 1); }
    [[nodiscard]] auto after_uhead() const -> char { return at(ptr + 1); }
    [[nodiscard]] auto after_after_uhead() const -> char { return at(ptr + 2); }
    auto               append_checked_line(LinePtr &) -> int;
    [[nodiscard]] auto at_eol() const -> bool { return wptr <= ptr; }
    auto               backup_space() -> bool;
    void               bib_spec();
    void               brace_match();
    void               bracket_match();
    [[nodiscard]] auto c_str(size_t k) const -> String { return data() + k; }
    [[nodiscard]] auto c_str() const -> String { return data(); }
    void               chars_to_buffer(Buffer &);
    void               check_before_brace(String);
    auto               check_cat_perso(int, int, bool) -> std::string;
    auto               contains(String s) const -> bool { return strstr(data(), s) != nullptr; }
    auto               contains_braced(String s) -> bool;
    auto               contains_env(String env) -> bool;
    auto               contains_here(String s) const -> bool;
    [[nodiscard]] auto convert(int k) const -> std::string;
    auto               convert_dim(int, int) -> std::string;
    [[nodiscard]] auto convert_to_str() const -> String;
    auto               convert_to_out_encoding(String a) const -> String;
    [[nodiscard]] auto convert_to_log_encoding() const -> String;
    [[nodiscard]] auto convert_to_latin1(bool nonascii) const -> String;
    void               convert_custom(int l);
    auto               convert_line0(size_t wc) -> bool;
    void               convert_line(int l, size_t wc);
    void               copy(const Buffer &B);
    void               decr_wptr() { wptr--; };
    auto               double_hat_aux(int) -> bool;
    void               dump_prefix(bool err, bool gbl, symcodes K);
    [[nodiscard]] auto empty() const -> bool { return size() == 0; }
    void               extract_cnrs_info();
    void               extract_dtd(String a, std::string &b, std::string &c);
    void               extract_strs(Buffer &A, Buffer &B);
    void               extract_chars(vector<codepoint> &V);
    auto               fetch_beg_end_spec(bool k, bool incat, int &com_loc, bool &seen_dollar, String &) -> int;
    auto               fetch_citation(String, String &a, String &b) -> bool;
    auto               fetch_spec_arg() -> bool;
    auto               figline(int &ctr, std::string &junk) -> std::string;
    void               figtable(String start, int id);
    void               fill_table(bchar_type *table);
    auto               find_alias(const vector<std::string> &SL, std::string &res) -> bool;
    auto               find_and(const bchar_type *table) -> bool;
    void               find_bibtex_name_token(int);
    auto               find_brace() -> int;
    auto               find_bracket() -> int;
    auto               find_configuration(Buffer &aux) -> bool;
    auto               find_doctype() -> size_t;
    auto               find_documentclass(Buffer &aux) -> bool;
    auto               find_equals() -> bool;
    auto               find_one_bibtex_name() -> String;
    void               find_one_type(vector<std::string> &S);
    void               find_top_atts();
    void               finish_figure(String start, std::string junk, int w);
    void               finish_xml_print();
    auto               full_bracket_match() -> bool;
    auto               full_brace_match() -> bool;
    [[nodiscard]] auto get_int_val() const -> int;
    auto               get_machine_name() -> std::string;
    [[nodiscard]] auto hashcode(size_t prime) const -> size_t;
    [[nodiscard]] auto head() const -> char { return at(ptr); }
    auto               is_not_char(size_t p, uchar x) -> bool { return uchar(at(p)) != x; }
    auto               holds_documentclass(Buffer &a, Buffer &b, Buffer &c) -> int;
    auto               holds_env(String &a, String &b, String &c) -> int;
    auto               horner(size_t p) -> unsigned int;
    auto               how_many_bibtex_name_token() -> int;
    void               kill_at(size_t p) { at(p) = 0; }
    void               init_from_buffer(Buffer &b);
    void               insert_and_kill(Buffer &a) {
        reset();
        push_back(a.data());
        a.reset();
    }
    void               insert_escape_char();
    void               insert_escape_char_raw();
    auto               insert_fp(const FpNum &X) -> String;
    [[nodiscard]] auto insert_space_here(size_t k) const -> bool;
    void               insert_string(const Buffer &s);
    auto               insert_break(const std::string &x) -> std::string;
    void               insert_token0(Token c);
    auto               install_att(Xid idx, Istring match) -> bool;
    void               interpret_aux(vector<Istring> &bib, vector<Istring> &bib2);
    void               interpret_bibtex_list();
    void               interpret_bibtex_extension_list();
    auto               is_and(size_t k) -> bool;
    [[nodiscard]] auto is_begin_end() const -> int;
    auto               is_begin_something(String s) -> int;
    auto               is_begin_something(String, bool) -> int;
    auto               is_equal(String x) const -> bool { return strcmp(data(), x) == 0; }
    auto               is_at_end(String s) const -> bool;
    auto               is_here(String s) -> bool;
    auto               is_here_case(String s) -> bool;
    [[nodiscard]] auto is_letter_digit() const -> bool;
    [[nodiscard]] auto is_all_ascii() const -> bool;
    [[nodiscard]] auto is_good_ascii() const -> bool;
    [[nodiscard]] auto is_spaceh(size_t j) const -> bool { return is_space(at(j)); }
    [[nodiscard]] auto last_char() const -> char { return (wptr == 0) ? char(0) : at(wptr - 1); }
    [[nodiscard]] auto last_slash() const -> long;
    [[nodiscard]] auto size() const -> size_t { return wptr; }
    auto               look_at_space(const std::string &s) -> bool;
    void               lowercase();
    void               make_citation(String a, String b);
    auto               make_unique_bid(int) -> std::string;
    void               modify(size_t p, char c) { at(p) = c; }
    void               new_keyword();
    void               new_word();
    void               next_bibtex_char();
    void               next_bibtex_name_token();
    auto               next_char() { return at(ptr++); }
    auto               next_env_spec() -> bool;
    auto               next_kw() -> bool;
    auto               next_macro() -> bool;
    auto               next_macro_spec() -> bool;
    auto               next_utf8_byte() -> uchar;
    auto               next_utf8_char() -> codepoint;
    auto               next_utf8_char_aux() -> codepoint;
    void               no_control_M();
    void               no_newline();
    void               no_double_dot();
    void               normalise_for_bibtex(String s);
    void               optslash();
    void               out_four_hats(codepoint ch);
    void               out_log(codepoint ch, output_encoding_type T);
    auto               pack_or_class(Buffer &aux) -> int;
    void               pt_to_mu();
    void               process_big_char(size_t n);
    void               purify();
    void               purify(String s);
    void               push_back(char c);
    void               push_back(const Buffer &b) { push_back(b.data()); }
    void               push_back(const std::string &b) { push_back(b.c_str()); }
    void               push_back(const Istring &X);
    void               push_back(String s);
    void               push_back(ScaledInt v, glue_spec unit);
    void               push_back(const SthInternal &x);
    void               push_back(const Glue &x);
    void               push_back(const AttList &Y);
    void               push_back(const AttPair &X);
    void               push_back(const Macro &x);
    void               push_back(const Macro &x, bool sw);
    auto               push_back(Token T) -> bool;
    void               insert_token(Token T, bool sw);
    void               push_back_alt(const AttPair &X);
    void               push_back_braced(const std::string &s);
    void               push_back_braced(String s);
    void               push_back_def(String, std::string);
    void               push_back_elt(Istring name, Xid id, int w);
    void               push_back_int(long n);
    void               push_back16(size_t n, bool uni);
    void               push_back16l(bool hat, uint n);
    void               push_back_ent(codepoint ch);
    void               push_back_hex(uint c);
    void               push_back_Hex(uint c);
    void               push_back_math_token(const CmdChr &x, bool space);
    void               push_back_math_tag(const CmdChr &x, int type);
    void               push_back_math_tag(String s, int type);
    void               push_back_math_aux(String s);
    void               push_back_newline();
    auto               push_back_newline_spec() -> bool;
    void               push_back_open_brace() { push_back('{'); };
    void               push_back_close_brace() { push_back('}'); };
    void               push_back_roman(long n);
    void               push_back_Roman(long n);
    void               push_back_space() { push_back(' '); };
    void               push_back_special_att(Xid id);
    void               push_back_special_string(String s);
    void               push_back_real_utf8(codepoint c);
    void               push_back_xml_char(uchar c);
    void               push_back_substring(String S, size_t n);
    void               push_back_substring(const std::string &S, size_t p, size_t n);
    void               push_back_unless_punct(char c);
    void               push_back(codepoint c);
    void               push_back3(unsigned int x);
    void               push_back9(unsigned int x);
    void               put_at_end(String s);
    auto               remove_digits(const std::string &s) -> std::string;
    auto               remove_space(const std::string &x) -> std::string;
    void               rrl() {
        wptr--;
        at(wptr) = 0;
    } // really remove last
    void remove_last() {
        if (wptr > 0) rrl();
    }
    void remove_last_n(size_t n) {
        if (wptr >= n) wptr -= n;
        at(wptr) = 0;
    }
    void remove_last_comma() {
        if (wptr > 0 && at(wptr - 1) == ',') rrl();
    }
    void remove_last_quote() {
        if (wptr > 0 && at(wptr - 1) == '\'') rrl();
    }
    void remove_last_space();
    void remove_space_at_end();
    void remove_spec_chars(bool url, Buffer &B);
    void reset0() { wptr = 0; }
    void reset() {
        wptr  = 0;
        at(0) = 0;
    }
    void               reset_ptr() { ptr = 0; }
    void               resize();
    auto               reverse_horner() -> uint;
    auto               see_begin_end(Buffer &before, Buffer &after) -> int;
    [[nodiscard]] auto see_config_env() const -> int;
    auto               see_config_kw(String s, bool c) -> String;
    auto               see_equals(String s) -> bool;
    auto               see_something(const String *Table) -> int;
    auto               see_year(const std::string &a, std::string &b) -> int;
    void               set(String s) {
        reset0();
        push_back(s);
    }
    void set_last(size_t k) {
        wptr     = k;
        at(wptr) = 0;
    }
    void               set_ptr1_to_ptr() { ptr1 = ptr; }
    void               show_uncomplete(String m);
    void               skip_over_brace();
    void               skip_over_comment();
    [[nodiscard]] auto skip_space(size_t j) const {
        while (is_spaceh(j)) j++;
        return j;
    }
    void skip_letter() {
        while (is_letter(head())) ptr++;
    }
    void skip_sp_tab() {
        while (at(ptr) == ' ' || at(ptr) == '\t') ptr++;
    }
    void skip_sp_tab_nl() {
        while (is_space(at(ptr))) ptr++;
    }
    void skip_sp_tab_comma() {
        while (at(ptr) == ' ' || at(ptr) == '\t' || at(ptr) == ',') ptr++;
    }
    [[nodiscard]] auto single_char() const -> char;
    auto               slash_separated(std::string &a) -> bool;
    auto               split_at_colon(std::string &before, std::string &after) -> bool;
    auto               sortify(String s) -> String;
    auto               svn_id(std::string &name, std::string &date, std::string &version) -> bool;
    [[nodiscard]] auto space_or_underscore() const -> bool { return at(ptr) == '_' || at(ptr) == ' '; }
    auto               special_convert(bool init) -> std::string;
    [[nodiscard]] auto special_exponent() const -> String;
    void               special_purify(String s, int &pos);
    void               special_title(std::string s);
    auto               str_toks(nl_to_tok nl) -> TokenList;
    auto               str_toks11(bool nl) -> TokenList;
    auto               string_delims() -> bool;
    auto               substring() -> std::string;
    [[nodiscard]] auto tex_comment_line() const -> bool;
    void               to_seven_bits();
    [[nodiscard]] auto to_string() const -> std::string;
    [[nodiscard]] auto to_string(size_t k) const -> std::string;
    auto               convert_for_xml_err(Token t) -> Istring;
    auto               tp_next_char(char &res) -> bool;
    auto               tp_fetch_something() -> tpa_line;
    auto               trace_scan_dimen(Token T, ScaledInt v, bool mu) -> String;
    [[nodiscard]] auto uhead() const { return at(ptr); }
    void               undo() { ptr--; }
    void               unicode_char(int);
    [[nodiscard]] auto unique_character() const -> codepoint;
    [[nodiscard]] auto make_character() const -> codepoint;
    void               uppercase();
    void               utf8_error(bool first);
    static void        utf8_ovf(size_t n);
    auto               xml_and_attrib(const std::string &s) -> Xml *;
    auto               without_end_spaces(String T) -> String;
    auto               find_char(char c) -> bool;
    void               l3_fabricate_cond(const std::string &base, const std::string &sig, subtypes w);
    [[nodiscard]] auto is_special_end() const -> bool { return at(ptr) == '\n' || at(ptr) == '#' || at(ptr) == '%'; }

private:
    void realloc(size_t s);
    auto after_slash() -> bool;
    void advance_letter_dig() {
        while (is_letter(at(ptr)) || is_digit(at(ptr))) ptr++;
    }
    void advance_letter_dig_dot() {
        while (is_letter(at(ptr)) || is_digit(at(ptr)) || at(ptr) == '.') ptr++;
    }
    void advance_letter_dig_dot_slash() {
        while (is_letter(at(ptr)) || is_digit(at(ptr)) || at(ptr) == '.' || at(ptr) == '/') ptr++;
    }
};

inline void bf_reset(Buffer &B) { B.reset0(); }
inline void bf_optslash(Buffer &B) { B.optslash(); }
inline void bf_comma(Buffer &B) {
    if (!B.empty()) B.push_back(',');
}

extern Buffer Txbuf, err_buf, ssa2;