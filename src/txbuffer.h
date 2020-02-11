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

#include "txscaled.h"
#include "txtokenlist.h"

using buffer_fn = void(Buffer &);

// a big structure
class Buffer {
private:
    char *buf;        // the characters
    int   wptr{0};    // the write pointer
    int   asize{128}; // allocated size
    int   ptr{0};     // the read pointer
    int   ptr1{0};    // a seconnd read pointer
public:
    Buffer();
    ~Buffer();
    auto operator[](int k) const -> char { return buf[k]; }
    auto operator<<(String s) -> Buffer & {
        push_back(s);
        return *this;
    }
    auto operator<<(char c) -> Buffer & {
        push_back(c);
        return *this;
    }
    auto operator<<(uchar c) -> Buffer & {
        push_back(c);
        return *this;
    }
    auto operator<<(int c) -> Buffer & {
        push_back_int(c);
        return *this;
    }
    auto operator<<(const TokenList &) -> Buffer &;
    auto operator<<(const Macro &) -> Buffer &;
    auto operator<<(const Utf8Char &b) -> Buffer & {
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
    auto operator<<(const string &b) -> Buffer & {
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
    auto               add_with_space(String) -> string;
    void               advance() { ptr++; }
    void               advance(int k) { ptr += k; }
    void               alloc(int);
    [[nodiscard]] auto after_head() const -> char { return buf[ptr + 1]; }
    [[nodiscard]] auto after_uhead() const -> uchar { return buf[ptr + 1]; }
    [[nodiscard]] auto after_after_uhead() const -> uchar { return buf[ptr + 2]; }
    auto               append_checked_line(LinePtr &) -> int;
    [[nodiscard]] auto at_eol() const -> bool { return wptr <= ptr; }
    auto               backup_space() -> bool;
    void               bib_spec();
    void               brace_match();
    void               bracket_match();
    [[nodiscard]] auto c_str(int k) const -> String { return buf + k; }
    [[nodiscard]] auto c_str() const -> String { return buf; }
    void               chars_to_buffer(Buffer &);
    void               check_before_brace(String);
    auto               check_cat_perso(int, int, bool) -> string;
    auto               contains(String s) const -> bool { return strstr(buf, s) != nullptr; }
    auto               contains_braced(String env) -> bool;
    auto               contains_env(String env) -> bool;
    auto               contains_here(String) const -> bool;
    [[nodiscard]] auto convert(int k) const -> String;
    auto               convert_dim(int, int) -> string;
    [[nodiscard]] auto convert_to_str() const -> String;
    auto               convert_to_out_encoding(String) const -> String;
    [[nodiscard]] auto convert_to_log_encoding() const -> String;
    [[nodiscard]] auto convert_to_latin1(bool) const -> String;
    void               convert_custom(int l);
    auto               convert_line0(int) -> bool;
    void               convert_line(int l, int);
    void               copy(const Buffer &);
    void               decr_wptr() { wptr--; };
    auto               double_hat_aux(int) -> bool;
    void               dump_prefix(bool err, bool gbl, symcodes K);
    [[nodiscard]] auto empty() const -> bool { return size() == 0; }
    void               extract_cnrs_info();
    void               extract_dtd(String a, string &b, string &c);
    void               extract_strs(Buffer &A, Buffer &B);
    void               extract_chars(vector<Utf8Char> &);
    auto               fetch_beg_end_spec(bool k, bool incat, int &com_loc, bool &seen_dollar, String &) -> int;
    auto               fetch_citation(String, String &a, String &b) -> bool;
    auto               fetch_spec_arg() -> bool;
    auto               figline(int &ctr, string &junk) -> string;
    void               figtable(String start, int id);
    void               fill_table(bchar_type *table);
    auto               find_alias(const vector<string> &SL, string &res) -> bool;
    auto               find_and(bchar_type *table) -> bool;
    void               find_bibtex_name_token(int);
    auto               find_brace() -> int;
    auto               find_bracket() -> int;
    auto               find_configuration(Buffer &) -> bool;
    auto               find_doctype() -> int;
    auto               find_documentclass(Buffer &) -> bool;
    auto               find_equals() -> bool;
    auto               find_one_bibtex_name() -> String;
    void               find_one_type(vector<string> &);
    void               find_top_atts();
    void               finish_figure(String start, string junk, int w);
    void               finish_xml_print();
    auto               full_bracket_match() -> bool;
    auto               full_brace_match() -> bool;
    [[nodiscard]] auto get_int_val() const -> int;
    auto               get_machine_name() -> string;
    [[nodiscard]] auto get_ptr() const -> int { return ptr; }
    [[nodiscard]] auto get_ptr1() const -> int { return ptr1; }
    [[nodiscard]] auto hashcode(int hash) const -> int;
    [[nodiscard]] auto head() const -> char { return buf[ptr]; }
    auto               is_not_char(int p, uchar x) -> bool { return uchar(buf[p]) != x; }
    auto               holds_documentclass(Buffer &a, Buffer &b, Buffer &c) -> int;
    auto               holds_env(String &a, String &b, String &c) -> int;
    auto               horner(int) -> unsigned int;
    auto               how_many_bibtex_name_token() -> int;
    void               kill_at(int p) { buf[p] = 0; }
    void               init_from_buffer(Buffer &);
    void               insert_and_kill(Buffer &a) {
        reset();
        push_back(a.buf);
        a.reset();
    }
    void               insert_escape_char();
    void               insert_escape_char_raw();
    auto               insert_fp(const FpNum &) -> String;
    [[nodiscard]] auto insert_space_here(int) const -> bool;
    void               insert_string(const Buffer &s);
    auto               insert_break(const string &) -> string;
    void               insert_token0(Token c);
    auto               install_att(Xid idx, Istring match) -> bool;
    void               interpret_aux(vector<Istring> &, vector<Istring> &);
    void               interpret_bibtex_list();
    void               interpret_bibtex_extension_list();
    auto               is_and(int) -> bool;
    [[nodiscard]] auto is_begin_end() const -> int;
    auto               is_begin_something(String) -> int;
    auto               is_begin_something(String, bool) -> int;
    auto               is_equal(String x) const -> bool { return strcmp(buf, x) == 0; }
    auto               is_at_end(String x) const -> bool;
    auto               is_here(String s) -> bool;
    auto               is_here_case(String s) -> bool;
    [[nodiscard]] auto is_letter_digit() const -> bool;
    [[nodiscard]] auto is_all_ascii() const -> bool;
    [[nodiscard]] auto is_good_ascii() const -> bool;
    [[nodiscard]] auto is_spaceh(int j) const -> bool { return is_space(buf[j]); }
    [[nodiscard]] auto last_char() const -> char {
        if (wptr == 0)
            return 0;
        else
            return buf[wptr - 1];
    }
    [[nodiscard]] auto last_slash() const -> int;
    [[nodiscard]] auto length() const -> int { return wptr; }
    [[nodiscard]] auto size() const -> int { return wptr; }
    auto               look_at_space(string s) -> bool;
    void               lowercase();
    void               make_citation(String a, String b);
    auto               make_unique_bid(int) -> string;
    void               modify(int p, char c) { buf[p] = c; }
    void               new_keyword();
    void               new_word();
    void               next_bibtex_char();
    void               next_bibtex_name_token();
    auto               next_char() -> uchar {
        uchar c = buf[ptr];
        ptr++;
        return c;
    }
    auto next_env_spec() -> bool;
    auto next_kw() -> bool;
    auto next_macro() -> bool;
    auto next_macro_spec(bool incat, int &com_loc, bool &seen_dollar) -> bool;
    auto next_macro_spec() -> bool;
    auto next_utf8_byte() -> uchar;
    auto next_utf8_char() -> Utf8Char;
    auto next_utf8_char_aux() -> Utf8Char;
    void no_control_M();
    void no_newline();
    void no_double_dot();
    void normalise_for_bibtex(String s);
    void optslash();
    void out_four_hats(Utf8Char);
    void out_log(Utf8Char ch, output_encoding_type T);
    auto pack_or_class(Buffer &aux) -> int;
    void pt_to_mu();
    void process_big_char(unsigned int c);
    void purify();
    void purify(String s);
    void push_back(char c);
    void push_back(const Buffer &b) { push_back(b.buf); }
    void push_back(const string &b) { push_back(b.c_str()); }
    void push_back(const Istring &);
    void push_back(String s);
    void push_back(ScaledInt v, glue_spec unit);
    void push_back(const SthInternal &);
    void push_back(const Glue &);
    void push_back(const AttList &);
    void push_back(const AttPair &);
    void push_back(const Macro &);
    void push_back(const Macro &, bool);
    auto push_back(Token c) -> bool;
    void insert_token(Token c, bool);
    void push_back_alt(const AttPair &);
    void push_back_braced(string);
    void push_back_braced(String);
    void push_back_def(String, string);
    void push_back_elt(Istring, Xid, int);
    void push_back_int(int);
    void push_back16(uint, bool);
    void push_back16l(bool, uint);
    void push_back_ent(Utf8Char);
    void push_back_hex(uint);
    void push_back_Hex(uint);
    void push_back_math_token(const CmdChr &, bool);
    void push_back_math_tag(const CmdChr &, int);
    void push_back_math_tag(String, int);
    void push_back_math_aux(String);
    void push_back_newline();
    auto push_back_newline_spec() -> bool;
    void push_back_open_brace() { push_back('{'); };
    void push_back_close_brace() { push_back('}'); };
    void push_back_roman(int);
    void push_back_Roman(int);
    void push_back_space() { push_back(' '); };
    void push_back_special_att(Xid id);
    void push_back_special_string(String s);
    void push_back_real_utf8(Utf8Char c);
    void push_back_xml_char(uchar c);
    void push_back_substring(String, int);
    void push_back_substring(const string &, int, int);
    void push_back_unless_punct(char c);
    void push_back(Utf8Char c);
    void push_back3(unsigned int);
    void push_back9(unsigned int);
    void put_at_end(String x);
    auto remove_digits(string) -> string;
    auto remove_space(const string &) -> string;
    void rrl() {
        wptr--;
        buf[wptr] = 0;
    } // really remove last
    void remove_last() {
        if (wptr > 0) rrl();
    }
    void remove_last_n(int n) {
        if (wptr >= n) wptr -= n;
        buf[wptr] = 0;
    }
    void remove_last_comma() {
        if (wptr > 0 && buf[wptr - 1] == ',') rrl();
    }
    void remove_last_quote() {
        if (wptr > 0 && buf[wptr - 1] == '\'') rrl();
    }
    void remove_last_space();
    void remove_space_at_end();
    void remove_spec_chars(bool, Buffer &);
    void reset0() { wptr = 0; }
    void reset() {
        wptr   = 0;
        buf[0] = 0;
    }
    void               reset_ptr() { ptr = 0; }
    void               resize();
    auto               reverse_horner() -> uint;
    auto               see_begin_end(Buffer &before, Buffer &after) -> int;
    [[nodiscard]] auto see_config_env() const -> int;
    auto               see_config_kw(String s, bool c) -> String;
    auto               see_equals(String s) -> bool;
    auto               see_something(const String *Table) -> int;
    auto               see_year(const string &a, string &b) -> int;
    void               set(String s) {
        reset0();
        push_back(s);
    }
    void set_last(int k) {
        wptr      = k;
        buf[wptr] = 0;
    }
    void               set_ptr(int j) { ptr = j; }
    void               set_ptr1(int j) { ptr1 = j; }
    void               set_ptr1() { ptr1 = ptr; }
    void               show_uncomplete(String);
    void               skip_over_brace();
    void               skip_over_comment();
    [[nodiscard]] auto skip_space(int j) const -> int {
        while (is_spaceh(j)) j++;
        return j;
    }
    void skip_letter() {
        while (is_letter(head())) ptr++;
    }
    void skip_sp_tab() {
        while (buf[ptr] == ' ' || buf[ptr] == '\t') ptr++;
    }
    void skip_sp_tab_nl() {
        while (is_space(buf[ptr])) ptr++;
    }
    void skip_sp_tab_comma() {
        while (buf[ptr] == ' ' || buf[ptr] == '\t' || buf[ptr] == ',') ptr++;
    }
    [[nodiscard]] auto single_char() const -> char;
    auto               slash_separated(string &) -> bool;
    auto               some_substring(int a, int b) -> String;
    auto               some_sub_string(int a, int b) -> string;
    auto               split_at_colon(string &, string &) -> bool;
    auto               sortify(String s) -> String;
    auto               svn_id(string &name, string &date, string &version) -> bool;
    [[nodiscard]] auto space_or_underscore() const -> bool { return buf[ptr] == '_' || buf[ptr] == ' '; }
    auto               special_convert(bool) -> string;
    [[nodiscard]] auto special_exponent() const -> String;
    void               special_purify(String s, int &pos);
    void               special_title(string s);
    auto               str_toks(nl_to_tok) -> TokenList;
    auto               str_toks11(bool) -> TokenList;
    auto               string_delims() -> bool;
    auto               substring() -> string;
    [[nodiscard]] auto tex_comment_line() const -> bool;
    void               to_seven_bits();
    [[nodiscard]] auto to_string() const -> string;
    [[nodiscard]] auto to_string(int k) const -> string;
    auto               convert_for_xml_err(Token t) -> Istring;
    auto               tp_next_char(char &) -> bool;
    auto               tp_fetch_something() -> tpa_line;
    auto               trace_scan_dimen(Token, ScaledInt, bool) -> String;
    [[nodiscard]] auto uhead() const -> unsigned char { return buf[ptr]; }
    void               undo() { ptr--; }
    void               unicode_char(int);
    [[nodiscard]] auto unique_character() const -> Utf8Char;
    [[nodiscard]] auto make_character() const -> Utf8Char;
    void               uppercase();
    void               utf8_error(bool);
    void               utf8_ovf(int);
    auto               xml_and_attrib(string s) -> Xml *;
    auto               without_end_spaces(String T) -> String;
    auto               find_char(char c) -> bool;
    void               l3_fabricate_cond(const string &, const string &, subtypes);
    [[nodiscard]] auto is_special_end() const -> bool { return buf[ptr] == '\n' || buf[ptr] == '#' || buf[ptr] == '%'; }

private:
    void realloc();
    auto after_slash() -> bool;
    void advance_letter_dig() {
        while (is_letter(buf[ptr]) || is_digit(buf[ptr])) ptr++;
    }
    void advance_letter_dig_dot() {
        while (is_letter(buf[ptr]) || is_digit(buf[ptr]) || buf[ptr] == '.') ptr++;
    }
    void advance_letter_dig_dot_slash() {
        while (is_letter(buf[ptr]) || is_digit(buf[ptr]) || buf[ptr] == '.' || buf[ptr] == '/') ptr++;
    }
};

inline void bf_reset(Buffer &B) { B.reset0(); }
inline void bf_optslash(Buffer &B) { B.optslash(); }
inline void bf_comma(Buffer &B) {
    if (!B.empty()) B.push_back(',');
}
