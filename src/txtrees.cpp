// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2006, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains a lot of stuff dealing with trees
// It contains also some eTeX extensions.

#include "txinline.h"
#include "txparser.h"
#include "txtrees.h"

namespace trees_ns {
    void normalise_space(TokenList &L);
    auto which_index(std::string) -> int;
} // namespace trees_ns

namespace date_ns {
    auto check_date(long y, long m, long d) -> bool;
    auto year_length(long y) -> long;
    auto month_length(long y, long m) -> long;
    void prev_date(long &year, long &month, long &day);
    void next_date(long &year, long &month, long &day);
} // namespace date_ns

namespace {
    Buffer   local_buf;
    int      month_length_table[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    AllIndex the_index;
    Token    day_ctr, year_ctr, month_ctr;
} // namespace

// By default, this is a glossary and a main index
AllIndex::AllIndex() {
    value.push_back(new OneIndex("glossary", "Glossary", 6));
    value.push_back(new OneIndex("default", "Index", 5));
}

// Returns the index location associated to the name S
// If S is not found, the main index is used
auto AllIndex::find_index(const std::string &s) -> size_t {
    auto n = value.size();
    for (size_t i = 0; i < n; i++)
        if (value[i]->has_name(s)) return i;
    return 1;
}

void AllIndex::new_index(const std::string &s, const std::string &title) {
    auto n = value.size();
    for (size_t i = 0; i < n; i++)
        if (value[i]->has_name(s)) return;
    auto id = the_main->the_stack->next_xid(nullptr).value;
    value.push_back(new OneIndex(s, title, to_unsigned(id)));
}

// For \addatttoindex[foo]{bar}{gee}, returns the idx of foo,
// then we can say \XMLaddatt{idx}{bar}{gee}
auto Parser::get_index_value() -> size_t {
    std::string s = sT_optarg_nopar();
    auto        i = the_index.find_index(s);
    return the_index.get_index(i)->get_AL();
}

// Case \printglossary or \printindex[foo].
// Marks the place where to insert the index
void AllIndex::mark_print(size_t g) {
    Xml *mark = new Xml(Istring(""), nullptr);
    Xml *Foo  = new Xml(Istring(""), mark);
    the_main->the_stack->add_last(Foo);
    value[g]->set_position(mark);
}

// Case of \index{key@value|encap}
// Translates to <index encap='encap' target='x1 x2'>value<index>

// This normalises spaces
void trees_ns::normalise_space(TokenList &L) {
    bool      prev_space = false;
    TokenList res;
    auto      SP = Token(space_token_val);
    auto      C  = L.begin();
    auto      E  = L.end();
    while (C != E) {
        if (C->is_space_token()) {
            if (!prev_space) res.push_back(SP);
            prev_space = true;
        } else {
            res.push_back(*C);
            prev_space = false;
        }
        ++C;
    }
    L.swap(res);
}

auto trees_ns::xless(Indexer *A, Indexer *B) -> bool { return A->key < B->key; }

auto Parser::index_aux(TokenList &L, std::optional<size_t> father, size_t g) -> size_t {
    static const Token escape_t(other_t_offset, '"');
    static const Token actual_t(other_t_offset, '@');
    static const Token actualb_t(letter_t_offset, '@');
    static const Token encap_t(other_t_offset, '|');
    std::string        key, encap;
    bool               have_key = false;
    TokenList          z;
    if (token_ns::split_at(escape_t, actual_t, actualb_t, L, z, false)) {
        have_key = true;
        token_ns::remove_first_last_space(z);
        key = to_stringE(z);
    }
    if (token_ns::split_at(escape_t, encap_t, encap_t, L, z, false)) {
        token_ns::remove_first_last_space(L);
        encap = to_stringE(L);
        swap(L, z);
    }
    if (!have_key) {
        z = L;
        token_ns::remove_first_last_space(z);
        key = to_stringE(z);
    }
    z               = L;
    std::string aux = to_stringE(z);
    // We have now: key@aux|encap
    std::vector<Indexer *> &IR    = the_index.get_data(g);
    int                     level = 1;
    if (father) level = 1 + IR[*father]->level;
    auto n = IR.size();
    for (size_t i = 0; i < n; i++)
        if (IR[i]->is_same(level, aux)) return i;
    Buffer &B = local_buf;
    B.reset();
    if (father) B << IR[*father]->key << "____";
    B << key;
    B.lowercase();
    B.no_newline();
    Xml *res = translate_list(L);
    Xml *x   = new Xml(np_index, res);
    if (!encap.empty()) x->get_id().add_attribute(np_encap, Istring(encap));
    x->get_id().add_attribute(np_level, name_positions(cst_dig0 + level));
    auto iid = the_index.next_iid();
    IR.push_back(new Indexer(B.to_string(), aux, x, level, iid));
    return n;
}

// \index, \makeindex, \printindex, + glossary, and \newindex
void Parser::T_index(subtypes c) {
    if (c == makeindex_code || c == makeglossary_code) return;
    flush_buffer();
    if (c == index_code) remove_initial_star();
    if (c == newindex_code) {
        std::string s     = sT_arg_nopar();
        std::string title = sT_arg_nopar();
        the_index.new_index(s, title);
        return;
    }
    size_t g = 0;
    if (c == printindex_code || c == index_code) {
        std::string s = sT_optarg_nopar();
        g             = the_index.find_index(s);
    }
    if (c == printindex_code || c == printglossary_code) {
        the_index.mark_print(g);
        return;
    }
    TokenList L = read_arg();
    trees_ns::normalise_space(L);
    TokenList          z1, z2;
    static const Token escape_t(other_t_offset, '"');
    static const Token level_t(other_t_offset, '!');
    flush_buffer();
    int level = 1;
    if (token_ns::split_at(escape_t, level_t, level_t, L, z1, true)) {
        if (token_ns::split_at(escape_t, level_t, level_t, L, z2, true))
            level = 3; // z1, z2, L
        else
            level = 2; // z1, L
    }
    // If level != 1, make sure parent exists
    std::optional<size_t> position;
    if (level > 1) position = index_aux(z1, position, g);
    // If level == 3, make sure grand parent exists
    if (level == 3) position = index_aux(z2, position, g);
    // make sure this exists
    auto pposition = index_aux(L, position, g);
    // Now, add a label here
    auto iid = the_index.get_data(g)[pposition]->iid;
    int  nid = the_index.next_index();
    ;
    local_buf << bf_reset << "lid" << nid;
    std::string W  = local_buf.to_string();
    Istring     id = the_stack.add_anchor(W, false);
    create_label(W, id);
    tralics_ns::add_ref(to_signed(iid), W, true);
}

void Parser::finish_index() {
    auto labels = std::vector<std::string>(the_index.get_last_iid(), "");
    tralics_ns::find_index_labels(labels);
    int  idx_size = 0;
    int  idx_nb   = 0;
    auto q        = the_index.size();
    for (size_t jj = 1; jj <= q; jj++) {
        auto j  = jj == q ? 0 : jj;
        auto CI = the_index.get_index(j);
        auto n  = CI->size();
        if (n == 0) continue;
        idx_size += n;
        idx_nb++;
        Xml *res = new Xml(j == 0 ? np_theglossary : np_theindex, nullptr); // OK?
        Xid  id  = res->get_id();
        {
            const std::string &t = CI->get_title();
            if (!t.empty()) id.add_attribute(the_names[cstb_title], Istring(t));
        }
        {
            AttList &L = the_stack.get_att_list(CI->get_AL());
            id.add_attribute(L, true);
        }
        for (size_t i = 0; i < n; i++) {
            Xml *A = CI->get_translation(i);
            A->get_id().add_attribute(np_target, Istring(labels[CI->get_iid(i)]));
        }
        CI->do_sort();
        for (size_t i = 0; i < n; i++) {
            Xml *A = CI->get_translation(i);
            res->push_back(A);
            res->add_nl();
        }
        the_stack.document_element()->insert_bib(res, CI->get_position());
    }
    if (idx_size == 0) return;
    log_and_tty << "Index has " << idx_size << " entries";
    if (idx_nb > 1) log_and_tty << " in " << idx_nb << " clusters";
    log_and_tty << "\n";
}

// -----------------------------------------------
// Commands from tree-dvips.sty
void Parser::T_trees(int c) {
    flush_buffer();
    if (c == node_code)
        T_node();
    else if (c == nodepoint_code)
        T_nodepoint();
    else if (c == nodeconnect_code)
        T_nodeconnect(np_nodeconnect);
    else if (c == anodeconnect_code)
        T_nodeconnect(np_anodeconnect);
    else if (c == barnodeconnect_code)
        T_barnodeconnect(np_barnodeconnect);
    else if (c == abarnodeconnect_code)
        T_barnodeconnect(np_abarnodeconnect);
    else if (c == nodecurve_code)
        T_nodecurve(np_nodecurve);
    else if (c == anodecurve_code)
        T_nodecurve(np_anodecurve);
    else if (c == nodebox_code)
        T_nodebox(np_nodebox);
    else if (c == nodeoval_code)
        T_nodebox(np_nodeoval);
    else if (c == nodetriangle_code)
        T_nodetriangle(np_nodetriangle);
    else if (c == nodecircle_code)
        T_nodecircle(np_nodecircle);
}

// Something like
// \def\node#1#2{\leavevmode
//  \xbox{node}{\XMLaddatt{name}{#1}#2}}

// Initial code uses nodemargin=2pt (value from \nodemargin)
// and nodemargin=2pt for nodepoint below
void Parser::T_node() {
    Istring A = nT_arg_nopar();
    leave_v_mode();
    the_stack.push1(np_node);
    AttList &cur = last_att_list();
    cur.push_back(np_name, A);
    T_arg();
    the_stack.pop(np_node);
}

// A node of 0pt width and height
// \nodepoint{nodename}[horizontal displacement][vertical displacement]

void Parser::T_nodepoint() {
    Istring A = nT_arg_nopar();
    Istring B = nT_optarg_nopar();
    Istring C = nT_optarg_nopar();
    the_stack.push1(np_node);
    AttList &cur = last_att_list();
    cur.push_back(np_name, A);
    cur.push_back(np_xpos, B); // default value is zero
    cur.push_back(np_ypos, C); // default value is zero
    the_stack.pop(np_node);
}

// \nodeconnect[fromloc]{fromnode}[toloc]{tonode}
void Parser::T_nodeconnect(name_positions W) {
    name_positions A = get_trees_opt();
    if (A == cst_invalid) A = np_letter_b;
    Istring        B = nT_arg_nopar();
    name_positions C = get_trees_opt();
    if (C == cst_invalid) C = np_letter_t;
    Istring D = nT_arg_nopar();
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_posB, C);
    cur.push_back(np_posA, A);
    cur.push_back(np_nameB, D);
    cur.push_back(np_nameA, B);
    the_stack.pop(W);
}

// \barnodeconnect[depth]{fromnode}{tonode}
void Parser::T_barnodeconnect(name_positions W) {
    Istring A = nT_optarg_nopar();
    Istring B = nT_arg_nopar();
    Istring C = nT_arg_nopar();
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_depth, A);
    cur.push_back(np_nameB, C);
    cur.push_back(np_nameA, B);
    the_stack.pop(W);
}

// \nodecurve[fromloc]{fromnode}[toloc]{tonode}{depthfrom}[depthto]
// \anodecurve[fromloc]{fromnode}[toloc]{tonode}{depth}
void Parser::T_nodecurve(name_positions W) {
    name_positions A = get_trees_opt();
    if (A == cst_invalid) A = np_letter_b;
    Istring        B = nT_arg_nopar();
    name_positions C = get_trees_opt();
    if (C == cst_invalid) C = np_letter_t;
    Istring D = nT_arg_nopar();
    Istring E = nT_arg_nopar();
    Istring F = nT_optarg_nopar();
    if (F.null() || F.empty()) F = E;
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_depthA, E);
    cur.push_back(np_depthB, F);
    cur.push_back(np_posB, C);
    cur.push_back(np_posA, A);
    cur.push_back(np_nameB, D);
    cur.push_back(np_nameA, B);
    the_stack.pop(W);
}

void Parser::T_nodebox(name_positions W) {
    Istring A = nT_arg_nopar();
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_nameA, A);
    the_stack.pop(W);
}

void Parser::T_nodetriangle(name_positions W) {
    Istring A = nT_arg_nopar();
    Istring B = nT_arg_nopar();
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_nameA, A);
    cur.push_back(np_nameB, B);
    the_stack.pop(W);
}

// \nodecircle[depth]{nodename}
void Parser::T_nodecircle(name_positions W) {
    Istring B = nT_optarg_nopar();
    Istring A = nT_arg_nopar();
    the_stack.push1(W);
    AttList &cur = last_att_list();
    cur.push_back(np_depth, B);
    cur.push_back(np_nameA, A);
    the_stack.pop(W);
}

//  gloss -----------------------------------------------------------------

void Parser::T_gloss(bool c) {
    Token A   = hash_table.locate("eachwordone");
    Token B   = hash_table.locate("eachwordtwo");
    Token C   = hash_table.locate("eachwordthree");
    Token amp = Token(ampersand_t_offset, '&');
    ;
    Token     BS          = hash_table.locate("\\");
    TokenList first_line  = read_until(BS);
    TokenList second_line = read_until(BS);
    TokenList third_line;
    int       n1 = 0, n2 = 0, n3 = 0;
    if (!c) third_line = read_until(BS);
    n1 = token_ns::replace_space(first_line, amp, A);
    first_line.push_front(A);
    first_line.push_back(BS);
    n2 = token_ns::replace_space(second_line, amp, B);
    second_line.push_front(B);
    second_line.push_back(BS);
    if (!c) {
        n3 = token_ns::replace_space(third_line, amp, C);
        third_line.push_front(C);
        third_line.push_back(BS);
    }

    if (n3 > n1) n1 = n3;
    if (n2 > n1) n1 = n2;
    n1++;
    TokenList res;
    res.push_back(hash_table.OB_token);
    while (n1 > 0) {
        --n1;
        res.push_back(Token(other_t_offset, 'l'));
    }
    res.push_back(hash_table.CB_token);
    res.splice(res.end(), first_line);
    res.splice(res.end(), second_line);
    res.splice(res.end(), third_line);
    token_ns::add_env(res, "tabular");
    if (tracing_commands()) the_log << lg_startbrace << "Gloss: " << res << lg_endbrace;
    back_input(res);
}

//      eTEX ----------------------------------------------------------

void Parser::T_etex() { parse_error(cur_tok, "Unimplemented e-TeX extension ", cur_tok, "", "unimp"); }

// --------------------------------------------------------------------
// GB4e

auto mk_ensure(const std::string &a, const std::string &b) -> std::string { return a + "{\\ensuremath{" + b + "}}"; }

// We should perhaps do something with this list ??
void gb4eboot() {
    LinePtr     L;
    std::string matha1       = "{\\mathrm{#1}}";
    std::string open_bra     = "{[}";
    std::string close_bra    = "{]}";
    std::string simple_index = "\\sb" + matha1 + "\\;";
    std::string double_index = "\\sb{" + matha1 + "\\sb{#2}}";
    L.insert("%% Begin bootstrap commands for gb4e", true);
    L.insert(R"(\def\lb#1{\@ifnextchar [{\@glarph{#1}}{\@bl{#1}}})", true);
    L.insert(mk_ensure("\\def\\@glarph#1[#2]", open_bra + double_index), true);
    L.insert(mk_ensure("\\def\\@bl#1", open_bra + simple_index), true);
    L.insert(R"(\def\rb#1{\@ifnextchar [{\@grarph{#1}}{\@br{#1}}})", true);
    L.insert(mk_ensure("\\def\\@grarph#1[#2]", close_bra + double_index), true);
    L.insert(mk_ensure("\\def\\@br#1", close_bra + simple_index), true);
}

// --------------------------------------------------------------------
// DATES
using tralics_ns::is_leap_year;

// Dispatcher function
void Parser::date_commands(int c) {
    if (c == 0)
        is_date_valid();
    else if (c == 1)
        count_days();
    else if (c == 2)
        next_date();
    else if (c == 3)
        prev_date();
    else if (c == 4)
        datebynumber();
}

// Stores the value c in the counter T if possible
void Parser::set_counter(Token T, long c) {
    see_cs_token(T);
    if (cur_cmd_chr.cmd != assign_int_cmd) return;
    word_define(cur_cmd_chr.chr, c, false);
}

// Stores the value c in the counter T if possible
void Parser::get_counter(Token T, long &c) {
    c = 0;
    see_cs_token(T);
    if (cur_cmd_chr.cmd != assign_int_cmd) return;
    c = eqtb_int_table[cur_cmd_chr.chr].val;
}

// Reads three counter names; return true if OK
auto Parser::scan_date_ctrs() -> bool {
    year_ctr  = hash_table.relax_token;
    month_ctr = hash_table.relax_token;
    day_ctr   = hash_table.relax_token;
    bool bad  = M_counter(false);
    bool res  = true;
    if (!bad) {
        get_token();
        year_ctr = cur_tok;
    } else
        res = false;
    bad = M_counter(false);
    if (!bad) {
        get_token();
        month_ctr = cur_tok;
    } else
        res = false;
    bad = M_counter(false);
    if (!bad) {
        get_token();
        day_ctr = cur_tok;
    } else
        res = false;
    return res;
}

// This sets the three counters
void Parser::set_date_ctrs(long year, long month, long day) {
    set_counter(year_ctr, year);
    set_counter(month_ctr, month);
    set_counter(day_ctr, day);
}

// This sets the three counters
void Parser::get_date_ctrs(long &year, long &month, long &day) {
    get_counter(year_ctr, year);
    get_counter(month_ctr, month);
    get_counter(day_ctr, day);
}

// True if year Y is a leap year

auto tralics_ns::is_leap_year(long y) -> bool {
    if ((y % 4) != 0) return false;
    if (y <= 1500) return true;
    if (y % 100 == 0) return y % 400 == 0;
    return true;
}

// Returns number of days in the year
auto date_ns::year_length(long y) -> long {
    if (y == 1582) return 355;
    if (is_leap_year(y)) return 366;
    return 365;
}

// Returns number of days in the month
auto date_ns::month_length(long y, long m) -> long {
    if (m != 2) return month_length_table[m];
    if (is_leap_year(y)) return 29;
    return 28;
}

// Return true if valid, signals error otherwise
auto date_ns::check_date(long y, long m, long d) -> bool {
    String Bad = nullptr;
    long   ml  = 0;
    if (y <= 0)
        Bad = "year<1";
    else if (m <= 0)
        Bad = "month<1";
    else if (m > 12)
        Bad = "month>12";
    else if (d < 1)
        Bad = "day<1";
    else {
        ml = date_ns::month_length(y, m);
        if (d > ml)
            Bad = ".";
        else if (y == 1582 && m == 10 && d > 4 && d < 15)
            Bad = "inexistant day";
    }
    if (Bad == nullptr) return true;
    local_buf << bf_reset << "Date error: ";
    if (Bad[0] == '.')
        local_buf << "day>" << ml;
    else
        local_buf << Bad;
    local_buf << " " << y << '/' << m << '/' << d;
    the_parser.parse_error(the_parser.err_tok, local_buf.to_string());
    return false;
}

// Returns the number of days between start/01/01 and cur/month/day
void Parser::count_days() {
    Token T   = cur_tok;
    bool  bad = M_counter(false);
    if (!bad) get_token();
    Token ctr   = cur_tok;
    auto  start = scan_braced_int(T);
    auto  cur   = scan_braced_int(T);
    auto  month = scan_braced_int(T);
    auto  day   = scan_braced_int(T);
    if (bad) return;
    int        c               = 0;
    static int month_table[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    for (auto y = start; y < cur; y++) c += date_ns::year_length(y);
    c += month_table[month];
    if (is_leap_year(cur) && month > 2) c++;
    if (cur == 1582 && (month > 10 || (month == 10 && day > 14))) c -= 10;
    c += day;
    set_counter(ctr, c);
}

void Parser::datebynumber() {
    Token T     = cur_tok;
    auto  start = scan_braced_int(T); // start date
    auto  val   = scan_braced_int(T); // value to convert
    scan_date_ctrs();                 // fetch the counters
    int  month = 1, day = 1;
    auto year = start;
    int  c    = 1;
    for (;;) {
        auto n = date_ns::year_length(year);
        if (c + n <= val) {
            c += n;
            ++year;
        } else
            break;
    }
    month = 1;
    day   = 1;
    for (int i = 1; i <= 12; i++) {
        auto n = date_ns::month_length(year, month);
        if (c + n <= val) {
            c += n;
            ++month;
        } else
            break;
    }
    while (c < val) {
        ++c;
        ++day;
    }
    set_date_ctrs(year, month, day);
}

// gives date of yesterday
void date_ns::prev_date(long &year, long &month, long &day) {
    --day;
    if (year == 1582 && month == 10 && day == 14) day = 4;
    if (day > 0) return;
    --month;
    if (month == 0) {
        month = 12;
        --year;
    }
    day = date_ns::month_length(year, month);
}

// gives date of tomorrow
void date_ns::next_date(long &year, long &month, long &day) {
    ++day;
    if (year == 1582 && month == 10 && day == 5) day = 15;
    auto ml = date_ns::month_length(year, month);
    if (day <= ml) return;
    day = 1;
    ++month;
    if (month <= 12) return;
    month = 1;
    ++year;
}

void Parser::next_date() {
    scan_date_ctrs(); // fetch the counters
    long year, month, day;
    get_date_ctrs(year, month, day);
    date_ns::check_date(year, month, day);
    date_ns::next_date(year, month, day);
    set_date_ctrs(year, month, day);
}

void Parser::prev_date() {
    scan_date_ctrs(); // fetch the counters
    long year, month, day;
    get_date_ctrs(year, month, day);
    date_ns::check_date(year, month, day);
    date_ns::prev_date(year, month, day);
    set_date_ctrs(year, month, day);
}

void Parser::is_date_valid() {
    Token T = cur_tok;
    auto  y = scan_braced_int(T);
    auto  m = scan_braced_int(T);
    auto  d = scan_braced_int(T);
    date_ns::check_date(y, m, d);
}

// C is even for a month, odd for a day
void Parser::month_day(subtypes c) {
    Token T = cur_tok;
    auto  n = scan_braced_int(cur_tok);
    if (n <= 0 || n > ((int(c) & 1) != 0 ? 7 : 12)) {
        if (tracing_macros()) the_log << T << "<-" << lg_end;
        return;
    }
    static String fm[12] = {"janvier", "f\\'evrier", "mars",      "avril",   "mai",      "juin",
                            "juillet", "ao\\^ut",    "septembre", "octobre", "novembre", "d\\'ecembre"};
    static String sm[12] = {"enero", "febrero", "marzo",      "abril",   "mayo",      "junio",
                            "julio", "agosto",  "septiembre", "octubre", "noviembre", "diciembre"};
    static String gm[12] = {"Januar", "Februar", "M\\\"arz",  "April",   "Mai",      "Juni",
                            "Juli",   "August",  "September", "Oktober", "November", "Dezember"};
    static String em[12] = {"January", "February", "March",     "April",   "May",      "June",
                            "July",    "August",   "September", "October", "November", "December"};

    static String fd[7] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
    static String sd[7] = {"Lunes", "Martes", "Mi\\'ercoles", "Jueves", "Viernes", "S\\'abado", "Domingo"};
    static String gd[7] = {"Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag", "Sonntag"};
    static String ed[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    local_buf.reset();
    switch (c) {
    case md_frenchm_code: local_buf << fm[n - 1]; break;
    case md_frenchd_code: local_buf << fd[n - 1]; break;
    case md_spanishm_code: local_buf << sm[n - 1]; break;
    case md_spanishd_code: local_buf << sd[n - 1]; break;
    case md_germanm_code: local_buf << gm[n - 1]; break;
    case md_germand_code: local_buf << gd[n - 1]; break;
    case md_englishm_code: local_buf << em[n - 1]; break;
    case md_englishd_code: local_buf << ed[n - 1]; break;
    default: break;
    }
    TokenList L;
    tokenize_buffer(local_buf, L, "(month and day)");
    if (tracing_macros()) the_log << T << "<-" << L << lg_end;
    back_input(L);
}
