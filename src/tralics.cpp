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

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <ctime>
#include <fmt/format.h>

namespace main_ns {
    int    nb_errs = 0;             // number of errors seen so far
    Buffer path_buffer;             // Result of search_in_path is here
    bool   no_xml_error    = false; // should an error produce an XML element ?
    bool   use_quotes      = false; // How to translate quotes
    bool   bib_allow_break = true;  // <allowbreak> in bibliography
} // namespace main_ns

MainClass *the_main;     // the main class data structure
bool       ra_ok = true; // inhibits  redefinitions

// This is the main function. It does the following:
// print a banner, boot the parser and the math part,
// and execute the main prog.
auto main(int argc, char **argv) -> int {
    MainClass M;
    the_main    = &M;
    M.the_stack = &the_parser.the_stack;
    M.run(argc, argv);
    return main_ns::nb_errs == 0 ? 0 : 2;
}

// Function called when A=B is seen in the configuration file.
// Returns true if A is recognised
auto assign(Buffer &a, Buffer &b) -> bool {
    std::string aa = a.to_string(), bb = b.to_string();
    auto        n = a.size();

    if (a[0] == 'e' && a[1] == 'l' && a[2] == 't' && a[3] == '_') return config_ns::assign_name(aa.substr(4), bb);
    if (a[0] == 'x' && a[1] == 'm' && a[2] == 'l' && a[3] == '_') {
        if (a[n - 1] == 'e' && a[n - 2] == 'm' && a[n - 3] == 'a' && a[n - 4] == 'n' && a[n - 5] == '_') {
            a.at(n - 5) = 0;
            aa          = a.to_string();
        }
        return config_ns::assign_name(aa.substr(4), bb);
    }
    if (a[0] == 'a' && a[1] == 't' && a[2] == 't' && a[3] == '_') return config_ns::assign_att(a.to_string(4), bb);
    if (aa == "lang_fr") {
        the_names[np_french] = Istring(bb);
        return true;
    }
    if (aa == "lang_en") {
        the_names[np_english] = Istring(bb);
        return true;
    }
    if (aa == "distinguish_refer_in_rabib") {
        if ((bb == "true") || (bb == "yes"))
            the_main->distinguish_refer = true;
        else if ((bb == "false") || (bb == "no"))
            the_main->distinguish_refer = false;
        else
            b << bf_reset << "ignored";
        return true;
    }
    if (aa == "entity_names") {
        the_main->set_ent_names(bb);
        return true;
    }
    if (aa == "default_class") {
        the_main->default_class = b.to_string();
        return true;
    }
    if (aa == "alternate_item") {
        if (bb == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (bb == "true")
            the_parser.hash_table.eval_let("item", "@item");
        else
            a.reset();
        return true;
    }
    if (aa == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << b << "}";
        the_main->add_to_from_config(1, aux);
        return true;
    }
    if (aa == "everyjob") {
        Buffer aux;
        aux << "\\everyjob={" << b << "}";
        everyjob_string = aux.to_string();
        return true;
    }
    if (aa == "no_footnote_hack") {
        if (bb == "true") the_main->footnote_hack = false;
        if (bb == "false") the_main->footnote_hack = true;
        return true;
    }
    if (aa == "use_font_elt") {
        if (bb == "true") the_main->use_font_elt = true;
        if (bb == "false") the_main->use_font_elt = false;
        return true;
    }
    if (aa == "use_all_sizes") {
        if (bb == "true") the_main->use_all_sizes = true;
        if (bb == "false") the_main->use_all_sizes = false;
        return true;
    }
    if (aa == "bibtex_fields") {
        b.interpret_bibtex_list();
        return false;
    }
    if (aa == "bibtex_extensions") {
        b.interpret_bibtex_extension_list();
        return false;
    }
    if (aa == "mfenced_separator_val") {
        if (bb == "NONE")
            the_names[np_separator] = Istring();
        else
            the_names[np_separator] = Istring(bb);
        return true;
    }
    if (n > 5 && a[n - 5] == '_' && a[n - 4] == 'v' && a[n - 3] == 'a' && a[n - 2] == 'l' && a[n - 1] == 's') {
        a.at(n - 5) = 0;
        config_ns::interpret_list(a.to_string(), b);
        a.reset();
        return true;
    }
    if (aa == "mml_font_normal") {
        the_names[np_mml_normal] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_upright") {
        the_names[np_mml_upright] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_bold") {
        the_names[np_mml_bold] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_italic") {
        the_names[np_mml_italic] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_bold_italic") {
        the_names[np_mml_bold_italic] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_script") {
        the_names[np_mml_script] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_bold_script") {
        the_names[np_mml_bold_script] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_fraktur") {
        the_names[np_mml_fraktur] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_doublestruck") {
        the_names[np_mml_doublestruck] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_bold_fraktur") {
        the_names[np_mml_bold_fraktur] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_sansserif") {
        the_names[np_mml_sansserif] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_bold_sansserif") {
        the_names[np_mml_bold_sansserif] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_sansserif_italic") {
        the_names[np_mml_sansserif_italic] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_sansserif_bold_italic") {
        the_names[np_mml_sansserif_bold_italic] = Istring(bb);
        return true;
    }
    if (aa == "mml_font_monospace") {
        the_names[np_mml_monospace] = Istring(bb);
        return true;
    }
    return false;
}

// Handles names starting with xml_ \todo move away from this file
auto config_ns::assign_name(const std::string &A, std::string B) -> bool {
    if (A == "accueil") {
        the_names[np_accueil] = Istring(B);
        return true;
    }
    if (A == "arc") {
        the_names[np_arc] = Istring(B);
        return true;
    }
    if (A == "allowbreak") {
        the_names[np_allowbreak] = Istring(B);
        return true;
    }
    if (A == "anchor") {
        the_names[np_anchor] = Istring(B);
        return true;
    }
    if (A == "anodeconnect") {
        the_names[np_anodeconnect] = Istring(B);
        return true;
    }
    if (A == "abarnodeconnect") {
        the_names[np_abarnodeconnect] = Istring(B);
        return true;
    }
    if (A == "anodecurve") {
        the_names[np_anodecurve] = Istring(B);
        return true;
    }
    if (A == "alt_caption") {
        the_names[np_alt_caption] = Istring(B);
        return true;
    }
    if (A == "alt_section") {
        the_names[np_alt_section] = Istring(B);
        return true;
    }
    if (A == "alternatives") {
        the_names[np_alternatives] = Istring(B);
        return true;
    }
    if (A == "box") {
        the_names[np_box] = Istring(B);
        return true;
    }
    if (A == "bezier") {
        the_names[np_bezier] = Istring(B);
        return true;
    }
    if (A == "bigcircle") {
        the_names[np_bigcircle] = Istring(B);
        return true;
    }
    if (A == "backmatter") {
        the_names[np_backmatter] = Istring(B);
        return true;
    }
    if (A == "biblio") {
        the_names[np_biblio] = Istring(B);
        return true;
    }
    if (A == "bpers") {
        the_names[np_bpers] = Istring(B);
        return true;
    }
    if (A == "barnodeconnect") {
        the_names[np_barnodeconnect] = Istring(B);
        return true;
    }
    if (A == "bibitem") {
        the_names[np_bibitem] = Istring(B);
        return true;
    }
    if (A == "bibkey") {
        the_names[np_bibkey] = Istring(B);
        return true;
    }
    if (A == "cell") {
        the_names[np_cell] = Istring(B);
        return true;
    }
    if (A == "caption") {
        the_names[np_captions] = Istring(B);
        return true;
    }
    if (A == "circle") {
        the_names[np_circle] = Istring(B);
        return true;
    }
    if (A == "closecurve") {
        the_names[np_closecurve] = Istring(B);
        return true;
    }
    if (A == "curve") {
        the_names[np_curve] = Istring(B);
        return true;
    }
    if (A == "catperso") {
        the_names[np_catperso] = Istring(B);
        return true;
    }
    if (A == "composition_ra") {
        the_names[cst_composition] = Istring(B);
        return true;
    }
    if (A == "cleaders") {
        the_names[np_cleaders] = Istring(B);
        return true;
    }
    if (A == "caps") {
        the_names[np_s_caps] = Istring(B);
        return true;
    }
    if (A == "cit") {
        the_names[np_cit] = Istring(B);
        return true;
    }
    if (A == "citation") {
        the_names[np_citation] = Istring(B);
        return true;
    }
    if (A == "citetype") {
        the_names[np_cite_type] = Istring(B);
        return true;
    }
    if (A == "dashline") {
        the_names[np_dashline] = Istring(B);
        return true;
    }
    if (A == "div0") {
        the_names[np_div0] = Istring(B);
        return true;
    }
    if (A == "div1") {
        the_names[np_div1] = Istring(B);
        return true;
    }
    if (A == "div2") {
        the_names[np_div2] = Istring(B);
        return true;
    }
    if (A == "div3") {
        the_names[np_div3] = Istring(B);
        return true;
    }
    if (A == "div4") {
        the_names[np_div4] = Istring(B);
        return true;
    }
    if (A == "div5") {
        the_names[np_div5] = Istring(B);
        return true;
    }
    if (A == "div6") {
        the_names[np_div6] = Istring(B);
        return true;
    }
    if (A == "drawline") {
        the_names[np_drawline] = Istring(B);
        return true;
    }
    if (A == "dottedline") {
        the_names[np_dottedline] = Istring(B);
        return true;
    }
    if (A == "eqnpos") {
        the_names[np_eqnpos] = Istring(B);
        return true;
    }
    if (A == "footnote") {
        the_names[np_footnote] = Istring(B);
        return true;
    }
    if (A == "formula") {
        the_names[np_formula] = Istring(B);
        return true;
    }
    if (A == "fbox") {
        the_names[np_fbox] = Istring(B);
        return true;
    }
    if (A == "figure") {
        the_names[np_figure] = Istring(B);
        return true;
    }
    if (A == "figure_env") {
        the_names[np_float_figure] = Istring(B);
        return true;
    }
    if (A == "frontmatter") {
        the_names[np_frontmatter] = Istring(B);
        return true;
    }
    if (A == "font_small") {
        the_names[np_font_small] = Istring(B);
        return true;
    }
    if (A == "font_small1") {
        the_names[np_font_small1] = Istring(B);
        return true;
    }
    if (A == "font_small2") {
        the_names[np_font_small2] = Istring(B);
        return true;
    }
    if (A == "font_small3") {
        the_names[np_font_small3] = Istring(B);
        return true;
    }
    if (A == "font_small4") {
        the_names[np_font_small4] = Istring(B);
        return true;
    }
    if (A == "font_small5") {
        the_names[np_font_small5] = Istring(B);
        return true;
    }
    if (A == "font_small6") {
        the_names[np_font_small6] = Istring(B);
        return true;
    }
    if (A == "font_large") {
        the_names[np_font_large] = Istring(B);
        return true;
    }
    if (A == "font_large1") {
        the_names[np_font_large1] = Istring(B);
        return true;
    }
    if (A == "font_large2") {
        the_names[np_font_large2] = Istring(B);
        return true;
    }
    if (A == "font_large3") {
        the_names[np_font_large3] = Istring(B);
        return true;
    }
    if (A == "font_large4") {
        the_names[np_font_large4] = Istring(B);
        return true;
    }
    if (A == "font_large5") {
        the_names[np_font_large5] = Istring(B);
        return true;
    }
    if (A == "font_large6") {
        the_names[np_font_large6] = Istring(B);
        return true;
    }
    if (A == "font_normalsize") {
        the_names[np_font_normalsize] = Istring(B);
        return true;
    }
    if (A == "font_upright") {
        the_names[np_font_upright] = Istring(B);
        return true;
    }
    if (A == "font_medium") {
        the_names[np_font_medium] = Istring(B);
        return true;
    }
    if (A == "font_roman") {
        the_names[np_font_roman] = Istring(B);
        return true;
    }
    if (A == "font_it") {
        the_names[np_font_it] = Istring(B);
        return true;
    }
    if (A == "font_slanted") {
        the_names[np_font_slanted] = Istring(B);
        return true;
    }
    if (A == "font_sc") {
        the_names[np_font_sc] = Istring(B);
        return true;
    }
    if (A == "font_tt") {
        the_names[np_font_tt] = Istring(B);
        return true;
    }
    if (A == "font_sansserif") {
        the_names[np_font_sansserif] = Istring(B);
        return true;
    }
    if (A == "font_bold") {
        the_names[np_font_bold] = Istring(B);
        return true;
    }
    if (A == "font_boldextended") {
        the_names[np_font_boldextended] = Istring(B);
        return true;
    }
    if (A == "font_semibold") {
        the_names[np_font_semibold] = Istring(B);
        return true;
    }
    if (A == "font_condensed") {
        the_names[np_font_condensed] = Istring(B);
        return true;
    }
    if (A == "gloitem") {
        the_names[np_label_glo] = Istring(B);
        return true;
    }
    if (A == "graphics") {
        the_names[np_graphics] = Istring(B);
        return true;
    }
    if (A == "glo") {
        the_names[np_glo_name] = Istring(B);
        return true;
    }
    if (A == "glossary") {
        the_names[np_glossary] = Istring(B);
        return true;
    }
    if (A == "head") {
        the_names[np_head] = Istring(B);
        return true;
    }
    if (A == "hl") {
        the_names[np_s_hl] = Istring(B);
        return true;
    }
    if (A == "item") {
        the_names[np_item] = Istring(B);
        return true;
    }
    if (A == "index") {
        the_names[np_index] = Istring(B);
        return true;
    }
    if (A == "keywords") {
        the_names[np_keywords] = Istring(B);
        return true;
    }
    if (A == "labelitem") {
        the_names[np_label_item] = Istring(B);
        return true;
    }
    if (A == "lineC") {
        the_names[np_lineC] = Istring(B);
        return true;
    }
    if (A == "line") {
        the_names[np_line] = Istring(B);
        return true;
    }
    if (A == "listoffigures") {
        the_names[np_toc2] = Istring(B);
        return true;
    }
    if (A == "listoftables") {
        the_names[np_toc1] = Istring(B);
        return true;
    }
    if (A == "llap") {
        the_names[np_llap] = Istring(B);
        return true;
    }
    if (A == "linethickness") {
        the_names[np_line_thickness] = Istring(B);
        return true;
    }
    if (A == "list") {
        the_names[np_list] = Istring(B);
        return true;
    }
    if (A == "leaders") {
        the_names[np_leaders] = Istring(B);
        return true;
    }
    if (A == "leg") {
        the_names[np_leg] = Istring(B);
        return true;
    }
    if (A == "mbox") {
        the_names[np_mbox] = Istring(B);
        return true;
    }
    if (A == "math") {
        the_names[cst_math] = Istring(B);
        return true;
    }
    if (A == "multiput") {
        the_names[np_multiput] = Istring(B);
        return true;
    }
    if (A == "mainmatter") {
        the_names[np_mainmatter] = Istring(B);
        return true;
    }
    if (A == "node") {
        the_names[np_node] = Istring(B);
        return true;
    }
    if (A == "nodeconnect") {
        the_names[np_nodeconnect] = Istring(B);
        return true;
    }
    if (A == "nodecurve") {
        the_names[np_nodecurve] = Istring(B);
        return true;
    }
    if (A == "nodetriangle") {
        the_names[np_nodetriangle] = Istring(B);
        return true;
    }
    if (A == "nodecircle") {
        the_names[np_nodecircle] = Istring(B);
        return true;
    }
    if (A == "nodebox") {
        the_names[np_nodebox] = Istring(B);
        return true;
    }
    if (A == "nodeoval") {
        the_names[np_nodeoval] = Istring(B);
        return true;
    }
    if (A == "natcit") {
        the_names[np_natcit] = Istring(B);
        return true;
    }
    if (A == "oval") {
        the_names[np_oval] = Istring(B);
        return true;
    }
    if (A == "oldstyle") {
        the_names[np_s_old] = Istring(B);
        return true;
    }
    if (A == "overline") {
        the_names[np_overline] = Istring(B);
        return true;
    }
    if (A == "picture") {
        the_names[np_picture] = Istring(B);
        return true;
    }
    if (A == "put") {
        the_names[np_put] = Istring(B);
        return true;
    }
    if (A == "project") {
        the_names[np_projet] = Istring(B);
        return true;
    }
    if (A == "pers") {
        the_names[np_pers] = Istring(B);
        return true;
    }
    if (A == "prenote") {
        the_names[np_prenote] = Istring(B);
        return true;
    }
    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return true;
    }
    if (A == "row") {
        the_names[np_row] = Istring(B);
        return true;
    }
    if (A == "raisebox") {
        the_names[np_raisebox] = Istring(B);
        return true;
    }
    if (A == "rlap") {
        the_names[np_rlap] = Istring(B);
        return true;
    }
    if (A == "rotatebox") {
        the_names[np_rotatebox] = Istring(B);
        return true;
    }
    if (A == "ref") {
        the_names[np_ref] = Istring(B);
        return true;
    }
    if ((A == "rclist") && ra_ok) {
        the_names[np_rclist] = Istring(B);
        return true;
    }
    if ((A == "rcval") && ra_ok) {
        the_names[np_rcval] = Istring(B);
        return true;
    }
    if ((A == "rasection") && ra_ok) {
        the_names[np_rasection] = Istring(B);
        return true;
    }
    if (A == "subfigure") {
        the_names[np_subfigure] = Istring(B);
        return true;
    }
    if (A == "scaleput") {
        the_names[np_scaleput] = Istring(B);
        return true;
    }
    if (A == "scalebox") {
        the_names[np_sbox] = Istring(B);
        return true;
    }
    if (A == "scaption") {
        the_names[np_caption] = Istring(B);
        return true;
    }
    if (A == "sup") {
        the_names[np_s_sup] = Istring(B);
        return true;
    }
    if (A == "sub") {
        the_names[np_s_sub] = Istring(B);
        return true;
    }
    if (A == "so") {
        the_names[np_s_so] = Istring(B);
        return true;
    }
    if (A == "st") {
        the_names[np_s_st] = Istring(B);
        return true;
    }
    if (A == "stylesheet") {
        the_names[np_stylesheet] = Istring(B);
        return true;
    }
    if (A == "stylesheettype") {
        the_names[np_stylesheet_type] = Istring(B);
        return true;
    }
    if (A == "term") {
        the_names[np_term] = Istring(B);
        return true;
    }
    if (A == "texmath") {
        the_names[np_texmath] = Istring(B);
        return true;
    }
    if (A == "table") {
        the_names[np_table] = Istring(B);
        return true;
    }
    if (A == "table_env") {
        the_names[np_float_table] = Istring(B);
        return true;
    }
    if (A == "Table") {
        the_names[np_Table] = Istring(B);
        return true;
    }
    if (A == "tagcurve") {
        the_names[np_tagcurve] = Istring(B);
        return true;
    }
    if (A == "thicklines") {
        the_names[np_thick_lines] = Istring(B);
        return true;
    }
    if (A == "thinlines") {
        the_names[np_thin_lines] = Istring(B);
        return true;
    }
    if (A == "theorem_head") {
        the_names[np_alt_head] = Istring(B);
        return true;
    }
    if (A == "theorem") {
        if (B[0] == 0) { // reverst to old behavior
            the_names[np_theorem] = Istring(B);
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else if (B[0] == ' ') { // special case
            the_names[np_theorem] = Istring(B.substr(1));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else {
            the_names[np_theorem] = Istring(B);
            the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
        }
        return true;
    }
    if (A == "theindex") {
        the_names[np_theindex] = Istring(B);
        return true;
    }
    if (A == "theglossary") {
        the_names[np_theglossary] = Istring(B);
        return true;
    }
    if (A == "tableofcontents") {
        the_names[np_toc] = Istring(B);
        return true;
    }
    if (A == "texte") {
        the_names[np_texte] = Istring(B);
        return true;
    }
    if (A == "ul") {
        the_names[np_s_ul] = Istring(B);
        return true;
    }
    if (A == "underline") {
        the_names[np_underline] = Istring(B);
        return true;
    }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return true;
    }
    if (A == "vector") {
        the_names[np_vector] = Istring(B);
        return true;
    }
    if (A == "warning") {
        the_names[np_warning] = Istring(B);
        return true;
    }
    if (A == "xref") {
        the_names[np_xref] = Istring(B);
        return true;
    }
    if (A == "xtheorem") {
        the_names[np_theorem] = Istring(B);
        return true;
    }
    if (A == "xleaders") {
        the_names[np_xleaders] = Istring(B);
        return true;
    }
    return false;
}

// Handles names starting with att_
auto config_ns::assign_att(const std::string &A, const std::string &B) -> bool {
    if (A == "angle") {
        the_names[np_angle] = Istring(B);
        return true;
    }
    if (A == "affiliation") {
        the_names[np_affiliation] = Istring(B);
        return true;
    }
    if (A == "box_pos") {
        the_names[np_box_pos] = Istring(B);
        return true;
    }
    if (A == "box_scale") {
        the_names[np_s_scale] = Istring(B);
        return true;
    }
    if (A == "box_width") {
        the_names[np_box_width] = Istring(B);
        return true;
    }
    if (A == "boxed") {
        the_names[np_boxed] = Istring(B);
        return true;
    }
    if (A == "bibkey") {
        the_names[np_bibkey] = Istring(B);
        return true;
    }
    if (A == "border_bottom_width") {
        the_names[np_border_bottomw] = Istring(B);
        return true;
    }
    if (A == "border_top_width") {
        the_names[np_border_topw] = Istring(B);
        return true;
    }
    if (A == "border_left_width") {
        the_names[np_border_leftw] = Istring(B);
        return true;
    }
    if (A == "border_right_width") {
        the_names[np_border_rightw] = Istring(B);
        return true;
    }
    if (A == "centering") {
        the_names[np_center_etc1] = Istring(B);
        return true;
    }
    if (A == "clip") {
        the_names[np_clip] = Istring(B);
        return true;
    }
    if (A == "cols") {
        the_names[np_cols] = Istring(B);
        return true;
    }
    if (A == "cell_left") {
        the_names[np_c_left] = Istring(B);
        return true;
    }
    if (A == "cell_right") {
        the_names[np_c_right] = Istring(B);
        return true;
    }
    if (A == "cell_center") {
        the_names[np_c_center] = Istring(B);
        return true;
    }
    if (A == "cell_leftborder") {
        the_names[np_leftborder] = Istring(B);
        return true;
    }
    if (A == "cell_rightborder") {
        the_names[np_rightborder] = Istring(B);
        return true;
    }
    if (A == "cell_topborder") {
        the_names[np_topborder] = Istring(B);
        return true;
    }
    if (A == "cell_bottomborder") {
        the_names[np_bottomborder] = Istring(B);
        return true;
    }
    if (A == "curve_nbpts") {
        the_names[np_curve_nbpts] = Istring(B);
        return true;
    }
    if (A == "display") {
        the_names[np_display] = Istring(B);
        return true;
    }
    if (A == "dx") {
        the_names[np_dx] = Istring(B);
        return true;
    }
    if (A == "dy") {
        the_names[np_dy] = Istring(B);
        return true;
    }
    if (A == "depthA") {
        the_names[np_depthA] = Istring(B);
        return true;
    }
    if (A == "depthB") {
        the_names[np_depthB] = Istring(B);
        return true;
    }
    if (A == "depth") {
        the_names[np_depth] = Istring(B);
        return true;
    }
    if (A == "encap") {
        the_names[np_encap] = Istring(B);
        return true;
    }
    if (A == "framed") {
        the_names[np_framed] = Istring(B);
        return true;
    }
    if (A == "full") {
        the_names[np_full] = Istring(B);
        return true;
    }
    if (A == "file") {
        the_names[np_file] = Istring(B);
        return true;
    }
    if (A == "file_extension") {
        the_names[np_fileextension] = Istring(B);
        return true;
    }
    if (A == "fbox_rend") {
        the_names[np_b_rend] = Istring(B);
        return true;
    }
    if (A == "flush_left") {
        the_names[np_center_etc4] = Istring(B);
        return true;
    }
    if (A == "flush_right") {
        the_names[np_center_etc5] = Istring(B);
        return true;
    }
    if (A == "foot_position") {
        the_names[np_foot] = Istring(B);
        return true;
    }
    if (A == "full_first") {
        the_names[np_full_first] = Istring(B);
        return true;
    }
    if (A == "gloss_type") {
        the_names[np_gloss] = Istring(B);
        return true;
    }
    if (A == "height") {
        the_names[np_height] = Istring(B);
        return true;
    }
    if (A == "halign") {
        the_names[np_halign] = Istring(B);
        return true;
    }
    if (A == "hdr") {
        the_names[np_hdr] = Istring(B);
        return true;
    }
    if (A == "inner_pos") {
        the_names[np_posi] = Istring(B);
        return true;
    }
    if (A == "inline") {
        the_names[np_inline] = Istring(B);
        return true;
    }
    if (A == "junior") {
        the_names[np_junior] = Istring(B);
        return true;
    }
    if (A == "language") {
        the_names[np_language] = Istring(B);
        return true;
    }
    if (A == "level") {
        the_names[np_level] = Istring(B);
        return true;
    }
    if (A == "minipage_width") {
        the_names[np_minipage_width] = Istring(B);
        return true;
    }
    if (A == "marginpar") {
        the_names[np_marginpar] = Istring(B);
        return true;
    }
    if (A == "mathmlns") {
        the_names[cst_mathml] = Istring(B);
        return true;
    }
    if (A == "marginpar") {
        the_names[np_marginpar] = Istring(B);
        return true;
    }
    if (A == "mode") {
        the_names[cst_mode] = Istring(B);
        return true;
    }
    if (A == "noindent") {
        the_names[np_noindent] = Istring(B);
        return true;
    }
    if (A == "nonumber") {
        the_names[np_nonumber] = Istring(B);
        return true;
    }
    if (A == "nom") {
        the_names[np_nom] = Istring(B);
        return true;
    }
    if (A == "name") {
        the_names[np_name] = Istring(B);
        return true;
    }
    if (A == "nameA") {
        the_names[np_nameA] = Istring(B);
        return true;
    }
    if (A == "nameB") {
        the_names[np_nameB] = Istring(B);
        return true;
    }
    if (A == "place") {
        the_names[np_place] = Istring(B);
        return true;
    }
    if (A == "prenom") {
        the_names[np_prenom] = Istring(B);
        return true;
    }
    if (A == "pre") {
        the_names[np_pre] = Istring(B);
        return true;
    }
    if (A == "particule") {
        the_names[np_particle] = Istring(B);
        return true;
    }
    if (A == "page") {
        the_names[np_page] = Istring(B);
        return true;
    }
    if (A == "profession") {
        the_names[np_profession] = Istring(B);
        return true;
    }
    if (A == "posA") {
        the_names[np_posA] = Istring(B);
        return true;
    }
    if (A == "posB") {
        the_names[np_posB] = Istring(B);
        return true;
    }
    if (A == "pos") {
        the_names[np_pos] = Istring(B);
        return true;
    }
    if (A == "quote") {
        the_names[np_center_etc2] = Istring(B);
        return true;
    }
    if (A == "quotation") {
        the_names[np_center_etc3] = Istring(B);
        return true;
    }
    if (A == "rotate_angle") {
        the_names[np_r_angle] = Istring(B);
        return true;
    }
    if (A == "rend") {
        the_names[np_rend] = Istring(B);
        return true;
    }
    if (A == "row_spaceafter") {
        the_names[np_spaceafter] = Istring(B);
        return true;
    }
    if (A == "repeat") {
        the_names[np_repeat] = Istring(B);
        return true;
    }
    if (A == "scale") {
        the_names[np_scale] = Istring(B);
        return true;
    }
    if (A == "space_before") {
        the_names[np_spacebefore] = Istring(B);
        return true;
    }
    if (A == "size") {
        the_names[np_size] = Istring(B);
        return true;
    }
    if (A == "starred") {
        the_names[np_starred] = Istring(B);
        return true;
    }
    if (A == "table_width") {
        the_names[np_tab_width] = Istring(B);
        return true;
    }
    if (A == "type") {
        the_names[np_type] = Istring(B);
        return true;
    }
    if (A == "textype") {
        the_names[np_textype] = Istring(B);
        return true;
    }
    if (A == "unit_length") {
        the_names[np_unit_length] = Istring(B);
        return true;
    }
    if (A == "user_list") {
        the_names[np_user_list] = Istring(B);
        return true;
    }
    if (A == "vpos") {
        the_names[np_vpos] = Istring(B);
        return true;
    }
    if (A == "verse") {
        the_names[np_center_etc6] = Istring(B);
        return true;
    }
    if (A == "width") {
        the_names[np_width] = Istring(B);
        return true;
    }
    if (A == "xscale") {
        the_names[np_xscale] = Istring(B);
        return true;
    }
    if (A == "xscaley") {
        the_names[np_xscaley] = Istring(B);
        return true;
    }
    if (A == "xpos") {
        the_names[np_xpos] = Istring(B);
        return true;
    }
    if (A == "xdir") {
        the_names[np_xdir] = Istring(B);
        return true;
    }
    if (A == "yscale") {
        the_names[np_yscale] = Istring(B);
        return true;
    }
    if (A == "yscalex") {
        the_names[np_yscalex] = Istring(B);
        return true;
    }
    if (A == "ydir") {
        the_names[np_ydir] = Istring(B);
        return true;
    }
    if (A == "ypos") {
        the_names[np_ypos] = Istring(B);
        return true;
    }
    return false;
}

// Todo Bouche
// \def\Q{\mathbb{Q}} $\bar \Q$
