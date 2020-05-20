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
    def("stylesheet", np_stylesheet, "");
    def("np_theorem", np_theorem, "");
    def("warning", np_warning, "");
    def("particule", np_particle, "part");
    def("quotation", np_center_etc3, "quoted");
    def("quote", np_center_etc2, "quoted");
    def("rotate_angle", np_r_angle, "angle");
    def("scaption", np_caption, "caption");
    def("table_env", np_float_table, "table");
    def("table_width", np_tab_width, "width");
    def("theorem_head", np_theorem_head, "alt_head");
    def("user_list", np_user_list, "description");
    def("vector", np_vector, "pic-vector");

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
    def("font_boldextended", np_font_boldextended, "boldextended");
    def("font_condensed", np_font_condensed, "condensed");
    def("font_it", np_font_it, "it");
    def("font_large", np_font_large, "large");
    def("font_large1", np_font_large1, "large1");
    def("font_large2", np_font_large2, "large2");
    def("font_large3", np_font_large3, "large3");
    def("font_large4", np_font_large4, "large4");
    def("font_large5", np_font_large5, "large5");
    def("font_large6", np_font_large6, "large6");
    def("font_medium", np_font_medium, "medium");
    def("font_normalsize", np_font_normalsize, "normalsize");
    def("font_roman", np_font_roman, "roman");
    def("font_sansserif", np_font_sansserif, "sansserif");
    def("font_semibold", np_font_semibold, "semibold");
    def("font_slanted", np_font_slanted, "slanted");
    def("font_small", np_font_small, "small");
    def("font_small1", np_font_small1, "small1");
    def("font_small2", np_font_small2, "small2");
    def("font_small3", np_font_small3, "small3");
    def("font_small4", np_font_small4, "small4");
    def("font_small5", np_font_small5, "small5");
    def("font_small6", np_font_small6, "small6");
    def("font_tt", np_font_tt, "tt");
    def("font_upright", np_font_upright, "upright");
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
    def("glo", np_glo_name, "Glossary");
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
    def("linethickness", np_line_thickness, "pic-linethickness");
    def("line", np_line, "pic-line");
    def("lineC", np_lineC, "line");
    def("np_linethickness", np_linethickness, "linethickness");
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
    def("multiput", np_multiput, "pic-multiput");
    def(np_name, "name");
    def(np_nameA, "nameA");
    def(np_nameB, "nameB");
    def("natcit", np_natcit, "Cit");
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
    def("oval", np_oval, "pic-oval");
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
    def("project", np_projet, "projet");
    def(np_projetdeveloppe, "projetdeveloppe");
    def("put", np_put, "pic-put");
    def(np_quoted, "quoted");
    def(np_raisebox, "raisebox");
    def("rclist", np_rclist, "UR");
    def("rcval", np_rcval, "+UR");
    def(np_ref, "ref");
    def(np_rend, "rend");
    def(np_repeat, "repeat");
    def(np_research_centre, "research-centre");
    def(np_right, "right");
    def(np_rightborder, "right-border");
    def(np_rlap, "rlap");
    def("rotatebox", np_rotatebox, "pic-rotatebox");
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
    def("scaleput", np_scaleput, "pic-scaleput");
    def(np_simple, "simple");
    def(np_simplemath, "simplemath");
    def(np_size, "size");
    def(np_spaceafter, "spaceafter");
    def(np_spacebefore, "spacebefore");
    def(np_specs, "specs");
    def(np_starred, "starred");
    def(np_style, "style");
    def("stylesheettype", np_stylesheet_type, "text/css");
    def(np_subfigure, "subfigure");
    def(np_table, "table");
    def(np_Table, "Table");
    def(np_tabular_star, "tabular*");
    def(np_tabular, "tabular");
    def("tagcurve", np_tagcurve, "pic-tagcurve");
    def(np_target, "target");
    def(np_term, "term");
    def(np_texmath, "texmath");
    def(np_texte, "texte");
    def(np_textype, "textype");
    def(np_theglossary, "theglossary");
    def(np_theindex, "theindex");
    def(np_theme, "theme");
    def("thicklines", np_thick_lines, "pic-thicklines");
    def("thinlines", np_thin_lines, "pic-thinlines");
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
    spdlog::trace("Setting {} to {}", A, B);

    if (A == "arc") {
        set(np_arc, B);
        return;
    }
    if (A == "allowbreak") {
        set(np_allowbreak, B);
        return;
    }
    if (A == "anchor") {
        set(np_anchor, B);
        return;
    }
    if (A == "anodeconnect") {
        set(np_anodeconnect, B);
        return;
    }
    if (A == "abarnodeconnect") {
        set(np_abarnodeconnect, B);
        return;
    }
    if (A == "anodecurve") {
        set(np_anodecurve, B);
        return;
    }
    if (A == "alt_caption") {
        set(np_alt_caption, B);
        return;
    }
    if (A == "alt_section") {
        set(np_alt_section, B);
        return;
    }
    if (A == "alternatives") {
        set(np_alternatives, B);
        return;
    }
    if (A == "box") {
        set(np_box, B);
        return;
    }
    if (A == "bezier") {
        set(np_bezier, B);
        return;
    }
    if (A == "bigcircle") {
        set(np_bigcircle, B);
        return;
    }
    if (A == "backmatter") {
        set(np_backmatter, B);
        return;
    }
    if (A == "biblio") {
        set(np_biblio, B);
        return;
    }
    if (A == "bpers") {
        set(np_bpers, B);
        return;
    }
    if (A == "barnodeconnect") {
        set(np_barnodeconnect, B);
        return;
    }
    if (A == "bibitem") {
        set(np_bibitem, B);
        return;
    }
    if (A == "bibkey") {
        set(np_bibkey, B);
        return;
    }
    if (A == "cell") {
        set(np_cell, B);
        return;
    }
    if (A == "caption") {
        set(np_captions, B);
        return;
    }
    if (A == "circle") {
        set(np_circle, B);
        return;
    }
    if (A == "closecurve") {
        set(np_closecurve, B);
        return;
    }
    if (A == "curve") {
        set(np_curve, B);
        return;
    }
    if (A == "catperso") {
        set(np_catperso, B);
        return;
    }
    if (A == "composition_ra") {
        set(cst_composition, B);
        return;
    }
    if (A == "cleaders") {
        set(np_cleaders, B);
        return;
    }
    if (A == "caps") {
        set(np_s_caps, B);
        return;
    }
    if (A == "cit") {
        set(np_cit, B);
        return;
    }
    if (A == "citation") {
        set(np_citation, B);
        return;
    }
    if (A == "citetype") {
        set(np_cite_type, B);
        return;
    }
    if (A == "dashline") {
        set(np_dashline, B);
        return;
    }
    if (A == "div0") {
        set(np_div0, B);
        return;
    }
    if (A == "div1") {
        set(np_div1, B);
        return;
    }
    if (A == "div2") {
        set(np_div2, B);
        return;
    }
    if (A == "div3") {
        set(np_div3, B);
        return;
    }
    if (A == "div4") {
        set(np_div4, B);
        return;
    }
    if (A == "div5") {
        set(np_div5, B);
        return;
    }
    if (A == "div6") {
        set(np_div6, B);
        return;
    }
    if (A == "drawline") {
        set(np_drawline, B);
        return;
    }
    if (A == "dottedline") {
        set(np_dottedline, B);
        return;
    }
    if (A == "eqnpos") {
        set(np_eqnpos, B);
        return;
    }
    if (A == "footnote") {
        set(np_footnote, B);
        return;
    }
    if (A == "formula") {
        set(np_formula, B);
        return;
    }
    if (A == "fbox") {
        set(np_fbox, B);
        return;
    }
    if (A == "figure") {
        set(np_figure, B);
        return;
    }
    if (A == "figure_env") {
        set(np_float_figure, B);
        return;
    }
    if (A == "frontmatter") {
        set(np_frontmatter, B);
        return;
    }

    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return;
    }
    if (A == "rasection" && ra_ok) {
        set(np_rasection, B);
        return;
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
        return;
    }
    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return;
    }
    if (A == "xtheorem") {
        set(np_theorem, B);
        return;
    }

    spdlog::trace("Setting XML symbol `{}' to \"{}\" (generic method)", A, B);
    set(A, B);
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
