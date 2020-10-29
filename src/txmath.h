#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) 2003-2004 2006, 2007 INRIA, Jos\'e Grimm

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Math.h"
#include "tralics/MathElt.h"
#include "tralics/MathP.h"
#include "tralics/MathQ.h"
#include "tralics/Xml.h"
#include <spdlog/spdlog.h>
#include <vector>

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

namespace tralics_ns {
    auto math_env_name(subtypes c) -> String;
} // namespace tralics_ns

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
    void        set_type(size_t k, math_list_type c);
};

inline MathDataP math_data; // \todo unique instance, should we use static stuff?

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
    auto special_fence(subtypes s, size_t &open, size_t &close) -> bool; // \todo return a pair?
    auto style_level(subtypes tt) -> math_style;
    auto make_math_char(uchar c, size_t n) -> Xml *;
    auto xml2sons(std::string elt, gsl::not_null<Xml *> first_arg, gsl::not_null<Xml *> second_arg) -> Xml;
} // namespace math_ns

//---------------------------------------------------------------------
//  Some inline functions
