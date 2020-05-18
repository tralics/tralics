// tralics, a LaTeX to XML translator
// Copyright INRIA (Jose Grimm) 2002, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// InterDepositDigitalNumber = IDDN.FR.001.510030.000.S.P.2002.000.31235
// for version 1.6, dated 05/12/2002.
// InterDepositDigitalNumber = IDDN.FR.001.510030.001.S.P.2002.000.31235
// for version 2.9.4, dated 18/02/2007.

// The file contains the main data structures, and code to fill the tables.

#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <ctime>
#include <fmt/format.h>

namespace main_ns {
    int  nb_errs         = 0;     // number of errors seen so far
    bool no_xml_error    = false; // should an error produce an XML element ?
    bool use_quotes      = false; // How to translate quotes
    bool bib_allow_break = true;  // <allowbreak> in bibliography
} // namespace main_ns

MainClass *the_main;     // the main class data structure
bool       ra_ok = true; // inhibits  redefinitions

// This is the main function. It does the following: boot the parser and the
// math part, and execute the main prog.
auto main(int argc, char **argv) -> int {
    spdlog::info("This is tralics {}, a LaTeX to XML translator", tralics_version);
    MainClass M;
    the_main    = &M;
    M.the_stack = &the_parser.the_stack;
    spdlog::info("Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm");
    the_parser.boot(); // create the hash table and all that
    spdlog::info("Licensed under the CeCILL Free Software Licensing Agreement");
    M.run(argc, argv);
    return main_ns::nb_errs == 0 ? 0 : 2;
}

// Function called when A=B is seen in the configuration file.
// Returns true if A is recognised
auto assign(const std::string &sa, const std::string &sb) -> bool {
    auto n = sa.size();

    if (sa.starts_with("elt_")) return the_names.assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return the_names.assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
    if (sa.starts_with("att_")) return config_ns::assign_att(sa.substr(4), sb);

    if (sa == "lang_fr") {
        the_names.set(np_french, Istring(sb));
        return true;
    }
    if (sa == "lang_en") {
        the_names.set(np_english, Istring(sb));
        return true;
    }
    if (sa == "distinguish_refer_in_rabib") {
        if ((sb == "true") || (sb == "yes"))
            the_main->distinguish_refer = true;
        else if ((sb == "false") || (sb == "no"))
            the_main->distinguish_refer = false;
        else
            spdlog::warn("distinguish_refer_in_rabib = {} ignored");
        return true;
    }
    if (sa == "entity_names") {
        the_main->set_ent_names(sb);
        return true;
    }
    if (sa == "default_class") {
        the_main->default_class = sb;
        return true;
    }
    if (sa == "alternate_item") {
        if (sb == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (sb == "true")
            the_parser.hash_table.eval_let("item", "@item");
        return true;
    }
    if (sa == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << sb << "}";
        the_main->add_to_from_config(1, aux); // \todo no Buffer
        return true;
    }
    if (sa == "everyjob") {
        everyjob_string = fmt::format("\\everyjob={{{}}}", sb);
        return true;
    }
    if (sa == "no_footnote_hack") {
        if (sb == "true") the_main->footnote_hack = false;
        if (sb == "false") the_main->footnote_hack = true;
        return true;
    }
    if (sa == "use_font_elt") {
        if (sb == "true") the_main->use_font_elt = true;
        if (sb == "false") the_main->use_font_elt = false;
        return true;
    }
    if (sa == "use_all_sizes") {
        if (sb == "true") the_main->use_all_sizes = true;
        if (sb == "false") the_main->use_all_sizes = false;
        return true;
    }
    if (sa == "bibtex_fields") {
        Buffer(sb).interpret_bibtex_list(); // \todo without Buffer
        return false;
    }
    if (sa == "bibtex_extensions") {
        Buffer(sb).interpret_bibtex_extension_list();
        return false;
    }
    if (sa == "mfenced_separator_val") {
        if (sb == "NONE")
            the_names.set(np_separator, Istring());
        else
            the_names.set(np_separator, Istring(sb));
        return true;
    }
    if (sa.ends_with("_vals")) {
        Buffer B(sb);
        config_ns::interpret_list(sa.substr(0, n - 5), B); // \todo without Buffer
        return true;
    }
    if (sa == "mml_font_normal") {
        the_names.set(np_mml_normal, Istring(sb));
        return true;
    }
    if (sa == "mml_font_upright") {
        the_names.set(np_mml_upright, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold") {
        the_names.set(np_mml_bold, Istring(sb));
        return true;
    }
    if (sa == "mml_font_italic") {
        the_names.set(np_mml_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_italic") {
        the_names.set(np_mml_bold_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_script") {
        the_names.set(np_mml_script, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_script") {
        the_names.set(np_mml_bold_script, Istring(sb));
        return true;
    }
    if (sa == "mml_font_fraktur") {
        the_names.set(np_mml_fraktur, Istring(sb));
        return true;
    }
    if (sa == "mml_font_doublestruck") {
        the_names.set(np_mml_doublestruck, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_fraktur") {
        the_names.set(np_mml_bold_fraktur, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif") {
        the_names.set(np_mml_sansserif, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_sansserif") {
        the_names.set(np_mml_bold_sansserif, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif_italic") {
        the_names.set(np_mml_sansserif_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif_bold_italic") {
        the_names.set(np_mml_sansserif_bold_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_monospace") {
        the_names.set(np_mml_monospace, Istring(sb));
        return true;
    }
    return false;
}

// Handles names starting with att_
auto config_ns::assign_att(const std::string &A, const std::string &B) -> bool {
    if (A == "angle") {
        the_names.set(np_angle, Istring(B));
        return true;
    }
    if (A == "affiliation") {
        the_names.set(np_affiliation, Istring(B));
        return true;
    }
    if (A == "box_pos") {
        the_names.set(np_box_pos, Istring(B));
        return true;
    }
    if (A == "box_scale") {
        the_names.set(np_s_scale, Istring(B));
        return true;
    }
    if (A == "box_width") {
        the_names.set(np_box_width, Istring(B));
        return true;
    }
    if (A == "boxed") {
        the_names.set(np_boxed, Istring(B));
        return true;
    }
    if (A == "bibkey") {
        the_names.set(np_bibkey, Istring(B));
        return true;
    }
    if (A == "border_bottom_width") {
        the_names.set(np_border_bottomw, Istring(B));
        return true;
    }
    if (A == "border_top_width") {
        the_names.set(np_border_topw, Istring(B));
        return true;
    }
    if (A == "border_left_width") {
        the_names.set(np_border_leftw, Istring(B));
        return true;
    }
    if (A == "border_right_width") {
        the_names.set(np_border_rightw, Istring(B));
        return true;
    }
    if (A == "centering") {
        the_names.set(np_center_etc1, Istring(B));
        return true;
    }
    if (A == "clip") {
        the_names.set(np_clip, Istring(B));
        return true;
    }
    if (A == "cols") {
        the_names.set(np_cols, Istring(B));
        return true;
    }
    if (A == "cell_left") {
        the_names.set(np_c_left, Istring(B));
        return true;
    }
    if (A == "cell_right") {
        the_names.set(np_c_right, Istring(B));
        return true;
    }
    if (A == "cell_center") {
        the_names.set(np_c_center, Istring(B));
        return true;
    }
    if (A == "cell_leftborder") {
        the_names.set(np_leftborder, Istring(B));
        return true;
    }
    if (A == "cell_rightborder") {
        the_names.set(np_rightborder, Istring(B));
        return true;
    }
    if (A == "cell_topborder") {
        the_names.set(np_topborder, Istring(B));
        return true;
    }
    if (A == "cell_bottomborder") {
        the_names.set(np_bottomborder, Istring(B));
        return true;
    }
    if (A == "curve_nbpts") {
        the_names.set(np_curve_nbpts, Istring(B));
        return true;
    }
    if (A == "display") {
        the_names.set(np_display, Istring(B));
        return true;
    }
    if (A == "dx") {
        the_names.set(np_dx, Istring(B));
        return true;
    }
    if (A == "dy") {
        the_names.set(np_dy, Istring(B));
        return true;
    }
    if (A == "depthA") {
        the_names.set(np_depthA, Istring(B));
        return true;
    }
    if (A == "depthB") {
        the_names.set(np_depthB, Istring(B));
        return true;
    }
    if (A == "depth") {
        the_names.set(np_depth, Istring(B));
        return true;
    }
    if (A == "encap") {
        the_names.set(np_encap, Istring(B));
        return true;
    }
    if (A == "framed") {
        the_names.set(np_framed, Istring(B));
        return true;
    }
    if (A == "full") {
        the_names.set(np_full, Istring(B));
        return true;
    }
    if (A == "file") {
        the_names.set(np_file, Istring(B));
        return true;
    }
    if (A == "file_extension") {
        the_names.set(np_fileextension, Istring(B));
        return true;
    }
    if (A == "fbox_rend") {
        the_names.set(np_b_rend, Istring(B));
        return true;
    }
    if (A == "flush_left") {
        the_names.set(np_center_etc4, Istring(B));
        return true;
    }
    if (A == "flush_right") {
        the_names.set(np_center_etc5, Istring(B));
        return true;
    }
    if (A == "foot_position") {
        the_names.set(np_foot, Istring(B));
        return true;
    }
    if (A == "full_first") {
        the_names.set(np_full_first, Istring(B));
        return true;
    }
    if (A == "gloss_type") {
        the_names.set(np_gloss, Istring(B));
        return true;
    }
    if (A == "height") {
        the_names.set(np_height, Istring(B));
        return true;
    }
    if (A == "halign") {
        the_names.set(np_halign, Istring(B));
        return true;
    }
    if (A == "hdr") {
        the_names.set(np_hdr, Istring(B));
        return true;
    }
    if (A == "inner_pos") {
        the_names.set(np_posi, Istring(B));
        return true;
    }
    if (A == "inline") {
        the_names.set(np_inline, Istring(B));
        return true;
    }
    if (A == "junior") {
        the_names.set(np_junior, Istring(B));
        return true;
    }
    if (A == "language") {
        the_names.set(np_language, Istring(B));
        return true;
    }
    if (A == "level") {
        the_names.set(np_level, Istring(B));
        return true;
    }
    if (A == "minipage_width") {
        the_names.set(np_minipage_width, Istring(B));
        return true;
    }
    if (A == "marginpar") {
        the_names.set(np_marginpar, Istring(B));
        return true;
    }
    if (A == "mathmlns") {
        the_names.set(cst_mathml, Istring(B));
        return true;
    }
    if (A == "marginpar") {
        the_names.set(np_marginpar, Istring(B));
        return true;
    }
    if (A == "mode") {
        the_names.set(cst_mode, Istring(B));
        return true;
    }
    if (A == "noindent") {
        the_names.set(np_noindent, Istring(B));
        return true;
    }
    if (A == "nonumber") {
        the_names.set(np_nonumber, Istring(B));
        return true;
    }
    if (A == "nom") {
        the_names.set(np_nom, Istring(B));
        return true;
    }
    if (A == "name") {
        the_names.set(np_name, Istring(B));
        return true;
    }
    if (A == "nameA") {
        the_names.set(np_nameA, Istring(B));
        return true;
    }
    if (A == "nameB") {
        the_names.set(np_nameB, Istring(B));
        return true;
    }
    if (A == "place") {
        the_names.set(np_place, Istring(B));
        return true;
    }
    if (A == "prenom") {
        the_names.set(np_prenom, Istring(B));
        return true;
    }
    if (A == "pre") {
        the_names.set(np_pre, Istring(B));
        return true;
    }
    if (A == "particule") {
        the_names.set(np_particle, Istring(B));
        return true;
    }
    if (A == "page") {
        the_names.set(np_page, Istring(B));
        return true;
    }
    if (A == "profession") {
        the_names.set(np_profession, Istring(B));
        return true;
    }
    if (A == "posA") {
        the_names.set(np_posA, Istring(B));
        return true;
    }
    if (A == "posB") {
        the_names.set(np_posB, Istring(B));
        return true;
    }
    if (A == "pos") {
        the_names.set(np_pos, Istring(B));
        return true;
    }
    if (A == "quote") {
        the_names.set(np_center_etc2, Istring(B));
        return true;
    }
    if (A == "quotation") {
        the_names.set(np_center_etc3, Istring(B));
        return true;
    }
    if (A == "rotate_angle") {
        the_names.set(np_r_angle, Istring(B));
        return true;
    }
    if (A == "rend") {
        the_names.set(np_rend, Istring(B));
        return true;
    }
    if (A == "row_spaceafter") {
        the_names.set(np_spaceafter, Istring(B));
        return true;
    }
    if (A == "repeat") {
        the_names.set(np_repeat, Istring(B));
        return true;
    }
    if (A == "scale") {
        the_names.set(np_scale, Istring(B));
        return true;
    }
    if (A == "space_before") {
        the_names.set(np_spacebefore, Istring(B));
        return true;
    }
    if (A == "size") {
        the_names.set(np_size, Istring(B));
        return true;
    }
    if (A == "starred") {
        the_names.set(np_starred, Istring(B));
        return true;
    }
    if (A == "table_width") {
        the_names.set(np_tab_width, Istring(B));
        return true;
    }
    if (A == "type") {
        the_names.set(np_type, Istring(B));
        return true;
    }
    if (A == "textype") {
        the_names.set(np_textype, Istring(B));
        return true;
    }
    if (A == "unit_length") {
        the_names.set(np_unit_length, Istring(B));
        return true;
    }
    if (A == "user_list") {
        the_names.set(np_user_list, Istring(B));
        return true;
    }
    if (A == "vpos") {
        the_names.set(np_vpos, Istring(B));
        return true;
    }
    if (A == "verse") {
        the_names.set(np_center_etc6, Istring(B));
        return true;
    }
    if (A == "width") {
        the_names.set(np_width, Istring(B));
        return true;
    }
    if (A == "xscale") {
        the_names.set(np_xscale, Istring(B));
        return true;
    }
    if (A == "xscaley") {
        the_names.set(np_xscaley, Istring(B));
        return true;
    }
    if (A == "xpos") {
        the_names.set(np_xpos, Istring(B));
        return true;
    }
    if (A == "xdir") {
        the_names.set(np_xdir, Istring(B));
        return true;
    }
    if (A == "yscale") {
        the_names.set(np_yscale, Istring(B));
        return true;
    }
    if (A == "yscalex") {
        the_names.set(np_yscalex, Istring(B));
        return true;
    }
    if (A == "ydir") {
        the_names.set(np_ydir, Istring(B));
        return true;
    }
    if (A == "ypos") {
        the_names.set(np_ypos, Istring(B));
        return true;
    }
    return false;
}
