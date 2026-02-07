// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// Converts an integer into a token list, catcode 12
// Assumes n>0, otherwise result is empty.
auto token_ns::posint_to_list(long n) -> TokenList {
    TokenList L;
    if (n <= 0) return L;
    while (n != 0) {
        auto k  = n % 10;
        n       = n / 10;
        Token t = Token(other_t_offset, uchar(k + '0'));
        L.push_front(t);
    }
    return L;
}

// Adds an integer to the end of the token list.
// Uses a temporary list, since we get digits from right to left.
// Noting done if n<0. This is used for inserting verbatim line numbers.
void token_ns::push_back_i(TokenList &L, long n) {
    TokenList tmp = posint_to_list(n);
    L.splice(L.end(), tmp);
}

// Finds the first M, not escaped by E in the list L.
// If so, puts the prefix in z, removes it from L
// A pair EX is replaced by X in Z if S is false
auto token_ns::split_at(Token e, Token m, Token m1, TokenList &L, TokenList &z, bool s) -> bool {
    z.clear();
    while (!L.empty()) {
        Token x = L.front();
        L.pop_front();
        if (x == e) {
            if (s) z.push_back(x);
            if (L.empty()) break;
            x = L.front();
            L.pop_front();
            z.push_back(x);
        } else if (x == m)
            return true;
        else if (x == m1)
            return true;
        else
            z.push_back(x);
    }
    swap(L, z);
    return false;
}

// Checks that x is a brace, increases or decreases brace level
// returns true if this is the toplevel closing brace
auto token_ns::check_brace(Token x, int &bl) -> bool {
    if (x.is_a_brace()) {
        if (x.is_a_left_brace()) {
            bl++;
        } else {
            bl--;
            if (bl == 0) return true;
        }
    }
    return false;
}

// Replace x1 by x2 at toplevel in the list a
void token_ns::replace(TokenList &A, Token x1, Token x2) {
    auto C  = A.begin();
    auto E  = A.end();
    int  bl = 0;
    while (C != E) {
        Token x = *C;
        check_brace(x, bl);
        if (bl == 0 && x == x1) *C = x2;
        ++C;
    }
}

// Replace space by x2,x3  at brace-level zero.
// Two consecutive space tokens are replaced by
// a single occurence.
auto token_ns::replace_space(TokenList &A, Token x2, Token x3) -> int {
    remove_first_last_space(A);
    auto C             = A.begin();
    auto E             = A.end();
    int  bl            = 0;
    int  n             = 0;
    bool prev_is_space = false;
    while (C != E) {
        Token x = *C;
        check_brace(x, bl);
        if (bl == 0 && x.is_space_token()) {
            if (!prev_is_space) {
                A.insert(C, x2);
                *C = x3;
                ++n;
            }
            prev_is_space = true;
        } else
            prev_is_space = false;
        ++C;
    }
    return n;
}

auto token_ns::compare(const TokenList &A, const TokenList &B) -> bool {
    auto C1 = A.begin();
    auto E1 = A.end();
    auto C2 = B.begin();
    auto E2 = B.end();
    for (;;) {
        if (C1 == E1 || C2 == E2) return C1 == E1 && C2 == E2;
        if (!(*C1).is_same_token(*C2)) return false;
        ++C1;
        ++C2;
    }
}

// Removes the external braces in {foo}, but not in {foo}{bar}.
void token_ns::remove_ext_braces(TokenList &L) {
    if (L.empty()) return;
    if (!L.front().is_OB_token()) return;
    if (!L.back().is_CB_token()) return;
    auto C = L.begin();
    auto E = L.end();
    ++C;
    --E;
    int b = 0;
    while (C != E) {
        if (C->is_OB_token())
            b++;
        else if (C->is_CB_token()) {
            b--;
            if (b < 0) return;
        }
        ++C;
    }
    if (b != 0) return; // should not happen.
    L.pop_front();
    L.pop_back();
}

void token_ns::remove_initial_spaces(TokenList &L) {
    while (!L.empty()) {
        if (!L.front().is_space_token()) return;
        L.pop_front();
    }
}

// Converts the string in the buffer into a token list.
// Everything is of \catcode 12, except space.
// If the switch is true, \n is converted to space, otherwise newline

auto Buffer::str_toks(nl_to_tok nl) -> TokenList {
    TokenList L;
    auto      SP = Token(space_token_val);
    auto      CR = Token(space_t_offset + '\n'); // behaves as space
    auto      NL = Token(other_t_offset + '\n'); // is ^^J
    ptrs.b       = 0;
    for (; !at_eol();) {
        char32_t c = next_utf8_char();
        if (c == 0) {
        } // ignore bad chars
        else if (c == ' ')
            L.push_back(SP);
        else if (c == '\n')
            L.push_back(nl == nlt_space ? SP : (nl == nlt_cr ? CR : NL));
        else
            L.push_back(Token(other_t_offset, c));
    }
    return L;
}

// Use character code 11 whenever possible
auto Buffer::str_toks11(bool nl) -> TokenList {
    auto      SP = Token(space_token_val);
    Token     NL = hash_table.newline_token;
    TokenList L;
    ptrs.b = 0;
    for (;;) {
        if (at_eol()) return L;
        char32_t c = next_utf8_char();
        if (c == 0) {
        } // ignore bad chars
        else if (c == ' ')
            L.push_back(SP);
        else if (c == '\n')
            L.push_back(nl ? SP : NL);
        else if ((std::isalpha(static_cast<int>(c)) != 0) || c == '@')
            L.push_back(Token(letter_t_offset, c));
        else
            L.push_back(Token(other_t_offset, c));
    }
}

// Converts a string to a token list. If b is true, we add braces.
// NOTE:  in every case converts newline to space
auto token_ns::string_to_list(const std::string &s, bool b) -> TokenList {
    Buffer &B   = buffer_for_log;
    B           = s;
    TokenList L = B.str_toks(nlt_space);
    if (b) L.brace_me();
    return L;
}

// Prints a token list.
// Note: conversion to log_encoding
auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream & {
    auto C = L.begin();
    auto E = L.end();
    while (C != E) {
        buffer_for_log.clear();
        if (buffer_for_log.push_back(*C)) buffer_for_log << ' ';
        fp << buffer_for_log;
        ++C;
    }
    return fp;
}

// Puts a macro into a buffer.
void Buffer::push_back(const Macro &x) {
    *this << x[0];
    auto K = x.nbargs;
    if (x.type != dt_optional) {
        for (size_t i = 0; i < K; i++) {
            format("#{}", i + 1);
            *this << x[i + 1];
        }
    } else {
        *this << x[1];
        for (size_t i = 1; i < K; i++) { format("#{}", i + 1); }
    }
    if (!empty() && back() == '{') back() = '#';
    *this += "->";
    *this << x.body;
}

// Puts a macro into a buffer.
// Sw is true when we want to print it
void Buffer::push_back(const Macro &x, bool sw) {
    if (!sw)
        push_back(x);
    else {
        Buffer B;
        B.push_back(x);
        append(B.convert_to_log_encoding());
    }
}

// Puts a macro definition in a file.
auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream & {
    Buffer &B = buffer_for_log;
    B.clear();
    B.push_back(x, true);
    return fp << B;
}

// True if L has a single token
auto token_ns::has_a_single_token(const TokenList &L) -> bool {
    auto C = L.begin();
    auto E = L.end();
    if (C == E) return false;
    ++C;
    return C == E;
}

// True if L has a single token that is T
auto token_ns::has_a_single_token(const TokenList &L, Token t) -> bool {
    auto C = L.begin();
    auto E = L.end();
    if (C == E) return false;
    if (*C != t) return false;
    ++C;
    return C == E;
}

// Removes first and last spaces in a token list.
void token_ns::remove_first_last_space(TokenList &L) {
    while (!L.empty() && L.front().is_space_token()) L.pop_front();
    while (!L.empty() && L.back().is_space_token()) L.pop_back();
}

// ------------------------ token lists ----------------------------

// kill L. If it has 1 element, returns it, otherwise 0.
auto token_ns::get_unique(TokenList &L) -> Token {
    if (L.empty()) return Token(0);
    Token x = L.front();
    L.pop_front();
    if (!L.empty()) x.kill();
    L.clear();
    return x;
}

// kill L. If it has 1,2 element, put in t1,t2
void token_ns::get_unique(TokenList &L, Token &t1, Token &t2) {
    t1 = Token(0);
    t2 = Token(0);
    if (L.empty()) return;
    t1 = L.front();
    L.pop_front();
    if (L.empty()) return;
    t2 = L.front();
    L.pop_front();
    if (!L.empty()) t1.kill();
    L.clear();
}

void Buffer::dump_prefix(bool err, bool gbl, symcodes K) {
    if (gbl) append("\\global");
    if (K == user_cmd) return;
    if (K == userp_cmd || K == userlp_cmd || K == userpo_cmd || K == userlpo_cmd) {
        if (err)
            append("\\");
        else
            insert_escape_char_raw();
        append("protected");
    }
    if (K == userl_cmd || K == userlo_cmd || K == userlp_cmd || K == userlpo_cmd) {
        if (err)
            append("\\");
        else
            insert_escape_char_raw();
        append("long");
    }
    if (K == usero_cmd || K == userlo_cmd || K == userpo_cmd || K == userlpo_cmd) {
        if (err)
            append("\\");
        else
            insert_escape_char_raw();
        append("outer");
    }
    push_back(' ');
}

// --------------------------------------------------
// Finds the first m at brace level 0; before in Z, after in L
auto token_ns::split_at(Token m, TokenList &L, TokenList &z) -> bool {
    z.clear();
    int bl = 0;
    while (!L.empty()) {
        Token x = L.front();
        L.pop_front();
        check_brace(x, bl);
        if (bl == 0 && x == m) return true;
        z.push_back(x);
    }
    return false;
}

// For all level-zero characters c, use a category code 12 char instead
void token_ns::sanitize_one(TokenList &L, uchar c) {
    Token T  = Token(other_t_offset, c);
    auto  C  = L.begin();
    auto  E  = L.end();
    int   bl = 0;
    while (C != E) {
        Token x = *C;
        check_brace(x, bl);
        if (bl == 0 && x.is_a_char() && x.char_val() == c) *C = T;
        ++C;
    }
}

// Replace in L all character tokens by  category code 12 ones
// All other tokens are discarded
void token_ns::sanitize_one(TokenList &L) {
    TokenList res;
    auto      C = L.begin();
    auto      E = L.end();
    while (C != E) {
        Token x = *C;
        if (x.is_a_char()) res.push_back(Token(other_t_offset, x.char_val()));
        ++C;
    }
    L.swap(res);
}

// For all characters c in s, at level at most n
// use a category code 12 char instead
void token_ns::sanitize_one(TokenList &L, TokenList &s, long n) {
    int bl = 0;
    for (auto &x : L) {
        check_brace(x, bl);
        if (bl <= n && x.is_a_char() && !s.empty() && s.begin()->char_val() == x.char_val()) x = *s.begin();
    }
}

// The name of the command is misleading: there is a single hack
void token_ns::double_hack(TokenList &key) {
    remove_first_last_space(key);
    remove_ext_braces(key);
}

// \tralics@split{L}\A\B{u=v,w} expands into
// \A{Lu}{v}\B{Lw}
void Parser::E_split() {
    Token     T       = cur_tok;
    TokenList prefix  = read_arg();
    TokenList cmd     = read_arg();
    TokenList cmd_def = read_arg();
    TokenList L       = read_arg();
    TokenList R;
    Token     x1 = hash_table.equals_token;
    Token     x2 = Token(other_t_offset, ',');
    TokenList key, val;
    for (;;) {
        if (L.empty()) break;
        token_ns::split_at(x2, L, val);
        bool seen_val = token_ns::split_at(x1, val, key);
        token_ns::double_hack(key);
        if (key.empty()) continue;
        {
            TokenList tmp = prefix;
            key.splice(key.begin(), tmp);
        }
        key.brace_me();

        token_ns::double_hack(val);
        if (seen_val) val.brace_me();
        TokenList tmp = seen_val ? cmd : cmd_def;
        R.splice(R.end(), tmp);
        R.splice(R.end(), key);
        if (seen_val) R.splice(R.end(), val);
    }
    if (tracing_macros()) {
        spdlog::trace("{}->{}", fmt::streamed(T), fmt::streamed(R));
    }
    back_input(R);
}

auto token_ns::is_sublist(TokenList::iterator A, TokenList::iterator B, int n) -> bool {
    while (n > 0) {
        if (*A != *B) return false;
        ++A;
        ++B;
        --n;
    }
    return true;
}
