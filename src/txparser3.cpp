// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains a part of the TeX parser of tralics

#include "tralics/Parser.h"
#include "tralics/SaveAux.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/format.h>

namespace parser_ns {
    auto to_string(boundary_type v) -> String;
} // namespace parser_ns

namespace {
    std::vector<std::unique_ptr<SaveAux>> the_save_stack;
} // namespace

// --------------------------------------------------

auto to_string(save_type v) -> String { // \todo std::string
    switch (v) {
    case st_boundary: return "boundary";
    case st_cmd: return "command";
    case st_int: return "integer";
    case st_dim: return "dimension";
    case st_glue: return "glue";
    case st_token: return "token";
    case st_save: return "save";
    case st_env: return "environment";
    case st_font: return "font";
    case st_box: return "box";
    case st_box_end: return "end of box";
    case st_string: return "string";
    default: return "unknown";
    }
}

auto gbl_or_assign(bool gbl, bool re) -> String {
    if (gbl) return "globally changing ";
    if (re) return "reassigning ";
    return "changing ";
}

Parser::Parser() : cur_env_name("document") {
    sectionning_offset                 = section_code;
    restricted                         = false;
    cur_level                          = 1;
    calc_loaded                        = false;
    cur_in_chan                        = main_in_chan;
    long_state                         = ls_long;
    scanner_status                     = ss_normal;
    list_files_p                       = false;
    allocation_table[newcount_code]    = 20;
    allocation_table[newdimen_code]    = 10;
    allocation_table[newlength_code]   = 10;
    allocation_table[newmuskip_code]   = 10;
    allocation_table[newbox_code]      = 10;
    allocation_table[newtoks_code]     = 10;
    allocation_table[newread_code]     = 0;
    allocation_table[newwrite_code]    = 0;
    allocation_table[newlanguage_code] = 10;
    cur_font.pack();
    cur_font.set_old_from_packed();
}

// saving and restoring things

// Implements \currentgrouptype.
// (see online doc for the meaning of the numbers).
auto Parser::cur_group_type() -> int {
    boundary_type V = first_boundary();
    switch (V) {
    case bt_impossible: return 0;
    case bt_brace: return 1;
    case bt_math: return 9;
    case bt_semisimple: return 14;
    case bt_esemisimple: return 17;
    case bt_env: return 18;
    case bt_cell: return 19;
    case bt_local: return 20;
    case bt_tpa: return 21;
    default: return 17;
    }
}

auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream & {
    fp << bt_to_string(x);
    return fp;
}

// This adds a new element to the save stack.
void Parser::push_save_stack(SaveAux *v) {
    my_stats.one_more_up();
    the_save_stack.emplace_back(v);
}

// This is done when we evaluate { or \begingroup.
void Parser::push_level(boundary_type v) {
    push_save_stack(new SaveAuxBoundary(*this, v));
    cur_level++;
    if (tracing_stack()) {
        Logger::finish_seq();
        the_log << "+stack: level + " << cur_level << " for " << v << " entered on line " << get_cur_line() << "\n";
    }
}

void Parser::push_tpa() {
    push_save_stack(new SaveAuxBoundary(*this, bt_tpa));
    if (tracing_stack()) {
        Logger::finish_seq();
        the_log << "+stack: level = " << cur_level << " for " << bt_tpa << "\n";
    }
}

// Defines something at position A, with type and subtype B and C.
// Definition is global if gbl is true. In the case of a user defined
// command, we must increase the reference count, and if we override
// a user defined command we must decrease its reference count (maybe kill).
// If the definition is local, the old definition is saved on the save stack.
void Parser::eq_define(size_t a, CmdChr bc, bool gbl) {
    if (bc.is_user()) mac_table.incr_macro_ref(bc.chr);
    if (!gbl && hash_table.eqtb[a].must_push(cur_level))
        push_save_stack(new SaveAuxCmd(*this, a, hash_table.eqtb[a]));
    else if (hash_table.eqtb[a].is_user())
        mac_table.delete_macro_ref(hash_table.eqtb[a].chr);
    if (gbl)
        hash_table.eqtb[a].primitive(bc);
    else
        hash_table.eqtb[a].set(bc, cur_level);
}

// This defines a user command, associated to the token A.
// Depending of redef, this may be valid or not
// (implements \provideXX, \newXX and \renewXX)
// We either kill the macro, or increment its reference count.
void Parser::mac_define(Token a, Macro *b, bool gbl, rd_flag redef, symcodes what) {
    if (ok_to_define(a, redef)) {
        auto nv = CmdChr(what, subtypes(mac_table.mc_new_macro(b)));
        if (tracing_assigns()) {
            Logger::finish_seq();
            the_log << "{" << gbl_or_assign(gbl, false) << a << "=";
            token_for_show(hash_table.eqtb[a.eqtb_loc()]);
            the_log << "}\n{into " << a << "=";
            token_for_show(nv);
            the_log << "}\n";
        }
        eq_define(a.eqtb_loc(), nv, gbl);
    } else
        delete b;
}

// For \newcommand, \renewcommand: we signal an error if we cannot define
// the thing. For \providecommand: the function returns true if we have
// to define, false otherwise.
auto Parser::ok_to_define(Token a, rd_flag redef) -> bool {
    if (redef == rd_never) return false;
    if (a.not_a_cmd()) return false; // Should not happen
    if (a == hash_table.frozen_protection) return false;
    if (redef == rd_always) return true;
    auto A              = a.eqtb_loc();
    bool undef_or_relax = hash_table.eqtb[A].is_undef_or_relax();
    if (redef == rd_if_defined && undef_or_relax) {
        bad_redefinition(1, a);
        return false;
    }
    if (redef == rd_if_undef && !undef_or_relax) {
        bad_redefinition(0, a);
        return false;
    }
    return !(redef == rd_skip && !undef_or_relax);
}

// Define for an integer quantity. Like eq_define without reference counts.
void Parser::word_define(size_t a, long c, bool gbl) {
    EqtbInt &W        = eqtb_int_table[a];
    bool     reassign = !gbl && W.val == c;
    if (tracing_assigns()) {
        CmdChr tmp(assign_int_cmd, subtypes(a));
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, reassign) << "\\" << tmp.name() << "=" << W.val;
        if (!reassign) the_log << " into \\" << tmp.name() << "=" << c;
        the_log << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else if (reassign)
        return;
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxInt(*this, W.level, a, W.val));
        W = {c, cur_level};
    }
}
// Define for an string quantity. Like eq_define without reference counts.
void Parser::string_define(size_t a, const std::string &c, bool gbl) {
    EqtbString &W        = eqtb_string_table[a];
    bool        reassign = !gbl && W.val == c;
    if (tracing_assigns()) {
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, reassign) << save_string_name(a) << "=" << W.val;
        if (!reassign) the_log << " into " << save_string_name(a) << "=" << c;
        the_log << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else if (reassign)
        return;
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxString(*this, W.level, a, W.val));
        W = {c, cur_level};
    }
}

// Define for a dimension quantity. Like eq_define without reference counts.
void Parser::dim_define(size_t a, ScaledInt c, bool gbl) {
    EqtbDim &W        = eqtb_dim_table[a];
    bool     reassign = !gbl && W.val == c;
    if (tracing_assigns()) {
        CmdChr tmp(assign_dimen_cmd, subtypes(a));
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, reassign) << "\\" << tmp.name() << "=" << W.val;
        if (!reassign) the_log << " into \\" << tmp.name() << "=" << c;
        the_log << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else if (reassign)
        return;
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxDim(*this, W.level, a, W.val));
        W = {c, cur_level};
    }
}

auto operator==(const Glue &a, const Glue &b) -> bool {
    return a.get_width() == b.get_width() && a.get_shrink() == b.get_shrink() && a.get_stretch() == b.get_stretch() &&
           a.get_shrink_order() == b.get_shrink_order() && a.get_stretch_order() == b.get_stretch_order();
}

// Define for a glue quantity
void Parser::glue_define(size_t a, Glue c, bool gbl) {
    EqtbGlue &W        = glue_table[a];
    bool      reassign = !gbl && W.val == c;
    if (tracing_assigns()) {
        CmdChr tmp(assign_glue_cmd, subtypes(a));
        Thbuf1 << bf_reset << W.val;
        if (a >= thinmuskip_code) Thbuf1.pt_to_mu();
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, reassign) << "\\" << tmp.name() << "=" << Thbuf1;
        if (!reassign) {
            Thbuf1 << bf_reset << c;
            if (a >= thinmuskip_code) Thbuf1.pt_to_mu();
            the_log << " into \\" << tmp.name() << "=" << Thbuf1;
        }
        the_log << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else if (reassign)
        return;
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxGlue(*this, W.level, a, W.val));
        W = {c, cur_level};
    }
}

// Define for a box quantity
void Parser::box_define(size_t a, Xml *c, bool gbl) {
    EqtbBox &W = box_table[a];
    if (tracing_assigns()) {
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, false) << "\\box" << a << "=" << W.val;
        the_log << " into \\box" << a << "=" << c << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxBox(*this, W.level, a, W.val));
        W = {c, cur_level};
    }
}

// Same code for a token list.
void Parser::token_list_define(size_t p, TokenList &c, bool gbl) {
    EqtbToken &W        = toks_registers[p];
    bool       reassign = !gbl && W.val == c;
    if (tracing_assigns()) {
        CmdChr tmp(assign_toks_cmd, subtypes(p));
        Logger::finish_seq();
        the_log << "{" << gbl_or_assign(gbl, reassign) << "\\" << tmp.name() << "=" << W.val;
        if (!reassign) the_log << " into \\" << tmp.name() << "=" << c;
        the_log << "}\n";
    }
    if (gbl)
        W = {c, 1};
    else if (reassign)
        return;
    else {
        if (W.must_push(cur_level)) push_save_stack(new SaveAuxToken(*this, W.level, p, W.val));
        W = {c, cur_level};
    }
}

// This is called whenever a font has changed.
// It pushes the value on the save stack if needed.
void Parser::save_font() {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{font change " << cur_font << "}\n";
    }
    if (cur_font.level == cur_level) return;
    push_save_stack(new SaveAuxFont(*this, cur_font.level, cur_font.old, cur_font.old_color));
    cur_font.set_level(cur_level);
}

// returns the type of the first boundary
// sets first_boundary_loc if a boundary has been seen.

static int first_boundary_loc = 0;
auto       Parser::first_boundary() -> boundary_type {
    auto n = the_save_stack.size();
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        if (p->type != st_boundary) continue;
        first_boundary_loc = p->line;
        return dynamic_cast<SaveAuxBoundary *>(p)->val;
    }
    return bt_impossible;
}

// case where a table preamble says  >{}c<{xx$yy} and we see &
// here xy can be } or \endgroup
auto Parser::stack_math_in_cell() -> bool {
    auto n     = the_save_stack.size();
    bool first = true;
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        if (p->type != st_boundary) continue;
        boundary_type cur = dynamic_cast<SaveAuxBoundary *>(p)->val;
        if (cur == bt_brace || cur == bt_semisimple) continue;
        if (first) {
            if (cur != bt_math) return false;
            first = false;
            continue;
        }
        return cur == bt_cell;
    }
    return false;
}

void Parser::dump_save_stack() const {
    int  L = cur_level - 1;
    auto n = the_save_stack.size();
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        if (p->type != st_boundary) continue;
        dynamic_cast<SaveAuxBoundary *>(p)->dump(L);
        --L;
    }
    the_log << "### bottom level\n";
}

// The function called when we see a closing brace or \endgroup.
// \end{foo} expands to \endfoo\endenv\endfoo, the last \endfoo is in cur_tok
// there should be an \endfoo token in cur_tok
void Parser::pop_level(boundary_type v) {
    bool          must_throw = false;
    boundary_type w          = first_boundary();
    if (w == bt_impossible) {
        if (v == bt_brace)
            parse_error(err_tok, "Extra }");
        else if (v == bt_semisimple)
            parse_error(err_tok, "Extra \\endgroup");
        else
            parse_error(err_tok, "Empty save stack");
        return;
    }
    if (v != w) {
        if (v == bt_brace && w == bt_tpa)
            must_throw = true;
        else if (w == bt_env) {
            if (v == bt_semisimple) v = bt_esemisimple;
            err_buf << bf_reset << "Extra " << bt_to_string(v) << " found in unclosed environment " << cur_env_name;
            signal_error(err_tok, "extra brace");
            return;
        } else {
            if (w == bt_semisimple) w = bt_esemisimple;
            if (v == bt_semisimple) v = bt_esemisimple;
            wrong_pop(err_tok, bt_to_string(v), bt_to_string(w));
        }
    }
    if (v == bt_env && cur_tok.is_valid()) {
        std::string foo = cur_tok.tok_to_str();
        if (foo != "\\end" + cur_env_name) {
            err_buf << bf_reset << fmt::format("Environment '{}' started at line {}", cur_env_name, first_boundary_loc) << " ended by "
                    << cur_tok;
            signal_error(err_tok, "bad end env");
        }
        // this is the wrong env, we nevertheless pop
    }
    for (;;) {
        if (the_save_stack.empty()) {
            parse_error(err_tok, "Internal error: empty save stack");
            return;
        }
        bool ok = the_save_stack.back()->type == st_boundary;
        my_stats.one_more_down();
        the_save_stack.pop_back();
        if (ok) {
            if (must_throw) {
                cur_level++;
                throw EndOfData();
            }
            return;
        }
    }
}

// Signals error for unclosed environments and braces
void Parser::pop_all_levels() {
    cur_tok.kill();
    pop_level(bt_env); // pop the end document
    bool        started = false;
    std::string ename;
    Buffer &    B = err_buf;
    B.clear();
    for (;;) {
        if (the_save_stack.empty()) break;
        SaveAux *tmp = the_save_stack.back().get();
        std::cout << to_string(tmp->type) << " at " << tmp->line << "\n";
        if (tmp->type == st_env) {
            auto *q = dynamic_cast<SaveAuxEnv *>(tmp);
            ename   = q->name;
        }
        if (tmp->type == st_boundary) {
            boundary_type w = dynamic_cast<SaveAuxBoundary *>(tmp)->val;
            int           l = tmp->line;
            if (started) {
                B << ".\n"; // finish prev line
                main_ns::nb_errs++;
            }
            started = true;
            B << "Non-closed " << bt_to_string(w);
            if (w == bt_env) B << " `" << ename << "'";
            B << fmt::format(" started at line {}", l);
        }
        my_stats.one_more_down();
        the_save_stack.pop_back();
    }
    if (started) {
        signal_error(Token(), "");
        B << ".\n";
        out_warning(B, mt_none); // insert a warning in the XML if desired
    }
    push_level(bt_env);
}

// This is done when all is finished.
// We try to pop a font command
void Parser::final_checks() {
    conditions.terminate();
    auto n = the_save_stack.size();
    if (n == 1) {
        if (the_save_stack.back()->type == st_font) {
            my_stats.one_more_down();
            the_save_stack.pop_back();
            n = the_save_stack.size();
        }
    }
    if (n == 0) return;
    the_log << "Number of items on the save stack: " << n << "\n";
    Buffer &B = err_buf;
    B.clear();
    Buffer &A = Thbuf1;
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        A.clear();
        A << fmt::format("{} at {}", to_string(p->type), p->line);
        if (B.empty()) {
            B << "  " << A;
        } else if (B.size() + A.size() < 78) {
            B << "; " << A;
        } else {
            the_log << B << "\n";
            B.clear();
            B << "  " << A;
        }
    }
    the_log << B << ".\n";
}

// Returns the slot associated to the env S
auto Parser::is_env_on_stack(const std::string &s) -> SaveAuxEnv * {
    auto n = the_save_stack.size();
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        if (p->type != st_env) continue;
        auto *q = dynamic_cast<SaveAuxEnv *>(p);
        if (q->name == s) return q;
    }
    return nullptr;
}

// Returns the number of environments
auto Parser::nb_env_on_stack() -> int {
    auto n = the_save_stack.size();
    int  k = 0;
    for (size_t i = n; i > 0; i--) {
        SaveAux *p = the_save_stack[i - 1].get();
        if (p->type == st_env) ++k;
    }
    return k;
}

// ----------------------------------------------------------------------
// TIPA macros
void Parser::T_ipa(subtypes c) {
    if (c == 0) tipa_star();
    if (c == 1) tipa_semi();
    if (c == 2) tipa_colon();
    if (c == 3) tipa_exclam();
    if (c == 4) tipa_normal();
}

void Parser::mk_hi(String x, char c) {
    the_parser.LC();
    the_parser.unprocessed_xml << "<hi rend='" << x << "'>" << c << "</hi>";
}

void Parser::tipa_star() {
    if (get_token()) return; // should not happen
    int cmd = cur_cmd_chr.cmd;
    if (cmd == 12 || cmd == 11) {
        int n = cur_cmd_chr.chr;
        if (n > 0 && n < 128) {
            if (n == 'k') {
                extended_chars(0x29e);
                return;
            }
            if (n == 'f') {
                mk_hi("turned", 'f');
                return;
            }
            if (n == 't') {
                extended_chars(0x287);
                return;
            }
            if (n == 'r') {
                extended_chars(0x279);
                return;
            }
            if (n == 'w') {
                extended_chars(0x28d);
                return;
            }
            if (n == 'j') {
                extended_chars(0x25f);
                return;
            }
            if (n == 'n') {
                extended_chars(0x272);
                return;
            }
            if (n == 'h') {
                extended_chars(0x127);
                return;
            } // 0x45b ?
            if (n == 'l') {
                extended_chars(0x26c);
                return;
            }
            if (n == 'z') {
                extended_chars(0x26e);
                return;
            }
        }
    }
    back_input(cur_tok);
}

void Parser::tipa_semi() {
    if (get_token()) return; // should not happen
    int cmd = cur_cmd_chr.cmd;
    if (cmd == 12 || cmd == 11) {
        int n = cur_cmd_chr.chr;
        if (n > 0 && n < 128) {
            if (n == 'E' || n == 'J' || n == 'A' || n == 'U') {
                mk_hi("sc", char(n));
                return;
            }
            if (n == 'H') {
                extended_chars(0x29c);
                return;
            }
            if (n == 'L') {
                extended_chars(0x29f);
                return;
            }
            if (n == 'B') {
                extended_chars(0x299);
                return;
            }
            if (n == 'G') {
                extended_chars(0x262);
                return;
            }
            if (n == 'N') {
                extended_chars(0x274);
                return;
            }
            if (n == 'R') {
                extended_chars(0x280);
                return;
            }
        }
    }
    back_input(cur_tok);
}

void Parser::tipa_colon() {
    if (get_token()) return; // should not happen
    int cmd = cur_cmd_chr.cmd;
    if (cmd == 12 || cmd == 11) {
        int n = cur_cmd_chr.chr;
        if (n > 0 && n < 128) {
            if (n == 'd') {
                extended_chars(0x256);
                return;
            }
            if (n == 'l') {
                extended_chars(0x26d);
                return;
            }
            if (n == 'n') {
                extended_chars(0x273);
                return;
            }
            if (n == 'r') {
                extended_chars(0x27d);
                return;
            }
            if (n == 'R') {
                extended_chars(0x27b);
                return;
            }
            if (n == 's') {
                extended_chars(0x282);
                return;
            }
            if (n == 't') {
                extended_chars(0x288);
                return;
            }
            if (n == 'z') {
                extended_chars(0x290);
                return;
            }
        }
    }
    back_input(cur_tok);
}

void Parser::tipa_exclam() {
    if (get_token()) return; // should not happen
    int cmd = cur_cmd_chr.cmd;
    if (cmd == 12 || cmd == 11) {
        int n = cur_cmd_chr.chr;
        if (n > 0 && n < 128) {
            if (n == 'G') {
                extended_chars(0x29b);
                return;
            }
            if (n == 'b') {
                extended_chars(0x253);
                return;
            }
            if (n == 'd') {
                extended_chars(0x257);
                return;
            }
            if (n == 'g') {
                extended_chars(0x260);
                return;
            }
            if (n == 'j') {
                extended_chars(0x284);
                return;
            }
            if (n == 'o') {
                extended_chars(0x298);
                return;
            }
        }
    }
    back_input(cur_tok);
}

void Parser::tipa_normal() {
    if (get_token()) return; // should not happen
    int cmd = cur_cmd_chr.cmd;
    if (cmd == 12 || cmd == 11) {
        int n = cur_cmd_chr.chr;
        if (n > 0 && n < 128) {
            if (n == '0') {
                extended_chars(0x289);
                return;
            }
            if (n == '1') {
                extended_chars(0x268);
                return;
            }
            if (n == '2') {
                extended_chars(0x28c);
                return;
            }
            if (n == '3') {
                extended_chars(0x25c);
                return;
            }
            if (n == '4') {
                extended_chars(0x265);
                return;
            }
            if (n == '5') {
                extended_chars(0x250);
                return;
            }
            if (n == '6') {
                extended_chars(0x252);
                return;
            }
            if (n == '7') {
                extended_chars(0x264);
                return;
            }
            if (n == '8') {
                extended_chars(0x275);
                return;
            }
            if (n == '9') {
                extended_chars(0x258);
                return;
            }
            if (n == '@') {
                extended_chars(0x259);
                return;
            }
            if (n == 'A') {
                extended_chars(0x251);
                return;
            }
            if (n == 'B') {
                extended_chars(0x3b2);
                return;
            } // beta
            if (n == 'C') {
                extended_chars(0x255);
                return;
            }
            if (n == 'D') {
                extended_chars(0x0f0);
                return;
            } // eth
            if (n == 'E') {
                extended_chars(0x25B);
                return;
            }
            if (n == 'F') {
                extended_chars(0x278);
                return;
            }
            if (n == 'G') {
                extended_chars(0x263);
                return;
            }
            if (n == 'H') {
                extended_chars(0x266);
                return;
            }
            if (n == 'I') {
                extended_chars(0x26A);
                return;
            }
            if (n == 'J') {
                extended_chars(0x29D);
                return;
            }
            if (n == 'K') {
                extended_chars(0x261);
                return;
            }
            if (n == 'L') {
                extended_chars(0x28E);
                return;
            }
            if (n == 'M') {
                extended_chars(0x271);
                return;
            }
            if (n == 'N') {
                extended_chars(0x14b);
                return;
            } // eng
            if (n == 'O') {
                extended_chars(0x254);
                return;
            }
            if (n == 'P') {
                extended_chars(0x294);
                return;
            }
            if (n == 'Q') {
                extended_chars(0x295);
                return;
            }
            if (n == 'R') {
                extended_chars(0x27E);
                return;
            }
            if (n == 'S') {
                extended_chars(0x283);
                return;
            }
            if (n == 'T') {
                extended_chars(0x3b8);
                return;
            } // theta
            if (n == 'U') {
                extended_chars(0x28a);
                return;
            }
            if (n == 'V') {
                extended_chars(0x28b);
                return;
            }
            if (n == 'W') {
                extended_chars(0x26f);
                return;
            }
            if (n == 'X') {
                extended_chars(0x3c7);
                return;
            } // chi
            if (n == 'Y') {
                extended_chars(0x28f);
                return;
            }
            if (n == 'Z') {
                extended_chars(0x292);
                return;
            }
        }
    }
    back_input(cur_tok);
}

// Unicode characters not in the tables above.
// 25a 25d 25e
// 262 267 269 26b
// 270 276 277 27a 27c 27f
// 281 285 286
// 291 x293 296 297 29a
// 2a0 2a1 2a2 2a3 2a4 2a5 2a6 2a7 2a8 2a9 2aa 2ab 2ac 2ad 2ae 2af
