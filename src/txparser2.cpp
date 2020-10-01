// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2002-2011.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains the TeX parser of tralics It contains a lot of other
// things, but not the XML generator.

#include "txparser2.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Saver.h"
#include "tralics/util.h"
#include "txparam.h"
#include <fmt/format.h>

namespace {
    // The  date parser
    class FormatDate { // \todo c++ standard class for date?
        Token       err_tok;
        auto        scan_a_field(Buffer &B, int &res) -> bool;
        static auto scan_a_month(Buffer &B, int &res) -> bool;
        auto        scan_next(Buffer &B, int &res) -> bool;
        auto        sort() -> bool;
        static auto next_format_char(Buffer &B) -> int;
        auto        parse_format(Buffer &B) -> bool;
        auto        parse(Buffer &B) -> bool;

    public:
        int month{0}, day{0}, year{0};

        auto interpret(const std::string &s, Token T) -> bool;
    };

    Buffer      local_buf, mac_buf, buf_for_del;
    TokenList   KV_list;
    bool        xkv_is_global;
    bool        xkv_is_save;
    std::string xkv_prefix;
    std::string xkv_header;
    TokenList   xkv_action;

    // return non-empty string only if section is new
    auto check_section(const std::string &s) -> std::string {
        static long cur_section = -1;
        long        k           = -1;
        err_buf.clear();
        std::vector<ParamDataSlot> &X = config_data.data[1]->data;
        auto                        n = X.size(); // number of sections
        if (s.empty())
            k = cur_section;
        else
            for (size_t i = 0; i < n; i++)
                if (X[i].key == s) {
                    k = to_signed(i + 1);
                    break;
                }
        if (k > 0 && k < cur_section) {
            err_buf.format("Bad section {} after {}\nOrder of sections is{}", s, X[to_unsigned(cur_section - 1)].key, sec_buffer);
            the_parser.signal_error();
        } else if (k == -1) {
            if (n == 0) {
                the_parser.parse_error("Illegal access to fullsection list.");
                return "";
            }
            if (!s.empty()) {
                err_buf.format("Invalid section {}\nValid sections are{}", s, sec_buffer);
                the_parser.signal_error();
                if (cur_section < 0) cur_section = 1;
            }
        } else
            cur_section = k;
        if (cur_section < 0) {
            the_parser.parse_error("No default section");
            cur_section = 1;
        }
        if (cur_section == composition_section) {
            static bool first_module = true;
            if (first_module)
                first_module = false;
            else {
                the_parser.parse_error("Only one module accepted in composition");
                cur_section++;
                return "";
            }
        }
        static long prev = -1;
        if (prev == cur_section) return "";
        prev             = cur_section;
        cur_sec_no_topic = X[to_unsigned(cur_section - 1)].no_topic();
        X[to_unsigned(cur_section - 1)].mark_used(); // incompatible with topics
        return X[to_unsigned(cur_section - 1)].value;
    }

    auto is_good_ur(const std::string &x) -> bool {
        std::vector<ParamDataSlot> &ur_list = config_data.data[0]->data;
        auto                        n       = ur_list.size();
        if (ur_size == 0) {
            for (size_t i = 0; i < n; i++) ur_list[i].mark_used();
            ur_size = n;
        }
        for (size_t i = 0; i < n; i++)
            if (ur_list[i].matches(x)) return true;
        return false;
    }

    // Interprets the RC argument of a pers command \todo RA
    // This returns the short name, said otherwise, the argument.
    // Notice the space case when argument is empty, or +foo or =foo.
    auto pers_rc(const std::string &rc) -> std::string {
        if (rc.empty()) {
            if (have_default_ur) return the_default_rc;
            if (the_main->handling_ra && the_parser.get_ra_year() > 2006) {
                // signal error, make a default
                the_parser.parse_error("No default Research Centre defined");
                the_default_rc = "unknown";
            }
            have_default_ur = true;
            return the_default_rc;
        }
        if (rc[0] == '+') { return rc.substr(1); }
        bool spec = (rc.size() >= 2 && rc[0] == '=');
        auto RC   = spec ? rc.substr(1) : rc;
        if (!is_good_ur(RC)) {
            err_buf                       = "Invalid Unit Centre " + rc + "\nUse one of:";
            std::vector<ParamDataSlot> &V = config_data.data[0]->data;
            for (auto &i : V)
                if (i.is_used) err_buf += " " + i.key;
            the_parser.signal_error(the_parser.err_tok, "illegal data");
        }
        if (spec) {
            the_default_rc  = RC;
            have_default_ur = true;
        }
        return RC;
    }

    // Special command. We assume that cur_sec_no_topic
    // is correctlty set.
    auto check_spec_section(const std::string &s) -> std::string {
        if (cur_sec_no_topic) return "";
        if (s.empty()) return "default";
        return s;
    }

    // Return the value of the key in a list.
    auto find_one_key(const std::string &name, const std::string &key) -> std::string {
        if (name == "ur") return pers_rc(key);
        if (name == "theme") return MainClass::check_theme(key);
        if (name == "fullsection") return check_section(key);
        if (name == "section") return check_spec_section(key);
        ParamDataList *X = config_data.find_list(name, false);
        if (X == nullptr) {
            the_parser.parse_error(the_parser.err_tok, "Configuration file does not define ", name, "no list");
            return "";
        }
        auto n = X->size();
        for (size_t i = 0; i < n; i++)
            if (X->data[i].key == key) return X->data[i].value;
        err_buf = fmt::format("Illegal value '{}' for {}\nUse one of:", key, name);
        X->keys_to_buffer(err_buf);
        the_parser.signal_error(the_parser.err_tok, "illegal data");
        return "";
    }

    // Converts the whole data struture as foo1=bar1,foo2=bar2,
    auto find_keys(const std::string &name) -> std::string {
        ParamDataList *X = config_data.find_list(name, false); // \todo optional?
        if (X == nullptr) return "";
        std::string res;
        for (const auto &d : X->data) res.append(fmt::format("{}={},", d.key, d.value));
        if (!X->empty()) res.pop_back();
        return res;
    }
} // namespace

namespace xkv_ns {
    void find_aux(int c);
    auto find_key_of(const TokenList &L, int type) -> std::string;
    void merge(TokenList &W, TokenList &L, int type);
    void remove(TokenList &W, TokenList &L, int type);
    void makehd(const std::string &fam);
    auto is_Gin(const TokenList &x) -> bool;
} // namespace xkv_ns

namespace token_ns {
    void lower_case(TokenList &L);
    auto find_in(TokenList &A, TokenList &B, Token t, bool sw, int &n) -> bool;
    void int_to_roman(Buffer &b, long n);
} // namespace token_ns

namespace classes_ns {
    auto cur_options(bool, TokenList &, bool) -> TokenList;
    void register_key(const std::string &);
    void unknown_optionX(TokenList &cur_keyval, TokenList &action);
} // namespace classes_ns

// Optimised version, because \zap@space 12 3 {44} 5 6\@empty
// is wrong after all
void Parser::E_zapspace() {
    TokenList result;
    for (;;) {
        TokenList L = read_until_nopar(hash_table.space_token);
        result.splice(result.end(), L);
        if (get_token()) break;
        if (cur_tok == hash_table.empty_token) break;
        back_input();
    }
    back_input(result);
}

// \fancyinternal
void Parser::T_xfancy() {
    std::string s = nT_arg_nopar();
    the_stack.push1(the_names["headings"]);
    the_stack.add_att_to_last(the_names["type"], s);
    the_stack.set_arg_mode();
    T_arg();
    flush_buffer();
    the_stack.pop(the_names["headings"]);
}

// Stuff for fancyheadings
void Parser::T_fancy(String s, TokenList &L) {
    flush_buffer();
    push_level(bt_brace);
    hash_table.eval_let("thepage", "inert@thepage");
    back_input(hash_table.CB_token);
    TokenList tmp = L; // make a copy of the list
    brace_me(tmp);
    back_input(tmp);
    TokenList sl = token_ns::string_to_list(s, true);
    back_input(sl);
    back_input(hash_table.locate("fancyinternal"));
}

void Parser::T_fancy() {
    subtypes c  = cur_cmd_chr.chr;
    int      hf = 0;
    if (c == fancy_head_code)
        hf = 1;
    else if (c == fancy_foot_code)
        hf = 2;
    else if (c == fancy_hf_code)
        hf = 3;
    if (hf == 0) {
        TokenList L1;
        bool      o  = read_optarg_nopar(L1);
        TokenList L2 = read_arg();
        if (!o) L1 = L2;
        if (c == fancy_lhead_code) {
            T_fancy("elh", L1);
            T_fancy("olh", L2);
            return;
        }
        if (c == fancy_chead_code) {
            T_fancy("ech", L1);
            T_fancy("och", L2);
            return;
        }
        if (c == fancy_rhead_code) {
            T_fancy("erh", L1);
            T_fancy("orh", L2);
            return;
        }
        if (c == fancy_lfoot_code) {
            T_fancy("elf", L1);
            T_fancy("olf", L2);
            return;
        }
        if (c == fancy_cfoot_code) {
            T_fancy("ecf", L1);
            T_fancy("ocf", L2);
            return;
        }
        if (c == fancy_rfoot_code) {
            T_fancy("erf", L1);
            T_fancy("orf", L2);
            return;
        }
    }
    TokenList L1;
    read_optarg_nopar(L1);
    TokenList L = read_arg();
    for (;;) {
        bool use_e = false, use_o = false;
        bool use_h = false, use_f = false;
        bool use_l = false, use_r = false, use_c = false;
        for (;;) {
            if (L1.empty()) break;
            Token t = L1.front();
            L1.pop_front();
            if (t.is_comma_token()) break;
            if (t.no_case_letter('e')) {
                use_e = true;
            } else if (t.no_case_letter('o')) {
                use_o = true;
            } else if (t.no_case_letter('l')) {
                use_l = true;
            } else if (t.no_case_letter('c')) {
                use_c = true;
            } else if (t.no_case_letter('r')) {
                use_r = true;
            } else if (t.no_case_letter('h')) {
                use_h = true;
            } else if (t.no_case_letter('f')) {
                use_f = true;
            } else
                continue;
        }
        if (!use_e && !use_o) {
            use_e = true;
            use_o = true;
        }
        if (hf == 1) use_h = true;
        if (hf == 2) use_f = true;
        if (!use_h && !use_f) {
            use_h = true;
            use_f = true;
        }
        if (!use_l && !use_c && !use_r) {
            use_l = true;
            use_c = true;
            use_r = true;
        }
        if (use_h) {
            if (use_e) {
                if (use_l) T_fancy("elh", L);
                if (use_c) T_fancy("ech", L);
                if (use_r) T_fancy("erh", L);
            }
            if (use_o) {
                if (use_l) T_fancy("olh", L);
                if (use_c) T_fancy("och", L);
                if (use_r) T_fancy("orh", L);
            }
        }
        if (use_f) {
            if (use_e) {
                if (use_l) T_fancy("elf", L);
                if (use_c) T_fancy("ecf", L);
                if (use_r) T_fancy("erf", L);
            }
            if (use_o) {
                if (use_l) T_fancy("olf", L);
                if (use_c) T_fancy("ocf", L);
                if (use_r) T_fancy("orf", L);
            }
        }
        if (L1.empty()) break;
    }
}

auto CmdChr::is_ok_for_xspace() const -> bool {
    if (cmd == other_catcode) {
        if (chr < int(nb_xspace_values)) return the_parser.ok_for_xspace[chr];
        return false;
    }
    if (cmd == 2 || cmd == 10) return true;
    if (cmd == 1) return false; // doc says true; FIXME
    if (cmd == cst1_cmd && chr == space_code) return true;
    return false;
}

// expansion of \xspace
void Parser::E_xspace() {
    if (get_token()) return;
    back_input();
    bool val = cur_cmd_chr.is_ok_for_xspace();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "\\xspace after " << cur_tok << (val ? " did nothing " : " added space") << "\n";
    }
    if (val) return;
    back_input(hash_table.space_token);
}

// arg2 ignored, in arg1 \ is special
auto Parser::T_xmllatex() -> std::string {
    TokenList L = read_arg();
    ignore_arg();
    mac_buffer.clear();
    while (!L.empty()) {
        Token x = L.front();
        L.pop_front();
        if (x.active_or_single())
            mac_buffer.push_back(x.char_val());
        else if (x.is_in_hash())
            mac_buffer.append(hash_table[x.hash_loc()]);
        // else token is bad or null
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{Rawxml: " << mac_buffer << "}\n";
    }
    return mac_buffer;
}

void Parser::E_get_config(int c) {
    Token       T        = cur_tok;
    TokenList   L1       = read_arg();
    std::string resource = list_to_string_c(L1, "Problem scanning resource name");
    std::string key;
    std::string res;
    if (c != 0) {
        TokenList L2 = read_arg();
        key          = list_to_string_c(L2, "Problem scanning key");
        res          = find_one_key(resource, key);
    } else
        res = find_keys(resource);
    mac_buf     = res;
    TokenList L = mac_buf.str_toks11(false);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << " #1=" << resource;
        if (c != 0) the_log << " #2=" << key;
        the_log << " -> " << L << "\n";
    }
    back_input(L);
}

void Parser::T_usefont() {
    flush_buffer();
    cur_font.ltfont(sT_arg_nopar(), fontencoding_code);
    cur_font.ltfont(sT_arg_nopar(), fontfamily_code);
    cur_font.ltfont(sT_arg_nopar(), fontseries_code);
    cur_font.ltfont(sT_arg_nopar(), fontshape_code);
    font_has_changed();                    // \selectfont
    remove_initial_space_and_back_input(); // \ignorespaces
}

void Parser::T_unimplemented_font(subtypes c) {
    Token T = cur_tok;
    if (c == DeclareMathSizes_code) remove_initial_star();
    int n = 0;
    switch (c) {
    case TextSymbolUnavailable_code:
    case DeclareMathVersion_code:
    case DeclareMathDelimiter_code: n = 1; break;
    case ProvideTextCommand_code:
    case DeclareTextCommand_code:
    case DeclareTextCommandDefault_code:
    case ProvideTextCommandDefault_code:
    case DeclareTextSymbolDefault_code:
    case UseTextSymbol_code:
    case UndeclareTextCommand_code:
    case DeclareFontEncodingDefaults_code:
    case DeclareSizeFunction_code:
    case DeclareSymbolFontAlphabet_code:
    case DeclareTextFontCommand_code: n = 2; break;
    case DeclareTextAccent_code:
    case UseTextAccent_code:
    case DeclareTextSymbol_code:
    case DeclareFontFamily_code:
    case DeclareFontEncoding_code:
    case DeclareOldFontCommand_code: n = 3; break;
    case DeclareTextCompositeCommand_code:
    case DeclareTextComposite_code:
    case DeclareFontSubstitution_code:
    case DeclareMathAccent_code:
    case DeclareMathSymbol_code:
    case DeclareMathSizes_code: n = 4; break;
    case DeclarePreloadSizes_code:
    case DeclareErrorFont_code:
    case DeclareSymbolFont_code:
    case DeclareMathAlphabet_code:
    case DeclareMathRadical_code: n = 5; break;
    case DeclareFontShape_code:
    case DeclareFixedFont_code:
    case SetSymbolFont_code:
    case SetMathAlphabet_code: n = 6; break;
    default: break;
    }
    while (n > 0) {
        ignore_arg();
        n--;
    }
    parse_error(T, "Unimplemented NFSS command ", T, "", "unimplemented");
    if (c == ProvideTextCommand_code || c == DeclareTextCommand_code) {
        back_input(hash_table.frozen_protection);
        M_newcommand(rd_always);
    }
}

// raisebox{1cm}[2cm][3cm]{a box}

void Parser::T_raisebox() {
    std::string A = nT_arg_nopar();
    auto        B = nT_optarg_nopar();
    auto        C = nT_optarg_nopar();
    leave_v_mode();
    the_stack.push1(the_names["raisebox"]);
    AttList &cur = last_att_list();
    cur.push_back(the_names["val"], A);
    if (B) cur.push_back(the_names["height"], *B);
    if (C) cur.push_back(the_names["depth"], *C);
    T_arg_local();
    the_stack.pop(the_names["raisebox"]);
}

// Implements \in@ , evaluates \in@false or \in@true
void Parser::T_isin() {
    TokenList A     = read_arg();
    TokenList B     = read_arg();
    int       n     = 0; // unused
    bool      found = token_ns::is_in(A, B, false, n);
    back_input(found ? hash_table.intrue_token : hash_table.infalse_token);
}

// \@car \@cdr
void Parser::E_car(bool first) {
    Token     T = cur_tok;
    TokenList L = read_arg();
    TokenList M = read_until_nopar(hash_table.nil_token);
    if (tracing_macros()) {
        Logger::finish_seq();
        the_log << T << "#1#2\\@nil ->#" << (first ? "1" : "2") << "\n";
        the_log << "#1 <-" << L << "\n";
        the_log << "#2 <-" << M << "\n";
        Logger::finish_seq();
        the_log << T << "<- " << (first ? L : M) << "\n";
    }
    if (first)
        back_input(L);
    else
        back_input(M);
}

// Translation of \foo is <foo/>
void Parser::T_ignoreA() {
    Token T = cur_tok;
    auto  s = T.tok_to_str();
    if (s[0] == '\\') s.erase(0, 1);
    if ((s == "newpage") || (s == "clearpage") || (s == "cleardoublepage")) leave_h_mode();
    flush_buffer();
    unprocessed_xml += "<" + s + "/>";
    flush_buffer();
}

void Parser::T_line(subtypes c) {
    std::string k = "np_center";
    switch (c) {
    case leftline_code: k = "left"; break;
    case rightline_code: k = "right"; break;
    case llap_code: k = "llap"; break;
    case rlap_code: k = "rlap"; break;
    default:;
    }
    if (c == leftline_code || c == centerline_code || c == rightline_code) {
        if (the_stack.is_float()) return;
        leave_v_mode();
    }
    the_stack.push1(the_names["lineC"]);
    AttList &cur = last_att_list();
    cur.push_back(the_names["rend"], the_names[k]);
    T_arg_local();
    the_stack.pop(the_names["lineC"]);
}

// Implements \typein[\foo]{bar},  or \typein{bar}. In the first case
// it is like \typeout{bar}\read0to\foo, otherwise backinputs what is read.
void Parser::T_typein() {
    Token     cmd = hash_table.relax_token;
    TokenList res;
    bool      has_opt = false;
    if (read_optarg_nopar(res)) {
        Token Q = token_ns::get_unique(res);
        back_input(Q);
        cmd     = get_r_token();
        has_opt = true;
    }
    log_and_tty << string_to_write(negative_out_slot); // \typeout next arg
    auto cc                              = eqtb_int_table[endlinechar_code].val;
    eqtb_int_table[endlinechar_code].val = -1;
    TokenList L                          = read_from_file(0, false);
    eqtb_int_table[endlinechar_code].val = cc;
    if (has_opt)
        new_macro(L, cmd);
    else
        back_input(L);
}

// In the case of \@tfor \foo:=... reads the command name \foo
// and skips the :=
auto Parser::read_for_variable() -> Token {
    TokenList W;
    W.push_back(Token(other_t_offset, ':'));
    W.push_back(hash_table.equals_token);
    Token cmd = get_r_token();
    read_delimited(W); // this should read an empty list
    return cmd;
}

// expand the first element of L, like in \expandafter{\foo...}
// the result is pushed back in L
void Parser::expand_first(TokenList &L) {
    back_input(hash_table.CB_token);
    back_input(L);
    expand_when_ok(true);
    back_input(hash_table.OB_token);
    TokenList res = read_arg();
    L.swap(res);
}

// Implementation of some loops
// should be expand rather than translate
void Parser::T_xkv_for(subtypes c) {
    Token     comma    = hash_table.comma_token;
    Token     nil      = hash_table.nil_token;
    Token     doubleat = hash_table.doubleat_token;
    Token     T        = cur_tok;
    TokenList res;
    switch (c) {
    case for_code: { // case \@for \A:= list \do code
        Token     cmd      = read_for_variable();
        TokenList L        = read_until(hash_table.do_token);
        TokenList function = read_arg();
        expand_first(L);
        if (L.empty()) break;
        brace_me(function);
        res.push_back(hash_table.forloop_token);
        res.splice(res.end(), L);
        res.push_back(comma);
        res.push_back(nil);
        res.push_back(comma);
        res.push_back(nil);
        res.push_back(doubleat);
        res.push_back(cmd);
        res.splice(res.end(), function);
        break;
    }
    case forloop_code: { // \@forloop
        TokenList entry      = read_until(comma);
        TokenList next_entry = read_until(comma);
        TokenList remainder  = read_until(doubleat);
        Token     cmd        = get_r_token();
        TokenList function   = read_arg();
        if (token_ns::has_a_single_token(entry, nil)) break;
        new_macro(entry, cmd);
        TokenList aux = function;
        res.splice(res.end(), aux);
        if (token_ns::has_a_single_token(next_entry, nil)) break;
        brace_me(next_entry);
        TokenList aux2 = function;
        brace_me(function);
        res.push_back(hash_table.def_token);
        res.push_back(cmd);
        res.splice(res.end(), next_entry);
        res.splice(res.end(), aux2);
        res.push_back(hash_table.iforloop_token);
        res.splice(res.end(), remainder);
        res.push_back(doubleat);
        res.push_back(cmd);
        res.splice(res.end(), function);
        break;
    }
    case tfor_code: { // \@tfor
        Token cmd = read_for_variable();
        back_input(hash_table.space_token);
        TokenList L        = read_until(hash_table.do_token);
        TokenList function = read_arg();
        if (token_ns::has_a_single_token(L, hash_table.space_token)) break;
        brace_me(function);
        res.push_back(hash_table.tforloop_token);
        res.splice(res.end(), L);
        res.push_back(nil);
        res.push_back(nil);
        res.push_back(doubleat);
        res.push_back(cmd);
        res.splice(res.end(), function);
        break;
    }
    case xkv_breaktfor_code: { // \tralics@for@break
        // should we insert \fi here
        TokenList W;
        W.push_back(nil);
        W.push_back(comma);
        read_delimited(W);
        res.push_back(hash_table.fi_token);
        break;
    }
    case breaktfor_code: // \@break@tfor
        read_until(hash_table.doubleat_token);
        ignore_arg(); // should this be long ?
        ignore_arg();
        res.push_back(hash_table.fi_token);
        break;
    case iforloop_code: // \@iforloop  \@tforloop
    case tforloop_code: {
        TokenList entry     = c == iforloop_code ? read_until(comma) : read_arg();
        TokenList remainder = read_until(doubleat);
        Token     cmd       = get_r_token();
        TokenList function  = read_arg();
        if (token_ns::has_a_single_token(entry, nil)) break;
        new_macro(entry, cmd);
        TokenList aux = function;
        res.splice(res.end(), aux);
        brace_me(function);
        res.push_back(hash_table.relax_token);
        res.push_back(c == iforloop_code ? hash_table.iforloop_token : hash_table.tforloop_token);
        res.splice(res.end(), remainder);
        res.push_back(doubleat);
        res.push_back(cmd);
        res.splice(res.end(), function);
        break;
    }
    case xkv_for_n_code: // \tralics@for@n and variants
    case xkv_for_o_code:
    case xkv_for_en_code:
    case xkv_for_eo_code: {
        TokenList L = read_arg(); // the list to operate on
        if (c == xkv_for_o_code || c == xkv_for_eo_code) expand_first(L);
        TokenList cmd      = read_arg();
        TokenList function = read_arg();
        if (L.empty() && (c == xkv_for_n_code || c == xkv_for_o_code)) break;
        brace_me(function);
        res.push_front(hash_table.xkv_for_i_token);
        res.splice(res.end(), cmd);
        res.splice(res.end(), function);
        res.splice(res.end(), L);
        res.push_back(comma);
        res.push_back(nil);
        res.push_back(comma);
        break;
    }
    case xkv_for_i_code: {                  // \tralics@for@i
        Token     cmd      = get_r_token(); // the command
        TokenList function = read_arg();    // the code
        TokenList entry    = read_until(comma);
        if (token_ns::has_a_single_token(entry, nil)) break;
        new_macro(entry, cmd);
        TokenList aux = function;
        brace_me(function);
        res.splice(res.end(), aux);
        res.push_back(hash_table.xkv_for_i_token);
        res.push_back(cmd);
        res.splice(res.end(), function);
        break;
    }
        //  case xkv_whilist_code:
        //
    default:;
    }
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << T << "<- " << res << "\n";
    }
    back_input(res);
}

// Implementation of \@cons
// Read a token T and a list L, calls the function below
void Parser::M_cons() {
    Token     cmd = get_r_token();
    TokenList L   = read_arg();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\@cons " << cmd << " + " << L << "}\n";
    }
    M_cons(cmd, L);
}

// \@cons\T{L} is \edef\T{\T\@elt L}, with \let\@elt\relax in a group
// Question: latex has \xdef, is a \edef useful ?
void Parser::M_cons(Token cmd, TokenList &L) {
    Token E = hash_table.elt_token;
    L.push_front(E);
    L.push_front(cmd);
    push_level(bt_brace);
    M_let_fast(E, hash_table.relax_token, false);
    auto guard = SaveErrTok(cmd);
    read_toks_edef(L);
    pop_level(bt_brace);
    new_macro(L, cmd, true);
}

// Implements \@testopt \A B as: if bracket, then \A, else \A[{B}]
void Parser::T_testopt() {
    TokenList L = read_arg();
    TokenList R = read_arg();
    skip_initial_space();
    if (cur_tok.is_valid()) back_input();
    if (!cur_tok.is_open_bracket()) {
        brace_me(R);
        R.push_front(Token(other_t_offset, '['));
        R.push_back(Token(other_t_offset, ']'));
        back_input(R);
    }
    back_input(L);
}

// Implements \tralics@addtolist@n{sep}{\cmd}{val}
// and \tralics@addtolist@o{sep}{\cmd}{val}
// In the second case, the first token in val is expanded;
// This puts val to the end of \cmd, with sep unless \cmd is empty
void Parser::T_addtomacro(bool exp) {
    TokenList D = read_arg();        // The separator
    Token     T = get_r_token(true); // the macro to define
    TokenList L = read_arg();        //
    back_input(hash_table.CB_token);
    back_input(L);
    if (exp) expand_when_ok(true);
    TokenList W = get_mac_value(T);
    if (!W.empty()) back_input(D);
    back_input(W);
    L.clear();
    skip_group0(L);
    new_macro(L, T);
}

// Implements \@expandtwoargs\foo\bar\gee
// result is \foo{barval}{geeval}
void Parser::expand_twoargs() {
    TokenList L = read_arg();
    TokenList M = read_arg();
    TokenList N = read_arg();
    brace_me(M);
    brace_me(N);
    M.splice(M.end(), N);
    read_toks_edef(M);
    back_input(M);
    back_input(L);
}

// ------------------------------------------------------------
// Special commands for xkeyval

void Parser::T_xkeyval(subtypes c) {
    switch (c) {
    case boot_keyval_code: hash_table.boot_keyval(); return;
    case definekey_code: T_define_key(false); return;
    case xdefinekey_code: T_define_key(true); return;
    case define_cmdkey_code:
    case define_cmdkeys_code: define_cmd_key(c); return;
    case define_boolkey_code:
    case define_boolkeys_code: define_bool_key(c); return;
    case define_choicekey_code: define_choice_key(); return;
    case define_cc_code: internal_choice_key(); return;
    case disable_keys_code: disable_keys(); return;
    case key_ifundefined_code: key_ifundefined(); return;
    case save_keys_code:
    case gsave_keys_code: {
        xkv_save_keys_aux(false, 0);
        TokenList A = read_arg();
        xkv_merge(c == gsave_keys_code, 0, A, true);
        return;
    }
    case delsave_keys_code:
    case gdelsave_keys_code: {
        bool      bad = xkv_save_keys_aux(false, 0);
        TokenList A   = read_arg();
        if (!bad) xkv_merge(c == gdelsave_keys_code, 0, A, false);
        return;
    }
    case unsave_keys_code:
    case gunsave_keys_code:
        if (xkv_save_keys_aux(true, 0)) return;
        M_let_fast(cur_tok, hash_table.frozen_undef_token, c == gunsave_keys_code);
        return;
    case preset_keys_code:
    case gpreset_keys_code: {
        xkv_save_keys_aux(false, 1);
        TokenList A = read_arg();
        TokenList B = read_arg();
        xkv_merge(c == gpreset_keys_code, 1, A, true);
        xkv_merge(c == gpreset_keys_code, 2, B, true);
        return;
    }
    case delpreset_keys_code:
    case gdelpreset_keys_code: {
        bool      bad = xkv_save_keys_aux(true, 1);
        TokenList A   = read_arg();
        TokenList B   = read_arg();
        if (bad) return;
        xkv_merge(c == gdelpreset_keys_code, 1, A, false);
        xkv_merge(c == gdelpreset_keys_code, 2, B, false);
        return;
    }
    case unpreset_keys_code:
    case gunpreset_keys_code: {
        if (xkv_save_keys_aux(true, 1)) return;
        M_let_fast(cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
        xkv_ns::find_aux(2);
        cur_tok = hash_table.locate(local_buf);
        M_let_fast(cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
        return;
    }
    case setrmkeys_code: setkeys(false); return;
    case setkeys_code: setkeys(true); return;
    case declare_optionsX_code: xkv_declare_option(); return;
    case process_optionsX_code: xkv_process_options(); return;
    case execute_optionsX_code: xkv_execute_options(); return;
    default: return;
    }
}

// Case \define@key{foo}{bar}{ETC}
// or   \define@key{foo}{bar}[gee]{ETC}
// Read foo and bar, construct KV@foo@bar, call it \cmd
// In the first case, we produce \def\cmd#1{ETC}
// Otherwise \def\KV@foo@bar@default{\cmd{gee}}, then \def\cmd#1{ETC}

void Parser::T_define_key(bool xkv) {
    Buffer &B = local_buf;
    if (xkv)
        xkv_fetch_prefix_family();
    else {
        TokenList fam = read_arg();
        list_to_string_c(fam, "KV@", "", "Problem in define@key", B);
        B << '@';
        xkv_header = B;
    }
    TokenList key = read_arg();
    list_to_string_c(key, xkv_header, "", "bad key name", B);
    Token     T = hash_table.locate(B);
    TokenList opt;
    if (read_optarg(opt)) internal_define_key_default(T, opt);
    internal_define_key(T);
}

// Implements \define@cmdkey, \define@cmdkeys
// We make the assumption that a key does not contain a comma
void Parser::define_cmd_key(subtypes c) {
    Buffer &B = local_buf;
    xkv_fetch_prefix_family(); // read prefix and family
    TokenList L;
    if (read_optarg_nopar(L)) {
        list_to_string_c(L, "", "", "Problem scanning macro prefix", B);
    } else
        B = "cmd" + xkv_header;
    std::string mp      = B;
    TokenList   keytoks = read_arg();
    TokenList   dft;
    bool        has_dft = read_optarg(dft); // \par ok here
    // construct the key or key list
    std::string Keys = list_to_string_c(keytoks, "problem scanning key");
    for (const auto &Key : split_commas(Keys)) {
        if (Key.empty()) continue;
        B         = mp + Key;
        Token cmd = hash_table.locate(B);
        B         = xkv_header + Key;
        Token T   = hash_table.locate(B);
        if (has_dft) {
            TokenList D = dft;
            internal_define_key_default(T, D);
        }
        TokenList LL;
        if (c == define_cmdkey_code) { // case of cmdkey
            TokenList u = read_arg();
            LL.splice(LL.end(), u);
        }
        LL.push_front(hash_table.CB_token);
        LL.push_front(Token(other_t_offset, '1'));
        LL.push_front(make_char_token('#', 6));
        LL.push_front(hash_table.OB_token);
        LL.push_front(cmd);
        LL.push_front(hash_table.locate("def"));
        brace_me(LL);
        back_input(LL);
        auto *X = new Macro;
        X->set_nbargs(1);
        X->set_type(dt_normal);
        read_mac_body(X->body, false, 1);
        X->correct_type();
        mac_define(T, X, false, rd_always, user_cmd);
    }
}

// Implements \define@choicekey
void Parser::define_choice_key() {
    bool if_star = remove_initial_plus(false);
    bool if_plus = remove_initial_plus(true);
    xkv_fetch_prefix_family();
    TokenList keytoks = read_arg();
    list_to_string_c(keytoks, xkv_header, "", "bad key name", local_buf);
    Token     T = hash_table.locate(local_buf);
    TokenList storage_bin;
    read_optarg_nopar(storage_bin);
    TokenList allowed = read_arg();
    TokenList opt;
    if (read_optarg(opt)) internal_define_key_default(T, opt);
    TokenList F;
    if (if_plus) {
        TokenList x = read_arg();
        TokenList y = read_arg();
        brace_me(x);
        brace_me(y);
        F.splice(F.end(), x);
        F.splice(F.end(), y);
    } else {
        F = read_arg();
        brace_me(F);
    }
    TokenList body;
    body.push_back(hash_table.xkv_cc_token);
    if (if_star) body.push_back(Token(other_t_offset, '*'));
    if (if_plus) body.push_back(Token(other_t_offset, '+'));
    storage_bin.push_front(Token(other_t_offset, '['));
    storage_bin.push_back(Token(other_t_offset, ']'));
    body.splice(body.end(), storage_bin);
    TokenList argument;
    argument.push_back(make_char_token('#', 6));
    argument.push_back(Token(other_t_offset, '1'));
    brace_me(argument);
    body.splice(body.end(), argument);
    brace_me(allowed);
    body.splice(body.end(), allowed);
    body.splice(body.end(), F);
    brace_me(body);
    back_input(body);
    internal_define_key(T);
}

// Implements \XKV@cc
void Parser::internal_choice_key() {
    bool      if_star = remove_initial_plus(false);
    bool      if_plus = remove_initial_plus(true);
    TokenList bin;
    read_optarg_nopar(bin);
    TokenList input   = read_arg();
    TokenList allowed = read_arg();
    TokenList ok_code = read_arg();
    TokenList bad_code;
    if (if_plus) bad_code = read_arg();
    if (if_star) {
        token_ns::lower_case(input);
        token_ns::lower_case(allowed);
    }
    Token relax = hash_table.relax_token;
    Token B1 = relax, B2 = relax;
    if (!bin.empty()) {
        B1 = bin.front();
        bin.pop_front();
    }
    if (!bin.empty()) {
        B2 = bin.front();
        bin.pop_front();
    }
    if (B1.not_a_cmd()) B1 = relax;
    if (B2.not_a_cmd()) B2 = relax;
    if (B1 != relax) {
        TokenList u = input;
        new_macro(u, B1);
    }
    TokenList xinput = input;
    int       k      = 0;
    bool      found  = token_ns::find_in(xinput, allowed, hash_table.comma_token, false, k);
    if (B2 != relax) {
        local_buf   = fmt::format("{}", k);
        TokenList u = local_buf.str_toks(nlt_cr); // Should be irrelevant ?
        new_macro(u, B2);
    }
    if (found)
        back_input(ok_code);
    else if (if_plus) // invalid, but catched
        back_input(bad_code);
    else {
        parse_error(err_tok, "XKV: value is not allowed");
        log_and_tty << " " << input << "\n";
    }
}

void Parser::define_bool_key(subtypes c) {
    Buffer &B = local_buf;
    remove_initial_plus(false);
    bool if_plus = remove_initial_plus(true);
    if (c != define_boolkey_code) if_plus = false;
    xkv_fetch_prefix_family(); // read prefix and family
    TokenList L;
    if (read_optarg_nopar(L)) {
        list_to_string_c(L, "", "", "Problem scanning macro prefix", B);
    } else
        B = xkv_header;
    std::string mp      = B;
    TokenList   keytoks = read_arg();
    TokenList   dft;
    bool        has_dft = read_optarg_nopar(dft);
    // construct the key or key list
    std::string Keys = list_to_string_c(keytoks, "Problem scanning key");
    for (const auto &Key : split_commas(Keys)) {
        if (Key.empty()) continue;
        B           = mp + Key;
        TokenList u = B.str_toks11(false);
        B           = "if" + mp + Key;
        back_input(hash_table.locate(B));
        M_newif();
        B           = "true,false";
        TokenList v = B.str_toks11(false);
        B           = xkv_header + Key;
        Token T     = hash_table.locate(local_buf);
        if (has_dft) {
            TokenList D = dft;
            internal_define_key_default(T, D);
        }
        u.push_front(hash_table.csname_token);
        u.push_back(hash_table.xkv_resa_token);
        u.push_back(hash_table.endcsname_token);
        if (c == define_boolkey_code) {
            TokenList add1 = read_arg();
            u.splice(u.end(), add1);
        }
        brace_me(u);
        TokenList LL;
        LL.splice(LL.end(), u);
        if (if_plus) {
            TokenList add2 = read_arg();
            brace_me(add2);
            LL.splice(LL.end(), add2);
        }
        brace_me(v);
        LL.splice(LL.begin(), v);
        LL.push_front(hash_table.CB_token);
        LL.push_front(Token(other_t_offset, '1'));
        LL.push_front(make_char_token('#', 6));
        LL.push_front(hash_table.OB_token);
        LL.push_front(Token(other_t_offset, ']'));
        LL.push_front(hash_table.xkv_resa_token);
        LL.push_front(Token(other_t_offset, '['));
        if (if_plus) LL.push_front(Token(other_t_offset, '+'));
        LL.push_front(Token(other_t_offset, '*'));
        LL.push_front(hash_table.xkv_cc_token);
        brace_me(LL);
        back_input(LL);
        auto *X = new Macro;
        X->set_nbargs(1);
        X->set_type(dt_normal);
        read_mac_body(X->body, false, 1);
        X->correct_type();
        mac_define(T, X, false, rd_always, user_cmd);
    }
}

// Implements \key@ifundefined
void Parser::key_ifundefined() {
    Buffer &B = local_buf;
    xkv_fetch_prefix();
    TokenList   fams      = read_arg();
    bool        undefined = true;
    TokenList   key       = read_arg();
    std::string Key       = list_to_string_c(key, "problem scanning key");
    std::string Fams      = list_to_string_c(fams, "Problem with the families");
    std::string fam;
    for (const auto &f : split_commas(Fams)) {
        fam = f;
        B   = xkv_prefix + fam;
        if (!fam.empty()) B.push_back('@');
        B.append(Key);
        if (hash_table.is_defined(B)) {
            undefined = false;
            break;
        }
    }
    new_macro(fam, hash_table.xkv_tfam_token);
    one_of_two(undefined);
}

// Implements \disable@keys
void Parser::disable_keys() {
    Buffer &B = local_buf;
    xkv_fetch_prefix_family(); // read prefix and family
    TokenList   keys = read_arg();
    std::string Keys = list_to_string_c(keys, "problem scanning keys");
    for (const auto &Key : split_commas(Keys)) {
        B = xkv_header + Key;
        if (hash_table.is_defined(B)) {
            Token T = hash_table.last_tok;
            B.append("@default");
            if (hash_table.is_defined(B)) {
                TokenList L;
                brace_me(L);
                L.push_front(T);
                new_macro(L, hash_table.last_tok);
            }
            B           = "Key `" + Key + "' has been disabled";
            TokenList L = B.str_toks(nlt_space); // should be irrelevant
            brace_me(L);
            L.push_front(hash_table.xkv_warn_token);
            auto *X = new Macro(L);
            X->set_nbargs(1);
            X->set_type(dt_normal);
            mac_define(T, X, false, rd_always, user_cmd);
        } else
            parse_error(err_tok, "Undefined key cannot be disabled: ", Key, "");
    }
}

// Find saved or preset keys, depending on c2. If not found:
// signals a an error if c is true (creates otherwise), return true.
// Creates cur_tok if needed
auto Parser::xkv_save_keys_aux(bool c, int c2) -> bool {
    xkv_fetch_prefix_family();
    xkv_ns::find_aux(c2);
    Buffer &B   = local_buf;
    bool    ret = !hash_table.is_defined(B);
    if (c && ret) {
        B = (c2 != 0 ? "No presets defined for `" : "No save keys defined for `") + xkv_header + "'";
        parse_error(err_tok, B);
        return true;
    }
    cur_tok = hash_table.last_tok;
    return ret;
}

// The following function takes in L one item and puts the key in x.
// If type is 0, we are looking for \global, and there is no equals
// Otherwise we look for \savevalue or \gsavevalue, skip equals.
// We set some booleans
auto xkv_ns::find_key_of(const TokenList &L, int type) -> std::string {
    Hashtab & H      = the_parser.hash_table;
    Token     equals = H.equals_token;
    auto      C      = L.begin();
    auto      E      = L.end();
    TokenList x; // will hold the tokens
    while (C != E) {
        if (*C == equals) break;
        x.push_back(*C);
        ++C;
    }
    xkv_is_save   = false;
    xkv_is_global = false;
    if (x.empty()) return "";
    Token first = x.front();
    if (type == 0) {
        if (first == H.global_token) {
            x.pop_front();
            xkv_is_global = true;
        }
    } else {
        if (first == H.savevalue_token) {
            x.pop_front();
            xkv_is_save = true;
        } else if (first == H.gsavevalue_token) {
            x.pop_front();
            xkv_is_save   = true;
            xkv_is_global = true;
        }
    }
    token_ns::remove_ext_braces(x);
    return the_parser.list_to_string_c(x, "Invalid key name");
}

// Command savekeys and presetkeys use a common whose name is constructed
// here
void xkv_ns::find_aux(int c) {
    local_buf = "XKV@" + xkv_header;
    local_buf += (c == 0 ? "save" : (c == 1 ? "preseth" : "presett"));
}

// This merges L into W; both lists have the form \global{key}=value
// or, depenfing on the type, are lists of keys
void xkv_ns::merge(TokenList &W, TokenList &L, int type) {
    Token     comma = the_parser.hash_table.comma_token;
    TokenList key;
    TokenList tmp;
    if (W.empty()) {
        while (!L.empty()) {
            token_ns::split_at(comma, L, key);
            token_ns::remove_first_last_space(key);
            if (!W.empty()) W.push_back(comma);
            W.splice(W.end(), key);
        }
        return;
    }
    while (!L.empty()) {
        token_ns::split_at(comma, L, key);
        token_ns::remove_first_last_space(key);
        std::string key_name = find_key_of(key, type);
        bool        found    = false; // Was key in W ?
        TokenList   z;
        W.swap(tmp);
        while (!tmp.empty()) {
            token_ns::split_at(comma, tmp, z);
            std::string zname = find_key_of(z, type);
            if (!W.empty()) W.push_back(comma);
            if (key_name == zname) {
                found = true; // replace in W old value z by new value
                W.splice(W.end(), key);
            } else
                W.splice(W.end(), z);
        }
        if (!found) {
            if (!W.empty()) W.push_back(comma);
            W.splice(W.end(), key); // Insert key at the end
        }
    }
}

// This deletes L from W. Here L is a simple list of keys
void xkv_ns::remove(TokenList &W, TokenList &L, int type) {
    Buffer &B   = buf_for_del;
    Buffer &aux = local_buf;
    the_parser.list_to_string_c(L, ",", ",", "Invalid key name list", B);
    Token     comma = the_parser.hash_table.comma_token;
    TokenList tmp;
    TokenList key;
    W.swap(tmp);
    while (!tmp.empty()) {
        token_ns::split_at(comma, tmp, key);
        std::string key_name = xkv_ns::find_key_of(key, type);
        aux                  = "," + key_name + ",";
        if (B.find(aux) == std::string::npos) {
            if (!W.empty()) W.push_back(comma);
            W.splice(W.end(), key);
        }
    }
}

// This finds mykey in the list whose name is in the buffer
auto Parser::find_a_save_key(const std::string &mykey) -> bool {
    Buffer &  B = local_buf;
    TokenList W = get_mac_value(hash_table.locate(B));
    TokenList key;
    while (!W.empty()) {
        token_ns::split_at(hash_table.comma_token, W, key);
        std::string key_name = xkv_ns::find_key_of(key, 0);
        if (key_name == mykey) return true;
    }
    return false;
}

// Merges or deletes depending on mg globally if gbl is true
// the keys in L from the variable depending on type
// Implements preset or save depending on type
void Parser::xkv_merge(bool gbl, int type, TokenList &L, bool mg) {
    token_ns::sanitize_one(L, '=');
    token_ns::sanitize_one(L, ',');
    xkv_ns::find_aux(type);
    Token     T = hash_table.locate(local_buf);
    TokenList W = get_mac_value(T);
    if (mg)
        xkv_ns::merge(W, L, type);
    else
        xkv_ns::remove(W, L, type);
    new_macro(W, T, gbl);
}

// Assume local_buf contains the name of T without the extension
void Parser::internal_define_key_default(Token T, TokenList &L) {
    brace_me(L);
    L.push_front(T);
    local_buf.append("@default");
    cur_tok = hash_table.locate(local_buf);
    new_macro(L, cur_tok);
}

// This is like \def\T#1, optimised
void Parser::internal_define_key(Token T) {
    auto *X = new Macro;
    X->set_nbargs(1);
    X->set_type(dt_normal);
    read_mac_body(X->body, false, 1);
    X->correct_type();
    mac_define(T, X, false, rd_always, user_cmd);
}

// Skips initial + or *, catcode irrelevant
auto Parser::remove_initial_plus(bool plus) -> bool {
    skip_initial_space();
    if (cur_tok.is_a_char()) {
        if (cur_tok.char_val() == (plus ? '+' : '*')) return true;
    }
    if (!cur_tok.is_invalid()) back_input();
    return false;
}

// Reads optional prefix, and family, and handles them
void Parser::xkv_fetch_prefix() {
    TokenList L;
    if (!read_optarg(L)) {
        xkv_prefix = "KV@";
        return;
    }
    Buffer &B = local_buf;
    B.clear();
    token_ns::remove_first_last_space(L);
    bool t = list_to_string(L, B);
    if (t) {
        parse_error(err_tok, "Bad command ", cur_tok, " in XKV prefix (more errors may follow)", "bad kv prefix");
        B.clear();
    }
    if (B == "XKV") {
        parse_error(err_tok, "xkeyval: `XKV' prefix is not allowed");
        B.clear();
    }
    if (!B.empty()) B.push_back('@');
    xkv_prefix = B;
}

// Creates the XKV header

void Parser::xkv_makehd(TokenList &L) {
    token_ns::remove_first_last_space(L);
    Buffer &B = local_buf;
    B         = xkv_prefix;
    auto k    = B.size();
    if (list_to_string(L, B)) {
        parse_error(err_tok, "Bad command ", cur_tok, " in XKV family (more errors may follow)", "bad kv family");
        B.resize(k);
    }
    if (B.size() != k) B.push_back('@');
    xkv_header = B;
}

void xkv_ns::makehd(const std::string &fam) {
    Buffer &B = local_buf;
    B         = xkv_prefix + fam;
    if (!fam.empty()) B.push_back('@');
    xkv_header = B;
}

void Parser::xkv_fetch_prefix_family() {
    xkv_fetch_prefix();
    TokenList M = read_arg();
    xkv_makehd(M);
}

void token_ns::lower_case(TokenList &L) {
    auto P      = L.begin();
    auto E      = L.end();
    auto offset = lc_code_offset;
    while (P != E) {
        Token a = *P;
        if (a.val < single_offset) {
            auto b  = a.chr_val();
            auto cx = the_parser.eqtb_int_table[b + offset].val;
            if (cx != 0) *P = Token(a.val - b + to_unsigned(cx));
        }
        ++P;
    }
}

void Parser::xkv_declare_option() {
    // \let\@fileswith@pti@ns\@badrequireerror
    bool star = remove_initial_star();
    if (star) {
        T_declare_option_star();
        return;
    }
    xkv_fetch_prefix();
    TokenList FL = XKV_parse_filename();
    xkv_makehd(FL);
    TokenList   key  = read_arg();
    TokenList   xkey = key;
    std::string Key  = list_to_string_c(xkey, "Invalid option");
    classes_ns::register_key(Key);
    list_to_string_c(key, xkv_header, "", "bad option name", local_buf);
    Token     T = hash_table.locate(local_buf);
    TokenList opt;
    read_optarg(opt);
    internal_define_key_default(T, opt);
    internal_define_key(T);
}

// Implements ExecuteOptionsX
void Parser::xkv_execute_options() {
    XkvSetkeys data(this);
    xkv_fetch_prefix();
    data.special_fams();
    data.fetch_keys(true);
    data.dump_keys();
    data.set_aux();
    data.finish();
}

void XkvSetkeys::dump_keys() {
    Logger::finish_seq();
    the_log << "{Options to execute->" << keyvals << "}\n";
}

// Implements ProcessOptionsX
void Parser::xkv_process_options() {
    XkvSetkeys data(this);
    //  data.no_err = remove_initial_plus(false);
    bool s = remove_initial_star(); // we should do something with this
    xkv_fetch_prefix();
    data.set_inpox();
    data.special_fams();
    TokenList uo;
    TokenList L = classes_ns::cur_options(s, uo, false);
    brace_me(L);
    back_input(L);
    data.fetch_keys(true);
    data.set_aux();
    data.finish();
}

// Removes A from B, result will be in C
void Parser::T_remove_element() {
    TokenList A = read_arg();
    TokenList B = read_arg();
    get_r_token(true);
    remove_element(A, B, cur_tok);
}

auto token_ns::find_in(TokenList &A, TokenList &B, Token t, bool sw, int &n) -> bool {
    A.push_back(t);
    B.push_back(t);
    A.push_front(t);
    B.push_front(t);
    return token_ns::is_in(A, B, sw, n);
}

// We add commas around A and B, and must remove them later
void Parser::remove_element(TokenList &A, TokenList &B, Token C) {
    Token t = hash_table.comma_token;
    int   n = 0;
    token_ns::find_in(A, B, t, true, n);
    B.pop_front();
    if (!B.empty()) B.pop_back();
    new_macro(B, C);
}

// Redefinition of T_pass_options if xkvltxp is loaded
// The idea is to not expand the option.
// Pushes tokens into \opt@foo.cls
void Parser::xkv_pass_options(bool c) // true if a class
{
    TokenList   opt  = read_arg();
    std::string name = sE_arg_nopar();
    name             = "opt@" + name;
    name += (c ? ".cls" : ".sty");
    Token t = hash_table.CurrentOption_token;
    if (token_ns::has_a_single_token(opt, t)) opt = get_mac_value(t);
    Token     tname = hash_table.locate(name);
    TokenList res   = get_mac_value(tname);
    if (!res.empty()) res.push_back(Token(other_t_offset, ','));
    res.splice(res.end(), opt);
    new_macro(res, tname, true);
}

void Parser::selective_sanitize() {
    Token     T = cur_tok;
    long      n = 10000;
    TokenList nb;
    read_optarg_nopar(nb);
    if (!nb.empty()) n = scan_int(nb, T);
    TokenList chars = read_arg();
    token_ns::sanitize_one(chars);
    Token res = get_r_token(true);
    if (chars.empty()) return;
    TokenList L = get_mac_value(res);
    token_ns::sanitize_one(L, chars, n);
    new_macro(L, res);
}

// --------------------------------------------------
// Implementation of \setkeys
// We start with some auxiliary functions

// Ctor
XkvSetkeys::XkvSetkeys(Parser *P) : P(P) {
    comma_token     = P->hash_table.comma_token;
    equals_token    = P->hash_table.equals_token;
    na_token        = P->hash_table.xkv_na_token;
    fams_token      = P->hash_table.xkv_fams_token;
    rm_token        = P->hash_table.xkv_rm_token;
    savevalue_token = P->hash_table.savevalue_token;
    usevalue_token  = P->hash_table.usevalue_token;
    in_pox          = false;
    no_err          = false;
    set_all         = false;
}

// This reads and manages the list of families
void XkvSetkeys::fetch_fams() {
    fams = P->read_arg();
    extract_keys(fams, Fams);
}

// Special case of ExecuteOptions
void XkvSetkeys::special_fams() {
    fams = P->XKV_parse_filename();
    extract_keys(fams, Fams);
}

// Reads the optional list of keys that should not be set
void XkvSetkeys::fetch_na() {
    P->read_optarg_nopar(na);
    extract_keys(na, Na);
}

void XkvSetkeys::fetch_keys(bool c) {
    if (!c)
        keyvals = P->get_mac_value(rm_token); // case of \setrmkeys
    else
        keyvals = P->read_arg();
    if (the_parser.tracing_commands()) {
        Logger::finish_seq();
        the_log << "setkeys -> " << keyvals << "\n";
    }
    extract_keys(keyvals, Keys);
}

// Evaluate now everything
void XkvSetkeys::finish() {
    P->new_macro(xkv_prefix, P->hash_table.xkv_prefix_token);
    P->new_macro(fams, fams_token);
    P->new_macro(na, na_token);
    if (!delayed.empty()) delayed.pop_back(); // remove trailing comma
    if (the_parser.tracing_commands()) {
        Logger::finish_seq();
        the_log << "setkeys <- " << action << "\n";
    }
    P->new_macro(delayed, rm_token);
    P->back_input(action);
}

// Extract the keys from a list, result in the vector R
// Will store a normalised version of the list in L
void XkvSetkeys::extract_keys(TokenList &L, std::vector<std::string> &R) {
    if (L.empty()) return;
    Token     T = comma_token;
    TokenList res;
    TokenList z;
    int       bl = 0;
    L.push_back(T);
    for (;;) {
        if (L.empty()) break;
        Token x = L.front();
        L.pop_front();
        token_ns::check_brace(x, bl);
        if (bl == 0 && x.is_a_char() && x.char_val() == ',') {
            token_ns::remove_first_last_space(z);
            if (z.empty()) continue;
            std::string s = xkv_ns::find_key_of(z, 1);
            R.push_back(s);
            res.splice(res.end(), z);
            res.push_back(T);
        } else
            z.push_back(x);
    }
    if (!res.empty()) res.pop_back(); // remove final comma
    L.swap(res);
}

// May set \XKV@prefix \XKV@fams \XKV@tfam \XKV@header \XKV@tkey \XKV@na
void Parser::setkeys(bool c) {
    XkvSetkeys data(this);
    data.run(c);
}

auto xkv_ns::is_Gin(const TokenList &x) -> bool {
    auto C = x.begin();
    auto E = x.end();
    if (C == E) return false;
    if (*C != Token(letter_t_offset, 'G')) return false;
    ++C;
    if (C == E) return false;
    if (*C != Token(letter_t_offset, 'i')) return false;
    ++C;
    if (C == E) return false;
    if (*C != Token(letter_t_offset, 'n')) return false;
    ++C;
    return C == E;
}

void XkvSetkeys::run(bool c) {
    no_err  = P->remove_initial_plus(false);
    set_all = P->remove_initial_plus(true);
    P->xkv_fetch_prefix();
    fams = P->read_arg();
    if (xkv_ns::is_Gin(fams)) {
        TokenList L = the_parser.read_arg();
        L.push_back(comma_token);
        the_parser.new_macro(L, the_parser.hash_table.locate("Gin@keys"));
        return;
    }
    extract_keys(fams, Fams);
    fetch_na();
    fetch_keys(c);
    check_preset("preseth");
    set_aux(keyvals, -1);
    check_preset("presett");
    finish();
}

void XkvSetkeys::check_preset(String s) {
    Buffer &B = local_buf;
    auto    N = Fams.size();
    for (size_t i = 0; i < N; i++) {
        xkv_ns::makehd(Fams[i]);
        B = "XKV@" + xkv_header + s;
        if (P->hash_table.is_defined(B)) {
            Token     T = P->hash_table.locate(B);
            TokenList W = P->get_mac_value(T);
            set_aux(W, to_signed(i));
        }
    }
}

// If i=-1 this is the normal case, else
// we are in preset and i is the family index
void XkvSetkeys::set_aux(TokenList &W, long idx) {
    TokenList val;
    XkvToken  cur;
    for (;;) {
        if (W.empty()) return;
        token_ns::split_at(comma_token, W, val);
        token_ns::remove_initial_spaces(val);
        if (val.empty()) continue;
        cur.set_initial(val);
        cur.extract();
        if (cur.key_empty()) {
            if (!cur.val_empty()) P->parse_error(P->err_tok, "No key for a value");
            continue;
        }
        if (cur.ignore_this(Na)) continue;
        if (idx >= 0 && cur.ignore_this(Keys)) continue;
        bool found = false;
        auto N     = Fams.size();
        // if idx>=0, execute the loop once
        size_t i = 0;
        if (idx >= 0) {
            i = to_unsigned(idx);
            N = to_unsigned(idx + 1);
        }
        for (; i < N; i++) {
            if (!cur.is_defined(Fams[i])) continue;
            Token T = P->hash_table.last_tok;
            found   = true;
            run_key(T, cur, Fams[i]);
            if (!set_all && found) break;
        }
        if (!found) check_action(cur);
    }
}

// Splits key=val into pieces
void XkvToken::extract() {
    TokenList key;
    value   = initial;
    has_val = token_ns::split_at(Token(other_t_offset, '='), value, key);
    token_ns::remove_first_last_space(key);
    token_ns::remove_first_last_space(value);
    token_ns::remove_ext_braces(value);
    token_ns::remove_ext_braces(value);
    keyname   = xkv_ns::find_key_of(key, 1);
    has_save  = xkv_is_save;
    is_global = xkv_is_global;
    token_ns::remove_first_last_space(value);
}

// True if the key is in the ignore list
auto XkvToken::ignore_this(std::vector<std::string> &igna) -> bool {
    auto n = igna.size();
    for (size_t i = 0; i < n; i++)
        if (keyname == igna[i]) return true;
    return false;
}

// Constructs the header, in xkv_header
// Constructs the 3 macros in action
void XkvToken::prepare(const std::string &fam) {
    Hashtab &H = the_parser.hash_table;
    // We start constructing the three macros
    Buffer &B = local_buf;
    action.push_back(H.def_token);
    action.push_back(H.xkv_tkey_token);
    B           = keyname;
    TokenList L = B.str_toks11(false);
    the_parser.brace_me(L);
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.xkv_tfam_token);
    B = fam;
    L = B.str_toks11(false);
    the_parser.brace_me(L);
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.xkv_header_token);
    xkv_ns::makehd(fam);
    L = B.str_toks11(false);
    the_parser.brace_me(L);
    action.splice(action.end(), L);
}

// Returns true if the key is defined
auto XkvToken::is_defined(const std::string &fam) -> bool {
    xkv_ns::makehd(fam);
    local_buf += keyname;
    return the_parser.hash_table.is_defined(local_buf);
}

// Returns true if must be saved; may set xkv_is_global
auto XkvToken::check_save() -> bool {
    if (has_save) {
        xkv_is_global = is_global;
        return true;
    }
    xkv_ns::find_aux(0);
    if (!the_parser.hash_table.is_defined(local_buf)) return false;
    return the_parser.find_a_save_key(keyname);
}

// This is called if the value must be saved
void XkvSetkeys::save_key(const std::string &Key, TokenList &L) {
    local_buf = "XKV@" + xkv_header + Key + "@value";
    Token T   = P->hash_table.locate(local_buf);
    P->new_macro(L, T, xkv_is_global);
}

// This is called when the key is defined
// What happens if you say \savevalue{keya} = \usevalue{keyb} ?
void XkvSetkeys::run_key(Token mac, XkvToken &cur, const std::string &fam) {
    cur.prepare(fam);
    more_action(cur.get_action());
    bool               s   = cur.check_save();
    const std::string &Key = cur.get_name();
    TokenList          L   = cur.get_code();
    if (s) save_key(Key, L);
    if (cur.val_empty())
        run_default(Key, mac, s);
    else {
        TokenList LL = cur.get_code();
        replace_pointers(LL);
        action.push_back(mac);
        P->brace_me(LL);
        more_action(LL);
    }
}

// Interprets \usevalue{foo} in the list L
void XkvSetkeys::replace_pointers(TokenList &L) {
    Buffer &  B = local_buf;
    TokenList res;
    int       n = 1000;
    for (;;) {
        if (L.empty()) break;
        Token t = L.front();
        if (t.is_a_left_brace()) {
            L.fast_get_block(res);
            continue;
        }
        L.pop_front();
        if (t != usevalue_token) {
            res.push_back(t);
            continue;
        }
        if (!L.empty()) t = L.front();
        if (!t.is_a_left_brace()) {
            P->parse_error(P->err_tok, "Invalid \\usevalue token", "invalid usevalue");
            continue;
        }
        --n;
        if (n < 0) {
            P->parse_error(P->err_tok, "Replace pointer aborted, (infinite loop?)", "key loop");
            res.clear();
            break;
        }
        TokenList key = L.fast_get_block();
        token_ns::remove_ext_braces(key);
        std::string Key = P->list_to_string_c(key, "Argument of \\savevalue");
        local_buf       = "XKV@" + xkv_header + Key + "@value";
        if (P->hash_table.is_defined(B)) {
            TokenList w = P->get_mac_value(P->hash_table.last_tok);
            L.splice(L.begin(), w);
        } else {
            B = "No value recorded for key `" + Key + "'; ignored";
            P->parse_error(P->err_tok, B, "no val recorded");
        }
    }
    L.swap(res);
}

//
void XkvSetkeys::run_default(const std::string &Key, Token mac, bool s) {
    Buffer &B = local_buf;
    B         = xkv_header + Key + "@default";
    if (!P->hash_table.is_defined(B)) {
        B = "No value specified for key `" + Key + "'";
        P->parse_error(P->err_tok, B);
        return;
    }
    Token     T = P->hash_table.locate(B);
    TokenList L = P->get_mac_value(T);
    if (L.empty() || L.front() != mac) {
        // no hack needed
        action.push_back(T);
        return;
    }
    L.pop_front();
    // We apply mac to L via setkeys and not the macro
    TokenList args;
    if (!L.empty()) {
        if (L.front().is_a_left_brace()) { // get the block without the braces
            L.fast_get_block(args);
            args.pop_front();
            args.pop_back();
        } else { // otherwise get just a token
            args.push_back(L.front());
            L.pop_front();
        }
    }
    // We have now in args the real argument.
    if (s) save_key(Key, args);
    replace_pointers(args);
    action.push_back(mac);
    P->brace_me(args);
    more_action(args);
}

// This considers the case when \ProcessOptionsX has seen cur_opt
void XkvSetkeys::check_action(XkvToken &cur) {
    TokenList cur_opt = cur.get_all();
    if (in_pox) {
        classes_ns::unknown_optionX(cur_opt, action);
        return;
    }
    // This is the normal case
    if (no_err)
        new_unknown(cur_opt);
    else
        P->parse_error(P->err_tok, "Undefined key: ", cur.get_name(), "undefined key");
}
void Parser::formatdate() {
    flush_buffer();
    std::string s = sT_arg_nopar();
    FormatDate  FP;
    if (!FP.interpret(s, err_tok)) { the_log << "Date to scan was " << s << "\n"; }
    Xml *X = new Xml(std::string("date"), nullptr);
    the_stack.add_last(X);
    AttList &AL = X->id.get_att();
    AL.push_back(std::string("year"), std::string(std::to_string(FP.year)));
    AL.push_back(std::string("month"), std::string(std::to_string(abs(FP.month))));
    AL.push_back(std::string("day"), std::string(std::to_string(FP.day)));
}

// True if ok, parse_error otherwise
auto FormatDate::sort() -> bool {
    if (month > day) std::swap(month, day);
    if (day > year) { // else ok
        std::swap(year, day);
        if (month > day) std::swap(month, day);
    }
    if (day > 31) {
        the_parser.parse_error(err_tok, "Date has two fields with value>31");
        return false;
    }
    if (day < 0) {
        the_parser.parse_error(err_tok, "Date has two month fields");
        return false;
    }
    return true;
}

// True if ok, parse_error otherwise
// assumes current character is a digit.
auto FormatDate::scan_a_field(Buffer &B, int &res) -> bool {
    res = 0;
    for (;;) {
        if (B.at_eol()) return true;
        auto c = B.head();
        if (std::isdigit(c) == 0) { return true; }
        B.advance();
        res = 10 * res + (c - '0');
        if (res > 9999) {
            the_parser.parse_error(err_tok, "Too many digits in date field");
            return false;
        }
    }
}

// True if ok. Assume cur char is non-space
auto FormatDate::scan_a_month(Buffer &B, int &res) -> bool {
    if (B.skip_word_ci("janvier")) {
        res = -1;
        return true;
    }
    if (B.skip_word_ci("januar")) {
        res = -1;
        return true;
    }
    if (B.skip_word_ci("jan")) {
        res = -1;
        return true;
    }
    if (B.skip_word_ci("fevrier")) {
        res = -2;
        return true;
    }
    if (B.skip_word_ci("f\303\251vrier")) {
        res = -2;
        return true;
    }
    if (B.skip_word_ci("februar")) {
        res = -2;
        return true;
    }
    if (B.skip_word_ci("feb")) {
        res = -2;
        return true;
    }
    if (B.skip_word_ci("fev")) {
        res = -2;
        return true;
    }
    if (B.skip_word_ci("mars")) {
        res = -3;
        return true;
    }
    if (B.skip_word_ci("march")) {
        res = -3;
        return true;
    }
    if (B.skip_word_ci("mar")) {
        res = -3;
        return true;
    }
    if (B.skip_word_ci("avril")) {
        res = -4;
        return true;
    }
    if (B.skip_word_ci("april")) {
        res = -4;
        return true;
    }
    if (B.skip_word_ci("apr")) {
        res = -4;
        return true;
    }
    if (B.skip_word_ci("avr")) {
        res = -4;
        return true;
    }
    if (B.skip_word_ci("mai")) {
        res = -5;
        return true;
    }
    if (B.skip_word_ci("may")) {
        res = -5;
        return true;
    }
    if (B.skip_word_ci("juin")) {
        res = -6;
        return true;
    }
    if (B.skip_word_ci("june")) {
        res = -6;
        return true;
    }
    if (B.skip_word_ci("jun")) {
        res = -6;
        return true;
    }
    if (B.skip_word_ci("juillet")) {
        res = -7;
        return true;
    }
    if (B.skip_word_ci("july")) {
        res = -7;
        return true;
    }
    if (B.skip_word_ci("jul")) {
        res = -7;
        return true;
    }
    if (B.skip_word_ci("aout")) {
        res = -8;
        return true;
    }
    if (B.skip_word_ci("ao\303\273t")) {
        res = -8;
        return true;
    }
    if (B.skip_word_ci("august")) {
        res = -8;
        return true;
    }
    if (B.skip_word_ci("aug")) {
        res = -8;
        return true;
    }
    if (B.skip_word_ci("septembre")) {
        res = -9;
        return true;
    }
    if (B.skip_word_ci("september")) {
        res = -9;
        return true;
    }
    if (B.skip_word_ci("sep")) {
        res = -9;
        return true;
    }
    if (B.skip_word_ci("octobre")) {
        res = -10;
        return true;
    }
    if (B.skip_word_ci("october")) {
        res = -10;
        return true;
    }
    if (B.skip_word_ci("oct")) {
        res = -10;
        return true;
    }
    if (B.skip_word_ci("novembre")) {
        res = -11;
        return true;
    }
    if (B.skip_word_ci("novembre")) {
        res = -11;
        return true;
    }
    if (B.skip_word_ci("nov")) {
        res = -11;
        return true;
    }
    if (B.skip_word_ci("decembre")) {
        res = -12;
        return true;
    }
    if (B.skip_word_ci("december")) {
        res = -12;
        return true;
    }
    if (B.skip_word_ci("dec")) {
        res = -12;
        return true;
    }
    if (B.skip_word_ci("d\303\251cembre")) {
        res = -12;
        return true;
    }
    return false;
}

// true if OK.
auto FormatDate::scan_next(Buffer &B, int &res) -> bool {
    B.skip_sp_tab_nl();
    if (B.at_eol()) {
        the_parser.parse_error(err_tok, "Missing fields in date");
        return false;
    }
    auto c = B.head();
    if (c == '-' || c == '/' || c == ',' || c == ';') {
        B.advance();
        B.skip_sp_tab_nl();
        if (B.at_eol()) {
            the_parser.parse_error(err_tok, "Missing fields in date");
            return false;
        }
        c = B.head();
    }
    if (std::isdigit(c) != 0) return scan_a_field(B, res);
    if (!scan_a_month(B, res)) {
        the_parser.parse_error(err_tok, "Expected digits or a month in letters");
        return false;
    }
    return true;
}

auto FormatDate::next_format_char(Buffer &B) -> int {
    if (B.at_eol()) return 0;
    auto c = B.next_char();
    if (c == 'j' || c == 'J' || c == 'D' || c == 'd') return 2;
    if (c == 'M' || c == 'm') return 1;
    if (c == 'Y' || c == 'y' || c == 'A' || c == 'a') return 3;
    return 0;
}

// this is called if a random character is after the date
auto FormatDate::parse_format(Buffer &B) -> bool {
    int  c1 = 0, c2 = 0, c3 = 0;
    bool ok = false;
    c1      = next_format_char(B);
    if (c1 != 0) c2 = next_format_char(B);
    if (c2 != 0) c3 = next_format_char(B);
    if (c3 != 0) {
        B.skip_sp_tab_nl();
        if (B.at_eol()) ok = true;
    }
    if (c1 == c2 || c1 == c3 || c2 == c3) ok = false;
    if (!ok) {
        the_parser.parse_error(err_tok, "Illegal date format");
        return false;
    }
    if (c1 > c2) {
        std::swap(c1, c2);
        std::swap(month, day);
    }
    if (c2 > c3) { // else ok
        std::swap(c2, c3);
        std::swap(day, year);
        if (c1 > c2) {
            std::swap(c1, c2);
            std::swap(month, day);
        }
    }
    // now c1=m c2=d c3=y
    if (month > 12) {
        the_parser.parse_error(err_tok, "Bad month field");
        return false;
    }
    if (day > 31 || day < 0) {
        the_parser.parse_error(err_tok, "Bad day field");
        return false;
    }
    if (year < 0) {
        the_parser.parse_error(err_tok, "Bad year field");
        return false;
    }
    return true;
}

auto FormatDate::parse(Buffer &B) -> bool {
    if (!scan_next(B, month)) return false;
    if (!scan_next(B, day)) return false;
    if (!scan_next(B, year)) return false;
    B.skip_sp_tab_nl();
    if (!B.at_eol()) return parse_format(B);
    sort();
    if (year > 31) {
        if (month < 0) return true;
        if (month == day) return true;
        if (day > 12) return true;
    }
    the_parser.parse_error(err_tok, "Unable to distinguish between year day and month");
    return false;
}

// fills year month day
auto FormatDate::interpret(const std::string &s, Token T) -> bool {
    err_tok   = T;
    Buffer &B = local_buf;
    B         = s;
    B.ptrs.b  = 0;
    bool res  = parse(B);
    if (!res) {
        month = 1;
        day   = 1;
        year  = 2008;
    }
    return res;
}

// \@dblarg\foo provides as optional argument of \foo the required one
void Parser::dbl_arg() {
    TokenList L = read_arg();
    skip_initial_space();
    if (cur_tok.is_valid()) back_input();
    if (!cur_tok.is_open_bracket()) {
        TokenList A = read_arg();
        brace_me(A);
        TokenList B = A;
        B.push_front(Token(other_t_offset, '['));
        B.push_back(Token(other_t_offset, ']'));
        B.splice(B.end(), A);
        back_input(B);
    }
    back_input(L);
}

// \numberwithin[\c]{foo}{bar}
// evaluate: \@cons\cl@bar{{foo}}; i.e. M_cons(\cl@bar, {foo}).
// and \def\thefoo{\thebar.\c{foo}}
void Parser::numberwithin() {
    TokenList L;
    read_optarg(L);
    if (L.empty()) L.push_back(hash_table.arabic_token);
    TokenList foo_list = read_arg();
    TokenList A        = foo_list;
    TokenList bar_list = read_arg();
    Buffer &  b        = local_buf;
    if (csname_ctr(foo_list, b)) {
        bad_counter0();
        return;
    }
    if (counter_check(b, false)) return;
    auto fooname = b.substr(2);
    if (csname_ctr(bar_list, b)) {
        bad_counter0();
        return;
    }
    if (counter_check(b, false)) return;
    auto barname      = b.substr(2);
    b                 = "cl@" + barname;
    Token clbar_token = hash_table.locate(b);
    brace_me(A);
    TokenList B = A;
    M_cons(clbar_token, B);
    b            = "the" + barname;
    Token thebar = hash_table.locate(b);
    b            = "the" + fooname;
    Token thefoo = hash_table.locate(b);
    L.splice(L.end(), A);
    L.push_front(Token(other_t_offset, '.'));
    L.push_front(thebar);
    new_macro(L, thefoo, true);
}

auto Parser::make_label_inner(const std::string &name) -> std::string {
    TokenList res;
    Buffer &  b = local_buf;
    b           = "the" + name;
    res.push_back(hash_table.locate(b));
    b = "p@" + name;
    if (hash_table.is_defined(b)) // ignore \p@foo if undefined
        res.push_front(hash_table.last_tok);
    b.clear();
    if (list_to_string(res, b)) { // evaluate the label
        parse_error(err_tok, "Illegal tokens in \\makelabel");
        return "";
    }
    return b;
}

// executes \stepcounter{foo}\makelabel*{foo}
// the name of the counter is in local_buf abd L
void Parser::refstepcounter_inner(TokenList &L, bool star) {
    std::string name = local_buf;
    brace_me(L);
    L.push_front(hash_table.stepcounter_token);
    T_translate(L);
    std::string v = make_label_inner(name);
    string_define(0, v, false);
    if (star) the_stack.add_new_anchor();
}

// takes a string as argument and translates the thing
void Parser::refstepcounter(const std::string &S, bool star) {
    Buffer &b = local_buf;
    b.clear();
    b.append(S);
    TokenList L = b.str_toks11(true);
    refstepcounter(L, star);
}

// user function: read an argument and back_input
void Parser::refstepcounter() {
    bool      star = remove_initial_star();
    TokenList L    = read_arg();
    flush_buffer();
    refstepcounter(L, star);
}

// Case where the nale of the label is in L
void Parser::refstepcounter(TokenList &L, bool star) {
    Buffer &b = local_buf;
    b.clear();
    TokenList L1 = L;
    if (list_to_string(L1, b)) {
        bad_counter0();
        return;
    }
    refstepcounter_inner(L, star);
}

// If the argument is foo, executes \global\c@foo=0 with a test
// remembers locally the name.
void Parser::T_use_counter(const std::string &s) {
    string_define(1, s, false);
    Buffer &b     = local_buf;
    b             = "c@" + s;
    Token       T = hash_table.locate(b);
    EqtbCmdChr &E = hash_table.eqtb[T.eqtb_loc()];
    if (E.val.cmd != assign_int_cmd) return;
    word_define(E.val.chr, 0, true);
}

// \usecounter{foo}. Signals an error if foo is not a counter
void Parser::T_use_counter() {
    TokenList L = read_arg();
    Buffer    b; // will contain c@foo
    if (csname_ctr(L, b)) {
        bad_counter0();
        return;
    }
    if (counter_check(b, false)) return;
    T_use_counter(b.substr(2));
}

// \@ifdefinable\foo{\bar} errs if not definable, executes \vbar otherwise.
void Parser::T_ifdefinable() {
    bool      bad = read_token_arg(cur_tok);
    Token     C   = cur_tok;
    TokenList B   = read_arg();
    if (bad) return;
    if (C.not_a_cmd())
        bad = true;
    else if (C == hash_table.relax_token)
        bad = true;
    else if (!hash_table.eqtb[C.eqtb_loc()].val.is_undef_or_relax())
        bad = true;
    else {
        if (C.is_in_hash()) {
            auto s = hash_table[C.hash_loc()];
            if (s[0] == 'e' && s[1] == 'n' && s[2] == 'd') bad = true;
        }
    }
    if (!bad)
        back_input(B);
    else
        parse_error(err_tok, "Undefinable command ", C, "", "bad");
}

// Here L is the optional argument of the enumerate env, for instance [(i)]
// and ctr the counter, for instance enumiv
// executes \def\@itemlabel{(\theenumiv)}
// redefines \def\theenumiv{\roman{enumiv}}
auto Parser::optional_enumerate(TokenList &L, const std::string &ctr) -> bool {
    Hashtab & H = hash_table;
    TokenList res;
    int       b   = 0;
    Buffer &  B   = local_buf;
    B             = "the" + ctr;
    Token the_ctr = H.locate(B);
    Token cmd     = H.relax_token;
    for (;;) {
        if (L.empty()) break;
        Token T = L.front();
        L.pop_front();
        if (T.is_a_brace()) {
            if (T.is_a_left_brace())
                b++;
            else
                b--;
        }
        if (b != 0) {
            res.push_back(T);
            continue;
        }
        if (T == Token(other_t_offset, '1')) {
            cmd = H.arabic_token;
        } else if (T == Token(letter_t_offset, 'A')) {
            cmd = H.Alph_token;
        } else if (T == Token(letter_t_offset, 'a')) {
            cmd = H.alph_token;
        } else if (T == Token(letter_t_offset, 'i')) {
            cmd = H.roman_token;
        } else if (T == Token(letter_t_offset, 'I')) {
            cmd = H.Roman_token;
        } else {
            res.push_back(T);
            continue;
        }
        res.push_back(the_ctr);
    }
    if (cmd == H.relax_token) return false; // Invalid argument, no warning ?
    auto *X = new Macro(res);
    Token t = hash_table.itemlabel_token;
    mac_define(t, X, false, rd_always, user_cmd);
    B                       = ctr;
    TokenList the_ctr_value = B.str_toks11(true);
    brace_me(the_ctr_value);
    the_ctr_value.push_front(cmd);
    auto *Y = new Macro(the_ctr_value);
    mac_define(the_ctr, Y, false, rd_always, user_cmd);
    return true;
}

void token_ns::int_to_roman(Buffer &b, long n) {
    switch (n) {
    case 1: b += "i"; break;
    case 2: b += "ii"; break;
    case 3: b += "iii"; break;
    case 4: b += "iv"; break;
    case 5: b += "v"; break;
    case 6: b += "vi"; break;
    case 7: b += "vii"; break;
    case 8: b += "viii"; break;
    case 9: b += "ix"; break;
    default: b += "x";
    }
}

// Start of list, itemize, enumerate, or description

void Parser::T_listenv(symcodes x) {
    leave_h_mode();
    bool is_enum   = x == enumerate_cmd;
    auto listdepth = eqtb_int_table[list_depth_code].val;
    listdepth += is_enum ? 100 : 1;
    word_define(list_depth_code, listdepth, false);
    auto n = listdepth;
    if (is_enum) n = n / 100;
    n         = n % 100;
    Buffer &b = local_buf;
    b         = is_enum ? "enum" : "Enum";
    token_ns::int_to_roman(b, n);
    std::string list_ctr = b;
    auto        np       = x == list_cmd ? "user_list" : x == itemize_cmd ? "simple" : x == enumerate_cmd ? "ordered" : "description";
    Token       t        = hash_table.itemlabel_token;
    M_let_fast(t, hash_table.relax_token, false);
    T_use_counter(list_ctr);
    TokenList L;
    if (x == enumerate_cmd) {
        read_optarg_nopar(L);
        // Token cmd = hash_table.relax_token;  unused (why ?)
        if (!optional_enumerate(L, list_ctr)) {
            b = "labelenum";
            token_ns::int_to_roman(b, n);
            M_let_fast(t, hash_table.locate(b), false);
        }
    }
    if (x == list_cmd) {
        TokenList LL = read_arg();
        auto *    X  = new Macro(LL);
        mac_define(t, X, false, rd_always, user_cmd);
        TokenList L2 = read_arg();
        back_input(L2); // remove a pair of braces here
    }
    if (x == enumerate_cmd) {
        b = "enum";
        token_ns::int_to_roman(b, n);
        b += "@hook";
        Token T   = hash_table.locate(b);
        auto  pos = T.eqtb_loc();
        if (!hash_table.eqtb[pos].val.is_undef()) back_input(T);
    }
    Xml *res = new Xml(the_names["list"], nullptr);
    the_stack.push(the_names["list"], res);
    res->id.add_attribute(the_names["type"], the_names[np]);
}

// converts T1/ OT2 into a Unicode character (expandable command)
// what=1 is OT2, everything else is T1
void Parser::E_parse_encoding(bool vb, subtypes what) {
    Token T = cur_tok;
    auto  c = scan_braced_int(T);
    long  r = 0;
    if (what == 1) {
        switch (c) {
        case 0: r = 0x40A; break; // 0-15
        case 1: r = 0x409; break;
        case 2: r = 0x40F; break;
        case 3: r = 0x42D; break;
        case 4: r = 0x406; break;
        case 5: r = 0x404; break;
        case 6: r = 0x402; break;
        case 7: r = 0x40B; break;
        case 8: r = 0x45A; break;
        case 9: r = 0x459; break;
        case 10: r = 0x45F; break;
        case 11: r = 0x44D; break;
        case 12: r = 0x456; break;
        case 13: r = 0x454; break;
        case 14: r = 0x452; break;
        case 15: r = 0x45B; break;
        case 16: r = 0x42E; break; // 16-31
        case 17: r = 0x416; break;
        case 18: r = 0x419; break;
        case 19: r = 0x401; break;
        case 20: r = 0x474; break;
        case 21: r = 0x472; break;
        case 22: r = 0x405; break;
        case 23: r = 0x42F; break;
        case 24: r = 0x44E; break;
        case 25: r = 0x436; break;
        case 26: r = 0x439; break;
        case 27: r = 0x451; break;
        case 28: r = 0x475; break;
        case 29: r = 0x473; break;
        case 30: r = 0x455; break;
        case 31: r = 0x44F; break;
        case 32: r = 0x308; break; // 32 - 47
        case 33: r = '!'; break;
        case 34: r = 0x201D; break; // right double quotation mark
        case 35: r = 0x462; break;
        case 36: r = 0x306; break;
        case 37: r = '%'; break;
        case 38: r = 0x301; break;
        case 39: r = 0x2019; break; // right single quotation mark
        case 40: r = '('; break;
        case 41: r = ')'; break;
        case 42: r = '*'; break;
        case 43: r = 0x463; break;
        case 44: r = ','; break;
        case 45: r = '-'; break;
        case 46: r = '.'; break;
        case 47: r = '/'; break;
        case 48: r = '0'; break; // 48 - 63
        case 49: r = '1'; break;
        case 50: r = '2'; break;
        case 51: r = '3'; break;
        case 52: r = '4'; break;
        case 53: r = '5'; break;
        case 54: r = '6'; break;
        case 55: r = '7'; break;
        case 56: r = '8'; break;
        case 57: r = '9'; break;
        case 58: r = ':'; break;
        case 59: r = ';'; break;
        case 60: r = 0xAB; break;
        case 61: r = 0xB9; break;
        case 62: r = 0xBB; break;
        case 63: r = '?'; break;
        case 64: r = 0x306; break; // 64 -79
        case 65: r = 0x410; break;
        case 66: r = 0x411; break;
        case 67: r = 0x426; break;
        case 68: r = 0x414; break;
        case 69: r = 0x415; break;
        case 70: r = 0x424; break;
        case 71: r = 0x413; break;
        case 72: r = 0x425; break;
        case 73: r = 0x418; break;
        case 74: r = 0x408; break;
        case 75: r = 0x41A; break;
        case 76: r = 0x41B; break;
        case 77: r = 0x41C; break;
        case 78: r = 0x41D; break;
        case 79: r = 0x41E; break;
        case 80: r = 0x41F; break; // 80 - 95
        case 81: r = 0x427; break;
        case 82: r = 0x420; break;
        case 83: r = 0x421; break;
        case 84: r = 0x422; break;
        case 85: r = 0x423; break;
        case 86: r = 0x412; break;
        case 87: r = 0x429; break;
        case 88: r = 0x428; break;
        case 89: r = 0x42B; break;
        case 90: r = 0x417; break;
        case 91: r = '['; break;
        case 92: r = 0x201C; break; //  left double quotation mark
        case 93: r = ']'; break;
        case 94: r = 0x42C; break;
        case 95: r = 0x42A; break;
        case 96: r = 0x2018; break; // 96 - 111 left single quotation mark
        case 97: r = 0x430; break;
        case 98: r = 0x431; break;
        case 99: r = 0x446; break;
        case 100: r = 0x434; break;
        case 101: r = 0x435; break;
        case 102: r = 0x444; break;
        case 103: r = 0x433; break;
        case 104: r = 0x445; break;
        case 105: r = 0x438; break;
        case 106: r = 0x458; break;
        case 107: r = 0x43A; break;
        case 108: r = 0x43B; break;
        case 109: r = 0x43C; break;
        case 110: r = 0x43D; break;
        case 111: r = 0x43E; break;
        case 112: r = 0x43F; break; // 112 - 127
        case 113: r = 0x447; break;
        case 114: r = 0x440; break;
        case 115: r = 0x441; break;
        case 116: r = 0x442; break;
        case 117: r = 0x443; break;
        case 118: r = 0x432; break;
        case 119: r = 0x449; break;
        case 120: r = 0x448; break;
        case 121: r = 0x44B; break;
        case 122: r = 0x437; break;
        case 123: r = '-'; break;
        case 124: r = 0x2014; break; // em dash
        case 125: r = 0x2116; break; // numero sign
        case 126: r = 0x44C; break;
        case 127: r = 0x44A; break;
        }
    } else if (what == 2) { // OT1 128 bits, parttly implemented
        if (33 <= c && c <= 123) r = c;
        switch (c) {
        case 0: r = 0x393; break;  // Gamma
        case 1: r = 0x394; break;  // Delta
        case 2: r = 0x398; break;  // Theta
        case 3: r = 0x39B; break;  // Lamnda
        case 4: r = 0x39E; break;  // Xi
        case 5: r = 0x3A0; break;  // Pi
        case 6: r = 0x3A3; break;  // Sigma
        case 7: r = 0x3A5; break;  // Upsilon
        case 8: r = 0x3A6; break;  // Phi
        case 9: r = 0x3A8; break;  // Psi
        case 10: r = 0x3A9; break; // Omega
        case 25: r = 0xDF; break;  // ss
        case 26: r = 0xE6; break;  // ae
        case 27: r = 0x153; break; // oe
        case 28: r = 0xF8; break;  // \o
        case 29: r = 0xC6; break;  // AE
        case 30: r = 0x152; break; // OE
        case 31: r = 0xD8; break;  // \O
        case 60: r = 0xA1; break;  // exclam
        case 62: r = 0xBF; break;  // question
        case 92:
        case 94:
        case 95: r = 0; break;
        }
    } else { // T1
        if (c == 0xFF)
            r = 0xDF; // \ss
        else if (c == 0xDF)
            r = 0x0; // SS is not a char in Unicode
        else if (c == 0xD7)
            r = 0x152; // \OE
        else if (c == 0xF7)
            r = 0x153; //\oe
        else if (0xC0 <= c && c <= 255)
            r = c;
        else if (33 <= c && c <= 126)
            r = c;
        else if (c == 32)
            r = 0x2423; // visible space
        else if (c == 127)
            r = '-'; // is this ok ?
        // if c<32 we reject
        else
            switch (c) {
            case 128: r = 0x102; break; // A breve
            case 129: r = 0x104; break; // A ogonek
            case 130: r = 0x106; break; // C acute
            case 131: r = 0x10C; break; // C caron
            case 132: r = 0x10E; break; // D caron
            case 133: r = 0x11A; break; // E caron
            case 134: r = 0x118; break; // E ogonek
            case 135: r = 0x11E; break; // G breve
            case 136: r = 0x139; break; // L acute
            case 137: r = 0x13D; break; // L caron
            case 138: r = 0x141; break; // L stroke
            case 139: r = 0x147; break; // N acute
            case 140: r = 0x143; break; // N caron
            case 141: r = 0x14A; break; // Eng
            case 142: r = 0x150; break; // O double acute
            case 143: r = 0x154; break; // R acute
            case 144: r = 0x158; break; // R caron
            case 145: r = 0x15A; break; // S acute
            case 146: r = 0x160; break; // S caron
            case 147: r = 0x15E; break; // S cedilla
            case 148: r = 0x164; break; // T caron
            case 149: r = 0x162; break; // T cedilla
            case 150: r = 0x170; break; // U double acute
            case 151: r = 0x16E; break; // U ring
            case 152: r = 0x178; break; // Y diaeresis
            case 153: r = 0x179; break; // Z acute
            case 154: r = 0x17D; break; // Z caron
            case 155: r = 0x17B; break; // Z dot
            case 156: r = 0x132; break; // IJ
            case 157: r = 0x130; break; // I dot
            case 158: r = 0x111; break; // d stroke
            case 159: r = 0xA7; break;  // section
            case 160: r = 0x103; break; // a breve
            case 161: r = 0x105; break; // a ogonek
            case 162: r = 0x107; break; // c acute
            case 163: r = 0x10D; break; // c caron
            case 164: r = 0x10F; break; // d caron
            case 165: r = 0x11B; break; // e caron
            case 166: r = 0x119; break; // e ogonek
            case 167: r = 0x11F; break; // g breve
            case 168: r = 0x13A; break; // l acute
            case 169: r = 0x13E; break; // l caron
            case 170: r = 0x142; break; // l stroke
            case 171: r = 0x144; break; // n acure
            case 172: r = 0x148; break; // n caron
            case 173: r = 0x14B; break; // eng
            case 174: r = 0x151; break; // o double acute
            case 175: r = 0x155; break; // r acute
            case 176: r = 0x159; break; // r caron
            case 177: r = 0x15B; break; // s acute
            case 178: r = 0x161; break; // s caron
            case 179: r = 0x15F; break; // s cedilla
            case 180: r = 0x165; break; // t caron
            case 181: r = 0x163; break; // t cedilla
            case 182: r = 0x171; break; // u double acute
            case 183: r = 0x16F; break; // u ring
            case 184: r = 0xFF; break;  // y diaeresis
            case 185: r = 0x17A; break; // z acute
            case 186: r = 0x17E; break; // z caron
            case 187: r = 0x17C; break; // z dot
            case 188: r = 0x133; break; // ij
            case 189: r = 0xA1; break;  // inverted exclam
            case 190: r = 0xBF; break;  // inverted question
            case 191: r = 0xA3; break;  // pound
            }
    }
    if (vb) {
        Buffer &B = mac_buffer;
        B         = fmt::format("-> \\char\"{:X}", r);
        Logger::finish_seq();
        the_log << T << c << B << ".\n";
    }
    if (r == 0) {
        Buffer &B = err_buf;
        B         = "Invalid chararacter specification ";
        B.push_back(T);
        B.format("{{{}}}", c);
        signal_error(T, "bad char spec");
        return;
    }
    Token t = Token(other_t_offset, char32_t(to_unsigned(r)));
    back_input(t);
}
