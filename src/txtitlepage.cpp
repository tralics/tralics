// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txtitlepage.h"
#include "tralics/Bibliography.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"

namespace {
    Buffer     docspecial; // Buffer for document special things
    TpiOneItem Toi;

    auto is_tp_delim(char c) -> bool { return c == '<' || c == '\\' || c == '"'; }
} // namespace

namespace tpage_ns {
    void init_error();
    auto scan_item(Buffer &in, Buffer &out, char del) -> bool;
    auto next_item(Buffer &in, Buffer &out) -> tpi_vals;

} // namespace tpage_ns

// This is called in case of trouble.
void tpage_ns::init_error() { log_and_tty << "Syntax error in init file (line " << init_file_pos << ")\n"; }

// return tl_end if seen End, tl_empty if empty (or comment),
// tl_normal otherwise
auto Buffer::tp_fetch_something() -> tpa_line {
    ptrs.b = 0;
    if (starts_with("End")) return tl_end;
    skip_sp_tab();
    if (is_special_end()) return tl_empty;
    if (ptrs.b == 0) {
        tpage_ns::init_error();
        log_and_tty << data();
        log_and_tty << "Wanted End, or line starting with space\n";
        return tl_empty;
    }
    return tl_normal;
}

// Example of usage
// CESS  \makeRR <rrstart> "myatt" "docatt"
// ACSS  alias \makeRT "myTatt" "docTatt"
// CES   \title <title> "notitle"
// CEES  \author +<autlist> <au> "default"
// E     <UR> -
// CE    \URsop ?+ <UR>
// CEE   \Paris ?<UR> <Rocq>
// CCS   \myself \author "JG"
// AC    alias \foo

// CES \abstract e<abstract> "abstract"  % no par
// CES \abstract E<abstract> "abstract"  % with par

// Gets next non space. return false if EOL
auto Buffer::tp_next_char(char &res) -> bool {
    skip_sp_tab();
    if (is_special_end()) return false;
    res = head();
    advance();
    return true;
}

// Returns an item that start with del
// result is in tp_local_buf, return value is true if OK.
auto tpage_ns::scan_item(Buffer &in, Buffer &out, char del) -> bool {
    out.clear();
    if (del == '\\') { // scan a command
        while (std::isalpha(in.head()) != 0) out.push_back(in.next_char());
        return true;
    }
    if (del == '<') del = '>'; // else del is ""
    while (in.head() != del) {
        if (in.head() == 0) {
            tpage_ns::init_error();
            log_and_tty << "could not find end delimiter\n";
            out = "notfound";
            return false;
        }
        out.push_back(in.next_char());
    }
    in.advance();
    return true;
}

// Scans an item: prefix plus value. Result in Toi
// returns the type of the thing to create.
auto tpage_ns::next_item(Buffer &in, Buffer &out) -> tpi_vals {
    Toi.reset();
    char c = 0;
    if (!in.tp_next_char(c)) return tpi_noval;
    if (c == 'a' && in.skip_string("lias")) return tpi_alias;
    if (c == 'e' && in.skip_string("xecute")) return tpi_execute;
    if (c == 'a' && in.skip_string("ction")) return tpi_execute;
    if (!is_tp_delim(c)) {
        Toi.p1 = c;
        if (!in.tp_next_char(c)) return tpi_noval;
        if (!is_tp_delim(c)) {
            Toi.p2 = c;
            if (!in.tp_next_char(c)) return tpi_noval;
            if (!is_tp_delim(c)) return tpi_err;
        }
    }
    if (!tpage_ns::scan_item(in, out, c)) return tpi_err;
    Toi.value = out;
    if (c == '<') return tpi_elt;
    if (c == '\\') return tpi_cmd;
    return tpi_str;
}

// This reads up to four items.
// returns a number that tells what types have been read.
auto TitlePageFullLine::read() -> int {
    item1.reset();
    item2.reset();
    item3.reset();
    item4.reset();
    int      res = 0;
    tpi_vals w   = tpage_ns::next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item1   = Toi;
    item1.v = w;
    res += 1000 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item2   = Toi;
    item2.v = w;
    res += 100 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item3   = Toi;
    item3.v = w;
    res += 10 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item4   = Toi;
    item4.v = w;
    res += (w - 1);
    if (w == tpi_noval) return res;
    char c = 0;
    if (tp_main_buf.tp_next_char(c)) return -1;
    return res;
}

// First attempt at classification
// E=1 C=2 S=3 A=4 e=5
auto TitlePageFullLine::classify(int w, int state) -> tpi_vals {
    if (w == -1) return tpi_err; // forget about this
    flags = 0;
    if (item1.has_a_char() || item4.has_a_char()) return tpi_err;
    if (w == 0) return tpi_zero; // ignore this item
    if (w == 2133) {
        if (state != 0) return tpi_err;
    } else {
        if (state == 0) return tpi_err;
    }
    if (w == 1000) return item2.only_dash() ? tpi_E : tpi_err;
    if (w == 3000) return tpi_S;
    if (w == 4200 || w == 4233) {
        if (item2.has_a_char() || item3.has_a_char()) return tpi_err;
        if (state == 1 && w == 4233) return tpi_ACSS;
        if (state == 2 && w == 4200) return tpi_AC;
    } else if (w == 2230 || w == 2133) {
        if (item2.has_a_char() || item3.has_a_char()) return tpi_err;
        return w == 2230 ? tpi_CCS : tpi_CESS;
    } else if (w == 2100)
        return item2.quest_plus() && !item3.has_a_char() ? tpi_CE : tpi_err;
    else if (w == 5200)
        return !item2.has_a_char() ? tpi_EC : tpi_err;
    else if (w == 2110)
        return item2.question() && !item3.has_a_char() ? tpi_CEE : tpi_err;
    else if (w == 2113)
        return item2.plus() && !item3.has_a_char() ? tpi_CEES : tpi_err;
    if (w != 2130) return tpi_err;
    if (!encode_flags(item2.p1, item3.p1)) return tpi_err;
    return tpi_CES;
}

// This is executed when we see the \titlepage command,
// After that, no more titlepage ...
void Parser::T_titlepage_finish(size_t v) {
    Buffer B;
    auto   kmax = Titlepage.bigtable.size();
    for (size_t k = 0; k < kmax; k++) Titlepage.bigtable[k].exec_post();
    add_language_att();
    TitlePageAux &tpa      = Titlepage.bigtable[v];
    std::string   tmp      = tpa.T4;
    bool          finished = false;
    bool          also_bib = false;
    if (tmp.find("'only title page'") != std::string::npos) finished = true;
    if (tmp.find("'translate also bibliography'") != std::string::npos) also_bib = true;
    Xid(1).add_special_att(tmp, B);
    Xml *res = tpa.convert(2);
    res->id.add_special_att(tpa.T3, B);
    kmax = Titlepage.get_len2();
    for (size_t k = 1; k < kmax; k++) res->add_last_nl(Titlepage[k]);
    the_stack.pop_if_frame(the_names["cst_p"]);
    the_stack.add_nl();
    the_stack.add_last(res);
    the_stack.add_nl();
    ileave_v_mode();
    Titlepage.make_invalid();
    if (the_main->tpa_mode == 1) return;
    if (the_main->tpa_mode == 2)
        finished = true;
    else if (the_main->tpa_mode == 0) {
        if (nb_errs > 0) finished = true;
    }
    if (finished && also_bib) {
        the_bibliography.set_nocite();
        if (the_bibliography.number_of_data_bases() == 0) { the_bibliography.push_back_src(file_name); }
    }
    if (finished) {
        log_and_tty << "Translation terminated after title page\n";
        E_input(end_all_input_code);
    }
}

void Parser::T_titlepage(size_t v) const {
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{\\titlepage " << v << "}\n";
    }
    if (!Titlepage.is_valid()) {
        log_and_tty << "No title page info, bug?\n";
        return; // why ?
    }
    if (v >= Titlepage.bigtable.size()) {
        spdlog::critical("T_titlepage strange");
        abort();
    }
    Titlepage.bigtable[v].exec(v, tracing_commands());
}

// This is executed when the translation begins.
void TitlePage::start_thing(bool verbose) {
    if (!valid) return;
    Data = new Xml *[len2];
    for (unsigned int k = 0; k < bigtable.size(); k++) {
        if (verbose) bigtable[k].dump(k);
        bigtable[k].exec_start(k);
    }
}

// clears a TPI
void TpiOneItem::reset() {
    p1    = 0;
    p2    = 0;
    value = "";
    v     = tpi_noval;
}

// For the case CE, \URsop ?+ <UR myflags>
// We put `URsop myflags' in the buffer tp_local_buf in case of success.
auto TitlePage::find_UR(const std::string &s, const std::string &name) const -> size_t {
    Buffer &B = tp_local_buf;
    B         = s;
    size_t j  = 0;
    while ((B[j] != 0) && (std::isspace(B[j]) == 0)) j++;
    bool have_space = B[j] != 0;
    B[j]            = 0;
    auto   match    = B;
    size_t res      = 0;
    for (const auto &k : bigtable) {
        res = k.find_UR(match, j);
        if (res != 0) break;
    }
    if (res == 0) return 0;
    if (!name.empty()) {
        std::string w = B.data() + j + 1;
        B             = name;
        if (have_space) B += " " + w;
    }
    return res;
}

// This compares a Begin line with the string s.
// Returns : 0 not a begin; 1 not this type; 2 not this object
// 3 this type; 4 this object; 5 this is a type
auto Buffer::is_begin_something(const std::string &s) -> int {
    if (!starts_with("Begin")) return 0;
    if (substr(5).starts_with("Type")) {
        ptrs.b = 9;
        skip_sp_tab();
        if (ptrs.b == 9) return 2; // bad
        ptrs.a = ptrs.b;
        skip_letter();
        if (ptrs.b == ptrs.a) return 2; // bad
        resize(ptrs.b);                 // what follows the type is a comment
        if (substr(ptrs.a) == s) return 3;
        return 1;
    }
    ptrs.b = 5;
    ptrs.a = ptrs.b;
    skip_letter();
    if (ptrs.b == ptrs.a) return 2;
    resize(ptrs.b);
    if (substring() == s) return 4;
    return 2;
}

// Converts two characters into a flag.
auto TitlePageFullLine::encode_flags(char c1, char c2) -> bool {
    flags = 0;
    if (c1 == 'p')
        flags = tp_p_flag;
    else if (c1 == 'e')
        flags = tp_e_flag;
    else if (c1 == 'E')
        flags = tp_p_flag + tp_e_flag;
    else if (c1 == 'q')
        flags = tp_q_flag;
    else if (c1 != 0)
        return false;
    if (c2 == '+')
        flags += tp_plus_flag;
    else if (c2 == 'A')
        flags += tp_A_flag;
    else if (c2 == 'B')
        flags += tp_B_flag;
    else if (c2 == 'C')
        flags += tp_C_flag;
    else if (c2 != 0)
        return false;
    return true;
}

// See foo=bar. Returns bar.
// If c is true, stop at white space or comment.
// Otherwise, just remove trailing space.

auto Buffer::see_config_kw(const std::string &s, bool c) -> String {
    if (!see_equals(s)) return nullptr;
    if (c) {
        auto k = ptrs.b;
        while ((*this)[k] != 0 && at(k) != '%' && at(k) != '#') k++;
        resize(k);
    }
    remove_space_at_end();
    return data() + ptrs.b;
}

void Buffer::find_top_atts() {
    if (!see_equals("DocAttrib")) return;
    ptrs.a = ptrs.b;
    skip_letter();
    std::string a = substring();
    skip_sp_tab();
    if (head() != '\\' && head() != '\"') return;
    remove_space_at_end();
    if (at(ptrs.b) == '\"' && ptrs.b < size() - 1 && back() == '\"') pop_back();
    if (at(ptrs.b) == '\"') {
        auto        as = std::string(a);
        std::string bs = std::string(substr(ptrs.b + 1));
        Xid(1).add_attribute(as, bs);
    } else if (substr(ptrs.b) == "\\specialyear") {
        auto as = std::string(a);
        auto bs = std::string(the_main->year_string);
        Xid(1).add_attribute(as, bs);
    } else if (substr(ptrs.b) == "\\tralics") {
        auto as = std::string(a);
        clear();
        append("Tralics version ");
        append(tralics_version);
        std::string bs = std::string(*this);
        Xid(1).add_attribute(as, bs);
    } else {
        docspecial = fmt::format("\\addattributestodocument{{{}}}{{{}}}", a, data() + ptrs.b);
        the_main->add_to_from_config(init_file_pos, docspecial);
    }
}

// Returns +1 if begin, -1 if end, 0 otherwise.
auto Buffer::see_config_env() const -> int {
    if (starts_with("Begin")) return 1;
    if (starts_with("End")) return -1;
    return 0;
}

auto Buffer::find_alias(const std::vector<std::string> &SL, std::string &res) -> bool {
    ptrs.b = 0;
    if (starts_with("End")) return false;
    skip_sp_tab();
    if (is_special_end()) return false;
    if (ptrs.b == 0) return false;
    ptrs.a = ptrs.b;
    skip_letter_dig();
    if (ptrs.a == ptrs.b) return true; // this is bad
    std::string pot_res      = substring();
    bool        local_potres = false;
    if (std::find(SL.begin(), SL.end(), pot_res) != SL.end()) local_potres = true;
    for (;;) {
        skip_sp_tab();
        if (is_special_end()) break;
        ptrs.a = ptrs.b;
        skip_letter_dig();
        if (ptrs.a == ptrs.b) break;
        std::string a  = substring();
        bool        ok = a == res;
        if (ok) {
            the_log << "Potential type " << res << " aliased to " << pot_res << "\n";
            if (!local_potres) local_potres = the_main->check_for_tcf(pot_res);
            if (local_potres) {
                res = pot_res;
                return true;
            }
            the_log << "Alias " << pot_res << " undefined\n";
            return false;
        }
    }
    the_log << "Alias " << pot_res << " does not match " << res << "\n";
    return false;
}
