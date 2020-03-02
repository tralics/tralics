// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains stuff for latex3

#include "txinline.h"
#include "txparser.h"
#include <fmt/format.h>

namespace {
    const TokenList empty_list;
    //  TokenList double_at_list,double_at_no_list;
    Buffer      local_buffer, local_bufferB, local_bufferC;
    Token       q_nil, q_stop, q_recursion_stop;
    Token       T_roman, T_use1, T_use2, T_usen, T_use_nonen;
    Token       T_exp_notN, T_exp_notn, T_empty;
    Token       gen_from_sig_tok, expargsnc_tok, T3col_tok;
    Token       Tc_zero, Tc_true_bool, Tc_false_bool, cmd_name;
    Token       token_to_split;
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
    Token       T  = cur_tok;
    std::string s1 = sE_arg();
    std::string s2 = sE_arg();
    int         cp = strcmp(s1.c_str(), s2.c_str());
    TokenList   L  = token_ns::string_to_list(cp == 0 ? "0" : (cp < 0 ? "-1" : "1"), false);
    if (tracing_macros()) the_log << lg_start << T << s1 << "==" << s2 << lg_arrow << L << lg_end;
    back_input(L);
}

// Parses svn info. returns false in case of bad syntax
auto Buffer::svn_id(std::string &name, std::string &date, std::string &version) -> bool {
    reset_ptr();
    date    = "0000/00/00";
    version = "-1";
    name    = "";
    if (head() == 'I')
        advance();
    else
        return false;
    if (head() == 'd')
        advance();
    else
        return false;
    if (head() == ':')
        advance();
    else
        return false;
    if (head() == ' ')
        advance();
    else
        return false;
    set_ptr1_to_ptr();
    while ((head() != 0) && head() != '.') advance();
    at(ptr) = 0;
    name    = data() + ptr1;
    advance();
    while ((head() != 0) && head() != ' ') advance(); // ignore file name extension
    advance();
    set_ptr1_to_ptr();
    if (head() == '-') return true;
    while ((head() != 0) && head() != ' ') advance();
    at(ptr) = 0;
    version = data() + ptr1;
    advance();
    set_ptr1_to_ptr();
    if (wptr < ptr + 10) return true;
    if (at(ptr + 4) == '-') at(ptr + 4) = '/';
    if (at(ptr + 7) == '-') at(ptr + 7) = '/';
    if (at(ptr + 7) == '-') at(ptr + 7) = '/';
    if (at(ptr + 10) == ' ')
        at(ptr + 10) = 0;
    else
        return true;
    date = data() + ptr1;
    return true;
}

// In latex3, a space is ignored, so locally redefine the catcode
void Parser::L3_getid() {
    get_token(); // should be a dollar sign, but hack!
    auto spcat = eqtb_int_table[uchar(' ')].val;
    eqtb_int_table[uchar(' ')].set_val(10);
    TokenList l    = read_until(cur_tok);
    TokenList info = read_arg();
    eqtb_int_table[uchar(' ')].set_val(spcat);
    read_toks_edef(l);
    Buffer &B = local_buffer;
    B.reset();
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
    B.reset();
    B << get_mac_value(hash_table.ExplFileName_token) << " ";
    B << get_mac_value(hash_table.ExplFileDate_token) << " v";
    B << get_mac_value(hash_table.ExplFileVersion_token) << " ";
    B << get_mac_value(hash_table.ExplFileDescription_token);
    the_log << "svn Id: " << B.convert_to_log_encoding() << lg_end;
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
        the_log << lg_start << T << "#1#2->#" << (c == 0 ? "1" : "2") << lg_end;
        the_log << "#1<-" << L1 << lg_end;
        the_log << "#2<-" << L2 << lg_end;
    }
    one_of_two(L1, L2, c == 0);
}

// Generate #1#2#3 etc
// s is true when n is given by specification, false when by value
auto Parser::l3_parms_from_ac(long n, Token t, bool s) -> TokenList {
    if (n < 0) {
        err_buf << bf_reset << "Negative number of arguments " << n << " for " << t;
        signal_error(err_tok, "bad args");
        n = 0;
    }
    if (n > 9) {
        err_buf << bf_reset << "Too many arguments " << n << " for " << t;
        if (s) err_buf << " (wrong argument specification)";
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
    B.reset();
    token_to_split = cur_tok;
    B << hash_table[cur_tok.hash_loc()];
    bool ok = B.split_at_colon(tok_base, tok_sig);
    if (!ok) {
        err_buf << bf_reset << "Missing colon in macro name " << cur_tok << " by " << err_tok;
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
    B.reset();
    B << hash_table[cur_tok.hash_loc()];
    B.split_at_colon(tok_base, tok_sig);
    std::string res = base ? tok_base : tok_sig;
    if (tracing_macros()) the_log << lg_start << T << cur_tok << lg_arrow << res << lg_end;
    B << res;
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
    reset();
    push_back(base);
    if (w == l3_p_code)
        push_back("_p:");
    else
        push_back(':');
    push_back(sig);
    if (w == l3_T_code)
        push_back('T');
    else if (w == l3_F_code)
        push_back('F');
    else if (w == l3_TF_code)
        push_back("TF");
}

auto l3_ns::conditional_aux(const std::string &p) -> subtypes {
    if (p == "p") return l3_p_code;
    if (p == "TF") return l3_TF_code;
    if (p == "T") return l3_T_code;
    if (p == "F") return l3_F_code;
    if (p.empty()) return l3_bad_code;

    err_buf.reset();
    err_buf << "Bad specification '" << p << "' for " << token_to_split << " by " << cmd_name;
    the_parser.signal_error(the_parser.err_tok, "bad spec");
    return l3_bad_code;
}

// read variant and body, loops over variants
void Parser::L3_new_conditional_aux(TokenList &arg_spec, subtypes s) {
    std::string spec = sE_arg_nopar();
    TokenList   body = read_arg();
    Splitter    S(spec);
    for (;;) {
        if (S.at_end()) return;
        std::string p = S.get_next();
        subtypes    c = l3_ns::conditional_aux(p);
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
            err_buf.reset();
            err_buf << cmd_name << " for \\" << tok_base << ":" << tok_sig << ": A predicate cannot be protected";
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
    Buffer & B = local_buffer;
    Splitter S(spec);
    for (;;) {
        if (S.at_end()) return;
        std::string p = S.get_next();
        subtypes    c = l3_ns::conditional_aux(p);
        if (c == l3_bad_code) continue;
        B.l3_fabricate_cond(tok_base, tok_sig, c);
        if (!hash_table.is_defined(B)) {
            parse_error(err_tok, "Undefined token", B.c_str());
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
    if (tracing_macros()) the_log << lg_start << T << lg_arrow << res << lg_end;
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
    group_buffer.reset();
    group_buffer << L;
    return group_buffer.to_string();
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
    if (tracing_macros()) the_log << lg_start << caller << " compares " << s1 << lg_end;
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
    if (tracing_macros()) the_log << lg_start << caller << lg_arrow << res << lg_end;
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

auto Parser::l3_read_int(Token T) -> int {
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
    size_t w;
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
    int  offset = 0;
    int  max    = 0;
    bool show   = false;
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
        int       N  = l3_read_int(T);
        if (N < 0 || N > max) {
            signal_ovf(T, "Invalid code (out of bounds)\n", N, max);
            N = 0;
        }
        back_input(L1);
        cur_tok = T;
        int m   = scan_char_num();
        word_define(to_unsigned(m + offset), N, false);
        return;
    }
    int m = l3_read_int(T);
    if (m < 0 || m > int(scan_char_num_max)) {
        signal_ovf(T, "Bad character code replaced by 0\n", m, scan_char_num_max);
        m = 0;
    }
    auto v = eqtb_int_table[to_unsigned(m + offset)].val;
    if (show)
        log_and_tty << T << "{" << m << "}=" << v << "\n";
    else {
        Buffer &B = local_buffer;
        if (tracing_commands()) the_log << lg_start << T << "->" << m << "." << lg_end;
        B.reset();
        B.push_back_int(v);
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
    symcodes a;
    subtypes b;
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
        symcodes a1;
        subtypes b1;
        symcodes a2;
        subtypes b2;
        if (!l3_get_cat(a1, b1, caller))
            if (!l3_get_cat(a2, b2, caller)) res = ((c / 4) == tok_eq_cat_code ? a1 == a2 : b1 == b2);
        break;
    }
    case tok_if_cs_code: {
        symcodes a1;
        subtypes b1;
        if (!l3_get_cat(a1, b1, caller)) res = a1 == relax_cmd;
        break;
    }
    case tok_if_macro_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.is_user());
        break;
    case tok_if_long_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.get_cmd() == userl_cmd);
        break;
    case tok_if_prot_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.get_cmd() == userp_cmd);
        break;
    case tok_if_longprot_code:
        if (read_token_arg(caller)) break;
        res = (cur_cmd_chr.get_cmd() == userlp_cmd);
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
            res = (cur_cmd_chr.get_cmd() == char_given_cmd);
        break;
    case tok_if_mathchardef_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == math_given_cmd);
        break;
    case tok_if_dim_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == assign_dimen_cmd && cur_cmd_chr.get_chr() < int(nb_registers));
        break;
    case tok_if_skip_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == assign_glue_cmd && cur_cmd_chr.get_chr() < int(nb_registers));
        break;
    case tok_if_muskip_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == assign_mu_glue_cmd && cur_cmd_chr.get_chr() >= muskip_reg_offset);
        break;
    case tok_if_toks_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == assign_toks_cmd && int(cur_cmd_chr.get_chr()) < int(nb_registers));
        break;
    case tok_if_int_code:
        if (read_token_arg(caller))
            break;
        else
            res = (cur_cmd_chr.get_cmd() == assign_int_cmd && cur_cmd_chr.get_chr() >= count_reg_offset &&
                   cur_cmd_chr.get_chr() < int(count_reg_offset + nb_registers));
        break;
    case tok_if_primitive_code:
        if (read_token_arg(caller)) break;
        {
            symcodes K = cur_cmd_chr.get_cmd();
            if (K == undef_cmd)
                res = false; // undefined
            else if (K >= user_cmd)
                res = false; // macro
            else if (K < 16)
                res = false; // character
            else
                res = true;
        }
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
    int n = scan_int(caller);
    read_until(hash_table.relax_token); // read all unused tokens
    TokenList parms = l3_parms_from_ac(n, fun, false);
    if (tracing_commands()) the_log << lg_startbrace << caller << "-> " << creator << fun << parms << lg_endbrace;
    back_input(parms);
    back_input(fun);
    back_input(creator);
}

// if args are set, Nn, Npn, this is
//  \def\cs_set:Nn{\__cs_generate_from_signature:NNn \cs_set:Npn}
void Parser::define_definer(String base, String nsig, String osig) {
    Buffer &B = local_buffer;
    B << bf_reset << "cs_" << base << ":" << nsig;
    Token nt = hash_table.locate(B);
    B << bf_reset << "cs_" << base << ":" << osig;
    Token     ot = hash_table.locate(B);
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
    Buffer &B = local_buffer;
    B << bf_reset << "cs_" << base << ":cn";
    Token nt = hash_table.locate(B);
    B << bf_reset << "cs_" << base << ":Nn";
    Token     ot = hash_table.locate(B);
    TokenList L;
    L.push_back(expargsnc_tok);
    L.push_back(ot);
    auto *X = new Macro(L);
    mac_define(nt, X, true, rd_always, userp_cmd);
    // gain
    B << bf_reset << "cs_" << base << ":cx";
    nt = hash_table.locate(B);
    B << bf_reset << "cs_" << base << ":Nx";
    ot = hash_table.locate(B);
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
    if (cur_cmd_chr.get_cmd() >= userp_cmd) prot = true;
    if (cur_cmd_chr.is_undef()) {
        undefined_mac();
        nok = true;
    }
    std::string spec = sE_arg_nopar();
    if (nok) return; // an error has already been generated
    if (tracing_commands()) the_log << lg_startbrace << "Generating variants for " << orig << " with " << spec << lg_endbrace;
    Splitter S(spec);
    for (;;) {
        if (S.at_end()) return;
        std::string p = S.get_next();
        l3_generate_variant(p, prot, orig);
    }
}

// This one is used by the bootstrap code.
// The original macro need not be defined yet.
void Parser::l3_generate_variant(String orig, String var) {
    Token   T = hash_table.locate(orig);
    Buffer &B = local_buffer;
    B.reset();
    B << orig;
    B.split_at_colon(tok_base, tok_sig);
    l3_generate_variant(var, false, T);
}

// This produces a single variant;
void Parser::l3_generate_variant(const std::string &var, bool prot, Token orig) {
    auto n = var.size();
    if (n == 0) return; // ignore empty sepc
    if (n > tok_sig.size()) {
        err_buf.reset();
        err_buf << "New spec size '" << var << "' too big for " << orig;
        signal_error(err_tok, "spec too big");
        return;
    }
    Buffer &osig    = local_buffer;
    Buffer &nsig    = local_bufferB;
    Buffer &changes = local_bufferC;
    osig << bf_reset << tok_sig;
    nsig << bf_reset << var;
    changes.reset();
    size_t last_ok = 0;
    for (size_t i = 0; i < n; i++) {
        char oc = osig[i], nc = nsig[i];
        if (oc == nc) {                              // no conversion needed
            if (!(nc == 'N' || nc == 'p')) nc = 'n'; // cannot always use n
        } else if (oc == 'N' || oc == 'n') {
            osig.modify(i, nc);
            last_ok = i + 1;
        } else {
            err_buf.reset();
            err_buf << fmt::format("Old spec at position {} should be n or N for ", i) << orig;
            signal_error(err_tok, "variant, bad orig");
            return;
        }
        changes << nc;
    }
    changes.kill_at(last_ok);
    bool need_prot = false; // Protect result and aux function
    for (size_t i = 0; i < last_ok; i++)
        if (changes[i] == 'x') need_prot = true;
    if (need_prot) prot = true;
    nsig << bf_reset << tok_base << ':' << osig;
    Token newfun = hash_table.locate(nsig);
    if (!hash_table.eqtb[newfun.eqtb_loc()].is_undefined()) {
        the_log << "Variant " << newfun << " already defined, not changing it.\n";
        return;
    }
    nsig << bf_reset << "exp_args:N" << changes;
    Token     converter = hash_table.locate(nsig);
    TokenList body;
    body.push_back(converter);
    body.push_back(orig);
    auto *X = new Macro(body);
    mac_define(newfun, X, true, rd_always, prot ? userp_cmd : user_cmd);
    if (!hash_table.eqtb[converter.eqtb_loc()].is_undefined()) return;
    TokenList cb;
    for (size_t i = 0;; i++) {
        char c = changes[i];
        if (c == 0) c = ':';
        nsig << bf_reset << "::" << c;
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
    if (tracing_macros()) the_log << lg_start << T << " is expanded " << lg_end;
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
    if (tracing_macros()) the_log << lg_start << T << lg_arrow << res << lg_end;
    back_input(res);
}

// Case of \exp_not:c and variant. Expands according to the variant,
// and prevents further expansion (via \unexpanded in general).

void Parser::E_l3noexpand(subtypes c) {
    Token     T = cur_tok;
    bool      b = tracing_macros();
    TokenList L3, res;
    if (b) the_log << lg_start << T << " is expanded " << lg_end;
    switch (c) {
    case l3expc_code:
        csname_arg(); // optimise, there is a single token
        back_input(T_exp_notN);
        if (b) the_log << lg_start << T << "->\\noexpand " << cur_tok << lg_end;
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
    if (b) the_log << lg_start << T << lg_arrow << res << lg_end;
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
    if (tracing_macros()) the_log << lg_start << T << " is expanded " << lg_end;
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
    if (tracing_macros()) the_log << lg_start << T << lg_arrow << res << lg_end;
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
    hash_table.load_latex3();
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

    LinePtr L;
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

void Hashtab::load_latex3() {
    // renaming everything
    eval_let("tex_space:D", " ");
    eval_let("tex_italiccorrection:D", "/");
    eval_let("tex_hyphen:D", "-");
    eval_let("tex_let:D", "let");
    eval_let("tex_def:D", "def");
    eval_let("tex_edef:D", "edef");
    eval_let("tex_gdef:D", "gdef");
    eval_let("tex_xdef:D", "xdef");
    eval_let("tex_chardef:D", "chardef");
    eval_let("tex_countdef:D", "countdef");
    eval_let("tex_dimendef:D", "dimendef");
    eval_let("tex_skipdef:D", "skipdef");
    eval_let("tex_muskipdef:D", "muskipdef");
    eval_let("tex_mathchardef:D", "mathchardef");
    eval_let("tex_toksdef:D", "toksdef");
    eval_let("tex_futurelet:D", "futurelet");
    eval_let("tex_advance:D", "advance");
    eval_let("tex_divide:D", "divide");
    eval_let("tex_multiply:D", "multiply");
    eval_let("tex_font:D", "font");
    eval_let("tex_fam:D", "fam");
    eval_let("tex_global:D", "global");
    eval_let("tex_long:D", "long");
    eval_let("tex_outer:D", "outer");
    eval_let("tex_setlanguage:D", "setlanguage");
    eval_let("tex_setlanguage:D", "setlanguage");
    eval_let("tex_globaldefs:D", "globaldefs");
    eval_let("tex_afterassignment:D", "afterassignment");
    eval_let("tex_aftergroup:D", "aftergroup");
    eval_let("tex_expandafter:D", "expandafter");
    eval_let("tex_noexpand:D", "noexpand");
    eval_let("tex_begingroup:D", "begingroup");
    eval_let("tex_endgroup:D", "endgroup");
    eval_let("tex_halign:D", "halign");
    eval_let("tex_valign:D", "valign");
    eval_let("tex_cr:D", "cr");
    eval_let("tex_crcr:D", "crcr");
    eval_let("tex_noalign:D", "noalign");
    eval_let("tex_omit:D", "omit");
    eval_let("tex_span:D", "span");
    eval_let("tex_tabskip:D", "tabskip");
    eval_let("tex_everycr:D", "everycr");
    eval_let("tex_if:D", "if");
    eval_let("tex_ifcase:D", "ifcase");
    eval_let("tex_ifcat:D", "ifcat");
    eval_let("tex_ifnum:D", "ifnum");
    eval_let("tex_ifodd:D", "ifodd");
    eval_let("tex_ifdim:D", "ifdim");
    eval_let("tex_ifeof:D", "ifeof");
    eval_let("tex_ifhbox:D", "ifhbox");
    eval_let("tex_ifvbox:D", "ifvbox");
    eval_let("tex_ifvoid:D", "ifvoid");
    eval_let("tex_ifx:D", "ifx");
    eval_let("tex_iffalse:D", "iffalse");
    eval_let("tex_iftrue:D", "iftrue");
    eval_let("tex_ifhmode:D", "ifhmode");
    eval_let("tex_ifmmode:D", "ifmmode");
    eval_let("tex_ifvmode:D", "ifvmode");
    eval_let("tex_ifinner:D", "ifinner");
    eval_let("tex_fi:D", "fi");
    eval_let("tex_else:D", "else");
    eval_let("tex_or:D", "or");
    eval_let("tex_immediate:D", "immediate");
    eval_let("tex_closeout:D", "closeout");
    eval_let("tex_openin:D", "openin");
    eval_let("tex_openout:D", "openout");
    eval_let("tex_read:D", "read");
    eval_let("tex_write:D", "write");
    eval_let("tex_closein:D", "closein");
    eval_let("tex_newlinechar:D", "newlinechar");
    eval_let("tex_input:D", "input");
    eval_let("tex_endinput:D", "endinput");
    eval_let("tex_inputlineno:D", "inputlineno");
    eval_let("tex_errmessage:D", "errmessage");
    eval_let("tex_message:D", "message");
    eval_let("tex_show:D", "show");
    eval_let("tex_showthe:D", "showthe");
    eval_let("tex_showbox:D", "showbox");
    eval_let("tex_showlists:D", "showlists");
    eval_let("tex_errhelp:D", "errhelp");
    eval_let("tex_errorcontextlines:D", "errorcontextlines");
    eval_let("tex_tracingcommands:D", "tracingcommands");
    eval_let("tex_tracinglostchars:D", "tracinglostchars");
    eval_let("tex_tracingmacros:D", "tracingmacros");
    eval_let("tex_tracingonline:D", "tracingonline");
    eval_let("tex_tracingoutput:D", "tracingoutput");
    eval_let("tex_tracingpages:D", "tracingpages");
    eval_let("tex_tracingparagraphs:D", "tracingparagraphs");
    eval_let("tex_tracingrestores:D", "tracingrestores");
    eval_let("tex_tracingstats:D", "tracingstats");
    eval_let("tex_pausing:D", "pausing");
    eval_let("tex_showboxbreadth:D", "showboxbreadth");
    eval_let("tex_showboxdepth:D", "showboxdepth");
    eval_let("tex_batchmode:D", "batchmode");
    eval_let("tex_errorstopmode:D", "errorstopmode");
    eval_let("tex_nonstopmode:D", "nonstopmode");
    eval_let("tex_scrollmode:D", "scrollmode");
    eval_let("tex_end:D", "end");
    eval_let("tex_csname:D", "csname");
    eval_let("tex_endcsname:D", "endcsname");
    eval_let("tex_ignorespaces:D", "ignorespaces");
    eval_let("tex_relax:D", "relax");
    eval_let("tex_the:D", "the");
    eval_let("tex_mag:D", "mag");
    eval_let("tex_language:D", "language");
    eval_let("tex_mark:D", "mark");
    eval_let("tex_topmark:D", "topmark");
    eval_let("tex_firstmark:D", "firstmark");
    eval_let("tex_botmark:D", "botmark");
    eval_let("tex_splitfirstmark:D", "splitfirstmark");
    eval_let("tex_splitbotmark:D", "splitbotmark");
    eval_let("tex_fontname:D", "fontname");
    eval_let("tex_escapechar:D", "escapechar");
    eval_let("tex_endlinechar:D", "endlinechar");
    eval_let("tex_mathchoice:D", "mathchoice");
    eval_let("tex_delimiter:D", "delimiter");
    eval_let("tex_mathaccent:D", "mathaccent");
    eval_let("tex_mathchar:D", "mathchar");
    eval_let("tex_mskip:D", "mskip");
    eval_let("tex_radical:D", "radical");
    eval_let("tex_vcenter:D", "vcenter");
    eval_let("tex_mkern:D", "mkern");
    eval_let("tex_above:D", "above");
    eval_let("tex_abovewithdelims:D", "abovewithdelims");
    eval_let("tex_atop:D", "atop");
    eval_let("tex_atopwithdelims:D", "atopwithdelims");
    eval_let("tex_over:D", "over");
    eval_let("tex_overwithdelims:D", "overwithdelims");
    eval_let("tex_displaystyle:D", "displaystyle");
    eval_let("tex_textstyle:D", "textstyle");
    eval_let("tex_scriptscriptstyle:D", "scriptscriptstyle");
    eval_let("tex_scriptstyle:D", "scriptstyle");
    eval_let("tex_nonscript:D", "nonscript");
    eval_let("tex_eqno:D", "eqno");
    eval_let("tex_abovedisplayshortskip:D", "abovedisplayshortskip");
    eval_let("tex_abovedisplayskip:D", "abovedisplayskip");
    eval_let("tex_belowdisplayshortskip:D", "belowdisplayshortskip");
    eval_let("tex_belowdisplayskip:D", "belowdisplayskip");
    eval_let("tex_displaywidowpenalty:D", "displaywidowpenalty");
    eval_let("tex_displayindent:D", "displayindent");
    eval_let("tex_displaywidth:D", "displaywidth");
    eval_let("tex_everydisplay:D", "everydisplay");
    eval_let("tex_predisplaysize:D", "predisplaysize");
    eval_let("tex_predisplaypenalty:D", "predisplaypenalty");
    eval_let("tex_postdisplaypenalty:D", "postdisplaypenalty");
    eval_let("tex_mathbin:D", "mathbin");
    eval_let("tex_mathclose:D", "mathclose");
    eval_let("tex_mathinner:D", "mathinner");
    eval_let("tex_mathop:D", "mathop");
    eval_let("tex_displaylimits:D", "displaylimits");
    eval_let("tex_limits:D", "limits");
    eval_let("tex_nolimits:D", "nolimits");
    eval_let("tex_mathopen:D", "mathopen");
    eval_let("tex_mathord:D", "mathord");
    eval_let("tex_mathpunct:D", "mathpunct");
    eval_let("tex_mathpunct:D", "mathpunct");
    eval_let("tex_mathrel:D", "mathrel");
    eval_let("tex_overline:D", "overline");
    eval_let("tex_underline:D", "underline");
    eval_let("tex_left:D", "left");
    eval_let("tex_right:D", "right");
    eval_let("tex_binoppenalty:D", "binoppenalty");
    eval_let("tex_relpenalty:D", "relpenalty");
    eval_let("tex_delimitershortfall:D", "delimitershortfall");
    eval_let("tex_delimiterfactor:D", "delimiterfactor");
    eval_let("tex_nulldelimiterspace:D", "nulldelimiterspace");
    eval_let("tex_everymath:D", "everymath");
    eval_let("tex_mathsurround:D", "mathsurround");
    eval_let("tex_medmuskip:D", "medmuskip");
    eval_let("tex_thinmuskip:D", "thinmuskip");
    eval_let("tex_thickmuskip:D", "thickmuskip");
    eval_let("tex_scriptspace:D", "scriptspace");
    eval_let("tex_noboundary:D", "noboundary");
    eval_let("tex_char:D", "char");
    eval_let("tex_discretionary:D", "discretionary");
    eval_let("tex_hfil:D", "hfil");
    eval_let("tex_hfilneg:D", "hfilneg");
    eval_let("tex_hfill:D", "hfill");
    eval_let("tex_hskip:D", "hskip");
    eval_let("tex_hss:D", "hss");
    eval_let("tex_vfil:D", "vfil");
    eval_let("tex_vfilneg:D", "vfilneg");
    eval_let("tex_vfill:D", "vfill");
    eval_let("tex_vskip:D", "vskip");
    eval_let("tex_vss:D", "vss");
    eval_let("tex_unskip:D", "unskip");
    eval_let("tex_kern:D", "kern");
    eval_let("tex_unkern:D", "unkern");
    eval_let("tex_hrule:D", "hrule");
    eval_let("tex_vrule:D", "vrule");
    eval_let("tex_leaders:D", "leaders");
    eval_let("tex_cleaders:D", "cleaders");
    eval_let("tex_xleaders:D", "xleaders");
    eval_let("tex_lastkern:D", "lastkern");
    eval_let("tex_lastskip:D", "lastskip");
    eval_let("tex_indent:D", "indent");
    eval_let("tex_par:D", "par");
    eval_let("tex_noindent:D", "noindent");
    eval_let("tex_vadjust:D", "vadjust");
    eval_let("tex_baselineskip:D", "baselineskip");
    eval_let("tex_lineskip:D", "lineskip");
    eval_let("tex_lineskiplimit:D", "lineskiplimit");
    eval_let("tex_clubpenalty:D", "clubpenalty");
    eval_let("tex_widowpenalty:D", "widowpenalty");
    eval_let("tex_exhyphenpenalty:D", "exhyphenpenalty");
    eval_let("tex_hyphenpenalty:D", "hyphenpenalty");
    eval_let("tex_linepenalty:D", "linepenalty");
    eval_let("tex_doublehyphendemerits:D", "doublehyphendemerits");
    eval_let("tex_finalhyphendemerits:D", "finalhyphendemerits");
    eval_let("tex_adjdemerits:D", "adjdemerits");
    eval_let("tex_hangafter:D", "hangafter");
    eval_let("tex_hangindent:D", "hangindent");
    eval_let("tex_parshape:D", "parshape");
    eval_let("tex_hsize:D", "hsize");
    eval_let("tex_lefthyphenmin:D", "lefthyphenmin");
    eval_let("tex_righthyphenmin:D", "righthyphenmin");
    eval_let("tex_leftskip:D", "leftskip");
    eval_let("tex_rightskip:D", "rightskip");
    eval_let("tex_looseness:D", "looseness");
    eval_let("tex_parskip:D", "parskip");
    eval_let("tex_parindent:D", "parindent");
    eval_let("tex_uchyph:D", "uchyph");
    eval_let("tex_emergencystretch:D", "emergencystretch");
    eval_let("tex_pretolerance:D", "pretolerance");
    eval_let("tex_tolerance:D", "tolerance");
    eval_let("tex_spaceskip:D", "spaceskip");
    eval_let("tex_xspaceskip:D", "xspaceskip");
    eval_let("tex_parfillskip:D", "parfillskip");
    eval_let("tex_everypar:D", "everypar");
    eval_let("tex_prevgraf:D", "prevgraf");
    eval_let("tex_spacefactor:D", "spacefactor");
    eval_let("tex_shipout:D", "shipout");
    eval_let("tex_vsize:D", "vsize");
    eval_let("tex_interlinepenalty:D", "interlinepenalty");
    eval_let("tex_brokenpenalty:D", "brokenpenalty");
    eval_let("tex_topskip:D", "topskip");
    eval_let("tex_maxdeadcycles:D", "maxdeadcycles");
    eval_let("tex_maxdepth:D", "maxdepth");
    eval_let("tex_output:D", "output");
    eval_let("tex_deadcycles:D", "deadcycles");
    eval_let("tex_pagedepth:D", "pagedepth");
    eval_let("tex_pagestretch:D", "pagestretch");
    eval_let("tex_pagefilstretch:D", "pagefilstretch");
    eval_let("tex_pagefillstretch:D", "pagefillstretch");
    eval_let("tex_pagefilllstretch:D", "pagefilllstretch");
    eval_let("tex_pageshrink:D", "pageshrink");
    eval_let("tex_pagegoal:D", "pagegoal");
    eval_let("tex_pagetotal:D", "pagetotal");
    eval_let("tex_outputpenalty:D", "outputpenalty");
    eval_let("tex_hoffset:D", "hoffset");
    eval_let("tex_voffset:D", "voffset");
    eval_let("tex_insert:D", "insert");
    eval_let("tex_holdinginserts:D", "holdinginserts");
    eval_let("tex_floatingpenalty:D", "floatingpenalty");
    eval_let("tex_insertpenalties:D", "insertpenalties");
    eval_let("tex_lower:D", "lower");
    eval_let("tex_moveleft:D", "moveleft");
    eval_let("tex_moveright:D", "moveright");
    eval_let("tex_raise:D", "raise");
    eval_let("tex_copy:D", "copy");
    eval_let("tex_lastbox:D", "lastbox");
    eval_let("tex_vsplit:D", "vsplit");
    eval_let("tex_unhbox:D", "unhbox");
    eval_let("tex_unhcopy:D", "unhcopy");
    eval_let("tex_unvbox:D", "unvbox");
    eval_let("tex_unvcopy:D", "unvcopy");
    eval_let("tex_setbox:D", "setbox");
    eval_let("tex_hbox:D", "hbox");
    eval_let("tex_vbox:D", "vbox");
    eval_let("tex_vtop:D", "vtop");
    eval_let("tex_prevdepth:D", "prevdepth");
    eval_let("tex_badness:D", "badness");
    eval_let("tex_hbadness:D", "hbadness");
    eval_let("tex_vbadness:D", "vbadness");
    eval_let("tex_hfuzz:D", "hfuzz");
    eval_let("tex_vfuzz:D", "vfuzz");
    eval_let("tex_overfullrule:D", "overfullrule");
    eval_let("tex_boxmaxdepth:D", "boxmaxdepth");
    eval_let("tex_splitmaxdepth:D", "splitmaxdepth");
    eval_let("tex_splittopskip:D", "splittopskip");
    eval_let("tex_everyhbox:D", "everyhbox");
    eval_let("tex_everyvbox:D", "everyvbox");
    eval_let("tex_nullfont:D", "nullfont");
    eval_let("tex_textfont:D", "textfont");
    eval_let("tex_scriptfont:D", "scriptfont");
    eval_let("tex_scriptscriptfont:D", "scriptscriptfont");
    eval_let("tex_fontdimen:D", "fontdimen");
    eval_let("tex_hyphenchar:D", "hyphenchar");
    eval_let("tex_skewchar:D", "skewchar");
    eval_let("tex_defaulthyphenchar:D", "defaulthyphenchar");
    eval_let("tex_defaultskewchar:D", "defaultskewchar");
    eval_let("tex_number:D", "number");
    eval_let("tex_romannumeral:D", "romannumeral");
    eval_let("tex_string:D", "string");
    eval_let("tex_lowercase:D", "lowercase");
    eval_let("tex_uppercase:D", "uppercase");
    eval_let("tex_meaning:D", "meaning");
    eval_let("tex_penalty:D", "penalty");
    eval_let("tex_unpenalty:D", "unpenalty");
    eval_let("tex_lastpenalty:D", "lastpenalty");
    eval_let("tex_special:D", "special");
    eval_let("tex_dump:D", "dump");
    eval_let("tex_patterns:D", "patterns");
    eval_let("tex_hyphenation:D", "hyphenation");
    eval_let("tex_time:D", "time");
    eval_let("tex_day:D", "day");
    eval_let("tex_month:D", "month");
    eval_let("tex_year:D", "year");
    eval_let("tex_jobname:D", "jobname");
    eval_let("tex_everyjob:D", "everyjob");
    eval_let("tex_count:D", "count");
    eval_let("tex_dimen:D", "dimen");
    eval_let("tex_skip:D", "skip");
    eval_let("tex_toks:D", "toks");
    eval_let("tex_muskip:D", "muskip");
    eval_let("tex_box:D", "box");
    eval_let("tex_wd:D", "wd");
    eval_let("tex_ht:D", "ht");
    eval_let("tex_dp:D", "dp");
    eval_let("tex_catcode:D", "catcode");
    eval_let("tex_delcode:D", "delcode");
    eval_let("tex_sfcode:D", "sfcode");
    eval_let("tex_lccode:D", "lccode");
    eval_let("tex_uccode:D", "uccode");
    eval_let("tex_mathcode:D", "mathcode");
    eval_let("etex_ifdefined:D", "ifdefined");
    eval_let("etex_ifcsname:D", "ifcsname");
    eval_let("etex_unless:D", "unless");
    eval_let("etex_eTeXversion:D", "eTeXversion");
    eval_let("etex_eTeXrevision:D", "eTeXrevision");
    eval_let("etex_marks:D", "marks");
    eval_let("etex_topmarks:D", "topmarks");
    eval_let("etex_firstmarks:D", "firstmarks");
    eval_let("etex_botmarks:D", "botmarks");
    eval_let("etex_splitfirstmarks:D", "splitfirstmarks");
    eval_let("etex_splitbotmarks:D", "splitbotmarks");
    eval_let("etex_unexpanded:D", "unexpanded");
    eval_let("etex_detokenize:D", "detokenize");
    eval_let("etex_scantokens:D", "scantokens");
    eval_let("etex_showtokens:D", "showtokens");
    eval_let("etex_readline:D", "readline");
    eval_let("etex_tracingassigns:D", "tracingassigns");
    eval_let("etex_tracingscantokens:D", "tracingscantokens");
    eval_let("etex_tracingnesting:D", "tracingnesting");
    eval_let("etex_tracingifs:D", "tracingifs");
    eval_let("etex_currentiflevel:D", "currentiflevel");
    eval_let("etex_currentifbranch:D", "currentifbranch");
    eval_let("etex_currentiftype:D", "currentiftype");
    eval_let("etex_tracinggroups:D", "tracinggroups");
    eval_let("etex_currentgrouplevel:D", "currentgrouplevel");
    eval_let("etex_currentgrouptype:D", "currentgrouptype");
    eval_let("etex_showgroups:D", "showgroups");
    eval_let("etex_showifs:D", "showifs");
    eval_let("etex_interactionmode:D", "interactionmode");
    eval_let("etex_lastnodetype:D", "lastnodetype");
    eval_let("etex_iffontchar:D", "iffontchar");
    eval_let("etex_fontcharht:D", "fontcharht");
    eval_let("etex_fontchardp:D", "fontchardp");
    eval_let("etex_fontcharwd:D", "fontcharwd");
    eval_let("etex_fontcharic:D", "fontcharic");
    eval_let("etex_parshapeindent:D", "parshapeindent");
    eval_let("etex_parshapelength:D", "parshapelength");
    eval_let("etex_parshapedimen:D", "parshapedimen");
    eval_let("etex_numexpr:D", "numexpr");
    eval_let("etex_dimexpr:D", "dimexpr");
    eval_let("etex_glueexpr:D", "glueexpr");
    eval_let("etex_muexpr:D", "muexpr");
    eval_let("etex_gluestretch:D", "gluestretch");
    eval_let("etex_glueshrink:D", "glueshrink");
    eval_let("etexgluestretchorder_:D", "gluestretchorder");
    eval_let("etex_glueshrinkorder:D", "glueshrinkorder");
    eval_let("etex_gluetomu:D", "gluetomu");
    eval_let("etex_mutoglue:D", "mutoglue");
    eval_let("etex_lastlinefit:D", "lastlinefit");
    eval_let("etex_interlinepenalties:D", "interlinepenalties");
    eval_let("etex_clubpenalties:D", "clubpenalties");
    eval_let("etex_widowpenalties:D", "widowpenalties");
    eval_let("etex_displaywidowpenalties:D", "displaywidowpenalties");
    eval_let("etex_middle:D", "middle");
    eval_let("etex_savinghyphcodes:D", "savinghyphcodes");
    eval_let("etex_savingvdiscards:D", "savingvdiscards");
    eval_let("etex_pagediscards:D", "pagediscards");
    eval_let("etex_splitdiscards:D", "splitdiscards");
    eval_let("etex_TeXXeTstate:D", "TeXXeTstate");
    eval_let("etex_beginL:D", "beginL");
    eval_let("etex_endL:D", "endL");
    eval_let("etex_beginR:D", "beginR");
    eval_let("etex_endR:D", "endR");
    eval_let("etex_predisplaydirection:D", "predisplaydirection");
    eval_let("etex_everyeof:D", "everyeof");
    eval_let("etex_protected:D", "protected");
    eval_let("pdftex_strcmp:D", "pdfstrcmp");

    // Definition of frozen@ ... without the unimp code
    // \let\frozen@everymath\everymath \let\frozen@everydisplay\everydisplay
    // \newtoks\everymath \newtoks\everydisplay
    // \frozen@everymath = {\the\everymath}
    // \frozen@everydisplay = {\the\everydisplay}
    // possible redef
    eval_let("tex_end:D", "@@end");
    eval_let("tex_input:D", "@@input");
    // eval_let("tex_underline:D", "@@underline"); // math only \underline
    // eval_let("tex_hyphen:D", "@@hyp");  is \-
    // eval_let("\tex_italiccorrection:D", "@@italiccorr");  is \/
    // eval_let("tex_everydisplay:D", "frozen@everydisplay"); not needed ?
    // eval_let("tex_everymath:D", "frozen@everymath")

    // other redefinitions
    eval_let("if_true:", "tex_iftrue:D");
    eval_let("if_false:", "tex_iffalse:D");
    eval_let("or:", "tex_or:D");
    eval_let("else:", "tex_else:D");
    eval_let("fi:", "tex_fi:D");
    eval_let("reverse_if:N", "etex_unless:D");
    eval_let("if:w", "tex_if:D");
    eval_let("if_charcode:w", "tex_if:D");
    eval_let("if_catcode:w", "tex_ifcat:D");
    eval_let("if_meaning:w", "tex_ifx:D");
    eval_let("if_mode_math:", "tex_ifmmode:D");
    eval_let("if_mode_horizontal:", "tex_ifhmode:D");
    eval_let("if_mode_vertical:", "tex_ifvmode:D");
    eval_let("if_mode_inner:", "tex_ifinner:D");
    eval_let("if_cs_exist:N", "etex_ifdefined:D");
    eval_let("if_cs_exist:w", "etex_ifcsname:D");
    eval_let("cs:w", "tex_csname:D");
    eval_let("cs_end:", "tex_endcsname:D");
    eval_let("exp_after:wN", "tex_expandafter:D");
    T_exp_notN = eval_letv("exp_not:N", "tex_noexpand:D");
    T_exp_notn = eval_letv("exp_not:n", "etex_unexpanded:D");
    eval_let("token_to_meaning:N", "tex_meaning:D");
    eval_let("token_to_str:N", "tex_string:D");
    eval_let("cs_meaning:N", "tex_meaning:D");
    eval_let("scan_stop:", "tex_relax:D");
    eval_let("group_begin:", "tex_begingroup:D");
    eval_let("group_end:", "tex_endgroup:D");
    eval_let("if_int_compare:w", "tex_ifnum:D");
    T_roman = eval_letv("__int_to_roman:w", "tex_romannumeral:D");
    eval_let("group_insert_after:N", "tex_aftergroup:D");
    eval_let("c_minus_one", "m@ne");
    eval_let("cs_set_nopar:Npn", "tex_def:D");
    eval_let("cs_set_nopar:Npx", "tex_edef:D");
    eval_let("cs_gset_nopar:Npn", "tex_gdef:D");
    eval_let("cs_gset_nopar:Npx", "tex_xdef:D");
    T_use1 = eval_letv("use_i:nn", "@firstoftwo");
    T_use2 = eval_letv("use_ii:nn", "@secondoftwo");
    eval_let("__int_value:w", "tex_number:D");
    eval_let("__int_eval:w", "etex_numexpr:D");
    eval_let("__int_eval_end:", "tex_relax:D");
    eval_let("if_int_odd:w", "tex_ifodd:D");
    eval_let("if_case:w", "tex_ifcase:D");
    eval_let("prg_do_nothing:", "c_empty_tl");
    T_empty = locate("c_empty_tl");

    ExplFileName_token        = locate("ExplFileName");
    ExplFileDate_token        = locate("ExplFileDate");
    ExplFileVersion_token     = locate("ExplFileVersion");
    ExplFileDescription_token = locate("ExplFileDescription");
    q_nil                     = locate("q_nil");
    q_stop                    = locate("q_stop");
    q_recursion_stop          = locate("q_recursion_stop");
    T3col_tok                 = locate(":::");

    primitive("GetIdInfo", GetIdInfo_cmd);
    primitive("GetIdInfoLog", GetIdInfoLog_cmd);
    primitive("use:c", usename_cmd, zero_code);
    expargsnc_tok = primitive("exp_args:Nc", usename_cmd, one_code);
    primitive("exp_args:cc", usename_cmd, two_code);

    primitive("cs_meaning:c", convert_cmd, meaning_c_code);
    eval_let("token_to_meaning:c", "cs_meaning:c");
    primitive("cs_set:Npx", def_cmd, ledef_code);
    primitive("cs_set:Npn", def_cmd, ldef_code);
    primitive("cs_gset:Npx", def_cmd, lxdef_code);
    primitive("cs_gset:Npn", def_cmd, lgdef_code);
    primitive("cs_set_protected_nopar:Npx", def_cmd, pedef_code);
    primitive("cs_set_protected_nopar:Npn", def_cmd, pdef_code);
    primitive("cs_set_protected:Npx", def_cmd, pledef_code);
    primitive("cs_set_protected:Npn", def_cmd, pldef_code);
    primitive("cs_gset_protected_nopar:Npx", def_cmd, pxdef_code);
    primitive("cs_gset_protected_nopar:Npn", def_cmd, pgdef_code);
    primitive("cs_gset_protected:Npx", def_cmd, plxdef_code);
    primitive("cs_gset_protected:Npn", def_cmd, plgdef_code);
    primitive("cs_new_nopar:Npn", def_cmd, gdefn_code);
    primitive("cs_new_nopar:Npx", def_cmd, xdefn_code);
    primitive("cs_new:Npn", def_cmd, lgdefn_code);
    primitive("cs_new:Npx", def_cmd, lxdefn_code);
    primitive("cs_new_protected_nopar:Npn", def_cmd, pgdefn_code);
    primitive("cs_new_protected_nopar:Npx", def_cmd, pxdefn_code);
    primitive("cs_new_protected:Npn", def_cmd, plgdefn_code);
    primitive("cs_new_protected:Npx", def_cmd, plxdefn_code);

    primitive("cs_set_nopar:cpn", def_cmd, cdef_code);
    primitive("cs_set_nopar:cpx", def_cmd, cedef_code);
    primitive("cs_gset_nopar:cpn", def_cmd, cgdef_code);
    primitive("cs_gset_nopar:cpx", def_cmd, cxdef_code);
    primitive("cs_new_nopar:cpn", def_cmd, cgdefn_code);
    primitive("cs_new_nopar:cpx", def_cmd, cxdefn_code);
    primitive("cs_set:cpn", def_cmd, cldef_code);
    primitive("cs_set:cpx", def_cmd, cledef_code);
    primitive("cs_gset:cpn", def_cmd, clgdef_code);
    primitive("cs_gset:cpx", def_cmd, clxdef_code);
    primitive("cs_new:cpn", def_cmd, clgdefn_code);
    primitive("cs_new:cpx", def_cmd, clxdefn_code);
    primitive("cs_set_protected_nopar:cpn", def_cmd, cpdef_code);
    primitive("cs_set_protected_nopar:cpx", def_cmd, cpedef_code);
    primitive("cs_gset_protected_nopar:cpn", def_cmd, cpgdef_code);
    primitive("cs_gset_protected_nopar:cpx", def_cmd, cpxdef_code);
    primitive("cs_new_protected_nopar:cpn", def_cmd, cpgdefn_code);
    primitive("cs_new_protected_nopar:cpx", def_cmd, cpxdefn_code);
    primitive("cs_set_protected:cpn", def_cmd, cpldef_code);
    primitive("cs_set_protected:cpx", def_cmd, cpledef_code);
    primitive("cs_gset_protected:cpn", def_cmd, cplgdef_code);
    primitive("cs_gset_protected:cpx", def_cmd, cplxdef_code);
    primitive("cs_new_protected:cpn", def_cmd, cplgdefn_code);
    primitive("cs_new_protected:cpx", def_cmd, cplxdefn_code);

    T_usen = primitive("use:n", all_of_one_cmd, one_code);
    primitive("use:nn", all_of_one_cmd, two_code);
    primitive("use:nnn", all_of_one_cmd, three_code);
    primitive("use:nnnn", all_of_one_cmd, four_code);
    primitive("use_i_ii:nnn", all_of_one_cmd, subtypes(5));
    primitive("use_i:nnn", first_of_three_cmd, one_code);
    primitive("use_ii:nnn", first_of_three_cmd, two_code);
    primitive("use_iii:nnn", first_of_three_cmd, three_code);
    primitive("use_i:nnnn", first_of_four_cmd, one_code);
    primitive("use_ii:nnnn", first_of_four_cmd, two_code);
    primitive("use_iii:nnnn", first_of_four_cmd, three_code);
    primitive("use_iv:nnnn", first_of_four_cmd, four_code);
    T_use_nonen = primitive("use_none:n", ignore_n_args_cmd, one_code);
    primitive("use_none:nn", ignore_n_args_cmd, two_code);
    primitive("use_none:nnn", ignore_n_args_cmd, three_code);
    primitive("use_none:nnnn", ignore_n_args_cmd, four_code);
    primitive("use_none:nnnnn", ignore_n_args_cmd, subtypes(5));
    primitive("use_none:nnnnnn", ignore_n_args_cmd, subtypes(6));
    primitive("use_none:nnnnnnn", ignore_n_args_cmd, subtypes(7));
    primitive("use_none:nnnnnnnn", ignore_n_args_cmd, subtypes(8));
    primitive("use_none:nnnnnnnnn", ignore_n_args_cmd, subtypes(9));
    primitive("prg_return_true:", prg_return_cmd, zero_code);
    primitive("prg_return_false:", prg_return_cmd, one_code);
    primitive("prg_set_conditional:Npnn", l3_gen_cond_Npnn_cmd, L3_set_code);
    primitive("prg_new_conditional:Npnn", l3_gen_cond_Npnn_cmd, L3_new_code);
    primitive("prg_set_protected_conditional:Npnn", l3_gen_cond_Npnn_cmd, L3_setp_code);
    primitive("prg_new_protected_conditional:Npnn", l3_gen_cond_Npnn_cmd, L3_newp_code);
    primitive("prg_set_conditional:Nnn", l3_gen_cond_Nnn_cmd, L3_set_code);
    primitive("prg_new_conditional:Nnn", l3_gen_cond_Nnn_cmd, L3_new_code);
    primitive("prg_set_protected_conditional:Nnn", l3_gen_cond_Nnn_cmd, L3_setp_code);
    primitive("prg_new_protected_conditional:Nnn", l3_gen_cond_Nnn_cmd, L3_newp_code);
    primitive("prg_set_eq_conditional:NNn", l3_gen_eq_cond_cmd, L3_set_code);
    primitive("prg_new_eq_conditional:NNn", l3_gen_eq_cond_cmd, L3_new_code);
    primitive("cs_to_str:N", convert_cmd, l3string_code);
    primitive("__cs_get_function_name:N", splitfun_cmd, zero_code);
    primitive("__cs_get_function_signature:N", splitfun_cmd, one_code);
    primitive("iow_term:x", extension_cmd, write_term_code);
    primitive("iow_log:x", extension_cmd, write_log_code);
    primitive("__chk_if_free_cs:N", l3_check_cmd, zero_code);
    primitive("__chk_if_free_cs:c", l3_check_cmd, one_code);
    primitive("__chk_if_exist_cs:N", l3_check_cmd, two_code);
    primitive("__chk_if_exist_cs:c", l3_check_cmd, three_code);
    primitive("cs_set_eq:NN", let_cmd, letNN_code);
    primitive("cs_set_eq:cN", let_cmd, letcN_code);
    primitive("cs_set_eq:Nc", let_cmd, letNc_code);
    primitive("cs_set_eq:cc", let_cmd, letcc_code);
    primitive("cs_gset_eq:NN", let_cmd, gletNN_code);
    primitive("cs_gset_eq:cN", let_cmd, gletcN_code);
    primitive("cs_gset_eq:Nc", let_cmd, gletNc_code);
    primitive("cs_gset_eq:cc", let_cmd, gletcc_code);
    primitive("cs_new_eq:NN", let_cmd, nletNN_code);
    primitive("cs_new_eq:cN", let_cmd, nletcN_code);
    primitive("cs_new_eq:Nc", let_cmd, nletNc_code);
    primitive("cs_new_eq:cc", let_cmd, nletcc_code);
    primitive("cs_undefine:N", let_cmd, undef_code);
    primitive("cs_undefine:c", let_cmd, undefc_code);
    eval_let("tl_set_eq:NN", "cs_set_eq:NN");
    eval_let("tl_set_eq:cN", "cs_set_eq:cN");
    eval_let("tl_set_eq:Nc", "cs_set_eq:Nc");
    eval_let("tl_set_eq:cc", "cs_set_eq:cc");
    eval_let("tl_gset_eq:NN", "cs_gset_eq:NN");
    eval_let("tl_gset_eq:cN", "cs_gset_eq:cN");
    eval_let("tl_gset_eq:Nc", "cs_gset_eq:Nc");
    eval_let("tl_gset_eq:cc", "cs_gset_eq:cc");

    primitive("__kernel_register_show:N", xray_cmd, register_show_code);
    primitive("__kernel_register_show:c", xray_cmd, registerc_show_code);
    eval_let("int_show:N", "__kernel_register_show:N");
    eval_let("int_show:c", "__kernel_register_show:c");
    eval_let("dim_show:N", "__kernel_register_show:N");
    eval_let("dim_show:c", "__kernel_register_show:c");
    eval_let("skip_show:N", "__kernel_register_show:N");
    eval_let("skip_show:c", "__kernel_register_show:c");
    eval_let("muskip_show:N", "__kernel_register_show:N");
    eval_let("muskip_show:c", "__kernel_register_show:c");
    primitive("cs_generate_variant:Nn", l3_generate_variant_cmd);
    gen_from_sig_tok = primitive("__cs_generate_from_signature:NNn", l3_gen_from_sig_cmd);
    primitive("cs_generate_from_arg_count:NNnn", l3_gen_from_ac_cmd, zero_code);
    primitive("cs_generate_from_arg_count:cNnn", l3_gen_from_ac_cmd, one_code);
    primitive("cs_generate_from_arg_count:Ncnn", l3_gen_from_ac_cmd, two_code);
    primitive("::n", l3expand_aux_cmd, l3expn_code);
    primitive("::N", l3expand_aux_cmd, l3expN_code);
    primitive("::p", l3expand_aux_cmd, l3expp_code);
    primitive("::c", l3expand_aux_cmd, l3expc_code);
    primitive("::o", l3expand_aux_cmd, l3expo_code);
    primitive("::f", l3expand_aux_cmd, l3expf_code);
    primitive("::x", l3expand_aux_cmd, l3expx_code);
    primitive("::V", l3expand_aux_cmd, l3expV_code);
    primitive("::v", l3expand_aux_cmd, l3expv_code);
    primitive("::f_unbraced", l3expand_aux_cmd, l3expfu_code);
    primitive("::o_unbraced", l3expand_aux_cmd, l3expou_code);
    primitive("::v_unbraced", l3expand_aux_cmd, l3expvu_code);
    primitive("::V_unbraced", l3expand_aux_cmd, l3expVu_code);
    primitive("::x_unbraced", l3expand_aux_cmd, l3expxu_code);
    primitive("exp_not:c", l3noexpand_cmd, l3expc_code);
    primitive("exp_not:f", l3noexpand_cmd, l3expf_code);
    primitive("exp_not:o", l3noexpand_cmd, l3expo_code);
    primitive("exp_not:v", l3noexpand_cmd, l3expv_code);
    primitive("exp_not:V", l3noexpand_cmd, l3expV_code);
    primitive("exp_args:No", l3expand_base_cmd, l3exp_No_code);
    primitive("exp_args:NNo", l3expand_base_cmd, l3exp_NNo_code);
    primitive("exp_args:NNNo", l3expand_base_cmd, l3exp_NNNo_code);
    primitive("exp_args:NNc", l3expand_base_cmd, l3exp_NNc_code);
    primitive("exp_args:Ncc", l3expand_base_cmd, l3exp_Ncc_code);
    primitive("exp_args:Nccc", l3expand_base_cmd, l3exp_Nccc_code);
    primitive("exp_args:Nf", l3expand_base_cmd, l3exp_Nf_code);
    primitive("exp_args:NNf", l3expand_base_cmd, l3exp_NNf_code);
    primitive("exp_args:Nv", l3expand_base_cmd, l3exp_Nv_code);
    primitive("exp_args:NNv", l3expand_base_cmd, l3exp_NNv_code);
    primitive("exp_args:NV", l3expand_base_cmd, l3exp_NV_code);
    primitive("exp_args:NNV", l3expand_base_cmd, l3exp_NNV_code);
    primitive("exp_args:NNNV", l3expand_base_cmd, l3exp_NNNV_code);
    primitive("exp_args:Nco", l3expand_base_cmd, l3exp_Nco_code);
    primitive("exp_args:Ncf", l3expand_base_cmd, l3exp_Ncf_code);
    primitive("exp_args:NVV", l3expand_base_cmd, l3exp_NVV_code);
    primitive("exp_args:NcNc", l3expand_base_cmd, l3exp_NcNc_code);
    primitive("exp_args:Ncco", l3expand_base_cmd, l3exp_Ncco_code);
    primitive("exp_args:Nx", l3expand_base_cmd, l3exp_Nx_code);
    primitive("exp_args:Nnc", l3expand_base_cmd, l3exp_Nnc_code);
    primitive("exp_args:Nfo", l3expand_base_cmd, l3exp_Nfo_code);
    primitive("exp_args:Nff", l3expand_base_cmd, l3exp_Nff_code);
    primitive("exp_args:Nnf", l3expand_base_cmd, l3exp_Nnf_code);
    primitive("exp_args:Nno", l3expand_base_cmd, l3exp_Nno_code);
    primitive("exp_args:NnV", l3expand_base_cmd, l3exp_NnV_code);
    primitive("exp_args:Noo", l3expand_base_cmd, l3exp_Noo_code);
    primitive("exp_args:Nof", l3expand_base_cmd, l3exp_Nof_code);
    primitive("exp_args:Noc", l3expand_base_cmd, l3exp_Noc_code);
    primitive("exp_args:NNx", l3expand_base_cmd, l3exp_NNx_code);
    primitive("exp_args:Ncx", l3expand_base_cmd, l3exp_Ncx_code);
    primitive("exp_args:Nnx", l3expand_base_cmd, l3exp_Nnx_code);
    primitive("exp_args:Nox", l3expand_base_cmd, l3exp_Nox_code);
    primitive("exp_args:Nxo", l3expand_base_cmd, l3exp_Nxo_code);
    primitive("exp_args:Nxx", l3expand_base_cmd, l3exp_Nxx_code);
    primitive("exp_args:NNno", l3expand_base_cmd, l3exp_NNno_code);
    primitive("exp_args:NNoo", l3expand_base_cmd, l3exp_NNoo_code);
    primitive("exp_args:Nnnc", l3expand_base_cmd, l3exp_Nnnc_code);
    primitive("exp_args:Nnno", l3expand_base_cmd, l3exp_Nnno_code);
    primitive("exp_args:Nooo", l3expand_base_cmd, l3exp_Nooo_code);
    primitive("exp_args:NNnx", l3expand_base_cmd, l3exp_NNnx_code);
    primitive("exp_args:NNox", l3expand_base_cmd, l3exp_NNox_code);
    primitive("exp_args:Nnnx", l3expand_base_cmd, l3exp_Nnnx_code);
    primitive("exp_args:Nnox", l3expand_base_cmd, l3exp_Nnox_code);
    primitive("exp_args:Nccx", l3expand_base_cmd, l3exp_Nccx_code);
    primitive("exp_args:Ncnx", l3expand_base_cmd, l3exp_Ncnx_code);
    primitive("exp_args:Noox", l3expand_base_cmd, l3exp_Noox_code);
    primitive("exp_last_unbraced:NV", l3expand_base_cmd, l3exp_NVu_code);
    primitive("exp_last_unbraced:Nv", l3expand_base_cmd, l3exp_Nvu_code);
    primitive("exp_last_unbraced:No", l3expand_base_cmd, l3exp_Nou_code);
    primitive("exp_last_unbraced:Nf", l3expand_base_cmd, l3exp_Nfu_code);
    primitive("exp_last_unbraced:Nco", l3expand_base_cmd, l3exp_Ncou_code);
    primitive("exp_last_unbraced:NcV", l3expand_base_cmd, l3exp_NcVu_code);
    primitive("exp_last_unbraced:NNV", l3expand_base_cmd, l3exp_NNVu_code);
    primitive("exp_last_unbraced:NNo", l3expand_base_cmd, l3exp_NNou_code);
    primitive("exp_last_unbraced:NNNV", l3expand_base_cmd, l3exp_NNNVu_code);
    primitive("exp_last_unbraced:NNNo", l3expand_base_cmd, l3exp_NNNou_code);
    primitive("exp_last_unbraced:Nno", l3expand_base_cmd, l3exp_Nnou_code);
    primitive("exp_last_unbraced:Noo", l3expand_base_cmd, l3exp_Noou_code);
    primitive("exp_last_unbraced:Nfo", l3expand_base_cmd, l3exp_Nfou_code);
    primitive("exp_last_unbraced:NnNo", l3expand_base_cmd, l3exp_NnNou_code);
    primitive("exp_last_unbraced:Nx", l3expand_base_cmd, l3exp_Nxu_code);
    primitive("exp_last_two_unbraced:Noo", l3expand_base_cmd, l3exp_Nouou_code);
    primitive("cs_if_eq_p:NN", l3_ifx_cmd, l3_p_NN_code);
    primitive("cs_if_eq:NNTF", l3_ifx_cmd, l3_TF_NN_code);
    primitive("cs_if_eq:NNT", l3_ifx_cmd, l3_T_NN_code);
    primitive("cs_if_eq:NNF", l3_ifx_cmd, l3_F_NN_code);
    primitive("cs_if_eq_p:Nc", l3_ifx_cmd, l3_p_Nc_code);
    primitive("cs_if_eq:NcTF", l3_ifx_cmd, l3_TF_Nc_code);
    primitive("cs_if_eq:NcT", l3_ifx_cmd, l3_T_Nc_code);
    primitive("cs_if_eq:NcF", l3_ifx_cmd, l3_F_Nc_code);
    primitive("cs_if_eq_p:cN", l3_ifx_cmd, l3_p_cN_code);
    primitive("cs_if_eq:cNTF", l3_ifx_cmd, l3_TF_cN_code);
    primitive("cs_if_eq:cNT", l3_ifx_cmd, l3_T_cN_code);
    primitive("cs_if_eq:cNF", l3_ifx_cmd, l3_F_cN_code);
    primitive("cs_if_eq_p:cc", l3_ifx_cmd, l3_p_cc_code);
    primitive("cs_if_eq:ccTF", l3_ifx_cmd, l3_TF_cc_code);
    primitive("cs_if_eq:ccT", l3_ifx_cmd, l3_T_cc_code);
    primitive("cs_if_eq:ccF", l3_ifx_cmd, l3_F_cc_code);
    primitive("str_if_eq_p:nn", l3str_ifeq_cmd, l3_p_code);
    primitive("str_if_eq:nnTF", l3str_ifeq_cmd, l3_TF_code);
    primitive("str_if_eq:nnT", l3str_ifeq_cmd, l3_T_code);
    primitive("str_if_eq:nnF", l3str_ifeq_cmd, l3_F_code);
    primitive("str_if_eq_x_p:nn", l3str_ifeq_cmd, subtypes(l3_p_code + 4));
    primitive("str_if_eq_x:nnTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 4));
    primitive("str_if_eq_x:nnT", l3str_ifeq_cmd, subtypes(l3_T_code + 4));
    primitive("str_if_eq_x:nnF", l3str_ifeq_cmd, subtypes(l3_F_code + 4));
    primitive("str_if_eq_p:Vn", l3str_ifeq_cmd, subtypes(l3_p_code + 8));
    primitive("str_if_eq:VnTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 8));
    primitive("str_if_eq:VnT", l3str_ifeq_cmd, subtypes(l3_T_code + 8));
    primitive("str_if_eq:VnF", l3str_ifeq_cmd, subtypes(l3_F_code + 8));
    primitive("str_if_eq_p:on", l3str_ifeq_cmd, subtypes(l3_p_code + 12));
    primitive("str_if_eq:onTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 12));
    primitive("str_if_eq:onT", l3str_ifeq_cmd, subtypes(l3_T_code + 12));
    primitive("str_if_eq:onF", l3str_ifeq_cmd, subtypes(l3_F_code + 12));
    primitive("str_if_eq_p:no", l3str_ifeq_cmd, subtypes(l3_p_code + 16));
    primitive("str_if_eq:noTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 16));
    primitive("str_if_eq:noT", l3str_ifeq_cmd, subtypes(l3_T_code + 16));
    primitive("str_if_eq:noF", l3str_ifeq_cmd, subtypes(l3_F_code + 16));
    primitive("str_if_eq_p:nV", l3str_ifeq_cmd, subtypes(l3_p_code + 20));
    primitive("str_if_eq:nVTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 20));
    primitive("str_if_eq:nVT", l3str_ifeq_cmd, subtypes(l3_T_code + 20));
    primitive("str_if_eq:nVF", l3str_ifeq_cmd, subtypes(l3_F_code + 20));
    primitive("str_if_eq_p:VV", l3str_ifeq_cmd, subtypes(l3_p_code + 24));
    primitive("str_if_eq:VVTF", l3str_ifeq_cmd, subtypes(l3_TF_code + 24));
    primitive("str_if_eq:VVT", l3str_ifeq_cmd, subtypes(l3_T_code + 24));
    primitive("str_if_eq:VVF", l3str_ifeq_cmd, subtypes(l3_F_code + 24));
    primitive("str_case:nn", l3str_case_cmd, l3_p_code);
    primitive("str_case:nnTF", l3str_case_cmd, l3_TF_code);
    primitive("str_case:nnT", l3str_case_cmd, l3_T_code);
    primitive("str_case:nnF", l3str_case_cmd, l3_F_code);
    primitive("str_case:nnn", l3str_case_cmd, l3_F_code);
    primitive("str_case_x:nn", l3str_case_cmd, subtypes(l3_p_code + 4));
    primitive("str_case_x:nnTF", l3str_case_cmd, subtypes(l3_TF_code + 4));
    primitive("str_case_x:nnT", l3str_case_cmd, subtypes(l3_T_code + 4));
    primitive("str_case_x:nnF", l3str_case_cmd, subtypes(l3_F_code + 4));
    primitive("str_case_x:nnn", l3str_case_cmd, subtypes(l3_F_code + 4));
    primitive("str_case:on", l3str_case_cmd, subtypes(l3_p_code + 8));
    primitive("str_case:onTF", l3str_case_cmd, subtypes(l3_TF_code + 8));
    primitive("str_case:onT", l3str_case_cmd, subtypes(l3_T_code + 8));
    primitive("str_case:onF", l3str_case_cmd, subtypes(l3_F_code + 8));
    primitive("str_case:onn", l3str_case_cmd, subtypes(l3_F_code + 8));
    primitive("char_set_catcode_escape:n", l3_set_cat_cmd, subtypes(escape_catcode));
    primitive("char_set_catcode_group_begin:n", l3_set_cat_cmd, subtypes(open_catcode));
    primitive("char_set_catcode_group_end:n", l3_set_cat_cmd, subtypes(close_catcode));
    primitive("char_set_catcode_math_toggle:n", l3_set_cat_cmd, subtypes(dollar_catcode));
    primitive("char_set_catcode_alignment:n", l3_set_cat_cmd, subtypes(alignment_catcode));
    primitive("char_set_catcode_end_line:n", l3_set_cat_cmd, subtypes(eol_catcode));
    primitive("char_set_catcode_parameter:n", l3_set_cat_cmd, subtypes(parameter_catcode));
    primitive("char_set_catcode_math_superscript:n", l3_set_cat_cmd, subtypes(hat_catcode));
    primitive("char_set_catcode_math_subscript:n", l3_set_cat_cmd, subtypes(underscore_catcode));
    primitive("char_set_catcode_ignore:n", l3_set_cat_cmd, subtypes(ignored_catcode));
    primitive("char_set_catcode_space:n", l3_set_cat_cmd, subtypes(space_catcode));
    primitive("char_set_catcode_letter:n", l3_set_cat_cmd, subtypes(letter_catcode));
    primitive("char_set_catcode_other:n", l3_set_cat_cmd, subtypes(other_catcode));
    primitive("char_set_catcode_active:n", l3_set_cat_cmd, subtypes(active_catcode));
    primitive("char_set_catcode_invalid:n", l3_set_cat_cmd, subtypes(invalid_catcode));
    primitive("char_set_catcode_comment:n", l3_set_cat_cmd, subtypes(comment_catcode));
    primitive("char_set_catcode_escape:N", l3_set_cat_cmd, subtypes(escape_catcode + 16));
    primitive("char_set_catcode_group_begin:N", l3_set_cat_cmd, subtypes(open_catcode + 16));
    primitive("char_set_catcode_group_end:N", l3_set_cat_cmd, subtypes(close_catcode + 16));
    primitive("char_set_catcode_math_toggle:N", l3_set_cat_cmd, subtypes(dollar_catcode + 16));
    primitive("char_set_catcode_alignment:N", l3_set_cat_cmd, subtypes(alignment_catcode + 16));
    primitive("char_set_catcode_end_line:N", l3_set_cat_cmd, subtypes(eol_catcode + 16));
    primitive("char_set_catcode_parameter:N", l3_set_cat_cmd, subtypes(parameter_catcode + 16));
    primitive("char_set_catcode_math_superscript:N", l3_set_cat_cmd, subtypes(hat_catcode + 16));
    primitive("char_set_catcode_math_subscript:N", l3_set_cat_cmd, subtypes(underscore_catcode + 16));
    primitive("char_set_catcode_ignore:N", l3_set_cat_cmd, subtypes(ignored_catcode + 16));
    primitive("char_set_catcode_space:N", l3_set_cat_cmd, subtypes(space_catcode + 16));
    primitive("char_set_catcode_letter:N", l3_set_cat_cmd, subtypes(letter_catcode + 16));
    primitive("char_set_catcode_other:N", l3_set_cat_cmd, subtypes(other_catcode + 16));
    primitive("char_set_catcode_active:N", l3_set_cat_cmd, subtypes(active_catcode + 16));
    primitive("char_set_catcode_invalid:N", l3_set_cat_cmd, subtypes(invalid_catcode + 16));
    primitive("char_set_catcode_comment:N", l3_set_cat_cmd, subtypes(comment_catcode + 16));
    primitive("char_set_catcode:nn", l3_set_num_cmd, setcat_code);
    primitive("char_value_catcode:n", l3E_set_num_cmd, thecat_code);
    primitive("char_show_value_catcode:n", l3_set_num_cmd, showcat_code);
    primitive("char_set_mathcode:nn", l3_set_num_cmd, setmath_code);
    primitive("char_value_mathcode:n", l3E_set_num_cmd, themath_code);
    primitive("char_show_value_mathcode:n", l3_set_num_cmd, showmath_code);
    primitive("char_set_lccode:nn", l3_set_num_cmd, setlc_code);
    primitive("char_value_lccode:n", l3E_set_num_cmd, thelc_code);
    primitive("char_show_value_lccode:n", l3_set_num_cmd, showlc_code);
    primitive("char_set_uccode:nn", l3_set_num_cmd, setuc_code);
    primitive("char_value_uccode:n", l3E_set_num_cmd, theuc_code);
    primitive("char_show_value_uccode:n", l3_set_num_cmd, showuc_code);
    primitive("char_set_sfcode:nn", l3_set_num_cmd, setsf_code);
    primitive("char_value_sfcode:n", l3E_set_num_cmd, thesf_code);
    primitive("char_show_value_sfcode:n", l3_set_num_cmd, showsf_code);
    primitive("c_group_begin_token", open_catcode, subtypes('{'));
    primitive("c_group_end_token", close_catcode, subtypes('}'));
    primitive("c_math_toggle_token", dollar_catcode, subtypes('*'));
    primitive("c_alignment_token", alignment_catcode, subtypes('*'));
    primitive("c_parameter_token", parameter_catcode, subtypes('#'));
    primitive("c_math_superscript_token", hat_catcode, subtypes('^'));
    primitive("c_math_subscript_token", underscore_catcode, subtypes('*'));
    primitive("c_space_token", space_catcode, subtypes('~'));
    primitive("c_catcode_letter_token", letter_catcode, subtypes('a'));
    primitive("c_catcode_other_token", other_catcode, subtypes('1'));
    primitive("token_if_group_begin_p:N", cat_ifeq_cmd, subtypes(open_catcode * 4 + l3_p_code));
    primitive("token_if_group_begin:NTF", cat_ifeq_cmd, subtypes(open_catcode * 4 + l3_TF_code));
    primitive("token_if_group_begin:NT", cat_ifeq_cmd, subtypes(open_catcode * 4 + l3_T_code));
    primitive("token_if_group_begin:NF", cat_ifeq_cmd, subtypes(open_catcode * 4 + l3_F_code));
    primitive("token_if_group_end_p:N", cat_ifeq_cmd, subtypes(close_catcode * 4 + l3_p_code));
    primitive("token_if_group_end:NTF", cat_ifeq_cmd, subtypes(close_catcode * 4 + l3_TF_code));
    primitive("token_if_group_end:NT", cat_ifeq_cmd, subtypes(close_catcode * 4 + l3_T_code));
    primitive("token_if_group_end:NF", cat_ifeq_cmd, subtypes(close_catcode * 4 + l3_F_code));
    primitive("token_if_math_toggle_p:N", cat_ifeq_cmd, subtypes(dollar_catcode * 4 + l3_p_code));
    primitive("token_if_math_toggle:NTF", cat_ifeq_cmd, subtypes(dollar_catcode * 4 + l3_TF_code));
    primitive("token_if_math_toggle:NT", cat_ifeq_cmd, subtypes(dollar_catcode * 4 + l3_T_code));
    primitive("token_if_math_toggle:NF", cat_ifeq_cmd, subtypes(dollar_catcode * 4 + l3_F_code));
    primitive("token_if_alignment_p:N", cat_ifeq_cmd, subtypes(alignment_catcode * 4 + l3_p_code));
    primitive("token_if_alignment:NTF", cat_ifeq_cmd, subtypes(alignment_catcode * 4 + l3_TF_code));
    primitive("token_if_alignment:NT", cat_ifeq_cmd, subtypes(alignment_catcode * 4 + l3_T_code));
    primitive("token_if_alignment:NF", cat_ifeq_cmd, subtypes(alignment_catcode * 4 + l3_F_code));
    primitive("token_if_parameter_p:N", cat_ifeq_cmd, subtypes(parameter_catcode * 4 + l3_p_code));
    primitive("token_if_parameter:NTF", cat_ifeq_cmd, subtypes(parameter_catcode * 4 + l3_TF_code));
    primitive("token_if_parameter:NT", cat_ifeq_cmd, subtypes(parameter_catcode * 4 + l3_T_code));
    primitive("token_if_parameter:NF", cat_ifeq_cmd, subtypes(parameter_catcode * 4 + l3_F_code));
    primitive("token_if_math_superscript_p:N", cat_ifeq_cmd, subtypes(hat_catcode * 4 + l3_p_code));
    primitive("token_if_math_superscript:NTF", cat_ifeq_cmd, subtypes(hat_catcode * 4 + l3_TF_code));
    primitive("token_if_math_superscript:NT", cat_ifeq_cmd, subtypes(hat_catcode * 4 + l3_T_code));
    primitive("token_if_math_superscript:NF", cat_ifeq_cmd, subtypes(hat_catcode * 4 + l3_F_code));
    primitive("token_if_math_subscript_p:N", cat_ifeq_cmd, subtypes(underscore_catcode * 4 + l3_p_code));
    primitive("token_if_math_subscript:NTF", cat_ifeq_cmd, subtypes(underscore_catcode * 4 + l3_TF_code));
    primitive("token_if_math_subscript:NT", cat_ifeq_cmd, subtypes(underscore_catcode * 4 + l3_T_code));
    primitive("token_if_math_subscript:NF", cat_ifeq_cmd, subtypes(underscore_catcode * 4 + l3_F_code));
    primitive("token_if_space_p:N", cat_ifeq_cmd, subtypes(space_catcode * 4 + l3_p_code));
    primitive("token_if_space:NTF", cat_ifeq_cmd, subtypes(space_catcode * 4 + l3_TF_code));
    primitive("token_if_space:NT", cat_ifeq_cmd, subtypes(space_catcode * 4 + l3_T_code));
    primitive("token_if_space:NF", cat_ifeq_cmd, subtypes(space_catcode * 4 + l3_F_code));
    primitive("token_if_other_p:N", cat_ifeq_cmd, subtypes(other_catcode * 4 + l3_p_code));
    primitive("token_if_other:NTF", cat_ifeq_cmd, subtypes(other_catcode * 4 + l3_TF_code));
    primitive("token_if_other:NT", cat_ifeq_cmd, subtypes(other_catcode * 4 + l3_T_code));
    primitive("token_if_other:NF", cat_ifeq_cmd, subtypes(other_catcode * 4 + l3_F_code));
    primitive("token_if_letter_p:N", cat_ifeq_cmd, subtypes(letter_catcode * 4 + l3_p_code));
    primitive("token_if_letter:NTF", cat_ifeq_cmd, subtypes(letter_catcode * 4 + l3_TF_code));
    primitive("token_if_letter:NT", cat_ifeq_cmd, subtypes(letter_catcode * 4 + l3_T_code));
    primitive("token_if_letter:NF", cat_ifeq_cmd, subtypes(letter_catcode * 4 + l3_F_code));
    primitive("token_if_active_p:N", cat_ifeq_cmd, subtypes(active_catcode * 4 + l3_p_code));
    primitive("token_if_active:NTF", cat_ifeq_cmd, subtypes(active_catcode * 4 + l3_TF_code));
    primitive("token_if_active:NT", cat_ifeq_cmd, subtypes(active_catcode * 4 + l3_T_code));
    primitive("token_if_active:NF", cat_ifeq_cmd, subtypes(active_catcode * 4 + l3_F_code));
    primitive("token_if_eq_catcode_p:NN", token_if_cmd, subtypes(tok_eq_cat_code * 4 + l3_p_code));
    primitive("token_if_eq_charcode_p:NN", token_if_cmd, subtypes(tok_eq_char_code * 4 + l3_p_code));
    primitive("token_if_eq_meaning_p:NN", token_if_cmd, subtypes(tok_eq_meaning_code * 4 + l3_p_code));
    primitive("token_if_macro_p:N", token_if_cmd, subtypes(tok_if_macro_code * 4 + l3_p_code));
    primitive("token_if_cs_p:N", token_if_cmd, subtypes(tok_if_cs_code * 4 + l3_p_code));
    primitive("token_if_expandable_p:N", token_if_cmd, subtypes(tok_if_expandable_code * 4 + l3_p_code));
    primitive("token_if_long_macro_p:N", token_if_cmd, subtypes(tok_if_long_code * 4 + l3_p_code));
    primitive("token_if_protected_macro_p:N", token_if_cmd, subtypes(tok_if_prot_code * 4 + l3_p_code));
    primitive("token_if_protected_long_macro_p:N", token_if_cmd, subtypes(tok_if_longprot_code * 4 + l3_p_code));
    primitive("token_if_chardef_p:N", token_if_cmd, subtypes(tok_if_chardef_code * 4 + l3_p_code));
    primitive("token_if_mathchardef_p:N", token_if_cmd, subtypes(tok_if_mathchardef_code * 4 + l3_p_code));
    primitive("token_if_dim_register_p:N", token_if_cmd, subtypes(tok_if_dim_code * 4 + l3_p_code));
    primitive("token_if_int_register_p:N", token_if_cmd, subtypes(tok_if_int_code * 4 + l3_p_code));
    primitive("token_if_muskip_register_p:N", token_if_cmd, subtypes(tok_if_muskip_code * 4 + l3_p_code));
    primitive("token_if_skip_register_p:N", token_if_cmd, subtypes(tok_if_skip_code * 4 + l3_p_code));
    primitive("token_if_toks_register_p:N", token_if_cmd, subtypes(tok_if_toks_code * 4 + l3_p_code));
    primitive("token_if_primitive_p:N", token_if_cmd, subtypes(tok_if_primitive_code * 4 + l3_p_code));
    primitive("token_if_eq_catcode:NNTF", token_if_cmd, subtypes(tok_eq_cat_code * 4 + l3_TF_code));
    primitive("token_if_eq_charcode:NNTF", token_if_cmd, subtypes(tok_eq_char_code * 4 + l3_TF_code));
    primitive("token_if_eq_meaning:NNTF", token_if_cmd, subtypes(tok_eq_meaning_code * 4 + l3_TF_code));
    primitive("token_if_macro:NTF", token_if_cmd, subtypes(tok_if_macro_code * 4 + l3_TF_code));
    primitive("token_if_cs:NTF", token_if_cmd, subtypes(tok_if_cs_code * 4 + l3_TF_code));
    primitive("token_if_expandable:NTF", token_if_cmd, subtypes(tok_if_expandable_code * 4 + l3_TF_code));
    primitive("token_if_long_macro:NTF", token_if_cmd, subtypes(tok_if_long_code * 4 + l3_TF_code));
    primitive("token_if_protected_macro:NTF", token_if_cmd, subtypes(tok_if_prot_code * 4 + l3_TF_code));
    primitive("token_if_protected_long_macro:NTF", token_if_cmd, subtypes(tok_if_longprot_code * 4 + l3_TF_code));
    primitive("token_if_chardef:NTF", token_if_cmd, subtypes(tok_if_chardef_code * 4 + l3_TF_code));
    primitive("token_if_mathchardef:NTF", token_if_cmd, subtypes(tok_if_mathchardef_code * 4 + l3_TF_code));
    primitive("token_if_dim_register:NTF", token_if_cmd, subtypes(tok_if_dim_code * 4 + l3_TF_code));
    primitive("token_if_int_register:NTF", token_if_cmd, subtypes(tok_if_int_code * 4 + l3_TF_code));
    primitive("token_if_muskip_register:NTF", token_if_cmd, subtypes(tok_if_muskip_code * 4 + l3_TF_code));
    primitive("token_if_skip_register:NTF", token_if_cmd, subtypes(tok_if_skip_code * 4 + l3_TF_code));
    primitive("token_if_toks_register:NTF", token_if_cmd, subtypes(tok_if_toks_code * 4 + l3_TF_code));
    primitive("token_if_primitive:NTF", token_if_cmd, subtypes(tok_if_primitive_code * 4 + l3_TF_code));
    primitive("token_if_eq_catcode:NNT", token_if_cmd, subtypes(tok_eq_cat_code * 4 + l3_T_code));
    primitive("token_if_eq_charcode:NNT", token_if_cmd, subtypes(tok_eq_char_code * 4 + l3_T_code));
    primitive("token_if_eq_meaning:NNT", token_if_cmd, subtypes(tok_eq_meaning_code * 4 + l3_T_code));
    primitive("token_if_macro:NT", token_if_cmd, subtypes(tok_if_macro_code * 4 + l3_T_code));
    primitive("token_if_cs:NT", token_if_cmd, subtypes(tok_if_cs_code * 4 + l3_T_code));
    primitive("token_if_expandable:NT", token_if_cmd, subtypes(tok_if_expandable_code * 4 + l3_T_code));
    primitive("token_if_long_macro:NT", token_if_cmd, subtypes(tok_if_long_code * 4 + l3_T_code));
    primitive("token_if_protected_macro:NT", token_if_cmd, subtypes(tok_if_prot_code * 4 + l3_T_code));
    primitive("token_if_protected_long_macro:NT", token_if_cmd, subtypes(tok_if_longprot_code * 4 + l3_T_code));
    primitive("token_if_chardef:NT", token_if_cmd, subtypes(tok_if_chardef_code * 4 + l3_T_code));
    primitive("token_if_mathchardef:NT", token_if_cmd, subtypes(tok_if_mathchardef_code * 4 + l3_T_code));
    primitive("token_if_dim_register:NT", token_if_cmd, subtypes(tok_if_dim_code * 4 + l3_T_code));
    primitive("token_if_int_register:NT", token_if_cmd, subtypes(tok_if_int_code * 4 + l3_T_code));
    primitive("token_if_muskip_register:NT", token_if_cmd, subtypes(tok_if_muskip_code * 4 + l3_T_code));
    primitive("token_if_skip_register:NT", token_if_cmd, subtypes(tok_if_skip_code * 4 + l3_T_code));
    primitive("token_if_toks_register:NT", token_if_cmd, subtypes(tok_if_toks_code * 4 + l3_T_code));
    primitive("token_if_primitive:NT", token_if_cmd, subtypes(tok_if_primitive_code * 4 + l3_T_code));
    primitive("token_if_eq_catcode:NNF", token_if_cmd, subtypes(tok_eq_cat_code * 4 + l3_F_code));
    primitive("token_if_eq_charcode:NNF", token_if_cmd, subtypes(tok_eq_char_code * 4 + l3_F_code));
    primitive("token_if_eq_meaning:NNF", token_if_cmd, subtypes(tok_eq_meaning_code * 4 + l3_F_code));
    primitive("token_if_macro:NF", token_if_cmd, subtypes(tok_if_macro_code * 4 + l3_F_code));
    primitive("token_if_cs:NF", token_if_cmd, subtypes(tok_if_cs_code * 4 + l3_F_code));
    primitive("token_if_expandable:NF", token_if_cmd, subtypes(tok_if_expandable_code * 4 + l3_F_code));
    primitive("token_if_long_macro:NF", token_if_cmd, subtypes(tok_if_long_code * 4 + l3_F_code));
    primitive("token_if_protected_macro:NF", token_if_cmd, subtypes(tok_if_prot_code * 4 + l3_F_code));
    primitive("token_if_protected_long_macro:NF", token_if_cmd, subtypes(tok_if_longprot_code * 4 + l3_F_code));
    primitive("token_if_chardef:NF", token_if_cmd, subtypes(tok_if_chardef_code * 4 + l3_F_code));
    primitive("token_if_mathchardef:NF", token_if_cmd, subtypes(tok_if_mathchardef_code * 4 + l3_F_code));
    primitive("token_if_dim_register:NF", token_if_cmd, subtypes(tok_if_dim_code * 4 + l3_F_code));
    primitive("token_if_int_register:NF", token_if_cmd, subtypes(tok_if_int_code * 4 + l3_F_code));
    primitive("token_if_muskip_register:NF", token_if_cmd, subtypes(tok_if_muskip_code * 4 + l3_F_code));
    primitive("token_if_skip_register:NF", token_if_cmd, subtypes(tok_if_skip_code * 4 + l3_F_code));
    primitive("token_if_toks_register:NF", token_if_cmd, subtypes(tok_if_toks_code * 4 + l3_F_code));
    primitive("token_if_primitive:NF", token_if_cmd, subtypes(tok_if_primitive_code * 4 + l3_F_code));
    primitive("tl_new:N", tl_basic_cmd, l3_tl_new_code);
    primitive("tl_clear:N", tl_basic_cmd, l3_tl_clear_code);
    primitive("tl_gclear:N", tl_basic_cmd, l3_tl_gclear_code);
    primitive("tl_const:Nn", tl_basic_cmd, l3_tl_const_code);
    primitive("tl_const:Nx", tl_basic_cmd, l3_tl_xconst_code);
    primitive("tl_clear_new:N", tl_basic_cmd, l3_tl_clearnew_code);
    primitive("tl_gclear_new:N", tl_basic_cmd, l3_tl_gclearnew_code);
    primitive("tl_new:c", tl_basic_cmd, l3_tlx_new_code);
    primitive("tl_clear:c", tl_basic_cmd, l3_tlx_clear_code);
    primitive("tl_gclear:c", tl_basic_cmd, l3_tlx_gclear_code);
    primitive("tl_const:cn", tl_basic_cmd, l3_tlx_const_code);
    primitive("tl_const:cx", tl_basic_cmd, l3_tlx_xconst_code);
    primitive("tl_clear_new:c", tl_basic_cmd, l3_tlx_clearnew_code);
    primitive("tl_gclear_new:c", tl_basic_cmd, l3_tlx_gclearnew_code);
    primitive("tl_concat:NNN", tl_concat_cmd, zero_code);
    primitive("tl_concat:ccc", tl_concat_cmd, one_code);
    primitive("tl_gconcat:NNN", tl_concat_cmd, two_code);
    primitive("tl_gconcat:ccc", tl_concat_cmd, three_code);
    primitive("tl_set:Nn", tl_set_cmd, l3expn_code);
    primitive("tl_set:No", tl_set_cmd, l3expo_code);
    primitive("tl_set:Nf", tl_set_cmd, l3expf_code);
    primitive("tl_set:Nx", tl_set_cmd, l3expx_code);
    primitive("tl_set:NV", tl_set_cmd, l3expV_code);
    primitive("tl_set:Nv", tl_set_cmd, l3expv_code);
    primitive("tl_set:cn", tl_set_cmd, subtypes(l3expn_code + 9));
    primitive("tl_set:co", tl_set_cmd, subtypes(l3expo_code + 9));
    primitive("tl_set:cf", tl_set_cmd, subtypes(l3expf_code + 9));
    primitive("tl_set:cx", tl_set_cmd, subtypes(l3expx_code + 9));
    primitive("tl_set:cV", tl_set_cmd, subtypes(l3expV_code + 9));
    primitive("tl_set:cv", tl_set_cmd, subtypes(l3expv_code + 9));
    primitive("tl_gset:Nn", tl_set_cmd, subtypes(l3expn_code + 18));
    primitive("tl_gset:No", tl_set_cmd, subtypes(l3expo_code + 18));
    primitive("tl_gset:Nf", tl_set_cmd, subtypes(l3expf_code + 18));
    primitive("tl_gset:Nx", tl_set_cmd, subtypes(l3expx_code + 18));
    primitive("tl_gset:NV", tl_set_cmd, subtypes(l3expV_code + 18));
    primitive("tl_gset:Nv", tl_set_cmd, subtypes(l3expv_code + 18));
    primitive("tl_gset:cn", tl_set_cmd, subtypes(l3expn_code + 9 + 18));
    primitive("tl_gset:co", tl_set_cmd, subtypes(l3expo_code + 9 + 18));
    primitive("tl_gset:cf", tl_set_cmd, subtypes(l3expf_code + 9 + 18));
    primitive("tl_gset:cx", tl_set_cmd, subtypes(l3expx_code + 9 + 18));
    primitive("tl_gset:cV", tl_set_cmd, subtypes(l3expV_code + 9 + 18));
    primitive("tl_gset:cv", tl_set_cmd, subtypes(l3expv_code + 9 + 18));

    primitive("tl_put_left:Nn", tl_put_left_cmd, l3expn_code);
    primitive("tl_put_left:No", tl_put_left_cmd, l3expo_code);
    primitive("tl_put_left:Nx", tl_put_left_cmd, l3expx_code);
    primitive("tl_put_left:NV", tl_put_left_cmd, l3expV_code);
    primitive("tl_put_left:cn", tl_put_left_cmd, subtypes(l3expn_code + 9));
    primitive("tl_put_left:co", tl_put_left_cmd, subtypes(l3expo_code + 9));
    primitive("tl_put_left:cx", tl_put_left_cmd, subtypes(l3expx_code + 9));
    primitive("tl_put_left:cV", tl_put_left_cmd, subtypes(l3expV_code + 9));
    primitive("tl_gput_left:Nn", tl_put_left_cmd, subtypes(l3expn_code + 18));
    primitive("tl_gput_left:No", tl_put_left_cmd, subtypes(l3expo_code + 18));
    primitive("tl_gput_left:Nx", tl_put_left_cmd, subtypes(l3expx_code + 18));
    primitive("tl_gput_left:NV", tl_put_left_cmd, subtypes(l3expV_code + 18));
    primitive("tl_gput_left:cn", tl_put_left_cmd, subtypes(l3expn_code + 9 + 18));
    primitive("tl_gput_left:co", tl_put_left_cmd, subtypes(l3expo_code + 9 + 18));
    primitive("tl_gput_left:cx", tl_put_left_cmd, subtypes(l3expx_code + 9 + 18));
    primitive("tl_gput_left:cV", tl_put_left_cmd, subtypes(l3expV_code + 9 + 18));
    primitive("tl_put_right:Nn", tl_put_left_cmd, subtypes(l3expn_code + 36));
    primitive("tl_put_right:No", tl_put_left_cmd, subtypes(l3expo_code + 36));
    primitive("tl_put_right:Nx", tl_put_left_cmd, subtypes(l3expx_code + 36));
    primitive("tl_put_right:NV", tl_put_left_cmd, subtypes(l3expV_code + 36));
    primitive("tl_put_right:cn", tl_put_left_cmd, subtypes(l3expn_code + 9 + 36));
    primitive("tl_put_right:co", tl_put_left_cmd, subtypes(l3expo_code + 9 + 36));
    primitive("tl_put_right:cx", tl_put_left_cmd, subtypes(l3expx_code + 9 + 36));
    primitive("tl_put_right:cV", tl_put_left_cmd, subtypes(l3expV_code + 9 + 36));
    primitive("tl_gput_right:Nn", tl_put_left_cmd, subtypes(l3expn_code + 18 + 36));
    primitive("tl_gput_right:No", tl_put_left_cmd, subtypes(l3expo_code + 18 + 36));
    primitive("tl_gput_right:Nx", tl_put_left_cmd, subtypes(l3expx_code + 18 + 36));
    primitive("tl_gput_right:NV", tl_put_left_cmd, subtypes(l3expV_code + 18 + 36));
    primitive("tl_gput_right:cn", tl_put_left_cmd, subtypes(l3expn_code + 9 + 18 + 36));
    primitive("tl_gput_right:co", tl_put_left_cmd, subtypes(l3expo_code + 9 + 18 + 36));
    primitive("tl_gput_right:cx", tl_put_left_cmd, subtypes(l3expx_code + 9 + 18 + 36));
    primitive("tl_gput_right:cV", tl_put_left_cmd, subtypes(l3expV_code + 9 + 18 + 36));
    primitive("tl_set_rescan:Nnn", l3_rescan_cmd, subtypes(0));
    primitive("tl_set_rescan:Nno", l3_rescan_cmd, subtypes(1));
    primitive("tl_set_rescan:Nnx", l3_rescan_cmd, subtypes(2));
    primitive("tl_set_rescan:cnn", l3_rescan_cmd, subtypes(3));
    primitive("tl_set_rescan:cno", l3_rescan_cmd, subtypes(4));
    primitive("tl_set_rescan:cnx", l3_rescan_cmd, subtypes(5));
    primitive("tl_gset_rescan:Nnn", l3_rescan_cmd, subtypes(6));
    primitive("tl_gset_rescan:Nno", l3_rescan_cmd, subtypes(7));
    primitive("tl_gset_rescan:Nnx", l3_rescan_cmd, subtypes(8));
    primitive("tl_gset_rescan:cnn", l3_rescan_cmd, subtypes(9));
    primitive("tl_gset_rescan:cno", l3_rescan_cmd, subtypes(10));
    primitive("tl_gset_rescan:cnx", l3_rescan_cmd, subtypes(11));
    primitive("tl_rescan:nn", l3_rescan_cmd, subtypes(12));
    primitive("tl_to_lowercase:n", case_shift_cmd, subtypes(6));
    primitive("tl_to_uppercase:n", case_shift_cmd, subtypes(7));
}

// TODO.
// \:::
// \prg_do_nothing:

// implement \__cs_count_signature:N  and \__cs_count_signature:c
// call L3_split_next_name, without err,
//  return -1 if bad, size of tok_sig otherwise
