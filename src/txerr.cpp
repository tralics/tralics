// Tralics, a LaTeX to XML translator.
// Error handler for tralics
// Copyright INRIA/apics/marelle (Jose Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <fmt/format.h>
#include <fmt/ostream.h>

using namespace std::string_literals;

namespace {
    auto convert_for_xml_err(Token T) -> std::string {
        if (T.is_null()) return "\\invalid.";

        if (T.char_or_active()) {
            char32_t c = T.char_val();
            if (c == 0) return "^^@";
            Buffer B;
            B.push_back_real_utf8(c); // \todo without Buffer
            return std::move(B);
        }

        if (T.active_or_single()) {
            char32_t c = T.char_val();
            if (c == 0) return "\\^^@";
            Buffer B("\\");
            B.push_back_real_utf8(c);
            return std::move(B);
        }

        if (T.is_in_hash()) {
            auto s = the_parser.hash_table[T.hash_loc()];
            if (std::none_of(s.begin(), s.end(), [](char c) { return c == '<' || c == '>' || c == '&' || c < 32; })) return "\\" + s;

            Buffer B("\\");
            for (auto c : s) B.push_back_xml_char(c);
            return std::move(B);
        }

        return "\\csname\\endcsname";
    }
} // namespace

namespace err_ns {
    void convert_to_string(const TokenList &L);
} // namespace err_ns

void err_ns::fatal_error(String s) {
    std::cout << "Fatal error " << s << "\n";
    abort();
}

// The error mechanism is as follows: we put the error message in a buffer
// then use the following function to signal it.

void Parser::signal_error() {
    int         line = get_cur_line();
    std::string file = get_cur_filename();
    nb_errs++;
    flush_buffer();
    Logger::finish_seq();
    if (file.empty())
        spdlog::error("on line {}: {}", line, err_buf);
    else
        spdlog::error("{}:{} {}", file, line, err_buf);
    if (nb_errs >= 5000) {
        spdlog::critical("Translation aborted: Too many errors, aborting.");
        Logger::log_finish();
        exit(1);
    }
}

// If T is non-null and the option -noxmlerror is not given,
// then an error element with value s name T is inserted in the XML result

void Parser::signal_error(Token T, const std::string &s) {
    signal_error();
    if (no_xml_error) return;
    if (T.is_null()) return;
    auto str = std::string(s);
    the_stack.add_newid0("error");
    std::string cmd = convert_for_xml_err(T);
    the_stack.add_att_to_last(the_names["c"], str);
    the_stack.add_att_to_last(the_names["l"], cur_line_to_istring());
    the_stack.add_att_to_last(the_names["n"], cmd);
}

// identical to  ostream& operator<<(ostream&fp, const TokenList& L)
void err_ns::convert_to_string(const TokenList &L) {
    Buffer &      B = err_buf;
    static Buffer buffer_for_log;
    auto          C = L.begin();
    auto          E = L.end();
    while (C != E) {
        buffer_for_log.clear();
        if (buffer_for_log.push_back(*C)) buffer_for_log << ' ';
        B += buffer_for_log;
        ++C;
    }
}

// The simpliest error function (adds nothing to the XML)
void Parser::signal_error(const std::string &s) {
    err_buf = s;
    signal_error(Token(), s);
}

// Some generic errors (may add s to the XML)
void Parser::parse_error(Token T, const std::string &s) {
    err_buf = s;
    signal_error(T, s);
}

void Parser::parse_error(const std::string &s) {
    err_buf = s;
    signal_error(err_tok, s);
}

void Parser::parse_error(Token T, const std::string &s1, const std::string &s2) {
    err_buf = s1;
    signal_error(T, s2);
}

void Parser::parse_error(Token T, const std::string &s, TokenList &L) {
    err_buf = s;
    err_ns::convert_to_string(L);
    signal_error(T, s);
}

void Parser::parse_error(Token T, const std::string &s1, const std::string &s2, const std::string &s3) {
    err_buf = s1 + s2;
    signal_error(T, s3);
}

/// Useful error. Here s2 is a token.
void Parser::parse_error(Token T, const std::string &s1, Token s2, const std::string &s3, const std::string &s4) {
    err_buf = s1 + s2.tok_to_str() + s3;
    signal_error(T, s4);
}

// --------------------------------------------------

// May be signaled in case \end{verbatim} is missing
void Parser::bad_end_env(int cl) {
    err_buf = fmt::format("End of data reached while scanning end of environment {}\nscanning started line {}", get_cur_env_name(), cl);
    signal_error(err_tok, "Bad end env");
}

// Error signaled if no closing brace is found
void Parser::missing_close_brace(int cl) {
    err_buf = "End of data reached while scanning a group;\n";
    err_buf.format("scanning started line {}\nscanning argument of {}", cl, err_tok);
    signal_error(err_tok, "Missing close brace");
}

// Error signaled if EOF found while scanning a delimited argument
void Parser::bad_delimited(int cl, Token x) {
    err_buf = fmt::format("End of data reached while scanning argument of {}\n", err_tok);
    err_buf.format("scanning started at line {}", cl);
    if (!x.is_null()) err_buf += "\nexpected " + x.tok_to_str();
    signal_error(err_tok, "bad macro");
}

void Parser::err_one_arg(const TokenList &L) {
    err_buf = fmt::format("The command {} takes one token as argument instead of ", err_tok, L.size());
    if (!L.empty()) {
        err_buf += "\nargument is: ";
        err_buf << L;
    }
    signal_error(err_tok, "onearg");
}

// Case of \newcommand{\foo\bar}. Should use the previous code
void Parser::get_new_command_aux(const TokenList &a) {
    err_buf = fmt::format("Only one token allowed in argument list of {}\n", err_tok);
    err_ns::convert_to_string(a);
    signal_error(err_tok, "bad argument");
}

void Parser::runaway(int cl) {
    if (cl == -1)
        err_buf = "End of data reached while scanning start of argument";
    else {
        err_buf = "Runaway argument?\nParagraph ended while scanning an argument of " + err_tok.tok_to_str();
        if ((cl != 0) && cl != get_cur_line()) err_buf.format(";\nscanning started on line {}", cl);
    }
    signal_error(err_tok, "Runaway argument");
}

// May be signaled in the case of \hbox \foo.
void Parser::missing_open_brace() {
    err_buf = "A left brace was mandatory here, so I've put one in.\nArgument was replaced by {}";
    signal_error(err_tok, "Missing open brace");
}

// Error signaled in case of \end{\foo},
void Parser::bad_group_char() {
    err_buf = "Non-character found in environment name:\n" + cur_tok.tok_to_str() +
              "\nArgument was replaced by {}\nThe unread closing brace may signal an error";
    signal_error(err_tok, "bad char");
}

// Error signaled when pass_test sees end of data.
void Parser::lost_if(Token T, int l) {
    err_buf = fmt::format("End of data reached while scanning \\fi\nfor condition started on line {}", l);
    signal_error(T, "lostif");
}

void Parser::math_only() {
    if (is_pos_par(nomath_code)) {
        LC();
        unprocessed_xml << cur_tok;
        return;
    }
    parse_error(cur_tok, "Math only command ", cur_tok, "", "Mathonly command");
    static bool first_time = true;
    if (first_time) {
        spdlog::warn("(Contrarily to TeX, Tralics does not switch to math mode in such a case.)");
        first_time = false;
    }
}

void Parser::bad_nbargs(int k) {
    std::string S;
    if (k == -1) S = "Only one token allowed";
    if (k == -2) S = "Commands are not allowed";
    if (k == -3) S = "Argument should be a digit";
    err_buf = "Bad syntax in optional argument\n" + S + "\nI will assume that the command takes no argument";
    signal_error(err_tok, "Bad optional in \\newcommand");
}

void Parser::extra_fi_or_else() {
    err_buf = fmt::format("Extra {}\nI'm ignoring this; it doesn't match any \\if", cur_tok);
    signal_error(cur_tok, "extra fi or else");
}

// Case of \def\foo#2{}
void Parser::bad_definition(Token name, size_t nb) {
    err_buf = fmt::format("Error while scanning definition of {}\n", name);
    err_buf.format("got #{}, expected #{}", cur_tok, nb + 1);
    signal_error(name, "bad char after #");
}

void Parser::bad_csname(bool orig) {
    if (cur_tok.is_invalid())
        err_buf = "End of data reached while expecting \\endcsname";
    else {
        err_buf = "Invalid token " + cur_tok.tok_to_str();
        if (orig)
            err_buf += "\nfound between \\csname and \\endcsname";
        else
            err_buf += " found while scanning " + err_tok.tok_to_str();
    }
    signal_error(err_tok, "missing endcsname");
}

void Parser::bad_counter0() {
    err_buf = "Invalid token " + cur_tok.tok_to_str() + " found in counter name";
    signal_error(err_tok, "invalid cmd");
}

void Parser::bad_counter1(const Buffer &B, EqtbCmdChr &E) {
    err_buf = (E.val.is_undef() ? "Unknown counter `"s : "Invalid counter `"s) + B.substr(2) + "'";
    signal_error(err_tok, "bad counter");
}

// This is a hack. In some cases, we define the command after signaling
// the error. The hack is that no <error/> element must be created.
void Parser::undefined_mac() {
    bool noxml = the_main->no_undef_mac;
    err_buf    = "Undefined command " + cur_tok.tok_to_str();
    if (!cur_cmd_chr.is_undef()) err_buf.format("; command code = {}", cur_cmd_chr.cmd);
    if (noxml) {
        signal_error(Token(), "Undefined command");
        eq_define(cur_tok.eqtb_loc(), CmdChr(self_insert_cmd, zero_code), true);
        back_input(cur_tok);
    } else
        signal_error(cur_tok, "Undefined command");
}

void Parser::undefined_env(const std::string &s) {
    err_buf = "Undefined environment " + s;
    signal_error(cur_tok, "Undefined environment");
}

// This is executed when vertical mode is assumed. Current paragraph is ended
void Parser::wrong_mode(const std::string &s) {
    err_buf = s + "\nThe following command is not in outer vertical mode: " + cur_tok.tok_to_str();
    signal_error(cur_tok, "Not in outer vertical");
    leave_h_mode();
}

// This is called in case of \global\hbox
void Parser::prefix_error(bool b_global, symcodes K) {
    err_buf = "You cannot use the prefix ";
    err_buf.dump_prefix(true, b_global, K);
    err_buf += " before " + cur_tok.tok_to_str();
    signal_error(err_tok, "bad prefix");
}

void Parser::improper_alpha() { parse_error(err_tok, "Improper alphabetic constant ", cur_tok, "", "bad alpha"); }

void Parser::wrong_pop(Token T, const std::string &a, const std::string &b) {
    err_buf = fmt::format("Wrong group delimiter\ngot `{}', expected `{}'", a, b);
    signal_error(T, "wrong pop");
}

void Parser::extra_close_brace(int cl) {
    err_buf = "Extra closing brace\n";
    err_buf.format("scanning started at line {}\nfor argument to ", cl, err_tok);
    signal_error(err_tok, "extra close brace");
}

void Parser::bad_macro_prefix(Token x, Token C) {
    err_buf = "Use of " + err_tok.tok_to_str();
    err_buf += " doesn't match its definition;\ngot " + x.tok_to_str() + ", expected ";
    err_buf += C.tok_to_str();
    signal_error(err_tok, "bad prefix");
}

void Parser::invalid_key(Token T, const std::string &key, const TokenList &val) {
    err_buf = fmt::format("Invalid key in {} {} = {}", T, key, val);
    signal_error(T, "invalid key");
}

void Parser::verb_error(Token T, int msg) {
    std::string s;
    switch (msg) {
    case 0: s = "in argument of a command"; break;
    case 1: s = "at end of line"; break;
    case 2: s = "terminated by end of line"; break;
    default: s = ": no delimiter found";
    }
    err_buf = T.tok_to_str() + " " + s;
    signal_error(T, "bad \\verb");
}

void Parser::missing_equals(Token T) {
    String s = "Missing = inserted for comparison ";
    parse_error(T, s, cur_tok, "", s);
    back_input(cur_tok);
}

void Parser::short_verb_error(Token Tfe, Token t, int x) {
    err_buf = Tfe.tok_to_str() + ':';
    if (x == 3)
        err_buf.format("Not a short verb {}", t);
    else if (x == 2)
        err_buf.format("Already a short verb {}", t);
    else if (t.is_null())
        err_buf += "One argument required";
    else
        err_buf.format("A one-char control sequence is needed instead of {}", t);
    signal_error(Tfe, "Bad argument to Define/Undefine ShortVerb");
}

void Parser::fp_parse_error(Token a, Token b) {
    err_buf = "Non digit found " + a.tok_to_str();
    err_buf += "\nwhile scanning argument of " + b.tok_to_str();
    signal_error(b, "nondigit");
}

void Parser::counter_overflow(Token T, long n, int nmax) {
    err_buf = fmt::format("Illegal counter value {} for {}\n", n, T);
    if (n <= 0)
        err_buf += "Value must be positive";
    else
        err_buf.format("Value must be at most {}", nmax);
    signal_error(T, "counter overflow");
}

void Parser::bad_redefinition(int rd, Token T) {
    err_buf = fmt::format("{}: Cannot define {}; token is {}", err_tok, T,
                          (rd == 1 ? "undefined" : rd == 0 ? "already defined" : "not a command"));
    signal_error(err_tok, "bad definition");
}

void Parser::multiple_label(const std::string &name, int l, const std::string &f) {
    err_buf = fmt::format("Label '{}' multiply defined (first use line {} file {})", name, l, f);
    signal_error(hash_table.label_token, "already defined label");
}

// Called when unprocessed_xml is not empty.
void Parser::missing_flush() {
    auto k = unprocessed_xml.size();
    while (k > 0 && unprocessed_xml.is_spaceh(k - 1)) k--;
    if (k == 0) return;
    err_buf = "Internal error, non-empty buffer \n" + unprocessed_xml + "\nSome text may be lost";
    signal_error(Token(), "non-empty buffer");
}

void Parser::signal_ovf(Token T, String h, long cur, long max) {
    if (h != nullptr) err_buf = h;
    err_buf.format("{} wants 0<=N<={}, with N={}", T, max, cur);
    signal_error(T, "number too big");
}

// Error signaled when a token list in seen a number expected
void Parser::bad_number() {
    err_buf = fmt::format("Missing number, treated as zero\nfound token list {} while scanning {}", cur_tok, err_tok);
    signal_error(err_tok, "Missing number");
    cur_val.set_dim(0);
}

void Parser::bad_number1(Buffer &B) {
    err_buf = fmt::format("Missing number, treated as zero\nfound box name '{}' while scanning {}", B, err_tok);
    signal_error(err_tok, "Missing number");
    cur_val.set_dim(0);
}

void Parser::missing_number() {
    err_buf = "Missing number, treated as zero\n";
    if (cur_tok.is_valid()) err_buf.format("found  {} ", cur_tok);
    err_buf.format("while scanning {}", err_tok);
    signal_error(err_tok, "Missing number");
}

// Error signaled for bad mu
void Parser::mu_error(String s, int i) {
    err_buf = "Incompatible glue units in " + err_tok.tok_to_str() + "\n" + s;
    switch (i) {
    case it_int: err_buf += "integer"; break;
    case it_dimen: err_buf += "dimension"; break;
    case it_glue: err_buf += "glue"; break;
    case it_mu: err_buf += "muglue"; break;
    case it_ident: err_buf += "font indentifier"; break;
    case it_tok: err_buf += "token"; break;
    }
    signal_error(err_tok, "Incompatible glue units");
}
