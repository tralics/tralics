// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose Grimm) 2002-2011
// This file contains the array part of Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txarray.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "txinline.h"
#include <fmt/format.h>

namespace {
    NewArray new_array_object;
    char     char_for_error = 'c';
    bool     in_hlinee, have_above, have_below;
    Istring  hlinee_width, hlinee_above, hlinee_below;
    long     cline_first, cline_last;
    Buffer   errbuf;
} // namespace

void Parser::boot_NAO() { new_array_object.boot(this); }

void NewArray::boot(Parser *Q) {
    P = Q;
    for (unsigned i = 0; i < nb_newcolumn; i++) {
        nct_bool[i] = false;
        nct_tok[i]  = P->hash_table.relax_token;
    }
    nct_size = 0;
}

// This informs the system that char c is defined to be the token t.
void NewArray::add_a_type(uchar c, Token t) {
    if (c == 0) return;
    if (!nct_bool[c]) nct_size++;
    nct_bool[c] = true;
    nct_tok[c]  = t;
}

// This informs the system that char c is now undefined.
void NewArray::remove_a_type(uchar c) {
    if (nct_bool[c]) nct_size--;
    nct_bool[c] = false;
}

// Implements \newcolumntype{C}[opt]{body}
// as \newcommand\cmd[opt]{body}
// Here \cmd is some internal name, stored in nct_tok[C]
void Parser::T_newcolumn_type() {
    unsigned c   = 0;
    bool     bad = read_token_arg(cur_tok);
    if (!bad) {
        Token T = cur_tok;
        if (T.is_a_char()) c = T.char_val().value;
        if (!(c > 0 && c < nb_newcolumn)) {
            parse_error("Argument to \\newcolumntype is not a 7bit character");
            c = 0;
        }
    }
    Buffer B;
    B.push_back("newcolumtype@");
    B.push_back(static_cast<char>(c)); // special hack if c=0 !
    cur_tok = hash_table.locate(B);
    new_array_object.add_a_type(static_cast<uchar>(c), cur_tok);
    back_input();
    M_newcommand(rd_always); // definition is local
}

// Assume that T is the token defined and stored by the procedures above
// This returns -1 in case something strange happened (like redefinition)
// returns the number of arguments of the function otherwise.
auto Parser::nct_aux(Token T, TokenList &body) -> std::optional<size_t> {
    see_cs_token(T);
    if (!cur_cmd_chr.is_user()) return {}; // bad
    Macro &X = mac_table.get_macro(cur_cmd_chr.chr);
    if (!(X.type == dt_empty || X.type == dt_normal)) return {};
    body = X.body;
    return X.nbargs;
}

// This piece of code expands all the \newcolumntype commands
// There is a special case: nct_size=0,  case where nothing is
// defined, and nothing has to be done. Otherwise, we loop over all
// characters. If there is one substitution, we try again.
// If for some reason, the character is invalid, we remove it from the table.
void Parser::expand_nct(TokenList &L) {
    bool action = true;
    if (tracing_commands()) Logger::finish_seq(), the_log << "array preamble at start: " << L << "\n";
    if (!new_array_object.has_a_nct()) return;
    int max_iter = max_newcolumn_loops;
    while (action) {
        action = false;
        max_iter--;
        if (max_iter <= 0) {
            parse_error("array preamble expansion: Infinite loop?");
            return;
        }
        for (uchar i = 1; i < nb_newcolumn; i++) {
            const uchar c = i;
            if (!new_array_object.nct_exists(c)) continue;
            Token     T = new_array_object.nct_token(c);
            TokenList body;
            auto      n = nct_aux(T, body);
            if (!n) {
                new_array_object.remove_a_type(c);
                continue;
            }
            if (token_ns::expand_nct(L, *n, c, max_iter, body)) {
                action = true;
                if (tracing_commands()) the_log << "array preamble after " << c << ": " << L << "\n";
            }
        }
    }
}

// Here we have to find the character c.
// only top-level characters are considered. Active chars are allowed.
// MX is decreased. Job aborted if it becomes negative.
auto token_ns::expand_nct(TokenList &L, size_t n, uchar c, int &MX, TokenList &body) -> bool {
    TokenList                 res;
    bool                      result = false;
    std::array<TokenList, 10> Table; // arguments of the commands
    while (!L.empty()) {
        Token t = L.front();
        L.pop_front();
        if (t.is_a_left_brace()) { // get a whole block
            L.push_front(t);       // re-enter the brace in the list
            token_ns::fast_get_block(L, res);
            continue;
        }
        if (t.cmd_val() == 10) continue;             // ignore spaces.
        if (!(t.is_a_char() && t.char_val() == c)) { // just copy
            res.push_back(t);
            continue;
        }
        result = true; // We found something
        MX--;
        if (MX < 0) return true;
        for (size_t k = 0; k < n; k++) Table[k + 1] = get_a_param(L, false);
        TokenList W = Parser::expand_mac_inner(body, Table.data());
        L.splice(L.begin(), W);
    }
    L.splice(L.end(), res);
    return result;
}

// Prints a (ch_class, ch_num) pair for debug.
auto NewArray::dump_slot() -> String {
    switch (ch_class) {
    case chc_cell:
        switch (ch_num) {
        case chn_c: return "c ";
        case chn_l: return "l ";
        case chn_r: return "r ";
        case chn_m: return "m{} ";
        case chn_p: return "p{} ";
        case chn_b: return "b{} ";
        default: return "impossible!";
        }
    case chc_bar: return ch_num == 0 ? "| " : "!{} ";
    case chc_v: return "<{} ";
    case chc_u: return ">{} ";
    case chc_start: return "impossible!";
    case chc_inter: return "@{} ";
    case chc_pre_bar: return "!";
    case chc_pre_inter: return "@";
    case chc_pre_v: return "<";
    case chc_pre_u: return ">";
    case chc_pre_cell:
        if (ch_num == chn_m) return "m";
        if (ch_num == chn_p) return "p";
        if (ch_num == chn_b) return "b";
        return "impossible!";
    default: return "impossible!";
    }
}

// This sets (ch_class,ch_num) according to last_ch_class  (case where
// a brace-enclosed expression is expected) or the next token
// (in current_token, or the first element of current_list)
void NewArray::test_pach() {
    if (last_ch_class == chc_pre_bar) {
        ch_class = chc_bar;
        ch_num   = chn_l;
        return;
    }
    if (last_ch_class == chc_pre_inter) {
        ch_class = chc_inter;
        return;
    }
    if (last_ch_class == chc_pre_v) {
        ch_class = chc_v;
        return;
    }
    if (last_ch_class == chc_pre_u) {
        ch_class = chc_u;
        return;
    }
    ch_class = chc_cell;                       // we should have a cell here.
    if (last_ch_class == chc_pre_cell) return; // was ok
    if (have_token_list) {
        if (current_list.empty())
            current_token = Token();
        else
            current_token = current_list.front();
    }
    unsigned c = 0;
    if (current_token.cmd_val() == 11 || current_token.cmd_val() == 12 || current_token.cmd_val() == 13) c = current_token.char_val().value;
    if (c == 'c') {
        cur_h_pos = np_c_center;
        ch_num    = chn_c;
        return;
    }
    if (c == 'l') {
        cur_h_pos = np_c_left;
        ch_num    = chn_l;
        return;
    }
    if (c == 'r') {
        cur_h_pos = np_c_right;
        ch_num    = chn_r;
        return;
    }
    ch_num = chn_c; // in case of error
    // These want an argument
    char_for_error = static_cast<char>(c);
    if (c == '|') {
        ch_class = chc_bar;
        return;
    }
    if (c == '!') {
        ch_class = chc_pre_bar;
        return;
    }
    if (c == '@') {
        ch_class = chc_pre_inter;
        return;
    }
    if (c == '<') {
        ch_class = chc_pre_v;
        return;
    }
    if (c == '>') {
        ch_class = chc_pre_u;
        return;
    }
    ch_class = chc_pre_cell;
    if (c == 'm') {
        ch_num = chn_m;
        return;
    }
    if (c == 'p') {
        ch_num = chn_p;
        return;
    }
    if (c == 'b') {
        ch_num = chn_b;
        return;
    }
    ch_class = chc_cell;
    the_parser.parse_error(the_parser.err_tok, "Wrong character in array preamble ", current_token, "", "Wrong char");
}

// Big function that handles the array preamble. We first read and
// expand the preamble, then loop over all characters, then check
// that it ends correctly. If main_fct is false, we are in a multicolumn.
// In this case, the array preamble should specify only material
// for one cell. We read the content C of the cell, and push back
// the {UCV} list (where U and V the optional <u> and <v> parts.
// braces are added because cells are assumed to be typeset in a group.
void NewArray::run(Xid ID, bool main_fct) {
    id = ID;
    if (!main_fct) { // read and set the column span
        Istring            x = P->nT_arg_nopar();
        const std::string &s = x.name;
        if (s != "1") id.add_attribute(the_names[np_cols], x);
    }
    preamble = P->read_arg(); // read the preamble
    token_ns::expand_star(preamble);
    P->expand_nct(preamble);
    if (main_fct) AI = &P->the_stack.new_array_info(id);
    u_list.clear();
    v_list.clear();
    ac_cell_no    = 0;
    last_ch_class = chc_start;
    first_bar     = true;
    bool seen     = false;
    cur_h_pos     = np_c_center;
    if (P->tracing_commands()) the_log << "array preamble parse: ";
    for (;;) {
        if (preamble.empty()) break;
        if (ac_next()) continue;
        test_pach();
        if (P->tracing_commands()) the_log << dump_slot();
        switch (ch_class) {
        case chc_cell:
            if (main_fct)
                ac_maybe_finish(); // finish previous
            else
                ac_maybe_finish_multi(seen);
            attribs.push_back(np_halign, cur_h_pos, false); // set attributes
            cur_h_pos = np_c_center;                        // add a default
            current_list.clear();
            break;
        case chc_bar:
            if (ch_num == chn_c) {
                if (last_ch_class == chc_start)
                    attribs.push_back(np_leftborder, np_true);
                else if (last_ch_class == chc_bar) {
                } else
                    attribs.push_back(np_rightborder, np_true, false);
            }
            current_list.clear();
            break;
        case chc_v: // got a <v>list, add it
            if (!have_token_list)
                v_list.push_front(current_token);
            else
                v_list.splice(v_list.begin(), current_list);
            break;
        case chc_u: // got a <u>list, add it.
            if (!have_token_list)
                u_list.push_front(current_token);
            else
                u_list.splice(u_list.begin(), current_list);
            break;
        case chc_start: // preamble empty
            break;
        case chc_inter: current_list.clear(); break;
        case chc_pre_bar:
            if (last_ch_class == chc_u) P->parse_error("illegal ! or @");
            break;
        case chc_pre_inter:
            if (last_ch_class == chc_u) P->parse_error("illegal ! or @");
            break;
        case chc_pre_v:
            if (last_ch_class == chc_cell) break;
            if (last_ch_class == chc_v) break;
            P->parse_error("unexpected less-than sign");
            ch_class = chc_pre_bar;
            break;
        case chc_pre_u:
        case chc_pre_cell:
            if (main_fct)
                ac_maybe_finish();
            else
                ac_maybe_finish_multi(seen);
            break;
        }
        last_ch_class = ch_class;
    }
    switch (last_ch_class) {
    case chc_cell:
    case chc_bar:
    case chc_v:
    case chc_u:
    case chc_start:
    case chc_inter: break;
    default: {
        errbuf.reset();
        errbuf << "Array preamble: argument missing for " << char_for_error;
        P->parse_error(errbuf.c_str());
    }
    }
    if (P->tracing_commands()) the_log << "\n";
    if (main_fct) {
        ac_maybe_finish();
        return;
    }
    ac_maybe_finish_multi(seen);
    // Case of \multicolumn,
    id.add_attribute(attribs, true);
    attribs.reset();
    TokenList cell = P->read_arg();
    cell.splice(cell.begin(), u_list);
    cell.splice(cell.end(), v_list);
    u_list.clear();
    v_list.clear();
    P->brace_me(cell);
    P->back_input(cell);
}

// This is called when
// case 1 we see a >
// case 2 we see m, p or b :
// case 3 we see m{} p{} b{} or clr  : more will be done later
// in case m{}, this will be called twice (2 and 3) but nothing special happens
void NewArray::ac_maybe_finish() {
    if (last_ch_class == chc_bar && first_bar) {
        first_bar = false;
        return;
    }
    if (last_ch_class == chc_cell || last_ch_class == chc_v || last_ch_class == chc_bar) {
        AI->add_uv(u_list, v_list, attribs);
        attribs.reset();
        u_list.clear();
        v_list.clear();
        ac_cell_no++;
    }
}

// This is called in the case of multicolumn
void NewArray::ac_maybe_finish_multi(bool &seen) {
    if (last_ch_class == chc_bar && first_bar) {
        first_bar = false;
        return;
    }
    if (last_ch_class == chc_cell || last_ch_class == chc_v || last_ch_class == chc_bar) {
        if (seen) P->parse_error("multi specs in \\multicolumn");
        seen = true;
    }
}

// Return true if this is a space.
// otherwise get the next token in current_token
// if it is a brace, get the list in current_list
auto NewArray::ac_next() -> bool {
    current_token = preamble.front();
    if (current_token.cmd_val() == 10) {
        preamble.pop_front();
        return true;
    }
    // strange
    if (current_token == the_parser.hash_table.verb_token) {
        preamble.pop_front();
        return true;
    }
    if (current_token.is_a_left_brace()) {
        current_list = token_ns::fast_get_block(preamble);
        token_ns::remove_ext_braces(current_list);
        have_token_list = true;
    } else {
        have_token_list = false;
        preamble.pop_front();
    }
    return false;
}

// This starts or ends a cell, or does both
void Stack::push_pop_cell(int dir) {
    if (dir != push_only) {
        pop(np_cell);
        add_nl();
    }
    if (dir != pop_only) { push1(np_cell); }
}

void Xid::add_top_rule() const {
    add_attribute(np_topborder, np_true);
    if (in_hlinee) {
        add_attribute(np_border_topw, hlinee_width);
        if (have_above) add_attribute(np_rule_tsa, hlinee_above);
        if (have_below) add_attribute(np_rule_tsb, hlinee_below);
    }
}
void Xid::add_bottom_rule() const {
    add_attribute(np_bottomborder, np_true);
    if (in_hlinee) {
        add_attribute(np_border_bottomw, hlinee_width);
        if (have_above) add_attribute(np_rule_bsa, hlinee_above);
        if (have_below) add_attribute(np_rule_bsb, hlinee_below);
    }
}

// Start a row. If a=-1, it is the first row
// If a>0 it is a dimension to put between rows.
void Parser::start_a_row(long a) {
    Xid prev_row(2); // dummy id as default value
    {
        Xml *V = the_stack.top_stack()->last_addr();
        if (V != nullptr) prev_row = V->id;
    }
    bool initial_hline = false;
    if (a > 0) prev_row.add_attribute(the_names[np_spaceafter], Istring(to_unsigned(a)));
    for (;;) {
        remove_initial_space_and_back_input(); // get first non-space xtoken
        symcodes S = cur_cmd_chr.cmd;
        if (S == ifnextchar_cmd) {
            get_token();
            T_ifnextchar(true);
            continue;
        }
        if (S == hline_cmd) {
            auto     guard = SaveErrTok(cur_tok);
            subtypes c     = cur_cmd_chr.chr;
            get_token();
            int w = T_hline_parse(c);
            if (w == 2) T_cline();
            if (w == 1) { // a hline
                if (a < 0)
                    initial_hline = true;
                else
                    prev_row.add_bottom_rule();
            }
            continue;
        }
        if (S == end_cmd) return;
        the_stack.push1(np_row);
        if (initial_hline) the_stack.cur_xid().add_top_rule();
        start_a_cell(false);
        return;
    }
}

// If started is true, we have already started one
void Parser::start_a_cell(bool started) {
    if (!started) {
        the_stack.push_pop_cell(push_only);
        push_level(bt_cell);
    }
    remove_initial_space_and_back_input();
    symcodes S = cur_cmd_chr.cmd;
    if (S == unimp_cmd && cur_cmd_chr.chr == omit_code) {
        get_token();
        the_stack.mark_omit_cell();
    } else if (S == end_cmd) { // has to be \end{tabular}
        the_stack.mark_omit_cell();
    } else if (S == multicolumn_cmd) {
        the_stack.mark_omit_cell();
        get_token();
        unprocessed_xml.remove_last_space();
        Xid cid = the_stack.get_top_id();
        new_array_object.run(cid, false);
    } else {
        TokenList L = the_stack.get_u_or_v(true);
        if (tracing_commands() && !L.empty())
            Logger::finish_seq(), the_log << "{"
                                          << "template u-part " << L << "}\n";
        back_input(L);
    }
}

void Parser::finish_a_cell(Token T, const Istring &a) {
    flush_buffer();
    the_stack.remove_last_space();
    if (!a.null()) {
        back_input(hash_table.space_token);
        TokenList L = token_ns::string_to_list(a);
        back_input(L);
        T = hash_table.crwithargs_token;
    }
    back_input(T);
    if (the_stack.is_omit_cell()) return;
    TokenList L = the_stack.get_u_or_v(false);
    if (tracing_commands() && !L.empty())
        Logger::finish_seq(), the_log << "{"
                                      << "template v-part " << L << "}\n";
    back_input(L);
}

// This is \begin{tabular}. Should be in an environment (not necessarily)
void Parser::T_start_tabular(subtypes c) {
    Istring x = the_names[c == zero_code ? np_tabular : np_tabular_star];
    if (the_stack.is_float())
        leave_v_mode();
    else if (the_stack.is_frame(np_fbox)) {
    } else
        leave_h_mode();
    M_let_fast(hash_table.par_token, hash_table.empty_token, false);
    the_stack.push1(x, np_table);
    the_stack.add_att_to_last(np_rend, np_inline);
    Xid id = the_stack.get_xid();
    if (c != 0) { // case of tabular*
        Token     T         = cur_tok;
        TokenList L         = read_arg();
        ScaledInt tab_width = dimen_from_list(T, L);
        if (!tab_width.null()) id.add_attribute(the_names[np_tab_width], Istring(tab_width));
        get_token(); // eat the relax
        if (!cur_cmd_chr.is_relax()) back_input();
    }
    {
        auto pos = get_ctb_opt(); // Lamport does not mention c, who cares
        if (pos != 0) id.add_attribute(the_names[np_vpos], the_names[pos]);
    }
    new_array_object.run(id, true);
    the_stack.set_array_mode(); // Is the mode ok ?
    start_a_row(-1);
}

// implements \hline, \cline{p} or \hlinee[p]{w}{w}{w}
// p is a sequence of two integers, separated by a dash
// sets cline_first and cline_last;
// fills errbuf and returns a code in case of error

auto Parser::scan_pair_ints(Token T, TokenList &L) -> bool {
    back_input(hash_table.relax_token);
    back_input(L);
    cline_first = scan_int(T);
    if (get_token()) {
        errbuf.reset();
        errbuf << "Error in " << T << " after first integer";
        return true;
    }
    if (cur_tok == hash_table.relax_token) {
        errbuf.reset();
        errbuf << "Error in " << T << " after first integer";
        return true;
    }
    cline_last = scan_int(T);
    read_until(hash_table.relax_token);
    if (1 <= cline_first && cline_first <= cline_last) return false;
    errbuf.reset();
    errbuf << "Bad range in " << T << fmt::format(": {}-{}", cline_first, cline_last);
    return true;
}

// Now a function that parses the arguments, and returns a status
// 0:none, 1: hline, 2: cline,
// may set in_hlinee and hlinee_width

auto Parser::T_hline_parse(subtypes c) -> int {
    in_hlinee = false;
    Token T   = cur_tok;
    if (c == one_code) { // cline
        TokenList arg = read_arg();
        if (scan_pair_ints(T, arg)) {
            parse_error(errbuf.c_str());
            return 0;
        }
        return 2;
    }
    if (c != two_code) return 1;
    TokenList cols = read_arg();
    int       rt   = 1;
    if (!cols.empty()) {
        rt = 2;
        if (scan_pair_ints(T, cols)) {
            parse_error(errbuf.c_str());
            ignore_arg();
            ignore_arg();
            ignore_arg();
            return 0; // read the mandatory arguments before returning
        }
    }
    have_above  = false;
    have_below  = false;
    TokenList L = read_arg();
    if (!L.empty()) {
        ScaledInt tab_width = dimen_from_list(T, L);
        if (!tab_width.null()) {
            have_above   = true;
            hlinee_above = Istring(tab_width);
        }
    }
    L = read_arg();
    if (!L.empty()) {
        ScaledInt tab_width = dimen_from_list(T, L);
        if (!tab_width.null()) {
            have_below   = true;
            hlinee_below = Istring(tab_width);
        }
    }
    L                   = read_arg();
    ScaledInt tab_width = dimen_from_list(T, L);
    in_hlinee           = true;
    hlinee_width        = Istring(tab_width);
    return rt;
}

// Case of cline, placed after a row. If action is false, we check whether the
// row, including the spans of the cells, is adapted to the pattern.
// If action is true, we do something
auto Xml::try_cline(bool action) -> bool {
    auto a    = cline_first - 1;
    bool a_ok = false; // true after skip
    auto len  = size();
    for (size_t k = 0; k < len; k++) {
        if (a == 0) {
            if (a_ok) return true;
            a    = (cline_last - cline_first) + 1;
            a_ok = true;
        }
        if (at(k)->is_xmlc()) {
            Istring N = at(k)->name;
            if (N.name == "\n") continue; // allow newline separator
            return false;
        }
        auto c = at(k)->get_cell_span();
        if (c == -1) return false;
        if (c == 0) continue; // ignore null span cells
        if (a_ok && action) at(k)->id.add_bottom_rule();
        a = a - c;
        if (a < 0) return false;
    }
    return false; // list too small
}

// Puts the total span in res, return false in case of trouble
auto Xml::total_span(long &res) const -> bool {
    int  r   = 0;
    auto len = size();
    for (size_t k = 0; k < len; k++) {
        if (at(k)->is_xmlc()) {
            Istring N = at(k)->name;
            if (N.name == "\n") continue; // allow newline separator
            return false;
        }
        auto c = at(k)->get_cell_span();
        if (c == -1) return false;
        r += c;
    }
    res = r;
    return true;
}

// Special case when \\ started a row; we may have an empty cell followed by
// newline. Returns false if the cell is non-empty, has a span or a top border
// If action is true, we kill the cell/newline
auto Xml::try_cline_again(bool action) -> bool {
    bool seen_cell = false;
    auto len       = size();
    for (size_t k = 0; k < len; k++) {
        if (action) {
            erase(begin() + to_signed(k));
            --k;
            continue;
        }
        if (at(k)->is_xmlc() && k == len - 1) {
            Istring N = at(k)->name;
            if (N.name == "\n") continue;
            return false;
        }
        if (at(k)->get_cell_span() != 1) return false;
        if (!at(k)->id.has_attribute(the_names[np_topborder]).null()) return false;
        if (seen_cell) return false;
        if (!at(k)->is_whitespace()) return false;
        seen_cell = true;
    }
    return seen_cell;
}

// adds a span value of n to the current cell
void Xid::add_span(long n) const {
    if (n == 1) return;
    errbuf.reset();
    errbuf << std::to_string(n);
    add_attribute(the_names[np_cols], Istring(errbuf.to_string()));
}

// If the previous fails, we add a row of empty cells,
// This adds a-1 empty cells, then b cells with a bottom_border
void Stack::add_border(long a, long b) {
    push1(np_row);
    push_pop_cell(push_only);
    if (a != 1) {
        get_xid().add_span(a - 1);
        push_pop_cell(push_and_pop);
    }
    get_xid().add_span(b);
    get_xid().add_bottom_rule();
    push_pop_cell(pop_only);
    pop(np_row);
}

// This is now cline
void Parser::T_cline() {
    Xml *R       = the_stack.top_stack()->last_addr();
    auto cl_span = (cline_last - cline_first) + 1;
    if (R != nullptr) {
        if (R->try_cline(false)) {
            R->try_cline(true);
            return;
        }
        long tot_span = 0;
        if (R->total_span(tot_span)) {
            tot_span = cline_first - 1 - tot_span;
            if (0 <= tot_span) {
                if (tot_span != 0) {
                    Xml *x = new Xml(np_cell, nullptr);
                    R->push_back_unless_nullptr(x);
                    x->id.add_span(tot_span);
                }
                Xml *x = new Xml(np_cell, nullptr);
                R->push_back_unless_nullptr(x);
                x->id.add_span(cl_span);
                x->id.add_bottom_rule();
                return;
            }
        }
        if (!R->is_xmlc() && R->has_name(the_names[np_row])) {
            if (R->try_cline_again(false)) {
                R->try_cline_again(true);
                R->name = Istring(0UL);
                the_stack.add_border(cline_first, cl_span);
                the_log << "\\cline killed a cell \n";
                return;
            }
        }
    }
    the_stack.add_border(cline_first, cl_span);
}

// This implements \hline, its adds a top attribute to a row
void Stack::T_hline() {
    Xid cid, rid, tid;
    find_cid_rid_tid(cid, rid, tid);
    if (rid.has_attribute(the_names[np_topborder]).empty()) rid.add_top_rule();
}

// Case when the command is found outside the tabular loop.
// Is this legal? we parse arguments the check again
void Parser::T_hline(subtypes c) {
    Token T   = cur_tok;
    int   res = T_hline_parse(c);
    if (the_stack.in_array_mode() && the_stack.is_frame(np_array)) {
        if (res == 1)
            the_stack.T_hline();
        else if (res == 2)
            T_cline();
    } else
        parse_error(err_tok, "misplaced ", T, " should occur in tables only", "Current command should occur in tables only");
}

// This is executed when we see \end{tabular}
// If true, the \end{tabular} is not executed.
// and we have to push back the `\end{tabular}' tokens
auto Parser::false_end_tabular(const std::string &s) -> bool {
    if (the_stack.is_frame(np_cell)) {
        TokenList L = token_ns::string_to_list(s, true);
        back_input(L);
        back_input(hash_table.end_token);
        finish_a_cell(hash_table.cr_token, Istring());
        return true;
    }
    return false;
}

// This is the code of \end{tabular}
void Parser::T_end_tabular(subtypes c) {
    the_stack.pop(c == 0 ? np_tabular : np_tabular_star);
    the_stack.delete_table_atts();
    if (!the_stack.is_float()) leave_v_mode();
}

//
void Parser::T_endv() {
    if (the_stack.is_frame(np_cell)) {
        flush_buffer();
        the_stack.finish_cell(-1);
        the_stack.push_pop_cell(pop_only);
        pop_level(bt_cell);
        start_a_cell(false);
    } else
        parse_error("Bad endv token");
}

// This is done when we see a \\.
void Parser::T_cr() {
    flush_buffer();
    long a = 0;
    if (cur_cmd_chr.chr == crwithargs_code) a = scan_int(cur_tok);
    if (!the_stack.is_frame(np_cell)) {
        parse_error("bad \\cr");
        return;
    }
    the_stack.finish_cell(0);
    the_stack.push_pop_cell(pop_only);
    pop_level(bt_cell);
    the_stack.pop(np_row);
    start_a_row(a);
}

// Case of \multispan; fully expandable
void Parser::E_multispan() {
    Token omit = hash_table.locate("omit");
    Token span = hash_table.locate("span");
    auto  n    = scan_int(cur_tok);
    back_input(omit);
    while (n > 1) {
        back_input(span);
        back_input(omit);
        --n;
    }
}
