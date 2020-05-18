#include "tralics/NameMapper.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

auto NameMapper::assign_name(const std::string &A, std::string B) -> bool {
    if (A == "accueil") {
        set(np_accueil, Istring(B));
        return true;
    }
    if (A == "arc") {
        set(np_arc, Istring(B));
        return true;
    }
    if (A == "allowbreak") {
        set(np_allowbreak, Istring(B));
        return true;
    }
    if (A == "anchor") {
        set(np_anchor, Istring(B));
        return true;
    }
    if (A == "anodeconnect") {
        set(np_anodeconnect, Istring(B));
        return true;
    }
    if (A == "abarnodeconnect") {
        set(np_abarnodeconnect, Istring(B));
        return true;
    }
    if (A == "anodecurve") {
        set(np_anodecurve, Istring(B));
        return true;
    }
    if (A == "alt_caption") {
        set(np_alt_caption, Istring(B));
        return true;
    }
    if (A == "alt_section") {
        set(np_alt_section, Istring(B));
        return true;
    }
    if (A == "alternatives") {
        set(np_alternatives, Istring(B));
        return true;
    }
    if (A == "box") {
        set(np_box, Istring(B));
        return true;
    }
    if (A == "bezier") {
        set(np_bezier, Istring(B));
        return true;
    }
    if (A == "bigcircle") {
        set(np_bigcircle, Istring(B));
        return true;
    }
    if (A == "backmatter") {
        set(np_backmatter, Istring(B));
        return true;
    }
    if (A == "biblio") {
        set(np_biblio, Istring(B));
        return true;
    }
    if (A == "bpers") {
        set(np_bpers, Istring(B));
        return true;
    }
    if (A == "barnodeconnect") {
        set(np_barnodeconnect, Istring(B));
        return true;
    }
    if (A == "bibitem") {
        set(np_bibitem, Istring(B));
        return true;
    }
    if (A == "bibkey") {
        set(np_bibkey, Istring(B));
        return true;
    }
    if (A == "cell") {
        set(np_cell, Istring(B));
        return true;
    }
    if (A == "caption") {
        set(np_captions, Istring(B));
        return true;
    }
    if (A == "circle") {
        set(np_circle, Istring(B));
        return true;
    }
    if (A == "closecurve") {
        set(np_closecurve, Istring(B));
        return true;
    }
    if (A == "curve") {
        set(np_curve, Istring(B));
        return true;
    }
    if (A == "catperso") {
        set(np_catperso, Istring(B));
        return true;
    }
    if (A == "composition_ra") {
        set(cst_composition, Istring(B));
        return true;
    }
    if (A == "cleaders") {
        set(np_cleaders, Istring(B));
        return true;
    }
    if (A == "caps") {
        set(np_s_caps, Istring(B));
        return true;
    }
    if (A == "cit") {
        set(np_cit, Istring(B));
        return true;
    }
    if (A == "citation") {
        set(np_citation, Istring(B));
        return true;
    }
    if (A == "citetype") {
        set(np_cite_type, Istring(B));
        return true;
    }
    if (A == "dashline") {
        set(np_dashline, Istring(B));
        return true;
    }
    if (A == "div0") {
        set(np_div0, Istring(B));
        return true;
    }
    if (A == "div1") {
        set(np_div1, Istring(B));
        return true;
    }
    if (A == "div2") {
        set(np_div2, Istring(B));
        return true;
    }
    if (A == "div3") {
        set(np_div3, Istring(B));
        return true;
    }
    if (A == "div4") {
        set(np_div4, Istring(B));
        return true;
    }
    if (A == "div5") {
        set(np_div5, Istring(B));
        return true;
    }
    if (A == "div6") {
        set(np_div6, Istring(B));
        return true;
    }
    if (A == "drawline") {
        set(np_drawline, Istring(B));
        return true;
    }
    if (A == "dottedline") {
        set(np_dottedline, Istring(B));
        return true;
    }
    if (A == "eqnpos") {
        set(np_eqnpos, Istring(B));
        return true;
    }
    if (A == "footnote") {
        set(np_footnote, Istring(B));
        return true;
    }
    if (A == "formula") {
        set(np_formula, Istring(B));
        return true;
    }
    if (A == "fbox") {
        set(np_fbox, Istring(B));
        return true;
    }
    if (A == "figure") {
        set(np_figure, Istring(B));
        return true;
    }
    if (A == "figure_env") {
        set(np_float_figure, Istring(B));
        return true;
    }
    if (A == "frontmatter") {
        set(np_frontmatter, Istring(B));
        return true;
    }
    if (A == "font_small") {
        set(np_font_small, Istring(B));
        return true;
    }
    if (A == "font_small1") {
        set(np_font_small1, Istring(B));
        return true;
    }
    if (A == "font_small2") {
        set(np_font_small2, Istring(B));
        return true;
    }
    if (A == "font_small3") {
        set(np_font_small3, Istring(B));
        return true;
    }
    if (A == "font_small4") {
        set(np_font_small4, Istring(B));
        return true;
    }
    if (A == "font_small5") {
        set(np_font_small5, Istring(B));
        return true;
    }
    if (A == "font_small6") {
        set(np_font_small6, Istring(B));
        return true;
    }
    if (A == "font_large") {
        set(np_font_large, Istring(B));
        return true;
    }
    if (A == "font_large1") {
        set(np_font_large1, Istring(B));
        return true;
    }
    if (A == "font_large2") {
        set(np_font_large2, Istring(B));
        return true;
    }
    if (A == "font_large3") {
        set(np_font_large3, Istring(B));
        return true;
    }
    if (A == "font_large4") {
        set(np_font_large4, Istring(B));
        return true;
    }
    if (A == "font_large5") {
        set(np_font_large5, Istring(B));
        return true;
    }
    if (A == "font_large6") {
        set(np_font_large6, Istring(B));
        return true;
    }
    if (A == "font_normalsize") {
        set(np_font_normalsize, Istring(B));
        return true;
    }
    if (A == "font_upright") {
        set(np_font_upright, Istring(B));
        return true;
    }
    if (A == "font_medium") {
        set(np_font_medium, Istring(B));
        return true;
    }
    if (A == "font_roman") {
        set(np_font_roman, Istring(B));
        return true;
    }
    if (A == "font_it") {
        set(np_font_it, Istring(B));
        return true;
    }
    if (A == "font_slanted") {
        set(np_font_slanted, Istring(B));
        return true;
    }
    if (A == "font_sc") {
        set(np_font_sc, Istring(B));
        return true;
    }
    if (A == "font_tt") {
        set(np_font_tt, Istring(B));
        return true;
    }
    if (A == "font_sansserif") {
        set(np_font_sansserif, Istring(B));
        return true;
    }
    if (A == "font_bold") {
        set(np_font_bold, Istring(B));
        return true;
    }
    if (A == "font_boldextended") {
        set(np_font_boldextended, Istring(B));
        return true;
    }
    if (A == "font_semibold") {
        set(np_font_semibold, Istring(B));
        return true;
    }
    if (A == "font_condensed") {
        set(np_font_condensed, Istring(B));
        return true;
    }
    if (A == "gloitem") {
        set(np_label_glo, Istring(B));
        return true;
    }
    if (A == "graphics") {
        set(np_graphics, Istring(B));
        return true;
    }
    if (A == "glo") {
        set(np_glo_name, Istring(B));
        return true;
    }
    if (A == "glossary") {
        set(np_glossary, Istring(B));
        return true;
    }
    if (A == "head") {
        set(np_head, Istring(B));
        return true;
    }
    if (A == "hl") {
        set(np_s_hl, Istring(B));
        return true;
    }
    if (A == "item") {
        set(np_item, Istring(B));
        return true;
    }
    if (A == "index") {
        set(np_index, Istring(B));
        return true;
    }
    if (A == "keywords") {
        set(np_keywords, Istring(B));
        return true;
    }
    if (A == "labelitem") {
        set(np_label_item, Istring(B));
        return true;
    }
    if (A == "lineC") {
        set(np_lineC, Istring(B));
        return true;
    }
    if (A == "line") {
        set(np_line, Istring(B));
        return true;
    }
    if (A == "listoffigures") {
        set(np_toc2, Istring(B));
        return true;
    }
    if (A == "listoftables") {
        set(np_toc1, Istring(B));
        return true;
    }
    if (A == "llap") {
        set(np_llap, Istring(B));
        return true;
    }
    if (A == "linethickness") {
        set(np_line_thickness, Istring(B));
        return true;
    }
    if (A == "list") {
        set(np_list, Istring(B));
        return true;
    }
    if (A == "leaders") {
        set(np_leaders, Istring(B));
        return true;
    }
    if (A == "leg") {
        set(np_leg, Istring(B));
        return true;
    }
    if (A == "mbox") {
        set(np_mbox, Istring(B));
        return true;
    }
    if (A == "math") {
        set(cst_math, Istring(B));
        return true;
    }
    if (A == "multiput") {
        set(np_multiput, Istring(B));
        return true;
    }
    if (A == "mainmatter") {
        set(np_mainmatter, Istring(B));
        return true;
    }
    if (A == "node") {
        set(np_node, Istring(B));
        return true;
    }
    if (A == "nodeconnect") {
        set(np_nodeconnect, Istring(B));
        return true;
    }
    if (A == "nodecurve") {
        set(np_nodecurve, Istring(B));
        return true;
    }
    if (A == "nodetriangle") {
        set(np_nodetriangle, Istring(B));
        return true;
    }
    if (A == "nodecircle") {
        set(np_nodecircle, Istring(B));
        return true;
    }
    if (A == "nodebox") {
        set(np_nodebox, Istring(B));
        return true;
    }
    if (A == "nodeoval") {
        set(np_nodeoval, Istring(B));
        return true;
    }
    if (A == "natcit") {
        set(np_natcit, Istring(B));
        return true;
    }
    if (A == "oval") {
        set(np_oval, Istring(B));
        return true;
    }
    if (A == "oldstyle") {
        set(np_s_old, Istring(B));
        return true;
    }
    if (A == "overline") {
        set(np_overline, Istring(B));
        return true;
    }
    if (A == "picture") {
        set(np_picture, Istring(B));
        return true;
    }
    if (A == "put") {
        set(np_put, Istring(B));
        return true;
    }
    if (A == "project") {
        set(np_projet, Istring(B));
        return true;
    }
    if (A == "pers") {
        set(np_pers, Istring(B));
        return true;
    }
    if (A == "prenote") {
        set(np_prenote, Istring(B));
        return true;
    }
    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return true;
    }
    if (A == "row") {
        set(np_row, Istring(B));
        return true;
    }
    if (A == "raisebox") {
        set(np_raisebox, Istring(B));
        return true;
    }
    if (A == "rlap") {
        set(np_rlap, Istring(B));
        return true;
    }
    if (A == "rotatebox") {
        set(np_rotatebox, Istring(B));
        return true;
    }
    if (A == "ref") {
        set(np_ref, Istring(B));
        return true;
    }
    if ((A == "rclist") && ra_ok) {
        set(np_rclist, Istring(B));
        return true;
    }
    if ((A == "rcval") && ra_ok) {
        set(np_rcval, Istring(B));
        return true;
    }
    if ((A == "rasection") && ra_ok) {
        set(np_rasection, Istring(B));
        return true;
    }
    if (A == "subfigure") {
        set(np_subfigure, Istring(B));
        return true;
    }
    if (A == "scaleput") {
        set(np_scaleput, Istring(B));
        return true;
    }
    if (A == "scalebox") {
        set(np_sbox, Istring(B));
        return true;
    }
    if (A == "scaption") {
        set(np_caption, Istring(B));
        return true;
    }
    if (A == "sup") {
        set(np_s_sup, Istring(B));
        return true;
    }
    if (A == "sub") {
        set(np_s_sub, Istring(B));
        return true;
    }
    if (A == "so") {
        set(np_s_so, Istring(B));
        return true;
    }
    if (A == "st") {
        set(np_s_st, Istring(B));
        return true;
    }
    if (A == "stylesheet") {
        set(np_stylesheet, Istring(B));
        return true;
    }
    if (A == "stylesheettype") {
        set(np_stylesheet_type, Istring(B));
        return true;
    }
    if (A == "term") {
        set(np_term, Istring(B));
        return true;
    }
    if (A == "texmath") {
        set(np_texmath, Istring(B));
        return true;
    }
    if (A == "table") {
        set(np_table, Istring(B));
        return true;
    }
    if (A == "table_env") {
        set(np_float_table, Istring(B));
        return true;
    }
    if (A == "Table") {
        set(np_Table, Istring(B));
        return true;
    }
    if (A == "tagcurve") {
        set(np_tagcurve, Istring(B));
        return true;
    }
    if (A == "thicklines") {
        set(np_thick_lines, Istring(B));
        return true;
    }
    if (A == "thinlines") {
        set(np_thin_lines, Istring(B));
        return true;
    }
    if (A == "theorem_head") {
        set(np_alt_head, Istring(B));
        return true;
    }
    if (A == "theorem") {
        if (B[0] == 0) { // reverst to old behavior
            set(np_theorem, Istring(B));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else if (B[0] == ' ') { // special case
            set(np_theorem, Istring(B.substr(1)));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else {
            set(np_theorem, Istring(B));
            the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
        }
        return true;
    }
    if (A == "theindex") {
        set(np_theindex, Istring(B));
        return true;
    }
    if (A == "theglossary") {
        set(np_theglossary, Istring(B));
        return true;
    }
    if (A == "tableofcontents") {
        set(np_toc, Istring(B));
        return true;
    }
    if (A == "texte") {
        set(np_texte, Istring(B));
        return true;
    }
    if (A == "ul") {
        set(np_s_ul, Istring(B));
        return true;
    }
    if (A == "underline") {
        set(np_underline, Istring(B));
        return true;
    }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return true;
    }
    if (A == "vector") {
        set(np_vector, Istring(B));
        return true;
    }
    if (A == "warning") {
        set(np_warning, Istring(B));
        return true;
    }
    if (A == "xref") {
        set(np_xref, Istring(B));
        return true;
    }
    if (A == "xtheorem") {
        set(np_theorem, Istring(B));
        return true;
    }
    if (A == "xleaders") {
        set(np_xleaders, Istring(B));
        return true;
    }
    return false;
}
