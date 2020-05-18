#include "tralics/NameMapper.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

void NameMapper::def(const std::string &name, name_positions pos, const std::string &value) {
    set(pos, value);
    if (s_to_id.contains(name)) {
        spdlog::warn("Already present: {} {} {}", name, pos, value);
        return;
    }
    if (name.empty()) {
        spdlog::warn("Empty name");
        return;
    }
    s_to_id.emplace(name, pos);
}

void NameMapper::def(name_positions i, const std::string &s) { def(s, i, s); }

void NameMapper::boot() {
    def("cst_empty", cst_empty, "");
    def(cst_accent, "accent");
    def(cst_accentunder, "accentunder");
    def(cst_argument, "argument");
    def(cst_biblio, "thebibliography");
    def(cst_composition, "composition");
    def(cst_displaystyle, "displaystyle");
    def(cst_document, "document");
    def(cst_elt, "ELT");
    def(cst_flaghi, "'hi_flag");
    def(cst_fonts, "fonts");
    def(cst_foot, "foot");
    def(cst_hanl, "hanl");
    def(cst_hbox, "hbox");
    def(cst_hi, "hi");
    def(cst_math, "math");
    def(cst_mathml, "http://www.w3.org/1998/Math/MathML");
    def(cst_mathvariant, "mathvariant");
    def(cst_mfenced, "mfenced");
    def(cst_mfrac, "mfrac");
    def(cst_mpadded, "mpadded");
    def(cst_mphantom, "mphantom");
    def(cst_mi, "mi");
    def(cst_mn, "mn");
    def(cst_mo, "mo");
    def(cst_ci, "ci");
    def(cst_cn, "cn");
    def(cst_csymbol, "csymbol");
    def(cst_multiscripts, "mmultiscripts");
    def("cst_nothing", cst_nothing, "");
    def(cst_mroot, "mroot");
    def(cst_mover, "mover");
    def(cst_mrow, "mrow");
    def(cst_mrow_cst, "<mrow></mrow>");
    def(cst_mspace, "mspace");
    def(cst_msqrt, "msqrt");
    def(cst_msub, "msub");
    def(cst_mstyle, "mstyle");
    def(cst_msubsup, "msubsup");
    def(cst_msup, "msup");
    def(cst_mtable, "mtable");
    def(cst_mtd, "mtd");
    def(cst_mtext, "mtext");
    def(cst_mtr, "mtr");
    def(cst_munder, "munder");
    def(cst_mref, "mref");
    def(cst_munderover, "munderover");
    def(cst_mode, "mode");
    def(cst_display, "display");
    def(cst_nl, "\n");
    def(cst_p, "p");
    def(cst_particip, "particip");
    def(cst_rasection, "RAsection");
    def(cst_refer, "refer");
    def(cst_scriptlevel, "scriptlevel");
    def(cst_separators, "separators");
    def(cst_temporary, "temporary");
    def(cst_theorem, "theorem");
    def(cst_xmlns, "xmlns");
    def(cst_dig0, "0");
    def(cst_dig1, "1");
    def(cst_dig2, "2");
    def(cst_dig3, "3");
    def(cst_dig4, "4");
    def(cst_dig5, "5");
    def(cst_dig6, "6");
    def(cst_dig7, "7");
    def(cst_dig8, "8");
    def(cst_dig9, "9");
    def(cstb_article, "article");
    def(cstb_book, "book");
    def(cstb_booklet, "booklet");
    def(cstb_conference, "conference");
    def(cstb_coursenotes, "coursenotes");
    def(cstb_comment, "comment");
    def(cstb_extension, "extension");
    def(cstb_inbook, "inbook");
    def(cstb_incollection, "incollection");
    def(cstb_inproceedings, "inproceedings");
    def(cstb_manual, "manual");
    def(cstb_masterthesis, "mastersthesis");
    def(cstb_mastersthesis, "masterthesis");
    def(cstb_misc, "misc");
    def(cstb_phdthesis, "phdthesis");
    def(cstb_proceedings, "proceedings");
    def(cstb_preamble, "preamble");
    def(cstb_techreport, "techreport");
    def(cstb_string, "string");
    def(cstb_unpublished, "unpublished");
    def(cstb_unknown, "unknown");
    def(cstb_address, "address");
    def(cstb_author, "author");
    def(cstb_booktitle, "booktitle");
    def(cstb_chapter, "chapter");
    def(cstb_crossref, "crossref");
    def(cstb_doi, "doi");
    def(cstb_edition, "edition");
    def(cstb_editor, "editor");
    def(cstb_howpublished, "howpublished");
    def(cstb_institution, "institution");
    def(cstb_isbn, "isbn");
    def(cstb_issn, "issn");
    def(cstb_isrn, "isrn");
    def(cstb_journal, "journal");
    def(cstb_key, "key");
    def(cstb_month, "month");
    def(cstb_language, "language");
    def(cstb_langue, "langue");
    def(cstb_note, "note");
    def(cstb_number, "number");
    def(cstb_organization, "organization");
    def(cstb_pages, "pages");
    def(cstb_publisher, "publisher");
    def(cstb_school, "school");
    def(cstb_series, "series");
    def(cstb_title, "title");
    def(cstb_type, "type");
    def(cstb_url, "url");
    def(cstb_volume, "volume");
    def(cstb_year, "year");
    def(cstb_equipe, "equipe");
    def(cstb_subtype, "subtype");
    def(cstb_unite, "unite");
    def(cstb_statut, "statut");
    def(cstb_section, "section");
    def(cst_numalign, "numalign");
    def(cst_denalign, "denomalign");
    def(cstf_normal, "normal");
    def("cstf_upright", cstf_upright, "");
    def(cstf_bold, "bold");
    def(cstf_italic, "italic");
    def(cstf_bold_italic, "bold-italic");
    def(cstf_script, "script");
    def(cstf_bold_script, "bold-script");
    def(cstf_fraktur, "fraktur");
    def(cstf_doublestruck, "double-struck");
    def(cstf_bold_fraktur, "bold-fraktur");
    def(cstf_sansserif, "sans-serif");
    def(cstf_bold_sansserif, "bold-sans-serif");
    def(cstf_sansserif_italic, "sans-serif-italic");
    def(cstf_sansserif_bold_italic, "sans-serif-bold-italic");
    def(cstf_monospace, "monospace");
    def(np_a1, "a1");
    def(np_a2, "a2");
    def(np_abarnodeconnect, "abarnodeconnect");
    def(np_accueil, "accueil");
    def(np_affiliation, "affiliation");
    def(np_allowbreak, "allowbreak");
    def(np_alt_head, "alt_head");
    def(np_alt_caption, "alt_head");
    def(np_alt_section, "alt_head");
    def(np_anchor, "anchor");
    def(np_angle, "angle");
    def(np_anodeconnect, "anodeconnect");
    def(np_anodecurve, "anodecurve");
    def(np_arc, "pic-arc");
    def(np_arg1, "arg1");
    def(np_arg2, "arg2");
    def(np_arg3, "arg3");
    def(np_array, "array");
    def(np_alternatives, "alternatives");
    def(np_b1, "b1");
    def(np_b2, "b2");
    def(np_b_rend, "rend");
    def(np_backmatter, "backmatter");
    def(np_bauteurs, "bauteurs");
    def(np_barnodeconnect, "barnodeconnect");
    def(np_bediteurs, "bediteur");
    def(np_bezier, "pic-bezier");
    def(np_biblio, "biblio");
    def(np_bigcircle, "pic-bigcircle");
    def(np_bottomborder, "bottom-border");
    def(np_border_bottomw, "border-bottom-width");
    def(np_border_topw, "border-top-width");
    def(np_border_leftw, "border-left-width");
    def(np_border_rightw, "border-right-width");
    def(np_box, "pic-framebox");
    def(np_box_pos, "position");
    def(np_boxed, "boxed");
    def(np_bpers, "bpers");
    def(np_box_width, "width");
    def(np_bibitem, "bibitem");
    def(np_bibkey, "bib-key");
    def(np_c1, "c1");
    def(np_c2, "c2");
    def(np_c_left, "left");
    def(np_c_right, "right");
    def(np_c_center, "center");
    def(np_caption, "caption");
    def(np_captions, "head");
    def(np_catperso, "catperso");
    def(np_cell, "cell");
    def(np_center, "center");
    def(np_center_etc, "center");
    def(np_center_etc1, "center");
    def(np_center_etc2, "quoted");
    def(np_center_etc3, "quoted");
    def(np_center_etc4, "flushed-left");
    def(np_center_etc5, "flushed-right");
    def(np_center_etc6, "verse");
    def(np_centered, "centered");
    def(np_chapters, "chapters");
    def(np_circle, "pic-circle");
    def(np_natcit, "Cit");
    def(np_cit, "cit");
    def(np_cite_type, "type");
    def(np_citation, "citation");
    def(np_cleaders, "cleaders");
    def(np_clip, "clip");
    def(np_close, "close");
    def(np_closecurve, "pic-closecurve");
    def(np_columnalign, "columnalign");
    def(np_columnspan, "columnspan");
    def(np_pagecolor, "pagecolor");
    def(np_color, "color");
    def(np_color2, "color2");
    def(np_cols, "cols");
    def(np_cst_width, "width");
    def(np_curve, "pic-curve");
    def(np_curve_nbpts, "nbsymb");
    def(np_dashbox, "pic-dashbox");
    def(np_dashline, "dashline");
    def(np_dashdim, "dashdim");
    def(np_depth, "depth");
    def(np_depthA, "depthA");
    def(np_depthB, "depthB");
    def(np_description, "description");
    def(np_display, "display");
    def(cst_div0, "div0");
    def(cst_div1, "div1");
    def(cst_div2, "div2");
    def(cst_div3, "div3");
    def(cst_div4, "div4");
    def(cst_div5, "div5");
    def(cst_div6, "div6");
    def(np_div0, "div0");
    def(np_div1, "div1");
    def(np_div2, "div2");
    def(np_div3, "div3");
    def(np_div4, "div4");
    def(np_div5, "div5");
    def(np_div6, "div6");
    def(np_dottedline, "dottedline");
    def(np_drawline, "drawline");
    def(np_dx, "dx");
    def(np_dy, "dy");
    def(np_encap, "encap");
    def(np_english, "english");
    def(np_eqnpos, "eqnpos");
    def(np_error, "error");
    def(np_Error, "Error");
    def("np_warning", np_warning, "");
    def(np_Warning, "Warning");
    def(np_Info, "Info");
    def(np_inline, "inline");
    def(np_false, "false");
    def(np_fbox, "fbox");
    def(np_figure, "figure");
    def(np_file, "file");
    def(np_fileextension, "extension");
    def(np_float, "float");
    def(np_float_figure, "figure");
    def(np_float_table, "table");
    def(np_font_bold, "bold");
    def(np_font_boldextended, "boldextended");
    def(np_font_semibold, "semibold");
    def(np_font_condensed, "condensed");
    def(np_font_it, "it");
    def(np_font_large, "large");
    def(np_font_large1, "large1");
    def(np_font_large2, "large2");
    def(np_font_large3, "large3");
    def(np_font_large4, "large4");
    def(np_font_large5, "large5");
    def(np_font_large6, "large6");
    def(np_font_medium, "medium");
    def(np_font_normalsize, "normalsize");
    def(np_font_roman, "roman");
    def(np_font_sansserif, "sansserif");
    def(np_font_sc, "sc");
    def(np_font_slanted, "slanted");
    def(np_font_small, "small");
    def(np_font_small1, "small1");
    def(np_font_small2, "small2");
    def(np_font_small3, "small3");
    def(np_font_small4, "small4");
    def(np_font_small5, "small5");
    def(np_font_small6, "small6");
    def(np_font_tt, "tt");
    def(np_font_upright, "upright");
    def(np_footnote, "note");
    def(np_form, "form");
    def(np_formula, "formula");
    def(np_foot, "foot");
    def(np_frame, "pic-frame");
    def(np_framed, "framed");
    def(np_french, "french");
    def(np_from, "from");
    def(np_frontmatter, "frontmatter");
    def(np_full, "full");
    def(np_full_first, "prenomcomplet");
    def(np_german, "german");
    def(np_glo_name, "Glossary");
    def(np_gloss, "gloss");
    def(np_graphics, "graphics");
    def(np_halfem, "4.pt");
    def(np_halign, "halign");
    def(np_hdr, "hdr");
    def(np_head, "head");
    def(np_header, "headings");
    def(np_height, "height");
    def(np_hfil, "hfil");
    def(np_hfill, "hfill");
    def(np_hfilneg, "hfilneg");
    def(np_html, "html");
    def(np_hss, "hss");
    def(np_id, "id");
    def(np_idtext, "id-text");
    def(np_index, "index");
    def(np_glossary, "glossary");
    def(np_inline, "inline");
    def(np_isproject, "isproject");
    def(np_item, "item");
    def(np_junior, "junior");
    def(np_key, "key");
    def(np_keywords, "keywords");
    def(np_label_glo, "label");
    def(np_label_item, "label");
    def("np_language", np_language, "");
    def(np_leaders, "leaders");
    def(np_left, "left");
    def(np_leftborder, "left-border");
    def(np_leg, "leg");
    def(np_level, "level");
    def(np_linethickness, "linethickness");
    def(np_lineC, "line");
    def(np_line, "pic-line");
    def(np_line_thickness, "pic-linethickness");
    def(np_list, "list");
    def(np_llap, "llap");
    def(np_rlap, "rlap");
    def("nb_rasection", np_rasection, "");
    def(np_mainmatter, "mainmatter");
    def(np_mbox, "mbox");
    def(np_marginpar, "marginpar");
    def(np_minipage, "minipage");
    def(np_minipage_width, "width");
    def(np_module, "module");
    def(np_moreinfo, "moreinfo");
    def(np_movablelimits, "movablelimits");
    def(np_multiput, "pic-multiput");
    def(np_name, "name");
    def(np_nameA, "nameA");
    def(np_nameB, "nameB");
    def(np_node, "node");
    def(np_nodebox, "nodebox");
    def(np_nodecircle, "nodecircle");
    def(np_nodeconnect, "nodeconnect");
    def(np_nodecurve, "nodecurve");
    def(np_nodeoval, "nodeoval");
    def(np_nodetriangle, "nodetriangle");
    def(np_noindent, "noindent");
    def(np_nom, "nom");
    def(np_nonumber, "nonumber");
    def(np_open, "open");
    def(np_ordered, "ordered");
    def(np_oval, "pic-oval");
    def(np_overline, "overline");
    def(np_page, "page");
    def(np_part, "part");
    def(np_part0, "participant");
    def(np_part1, "participante");
    def(np_part2, "participantes");
    def(np_part3, "participants");
    def(np_particle, "part");
    def(np_pers, "pers");
    def(np_place, "place");
    def(np_picture, "picture");
    def(np_point, "point");
    def(np_pos, "pos");
    def(np_posA, "posA");
    def(np_posB, "posB");
    def(np_posi, "inner-pos");
    def(np_pre, "pre");
    def(np_prefix, "prefix");
    def(np_prenom, "prenom");
    def(np_prenote, "prenote");
    def(np_profession, "profession");
    def(np_research_centre, "research-centre");
    def(np_projet, "projet");
    def(np_projetdeveloppe, "projetdeveloppe");
    def(np_put, "pic-put");
    def(np_quoted, "quoted");
    def(np_r_angle, "angle");
    def(np_raisebox, "raisebox");
    def(np_ref, "ref");
    def(np_rend, "rend");
    def(np_repeat, "repeat");
    def(np_right, "right");
    def(np_rightborder, "right-border");
    def(np_rotatebox, "pic-rotatebox");
    def(np_row, "row");
    def(np_rule, "rule");
    def(np_rule_tsa, "top_rule_space_above");
    def(np_rule_tsb, "top_rule_space_below");
    def(np_rule_bsa, "bottom_rule_space_above");
    def(np_rule_bsb, "bottom_rule_space_below");
    def(np_rclist, "UR");
    def(np_rcval, "+UR");
    def(np_sbox, "scalebox");
    def(np_sc, "sc");
    def(np_scale, "scale");
    def(np_scaleput, "pic-scaleput");
    def(np_s_caps, "caps");
    def(np_s_hl, "hl");
    def(np_s_old, "oldstyle");
    def(np_s_scale, "scale");
    def(np_s_so, "so");
    def(np_s_st, "st");
    def(np_s_sub, "sub");
    def(np_s_sup, "sup");
    def(np_s_ul, "ul");
    def("np_separator", np_separator, "");
    def(np_simple, "simple");
    def(np_simplemath, "simplemath");
    def(np_size, "size");
    def(np_spaceafter, "spaceafter");
    def(np_spacebefore, "spacebefore");
    def(np_specs, "specs");
    def(np_style, "style");
    def(np_starred, "starred");
    def(np_subfigure, "subfigure");
    def("np_stylesheet", np_stylesheet, "");
    def(np_stylesheet_type, "text/css");
    def(np_table, "table");
    def(np_Table, "Table");
    def(np_tabular, "tabular");
    def(np_tabular_star, "tabular*");
    def(np_tab_width, "width");
    def(np_tagcurve, "pic-tagcurve");
    def(np_target, "target");
    def(np_term, "term");
    def(np_texte, "texte");
    def(np_texmath, "texmath");
    def(np_theindex, "theindex");
    def(np_theglossary, "theglossary");
    def(np_theme, "theme");
    def("np_theorem", np_theorem, "");
    def(np_thick_lines, "pic-thicklines");
    def(np_thin_lines, "pic-thinlines");
    def(np_toc, "tableofcontents");
    def(np_toc1, "listoftables");
    def(np_toc2, "listoffigures");
    def(np_topborder, "top-border");
    def(np_totalwidth, "totalwidth");
    def(np_true, "true");
    def(np_type, "type");
    def(np_textype, "textype");
    def(np_underline, "underline");
    def(np_unknown, "unknown");
    def(np_unit_length, "unit-length");
    def(np_url, "url");
    def(np_userid, "userid");
    def(np_user_list, "description");
    def(np_val, "val");
    def(np_vector, "pic-vector");
    def(np_vfil, "vfil");
    def(np_vfill, "vfill");
    def(np_vfilneg, "vfilneg");
    def(np_vpos, "vpos");
    def(np_vss, "vss");
    def(np_width, "width");
    def(np_xscale, "xscale");
    def(np_xscaley, "xscaley");
    def(np_xdir, "xdir");
    def(np_xleaders, "xleaders");
    def(np_xpos, "xpos");
    def(np_xref, "xref");
    def(np_yscale, "yscale");
    def(np_yscalex, "yscalex");
    def(np_ydir, "ydir");
    def(np_ypos, "ypos");
    def(np_zerodim, "0pt");
    def(np_mml_normal, "mml@font@normal");
    def(np_mml_upright, "mml@font@upright");
    def(np_mml_bold, "mml@font@bold");
    def(np_mml_italic, "mml@font@italic");
    def(np_mml_bold_italic, "mml@font@bolditalic");
    def(np_mml_script, "mml@font@script");
    def(np_mml_bold_script, "mml@font@boldscript");
    def(np_mml_fraktur, "mml@font@fraktur");
    def(np_mml_doublestruck, "mml@font@doublestruck");
    def(np_mml_bold_fraktur, "mml@font@boldfraktur");
    def(np_mml_sansserif, "mml@font@sansserif");
    def(np_mml_bold_sansserif, "mml@font@boldsansserif");
    def(np_mml_sansserif_italic, "mml@font@sansserifitalic");
    def(np_mml_sansserif_bold_italic, "mml@font@sansserifbolditalic");
    def(np_mml_monospace, "mml@font@monospace");
    def(np_3pt, "3pt");
    def(np_6pt, "6pt");
    def(np_12pt, "12pt");
    def(np_letters_tl, "tl");
    def(np_letters_tr, "tr");
    def(np_letters_bl, "bl");
    def(np_letters_br, "br");
    std::array<char, 2> foo{};
    foo[1] = 0;
    for (char x = 'a'; x <= 'z'; x++) {
        foo[0] = x;
        def(name_positions(to_unsigned(np_letter_a + x - 'a')), foo.data());
    }
    for (char x = 'A'; x <= 'Z'; x++) {
        foo[0] = x;
        def(name_positions(to_unsigned(np_letter_A + x - 'A')), foo.data());
    }
}

auto NameMapper::assign(const std::string &sa, const std::string &sb) -> bool {
    auto n = sa.size();

    if (sa.starts_with("elt_")) return the_names.assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return the_names.assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
    if (sa.starts_with("att_")) return the_names.assign_att(sa.substr(4), sb);

    if (sa == "lang_fr") {
        set(np_french, sb);
        return true;
    }
    if (sa == "lang_en") {
        set(np_english, sb);
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
            set(np_separator, "");
        else
            set(np_separator, sb);
        return true;
    }
    if (sa.ends_with("_vals")) {
        Buffer B(sb);
        config_ns::interpret_list(sa.substr(0, n - 5), B); // \todo without Buffer
        return true;
    }
    if (sa == "mml_font_normal") {
        set(np_mml_normal, sb);
        return true;
    }
    if (sa == "mml_font_upright") {
        set(np_mml_upright, sb);
        return true;
    }
    if (sa == "mml_font_bold") {
        set(np_mml_bold, sb);
        return true;
    }
    if (sa == "mml_font_italic") {
        set(np_mml_italic, sb);
        return true;
    }
    if (sa == "mml_font_bold_italic") {
        set(np_mml_bold_italic, sb);
        return true;
    }
    if (sa == "mml_font_script") {
        set(np_mml_script, sb);
        return true;
    }
    if (sa == "mml_font_bold_script") {
        set(np_mml_bold_script, sb);
        return true;
    }
    if (sa == "mml_font_fraktur") {
        set(np_mml_fraktur, sb);
        return true;
    }
    if (sa == "mml_font_doublestruck") {
        set(np_mml_doublestruck, sb);
        return true;
    }
    if (sa == "mml_font_bold_fraktur") {
        set(np_mml_bold_fraktur, sb);
        return true;
    }
    if (sa == "mml_font_sansserif") {
        set(np_mml_sansserif, sb);
        return true;
    }
    if (sa == "mml_font_bold_sansserif") {
        set(np_mml_bold_sansserif, sb);
        return true;
    }
    if (sa == "mml_font_sansserif_italic") {
        set(np_mml_sansserif_italic, sb);
        return true;
    }
    if (sa == "mml_font_sansserif_bold_italic") {
        set(np_mml_sansserif_bold_italic, sb);
        return true;
    }
    if (sa == "mml_font_monospace") {
        set(np_mml_monospace, sb);
        return true;
    }
    return false;
}

auto NameMapper::assign_name(const std::string &A, const std::string &B) -> bool {
    if (A == "accueil") {
        set(np_accueil, B);
        return true;
    }
    if (A == "arc") {
        set(np_arc, B);
        return true;
    }
    if (A == "allowbreak") {
        set(np_allowbreak, B);
        return true;
    }
    if (A == "anchor") {
        set(np_anchor, B);
        return true;
    }
    if (A == "anodeconnect") {
        set(np_anodeconnect, B);
        return true;
    }
    if (A == "abarnodeconnect") {
        set(np_abarnodeconnect, B);
        return true;
    }
    if (A == "anodecurve") {
        set(np_anodecurve, B);
        return true;
    }
    if (A == "alt_caption") {
        set(np_alt_caption, B);
        return true;
    }
    if (A == "alt_section") {
        set(np_alt_section, B);
        return true;
    }
    if (A == "alternatives") {
        set(np_alternatives, B);
        return true;
    }
    if (A == "box") {
        set(np_box, B);
        return true;
    }
    if (A == "bezier") {
        set(np_bezier, B);
        return true;
    }
    if (A == "bigcircle") {
        set(np_bigcircle, B);
        return true;
    }
    if (A == "backmatter") {
        set(np_backmatter, B);
        return true;
    }
    if (A == "biblio") {
        set(np_biblio, B);
        return true;
    }
    if (A == "bpers") {
        set(np_bpers, B);
        return true;
    }
    if (A == "barnodeconnect") {
        set(np_barnodeconnect, B);
        return true;
    }
    if (A == "bibitem") {
        set(np_bibitem, B);
        return true;
    }
    if (A == "bibkey") {
        set(np_bibkey, B);
        return true;
    }
    if (A == "cell") {
        set(np_cell, B);
        return true;
    }
    if (A == "caption") {
        set(np_captions, B);
        return true;
    }
    if (A == "circle") {
        set(np_circle, B);
        return true;
    }
    if (A == "closecurve") {
        set(np_closecurve, B);
        return true;
    }
    if (A == "curve") {
        set(np_curve, B);
        return true;
    }
    if (A == "catperso") {
        set(np_catperso, B);
        return true;
    }
    if (A == "composition_ra") {
        set(cst_composition, B);
        return true;
    }
    if (A == "cleaders") {
        set(np_cleaders, B);
        return true;
    }
    if (A == "caps") {
        set(np_s_caps, B);
        return true;
    }
    if (A == "cit") {
        set(np_cit, B);
        return true;
    }
    if (A == "citation") {
        set(np_citation, B);
        return true;
    }
    if (A == "citetype") {
        set(np_cite_type, B);
        return true;
    }
    if (A == "dashline") {
        set(np_dashline, B);
        return true;
    }
    if (A == "div0") {
        set(np_div0, B);
        return true;
    }
    if (A == "div1") {
        set(np_div1, B);
        return true;
    }
    if (A == "div2") {
        set(np_div2, B);
        return true;
    }
    if (A == "div3") {
        set(np_div3, B);
        return true;
    }
    if (A == "div4") {
        set(np_div4, B);
        return true;
    }
    if (A == "div5") {
        set(np_div5, B);
        return true;
    }
    if (A == "div6") {
        set(np_div6, B);
        return true;
    }
    if (A == "drawline") {
        set(np_drawline, B);
        return true;
    }
    if (A == "dottedline") {
        set(np_dottedline, B);
        return true;
    }
    if (A == "eqnpos") {
        set(np_eqnpos, B);
        return true;
    }
    if (A == "footnote") {
        set(np_footnote, B);
        return true;
    }
    if (A == "formula") {
        set(np_formula, B);
        return true;
    }
    if (A == "fbox") {
        set(np_fbox, B);
        return true;
    }
    if (A == "figure") {
        set(np_figure, B);
        return true;
    }
    if (A == "figure_env") {
        set(np_float_figure, B);
        return true;
    }
    if (A == "frontmatter") {
        set(np_frontmatter, B);
        return true;
    }
    if (A == "font_small") {
        set(np_font_small, B);
        return true;
    }
    if (A == "font_small1") {
        set(np_font_small1, B);
        return true;
    }
    if (A == "font_small2") {
        set(np_font_small2, B);
        return true;
    }
    if (A == "font_small3") {
        set(np_font_small3, B);
        return true;
    }
    if (A == "font_small4") {
        set(np_font_small4, B);
        return true;
    }
    if (A == "font_small5") {
        set(np_font_small5, B);
        return true;
    }
    if (A == "font_small6") {
        set(np_font_small6, B);
        return true;
    }
    if (A == "font_large") {
        set(np_font_large, B);
        return true;
    }
    if (A == "font_large1") {
        set(np_font_large1, B);
        return true;
    }
    if (A == "font_large2") {
        set(np_font_large2, B);
        return true;
    }
    if (A == "font_large3") {
        set(np_font_large3, B);
        return true;
    }
    if (A == "font_large4") {
        set(np_font_large4, B);
        return true;
    }
    if (A == "font_large5") {
        set(np_font_large5, B);
        return true;
    }
    if (A == "font_large6") {
        set(np_font_large6, B);
        return true;
    }
    if (A == "font_normalsize") {
        set(np_font_normalsize, B);
        return true;
    }
    if (A == "font_upright") {
        set(np_font_upright, B);
        return true;
    }
    if (A == "font_medium") {
        set(np_font_medium, B);
        return true;
    }
    if (A == "font_roman") {
        set(np_font_roman, B);
        return true;
    }
    if (A == "font_it") {
        set(np_font_it, B);
        return true;
    }
    if (A == "font_slanted") {
        set(np_font_slanted, B);
        return true;
    }
    if (A == "font_sc") {
        set(np_font_sc, B);
        return true;
    }
    if (A == "font_tt") {
        set(np_font_tt, B);
        return true;
    }
    if (A == "font_sansserif") {
        set(np_font_sansserif, B);
        return true;
    }
    if (A == "font_bold") {
        set(np_font_bold, B);
        return true;
    }
    if (A == "font_boldextended") {
        set(np_font_boldextended, B);
        return true;
    }
    if (A == "font_semibold") {
        set(np_font_semibold, B);
        return true;
    }
    if (A == "font_condensed") {
        set(np_font_condensed, B);
        return true;
    }
    if (A == "gloitem") {
        set(np_label_glo, B);
        return true;
    }
    if (A == "graphics") {
        set(np_graphics, B);
        return true;
    }
    if (A == "glo") {
        set(np_glo_name, B);
        return true;
    }
    if (A == "glossary") {
        set(np_glossary, B);
        return true;
    }
    if (A == "head") {
        set(np_head, B);
        return true;
    }
    if (A == "hl") {
        set(np_s_hl, B);
        return true;
    }
    if (A == "item") {
        set(np_item, B);
        return true;
    }
    if (A == "index") {
        set(np_index, B);
        return true;
    }
    if (A == "keywords") {
        set(np_keywords, B);
        return true;
    }
    if (A == "labelitem") {
        set(np_label_item, B);
        return true;
    }
    if (A == "lineC") {
        set(np_lineC, B);
        return true;
    }
    if (A == "line") {
        set(np_line, B);
        return true;
    }
    if (A == "listoffigures") {
        set(np_toc2, B);
        return true;
    }
    if (A == "listoftables") {
        set(np_toc1, B);
        return true;
    }
    if (A == "llap") {
        set(np_llap, B);
        return true;
    }
    if (A == "linethickness") {
        set(np_line_thickness, B);
        return true;
    }
    if (A == "list") {
        set(np_list, B);
        return true;
    }
    if (A == "leaders") {
        set(np_leaders, B);
        return true;
    }
    if (A == "leg") {
        set(np_leg, B);
        return true;
    }
    if (A == "mbox") {
        set(np_mbox, B);
        return true;
    }
    if (A == "math") {
        set(cst_math, B);
        return true;
    }
    if (A == "multiput") {
        set(np_multiput, B);
        return true;
    }
    if (A == "mainmatter") {
        set(np_mainmatter, B);
        return true;
    }
    if (A == "node") {
        set(np_node, B);
        return true;
    }
    if (A == "nodeconnect") {
        set(np_nodeconnect, B);
        return true;
    }
    if (A == "nodecurve") {
        set(np_nodecurve, B);
        return true;
    }
    if (A == "nodetriangle") {
        set(np_nodetriangle, B);
        return true;
    }
    if (A == "nodecircle") {
        set(np_nodecircle, B);
        return true;
    }
    if (A == "nodebox") {
        set(np_nodebox, B);
        return true;
    }
    if (A == "nodeoval") {
        set(np_nodeoval, B);
        return true;
    }
    if (A == "natcit") {
        set(np_natcit, B);
        return true;
    }
    if (A == "oval") {
        set(np_oval, B);
        return true;
    }
    if (A == "oldstyle") {
        set(np_s_old, B);
        return true;
    }
    if (A == "overline") {
        set(np_overline, B);
        return true;
    }
    if (A == "picture") {
        set(np_picture, B);
        return true;
    }
    if (A == "put") {
        set(np_put, B);
        return true;
    }
    if (A == "project") {
        set(np_projet, B);
        return true;
    }
    if (A == "pers") {
        set(np_pers, B);
        return true;
    }
    if (A == "prenote") {
        set(np_prenote, B);
        return true;
    }
    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return true;
    }
    if (A == "row") {
        set(np_row, B);
        return true;
    }
    if (A == "raisebox") {
        set(np_raisebox, B);
        return true;
    }
    if (A == "rlap") {
        set(np_rlap, B);
        return true;
    }
    if (A == "rotatebox") {
        set(np_rotatebox, B);
        return true;
    }
    if (A == "ref") {
        set(np_ref, B);
        return true;
    }
    if ((A == "rclist") && ra_ok) {
        set(np_rclist, B);
        return true;
    }
    if ((A == "rcval") && ra_ok) {
        set(np_rcval, B);
        return true;
    }
    if ((A == "rasection") && ra_ok) {
        set(np_rasection, B);
        return true;
    }
    if (A == "subfigure") {
        set(np_subfigure, B);
        return true;
    }
    if (A == "scaleput") {
        set(np_scaleput, B);
        return true;
    }
    if (A == "scalebox") {
        set(np_sbox, B);
        return true;
    }
    if (A == "scaption") {
        set(np_caption, B);
        return true;
    }
    if (A == "sup") {
        set(np_s_sup, B);
        return true;
    }
    if (A == "sub") {
        set(np_s_sub, B);
        return true;
    }
    if (A == "so") {
        set(np_s_so, B);
        return true;
    }
    if (A == "st") {
        set(np_s_st, B);
        return true;
    }
    if (A == "stylesheet") {
        set(np_stylesheet, B);
        return true;
    }
    if (A == "stylesheettype") {
        set(np_stylesheet_type, B);
        return true;
    }
    if (A == "term") {
        set(np_term, B);
        return true;
    }
    if (A == "texmath") {
        set(np_texmath, B);
        return true;
    }
    if (A == "table") {
        set(np_table, B);
        return true;
    }
    if (A == "table_env") {
        set(np_float_table, B);
        return true;
    }
    if (A == "Table") {
        set(np_Table, B);
        return true;
    }
    if (A == "tagcurve") {
        set(np_tagcurve, B);
        return true;
    }
    if (A == "thicklines") {
        set(np_thick_lines, B);
        return true;
    }
    if (A == "thinlines") {
        set(np_thin_lines, B);
        return true;
    }
    if (A == "theorem_head") {
        set(np_alt_head, B);
        return true;
    }
    if (A == "theorem") {
        if (B[0] == 0) { // reverst to old behavior
            set(np_theorem, B);
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else if (B[0] == ' ') { // special case
            set(np_theorem, B.substr(1));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else {
            set(np_theorem, B);
            the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
        }
        return true;
    }
    if (A == "theindex") {
        set(np_theindex, B);
        return true;
    }
    if (A == "theglossary") {
        set(np_theglossary, B);
        return true;
    }
    if (A == "tableofcontents") {
        set(np_toc, B);
        return true;
    }
    if (A == "texte") {
        set(np_texte, B);
        return true;
    }
    if (A == "ul") {
        set(np_s_ul, B);
        return true;
    }
    if (A == "underline") {
        set(np_underline, B);
        return true;
    }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return true;
    }
    if (A == "vector") {
        set(np_vector, B);
        return true;
    }
    if (A == "warning") {
        set(np_warning, B);
        return true;
    }
    if (A == "xref") {
        set(np_xref, B);
        return true;
    }
    if (A == "xtheorem") {
        set(np_theorem, B);
        return true;
    }
    if (A == "xleaders") {
        set(np_xleaders, B);
        return true;
    }
    return false;
}

auto NameMapper::assign_att(const std::string &A, const std::string &B) -> bool {
    if (A == "angle") {
        set(np_angle, B);
        return true;
    }
    if (A == "affiliation") {
        set(np_affiliation, B);
        return true;
    }
    if (A == "box_pos") {
        set(np_box_pos, B);
        return true;
    }
    if (A == "box_scale") {
        set(np_s_scale, B);
        return true;
    }
    if (A == "box_width") {
        set(np_box_width, B);
        return true;
    }
    if (A == "boxed") {
        set(np_boxed, B);
        return true;
    }
    if (A == "bibkey") {
        set(np_bibkey, B);
        return true;
    }
    if (A == "border_bottom_width") {
        set(np_border_bottomw, B);
        return true;
    }
    if (A == "border_top_width") {
        set(np_border_topw, B);
        return true;
    }
    if (A == "border_left_width") {
        set(np_border_leftw, B);
        return true;
    }
    if (A == "border_right_width") {
        set(np_border_rightw, B);
        return true;
    }
    if (A == "centering") {
        set(np_center_etc1, B);
        return true;
    }
    if (A == "clip") {
        set(np_clip, B);
        return true;
    }
    if (A == "cols") {
        set(np_cols, B);
        return true;
    }
    if (A == "cell_left") {
        set(np_c_left, B);
        return true;
    }
    if (A == "cell_right") {
        set(np_c_right, B);
        return true;
    }
    if (A == "cell_center") {
        set(np_c_center, B);
        return true;
    }
    if (A == "cell_leftborder") {
        set(np_leftborder, B);
        return true;
    }
    if (A == "cell_rightborder") {
        set(np_rightborder, B);
        return true;
    }
    if (A == "cell_topborder") {
        set(np_topborder, B);
        return true;
    }
    if (A == "cell_bottomborder") {
        set(np_bottomborder, B);
        return true;
    }
    if (A == "curve_nbpts") {
        set(np_curve_nbpts, B);
        return true;
    }
    if (A == "display") {
        set(np_display, B);
        return true;
    }
    if (A == "dx") {
        set(np_dx, B);
        return true;
    }
    if (A == "dy") {
        set(np_dy, B);
        return true;
    }
    if (A == "depthA") {
        set(np_depthA, B);
        return true;
    }
    if (A == "depthB") {
        set(np_depthB, B);
        return true;
    }
    if (A == "depth") {
        set(np_depth, B);
        return true;
    }
    if (A == "encap") {
        set(np_encap, B);
        return true;
    }
    if (A == "framed") {
        set(np_framed, B);
        return true;
    }
    if (A == "full") {
        set(np_full, B);
        return true;
    }
    if (A == "file") {
        set(np_file, B);
        return true;
    }
    if (A == "file_extension") {
        set(np_fileextension, B);
        return true;
    }
    if (A == "fbox_rend") {
        set(np_b_rend, B);
        return true;
    }
    if (A == "flush_left") {
        set(np_center_etc4, B);
        return true;
    }
    if (A == "flush_right") {
        set(np_center_etc5, B);
        return true;
    }
    if (A == "foot_position") {
        set(np_foot, B);
        return true;
    }
    if (A == "full_first") {
        set(np_full_first, B);
        return true;
    }
    if (A == "gloss_type") {
        set(np_gloss, B);
        return true;
    }
    if (A == "height") {
        set(np_height, B);
        return true;
    }
    if (A == "halign") {
        set(np_halign, B);
        return true;
    }
    if (A == "hdr") {
        set(np_hdr, B);
        return true;
    }
    if (A == "inner_pos") {
        set(np_posi, B);
        return true;
    }
    if (A == "inline") {
        set(np_inline, B);
        return true;
    }
    if (A == "junior") {
        set(np_junior, B);
        return true;
    }
    if (A == "language") {
        set(np_language, B);
        return true;
    }
    if (A == "level") {
        set(np_level, B);
        return true;
    }
    if (A == "minipage_width") {
        set(np_minipage_width, B);
        return true;
    }
    if (A == "marginpar") {
        set(np_marginpar, B);
        return true;
    }
    if (A == "mathmlns") {
        set(cst_mathml, B);
        return true;
    }
    if (A == "marginpar") {
        set(np_marginpar, B);
        return true;
    }
    if (A == "mode") {
        set(cst_mode, B);
        return true;
    }
    if (A == "noindent") {
        set(np_noindent, B);
        return true;
    }
    if (A == "nonumber") {
        set(np_nonumber, B);
        return true;
    }
    if (A == "nom") {
        set(np_nom, B);
        return true;
    }
    if (A == "name") {
        set(np_name, B);
        return true;
    }
    if (A == "nameA") {
        set(np_nameA, B);
        return true;
    }
    if (A == "nameB") {
        set(np_nameB, B);
        return true;
    }
    if (A == "place") {
        set(np_place, B);
        return true;
    }
    if (A == "prenom") {
        set(np_prenom, B);
        return true;
    }
    if (A == "pre") {
        set(np_pre, B);
        return true;
    }
    if (A == "particule") {
        set(np_particle, B);
        return true;
    }
    if (A == "page") {
        set(np_page, B);
        return true;
    }
    if (A == "profession") {
        set(np_profession, B);
        return true;
    }
    if (A == "posA") {
        set(np_posA, B);
        return true;
    }
    if (A == "posB") {
        set(np_posB, B);
        return true;
    }
    if (A == "pos") {
        set(np_pos, B);
        return true;
    }
    if (A == "quote") {
        set(np_center_etc2, B);
        return true;
    }
    if (A == "quotation") {
        set(np_center_etc3, B);
        return true;
    }
    if (A == "rotate_angle") {
        set(np_r_angle, B);
        return true;
    }
    if (A == "rend") {
        set(np_rend, B);
        return true;
    }
    if (A == "row_spaceafter") {
        set(np_spaceafter, B);
        return true;
    }
    if (A == "repeat") {
        set(np_repeat, B);
        return true;
    }
    if (A == "scale") {
        set(np_scale, B);
        return true;
    }
    if (A == "space_before") {
        set(np_spacebefore, B);
        return true;
    }
    if (A == "size") {
        set(np_size, B);
        return true;
    }
    if (A == "starred") {
        set(np_starred, B);
        return true;
    }
    if (A == "table_width") {
        set(np_tab_width, B);
        return true;
    }
    if (A == "type") {
        set(np_type, B);
        return true;
    }
    if (A == "textype") {
        set(np_textype, B);
        return true;
    }
    if (A == "unit_length") {
        set(np_unit_length, B);
        return true;
    }
    if (A == "user_list") {
        set(np_user_list, B);
        return true;
    }
    if (A == "vpos") {
        set(np_vpos, B);
        return true;
    }
    if (A == "verse") {
        set(np_center_etc6, B);
        return true;
    }
    if (A == "width") {
        set(np_width, B);
        return true;
    }
    if (A == "xscale") {
        set(np_xscale, B);
        return true;
    }
    if (A == "xscaley") {
        set(np_xscaley, B);
        return true;
    }
    if (A == "xpos") {
        set(np_xpos, B);
        return true;
    }
    if (A == "xdir") {
        set(np_xdir, B);
        return true;
    }
    if (A == "yscale") {
        set(np_yscale, B);
        return true;
    }
    if (A == "yscalex") {
        set(np_yscalex, B);
        return true;
    }
    if (A == "ydir") {
        set(np_ydir, B);
        return true;
    }
    if (A == "ypos") {
        set(np_ypos, B);
        return true;
    }
    return false;
}
