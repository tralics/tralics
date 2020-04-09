// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txinline.h"
#include "txparser.h"
#include <fmt/format.h>

extern Buffer file_list;

// token lists.
namespace {
    Buffer buffer_for_log;
    // never use this outside function buffer::push_back(const macro&)
    Buffer buffer_for_log2;
} // namespace

namespace token_ns {
    auto length_normalise(TokenList &L) -> int;
} // namespace token_ns

// Prints some statistics at enfd of run.
void Stats::token_stats() const {
    main_ns::log_or_tty << "Math stats: formulas " << m_cv << ", kernels " << m_k << ", trivial " << m_trivial << ", \\mbox " << m_spec_box
                        << ", large " << m_large << ", small " << m_small << ".\n";
    if (nb_hdr != 0) main_ns::log_or_tty << "Number of HdR: " << nb_hdr << ".\n";
    main_ns::log_or_tty << "Buffer realloc " << stb_alloc << ", string " << st_nb_string << ", size " << static_cast<int>(str_length)
                        << ", merge " << m_merge << "\n"
                        << "Macros created " << nb_macros << ", deleted " << nb_macros_del << "; hash size "
                        << the_parser.hash_table.hash_usage << " + " << the_parser.hash_table.hash_bad << "; foonotes " << footnotes
                        << ".\n"
                        << "Save stack +" << level_up << " -" << level_down << ".\n"
                        << "Attribute list search " << sh_find << "(" << sh_boot << ") found " << sh_used << " in "
                        << static_cast<int>(the_main->the_stack->get_xid().value) << " elements (" << static_cast<int>(nb_xboot)
                        << " at boot).\n"
                        << "Number of ref " << nb_ref << ", of used labels " << nb_used_ref << ", of defined labels " << nb_label_defined
                        << ", of ext. ref. " << nb_href << ".\n";
    if (the_parser.get_list_files()) {
        log_and_tty << " *File List*\n";
        log_and_tty << file_list;
        log_and_tty << " ***********\n";
    }
    io_convert_stats();
}

// Converts an integer into a token list, catcode 12
// Assumes n>0, otherwise result is empty.
auto token_ns::posint_to_list(long n) -> TokenList {
    TokenList L;
    if (n <= 0) return L;
    while (n != 0) {
        int k   = n % 10;
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

// True if the Token is upper case or lower case x
auto Token::no_case_letter(char x) const -> bool {
    if (cmd_val() != letter_catcode) return false;
    auto c = to_signed(val_as_letter());
    if (c == x) return true;
    if (c == x + 'A' - 'a') return true;
    return false;
}

// The hash table  --------------------------------------------------

// Given a string s, with hash code p, we have p<hash_prime. In the good case,
// thhe string is at location p, otherwise at next(p), next(next(p)), etc.
// The idea is to use locations after hash prime. This means that all strings in
// the list has hash code p. If there is not enough room, we may use a location q
// less then hash prime. This means that, when looking for s string s' with
// hash code q we may encourer strings with hash code p.
// Note: initially Next[p] is zero; if non-zero the Text[Next[p]] is non-empty.

// Finds an empty slot in the hash table; fills it with name s.
// Return value is hash_used
// The string s must be a permanent string
auto Hashtab::find_empty(String s) -> size_t {
    for (;;) {
        hash_used--;
        if (hash_used <= 0) {
            log_and_tty << "Size of hash table is " << hash_size << "\n";
            log_and_tty << "Value of hash_prime is " << hash_prime << "\n";
            log_and_tty << "hash table full\n" << lg_fatal;
            abort();
        }
        if (!Text[hash_used]) break;
    }
    Text[hash_used] = s;
    hash_bad++;
    return hash_used;
}

// unused, kept for debug
// void Hashtab::dump()
// {
//   for(int i=0;i<hash_size;i++) {
//     if(Text[i])std::cout<< i << " " << Text[i] << "\n";
//   }
// }

// Returns the hash location of the name in the buffer.
// If a new slot has to be created, uses the string name, if not empty.
// The string name must be a permanent string
auto Hashtab::hash_find(const Buffer &b, String name) -> size_t {
    auto p = b.hashcode(hash_prime);
    for (;;) {
        if (Text[p] && (b.to_string() == *Text[p])) return p;
        if (Next[p] != 0)
            p = Next[p];
        else
            break;
    }
    if (name == nullptr) name = b.convert_to_str();
    return find_aux(p, name);
}

// Finds the object in the buffer B.
auto Hashtab::hash_find() -> size_t {
    auto p = B.hashcode(hash_prime);
    for (;;) {
        if (Text[p] && (B.to_string() == *Text[p])) return p;
        if (Next[p] != 0)
            p = Next[p];
        else
            break;
    }
    return find_aux(p, B.convert_to_str());
}

// This inserts name in the hash table.
// If Text[p] is empty, then p is not Next[q] so p is the hash code of s
// non empty, use this position. Otherwise find an empty position,
// and set Next[p] to this position.
auto Hashtab::find_aux(size_t p, String name) -> size_t {
    if (Text[p]) {
        auto q  = find_empty(name);
        Next[p] = q;
        return q;
    }
    hash_usage++;
    Text[p] = name;
    return p;
}

// Defines the command named a, but hash_find will not find it.
// The string a must be a permanent string
// This must be used at bootstrap code.
auto Hashtab::nohash_primitive(String a, CmdChr b) -> Token {
    hash_used--;
    auto p = hash_used;
    if (Text[p] || (p < hash_prime)) {
        log_and_tty << "Size of hash table is " << hash_size << "\n";
        log_and_tty << "Value of hash_prime is " << hash_prime << "\n";
        log_and_tty << "Current position is " << to_signed(p) << "\n";
        log_and_tty << "Bug in nohash_primitive\n" << lg_fatal;
        abort();
    }
    hash_bad++;
    Text[p] = a;
    auto t  = p + hash_offset;
    eqtb[t - eqtb_offset].special_prim(b); // allows to define an undefined command
    return Token(t);
}

// Returns the hashcode of the string in the buffer (assumed zero-terminated).
// \todo standalone from std::string
auto Buffer::hashcode(size_t prime) const -> size_t {
    size_t j = 1;
    size_t h = static_cast<uchar>(at(0));
    if (h == 0) return 0;
    for (;;) {
        size_t c = static_cast<uchar>(at(j));
        if (c == 0) return h;
        h = (h + h + c) % prime;
        j++;
    }
}

// Returns the hash table location of the string s.
// The string must be a non-empty permanent string
auto Hashtab::locate(String s) -> Token {
    if (s[1] == 0) return Token(uchar(s[0]) + single_offset);
    B << bf_reset << s;
    return Token(hash_find(B, s) + hash_offset);
}

auto Hashtab::locate(const std::string &s) -> Token {
    if (s.size() == 1) return Token(uchar(s[0]) + single_offset);
    B << bf_reset << s;
    return locate(B);
}

// This returns the token associated to the string in the buffer.
auto Hashtab::locate(const Buffer &b) -> Token {
    if (b.empty()) return Token(null_tok_val);
    codepoint c = b.single_character();
    if (c.non_null()) return Token(c.value + single_offset);
    return Token(hash_find(b, nullptr) + hash_offset);
}

// This returns true if the token associated to the string in the buffer
// exists in the hash table and is not undefined.
// Sets last_tok to the result
auto Hashtab::is_defined(const Buffer &b) -> bool {
    size_t T = 0;
    if (b.empty())
        T = null_tok_val;
    else {
        codepoint c = b.single_character();
        if (c.non_null())
            T = c.value + single_offset;
        else {
            auto p = b.hashcode(hash_prime);
            for (;;) {
                if (Text[p] && (b.to_string() == *Text[p])) break;
                if (Next[p] != 0)
                    p = Next[p];
                else
                    return false;
            }
            T = p + hash_offset;
        }
    }
    last_tok = Token(T);
    return !eqtb[T - eqtb_offset].is_undef();
}

// Creates a primitive.
// The string s must be a permanent string
auto Hashtab::primitive(String s, symcodes c, subtypes v) -> Token {
    Token res = locate(s);
    auto  w   = res.eqtb_loc();
    eqtb[w].primitive(CmdChr(c, v));
    return res;
}

// \global\let\firststring = \secondstring
// Both strings must be permanent strings
void Hashtab::eval_let(String a, String b) {
    auto A    = locate(a).eqtb_loc();
    auto Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], true);
}

auto Hashtab::eval_letv(String a, String b) -> Token {
    Token Av   = locate(a);
    auto  A    = Av.eqtb_loc();
    auto  Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], true);
    return Av;
}

// \let\firststring = \secondstring
// Both strings must be permanent strings
void Hashtab::eval_let_local(String a, String b) {
    auto A    = locate(a).eqtb_loc();
    auto Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], false);
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

// compares two macros
auto Macro::is_same(const Macro &aux) const -> bool {
    if (nbargs != aux.nbargs) return false;
    if (type != aux.type) return false;
    if (!token_ns::compare(body, aux.body)) return false;
    for (size_t i = 0; i < 10; i++)
        if (!token_ns::compare(delimiters[i], aux.delimiters[i])) return false;
    return true;
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

// If the token list is `{foo}bar{gee}'
// returns foo, leaves `bar{gee}' in the list
auto token_ns::get_block(TokenList &L) -> TokenList {
    int       bl = 0;
    TokenList res;
    remove_initial_spaces(L);
    while (!L.empty()) {
        Token t = L.front();
        L.pop_front();
        res.push_back(t);
        if (check_brace(t, bl)) {
            // Here we have {foo} in res
            remove_ext_braces(res);
            return res;
        }
    }
    return TokenList();
}

// Assumes that the list starts with a brace.
// Returns the number of tokens in sublist with its braces.
// If the sublist is everything, returns -1.
// in case of problem, returns -2.
auto token_ns::block_size(const TokenList &L) -> int {
    int  res = 0;
    int  bl  = 0;
    auto C   = L.begin();
    auto E   = L.end();
    while (C != E) {
        Token t = *C;
        ++C;
        ++res;
        if (check_brace(t, bl)) {
            if (C == E) return -1;
            return res;
        }
    }
    return -2;
}

// Assumes that the list starts with a brace.
// Returns the sublist with its braces.
auto token_ns::fast_get_block(TokenList &L) -> TokenList {
    int       len = block_size(L);
    TokenList res;
    if (len == -2) {
        L.clear();
        return res;
    }
    if (len == -1) {
        L.swap(res);
        return res;
    }
    auto C = L.begin();
    while (len > 0) {
        len--;
        ++C;
    }
    res.splice(res.begin(), L, L.begin(), C);
    return res;
}

// Assumes that the list L starts with a brace.
// puts the first block to the end of res
void token_ns::fast_get_block(TokenList &L, TokenList &res) {
    int len = block_size(L);
    if (len == -2) {
        L.clear();
        return;
    }
    if (len == -1) {
        res.splice(res.end(), L);
        return;
    }
    auto C = L.begin();
    while (len > 0) {
        len--;
        ++C;
    }
    res.splice(res.end(), L, L.begin(), C);
}

// Returns the first token, or the first token-list
// There are braces around the thing if br is true
auto token_ns::get_a_param(TokenList &L, bool br) -> TokenList {
    TokenList res;
    while (!L.empty()) {
        Token t = L.front();
        if (t.is_a_left_brace()) {
            TokenList w = fast_get_block(L);
            if (!br) remove_ext_braces(w);
            return w;
        }
        L.pop_front();
        if (t.is_space_token()) continue;
        res.push_back(t);
        break;
    }
    if (br) the_parser.brace_me(res);
    return res;
}

// Like getblock, returns nothing
void token_ns::remove_block(TokenList &L) {
    int bl = 0;
    remove_initial_spaces(L);
    while (!L.empty()) {
        Token t = L.front();
        L.pop_front();
        if (check_brace(t, bl)) return;
    }
}

// Interprets *{3}{abc} as 3 copies of abc
// This is used for expanding the header of a table
void token_ns::expand_star(TokenList &L) {
    TokenList res;
    while (!L.empty()) {
        Token t = L.front();
        if (t.is_a_left_brace()) {
            fast_get_block(L, res);
        } else if (!t.is_star_token()) {
            L.pop_front();
            res.push_back(t);
        } else {
            L.pop_front();
            TokenList u = fast_get_block(L);
            TokenList v = fast_get_block(L);
            remove_ext_braces(u);
            remove_ext_braces(v);
            size_t n = 0;
            while (!u.empty()) {
                Token q = u.front();
                u.pop_front();
                if (!q.is_digit_token()) break;
                n = 10 * n + q.val_as_digit();
                if (n > 1000000) break; // bug?
            }
            while (n > 0) {
                TokenList w = v;
                L.splice(L.begin(), w);
                n--;
            }
        }
    }
    L.swap(res);
}

// Converts the string in the buffer into a token list.
// Everything is of \catcode 12, except space.
// If the switch is true, \n is converted to space, otherwise newline

auto Buffer::str_toks(nl_to_tok nl) -> TokenList {
    TokenList L;
    auto      SP = Token(space_token_val);
    auto      CR = Token(space_t_offset + '\n'); // behaves as space
    auto      NL = Token(other_t_offset + '\n'); // is ^^J
    ptr          = 0;
    for (;;) {
        if (at_eol()) return L;
        codepoint c = next_utf8_char();
        if (c == 0) {
        } // ignore bad chars
        else if (c == ' ')
            L.push_back(SP);
        else if (c == '\n')
            L.push_back(nl == nlt_space ? SP : (nl == nlt_cr ? CR : NL));
        else
            L.push_back(Token(other_t_offset, c));
    }
}

// Use character code 11 whenever possible
auto Buffer::str_toks11(bool nl) -> TokenList {
    auto      SP = Token(space_token_val);
    Token     NL = the_parser.hash_table.newline_token;
    TokenList L;
    ptr = 0;
    for (;;) {
        if (at_eol()) return L;
        codepoint c = next_utf8_char();
        if (c == 0) {
        } // ignore bad chars
        else if (c == ' ')
            L.push_back(SP);
        else if (c == '\n')
            L.push_back(nl ? SP : NL);
        else if (c.is_letter() || c == '@')
            L.push_back(Token(letter_t_offset, c));
        else
            L.push_back(Token(other_t_offset, c));
    }
}

// Converts a string to a token list. If b is true, we add braces.
// NOTE:  in every case converts newline to space
auto token_ns::string_to_list(String s, bool b) -> TokenList {
    Buffer &B = buffer_for_log;
    B << bf_reset << s;
    TokenList L = B.str_toks(nlt_space);
    if (b) the_parser.brace_me(L);
    return L;
}

auto token_ns::string_to_list(const std::string &s, bool b) -> TokenList {
    Buffer &B = buffer_for_log;
    B << bf_reset << s;
    TokenList L = B.str_toks(nlt_space);
    if (b) the_parser.brace_me(L);
    return L;
}

// Converts a istring to a token list.
// Special hack, because we insert the number, not the value
auto token_ns::string_to_list(Istring s) -> TokenList {
    Buffer &B = buffer_for_log;
    B << bf_reset << std::to_string(s.id);
    return B.str_toks(nlt_space);
}

// Prints a token list.
// Note: conversion to log_encoding
auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream & {
    auto C = L.begin();
    auto E = L.end();
    while (C != E) {
        buffer_for_log.reset();
        if (buffer_for_log.push_back(*C)) buffer_for_log << ' ';
        fp << buffer_for_log.c_str();
        ++C;
    }
    return fp;
}

// Prints a token list.
auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream & { return fp << L.c_str(); }

// Puts a macro into a buffer.
void Buffer::push_back(const Macro &x) {
    *this << x[0];
    auto K = x.nbargs;
    if (x.type != dt_optional) {
        for (size_t i = 0; i < K; i++) { *this << fmt::format("#{}", i + 1) << x[i + 1]; }
    } else {
        *this << x[1];
        for (size_t i = 1; i < K; i++) { *this << fmt::format("#{}", i + 1); }
    }
    if (wptr > 0 && at(wptr - 1) == '{') at(wptr - 1) = '#';
    *this << "->" << x.body;
}

// Puts a macro into a buffer.
// Sw is true when we want to print it
void Buffer::push_back(const Macro &x, bool sw) {
    if (!sw)
        push_back(x);
    else {
        Buffer B;
        B.push_back(x);
        push_back(B.convert_to_log_encoding());
    }
}

// Puts a macro definition in a file.
auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream & {
    Buffer &B = buffer_for_log;
    B << bf_reset;
    B.push_back(x, true);
    return fp << B.c_str();
}

auto operator<<(FullLogger &fp, const Macro &x) -> FullLogger & {
    Buffer &B = buffer_for_log;
    B << bf_reset;
    B.push_back(x, true);
    return fp << B.c_str();
}

// Find something in the StrHash table. The buffer mybuf holds the string
// to search. result is never zero
auto StrHash::hash_find(const std::string &s) -> size_t {
    the_parser.my_stats.one_more_sh_find();
    if (s.empty()) return 1;
    auto p = Buffer(s).hashcode(hash_prime) + 3; // skip the special values 0, 1 and 2
    for (;;) {
        if ((at(p).name != nullptr) && s == at(p).name) return p;
        if (at(p).next == 0) break;
        p = at(p).next;
    }
    String name  = (new std::string(s))->c_str();
    auto   value = Buffer(s).convert_to_out_encoding();
    if (at(p).name == nullptr) {
        the_parser.my_stats.one_more_sh_used();
        at(p).name  = name;
        at(p).value = value;
        return p;
    }
    if (hash_last >= size()) resize(size() + 10'000); // \todo simplify using push_back
    auto k = hash_last;
    hash_last++;
    the_parser.my_stats.one_more_sh_used();
    at(k).name  = name;
    at(k).value = value;
    at(p).next  = k;
    return k;
}

auto StrHash::find(String s) -> size_t { return hash_find(s); }
auto StrHash::find(const std::string &s) -> size_t { return hash_find(s); }
auto StrHash::find(int s) -> size_t { return hash_find(fmt::format("{}", s)); }

// if s is the integer associated to 15pt, returns its hash location.
auto StrHash::find_scaled(ScaledInt s) -> Istring {
    Buffer B;
    B.push_back(s, glue_spec_pt);
    return Istring(hash_find(B.to_string()));
}

void Buffer::push_back(const Istring &X) {
    auto v = X.id;
    if (v == 0) return;
    if (v == 1) return;
    push_back(X.p_str());
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
// finishes a sequence of characters.
void Logger::finish_seq() {
    if (!finished) {
        *fp << buffer_for_log2.convert_to_log_encoding();
        *fp << ".\n";
        finished = true;
    }
}

// starts a sequence of characters if needed, adds character c
void Logger::out_single_char(codepoint c) {
    if (finished) {
        finished = false;
        buffer_for_log2.reset();
        buffer_for_log2 << "Character sequence: ";
    }
    buffer_for_log2 << c;
}

auto operator<<(Logger &X, const Macro &x) -> Logger & {
    *(X.fp) << x;
    return X;
}

void Logger::dump(String s) {
    finish_seq();
    *fp << "{\\" << s << "}\n";
}

void Logger::dump0(String s) {
    finish_seq();
    *fp << "{" << s << "}\n";
}

// This prints a control sequence value on the log file.
// Used when tracing a command (catcode not 11 nor 12)
// used in the case {\let\x\y}, after the closing brace.
// It it's not a char, it's a command, with a plain ASCII name.
void Parser::print_cmd_chr(CmdChr X) {
    String a = X.special_name();
    String b = X.name();
    if ((a != nullptr) && (b != nullptr)) { // print both values
        the_log << "\\" << b << " " << a;
        return;
    }
    if (a != nullptr) { // chr
        the_log << a;
        codepoint y(char32_t(X.chr));
        Buffer &  B = buffer_for_log;
        B.reset();
        B.out_log(y, the_main->log_encoding);
        return;
    }
    if (b != nullptr)
        the_log << "\\" << b;
    else
        the_log << "(Unknown " << X.cmd << "," << X.chr << ")";
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

// insert an open brace at the beginning, a close brace at the end.
void Parser::brace_me(TokenList &L) const {
    L.push_front(hash_table.OB_token);
    L.push_back(hash_table.CB_token);
}

void token_ns::add_env(TokenList &L, String name) {
    TokenList L1 = string_to_list(name, true);
    TokenList L2 = L1;
    TokenList res;
    res.push_back(the_parser.hash_table.begin_token);
    res.splice(res.end(), L1);
    res.splice(res.end(), L);
    res.push_back(the_parser.hash_table.end_token);
    res.splice(res.end(), L2);
    swap(L, res);
}

void Buffer::dump_prefix(bool err, bool gbl, symcodes K) {
    if (gbl) push_back("\\global");
    if (K == user_cmd) return;
    if (K == userp_cmd || K == userlp_cmd || K == userpo_cmd || K == userlpo_cmd) {
        if (err)
            push_back("\\");
        else
            insert_escape_char_raw();
        push_back("protected");
    }
    if (K == userl_cmd || K == userlo_cmd || K == userlp_cmd || K == userlpo_cmd) {
        if (err)
            push_back("\\");
        else
            insert_escape_char_raw();
        push_back("long");
    }
    if (K == usero_cmd || K == userlo_cmd || K == userpo_cmd || K == userlpo_cmd) {
        if (err)
            push_back("\\");
        else
            insert_escape_char_raw();
        push_back("outer");
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
    auto C  = L.begin();
    auto E  = L.end();
    int  bl = 0;
    while (C != E) {
        Token x = *C;
        check_brace(x, bl);
        if (bl <= n && x.is_a_char()) {
            codepoint c  = x.char_val();
            auto      sC = s.begin();
            auto      sE = s.end();
            while (sC != sE) {
                if (sC->char_val() == c) *C = *sC;
                break;
                ++sC;
            }
        }
        ++C;
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
        brace_me(key);

        token_ns::double_hack(val);
        if (seen_val) brace_me(val);
        TokenList tmp = seen_val ? cmd : cmd_def;
        R.splice(R.end(), tmp);
        R.splice(R.end(), key);
        if (seen_val) R.splice(R.end(), val);
    }
    if (tracing_macros()) the_log << lg_start << T << lg_arrow << R << lg_end;
    back_input(R);
}

auto token_ns::length_normalise(TokenList &L) -> int {
    auto u = Token(space_t_offset + '\n');
    auto v = Token(space_t_offset + ' ');
    auto A = L.begin();
    auto B = L.end();
    int  n = 0;
    while (A != B) {
        if (*A == u) *A = v;
        ++n;
        ++A;
    }
    return n;
}

auto token_ns::is_sublist(token_iterator A, token_iterator B, int n) -> bool {
    while (n > 0) {
        if (*A != *B) return false;
        ++A;
        ++B;
        --n;
    }
    return true;
}

// Returns true if A is in B. If the switch is true, the value is removed
// but the last token of B is not
// Counts the number of skipped commas.
auto token_ns::is_in(TokenList &A, TokenList &B, bool remove, int &is_in_skipped) -> bool {
    int n = length_normalise(A);
    int m = length_normalise(B);
    int k = m - n;
    if (k < 0) return false;
    auto  AA      = A.begin();
    auto  BB      = B.begin();
    bool  found   = false;
    int   skipped = -1;
    Token to_skip = A.front();
    while (k >= 0) {
        if (*BB == to_skip) ++skipped;
        if (is_sublist(AA, BB, n)) {
            found = true;
            break;
        }
        ++BB;
        --k;
    }
    if (remove && found) {
        auto CC = BB;
        --n;
        while (n > 0) {
            ++CC;
            --n;
        }
        B.erase(BB, CC);
    }
    is_in_skipped = found ? skipped : -1;
    return found;
}
