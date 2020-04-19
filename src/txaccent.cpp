// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains the code for accents.
// for instance, \'e expands to the character 0351 (internal encoding is
// iso-8859-1)

/// \todo The first 256 characters of Unicode are identical to Latin-1. So in
/// fact internal encoding is always unicode, latin1 is a red herring here.

#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"

namespace accent_ns {
    auto fetch_accent(size_t chr, int accent_code) -> Token;
    auto fetch_double_accent(int a, int acc3) -> Token;
    auto combine_accents(int acc1, int acc2) -> int;
    auto double_a_accent(int acc3) -> unsigned;
    auto double_e_accent(int acc3) -> unsigned;
    auto double_o_accent(int acc3) -> unsigned;
    auto double_u_accent(int acc3) -> unsigned;
    auto double_other_accent(int a, int acc3) -> unsigned;
    void special_acc_hack(TokenList &y);
    auto special_double_acc(int chr, int acc) -> Token;
} // namespace accent_ns

// For the case of \'\^e, construct an accent code.
// It is assumed that the order is irrelevant.

auto accent_ns::combine_accents(int acc1, int acc2) -> int {
    // start with acute and something
    if (acc1 == '\'' && acc2 == '~') return 9;
    if (acc2 == '\'' && acc1 == '~') return 9;
    if (acc1 == '\'' && acc2 == '=') return 10;
    if (acc2 == '\'' && acc1 == '=') return 10;
    if (acc1 == '\'' && acc2 == '.') return 11;
    if (acc2 == '\'' && acc1 == '.') return 11;
    if (acc1 == '\'' && acc2 == '^') return 12;
    if (acc2 == '\'' && acc1 == '^') return 12;
    if (acc1 == '\'' && acc2 == 'H') return 16;
    if (acc2 == '\'' && acc1 == 'H') return 16;
    if (acc1 == '\'' && acc2 == 'c') return 17;
    if (acc2 == '\'' && acc1 == 'c') return 17;
    if (acc1 == '\'' && acc2 == '"') return 2;
    if (acc2 == '\'' && acc1 == '"') return 2;
    if (acc1 == '\'' && acc2 == '^') return 21;
    if (acc2 == '\'' && acc1 == '^') return 21;
    if (acc1 == '\'' && acc2 == 'u') return 34;
    if (acc2 == '\'' && acc1 == 'u') return 34;
    // now grave and something
    if (acc1 == '`' && acc2 == '=') return 13;
    if (acc2 == '`' && acc1 == '=') return 13;
    if (acc1 == '`' && acc2 == '^') return 15;
    if (acc2 == '`' && acc1 == '^') return 15;
    if (acc1 == '`' && acc2 == 'H') return 18;
    if (acc2 == '`' && acc1 == 'H') return 18;
    if (acc1 == '`' && acc2 == '"') return 4;
    if (acc2 == '`' && acc1 == '"') return 4;
    if (acc1 == '`' && acc2 == 'u') return 22;
    if (acc2 == '`' && acc1 == 'u') return 22;
    // now dot and something
    if (acc1 == '.' && acc2 == '=') return 5;
    if (acc2 == '.' && acc1 == '=') return 5;
    if (acc1 == '.' && acc2 == 'v') return 19;
    if (acc2 == '.' && acc1 == 'v') return 19;
    if (acc1 == '.' && acc2 == 'd') return 20;
    if (acc2 == '.' && acc1 == 'd') return 20;
    // now hat and something
    if (acc1 == '^' && acc2 == 'h') return 23;
    if (acc2 == '^' && acc1 == 'h') return 23;
    if (acc1 == '^' && acc2 == '~') return 24;
    if (acc2 == '^' && acc1 == '~') return 24;
    if (acc1 == '^' && acc2 == 'd') return 25;
    if (acc2 == '^' && acc1 == 'd') return 25;
    // diaresis
    if (acc1 == '"' && acc2 == '=') return 1;
    if (acc2 == '"' && acc1 == '=') return 1;
    if (acc1 == '"' && acc2 == 'v') return 3;
    if (acc2 == '"' && acc1 == 'v') return 3;
    if (acc1 == '"' && acc2 == '~') return 27;
    if (acc2 == '"' && acc1 == '~') return 27;
    // other
    if (acc1 == '=' && acc2 == '~') return 6;
    if (acc2 == '=' && acc1 == '~') return 6;
    if (acc1 == 'k' && acc2 == '=') return 7;
    if (acc2 == 'k' && acc1 == '=') return 7;
    if (acc1 == 'd' && acc2 == '=') return 14;
    if (acc2 == 'd' && acc1 == '=') return 14;
    if (acc1 == 'u' && acc2 == 'c') return 26;
    if (acc2 == 'u' && acc1 == 'c') return 26;
    if (acc1 == 'H' && acc2 == 'h') return 28;
    if (acc2 == 'H' && acc1 == 'h') return 28;
    if (acc1 == 'H' && acc2 == '~') return 29;
    if (acc2 == 'H' && acc1 == '~') return 29;
    if (acc1 == 'H' && acc2 == 'd') return 30;
    if (acc2 == 'H' && acc1 == 'd') return 30;
    if (acc1 == 'u' && acc2 == 'h') return 31;
    if (acc2 == 'u' && acc1 == 'h') return 31;
    if (acc1 == 'u' && acc2 == '~') return 32;
    if (acc2 == 'u' && acc1 == '~') return 32;
    if (acc1 == 'u' && acc2 == 'd') return 33;
    if (acc2 == 'u' && acc1 == 'd') return 33;
    return 0;
}

// Fetches the position of a double accent on capital O
auto accent_ns::double_o_accent(int acc3) -> unsigned {
    switch (acc3) {
    case 1: return Odiamacro_cc;
    case 5: return Odotmacro_cc;
    case 6: return Otilmacro_cc;
    case 7: return Oogonmacron_cc;
    case 9: return Otildeaigu_cc;
    case 10: return Obaraigu_cc;
    case 12: return Ociracute_cc;
    case 13: return Obargrave_cc;
    case 16: return Oacuteyy_cc;
    case 18: return Ograveyy_cc;
    case 21: return Ociracute_cc;
    case 15: return Ocirgrave_cc;
    case 23: return Ocirxx_cc;
    case 24: return Ocirtilde_cc;
    case 25: return Ocirdot_cc;
    case 27: return Otildedp_cc;
    case 28: return Oxxyy_cc;
    case 29: return Otildeyy_cc;
    case 30: return Odotyy_cc;
    default: return unused_accent_even_cc;
    }
}

// Fetches the position of a double accent on capital E
auto accent_ns::double_e_accent(int acc3) -> unsigned {
    if (acc3 == 10) return Ebaracute_cc;
    if (acc3 == 12) return Eciracute_cc;
    if (acc3 == 13) return Ebargrave_cc;
    if (acc3 == 26) return Ecedrond_cc;
    if (acc3 == 15) return Ecirgrave_cc;
    if (acc3 == 23) return Ecirxx_cc;
    if (acc3 == 24) return Ecirtilde_cc;
    if (acc3 == 25) return Ecirdot_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on capital A
auto accent_ns::double_a_accent(int acc3) -> unsigned {
    if (acc3 == 1) return Adiamacro_cc;
    if (acc3 == 5) return Adotmacro_cc;
    if (acc3 == 12) return Aciracute_cc;
    if (acc3 == 15) return Acirgrave_cc;
    if (acc3 == 25) return Acirdot_cc;
    if (acc3 == 23) return Acirxx_cc;
    if (acc3 == 24) return Acirtilde_cc;
    if (acc3 == 34) return Abreveacute_cc;
    if (acc3 == 22) return Abrevegrave_cc;
    if (acc3 == 31) return Abrevexx_cc;
    if (acc3 == 32) return Abrevetilde_cc;
    if (acc3 == 33) return Abrevedot_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on capital U
auto accent_ns::double_u_accent(int acc3) -> unsigned {
    if (acc3 == 1) return Udiamacro_cc;
    if (acc3 == 2) return Udiaacute_cc;
    if (acc3 == 3) return Udiacaron_cc;
    if (acc3 == 4) return Udiagrave_cc;
    if (acc3 == 9) return Utildeacute_cc;
    if (acc3 == 16) return Uacuteyy_cc;
    if (acc3 == 18) return Ugraveyy_cc;
    if (acc3 == 28) return Uxxyy_cc;
    if (acc3 == 29) return Utildeyy_cc;
    if (acc3 == 30) return Udotyy_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on a, not in the list above
auto accent_ns::double_other_accent(int a, int acc3) -> unsigned {
    if (a == 'I') {
        if (acc3 == 2) return Itremaacute_cc;
    }
    if (a == 'S') {
        if (acc3 == 11) return Sdotacute_cc;
        if (acc3 == 19) return Sdotcaron_cc;
        if (acc3 == 20) return Sdotdot_cc;
    }
    if (a == 'C') {
        if (acc3 == 17) return Ccedilacute_cc;
    }
    if (a == 'L') {
        if (acc3 == 14) return Ldotbar_cc;
    }
    if (a == 'R') {
        if (acc3 == 14) return Rdotbar_cc;
    }
    if ('a' <= a && a <= 'z') return double_other_accent(a - 'a' + 'A', acc3) + 1;
    return unused_accent_even_cc;
}

// This implements \a', or \a{'}, expansion is \', empty in case of an error
// cur_tok is \a, in case of error
void Parser::E_accent_a() {
    if (tracing_macros()) the_log << lg_startbrace << cur_tok << lg_endbrace;
    TokenList y = read_arg();
    if (!token_ns::has_a_single_token(y)) {
        parse_error("wanted a single token as argument to \\a");
        return;
    }
    Token t = token_ns::get_unique(y);
    if (!t.is_a_char()) { // latex says \csname\string #1 \endcsname
        parse_error(err_tok, "Bad syntax of \\a, argument not a character ", t, "", "bad accent");
        return;
    }
    auto Y = Token(t.chr_val() + single_offset);
    token_from_list(Y);
    if (cur_cmd_chr.cmd != accent_cmd) {
        parse_error(err_tok, "Bad syntax of \\a, argument is ", t, "", "bad accent");
        return;
    }
    back_input();
}

// removes some useless braces.
void accent_ns::special_acc_hack(TokenList &y) {
    if (y.empty()) return;
    if (y.front().is_space_token()) y.pop_front();
    if (y.empty()) return;
    Token T = y.front();
    if (!T.is_OB_token()) return;
    y.pop_front();
    if (y.empty()) return; // this should not happen
    Token T2 = y.front();
    y.pop_front();
    if (y.empty()) return; // this should not happen
    if (y.front().is_CB_token()) {
        y.pop_front();
        y.push_front(T2);
    } else {
        y.push_front(T2);
        y.push_front(T);
    }
}

auto accent_ns::special_double_acc(int chr, int acc) -> Token {
    if (acc == '`' && chr == '*') return special_double[0];
    if (acc == '`' && chr == '.') return special_double[1];
    if (acc == '\'' && chr == '*') return special_double[2];
    if (acc == '\'' && chr == '=') return special_double[3];
    if (acc == '^' && chr == '*') return special_double[4];
    if (acc == '^' && chr == '.') return special_double[5];
    if (acc == '~' && chr == '*') return special_double[6];
    if (acc == '~' && chr == '.') return special_double[7];
    if (acc == '"' && chr == '*') return special_double[8];
    if (acc == 'H' && chr == '*') return special_double[9];
    if (acc == 'r' && chr == '*') return special_double[10];
    if (acc == 'r' && chr == '=') return special_double[11];
    if (acc == 'v' && chr == '*') return special_double[12];
    if (acc == 'v' && chr == '\'') return special_double[13];
    if (acc == 'u' && chr == '*') return special_double[14];
    if (acc == 'u' && chr == '=') return special_double[15];
    if (acc == '=' && chr == '*') return special_double[16];
    if (acc == '.' && chr == '*') return special_double[17];
    if (acc == '.' && chr == '\'') return special_double[18];
    if (acc == 't' && chr == '*') return special_double[19];
    return Token();
}

// This implements \'e, or \'{a} or \a{'}{e}, \href{\~grimm}
// or \'{\= u}, a double accent
// cur_tok, cur_cmd_chr explain what to do.
void Parser::E_accent() {
    if (tracing_macros()) the_log << lg_startbrace << "accent " << cur_tok << lg_endbrace;
    int acc_code = cur_cmd_chr.chr;
    if (global_in_url && acc_code == '~') {
        if (tracing_macros()) { the_log << lg_startbrace << "\\~ gives ~ " << lg_endbrace; }
        back_input(Token(other_t_offset, '~'));
        return;
    }
    String msg1 = "Error in accent, command = ";
    String msg2 = "bad accent";
    Token  tfe  = cur_tok;
    // Fetch the accent
    TokenList y = read_arg();
    if (y.empty()) {
        err_buf << bf_reset << msg1 << tfe.tok_to_str();
        err_buf << (acc_code == '~' ? "\n\\~{} is the wrong way to put a tilde in an URL" : "\nThings like {\\'{}} are a bit strange");
        signal_error(err_tok, msg2);
        return;
    }
    int   acc_code2 = 0; // will be set in the double acc case.
    Token tfe2;          // Second token for error.
    if (y.size() >= 2) {
        token_from_list(y.front()); // Get the meaning of the start of y
        // case \'{\a'e};
        if (cur_cmd_chr.cmd == a_cmd) {
            y.pop_front(); // discard the \a
            accent_ns::special_acc_hack(y);
            if (y.empty()) {
                parse_error("Internal error, lost argument of \\a");
                return;
            }
            Token t = y.front(); // get the '
            if (!t.is_a_char()) {
                parse_error(err_tok, "Bad syntax of \\a, argument not a character ", t, "", msg2);
                return;
            }
            y.pop_front(); // discard the accent
            tfe2 = Token(t.chr_val() + single_offset);
            token_from_list(tfe2);
            if (cur_cmd_chr.cmd != accent_cmd) {
                parse_error(err_tok, "Bad syntax of \\a,  argument is ", t, "", msg2);
                return;
            }
        } else if (cur_cmd_chr.cmd == accent_cmd) {
            tfe2 = cur_tok;
            y.pop_front();
        } else {
            parse_error(err_tok, msg1, tfe, "\nWanted a single token", msg2);
            return;
        }
        acc_code2 = cur_cmd_chr.chr;
        accent_ns::special_acc_hack(y);
        if (y.size() != 1) {
            parse_error(err_tok, msg1, tfe, "\nWanted a single token", msg2);
            return;
        }
    }
    Token Y = token_ns::get_unique(y);
    token_from_list(Y);
    unsigned int achar = cur_cmd_chr.chr;
    if (achar <= 8)
        achar = 8; // make these invalid
    else if (achar == 0xc6)
        achar = 1;
    else if (achar == 0xe6)
        achar = 2;
    else if (achar == 0xc5)
        achar = 3;
    else if (achar == 0xe5)
        achar = 4;
    else if (achar == 0xd8)
        achar = 5;
    else if (achar == 0xf8)
        achar = 6;
    if (cur_cmd_chr.cmd == cst1_cmd && achar == i_code)
        achar = 'i';
    else if ((cur_cmd_chr.cmd == specchar_cmd) || cur_cmd_chr.is_letter_other()) {
    } else {
        err_buf << bf_reset << msg1;
        err_buf << tfe.tok_to_str();
        if (acc_code2 != 0) err_buf << tfe2.tok_to_str();
        if (Y.is_invalid())
            err_buf << "\nEnd of data reached while scanning argument";
        else if (!cur_tok.not_a_cmd())
            err_buf << "\nLetter needed instead of " << cur_tok.tok_to_str();
        else
            err_buf << "\nLetter needed";
        signal_error(err_tok, msg2);
        return;
    }
    Token res;
    bool  spec = false;
    if (achar < nb_accents) {
        if (acc_code2 != 0) {
            int acc3_code = accent_ns::combine_accents(acc_code, acc_code2);
            res           = accent_ns::fetch_double_accent(to_signed(achar), acc3_code);
        } else {
            res = accent_ns::fetch_accent(achar, acc_code);
            if (res.is_null() && achar < 128) {
                if (is_letter(static_cast<char>(achar))) {
                    spec = true; // T. Bouche veut une erreur
                    res  = accent_ns::fetch_accent(0, acc_code);
                } else
                    res = accent_ns::special_double_acc(to_signed(achar), acc_code);
            }
        }
    }
    if (res.is_null()) {
        String s =
            achar >= 128
                ? "a non 7-bit character"
                : is_letter(static_cast<char>(achar)) ? "letter" : is_digit(static_cast<char>(achar)) ? "digit" : "non-letter character";
        err_buf << bf_reset << msg1;
        err_buf << tfe.tok_to_str();
        if (acc_code2 != 0) err_buf << tfe2.tok_to_str();
        err_buf << "\nCannot put accent on " << s;
        if (achar < 128) err_buf << " " << char(achar);
        signal_error(err_tok, msg2);
        return;
    }
    TokenList expansion;
    expansion.push_back(res);
    if (spec) {
        expansion.push_front(Y);
        brace_me(expansion);
        expansion.push_front(hash_table.composite_token);
    }

    if (tracing_macros()) { the_log << lg_startbrace << "accent on " << uchar(achar) << " -> " << expansion << lg_endbrace; }
    back_input(expansion);
}
