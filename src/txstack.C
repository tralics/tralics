// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2004, 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file implements the XML stack for Tralics

#include "tralics.h"
#include <cstring>

namespace stack_ns {
    auto mode_to_string(mode x) -> String;
}

extern bool booted;
// Increases xid, makes sure that the attribute table is big enough
auto Stack::next_xid(Xml *elt) -> Xid {
    attributes.emplace_back();
    enames.push_back(elt);
    //  if(booted && elt) cout << "XX" << last_xid+1 << elt << "\n";
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
    int k = enames.size();
    for (int i = xid_boot + 1; i < k; i++)
        if (enames[i]) {
            cout << i << "::"; //<< enames[i] -> get_name() << "\n";
            cout << enames[i] << "\n";
        }
}

// Returns the element in the table with id n
// This should be at position N
auto Stack::fetch_by_id(int n) -> Xml * {
    if (n <= 0) return nullptr; // no need to look at this
    if (int(enames.size()) <= n) return nullptr;
    Xml *x = enames[n];
    if (!x) return nullptr;
    if (x->get_id().value == n) return x;
    cout << "This cannot happen: bug in table at position " << n << "\n";
    return nullptr;
}

// returns a parent of x
auto Stack::find_parent(Xml *x) -> Xml * {
    if (!x) return nullptr;
    int k = enames.size();
    // debug: print all parents
    // cout << "Search parent of " << x << "\n";
    // for(int i= 0; i< k; i++) {
    //   if(enames[i] && enames[i]-> is_child (x))
    //     cout << "fount at " << enames[i] << "\n";
    // }
    for (int i = xid_boot + 1; i < k; i++) {
        if (!enames[i]) continue;
        if (enames[i]->is_child(x)) return enames[i];
    }
    return nullptr;
}

auto Parser::last_att_list() -> AttList & { return the_stack.get_top_id().get_att(); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(Istring A, Istring B) { get_att_list(last_xid).push_back(A, B); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(name_positions A, Istring B) { get_att_list(last_xid).push_back(A, B); }

// Add A=B as attribute list to last_xid.
void Stack::add_att_to_last(name_positions A, name_positions B) { get_att_list(last_xid).push_back(A, B); }

// Add A=B as attribute list to top stack
void Stack::add_att_to_cur(Istring A, Istring B) { cur_xid().add_attribute(A, B); }

// Add A=B as attribute list to last_xid
// (if force is true, ignores old value otherwise new value).
void Stack::add_att_to_last(Istring A, Istring B, bool force) { get_att_list(last_xid).push_back(A, B, force); }

// Add A=B as attribute list to top stack
// (if force is true, ignores old value otherwise new value).
void Stack::add_att_to_cur(Istring A, Istring B, bool force) { cur_xid().get_att().push_back(A, B, force); }

// Returns a new element named N, initialised with z (if not empty...)
Xml::Xml(Istring N, Xml *z) : name(N) {
    id = the_main->the_stack->next_xid(this);
    if (z) add_tmp(z);
}

Xml::Xml(name_positions N, Xml *z) : name(Istring(N)) {
    id = the_main->the_stack->next_xid(this);
    if (z) add_tmp(z);
}

void Stack::hack_for_hanl() {
    int ptr = Table.size() - 1;
    if (ptr > 0) { Table[ptr].obj = Table[ptr - 1].obj; }
    // else ?
}

// Returns element below topstack.
auto Stack::get_father() -> Xml * {
    int ptr = Table.size() - 1;
    if (ptr > 0)
        return Table[ptr - 1].obj;
    else
        return nullptr;
}

// Creates an empty element named x, and adds it to the stack.
// returns a reference to the attribute list of the object.
auto Stack::add_newid0(name_positions x) -> AttList & {
    top_stack()->push_back(new Xml(x, nullptr));
    return Xid(last_xid).get_att();
}

// Adds x to the tail of the current list.
void Stack::add_last(Xml *x) { top_stack()->push_back(x); }

// This adds x at the end the element
void Xml::push_back(Xml *x) {
    if (x) tree.push_back(x);
}

// Adds B to the tail of the current list.
void Stack::add_last_string(const Buffer &B) { top_stack()->add_last_string(B); }

// True if last element on the tree is a string.
auto Xml::last_is_string() const -> bool { return tree.size() > 0 && tree.back() && tree.back()->get_id().value == 0; }

// Assume that last element is a string. This string is put in the
// internal buffer of SH.
void Xml::last_to_SH() {
    Buffer &b = the_main->SH.shbuf(); // write directly in the buffer.
    b.reset();
    b.push_back(tree.back()->name.c_str());
}

// This adds B at the end the element, via concatenation, if possible.
void Xml::add_last_string(const Buffer &B) {
    if (B.empty()) return;
    Buffer &aux = the_main->SH.shbuf(); // write directly in the buffer.
    aux.reset();
    if (last_is_string()) {
        last_to_SH();
        tree.pop_back();
        the_parser.my_stats.one_more_merge();
    }
    aux.push_back(B.c_str());
    tree.push_back(new Xml(the_main->SH));
}

// This adds x and a \n at the end of this.
void Xml::add_last_nl(Xml *x) {
    if (x) {
        tree.push_back(x);
        tree.push_back(the_main->the_stack->newline_xml);
    }
}

// Implements unhbox, etc.
void Stack::unbox(Xml *x) { top_stack()->unbox(x); }

// Removes a trailing space on the current tree.
void Stack::remove_last_space() { top_stack()->remove_last_space(); }

// Removes a trailing space on the tree.
void Xml::remove_last_space() {
    if (!last_is_string()) return;
    Buffer &aux = the_main->SH.shbuf(); // write directly in the buffer.
    last_to_SH();
    int k = aux.length();
    aux.remove_space_at_end();
    if (k != aux.length()) {
        tree.pop_back();
        if (!aux.empty()) tree.push_back(new Xml(the_main->SH));
    }
}

// This adds a newline to the XML.
void Stack::add_nl() { top_stack()->add_nl(); }

// This adds a NL to the end of the element
void Xml::add_nl() {
    if (tree.size() > 0 && tree.back() == the_main->the_stack->newline_xml) return;
    tree.push_back(the_main->the_stack->newline_xml);
}

// Returns the slot of the first non-empty frame
// It is assumed that the first frame is "document", hence never empty.
auto Stack::first_non_empty() const -> const Stack::StackSlot & {
    int k = Table.size() - 1;
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
auto Stack::get_cur_par() -> Xml * {
    const StackSlot &X     = first_non_empty();
    Istring          pname = the_names[cst_p];
    if (X.frame == pname && !X.obj->is_xmlc() && X.obj->has_name(pname))
        return X.obj;
    else
        return nullptr;
}

// In case the current element happens to be a P,
// and has no rend attribute, then add one to it (can be center, can also
// be flush left, etc.)
void Stack::add_center_to_p() {
    Xml *x = get_cur_par();
    if (!x) return;
    int w = the_parser.cur_centering();
    x->get_id().get_att().push_back(np_rend, name_positions(np_center_etc + w), false);
}

auto Stack::is_frame(name_positions s) const -> bool { return first_frame() == the_names[s]; }

// Returns true if frame is s, or argument, and next frame is s.
// ignores a font change
auto Stack::is_frame2(name_positions S) const -> bool {
    Istring s = the_names[S];
    int     k = Table.size() - 1;
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
    if (Table[k].frame == s) return true;
    return false;
}

// Returns true inside a float (table or figure)
auto Stack::is_float() -> bool {
    int k = Table.size();
    for (int i = 0; i < k; i++) {
        if (Table[i].frame.spec_empty()) continue;
        if (Table[i].frame == the_names[np_float_figure]) return true;
        if (Table[i].frame == the_names[np_float_table]) return true;
    }
    return false;
}

// Prints something in trace mode when we push a non-empty frame.
void Stack::push_trace() {
    if (the_parser.tracing_stack()) {
        int     ptr = Table.size() - 1;
        Istring fr  = Table[ptr].frame;
        if (!fr.spec_empty()) the_log << lg_startbrace << "Push " << fr << " " << ptr << lg_endbrace;
    }
}

// Internal code of push
void Stack::ipush(Istring fr, Xml *V) { Table.push_back(StackSlot(V, the_parser.get_cur_line(), fr, cur_mode, cur_lid)); }

// Pushes code with frame.
void Stack::push(Istring fr, Xml *V) {
    top_stack()->push_back(V);
    ipush(fr, V);
    push_trace();
}

// Pushes a new empty object, for \hbox, etc
auto Stack::push_hbox(Istring name) -> Xml * {
    Xml *code = new Xml(name, nullptr);
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
void Stack::init_all(string a) {
    cur_mode = mode_v;
    cur_lid  = Istring("uid1");
    Xml *V   = new Xml(Istring(a), nullptr);
    V->push_back(nullptr); // Make a hole for the color pool
    V->change_id(1);
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
void Stack::StackSlot::fulldump(int i) {
    the_log << "level " << i << " entered at line " << line << ", type " << (!frame.spec_empty() ? frame.c_str() : "()") << ", mode"
            << stack_ns::mode_to_string(md) << ":\n";
    if (obj) the_log << obj << "\n";
}

// This prints the whole stack.
void Stack::dump() {
    int l = Table.size();
    for (int i = 0; i < l; i++) Table[i].fulldump(i);
}

// This prints a simplified version of the stack.
void Stack::StackSlot::dump() { the_log << " " << (!frame.spec_empty() ? frame.c_str() : "()") << stack_ns::mode_to_string(md); }

// Like Stack::dump, less verbose.
void Stack::trace_stack() {
    int l = Table.size();
    for (int i = 0; i < l; i++) Table[i].dump();
}

// Pop an element, top-stack should be a.
void Stack::pop(name_positions a) { pop(the_names[a]); }

// This pops an element, top-stack should be a.
void Stack::pop(Istring a) {
    trace_pop(false);
    while (Table.back().frame.spec_empty()) { // ignore dummy frames
        the_parser.cur_font.not_on_stack();
        Table.pop_back();
    }
    if (a != Table.back().frame) {
        err_ns::local_buf << bf_reset << "Error in pop; stack holds " << Table.back().frame << "; trying to pop " << a;
        the_parser.signal_error();
        return;
    }
    cur_mode = Table.back().md;
    cur_lid  = Table.back().uid;
    Table.pop_back();
    if (Table.empty()) {
        err_ns::local_buf << bf_reset << "Error in pop; stack empty; trying to pop " << a;
        the_parser.signal_error();
        Istring S = the_names[cst_document];
        ipush(S, new Xml(S, nullptr)); // stack should never be empty
    }
}

// Pushes a <p>. The argument a is (an attribute value for) a vertical space
// if indent =false, the paragraph is not indented.
// if a<0 ? beurks...
auto Stack::push_par(int k) -> Xid {
    Xml *res = new Xml(cst_p, nullptr);
    Xid  id  = res->get_id();
    push(the_names[cst_p], res);
    cur_mode = mode_h; // we are in horizontal mode now
    check_font();
    if (k > 0) id.add_attribute(np_rend, name_positions(np_center_etc + k));
    return id;
}

auto Stack::fonts1(name_positions x) -> Xml * {
    bool     w   = the_main->use_font_elt();
    Xml *    res = new Xml(w ? x : cst_hi, nullptr);
    AttList &W   = res->get_id().get_att();
    if (!w) W.push_back(np_rend, x);
    return res;
}

// Fonts without argument like \it, (still ok ?)
void Stack::fonts0(name_positions x) {
    Xml *res = fonts1(x);
    res->get_id().get_att().push_back(the_names[cst_flaghi], Istring(1));
    push(Istring(2), res);
}

// Adds font info when required
void Stack::check_font() {
    while (Table.back().frame.spec_empty()) Table.pop_back();
    bool           w = the_main->pack_font_elt();
    name_positions s;
    if (w) {
        Buffer &aux = the_main->SH.shbuf(); // write directly in the buffer.
        aux.reset();
        bool nonempty = false;
        s             = the_parser.cur_font.size_change();
        if (s) {
            aux << the_names[s].c_str();
            nonempty = true;
        }
        s = the_parser.cur_font.shape_change();
        if (s) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].c_str();
            nonempty = true;
        }
        s = the_parser.cur_font.family_change();
        if (s) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].c_str();
            nonempty = true;
        }
        s = the_parser.cur_font.series_change();
        if (s) {
            if (nonempty) aux.push_back(",");
            aux << the_names[s].c_str();
            nonempty = true;
        }
        if (nonempty) {
            auto     a   = Istring(the_main->SH.hash_find());
            Xml *    res = new Xml(cst_hi, nullptr);
            AttList &W   = res->get_id().get_att();
            W.push_back(the_names[np_rend], a);
            W.push_back(the_names[cst_flaghi], Istring(1));
            push(Istring(2), res);
        }
    } else {
        s = the_parser.cur_font.size_change();
        if (s) fonts0(s);
        s = the_parser.cur_font.shape_change();
        if (s) fonts0(s);
        s = the_parser.cur_font.family_change();
        if (s) fonts0(s);
        s = the_parser.cur_font.series_change();
        if (s) fonts0(s);
    }
    Istring c = the_parser.cur_font.get_color();
    if (!(c.empty() || c.null())) {
        Xml *    res = new Xml(cst_hi, nullptr);
        AttList &W   = res->get_id().get_att();
        W.push_back(np_color, c);
        W.push_back(the_names[cst_flaghi], Istring(1));
        push(Istring(2), res);
    }
    the_parser.cur_font.is_on_stack();
}

// Push a new XML element
void Stack::push1(Istring name, name_positions x) { push(name, new Xml(x, nullptr)); }

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
void Stack::pop_if_frame(Istring x) {
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
    if (!AI.back().its_me(id)) return nullptr;
    return &AI.back();
}

// This finds the cell row and table ID currently on the  stack.
void Stack::find_cid_rid_tid(Xid &cid, Xid &rid, Xid &tid) {
    int k = Table.size() - 1;
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_cell]) {
        cid = Table[k].obj->get_id();
        k--;
    }
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_row]) {
        rid = Table[k].obj->get_id();
        k--;
    }
    while (Table[k].frame.spec_empty()) k--;
    if (Table[k].frame == the_names[np_tabular] || Table[k].frame == the_names[np_tabular_star]) {
        tid = Table[k].obj->get_id();
        k--;
    }
}

auto Stack::find_ctrid(subtypes m) -> int {
    int k = Table.size() - 1;
    while (k >= 0) {
        Istring frame = Table[k].frame;
        Xml *   obj   = Table[k].obj;
        k--;
        if (!obj) continue;
        if (frame.spec_empty()) continue;
        if (m == xmlcurrow_code && frame == the_names[np_row]) return obj->get_id().value;
        if (m == xmlcurcell_code && frame == the_names[np_cell]) return obj->get_id().value;
        if (m == xmlcurarray_code && (frame == the_names[np_tabular] || frame == the_names[np_tabular_star])) return obj->get_id().value;
    }
    return 0;
}

// Computes the cell-id in cid, and returns the table associated.
auto Stack::get_my_table(Xid &cid) -> ArrayInfo * {
    Xid rid, tid;
    find_cid_rid_tid(cid, rid, tid);
    ArrayInfo *A = find_cell_props(tid);
    if (!A) {
        main_ns::log_and_tty << tid.value << "find_cell_prop_failure\n" << lg_fatal;
        abort();
    }
    return A;
}

// This is the Dtor
void ArrayInfo::del_array_info() {
    for (int i = 0; i < size; i++) { attribs[i].destroy(); }
}

// This deletes a table-information slot from the stack.
void Stack::delete_table_atts() {
    if (AI.empty()) return;
    AI.back().del_array_info();
    AI.pop_back();
}

// Adds a new <u>, <v>, and att-list slot
void ArrayInfo::add_uv(TokenList &u, TokenList &v, AttList At) {
    u_table.push_back(u);
    v_table.push_back(v);
    attribs.push_back(At);
    size++;
}

// This gets u-part or v-part
auto ArrayInfo::get_u_or_v(bool u_or_v, int pos) -> TokenList {
    if (pos < 0 || pos >= size) return TokenList();
    return u_or_v ? u_table[pos] : v_table[pos];
}

auto Stack::get_u_or_v(bool u_or_v) -> TokenList {
    Xid        unused;
    ArrayInfo *A       = get_my_table(unused);
    int        cell_no = A->get_cell_no();
    return A->get_u_or_v(u_or_v, cell_no);
}

auto ArrayInfo::get_cell_atts(int k) -> AttList {
    if (k < 0 || k >= size)
        return AttList();
    else
        return attribs[k];
}

// Adds positions attributes to the current cell, given the current
// table info.
void Stack::finish_cell(int w) {
    Xid        cid;
    ArrayInfo *A       = get_my_table(cid);
    int        cell_no = A->get_cell_no();
    AttList    atts    = A->get_cell_atts(cell_no);
    Buffer &   B       = the_main->SH.shbuf();
    int        n;
    if (!B.install_att(cid, the_names[np_cols]))
        n = 0;
    else
        n = atoi(B.c_str());
    if (n) {
        cell_no += n;
    } else {
        cell_no++;
        cid.add_attribute(atts, true);
    }
    if (w == -1) w = cell_no;
    A->set_cell_no(w);
}

// This returns the span of the current cell; -1 in case of trouble
// the default value is 1
auto Xml::get_cell_span() const -> int {
    Buffer &B = the_main->SH.shbuf();
    if (is_xmlc()) return 0;
    if (!has_name(the_names[np_cell])) return -1;               // not a cell
    if (!B.install_att(get_id(), the_names[np_cols])) return 1; // no property, default is 1
    return B.get_int_val();
}

// hack...
void Stack::mark_omit_cell() { Table.back().omit_cell = true; }

// This removes the last element of the top-stack
auto Stack::remove_last() -> Xml * { return top_stack()->remove_last(); }

inline auto get_cur_label() -> Istring { return Istring(the_parser.eqtb_string_table[0].get_val()); }

void Stack::create_new_anchor(Xid xid, Istring id, Istring idtext) {
    AttList &AL = get_att_list(xid.value);
    AL.push_back(the_names[np_id], id);
    AL.push_back(the_names[np_idtext], idtext);
}

// mark current element as target for a label.
auto Stack::add_new_anchor() -> Istring {
    Istring id = the_main->SH.next_label_id();
    set_cur_id(id);
    create_new_anchor(last_xid, id, get_cur_label());
    return id;
}

auto Stack::add_new_anchor_spec() -> Istring {
    Istring id = the_main->SH.next_top_label_id();
    set_cur_id(id);
    create_new_anchor(last_xid, id, get_cur_label());
    return id;
}

auto Xml::tail_is_anchor() const -> bool { return tree.size() > 0 && tree.back() && tree.back()->is_anchor(); }

// Add an anchor if needed.
auto Stack::add_anchor(const string &s, bool spec) -> Istring {
    if (!spec && (top_stack()->tail_is_anchor())) return get_cur_id();
    Istring id = the_main->SH.next_label_id();
    set_cur_id(id);
    if (!spec) {
        add_newid0(np_anchor);
        create_new_anchor(last_xid, id, Istring(s));
    } else {
        create_new_anchor(cur_xid(), id, Istring(s));
    }
    return id;
}
