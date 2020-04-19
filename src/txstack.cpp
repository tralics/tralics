// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file implements the XML stack for Tralics

#include "tralics/globals.h"
#include "txinline.h"
#include "txparser.h"
#include <spdlog/spdlog.h>
#include <utility>

namespace stack_ns {
    auto mode_to_string(mode x) -> String;
} // namespace stack_ns

namespace {
    Buffer shbuf;
} // namespace

// Increases xid, makes sure that the attribute table is big enough
auto Stack::next_xid(Xml *elt) -> Xid {
    attributes.emplace_back();
    enames.push_back(elt);
    last_xid++;
    return last_xid;
}

Stack::Stack() {
    attributes.reserve(2048);
    enames.reserve(2048);
    last_xid = -1;
    xid_boot = 0;
    next_xid(nullptr); // 0
    next_xid(nullptr); // 1 <document>
    next_xid(nullptr); // 2 dummy, see Stack::temporary
    next_xid(nullptr); // 3 is for the biblio, see after_main_text
    next_xid(nullptr); // 4 is <tableofcontents/>
    next_xid(nullptr); // 5 is for theindex (see txtrees)
    next_xid(nullptr); // 6 is for the glossary (see txtrees)
}

// Debug: this prints all elements, except those allocated at boot
void Stack::dump_xml_table() {
    auto k = enames.size();
    for (size_t i = to_unsigned(xid_boot + 1); i < k; i++)
        if (enames[i] != nullptr) {
            std::cout << i << "::"; //<< enames[i] -> get_name() << "\n";
            std::cout << enames[i] << "\n";
        }
}

// Returns the element in the table with id n
// This should be at position N
auto Stack::fetch_by_id(size_t n) -> Xml * {
    if (enames.size() <= n) return nullptr;
    Xml *x = enames[n];
    if (x == nullptr) return nullptr;
    if (x->id.value == to_signed(n)) return x;
    spdlog::error("This cannot happen: bug in table at position {}", n);
    return nullptr;
}

// returns a parent of x
auto Stack::find_parent(Xml *x) -> Xml * {
    if (x == nullptr) return nullptr;
    auto k = enames.size();
    // debug: print all parents
    // std::cout<< "Search parent of " << x << "\n";
    // for(int i= 0; i< k; i++) {
    //   if(enames[i] && enames[i]-> is_child (x))
    //    std::cout<< "fount at " << enames[i] << "\n";
    // }
    for (size_t i = to_unsigned(xid_boot + 1); i < k; i++) {
        if (enames[i] == nullptr) continue;
        if (enames[i]->is_child(x)) return enames[i];
    }
    return nullptr;
}

auto Parser::last_att_list() -> AttList & { return the_stack.get_top_id().get_att(); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(const Istring& A, const Istring& B) { get_att_list(to_unsigned(last_xid)).push_back(A, B); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(name_positions A, const Istring& B) { get_att_list(to_unsigned(last_xid)).push_back(A, B); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(name_positions A, name_positions B) { get_att_list(to_unsigned(last_xid)).push_back(A, B); }

// Add A=B as attribute list to top stack
void Stack::add_att_to_cur(const Istring& A, const Istring& B) { cur_xid().add_attribute(A, B); }

// Add A=B as attribute list to last_xid
// (if force is true, ignores old value otherwise new value).
void Stack::add_att_to_last(const Istring& A, const Istring& B, bool force) {
    get_att_list(to_unsigned(last_xid)).push_back(A, B, force);
}

// Add A=B as attribute list to top stack
// (if force is true, ignores old value otherwise new value).
void Stack::add_att_to_cur(const Istring& A, const Istring& B, bool force) { cur_xid().get_att().push_back(A, B, force); }

// Returns a new element named N, initialised with z (if not empty...)
Xml::Xml(Istring N, Xml *z) : name(std::move(N)) {
    id = the_main->the_stack->next_xid(this);
    if (z != nullptr) add_tmp(gsl::not_null{z});
}

Xml::Xml(name_positions N, Xml *z) : name(the_names[N]) {
    id = the_main->the_stack->next_xid(this);
    if (z != nullptr) add_tmp(gsl::not_null{z});
}

void Stack::hack_for_hanl() {
    auto ptr = Table.size() - 1;
    if (ptr > 0) { Table[ptr].obj = Table[ptr - 1].obj; }
    // else ?
}

// Creates an empty element named x, and adds it to the stack.
// returns a reference to the attribute list of the object.
auto Stack::add_newid0(name_positions x) -> AttList & {
    top_stack()->push_back_unless_nullptr(new Xml(x, nullptr));
    return Xid(last_xid).get_att();
}

// Adds x to the tail of the current list.
void Stack::add_last(Xml *x) { top_stack()->push_back_unless_nullptr(x); }

// This adds x at the end the element
void Xml::push_back_unless_nullptr(Xml *x) {
    if (x != nullptr) push_back(gsl::not_null{x});
}

// Adds B to the tail of the current list.
void Stack::add_last_string(const Buffer &B) { top_stack()->add_last_string(B); }

// True if last element on the tree is a string.
auto Xml::last_is_string() const -> bool { return !empty() && back()->id.value == 0; }

// Assume that last element is a string. This string is put in the internal
// buffer
void Xml::last_to_SH() const {
    shbuf.reset();
    shbuf.push_back(back()->name.name);
}

// This adds B at the end the element, via concatenation, if possible.
void Xml::add_last_string(const Buffer &B) {
    if (B.empty()) return;
    shbuf.reset();
    if (last_is_string()) {
        last_to_SH();
        pop_back();
        the_parser.my_stats.one_more_merge();
    }
    shbuf.push_back(B.c_str());
    push_back_unless_nullptr(new Xml(shbuf));
}

// This adds x and a \n at the end of this.
void Xml::add_last_nl(Xml *x) {
    if (x != nullptr) {
        push_back_unless_nullptr(x);
        push_back_unless_nullptr(the_main->the_stack->newline_xml);
    }
}

// Implements unhbox, etc.
void Stack::unbox(Xml *x) { top_stack()->unbox(x); }

// Removes a trailing space on the current tree.
void Stack::remove_last_space() { top_stack()->remove_last_space(); }

// Removes a trailing space on the tree.
void Xml::remove_last_space() {
    if (!last_is_string()) return;
    last_to_SH();
    auto k = shbuf.size();
    shbuf.remove_space_at_end();
    if (k != shbuf.size()) {
        pop_back();
        if (!shbuf.empty()) push_back_unless_nullptr(new Xml(shbuf.to_string()));
    }
}

// This adds a newline to the XML.
void Stack::add_nl() { top_stack()->add_nl(); }

// This adds a NL to the end of the element
void Xml::add_nl() {
    if (!all_empty() && back_or_nullptr() == the_main->the_stack->newline_xml) return;
    push_back_unless_nullptr(the_main->the_stack->newline_xml);
}

// Returns the slot of the first non-empty frame
// It is assumed that the first frame is "document", hence never empty.
auto Stack::first_non_empty() const -> const Stack::StackSlot & {
    auto k = Table.size() - 1;
    while (Table[k].frame.spec_empty()) k--;
    return Table[k];
}

// Returns the name of the first non-empty frame.
auto Stack::first_frame() const -> Istring {
    const StackSlot &X = first_non_empty();
    return X.frame;
}

// Returns the first p on the stack.
// Too bad that the frame AND the element have to be called p.
auto Stack::get_cur_par() const -> Xml * {
    const StackSlot &X     = first_non_empty();
    Istring          pname = the_names[cst_p];
    if (X.frame == pname && !X.obj->is_xmlc() && X.obj->has_name(pname)) return X.obj;
    return nullptr;
}

// In case the current element happens to be a P,
// and has no rend attribute, then add one to it (can be center, can also
// be flush left, etc.)
void Stack::add_center_to_p() const {
    Xml *x = get_cur_par();
    if (x == nullptr) return;
    auto w = the_parser.cur_centering();
    x->id.get_att().push_back(np_rend, name_positions(np_center_etc + w), false);
}

auto Stack::is_frame(name_positions s) const -> bool { return first_frame() == the_names[s]; }

// Returns true if frame is s, or argument, and next frame is s.
// ignores a font change
auto Stack::is_frame2(name_positions S) const -> bool {
    Istring s = the_names[S];
    auto    k = Table.size() - 1;
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == s) return true;
    if (Table[k].frame == the_names[cst_fonts]) {
        if (k < 1) return false;
        k--;
    }
    if (Table[k].frame == s) return true;
    if (Table[k].frame != the_names[cst_argument]) return false;
    if (k < 1) return false;
    k--;
    while (Table[k].frame.spec_empty()) k--;
    return Table[k].frame == s;
}

// Returns true inside a float (table or figure)
auto Stack::is_float() -> bool {
    auto k = Table.size();
    for (size_t i = 0; i < k; i++) {
        if (Table[i].frame.spec_empty()) continue;
        if (Table[i].frame == the_names[np_float_figure]) return true;
        if (Table[i].frame == the_names[np_float_table]) return true;
    }
    return false;
}

// Prints something in trace mode when we push a non-empty frame.
void Stack::push_trace() {
    if (the_parser.tracing_stack()) {
        auto    ptr = Table.size() - 1;
        Istring fr  = Table[ptr].frame;
        if (!fr.spec_empty()) the_log << lg_startbrace << "Push " << fr << " " << ptr << lg_endbrace;
    }
}

// Internal code of push
void Stack::ipush(Istring fr, Xml *V) { Table.push_back(StackSlot(V, the_parser.get_cur_line(), std::move(fr), cur_mode, cur_lid)); }

// Pushes code with frame.
void Stack::push(Istring fr, Xml *V) {
    top_stack()->push_back_unless_nullptr(V);
    ipush(std::move(fr), V);
    push_trace();
}

// Pushes a new empty object, for \hbox, etc
auto Stack::push_hbox(Istring name) -> Xml * {
    Xml *code = new Xml(std::move(name), nullptr);
    ipush(the_names[cst_hbox], code);
    push_trace();
    return code;
}

// Creates a temporary, sets mode to argument mode.
// The number of the element is 2. No other element has 2 as number
// (see Stack::Stack).
auto Stack::temporary() -> Xml * {
    Xml *res = new Xml(cst_temporary, Xid(2));
    ipush(the_names[cst_argument], res);
    cur_mode = mode_argument;
    push_trace();
    return res;
}

// We assume that document is numbered 1. This simplifies  the mechanism
// for adding attributes to the document.
// Called after all math elements are created
void Stack::init_all(const std::string &a) {
    cur_mode = mode_v;
    cur_lid  = Istring("uid1");
    Xml *V   = new Xml(Istring(a), nullptr);
    V->push_back_unless_nullptr(nullptr); // Make a hole for the color pool
    V->id = 1;
    ipush(the_names[cst_document], V);
    newline_xml = new Xml(Istring("\n"));
}

// For debug. Returns symbolic name of mode
auto stack_ns::mode_to_string(mode x) -> String {
    switch (x) {
    case mode_argument: return "_t";
    case mode_h: return "_h";
    case mode_v: return "_v";
    case mode_none: return "_n";
    case mode_bib: return "_b";
    case mode_array: return "_a";
    case mode_math: return "_m";
    default: return "_?";
    }
}

// Tracing of stack when popping
void Stack::trace_pop(bool sw) {
    if (the_parser.tracing_stack()) {
        the_log << lg_startbrace << "Pop ";
        if (sw) the_log << "(module) ";
        the_log << int(Table.size() - 1) << ":";
        trace_stack();
        the_log << lg_endbrace;
    }
}

// Dumps a stack slot
void Stack::StackSlot::fulldump(size_t i) {
    the_log << "level " << i << " entered at line " << line << ", type " << (!frame.spec_empty() ? frame.name : "()") << ", mode"
            << stack_ns::mode_to_string(md) << ":\n";
    if (obj != nullptr) the_log << obj << "\n";
}

// This prints the whole stack.
void Stack::dump() {
    auto l = Table.size();
    for (size_t i = 0; i < l; i++) Table[i].fulldump(i);
}

// This prints a simplified version of the stack.
void Stack::StackSlot::dump() { the_log << " " << (!frame.spec_empty() ? frame.name : "()") << stack_ns::mode_to_string(md); }

// Like Stack::dump, less verbose.
void Stack::trace_stack() {
    auto l = Table.size();
    for (size_t i = 0; i < l; i++) Table[i].dump();
}

// Pop an element, top-stack should be a.
void Stack::pop(name_positions a) { pop(the_names[a]); }

// This pops an element, top-stack should be a.
void Stack::pop(const Istring &a) {
    trace_pop(false);
    while (Table.back().frame.spec_empty()) { // ignore dummy frames
        the_parser.cur_font.not_on_stack();
        Table.pop_back();
    }
    if (a != Table.back().frame) {
        err_buf << bf_reset << "Error in pop; stack holds " << Table.back().frame << "; trying to pop " << a;
        the_parser.signal_error();
        return;
    }
    cur_mode = Table.back().md;
    cur_lid  = Table.back().uid;
    Table.pop_back();
    if (Table.empty()) {
        err_buf << bf_reset << "Error in pop; stack empty; trying to pop " << a;
        the_parser.signal_error();
        Istring S = the_names[cst_document];
        ipush(S, new Xml(S, nullptr)); // stack should never be empty
    }
}

// Pushes a <p>. The argument a is (an attribute value for) a vertical space
// if indent =false, the paragraph is not indented.
// if a<0 ? beurks...
auto Stack::push_par(long k) -> Xid {
    Xml *res = new Xml(cst_p, nullptr);
    Xid  id  = res->id;
    push(the_names[cst_p], res);
    cur_mode = mode_h; // we are in horizontal mode now
    check_font();
    if (k > 0) id.add_attribute(np_rend, name_positions(np_center_etc + k));
    return id;
}

auto Stack::fonts1(name_positions x) -> Xml * {
    bool     w   = the_main->use_font_elt;
    Xml *    res = new Xml(w ? x : cst_hi, nullptr);
    AttList &W   = res->id.get_att();
    if (!w) W.push_back(np_rend, x);
    return res;
}

// Fonts without argument like \it, (still ok ?)
void Stack::fonts0(name_positions x) {
    Xml *res = fonts1(x);
    res->id.get_att().push_back(the_names[cst_flaghi], Istring(1));
    push(Istring(2), res);
}

// Adds font info when required
void Stack::check_font() {
    while (Table.back().frame.spec_empty()) Table.pop_back();
    bool           w = the_main->pack_font_elt;
    name_positions s{};
    if (w) {
        Buffer aux;
        bool   nonempty = false;
        s               = the_parser.cur_font.size_change();
        if (s != 0U) {
            aux << the_names[s].name;
            nonempty = true;
        }
        s = the_parser.cur_font.shape_change();
        if (s != 0U) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].name;
            nonempty = true;
        }
        s = the_parser.cur_font.family_change();
        if (s != 0U) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].name;
            nonempty = true;
        }
        s = the_parser.cur_font.series_change();
        if (s != 0U) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].name;
            nonempty = true;
        }
        if (nonempty) {
            auto     a   = Istring(aux.to_string());
            Xml *    res = new Xml(cst_hi, nullptr);
            AttList &W   = res->id.get_att();
            W.push_back(the_names[np_rend], a);
            W.push_back(the_names[cst_flaghi], Istring(1));
            push(Istring(2), res);
        }
    } else {
        s = the_parser.cur_font.size_change();
        if (s != 0U) fonts0(s);
        s = the_parser.cur_font.shape_change();
        if (s != 0U) fonts0(s);
        s = the_parser.cur_font.family_change();
        if (s != 0U) fonts0(s);
        s = the_parser.cur_font.series_change();
        if (s != 0U) fonts0(s);
    }
    auto c = the_parser.cur_font.color;
    if (!(c.empty() || c.null())) {
        Xml *    res = new Xml(cst_hi, nullptr);
        AttList &W   = res->id.get_att();
        W.push_back(np_color, c);
        W.push_back(the_names[cst_flaghi], Istring(1));
        push(Istring(2), res);
    }
    the_parser.cur_font.is_on_stack();
}

// Push a new XML element
void Stack::push1(Istring name, name_positions x) { push(std::move(name), new Xml(x, nullptr)); }

// Push a new XML element
void Stack::push1(name_positions x) { push(the_names[x], new Xml(x, nullptr)); }

// Code done when a module ends. pop until stack (nearly) empty
void Stack::end_module() {
    trace_pop(true);
    for (;;) {
        if (Table.empty()) err_ns::fatal_error("unexpected empty stack");
        if (Table.back().frame == the_names[cst_document]) return;
        if (Table.back().frame == the_names[np_module]) {
            pop(the_names[np_module]);
            return;
        }
        Table.pop_back();
    }
}

// Conditional pop
void Stack::pop_if_frame(const Istring &x) {
    if (first_frame() == x) pop(x);
}

// Code done when a section ends. Pops all that is required
void Stack::para_aux(int x) {
    if (x <= 6) pop_if_frame(the_names[cst_div6]);
    if (x <= 5) pop_if_frame(the_names[cst_div5]);
    if (x <= 4) pop_if_frame(the_names[cst_div4]);
    if (x <= 3) pop_if_frame(the_names[cst_div3]);
    if (x <= 2) pop_if_frame(the_names[cst_div2]);
    if (x <= 1) pop_if_frame(the_names[cst_div1]);
    if (x <= 0) pop_if_frame(the_names[cst_div0]);
}

// This allocates a slot for a new table.
auto Stack::new_array_info(Xid id) -> ArrayInfo & {
    AI.emplace_back(id);
    return AI.back();
}

// This finds the table info given an ID
auto Stack::find_cell_props(Xid id) -> ArrayInfo * {
    if (AI.empty()) return nullptr;
    if (!(AI.back().id == id)) return nullptr;
    return &AI.back();
}

// This finds the cell row and table ID currently on the  stack.
void Stack::find_cid_rid_tid(Xid &cid, Xid &rid, Xid &tid) {
    auto k = Table.size() - 1;
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_cell]) {
        cid = Table[k].obj->id;
        k--;
    }
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_row]) {
        rid = Table[k].obj->id;
        k--;
    }
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_tabular] || Table[k].frame == the_names[np_tabular_star]) {
        tid = Table[k].obj->id;
        k--;
    }
}

auto Stack::find_ctrid(subtypes m) -> long {
    for (auto k = Table.size(); k-- >= 1;) {
        Xml *obj = Table[k].obj;
        if (obj == nullptr) continue;
        Istring frame = Table[k].frame;
        if (frame.spec_empty()) continue;
        if (m == xmlcurrow_code && frame == the_names[np_row]) return obj->id.value;
        if (m == xmlcurcell_code && frame == the_names[np_cell]) return obj->id.value;
        if (m == xmlcurarray_code && (frame == the_names[np_tabular] || frame == the_names[np_tabular_star])) return obj->id.value;
    }
    return 0;
}

// Computes the cell-id in cid, and returns the table associated.
auto Stack::get_my_table(Xid &cid) -> ArrayInfo * {
    Xid rid, tid;
    find_cid_rid_tid(cid, rid, tid);
    ArrayInfo *A = find_cell_props(tid);
    if (A == nullptr) {
        log_and_tty << tid.value << "find_cell_prop_failure\n" << lg_fatal;
        abort();
    }
    return A;
}

// This deletes a table-information slot from the stack.
void Stack::delete_table_atts() {
    if (AI.empty()) return;
    AI.pop_back();
}

// Adds a new <u>, <v>, and att-list slot
void ArrayInfo::add_uv(TokenList &u, TokenList &v, const AttList &At) {
    u_table.push_back(u);
    v_table.push_back(v);
    attribs.push_back(At);
}

// This gets u-part or v-part
auto ArrayInfo::get_u_or_v(bool u_or_v, size_t pos) const -> TokenList {
    if (pos >= attribs.size()) return TokenList();
    return u_or_v ? u_table[pos] : v_table[pos];
}

auto Stack::get_u_or_v(bool u_or_v) -> TokenList {
    Xid        unused;
    ArrayInfo *A       = get_my_table(unused);
    auto       cell_no = A->cell_no;
    return A->get_u_or_v(u_or_v, cell_no);
}

auto ArrayInfo::get_cell_atts(size_t k) const -> AttList {
    if (k >= attribs.size()) return AttList();
    return attribs[k];
}

// Adds positions attributes to the current cell, given the current
// table info.
void Stack::finish_cell(int w) {
    Xid        cid;
    ArrayInfo *A       = get_my_table(cid);
    auto       cell_no = A->cell_no;
    AttList    atts    = A->get_cell_atts(cell_no);
    int        n       = 0;
    if (!shbuf.install_att(cid, the_names[np_cols]))
        n = 0;
    else
        n = atoi(shbuf.c_str());
    if (n != 0) {
        cell_no += to_unsigned(n);
    } else {
        cell_no++;
        cid.add_attribute(atts, true);
    }
    A->cell_no = (w == -1 ? cell_no : to_unsigned(w));
}

// This returns the span of the current cell; -1 in case of trouble
// the default value is 1
auto Xml::get_cell_span() const -> long { // \todo std::optional<size_t>
    if (is_xmlc()) return 0;
    if (!has_name(the_names[np_cell])) return -1;             // not a cell
    if (!shbuf.install_att(id, the_names[np_cols])) return 1; // no property, default is 1
    auto o = shbuf.int_val();
    return o ? to_signed(*o) : -1;
}

// hack...
void Stack::mark_omit_cell() { Table.back().omit_cell = true; }

// This removes the last element of the top-stack
auto Stack::remove_last() -> Xml * { return top_stack()->remove_last(); }

inline auto get_cur_label() -> Istring { return Istring(the_parser.eqtb_string_table[0].val); }

void Stack::create_new_anchor(Xid xid, const Istring& id, const Istring& idtext) {
    AttList &AL = get_att_list(to_unsigned(xid.value));
    AL.push_back(the_names[np_id], id);
    AL.push_back(the_names[np_idtext], idtext);
}

// mark current element as target for a label.
auto Stack::add_new_anchor() -> Istring {
    Istring id = next_label_id();
    set_cur_id(id);
    create_new_anchor(last_xid, id, get_cur_label());
    return id;
}

auto Stack::add_new_anchor_spec() -> Istring {
    static size_t last_top_label_id = 0;
    auto          id                = Istring(fmt::format("cid{}", ++last_top_label_id));
    set_cur_id(id);
    create_new_anchor(last_xid, id, get_cur_label());
    return id;
}

auto Xml::tail_is_anchor() const -> bool { return !empty() && back()->is_anchor(); }

// Add an anchor if needed.
auto Stack::add_anchor(const std::string &s, bool spec) -> Istring {
    if (!spec && (top_stack()->tail_is_anchor())) return get_cur_id();
    Istring id = next_label_id();
    set_cur_id(id);
    if (!spec) {
        add_newid0(np_anchor);
        create_new_anchor(last_xid, id, Istring(s));
    } else {
        create_new_anchor(cur_xid(), id, Istring(s));
    }
    return id;
}
