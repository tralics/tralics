// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2015

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains the TeX parser of tralics

#include "tralics/Logger.h"
#include "tralics/SaveAux.h"
#include "tralics/Saver.h"
#include "tralics/util.h"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {
    struct SpecialHash : public std::unordered_map<std::string, std::string> {
        SpecialHash(const std::string &str) {
            for (const auto &s : split_commas(str)) {
                auto [a, b] = split_assign(s);
                if (find(a) == end()) emplace(a, b);
            }
        }

        [[nodiscard]] auto get(const std::string &x) const -> std::string {
            if (auto i = find(x); i != end()) return i->second;
            return {};
        }
    };

    Buffer trace_buffer, Thbuf2;
} // namespace

namespace token_ns {
    void strip_pt(TokenList &L);
} // namespace token_ns

inline auto boolean(bool x) -> String { return x ? "true" : "false"; }

// --------------------------------------------------
// Reading arguments, groups, etc.

// There are two parameters that control what happens in case
// an outer cmd or a \par is seen.
// If scanner_status is ss_normal then an outer command is valid
// If long_state is ls_long then a \par command is valid
// A \par in a non-long macro or an outer command in an argument aborts expansion.

// scanner_status = ss_macro holds when expanding a macro
// scanner_status = ss_matching when reading arguments of a C++ macro
// in all other cases long_state should be ls_long

// The next command is called when get_token sees EOF or an outer command
// does nothing if scanner_status is ss_normal
// otherwise signals an error depending on the value of this quantity.
// May insert a brace or \fi or \par
// May change long_state to ls_bad
// Replaces offending token by space

void Parser::check_outer_validity() {
    Buffer &B = err_buf;
    B.clear();
    switch (scanner_status) {
    case ss_skipping:
        B += "Incomplete \\if? missing \\fi inserted\n";
        back_input(hash_table.fi_token);
        break;
    case ss_defining:
        B += "Runaway definition?\n";
        back_input(hash_table.CB_token);
        break;
    case ss_absorbing:
        B += "Runaway text?\n";
        back_input(hash_table.CB_token);
        break;
    case ss_macro:
        B += "Runaway argument?\n";
        long_state = ls_bad;
        back_input(hash_table.par_token);
        break;
    default: // matching
        B += "Runaway argument?\n";
    }
    if (cur_cmd_chr.cmd == eof_marker_cmd)
        B += "End of file";
    else
        B += "Forbidden control sequence " + cur_tok.tok_to_str();
    if (scanner_status == ss_skipping)
        B.format(" in conditional started at line {}", conditions.top_line());
    else {
        B += " found while scanning ";
        if (scanner_status == ss_defining)
            B += "definition";
        else if (scanner_status == ss_absorbing)
            B += "text";
        else // ss_matching or ss_macro
            B += "use";
        B += " of " + err_tok.tok_to_str();
    }
    signal_error(err_tok, "Runaway argument");
    if (cur_cmd_chr.cmd == eof_marker_cmd) return;
    cur_tok     = hash_table.space_token;
    cur_cmd_chr = CmdChr(space_catcode, subtypes(' '));
}

// Functions of the form read_something or ignore_something set long_state;
// we start with functions that are called by these.

// Adjusts the brace counter b. Returns true if it becomes 0.
// The scanner returns only balanced lists.
auto Parser::check_brace(int &b) -> bool {
    if (cur_tok.is_a_brace()) {
        if (cur_tok.is_a_left_brace())
            b++;
        else {
            b--;
            if (b == 0) return true;
        }
    }
    return false;
}

// Assumes an open brace has been read. Reads in res everything up to the
// matching close brace. Returns true in case of error (cl is start line).
auto Parser::scan_group0(TokenList &res, int cl) -> bool {
    int b = 1; // brace counter, we start with 1
    for (;;) {
        if (get_token()) {
            missing_close_brace(cl);
            return true;
        }
        if (check_brace(b)) return false;
        if (cur_tok == hash_table.par_token && long_state != ls_long) {
            if (long_state == ls_bad)
                long_state = ls_normal;
            else
                runaway(cl);
            return true;
        }
        if (cur_cmd_chr.cmd == eof_marker_cmd) return true;
        res.push_back(cur_tok);
    }
}

// Same as the function above, but with one argument less
void Parser::skip_group0(TokenList &L) { scan_group0(L, get_cur_line()); }

// Assumes an open brace has been read. Like skip_group0, returns nothing.
// Used by ignore_arg, assumes scanner_status==ss_skipping
void Parser::scan_ignore_group() {
    int cl = get_cur_line();
    int b  = 1;
    for (;;) {
        if (get_token()) {
            missing_close_brace(cl);
            return;
        }
        if (check_brace(b)) return;
        if (cur_tok == hash_table.par_token && long_state != ls_long) {
            runaway(cl);
            return;
        }
    }
}

// In the case of \edef, all tokens are expanded, unless they come from the
// \the or a protected command; these are appended to L by this procedure.
// Returns true if EOF; otherwise cur_tok holds the next token
auto Parser::edef_aux(TokenList &L) -> bool {
    for (;;) {
        if (get_token()) return true;
        if (!cur_cmd_chr.is_expandable()) return false;
        if (cur_cmd_chr.is_protected()) return false;
        if (cur_cmd_chr.cmd != the_cmd)
            expand();
        else {
            Token     T = cur_tok;
            TokenList q = E_the(cur_cmd_chr.chr);
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << T << "->" << q << ".\n";
            }
            L.splice(L.end(), q);
        }
    }
}

// Same as scan_group0, tokens may be expanded if exp is true
// We are reading a macro body with N arguments, so that #1 must be interpreted.
// par tokens are allowed, no runaway test

void Parser::scan_group3(TokenList &res, int cl, bool exp, size_t N) {
    int b = 1;
    for (;;) {
        bool stop = exp ? edef_aux(res) : get_token();
        if (stop) {
            missing_close_brace(cl);
            return;
        }
        if (check_brace(b)) return;
        if (cur_cmd_chr.is_parameter()) after_parameter(exp, N);
        res.push_back(cur_tok);
    }
}

// special case of scan_group3, with exp=true, N=0,
// but there is no closing brace: we read until end of data.
// Thus check_brace() should not yield true

void Parser::scan_group4(TokenList &res, int cl) {
    int b = 1;
    for (;;) {
        if (edef_aux(res)) {
            if (b != 1) missing_close_brace(cl);
            return;
        }
        if (check_brace(b)) {
            parse_error(err_tok, "Too many closing braces");
            return;
        }
        res.push_back(cur_tok);
    }
}

// Used in case of scan_group_del or scan_group_del1
// Contribute a group or a single token to the list res.
// Returns true in case of error.
// Invariant b=-1 is res is empty, b=0 if it contains a single group, b>0 otherwise

auto Parser::scan_group1(TokenList &res, int &b, int cl) -> bool {
    Token aux = cur_tok;
    if (aux.is_OB_token()) {
        res.push_back(cur_tok); // Insert the opening brace
        if (scan_group0(res, cl)) {
            res.push_back(hash_table.CB_token); // Insert the closing brace
            return true;
        }
        res.push_back(cur_tok);
        b++;
        return false;
    }
    if (aux.is_CB_token()) {
        extra_close_brace(cl);
        return true;
    }
    if (aux == hash_table.par_token && long_state != ls_long) {
        if (long_state == ls_bad)
            long_state = ls_normal;
        else
            runaway(cl);
        return true;
    }
    b = 2;
    res.push_back(aux);
    return false;
}

// In the case of \foo {x} and \foo x, this command reads an optional space
// reduces the second case to the first one, and reads an open brace.
// Returns true in case of error. Is used in math mode. Normal arguments
// are read via
//    if(!before_mac_arg()) skip_group0(L);
// or optimised version: scan_group2(L)

auto Parser::before_mac_arg() -> bool {
    for (;;) {
        if (get_token()) {
            runaway(-1);
            return true;
        }
        if (cur_tok.is_space_token()) continue;
        if (cur_tok.is_OB_token()) return false;
        if (cur_tok.is_CB_token()) {
            parse_error(err_tok, "Missing { inserted before unexpected } in argument of ", err_tok, "", "extra}");
            back_input(cur_tok);
            return true;
        }
        back_input(hash_table.CB_token);
        back_input(cur_tok);
        return false;
    }
}

// Inner code of read_arg. This is the command that reads an argument;
// note that outer braces disappear
auto Parser::scan_group2(TokenList &L) -> bool {
    int cl = get_cur_line();
    for (;;) {
        if (get_token()) {
            runaway(-1);
            return true;
        }
        if (cur_tok.is_space_token()) continue;
        if (cur_tok.is_OB_token()) return scan_group0(L, cl);
        if (cur_tok.is_CB_token()) { // the message is stange ...
            parse_error(err_tok, "Missing { inserted before unexpected } in argument of ", err_tok, "", "extra}");
            return true;
        }
        if (cur_tok == hash_table.par_token && long_state != ls_long) {
            if (long_state == ls_bad)
                long_state = ls_normal;
            else
                runaway(cl);
            return true;
        }
        L.push_back(cur_tok);
        return false;
    }
}

// Special case of read_delimited, in case a single token is wanted.
// Gathers all tokens until finding x
// A newline token is identified with a space token.
// If this is a group (b==0), then outer braces will be removed;
auto Parser::scan_group_del1(TokenList &res, Token x) -> bool {
    int cl = get_cur_line();
    int b  = -1;
    for (;;) {
        if (get_token()) {
            bad_delimited(cl, x);
            return true;
        }
        if (x.is_same_token(cur_tok)) break;
        if (scan_group1(res, b, cl)) return true;
    }
    if (b == 0) { // Remove external braces
        res.pop_back();
        res.pop_front();
    }
    return false;
}

// reads a token list R followed by L (which is discarded).
// At every stage we check that L is present; if not so, a token or a group is read
// outer braces are removed if possible.
auto Parser::scan_group_del(TokenList &res, const TokenList &L) -> bool {
    int cl = get_cur_line();
    int b  = -1;
    for (;;) {
        // First check if L is found
        TokenList backup;
        auto      C = L.begin();
        auto      E = L.end();
        for (;;) {
            if (C == E) { // ok found
                if (b == 0) {
                    res.pop_back();
                    res.pop_front();
                }
                return false;
            }
            Token x = *C;
            ++C;
            if (get_token()) { // EOF abort
                bad_delimited(cl, Token(0));
                return true;
            }
            backup.push_back(cur_tok);
            if (x.is_same_token(cur_tok)) continue;
            back_input(backup);
            break;
        }
        if (get_token()) {
            bad_delimited(cl, Token(0));
            return true;
        }
        if (scan_group1(res, b, cl)) return true;
    }
}

// Reads the tokens of the list L
// in the case below, a closing brace is inserted so that no runaway is signaled.
// \def\tfoo a{} \edef\xx{\tfoo}
auto Parser::skip_prefix(const TokenList &L) -> bool {
    auto C = L.begin();
    auto E = L.end();
    while (C != E) {
        Token T = *C;
        ++C;
        get_token();
        if (cur_tok.is_CB_token()) back_input();
        if (!T.is_same_token(cur_tok)) {
            bad_macro_prefix(cur_tok, T);
            return true;
        }
    }
    return false;
}

// Reads a LaTeX optional argument, enclosed in brackets
// Initial spaces are discarded. Return false in no bracket found
auto Parser::scan_group_opt(TokenList &L, bool &have_arg) -> bool {
    for (;;) {
        if (get_token()) return true;
        if (cur_tok.is_space_token()) continue;
        if (cur_tok.is_open_bracket()) {
            have_arg = true;
            return scan_group_del1(L, Token(other_t_offset, ']'));
        }
        back_input();
        return false;
    }
}

// --------------------------------------------------
// higher level functions. These set scanner_status to ss_matching at the start,
// and normal at the end, and may locally set long state.
// Note: scan_group_opt, scan_group2, scan_group_del,
// are used by expand_mac that temporarily resets scanner_status.
// Return value does noy say if an error occured

auto Parser::read_optarg(TokenList &L) -> bool {
    scanner_status = ss_matching;
    bool retval    = false;
    scan_group_opt(L, retval);
    scanner_status = ss_normal;
    return retval;
}

void Parser::ignore_optarg() {
    TokenList L;
    auto      guard = SaveLongState(ls_normal); // \par forbidden
    read_optarg(L);
}

// The three next commands set locally long_state to normal
// Reads an optional argument in L, true if found
auto Parser::read_optarg_nopar(TokenList &L) -> bool {
    auto guard = SaveLongState(ls_normal);
    return read_optarg(L);
}

// Read an argument delimited by a token
auto Parser::read_until_nopar(Token x) -> TokenList {
    TokenList res;
    auto      guard = SaveLongState(ls_normal);
    scanner_status  = ss_matching;
    scan_group_del1(res, x);
    scanner_status = ss_normal;
    return res;
}

// This reads all tokens before x. The scanner status is set to matching
// and to normal at the end.
auto Parser::read_until(Token x) -> TokenList {
    TokenList res;
    auto      guard = SaveLongState(ls_long);
    scanner_status  = ss_matching;
    scan_group_del1(res, x);
    scanner_status = ss_normal;
    return res;
}

// Argument of macro. A single token, or a brace delimited non-expanded list
auto Parser::read_arg() -> TokenList {
    TokenList L;
    auto      guard = SaveLongState(ls_long);
    scanner_status  = ss_matching;
    scan_group2(L);
    scanner_status = ss_normal;
    return L;
}

// Read a normal argument
auto Parser::read_arg_nopar() -> TokenList {
    TokenList L;
    auto      guard = SaveLongState(ls_normal);
    scanner_status  = ss_matching;
    scan_group2(L);
    scanner_status = ss_normal;
    return L;
}

// Argument of macro that should be a single token
// sets cur_tok and cmd_chr,
auto Parser::read_token_arg(Token T) -> bool {
    auto guard1    = SaveLongState(ls_long);
    auto guard2    = SaveErrTok(T);
    scanner_status = ss_matching;
    bool res       = read_token_arg(get_cur_line());
    scanner_status = ss_normal;
    return res;
}

auto Parser::read_token_arg(int cl) -> bool {
    TokenList L;
    for (;;) {
        if (get_token()) {
            runaway(-1);
            return true;
        }
        if (cur_tok.is_space_token()) continue;
        if (cur_tok.is_OB_token()) {
            scan_group0(L, cl);
            if (L.size() == 1) {
                back_input(L);
                get_token();
                return false;
            }
            err_one_arg(L);
            return true;
        }
        if (cur_tok.is_CB_token()) {
            parse_error(err_tok, "Extra } ignored", "extra}");
            return true;
        }
        return false;
    }
}

// Paragraphs are forbidden in ignored arguments
// Use read_arg() if \par allowed
void Parser::ignore_arg() {
    auto guard     = SaveLongState(ls_normal);
    scanner_status = ss_matching;
    skip_initial_space();
    if (cur_tok.is_invalid())
        runaway(-1);
    else if (cur_tok.is_OB_token())
        scan_ignore_group();
    scanner_status = ss_normal;
}

// In a case like \def\bar#1foo#2{...}, this returns the value of #1
// given the token list foo as argument.
// NOTE. used only in \for loops and result ignored.
auto Parser::read_delimited(const TokenList &L) -> TokenList {
    TokenList res;
    scanner_status = ss_matching;
    scan_group_del(res, L);
    scanner_status = ss_normal;
    return res;
}

// Reads the body of a \def ou \xdef. A space is skipped,
// and an open brace is read first.
void Parser::read_mac_body(TokenList &L, bool exp, size_t N) {
    scanner_status = ss_defining;
    skip_initial_space();
    if (cur_cmd_chr.is_open_brace())
        scan_group3(L, get_cur_line(), exp, N);
    else
        missing_open_brace();
    scanner_status = ss_normal;
}

// This is like scan_toks(false,true) in TeX; but the token list has already been
// read.
// This feeds the list L to the body of an edef.
// The expansion of L will be in L. The loop stops at the end of L
// at brace_level zero (b=1)
void Parser::read_toks_edef(TokenList &L) {
    scanner_status = ss_absorbing;
    SaveState sv_state(TL, restricted);
    restricted = true;
    TL.swap(L);                     // now L is empty
    scan_group4(L, get_cur_line()); // This fills L
    scanner_status = ss_normal;
}

// --------------------------------------------------

// Adds the content of the buffer to the document-hook token list.
void Parser::add_buffer_to_document_hook(Buffer &b, const std::string &name) {
    TokenList L = tokenize_buffer(b, name);
    document_hook.splice(document_hook.end(), L);
}

// Evaluates now a token string. First action is to put chars in a buffer
// (because we add a '\n' at the end of the string).
void Parser::titlepage_evaluate(const std::string &s, const std::string &cmd) {
    TokenList L = tokenize_buffer(s, "(tpa post " + cmd + ")");
    T_translate(L);
}

// Puts in cur_tok the next non-expandable token.
auto Parser::get_x_token() -> bool {
    for (;;) {
        if (get_token()) return true;
        if (cur_cmd_chr.is_expandable())
            expand();
        else
            return false;
    }
}

// Returns true if the next line contains \end{env}
// In this case, removes the \end{env}
auto Parser::is_verbatim_end() -> bool {
    file_ended = false;
    if (at_eol()) get_a_new_line();
    if (file_ended) return false;
    bool res = input_buffer.contains_env(get_cur_env_name());
    if (res) {
        if (input_buffer.head() == 0)
            kill_line();
        else { // remove chars from input_line
            auto n = input_line.size();
            for (size_t i = 0; i < n; i++)
                if (input_line[i] == '}') {
                    input_line_pos = i + 1;
                    break;
                }
        }
    }
    return res;
}

// Reads all characters until finding test; most character have catcode 12
// but some non-letters have catcode 11. result is \verbatimprefix{\verbatim foo},
// pushed at the end of L (or with prefix  \verbprefix pushed at the start of L).
// Returns true in case of problem

auto Parser::vb_tokens(char32_t test, TokenList &L, bool before) -> bool {
    TokenList res;
    state = state_M;
    for (;;) {
        if (at_eol()) return true;
        char32_t c = get_next_char();
        if (c == test) break;
        res.push_back(verbatim_chars[c]);
    }
    if (res.empty() && !before) res.push_back(hash_table.tilda_token);
    res.push_front(hash_table.verbatim_font);
    brace_me(res);
    if (before)
        res.push_front(hash_table.verb_prefix_token);
    else
        res.push_front(hash_table.verbatim_prefix_token);
    if (before)
        L.splice(L.begin(), res);
    else
        L.splice(L.end(), res);
    return false;
}

// Case of \begin{verbatim} and variants.
// Locally sets \endlinechar to CR, and reads each line via vb_tokens
void Parser::T_verbatim(int my_number, Token style, Token pre, Token post) {
    TokenList res;
    kill_line();
    Token par                            = hash_table.par_token;
    Token noindent                       = hash_table.noindent_token;
    auto  cc                             = eqtb_int_table[endlinechar_code].val;
    eqtb_int_table[endlinechar_code].val = '\r';

    bool ok          = true;
    bool want_number = false;
    long n           = 0;
    if (my_number >= 0) {
        my_number += count_reg_offset;
        n           = eqtb_int_table[to_unsigned(my_number)].val;
        want_number = true;
    }
    int cl = get_cur_line();
    for (;;) {
        if (is_verbatim_end()) break;
        if (file_ended) {
            ok = false;
            break;
        }
        // add \par\noindent before each line
        res.push_back(par);
        res.push_back(noindent);
        n++;
        if (want_number) {
            word_define(to_unsigned(my_number), n, true);
            res.add_verbatim_number(hash_table, n);
        }
        if (vb_tokens(char32_t('\r'), res, false)) {
            ok = false;
            break;
        }
    }
    if (!ok) bad_end_env(cl);
    eqtb_int_table[endlinechar_code].val = cc; // restore
    // pop save stack
    cur_tok.kill();
    pop_level(bt_env);
    if (style != hash_table.relax_token) res.push_front(style);
    if (pre != hash_table.relax_token) res.push_front(pre);
    if (post != hash_table.relax_token) res.push_back(post);
    remove_initial_space_and_back_input();
    if (cur_cmd_chr.cmd != par_cmd) { // add final \par\noindent
        res.push_back(par);
        res.push_back(noindent);
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{Verbatim tokens: " << res << "}\n";
    }
    back_input(res);
}

// Caller of the previous. There could be an optional argument, a list to be
// interpreted. Needs to be completed.

void Parser::T_verbatim() {
    bool extended = cur_cmd_chr.chr == one_code;
    bool noparse  = cur_cmd_chr.chr == two_code;
    bool optional = false;
    if (extended || noparse) {
        // do not read the next line (unless we have a % here)
        for (;;) {
            if (get_token()) break;
            if (cur_tok.is_open_bracket()) {
                optional = true;
                back_input();
                break;
            }
            if (cur_tok.is_only_space_token()) continue; // ok for space, not newline.
            back_input();
            break;
        }
    }
    // Now, we know if we have an optional argument.
    TokenList largs;
    if (optional) read_optarg(largs);
    std::string hook = get_cur_env_name() + "@hook";
    if (noparse) {
        Token t1 = hash_table.locate(hook);
        new_macro(largs, t1, true); // definition is outside env
        Token t2 = hash_table.locate("FV@style@lst");
        Token t3 = hash_table.locate("FV@pre@lst");
        Token t4 = hash_table.locate("FV@post@lst");
        T_verbatim(-1, t2, t3, t4);
        return;
    }
    TokenList lopt = get_mac_value(hook);
    largs.push_back(hash_table.comma_token);
    largs.splice(largs.end(), lopt);
    std::string args        = to_stringE(largs);
    bool        want_number = false;
    SpecialHash S(args);
    //  Check if numbering wanted
    if (!S.get("numbers").empty()) want_number = true;
    if (!extended && numbered_verbatim) want_number = true;
    bool reset = true;
    if (!extended) reset = false;
    // Check if a register number is given
    int reg_number = find_counter(S.get("counter"));
    if (reg_number >= 0) reset = false;
    // Check if a start number is given
    int         n = 0;
    std::string w = S.get("firstnumber");
    if (w.empty())
        n = 1;
    else if (w == "last") {
        reset       = false;
        want_number = true;
    } else if (only_digits(w)) {
        reset       = true;
        n           = std::stoi(w);
        want_number = true;
    } else {
        reset = true;
        n     = 0;
    }
    if (want_number && reg_number < 0) reg_number = 21; // hardcoded
    if (reset) word_define(to_unsigned(reg_number + to_signed(static_cast<size_t>(count_reg_offset))), n - 1, true);
    Token       t2 = hash_table.relax_token;
    Token       t3 = hash_table.relax_token;
    Token       t4 = hash_table.relax_token;
    std::string w2 = S.get("style");
    std::string w3 = S.get("pre");
    if (!w2.empty()) t2 = hash_table.locate("FV@style@" + w2);
    if (!w3.empty()) t3 = hash_table.locate("FV@pre@" + w3);
    if (!w3.empty()) t4 = hash_table.locate("FV@post@" + w3);
    T_verbatim(reg_number, t2, t3, t4);
}

// Still incomplete
void Parser::special_fvset() {
    flush_buffer();
    auto s = SpecialHash(sT_arg_nopar()).get("showspaces");
    if (s.empty()) return;
    if (s == "true")
        verbatim_chars[uchar(' ')] = hash_table.textvisiblespace_token;
    else if (s == "false")
        verbatim_chars[uchar(' ')] = hash_table.tilda_token;
}

// In the case of \verb+foo+, reads and returns the + character.
// Sets special_space to true if spaces have to be treaded specially
// Returns null in case of error
auto Parser::delimiter_for_verb(bool &special_space) -> char32_t {
    char32_t t = get_next_char();
    if (!(t == '*')) return t;
    if (at_eol()) return char32_t();
    special_space = true;
    return get_next_char();
}

// In the case of \SaveVerb{ok}+foo+, reads and returns the + character,
// which must have spacial_catcode. returns 0 in case of trouble
auto Parser::delimiter_for_saveverb() -> char32_t {
    for (;;) {
        if (at_eol()) return char32_t();
        char32_t c = get_next_char();
        if (c == 0) return c;
        if (c > 65535) return char32_t();
        if (get_catcode(c) == space_catcode) continue;
        if (get_catcode(c) == special_catcode) return c;
        return char32_t();
    }
}

// Case of the \verb command (t=0) or |foo|, where t is the char
void Parser::T_verb(char32_t t) {
    Token T = cur_tok;
    if (!TL.empty()) {
        verb_error(T, 0);
        return;
    }
    if (at_eol()) {
        verb_error(T, 1);
        return;
    }
    bool special_space = false;
    if (t == 0) t = delimiter_for_verb(special_space);
    if (t == 0) {
        verb_error(T, 1);
        return;
    }

    auto saved = verbatim_chars[uchar(' ')]; // \todo use Saver for this
    if (special_space) verbatim_chars[uchar(' ')] = hash_table.textvisiblespace_token;
    if (vb_tokens(t, TL, true)) verb_error(T, 2);
    verbatim_chars[uchar(' ')] = saved;
}

// Case of the \SaveVerb command
void Parser::T_saveverb() {
    Token T = cur_tok;
    E_useverb();
    get_token();
    Token t = cur_tok; // the token to define
    if (!TL.empty()) {
        verb_error(T, 0);
        return;
    }
    char32_t c = delimiter_for_saveverb();
    if (c == 0) {
        verb_error(T, 3);
        return;
    }
    if (at_eol()) {
        verb_error(T, 1);
        return;
    }
    TokenList res;
    if (vb_tokens(c, res, true))
        verb_error(T, 2);
    else
        new_macro(res, t, true);
}

// \begin{comment}...\end{comment} or \begin{rawxml}...\end{rawxml}
// The env macro will read the \end{...}, so that we pop the stack.
// we make sure that reading is not from a token list
// Ignores everything, until end of environment.
// or take it verbatim
auto Parser::T_raw_env(bool want_result) -> std::string {
    kill_line();
    if (!TL.empty()) {
        parse_error(err_tok, "Verbatim-like environment in argument : ", get_cur_env_name(), "Verbatim-like environment in argument");
        cur_tok.kill();
        pop_level(bt_env);
        return "";
    }
    mac_buffer.clear();
    int cl = get_cur_line();
    for (;;) {
        if (is_verbatim_end()) break;
        if (file_ended) {
            bad_end_env(cl);
            break;
        }
        if (want_result) {
            mac_buffer.append(input_buffer);
            mac_buffer.push_back('\n');
        }
        kill_line();
    }
    cur_tok.kill();
    pop_level(bt_env);
    return want_result ? static_cast<std::string>(mac_buffer) : "";
}

// --------------------------------------------------
// Space hacking

void Parser::back_input_unless_space() {
    if (cur_tok.is_invalid()) return;
    if (cur_cmd_chr.is_space()) return;
    back_input();
}

void Parser::read_one_space() {
    get_x_token();
    back_input_unless_space();
}

// This sets cur_tok to a token that has to be handled.
void Parser::remove_initial_space() {
    for (;;) {
        get_x_token();
        if (!cur_cmd_chr.is_space()) return;
    }
}

void Parser::remove_initial_space_and_back_input() {
    for (;;) {
        get_x_token();
        if (!cur_cmd_chr.is_space()) break;
    }
    if (cur_tok.is_valid()) back_input();
}

// This sets cur_tok to a token that has to be handled.
void Parser::remove_initial_space_relax() {
    for (;;) {
        get_x_token();
        if (!cur_cmd_chr.is_space() && !cur_cmd_chr.is_relax()) return;
    }
}

// No expand here ...
// This sets cur_tok to a token that has to be handled.
void Parser::skip_initial_space() {
    for (;;) {
        get_token();
        if (!cur_cmd_chr.is_space()) return;
    }
}

void Parser::skip_initial_space_and_back_input() {
    skip_initial_space();
    if (cur_tok.is_valid()) back_input();
}

void Parser::back_input_braced(TokenList &L) {
    back_input(hash_table.CB_token);
    back_input(L);
    back_input(hash_table.OB_token);
}

// Case of for instance: \catcode`A \space=2.
// Reads up to the equals sign
void Parser::scan_optional_equals() {
    for (;;) {
        if (get_x_token()) return;
        if (!cur_cmd_chr.is_space()) break;
    }
    if (!cur_tok.is_equal_token()) back_input();
}

// Scans the left brace in the case of \hbox{}.
void Parser::scan_left_brace() {
    remove_initial_space_relax();
    if (!cur_cmd_chr.is_open_brace()) {
        parse_error(err_tok, "A left brace was mandatory here, so I've put one in", "Missing open brace");
        if (cur_tok.is_valid()) back_input();
    }
}

// Used by unimplemenrted commands like \vadjust
// reads a mandatoty open brace, and reinserts it
void Parser::scan_left_brace_and_back_input() {
    scan_left_brace();
    back_input(hash_table.OB_token);
}

// Reads an optional *. Returns true if we have seen one.
auto Parser::remove_initial_star() -> bool {
    skip_initial_space();
    if (cur_tok.is_star_token()) return true;
    if (cur_tok.is_valid()) back_input();
    return false;
}

// This is done if we see a # while reading the body of a command.
// if exp is true, the token that follows the # is expanded,
void Parser::after_parameter(bool exp, size_t N) {
    Token s    = cur_tok;
    bool  stop = exp ? get_x_token() : get_token();
    if (stop) {
        parse_error(err_tok, "end of data instead of a number after #");
        return;
    }
    if (cur_cmd_chr.is_parameter()) return; //  ## gives #
    if (cur_tok.is_digit_token()) {
        auto n = cur_cmd_chr.val_as_digit();
        if (0 < n && n <= N) {
            cur_tok = Token(eol_t_offset + n);
            return;
        }
    }
    parse_error(err_tok, "bad token (", cur_tok, ") after #", "bad after #");
    back_input(cur_tok);
    cur_tok = s; // #0 is tread as ##0
}

// The token list read by \mark or \marks NNN is unused.
void Parser::T_mark(subtypes c) {
    Token T = cur_tok;
    if (c == marks_code) scan_int(T);
    TokenList L = scan_general_text();
    read_toks_edef(L);
}

// a special kind of csname, without expand. Returns a name
auto Parser::group_to_string() -> std::string {
    skip_initial_space();
    if (!cur_cmd_chr.is_open_brace()) {
        missing_open_brace();
        return "";
    }
    group_buffer.clear();
    for (;;) {
        get_token();
        symcodes S = cur_cmd_chr.cmd;
        if (S == close_catcode) return group_buffer;
        if (S == space_catcode)
            group_buffer.push_back(' ');
        else if (S > 2 && S < 16)
            group_buffer.push_back(cur_cmd_chr.char_val());
        else {
            bad_group_char();
            return "";
        }
    }
}

// Backinput \end{argument} (without the \end token).
void Parser::unskip_group(String s) {
    TokenList L = token_ns::string_to_list(s, true);
    back_input(L);
}

auto Parser::sE_arg() -> std::string {
    TokenList L = read_arg();
    return to_stringE(L);
}

auto Parser::sE_arg_nopar() -> std::string {
    TokenList L = read_arg_nopar();
    return to_stringE(L);
}

// Returns next arg as a string (not translated)
auto Parser::sE_optarg_nopar() -> std::string {
    TokenList L;
    read_optarg_nopar(L);
    return to_stringE(L);
}

// Here, we expand all tokens, and gather all non-expandable tokens
auto Parser::to_stringE(TokenList &L) -> std::string {
    read_toks_edef(L);
    Buffer &B = group_buffer;
    B.clear();
    B << L;
    return B;
}

// Scan_general_text is like scan_toks(false,false).
// It can be recursive, hence scanner_status not reset to normal
auto Parser::scan_general_text() -> TokenList {
    TokenList L;
    scan_left_brace();
    auto guard1 = SaveLongState(ls_long); // allows \par
    auto guard2 = SaveScannerStatus(ss_absorbing);
    scan_group0(L, get_cur_line());
    return L;
}

// number of arguments for \newcommand; tries to be clever.
auto Parser::read_mac_nbargs() -> size_t {
    TokenList L;
    read_optarg(L);
    if (!L.empty() && L.front().is_plus_token()) L.pop_front();
    while (!L.empty() && L.front().is_zero_token()) L.pop_front();
    if (L.empty()) return 0; // case of \newcommand\x[]{x}
    Token t = token_ns::get_unique(L);
    if (t.is_null()) {
        bad_nbargs(-1);
        return 0;
    }
    if (!t.is_a_char()) {
        bad_nbargs(-2);
        return 0;
    }
    if (t.cmd_val() == other_catcode) {
        auto tt = t.val_as_digit();
        if (tt >= 0 && tt <= 9) return tt;
    }
    bad_nbargs(-3);
    return 0;
}

// scans the `#1#2foo' part of \def\bar#1#2foo{}
void Parser::get_def_nbargs(Macro *X, Token name) {
    def_type  code = dt_normal;
    size_t    nb   = 0;
    TokenList L;
    scanner_status = ss_defining;
    for (;;) {
        if (get_token()) break; // will signal an error later
        if (cur_cmd_chr.is_parameter()) {
            if (get_token()) break;
            if (cur_cmd_chr.is_other() && cur_cmd_chr.chr == nb + '1') {
                X->set_delimiters(nb, L);
                L.clear();
                nb++;
            } else if (cur_cmd_chr.is_open_brace()) {
                code = dt_brace;
                L.push_back(cur_tok);
                break;
            } else
                bad_definition(name, nb);
            continue;
        }
        if (!cur_tok.is_a_char()) // \bgroup will not match here
        {
            code = dt_delim;
            L.push_back(cur_tok);
            continue;
        }
        if (cur_cmd_chr.cmd == open_catcode) break;  // start of body
        if (cur_cmd_chr.cmd == close_catcode) break; // will signal an error later
        code = dt_delim;
        L.push_back(cur_tok);
    }
    scanner_status = ss_normal;
    X->set_delimiters(nb, L);
    X->set_nbargs(nb);
    X->set_type(code);
}

// Changes the type of a trivial macro.
void Macro::correct_type() {
    if (type == dt_empty) type = dt_normal; // Is this needed ?
    if (type == dt_normal && body.empty() && nbargs == 0) type = dt_empty;
    if (type == dt_optional && nbargs > 1) {
        const TokenList &L = delimiters[1];
        auto             A = L.begin();
        auto             B = L.end();
        if (A == B) return;
        if (*A != the_parser.hash_table.dblarg_token) return;
        ++A;
        if (A != B) return;
        type = dt_spec_opt;
    }
}

// case of \def... or \edef...
void Parser::M_def(bool edef, bool gbl, symcodes what, rd_flag fl) {
    skip_initial_space();
    Token name = cur_tok;
    if (cur_tok.not_a_cmd()) bad_redefinition(2, name);
    auto *X = new Macro;
    {
        auto guard = SaveErrTok(name);
        get_def_nbargs(X, name);
        back_input();
        read_mac_body(X->body, edef, X->nbargs);
        X->correct_type();
    }
    mac_define(name, X, gbl, fl, what);
}

// \DeclareMathOperator*{foo}{bar} is
// \newcommand{\foo}{\operatorname*{bar}}
// The star is optional
void Parser::M_declare_math_operator() {
    bool  see_star = remove_initial_star();
    Token name     = get_r_token(true);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\DeclareMathOperator " << name << "}\n";
    }
    auto *     X = new Macro;
    TokenList &L = X->body;
    read_mac_body(L, false, 0);
    brace_me(L);
    if (see_star) L.push_front(hash_table.star_token);
    L.push_front(hash_table.operatorname_token);
    mac_define(name, X, true, rd_if_undef, user_cmd);
}

// \newtheorem{name}{text}[c]    case 1
// \newtheorem{name}{text}       case 2
// \newtheorem{name}[c]{text}    case 3
// \newtheorem*{name}{text}      case 4
void Parser::M_new_thm() {
    bool        star = remove_initial_star();
    std::string name = group_to_string();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\newtheorem " << name << "}\n";
    }
    TokenList ctr;
    int       which_case = star ? 4 : 2;
    if (which_case == 2 && read_optarg_nopar(ctr)) which_case = 3;
    TokenList text = read_arg();
    if (which_case == 2 && read_optarg_nopar(ctr)) // ctr is empty here
        which_case = 1;
    // We have now read the arguments.
    Token y = find_env_token(name, true);
    err_tok = y; // replaces \newtheorem by \name
    if (!ok_to_define(y, rd_if_undef)) return;
    find_env_token(name, false);
    M_let_fast(cur_tok, hash_table.eth_token, true); // defines \endname
    Token thename_cmd;
    brace_me(text);
    if (which_case == 4) {
        text.push_back(hash_table.OB_token);
        text.push_back(hash_table.CB_token);
    } else {
        Thbuf1      = "the" + name;
        thename_cmd = hash_table.locate(Thbuf1);
        M_let_fast(thename_cmd, hash_table.relax_token, true); // in case of error
        text.push_back(thename_cmd);
    }
    TokenList aux = toks_registers[theorem_bodyfont_code].val;
    brace_me(aux);
    text.splice(text.end(), aux);
    aux = token_ns::string_to_list(name, true);
    text.splice(text.end(), aux);
    if (which_case == 3) {
        aux = ctr;
        brace_me(aux);
    } else
        aux = token_ns::string_to_list(name, true);
    text.splice(text.end(), aux);
    aux = toks_registers[theorem_style_code].val;
    brace_me(aux);
    text.splice(text.end(), aux);
    text.push_front(hash_table.bth_token);
    new_macro(text, y, true); // This defines \name
    if (which_case == 4) return;
    if (which_case == 3) {
        if (csname_ctr(ctr, Thbuf1)) {
            bad_counter0();
            return;
        }
        if (counter_check(Thbuf1, false)) return; // checks the counter
        Thbuf2  = "the" + Thbuf1.substr(2);
        cur_tok = hash_table.locate(Thbuf2);
        TokenList R;
        R.push_back(cur_tok);
        new_macro(R, thename_cmd, true);
        return;
    }
    TokenList the_value = token_ns::string_to_list(name, true);
    TokenList V         = the_value;
    the_value.push_front(hash_table.arabic_token);
    if (which_case == 2) {
        back_input(hash_table.relax_token);
    } else {
        back_input(Token(other_t_offset, ']'));
        TokenList ccopy = ctr;
        back_input(ccopy);
        back_input(Token(other_t_offset, '['));
        Thbuf2 = "the";
        if (list_to_string(ctr, Thbuf2)) parse_error(err_tok, "bad counter");
        Token x = hash_table.locate(Thbuf2);
        the_value.push_front(Token(other_t_offset, '.'));
        the_value.push_front(x);
    }
    back_input(V);
    if (!M_counter(true)) new_macro(the_value, thename_cmd, true);
}

void Parser::T_end_theorem() {
    leave_h_mode();
    the_stack.pop(the_names["theorem"]);
    the_stack.add_nl();
}

void Parser::T_start_theorem(int c) {
    TokenList name      = read_arg();
    TokenList ctr       = read_arg(); // empty in case of theorem*
    TokenList font2     = read_arg();
    TokenList me        = read_arg();
    TokenList mecounter = read_arg();
    TokenList style     = read_arg();
    TokenList opt;
    bool      was_o = read_optarg(opt);
    leave_h_mode();
    the_stack.push1(the_names["theorem"], the_names["np_theorem"]);
    the_stack.set_v_mode();
    bool noref = ctr.empty();
    if (!noref) {
        refstepcounter(mecounter, true);
        T_translate(mecounter);
    }
    if (c == 0) {
        Xid id1 = the_stack.get_xid();
        leave_v_mode();
        Xid id2 = the_stack.get_xid();
        if (!(id1 == id2) && the_names["np_theorem"].empty()) id2.add_attribute(id1);
    }
    if (c == 0) {
        if (!noref) name.push_back(hash_table.space_token);
        name.splice(name.end(), ctr);
        if (was_o) {
            name.push_back(hash_table.space_token);
            name.push_back(Token(other_t_offset, '('));
            name.splice(name.end(), opt);
            name.push_back(Token(other_t_offset, ')'));
        }
        brace_me(name);
        TokenList font1 = toks_registers[theorem_headerfont_code].val;
        name.splice(name.begin(), font1);
        brace_me(name);
        name.push_back(hash_table.space_token);
        name.splice(name.end(), font2);
        back_input(name);

    } else {
        the_stack.set_arg_mode();
        the_stack.add_nl();
        back_input_braced(name);
        T_arg1(the_names["head"]);
        the_stack.add_nl();
        back_input_braced(me);
        std::string n = nT_arg_nopar();
        the_stack.add_att_to_cur(the_names["type"], n);
        back_input_braced(style);
        n = nT_arg_nopar();
        the_stack.add_att_to_cur(the_names["style"], n);
        if (was_o) {
            back_input_braced(opt);
            T_arg1(the_names["theorem_head"]);
            the_stack.add_nl();
        }
        the_stack.set_v_mode();
        remove_initial_space_and_back_input();
    }
}

// Returns all tokens before the \end at level zero
void Parser::grab_env(TokenList &v) {
    int b      = 1;
    int elevel = 0;
    for (;;) {
        if (get_token()) {
            parse_error(err_tok, "End of data reached while scanning content of ", get_cur_env_name(), "bad grab env");
            return;
        }
        if (check_brace(b)) {
            parse_error(err_tok, "Spurious close brace removed");
            ++b;
            continue;
        }
        if (b == 1 && cur_cmd_chr.cmd == begin_cmd) elevel++;
        if (b == 1 && cur_cmd_chr.cmd == end_cmd) {
            if (elevel == 0) {
                back_input();
                return;
            }
            elevel--;
        }
        v.push_back(cur_tok);
    }
}

// Idem, but stops at comma at level 0, returning false
// uses get_x_token
auto Parser::grab_env_comma(TokenList &v) -> bool {
    int b      = 1;
    int elevel = 0;
    for (;;) {
        if (get_x_token()) {
            parse_error(err_tok, "End of data reached while scanning content of ", get_cur_env_name(), "bad grab env");
            return true;
        }
        if (check_brace(b)) {
            parse_error(err_tok, "Spurious close brace removed");
            ++b;
            continue;
        }
        if (b == 1 && elevel == 0 && cur_tok.is_comma_token()) return false;
        if (b == 1 && cur_cmd_chr.cmd == begin_cmd) elevel++;
        if (b == 1 && cur_cmd_chr.cmd == end_cmd) {
            if (elevel == 0) {
                back_input();
                return true;
            }
            elevel--;
        }
        v.push_back(cur_tok);
    }
}

//  Macro expansion ------------------------------

// This is macro_call in TeX
// Assumes that cur_tok holds the macro name, the argument the value.
void Parser::expand_mac(Macro &X) {
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << " " << X << "\n";
    }
    def_type spec = X.type;
    if (spec == dt_empty) return;
    if (skip_prefix(X[0])) return;
    bool                      optional      = spec == dt_optional || spec == dt_spec_opt;
    bool                      spec_optional = spec == dt_spec_opt;
    auto                      n             = X.nbargs;
    std::array<TokenList, 10> arguments;
    for (size_t i = 1; i <= n; i++) {
        if (spec == dt_delim || spec == dt_brace) {
            if (!X[i].empty()) {
                if (scan_group_del(arguments[i], X[i])) return;
            } else if (scan_group2(arguments[i]))
                return;
        } else if (!optional) {
            if (scan_group2(arguments[i])) return;
        } else {
            optional = false;
            TokenList u;
            bool      have_arg = false;
            if (scan_group_opt(u, have_arg)) return;
            if (have_arg)
                arguments[i] = u;
            else {
                if (spec_optional) { // value of #1 defaults to #2 which is scanned here
                    if (scan_group2(arguments[1])) return;
                    i++;
                    arguments[2] = arguments[1];
                } else
                    arguments[1] = X[1];
            }
        }
        if (tracing_macros()) the_log << "#" << i << "<-" << arguments[i] << "\n";
    }
    TokenList res = expand_mac_inner(X.body, arguments.data()); // \todo pass the array instead
    if (spec == dt_brace) res.push_back(hash_table.OB_token);
    back_input(res);
}

// This is the code that replaces arguments by values in the body.
// note that Tex uses a completely different method (there is a stack with
// all arguments of all macros; here we have just one table).
auto Parser::expand_mac_inner(const TokenList &W, TokenList *arguments) -> TokenList {
    auto      C = W.begin();
    auto      E = W.end();
    TokenList res;
    while (C != E) {
        Token x = *C;
        ++C;
        if (x.is_a_char() && x.cmd_val() == eol_catcode) {
            int       k  = x.chr_val();
            TokenList ww = arguments[k];
            res.splice(res.end(), ww);
        } else
            res.push_back(x);
    }
    return res;
}

// -------------------
// Converting a list of token into a string or a CS name

// returns true if bad; fills the buffer b until finding \endcsname
auto Parser::list_to_string0(Buffer &b) -> bool {
    for (;;) {
        if (get_x_token()) return true;
        if (cur_cmd_chr.cmd == endcsname_cmd) return false;
        if (cur_tok.not_a_cmd()) {
            b.push_back(cur_cmd_chr.char_val());
        } else
            return true;
    }
}

// takes the list of tokens as argument
auto Parser::list_to_string(TokenList &L, Buffer &b) -> bool {
    back_input(hash_table.frozen_endcsname);
    back_input(L);
    return list_to_string0(b);
}

// as above, converts to lower case
auto Parser::list_to_string_cv(TokenList &L, Buffer &b) -> bool {
    back_input(hash_table.frozen_endcsname);
    back_input(L);
    for (;;) {
        if (get_x_token()) return true;
        if (cur_cmd_chr.cmd == endcsname_cmd) return false;
        if (cur_tok.not_a_cmd())
            b.push_back(static_cast<char32_t>(std::tolower(static_cast<int>(cur_cmd_chr.char_val()))));
        else
            return true;
    }
}

// Special case of a counter
auto Parser::csname_ctr(TokenList &L, Buffer &b) -> bool {
    b.clear();
    b.append("c@");
    return list_to_string(L, b);
}

// Signals an error; returns the string or bad
auto Parser::list_to_string_c(TokenList &x, String msg) -> std::string {
    Buffer &B = Thbuf1;
    B.clear();
    if (list_to_string(x, B)) {
        parse_error(err_tok, msg, x);
        B = "bad";
    }
    return B;
}

// Converts the token list X into a string, adding s1 and s2
// May signal an error, use bad instead
void Parser::list_to_string_c(TokenList &x, const std::string &s1, const std::string &s2, const std::string &msg, Buffer &B) {
    B = s1;
    if (list_to_string(x, B)) {
        parse_error(err_tok, msg, x);
        B = s1 + "bad";
    }
    B += s2;
}

// This is like \csname s1 L s2 \endcsname
// It returns true and signals in case of error.
auto Parser::my_csname(String s1, String s2, TokenList &L, String s) -> bool {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << s << "}\n";
    }
    Buffer b;
    b.append(s1);
    bool r = list_to_string(L, b);
    b.append(s2);
    if (r) {
        bad_csname(s != nullptr);
        return true;
    }
    finish_csname(b, s);
    return false;
}

// Same as above; but converts the result into the string fetch_name_res
void Parser::fetch_name2() {
    Buffer b;
    if (list_to_string0(b)) {
        if (cur_tok.is_valid()) back_input();
        bad_csname(false);
    }
    fetch_name_res.clear();
    fetch_name_res.append(b);
}

// Converts the buffer into a command; locally defines it to \relax if undef
// result is in cur_tok
void Parser::finish_csname(const std::string &b) {
    cur_tok  = hash_table.locate(b);
    auto pos = cur_tok.eqtb_loc();
    if (hash_table.eqtb[pos].val.is_undef()) eq_define(pos, CmdChr(relax_cmd, relax_code), false);
}

// Same as above, but the token is to be read again
void Parser::finish_csname(Buffer &b, const std::string &s) {
    finish_csname(b);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << s << "->\\" << b.convert_to_log_encoding() << "}\n";
    }
    back_input();
}

// This interprets \csname ... \endcsname; token is to be read again
void Parser::E_csname() {
    Buffer b;
    if (tracing_commands()) Logger::log_dump("csname");
    if (list_to_string0(b)) {
        if (cur_tok.is_valid()) back_input();
        bad_csname(true);
    }
    finish_csname(b, "\\csname");
}

// this is \@nameuse; creates a name to be read again
void Parser::csname_arg() {
    TokenList L = read_arg();
    back_input(hash_table.frozen_endcsname);
    back_input(L);
    E_csname();
}

// Latex3 variants of \csname .. \endcsname
void Parser::E_usename(subtypes c, bool vb) {
    Token t = cur_tok;
    if (vb) {
        Logger::finish_seq();
        the_log << "{" << t << "}\n";
    }
    if (c == 0) { //  \use:c
        csname_arg();
        if (vb) the_log << t << " -> " << cur_tok << "\n";
    } else if (c == 1) { // \exp_args:Nc
        get_token();
        Token T = cur_tok;
        csname_arg();
        back_input(T);
        if (vb) the_log << t << " -> " << T << cur_tok << "\n";
    } else { // \exp_args:cc
        csname_arg();
        get_token();
        Token T = cur_tok;
        csname_arg();
        back_input(T);
        if (vb) the_log << t << " -> " << T << cur_tok << "\n";
    }
}

// Converts a token list into a string, like csname_arg
auto Parser::fetch_name0() -> std::string {
    TokenList L = read_arg();
    return fetch_name1(L);
}
auto Parser::fetch_name0_nopar() -> std::string {
    TokenList L = read_arg_nopar();
    return fetch_name1(L);
}

auto Parser::fetch_name1(TokenList &L) -> std::string {
    if (L.empty()) return "";
    SaveState st_state(TL, restricted);
    restricted = true;
    L.push_back(hash_table.frozen_endcsname);
    TL.swap(L);
    fetch_name2();
    return fetch_name_res;
}

// if exp is true, this is \csname ... \endcsname, otherwise get_r_token
// Note that the token is left undefined
auto Parser::fetch_csname(bool exp) -> Token {
    if (exp) {
        fetch_name0();
        return hash_table.locate(fetch_name_res);
    }
    return get_r_token(true);
}

// Use a non-long method
auto Parser::fetch_name_opt() -> std::string {
    TokenList L;
    bool      res = read_optarg_nopar(L);
    if (!res || L.empty()) return "";

    return fetch_name1(L);
}

// Implements \@ifundefined{cmd} {A}{B}
// token lists A and B are read by one_of_two. A command is undefined
// if its command code is relax.
void Parser::E_ifundefined(bool c) {
    Token T = cur_tok;
    // you lose in case \@ifundefined {a\par b}{c}{d}, pars
    TokenList a     = read_arg_nopar();
    bool      undef = true;
    back_input(hash_table.frozen_endcsname);
    back_input(a);
    if (c) { // \traclics@ifundefined, undocumented
        fetch_name2();
        undef = !hash_table.is_defined(fetch_name_res);
    } else {
        E_csname();
        get_token();
        undef = cur_cmd_chr.is_relax();
    }
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << "{" << T << boolean(undef) << "}\n";
    }
    one_of_two(undef);
}

// Implements \@iftempty#1{A}{B} \@ifbempty#1{A}{B}
void Parser::E_ifempty() {
    Token     T = cur_tok;
    subtypes  c = cur_cmd_chr.chr;
    TokenList L = read_arg();
    TokenList a = read_arg();
    TokenList b = read_arg();
    if (c != 0U) token_ns::remove_first_last_space(L);
    bool ok = L.empty();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << T << " " << boolean(ok) << "}\n";
    }
    one_of_two(a, b, ok);
}

// Implements \@ifstar{A}{B}. If the next character is a star, it is read.
void Parser::T_ifstar() {
    TokenList a  = read_arg();
    TokenList b  = read_arg();
    bool      ok = remove_initial_star();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\@ifstar " << boolean(ok) << "}\n";
    }
    one_of_two(a, b, ok);
}

// Implements \@ifnextchar W{A}{B}.
// If the next character is a W, it is not read.
void Parser::T_ifnextchar(bool c) {
    Token T = cur_tok;
    if (read_token_arg(T)) cur_tok = hash_table.relax_token; // ok ?
    Token     d = cur_tok;
    TokenList a = read_arg();
    TokenList b = read_arg();
    skip_initial_space();
    if (cur_tok.is_valid()) back_input();
    bool ok = cur_tok == d;
    if (!c && cur_tok.char_or_active() && d.char_or_active() && cur_tok.chr_val() == d.chr_val()) ok = true;
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << T << boolean(ok) << "}\n";
    }
    one_of_two(a, b, ok);
}

void Parser::E_while(subtypes cc) {
    Token     T = cur_tok;
    TokenList A = read_until(cc == 2 ? hash_table.fi_token : hash_table.do_token);
    TokenList B = read_arg();
    if (cc != 2) A.push_back(hash_table.relax_token);
    A.splice(A.end(), B);
    TokenList res;
    if (cc == 2) {
        res.push_back(hash_table.fi_token);
        res.push_back(hash_table.fi_token);
        TokenList C = A;
        brace_me(C);
        res.splice(res.begin(), C);
        res.push_front(hash_table.iwhilesw_token);
        res.splice(res.begin(), A);
    } else {
        if (cc == 0) A.push_back(hash_table.relax_token);
        TokenList D = A;
        brace_me(D);
        res.push_front(hash_table.fi_token);
        res.splice(res.begin(), D);
        res.push_front(cc == 1 ? hash_table.iwhiledim_token : hash_table.iwhilenum_token);
        res.splice(res.begin(), A);
        res.push_front(cc == 1 ? hash_table.ifdim_token : hash_table.ifnum_token);
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "<- " << res << "\n";
    }
    back_input(res);
}

void Parser::E_iwhile(subtypes cc) {
    Token     T = cur_tok;
    TokenList A = cc == 2 ? read_until(hash_table.fi_token) : read_arg();
    TokenList B = A;
    brace_me(B);
    TokenList w;
    if (cc == 2) {
        w.push_front(hash_table.fi_token);
        w.splice(w.begin(), B);
        w.push_front(hash_table.fi_token);
        w.push_front(hash_table.gobbletwo_token);
        w.push_front(hash_table.else_token);
        w.push_front(T);
        w.push_front(hash_table.expandafter_token);
        w.splice(w.begin(), A);
    } else {
        w.splice(w.begin(), B);
        w.push_front(hash_table.fi_token);
        w.push_front(hash_table.gobble_token);
        w.push_front(hash_table.expandafter_token);
        w.push_front(hash_table.else_token);
        w.push_front(cc == 1 ? hash_table.iwhiledim_token : hash_table.iwhilenum_token);
        w.push_front(hash_table.expandafter_token);
        w.splice(w.begin(), A);
        w.push_front(cc == 1 ? hash_table.ifdim_token : hash_table.ifnum_token);
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "<- " << w << "\n";
    }
    back_input(w);
}

// Case of  \loop....\repeat.
// We define \iterate as: \def\iterate{...\relax\expandafter\iterate\fi}
// then execute \iterate\let\iterate\relax
void Parser::E_loop() {
    if (tracing_commands()) Logger::log_dump("loop");
    TokenList R = read_until(hash_table.repeat_token);
    R.push_back(hash_table.relax_token);
    R.push_back(hash_table.expandafter_token);
    R.push_back(hash_table.iterate_token);
    R.push_back(hash_table.fi_token);
    new_macro(R, hash_table.iterate_token);
    back_input(hash_table.relax_token);
    back_input(hash_table.iterate_token);
    back_input(hash_table.let_token);
    back_input(hash_table.iterate_token);
}

// Implements \newcounter{foo} when def is true or part of \stepcounter{foo} etc
// If def is false, then \c@foo is in cur_tok, to be read again
// Returns true in case of error. If def is false we check
// that foo is a counter i.e. \c@foo is assign_int.
// Otherwise we define a counter
auto Parser::M_counter(bool def) -> bool {
    Token     T = cur_tok;
    Buffer    b;
    TokenList L0 = read_arg();
    if (csname_ctr(L0, b)) {
        bad_counter0();
        return true;
    }
    if (counter_check(b, def)) return true;
    back_input();
    if (!def) return false;
    return counter_aux(b.substr(2), nullptr, T);
}

// Used by the bootstrap phase to define a dependent counter
void Parser::counter_boot(const std::string &s, String aux) {
    Token   T = hash_table.relax_token;
    Buffer &b = Thbuf2;
    b         = "c@" + s;
    if (counter_check(b, true)) return; // should not happen
    back_input();
    counter_aux(s, aux, T);
}

// Given c@foo in the buffer b, creates the token \c@foo
// if def is false, check that \c@foo=2 is OK
// otherwise, checks that \c@foo is undefined or relax
// Returns true if bad
auto Parser::counter_check(Buffer &b, bool def) -> bool {
    cur_tok       = hash_table.locate(b);
    EqtbCmdChr &E = hash_table.eqtb[cur_tok.eqtb_loc()];
    if (def) {
        if (!E.val.is_undef_or_relax()) {
            bad_redefinition(0, cur_tok);
            return true;
        }
    } else {
        if (E.val.cmd != assign_int_cmd) {
            bad_counter1(b, E);
            return true;
        }
    }
    return false;
}

// Reads an optional argument bar; puts \cl@bar in cur_tok
// Return 0 if no opt argument, 1 if error, 2 if OK
// If s is a valid string, it contains the optional argument

auto Parser::counter_read_opt(String s) -> int {
    if (s != nullptr) {
        if (s[0] == 0) return 0; // s empty says no opt arg
        Buffer &b = Thbuf2;
        b         = fmt::format("cl@{}", s);
        finish_csname(b, "newcounter");
        return 2;
    }
    TokenList bar_list;
    int       ne = nb_errs;
    if (!read_optarg_nopar(bar_list)) return 0;
    if (ne != nb_errs) return 1;
    if (my_csname("cl@", "", bar_list, "newcounter_opt")) return 1;
    return 2;
}

// This defines a new counter, named name; second arg is for counter_read_opt
// cur_tok (o be read again) holds the token \c@foo
auto Parser::counter_aux(const std::string &name, String opt, Token T) -> bool {
    Buffer &b = Thbuf1;
    // We are defining a counter now
    // We construct a macro without argument that expands to \number\c@foo
    TokenList L;
    L.push_front(cur_tok);
    L.push_front(hash_table.number_token);

    // evaluate : \global\let\cl@foo\@empty
    b = "cl@" + name; // b is now cl@foo
    M_let_fast(hash_table.locate(b), hash_table.empty_token, true);

    // thexx_cmd is the token associated to \thefoo
    b            = "the" + name; // B is now thefoo
    Token thecmd = hash_table.locate(b);
    cur_tok      = T;            // in case of overflow, this gives the correct message
    new_constant(newcount_code); // this will eat the \c@foo token
    new_macro(L, thecmd, true);
    switch (counter_read_opt(opt)) {
    case 0: return false;
    case 1: return true;
    default: break;
    }
    // evaluate: \xdef\cl@bar{\cl@bar\noexpand\@elt{foo}}
    // We know that \cl@bar can be expanded only once.
    TokenList foo_list = token_ns::string_to_list(name, true);
    Token     cl_token = cur_tok; // \cl@bar
    get_token();                  // get the \cl@bar token
    M_cons(cl_token, foo_list);
    return false;
}

// \@addtreset{foo}{bar}
// evaluate: \@cons\cl@bar{{foo}}; i.e. M_cons(\cl@bar, {foo}).
void Parser::E_addtoreset() {
    int       ne       = nb_errs;
    TokenList foo_list = read_arg_nopar();
    TokenList L        = read_arg_nopar();
    if (ne != nb_errs) return;
    brace_me(foo_list);
    if (my_csname("cl@", "", L, "\\@addtoreset")) return;
    Token cl_token = cur_tok; // \cl@bar
    get_token();              // get the \cl@bar token
    M_cons(cl_token, foo_list);
}

//  Implements \newcount etc.
void Parser::new_constant(subtypes c) {
    switch (c) {
    case newtoks_code: new_constant("toksdef", last_register, c, assign_toks_cmd); return;
    case newcount_code: new_constant("countdef", last_register, c, assign_int_cmd); return;
    case newbox_code: new_constant("chardef", last_register, c, char_given_cmd); return;
    case newdimen_code: new_constant("dimendef", last_register, c, assign_dimen_cmd); return;
    case newlength_code: new_constant("skipdef", last_register, c, assign_glue_cmd); return;
    case newmuskip_code: new_constant("muskipdef", last_register, c, assign_mu_glue_cmd); return;
    case newread_code: new_constant("chardef", 15, c, char_given_cmd); return;
    case newwrite_code: new_constant("chardef", 15, c, char_given_cmd); return;
    case newlanguage_code: new_constant("chardef", last_register, c, char_given_cmd); return;
    default: return;
    }
}

// Arguments: a command like "chardef", max value, a subtype like newwrite_code
// and a symcode like char_given_cmd
// allocates registter number k whose value at the table at position alloc_pos
// (plus an offset, maybe)
void Parser::new_constant(String /*name*/, size_t max_val, subtypes alloc_pos, symcodes c) {
    Token T = cur_tok;
    get_r_token(true);
    auto k = allocation_table[alloc_pos];
    if (k >= max_val) {
        err_buf = fmt::format("Overflow in {}; max value is {}", T, max_val);
        signal_error(T, "allocation overflow");
        return;
    }
    ++k;
    if (k == 255 && c == assign_int_cmd) ++k;
    allocation_table[alloc_pos] = k;
    if (c == assign_int_cmd) k += count_reg_offset;
    if (c == assign_mu_glue_cmd) k += muskip_reg_offset;
    CmdChr R(c, subtypes(k));
    eq_define(cur_tok.eqtb_loc(), R, true);
    Logger::finish_seq();
}

// c is the code of \value, \stepcounter, \addtocounter, \setcounter, or
//  \@stpelt
// This may generate \calc{\global\advance\c@foo} for \addtocounter{foo}
// or \global\advance\c@foo 25\relax for \addtocounter{foo}{25}
// In the case of \stepcounter, we do
//  \global\advance\c@foo\@one\let\elt@stpelt\cl@foo
void Parser::E_counter(subtypes c) {
    Token first = cur_tok;
    if (c == addtoreset_code) {
        E_addtoreset();
        return;
    }
    if (M_counter(false)) return;
    if (c == value_code) return;
    get_token();
    Token     t = cur_tok; // we have \c@foo
    TokenList L;
    if (!calc_loaded && c != stepcounter_code && c != killcounter_code) L = read_arg();
    L.push_front(t);
    if (c != setcounter_code && c != killcounter_code) L.push_front(hash_table.advance_token);
    L.push_front(hash_table.global_token);
    if (c == stepcounter_code) {
        L.push_back(hash_table.at_one_token);
        L.push_back(hash_table.let_token);
        L.push_back(hash_table.elt_token);
        L.push_back(hash_table.killcounter_token);
        group_buffer = "cl@" + hash_table[t.hash_loc()].substr(2);
        t            = hash_table.locate(group_buffer);
        //  L.push_back(hash_table.OB_token);
        L.push_back(t);
        //    L.push_back(hash_table.CB_token);
    } else if (c == killcounter_code)
        L.push_back(hash_table.at_zero_token);
    else if (calc_loaded) {
        brace_me(L);
        L.push_front(hash_table.calc_token);
    } else
        L.push_back(hash_table.relax_token);
    finish_counter_cmd(first, L);
}

// Backinputs the token list, plus some tracing info.
void Parser::finish_counter_cmd(Token first, TokenList &L) {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << first << "->" << L << "\n";
    }
    back_input(L);
}

// c is 0 for \setlength, 1 for \addtolength
void Parser::E_setlength(subtypes c) {
    Token caller = cur_tok;
    if (read_token_arg(caller)) return;
    if (cur_tok.not_a_cmd()) {
        bad_redefinition(2, cur_tok);
        return;
    }
    if (cur_cmd_chr.is_undef()) {
        undefined_mac();
        return;
    }
    Token     t = cur_tok;
    TokenList L;
    if (calc_loaded) {
        L.push_back(t);
        if (c != 0) L.push_front(hash_table.advance_token);
        brace_me(L);
        L.push_front(hash_table.calc_token);
    } else {
        L = read_arg();
        L.push_back(hash_table.relax_token);
        L.push_front(t);
        if (c != 0) L.push_front(hash_table.advance_token);
    }
    finish_counter_cmd(caller, L);
}

// Implements \newif. Check that the command starts with the letters if
void Parser::M_newif() {
    Token T = get_r_token();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\newif " << T << "}\n";
    }
    if (T.active_or_single()) {
        parse_error(err_tok, "Illegal argument of \\newif: ", T, "", "bad newif");
        return;
    }
    auto S = hash_table[T.hash_loc()];
    if (S[0] != 'i' || S[1] != 'f' || S[2] == 0) {
        parse_error(err_tok, "token list \\newif does not start with if");
        return;
    }
    eq_define(T.eqtb_loc(), CmdChr(if_test_cmd, if_false_code), false);
    M_newif_aux(T, S.substr(2), true);
    M_newif_aux(T, S.substr(2), false);
}

// If you say \newif\iffoo, this is the same as
// \def\footrue{\let\iffoo\iftrue} and \def\foofalse{\let\iffoo\iffalse}
// this code does one or the other definitions,
// given T=\iffoo, s=foo and b=true of false
void Parser::M_newif_aux(Token T, const std::string &S, bool b) {
    TokenList L1;
    L1.push_front(hash_table.let_token);
    L1.push_back(T);
    L1.push_back(b ? hash_table.iftrue_token : hash_table.iffalse_token);
    mac_buffer = S + boolean(b);
    Token res  = hash_table.locate(mac_buffer);
    new_macro(L1, res);
}

// \newboolean , \provideboolean
void Parser::M_newboolean(subtypes c) {
    TokenList L = read_arg();
    if (my_csname("if", "", L, "newboolean")) return;
    get_token();
    if (cur_cmd_chr.is_relax()) {
        back_input();
        M_newif();
    } else if (c == 1)
        return;
    else
        parse_error(err_tok, "boolean ", cur_tok, " already defined in \\newboolean", "bad newboolean");
}

// Used for bootstrap
void Parser::make_token(String s) {
    Buffer &b = Thbuf1;
    b.clear();
    b.append(s);
    back_input(hash_table.locate(b));
}

// definining macros
// new_macro constructs a command without arguments.
// unless gbl is true, the definition will be local
void Parser::new_macro(TokenList &L, Token name, bool gbl) {
    auto *X = new Macro(L);
    mac_define(name, X, gbl, rd_always, user_cmd);
}
void Parser::new_macro(TokenList &L, Token name) {
    auto *X = new Macro(L);
    mac_define(name, X, false, rd_always, user_cmd);
}

void Parser::new_macro(const std::string &s, Token name) {
    Thbuf1      = s;
    TokenList L = Thbuf1.str_toks11(false);
    auto *    X = new Macro(L);
    mac_define(name, X, false, rd_always, user_cmd);
}

// The following are used by the bootstrap phase
// Defines globally a macro without argument.
// Nothing appears in the transcript file.

void Parser::new_prim(Token name, TokenList &L) {
    auto *X = new Macro(L);
    auto  B = subtypes(mac_table.mc_new_macro(X));
    eq_define(name.eqtb_loc(), CmdChr(user_cmd, B), true);
}

void Parser::new_prim(String a, TokenList &b) { new_prim(hash_table.locate(a), b); }

// new_primx("@nnil","@nil"); implements \def\@nnil{\@nil}
void Parser::new_primx(String a, String b) {
    TokenList L;
    L.push_back(hash_table.locate(b));
    new_prim(hash_table.locate(a), L);
}

// new_prim("@plus","plus"); implements \def\@plus{plus}
void Parser::new_prim(String a, String b) {
    TokenList L;
    auto      n = strlen(b);
    for (size_t i = 0; i < n; i++) {
        auto c = b[i];
        if (uchar(c) > 128) {
            spdlog::critical("Internal error in new_prim");
            abort();
        }
        spec_offsets T = c == ' ' ? space_t_offset : std::isalpha(c) != 0 ? letter_t_offset : other_t_offset;
        L.push_back(Token(T, char32_t(c)));
    }
    new_prim(hash_table.locate(a), L);
}

// Implements  \DefineShortVerb and \UndefineShortVerb
// saves and restores catcode if value <256
void Parser::M_shortverb(subtypes x) {
    Token Tfe = cur_tok;
    if (read_token_arg(Tfe)) return;
    Token t = cur_tok;
    auto  T = t.val - single_offset;
    if (0 < T && T < int(nb_characters)) {
        if (x == 0) { // define
            if (get_catcode(T) == special_catcode) {
                short_verb_error(Tfe, t, 2);
                return;
            }
            if (T < int(nb_shortverb_values)) old_catcode[T] = get_catcode(T);
            set_catcode(T, special_catcode);
        } else { // undefine
            if (get_catcode(T) != special_catcode) {
                short_verb_error(Tfe, t, 3);
                return;
            }
            if (T < int(nb_shortverb_values))
                set_catcode(T, old_catcode[T]);
            else
                set_catcode(T, other_catcode);
        }
    } else
        short_verb_error(Tfe, t, x);
}

// Returns the token \foo or \endfoo.
auto Parser::find_env_token(const std::string &name, bool beg) -> Token {
    mac_buffer.clear();
    if (!beg) mac_buffer += "end";
    mac_buffer += name;
    see_cs_token(hash_table.locate(mac_buffer));
    return cur_tok;
}

// Hack for \begin{foo}
auto Parser::env_helper(const std::string &s) -> SaveAuxEnv * {
    int cl = get_cur_line();
    find_env_token(s, true);
    std::string cur_e_name = get_cur_env_name();
    if (cur_cmd_chr.is_undef())
        undefined_env(s);
    else
        back_input();
    find_env_token(s, false);
    Token t = cur_tok;
    if (cur_cmd_chr.is_undef()) cur_cmd_chr = CmdChr(relax_cmd, relax_code);
    return new SaveAuxEnv(*this, cur_e_name, s, cl, t, cur_cmd_chr);
}

// This implements \begin{foo}
void Parser::T_begin(const std::string &s) {
    SaveAuxEnv *X = env_helper(s);
    push_level(bt_env);
    push_save_stack(X);
    set_cur_env_name(s);
}

// This is the code of \end{foo}.
void Parser::T_end(const std::string &s) {
    if (s == "document") // hack, because document is at outer level
    {
        T_begin(s);
        get_token();
    }
    if (s == "tabular" || s == "tabular*") {
        if (false_end_tabular(s)) return;
    }
    if (s.empty()) {
        parse_error(err_tok, "Illegal \\end{}");
        return;
    }
    SaveAuxEnv *X = is_env_on_stack(s);
    if (X == nullptr) {
        parse_error(err_tok, "cannot close environment ", s, "bad \\end");
        return;
    }
    if (first_boundary() == bt_tpa) {
        pop_level(bt_tpa);
        cur_tok.kill();
        pop_level(bt_env);
        cur_level++;
        throw EndOfData();
    }
    back_input(X->token);
    back_input(hash_table.sendgroup_token);
    cur_tok = X->token;
    if (X->cc.is_user()) {
        cur_cmd_chr = X->cc;
        Macro &T    = mac_table.get_macro(cur_cmd_chr.chr);
        if (T.nbargs != 0)
            parse_error(err_tok, "Illegal end of environment");
        else
            expand();
    } else {
        Token t                = hash_table.temp_token;
        auto  k                = t.eqtb_loc();
        hash_table.eqtb[k].val = X->cc;
        back_input(t);
    }
}

// Given a string like foo, this evaluates to \expandafter{\foo}
// If \foo is undefined, the result is {}; if \foo expands to 'gee'
// the result is {gee}. The result can be obtained via read_arg.
void Parser::expand_no_arg(const std::string &s) {
    Token t = hash_table.locate(s);
    back_input(hash_table.CB_token);
    back_input(t);
    expand_when_ok(true);
    back_input(hash_table.OB_token);
}

void Parser::expand_when_ok(bool allow_undef) {
    get_token();
    if (allow_undef && cur_cmd_chr.is_undef()) {
    } else if (cur_cmd_chr.is_expandable())
        expand();
    else
        back_input();
}

// Returns the value of the macro \foo;
// If \foo is not a user macro without args, returns empty list
auto Parser::get_mac_value(const std::string &s) -> TokenList { return get_mac_value(hash_table.locate(s)); }

auto Parser::get_mac_value(Token t) -> TokenList {
    TokenList res;
    if (t.not_a_cmd()) return res;
    see_cs_token(t);
    if (!cur_cmd_chr.is_user()) return res;
    subtypes c = cur_cmd_chr.chr;
    Macro &  M = mac_table.get_macro(c);
    if (M.type != dt_normal) return res;
    if (M.nbargs != 0) return res;
    return M.body;
}

// c is the number of arguments, c=0 is the same as 1, 5 is 12of3
void Parser::E_all_of_one(Token T, subtypes c) {
    String s{nullptr};
    int    n  = 0;
    bool   vb = tracing_macros();
    switch (c) {
    case 2:
        s = " #1#2->#1#2";
        n = 2;
        break;
    case 3:
        s = " #1#2#3->#1#2#3";
        n = 3;
        break;
    case 4:
        s = " #1#2#3#4->#1#2#3#4";
        n = 4;
        break;
    case 5:
        s = " #1#2#3->#1#2";
        n = 3;
        break;
    default: s = " #1->#1 "; n = 1;
    }
    if (vb) {
        Logger::finish_seq();
        the_log << T << s << "\n";
    }
    TokenList L = read_arg();
    if (vb) the_log << "#1<-" << L << "\n";
    if (n == 1) {
        back_input(L);
        return;
    }
    TokenList L2 = read_arg();
    if (vb) the_log << "#2<-" << L2 << "\n";
    if (n == 2) {
        back_input(L2);
        back_input(L);
        return;
    }
    TokenList L3 = read_arg();
    if (vb) the_log << "#3<-" << L3 << "\n";
    if (c == 5) {
        back_input(L2);
        back_input(L);
        return;
    }
    if (c == 3) {
        back_input(L3);
        back_input(L2);
        back_input(L);
        return;
    }
    TokenList L4 = read_arg();
    if (vb) the_log << "#4<-" << L4 << "\n";
    back_input(L4);
    back_input(L3);
    back_input(L2);
    back_input(L);
}

// converts 12.0pt in 12, and 12.3pt in 12.3
// no error is signaled
void token_ns::strip_pt(TokenList &L) {
    if (L.back() != Token(other_t_offset, 't')) return;
    L.pop_back();
    if (L.back() != Token(other_t_offset, 'p')) return;
    L.pop_back();
    if (L.back() == Token(other_t_offset, '.')) {
        L.pop_back();
        return;
    }
    if (L.back() == Token(other_t_offset, '0')) {
        L.pop_back();
        if (L.back() == Token(other_t_offset, '.')) {
            L.pop_back();
            return;
        }
        L.push_back(Token(other_t_offset, '0'));
    }
}

// \@scanupdown\foo{aa}{bb}{^x_y} expands to \foo{y}{x}
// If no x given, then aa is used instead
// If no y given then bb is used instead
// You can say {_y^x}, or {'^x_y} if you like

void Parser::E_scan_up_down() {
    TokenList cmd     = read_arg();
    TokenList df_up   = read_arg();
    TokenList df_down = read_arg();
    TokenList w       = read_arg();
    E_scan_up_down(df_up, df_down, w, cmd);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "\\scanupdown ->" << cmd << "\n";
    }
    back_input(cmd);
}

void Parser::E_scan_up_down(TokenList &A, TokenList &B, TokenList &C, TokenList &res) {
    back_input(hash_table.relax_token); // end marker
    back_input(C);
    TokenList df_up   = A;
    TokenList df_down = B;
    get_token();
    if (cur_cmd_chr.is_single_quote()) {
        scan_prime();
        get_token();
    }
    if (cur_cmd_chr.cmd == hat_catcode) {
        df_up = read_arg();
        get_token();
        if (cur_cmd_chr.cmd == underscore_catcode) {
            df_down = read_arg();
        } else
            back_input();
    } else if (cur_cmd_chr.cmd == underscore_catcode) {
        df_down = read_arg();
        get_token();
        if (cur_cmd_chr.is_single_quote()) {
            scan_prime();
            get_token();
        }
        if (cur_cmd_chr.cmd == hat_catcode) {
            df_up = read_arg();
        } else
            back_input();
    } else
        back_input();
    read_until(hash_table.relax_token); // read all unused tokens
    brace_me(df_up);
    brace_me(df_down);
    res.splice(res.end(), df_down);
    res.splice(res.end(), df_up);
}

// Implements \sideset
void Parser::E_sideset() {
    TokenList A = read_arg();
    TokenList B = read_arg();
    TokenList C = read_arg();
    TokenList none;
    none.push_back(hash_table.mmlnone_token);
    if (A.empty() && B.empty()) {
    } // \mathop{C}\limits
    else {
        if (!B.empty()) E_scan_up_down(none, none, B, C);
        if (!A.empty()) {
            C.push_back(hash_table.mmlprescripts_token);
            E_scan_up_down(none, none, A, C);
        }
        brace_me(C);
        C.push_front(hash_table.mmlmultiscripts_token);
    }
    brace_me(C);
    C.push_front(hash_table.mathop_token);
    C.push_back(hash_table.limits_token);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "\\sideset ->" << C << "\n";
    }
    back_input(C);
}

// \expandafter\foo\bar
void Parser::E_expandafter() {
    get_token();
    Token T = cur_tok;
    get_token();
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << "{\\expandafter " << T << " " << cur_tok << "}\n";
    }
    if (cur_cmd_chr.is_expandable())
        expand();
    else
        back_input();
    back_input(T);
}

// \mathversion{bold} is the same as \@mathversion1\relax
// \mathversion{otherwise} is the same as \@mathversion0\relax
void Parser::E_mathversion() {
    TokenList arg = read_arg();
    Buffer &  B   = Thbuf1;
    B.clear();
    if (list_to_string(arg, B)) {
        parse_error(err_tok, "bad \\mathversion");
        return;
    }
    TokenList L;
    Token     T = Token(other_t_offset, B == "bold" ? '1' : '0');
    L.push_back(hash_table.mathversion_token);
    L.push_back(T);
    L.push_back(hash_table.relax_token);
    back_input(L);
}

// expands \ensuremath{foo} outside math mode
// Note that \ensuremath is a robust command, in LaTeX so that
// \if$\ensuremath true \else false \fi is false.
// Thus, we can implement is as a non-macro in Tralics
//
void Parser::E_ensuremath() {
    TokenList L = read_arg();
    if (the_stack.get_mode() != mode_math) {
        L.push_front(hash_table.relax_token);
        L.push_front(hash_table.dollar_token);
        L.push_back(hash_table.dollar_token);
    }
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << "\\ensuremath-> " << L << "\n";
    }
    back_input(L);
}

//  Produces a random number
void Parser::E_random() {
    Token T = cur_tok;
    auto  t = scan_int(T);
    int   u = std::rand();
    if (t > 0) u = u % t;
    Buffer B;
    B.format("{}", u);
    TokenList L = B.str_toks11(false);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << t << "->" << L << "\n";
    }
    back_input(L);
}

void Parser::E_user(bool vb, subtypes c, symcodes C) {
    if (vb) {
        Logger::finish_seq();
        the_log << cur_tok;
    }
    auto guard1 = SaveScannerStatus(ss_macro);
    auto guard2 = SaveLongState(ls_normal);
    if (C == userl_cmd || C == userlo_cmd || C == userlp_cmd || C == userlpo_cmd) long_state = ls_long;
    expand_mac(mac_table.get_macro(c));
}

void Parser::E_first_of_two(bool vb, subtypes c) {
    if (vb) {
        Logger::finish_seq();
        the_log << cur_tok << "#1#2->#" << (c == 1 ? "1" : "2") << "\n";
    }
    TokenList L1 = read_arg();
    TokenList L2 = read_arg();
    if (vb) {
        the_log << "#1<-" << L1 << "\n";
        the_log << "#2<-" << L2 << "\n";
    }
    one_of_two(L1, L2, c == 1);
}

void Parser::E_first_of_three(bool vb, subtypes c) {
    if (vb) {
        Logger::finish_seq();
        the_log << cur_tok << "#1#2#3->#" << (c == 1 ? "1" : (c == 2 ? "2" : "3")) << "\n";
    }
    TokenList L1 = read_arg();
    TokenList L2 = read_arg();
    TokenList L3 = read_arg();
    if (vb) {
        the_log << "#1<-" << L1 << "\n";
        the_log << "#2<-" << L2 << "\n";
        the_log << "#3<-" << L3 << "\n";
    }
    if (c == 1)
        back_input(L1);
    else if (c == 2)
        back_input(L2);
    else
        back_input(L3);
}

void Parser::E_first_of_four(bool vb, subtypes c) {
    if (vb) {
        Logger::finish_seq();
        the_log << cur_tok << "#1#2#3#4->#" << (c == 1 ? "1" : (c == 2 ? "2" : (c == 3 ? "3" : "4"))) << "\n";
    }
    TokenList L1 = read_arg();
    TokenList L2 = read_arg();
    TokenList L3 = read_arg();
    TokenList L4 = read_arg();
    if (vb) {
        the_log << "#1<-" << L1 << "\n";
        the_log << "#2<-" << L2 << "\n";
        the_log << "#3<-" << L3 << "\n";
        the_log << "#4<-" << L4 << "\n";
    }
    if (c == 1)
        back_input(L1);
    else if (c == 2)
        back_input(L2);
    else if (c == 3)
        back_input(L3);
    else
        back_input(L4);
}

void Parser::E_ignore_n_args(bool vb, subtypes c) {
    auto n = (c < 1 ? 1 : (c > 9 ? 9 : c));
    int  i = 1;
    if (vb) {
        Logger::finish_seq();
        the_log << cur_tok << " ...#" << n << "->\n";
    }
    while (n > 0) {
        TokenList L = read_arg();
        if (vb) the_log << "#" << i << "<-" << L << "\n";
        ++i;
        --n;
    }
}

// If the tokens that follows are that of the string S (upper or lower case)
// then they are read. Initial spaces are read. Full expansion.
auto Parser::scan_keyword(String s) -> bool {
    size_t    k = 0;
    TokenList L;
    for (;;) {
        if (s[k] == 0) return true;
        get_x_token();
        if (cur_tok.not_a_cmd() && (cur_cmd_chr.char_val() == uchar(s[k]) || cur_cmd_chr.char_val() == uchar(s[k] + 'A' - 'a'))) {
            L.push_back(cur_tok);
            k++;
        } else if (L.empty() && cur_cmd_chr.is_space())
            continue;
        else {
            back_input();
            back_input(L);
            return false;
        }
    }
}

// ------------------------------------

void Parser::expand() {
    auto     guard = SaveErrTok(cur_tok);
    subtypes c     = cur_cmd_chr.chr;
    symcodes C     = cur_cmd_chr.cmd;
    Token    T     = cur_tok;
    bool     vb    = tracing_macros();
    switch (C) {
    case a_cmd: E_accent_a(); return;
    case accent_cmd: E_accent(); return;
    case ifundefined_cmd: E_ifundefined(c != 0); return;
    case car_cmd: E_car(c == zero_code); return;
    case all_of_one_cmd: E_all_of_one(cur_tok, c); return;
    case refstepcounter_cmd: refstepcounter(); return;
    case month_day_cmd: month_day(c); return;
    case scan_up_down_cmd: E_scan_up_down(); return;
    case sideset_cmd: E_sideset(); return;
    case multispan_cmd: E_multispan(); return;
    case ifempty_cmd: E_ifempty(); return;
    case split_cmd: E_split(); return;
    case useverb_cmd: E_useverb(); return;
    case counter_cmd: E_counter(c); return;
    case setlength_cmd: E_setlength(c); return;
    case csname_cmd: E_csname(); return;
    case usename_cmd: E_usename(c, vb); return;
    case convert_cmd: E_convert(); return;
    case the_cmd: E_the_traced(T, c); return;
    case if_test_cmd: E_if_test(c, false); return;
    case fi_or_else_cmd: E_fi_or_else(); return;
    case loop_cmd: E_loop(); return;
    case input_cmd: E_input(c); return;
    case xspace_cmd: E_xspace(); return;
    case ensuremath_cmd: E_ensuremath(); return;
    case while_cmd: E_while(c); return;
    case iwhile_cmd: E_iwhile(c); return;
    case latex_ctr_cmd: E_latex_ctr(); return;
    case undef_cmd: undefined_mac(); return;
    case mathversion_cmd: E_mathversion(); return;
    case get_config_cmd: E_get_config(c); return;
    case random_cmd: E_random(); return;
    case pdfstrcmp_cmd: E_pdfstrcmp(); return;
    case ot2enc_cmd: E_parse_encoding(vb, c); return;
    case afterfi_cmd: E_afterfi(); return;
    case afterelsefi_cmd: E_afterelsefi(); return;
    case expandafter_cmd:
        if (c != 0U)
            E_unless();
        else
            E_expandafter();
        return;
    case l3expand_aux_cmd: E_l3expand_aux(c); return;
    case l3expand_base_cmd: E_l3expand_base(c); return;
    case l3noexpand_cmd: E_l3noexpand(c); return;
    case l3E_set_num_cmd: L3_set_num_code(c); return;
    case l3_ifx_cmd: E_l3_ifx(c); return;
    case l3str_ifeq_cmd: E_l3str_ifeq(c); return;
    case l3str_case_cmd: E_l3str_case(c); return;
    case token_if_cmd: l3_token_check(c); return;
    case cat_ifeq_cmd: E_cat_ifeq(c); return;
    case specchar_cmd: back_input(Token(other_t_offset, char32_t(char32_t(c)))); return;
    case splitfun_cmd: L3_user_split_next_name(c == 0); return;
    case user_cmd:
    case usero_cmd:
    case userl_cmd:
    case userlo_cmd:
    case userp_cmd:
    case userpo_cmd:
    case userlp_cmd:
    case userlpo_cmd: E_user(vb, c, C); return;
    case prg_return_cmd: E_prg_return(c); return;
    case first_of_two_cmd: E_first_of_two(vb, c); return;
    case first_of_three_cmd: E_first_of_three(vb, c); return;
    case first_of_four_cmd: E_first_of_four(vb, c); return;
    case gobble_cmd:
    case ignore_n_args_cmd: E_ignore_n_args(vb, c); return;
    case zapspace_cmd:
        if (c != 0U) {
            TokenList a = read_arg();
            token_ns::remove_first_last_space(a);
            back_input(a);
        } else
            E_zapspace();
        return;
    case stripprefix_cmd: read_until(Token(other_t_offset, '>')); return;
    case hexnumber_cmd: {
        TokenList L = read_arg();
        back_input(L);
        auto i = scan_int(cur_tok);
        if (i >= 0 && i < 10) back_input(Token(other_t_offset, uchar(i + '0')));
        if (i >= 10 && i < 16) back_input(Token(letter_t_offset, uchar(i + 'A' - 10)));
    }
        return;
    case strippt_cmd: {
        Token     TT = cur_tok;
        TokenList L  = E_the(the_code);
        token_ns::strip_pt(L);
        if (vb) {
            Logger::finish_seq();
            the_log << TT << "->" << L << ".\n";
        }
        back_input(L);
        return;
    }
    case obracket_cmd:
        if (vb) {
            Logger::finish_seq();
            the_log << cur_tok << "->$$\n";
        }
        back_input(hash_table.dollar_token);
        back_input(hash_table.dollar_token);
        return;
    case oparen_cmd:
        if (vb) {
            Logger::finish_seq();
            the_log << cur_tok << "->$\n";
        }
        back_input(hash_table.dollar_token);
        return;
    case noexpand_cmd: { // see comments in txscan
        auto guard2 = SaveScannerStatus(ss_normal);
        if (get_token()) return;
        Token t = cur_tok;
        if (vb) {
            Logger::finish_seq();
            the_log << "{\\noexpand " << t << "}\n";
        }
        back_input(t);
        if (!cur_tok.not_a_cmd()) back_input(hash_table.frozen_dont_expand);
        return;
    }
    case top_bot_mark_cmd: // no marks...
        if (c >= topmarks_code) scan_int(T);
        return;
    default: parse_error(cur_tok, "unexpandable command in expand? ", T, "", "bad"); return;
    }
}

// Insert \relax before current token.
// for instance \input foo\input bar.
void Parser::insert_relax() {
    back_input();
    cur_tok = hash_table.frozen_relax_token;
    back_input();
}

// Prints trace for \if in pass_text
// If k=-1, this is \if, must print \unless,
// If k=-2, this is \if, must not print \unless,
void Parser::trace_if(int k) {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "+";
        if (k == -1) the_log << "\\unless";
        the_log << cur_tok << conditions.top_serial();
        if (k > 0) the_log << "(+" << k << ")";
        the_log << "\n";
    }
}

void Parser::trace_if(String a, int k, String b) const {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "+" << a << k << " " << b << "\n";
    }
}

// same code
void Parser::trace_if(String a, int k, long b) const {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "+" << a << k << " " << b << "\n";
    }
}

// Find \fi, \else , \or at level zero.
void Parser::pass_text(Token Tfe) {
    int  l     = 0;
    auto guard = SaveScannerStatus(ss_skipping);
    for (;;) {
        if (get_token()) {
            cur_tok     = hash_table.fi_token;
            cur_cmd_chr = CmdChr(fi_or_else_cmd, fi_code);
            lost_if(Tfe, conditions.top_line());
            return;
        }
        if (cur_cmd_chr.cmd == fi_or_else_cmd) {
            trace_if(l);
            if (l == 0) return;
            if (cur_cmd_chr.chr == fi_code) l--;
        } else if (cur_cmd_chr.cmd == if_test_cmd || cur_cmd_chr.cmd == fpif_cmd) {
            l++;
            trace_if(l);
        }
    }
}

// This is called when we see \fi \or \else, and don't expect it via
// pass_text. If it's an \else, we call pass_text to get the \fi.
// the codes are, in order: if_code,fi_code,else_code,or_code.

void Parser::E_fi_or_else() {
    Token T = cur_tok;
    trace_if(0);
    if (conditions.empty()) {
        extra_fi_or_else();
        return;
    }
    int K = conditions.top_limit();
    if (K == if_code)
        insert_relax();
    else if (cur_cmd_chr.chr > K)
        extra_fi_or_else();
    else {
        while (cur_cmd_chr.chr != fi_code) pass_text(T);
        conditions.pop();
    }
}

void Parser::E_unless() {
    get_token();
    if (cur_cmd_chr.cmd == if_test_cmd && cur_cmd_chr.chr != if_case_code)
        E_if_test(cur_cmd_chr.chr, true);
    else {
        parse_error(err_tok, "You cannot use \\unless before ", cur_tok, "", "bad unless");
        back_input();
    }
}

void Parser::E_if_test(subtypes test, bool negate) {
    Token Tfe = cur_tok;
    auto  sz  = conditions.push(negate ? test + unless_code : test);
    int   k   = conditions.top_serial();
    trace_if(negate ? -1 : -2);
    if (test == if_case_code) {
        auto n = scan_int(cur_tok);
        trace_if("\\ifcase", k, n);
        while (n != 0) {
            pass_text(Tfe);
            if (conditions.is_this_if(sz)) {
                if (cur_cmd_chr.chr == or_code)
                    n--;
                else if (cur_cmd_chr.chr == fi_code) {
                    trace_if("\\ifcase", k, "failed");
                    conditions.pop();
                    return;
                } else {
                    trace_if("\\ifcase", k, "choose else");
                    conditions.wait_for_fi();
                    return;
                }
            } else if (cur_cmd_chr.chr == fi_code)
                conditions.pop();
        }
        conditions.set_limit(sz, or_code);
        trace_if("\\ifcase", k, "found");
        return;
    }
    bool b = eval_condition(test);
    if (negate) b = !b;
    trace_if("iftest", k, boolean(b));
    if (b) {
        conditions.set_limit(sz, else_code);
        return;
    }
    for (;;) {
        pass_text(Tfe);
        if (conditions.is_this_if(sz)) {
            if (cur_cmd_chr.chr != or_code) break;

            extra_fi_or_else();
        } else if (cur_cmd_chr.chr == fi_code)
            conditions.pop();
    }
    if (cur_cmd_chr.chr == fi_code)
        conditions.pop();
    else
        conditions.wait_for_fi();
}

// Returns the truth value of the next condition
auto Parser::eval_condition(subtypes test) -> bool {
    switch (test) {
    case if_true_code: return true;
    case if_false_code: return false;
    case ifx_code: return E_ifx();
    case if_char_code:
    case if_cat_code: {
        symcodes a1{}, a2{};
        subtypes b1{}, b2{};
        get_x_token_or_active_char(a1, b1);
        get_x_token_or_active_char(a2, b2);
        if (test == if_char_code) return b1 == b2;
        return a1 == a2;
    }
    case if_odd_code: {
        auto k = scan_int(cur_tok);
        return (k & 1) == 1;
    }
    case if_leapyear_code: return is_leap_year(scan_braced_int(cur_tok));
    case if_int_code:
    case if_dim_code: {
        Token T = cur_tok;
        if (test == if_int_code)
            scan_int(T);
        else
            scan_dimen(false, T);
        auto n = cur_val.get_int_val();
        remove_initial_space();
        auto r = cur_tok.val_as_other();
        if (r == '=' || r == '<' || r == '>') {
        } else {
            r = '=';
            missing_equals(T);
        }
        if (test == if_int_code)
            scan_int(T);
        else
            scan_dimen(false, T);
        if (r == '<') return n < cur_val.get_int_val();
        if (r == '>') return n > cur_val.get_int_val();
        return n == cur_val.get_int_val();
    }
    case if_mmode_code: return the_stack.get_mode() == mode_math;
    case if_vmode_code: return the_stack.in_v_mode();
    case if_hmode_code: return the_stack.in_h_mode();
    case if_eof_code: return is_input_open();
    case if_csname_code: fetch_name2(); return hash_table.is_defined(fetch_name_res);
    case if_defined_code: {
        auto guard = SaveScannerStatus(ss_normal);
        get_token();
        return !cur_cmd_chr.is_undef();
    }
    case if_fontchar_code: {
        auto k = scan_font_ident();
        scan_char_num(); // pretend all chars are in the font
        return k != 0;
    }
    case if_void_code: {
        auto n = scan_reg_num();
        Xml *x = box_table[n].val;
        if (x == nullptr) return true;
        return x->all_empty();
    }
    case if_hbox_code:
    case if_vbox_code: scan_reg_num(); return false; // pretend it's neither H nor V
    case if_inner_code:
        if (the_stack.get_mode() == mode_math) return is_inner_math();
        return false;
    default: return false;
    }
}

// Returns truth value of \ifx \A\B
auto Parser::E_ifx() -> bool {
    Token T = cur_tok;
    if (get_token_o()) {
        parse_error(T, "\\ifx at end of file");
        return false;
    }
    CmdChr pq = cur_cmd_chr;
    Token  a  = cur_tok;
    if (get_token_o()) {
        parse_error(T, "\\ifx at end of file");
        return false;
    }
    Token b = cur_tok;
    if (tracing_commands()) the_log << "\\ifx compares " << a << b << "\n";
    if (cur_cmd_chr.cmd != pq.cmd) return false;
    if (cur_cmd_chr.is_user()) {
        Macro &A = mac_table.get_macro(cur_cmd_chr.chr);
        Macro &B = mac_table.get_macro(pq.chr);
        return A.is_same(B);
    }
    return (cur_cmd_chr.chr == pq.chr);
}

void Parser::E_afterfi() {
    Token     T = hash_table.fi_token;
    TokenList L = read_until(T);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << cur_tok << "#1\\fi->\\fi#1\n#1<-" << L << "\n";
    }
    back_input(L);
    back_input(T);
}

void Parser::E_afterelsefi() {
    Token     T1 = hash_table.else_token;
    Token     T2 = hash_table.fi_token;
    TokenList L1 = read_until(T1);
    TokenList L2 = read_until(T2);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << cur_tok << R"(#1\else#2\fi->\fi#1\n#1<-)" << L1 << "#2<-" << L2;
    }
    back_input(L1);
    back_input(T2);
}

// Definitions -------------------

// Reads a name to define; returns eqtb loc of token
// If br is true (default value false), reads an argument in braces;
// initial explicit spaces are ignored
auto Parser::get_r_token(bool br) -> Token {
    for (;;) {
        get_token();
        if (!cur_tok.is_space_token()) break;
    }
    if (!cur_tok.not_a_cmd()) return cur_tok; // OK
    if (br && cur_cmd_chr.is_open_brace()) {
        TokenList L;
        skip_group0(L);
        if (L.empty()) {
            parse_error(err_tok, "Empty argument list for ", err_tok, "", "empty argument");
            cur_tok = hash_table.frozen_protection;
        } else if (L.size() != 1) {
            get_new_command_aux(L); // signals an error
            cur_tok = hash_table.frozen_protection;
        } else
            cur_tok = token_ns::get_unique(L);
    }
    if (cur_tok.not_a_cmd()) {
        bad_redefinition(2, cur_tok);
        cur_tok = hash_table.frozen_protection;
    }
    see_cs_token(); // sets cur_cmd_chr is this needed ?
    return cur_tok;
}

// Internal code of \let\a\b. Here \b is in cur_tok (for tracing purpose)
// and the value in cur_cmd_chr
// If redef is true, it is an error to redefine the command

void Parser::M_let(Token A, bool global, bool redef) {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\let " << A << " " << cur_tok << "}\n";
    }
    if (redef && !ok_to_define(A, rd_if_undef)) return;
    auto pos = A.eqtb_loc();
    if (tracing_assigns()) {
        String action = global ? "globally " : "";
        Logger::finish_seq();
        the_log << "{" << action << "changing " << A << "=";
        token_for_show(hash_table.eqtb[pos].val);
        the_log << "}\n{into " << A << "=";
        token_for_show(cur_cmd_chr);
        the_log << "}\n";
    }
    eq_define(pos, cur_cmd_chr, global);
}

// as above, calls get_token in order to set cur_tok and cur_cmd_chr
void Parser::M_let(Token a, Token b, bool gbl, bool redef) {
    if (a.not_a_cmd()) {
        bad_redefinition(2, a);
        return;
    }
    examine_token(b);
    M_let(a, gbl, redef);
}

// Assumes a and b are command names
void Parser::M_let_fast(Token a, Token b, bool gbl) {
    see_cs_token(b);
    M_let(a, gbl, false);
}

// You can say \let\a= b, with optional =, one optional space after =
void Parser::M_let(bool gbl) {
    Token A = get_r_token();
    skip_initial_space();
    if (cur_tok.is_equal_token()) {
        get_token();
        if (cur_cmd_chr.is_space()) get_token();
    }
    M_let(A, gbl, false);
}

// first \let, \futurelet then L3 variants
// {\let\foo\undef \cs_undefine:c{foo}} leaves \foo unchanged; ok ?
void Parser::M_let(int chr, bool gbl) {
    if (chr == let_code)
        M_let(gbl);
    else if (chr == futurelet_code)
        M_future_let(gbl);
    else if (chr == undef_code || chr == undefc_code) {
        Token T;
        if (chr == undefc_code) { // \cs_undefine:c
            TokenList a = read_arg();
            back_input(hash_table.frozen_endcsname);
            back_input(a);
            fetch_name2();
            if (!hash_table.is_defined(fetch_name_res)) return;
            T = hash_table.locate(fetch_name_res);
        } else
            T = get_r_token(true); // \cs_undefine:N
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{\\let " << T << "=\\undef}\n";
        }
        if (tracing_assigns()) {
            Logger::finish_seq();
            the_log << "{globally killing " << T << "}\n";
        }
        eq_define(T.eqtb_loc(), CmdChr(undef_cmd, zero_code), true);
    } else {               // cs_set_eq:NN and variants
        gbl       = false; // ignore \globaldefs
        bool nnew = false;
        if (chr >= 8) {
            nnew = true;
            gbl  = true;
            chr -= 8;
        } else if (chr >= 4) {
            gbl = true;
            chr -= 4;
        }
        Token a;
        if (chr == 1 || chr == 3)
            a = fetch_csname(true);
        else {
            get_token();
            a = cur_tok;
        }
        if (chr == 2 || chr == 3)
            cur_tok = fetch_csname(true);
        else
            get_token();
        M_let(a, cur_tok, gbl, nnew);
    }
}

// This implements \futurelet\A\B\C
void Parser::M_future_let(bool gbl) {
    Token A = get_r_token();
    auto  p = cur_tok.eqtb_loc();
    get_token();
    Token B = cur_tok;
    get_token();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\futurelet " << A << " ";
        the_log << B << " ";
        the_log << cur_tok << "}\n";
    }
    eq_define(p, cur_cmd_chr, gbl); // \let\A\C
    back_input();
    back_input(B); // to be read again \B and \C
}

// This implements \newcommand and variants
void Parser::M_newcommand(rd_flag redef) {
    bool     is_star = remove_initial_star(); // false means long
    symcodes what    = is_star ? user_cmd : userl_cmd;
    Token    name    = get_r_token(true);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << err_tok << (is_star ? "* " : " ") << name << "}\n";
    }
    Macro *X{nullptr};
    {
        auto guard = SaveErrTok(name);
        X          = read_latex_macro();
        if (redef == rd_never) { // case \CheckCommand
            bool is_same = true;
            auto pq      = hash_table.eqtb[name.eqtb_loc()].val;
            if (pq.cmd != what)
                is_same = false;
            else {
                Macro &B = mac_table.get_macro(pq.chr);
                is_same  = X->is_same(B);
            }
            if (!is_same) {
                Thbuf1 = fmt::format("Tralics Warning: Command {} has changed\n", name);
                out_warning(Thbuf1, mt_warning);
            }
        }
    }
    mac_define(name, X, false, redef, what);
}

// Code of \newenvironment{foo}...
void Parser::M_new_env(rd_flag redef) {
    bool        is_star = remove_initial_star(); // false means long
    symcodes    what    = is_star ? user_cmd : userl_cmd;
    std::string name    = group_to_string();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << err_tok << (is_star ? "* " : " ") << name << "}\n";
    }
    Token  T = find_env_token(name, true); // this is \foo
    Macro *X{nullptr};
    {
        auto guard = SaveErrTok(T);
        X          = read_latex_macro();
    }
    mac_define(T, X, false, redef, what);
    T = find_env_token(name, false);
    X = new Macro;
    {
        auto guard = SaveErrTok(T);
        read_mac_body(X->body, false, 0);
        X->correct_type();
    }
    mac_define(T, X, false, redef, what);
}

// Common code for \newcommand and \newenv, constructs the macro
auto Parser::read_latex_macro() -> Macro * {
    auto *X = new Macro;
    auto  n = read_mac_nbargs();
    X->set_nbargs(n);
    TokenList op_arg;
    bool      have_op_arg = read_optarg(op_arg);
    X->set_delimiters(1, op_arg);
    X->set_type(have_op_arg ? dt_optional : dt_normal);
    read_mac_body(X->body, false, n);
    X->correct_type();
    return X;
}

// \def, \xdef, \gdef, \edef, latex variants and latex3 variants
// \long \protected flags are integrated in w
void Parser::define_something(int chr, bool gbl, symcodes w) {
    switch (chr) {
    case def_code: M_def(false, gbl, w, rd_always); return; // hack
    case newcommand_code: M_newcommand(rd_if_undef); return;
    case checkcommand_code: M_newcommand(rd_never); return;
    case newthm_code: M_new_thm(); return;
    case newenv_code: M_new_env(rd_if_undef); return;
    case renewenv_code: M_new_env(rd_if_defined); return;
    case renew_code: M_newcommand(rd_if_defined); return;
    case provide_code: M_newcommand(rd_skip); return;
    case declare_math_operator_code: M_declare_math_operator(); return;
    }
    bool csname = false;
    if (32 <= chr) {
        chr -= 32;
        csname = true;
    }
    rd_flag fl = rd_always;
    if (16 <= chr) {
        chr -= 16;
        fl = rd_if_undef;
    }
    // 16 case; expand, global, protected, long
    if (chr == gdef_code || chr == xdef_code || chr == lgdef_code || chr == lxdef_code || chr == pgdef_code || chr == pxdef_code ||
        chr == plgdef_code || chr == plxdef_code) {
        chr--;
        gbl = true;
    }
    bool exp = false;
    if (chr == edef_code || chr == ledef_code || chr == pedef_code || chr == pledef_code) {
        chr -= 2;
        exp = true;
    }
    if (chr == ldef_code || chr == pldef_code) {
        if (w == user_cmd || w == usero_cmd || w == userp_cmd || w == userpo_cmd) w = (symcodes(w + 1));
    }
    if (chr == pdef_code || chr == pldef_code) {
        if (w == user_cmd || w == userl_cmd || w == usero_cmd || w == userlo_cmd) w = (symcodes(w + 2));
    }
    if (csname) {
        back_input(fetch_csname(true));
        cur_tok = err_tok; // hack
    }
    M_def(exp, gbl, w, fl);
}

// This handles \catcode, \lccode etc, in a set context.
void Parser::assign_def_something(bool gbl) {
    Token  T      = cur_tok;
    int    n      = 0;
    size_t offset = cur_cmd_chr.chr;
    if (offset == 0)
        n = 15; // catcode
    else if (offset == math_code_offset)
        n = 0100000;
    else if (offset == sf_code_offset)
        n = 077777;
    else if (offset == del_code_offset)
        n = 077777777;
    else if (offset == lc_code_offset || offset == uc_code_offset)
        n = nb_characters - 1;
    else
        n = 0; // This should not happen
    auto k = to_signed(scan_char_num());
    offset += to_unsigned(k);
    scan_optional_equals();
    k = scan_int(T);
    if ((k < 0 && offset < del_code_offset) || k > n) {
        signal_ovf(T, "Invalid code (out of bounds)\n", k, n);
        k = 0;
    }
    word_define(offset, k, gbl);
}

// Simplified for latex3; requires a macro name. returns true if error
auto Parser::l3_get_name(Token T) -> bool {
    if (get_token()) return true;
    if (cur_tok.not_a_cmd()) {
        parse_error(T, "Expecting a command name instead of ", cur_tok, "", "name expected");
        return true;
    }
    return false;
}

// This interprets \countdef, and things like that
void Parser::M_shorthand_define(subtypes cmd, bool gbl) {
    Token t   = cur_tok;
    Token tbd = get_r_token();
    if (tbd == hash_table.frozen_protection) return;
    auto pos = cur_tok.eqtb_loc();
    eq_define(pos, CmdChr(relax_cmd, relax_code), gbl);
    scan_optional_equals();
    cur_tok    = t;
    size_t   k = 0;
    symcodes ncmd{};
    String   name = "unknown";
    switch (cmd) {
    case char_def_code:
        name = "chardef";
        k    = scan_27bit_int();
        ncmd = char_given_cmd;
        break;
    case math_char_def_code:
        name = "mathchardef";
        k    = scan_fifteen_bit_int();
        ncmd = math_given_cmd;
        break;
    case count_def_code:
        name = "countdef";
        k    = scan_reg_num();
        k += count_reg_offset;
        ncmd = assign_int_cmd;
        break;
    case dimen_def_code:
        name = "dimendef";
        k    = scan_reg_num();
        ncmd = assign_dimen_cmd;
        break;
    case skip_def_code:
        name = "skipdef";
        k    = scan_reg_num();
        ncmd = assign_glue_cmd;
        break;
    case mu_skip_def_code:
        name = "muskipdef";
        k    = scan_reg_num();
        k += muskip_reg_offset;
        ncmd = assign_mu_glue_cmd;
        break;
    case toks_def_code:
        name = "toksdef";
        k    = scan_reg_num();
        ncmd = assign_toks_cmd;
        break;
    default: return;
    }
    CmdChr R(ncmd, subtypes(k));
    eq_define(pos, R, gbl);
    Logger::finish_seq();
    // the_log << "{\\" << name << " " << tbd << "=\\" << R.name() << "}\n";
}

// For bootstrap; always traced
auto Parser::shorthand_gdefine(int cmd, String sh, int k) -> Token {
    Token    T = hash_table.locate(sh);
    auto     p = T.eqtb_loc(); // return value
    symcodes ncmd{};
    String   name{nullptr};
    switch (cmd) {
    case char_def_code:
        name = "chardef";
        ncmd = char_given_cmd;
        break;
    case math_char_def_code:
        name = "mathchardef";
        ncmd = math_given_cmd;
        break;
    case count_def_code:
        name = "countdef";
        k += count_reg_offset;
        ncmd = assign_int_cmd;
        break;
    case dimen_def_code:
        name = "dimendef";
        ncmd = assign_dimen_cmd;
        break;
    case skip_def_code:
        name = "skipdef";
        ncmd = assign_glue_cmd;
        break;
    case mu_skip_def_code:
        name = "muskipdef";
        k += muskip_reg_offset;
        ncmd = assign_mu_glue_cmd;
        break;
    case toks_def_code:
        name = "toksdef";
        ncmd = assign_toks_cmd;
        break;
    default: // should not happen
        return T;
    }
    CmdChr R(ncmd, subtypes(k));
    eq_define(p, R, true);
    Logger::finish_seq();
    // the_log << "{\\" << name << " \\" << sh << "=\\" << R.name() << "}\n";
    return T;
}

// q is register, advance, multiply, divide
// We may have seen \count: here we read a number, the register number.
// We may have seen \advance, and read \count. Then we read the number.
// We may have seen \advance, then \foo, where \foo is defined by
// \countdef, so that we can return after seeing \foo.
// In any case, returns a position and sets p to the type.
auto Parser::do_register_arg(int q, int &p, Token &tfe) -> size_t {
    Token T = cur_tok;
    if (q != register_cmd) {
        get_x_token();
        tfe    = cur_tok;
        auto m = cur_cmd_chr.chr;
        if (cur_cmd_chr.cmd == assign_int_cmd) {
            p = it_int;
            return m;
        }
        if (cur_cmd_chr.cmd == assign_dimen_cmd) {
            p = it_dimen;
            return m;
        }
        if (cur_cmd_chr.cmd == assign_glue_cmd) {
            p = it_glue;
            return m;
        }
        if (cur_cmd_chr.cmd == assign_mu_glue_cmd) {
            p = it_mu;
            return m;
        }
    }
    if (cur_cmd_chr.is_undef()) {
        undefined_mac();
        return 0;
    }
    // T is caller,
    if (cur_cmd_chr.cmd != register_cmd) {
        parse_error(T, "Invalid argument for ", T, "", "bad register");
        cur_tok.kill();
        return 0;
    }
    p      = cur_cmd_chr.chr;
    auto m = scan_reg_num();
    if (p == it_int) return m + count_reg_offset;
    if (p == it_dimen) return m;
    if (p == it_glue) return m;
    return m + muskip_reg_offset;
}

// This is a more complex function.
// Handles \advance\skip0 by \skip1, \divide \skip0 by \count1

void Parser::do_register_command(bool gbl) {
    Token T = cur_tok;
    int   q = cur_cmd_chr.cmd;
    int   p = 0;
    auto  l = do_register_arg(q, p, T); // changes T from \advance to \skip
    if (cur_tok.is_invalid()) return;   // was an error
    if (q == register_cmd)
        scan_optional_equals();
    else
        scan_keyword("by");
    if (q < multiply_cmd) { // advance or set.
        if (p < it_glue) {
            if (p == it_int)
                scan_int(T);
            else
                scan_dimen(false, T);
            if (q == advance_cmd) {
                if (p == it_int)
                    cur_val.incr_int(eqtb_int_table[l].val);
                else
                    cur_val.incr_dim(eqtb_dim_table[l].val);
            }
        } else {
            scan_glue(internal_type(p), T);
            if (q == advance_cmd) cur_val.incr_glue(glue_table[l].val);
        }
    } else {
        auto v = scan_int(T); // Here we need an integer.
        if (p < it_glue) {
            if (p == it_int)
                cur_val.set_int(eqtb_int_table[l].val);
            else
                cur_val.set_dim(eqtb_dim_table[l].val);
            ScaledInt &W = cur_val.int_val;
            if (q == multiply_cmd)
                if (p == it_int)
                    W.mult_integer(v);
                else
                    W.mult_scaled(v);
            else { // division
                W.divide(v);
            }
        } else {
            Glue s = glue_table[l].val;
            if (q == multiply_cmd)
                s.multiply(cur_val.get_int_val());
            else
                s.divide(cur_val.get_int_val());
            cur_val.set_glue_val(s);
        }
    }
    if (p == it_dimen)
        dim_define(l, cur_val.get_dim_val(), gbl);
    else if (p < it_glue)
        word_define(l, cur_val.get_int_val(), gbl);
    else
        glue_define(l, cur_val.glue_val, gbl);
}

// This implements \tracingall
// Last assignment should be \tracingassigns...
void Parser::M_tracingall() {
    word_define(tracingmacros_code, 1, false);
    word_define(tracingoutput_code, 1, false);
    word_define(tracingcommands_code, 1, false);
    word_define(tracingrestores_code, 1, false);
    word_define(tracingmath_code, 1, false);
    word_define(tracingassigns_code, 1, false);
}

// This implements \arabic, \@arabic, etc
void Parser::E_latex_ctr() {
    int       t = cur_cmd_chr.chr;
    Token     T = cur_tok;
    long      n = 0;
    TokenList res;
    if (t < at_number_code) {
        M_counter(false);
        get_token();
        res.push_back(cur_tok);
    } else
        n = scan_int(T);
    switch (t) {
    case arabic_code: res.push_front(hash_table.number_token); break;
    case roman_code: res.push_front(hash_table.roman_numeral_token); break;
    case Roman_code: res.push_front(hash_table.Roman_numeral_token); break;
    case alph_code: res.push_front(hash_table.atalph_token); break;
    case Alph_code: res.push_front(hash_table.atAlph_token); break;
    case fnsymbol_code: res.push_front(hash_table.fnsymbol_token); break;
    case at_alph_code:
        if (n >= 1 && n <= 26)
            res.push_front(Token(letter_t_offset + 'a' - 1 + to_unsigned(n)));
        else
            counter_overflow(T, n, 26);
        break;
    case at_Alph_code:
        if (n > 0 && n <= 26)
            res.push_front(Token(letter_t_offset + 'A' - 1 + to_unsigned(n)));
        else
            counter_overflow(T, n, 26);
        break;
    case at_fnsymbol_code:
        if (n <= 0 || n > 9)
            counter_overflow(T, n, 9);
        else
            E_latex_ctr_fnsymbol(n, res);
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "->" << res << "\n";
    }
    back_input(res);
}

// In some cases,  we must enter math mode.
void Parser::E_latex_ctr_fnsymbol(long n, TokenList &res) const {
    if (n == 1) {
        res.push_back(hash_table.star_token);
        return;
    }
    if (n == 4) {
        res.push_back(Token(other_t_offset, uchar('\247')));
        return;
    }
    if (n == 5) {
        res.push_back(Token(other_t_offset, uchar('\266')));
        return;
    }
    if (n == 7) {
        res.push_back(hash_table.star_token);
        res.push_back(hash_table.star_token);
        return;
    }
    if (n == 2) { res.push_back(hash_table.dagger_token); }
    if (n == 6) { res.push_back(hash_table.vbar_token); }
    if (n == 3) { res.push_back(hash_table.ddagger_token); }
    if (n == 8) {
        res.push_back(hash_table.dagger_token);
        res.push_back(hash_table.dagger_token);
    }
    if (n == 9) {
        res.push_back(hash_table.ddagger_token);
        res.push_back(hash_table.ddagger_token);
    }
    brace_me(res);
    res.push_front(hash_table.ensuremath_token);
}

// Implementation of \ifthenelse ------------------------------

// \setboolean{foo}{true} is the same as \footrue.
// You can say TRUE instead of true.
void Parser::set_boolean() {
    Token     T = cur_tok;
    TokenList A = read_arg();
    TokenList B = read_arg();
    String    s = "";
    Buffer    b;
    list_to_string_cv(B, b);
    if (b == "true")
        s = "true";
    else if (b == "false")
        s = "false";
    else {
        parse_error(T, "boolean value neither true nor false: " + b);
        return;
    }
    if (my_csname("", s, A, "setboolean")) return;
    if (hash_table.eqtb[cur_tok.eqtb_loc()].val.is_relax()) {
        get_token();
        parse_error(T, "Undefined boolean ", cur_tok, "", "undefined boolean");
    }
}

// \( and \) serve the role of braces.
void Parser::skip_over_parens() {
    int b = 1;
    for (;;) {
        if (get_token()) return; // no error ?
        if (cur_tok.is_bs_oparen()) b++;
        if (cur_tok.is_bs_cparen()) b--;
        if (b == 0) return;
    }
}

inline auto skip_or_continue(bool s) -> String { return s ? "skipping" : "continuing"; }

// This is the \ifthenelse function
void Parser::T_ifthenelse() {
    Token     t = cur_tok;
    TokenList A = read_arg();
    cur_tok     = t;
    bool b      = T_ifthenelse(A);
    one_of_two(b);
}

// This evaluates a condition in an environment.
auto Parser::T_ifthenelse(TokenList &A) -> bool {
    Token     t = cur_tok;
    SaveState st_state(TL, restricted);
    restricted = true;
    TL.swap(A);
    bool b = T_ifthenelse_inner(t);
    return b;
}

// This returns true if the ifthenelse condition should be true.
// is recursive...
auto Parser::T_ifthenelse_inner(Token t) -> bool {
    bool res         = true;
    bool must_negate = false;
    for (;;) {
        if (get_token()) break; // seen the end
        Token x = cur_tok;
        if (x.is_space_token() || cur_cmd_chr.is_relax()) continue;
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{ifthenelse " << x << "}\n";
        }
        if (x == hash_table.not_token || x == hash_table.NOT_token) {
            must_negate = !must_negate;
            continue;
        }
        if (x.is_bs_oparen()) {
            res = T_ifthenelse_inner(t);
            skip_over_parens();
        } else if (x == hash_table.isodd_token) {
            TokenList L = read_arg();
            back_input(hash_table.relax_token);
            back_input(L);
            auto n = scan_int(x);
            res    = (n & 1) != 0;
        } else if (x == hash_table.isundefined_token) {
            TokenList L = read_arg();
            if (L.empty())
                res = false;
            else {
                Token target = L.front();
                token_from_list(target);
                res = cur_cmd_chr.is_undef();
            }
        } else if (x == hash_table.lengthtest_token) {
            TokenList L = read_arg();
            back_input(hash_table.relax_token);
            back_input(L);
            cur_tok = t;
            res     = eval_condition(if_dim_code);
        } else if (x == hash_table.boolean_token) {
            TokenList L = read_arg();
            if (my_csname("if", "", L, "boolean"))
                res = false;
            else {
                get_token();
                res = cur_cmd_chr.cmd == if_test_cmd && cur_cmd_chr.chr == if_true_code;
            }
        } else if (x == hash_table.equal_token) {
            TokenList L1 = read_arg();
            TokenList L2 = read_arg();
            cur_tok      = t;
            read_toks_edef(L1);
            cur_tok = t;
            read_toks_edef(L2);
            res = L1 == L2;
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{ifthenelse equal " << boolean(res) << "}\n";
            }
        } else {
            back_input();
            cur_tok = t;
            res     = eval_condition(if_int_code);
        }
        break;
    }
    if (must_negate) res = !res;
    for (;;) {                  // we have the value. Look at what remains.
        if (get_token()) break; // seen the end
        Token x = cur_tok;
        if (x.is_space_token() || cur_cmd_chr.is_relax()) continue;
        if (x.is_bs_cparen()) {
            back_input(x);
            break;
        }
        if (x == hash_table.or_token || x == hash_table.OR_token) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{ifthenelse \\or " << skip_or_continue(res) << "}\n";
            }
            if (res) break;
            return T_ifthenelse_inner(t);
        }
        if (x == hash_table.and_token || x == hash_table.AND_token) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{ifthenelse \\and " << skip_or_continue(!res) << "}\n";
            }
            if (!res) return false;
            return T_ifthenelse_inner(t);
        }
        break;
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{ifthenelse -> " << boolean(res) << "}\n";
    }
    return res;
}

// Evaluates one of the two token lists, depending on the flag.
void Parser::one_of_two(TokenList &A, TokenList &B, bool which) {
    if (which) {
        back_input(A);
    } else {
        back_input(B);
    }
}

// Like above, but reads the lists first.
void Parser::one_of_two(bool which) {
    TokenList A = read_arg();
    TokenList B = read_arg();
    one_of_two(A, B, which);
}

// if A is true, then execute B and restart
void Parser::T_whiledo() {
    Token     t  = cur_tok;
    TokenList A  = read_arg();
    TokenList B  = read_arg();
    cur_tok      = t;
    TokenList AA = A;
    bool      b  = T_ifthenelse(AA);
    if (!b) return;
    TokenList BB = B;
    back_input_braced(BB);
    back_input_braced(A);
    back_input(t);
    back_input(B);
}

// implementation of the calc package.

// We want to multiply res by val.
void Parser::calc_spec_mul(RealNumber val, SthInternal &res) {
    cur_val = res;
    cur_val.change_level(it_dimen);
    multiply_dim(val, cur_val.get_int_val());
    res.copy(cur_val);
}

// This computes num/den with 6 decimals (why not use C++ code ?)
// and constructs \pm(tmp+f/2^16)
// After that, we multiply.
void Parser::calc_ratio_eval(long num, long den, SthInternal &res) {
    static const unsigned int my_max = ((1U << 31) - 1);
    RealNumber                val;
    if (num < 0) {
        val.negative = true;
        num          = -num;
    }
    if (den < 0) {
        val.negative = !val.negative;
        den          = -den;
    }
    while (den >= int(my_max / 10)) {
        den /= 2;
        num /= 2;
    }
    if (den == 0) {
        parse_error(hash_table.calc_token, "Division by zero");
        return;
    }
    auto A    = num / den;
    val.ipart = A;
    std::array<long, 17> table{};
    for (size_t k = 0; k < 10; k++) {
        num -= A * den;
        num *= 10;
        A        = num / den;
        table[k] = A;
    }
    val.convert_decimal_part(10, table.data()); // \todo pass the std::array instead
    calc_spec_mul(val, res);
}

// Multiplies res by what follows (we must remove braces...)
void Parser::calc_mul_aux(SthInternal &res) {
    TokenList x = read_arg();
    back_input(hash_table.space_token); // in case...
    back_input(x);
    RealNumber val;
    val.negative = scan_sign();
    back_input();
    scan_double(val);
    skip_initial_space_and_back_input();
    calc_spec_mul(val, res);
}

// Reads something, and puts it in A.
// Note: \widthof{box} is not handled. We return 0.
void Parser::calc_primitive(SthInternal &A) {
    if (get_token()) {
        parse_error(hash_table.calc_token, "eof in calc");
        return;
    }
    if (cur_tok.is_open_paren())
        calc_aux(A);
    else if (cur_cmd_chr.cmd == unimp_cmd &&
             (cur_cmd_chr.chr == widthof_code || cur_cmd_chr.chr == depthof_code || cur_cmd_chr.chr == heightof_code)) {
        ignore_arg();
        A.initialise(A.type); // this puts 0 in A.
    } else {
        back_input();
        scan_optional_equals(); // can do no harm.
        if (A.is_dimen()) {
            scan_dimen(false, hash_table.calc_token);
            A.int_val = cur_val.int_val;
        } else if (A.is_glue()) {
            scan_glue(it_glue, hash_table.calc_token);
            A.set_glue_val(cur_val.glue_val);
        } else {
            scan_int(hash_table.calc_token);
            A.int_val = cur_val.int_val;
        }
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{calc primitive =" << A << "}\n";
    }
}

// This is the big function
void Parser::calc_aux(SthInternal &A) {
    skip_initial_space_and_back_input();
    calc_primitive(A);
    for (;;) {
        skip_initial_space();
        Token T = cur_tok;               // what next ?
        if (T.is_exclam_token()) return; // special end marker.
        if (T.is_close_paren()) return;
        if (T.is_plus_token() || T.is_minus_token()) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{calc +-}\n";
            }
            SthInternal B;
            B.initialise(A.type);
            calc_primitive(B);
            if (T.is_minus_token()) B.negate();
            A.add(B);
            continue;
        }
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{calc */}\n";
        }
        if (!(T.is_star_token() || T.is_slash_token())) {
            parse_error(hash_table.calc_token, "unexpected token in calc\n", T.tok_to_str(), "unexpected in calc");
            return;
        }
        bool mul = T.is_star_token();
        skip_initial_space();
        T = cur_tok;
        if (T == hash_table.ratio_token) {
            TokenList x = read_arg();
            TokenList y = read_arg();
            if (mul)
                ratio_evaluate(x, y, A);
            else
                ratio_evaluate(y, x, A);
            continue;
        }
        if (T == hash_table.real_token) {
            if (!mul) { // special case 3 / \real{1.5} ...
                TokenList x = read_arg();
                x.push_back(Token(other_t_offset, 'p'));
                x.push_back(Token(other_t_offset, 't'));
                TokenList y; // empty
                ratio_evaluate(y, x, A);
                continue;
            } // case 3 * \real{1.5}
            calc_mul_aux(A);
            continue;
        }
        // Could be anything, says (2+3)*(4+5);
        // Must be an integer...
        SthInternal aux;
        aux.initialise(it_int);
        back_input();
        calc_primitive(aux);
        auto v = aux.get_int_val();
        if (mul) {
            if (A.is_glue())
                A.glue_multiply(v);
            else if (A.is_int())
                A.int_val.mult_integer(v);
            else
                A.int_val.mult_scaled(v);
        } else {
            if (A.is_glue())
                A.glue_divide(v);
            else
                A.int_val.divide(v);
        }
    }
}

// We want to multiply res by A/B.
void Parser::ratio_evaluate(TokenList &A, TokenList &B, SthInternal &res) {
    SthInternal num, den;
    num.initialise(it_int);
    den.initialise(it_int);
    if (A.empty())
        num.int_val = 1 << 16; // Same as 1pt, but faster.
    else
        calc_main(it_dimen, num, A);
    calc_main(it_dimen, den, B);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{calc \\ratio " << num << "/" << den << "}\n";
    }
    calc_ratio_eval(num.get_int_val(), den.get_int_val(), res);
}

void Parser::calc_main(internal_type type, SthInternal &res, TokenList &B) {
    SthInternal A;
    A.initialise(type);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{calc argument: " << B << "}\n";
    }
    back_input(Token(other_t_offset, '!'));
    back_input(B);
    calc_aux(A);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{calc res " << A << "}\n";
    }
    res.type = type;
    res.copy(A);
}

void Parser::exec_calc() {
    TokenList a       = read_arg();
    TokenList b       = read_arg();
    bool      gbl     = false;
    bool      advance = false;
    if (a.front() == hash_table.global_token) {
        gbl = true;
        a.pop_front();
    }
    if (a.front() == hash_table.advance_token) {
        advance = true;
        a.pop_front();
    }
    Token T = a.front();
    a.pop_front();
    token_from_list(T);
    internal_type p{};
    unsigned      l = cur_cmd_chr.chr;
    if (cur_cmd_chr.cmd == assign_int_cmd)
        p = it_int;
    else if (cur_cmd_chr.cmd == assign_dimen_cmd)
        p = it_dimen;
    else
        p = it_glue; // ok ? should add a test here....
    if (tracing_commands()) {
        String s = p == it_int ? "integer" : (p == it_dimen ? " dimension" : "glue");
        Logger::finish_seq();
        the_log << "{calc modifying " << s << " at position " << to_signed(l) << "}\n";
    }
    SthInternal res;
    calc_main(p, res, b);
    cur_val = res;
    if (advance) {
        if (p == it_glue)
            cur_val.incr_glue(glue_table[l].val);
        else if (p == it_dimen)
            cur_val.incr_dim(eqtb_dim_table[l].val);
        else
            cur_val.incr_int(eqtb_int_table[l].val);
    }
    if (p == it_glue)
        glue_define(l, cur_val.glue_val, gbl);
    else if (p == it_dimen)
        dim_define(l, cur_val.get_dim_val(), gbl);
    else
        word_define(l, cur_val.get_int_val(), gbl);
}

// You say \strip@pt\dimen25, this gives 34.37 or 34 (no dot)
// Assume arg is a dimension

void Parser::dimen_from_list0(Token T, TokenList &L) {
    back_input(hash_table.relax_token);
    back_input(L);
    scan_dimen(false, T);
    read_until(hash_table.relax_token);
}

auto Parser::dimen_from_list(Token T, TokenList &L) -> ScaledInt {
    if (!calc_loaded)
        dimen_from_list0(T, L);
    else
        calc_main(it_dimen, cur_val, L);
    return cur_val.get_dim_val();
}

// Let M be the number of registers (1024)
// scan_box is used by \shipout, \leaders, etc with an argument > 2M+2,
// is used by \moveleft, etc, with 2M as argument,
// is used by \setbox with argument < M,
// to which M is added if the assignment is global.
// The command reads a box, and performs some action.
// Exception: you can say \cleaders\hrule.

void Parser::scan_box(size_t bc) {
    remove_initial_space_relax();
    if (cur_cmd_chr.cmd == make_box_cmd)
        begin_box(bc, cur_cmd_chr.chr);
    else if (bc >= leaders_location && cur_cmd_chr.cmd == rule_cmd) {
        scan_rule(cur_cmd_chr.chr);
        box_end(the_stack.remove_last(), bc);
    } else
        parse_error(err_tok, "A box was assumed to be here, got ", cur_tok, "", "Missing box");
}

// In case of \leaders\glue or \leaders\hbox{} a box or a rule is fetched;
// It will be put in a element named <leaders>. In the case of \box
// we fill the box (numbered pos or pos-M).
// An error is signaled in the case of \shipout, the box is inserted otherwise.
void Parser::box_end(Xml *res, size_t pos) {
    if (pos < last_register)
        box_define(pos, res, false);
    else if (pos <= setbox_last)
        box_define(pos - setbox_offset, res, true);
    else if (pos == shipout_location)
        parse_error(err_tok, "\\shipout is undefined");
    else if (pos == xmlA_location)
        the_xmlA = res;
    else if (pos == xmlB_location)
        the_xmlB = res;
    else if (pos == leaders_location || pos == cleaders_location || pos == xleaders_location) {
        const auto *p = pos == leaders_location ? "leaders" : pos == cleaders_location ? "cleaders" : "xleaders";
        Xml *       Y = new Xml(the_names[p], res);
        flush_buffer();
        the_stack.add_last(Y);

    } else {
        flush_buffer();
        the_stack.add_last(res);
    }
}

// This function is called from scan_box, see above,
// or \box\copy\usebox\hbox etc, src is 2M in these cases
// In some cases, the box is already there, and the code is easy.
// Otherwise we push a continuation on the stack.

void Parser::begin_box(size_t src, subtypes c) {
    Token  T   = cur_tok;
    size_t res = 0;
    Xml *  cur_box{nullptr};
    if (c == usebox_code) { // a variant of \copy with an argument
        leave_v_mode();
        TokenList L = read_arg();
        back_input(hash_table.relax_token);
        back_input(L);
        c = copy_code;
    }
    if (c == box_code) {
        res                = scan_reg_num();
        cur_box            = box_table[res].val;
        box_table[res].val = nullptr;
        box_end(cur_box, src);
        return;
    }
    if (c == copy_code) {
        res     = scan_reg_num();
        cur_box = box_table[res].val;
        box_end(cur_box, src);
        return;
    }
    if (c == lastbox_code) {
        flush_buffer();
        cur_box = the_stack.top_stack()->last_box();
        box_end(cur_box, src);
        return;
    }
    if (c == useboxA_code || c == useboxB_code) {
        cur_box = (c == useboxA_code ? the_xmlA : the_xmlB);
        if (cur_box != nullptr)
            cur_box = cur_box->deep_copy();
        else
            cur_box = new Xml(std::string(), nullptr);
        box_end(cur_box, src);
        return;
    }
    if (c == useboxB_code) {
        cur_box = the_stack.top_stack()->last_box();
        box_end(cur_box, src);
        return;
    }
    if (c == vsplit_code) {
        scan_reg_num();
        scan_keyword("to");
        scan_dimen(false, T);
        box_end(cur_box, src);
        return;
    }
    flush_buffer();
    std::string box_name;
    if (c == parbox_code) {
        // same arguments as minipage
        ignore_optarg();              // position
        ignore_optarg();              // height
        ignore_optarg();              // inner_pos
        scan_glue(it_glue, T, false); // width
    } else if (c == xbox_code)
        box_name = nT_arg_nopar();
    else if (c == marginpar_code) {
        ignore_optarg();
        box_name = the_names["marginpar"];
    } else if (scan_keyword("to") || scan_keyword("spread"))
        scan_dimen(false, T);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{";
        if (box_name.empty())
            the_log << "Constructing an anonymous box"
                    << "}\n";
        else
            the_log << "Constructing a box named " << box_name << "}\n";
    }
    scan_left_brace();
    push_level(bt_brace);
    TokenList L;
    if (c == xbox_code)
        L = toks_registers[everyxbox_code].val;
    else if (c == vbox_code)
        L = toks_registers[everyvbox_code].val;
    else if (c == vtop_code)
        L = toks_registers[everyvbox_code].val;
    else if (c == hbox_code)
        L = toks_registers[everyhbox_code].val;
    if (!L.empty()) {
        if (tracing_commands()) {
            String name = "<everyvbox> ";
            if (c == xbox_code)
                name = "<everyxbox> ";
            else if (c == hbox_code)
                name = "<everyhbox> ";
            Logger::finish_seq();
            the_log << "{" << name << L << "}\n";
        }
        back_input(L);
    }
    Xml *        cur_boxa = the_stack.push_hbox(box_name);
    SaveAuxBase *x        = new SaveAuxBoxend(*this, src, cur_boxa);
    push_save_stack(x);
    the_stack.set_arg_mode();
}

// Implements \show \showbox and \showthe
void Parser::M_xray(subtypes c) {
    switch (c) {
    case show_code:
        trace_buffer.clear();
        token_show(0, trace_buffer);
        return;
    case showbox_code: {
        auto k = scan_reg_num();
        log_and_tty << "Box " << k << ": ";
        show_box(box_table[k].val);
        return;
    }
    case show_xmlA_code:
        log_and_tty << "xmlA: ";
        show_box(the_xmlA);
        return;
    case show_xmlB_code:
        log_and_tty << "xmlB: ";
        show_box(the_xmlB);
        return;
    case register_show_code:
    case registerc_show_code: {
        Token tfe = cur_tok;
        if (c == registerc_show_code) csname_arg();
        get_token();
        Token T = cur_tok;
        if (cur_cmd_chr.is_undef_or_relax()) {
            parse_error(tfe, "Not a register value ", T, "", "bad reg");
            return;
        }
        back_input();
        TokenList L = E_the(the_code);
        log_and_tty << "\\show: " << T << " = " << L << "\n";
        return;
    }
    case showthe_code: {
        TokenList L = E_the(the_code);
        log_and_tty << "\\show: " << L << "\n";
        return;
    }
    case showgroups_code: dump_save_stack(); return;
    case showifs_code: conditions.dump(); return;
    case showlists_code: the_stack.dump(); return;
    case showtokens_code: {
        TokenList L = scan_general_text();
        log_and_tty << "\\show: " << L << "\n";
        return;
    }
    default: return;
    }
}

void Parser::show_box(Xml *X) {
    if (X != nullptr)
        log_and_tty << X << "\n";
    else
        log_and_tty << "empty.\n";
}

void Parser::E_useverb() {
    mac_buffer = "savedverb@" + group_to_string();
    Token t    = hash_table.locate(mac_buffer);
    back_input(t);
}

// Code of \long, \def, etc.  Handles \globaldefs
// This is called an assignment, so \afterassignment is completed here
void Parser::M_prefixed() {
    int flags = 0;
    while (cur_cmd_chr.cmd == prefix_cmd) {
        flags |= cur_cmd_chr.chr;
        remove_initial_space_relax();
    }
    bool b_global = false;
    if (flags >= 8) {
        flags -= 8;
        b_global = true;
    }
    auto     K = symcodes(user_cmd + flags);
    symcodes C = cur_cmd_chr.cmd;
    if (C <= max_non_prefixed_command) {
        if (b_global || (flags != 0)) prefix_error(b_global, K);
        if (cur_tok.is_valid()) back_input(); // case of prefix at end of list
        return;
    }
    if (C != def_cmd && (flags != 0)) prefix_error(b_global, K);
    // look at \globaldefs
    auto gd = eqtb_int_table[globaldefs_code].val;
    if (gd > 0) b_global = true;
    if (gd < 0) b_global = false;
    if (tracing_commands() && (b_global || (flags != 0))) {
        Logger::finish_seq();
        the_log << "{";
        trace_buffer.clear();
        trace_buffer.dump_prefix(true, b_global, K);
        the_log << trace_buffer << cur_tok << "}\n";
    }
    if (C == let_cmd)
        M_let(cur_cmd_chr.chr, b_global);
    else if (C == def_cmd)
        define_something(cur_cmd_chr.chr, b_global, K);
    else
        M_prefixed_aux(b_global);
    Token aat = get_after_ass_tok();
    if (!aat.is_null()) {
        back_input(aat);
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{after assignment: " << aat << "}\n";
        }
    }
}
