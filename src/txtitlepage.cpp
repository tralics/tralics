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
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txinline.h"

namespace {
    Buffer            docspecial; // Buffer for document special things
    Buffer            tp_main_buf;
    Buffer            local_buf; // some local buffer
    TitlePage         Titlepage; // title page info
    TitlePageFullLine tpfl;
    TpiOneItem        Toi;
} // namespace

namespace tpage_ns {
    void init_error();
    auto scan_item(Buffer &in, Buffer &out, char del) -> bool;
    auto next_item(Buffer &in, Buffer &out) -> tpi_vals;
    auto see_an_assignment(Buffer &in, Buffer &key, Buffer &val) -> int;

} // namespace tpage_ns

// needed elsewhere, because we have to call add_language_add.
auto tralics_ns::titlepage_is_valid() -> bool { return Titlepage.is_valid(); }

// needed in txmain.
void tralics_ns::Titlepage_start(bool verbose) { Titlepage.start_thing(verbose); }

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

// This is the function that creates the title page data
// from a list of lines
void tralics_ns::Titlepage_create(LineList &lines) {
    if (lines.empty()) return;
    Titlepage.make_valid();
    for (;;) {
        tp_main_buf.clear();
        int line = lines.get_next(tp_main_buf);
        if (line < 0) return;
        init_file_pos = line;
        tpa_line k    = tp_main_buf.tp_fetch_something();
        if (k == tl_empty) continue;
        if (k == tl_end) break;
        int      w0 = tpfl.read();
        tpi_vals w  = tpfl.classify(w0, Titlepage.state);
        if (w == tpi_err) {
            tpage_ns::init_error();
            continue;
        }
        TitlePageAux tpa(tpfl);
        bool         res = tpa.classify(w, Titlepage.state);
        if (!res) {
            tpage_ns::init_error();
            continue;
        }
        Titlepage.bigtable.push_back(tpa);
    }
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
// result is in local_buf, return value is true if OK.
auto tpage_ns::scan_item(Buffer &in, Buffer &out, char del) -> bool {
    out.clear();
    if (del == '\\') { // scan a command
        while (is_letter(in.head())) out.push_back(in.next_char());
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
        Toi.set_p1(c);
        if (!in.tp_next_char(c)) return tpi_noval;
        if (!is_tp_delim(c)) {
            Toi.set_p2(c);
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
    tpi_vals w   = tpage_ns::next_item(tp_main_buf, local_buf);
    if (w == tpi_err) return -1;
    item1 = Toi;
    item1.set_v(w);
    res += 1000 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, local_buf);
    if (w == tpi_err) return -1;
    item2 = Toi;
    item2.set_v(w);
    res += 100 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, local_buf);
    if (w == tpi_err) return -1;
    item3 = Toi;
    item3.set_v(w);
    res += 10 * (w - 1);
    if (w == tpi_noval) return res;
    w = tpage_ns::next_item(tp_main_buf, local_buf);
    if (w == tpi_err) return -1;
    item4 = Toi;
    item4.set_v(w);
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
    if (!encode_flags(item2.get_p1(), item3.get_p1())) return tpi_err;
    return tpi_CES;
}

// More classification. This allocates memory, and modifies state
auto TitlePageAux::classify(tpi_vals w, int &state) -> bool {
    switch (w) {
    case tpi_CESS: // \makeRR <rrstart> "ok" "ok"
        state = 1;
        idx   = 0;
        type  = tpi_rt_tp;
        return true;
    case tpi_ACSS: // alias \makeRT "ok" "ok"
        T1   = T2;
        T2   = Titlepage.bigtable[0].get_T2();
        type = tpi_rt_tp;
        return true; // state is still 1
    case tpi_CES:    // \title <title> "notitle"
        state = 2;
        type  = tpi_rt_normal;
        T4    = T3;
        T3    = T2;
        idx   = Titlepage.increase_data();
        if (get_flags2() == tp_A_flag) {
            Buffer &B = local_buf;
            B         = "\\" + T1;
            B.format("{{{}}}", T4);
            B.append("%\n");
            the_main->add_to_from_config(1, B);
        }
        return true;
    case tpi_CEES: // \author +<aulist> <au> "default"
        state = 2;
        type  = tpi_rt_list;
        idx   = Titlepage.increase_data();
        return true;
    case tpi_E:    //  <UR> -
        state = 3; // next line cannot be alias
        type  = tpi_rt_urlist;
        idx   = Titlepage.increase_data();
        T2    = T1;
        return true;
    case tpi_CE: // case \URsop ?+ <UR>
    {
        auto k = Titlepage.find_UR(T2, T1);
        if (k == 0) return false;
        state = 2;
        type  = tpi_rt_ur;
        idx   = k;
        T2    = local_buf;
        return true;
    }
    case tpi_S: // case "<foo/>"
        type = tpi_rt_constant;
        idx  = Titlepage.increase_data();
        return true;
    case tpi_EC: // case execute \foo
        type = tpi_rt_exec;
        idx  = Titlepage.increase_data();
        return true;
    case tpi_CEE: // \Paris ?<UR> <Rocqu>
    {
        auto k = Titlepage.find_UR(T2, "");
        if (k == 0) return false;
        state = 2;
        type  = tpi_rt_ur;
        idx   = k;
        T2    = T3;
        T3    = "";
        return true;
    }
    case tpi_CCS: // case \myself \author "JG"
    {
        auto k = Titlepage.find_cmd(T2);
        if (k == 0) return false;
        TitlePageAux &R = Titlepage.bigtable[k];
        state           = 2;
        idx             = R.idx;
        xflags          = k; // hack...
        T2              = T3;
        T3              = R.T3;
        type            = R.type == tpi_rt_normal ? tpi_rt_normal_def : tpi_rt_list_def;
    }
        return true;
    case tpi_AC:
        type = tpi_rt_alias;
        T1   = T2;
        T2   = Titlepage.bigtable.back().get_T1();
        return true;
    default: return false;
    }
}

// Evaluates at start of translation
void TitlePageAux::exec_start(size_t k) {
    if (type == tpi_rt_urlist) { // no command here
        Titlepage[idx] = convert(2, new Xml(the_names["\n"]));
        return;
    }
    if (type == tpi_rt_alias) { // \let\this\that
        the_parser.hash_table.eval_let(T1, T2);
        return;
    }
    // in all other cases define this as (titlepage,k)
    the_parser.hash_table.primitive(T1, titlepage_cmd, subtypes(k));
    if (type == tpi_rt_list) { // initialise the list
        Titlepage[idx] = convert(2, convert(3, std::string(T4)));
        return;
    }
    // remaining types are:  tp, ur, normal_def, list_def, and normal
    if (type != tpi_rt_normal) return;
    auto fl = get_flags2(); // this is 0,1, 2 or 3
    if (fl == 0) {
        Titlepage[idx] = convert(2, std::string(T4));
        return;
    }
    Buffer &B = local_buf;
    B         = "\\" + T1;
    B.format("{{{}}}", T4);
    Titlepage[idx] = new Xml(std::string("empty"));
    B.append("%\n");
    if (fl == tp_A_flag) {
    } else if (fl == tp_B_flag)
        the_parser.add_buffer_to_document_hook(B, "(tpa init)");
    else
        T4 = B;
}

// This is executed when we see the \Titlepage cmd
void TitlePageAux::exec_post() {
    if (type == tpi_rt_constant) Titlepage[idx] = new Xml(std::string(T1));
    if (type == tpi_rt_exec) Titlepage[idx] = the_parser.tpa_exec(T2);
    if (type != tpi_rt_normal) return;
    if (get_flags2() == tp_C_flag) the_parser.titlepage_evaluate(T4, T1);
    if (!has_plus_flags()) return;
    if (has_u_flags()) return;
    the_parser.parse_error(the_parser.err_tok, "No value given for command \\", T1, "");
}

// This is executed when the user asks for a titlepage command.
void TitlePageAux::exec(size_t v, bool vb) {
    if (vb) {
        Logger::finish_seq();
        the_log << "{\\titlepage " << v << "=\\" << T1 << "}\n";
    }
    if (type == tpi_rt_tp) {
        the_parser.T_titlepage_finish(v);
        return;
    }
    if (type == tpi_rt_ur) { // easy case
        Titlepage[idx]->add_last_nl(convert(2));
        return;
    }
    Xml *R{nullptr};
    if (type == tpi_rt_normal_def || type == tpi_rt_list_def)
        R = convert(3, std::string(T2));
    else { // we have to read the argument.
        R = the_parser.special_tpa_arg(T1, T3, has_p_flags(), has_e_flags(), has_q_flags());
    }
    bool replace = (type == tpi_rt_normal || type == tpi_rt_normal_def);
    if (increment_flag()) {
        if (!replace) Titlepage[idx]->remove_last();
    }
    if (replace)
        Titlepage[idx] = R;
    else
        Titlepage[idx]->add_last_nl(R);
}

// Finds the real flag to increment.
// If 0, increments it and returns true, otherwise return false.
// In the case of =tpi_rt_*_def, the flag is a pointer !
auto TitlePageAux::increment_flag() -> bool {
    if (type == tpi_rt_normal_def || type == tpi_rt_list_def) return Titlepage.bigtable[xflags].increment_flag();
    if (!has_u_flags()) {
        xflags++;
        return true;
    }
    return false;
}

// This is executed when we see the \titlepage command,
// After that, no more titlepage ...
void Parser::T_titlepage_finish(size_t v) {
    Buffer B;
    auto   kmax = Titlepage.bigtable.size();
    for (size_t k = 0; k < kmax; k++) Titlepage.bigtable[k].exec_post();
    add_language_att();
    TitlePageAux &tpa      = Titlepage.bigtable[v];
    std::string   tmp      = tpa.get_T4();
    bool          finished = false;
    bool          also_bib = false;
    if (tmp.find("'only title page'") != std::string::npos) finished = true;
    if (tmp.find("'translate also bibliography'") != std::string::npos) also_bib = true;
    Xid(1).add_special_att(tmp, B);
    Xml *res = tpa.convert(2);
    res->id.add_special_att(tpa.get_T3(), B);
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
        if (main_ns::nb_errs > 0) finished = true;
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

// For the case CEE, \Paris ?<UR flags> <Rocq>
// s is the string without attribs, and n is the length
auto TitlePageAux::find_UR(const std::string &s, size_t n) const -> size_t {
    if (type != tpi_rt_urlist) return 0;
    if (T2.substr(0, n) == s.substr(0, n)) return idx;
    return 0;
}

// For the case CE, \URsop ?+ <UR myflags>
// We put `URsop myflags' in the buffer local_buf in case of success.
auto TitlePage::find_UR(const std::string &s, const std::string &name) const -> size_t {
    Buffer &B = local_buf;
    B         = s;
    size_t j  = 0;
    while ((B[j] != 0) && !is_space(B[j])) j++;
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

// true if this OK for CCS and the nams is right.
auto TitlePageAux::find_cmd(const std::string &s) const -> bool {
    if (type != tpi_rt_normal && type != tpi_rt_list) return false;
    if (T1 != s) return false;
    return true;
}

// Returns the index for a CCS.
auto TitlePage::find_cmd(const std::string &s) const -> size_t {
    for (unsigned int k = 0; k < bigtable.size(); k++) {
        if (bigtable[k].find_cmd(s)) return k;
    }
    return 0;
}

// Ctor from a TitlePageFullLine. It's just a copy.
TitlePageAux::TitlePageAux(TitlePageFullLine &X) {
    T1     = X.item1.get_value();
    T2     = X.item2.get_value();
    T3     = X.item3.get_value();
    T4     = X.item4.get_value();
    xflags = X.get_flags();
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

// This prints the flags, in a symbolic way.
void TitlePageAux::decode_flags() const {
    auto f2 = xflags / 16;
    auto f1 = xflags % 16;
    f1      = f1 / 2;
    if (f1 == 0 && f2 == 0) return;
    the_log << " (flags";
    if (f1 == 1) the_log << " +par";
    if (f1 == 2) the_log << " +env";
    if (f1 == 3) the_log << " +par +env";
    if (f1 == 4) the_log << " -par";
    if (f2 == 1) the_log << " +list";
    if (f2 == 2) the_log << " +A";
    if (f2 == 4) the_log << " +B";
    if (f2 == 6) the_log << " +C";
    the_log << ")";
}

// This prints a slot.
void TitlePageAux::dump(size_t k) {
    tpi_vals t = get_type();
    if (t == tpi_rt_alias) {
        Logger::finish_seq();
        the_log << "Defining \\" << T1 << " as alias to \\" << T2 << "\n";
        return;
    }
    if (t == tpi_rt_constant) {
        Logger::finish_seq();
        the_log << "Inserting the string " << get_T1() << "\n";
        return;
    }
    if (t == tpi_rt_exec) {
        Logger::finish_seq();
        the_log << "Inserting the command \\" << get_T2() << "\n";
        return;
    }
    Logger::finish_seq();
    the_log << "Defining \\" << get_T1() << " as \\TitlePageCmd " << k << "\n";
    if (t == tpi_rt_normal)
        the_log << "   usual <" << T3 << "/>";
    else if (t == tpi_rt_normal_def)
        the_log << "   usual? <" << T2 << "/>"
                << " " << T3;
    else if (t == tpi_rt_list)
        the_log << "   list <" << T2 << "/> and <" << T3 << "/>";
    else if (t == tpi_rt_list_def)
        the_log << "   list? <" << T2 << "/> and <" << T3 << "/>";
    else if (t == tpi_rt_urlist)
        the_log << "   ur_list <" << T2 << "/>";
    else if (t == tpi_rt_ur)
        the_log << "   ur <" << T2 << "/>";
    else if (t == tpi_rt_tp)
        the_log << "   main <" << T2 << " " << T3 << " -- " << T4 << "/>"; //   id??
    else
        the_log << "   random";
    if (t != tpi_rt_normal_def && t != tpi_rt_list_def) decode_flags(); // otherwise flags are meaningless
    the_log << "\n";
}

// Converts one of the strings into an empty XML element
auto TitlePageAux::convert(int i) -> Xml * {
    std::string s = i == 1 ? T1 : i == 2 ? T2 : i == 3 ? T3 : T4;
    return new Xml(local_buf.xml_and_attrib(s));
}

// Converts one of the strings into a XML element containing R.
auto TitlePageAux::convert(int i, Xml *r) -> Xml * {
    std::string s   = i == 1 ? T1 : i == 2 ? T2 : i == 3 ? T3 : T4;
    Xml *       res = new Xml(local_buf.xml_and_attrib(s));
    res->push_back_unless_nullptr(r);
    return res;
}

// See foo=bar. Returns bar.
// If c is true, stop at white space or comment.
// Otherwise, just remove trailing space.

auto Buffer::see_config_kw(String s, bool c) -> String {
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

// This is called for all lines, outside groups.
void tpage_ns::see_main_a(Buffer &in, Buffer &val) {
    Buffer B;
    val.clear();
    int k = tpage_ns::see_an_assignment(in, B, val);
    if (k == 1) the_names.assign(B, val);
}

// Returns 0, unless we see A="B", fills the buffers A and B.
// return 2 if there is a space in A, 1 otherwise.
auto tpage_ns::see_an_assignment(Buffer &in, Buffer &key, Buffer &val) -> int {
    if (in.tp_fetch_something() != tl_normal) return 0;
    for (;;) {
        if (in.is_special_end()) return 0;
        if (in.head() == '=') break;
        key.push_back(in.head());
        in.advance();
    }
    in.advance();
    in.skip_sp_tab();
    if (in.head() != '\"') return 0;
    in.advance();
    tpage_ns::scan_item(in, val, '\"');
    key.ptrs.b = 0;
    key.remove_space_at_end();
    for (;;) {
        if (key.head() == 0) return 1;
        if (is_space(key.head())) return 2;
        key.advance();
    }
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
