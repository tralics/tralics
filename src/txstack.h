#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file holds the stack and related stuff

#include "txbuffer.h"
#include "txtokenlist.h"
#include "txxml.h"

// Array management
class ArrayInfo {
    Xid               id;      // the id of the table
    int               cell_no; // current cell number, first on row is zero
    vector<AttList>   attribs; // positions attributes for each row
    vector<TokenList> u_table, v_table;
    int               size;

public:
    ArrayInfo(Xid a1) : id(a1), cell_no(0), size(0) {}
    auto               its_me(Xid a) -> bool { return id == a; }
    [[nodiscard]] auto get_cell_no() const -> int { return cell_no; }
    void               set_cell_no(int k) { cell_no = k; }
    auto               get_cell_atts(int k) -> AttList;
    auto               get_u_or_v(bool, int) -> TokenList;
    void               add_uv(TokenList &u, TokenList &v, const AttList &);
    auto               get_size() -> int { return size; }
    void               del_array_info();
};

// This is the stack
class Stack {
    class StackSlot {
        Xml *   obj;
        int     line;
        Istring frame;
        mode    md;
        Istring uid;
        bool    omit_cell;
        void    dump();
        void    fulldump(int);
        StackSlot(Xml *a, int b, Istring c, mode M, Istring u) : obj(a), line(b), frame(c), md(M), uid(u), omit_cell(false) {}
        friend class Stack;
    };
    vector<StackSlot> Table;

private:
    int               last_xid; // id of the last
    int               xid_boot;
    Istring           cur_lid;    // the id to be pushed on uids[]
    vector<AttList>   attributes; // the main table of attributes
    vector<Xml *>     enames;     // the main table of element names
    Buffer            mybuffer;   // a buffer
    vector<ArrayInfo> AI;         // the attributes for the current TeX arrays
    mode              cur_mode;   // the mode to be pushed on modes[]
public:
    Stack();
    Xml *newline_xml;

    auto               add_anchor(const string &, bool) -> Istring;
    void               add_att_to_last(Istring, Istring, bool);
    void               add_att_to_last(Istring, Istring);
    void               add_att_to_last(name_positions, name_positions);
    void               add_att_to_last(name_positions, Istring);
    void               add_att_to_cur(Istring A, Istring B);
    void               add_att_to_cur(Istring A, Istring B, bool);
    void               add_border(int a, int b);
    void               add_borders(int a, int b);
    void               add_center_to_p();
    void               add_last(Xml *);
    void               add_last_string(const Buffer &B);
    auto               add_new_anchor() -> Istring;
    auto               add_new_anchor_spec() -> Istring;
    void               add_nl();
    auto               add_newid0(name_positions) -> AttList &;
    void               add_sp_to_p(int pid, int vid);
    void               check_font();
    void               create_new_anchor(Xid, Istring, Istring);
    auto               cur_xid() -> Xid { return top_stack()->get_id(); }
    void               delete_table_atts();
    void               dump();
    void               dump_xml_table();
    auto               document_element() -> Xml * { return Table[0].obj; }
    auto               elt_from_id(int n) -> Xml * { return enames[n]; }
    void               end_module();
    auto               fetch_by_id(int n) -> Xml *;
    auto               find_cell_props(Xid id) -> ArrayInfo *;
    void               find_cid_rid_tid(Xid &cid, Xid &rid, Xid &tid);
    auto               find_ctrid(subtypes) -> int;
    auto               find_parent(Xml *) -> Xml *;
    void               finish_cell(int w);
    [[nodiscard]] auto first_frame() const -> Istring;
    [[nodiscard]] auto first_non_empty() const -> const StackSlot &;
    void               fonts0(name_positions x);
    auto               fonts1(name_positions x) -> Xml *;
    auto               get_att_list(int k) -> AttList & { return attributes[k]; }
    [[nodiscard]] auto get_cur_id() const -> Istring { return cur_lid; }
    auto               get_cur_par() -> Xml *;
    auto               get_father() -> Xml *;
    [[nodiscard]] auto get_mode() const -> mode { return cur_mode; }
    auto               get_my_table(Xid &) -> ArrayInfo *;
    auto               get_top_id() -> Xid { return top_stack()->get_id(); }
    auto               get_u_or_v(bool u_or_v) -> TokenList;
    auto               get_xid() -> Xid { return last_xid; }
    void               hack_for_hanl();
    void               implement_cit(string b1, Istring b2, string a, string c);
    [[nodiscard]] auto in_v_mode() const -> bool { return get_mode() == mode_v; }
    [[nodiscard]] auto in_h_mode() const -> bool { return get_mode() == mode_h; }
    [[nodiscard]] auto in_no_mode() const -> bool { return get_mode() == mode_none; }
    [[nodiscard]] auto in_bib_mode() const -> bool { return get_mode() == mode_bib; }
    [[nodiscard]] auto in_array_mode() const -> bool { return get_mode() == mode_array; }
    void               init_all(string a);
    void               ipush(Istring, Xml *);
    auto               is_float() -> bool;
    [[nodiscard]] auto is_frame(name_positions) const -> bool;
    [[nodiscard]] auto is_frame2(name_positions) const -> bool;
    auto               is_omit_cell() -> bool { return Table.back().omit_cell; }
    auto               last_att_list() -> AttList & { return get_xid().get_att(); }
    void               mark_omit_cell();
    auto               new_array_info(Xid i) -> ArrayInfo &;
    auto               next_xid(Xml *) -> Xid;
    void               para_aux(int x);
    void               pop(Istring a);
    void               pop(name_positions a);
    void               pop_if_frame(Istring x);
    void               push(Istring fr, Xml *V);
    void               push1(Istring name, name_positions x);
    void               push1(name_positions x);
    auto               push_hbox(Istring) -> Xml *;
    void               push_pop_cell(int);
    void               push_trace();
    auto               push_par(int) -> Xid;
    auto               remove_last() -> Xml *;
    void               remove_last_space();
    void               set_arg_mode() { cur_mode = mode_argument; }
    void               set_array_mode() { cur_mode = mode_array; }
    void               set_bib_mode() { cur_mode = mode_bib; }
    void               set_cur_id(Istring k) { cur_lid = k; }
    void               set_h_mode() { cur_mode = mode_h; }
    void               set_m_mode() { cur_mode = mode_math; }
    void               set_mode(mode x) { cur_mode = x; }
    void               set_no_mode() { cur_mode = mode_none; }
    void               set_v_mode() { cur_mode = mode_v; }
    void               set_xid_boot() { xid_boot = last_xid; }
    void               T_ampersand();
    void               T_hline();
    auto               temporary() -> Xml *;
    auto               top_stack() -> Xml * { return Table.back().obj; }
    void               trace_pop(bool);
    void               trace_stack();
    void               unbox(Xml *);
    auto               xml2_space(Istring a, Xml *c, Xml *d) -> Xml *;
    auto               xml2_space(Istring elt, Istring, Istring, Xml *f_arg, Xml *s_arg) -> Xml *;
    auto               xml2_space(Istring elt, Istring, Xml *first_arg, Xml *second_arg) -> Xml *;
};
