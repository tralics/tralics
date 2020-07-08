// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains stuff for latex3

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/util.h"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {
    const TokenList empty_list;
    //  TokenList double_at_list,double_at_no_list;
    Buffer      local_buffer, local_bufferB, local_bufferC;
    std::string tok_base, tok_sig; // result of split
    subtypes    how_to_generate;   // set by genenate_cond
                                   //  bool check_declaration = false; should set and use this, TODO
} // namespace

namespace l3_ns {
    auto conditional_aux(const std::string &p) -> subtypes;
} // namespace l3_ns

// -----------------------------------------------------
// misc functions

// Compares two strings; l3 functions below

void Parser::E_pdfstrcmp() {
    auto      s1 = sE_arg(), s2 = sE_arg();
    auto      cmp = s1.compare(s2);
    TokenList L   = token_ns::string_to_list(cmp == 0 ? "0" : (cmp < 0 ? "-1" : "1"), false);
    if (tracing_macros()) {
        Logger::finish_seq();
        spdlog::trace("{}{}=={}->{}", cur_tok, s1, s2, L);
    }
    back_input(L);
}

// Parses svn info. returns false in case of bad syntax
auto Buffer::svn_id(std::string &name, std::string &date, std::string &version) -> bool {
    ptrs.b  = 0;
    date    = "0000/00/00";
    version = "-1";
    name    = "";
    if (starts_with("Id: "))
        advance(4);
    else
        return false;
    ptrs.a = ptrs.b;
    while ((head() != 0) && head() != '.') advance();
    name = std::string(data() + ptrs.a, ptrs.b - ptrs.a);
    advance();
    while ((head() != 0) && head() != ' ') advance(); // ignore file name extension
    advance();
    ptrs.a = ptrs.b;
    if (head() == '-') return true;
    while ((head() != 0) && head() != ' ') advance();
    version = std::string(data() + ptrs.a, ptrs.b - ptrs.a);
    advance();
    ptrs.a = ptrs.b;
    if (size() < ptrs.b + 10) return true;
    if (at(ptrs.b + 4) == '-') at(ptrs.b + 4) = '/';
    if (at(ptrs.b + 7) == '-') at(ptrs.b + 7) = '/';
    if (at(ptrs.b + 10) == ' ')
        resize(ptrs.b + 10);
    else
        return true;
    date = data() + ptrs.a;
    return true;
}

// In latex3, a space is ignored, so locally redefine the catcode
void Parser::L3_getid() {
    get_token(); // should be a dollar sign, but hack!
    auto spcat                     = eqtb_int_table[uchar(' ')].val;
    eqtb_int_table[uchar(' ')].val = 10;
    TokenList l                    = read_until(cur_tok);
    TokenList info                 = read_arg();
    eqtb_int_table[uchar(' ')].val = spcat;
    read_toks_edef(l);
    Buffer &B = local_buffer;
    B.clear();
    B << l;
    std::string name, date, version;
    B.svn_id(name, date, version);
    new_macro(name, hash_table.ExplFileName_token);
    new_macro(date, hash_table.ExplFileDate_token);
    new_macro(version, hash_table.ExplFileVersion_token);
    new_macro(info, hash_table.ExplFileDescription_token);
    L3_logid();
}

// Prints current identification in the log file
void Parser::L3_logid() {
    Buffer &B = local_buffer;
    B.clear();
    B << get_mac_value(hash_table.ExplFileName_token);
    B += " ";
    B << get_mac_value(hash_table.ExplFileDate_token);
    B += " v";
    B << get_mac_value(hash_table.ExplFileVersion_token);
    B += " ";
    B << get_mac_value(hash_table.ExplFileDescription_token);
    the_log << "svn Id: " << B.convert_to_log_encoding() << "\n";
}

// Complicated conditionals have the form \ifxxx ... \fi\c_zero{trueC}{falseC}
// the body contains \prg_return_true and \prg_return_false
// these macros expands \fi, \or, \else until finding \c_zero, then
// execute trueC or falseC.  Defined as
//  \expandafter\use_i:nn\__int_to_roman:w in latex,

void Parser::E_prg_return(int c) {
    Token T = (c == 0 ? T_use1 : T_use2);
    scan_int(T); // let's hope this will not fail
    TokenList L1 = read_arg();
    TokenList L2 = read_arg();
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << "#1#2->#" << (c == 0 ? "1" : "2") << "\n";
        the_log << "#1<-" << L1 << "\n";
        the_log << "#2<-" << L2 << "\n";
    }
    one_of_two(L1, L2, c == 0);
}

// Generate #1#2#3 etc
// s is true when n is given by specification, false when by value
auto Parser::l3_parms_from_ac(long n, Token t, bool s) -> TokenList {
    if (n < 0) {
        err_buf = fmt::format("Negative number of arguments {} for ", n);
        err_buf << t;
        signal_error(err_tok, "bad args");
        n = 0;
    }
    if (n > 9) {
        err_buf = fmt::format("Too many arguments {} for {}", n, t);
        if (s) err_buf += " (wrong argument specification)";
        signal_error(err_tok, "bad args");
        n = 0;
    }
    TokenList L;
    for (int i = 0; i < n; i++) {
        L.push_back(make_char_token('#', 6));
        L.push_back(Token(other_t_offset, '1' + static_cast<uchar>(i)));
    }
    return L;
}

// reads a command name, say \foo:nn, stores it in token_to_split
// sets tok_base = foo, tok_sig=nn
// return true if bad
auto Parser::L3_split_next_name() -> bool {
    if (l3_get_name(err_tok)) return true;
    Buffer &B = local_buffer;
    B.clear();
    token_to_split = cur_tok;
    B += hash_table[cur_tok.hash_loc()];
    bool ok = B.split_at_colon(tok_base, tok_sig);
    if (!ok) {
        err_buf = fmt::format("Missing colon in macro name {} by {}", cur_tok, err_tok);
        signal_error(err_tok, "no colon in name");
        return true;
    }
    return false;
}

// user function, no error if missing colon
void Parser::L3_user_split_next_name(bool base) {
    Token T = cur_tok;
    if (l3_get_name(T)) return;
    Buffer &B = local_buffer;
    B         = hash_table[cur_tok.hash_loc()];
    B.split_at_colon(tok_base, tok_sig);
    std::string res = base ? tok_base : tok_sig;
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << cur_tok << "->" << res << "\n";
    }
    B += res;
    TokenList L = B.str_toks(nlt_cr); // is this needed?
    back_input(L);
}

// Ex \prg_new_conditional:Npnn \cs_if_eq:NN #1#2 { p , T , F , TF } {body}
//    \prg_new_conditional:Nnn \quark_if_nil:N { p, T , F , TF } {body}
// In the second case parms are generated from the signature

void Parser::L3_new_conditional_parm(subtypes s) {
    cmd_name = cur_tok;
    if (L3_split_next_name()) return; // should we reads the params ?
    TokenList parms = read_until(hash_table.OB_token);
    back_input(hash_table.OB_token);
    L3_new_conditional_aux(parms, s);
}

void Parser::L3_new_conditional(subtypes s) {
    cmd_name = cur_tok;
    if (L3_split_next_name()) return;
    TokenList parms = l3_parms_from_ac(to_signed(tok_sig.size()), token_to_split, true);
    L3_new_conditional_aux(parms, s);
}

// creates the name of the variant. foo:N this gives foo_p:N  or foo:NTF
void Buffer::l3_fabricate_cond(const std::string &base, const std::string &sig, subtypes w) {
    clear();
    append(base);
    if (w == l3_p_code)
        append("_p:");
    else
        push_back(':');
    append(sig);
    if (w == l3_T_code)
        push_back('T');
    else if (w == l3_F_code)
        push_back('F');
    else if (w == l3_TF_code)
        append("TF");
}

auto l3_ns::conditional_aux(const std::string &p) -> subtypes {
    if (p == "p") return l3_p_code;
    if (p == "TF") return l3_TF_code;
    if (p == "T") return l3_T_code;
    if (p == "F") return l3_F_code;
    if (p.empty()) return l3_bad_code;

    err_buf = fmt::format("Bad specification '{}' for {} by {}", p, token_to_split, cmd_name);
    the_parser.signal_error(the_parser.err_tok, "bad spec");
    return l3_bad_code;
}

// read variant and body, loops over variants
void Parser::L3_new_conditional_aux(TokenList &arg_spec, subtypes s) {
    std::string spec = sE_arg_nopar();
    TokenList   body = read_arg();
    for (const auto &p : split_commas(spec)) {
        subtypes c = l3_ns::conditional_aux(p);
        if (c == l3_bad_code) continue;
        L3_generate_form(c, arg_spec, body, s);
    }
}

// Constructs \foo_p:xx, \foo:xxT or \foo:xxF \foo:xxTF, completes body with
// either \c_zero \c_true_bool c_false_bool
// or \c_zero \use:n \use_none:n TA
// or \czero {} TA
// or \czero TA TB
// here TA and TB are the arguments of the new function.
// in the case \foo:xxT, after 0, there are 2 commands that read one argument,
// namely TA and either use or discard it. \foo:xxT is simpler

void Parser::L3_generate_form(subtypes c, TokenList parms, TokenList body, subtypes s) {
    body.push_back(Tc_zero);
    Buffer &B   = local_buffer;
    bool    pt  = (s == L3_setp_code || s == L3_newp_code);
    bool    set = (s == L3_set_code || s == L3_setp_code);
    switch (c) {
    case l3_p_code:
        body.push_back(Tc_true_bool);
        body.push_back(Tc_false_bool);
        if (pt) { // predicate, requires non-protected
            err_buf = fmt::format("{} for \\{}:{}: A predicate cannot be protected", cmd_name, tok_base, tok_sig);
            signal_error(err_tok, "bad protected");
        }
        break;
    case l3_T_code: // T
        body.push_back(T_usen);
        body.push_back(T_use_nonen);
        break;
    case l3_F_code: // F
        body.push_back(hash_table.OB_token);
        body.push_back(hash_table.CB_token);
        break;
        // case L3_TF_code: TF nothing to be done
    default:;
    }
    B.l3_fabricate_cond(tok_base, tok_sig, c);
    brace_me(body);
    back_input(body);
    back_input(parms);
    back_input(hash_table.locate(B));
    cur_tok = cmd_name;
    M_def(false, false, pt ? userp_cmd : user_cmd, set ? rd_always : rd_if_undef);
}

// Case of
// \prg_new_eq_conditional:NNn \tl_if_exist:N \cs_if_exist:N { TF , T , F , p }
// rename X-extended into Y-extended

void Parser::L3_eq_conditional(subtypes s) {
    cmd_name        = cur_tok;
    how_to_generate = s;
    if (L3_split_next_name()) {
        read_arg();
        read_arg();
        return;
    }
    std::string tok_base1 = tok_base;
    std::string tok_sig1  = tok_sig;
    bool        bad       = L3_split_next_name();
    std::string spec      = sE_arg_nopar();
    if (bad) return;
    Buffer &B = local_buffer;
    for (const auto &p : split_commas(spec)) {
        subtypes c = l3_ns::conditional_aux(p);
        if (c == l3_bad_code) continue;
        B.l3_fabricate_cond(tok_base, tok_sig, c);
        if (!hash_table.is_defined(B)) {
            parse_error(err_tok, "Undefined token", B);
            continue;
        }
        Token old = hash_table.last_tok;
        B.l3_fabricate_cond(tok_base1, tok_sig1, c);
        Token newt = hash_table.locate(B);
        cur_tok    = cmd_name;
        M_let(newt, old, false, how_to_generate == L3_new_code);
    }
}

// This implements the variants, assuming the truth value is in test
// \prg_new_conditional:Npnn fun args { p , T , F , TF }
// \if_some_test \prg_return_true: \else: \prg_return_false: \fi:

void Parser::l3_after_cond(Token T, bool test, subtypes c) {
    TokenList res;
    switch (c) {
    case l3_p_code:
        if (test)
            res.push_back(Tc_true_bool);
        else
            res.push_back(Tc_false_bool);
        break;
    case l3_TF_code: {
        TokenList A = read_arg();
        TokenList B = read_arg();
        if (test)
            res = A;
        else
            res = B;
        break;
    }
    case l3_T_code: {
        TokenList L = read_arg();
        if (test) res = L;
        break;
    }
    case l3_F_code: {
        TokenList L = read_arg();
        if (!test) res = L;
        break;
    }
    default: return; // should not happen
    }
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << "->" << res << "\n";
    }
    back_input(res);
}

// Implements variants of \cs_if_eq:NN
void Parser::E_l3_ifx(subtypes c) {
    Token caller = cur_tok;
    Token a1, a2;
    if (c >= 8) {
        c = subtypes(c - 8);
        csname_arg();
    }
    read_token_arg(caller);
    a1 = cur_tok;
    if (c >= 4) {
        c = subtypes(c - 4);
        csname_arg();
    }
    read_token_arg(caller);
    a2 = cur_tok;
    back_input(a1);
    back_input(a2);
    bool res = E_ifx();
    return l3_after_cond(caller, res, c);
}

// In latex3, \str_if_eq:nn uses \unexpanded; this produces a character string
// converted to a token list, then back to character string. Simplified here.
// There is a variant \str_if_eq_x:nn with with full expansion
// and two variants v and o (for each argument).

auto Parser::l3_to_string(subtypes c, TokenList &L) -> std::string {
    switch (c) {
    case l3expx_code: read_toks_edef(L); break;
    case l3expo_code: l3_reexpand_o(L); break;
    case l3expV_code: l3_expand_Vv(L, false); break;
    default:;
    }
    group_buffer.clear();
    group_buffer << L;
    return group_buffer;
}

void Parser::E_l3str_ifeq(subtypes c) {
    Token    caller = cur_tok;
    subtypes exp1   = l3expn_code;
    subtypes exp2   = l3expn_code;
    if (c >= 24) {
        exp1 = l3expV_code;
        exp2 = l3expV_code;
        c    = subtypes(c - 24);
    } else if (c >= 20) {
        exp2 = l3expV_code;
        c    = subtypes(c - 20);
    } else if (c >= 16) {
        exp2 = l3expo_code;
        c    = subtypes(c - 16);
    } else if (c >= 12) {
        exp1 = l3expo_code;
        c    = subtypes(c - 12);
    } else if (c >= 8) {
        exp1 = l3expV_code;
        c    = subtypes(c - 8);
    } else if (c >= 4) {
        exp1 = l3expx_code;
        exp2 = l3expx_code;
        c    = subtypes(c - 4);
    }

    TokenList   a1 = read_arg();
    TokenList   a2 = read_arg();
    std::string s1 = l3_to_string(exp1, a1);
    std::string s2 = l3_to_string(exp2, a2);
    l3_after_cond(caller, s1 == s2, c);
}

// Args: string to compare, case a-list, plus additional args
void Parser::E_l3str_case(subtypes c) {
    subtypes exp = l3expn_code;
    if (c >= 8) {
        exp = l3expo_code;
        c   = subtypes(c - 8);
    }
    if (c >= 4) {
        exp = l3expx_code;
        c   = subtypes(c - 4);
    }
    Token     caller = cur_tok;
    TokenList true_res, false_res, res;
    TokenList as1     = read_arg();
    TokenList clauses = read_arg();
    if (c == l3_TF_code || c == l3_T_code) true_res = read_arg();
    if (c == l3_TF_code || c == l3_F_code) false_res = read_arg();
    bool        match = false;
    std::string s1    = l3_to_string(exp, as1);
    if (exp == l3expo_code) exp = l3expn_code;
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << caller << " compares " << s1 << "\n";
    }
    for (;;) {
        token_ns::remove_initial_spaces(clauses);
        if (clauses.empty()) break;
        TokenList as2 = token_ns::get_a_param(clauses, false);
        // should we check for bad termination?
        TokenList   code = token_ns::get_a_param(clauses, false);
        std::string s2   = l3_to_string(exp, as2);
        if (s1 == s2) {
            match = true;
            res   = code;
            break;
        }
    }
    if (match)
        res.splice(res.end(), true_res);
    else
        res.splice(res.end(), false_res);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << caller << "->" << res << "\n";
    }
    back_input(res);
}

void Parser::L3_check_cmd(int c) {
    Token T = cur_tok;
    Token what;
    if (c == 1 || c == 3) {
        std::string name = sE_arg_nopar();
        what             = hash_table.locate(name);
    } else {
        if (l3_get_name(T)) return;
        what = cur_tok;
    }
    bool is_free = hash_table.eqtb[what.eqtb_loc()].is_undef_or_relax();
    if (c == 0 || c == 1) {
        if (!is_free) bad_redefinition(0, what);
    } else {
        if (is_free) bad_redefinition(1, what);
    }
}

// --------------------------------------------------
// Token Lists

// \tl_new:N and variants (14); half of them are \tl_new:c
void Parser::l3_new_token_list(int c) {
    bool c_name = false;
    if (c >= l3_tlx_new_code) {
        c -= l3_tlx_new_code;
        c_name = true;
    }
    Token name = fetch_csname(c_name);
    if (c == l3_tl_clear_code || c == l3_tl_gclear_code) {
        M_let(name, T_empty, c == l3_tl_gclear_code, false);
        return;
    }
    bool u = hash_table.eqtb[name.eqtb_loc()].is_undef_or_relax();
    if (c == l3_tl_gclearnew_code || c == l3_tl_clearnew_code) {
        // define globally, reset locally
        if (u)
            M_let(name, T_empty, true, false);
        else
            M_let(name, T_empty, c == l3_tl_gclearnew_code, false);
        return;
    }
    if (!u) bad_redefinition(0, name);
    if (c == l3_tl_new_code) {
        M_let(name, T_empty, true, false);
        return;
    }
    TokenList L;
    if (c == l3_tl_const_code) {
        L = read_arg();
    } else
        l3_expand_x(L);
    new_macro(L, name, true);
}

// TODO:  if(check_declaration) check both args of
// \tl_set_eq:NN are not undef_or_relax

// \tl_set:Nn and variants. Second argument is (by default) expanded once
// There are 24 variants; missing Npc
void Parser::l3_tl_set(int c) {
    bool gbl   = false;
    bool c_arg = false;
    if (c >= 18) {
        gbl = true;
        c -= 18;
    }
    if (c >= 9) {
        c_arg = true;
        c -= 9;
    }
    Token name = fetch_csname(c_arg);
    // TODO:  if(check_declaration) check name is not undef_or_relax
    TokenList res;
    switch (c) {
    case l3expn_code: res = scan_general_text(); break;
    case l3expo_code: l3_expand_o(res); break;
    case l3expx_code: l3_expand_x(res); break;
    case l3expv_code:
        csname_arg();
        l3_expand_Vv(res, true);
        break;
    case l3expV_code:
        res = read_arg();
        l3_expand_Vv(res, false);
        break;
    case l3expf_code: l3_expand_f(res); break;
    default: parse_error(err_tok, "internal error unimplemented", err_tok, "", "internal error"); return;
    }
    new_macro(res, name, gbl);
}

// \tl_concat:NNN \tl_concat:ccc and global version.
// Args 2 and 3  are expanded once
void Parser::l3_tl_concat(int c) {
    bool  c_flag = (c & 1) != 0;
    Token name   = fetch_csname(c_flag);
    // TODO:  if(check_declaration) check all 3 arg are not undef_or_relax
    TokenList A, B;
    if (c_flag) csname_arg();
    l3_expand_o(A);
    if (c_flag) csname_arg();
    l3_expand_o(B);
    A.splice(A.end(), B);
    new_macro(A, name, (c >= 2));
}

// \tl_put_left:Nn and variants. Second argument is (by default) expanded once
// There are 16 variants: nVox
void Parser::l3_tl_put_left(int c) {
    bool gbl      = false;
    bool c_arg    = false;
    bool put_left = true;
    if (c >= 36) {
        put_left = false;
        c -= 36;
    }
    if (c >= 18) {
        gbl = true;
        c -= 18;
    }
    if (c >= 9) {
        c_arg = true;
        c -= 9;
    }
    Token name = fetch_csname(c_arg);
    // TODO:  if(check_declaration) check name not undef_or_relax
    TokenList res;
    switch (c) {
    case l3expn_code: res = scan_general_text(); break;
    case l3expo_code: l3_expand_o(res); break;
    case l3expx_code: l3_expand_x(res); break;
    case l3expV_code:
        res = read_arg();
        l3_expand_Vv(res, false);
        break;
    default: parse_error(err_tok, "internal error unimplemented ", err_tok, "", "internal error"); return;
    }
    TokenList cur = get_mac_value(name);
    if (put_left)
        cur.splice(cur.begin(), res);
    else
        cur.splice(cur.end(), res);
    new_macro(cur, name, gbl);
}

// set rescan and variant 12 variants + 1
void Parser::tl_set_rescan(int c) {
    bool special = false;
    if (c == 12) {
        special = true;
        c       = 0;
    }
    bool gbl = false;
    if (c >= 6) {
        gbl = true;
        c -= 6;
    }
    bool cflag = false;
    if (c >= 3) {
        cflag = true;
        c -= 3;
    }
    Token name;
    if (!special) name = fetch_csname(cflag);
    TokenList side_effects = read_arg();
    TokenList arg;
    if (c == 0)
        arg = read_arg();
    else if (c == 1)
        l3_expand_o(arg);
    else
        l3_expand_x(arg);
    TokenList res;
    push_level(bt_brace);
    word_define(endlinechar_code, -1, false);
    word_define(newlinechar_code, -1, false);
    TokenList eof;
    token_list_define(everyeof_code, eof, false);
    flush_buffer();
    T_translate(side_effects);
    back_input(hash_table.CB_token);
    T_scantokens(arg);
    back_input(hash_table.OB_token);
    res = read_arg();
    get_token();
    pop_level(bt_brace);
    if (special)
        back_input(res);
    else
        new_macro(res, name, gbl);
}

// --------------------------------------------------
// Character codes

auto Parser::l3_read_int(Token T) -> long {
    TokenList L = read_arg();
    back_input(hash_table.relax_token);
    back_input(L);
    cur_tok = T;
    scan_expr(numexpr_code);
    return cur_val.get_int_val();
}

// \char_set_catcode_ignore:N T or \char_set_catcode_ignore:n `T
void Parser::L3_set_cat_code(int c) {
    bool alpha = false;
    if (c >= 16) {
        c -= 16;
        alpha = true;
    }
    size_t w = 0;
    if (alpha)
        w = scan_alpha();
    else {
        Token T  = cur_tok;
        auto  ww = l3_read_int(T);
        if (ww < 0 || ww > int(scan_char_num_max)) {
            signal_ovf(T, "Bad character code replaced by 0\n", ww, scan_char_num_max);
            w = 0;
        } else
            w = to_unsigned(ww);
    }
    word_define(w, c, false);
}

// \char_set_catcode:nn{110}{`A+1} and variants: \the or \showthe
void Parser::L3_set_num_code(int c) {
    size_t offset = 0;
    int    max    = 0;
    bool   show   = false;
    switch (c) {
    case setcat_code:
        offset = 0;
        max    = 15;
        break;
    case thecat_code: offset = 0; break;
    case showcat_code:
        offset = 0;
        show   = true;
        break;
    case setmath_code:
        offset = math_code_offset;
        max    = 0100000;
        break;
    case themath_code: offset = math_code_offset; break;
    case showmath_code:
        offset = math_code_offset;
        show   = true;
        break;
    case setlc_code:
        offset = lc_code_offset;
        max    = nb_characters - 1;
        break;
    case thelc_code: offset = lc_code_offset; break;
    case showlc_code:
        offset = lc_code_offset;
        show   = true;
        break;
    case setuc_code:
        offset = uc_code_offset;
        max    = nb_characters - 1;
        break;
    case theuc_code: offset = uc_code_offset; break;
    case showuc_code:
        offset = uc_code_offset;
        show   = true;
        break;
    case setsf_code:
        offset = sf_code_offset;
        max    = 077777;
        break;
    case thesf_code: offset = sf_code_offset; break;
    case showsf_code:
        offset = sf_code_offset;
        show   = true;
        break;
    }
    Token T = cur_tok;
    if (max != 0) {
        TokenList L1 = read_arg();
        auto      N  = l3_read_int(T);
        if (N < 0 || N > max) {
            signal_ovf(T, "Invalid code (out of bounds)\n", N, max);
            N = 0;
        }
        back_input(L1);
        cur_tok = T;
        auto m  = scan_char_num();
        word_define(m + offset, N, false); // \todo is offset always >=0?
        return;
    }
    auto m = l3_read_int(T);
    if (m < 0 || m > int(scan_char_num_max)) {
        signal_ovf(T, "Bad character code replaced by 0\n", m, scan_char_num_max);
        m = 0;
    }
    auto v = eqtb_int_table[to_unsigned(m) + offset].val;
    if (show)
        log_and_tty << T << "{" << m << "}=" << v << "\n";
    else {
        Buffer &B = local_buffer;
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << T << "->" << m << ".\n";
        }
        B.clear();
        B.append(std::to_string(v));
        TokenList res = B.str_toks(nlt_space);
        back_input(res);
    }
}

// returns true in case of problem; otherwise L3 equivalent of \if\ifcat
auto Parser::l3_get_cat(symcodes &a, subtypes &b, Token caller) -> bool {
    if (read_token_arg(caller)) return true;
    back_input(); // fake \noexpand
    if (!cur_tok.not_a_cmd()) back_input(hash_table.frozen_dont_expand);
    get_x_token_or_active_char(a, b);
    return false;
}

// Compares a character code
void Parser::E_cat_ifeq(subtypes c) {
    Token    caller = cur_tok;
    symcodes a{};
    subtypes b{};
    bool     test = false;
    if (!l3_get_cat(a, b, caller)) test = (a == (c / 4));
    l3_after_cond(caller, test, subtypes(c % 4));
}

// \token_if_eq_catcode and variants
void Parser::l3_token_check(subtypes c) {
    Token caller = cur_tok;
    bool  res    = false;
    switch (c / 4) {
    case tok_eq_meaning_code: res = E_ifx(); break;
    case tok_eq_cat_code:  // \token_if_eq_catcode:NN
    case tok_eq_char_code: // \token_if_eq_charcode:NN
    {
        symcodes a1{};
        subtypes b1{};
        symcodes a2{};
        subtypes b2{};
        if (!l3_get_cat(a1, b1, caller))
            if (!l3_get_cat(a2, b2, caller)) res = ((c / 4) == tok_eq_cat_code ? a1 == a2 : b1 == b2);
        break;
    }
    case tok_if_cs_code: {
        symcodes a1{};
        subtypes b1{};
        if (!l3_get_cat(a1, b1, caller)) res = a1 == relax_cmd;
        break;
    }
    case tok_if_macro_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.is_user());
        break;
    case tok_if_long_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.cmd == userl_cmd);
        break;
    case tok_if_prot_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.cmd == userp_cmd);
        break;
    case tok_if_longprot_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.cmd == userlp_cmd);
        break;
    case tok_if_expandable_code:
        if (read_token_arg(caller)) break;
        if (cur_cmd_chr.is_undef())
            res = false;
        else
            res = (cur_cmd_chr.is_expandable());
        break;
    case tok_if_chardef_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == char_given_cmd);
        break;
    case tok_if_mathchardef_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == math_given_cmd);
        break;
    case tok_if_dim_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == assign_dimen_cmd && cur_cmd_chr.chr < int(nb_registers));
        break;
    case tok_if_skip_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == assign_glue_cmd && cur_cmd_chr.chr < int(nb_registers));
        break;
    case tok_if_muskip_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == assign_mu_glue_cmd && cur_cmd_chr.chr >= muskip_reg_offset);
        break;
    case tok_if_toks_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == assign_toks_cmd && int(cur_cmd_chr.chr) < int(nb_registers));
        break;
    case tok_if_int_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.cmd == assign_int_cmd && cur_cmd_chr.chr >= count_reg_offset &&
                   cur_cmd_chr.chr < int(count_reg_offset + nb_registers));
        break;
    case tok_if_primitive_code:
        if (read_token_arg(caller)) break;
        auto K = cur_cmd_chr.cmd;
        res    = !((K == undef_cmd) || (K >= user_cmd) || (K < 16));
    }

    l3_after_cond(caller, res, subtypes(c % 4));
}

// catcode active= " $& ^_ ~
// catcode special SP "#$%&\^_{}~

// --------------------------------------------------

// \__cs_generate_from_signature:NNn \def \foo:xy{body}
// \def\foo:xy#1#2{body}
void Parser::generate_from_sig() {
    Token definer = get_r_token(true);
    if (L3_split_next_name()) {
        ignore_arg();
        return;
    }
    TokenList parms = l3_parms_from_ac(to_signed(tok_sig.size()), token_to_split, true);
    back_input(parms);
    back_input(token_to_split);
    back_input(definer);
}

void Parser::Tl3_gen_from_ac(int c) {
    Token     caller  = cur_tok;
    Token     fun     = fetch_csname(c == 1);
    Token     creator = fetch_csname(c == 2);
    TokenList L       = read_arg();
    back_input(hash_table.relax_token);
    back_input(L);
    auto n = scan_int(caller);
    read_until(hash_table.relax_token); // read all unused tokens
    TokenList parms = l3_parms_from_ac(n, fun, false);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{" << caller << "-> " << creator << fun << parms << "}\n";
    }
    back_input(parms);
    back_input(fun);
    back_input(creator);
}

// if args are set, Nn, Npn, this is
//  \def\cs_set:Nn{\__cs_generate_from_signature:NNn \cs_set:Npn}
void Parser::define_definer(String base, String nsig, String osig) {
    Token     nt = hash_table.locate(fmt::format("cs_{}:{}", base, nsig));
    Token     ot = hash_table.locate(fmt::format("cs_{}:{}", base, osig));
    TokenList L;
    L.push_back(gen_from_sig_tok);
    L.push_back(ot);
    auto *X = new Macro(L);
    mac_define(nt, X, true, rd_always, userp_cmd);
}

// Example: if base is "set", this is
//  \protected\def\cs_set:Nn{\exp_args:Nc \cs_set:Nn}
//  \protected\def\cs_set:Nx{\exp_args:Nc \cs_set:Nx}
void Parser::define_definer(String base) {
    Token     nt = hash_table.locate(fmt::format("cs_{}:cn", base));
    Token     ot = hash_table.locate(fmt::format("cs_{}:Nn", base));
    TokenList L;
    L.push_back(expargsnc_tok);
    L.push_back(ot);
    auto *X = new Macro(L);
    mac_define(nt, X, true, rd_always, userp_cmd);
    // gain
    nt = hash_table.locate(fmt::format("cs_{}:cx", base));
    ot = hash_table.locate(fmt::format("cs_{}:Nx", base));
    TokenList L2;
    L2.push_back(expargsnc_tok);
    L2.push_back(ot);
    auto *X2 = new Macro(L2);
    mac_define(nt, X2, true, rd_always, userp_cmd);
}

// \cs_generate_variant:Nn \foo:nnn {Vn, xV}
// produces \def\foo:Vnn{\exp_args:NV \foo:nnn}
// and \protected\def\foo:xVn{\exp_args:NV \foo:xVn}

void Parser::l3_generate_variant() {
    bool  nok  = L3_split_next_name();
    Token orig = cur_tok;
    bool  prot = false; // do we need to protect
    if (!cur_cmd_chr.is_expandable()) prot = true;
    if (cur_cmd_chr.cmd >= userp_cmd) prot = true;
    if (cur_cmd_chr.is_undef()) {
        undefined_mac();
        nok = true;
    }
    std::string spec = sE_arg_nopar();
    if (nok) return; // an error has already been generated
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{Generating variants for " << orig << " with " << spec << "}\n";
    }
    for (const auto &p : split_commas(spec)) l3_generate_variant(p, prot, orig);
}

// This one is used by the bootstrap code.
// The original macro need not be defined yet.
void Parser::l3_generate_variant(String orig, String var) {
    Token   T = hash_table.locate(orig);
    Buffer &B = local_buffer;
    B         = orig;
    B.split_at_colon(tok_base, tok_sig);
    l3_generate_variant(var, false, T);
}

// This produces a single variant;
void Parser::l3_generate_variant(const std::string &var, bool prot, Token orig) {
    auto n = var.size();
    if (n == 0) return; // ignore empty sepc
    if (n > tok_sig.size()) {
        err_buf = fmt::format("New spec size '{}' too big for {}", var, orig);
        signal_error(err_tok, "spec too big");
        return;
    }
    Buffer &osig    = local_buffer;
    Buffer &nsig    = local_bufferB;
    Buffer &changes = local_bufferC;
    osig            = tok_sig;
    nsig            = var;
    changes.clear();
    size_t last_ok = 0;
    for (size_t i = 0; i < n; i++) {
        char oc = osig[i], nc = nsig[i];
        if (oc == nc) {                              // no conversion needed
            if (!(nc == 'N' || nc == 'p')) nc = 'n'; // cannot always use n
        } else if (oc == 'N' || oc == 'n') {
            osig[i] = nc;
            last_ok = i + 1;
        } else {
            err_buf.clear();
            err_buf.format("Old spec at position {} should be n or N for ", i);
            err_buf << orig;
            signal_error(err_tok, "variant, bad orig");
            return;
        }
        changes << nc;
    }
    changes.resize(last_ok);
    bool need_prot = false; // Protect result and aux function
    for (size_t i = 0; i < last_ok; i++)
        if (changes[i] == 'x') need_prot = true;
    if (need_prot) prot = true;
    nsig         = tok_base + ':' + osig;
    Token newfun = hash_table.locate(nsig);
    if (!hash_table.eqtb[newfun.eqtb_loc()].is_undef()) {
        the_log << "Variant " << newfun << " already defined, not changing it.\n";
        return;
    }
    nsig                = "exp_args:N" + changes;
    Token     converter = hash_table.locate(nsig);
    TokenList body;
    body.push_back(converter);
    body.push_back(orig);
    auto *X = new Macro(body);
    mac_define(newfun, X, true, rd_always, prot ? userp_cmd : user_cmd);
    if (!hash_table.eqtb[converter.eqtb_loc()].is_undef()) return;
    TokenList cb;
    for (size_t i = 0;; i++) {
        char c = changes[i];
        if (c == 0) c = ':';
        nsig = "::";
        nsig.push_back(c);
        cb.push_back(hash_table.locate(nsig));
        if (c == ':') break; //
    }
    X = new Macro(cb);
    mac_define(converter, X, true, rd_always, need_prot ? userp_cmd : user_cmd);
}

// Implements: \def\::n #1\::: #2#3{#1\:::{#2{#3}}}
// #2 is function to call, #3 arg to handle, #1 more things to do
// NOTE: Case of last_unbraced variant. here #1 should be empty
// Expansion is #2{#3} in case #1 is empty.
void Parser::E_l3expand_aux(subtypes c) {
    Token     T = cur_tok;
    TokenList L1, L2, L3, res;
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << " is expanded\n";
    }
    L1 = read_until(T3col_tok);
    L2 = read_arg();
    switch (c) { // read and start handling the argument
    case l3expc_code:
    case l3expv_code:
    case l3expvu_code: csname_arg(); break;
    case l3expo_code:
    case l3expou_code: l3_expand_o(L3); break;
    case l3expf_code:
    case l3expfu_code: l3_expand_f(L3); break;
    case l3expx_code:
    case l3expxu_code: l3_expand_x(L3); break;
    case l3expp_code: // this is special
        L3 = read_until(hash_table.OB_token);
        back_input(hash_table.OB_token);
        break;
    default: L3 = read_arg();
    }
    // Interprets third argment
    switch (c) {
    case l3expn_code: // \::n  normal case
    case l3expo_code: // \::o  argument is processed
    case l3expf_code: // \::f ditto
    case l3expx_code: // \::x ditto
        brace_me(L3);
        break;
    case l3expc_code: // \::c is \csname#3\csname
        get_token();
        L3.push_back(cur_tok);
        break;
    case l3expV_code: // \::V case of a list
    case l3expv_code: // \::v case of a csname unread
        l3_expand_Vv(L3, c == l3expv_code);
        brace_me(L3);
        break;
    case l3expVu_code: // \::v_unbraced idem, unbraced
    case l3expvu_code: l3_expand_Vv(L3, c == l3expvu_code); break;
    default: // \::N \::p there is nothing to do; idem for other unbraced variants
        break;
    }
    // returns  #1 \::: {#2#3} with #3 interpreted
    // if #1 is empty, this simplifies to #2#3
    L2.splice(L2.end(), L3);
    if (!L1.empty()) {
        res.splice(res.end(), L1);
        res.push_back(T3col_tok);
        brace_me(L2);
    }
    res.splice(res.end(), L2);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << "->" << res << "\n";
    }
    back_input(res);
}

// Case of \exp_not:c and variant. Expands according to the variant,
// and prevents further expansion (via \unexpanded in general).

void Parser::E_l3noexpand(subtypes c) {
    Token     T = cur_tok;
    bool      b = tracing_macros();
    TokenList L3, res;
    if (b) {
        Logger::finish_seq();
        the_log << T << " is expanded\n";
    }
    switch (c) {
    case l3expc_code:
        csname_arg(); // optimise, there is a single token
        back_input(T_exp_notN);
        if (b) {
            Logger::finish_seq();
            the_log << T << "->\\noexpand " << cur_tok << "\n";
        }
        return;
    case l3expv_code:
        csname_arg();
        l3_expand_Vv(L3, true);
        break;
    case l3expV_code:
        L3 = read_arg();
        l3_expand_Vv(L3, false);
        break;
    case l3expo_code: l3_expand_o(L3); break;
    case l3expf_code:
        l3_expand_f(L3);
        break;
        break;
        //  case l3expN_code: this is \noexpand
        //  case l3expp_code: no need to noexpand something  like #1#2
        //  case l3expn_code: this is \unexpanded
        //  case l3expx_code: not needed, everything is expanded
    default: return; // should not happen
    }
    brace_me(L3);
    res.push_back(T_exp_notn);
    res.splice(res.end(), L3);
    if (b) {
        Logger::finish_seq();
        the_log << T << "->" << res << "\n";
    }
    back_input(res);
}

// This is "o" conversion of latex3. Corresponds to \expandafter{#1}
// Instead of reading the argument, we read an open brace
// (if there is none, we read a token T, and insert T close-braceà)

void Parser::l3_expand_o(TokenList &L) {
    if (before_mac_arg()) return; // case of error
    expand_when_ok(false);
    back_input(hash_table.OB_token);
    L = read_arg();
}

// assumes the list already read
void Parser::l3_reexpand_o(TokenList &L) {
    back_input_braced(L);
    L.clear();
    l3_expand_o(L);
}

// \::f is  \expandafter{\romannumeral-`0#1}
// This reads a number and an optional space.
// Expansion is empty as the number is negative.
// so this is: read_one_space(). Hack: the next token is expanded

void Parser::l3_expand_f(TokenList &L) {
    if (before_mac_arg()) return; // case of error
    read_one_space();
    back_input(hash_table.OB_token);
    L = read_arg();
}

// \::x is full expand
void Parser::l3_expand_x(TokenList &L) {
    if (before_mac_arg()) return; // case of error
    scan_group3(L, get_cur_line(), true, 0);
}

// case of \::v and \::V; in the first case, the boolean is true, we have a name
// to read, otherwise something in L. If a macro, we expand; otherwise call \the
// the result is in L.

void Parser::l3_expand_Vv(TokenList &L, bool spec) {
    if (spec) get_token();
    back_input(hash_table.CB_token);
    if (spec)
        back_input(cur_tok);
    else
        back_input(L);
    get_token(); // look at what follows
    if (cur_cmd_chr.is_expandable())
        expand();
    else {
        back_input();
        E_the_traced(err_tok, the_code);
    }
    back_input(hash_table.OB_token);
    L = read_arg();
}

// used in the next function
#define INSERT res.splice(res.end(), L)
#define INSERT_B                                                                                                                           \
    brace_me(L);                                                                                                                           \
    INSERT
#define EXPAND_o                                                                                                                           \
    l3_expand_o(L);                                                                                                                        \
    INSERT_B
#define EXPAND_ou                                                                                                                          \
    l3_expand_o(L);                                                                                                                        \
    INSERT
#define EXPAND_c                                                                                                                           \
    csname_arg();                                                                                                                          \
    get_token();                                                                                                                           \
    res.push_back(cur_tok)
#define EXPAND_f                                                                                                                           \
    l3_expand_f(L);                                                                                                                        \
    INSERT_B
#define EXPAND_fu                                                                                                                          \
    l3_expand_f(L);                                                                                                                        \
    INSERT
#define EXPAND_x                                                                                                                           \
    l3_expand_x(L);                                                                                                                        \
    INSERT_B
#define EXPAND_xu                                                                                                                          \
    l3_expand_x(L);                                                                                                                        \
    INSERT
#define EXPAND_V                                                                                                                           \
    L = read_arg();                                                                                                                        \
    l3_expand_Vv(L, false);                                                                                                                \
    INSERT_B
#define EXPAND_Vu                                                                                                                          \
    L = read_arg();                                                                                                                        \
    l3_expand_Vv(L, false);                                                                                                                \
    INSERT
#define EXPAND_v                                                                                                                           \
    csname_arg();                                                                                                                          \
    l3_expand_Vv(L, true);                                                                                                                 \
    INSERT_B
#define EXPAND_vu                                                                                                                          \
    csname_arg();                                                                                                                          \
    l3_expand_Vv(L, true);                                                                                                                 \
    INSERT
#define EXPAND_n                                                                                                                           \
    L = read_arg();                                                                                                                        \
    INSERT_B
#define EXPAND_N                                                                                                                           \
    L = read_arg();                                                                                                                        \
    INSERT

void Parser::E_l3expand_base(subtypes c) {
    TokenList L, res;
    Token     T = cur_tok;
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << " is expanded\n";
    }
    EXPAND_N; // macro to call
    switch (c) {
    case l3exp_No_code: EXPAND_o; break;
    case l3exp_NNo_code:
        EXPAND_N;
        EXPAND_o;
        break;
    case l3exp_NNNo_code:
        EXPAND_N;
        EXPAND_N;
        EXPAND_o;
        break;
    case l3exp_NNc_code:
        EXPAND_N;
        EXPAND_c;
        break;
    case l3exp_Ncc_code:
        EXPAND_c;
        EXPAND_c;
        break;
    case l3exp_Nccc_code:
        EXPAND_c;
        EXPAND_c;
        EXPAND_c;
        break;
    case l3exp_Nf_code: EXPAND_f; break;
    case l3exp_NNf_code:
        EXPAND_N;
        EXPAND_f;
        break;
    case l3exp_Nv_code: EXPAND_v; break;
    case l3exp_NNv_code:
        EXPAND_N;
        EXPAND_v;
        break;
    case l3exp_NV_code: EXPAND_V; break;
    case l3exp_NNV_code:
        EXPAND_N;
        EXPAND_V;
        break;
    case l3exp_NNNV_code:
        EXPAND_N;
        EXPAND_N;
        EXPAND_V;
        break;
    case l3exp_Nco_code:
        EXPAND_c;
        EXPAND_o;
        break;
    case l3exp_Ncf_code:
        EXPAND_c;
        EXPAND_f;
        break;
    case l3exp_NVV_code:
        EXPAND_V;
        EXPAND_V;
        break;
    case l3exp_NcNc_code:
        EXPAND_c;
        EXPAND_N;
        EXPAND_c;
        break;
    case l3exp_Ncco_code:
        EXPAND_c;
        EXPAND_c;
        EXPAND_o;
        break;
    case l3exp_Nx_code: EXPAND_x; break;
    case l3exp_Nnc_code:
        EXPAND_n;
        EXPAND_c;
        break;
    case l3exp_Nfo_code:
        EXPAND_f;
        EXPAND_o;
        break;
    case l3exp_Nff_code:
        EXPAND_f;
        EXPAND_f;
        break;
    case l3exp_Nnf_code:
        EXPAND_n;
        EXPAND_f;
        break;
    case l3exp_Nno_code:
        EXPAND_n;
        EXPAND_o;
        break;
    case l3exp_NnV_code:
        EXPAND_n;
        EXPAND_V;
        break;
    case l3exp_Noo_code:
        EXPAND_o;
        EXPAND_o;
        break;
    case l3exp_Nof_code:
        EXPAND_o;
        EXPAND_f;
        break;
    case l3exp_Noc_code:
        EXPAND_o;
        EXPAND_c;
        break;
    case l3exp_NNx_code:
        EXPAND_N;
        EXPAND_x;
        break;
    case l3exp_Ncx_code:
        EXPAND_c;
        EXPAND_x;
        break;
    case l3exp_Nnx_code:
        EXPAND_n;
        EXPAND_x;
        break;
    case l3exp_Nox_code:
        EXPAND_o;
        EXPAND_x;
        break;
    case l3exp_Nxo_code:
        EXPAND_x;
        EXPAND_o;
        break;
    case l3exp_Nxx_code:
        EXPAND_x;
        EXPAND_x;
        break;
    case l3exp_NNno_code:
        EXPAND_N;
        EXPAND_n;
        EXPAND_o;
        break;
    case l3exp_NNoo_code:
        EXPAND_N;
        EXPAND_o;
        EXPAND_o;
        break;
    case l3exp_Nnnc_code:
        EXPAND_n;
        EXPAND_n;
        EXPAND_c;
        break;
    case l3exp_Nnno_code:
        EXPAND_n;
        EXPAND_n;
        EXPAND_o;
        break;
    case l3exp_Nooo_code:
        EXPAND_o;
        EXPAND_o;
        EXPAND_o;
        break;

    case l3exp_NNnx_code:
        EXPAND_N;
        EXPAND_n;
        EXPAND_x;
        break;
    case l3exp_NNox_code:
        EXPAND_N;
        EXPAND_o;
        EXPAND_x;
        break;
    case l3exp_Nnnx_code:
        EXPAND_n;
        EXPAND_n;
        EXPAND_x;
        break;
    case l3exp_Nnox_code:
        EXPAND_n;
        EXPAND_o;
        EXPAND_x;
        break;
    case l3exp_Nccx_code:
        EXPAND_c;
        EXPAND_c;
        EXPAND_x;
        break;
    case l3exp_Ncnx_code:
        EXPAND_c;
        EXPAND_n;
        EXPAND_x;
        break;
    case l3exp_Noox_code:
        EXPAND_o;
        EXPAND_o;
        EXPAND_x;
        break;
        // These are of the form last_unbraced
    case l3exp_Nvu_code: EXPAND_vu; break;
    case l3exp_NVu_code: EXPAND_Vu; break;
    case l3exp_Nou_code: EXPAND_ou; break;
    case l3exp_Nfu_code: EXPAND_fu; break;
    case l3exp_Ncou_code:
        EXPAND_c;
        EXPAND_ou;
        break;
    case l3exp_NcVu_code:
        EXPAND_c;
        EXPAND_Vu;
        break;
    case l3exp_NNVu_code:
        EXPAND_N;
        EXPAND_Vu;
        break;
    case l3exp_NNou_code:
        EXPAND_N;
        EXPAND_ou;
        break;
    case l3exp_NNNVu_code:
        EXPAND_N;
        EXPAND_N;
        EXPAND_Vu;
        break;
    case l3exp_NNNou_code:
        EXPAND_N;
        EXPAND_N;
        EXPAND_ou;
        break;
    case l3exp_Nnou_code:
        EXPAND_n;
        EXPAND_ou;
        break;
    case l3exp_Noou_code:
        EXPAND_o;
        EXPAND_ou;
        break;
    case l3exp_Nfou_code:
        EXPAND_f;
        EXPAND_ou;
        break;
    case l3exp_NnNou_code:
        EXPAND_n;
        EXPAND_N;
        EXPAND_ou;
        break;
    case l3exp_Nxu_code: EXPAND_xu; break;
    case l3exp_Nouou_code:
        EXPAND_ou;
        EXPAND_ou;
        break;
    default:;
    }
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << "->" << res << "\n";
    }
    back_input(res);
}

//
void Parser::L3_load(bool preload) {
    static bool loaded = false;
    if (loaded) return;
    loaded = true;
    the_log << "Loading latex3...$Date: 2015/11/25 07:38:28 $\n";
    {
        TokenList L;
        new_prim("c_empty_tl", L);
    }
    {
        TokenList L;
        L.push_back(Token(space_token_val));
        new_prim("c_space_tl", L);
    }
    //  double_at_list.push_back(Token(other_t_offset,'@'));
    //  double_at_list.push_back(Token(letter_t_offset,'@'));
    //  double_at_no_list = double_at_list;
    //  double_at_no_list.push_back(hash_table.locate("noexpand"));
    hash_table.boot_latex3();
    shorthand_gdefine(char_def_code, "c_sixteen", 16);
    Tc_zero = shorthand_gdefine(char_def_code, "c_zero", 0);
    shorthand_gdefine(char_def_code, "c_six", 6);
    shorthand_gdefine(char_def_code, "c_seven", 7);
    shorthand_gdefine(char_def_code, "c_twelve", 12);
    shorthand_gdefine(char_def_code, "c_max_register_int", 1024);
    Tc_true_bool  = shorthand_gdefine(char_def_code, "c_true_bool", 1);
    Tc_false_bool = shorthand_gdefine(char_def_code, "c_false_bool", 0);
    define_definer("set", "Nn", "Npn");
    define_definer("set", "Nx", "Npx");
    define_definer("set_nopar", "Nn", "Npn");
    define_definer("set_nopar", "Nx", "Npx");
    define_definer("set_protected", "Nn", "Npn");
    define_definer("set_protected", "Nx", "Npx");
    define_definer("set_protected_nopar", "Nn", "Npn");
    define_definer("set_protected_nopar", "Nx", "Npx");
    define_definer("gset", "Nn", "Npn");
    define_definer("gset", "Nx", "Npx");
    define_definer("gset_nopar", "Nn", "Npn");
    define_definer("gset_nopar", "Nx", "Npx");
    define_definer("gset_protected", "Nn", "Npn");
    define_definer("gset_protected", "Nx", "Npx");
    define_definer("gset_protected_nopar", "Nn", "Npn");
    define_definer("gset_protected_nopar", "Nx", "Npx");
    define_definer("new", "Nn", "Npn");
    define_definer("new", "Nx", "Npx");
    define_definer("new_nopar", "Nn", "Npn");
    define_definer("new_nopar", "Nx", "Npx");
    define_definer("new_protected", "Nn", "Npn");
    define_definer("new_protected", "Nx", "Npx");
    define_definer("new_protected_nopar", "Nn", "Npn");
    define_definer("new_protected_nopar", "Nx", "Npx");
    define_definer("set");
    define_definer("set_nopar");
    define_definer("set_protected");
    define_definer("set_protected_nopar");
    define_definer("gset");
    define_definer("gset_nopar");
    define_definer("gset_protected");
    define_definer("gset_protected_nopar");
    define_definer("new");
    define_definer("new_nopar");
    define_definer("new_protected");
    define_definer("new_protected_nopar");

    LineList L;
    L.insert("%% Begin bootstrap commands for latex3");
    L.insert("\\message{Bootstrapping latex3}");
    // Change catcode code for underscore and colon
    L.insert(R"(\edef\tmp{\catcode58=\the\catcode58 )");
    L.insert(R"(  \catcode95=\the\catcode95 \let\noexpand\tmp\relax})");
    L.insert("\\catcode58=11 \\catcode95=11 ");
    L.insert(R"(\def\token_to_str:c{\exp_args:Nc\token_to_str:N})");
    L.insert(R"(\def\token_new:Nn#1#2{\cs_new_eq:NN #1#2})");
    L.insert(R"(\def\c_catcode_active_tl{\noexpand~})");
    L.insert("\\tmp\\wlog{Loading latex3 (done)}");
    if (!preload) {
        flush_buffer();
        leave_h_mode(); // bootstrap should not produce XML
    }
    push_input_stack("latex3 code", false, true);
    init(L);
    if (preload) translate0();
}
