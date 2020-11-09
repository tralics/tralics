#include "tralics/NewArray.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace {
    char char_for_error = 'c';
} // namespace

void NewArray::boot(Parser *Q) {
    P = Q;
    for (unsigned i = 0; i < nb_newcolumn; i++) {
        nct_bool[i] = false;
        nct_tok[i]  = hash_table.relax_token;
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
    if (current_token.cmd_val() == 11 || current_token.cmd_val() == 12 || current_token.cmd_val() == 13) c = current_token.char_val();
    if (c == 'c') {
        cur_h_pos = "cell_center";
        ch_num    = chn_c;
        return;
    }
    if (c == 'l') {
        cur_h_pos = "cell_left";
        ch_num    = chn_l;
        return;
    }
    if (c == 'r') {
        cur_h_pos = "cell_right";
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
        std::string        x = P->nT_arg_nopar();
        const std::string &s = x;
        if (s != "1") id.add_attribute(the_names["cols"], x);
    }
    preamble = P->read_arg(); // read the preamble
    preamble.expand_star();
    P->expand_nct(preamble);
    if (main_fct) AI = &the_stack.new_array_info(id);
    u_list.clear();
    v_list.clear();
    ac_cell_no    = 0;
    last_ch_class = chc_start;
    first_bar     = true;
    bool seen     = false;
    cur_h_pos     = "cell_center";
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
            attribs.push_back(the_names["halign"], the_names[cur_h_pos], false); // set attributes
            cur_h_pos = "cell_center";                                           // add a default
            current_list.clear();
            break;
        case chc_bar:
            if (ch_num == chn_c) {
                if (last_ch_class == chc_start)
                    attribs.push_back(the_names["cell_leftborder"], the_names["true"]);
                else if (last_ch_class == chc_bar) {
                } else
                    attribs.push_back(the_names["cell_rightborder"], the_names["true"], false);
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
        errbuf = "Array preamble: argument missing for ";
        errbuf += char_for_error;
        P->parse_error(errbuf);
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
    attribs.clear();
    TokenList cell = P->read_arg();
    cell.splice(cell.begin(), u_list);
    cell.splice(cell.end(), v_list);
    u_list.clear();
    v_list.clear();
    cell.brace_me();
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
        attribs.clear();
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
    if (current_token == hash_table.verb_token) {
        preamble.pop_front();
        return true;
    }
    if (current_token.is_a_left_brace()) {
        current_list = preamble.fast_get_block();
        token_ns::remove_ext_braces(current_list);
        have_token_list = true;
    } else {
        have_token_list = false;
        preamble.pop_front();
    }
    return false;
}
