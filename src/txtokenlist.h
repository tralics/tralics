#pragma once
// -*- C++ -*-
// $Id: txtokenlist.h,v 2.15 2015/10/28 17:38:46 grimm Exp $
// Copyright INRIA/apics (Jose' Grimm)  2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

class Hashtab;

using TokenList            = std::list<Token>;
using const_token_iterator = std::list<Token>::const_iterator;
using token_iterator       = std::list<Token>::iterator;

namespace token_ns {
    void add_env(TokenList &L, String name);
    void add_par_noindent(TokenList &, const Hashtab &);
    void add_verbatim_number(TokenList &, const Hashtab &, int);
    auto block_size(const TokenList &) -> int;
    void expand_nct(TokenList &);
    auto expand_nct(TokenList &L, int n, uchar c, int &, TokenList &) -> bool;
    void expand_star(TokenList &);
    auto fast_get_block(TokenList &) -> TokenList;
    void fast_get_block(TokenList &, TokenList &w);
    auto get_a_param(TokenList &, bool) -> TokenList;
    auto get_block(TokenList &) -> TokenList;
    auto get_unique(TokenList &L) -> Token;
    void get_unique(TokenList &L, Token &t1, Token &t2);
    auto has_a_single_token(const TokenList &) -> bool;
    auto has_a_single_token(const TokenList &, Token) -> bool;
    auto posint_to_list(int n) -> TokenList;
    void push_back_i(TokenList &, int n);
    void remove_block(TokenList &);
    void remove_ext_braces(TokenList &);
    void remove_initial_spaces(TokenList &);
    void remove_first_last_space(TokenList &);
    void replace(TokenList &A, Token x1, Token x2);
    auto replace_space(TokenList &A, Token x2, Token x3) -> int;
    void show(const TokenList &);
    auto split_at(Token x1, Token x2, Token x3, TokenList &L, TokenList &z, bool) -> bool;
    auto string_to_list(Istring s) -> TokenList;
    auto string_to_list(String s, bool) -> TokenList;
    auto string_to_list(const string &s, bool) -> TokenList;
    void double_hack(TokenList &key);
    auto split_at(Token m, TokenList &L, TokenList &z) -> bool;
    auto is_sublist(token_iterator A, token_iterator B, int) -> bool;
    auto is_in(TokenList &A, TokenList &B, bool remove, int &) -> bool;
    void normalise_list(char c, TokenList &L);
    void sanitize_one(TokenList &L, uchar c);
    void sanitize_one(TokenList &L, TokenList &s, int n);
    void sanitize_one(TokenList &L);
    auto check_brace(Token, int &) -> bool;
    auto compare(const TokenList &A, const TokenList &B) -> bool;
} // namespace token_ns

// This is for fp
class FpGenList {
public:
    TokenList value;

public:
    FpGenList(TokenList A) : value(A) {}
    void add_last_space(String);
    void add_last_space(TokenList &, String);
    auto find_str(int &n) const -> Token;
    void fp_gen_add();
    void fp_gen_app();
    void fp_gen_exp();
    void fp_gen_komma();
    void fp_gen_mul();
    void push_back(Token L) { value.push_back(L); }
    void push_back(TokenList &L) { value.splice(value.end(), L); }
    void remove_first_n(int n);
    void remove_spaces();
    void split_after(int n, TokenList &z);
    auto split_at(Token x, Token y, TokenList &z) -> Token;
    void split_after(token_iterator X, TokenList &z);
    auto split_at_p(TokenList &, TokenList &z) -> bool;
    void to_postfix();
    void fp_check_paren();
};

//
class FpStack {
    TokenList value;

public:
    void clear() { value.clear(); }
    auto empty() const -> bool { return value.empty(); }
    void pop_upn(FpNum &);
    void pop_upn(TokenList &);
    void push_front(Token L) { value.push_front(L); }
    void push_front(TokenList &L) { value.splice(value.begin(), L); }
    void push_upn(TokenList &);
    void push_upn(FpNum);
};

// This represents the value of a user-defined command
class Macro {
    int       nbargs{0};       // number of arguments
    def_type  type{dt_normal}; // type of macro
    TokenList delimiters[10];  // deleimiters bewtween arguments
    TokenList body;            // the body
public:
    Macro() {}
    Macro(TokenList L) : nbargs(0), type(dt_normal), body(L) { correct_type(); }
    // other methods
    auto get_type() const -> def_type { return type; }
    auto get_nbargs() const -> int { return nbargs; }
    auto get_body() -> TokenList & { return body; }
    auto get_body() const -> const TokenList & { return body; }
    void set_nbargs(int n) { nbargs = n; }
    void set_type(def_type n) { type = n; }
    auto is_same(const Macro &) const -> bool;
    auto operator[](int n) const -> const TokenList & { return delimiters[n]; }
    void set_delimiters(int k, TokenList L) { delimiters[k] = L; }
    void correct_type();
};

// The table of macros. it contains the reference counts
// Consider: \def\mac{\def\mac{a} b}. When mac is expanded, its body is copied
// when the inner \def is executed, then \mac is destroyed (if nobody else
// points to it, i.e. if the reference count is zero)
class Mactab {
private:
    Macro **table{0};          // this contains the table
    int *   rc_table{0};       // this contains the reference counts
    int     cur_rc_mac_len{0}; // size of the table.
    int     ptr{-1};           // pointer to the first free position
private:
    void rc_mac_realloc();

public:
    Mactab(){};
    void incr_macro_ref(int c) { rc_table[c]++; }
    void delete_macro_ref(int i);
    auto get_macro(int k) -> Macro & { return *table[k]; }
    auto new_macro(Macro *s) -> subtypes;
};
