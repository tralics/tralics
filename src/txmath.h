#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) 2003-2004 2006, 2007 INRIA, Jos\'e Grimm

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/MathElt.h"
#include "tralics/Xml.h"
#include <spdlog/spdlog.h>

class Math;
class MathQ;
class MathP;
class MathF;

using MathQList = std::list<MathQ>;

inline auto math_to_sub(math_list_type x) -> subtypes { return subtypes(x - fml_offset); }

// A math object is a strange thing:
// a math_elt object has 4 fields, one is a pointer to another object.
// Other 3 objects are : cmd, chr, token in some cases.
// it can be cmd, chr, font (if cmd==12 or cmd==11), and font is one of
//  math_f_it,math_f_rm,math_f_cal,math_f_bb,math_f_sf,math_f_tt,math_f_bf,
// it can be math_list_cmd, pos, type, where pos is a position into a
//    table of math lists, and type is one of
//     math_open_cd, math_dollar_cd, math_ddollar_cd,math_env_cd, math_LR_cd,
//     math_hbox_cd,math_argument_cd,
// it can be math_xml_cmd, pos, type, where pos is a position into a
//    table of XML elements.

using MathList            = std::list<MathElt>;
using const_math_iterator = std::list<MathElt>::const_iterator;

namespace tralics_ns {
    auto math_env_name(subtypes c) -> String;
} // namespace tralics_ns

class Math : public MathList {
    friend class MathHelper;
    friend class MathDataP;
    friend class MathElt;
    math_list_type type{invalid_cd};

public:
    subtypes    sname{nomathenv_code};
    std::string saved;

    using MathList::push_back;

    [[nodiscard]] auto        duplicate(bool nomath) const -> subtypes;
    auto                      chars_to_mb(Buffer &B, bool rec) const -> bool;
    auto                      chars_to_mb1(Buffer &B) const -> bool;
    auto                      chars_to_mb2(Buffer &B) const -> bool;
    auto                      chars_to_mb3() -> std::string;
    auto                      convert_math(math_style k) -> Xml *;
    auto                      convert_math_noML(bool spec) -> Xml *;
    void                      convert_math_noML0();
    void                      convert_math_noMLt0();
    [[nodiscard]] auto        convert_opname() const -> std::string;
    auto                      convert_this_to_string(Buffer &B) const -> std::string;
    auto                      find_parens(MathQList &res, bool verbose) const -> bool;
    auto                      get_arg1() -> Math { return front().get_list(); }
    [[nodiscard]] auto        get_arg2() const -> Math { return second_element().get_list(); }
    [[nodiscard]] auto        get_arg3() const -> Math { return third_element().get_list(); }
    [[nodiscard]] auto        get_type() const -> math_list_type { return type; }
    [[nodiscard]] auto        get_name() const -> std::string;
    [[nodiscard]] static auto get_list(size_t w) -> Math &;
    void                      hack_type(int);
    [[nodiscard]] auto        has_type(int x) const -> bool { return type == x; }
    [[nodiscard]] auto        has_one_element() const -> bool;
    [[nodiscard]] auto        has_two_elements() const -> bool;
    void                      is_font_cmd1_list(const_math_iterator &B, const_math_iterator &E);
    auto                      M_array(bool numbered, math_style cms) -> Xml *;
    auto                      M_cv(math_style cms, int need_row) -> XmlAndType;
    void                      print() const;
    void                      push_back(CmdChr X, subtypes c, std::string s = "");
    void                      push_back_list(subtypes X, math_list_type c);
    void                      push_back_font(subtypes X, subtypes c);
    void                      push_back(CmdChr X);
    void                      push_back(Xml *A, int b, math_types c);
    void                      push_front(CmdChr X, subtypes c);
    void                      remove_initial_group();
    void                      remove_spaces();
    [[nodiscard]] auto        second_element() const -> const MathElt &;
    void                      set_display_type() { type = math_ddollar_cd; }
    void                      set_env_name(int);
    void                      set_nondisplay_type() { type = math_dollar_cd; }
    void                      set_type(math_list_type c) { type = c; }
    [[nodiscard]] auto        third_element() const -> const MathElt &;
    auto                      trivial_math(long action) -> Xml *;
    auto                      trivial_math_index(symcodes cmd) -> Xml *;
    auto                      check_align() -> int;

private:
    void               add_cur_cont();
    void               add_cur_font();
    auto               add_fence(bool final, MathF &M) -> bool;
    void               concat(Xml *res);
    void               concat_space(Xml *res);
    auto               convert_cell(size_t &n, std::vector<AttList> &table, math_style W) -> Xml;
    auto               convert_char_seq(const MathElt &W) -> MathElt;
    auto               convert_char_iseq(const MathElt &W, bool multiple) -> MathElt;
    void               fetch_rlc(std::vector<AttList> &table);
    void               find_paren0(MathP &aux) const;
    auto               finish_translate1(bool vb) -> bool;
    auto               finish_translate2() -> bool;
    void               handle_mbox(Math &res);
    void               handle_mbox_no();
    void               handle_mbox_not();
    [[nodiscard]] auto has_over() const -> bool;
    auto               is_font_cmd1() -> bool;
    auto               large1(MathElt &cl, math_style cms) -> Xml *;
    auto               M_cv0(math_style cms) -> XmlAndType;
    auto               M_cv3(math_style cms) -> Math;
    void               handle_cmd_Big(math_style cms);
    auto               handle_cmd_Big_aux(math_style cms) -> bool;
    void               cv_hspace_t(MathElt &cur);
    auto               M_cvaux() -> Math;
    auto               M_ref() -> Xml *;
    auto               M_mbox1(Buffer &B, subtypes &f) -> int;
    auto               only_digits(Buffer &B) const -> bool;
    void               push_back(Math &X);
    void               push_back_small(Xml *a);
    void               push_front(Math &X);
    void               skip_initial_space();
    [[nodiscard]] auto special1() const -> Xml *;
    void               special2(bool &ok, Xml *&res) const;
    auto               split_as_array(std::vector<AttList> &table, math_style W, bool numbered) -> Xml *;
    void               remove_opt_arg(bool star);
    auto               remove_req_arg() -> std::string;
    [[nodiscard]] auto remove_req_arg_noerr() const -> std::string;
};

// This is a global object for math handling
// but math handling is not recursive. reset() is called on every formula.
class MathHelper {
    bool                     current_mode{}; // display or not, needed for \label
    std::string              pos_att;        // position attribute, inline or display
    bool                     seen_label{};   // do we see already have a label
    bool                     warned_label{}; // was the used warned for labels on this formula ?
    std::string              label_val;      // name of the label
    std::vector<std::string> multi_labels;
    std::vector<int>         multi_labels_type;
    TokenList                tag_list;         // value of \tag{foo}, if given
    bool                     is_tag_starred{}; // \tag or \tag* ?
    Xid                      cur_cell_id;      // Id of current cell
    Xid                      cur_row_id;       // Id of current row
    Xid                      cur_table_id;     // Id of current table
    Xid                      cur_math_id;      // Id of current math element
    Xid                      cur_formula_id;   // Id of current formula
    Xid                      cur_texmath_id;   // Id of current texmath
    int                      math_env_ctr;     // two counters for environments
    int                      all_env_ctr;
    size_t                   last_ml_pos{0};
    int                      eqnum_status{}; // how many numbers for this equation?
public:
    MathHelper() {
        math_env_ctr = 0;
        all_env_ctr  = 0;
    }
    void reset_last_ml_pos() { last_ml_pos = 0; }
    auto end_of_row() -> bool;
    void dump_labels();
    void ml_check_labels();
    void new_label(const std::string &s, bool a);
    void ml_second_pass(Xml *row, bool vb);
    void ml_last_pass(bool vb);
    void insert_special_tag(std::string s) { multi_labels[last_ml_pos - 2] = std::move(s); }
    void new_multi_label(const std::string &s, int t) {
        multi_labels.push_back(s);
        multi_labels_type.push_back(t);
    }
    auto               get_multi_labels() -> std::vector<std::string> & { return multi_labels; }
    static void        finish_math_mem();
    void               set_type(bool b);
    [[nodiscard]] auto get_eqnum_status() const -> int { return eqnum_status; }
    [[nodiscard]] auto has_label() const -> bool { return seen_label || eqnum_status == 1 || eqnum_status == 3; }
    [[nodiscard]] auto get_label_val() const -> std::string { return label_val; }
    void               stats();
    [[nodiscard]] auto get_pos_att() const -> std::string { return pos_att; }
    void               reset(bool dual);
    [[nodiscard]] auto is_inline() const -> bool { return current_mode; }
    [[nodiscard]] auto has_tag() const -> bool { return !tag_list.empty(); }
    void               reset_tags() { tag_list = TokenList(); }
    void               handle_tags();
    void               add_tag(TokenList &L);
    void               update_all_env_ctr(bool open) {
        if (open)
            all_env_ctr++;
        else
            all_env_ctr--;
    }
    void update_math_env_ctr(bool open) {
        if (open)
            math_env_ctr++;
        else
            math_env_ctr--;
    }
    [[nodiscard]] auto get_math_env_ctr() const -> int { return math_env_ctr; }
    [[nodiscard]] auto get_all_env_ctr() const -> int { return all_env_ctr; }
    void               add_attribute(const std::string &a, const std::string &b, subtypes c);
    [[nodiscard]] auto get_cid() const -> Xid { return cur_cell_id; }
    [[nodiscard]] auto get_rid() const -> Xid { return cur_row_id; }
    [[nodiscard]] auto get_mid() const -> Xid { return cur_math_id; }
    [[nodiscard]] auto get_fid() const -> Xid { return cur_formula_id; }
    [[nodiscard]] auto get_tid() const -> Xid { return cur_texmath_id; }
    [[nodiscard]] auto get_taid() const -> Xid { return cur_table_id; }
    void               set_cid(Xid i) { cur_cell_id = i; }
    void               set_rid(Xid i) { cur_row_id = i; }
    void               set_taid(Xid i) { cur_table_id = i; }
    void               starred_tag() { is_tag_starred = true; }
    void               check_for_eqnum(subtypes type, bool multi);

private:
    void set_label(std::string s) {
        label_val  = std::move(s);
        seen_label = true;
    }
};

// This is a global object for math handling
class MathDataP {
    static const int                                      m_offset = 10000;
    std::array<Xml *, last_math_loc>                      built_in_table{};     // the static math table
    std::array<Xml *, last_math_loc>                      built_in_table_alt{}; // the static math table
    std::vector<Xml *>                                    xml_math_table;       // the dynamic math table
    size_t                                                xmath_pos{};          // number of slots used in the dynamic table
    std::vector<Math>                                     math_table;           // the table of math lists
    size_t                                                lmath_pos{};          // number of slots used in the math table
    std::array<std::optional<std::string>, del_tablesize> xml_lr_ptable;        // table of fence attributes
    std::array<math_types, nb_mathchars>                  math_char_type{};     // the math type for +, = etc
    std::array<Xml *, nb_simplemath>                      simplemath_table{};   // translation of $x$ etc
    std::array<Xml *, 29>                                 mc_table{};
    bool                                                  no_ent_names{};
    Token                                                 nomathsw0; // says next token is for nomathml only
    Token                                                 nomathsw1; // says next token is for normal mode only
private:
    void boot_table();
    void boot2();
    void boot_chars();
    void boot_xml_lr_tables();
    auto mk_gen(String name, String ent, String ent2, math_loc pos, const std::string &bl, symcodes t, bool hack) -> Token;
    auto mk_gen(String name, String ent, String ent2, math_loc pos, math_loc pos2, const std::string &bl, symcodes t, bool hack) -> Token;
    void mk_ic(String name, String ent, String ent2, math_loc pos);
    void mk_icb(String name, String ent, String ent2, math_loc pos);
    void mk_oc(String name, String ent, String ent2, math_loc pos);
    void mk_oco(String name, String ent, String ent2, math_loc pos);
    void mk_ocol(String name, String ent, String ent2, math_loc pos);
    void mk_ocb(String name, String ent, String ent2, math_loc pos);
    void mk_ocr(String name, String ent, String ent2, math_loc pos);
    void mk_oc(String name, String ent, String ent2, math_loc pos, symcodes t, bool hack);
    void mk_moo(String name, String ent, math_loc pos);
    void mk_accent(String name, String ent, String ent2, subtypes pos);
    void fill_lr(size_t a, String b, String c);
    void fill_lr(size_t a, String b);
    auto init_builtin(String name, math_loc pos, Xml *x, symcodes t) -> Token;

public:
    void        boot();
    void        realloc_list0();
    void        realloc_list();
    auto        find_math_location(math_list_type c, subtypes n, std::string s) -> subtypes;
    auto        find_xml_location() -> subtypes;
    auto        find_xml_location(Xml *y) -> subtypes;
    auto        make_mfenced(size_t open, size_t close, gsl::not_null<Xml *> val) -> gsl::not_null<Xml *>;
    static auto add_style(int lvl, gsl::not_null<Xml *> res) -> gsl::not_null<Xml *>;
    void        TM_mk(String a, String b, math_types c);
    void        finish_math_mem();
    auto        get_mc_table(size_t i) { return gsl::not_null{mc_table[i]}; }
    auto        get_builtin(size_t p) { return gsl::not_null{built_in_table[p]}; }
    auto        get_builtin_alt(size_t p) -> Xml * { return built_in_table_alt[p]; }
    void        init_builtin(size_t i, Xml *X) { built_in_table[i] = X; }
    void        init_builtin(size_t i, size_t j) { built_in_table[i] = built_in_table[j]; }
    void        init_builtin(size_t i, Buffer &B) { built_in_table[i] = new Xml(B); }
    auto        get_xml_val(size_t i) -> Xml * {
        if (i < m_offset) return built_in_table[i];
        return xml_math_table[i - m_offset];
    }
    auto        get_list(size_t k) -> Math & { return math_table[k]; }
    void        push_back(size_t k, CmdChr X, subtypes c);
    auto        get_simplemath_val(size_t i) -> Xml * { return simplemath_table[i]; }
    auto        get_fence(size_t k) -> std::optional<std::string> { return xml_lr_ptable[k]; }
    auto        get_math_char_type(size_t i) -> math_types { return math_char_type[i]; }
    static auto mk_mo(String a) -> gsl::not_null<Xml *>;
    void        set_type(size_t k, math_list_type c) { math_table[k].set_type(c); }
};

inline MathDataP math_data;

class Cv3Helper {
    Math               res;
    Xml *              p{};
    int                ploc{};
    math_types         ptype{};
    math_types         prefix_type{};
    Xml *              index{}, *exponent{};
    Math               object;
    int                special; // Sum or product
    void               pop_front() { object.pop_front(); }
    [[nodiscard]] auto get_cmd() const -> symcodes { return object.front().cmd; }
    [[nodiscard]] auto get_chr() const -> subtypes { return object.front().chr; }
    auto               get_xmltype() -> math_types { return object.front().get_xmltype(); }
    auto               empty() -> bool { return object.empty(); }

public:
    int state{};
    Cv3Helper(Math X) : object(std::move(X)), special(0) {}
    void reinit();
    void non_script();
    void find_kernel();
    void find_index(math_style cms);
    void add_kernel(math_style cms);
    auto find_operator(math_style cms) -> std::string;
    auto finish() -> Math { return res; }
};

namespace math_ns {
    void add_attribute_spec(const std::string &a, const std::string &b);
    auto cv_special_string(int c) -> std::string;
    auto get_builtin(size_t p) -> Xml *;
    auto get_builtin_alt(size_t p) -> Xml *;
    auto get_delimiter(CmdChr X) -> del_pos;
    auto get_delimiter(int k) -> del_pos;
    auto handle_hspace(Buffer &B) -> Xml *;
    auto handle_space(Buffer &) -> Xml *;
    void insert_delimiter(del_pos k);
    void insert_delimiter_t(del_pos k);
    auto math_constants(int c) -> Xml *;
    auto math_space_code(int c) -> bool;
    auto make_sup(Xml *xval) -> Xml *;
    auto mk_mi(char32_t c) -> Xml *;
    auto mk_mi(uchar c, size_t font) -> Xml *;
    auto mk_space(const std::string &a) -> Xml *;
    auto nb_args_for_cmd(int c) -> int;
    void fill_math_char_slots();
    void fill_math_char_slots_ent();
    void fill_single_char();
    auto next_math_style(math_style x) -> math_style;
    auto next_frac_style(math_style x) -> math_style;
    auto special_exponent(const_math_iterator L, const_math_iterator E) -> Xml *;
    auto special_fence(subtypes s, size_t &open, size_t &close) -> bool; // \todo return a pair?
    auto style_level(subtypes tt) -> math_style;
    auto make_math_char(uchar c, size_t n) -> Xml *;
    auto xml2sons(std::string elt, gsl::not_null<Xml *> first_arg, gsl::not_null<Xml *> second_arg) -> Xml;
} // namespace math_ns

//---------------------------------------------------------------------
//  Some inline functions

inline auto Math::has_two_elements() const -> bool {
    auto X = begin();
    if (X == end()) return false;
    ++X;
    return X != end();
}

inline auto Math::has_one_element() const -> bool {
    auto X = begin();
    if (X == end()) return false;
    ++X;
    return X == end();
}

inline auto Math::second_element() const -> const MathElt & {
    auto X = begin();
    ++X;
    return *X;
}

inline auto Math::third_element() const -> const MathElt & {
    auto X = begin();
    ++X;
    ++X;
    return *X;
}
