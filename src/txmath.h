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
