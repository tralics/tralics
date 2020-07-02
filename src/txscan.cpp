// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2003, 2004-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains the TeX scanner of tralics

#include "tralics/Saver.h"
#include "txinline.h"
#include "txmath.h"
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {
    Buffer                                      scratch;                  // See insert_string
    TexFonts                                    tfonts;                   // the font table
    std::vector<InputStack *>                   cur_input_stack;          // the input streams
    bool                                        name_in_progress = false; // see the source of TeX
    std::array<FileForInput, nb_input_channels> tex_input_files;          // the input files
    bool                                        scan_glue_opt = false;    // true if optional glue as found
    std::array<std::vector<long>, 4>            penalties;
    std::vector<ScaledInt>                      parshape_vector;
    bool                                        every_eof = false;  // true if every_eof can been inserted for the current file
    Buffer                                      local_buf;          // a local buffer
    bool                                        require_eof = true; // eof is an outer token

    auto find_no_path(const std::string &s)
        -> std::optional<std::filesystem::path> { // \todo is that just file_exists, or is the side-effect necessary?
        if (s.empty()) return {};
        if (tralics_ns::file_exists(s)) return s;
        return {};
    }

    void set_math_char(uchar c, size_t f, std::string s) { math_chars[c][f] = std::move(s); }
} // namespace

namespace io_ns {
    void set_enc_param(long enc, long pos, long v);
    auto get_enc_param(long enc, long pos) -> long;
} // namespace io_ns

// Section 1: output streams.

// max_openin=max_openout=15 is nb_input_channels -1
// There are 3 pseudo output channels 16, 17, 18, always closed
// and no file attached to them

TexOutStream::TexOutStream() {
    for (bool &i : write_open) i = false;
}

// This closes an output channel.
void TexOutStream::close(size_t chan) {
    if ((chan <= max_openout) && write_open[chan]) {
        write_file[chan].close();
        write_open[chan] = false;
    }
}

// This opens an output channel.
// What if the file cannot be opened ?
void TexOutStream::open(size_t chan, const std::string &fn) {
    if (chan < 0 || chan > max_openout) return; // This cannot happen
    write_file[chan] = std::ofstream(tralics_ns::get_out_dir(fn));
    write_open[chan] = true;
}

// This gets the object to \write in local_buf.
// A new line is added, except if chan is 18 or 19
// Since version 2.15.4, \message print a newline
// In the case of \write18, the string is printed on the log file
auto Parser::string_to_write(long chan) -> std::string {
    TokenList L = scan_general_text();
    read_toks_edef(L);
    Buffer &B = local_buf;
    B.clear();
    B << L;
    if (chan < write18_slot) B << "\n";
    auto res = B.convert_to_log_encoding();
    if (chan == write18_slot) {
        Logger::finish_seq();
        the_log << "\\write18=" << res << "\n";
    }
    return res;
}

// This implements \write, \openout, and \closeout
// and \iow_term:x \iow_log:x of latex3
// action is \immediate, since there is no shipout routine
void Parser::M_extension(int cc) {
    static TexOutStream tex_out_stream; // the output streams
    long                chan = 0;
    if (cc == openout_code)
        chan = to_signed(scan_int(cur_tok, max_openout, "output channel number"));
    else if (cc == write_log_code || cc == wlog_code)
        chan = negative_out_slot;
    else if (cc == write_term_code || cc == typeout_code)
        chan = positive_out_slot;
    else {
        chan = scan_int(cur_tok);
        if (chan < 0)
            chan = negative_out_slot;
        else if (chan > max_openout && chan != write18_slot)
            chan = positive_out_slot;
        if (chan == write18_slot && !the_main->shell_escape_allowed) chan = positive_out_slot;
    }
    auto uchan = to_unsigned(chan);
    if (cc == openout_code) {
        scan_optional_equals();
        tex_out_stream.open(uchan, scan_file_name());
    } else if (cc == closeout_code)
        tex_out_stream.close(uchan);
    else { // write to uchan
        auto s = string_to_write(chan);
        if (uchan == write18_slot)
            system(s.c_str());
        else if (tex_out_stream.is_open(uchan))
            tex_out_stream.write(uchan, s);
        else if (uchan == negative_out_slot)
            the_log << s;
        else
            log_and_tty << s;
    }
}

// Section 2. input file handling

// This closes a file opened by \openin
void FileForInput::close() {
    if (is_open) {
        is_open = false;
        lines.before_close(false);
        lines.clear();
    }
}

// This implements \ifeof
auto Parser::is_input_open() -> bool {
    auto ch = scan_int(cur_tok, max_openin, "input channel number");
    return !tex_input_files[ch].is_open;
}

// Open a file for \openin. if action is false, the file does not exist
// No on-the fly conversion here
void FileForInput::open(const std::string &file, const std::filesystem::path &fn, bool action) {
    if (!action) {
        Logger::finish_seq();
        the_log << "++ Cannot open file " << file << " for input\n";
    } else {
        tralics_ns::read_a_file(lines, fn, 1);
        is_open = true;
        cur_line.clear();
        line_no = 0;
        lines.after_open();
    }
}

// This puts in cur_input_stack a new slot, containing the current state
// of affairs. It kills the input buffer and the current line.
void Parser::push_input_stack(const std::string &name, bool restore_at, bool re) {
    auto *W = new InputStack(name, get_cur_line(), state, cur_file_pos, every_eof, require_eof);
    cur_input_stack.push_back(W);
    auto n = cur_input_stack.size();
    if (tracing_io()) {
        Logger::finish_seq();
        the_log << "++ Input stack ++ " << n << " " << W->name << "\n";
    }
    W->set_line_ptr(lines);
    W->line_pos = input_line_pos;
    W->line     = input_line;
    W->TL.swap(TL);
    if (restore_at) {
        W->at_val                      = eqtb_int_table[uchar('@')].val;
        eqtb_int_table[uchar('@')].val = 11;
        Logger::finish_seq();
        the_log << "++ Made @ a letter\n";
    }
    every_eof   = false; // might be set to true
    require_eof = re;
    input_line.clear();
    lines.set_interactive(false);
    input_line_pos = 0;
    cur_file_pos   = 0;
}

// Undoes push_input_stack.
// If vb is false, we are reading from a token list or something like that.
// Otherwise, we read from a real file, and indicate that it is closed.
void Parser::pop_input_stack(bool vb) {
    auto n = cur_input_stack.size();
    if (n == 0) {
        if (tracing_io()) {
            Logger::finish_seq();
            the_log << "++ Input stack empty\n";
        }
        return;
    }
    InputStack *W = cur_input_stack.back();
    if (vb) lines.before_close(true);
    set_cur_line(W->line_no);
    state = W->state;
    W->get_line_ptr(lines);
    require_eof = W->eof_outer;
    auto at     = W->at_val;
    if (at >= 0) {
        eqtb_int_table[uchar('@')].val = at;
        if (tracing_io()) {
            Logger::finish_seq();
            the_log << "++ Catcode of @ restored to " << at << "\n";
        }
    }
    input_line.clear();
    input_line.insert(input_line.end(), W->line.begin(), W->line.end());
    input_line_pos = W->line_pos;
    back_input(W->TL);
    if (cur_file_pos != 0) insert_hook(cur_file_pos);
    cur_file_pos = W->file_pos;
    every_eof    = W->every_eof;
    Logger::finish_seq();
    the_log << "++ cur_file_pos restored to " << cur_file_pos << "\n";
    cur_input_stack.pop_back();
    if (tracing_io()) {
        Logger::finish_seq();
        the_log << "++ Input stack -- " << n << " " << W->name << "\n";
    }
    delete W;
}

// Make sure no character can be obtained from the stream
void InputStack::destroy() {
    TL.clear();
    line.clear();
    L.clear();
}

// This kills all pending input
void Parser::close_all() {
    if (tracing_io()) {
        Logger::finish_seq();
        the_log << "++ close all files\n";
    }
    while (!cur_input_stack.empty()) pop_input_stack(true);
    TL.clear();
    input_line.clear();
    input_line_pos = 0;
    lines.clear();
    lines.set_interactive(false);
}

// A file name is a special thing in TeX.
// We read until we find a non-char, or a space.
auto Parser::scan_file_name() -> std::string {
    static Buffer file_name;

    if (name_in_progress) return "sabotage!"; // recursion killer.
    name_in_progress = true;
    remove_initial_space_and_back_input();
    file_name.clear();
    for (;;) {
        if (get_x_token()) break;
        if (cur_cmd_chr.is_letter_other())
            file_name.push_back(cur_cmd_chr.char_val());
        else if (cur_cmd_chr.cmd == underscore_catcode) // allow foo_bar
            file_name.push_back(cur_cmd_chr.char_val());
        else if (cur_cmd_chr.is_space())
            break;
        else {
            back_input();
            break;
        }
    }
    name_in_progress = false;
    return file_name;
}

// This implements \endinput, \scantokens
// and \input \Input, \include \readxml
// This is called from expand. We have to print the command name in the log
// file. The real code of input is in T_input().
void Parser::E_input(int q) {
    if (q == endinput_code) {
        if (tracing_commands()) Logger::log_dump("endinput");
        force_eof = true;
        return;
    }
    if (q == end_all_input_code) {
        if (tracing_commands()) Logger::log_dump("endallinput");
        for (auto &i : cur_input_stack) i->destroy();
        force_eof = true;
        return;
    }
    if (q == scantokens_code) {
        if (tracing_commands()) Logger::log_dump("scantokens");
        TokenList L = scan_general_text();
        T_scantokens(L);
        return;
    }
    if (name_in_progress) {
        insert_relax();
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{insert \\relax for \\input}\n";
        }
        return;
    }
    if (tracing_commands()) Logger::log_dump("input");
    T_input(q);
}

void Parser::T_scantokens(TokenList &L) {
    static Buffer B;
    B.clear();
    B << L;
    push_input_stack("(scantokens)", false, true);
    lines.split_string(B, 0);
    lines.after_open();
    every_eof = true;
}

// In TeX, you say \input foo, in latex you say \input{foo}
// This accepts both syntaxes.
auto Parser::latex_input(int q) -> std::string {
    remove_initial_space_and_back_input();
    std::string file;
    if (q == include_code || cur_tok.is_OB_token()) {
        flush_buffer();
        if (!cur_tok.is_OB_token()) {
            parse_error(err_tok, "Missing {} for ", err_tok, "", "Missing brace");
            file = ".tex";
        } else {
            auto guard = InLoadHandler();
            file       = sT_arg_nopar();
        }
    } else
        file = scan_file_name();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\input " << file << "}\n";
    }
    return file;
}

// This implements \IfFileExists, \InputIfFileExists,
//  \input, \include, \openin, \closein
void Parser::T_input(int q) {
    size_t stream    = 0;
    bool   seen_star = false;
    bool   seen_plus = false;
    Token  T         = cur_tok;
    if (q == openin_code || q == closein_code) {
        stream = scan_int(cur_tok, max_openin, "input channel number");
        tex_input_files[stream].close();
    }
    if (q == closein_code) return;
    std::string file;
    if (q == Input_code || q == include_code || q == inputifexists_code) seen_star = remove_initial_star();
    if (q == inputifexists_code || q == ifexists_code) seen_plus = remove_initial_plus(true);
    if (q == Input_code) q = include_code;
    if (q == openin_code) {
        scan_optional_equals();
        file = scan_file_name();
    } else if (q == input_code || q == include_code)
        file = latex_input(q);
    else {
        flush_buffer();
        auto guard = InLoadHandler();
        file       = sT_arg_nopar();
    }
    if (q == readxml_code) {
        the_xmlA = read_xml(file);
        return;
    }
    std::optional<std::filesystem::path> res;
    if (seen_plus)
        res = find_no_path(file);
    else {
        res = tralics_ns::find_in_path(file);
        if (!res) {
            Buffer &B = local_buf;
            B         = file;
            if (!B.ends_with(".tex")) {
                B.append(".tex");
                std::string F = B;
                res           = tralics_ns::find_in_path(F);
            }
        }
    }
    if (q == openin_code) {
        tex_input_files[stream].open(file, *res, static_cast<bool>(res));
        return;
    }
    if (q == ifexists_code) {
        one_of_two(static_cast<bool>(res));
        return;
    }
    if (q == inputifexists_code) {
        TokenList A = read_arg();
        TokenList B = read_arg();
        if (res) open_tex_file(*res, seen_star);
        if (A.empty() && B.empty()) return; // optimise
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "++ " << (res ? "iftrue" : "iffalse") << "{" << A << "}{" << B << "}\n";
        }
        one_of_two(A, B, static_cast<bool>(res));
        return;
    }
    if (!res) {
        parse_error(T, "Cannot open input file ", file, "cannot open input");
        return;
    }
    open_tex_file(*res, seen_star);
}

// On-the-fly conversion allowed
void Parser::open_tex_file(const std::string &f, bool seen_star) {
    push_input_stack(f, seen_star, true);
    tralics_ns::read_a_file(lines, f, 2);
    lines.after_open();
    every_eof = true;
}

// Saves the current token list, ready to read from another one.
void Parser::save_the_state(SaveState &x) {
    x.copy_and_reset(TL);
    x.set_restricted(restricted);
}

// Undoes a previous save_the_state.
void Parser::restore_the_state(SaveState &x) {
    TL.clear();
    x.restore(TL);
    restricted = x.restricted;
}

// We have seen a character c at category code 7, for instance ^
// It could be ^^Z ^^ab ^^^^abce or ^^^^^abcde
// If this is the case, a new character is constructed and replaces the last
// character read (i.e., will be returned by read_next_char)

auto Parser::scan_double_hat(codepoint c) -> bool {
    auto sz = input_line.size();
    auto p  = input_line_pos;
    auto w  = sz - p + 1; // number of unread chars aon the line
    if (w < 3) return false;
    if (input_line[p] != c) return false;
    if (w >= 10 && input_line[p + 1] == c && input_line[p + 2] == c && input_line[p + 3] == c) {
        auto hc0 = input_line[p + 4].hex_val();
        auto hc1 = input_line[p + 5].hex_val();
        auto hc2 = input_line[p + 6].hex_val();
        auto hc3 = input_line[p + 7].hex_val();
        auto hc4 = input_line[p + 8].hex_val();
        if (hc0 && hc1 && hc2 && hc3 && hc4) {
            input_line_pos    = p + 8;
            input_line[p + 8] = codepoint((*hc0 << 16) + (*hc1 << 12) + (*hc2 << 8) + (*hc3 << 4) + *hc4);
            return true;
        }
    }
    if (w >= 8 && input_line[p + 1] == c && input_line[p + 2] == c) {
        auto hc1 = input_line[p + 3].hex_val();
        auto hc2 = input_line[p + 4].hex_val();
        auto hc3 = input_line[p + 5].hex_val();
        auto hc4 = input_line[p + 6].hex_val();
        if (hc1 && hc2 && hc3 && hc4) {
            input_line_pos    = p + 6;
            input_line[p + 6] = codepoint((*hc1 << 12) + (*hc2 << 8) + (*hc3 << 4) + *hc4);
            return true;
        }
    }
    if (w >= 4) {
        auto hc1 = input_line[p + 1].hex_val();
        auto hc2 = input_line[p + 2].hex_val();
        if (hc1 && hc2) {
            input_line_pos             = p + 2;
            input_line[input_line_pos] = codepoint(*hc1 * 16 + *hc2);
            return true;
        }
    }
    codepoint C = input_line[p + 1];
    if (!C.is_ascii()) return false;
    auto c1 = C.value;
    input_line_pos++;
    input_line[p + 1] = codepoint(c1 < 64 ? c1 + 64 : c1 - 64);
    return true;
}

// This constructs a command name from the current line
auto Parser::cs_from_input() -> Token {
    if (at_eol()) return Token(null_tok_val);
    codepoint c = get_next_char();
    state       = state_S;
    if (c.is_big()) { // abort and return null_cs
        --input_line_pos;
        return Token(null_tok_val);
    }
    int C = get_catcode(c.value);
    if (C == letter_catcode) {
        mac_buffer.clear();
        mac_buffer.push_back(c);
        for (;;) {
            if (at_eol()) break;
            c = get_next_char();
            if (c.is_big()) {
                --input_line_pos;
                break;
            } // abort
            C = get_catcode(c.value);
            if (C == letter_catcode) {
                mac_buffer.push_back(c);
                continue;
            }
            if (C == hat_catcode) {
                if (scan_double_hat(c)) continue;
            }
            --input_line_pos;
            break;
        }
        return hash_table.locate(mac_buffer);
    }
    if (C == space_catcode) return Token(c);
    if (C == hat_catcode) {
        if (scan_double_hat(c)) return cs_from_input();
        state = state_M;
        return Token(c);
    }
    state = state_M;
    return Token(c);
}

// This constructs a new token by reading characters from the buffer.
// Returns true in case of problem. Either because we are at EOL
// or because a line ended (% or \n), or because a character was ignored
// (space or invalid chars), or because of ^^
auto Parser::next_from_line0() -> bool {
    if (at_eol()) return true;
    codepoint c = get_next_char();
    if (c.is_big()) { // convert to \char"ABCD
        Buffer &B = local_buf;
        B.clear();
        B.push_back('"');
        B.push_back16(c.value, false);
        auto k = B.size();
        back_input(hash_table.space_token);
        while (k > 0) {
            back_input(Token(other_t_offset, static_cast<uchar>(B[k - 1])));
            k--;
        }
        state = state_M;
        see_cs_token(hash_table.char_token);
        return false;
    }
    cur_cmd_chr = CmdChr(get_catcode(c.value), subtypes(c.value));
    switch (cur_cmd_chr.cmd) {
    case escape_catcode:
        cur_tok = cs_from_input();
        see_cs_token();
        return false;
    case active_catcode:
        cur_tok.active_char(c.value);
        see_cs_token();
        state = state_M;
        return false;
    case space_catcode:
        if (state == state_M) {
            state       = state_S;
            cur_cmd_chr = CmdChr(space_catcode, subtypes(' '));
            cur_tok     = hash_table.space_token;
            return false;
        }
        return true;
    case comment_catcode: kill_line(); return true;
    case eol_catcode:
        kill_line();
        if (state == state_N) {
            see_cs_token(hash_table.par_token);
            return false;
        }
        if (state == state_M) {
            // this is different from TeX
            cur_cmd_chr = CmdChr(space_catcode, subtypes('\n'));
            cur_tok     = hash_table.newline_token;
            return false;
        }
        return true; // stateS
    case hat_catcode:
        if (scan_double_hat(c)) return true; // cur char has changed
        state = state_M;
        cur_tok.from_cmd_chr(cur_cmd_chr);
        return false;
    case special_catcode: // case of ShortVerb.
        verb_saved_char = c;
        see_cs_token(hash_table.verb_token);
        state = state_M;
        return false;
    case ignored_catcode: return true; // ignored
    case invalid_catcode: return true; // invalid
    default:
        state = state_M;
        cur_tok.from_cmd_chr(cur_cmd_chr);
        return false;
    }
}

// This is like above, but we try again. Note that get_a_new_line
// may fail, or fill the buffer, or put something in TL.
auto Parser::next_from_line() -> bool {
    for (;;) {
        bool res = next_from_line0();
        if (!res) return false;
        if (!at_eol()) continue;
        if (get_a_new_line()) // no other line available, abort
            return true;
        if (!TL.empty()) {
            next_from_list();
            return false;
        }
    }
}

// Returns next token, setting cur_tok and cur_cmd_chr
// If TL is not empty, reads from TL, otherwise from file unless restricted.
// This should produce a valid token; returns false, in this case.
// Otherwise, return true; set cur_tok to a null token, which might be
// interpreted as a character of catcode 0; which is impossible.

auto Parser::get_itoken() -> bool {
    if (!TL.empty()) {
        next_from_list();
        return false;
    }
    if (!restricted)
        if (!next_from_line()) return false;
    cur_tok.kill();
    cur_cmd_chr.kill();
    return true;
}

// Like back_input(t); read_token()
// without outher check or noexpand
void Parser::examine_token(Token t) {
    cur_tok = t;
    if (t.not_a_cmd())
        see_char_token(t);
    else
        see_cs_token();
}

auto Parser::get_token() -> bool {
    bool err = get_itoken();
    if (!err && cur_cmd_chr.is_outer() && scanner_status != ss_normal) check_outer_validity();
    return err;
}

// Reads a token that can be outer for \ifx \ifdefined or \show
// no outer check, thus no error.
auto Parser::get_token_o() -> bool {
    auto guard = SaveScannerStatus(ss_normal);
    return get_itoken();
}

// This puts t in cur_tok and sets cur_cmd_chr
void Parser::token_from_list(Token t) {
    cur_tok = t;
    if (t.not_a_cmd())
        see_char_token(t);
    else
        see_cs_token();
}

// Returns the next element of TL, assumed non-empty
// executes token_from_list  with a hack explained below
void Parser::next_from_list() {
    cur_tok = TL.front();
    TL.pop_front();
    if (cur_tok.not_a_cmd())
        see_char_token(cur_tok);
    else {
        see_cs_token();
        if (cur_cmd_chr.cmd == dont_expand_cmd) { // see comment below
            if (TL.empty()) return;
            cur_tok = TL.front();
            TL.pop_front();
            if (cur_tok.not_a_cmd()) return;
            see_cs_token();
            if (cur_cmd_chr.is_expandable()) { cur_cmd_chr = CmdChr(relax_cmd, subtypes(special_relax)); }
        }
    }
}

// Let's explain the dont_expand_cmd machinery above.
// There is a token called \notexpanded which is not in the hash table
// (so cannot be redefined), whose command code is dont_expand_cmd
// so the previous test is t== hash_table.frozen_dont_expand.
// As the token is not in the hash table next_from_line0 will never return it.

// Let's consider what happens when get_token finds \noexpand. In some cases the
// token is not interpreted (\def\foo{\noexpand}, \let\foo\noexpand, \ifx\noexpand)
// Otherwise, the token is expanded. A token is read, say T. No error is signaled
// if this is an outer token. Expansion is T when T is not a command (so, when it
// is a non-active character token); otherwise expansion is \notexpanded
// followed by T. Nothing special happens in the first case.

// The next call to get_token will find a non-empty TL; the first token will be
// \notexpanded, followed by T which is a command. So the two "return" in the
// code above are never executed and T is put in cur_tok. If T is expandable
// (included undefined) then cur_cmd_chr is set to relax.
// Note that get_x_token looks at the command code, and sees a non-expandable
// token. In the case of \xdef\foo{\noexpand\bar}, a special version of get_x_token
// is used (\the and \protected are not expand) and cur_tok is added to the list.
// It is generally assumed that, after get_x_token(), back_input(); get_x_token()
// is a no-op; this is wrong in the case of \noexpand (see Tralics online doc).

// The code that follows is used by \if and \ifcat. In the special case
// where (a == relax_cmd && b ==special_relax) we know that cur_tok is as above
// so that, if it is char_or_active, it is active.
// Every non-char is considered to be \relax.

void Parser::get_x_token_or_active_char(symcodes &a, subtypes &b) {
    get_x_token();
    a = cur_cmd_chr.cmd;
    b = cur_cmd_chr.chr;
    if (a == relax_cmd && b == special_relax) {
        if (cur_tok.char_or_active()) {
            a = active_catcode;
            b = cur_tok.chr_val();
        } else
            b = relax_code;
    } else if (a > active_catcode) {
        a = relax_cmd;
        b = relax_code;
    }
}

// Reads until a closing brace is found at level 0 (dismissed), or when
// in_env is true, until \end is found a brace level zero (to be read again)
// Return true if end condition is reached false otherwise.
// Result is in the buffer.
// Avoids tokenisation in order to reevaluate with other catcodes.
// Parsing may continue after EOF, so that we just cannot say read from TL and
// if empty, read from file. Is this a good idea?

auto Parser::scan_for_eval(Buffer &B, bool in_env) -> bool {
    int b      = 0; // brace level
    int elevel = 0; // begin/end level at brace level zero when in_env true
    for (;;) {
        // First read tokens from TL
        if (!TL.empty()) {
            Token t = TL.front();
            TL.pop_front();
            if (in_env && b == 0) { // Check end in the case of env
                if (t == hash_table.end_token) {
                    if (elevel == 0) {
                        back_input(t);
                        return true;
                    }
                    elevel--;
                } else if (t == hash_table.begin_token)
                    elevel++;
            }
            // Check brace level
            if (t.char_or_active()) {
                codepoint c = t.char_val();
                if (c == '{')
                    b++;
                else if (c == '}') {
                    if (b == 0) return !in_env; // if in_env, this is bad else OK
                    b--;
                }
            }
            B.insert_token(t, true);
            continue;
        }
        if (restricted) return false;
        // Now try characters from the input channel
        if (at_eol()) {
            if (get_a_new_line()) // no other line available, abort
                return false;
            continue; // this may set TL
        }
        codepoint c = get_next_char();
        if (c == '\r') c = codepoint('\n');
        B.push_back(c);
        if (c == '{') b++;
        if (c == '}') {
            b--;
            if (b == -1 && !in_env) {
                B.remove_last();
                return true;
            }
            if (b == -1) return false;
        }
        if (!(c == '\\')) continue;
        if (at_eol()) continue;
        c = get_next_char();
        B.push_back(c); // \{ and \} and \\ are normal chars
        if (!in_env) continue;
        if (b != 0) continue;
        if (c == 'b') {
            auto p = input_line_pos;
            if (p + 3 < input_line.size() && input_line[p] == 'e' && input_line[p + 1] == 'g' && input_line[p + 2] == 'i' &&
                input_line[p + 3] == 'n' &&
                (p + 4 == input_line.size() // this is silly
                 || !input_line[p + 4].is_letter()))
                elevel++;
        }
        if (c == 'e') {
            auto p = input_line_pos;
            if (p + 1 < input_line.size() && input_line[p] == 'n' && input_line[p + 1] == 'd' &&
                (p + 2 == input_line.size() // this is silly
                 || !input_line[p + 2].is_letter())) {
                elevel--;
                if (elevel == -1) {
                    input_line_pos -= 2; // reread backshash e
                    B.remove_last();     // remove these two chars from the buffer
                    B.remove_last();
                    return true;
                }
            }
        }
    }
}

void Buffer::insert_string(const Buffer &s) {
    clear();
    for (size_t j = 0; j < s.size(); j++) {
        char c = s[j];
        if (c != '\n' && c != '\r') push_back(c);
    }
    while (!empty() && back() == ' ') remove_last();
    ptrs.b = 0;
}

// This piece of code is executed when a new line, numbered N, is in
// the buffer scratch. We copy it into input_buffer. We extract characters
// and put them in input_line. In case vb is true,
// we print the line in the log file.
void Parser::store_new_line(int n, bool vb) {
    set_cur_line(n);
    input_buffer.insert_string(scratch);
    input_line          = input_buffer.codepoints();
    input_buffer.ptrs.b = 0;
    if (vb) {
        Logger::finish_seq();
        the_log << "[" << n << "] " << input_buffer.convert_to_log_encoding() << "\n";
    }
}

void Parser::insert_endline_char() {
    input_line_pos = 0;
    auto cc        = eqtb_int_table[endlinechar_code].val;
    if (cc >= 0 && cc < int(nb_characters)) input_line.emplace_back(to_unsigned(cc));
}

// Like get_a_newline below. But we never print the line in the log file.
// Reading can be from the terminal or an external file.
// Knuth says: an empty line is appended at the end of the file.
// in the case of continuation (spec=false) this is useless.
// Return value is true in case of problem.

auto Parser::new_line_for_read(bool spec) -> bool {
    state = state_N;
    static std::array<char, 4096> m_ligne;
    static int                    tty_line_no = 0;
    int                           n           = 0;
    scratch.clear();
    input_line.clear();
    if (cur_in_chan == nb_input_channels) {
        readline(m_ligne.data(), 78); // \todo pass the array instead
        tty_line_no++;
        n = tty_line_no;
        scratch.append(m_ligne.data()); // \todo push_back(std::array<char>)
    } else
        n = tex_input_files[cur_in_chan].lines.get_next(scratch);
    if (n < 0) {
        tex_input_files[cur_in_chan].close();
        if (!spec) return true;
    } else {
        store_new_line(n, false);
    }
    insert_endline_char();
    return false;
}

// Read a new line from the source. Returns true in case of failure.
// If force_eof is false, a tentative
// is made to read from the current file. If no file is found, reading is
// aborted if either no_new_file is true or there is no other file.
// If a line has been read, force_eof is set to false.
// In the case of \input\foo\input bar, after foo is finished, the
// pop_input_stack may give us a non-empty line, even a non-empty TL
// And consider the case \def\foo{\input A\input B\relax}
auto Parser::get_a_new_line() -> bool {
    state = state_N;
    input_line.clear();
    scratch.clear();
    int n = -2;
    if (cur_in_chan <= nb_input_channels) return true; // this is the wrong function to call
    if (force_eof) {
        lines.clear();
        force_eof = false;
    } else {
        n = lines.get_next(scratch);
        if (n < 0 && every_eof) {
            every_eof   = false;
            TokenList L = toks_registers[everyeof_code].val;
            if (!L.empty()) {
                if (tracing_io()) {
                    Logger::finish_seq();
                    the_log << "++ everyeof=" << L << ".\n";
                }
                back_input(L);
                return false;
            }
        }
    }
    if (n < 0) { // force_eof is true, or cur file ended
        bool retval = true;
        file_ended  = true;
        if (require_eof && (scanner_status != ss_normal)) {
            back_input(hash_table.eof_token);
            retval = false;
        }
        if (cur_input_stack.empty()) {
            if (tracing_io()) {
                Logger::finish_seq();
                the_log << "++ Input stack empty at end of file\n";
            }
            return retval;
        }
        pop_input_stack(true);
        return false;
    }
    store_new_line(n, tracing_io());
    insert_endline_char();
    return false;
}

// This implements \read.
// Reads from file (or the tty if the file is closed).
// A whole line is read. If braces are unbalanced, a second (or third...)
// line is read.
auto Parser::read_from_file(long ch, bool rl_sw) -> TokenList { // \todo should ch be unsigned?
    std::string fn = "tty";
    if (ch < 0 || ch >= nb_input_channels)
        cur_in_chan = tty_in_chan;
    else if (!tex_input_files[to_unsigned(ch)].is_open)
        cur_in_chan = tty_in_chan;
    else {
        cur_in_chan = to_unsigned(ch);
        fn          = tex_input_files[to_unsigned(ch)].lines.file_name;
    }
    push_input_stack(fn, false, true);
    TokenList L;
    new_line_for_read(true);
    int b = 0;
    if (rl_sw) { // case of readline, only one line is read
        for (;;) {
            if (at_eol()) break;
            codepoint c = get_next_char();
            if (c.value == ' ')
                L.push_back(hash_table.space_token);
            else
                L.push_back(Token(other_t_offset, c));
        }
    } else {
        for (;;) {
            if (at_eol()) {                     // can we stop here ?
                if (b == 0) break;              // Ok, brace level is zero
                if (new_line_for_read(false)) { // a file cannot end here
                    parse_error(err_tok, "unbalanced \\read");
                    while (b > 0) {
                        L.push_back(hash_table.CB_token);
                        b--;
                    }
                    break; // stop because file ended
                }
            }
            if (next_from_line()) continue; // we might be at EOL here
            if (cur_tok.is_CB_token()) {
                if (b == 0) {
                    parse_error(err_tok, "too many closing braces in \\read");
                    continue;
                }
                b--;
            }
            L.push_back(cur_tok);
            if (cur_tok.is_OB_token()) b++;
        }
    }
    // common ending
    if (ch < 16) tex_input_files[to_unsigned(ch)].line_no = get_cur_line();
    pop_input_stack(false);
    cur_in_chan = main_in_chan;
    return L;
}

// Fills the token list with the content of the buffer.
void Parser::tokenize_buffer(Buffer &b, TokenList &L, const std::string &name) {
    auto guard = SaveScannerStatus(ss_normal);
    push_input_stack(name, false, true);
    auto n     = cur_input_stack.size();
    bool s     = restricted;
    restricted = false;
    b.push_back('\n');
    lines.push_front(Clines(-1));
    lines.push_front(Clines(1, b, true));
    file_ended = false;
    for (;;) {
        bool res = next_from_line0();
        if (!res) {
            L.push_back(cur_tok);
            continue;
        }
        if (file_ended) break;
        if (!at_eol()) continue;
        if (get_a_new_line()) break;
        if (file_ended) break;
    }
    if (!L.empty() && L.back() == hash_table.newline_token) L.pop_back();
    restricted = s;
    if (n == cur_input_stack.size()) pop_input_stack(false);
}

// --------------------------------------------------
// section 3. scan_int and related functions
// User functions take as argument a token, the caller

// This is the one argument scanint function. It calls two auxiliary functions
// puts the result in cur_val, and returns it. It may log it.
auto Parser::scan_int(Token T) -> long {
    Token et      = err_tok;
    err_tok       = T;
    bool negative = scan_sign();
    auto val      = scan_int_internal();
    err_tok       = et;
    if (negative) val = -val;
    cur_val.set_int(val);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "+scanint for " << T << "->" << cur_val << "\n";
    }
    return val;
}

void Parser::scan_dimen(bool mu, Token T) {
    glue_spec unused{};
    Token     te = err_tok;
    err_tok      = T;
    scan_dimen(mu, false, unused, false);
    err_tok = te;
}

void Parser::scan_glue(internal_type level, Token T) {
    Token t = err_tok;
    err_tok = T;
    scan_glue(level);
    err_tok = t;
}

// \setcounter{foo}{10} is the same as \c@foo=10\relax.
// This function reads a token list, puts it back in the
// input, then calls scan_int.
auto Parser::scan_braced_int(Token T) -> long {
    TokenList L = read_arg();
    return scan_int(L, T);
}

auto Parser::scan_int(TokenList &L, Token T) -> long {
    Token marker = hash_table.relax_token;
    back_input(marker);
    back_input(L);
    auto n = scan_int(T);
    read_until(marker);
    return n;
}

// This function calls scan_int for the token t,
// and checks that the result is between 0 and n (inclusive).
auto Parser::scan_int(Token t, int n, String s) -> size_t {
    auto N = scan_int(t);
    if (N < 0 || N > n) {
        err_buf = fmt::format("Bad {} replaced by 0\n", s);
        signal_ovf(t, nullptr, N, n);
        cur_val.set_int_val(0);
        return 0;
    }
    return to_unsigned(N);
}

// Here we may have an optional argument; default value is d
auto Parser::scan_special_int_d(Token T, long d) -> long {
    TokenList L;
    read_optarg_nopar(L);
    if (L.empty()) {
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "+scanint for " << T << "->" << d << "\n";
        }
        return d;
    }
    back_input(hash_table.space_token);
    back_input(L);
    return scan_int(T);
}

// The following functions all call the previous scan_int.
auto Parser::scan_reg_num() -> size_t { return scan_int(cur_tok, nb_registers - 1, "register code"); }

// Why not max Unicode 10FFFF ?
auto Parser::scan_27bit_int() -> size_t { return scan_int(cur_tok, (1 << 27) - 1, "character code"); }

auto Parser::scan_char_num() -> size_t { return scan_int(cur_tok, scan_char_num_max, "character code"); }

auto Parser::scan_fifteen_bit_int() -> size_t { return scan_int(cur_tok, 32767, "mathchar"); }

// Scan a sign (plus, minus, spaces, etc.). After that cur_tok is the first
// unread token.
auto Parser::scan_sign() -> bool {
    bool negative = false;
    for (;;) {
        get_x_token();
        if (cur_cmd_chr.is_space()) continue;
        if (cur_tok.is_minus_token())
            negative = !negative;
        else if (!cur_tok.is_plus_token())
            return negative;
    }
}

// This is now the scan_int routine. It reads either `\A or
// \language or '12345.
auto Parser::scan_int_internal() -> long {
    if (cur_tok.is_invalid()) {
        missing_number();
        return 0;
    }
    if (cur_tok.is_backquote()) return to_signed(scan_alpha());
    if (cur_cmd_chr.is_ok_for_the()) {
        scan_something_internal(it_int, false);
        return cur_val.get_int_val();
    }
    return scan_int_digs();
}

// Read an alpha token.
// we allow }, \} or active ~. One optional space is read.
auto Parser::scan_alpha() -> size_t {
    if (get_token()) {
        improper_alpha();
        return 0;
    }
    if (cur_tok.active_or_single()) {
        auto t = cur_tok.chr_val();
        read_one_space();
        return t;
    }
    improper_alpha();
    return 0;
}

// This reads the digits for scan_int, with an overflow check.
auto Parser::scan_int_digs() -> long {
    unsigned radix     = 10;
    int      m         = 214748364;
    int      ok_so_far = 0;
    if (cur_tok.is_singlequote()) {
        radix = 8;
        m     = 1 << 28;
        get_x_token();
    } else if (cur_tok.is_doublequote()) {
        radix = 16;
        m     = 1 << 27;
        get_x_token();
    }
    bool vacuous = true;
    long val     = 0;
    for (;;) {
        auto d = cur_tok.tex_is_digit(radix);
        if (d == -1) break;
        vacuous = false;
        if (val >= m && (val > m || d > 7 || radix != 10)) {
            if (ok_so_far == 0) {
                parse_error(err_tok, "number too big");
            } else if (ok_so_far > 1000) {
                parse_error(err_tok, "number too big (infinite recursion?)");
                get_token(); // this may finish infinite recursion
                return 0;
            }
            ok_so_far++;
            val = max_integer; // this is 2^{31}-1
        } else
            val = val * to_signed(radix) + d;
        get_x_token();
    }
    if (vacuous) { // cur_tok cannot be a space here
        if (cur_tok.is_valid()) back_input();
        missing_number();
    } else
        back_input_unless_space();
    return val;
}

// This is the interface of the scan_something_internal routine.
// We first call scan_something_internal(cur_tok,level),
// then change the level, and (maybe) change the sign of the result.
void Parser::scan_something_internal(internal_type level, bool negative) {
    scan_something_internal(level);
    cur_val.change_level(level);
    if (negative) cur_val.negate();
}

// A mu is a math-unit. it is one quad divided by 18.
// but the quad depends on the current size. This explains some errors
// e.g. $x\mskip 18mu y^{x\mskip18mu y}$ : different spaces
// in plain tex, 10pt, we have 9.99976 and 8.19443

// Remember: type and level in decreasing order:
//   it_mu, it_glue, it_dimen,it_int
// (there are other types, but not here). Here type can only decrease.
void SthInternal::change_level(internal_type level) {
    while (type > level) {
        if (type == it_glue)
            set_int_val(get_glue_width());
        else if (type == it_mu)
            the_parser.mu_error("in conversion from mu to ", level);
        type = internal_type(type - 1);
    }
}

// This is the scan_something_internal routine proper.
// cur_tok is something like \count,  in case of \dimen0=\count0pt
// cur_cmd_chr us set
// The variable level is used only in case the caller cannot change the level
// For instance, if you say \count0 = \everypar.
// Variables like \lastskip have no equivalent in Tralics, because the lastskip
// may depend on the value of \spacefactor, and quantities defined only
// in fonts.

void Parser::scan_something_internal(internal_type level) {
    subtypes m = cur_cmd_chr.chr;
    size_t   v = 0; // \todo size_t?
    switch (cur_cmd_chr.cmd) {
    case char_given_cmd: // result of \chardef
    case math_given_cmd: cur_val.set_int(m); return;
    case last_item_cmd: // \lastkern etc
        switch (m) {
        case inputlineno_code: cur_val.set_int(get_cur_line()); return;
        case lastpenalty_code:
        case badness_code:
        case lastnodetype_code: cur_val.set_int(0); return;
        case xmllastid_code: cur_val.set_int(the_stack.get_xid().value); return;
        case xmlAid_code: fetch_box_id(the_xmlA); return;
        case xmlBid_code: fetch_box_id(the_xmlB); return;
        case XMLboxid_code: {
            auto vv = scan_reg_num();
            fetch_box_id(box_table[vv].val);
            return;
        }
        case XMLboxname_code: {
            auto vv = scan_reg_num();
            xml_name(box_table[vv].val, level);
            return;
        }
        case xmlAname_code: xml_name(the_xmlA, level); return;
        case xmlBname_code: xml_name(the_xmlB, level); return;
        case xmlAsize_code: cur_val.set_int(the_xmlA != nullptr ? the_xmlA->real_size() : -1); return;
        case xmlcurrentid_code: cur_val.set_int(the_stack.cur_xid().value); return;
        case xmlcurrow_code:
        case xmlcurcell_code:
        case xmlcurarray_code: cur_val.set_int(the_stack.find_ctrid(m)); return;
        case currentgrouplevel_code: cur_val.set_int(cur_level - 1); return;
        case currentgrouptype_code: cur_val.set_int(cur_group_type()); return;
        case etexversion_code: cur_val.set_int(2); return;
        case currentiflevel_code: cur_val.set_int(to_signed(conditions.top_level())); return;
        case currentiftype_code: cur_val.set_int(conditions.top_type()); return;
        case currentifbranch_code: cur_val.set_int(conditions.top_branch()); return;
        case fontcharwd_code:
        case fontcharht_code:
        case fontchardp_code:
        case fontcharic_code:
            scan_font_ident();
            scan_char_num();
            cur_val.set_dim(0);
            return;
        case parshapelength_code:
        case parshapeindent_code:
        case parshapedimen_code: parshape_aux(m); return;
        case mutoglue_code:
            scan_glue(it_mu, cur_tok);
            cur_val.set_type(it_glue);
            return;
        case gluetomu_code:
            scan_glue(it_glue, cur_tok);
            cur_val.set_type(it_mu);
            return;
        case gluestretchorder_code:
        case glueshrinkorder_code:
        case gluestretch_code:
        case glueshrink_code:
            scan_glue(it_glue, cur_tok);
            cur_val.get_info(m);
            return;
        case numexpr_code:
        case dimexpr_code:
        case glueexpr_code:
        case muexpr_code: scan_expr(m); return;
        case lastskip_code: //\muskip0=\lastskip
            cur_val.set_dim(0);
            if (the_stack.get_mode() == mode_math) cur_val.set_mu(Glue());
            return;
        default: // lastkern lastskip
            cur_val.set_dim(0);
            return;
        }
    case toks_register_cmd:    // \toks 349
    case assign_toks_cmd:      // \output, \everypar etc
        if (level != it_tok) { // perhaps we wanted a dimen here...
            bad_number();
            return;
        }
        v = m;
        if (cur_cmd_chr.cmd == toks_register_cmd) // read the 349 in `\toks349'
            v = scan_reg_num();
        cur_val.set_toks(toks_registers[v].val);
        return;
    case assign_int_cmd: // \year etc
        cur_val.set_int(eqtb_int_table[m].val);
        return;
    case assign_dimen_cmd: // \parindent etc
        cur_val.set_dim(eqtb_dim_table[m].val);
        return;
    case assign_glue_cmd: // \baselineskip etc
        cur_val.set_glue(glue_table[m].val);
        return;
    case assign_mu_glue_cmd: // \thinmuskip etc
        cur_val.set_mu(glue_table[m].val);
        return;
    case assign_font_dimen_cmd: // \fontdimen
    {
        auto a = scan_int(cur_tok); // read the position in the table
        auto k = scan_font_ident(); // read the font ID
        cur_val.set_dim(tfonts.get_dimen_param(k, a).get_value());
        return;
    }
    case assign_font_int_cmd: // hyphenchar, \skewchar. Read a font ID
    {
        auto k = scan_font_ident();
        cur_val.set_int(tfonts.get_int_param(k, m));
        return;
    }
    case assign_enc_char_cmd: {
        Token T   = cur_tok;
        auto  enc = scan_int(T);
        auto  pos = scan_int(T);
        cur_val.set_int(io_ns::get_enc_param(enc, pos));
        return;
    }
    case set_mathprop_cmd: {
        auto k = scan_mathfont_ident();
        auto w = eqtb_int_table[mathprop_ctr_code].val;
        w      = (w & (1 << k)) != 0 ? 1 : 0;
        cur_val.set_int(w);
        return;
    }
    case set_mathchar_cmd: // returns the cur math char as a token list
    {
        Token T  = cur_tok;
        auto  k  = scan_mathfont_ident();
        auto  vv = scan_int(T, 127, "mathchar");
        if (level != it_tok) {
            cur_tok = T;
            bad_number();
            return;
        }
        std::string s = get_math_char(uchar(vv), k);
        cur_val.set_toks(token_ns::string_to_list(s, false));
        return;
    }
    case set_aux_cmd: // \spacefactor etc
        if (m == 0)
            cur_val.set_int(0); // \spacefactor
        else if (m == 1)
            cur_val.set_dim(0);
        else if (m == 2)
            cur_val.set_int(to_signed(lines.encoding));
        else if (m == 3)
            cur_val.set_int(to_signed(the_main->input_encoding));
        else if (m == 4)
            cur_val.set_int(cur_font.get_size());
        return;
    case set_prev_graf_cmd: // \prevgraf
        cur_val.set_int(0);
        return;
    case set_page_dimen_cmd: // \pagedepth etc
        cur_val.set_dim(0);
        return;
    case set_page_int_cmd: // \deadcycles etc
        cur_val.set_int(0);
        return;
    case set_shape_cmd: // \parshape
        if (m == parshape_code)
            cur_val.set_int(parshape_vector.size() / 2);
        else { // interlinepenalties, etc
            auto k = scan_int(cur_tok);
            if (k < 0)
                cur_val.set_int(0);
            else if (k == 0)
                cur_val.set_int(to_signed(penalties[m - 1].size()));
            else if (k <= to_signed(penalties[m - 1].size()))
                cur_val.set_int(penalties[m - 1][to_unsigned(k - 1)]);
            else
                cur_val.set_int(penalties[m - 1].back());
        }
        return;
    case set_box_dimen_cmd: // \width, \height \depth
        scan_reg_num();
        cur_val.set_dim(0);
        return;
    case def_code_cmd: // \catcode, \lccode etc
        v = scan_char_num();
        cur_val.set_int(eqtb_int_table[v + m].val);
        return;
    case def_family_cmd:       // \textfont
    case set_font_cmd:         // a font (like \tenrm in plain)
    case def_font_cmd:         // \font
        if (level != it_tok) { // perhaps we wanted a dimen here...
            bad_number();
            return;
        }
        {
            back_input(); // push back, and use scan_font_ident as parser.
            auto               k = scan_font_ident();
            const std::string &s = tfonts.name(k);
            cur_val.set_toks(token_ns::string_to_list(s, false));
        }
        return;
    case register_cmd: // \count, \dimen, etc
        v = scan_reg_num();
        switch (static_cast<internal_type>(m)) {
        case it_int: cur_val.set_int(eqtb_int_table[v + count_reg_offset].val); return;
        case it_dimen: cur_val.set_dim(eqtb_dim_table[v].val); return;
        case it_glue: cur_val.set_glue(glue_table[v].val); return;
        case it_mu: cur_val.set_mu(glue_table[v + muskip_reg_offset].val); return;
        default: parse_error(err_tok, "Confusion in \\register"); return;
        }
    default: parse_error(err_tok, "You can't use `", cur_tok, "' after \\the", "You can't use x after...");
    }
}

void Parser::fetch_box_id(Xml *x) { cur_val.set_int(x != nullptr ? x->id.value : -4); }

// Aux function for \parshapeXXX, XXX= length indent or dimen
void Parser::parshape_aux(subtypes m) {
    auto vv = scan_int(cur_tok);
    cur_val.set_dim(0);
    if (vv <= 0) return;
    auto v = to_unsigned(vv);
    auto n = parshape_vector.size() / 2;
    if (n == 0) return;
    size_t q = 0;
    if (m == parshapelength_code)
        q = 0;
    else if (m == parshapeindent_code)
        q = 1;
    else {
        if ((v & 1) != 0) q = 1;
        v = (v + q) / 2;
    }
    if (v >= n) v = n;
    v = 2 * v - q - 1;
    cur_val.set_dim(parshape_vector[v]);
}

// This implements \the. The result is a token list, of catcode 12
// (except for space). The idea is to call scan_something_internal.
// If this does not give a token list, we convert it to a string,
// then use the str_toks command.
// Newline is converted to space. Is this a good idea?

void Parser::E_the_traced(Token T, subtypes c) {
    TokenList L = E_the(c);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "->" << L << ".\n";
    }
    back_input(L);
}

// implemets \detokenize, \unexpanded or \the
auto Parser::E_the(subtypes c) -> TokenList {
    static Buffer B;
    if (c == unexpanded_code) {
        TokenList L = scan_general_text();
        return L;
    }
    if (c == detokenize_code) {
        TokenList L = scan_general_text();
        B.clear();
        B << L;
        return B.str_toks(nlt_nl); // \n gives ^^J
    }
    if (tracing_commands()) Logger::log_dump("the");
    get_x_token();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\the " << cur_tok << "}\n";
    }
    scan_something_internal(it_tok, false);
    B.clear();
    switch (cur_val.get_type()) {
    case it_ident: // case of a font name
    case it_tok:   // case of a token list
        return cur_val.get_token_val();
    case it_int: B.append(std::to_string(cur_val.get_int_val())); break;
    case it_dimen: B.push_back(ScaledInt(cur_val.get_int_val()), glue_spec_pt); break;
    case it_glue: B.push_back(cur_val.get_glue_val()); break;
    case it_mu:
        B.push_back(cur_val.get_glue_val());
        B.pt_to_mu();
        break;
    }
    return B.str_toks(nlt_space); // \n gives space
}

// Next objective is scan_dimen. This can read a real number (in fact
// scaled integers)

// Scans something like 2.5, may skip a final space.
// In the case of "17.2, scan_int set radix to non-ten, so that the result
// is an integer. Same for `\A, and '177.
void Parser::scan_double(RealNumber &res) {
    bool is_decimal = true;
    //  0.5 is the same as .5; case of initial dot is trivial
    if (!cur_tok.is_dec_separator()) {
        get_x_token();
        if (cur_tok.is_invalid()) {
            missing_number();
            return;
        }
        long val = 0;
        if (cur_tok.is_backquote()) {
            is_decimal = false;
            val        = to_signed(scan_alpha());
        } else if (cur_cmd_chr.is_ok_for_the()) {
            is_decimal = false;
            scan_something_internal(it_int, false);
            val = cur_val.get_int_val();
        } else {
            if (cur_tok.is_singlequote() || cur_tok.is_doublequote()) is_decimal = false;
            val = scan_int_digs();
        }
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "+scanint for " << err_tok << "->" << val << "\n";
        }
        res.set_ipart(val); // this sets the integer part
    }
    if (!(is_decimal && cur_tok.is_dec_separator())) return;
    get_token(); // read the . or ,
    std::array<long, 17> table{};
    size_t               k = 0;
    for (;;) {
        get_x_token();
        if (!cur_tok.is_digit_token()) break;
        if (k < 17) {
            table[k] = to_signed(cur_tok.val_as_digit());
            k++;
        }
    }
    back_input_unless_space();
    res.convert_decimal_part(k, table.data()); // \todo pass the array instead
}

// This is a bit more efficient then a lot of scan_keyword...
// it reads a unit, returns
auto Parser::read_unit() -> int { // \todo std::optional<size_t>
    remove_initial_space();
    codepoint c1, c2;
    if (cur_tok.is_a_char()) {
        c1         = cur_cmd_chr.char_val().to_lower();
        Token save = cur_tok;
        get_x_token();
        if (cur_tok.is_a_char()) {
            c2 = cur_cmd_chr.char_val().to_lower();
            if (c1 == 'p' && c2 == 't') return unit_pt;
            if (c1 == 'i' && c2 == 'n') return unit_in;
            if (c1 == 'p' && c2 == 'c') return unit_pc;
            if (c1 == 'c' && c2 == 'm') return unit_cm;
            if (c1 == 'm' && c2 == 'm') return unit_mm;
            if (c1 == 'b' && c2 == 'p') return unit_bp;
            if (c1 == 'd' && c2 == 'd') return unit_dd;
            if (c1 == 'c' && c2 == 'c') return unit_cc;
            if (c1 == 's' && c2 == 'p') return unit_sp;
        }
        if (cur_tok.is_valid()) back_input();
        cur_tok = save;
    }
    if (cur_tok.is_valid()) back_input();
    return -1;
}

// Assume that we have read the -3.5 in -3.5cm
// We have to read the cm and convert the cm into sp.
void Parser::scan_unit(RealNumber R) {
    scan_keyword("true"); // \mag is ignored, hence no multiply....
    int k0 = read_unit();
    if (k0 < 0) {
        parse_error(err_tok, "Missing unit (replaced by pt) ", cur_tok, "", "missing unit");
        k0 = unit_pt;
    }
    auto k = to_unsigned(k0);
    if (k == unit_sp) {
        cur_val.set_int_val(R.get_ipart());
        cur_val.attach_sign(R.get_negative());
        return; // special ignore frac. part
    }
    static std::array<int, 9> numerator_table   = {0, 7227, 12, 7227, 7227, 7227, 1238, 14856, 0};
    static std::array<int, 9> denominator_table = {0, 100, 1, 254, 2540, 7200, 1157, 1157, -1};
    int                       num               = numerator_table[k];
    int                       den               = denominator_table[k];
    if (k != unit_pt) {
        auto i         = R.get_ipart();
        auto f         = R.get_fpart();
        long remainder = 0;
        i              = arith_ns::xn_over_d(i, num, den, remainder);
        f              = (num * f + (remainder << 16)) / den;
        i += f >> 16;
        R.set_fpart(f % (1 << 16));
        R.set_ipart(i);
    }
    cur_val.attach_fraction(R);
}

// If mu is true we want math_units. This procedure returns false
// in case no internal quantity is there. If there is an internal quantity
// we accept either an integer (a potential factor) or a dimen, skip or muskip
auto Parser::scan_dim_helper(bool mu, bool allow_int) -> bool {
    if (!cur_cmd_chr.is_ok_for_the()) {
        if (cur_tok.is_valid()) back_input();
        return false;
    }
    if (mu) {
        scan_something_internal(it_mu, false);
        cur_val.glue_to_mu(); // Ignores stretch and shrink if desired
        if (cur_val.is_mu()) return true;
        if (allow_int && cur_val.is_int()) return true;
        mu_error("Converting to mu from ", cur_val.get_type());
    } else
        scan_something_internal(it_dimen, false);
    return true;
}

// This is the start of scan_dimen. It is not called if scan_glue reads
// and integer and calls scan_dimen for more.
// In the case of a number like 1.5, we just call scan_double. and return false
// Otherwise, if we see \parindent or -\parindent, we put the value in
// cur_val, and return true. If we see \count0 (an integer), we put it in R
// and return false

auto Parser::scan_dim2(RealNumber &R, bool mu) -> bool {
    R.set_negative(scan_sign());
    if (!scan_dim_helper(mu, true)) {
        scan_double(R);
        return false;
    }
    bool res = false;
    if (mu) {
        if (cur_val.is_mu()) res = true;
    } else {
        if (cur_val.is_dimen()) res = true;
        // We could have \dimen0=\count0 \dimen1 ....
    }
    if (res)
        cur_val.attach_sign(R.get_negative());
    else
        R.from_int(cur_val.get_int_val());
    return res;
}

// Case 1: shortcut is false. If scan_dim2 return true,  we are done.
// Otherwise we have a value (integer or real number in value).
// Case 2: shortcut is true: we have a number in cur_val, and copy it in value.
// Hence, assume value holds a real number.
// Case 3. inf is true. We accept 1.2fill or \count0 filll
// Case 4: not inf, or no fill seen. We accept 2.5\parindent
// or \count0\skip15 (but not 1.2 3.4). Via scan_dime_helper
// Case 5: Otherwise, we need a unit, either cm, or mu or ex.
// Returns true if the last token read was a keyword

auto Parser::scan_dimen1(bool mu, bool inf, glue_spec &co, bool shortcut) -> bool {
    RealNumber value;
    if (!shortcut) {
        if (scan_dim2(value, mu)) return false;
    } else
        value.from_int(cur_val.get_int_val());
    if (inf) {
        if (scan_keyword("fil")) {
            co = glue_spec_fil;
            while (scan_keyword("l")) {
                if (co == glue_spec_filll) {
                    parse_error(err_tok, "Illegal unit of measure (replaced by filll)");
                } else
                    co = glue_spec(co + 1);
            }
            cur_val.attach_fraction(value);
            return true;
        }
    }
    remove_initial_space();
    if (scan_dim_helper(mu, false)) {
        multiply_dim(value, cur_val.get_int_val());
        return false;
    }
    if (mu) {
        if (scan_keyword("mu")) {
            cur_val.attach_fraction(value);
            return true;
        }
        mu_error("Expected mu unit, found something else", it_tok + 1);
    } else {
        if (scan_keyword("em")) {
            multiply_dim(value, 10 << 16);
            return true;
        }
        if (scan_keyword("ex")) {
            multiply_dim(value, 17 << 14); // 4.25 vs 0.430555 in cmr10
            return true;
        }
    }
    scan_unit(value);
    return true;
}

// If scan_dimen1 return true, we have to read an optional space.
// In any case, we print something in the log file.

void Parser::scan_dimen(bool mu, bool inf, glue_spec &co, bool shortcut) {
    bool skip = scan_dimen1(mu, inf, co, shortcut);
    if (skip) read_one_space();
    cur_val.set_type(it_dimen);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << local_buf.trace_scan_dimen(err_tok, cur_val.get_dim_val(), mu);
    }
}

// Multiplies a dimension by an integer
void Parser::multiply_dim(RealNumber val, long v) {
    long rem = 0; // unused but modified
    auto A   = arith_ns::xn_over_d(v, val.get_fpart(), 1 << 16, rem);
    auto B   = arith_ns::nx_plus_y(val.get_ipart(), v, A);
    cur_val.set_int_val(B);
    cur_val.attach_sign(val.get_negative());
}

// This reads glue.
// level is either it_glue or it_mu. The call to scan_something_internal
// signals an error if current value is a token list, and sets the type
// to something else.
// In any case

void Parser::scan_glue(internal_type level) {
    bool      negative = scan_sign();
    bool      mu       = level == it_mu;
    glue_spec co       = glue_spec_pt;
    if (cur_cmd_chr.is_ok_for_the()) { // got something to evaluate
        scan_something_internal(level, negative);
        internal_type ct = cur_val.get_type();
        switch (ct) {
        case it_int:
            scan_dimen(mu, false, co, true); // we have a multiplier
            break;
        case it_dimen:
            if (!mu) break;
            mu_error("in conversion from dimen to ", level);
            break;
        case it_glue: // we have the value
        case it_mu:
            // Note that ct<=level, different means from ct=glue to level=mu
            if (level != ct) { // Assume level = glue, ct = mu
                mu_error("in conversion from glue to ", ct);
                cur_val.set_type(level);
            }
            return;
        default:
            // This should not happen
            parse_error(err_tok, "Unexpected error in ", err_tok, "", "bad");
            cur_val.set_int_val(0);
            break;
        }
    } else {
        if (!cur_tok.is_invalid()) back_input();
        scan_dimen(mu, false, co, false);
        if (negative) cur_val.fast_negate();
    }
    Glue q;

    q.set_width(cur_val.get_int_val());
    if (scan_keyword("plus")) {
        co = glue_spec_pt;
        scan_dimen(mu, true, co, false);
        q.set_stretch(cur_val.get_int_val());
        q.set_stretch_order(co);
    }
    if (scan_keyword("minus")) {
        co = glue_spec_pt;
        scan_dimen(mu, true, co, false);
        q.set_shrink(cur_val.get_int_val());
        q.set_shrink_order(co);
    }
    cur_val.set_glue_val(q);
    cur_val.set_type(level);
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{scanglue " << cur_val << "}\n";
    }
}

// interface to scan_glue. If opt is true reads an optional argument
// If no optional argument is given, scan_glue_opt is set to false,
// and nothing is done. Otherwise, an argument is read (braces or
// parens), and pushed back.
void Parser::scan_glue(internal_type level, Token t, bool opt) {
    TokenList L;
    if (opt) {
        read_optarg_nopar(L);
        scan_glue_opt = true;
        if (L.empty()) scan_glue_opt = false;
        if (!scan_glue_opt) return;
    } else
        L = read_arg();
    list_to_glue(level, t, L);
}

// Calls scan_glue; returns 0 if no argument given.
auto Parser::get_opt_dim(Token t) -> std::string {
    scan_glue(it_glue, t, true);
    if (!scan_glue_opt) return std::string();
    return std::string(ScaledInt(cur_val.get_glue_width()));
}

void Parser::list_to_glue(internal_type level, Token t, TokenList &L) {
    back_input(hash_table.relax_token);
    back_input(L);
    if (level == it_dimen)
        scan_dimen(false, t);
    else
        scan_glue(level, t);
    get_token();
    if (cur_tok != hash_table.relax_token) back_input();
}

// Handles a command like \advance\count0 by, that allows \global before it.
// here cur_tok would be \advance
void Parser::M_prefixed_aux(bool gbl) {
    auto  chr = cur_cmd_chr.chr;
    Token T   = cur_tok;
    int   p   = 0;
    long  q   = 0;
    switch (cur_cmd_chr.cmd) {
    case set_font_cmd: word_define(cur_font_loc, chr, gbl); return;
    case shorthand_def_cmd: M_shorthand_define(chr, gbl); return;
    case read_to_cs_cmd: {
        auto ch = scan_int(T);
        if (!scan_keyword("to")) parse_error(T, "Missing `to' inserted");
        Token pp       = get_r_token();
        scanner_status = ss_defining;
        TokenList L    = read_from_file(ch, chr == one_code);
        new_macro(L, pp, gbl);
        scanner_status = ss_normal;
        return;
    }
    case toks_register_cmd: // ok ???
    case assign_toks_cmd: assign_toks(T, chr, gbl); return;
    case assign_int_cmd:
        p = chr;
        scan_optional_equals();
        q = scan_int(T);
        word_define(to_unsigned(p), q, gbl);
        return;
    case assign_dimen_cmd:
        p = chr;
        scan_optional_equals();
        scan_dimen(false, T);
        dim_define(to_unsigned(p), cur_val.get_dim_val(), gbl);
        return;
    case assign_glue_cmd:
    case assign_mu_glue_cmd: {
        p     = chr;
        int n = cur_cmd_chr.cmd;
        scan_optional_equals();
        if (n == assign_mu_glue_cmd)
            scan_glue(it_mu, T);
        else
            scan_glue(it_glue, T);
        glue_define(to_unsigned(p), cur_val.get_glue_val(), gbl);
        return;
    }
    case def_code_cmd: assign_def_something(gbl); return;
    case def_family_cmd: {
        auto a = scan_int(cur_tok, 15, "family number");
        scan_optional_equals();
        auto c = scan_font_ident();
        word_define(a + chr, c, gbl);
        return;
    }
    case set_mathprop_cmd: {
        auto k    = scan_mathfont_ident();
        auto mask = 1U << k;
        auto w    = eqtb_int_table[mathprop_ctr_code].val;
        scan_optional_equals();
        auto v = scan_int(T);
        if (v != 0)
            w |= mask;
        else
            w &= ~mask;
        word_define(mathprop_ctr_code, w, gbl);
        return;
    }
    case set_mathchar_cmd: // set a slot in the math font table
    {
        auto k = scan_mathfont_ident();
        auto v = scan_int(T, 127, "mathchar");
        scan_optional_equals();
        flush_buffer();
        std::string value = sT_arg_nopar();
        set_math_char(static_cast<uchar>(v), k, value);
        return;
    }
    case register_cmd:
    case advance_cmd:
    case multiply_cmd:
    case divide_cmd: do_register_command(gbl); return;
    case set_box_cmd: { // \setbox0 =
        auto i = scan_reg_num();
        scan_optional_equals();
        scan_box(gbl ? setbox_offset + i : i);
        return;
    }
    case set_aux_cmd:
        scan_optional_equals();
        if (chr == 1)
            scan_dimen(false, T); // \prevdepth
        else {
            q = scan_int(T); // \spacefactor or \input@encoding
            if (chr == 2) lines.change_encoding(q);
            if (chr == 3) the_main->set_input_encoding(q >= 0 ? to_unsigned(q) : 0);
            if (chr == 4) {
                cur_font.change_size(q);
                font_has_changed();
            }
        }
        return;
    case set_prev_graf_cmd:
        scan_optional_equals();
        scan_int(T);
        return; // ignore value
    case set_page_dimen_cmd:
        scan_optional_equals();
        scan_dimen(false, T);
        return; // ignore value
    case set_page_int_cmd:
        scan_optional_equals();
        scan_int(T);
        return; // ignore value
    case set_box_dimen_cmd:
        scan_reg_num();
        scan_optional_equals();
        scan_dimen(false, T);
        return; // ignore value
    case set_shape_cmd: {
        scan_optional_equals();
        q = scan_int(T);
        if (chr == parshape_code) {
            if (q <= 0) {
                parshape_vector.resize(0);
                return;
            }
            q = 2 * q;
            parshape_vector.resize(to_unsigned(q));
            for (size_t j = 0; j < to_unsigned(q); j++) {
                scan_dimen(false, T);
                parshape_vector[j] = cur_val.get_dim_val();
            }
        } else {
            std::vector<long> &V = penalties[chr - 1];
            if (q < 0) {
                V.resize(0);
                return;
            }
            V.resize(to_unsigned(q));
            for (long &j : V) j = scan_int(T); // \todo range for
        }
    }
        return;
    case assign_font_dimen_cmd: {
        auto a = scan_int(T);
        auto b = scan_font_ident();
        scan_optional_equals();
        scan_dimen(false, T);
        auto c = cur_val.get_int_val();
        tfonts.set_dimen_param(b, a, c);
        return;
    }
    case assign_font_int_cmd: {
        auto k = scan_font_ident();
        scan_optional_equals();
        auto v = scan_int(T);
        tfonts.set_int_param(k, chr, v);
        return;
    }
    case assign_enc_char_cmd: {
        auto enc = scan_int(T);
        auto pos = scan_int(T);
        scan_optional_equals();
        auto v = scan_int(T);
        if (pos < 0 && v >= 0 && pos - v <= 256) {
            auto start = v;
            auto bound = v - pos;
            for (auto i = start; i < bound; i++) {
                v = scan_int(T);
                io_ns::set_enc_param(enc, i, v);
            }
        } else
            io_ns::set_enc_param(enc, pos, v);
        return;
    }
    case def_font_cmd: new_font(); return;
    case set_interaction_cmd: return;
    default: return;
    }
}

// Case of \everypar or \toks0.
// Location in table is subtype (for \everypar) or argument (for \toks)
// here p is subtype of object to assign
void Parser::assign_toks(Token T, size_t p, bool gbl) {
    if (cur_cmd_chr.cmd == toks_register_cmd) p = scan_reg_num();
    scan_optional_equals();
    remove_initial_space_relax();
    int    c        = cur_cmd_chr.cmd;
    bool   have_reg = true;
    size_t q        = cur_cmd_chr.chr;
    if (c == open_catcode)
        have_reg = false;
    else if (c == toks_register_cmd)
        q = scan_reg_num();
    else if (c != assign_toks_cmd) {
        err_buf = fmt::format("Missing {{ inserted for token register {}; got {}", T, cur_tok);
        signal_error(T, "missing brace");
        back_input();
        have_reg = false;
    }
    if (have_reg) {
        if (p == q) return;
        TokenList Q = toks_registers[q].val;
        token_list_define(p, Q, gbl);
    } else { // this is scan_toks (false,false)
        auto      guard = SaveScannerStatus(ss_absorbing);
        TokenList Q;
        skip_group0(Q);
        token_list_define(p, Q, gbl);
    }
}

// This implements \show. Puts characters in the buffer B, then prints it.
// If what=1, this implements \meaning
// If what=2, this implements \@onelevel@sanitize
// If what=3, this Latex3 \cs_meaning:c

void Parser::token_show(int what, Buffer &B) {
    if (what == 3) {
        fetch_name0();
        tok_is_defined = hash_table.is_defined(fetch_name_res);
        if (tok_is_defined)
            back_input(hash_table.last_tok);
        else {
            B << "undefined";
            return;
        }
    }
    if (get_token_o()) { return; }
    bool lg = what == 0;
    if (lg) {
        Logger::finish_seq();
        if (!cur_tok.not_a_cmd()) log_and_tty << cur_tok << "=";
    }
    token_for_show(lg, cur_cmd_chr, B);
    if (what == 2) { // find and strip the prefix
        auto i = B.find('>');
        if (i == std::string::npos) return;
        std::string s = B.substr(i + 1);
        B.clear();
        B.append(s);
    }
    if (lg) log_and_tty << B.convert_to_log_encoding() << ".\n";
}

// This is for tracingassigns
void Parser::token_for_show(const CmdChr &val) {
    static Buffer B;
    B.clear();
    token_for_show(false, val, B);
    the_log << B.convert_to_log_encoding();
}

// This is the common code
void Parser::token_for_show(bool lg, const CmdChr &val, Buffer &B) {
    symcodes K = val.cmd;
    if (K == undef_cmd)
        B << "undefined";
    else if (K >= user_cmd) {
        Macro &X = mac_table.get_macro(val.chr);
        if (X.type == dt_optional) B << "opt ";
        B.dump_prefix(false, false, K);
        B << "macro:" << (lg ? " " : "") << X;
    } else if (K == set_font_cmd) {
        B << "select font ";
        if (val.chr == 0)
            B << "nullfont";
        else
            tfonts.full_name(B, val.chr);
    } else if (K > 16) {
        auto s = val.name();
        B.insert_escape_char_raw();
        B << s;
    } else {
        String s = val.special_name();
        if (s == nullptr) s = "[unknown command code!]";
        B << s << " " << val.char_val();
        if (val.char_val() == 0) B << "^^@";
    }
}

// Implements \font\foo = cmr10 at 10 pt,
// Or \font\foo = cmr10 scaled 30
// [todo] replace scaled 30 by -10pt
void Parser::new_font() {
    Token T = cur_tok;
    get_r_token();
    auto u = cur_tok.eqtb_loc();
    eq_define(cur_tok.eqtb_loc(), CmdChr(set_font_cmd, zero_code), false);
    scan_optional_equals();
    std::string name       = scan_file_name();
    long        scaled_val = 0;
    long        at_val     = 0;
    if (scan_keyword("at")) {
        scan_dimen(false, T);
        at_val = cur_val.get_int_val();
    } else if (scan_keyword("scaled"))
        scaled_val = scan_int(T);
    auto res = tfonts.find_font(name, at_val, scaled_val);
    eq_define(u, CmdChr(set_font_cmd, subtypes(res)), false);
}

// Executes equivalent of
// \font\tenrm = cmr10 at 10pt \tenrm"
void Parser::initialise_font() {
    Token cmd = hash_table.locate("tenrm");
    auto  u   = cmd.eqtb_loc();
    auto  res = tfonts.find_font("cmr10", 10 << 16, 0);
    eq_define(u, CmdChr(set_font_cmd, subtypes(res)), false);
    word_define(cur_font_loc, to_signed(res), false);
}

// puts in cur_val the name of x
void Parser::xml_name(Xml *x, internal_type level) {
    static Buffer B;
    B.clear();
    if (x != nullptr) B.append(encode(x->name));
    if (level != it_tok) {
        bad_number1(B);
        return;
    }
    cur_val.set_toks(B.str_toks(nlt_space)); // space newline forbidden
}

// Convert handles \string, \number, \meaning, etc
void Parser::E_convert() {
    long   n = 0;
    Token  T = cur_tok;
    Buffer B;
    B.clear();
    int c = cur_cmd_chr.chr;
    switch (c) {
    case number_code:
    case at_arabic_code:
        n = scan_int(T);
        B.append(std::to_string(n));
        break;
    case twodigits_code: {
        TokenList L = read_arg();
        back_input(L);
    } // shit
        n = scan_int(T);
        if (n < 10) B.push_back('0');
        B.append(std::to_string(n));
        break;
    case romannumeral_code:
        n = scan_int(T);
        B.push_back_roman(n);
        break;
    case Romannumeral_code:
        n = scan_int(T);
        B.push_back_Roman(n);
        break;
    case l3string_code: // like \string sans escapechar
    case string_code:
        if (get_token_o()) return; // error ?
        tex_string(B, cur_tok, c == string_code);
        break;
    case meaning_c_code: // latex3
        token_show(3, B);
        break;
    case meaning_code: token_show(1, B); break;
    case sanitize_code: token_show(2, B); break;
    case fontname_code: {
        auto k = scan_font_ident();
        tfonts.full_name(B, k);
        break;
    }
    case jobname_code: B.append(get_job_name()); break;
    case ra_jobname_code: B.append(get_projet_val()); break;
    case attributeval_code: B.append(get_attval()); break;
    case tralicsversion_code: B.append(tralics_version); break;
    case etexrevision_code: B.append(".0"); break;
    case rayear_code: B.format("{}", the_parser.get_ra_year()); break;
    }
    TokenList L = B.str_toks(nlt_space); // SPACE
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "->" << L << "\n";
    }
    if (c == sanitize_code) {
        new_macro(L, cur_tok);
        return;
    }
    back_input(L);
}

// -----  scan_expr

// Kill everything but expr_type
void ScanSlot::kill() {
    expr_so_far.kill();
    term_so_far.kill();
    expr_state = se_none;
    term_state = se_none;
    numerator  = 0;
}

// This implements \numexpr, \dimexpr, \glueexpr, \muexpr
// Set cur_val
void Parser::scan_expr(subtypes m) {
    Token         T  = cur_tok;
    internal_type it = it_int;
    switch (m) {
    case dimexpr_code: it = it_dimen; break;
    case glueexpr_code: it = it_glue; break;
    case muexpr_code: it = it_mu; break;
    default: it = it_int;
    }
    bool b = scan_expr(T, it);
    if (b) {
        parse_error(T, "Arithmetic overflow");
        cur_val.kill();
    }
    if (the_parser.tracing_commands()) {
        Logger::finish_seq();
        the_log << "+scan for " << T << "= " << cur_val << "\n";
    }
}

// This finds the operator, or reads a terminating \relax
// if the operator stack is not empty, and no operator is given, we expect
// a closing paren.
auto Parser::scan_expr_next(Token T, bool stack_empty) -> scan_expr_t {
    remove_initial_space();
    if (cur_tok.is_plus_token()) return se_add;
    if (cur_tok.is_minus_token()) return se_sub;
    if (cur_tok.is_star_token()) return se_mult;
    if (cur_tok.is_slash_token()) return se_div;
    if (stack_empty) {
        // read a \relax, push back otherwise
        if (!cur_cmd_chr.is_relax()) back_input();
    } else if (!cur_tok.is_close_paren()) {
        parse_error(T, "Expected + - * / or ); closing paren inserted");
        back_input();
    }
    return se_none;
}

// This reads the next item. We have a read a token, and checked that it is
// not an open parenthesis, so that we must push it back.
void Parser::scan_expr_arg(Token T, internal_type x) {
    back_input();
    if (x == it_int)
        scan_int(T);
    else if (x == it_dimen)
        scan_dimen(false, T);
    else if (x == it_glue)
        scan_glue(it_glue, T);
    else
        scan_glue(it_mu, T);
}

// In the case y+x*a*b/c, we handle the two terms y and x*a*b/c
// Consider the second term. When x is read, the state is none, we copy
// x in term_so_far. Quantities a,b,c and integers. When they are read
// we look at what follows (in next_state) and preceeds (in term_state)
// case *a* : we multiply
// case *b/ we remember b in numerator, change next_state to scale
// case /c?: we divide b c
// case \c? : we scale

void ScanSlot::compute_term(scan_expr_t &next_state, SthInternal f, char &C) {
    C = ' ';
    f.check_overflow(term_state);
    if (term_state == se_none) {
        term_so_far = f;
        if (next_state != se_none) term_so_far.normalise();
    } else if (term_state == se_mult) {
        if (next_state == se_div) {
            next_state = se_scale;
            numerator  = f.get_int_val();
        } else {
            C = '*';
            term_so_far.expr_mul(f.get_int_val());
        }
    } else if (term_state == se_div) {
        C = '/';
        term_so_far.quotient(f.get_int_val());
    } else if (term_state == se_scale) {
        C = '\\';
        term_so_far.scale(numerator, f.get_int_val());
    }
}

// This performs addition or subtraction, or finishes term evaluation
void ScanSlot::add_or_sub(scan_expr_t &next_state, char &C) {
    C = ' ';
    if (next_state > se_sub) {
        term_state = next_state;
        return;
    }
    term_state = se_none; // done with term
    if (expr_state == se_none) {
        C           = '=';
        expr_so_far = term_so_far; // convert term to expre
    } else {
        if (expr_state == se_sub) {
            C = '-';
            term_so_far.negate();
        } else
            C = '+';
        expr_so_far.add(term_so_far);
    }
    expr_state = next_state;
}

void Parser::trace_scan_expr(String s, const SthInternal &v, char t, Token T) {
    if (the_parser.tracing_commands() && t != ' ') {
        Logger::finish_seq();
        the_log << "+" << s << " so far for " << T << t << ' ' << v << "\n";
    }
}

auto Parser::scan_expr(Token T, internal_type et) -> bool {
    std::vector<ScanSlot> estack;
    ScanSlot              W;
    W.expr_type   = et;
    bool b        = false; // return value, true in case of overflow
    char tr_state = 0;
    for (;;) {
        // next level is it_int in case we scan a factor,
        internal_type next_level = W.get_next_type();
        remove_initial_space();
        if (cur_tok.is_open_paren()) {
            estack.push_back(W);
            W.kill();
            W.expr_type = next_level;
            continue;
        }
        scan_expr_arg(T, next_level);
        SthInternal f = cur_val;
        for (;;) { // found here
            scan_expr_t next_state = scan_expr_next(T, estack.empty());
            W.compute_term(next_state, f, tr_state);
            trace_scan_expr("term", W.term_so_far, tr_state, T);
            W.add_or_sub(next_state, tr_state);
            trace_scan_expr("expr", W.expr_so_far, tr_state, T);
            if (next_state != se_none) break;
            if (estack.empty()) {
                cur_val = W.expr_so_far;
                return b;
            }
            f = W.expr_so_far;
            W = estack.back();
            estack.pop_back();
        }
    }
}

// OK for \font, or \scriptfont, or \nullfont or \tenrm
auto Parser::scan_font_ident() -> long {
    remove_initial_space();
    if (cur_cmd_chr.cmd == def_font_cmd) return eqtb_int_table[cur_font_loc].val;
    if (cur_cmd_chr.cmd == set_font_cmd) return cur_cmd_chr.chr;
    if (cur_cmd_chr.cmd == def_family_cmd) {
        auto a = cur_cmd_chr.chr;
        auto b = scan_int(cur_tok, 15, "family number");
        return eqtb_int_table[a + b].val;
    }
    if (cur_tok.is_valid()) back_input();
    parse_error(err_tok, "Missing font identifier");
    return -1;
}

// OK for \mml@font@fraktur, or a font like that or an integer.
auto Parser::scan_mathfont_ident() -> size_t {
    Token T = cur_tok;
    remove_initial_space();
    if (cur_cmd_chr.cmd == mathfont_cmd) return cur_cmd_chr.chr;
    if (cur_cmd_chr.cmd == math_font_cmd) {
        switch (cur_cmd_chr.chr) {
        case mathtt_code: return math_f_monospace;
        case mathcal_code: return math_f_script;
        case mathbf_code: return math_f_bold;
        case mathrm_code: return math_f_upright;
        case mathfrak_code: return math_f_fraktur;
        case mathit_code: return math_f_italic;
        case mathnormal_code: return math_f_normal;
        case mathbb_code: return math_f_doublestruck;
        case mathsf_code: return math_f_sansserif;
        default: break;
        }
    }
    if (cur_tok.is_valid()) back_input();
    return scan_int(T, 14, "math font identifier");
}

// This scans a hrule or a vrule.
// May be used in \cleaders\hrule; the result is the_stack.top_stack()
// Depending on the type, some fields have a default value of 0.4pt, or 0 pt.
// Others are -\infty (in reality, -2^14pt = - 2^30sp)
static const int default_rule_dimen = -1073741824;
void             TexRule::reset() {
    rule_h = default_rule_dimen;
    rule_w = default_rule_dimen;
    rule_d = default_rule_dimen;
}

void TexRule::convert(AttList &res) const {
    if (rule_h.get_value() != default_rule_dimen) res.push_back(the_names["height"], std::string(rule_h));
    if (rule_d.get_value() != default_rule_dimen) res.push_back(the_names["depth"], std::string(rule_d));
    if (rule_w.get_value() != default_rule_dimen) res.push_back(the_names["width"], std::string(rule_w));
}

void Parser::scan_rule(int c) {
    TexRule R;
    Token   T = cur_tok;
    if (c == rule_code) {
        R.rule_d = 0;
        TokenList L1;
        bool      b  = read_optarg(L1);
        TokenList L2 = read_arg();
        TokenList L3 = read_arg();
        if (b) R.rule_d = dimen_from_list(T, L1);
        R.rule_w = dimen_from_list(T, L2);
        R.rule_h = dimen_from_list(T, L3);
        R.adjust();
        leave_v_mode();
    } else {
        R.reset();
        if (c == vrule_code)
            R.init_vrule();
        else
            R.init_hrule();
        for (;;) {
            if (scan_keyword("width")) {
                scan_dimen(false, T);
                R.rule_w = cur_val.get_dim_val();
                continue;
            }
            if (scan_keyword("height")) {
                scan_dimen(false, T);
                R.rule_h = cur_val.get_dim_val();
                continue;
            }
            if (scan_keyword("depth")) {
                scan_dimen(false, T);
                R.rule_d = cur_val.get_dim_val();
                continue;
            }
            break;
        }
    }
    AttList &res = the_stack.add_newid0("rule");
    R.convert(res);
}

// This is the function associated to ' in math mode
// x' is x^{\prime}, x'' is  x^{\prime\prime} and x'^2 is x^{\prime2}
void Parser::scan_prime() {
    TokenList L;
    Token     T = hash_table.prime_token;
    for (;;) {
        L.push_back(T);
        if (get_token()) break;
        if (cur_cmd_chr.cmd == hat_catcode) { // TeX wants a hat token
            TokenList aux = read_arg();
            L.splice(L.end(), aux);
            break;
        }
        if (cur_cmd_chr.is_single_quote()) continue;
        back_input();
        break;
    }
    brace_me(L);
    back_input(L);
    back_input(hash_table.hat_token);
}
