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

    if (sa.starts_with("elt_")) return config_ns::assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return config_ns::assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
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

// Handles names starting with xml_ \todo move away from this file
auto config_ns::assign_name(const std::string &A, std::string B) -> bool {
    if (A == "accueil") {
        the_names.set(np_accueil, Istring(B));
        return true;
    }
    if (A == "arc") {
        the_names.set(np_arc, Istring(B));
        return true;
    }
    if (A == "allowbreak") {
        the_names.set(np_allowbreak, Istring(B));
        return true;
    }
    if (A == "anchor") {
        the_names.set(np_anchor, Istring(B));
        return true;
    }
    if (A == "anodeconnect") {
        the_names.set(np_anodeconnect, Istring(B));
        return true;
    }
    if (A == "abarnodeconnect") {
        the_names.set(np_abarnodeconnect, Istring(B));
        return true;
    }
    if (A == "anodecurve") {
        the_names.set(np_anodecurve, Istring(B));
        return true;
    }
    if (A == "alt_caption") {
        the_names.set(np_alt_caption, Istring(B));
        return true;
    }
    if (A == "alt_section") {
        the_names.set(np_alt_section, Istring(B));
        return true;
    }
    if (A == "alternatives") {
        the_names.set(np_alternatives, Istring(B));
        return true;
    }
    if (A == "box") {
        the_names.set(np_box, Istring(B));
        return true;
    }
    if (A == "bezier") {
        the_names.set(np_bezier, Istring(B));
        return true;
    }
    if (A == "bigcircle") {
        the_names.set(np_bigcircle, Istring(B));
        return true;
    }
    if (A == "backmatter") {
        the_names.set(np_backmatter, Istring(B));
        return true;
    }
    if (A == "biblio") {
        the_names.set(np_biblio, Istring(B));
        return true;
    }
    if (A == "bpers") {
        the_names.set(np_bpers, Istring(B));
        return true;
    }
    if (A == "barnodeconnect") {
        the_names.set(np_barnodeconnect, Istring(B));
        return true;
    }
    if (A == "bibitem") {
        the_names.set(np_bibitem, Istring(B));
        return true;
    }
    if (A == "bibkey") {
        the_names.set(np_bibkey, Istring(B));
        return true;
    }
    if (A == "cell") {
        the_names.set(np_cell, Istring(B));
        return true;
    }
    if (A == "caption") {
        the_names.set(np_captions, Istring(B));
        return true;
    }
    if (A == "circle") {
        the_names.set(np_circle, Istring(B));
        return true;
    }
    if (A == "closecurve") {
        the_names.set(np_closecurve, Istring(B));
        return true;
    }
    if (A == "curve") {
        the_names.set(np_curve, Istring(B));
        return true;
    }
    if (A == "catperso") {
        the_names.set(np_catperso, Istring(B));
        return true;
    }
    if (A == "composition_ra") {
        the_names.set(cst_composition, Istring(B));
        return true;
    }
    if (A == "cleaders") {
        the_names.set(np_cleaders, Istring(B));
        return true;
    }
    if (A == "caps") {
        the_names.set(np_s_caps, Istring(B));
        return true;
    }
    if (A == "cit") {
        the_names.set(np_cit, Istring(B));
        return true;
    }
    if (A == "citation") {
        the_names.set(np_citation, Istring(B));
        return true;
    }
    if (A == "citetype") {
        the_names.set(np_cite_type, Istring(B));
        return true;
    }
    if (A == "dashline") {
        the_names.set(np_dashline, Istring(B));
        return true;
    }
    if (A == "div0") {
        the_names.set(np_div0, Istring(B));
        return true;
    }
    if (A == "div1") {
        the_names.set(np_div1, Istring(B));
        return true;
    }
    if (A == "div2") {
        the_names.set(np_div2, Istring(B));
        return true;
    }
    if (A == "div3") {
        the_names.set(np_div3, Istring(B));
        return true;
    }
    if (A == "div4") {
        the_names.set(np_div4, Istring(B));
        return true;
    }
    if (A == "div5") {
        the_names.set(np_div5, Istring(B));
        return true;
    }
    if (A == "div6") {
        the_names.set(np_div6, Istring(B));
        return true;
    }
    if (A == "drawline") {
        the_names.set(np_drawline, Istring(B));
        return true;
    }
    if (A == "dottedline") {
        the_names.set(np_dottedline, Istring(B));
        return true;
    }
    if (A == "eqnpos") {
        the_names.set(np_eqnpos, Istring(B));
        return true;
    }
    if (A == "footnote") {
        the_names.set(np_footnote, Istring(B));
        return true;
    }
    if (A == "formula") {
        the_names.set(np_formula, Istring(B));
        return true;
    }
    if (A == "fbox") {
        the_names.set(np_fbox, Istring(B));
        return true;
    }
    if (A == "figure") {
        the_names.set(np_figure, Istring(B));
        return true;
    }
    if (A == "figure_env") {
        the_names.set(np_float_figure, Istring(B));
        return true;
    }
    if (A == "frontmatter") {
        the_names.set(np_frontmatter, Istring(B));
        return true;
    }
    if (A == "font_small") {
        the_names.set(np_font_small, Istring(B));
        return true;
    }
    if (A == "font_small1") {
        the_names.set(np_font_small1, Istring(B));
        return true;
    }
    if (A == "font_small2") {
        the_names.set(np_font_small2, Istring(B));
        return true;
    }
    if (A == "font_small3") {
        the_names.set(np_font_small3, Istring(B));
        return true;
    }
    if (A == "font_small4") {
        the_names.set(np_font_small4, Istring(B));
        return true;
    }
    if (A == "font_small5") {
        the_names.set(np_font_small5, Istring(B));
        return true;
    }
    if (A == "font_small6") {
        the_names.set(np_font_small6, Istring(B));
        return true;
    }
    if (A == "font_large") {
        the_names.set(np_font_large, Istring(B));
        return true;
    }
    if (A == "font_large1") {
        the_names.set(np_font_large1, Istring(B));
        return true;
    }
    if (A == "font_large2") {
        the_names.set(np_font_large2, Istring(B));
        return true;
    }
    if (A == "font_large3") {
        the_names.set(np_font_large3, Istring(B));
        return true;
    }
    if (A == "font_large4") {
        the_names.set(np_font_large4, Istring(B));
        return true;
    }
    if (A == "font_large5") {
        the_names.set(np_font_large5, Istring(B));
        return true;
    }
    if (A == "font_large6") {
        the_names.set(np_font_large6, Istring(B));
        return true;
    }
    if (A == "font_normalsize") {
        the_names.set(np_font_normalsize, Istring(B));
        return true;
    }
    if (A == "font_upright") {
        the_names.set(np_font_upright, Istring(B));
        return true;
    }
    if (A == "font_medium") {
        the_names.set(np_font_medium, Istring(B));
        return true;
    }
    if (A == "font_roman") {
        the_names.set(np_font_roman, Istring(B));
        return true;
    }
    if (A == "font_it") {
        the_names.set(np_font_it, Istring(B));
        return true;
    }
    if (A == "font_slanted") {
        the_names.set(np_font_slanted, Istring(B));
        return true;
    }
    if (A == "font_sc") {
        the_names.set(np_font_sc, Istring(B));
        return true;
    }
    if (A == "font_tt") {
        the_names.set(np_font_tt, Istring(B));
        return true;
    }
    if (A == "font_sansserif") {
        the_names.set(np_font_sansserif, Istring(B));
        return true;
    }
    if (A == "font_bold") {
        the_names.set(np_font_bold, Istring(B));
        return true;
    }
    if (A == "font_boldextended") {
        the_names.set(np_font_boldextended, Istring(B));
        return true;
    }
    if (A == "font_semibold") {
        the_names.set(np_font_semibold, Istring(B));
        return true;
    }
    if (A == "font_condensed") {
        the_names.set(np_font_condensed, Istring(B));
        return true;
    }
    if (A == "gloitem") {
        the_names.set(np_label_glo, Istring(B));
        return true;
    }
    if (A == "graphics") {
        the_names.set(np_graphics, Istring(B));
        return true;
    }
    if (A == "glo") {
        the_names.set(np_glo_name, Istring(B));
        return true;
    }
    if (A == "glossary") {
        the_names.set(np_glossary, Istring(B));
        return true;
    }
    if (A == "head") {
        the_names.set(np_head, Istring(B));
        return true;
    }
    if (A == "hl") {
        the_names.set(np_s_hl, Istring(B));
        return true;
    }
    if (A == "item") {
        the_names.set(np_item, Istring(B));
        return true;
    }
    if (A == "index") {
        the_names.set(np_index, Istring(B));
        return true;
    }
    if (A == "keywords") {
        the_names.set(np_keywords, Istring(B));
        return true;
    }
    if (A == "labelitem") {
        the_names.set(np_label_item, Istring(B));
        return true;
    }
    if (A == "lineC") {
        the_names.set(np_lineC, Istring(B));
        return true;
    }
    if (A == "line") {
        the_names.set(np_line, Istring(B));
        return true;
    }
    if (A == "listoffigures") {
        the_names.set(np_toc2, Istring(B));
        return true;
    }
    if (A == "listoftables") {
        the_names.set(np_toc1, Istring(B));
        return true;
    }
    if (A == "llap") {
        the_names.set(np_llap, Istring(B));
        return true;
    }
    if (A == "linethickness") {
        the_names.set(np_line_thickness, Istring(B));
        return true;
    }
    if (A == "list") {
        the_names.set(np_list, Istring(B));
        return true;
    }
    if (A == "leaders") {
        the_names.set(np_leaders, Istring(B));
        return true;
    }
    if (A == "leg") {
        the_names.set(np_leg, Istring(B));
        return true;
    }
    if (A == "mbox") {
        the_names.set(np_mbox, Istring(B));
        return true;
    }
    if (A == "math") {
        the_names.set(cst_math, Istring(B));
        return true;
    }
    if (A == "multiput") {
        the_names.set(np_multiput, Istring(B));
        return true;
    }
    if (A == "mainmatter") {
        the_names.set(np_mainmatter, Istring(B));
        return true;
    }
    if (A == "node") {
        the_names.set(np_node, Istring(B));
        return true;
    }
    if (A == "nodeconnect") {
        the_names.set(np_nodeconnect, Istring(B));
        return true;
    }
    if (A == "nodecurve") {
        the_names.set(np_nodecurve, Istring(B));
        return true;
    }
    if (A == "nodetriangle") {
        the_names.set(np_nodetriangle, Istring(B));
        return true;
    }
    if (A == "nodecircle") {
        the_names.set(np_nodecircle, Istring(B));
        return true;
    }
    if (A == "nodebox") {
        the_names.set(np_nodebox, Istring(B));
        return true;
    }
    if (A == "nodeoval") {
        the_names.set(np_nodeoval, Istring(B));
        return true;
    }
    if (A == "natcit") {
        the_names.set(np_natcit, Istring(B));
        return true;
    }
    if (A == "oval") {
        the_names.set(np_oval, Istring(B));
        return true;
    }
    if (A == "oldstyle") {
        the_names.set(np_s_old, Istring(B));
        return true;
    }
    if (A == "overline") {
        the_names.set(np_overline, Istring(B));
        return true;
    }
    if (A == "picture") {
        the_names.set(np_picture, Istring(B));
        return true;
    }
    if (A == "put") {
        the_names.set(np_put, Istring(B));
        return true;
    }
    if (A == "project") {
        the_names.set(np_projet, Istring(B));
        return true;
    }
    if (A == "pers") {
        the_names.set(np_pers, Istring(B));
        return true;
    }
    if (A == "prenote") {
        the_names.set(np_prenote, Istring(B));
        return true;
    }
    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return true;
    }
    if (A == "row") {
        the_names.set(np_row, Istring(B));
        return true;
    }
    if (A == "raisebox") {
        the_names.set(np_raisebox, Istring(B));
        return true;
    }
    if (A == "rlap") {
        the_names.set(np_rlap, Istring(B));
        return true;
    }
    if (A == "rotatebox") {
        the_names.set(np_rotatebox, Istring(B));
        return true;
    }
    if (A == "ref") {
        the_names.set(np_ref, Istring(B));
        return true;
    }
    if ((A == "rclist") && ra_ok) {
        the_names.set(np_rclist, Istring(B));
        return true;
    }
    if ((A == "rcval") && ra_ok) {
        the_names.set(np_rcval, Istring(B));
        return true;
    }
    if ((A == "rasection") && ra_ok) {
        the_names.set(np_rasection, Istring(B));
        return true;
    }
    if (A == "subfigure") {
        the_names.set(np_subfigure, Istring(B));
        return true;
    }
    if (A == "scaleput") {
        the_names.set(np_scaleput, Istring(B));
        return true;
    }
    if (A == "scalebox") {
        the_names.set(np_sbox, Istring(B));
        return true;
    }
    if (A == "scaption") {
        the_names.set(np_caption, Istring(B));
        return true;
    }
    if (A == "sup") {
        the_names.set(np_s_sup, Istring(B));
        return true;
    }
    if (A == "sub") {
        the_names.set(np_s_sub, Istring(B));
        return true;
    }
    if (A == "so") {
        the_names.set(np_s_so, Istring(B));
        return true;
    }
    if (A == "st") {
        the_names.set(np_s_st, Istring(B));
        return true;
    }
    if (A == "stylesheet") {
        the_names.set(np_stylesheet, Istring(B));
        return true;
    }
    if (A == "stylesheettype") {
        the_names.set(np_stylesheet_type, Istring(B));
        return true;
    }
    if (A == "term") {
        the_names.set(np_term, Istring(B));
        return true;
    }
    if (A == "texmath") {
        the_names.set(np_texmath, Istring(B));
        return true;
    }
    if (A == "table") {
        the_names.set(np_table, Istring(B));
        return true;
    }
    if (A == "table_env") {
        the_names.set(np_float_table, Istring(B));
        return true;
    }
    if (A == "Table") {
        the_names.set(np_Table, Istring(B));
        return true;
    }
    if (A == "tagcurve") {
        the_names.set(np_tagcurve, Istring(B));
        return true;
    }
    if (A == "thicklines") {
        the_names.set(np_thick_lines, Istring(B));
        return true;
    }
    if (A == "thinlines") {
        the_names.set(np_thin_lines, Istring(B));
        return true;
    }
    if (A == "theorem_head") {
        the_names.set(np_alt_head, Istring(B));
        return true;
    }
    if (A == "theorem") {
        if (B[0] == 0) { // reverst to old behavior
            the_names.set(np_theorem, Istring(B));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else if (B[0] == ' ') { // special case
            the_names.set(np_theorem, Istring(B.substr(1)));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else {
            the_names.set(np_theorem, Istring(B));
            the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
        }
        return true;
    }
    if (A == "theindex") {
        the_names.set(np_theindex, Istring(B));
        return true;
    }
    if (A == "theglossary") {
        the_names.set(np_theglossary, Istring(B));
        return true;
    }
    if (A == "tableofcontents") {
        the_names.set(np_toc, Istring(B));
        return true;
    }
    if (A == "texte") {
        the_names.set(np_texte, Istring(B));
        return true;
    }
    if (A == "ul") {
        the_names.set(np_s_ul, Istring(B));
        return true;
    }
    if (A == "underline") {
        the_names.set(np_underline, Istring(B));
        return true;
    }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return true;
    }
    if (A == "vector") {
        the_names.set(np_vector, Istring(B));
        return true;
    }
    if (A == "warning") {
        the_names.set(np_warning, Istring(B));
        return true;
    }
    if (A == "xref") {
        the_names.set(np_xref, Istring(B));
        return true;
    }
    if (A == "xtheorem") {
        the_names.set(np_theorem, Istring(B));
        return true;
    }
    if (A == "xleaders") {
        the_names.set(np_xleaders, Istring(B));
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
