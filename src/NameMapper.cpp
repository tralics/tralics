#include "tralics/NameMapper.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

auto NameMapper::operator[](const std::string &name) const -> Istring { return dict.contains(name) ? dict.at(name) : Istring(name); }

auto NameMapper::operator[](name_positions i) const -> Istring { return (*this)[id_to_name[i]]; }

auto NameMapper::operator[](size_t i) const -> Istring { return (*this)[id_to_name[i]]; }

void NameMapper::set(name_positions i, const std::string &s) { dict[id_to_name[i]] = Istring(s); }

void NameMapper::set(size_t i, const std::string &s) { dict[id_to_name[i]] = Istring(s); }

void NameMapper::set(const std::string &name, const std::string &value) { dict[name] = Istring(value); };

void NameMapper::def(const std::string &name, name_positions pos, const std::optional<std::string> &value) {
    id_to_name[pos] = name;
    dict[name]      = value ? Istring(*value) : Istring();
}

void NameMapper::def(name_positions i, const std::string &s) { id_to_name[i] = s; }

void NameMapper::boot() {
    // Special cases where name != default value
    def("alt_caption", np_alt_caption, "alt_head");
    def("alt_section", np_alt_section, "alt_head");
    def("box_scale", np_s_scale, "scale");
    def("box_width", np_box_width, "width");
    def("caption", np_captions, "head");
    def("cell_center", np_c_center, "center");
    def("cell_left", np_c_left, "left");
    def("cell_right", np_c_right, "right");
    def("centering", np_center_etc1, "center");
    def("citetype", np_cite_type, "type");
    def("cst_display", cst_display, "display");
    def("cst_div0", cst_div0, "div0");
    def("cst_div1", cst_div1, "div1");
    def("cst_div2", cst_div2, "div2");
    def("cst_div3", cst_div3, "div3");
    def("cst_div4", cst_div4, "div4");
    def("cst_div5", cst_div5, "div5");
    def("cst_div6", cst_div6, "div6");
    def("cst_empty", cst_empty, "");
    def("cst_invalid", cst_invalid);
    def("cst_nothing", cst_nothing, "");
    def("cst_p", cst_p, "p");
    def("cstb_key", cstb_key, "key");
    def("cstb_type", cstb_type, "type");
    def("cstb_unknown", cstb_unknown, "unknown");
    def("cstb_url", cstb_url, "url");
    def("cstf_upright", cstf_upright, "");
    def("fbox_rend", np_b_rend, "rend");
    def("figure_env", np_float_figure, "figure");
    def("file_extension", np_fileextension, "extension");
    def("font_bold", np_font_bold, "bold");
    def("font_sc", np_font_sc, "sc");
    def("foot_position", np_foot, "foot");
    def("footnote", np_footnote, "note");
    def("gloitem", np_label_glo, "label");
    def("labelitem", np_label_item, "label");
    def("minipage_width", np_minipage_width, "width");
    def("nb_rasection", np_rasection, "");
    def("np_center_etc", np_center_etc, "center");
    def("np_center", np_center, "center");
    def("np_cst_width", np_cst_width, "width");
    def("np_language", np_language, "");
    def("np_separator", np_separator, "");
    def("np_stylesheet", np_stylesheet, "");
    def("np_theorem", np_theorem, "");
    def("np_warning", np_warning, "");
    def("particule", np_particle, "part");
    def("quotation", np_center_etc3, "quoted");
    def("quote", np_center_etc2, "quoted");
    def("rotate_angle", np_r_angle, "angle");
    def("scaption", np_caption, "caption");
    def("table_env", np_float_table, "table");
    def("table_width", np_tab_width, "width");
    def("theorem_head", np_theorem_head, "alt_head");
    def("user_list", np_user_list, "description");

    // Below, this is a mapping id <-> name = default value
    def(cst_accent, "accent");
    def(cst_accentunder, "accentunder");
    def(cst_argument, "argument");
    def(cst_biblio, "thebibliography");
    def(cst_ci, "ci");
    def(cst_cn, "cn");
    def(cst_composition, "composition");
    def(cst_csymbol, "csymbol");
    def(cst_denalign, "denomalign");
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
    def(cst_mi, "mi");
    def(cst_mn, "mn");
    def(cst_mo, "mo");
    def(cst_mode, "mode");
    def(cst_mover, "mover");
    def(cst_mpadded, "mpadded");
    def(cst_mphantom, "mphantom");
    def(cst_mref, "mref");
    def(cst_mroot, "mroot");
    def(cst_mrow_cst, "<mrow></mrow>");
    def(cst_mrow, "mrow");
    def(cst_mspace, "mspace");
    def(cst_msqrt, "msqrt");
    def(cst_mstyle, "mstyle");
    def(cst_msub, "msub");
    def(cst_msubsup, "msubsup");
    def(cst_msup, "msup");
    def(cst_mtable, "mtable");
    def(cst_mtd, "mtd");
    def(cst_mtext, "mtext");
    def(cst_mtr, "mtr");
    def(cst_multiscripts, "mmultiscripts");
    def(cst_munder, "munder");
    def(cst_munderover, "munderover");
    def(cst_nl, "\n");
    def(cst_numalign, "numalign");
    def(cst_particip, "particip");
    def(cst_rasection, "RAsection");
    def(cst_refer, "refer");
    def(cst_scriptlevel, "scriptlevel");
    def(cst_separators, "separators");
    def(cst_temporary, "temporary");
    def(cst_theorem, "theorem");
    def(cst_xmlns, "xmlns");
    def(cstb_address, "address");
    def(cstb_article, "article");
    def(cstb_author, "author");
    def(cstb_book, "book");
    def(cstb_booklet, "booklet");
    def(cstb_booktitle, "booktitle");
    def(cstb_chapter, "chapter");
    def(cstb_comment, "comment");
    def(cstb_conference, "conference");
    def(cstb_coursenotes, "coursenotes");
    def(cstb_crossref, "crossref");
    def(cstb_doi, "doi");
    def(cstb_edition, "edition");
    def(cstb_editor, "editor");
    def(cstb_equipe, "equipe");
    def(cstb_extension, "extension");
    def(cstb_howpublished, "howpublished");
    def(cstb_inbook, "inbook");
    def(cstb_incollection, "incollection");
    def(cstb_inproceedings, "inproceedings");
    def(cstb_institution, "institution");
    def(cstb_isbn, "isbn");
    def(cstb_isrn, "isrn");
    def(cstb_issn, "issn");
    def(cstb_journal, "journal");
    def(cstb_language, "language");
    def(cstb_langue, "langue");
    def(cstb_manual, "manual");
    def(cstb_mastersthesis, "masterthesis");
    def(cstb_masterthesis, "mastersthesis");
    def(cstb_misc, "misc");
    def(cstb_month, "month");
    def(cstb_note, "note");
    def(cstb_number, "number");
    def(cstb_organization, "organization");
    def(cstb_pages, "pages");
    def(cstb_phdthesis, "phdthesis");
    def(cstb_preamble, "preamble");
    def(cstb_proceedings, "proceedings");
    def(cstb_publisher, "publisher");
    def(cstb_school, "school");
    def(cstb_section, "section");
    def(cstb_series, "series");
    def(cstb_statut, "statut");
    def(cstb_string, "string");
    def(cstb_subtype, "subtype");
    def(cstb_techreport, "techreport");
    def(cstb_title, "title");
    def(cstb_unite, "unite");
    def(cstb_unpublished, "unpublished");
    def(cstb_volume, "volume");
    def(cstb_year, "year");
    def(cstf_bold_fraktur, "bold-fraktur");
    def(cstf_bold_italic, "bold-italic");
    def(cstf_bold_sansserif, "bold-sans-serif");
    def(cstf_bold_script, "bold-script");
    def(cstf_bold, "bold");
    def(cstf_doublestruck, "double-struck");
    def(cstf_fraktur, "fraktur");
    def(cstf_italic, "italic");
    def(cstf_monospace, "monospace");
    def(cstf_normal, "normal");
    def(cstf_sansserif_bold_italic, "sans-serif-bold-italic");
    def(cstf_sansserif_italic, "sans-serif-italic");
    def(cstf_sansserif, "sans-serif");
    def(cstf_script, "script");
    def(np_12pt, "12pt");
    def(np_3pt, "3pt");
    def(np_6pt, "6pt");
    def(np_a1, "a1");
    def(np_a2, "a2");
    def(np_abarnodeconnect, "abarnodeconnect");
    def(np_accueil, "accueil");
    def(np_affiliation, "affiliation");
    def(np_allowbreak, "allowbreak");
    def(np_alternatives, "alternatives");
    def(np_anchor, "anchor");
    def(np_angle, "angle");
    def(np_anodeconnect, "anodeconnect");
    def(np_anodecurve, "anodecurve");
    def(np_arc, "pic-arc");
    def(np_arg1, "arg1");
    def(np_arg2, "arg2");
    def(np_arg3, "arg3");
    def(np_array, "array");
    def(np_b1, "b1");
    def(np_b2, "b2");
    def(np_backmatter, "backmatter");
    def(np_barnodeconnect, "barnodeconnect");
    def(np_bauteurs, "bauteurs");
    def(np_bediteurs, "bediteur");
    def(np_bezier, "pic-bezier");
    def(np_bibitem, "bibitem");
    def(np_bibkey, "bib-key");
    def(np_biblio, "biblio");
    def(np_bigcircle, "pic-bigcircle");
    def(np_border_bottomw, "border-bottom-width");
    def(np_border_leftw, "border-left-width");
    def(np_border_rightw, "border-right-width");
    def(np_border_topw, "border-top-width");
    def(np_bottomborder, "bottom-border");
    def(np_box_pos, "position");
    def(np_box, "pic-framebox");
    def(np_boxed, "boxed");
    def(np_bpers, "bpers");
    def(np_c1, "c1");
    def(np_c2, "c2");
    def(np_catperso, "catperso");
    def(np_cell, "cell");
    def(np_center_etc4, "flushed-left");
    def(np_center_etc5, "flushed-right");
    def(np_center_etc6, "verse");
    def(np_centered, "centered");
    def(np_chapters, "chapters");
    def(np_circle, "pic-circle");
    def(np_cit, "cit");
    def(np_citation, "citation");
    def(np_cleaders, "cleaders");
    def(np_clip, "clip");
    def(np_close, "close");
    def(np_closecurve, "pic-closecurve");
    def(np_color, "color");
    def(np_color2, "color2");
    def(np_cols, "cols");
    def(np_columnalign, "columnalign");
    def(np_columnspan, "columnspan");
    def(np_curve_nbpts, "nbsymb");
    def(np_curve, "pic-curve");
    def(np_dashbox, "pic-dashbox");
    def(np_dashdim, "dashdim");
    def(np_dashline, "dashline");
    def(np_depth, "depth");
    def(np_depthA, "depthA");
    def(np_depthB, "depthB");
    def(np_description, "description");
    def(np_display, "display");
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
    def(np_false, "false");
    def(np_fbox, "fbox");
    def(np_figure, "figure");
    def(np_file, "file");
    def(np_float, "float");
    def(np_font_boldextended, "boldextended");
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
    def(np_font_semibold, "semibold");
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
    def(np_form, "form");
    def(np_formula, "formula");
    def(np_frame, "pic-frame");
    def(np_framed, "framed");
    def(np_french, "french");
    def(np_from, "from");
    def(np_frontmatter, "frontmatter");
    def(np_full_first, "prenomcomplet");
    def(np_full, "full");
    def(np_german, "german");
    def(np_glo_name, "Glossary");
    def(np_gloss, "gloss");
    def(np_glossary, "glossary");
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
    def(np_hss, "hss");
    def(np_html, "html");
    def(np_id, "id");
    def(np_idtext, "id-text");
    def(np_index, "index");
    def(np_Info, "Info");
    def(np_inline, "inline");
    def(np_isproject, "isproject");
    def(np_item, "item");
    def(np_junior, "junior");
    def(np_key, "key");
    def(np_keywords, "keywords");
    def(np_leaders, "leaders");
    def(np_left, "left");
    def(np_leftborder, "left-border");
    def(np_leg, "leg");
    def(np_letters_bl, "bl");
    def(np_letters_br, "br");
    def(np_letters_tl, "tl");
    def(np_letters_tr, "tr");
    def(np_level, "level");
    def(np_line_thickness, "pic-linethickness");
    def(np_line, "pic-line");
    def(np_lineC, "line");
    def(np_linethickness, "linethickness");
    def(np_list, "list");
    def(np_llap, "llap");
    def(np_mainmatter, "mainmatter");
    def(np_marginpar, "marginpar");
    def(np_mbox, "mbox");
    def(np_minipage, "minipage");
    def(np_mml_bold_fraktur, "mml@font@boldfraktur");
    def(np_mml_bold_italic, "mml@font@bolditalic");
    def(np_mml_bold_sansserif, "mml@font@boldsansserif");
    def(np_mml_bold_script, "mml@font@boldscript");
    def(np_mml_bold, "mml@font@bold");
    def(np_mml_doublestruck, "mml@font@doublestruck");
    def(np_mml_fraktur, "mml@font@fraktur");
    def(np_mml_italic, "mml@font@italic");
    def(np_mml_monospace, "mml@font@monospace");
    def(np_mml_normal, "mml@font@normal");
    def(np_mml_sansserif_bold_italic, "mml@font@sansserifbolditalic");
    def(np_mml_sansserif_italic, "mml@font@sansserifitalic");
    def(np_mml_sansserif, "mml@font@sansserif");
    def(np_mml_script, "mml@font@script");
    def(np_mml_upright, "mml@font@upright");
    def(np_module, "module");
    def(np_moreinfo, "moreinfo");
    def(np_movablelimits, "movablelimits");
    def(np_multiput, "pic-multiput");
    def(np_name, "name");
    def(np_nameA, "nameA");
    def(np_nameB, "nameB");
    def(np_natcit, "Cit");
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
    def(np_pagecolor, "pagecolor");
    def(np_part, "part");
    def(np_part0, "participant");
    def(np_part1, "participante");
    def(np_part2, "participantes");
    def(np_part3, "participants");
    def(np_pers, "pers");
    def(np_picture, "picture");
    def(np_place, "place");
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
    def(np_projet, "projet");
    def(np_projetdeveloppe, "projetdeveloppe");
    def(np_put, "pic-put");
    def(np_quoted, "quoted");
    def(np_raisebox, "raisebox");
    def(np_rclist, "UR");
    def(np_rcval, "+UR");
    def(np_ref, "ref");
    def(np_rend, "rend");
    def(np_repeat, "repeat");
    def(np_research_centre, "research-centre");
    def(np_right, "right");
    def(np_rightborder, "right-border");
    def(np_rlap, "rlap");
    def(np_rotatebox, "pic-rotatebox");
    def(np_row, "row");
    def(np_rule_bsa, "bottom_rule_space_above");
    def(np_rule_bsb, "bottom_rule_space_below");
    def(np_rule_tsa, "top_rule_space_above");
    def(np_rule_tsb, "top_rule_space_below");
    def(np_rule, "rule");
    def(np_s_caps, "caps");
    def(np_s_hl, "hl");
    def(np_s_old, "oldstyle");
    def(np_s_so, "so");
    def(np_s_st, "st");
    def(np_s_sub, "sub");
    def(np_s_sup, "sup");
    def(np_s_ul, "ul");
    def(np_sbox, "scalebox");
    def(np_sc, "sc");
    def(np_scale, "scale");
    def(np_scaleput, "pic-scaleput");
    def(np_simple, "simple");
    def(np_simplemath, "simplemath");
    def(np_size, "size");
    def(np_spaceafter, "spaceafter");
    def(np_spacebefore, "spacebefore");
    def(np_specs, "specs");
    def(np_starred, "starred");
    def(np_style, "style");
    def(np_stylesheet_type, "text/css");
    def(np_subfigure, "subfigure");
    def(np_table, "table");
    def(np_Table, "Table");
    def(np_tabular_star, "tabular*");
    def(np_tabular, "tabular");
    def(np_tagcurve, "pic-tagcurve");
    def(np_target, "target");
    def(np_term, "term");
    def(np_texmath, "texmath");
    def(np_texte, "texte");
    def(np_textype, "textype");
    def(np_theglossary, "theglossary");
    def(np_theindex, "theindex");
    def(np_theme, "theme");
    def(np_thick_lines, "pic-thicklines");
    def(np_thin_lines, "pic-thinlines");
    def(np_toc, "tableofcontents");
    def(np_toc1, "listoftables");
    def(np_toc2, "listoffigures");
    def(np_topborder, "top-border");
    def(np_totalwidth, "totalwidth");
    def(np_true, "true");
    def(np_type, "type");
    def(np_underline, "underline");
    def(np_unit_length, "unit-length");
    def(np_unknown, "unknown");
    def(np_url, "url");
    def(np_userid, "userid");
    def(np_val, "val");
    def(np_vector, "pic-vector");
    def(np_vfil, "vfil");
    def(np_vfill, "vfill");
    def(np_vfilneg, "vfilneg");
    def(np_vpos, "vpos");
    def(np_vss, "vss");
    def(np_Warning, "Warning");
    def(np_width, "width");
    def(np_xdir, "xdir");
    def(np_xleaders, "xleaders");
    def(np_xpos, "xpos");
    def(np_xref, "xref");
    def(np_xscale, "xscale");
    def(np_xscaley, "xscaley");
    def(np_ydir, "ydir");
    def(np_ypos, "ypos");
    def(np_yscale, "yscale");
    def(np_yscalex, "yscalex");
    def(np_zerodim, "0pt");

    for (char x = 'a'; x <= 'z'; x++) def(name_positions(to_unsigned(np_letter_a + x - 'a')), std::string(1, x));
    for (char x = 'A'; x <= 'Z'; x++) def(name_positions(to_unsigned(np_letter_A + x - 'A')), std::string(1, x));
}

void NameMapper::assign(const std::string &sa, const std::string &sb) {
    auto n = sa.size();

    if (sa.starts_with("elt_")) return the_names.assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return the_names.assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
    if (sa.starts_with("att_")) return the_names.assign_att(sa.substr(4), sb);

    if (sa == "lang_fr") { set(np_french, sb); }
    if (sa == "lang_en") { set(np_english, sb); }
    if (sa == "distinguish_refer_in_rabib") {
        if ((sb == "true") || (sb == "yes"))
            the_main->distinguish_refer = true;
        else if ((sb == "false") || (sb == "no"))
            the_main->distinguish_refer = false;
        else
            spdlog::warn("distinguish_refer_in_rabib = {} ignored");
    }
    if (sa == "entity_names") { the_main->set_ent_names(sb); }
    if (sa == "default_class") { the_main->default_class = sb; }
    if (sa == "alternate_item") {
        if (sb == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (sb == "true")
            the_parser.hash_table.eval_let("item", "@item");
    }
    if (sa == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << sb << "}";
        the_main->add_to_from_config(1, aux); // \todo no Buffer
    }
    if (sa == "everyjob") { everyjob_string = fmt::format("\\everyjob={{{}}}", sb); }
    if (sa == "no_footnote_hack") {
        if (sb == "true") the_main->footnote_hack = false;
        if (sb == "false") the_main->footnote_hack = true;
    }
    if (sa == "use_font_elt") {
        if (sb == "true") the_main->use_font_elt = true;
        if (sb == "false") the_main->use_font_elt = false;
    }
    if (sa == "use_all_sizes") {
        if (sb == "true") the_main->use_all_sizes = true;
        if (sb == "false") the_main->use_all_sizes = false;
    }
    if (sa == "bibtex_fields") {
        Buffer(sb).interpret_bibtex_list(); // \todo without Buffer
    }
    if (sa == "bibtex_extensions") { Buffer(sb).interpret_bibtex_extension_list(); }
    if (sa == "mfenced_separator_val") {
        if (sb == "NONE")
            set(np_separator, "");
        else
            set(np_separator, sb);
    }
    if (sa.ends_with("_vals")) {
        Buffer B(sb);
        config_ns::interpret_list(sa.substr(0, n - 5), B); // \todo without Buffer
    }
    if (sa == "mml_font_normal") { set(np_mml_normal, sb); }
    if (sa == "mml_font_upright") { set(np_mml_upright, sb); }
    if (sa == "mml_font_bold") { set(np_mml_bold, sb); }
    if (sa == "mml_font_italic") { set(np_mml_italic, sb); }
    if (sa == "mml_font_bold_italic") { set(np_mml_bold_italic, sb); }
    if (sa == "mml_font_script") { set(np_mml_script, sb); }
    if (sa == "mml_font_bold_script") { set(np_mml_bold_script, sb); }
    if (sa == "mml_font_fraktur") { set(np_mml_fraktur, sb); }
    if (sa == "mml_font_doublestruck") { set(np_mml_doublestruck, sb); }
    if (sa == "mml_font_bold_fraktur") { set(np_mml_bold_fraktur, sb); }
    if (sa == "mml_font_sansserif") { set(np_mml_sansserif, sb); }
    if (sa == "mml_font_bold_sansserif") { set(np_mml_bold_sansserif, sb); }
    if (sa == "mml_font_sansserif_italic") { set(np_mml_sansserif_italic, sb); }
    if (sa == "mml_font_sansserif_bold_italic") { set(np_mml_sansserif_bold_italic, sb); }
    if (sa == "mml_font_monospace") { set(np_mml_monospace, sb); }
}

void NameMapper::assign_name(const std::string &A, const std::string &B) {
    spdlog::warn("Setting {} to {}", A, B);

    if (A == "arc") { set(np_arc, B); }
    if (A == "allowbreak") { set(np_allowbreak, B); }
    if (A == "anchor") { set(np_anchor, B); }
    if (A == "anodeconnect") { set(np_anodeconnect, B); }
    if (A == "abarnodeconnect") { set(np_abarnodeconnect, B); }
    if (A == "anodecurve") { set(np_anodecurve, B); }
    if (A == "alt_caption") { set(np_alt_caption, B); }
    if (A == "alt_section") { set(np_alt_section, B); }
    if (A == "alternatives") { set(np_alternatives, B); }
    if (A == "box") { set(np_box, B); }
    if (A == "bezier") { set(np_bezier, B); }
    if (A == "bigcircle") { set(np_bigcircle, B); }
    if (A == "backmatter") { set(np_backmatter, B); }
    if (A == "biblio") { set(np_biblio, B); }
    if (A == "bpers") { set(np_bpers, B); }
    if (A == "barnodeconnect") { set(np_barnodeconnect, B); }
    if (A == "bibitem") { set(np_bibitem, B); }
    if (A == "bibkey") { set(np_bibkey, B); }
    if (A == "cell") { set(np_cell, B); }
    if (A == "caption") { set(np_captions, B); }
    if (A == "circle") { set(np_circle, B); }
    if (A == "closecurve") { set(np_closecurve, B); }
    if (A == "curve") { set(np_curve, B); }
    if (A == "catperso") { set(np_catperso, B); }
    if (A == "composition_ra") { set(cst_composition, B); }
    if (A == "cleaders") { set(np_cleaders, B); }
    if (A == "caps") { set(np_s_caps, B); }
    if (A == "cit") { set(np_cit, B); }
    if (A == "citation") { set(np_citation, B); }
    if (A == "citetype") { set(np_cite_type, B); }
    if (A == "dashline") { set(np_dashline, B); }
    if (A == "div0") { set(np_div0, B); }
    if (A == "div1") { set(np_div1, B); }
    if (A == "div2") { set(np_div2, B); }
    if (A == "div3") { set(np_div3, B); }
    if (A == "div4") { set(np_div4, B); }
    if (A == "div5") { set(np_div5, B); }
    if (A == "div6") { set(np_div6, B); }
    if (A == "drawline") { set(np_drawline, B); }
    if (A == "dottedline") { set(np_dottedline, B); }
    if (A == "eqnpos") { set(np_eqnpos, B); }
    if (A == "footnote") { set(np_footnote, B); }
    if (A == "formula") { set(np_formula, B); }
    if (A == "fbox") { set(np_fbox, B); }
    if (A == "figure") { set(np_figure, B); }
    if (A == "figure_env") { set(np_float_figure, B); }
    if (A == "frontmatter") { set(np_frontmatter, B); }
    if (A == "font_small") { set(np_font_small, B); }
    if (A == "font_small1") { set(np_font_small1, B); }
    if (A == "font_small2") { set(np_font_small2, B); }
    if (A == "font_small3") { set(np_font_small3, B); }
    if (A == "font_small4") { set(np_font_small4, B); }
    if (A == "font_small5") { set(np_font_small5, B); }
    if (A == "font_small6") { set(np_font_small6, B); }
    if (A == "font_large") { set(np_font_large, B); }
    if (A == "font_large1") { set(np_font_large1, B); }
    if (A == "font_large2") { set(np_font_large2, B); }
    if (A == "font_large3") { set(np_font_large3, B); }
    if (A == "font_large4") { set(np_font_large4, B); }
    if (A == "font_large5") { set(np_font_large5, B); }
    if (A == "font_large6") { set(np_font_large6, B); }
    if (A == "font_normalsize") { set(np_font_normalsize, B); }
    if (A == "font_upright") { set(np_font_upright, B); }
    if (A == "font_medium") { set(np_font_medium, B); }
    if (A == "font_roman") { set(np_font_roman, B); }
    if (A == "font_it") { set(np_font_it, B); }
    if (A == "font_slanted") { set(np_font_slanted, B); }
    if (A == "font_sc") { set(np_font_sc, B); }
    if (A == "font_tt") { set(np_font_tt, B); }
    if (A == "font_sansserif") { set(np_font_sansserif, B); }
    if (A == "font_bold") { set(np_font_bold, B); }
    if (A == "font_boldextended") { set(np_font_boldextended, B); }
    if (A == "font_semibold") { set(np_font_semibold, B); }
    if (A == "font_condensed") { set(np_font_condensed, B); }
    if (A == "gloitem") { set(np_label_glo, B); }
    if (A == "graphics") { set(np_graphics, B); }
    if (A == "glo") { set(np_glo_name, B); }
    if (A == "glossary") { set(np_glossary, B); }
    if (A == "head") { set(np_head, B); }
    if (A == "hl") { set(np_s_hl, B); }
    if (A == "item") { set(np_item, B); }
    if (A == "index") { set(np_index, B); }
    if (A == "keywords") { set(np_keywords, B); }
    if (A == "labelitem") { set(np_label_item, B); }
    if (A == "lineC") { set(np_lineC, B); }
    if (A == "line") { set(np_line, B); }
    if (A == "listoffigures") { set(np_toc2, B); }
    if (A == "listoftables") { set(np_toc1, B); }
    if (A == "llap") { set(np_llap, B); }
    if (A == "linethickness") { set(np_line_thickness, B); }
    if (A == "list") { set(np_list, B); }
    if (A == "leaders") { set(np_leaders, B); }
    if (A == "leg") { set(np_leg, B); }
    if (A == "mbox") { set(np_mbox, B); }
    if (A == "math") { set(cst_math, B); }
    if (A == "multiput") { set(np_multiput, B); }
    if (A == "mainmatter") { set(np_mainmatter, B); }
    if (A == "node") { set(np_node, B); }
    if (A == "nodeconnect") { set(np_nodeconnect, B); }
    if (A == "nodecurve") { set(np_nodecurve, B); }
    if (A == "nodetriangle") { set(np_nodetriangle, B); }
    if (A == "nodecircle") { set(np_nodecircle, B); }
    if (A == "nodebox") { set(np_nodebox, B); }
    if (A == "nodeoval") { set(np_nodeoval, B); }
    if (A == "natcit") { set(np_natcit, B); }
    if (A == "oval") { set(np_oval, B); }
    if (A == "oldstyle") { set(np_s_old, B); }
    if (A == "overline") { set(np_overline, B); }
    if (A == "picture") { set(np_picture, B); }
    if (A == "put") { set(np_put, B); }
    if (A == "project") { set(np_projet, B); }
    if (A == "pers") { set(np_pers, B); }
    if (A == "prenote") { set(np_prenote, B); }
    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
    }
    if (A == "row") { set(np_row, B); }
    if (A == "raisebox") { set(np_raisebox, B); }
    if (A == "rlap") { set(np_rlap, B); }
    if (A == "rotatebox") { set(np_rotatebox, B); }
    if (A == "ref") { set(np_ref, B); }
    if ((A == "rclist") && ra_ok) { set(np_rclist, B); }
    if ((A == "rcval") && ra_ok) { set(np_rcval, B); }
    if ((A == "rasection") && ra_ok) { set(np_rasection, B); }
    if (A == "subfigure") { set(np_subfigure, B); }
    if (A == "scaleput") { set(np_scaleput, B); }
    if (A == "scalebox") { set(np_sbox, B); }
    if (A == "scaption") { set(np_caption, B); }
    if (A == "sup") { set(np_s_sup, B); }
    if (A == "sub") { set(np_s_sub, B); }
    if (A == "so") { set(np_s_so, B); }
    if (A == "st") { set(np_s_st, B); }
    if (A == "stylesheet") { set(np_stylesheet, B); }
    if (A == "stylesheettype") { set(np_stylesheet_type, B); }
    if (A == "term") { set(np_term, B); }
    if (A == "texmath") { set(np_texmath, B); }
    if (A == "table") { set(np_table, B); }
    if (A == "table_env") { set(np_float_table, B); }
    if (A == "Table") { set(np_Table, B); }
    if (A == "tagcurve") { set(np_tagcurve, B); }
    if (A == "thicklines") { set(np_thick_lines, B); }
    if (A == "thinlines") { set(np_thin_lines, B); }
    if (A == "theorem_head") { set(np_theorem_head, B); }
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
    }
    if (A == "theindex") { set(np_theindex, B); }
    if (A == "theglossary") { set(np_theglossary, B); }
    if (A == "tableofcontents") { set(np_toc, B); }
    if (A == "texte") { set(np_texte, B); }
    if (A == "ul") { set(np_s_ul, B); }
    if (A == "underline") { set(np_underline, B); }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
    }
    if (A == "vector") { set(np_vector, B); }
    if (A == "warning") { set(np_warning, B); }
    if (A == "xref") { set(np_xref, B); }
    if (A == "xtheorem") { set(np_theorem, B); }
    if (A == "xleaders") { set(np_xleaders, B); }

    // if (names.contains(A)) {
    //     spdlog::info("Setting {} to {} (generic method)", A, B);
    //     set(A, B);
    // }
}

void NameMapper::assign_att(const std::string &A, const std::string &B) {
    if (A == "angle") { set(np_angle, B); }
    if (A == "affiliation") { set(np_affiliation, B); }
    if (A == "box_pos") { set(np_box_pos, B); }
    if (A == "box_scale") { set(np_s_scale, B); }
    if (A == "box_width") { set(np_box_width, B); }
    if (A == "boxed") { set(np_boxed, B); }
    if (A == "bibkey") { set(np_bibkey, B); }
    if (A == "border_bottom_width") { set(np_border_bottomw, B); }
    if (A == "border_top_width") { set(np_border_topw, B); }
    if (A == "border_left_width") { set(np_border_leftw, B); }
    if (A == "border_right_width") { set(np_border_rightw, B); }
    if (A == "centering") { set(np_center_etc1, B); }
    if (A == "clip") { set(np_clip, B); }
    if (A == "cols") { set(np_cols, B); }
    if (A == "cell_left") { set(np_c_left, B); }
    if (A == "cell_right") { set(np_c_right, B); }
    if (A == "cell_center") { set(np_c_center, B); }
    if (A == "cell_leftborder") { set(np_leftborder, B); }
    if (A == "cell_rightborder") { set(np_rightborder, B); }
    if (A == "cell_topborder") { set(np_topborder, B); }
    if (A == "cell_bottomborder") { set(np_bottomborder, B); }
    if (A == "curve_nbpts") { set(np_curve_nbpts, B); }
    if (A == "display") { set(np_display, B); }
    if (A == "dx") { set(np_dx, B); }
    if (A == "dy") { set(np_dy, B); }
    if (A == "depthA") { set(np_depthA, B); }
    if (A == "depthB") { set(np_depthB, B); }
    if (A == "depth") { set(np_depth, B); }
    if (A == "encap") { set(np_encap, B); }
    if (A == "framed") { set(np_framed, B); }
    if (A == "full") { set(np_full, B); }
    if (A == "file") { set(np_file, B); }
    if (A == "file_extension") { set(np_fileextension, B); }
    if (A == "fbox_rend") { set(np_b_rend, B); }
    if (A == "flush_left") { set(np_center_etc4, B); }
    if (A == "flush_right") { set(np_center_etc5, B); }
    if (A == "foot_position") { set(np_foot, B); }
    if (A == "full_first") { set(np_full_first, B); }
    if (A == "gloss_type") { set(np_gloss, B); }
    if (A == "height") { set(np_height, B); }
    if (A == "halign") { set(np_halign, B); }
    if (A == "hdr") { set(np_hdr, B); }
    if (A == "inner_pos") { set(np_posi, B); }
    if (A == "inline") { set(np_inline, B); }
    if (A == "junior") { set(np_junior, B); }
    if (A == "language") { set(np_language, B); }
    if (A == "level") { set(np_level, B); }
    if (A == "minipage_width") { set(np_minipage_width, B); }
    if (A == "marginpar") { set(np_marginpar, B); }
    if (A == "mathmlns") { set(cst_mathml, B); }
    if (A == "marginpar") { set(np_marginpar, B); }
    if (A == "mode") { set(cst_mode, B); }
    if (A == "noindent") { set(np_noindent, B); }
    if (A == "nonumber") { set(np_nonumber, B); }
    if (A == "nom") { set(np_nom, B); }
    if (A == "name") { set(np_name, B); }
    if (A == "nameA") { set(np_nameA, B); }
    if (A == "nameB") { set(np_nameB, B); }
    if (A == "place") { set(np_place, B); }
    if (A == "prenom") { set(np_prenom, B); }
    if (A == "pre") { set(np_pre, B); }
    if (A == "particule") { set(np_particle, B); }
    if (A == "page") { set(np_page, B); }
    if (A == "profession") { set(np_profession, B); }
    if (A == "posA") { set(np_posA, B); }
    if (A == "posB") { set(np_posB, B); }
    if (A == "pos") { set(np_pos, B); }
    if (A == "quote") { set(np_center_etc2, B); }
    if (A == "quotation") { set(np_center_etc3, B); }
    if (A == "rotate_angle") { set(np_r_angle, B); }
    if (A == "rend") { set(np_rend, B); }
    if (A == "row_spaceafter") { set(np_spaceafter, B); }
    if (A == "repeat") { set(np_repeat, B); }
    if (A == "scale") { set(np_scale, B); }
    if (A == "space_before") { set(np_spacebefore, B); }
    if (A == "size") { set(np_size, B); }
    if (A == "starred") { set(np_starred, B); }
    if (A == "table_width") { set(np_tab_width, B); }
    if (A == "type") { set(np_type, B); }
    if (A == "textype") { set(np_textype, B); }
    if (A == "unit_length") { set(np_unit_length, B); }
    if (A == "user_list") { set(np_user_list, B); }
    if (A == "vpos") { set(np_vpos, B); }
    if (A == "verse") { set(np_center_etc6, B); }
    if (A == "width") { set(np_width, B); }
    if (A == "xscale") { set(np_xscale, B); }
    if (A == "xscaley") { set(np_xscaley, B); }
    if (A == "xpos") { set(np_xpos, B); }
    if (A == "xdir") { set(np_xdir, B); }
    if (A == "yscale") { set(np_yscale, B); }
    if (A == "yscalex") { set(np_yscalex, B); }
    if (A == "ydir") { set(np_ydir, B); }
    if (A == "ypos") { set(np_ypos, B); }
}
