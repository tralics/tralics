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

FullLogger log_and_tty;             // the logger
Logger &   the_log = log_and_tty.L; // the log file part of the logger.
MainClass *the_main;                // the main class data structure
bool       ra_ok = true;            // inhibits  redefinitions

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
    String A = a.c_str();
    String B = b.c_str();
    auto   n = a.size();
    if (A[0] == 'e' && A[1] == 'l' && A[2] == 't' && A[3] == '_') return config_ns::assign_name(A + 4, B);
    if (A[0] == 'x' && A[1] == 'm' && A[2] == 'l' && A[3] == '_') {
        if (A[n - 1] == 'e' && A[n - 2] == 'm' && A[n - 3] == 'a' && A[n - 4] == 'n' && A[n - 5] == '_') { a.at(n - 5) = 0; }
        return config_ns::assign_name(A + 4, B);
    }
    if (A[0] == 'a' && A[1] == 't' && A[2] == 't' && A[3] == '_') return config_ns::assign_att(A + 4, B);
    if (a == "lang_fr") {
        the_names[np_french] = Istring(B);
        return true;
    }
    if (a == "lang_en") {
        the_names[np_english] = Istring(B);
        return true;
    }
    if (a == "distinguish_refer_in_rabib") {
        if ((b == "true") || (b == "yes"))
            the_main->distinguish_refer = true;
        else if ((b == "false") || (b == "no"))
            the_main->distinguish_refer = false;
        else
            b << bf_reset << "ignored";
        return true;
    }
    if (a == "entity_names") {
        the_main->set_ent_names(B);
        return true;
    }
    if (a == "default_class") {
        the_main->default_class = b.to_string();
        return true;
    }
    if (a == "alternate_item") {
        if (b == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (b == "true")
            the_parser.hash_table.eval_let("item", "@item");
        else
            a.reset();
        return true;
    }
    if (a == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << B << "}";
        the_main->add_to_from_config(1, aux);
        return true;
    }
    if (a == "everyjob") {
        Buffer aux;
        aux << "\\everyjob={" << B << "}";
        everyjob_string = aux.to_string();
        return true;
    }
    if (a == "no_footnote_hack") {
        if (b == "true") the_main->footnote_hack = false;
        if (b == "false") the_main->footnote_hack = true;
        return true;
    }
    if (a == "use_font_elt") {
        if (b == "true") the_main->use_font_elt = true;
        if (b == "false") the_main->use_font_elt = false;
        return true;
    }
    if (a == "use_all_sizes") {
        if (b == "true") the_main->use_all_sizes = true;
        if (b == "false") the_main->use_all_sizes = false;
        return true;
    }
    if (a == "bibtex_fields") {
        b.interpret_bibtex_list();
        return false;
    }
    if (a == "bibtex_extensions") {
        b.interpret_bibtex_extension_list();
        return false;
    }
    if (a == "mfenced_separator_val") {
        if (strcmp(B, "NONE") == 0)
            the_names[np_separator] = Istring(0UL);
        else
            the_names[np_separator] = Istring(B);
        return true;
    }
    if (n > 5 && A[n - 5] == '_' && A[n - 4] == 'v' && A[n - 3] == 'a' && A[n - 2] == 'l' && A[n - 1] == 's') {
        a.at(n - 5) = 0;
        config_ns::interpret_list(a.to_string(), b);
        a.reset();
        return true;
    }
    if (a == "mml_font_normal") {
        the_names[np_mml_normal] = Istring(B);
        return true;
    }
    if (a == "mml_font_upright") {
        the_names[np_mml_upright] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold") {
        the_names[np_mml_bold] = Istring(B);
        return true;
    }
    if (a == "mml_font_italic") {
        the_names[np_mml_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_italic") {
        the_names[np_mml_bold_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_script") {
        the_names[np_mml_script] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_script") {
        the_names[np_mml_bold_script] = Istring(B);
        return true;
    }
    if (a == "mml_font_fraktur") {
        the_names[np_mml_fraktur] = Istring(B);
        return true;
    }
    if (a == "mml_font_doublestruck") {
        the_names[np_mml_doublestruck] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_fraktur") {
        the_names[np_mml_bold_fraktur] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif") {
        the_names[np_mml_sansserif] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_sansserif") {
        the_names[np_mml_bold_sansserif] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif_italic") {
        the_names[np_mml_sansserif_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif_bold_italic") {
        the_names[np_mml_sansserif_bold_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_monospace") {
        the_names[np_mml_monospace] = Istring(B);
        return true;
    }
    return false;
}

// Handles names starting with xml_
auto config_ns::assign_name(String A, String B) -> bool {
    switch (A[0]) {
    case 'a':
        if (strcmp(A, "accueil") == 0) {
            the_names[np_accueil] = Istring(B);
            return true;
        }
        if (strcmp(A, "arc") == 0) {
            the_names[np_arc] = Istring(B);
            return true;
        }
        if (strcmp(A, "allowbreak") == 0) {
            the_names[np_allowbreak] = Istring(B);
            return true;
        }
        if (strcmp(A, "anchor") == 0) {
            the_names[np_anchor] = Istring(B);
            return true;
        }
        if (strcmp(A, "anodeconnect") == 0) {
            the_names[np_anodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "abarnodeconnect") == 0) {
            the_names[np_abarnodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "anodecurve") == 0) {
            the_names[np_anodecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "alt_caption") == 0) {
            the_names[np_alt_caption] = Istring(B);
            return true;
        }
        if (strcmp(A, "alt_section") == 0) {
            the_names[np_alt_section] = Istring(B);
            return true;
        }
        if (strcmp(A, "alternatives") == 0) {
            the_names[np_alternatives] = Istring(B);
            return true;
        }
        return false;
    case 'b':
        if (strcmp(A, "box") == 0) {
            the_names[np_box] = Istring(B);
            return true;
        }
        if (strcmp(A, "bezier") == 0) {
            the_names[np_bezier] = Istring(B);
            return true;
        }
        if (strcmp(A, "bigcircle") == 0) {
            the_names[np_bigcircle] = Istring(B);
            return true;
        }
        if (strcmp(A, "backmatter") == 0) {
            the_names[np_backmatter] = Istring(B);
            return true;
        }
        if (strcmp(A, "biblio") == 0) {
            the_names[np_biblio] = Istring(B);
            return true;
        }
        if (strcmp(A, "bpers") == 0) {
            the_names[np_bpers] = Istring(B);
            return true;
        }
        if (strcmp(A, "barnodeconnect") == 0) {
            the_names[np_barnodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibitem") == 0) {
            the_names[np_bibitem] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibkey") == 0) {
            the_names[np_bibkey] = Istring(B);
            return true;
        }
        return false;
    case 'c':
        if (strcmp(A, "cell") == 0) {
            the_names[np_cell] = Istring(B);
            return true;
        }
        if (strcmp(A, "caption") == 0) {
            the_names[np_captions] = Istring(B);
            return true;
        }
        if (strcmp(A, "circle") == 0) {
            the_names[np_circle] = Istring(B);
            return true;
        }
        if (strcmp(A, "closecurve") == 0) {
            the_names[np_closecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "curve") == 0) {
            the_names[np_curve] = Istring(B);
            return true;
        }
        if (strcmp(A, "catperso") == 0) {
            the_names[np_catperso] = Istring(B);
            return true;
        }
        if (strcmp(A, "composition_ra") == 0) {
            the_names[cst_composition] = Istring(B);
            return true;
        }
        if (strcmp(A, "cleaders") == 0) {
            the_names[np_cleaders] = Istring(B);
            return true;
        }
        if (strcmp(A, "caps") == 0) {
            the_names[np_s_caps] = Istring(B);
            return true;
        }
        if (strcmp(A, "cit") == 0) {
            the_names[np_cit] = Istring(B);
            return true;
        }
        if (strcmp(A, "citation") == 0) {
            the_names[np_citation] = Istring(B);
            return true;
        }
        if (strcmp(A, "citetype") == 0) {
            the_names[np_cite_type] = Istring(B);
            return true;
        }
        return false;
    case 'd':
        if (strcmp(A, "dashline") == 0) {
            the_names[np_dashline] = Istring(B);
            return true;
        }
        if (strcmp(A, "div0") == 0) {
            the_names[np_div0] = Istring(B);
            return true;
        }
        if (strcmp(A, "div1") == 0) {
            the_names[np_div1] = Istring(B);
            return true;
        }
        if (strcmp(A, "div2") == 0) {
            the_names[np_div2] = Istring(B);
            return true;
        }
        if (strcmp(A, "div3") == 0) {
            the_names[np_div3] = Istring(B);
            return true;
        }
        if (strcmp(A, "div4") == 0) {
            the_names[np_div4] = Istring(B);
            return true;
        }
        if (strcmp(A, "div5") == 0) {
            the_names[np_div5] = Istring(B);
            return true;
        }
        if (strcmp(A, "div6") == 0) {
            the_names[np_div6] = Istring(B);
            return true;
        }
        if (strcmp(A, "drawline") == 0) {
            the_names[np_drawline] = Istring(B);
            return true;
        }
        if (strcmp(A, "dottedline") == 0) {
            the_names[np_dottedline] = Istring(B);
            return true;
        }
        return false;
    case 'e':
        if (strcmp(A, "eqnpos") == 0) {
            the_names[np_eqnpos] = Istring(B);
            return true;
        }
        return false;
    case 'f':
        if (strcmp(A, "footnote") == 0) {
            the_names[np_footnote] = Istring(B);
            return true;
        }
        if (strcmp(A, "formula") == 0) {
            the_names[np_formula] = Istring(B);
            return true;
        }
        if (strcmp(A, "fbox") == 0) {
            the_names[np_fbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "figure") == 0) {
            the_names[np_figure] = Istring(B);
            return true;
        }
        if (strcmp(A, "figure_env") == 0) {
            the_names[np_float_figure] = Istring(B);
            return true;
        }
        if (strcmp(A, "frontmatter") == 0) {
            the_names[np_frontmatter] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small") == 0) {
            the_names[np_font_small] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small1") == 0) {
            the_names[np_font_small1] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small2") == 0) {
            the_names[np_font_small2] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small3") == 0) {
            the_names[np_font_small3] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small4") == 0) {
            the_names[np_font_small4] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small5") == 0) {
            the_names[np_font_small5] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small6") == 0) {
            the_names[np_font_small6] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large") == 0) {
            the_names[np_font_large] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large1") == 0) {
            the_names[np_font_large1] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large2") == 0) {
            the_names[np_font_large2] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large3") == 0) {
            the_names[np_font_large3] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large4") == 0) {
            the_names[np_font_large4] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large5") == 0) {
            the_names[np_font_large5] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large6") == 0) {
            the_names[np_font_large6] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_normalsize") == 0) {
            the_names[np_font_normalsize] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_upright") == 0) {
            the_names[np_font_upright] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_medium") == 0) {
            the_names[np_font_medium] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_roman") == 0) {
            the_names[np_font_roman] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_it") == 0) {
            the_names[np_font_it] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_slanted") == 0) {
            the_names[np_font_slanted] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_sc") == 0) {
            the_names[np_font_sc] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_tt") == 0) {
            the_names[np_font_tt] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_sansserif") == 0) {
            the_names[np_font_sansserif] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_bold") == 0) {
            the_names[np_font_bold] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_boldextended") == 0) {
            the_names[np_font_boldextended] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_semibold") == 0) {
            the_names[np_font_semibold] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_condensed") == 0) {
            the_names[np_font_condensed] = Istring(B);
            return true;
        }
        return false;
    case 'g':
        if (strcmp(A, "gloitem") == 0) {
            the_names[np_label_glo] = Istring(B);
            return true;
        }
        if (strcmp(A, "graphics") == 0) {
            the_names[np_graphics] = Istring(B);
            return true;
        }
        if (strcmp(A, "glo") == 0) {
            the_names[np_glo_name] = Istring(B);
            return true;
        }
        if (strcmp(A, "glossary") == 0) {
            the_names[np_glossary] = Istring(B);
            return true;
        }
        return false;
    case 'h':
        if (strcmp(A, "head") == 0) {
            the_names[np_head] = Istring(B);
            return true;
        }
        if (strcmp(A, "hl") == 0) {
            the_names[np_s_hl] = Istring(B);
            return true;
        }
        return false;
    case 'i':
        if (strcmp(A, "item") == 0) {
            the_names[np_item] = Istring(B);
            return true;
        }
        if (strcmp(A, "index") == 0) {
            the_names[np_index] = Istring(B);
            return true;
        }
        return false;
    case 'k':
        if (strcmp(A, "keywords") == 0) {
            the_names[np_keywords] = Istring(B);
            return true;
        }
        return false;
    case 'l':
        if (strcmp(A, "labelitem") == 0) {
            the_names[np_label_item] = Istring(B);
            return true;
        }
        if (strcmp(A, "lineC") == 0) {
            the_names[np_lineC] = Istring(B);
            return true;
        }
        if (strcmp(A, "line") == 0) {
            the_names[np_line] = Istring(B);
            return true;
        }
        if (strcmp(A, "listoffigures") == 0) {
            the_names[np_toc2] = Istring(B);
            return true;
        }
        if (strcmp(A, "listoftables") == 0) {
            the_names[np_toc1] = Istring(B);
            return true;
        }
        if (strcmp(A, "llap") == 0) {
            the_names[np_llap] = Istring(B);
            return true;
        }
        if (strcmp(A, "linethickness") == 0) {
            the_names[np_line_thickness] = Istring(B);
            return true;
        }
        if (strcmp(A, "list") == 0) {
            the_names[np_list] = Istring(B);
            return true;
        }
        if (strcmp(A, "leaders") == 0) {
            the_names[np_leaders] = Istring(B);
            return true;
        }
        if (strcmp(A, "leg") == 0) {
            the_names[np_leg] = Istring(B);
            return true;
        }
        return false;
    case 'm':
        if (strcmp(A, "mbox") == 0) {
            the_names[np_mbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "math") == 0) {
            the_names[cst_math] = Istring(B);
            return true;
        }
        if (strcmp(A, "multiput") == 0) {
            the_names[np_multiput] = Istring(B);
            return true;
        }
        if (strcmp(A, "mainmatter") == 0) {
            the_names[np_mainmatter] = Istring(B);
            return true;
        }
        return false;
    case 'n':
        if (strcmp(A, "node") == 0) {
            the_names[np_node] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodeconnect") == 0) {
            the_names[np_nodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodecurve") == 0) {
            the_names[np_nodecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodetriangle") == 0) {
            the_names[np_nodetriangle] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodecircle") == 0) {
            the_names[np_nodecircle] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodebox") == 0) {
            the_names[np_nodebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodeoval") == 0) {
            the_names[np_nodeoval] = Istring(B);
            return true;
        }
        if (strcmp(A, "natcit") == 0) {
            the_names[np_natcit] = Istring(B);
            return true;
        }
        return false;
    case 'o':
        if (strcmp(A, "oval") == 0) {
            the_names[np_oval] = Istring(B);
            return true;
        }
        if (strcmp(A, "oldstyle") == 0) {
            the_names[np_s_old] = Istring(B);
            return true;
        }
        if (strcmp(A, "overline") == 0) {
            the_names[np_overline] = Istring(B);
            return true;
        }
        return false;
    case 'p':
        if (strcmp(A, "picture") == 0) {
            the_names[np_picture] = Istring(B);
            return true;
        }
        if (strcmp(A, "put") == 0) {
            the_names[np_put] = Istring(B);
            return true;
        }
        if (strcmp(A, "project") == 0) {
            the_names[np_projet] = Istring(B);
            return true;
        }
        if (strcmp(A, "pers") == 0) {
            the_names[np_pers] = Istring(B);
            return true;
        }
        if (strcmp(A, "prenote") == 0) {
            the_names[np_prenote] = Istring(B);
            return true;
        }
        if (strcmp(A, "pack_font_att") == 0) {
            if (strcmp(B, "true") == 0) the_main->pack_font_elt = true;
            if (strcmp(B, "false") == 0) the_main->pack_font_elt = false;
            return true;
        }
        return false;
    case 'r':
        if (strcmp(A, "row") == 0) {
            the_names[np_row] = Istring(B);
            return true;
        }
        if (strcmp(A, "raisebox") == 0) {
            the_names[np_raisebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "rlap") == 0) {
            the_names[np_rlap] = Istring(B);
            return true;
        }
        if (strcmp(A, "rotatebox") == 0) {
            the_names[np_rotatebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "ref") == 0) {
            the_names[np_ref] = Istring(B);
            return true;
        }
        if (strcmp(A, "rclist") == 0 && ra_ok) {
            the_names[np_rclist] = Istring(B);
            return true;
        }
        if (strcmp(A, "rcval") == 0 && ra_ok) {
            the_names[np_rcval] = Istring(B);
            return true;
        }
        if (strcmp(A, "rasection") == 0 && ra_ok) {
            the_names[np_rasection] = Istring(B);
            return true;
        }
        return false;
    case 's':
        if (strcmp(A, "subfigure") == 0) {
            the_names[np_subfigure] = Istring(B);
            return true;
        }
        if (strcmp(A, "scaleput") == 0) {
            the_names[np_scaleput] = Istring(B);
            return true;
        }
        if (strcmp(A, "scalebox") == 0) {
            the_names[np_sbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "scaption") == 0) {
            the_names[np_caption] = Istring(B);
            return true;
        }
        if (strcmp(A, "sup") == 0) {
            the_names[np_s_sup] = Istring(B);
            return true;
        }
        if (strcmp(A, "sub") == 0) {
            the_names[np_s_sub] = Istring(B);
            return true;
        }
        if (strcmp(A, "so") == 0) {
            the_names[np_s_so] = Istring(B);
            return true;
        }
        if (strcmp(A, "st") == 0) {
            the_names[np_s_st] = Istring(B);
            return true;
        }
        if (strcmp(A, "stylesheet") == 0) {
            the_names[np_stylesheet] = Istring(B);
            return true;
        }
        if (strcmp(A, "stylesheettype") == 0) {
            the_names[np_stylesheet_type] = Istring(B);
            return true;
        }
        return false;
    case 't':
        if (strcmp(A, "term") == 0) {
            the_names[np_term] = Istring(B);
            return true;
        }
        if (strcmp(A, "texmath") == 0) {
            the_names[np_texmath] = Istring(B);
            return true;
        }
        if (strcmp(A, "table") == 0) {
            the_names[np_table] = Istring(B);
            return true;
        }
        if (strcmp(A, "table_env") == 0) {
            the_names[np_float_table] = Istring(B);
            return true;
        }
        if (strcmp(A, "Table") == 0) {
            the_names[np_Table] = Istring(B);
            return true;
        }
        if (strcmp(A, "tagcurve") == 0) {
            the_names[np_tagcurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "thicklines") == 0) {
            the_names[np_thick_lines] = Istring(B);
            return true;
        }
        if (strcmp(A, "thinlines") == 0) {
            the_names[np_thin_lines] = Istring(B);
            return true;
        }
        if (strcmp(A, "theorem_head") == 0) {
            the_names[np_alt_head] = Istring(B);
            return true;
        }
        if (strcmp(A, "theorem") == 0) {
            if (B[0] == 0) { // reverst to old behavior
                the_names[np_theorem] = Istring(B);
                the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
            } else if (B[0] == ' ') { // special case
                the_names[np_theorem] = Istring(B + 1);
                the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
            } else {
                the_names[np_theorem] = Istring(B);
                the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
            }
            return true;
        }
        if (strcmp(A, "theindex") == 0) {
            the_names[np_theindex] = Istring(B);
            return true;
        }
        if (strcmp(A, "theglossary") == 0) {
            the_names[np_theglossary] = Istring(B);
            return true;
        }
        if (strcmp(A, "tableofcontents") == 0) {
            the_names[np_toc] = Istring(B);
            return true;
        }
        if (strcmp(A, "texte") == 0) {
            the_names[np_texte] = Istring(B);
            return true;
        }
        return false;
    case 'u':
        if (strcmp(A, "ul") == 0) {
            the_names[np_s_ul] = Istring(B);
            return true;
        }
        if (strcmp(A, "underline") == 0) {
            the_names[np_underline] = Istring(B);
            return true;
        }
        if (strcmp(A, "use_font_elt") == 0) {
            if (strcmp(B, "true") == 0) the_main->use_font_elt = true;
            if (strcmp(B, "false") == 0) the_main->use_font_elt = false;
            return true;
        }
        return false;
    case 'v':
        if (strcmp(A, "vector") == 0) {
            the_names[np_vector] = Istring(B);
            return true;
        }
        return false;
    case 'w':
        if (strcmp(A, "warning") == 0) {
            the_names[np_warning] = Istring(B);
            return true;
        }
        return false;
    case 'x':
        if (strcmp(A, "xref") == 0) {
            the_names[np_xref] = Istring(B);
            return true;
        }
        if (strcmp(A, "xtheorem") == 0) {
            the_names[np_theorem] = Istring(B);
            return true;
        }
        if (strcmp(A, "xleaders") == 0) {
            the_names[np_xleaders] = Istring(B);
            return true;
        }
        return false;
    default: return false;
    }
}

// Handles names starting with att_
auto config_ns::assign_att(String A, String B) -> bool {
    switch (A[0]) {
    case 'a':
        if (strcmp(A, "angle") == 0) {
            the_names[np_angle] = Istring(B);
            return true;
        }
        if (strcmp(A, "affiliation") == 0) {
            the_names[np_affiliation] = Istring(B);
            return true;
        }
        return false;
    case 'b':
        if (strcmp(A, "box_pos") == 0) {
            the_names[np_box_pos] = Istring(B);
            return true;
        }
        if (strcmp(A, "box_scale") == 0) {
            the_names[np_s_scale] = Istring(B);
            return true;
        }
        if (strcmp(A, "box_width") == 0) {
            the_names[np_box_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "boxed") == 0) {
            the_names[np_boxed] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibkey") == 0) {
            the_names[np_bibkey] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_bottom_width") == 0) {
            the_names[np_border_bottomw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_top_width") == 0) {
            the_names[np_border_topw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_left_width") == 0) {
            the_names[np_border_leftw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_right_width") == 0) {
            the_names[np_border_rightw] = Istring(B);
            return true;
        }
        return false;
    case 'c':
        if (strcmp(A, "centering") == 0) {
            the_names[np_center_etc1] = Istring(B);
            return true;
        }
        if (strcmp(A, "clip") == 0) {
            the_names[np_clip] = Istring(B);
            return true;
        }
        if (strcmp(A, "cols") == 0) {
            the_names[np_cols] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_left") == 0) {
            the_names[np_c_left] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_right") == 0) {
            the_names[np_c_right] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_center") == 0) {
            the_names[np_c_center] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_leftborder") == 0) {
            the_names[np_leftborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_rightborder") == 0) {
            the_names[np_rightborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_topborder") == 0) {
            the_names[np_topborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_bottomborder") == 0) {
            the_names[np_bottomborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "curve_nbpts") == 0) {
            the_names[np_curve_nbpts] = Istring(B);
            return true;
        }
        return false;
    case 'd':
        if (strcmp(A, "display") == 0) {
            the_names[np_display] = Istring(B);
            return true;
        }
        if (strcmp(A, "dx") == 0) {
            the_names[np_dx] = Istring(B);
            return true;
        }
        if (strcmp(A, "dy") == 0) {
            the_names[np_dy] = Istring(B);
            return true;
        }
        if (strcmp(A, "depthA") == 0) {
            the_names[np_depthA] = Istring(B);
            return true;
        }
        if (strcmp(A, "depthB") == 0) {
            the_names[np_depthB] = Istring(B);
            return true;
        }
        if (strcmp(A, "depth") == 0) {
            the_names[np_depth] = Istring(B);
            return true;
        }
        return false;
    case 'e':
        if (strcmp(A, "encap") == 0) {
            the_names[np_encap] = Istring(B);
            return true;
        }
        return false;
    case 'f':
        if (strcmp(A, "framed") == 0) {
            the_names[np_framed] = Istring(B);
            return true;
        }
        if (strcmp(A, "full") == 0) {
            the_names[np_full] = Istring(B);
            return true;
        }
        if (strcmp(A, "file") == 0) {
            the_names[np_file] = Istring(B);
            return true;
        }
        if (strcmp(A, "file_extension") == 0) {
            the_names[np_fileextension] = Istring(B);
            return true;
        }
        if (strcmp(A, "fbox_rend") == 0) {
            the_names[np_b_rend] = Istring(B);
            return true;
        }
        if (strcmp(A, "flush_left") == 0) {
            the_names[np_center_etc4] = Istring(B);
            return true;
        }
        if (strcmp(A, "flush_right") == 0) {
            the_names[np_center_etc5] = Istring(B);
            return true;
        }
        if (strcmp(A, "foot_position") == 0) {
            the_names[np_foot] = Istring(B);
            return true;
        }
        if (strcmp(A, "full_first") == 0) {
            the_names[np_full_first] = Istring(B);
            return true;
        }
        return false;
    case 'g':
        if (strcmp(A, "gloss_type") == 0) {
            the_names[np_gloss] = Istring(B);
            return true;
        }
        return false;
    case 'h':
        if (strcmp(A, "height") == 0) {
            the_names[np_height] = Istring(B);
            return true;
        }
        if (strcmp(A, "halign") == 0) {
            the_names[np_halign] = Istring(B);
            return true;
        }
        if (strcmp(A, "hdr") == 0) {
            the_names[np_hdr] = Istring(B);
            return true;
        }
        return false;
    case 'i':
        if (strcmp(A, "inner_pos") == 0) {
            the_names[np_posi] = Istring(B);
            return true;
        }
        if (strcmp(A, "inline") == 0) {
            the_names[np_inline] = Istring(B);
            return true;
        }
        return false;
    case 'j':
        if (strcmp(A, "junior") == 0) {
            the_names[np_junior] = Istring(B);
            return true;
        }
        return false;
    case 'l':
        if (strcmp(A, "language") == 0) {
            the_names[np_language] = Istring(B);
            return true;
        }
        if (strcmp(A, "level") == 0) {
            the_names[np_level] = Istring(B);
            return true;
        }
        return false;
    case 'm':
        if (strcmp(A, "minipage_width") == 0) {
            the_names[np_minipage_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "marginpar") == 0) {
            the_names[np_marginpar] = Istring(B);
            return true;
        }
        if (strcmp(A, "mathmlns") == 0) {
            the_names[cst_mathml] = Istring(B);
            return true;
        }
        if (strcmp(A, "marginpar") == 0) {
            the_names[np_marginpar] = Istring(B);
            return true;
        }
        if (strcmp(A, "mode") == 0) {
            the_names[cst_mode] = Istring(B);
            return true;
        }
        return false;
    case 'n':
        if (strcmp(A, "noindent") == 0) {
            the_names[np_noindent] = Istring(B);
            return true;
        }
        if (strcmp(A, "nonumber") == 0) {
            the_names[np_nonumber] = Istring(B);
            return true;
        }
        if (strcmp(A, "nom") == 0) {
            the_names[np_nom] = Istring(B);
            return true;
        }
        if (strcmp(A, "name") == 0) {
            the_names[np_name] = Istring(B);
            return true;
        }
        if (strcmp(A, "nameA") == 0) {
            the_names[np_nameA] = Istring(B);
            return true;
        }
        if (strcmp(A, "nameB") == 0) {
            the_names[np_nameB] = Istring(B);
            return true;
        }
        return false;
    case 'p':
        if (strcmp(A, "place") == 0) {
            the_names[np_place] = Istring(B);
            return true;
        }
        if (strcmp(A, "prenom") == 0) {
            the_names[np_prenom] = Istring(B);
            return true;
        }
        if (strcmp(A, "pre") == 0) {
            the_names[np_pre] = Istring(B);
            return true;
        }
        if (strcmp(A, "particule") == 0) {
            the_names[np_particle] = Istring(B);
            return true;
        }
        if (strcmp(A, "page") == 0) {
            the_names[np_page] = Istring(B);
            return true;
        }
        if (strcmp(A, "profession") == 0) {
            the_names[np_profession] = Istring(B);
            return true;
        }
        if (strcmp(A, "posA") == 0) {
            the_names[np_posA] = Istring(B);
            return true;
        }
        if (strcmp(A, "posB") == 0) {
            the_names[np_posB] = Istring(B);
            return true;
        }
        if (strcmp(A, "pos") == 0) {
            the_names[np_pos] = Istring(B);
            return true;
        }
        return false;
    case 'q':
        if (strcmp(A, "quote") == 0) {
            the_names[np_center_etc2] = Istring(B);
            return true;
        }
        if (strcmp(A, "quotation") == 0) {
            the_names[np_center_etc3] = Istring(B);
            return true;
        }
        return false;
    case 'r':
        if (strcmp(A, "rotate_angle") == 0) {
            the_names[np_r_angle] = Istring(B);
            return true;
        }
        if (strcmp(A, "rend") == 0) {
            the_names[np_rend] = Istring(B);
            return true;
        }
        if (strcmp(A, "row_spaceafter") == 0) {
            the_names[np_spaceafter] = Istring(B);
            return true;
        }
        if (strcmp(A, "repeat") == 0) {
            the_names[np_repeat] = Istring(B);
            return true;
        }
        return false;
    case 's':
        if (strcmp(A, "scale") == 0) {
            the_names[np_scale] = Istring(B);
            return true;
        }
        if (strcmp(A, "space_before") == 0) {
            the_names[np_spacebefore] = Istring(B);
            return true;
        }
        if (strcmp(A, "size") == 0) {
            the_names[np_size] = Istring(B);
            return true;
        }
        if (strcmp(A, "starred") == 0) {
            the_names[np_starred] = Istring(B);
            return true;
        }
        return false;
    case 't':
        if (strcmp(A, "table_width") == 0) {
            the_names[np_tab_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "type") == 0) {
            the_names[np_type] = Istring(B);
            return true;
        }
        if (strcmp(A, "textype") == 0) {
            the_names[np_textype] = Istring(B);
            return true;
        }
        return false;
    case 'u':
        if (strcmp(A, "unit_length") == 0) {
            the_names[np_unit_length] = Istring(B);
            return true;
        }
        if (strcmp(A, "user_list") == 0) {
            the_names[np_user_list] = Istring(B);
            return true;
        }
        return false;
    case 'v':
        if (strcmp(A, "vpos") == 0) {
            the_names[np_vpos] = Istring(B);
            return true;
        }
        if (strcmp(A, "verse") == 0) {
            the_names[np_center_etc6] = Istring(B);
            return true;
        }
        return false;
    case 'w':
        if (strcmp(A, "width") == 0) {
            the_names[np_width] = Istring(B);
            return true;
        }
        return false;
    case 'x':
        if (strcmp(A, "xscale") == 0) {
            the_names[np_xscale] = Istring(B);
            return true;
        }
        if (strcmp(A, "xscaley") == 0) {
            the_names[np_xscaley] = Istring(B);
            return true;
        }
        if (strcmp(A, "xpos") == 0) {
            the_names[np_xpos] = Istring(B);
            return true;
        }
        if (strcmp(A, "xdir") == 0) {
            the_names[np_xdir] = Istring(B);
            return true;
        }
        return false;
    case 'y':
        if (strcmp(A, "yscale") == 0) {
            the_names[np_yscale] = Istring(B);
            return true;
        }
        if (strcmp(A, "yscalex") == 0) {
            the_names[np_yscalex] = Istring(B);
            return true;
        }
        if (strcmp(A, "ydir") == 0) {
            the_names[np_ydir] = Istring(B);
            return true;
        }
        if (strcmp(A, "ypos") == 0) {
            the_names[np_ypos] = Istring(B);
            return true;
        }
        return false;
    default: return false;
    }
}

// Todo Bouche
// \def\Q{\mathbb{Q}} $\bar \Q$
