#pragma once
// -*- C++ -*-
// $Id: txxml.h,v 2.19 2015/07/27 16:15:40 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// a xml object
class Xml {
    Xid          id;   // id of the objet
    Istring      name; // name of the element
    vector<Xmlp> tree; // the aux field
public:
    Xml(Istring n) : id(0), name(n) {}
    Xml(const Buffer &n) : id(0), name(Istring(n.c_str())) {}
    Xml(StrHash &X) : id(0), name(Istring(X.hash_find())) {}
    Xml(Istring x, Xmlp z);
    Xml(name_positions x, Xmlp z);
    Xml(name_positions x, Xid n) : id(n), name(Istring(x)) {}
    Xml() : id(0), name(Istring(0)) {}

    void               change_id(Xid n) { id = n; } // use with care
    void               add_att(Istring a, Istring b) { id.add_attribute(a, b); }
    void               add_att(name_positions a, name_positions b) { id.add_attribute(a, b); }
    void               add_first(Xmlp x);
    void               add_ref(string s);
    void               add_tmp(Xmlp val);
    void               add_last_nl(Xmlp val);
    void               add_last_string(const Buffer &B);
    void               add_nl();
    void               add_non_empty_to(Xmlp res);
    auto               back() -> Xmlp { return tree.empty() ? nullptr : tree.back(); }
    void               change_name(Istring s) { name = s; }
    void               change_name(name_positions s) { name = the_names[s]; }
    void               compo_special();
    auto               contains_env(Istring name) -> bool;
    void               convert_to_string(Buffer &B);
    auto               convert_to_string() -> string;
    auto               delete_one_env0(Istring name) -> Xid;
    auto               deep_copy() -> Xmlp;
    [[nodiscard]] auto empty() const -> bool;
    [[nodiscard]] auto empty_name() const -> bool { return name.empty(); }
    auto               father(Xmlp X, int &) -> Xmlp;
    auto               figline(int &ctr, Xmlp junk) -> Xmlp;
    auto               find_on_tree(Xmlp check, Xmlp *res) const -> bool;
    auto               first_lower(Istring src) -> Xmlp;
    [[nodiscard]] auto get_cell_span() const -> int;
    auto               get_first_env(name_positions name) -> Xmlp;
    [[nodiscard]] auto get_id() const -> Xid { return id; }
    [[nodiscard]] auto get_name() const -> Istring { return name; }
    [[nodiscard]] auto has_name(Istring s) const -> bool { return name == s; }
    [[nodiscard]] auto has_name(name_positions s) const -> bool { return name == the_names[s]; }
    auto               how_many_env(Istring name) -> int;
    void               insert_at(int pos, Xmlp x);
    auto               insert_at_ck(int pos, Xmlp x) -> bool;
    void               insert_bib(Xmlp bib, Xmlp match);
    [[nodiscard]] auto is_anchor() const -> bool { return !is_xmlc() && name == the_names[np_anchor]; }
    auto               is_child(Xmlp) const -> bool;
    [[nodiscard]] auto is_empty() const -> bool;
    [[nodiscard]] auto is_empty_p() const -> bool;
    [[nodiscard]] auto is_empty_spec() const -> bool;
    [[nodiscard]] auto is_xmlc() const -> bool { return id.value <= 0; }
    void               kill_name() { name = Istring(); }
    [[nodiscard]] auto last_addr() const -> Xmlp;
    auto               last_box() -> Xmlp;
    [[nodiscard]] auto last_is(name_positions) const -> bool;
    [[nodiscard]] auto last_is_string() const -> bool;
    void               last_to_SH();
    void               make_hole(int pos);
    void               move(Istring name, Xmlp res);
    void               one_fig_tab(bool is_fig);
    [[nodiscard]] auto only_hi() const -> bool;
    [[nodiscard]] auto only_recur_hi() const -> bool;
    [[nodiscard]] auto only_text() const -> bool;
    auto               par_is_empty() -> bool;
    void               pop_back() { tree.pop_back(); }
    void               postprocess_fig_table(bool);
    auto               prev_sibling(Xmlp) -> Xmlp;
    auto               put_at(int n, Xmlp x) -> bool;
    void               put_in_buffer(Buffer &x);
    void               push_back(Buffer &B) { push_back(new Xml(B)); }
    void               push_back(Xmlp val);
    void               push_back_list(Xmlp);
    [[nodiscard]] auto real_size() const -> int;
    void               recurse(XmlAction &X);
    void               recurse0(XmlAction &X);
    void               remove_empty_par();
    auto               remove_at(int) -> bool;
    auto               remove_last() -> Xmlp;
    void               remove_last_empty_hi();
    void               remove_last_space();
    void               remove_par_bal_if_ok();
    void               rename(Istring, Istring);
    void               reset();
    void               sans_titre();
    auto               sans_titre(Xmlp) -> String;
    void               set_id(int i) { id = i; }
    [[nodiscard]] auto single_non_empty() const -> Xmlp;
    [[nodiscard]] auto single_son() const -> Xmlp;
    [[nodiscard]] auto size() const -> int { return tree.size(); }
    void               subst_env0(Istring name, Xmlp value);
    void               swap_x(Xmlp);
    [[nodiscard]] auto tail_is_anchor() const -> bool;
    void               to_buffer(Buffer &) const;
    auto               total_span(int &res) const -> bool;
    auto               try_cline(bool action) -> bool;
    auto               try_cline_again(bool) -> bool;
    void               unbox(Xmlp);
    auto               value_at(int) -> Xmlp;
    void               word_stats(string);
    void               word_stats_i();
    auto               spec_copy() -> Xmlp;
    void               replace_first(Xmlp x) {
        if (tree.size()) tree[0] = x;
    }
    void bordermatrix();
};

class XmlAndType {
    Xmlp       value;
    math_types type;

public:
    XmlAndType(Xmlp X) : value(X), type(mt_flag_small) {}
    XmlAndType(Xmlp X, math_types t) : value(X), type(t) {}
    [[nodiscard]] auto get_value() const -> Xmlp { return value; }
    [[nodiscard]] auto get_type() const -> math_types { return type; }
};
