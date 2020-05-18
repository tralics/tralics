#include "tralics/NameMapper.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

void NameMapper::boot() {
    set(cst_empty, Istring(""));
    set(cst_accent, Istring("accent"));
    set(cst_accentunder, Istring("accentunder"));
    set(cst_argument, Istring("argument"));
    set(cst_biblio, Istring("thebibliography"));
    set(cst_composition, Istring("composition"));
    set(cst_displaystyle, Istring("displaystyle"));
    set(cst_document, Istring("document"));
    set(cst_elt, Istring("ELT"));
    set(cst_flaghi, Istring("'hi_flag"));
    set(cst_fonts, Istring("fonts"));
    set(cst_foot, Istring("foot"));
    set(cst_hanl, Istring("hanl"));
    set(cst_hbox, Istring("hbox"));
    set(cst_hi, Istring("hi"));
    set(cst_math, Istring("math"));
    set(cst_mathml, Istring("http://www.w3.org/1998/Math/MathML"));
    set(cst_mathvariant, Istring("mathvariant"));
    set(cst_mfenced, Istring("mfenced"));
    set(cst_mfrac, Istring("mfrac"));
    set(cst_mpadded, Istring("mpadded"));
    set(cst_mphantom, Istring("mphantom"));
    set(cst_mi, Istring("mi"));
    set(cst_mn, Istring("mn"));
    set(cst_mo, Istring("mo"));
    set(cst_ci, Istring("ci"));
    set(cst_cn, Istring("cn"));
    set(cst_csymbol, Istring("csymbol"));
    set(cst_multiscripts, Istring("mmultiscripts"));
    set(cst_nothing, Istring(""));
    set(cst_mroot, Istring("mroot"));
    set(cst_mover, Istring("mover"));
    set(cst_mrow, Istring("mrow"));
    set(cst_mrow_cst, Istring("<mrow></mrow>"));
    set(cst_mspace, Istring("mspace"));
    set(cst_msqrt, Istring("msqrt"));
    set(cst_msub, Istring("msub"));
    set(cst_mstyle, Istring("mstyle"));
    set(cst_msubsup, Istring("msubsup"));
    set(cst_msup, Istring("msup"));
    set(cst_mtable, Istring("mtable"));
    set(cst_mtd, Istring("mtd"));
    set(cst_mtext, Istring("mtext"));
    set(cst_mtr, Istring("mtr"));
    set(cst_munder, Istring("munder"));
    set(cst_mref, Istring("mref"));
    set(cst_munderover, Istring("munderover"));
    set(cst_mode, Istring("mode"));
    set(cst_display, Istring("display"));
    set(cst_nl, Istring("\n"));
    set(cst_p, Istring("p"));
    set(cst_particip, Istring("particip"));
    set(cst_rasection, Istring("RAsection"));
    set(cst_refer, Istring("refer"));
    set(cst_scriptlevel, Istring("scriptlevel"));
    set(cst_separators, Istring("separators"));
    set(cst_temporary, Istring("temporary"));
    set(cst_theorem, Istring("theorem"));
    set(cst_xmlns, Istring("xmlns"));

    set(cst_dig0, Istring("0"));
    set(cst_dig1, Istring("1"));
    set(cst_dig2, Istring("2"));
    set(cst_dig3, Istring("3"));
    set(cst_dig4, Istring("4"));
    set(cst_dig5, Istring("5"));
    set(cst_dig6, Istring("6"));
    set(cst_dig7, Istring("7"));
    set(cst_dig8, Istring("8"));
    set(cst_dig9, Istring("9"));

    set(cstb_article, Istring("article"));
    set(cstb_book, Istring("book"));
    set(cstb_booklet, Istring("booklet"));
    set(cstb_conference, Istring("conference"));
    set(cstb_coursenotes, Istring("coursenotes"));
    set(cstb_comment, Istring("comment"));
    set(cstb_extension, Istring("extension"));
    set(cstb_inbook, Istring("inbook"));
    set(cstb_incollection, Istring("incollection"));
    set(cstb_inproceedings, Istring("inproceedings"));
    set(cstb_manual, Istring("manual"));
    set(cstb_masterthesis, Istring("mastersthesis")); // \todo why swap?
    set(cstb_mastersthesis, Istring("masterthesis")); // \todo why swap?
    set(cstb_misc, Istring("misc"));
    set(cstb_phdthesis, Istring("phdthesis"));
    set(cstb_proceedings, Istring("proceedings"));
    set(cstb_preamble, Istring("preamble"));
    set(cstb_techreport, Istring("techreport"));
    set(cstb_string, Istring("string"));
    set(cstb_unpublished, Istring("unpublished"));
    set(cstb_unknown, Istring("unknown"));
    set(cstb_address, Istring("address"));
    set(cstb_author, Istring("author"));
    set(cstb_booktitle, Istring("booktitle"));
    set(cstb_chapter, Istring("chapter"));
    set(cstb_crossref, Istring("crossref"));
    set(cstb_doi, Istring("doi"));
    set(cstb_edition, Istring("edition"));
    set(cstb_editor, Istring("editor"));
    set(cstb_howpublished, Istring("howpublished"));
    set(cstb_institution, Istring("institution"));
    set(cstb_isbn, Istring("isbn"));
    set(cstb_issn, Istring("issn"));
    set(cstb_isrn, Istring("isrn"));
    set(cstb_journal, Istring("journal"));
    set(cstb_key, Istring("key"));
    set(cstb_month, Istring("month"));
    set(cstb_language, Istring("language"));
    set(cstb_langue, Istring("langue"));
    set(cstb_note, Istring("note"));
    set(cstb_number, Istring("number"));
    set(cstb_organization, Istring("organization"));
    set(cstb_pages, Istring("pages"));
    set(cstb_publisher, Istring("publisher"));
    set(cstb_school, Istring("school"));
    set(cstb_series, Istring("series"));
    set(cstb_title, Istring("title"));
    set(cstb_type, Istring("type"));
    set(cstb_url, Istring("url"));
    set(cstb_volume, Istring("volume"));
    set(cstb_year, Istring("year"));
    set(cstb_equipe, Istring("equipe"));
    set(cstb_subtype, Istring("subtype"));
    set(cstb_unite, Istring("unite"));
    set(cstb_statut, Istring("statut"));
    set(cstb_section, Istring("section"));
    set(cst_numalign, Istring("numalign"));
    set(cst_denalign, Istring("denomalign"));

    set(cstf_normal, Istring("normal"));
    set(cstf_upright, Istring());
    set(cstf_bold, Istring("bold"));
    set(cstf_italic, Istring("italic"));
    set(cstf_bold_italic, Istring("bold-italic"));
    set(cstf_script, Istring("script"));
    set(cstf_bold_script, Istring("bold-script"));
    set(cstf_fraktur, Istring("fraktur"));
    set(cstf_doublestruck, Istring("double-struck"));
    set(cstf_bold_fraktur, Istring("bold-fraktur"));
    set(cstf_sansserif, Istring("sans-serif"));
    set(cstf_bold_sansserif, Istring("bold-sans-serif"));
    set(cstf_sansserif_italic, Istring("sans-serif-italic"));
    set(cstf_sansserif_bold_italic, Istring("sans-serif-bold-italic"));
    set(cstf_monospace, Istring("monospace"));

    set(np_a1, Istring("a1"));
    set(np_a2, Istring("a2"));
    set(np_abarnodeconnect, Istring("abarnodeconnect"));
    set(np_accueil, Istring("accueil"));
    set(np_affiliation, Istring("affiliation"));
    set(np_allowbreak, Istring("allowbreak"));
    set(np_alt_head, Istring("alt_head"));
    set(np_alt_caption, Istring("alt_head"));
    set(np_alt_section, Istring("alt_head"));
    set(np_anchor, Istring("anchor"));
    set(np_angle, Istring("angle"));
    set(np_anodeconnect, Istring("anodeconnect"));
    set(np_anodecurve, Istring("anodecurve"));
    set(np_arc, Istring("pic-arc"));
    set(np_arg1, Istring("arg1"));
    set(np_arg2, Istring("arg2"));
    set(np_arg3, Istring("arg3"));
    set(np_array, Istring("array"));
    set(np_alternatives, Istring("alternatives"));

    set(np_b1, Istring("b1"));
    set(np_b2, Istring("b2"));
    set(np_b_rend, Istring("rend"));
    set(np_backmatter, Istring("backmatter"));
    set(np_bauteurs, Istring("bauteurs"));
    set(np_barnodeconnect, Istring("barnodeconnect"));
    set(np_bediteurs, Istring("bediteur"));
    set(np_bezier, Istring("pic-bezier"));
    set(np_biblio, Istring("biblio"));
    set(np_bigcircle, Istring("pic-bigcircle"));
    set(np_bottomborder, Istring("bottom-border"));
    set(np_border_bottomw, Istring("border-bottom-width"));
    set(np_border_topw, Istring("border-top-width"));
    set(np_border_leftw, Istring("border-left-width"));
    set(np_border_rightw, Istring("border-right-width"));

    set(np_box, Istring("pic-framebox"));
    set(np_box_pos, Istring("position"));
    set(np_boxed, Istring("boxed"));
    set(np_bpers, Istring("bpers"));
    set(np_box_width, Istring("width"));
    set(np_bibitem, Istring("bibitem"));
    set(np_bibkey, Istring("bib-key"));

    set(np_c1, Istring("c1"));
    set(np_c2, Istring("c2"));
    set(np_c_left, Istring("left"));
    set(np_c_right, Istring("right"));
    set(np_c_center, Istring("center"));
    set(np_caption, Istring("caption"));
    set(np_captions, Istring("head"));
    set(np_catperso, Istring("catperso"));
    set(np_cell, Istring("cell"));
    set(np_center, Istring("center"));
    set(np_center_etc, Istring("center"));
    set(np_center_etc1, Istring("center"));
    set(np_center_etc2, Istring("quoted"));
    set(np_center_etc3, Istring("quoted"));
    set(np_center_etc4, Istring("flushed-left"));
    set(np_center_etc5, Istring("flushed-right"));
    set(np_center_etc6, Istring("verse"));
    set(np_centered, Istring("centered"));
    set(np_chapters, Istring("chapters"));
    set(np_circle, Istring("pic-circle"));
    set(np_natcit, Istring("Cit"));
    set(np_cit, Istring("cit"));
    set(np_cite_type, Istring("type"));
    set(np_citation, Istring("citation"));
    set(np_cleaders, Istring("cleaders"));
    set(np_clip, Istring("clip"));
    set(np_close, Istring("close"));
    set(np_closecurve, Istring("pic-closecurve"));
    set(np_columnalign, Istring("columnalign"));
    set(np_columnspan, Istring("columnspan"));
    set(np_pagecolor, Istring("pagecolor"));
    set(np_color, Istring("color"));
    set(np_color2, Istring("color2"));
    set(np_cols, Istring("cols"));
    set(np_cst_width, Istring("width"));
    set(np_curve, Istring("pic-curve"));
    set(np_curve_nbpts, Istring("nbsymb"));

    set(np_dashbox, Istring("pic-dashbox"));
    set(np_dashline, Istring("dashline"));
    set(np_dashdim, Istring("dashdim"));
    set(np_depth, Istring("depth"));
    set(np_depthA, Istring("depthA"));
    set(np_depthB, Istring("depthB"));
    set(np_description, Istring("description"));
    set(np_display, Istring("display"));
    set(cst_div0, Istring("div0"));
    set(cst_div1, Istring("div1"));
    set(cst_div2, Istring("div2"));
    set(cst_div3, Istring("div3"));
    set(cst_div4, Istring("div4"));
    set(cst_div5, Istring("div5"));
    set(cst_div6, Istring("div6"));
    set(np_div0, Istring("div0"));
    set(np_div1, Istring("div1"));
    set(np_div2, Istring("div2"));
    set(np_div3, Istring("div3"));
    set(np_div4, Istring("div4"));
    set(np_div5, Istring("div5"));
    set(np_div6, Istring("div6"));
    set(np_dottedline, Istring("dottedline"));
    set(np_drawline, Istring("drawline"));
    set(np_dx, Istring("dx"));
    set(np_dy, Istring("dy"));

    set(np_encap, Istring("encap"));
    set(np_english, Istring("english"));
    set(np_eqnpos, Istring("eqnpos"));
    set(np_error, Istring("error"));
    set(np_Error, Istring("Error"));
    set(np_warning, Istring(""));
    set(np_Warning, Istring("Warning"));
    set(np_Info, Istring("Info"));
    set(np_inline, Istring("inline"));

    set(np_false, Istring("false"));
    set(np_fbox, Istring("fbox"));
    set(np_figure, Istring("figure"));
    set(np_file, Istring("file"));
    set(np_fileextension, Istring("extension"));
    set(np_float, Istring("float"));
    set(np_float_figure, Istring("figure"));
    set(np_float_table, Istring("table"));
    set(np_font_bold, Istring("bold"));
    set(np_font_boldextended, Istring("boldextended"));
    set(np_font_semibold, Istring("semibold"));
    set(np_font_condensed, Istring("condensed"));
    set(np_font_it, Istring("it"));
    set(np_font_large, Istring("large"));
    set(np_font_large1, Istring("large1"));
    set(np_font_large2, Istring("large2"));
    set(np_font_large3, Istring("large3"));
    set(np_font_large4, Istring("large4"));
    set(np_font_large5, Istring("large5"));
    set(np_font_large6, Istring("large6"));
    set(np_font_medium, Istring("medium"));
    set(np_font_normalsize, Istring("normalsize"));
    set(np_font_roman, Istring("roman"));
    set(np_font_sansserif, Istring("sansserif"));
    set(np_font_sc, Istring("sc"));
    set(np_font_slanted, Istring("slanted"));
    set(np_font_small, Istring("small"));
    set(np_font_small1, Istring("small1"));
    set(np_font_small2, Istring("small2"));
    set(np_font_small3, Istring("small3"));
    set(np_font_small4, Istring("small4"));
    set(np_font_small5, Istring("small5"));
    set(np_font_small6, Istring("small6"));
    set(np_font_tt, Istring("tt"));
    set(np_font_upright, Istring("upright"));
    set(np_footnote, Istring("note"));
    set(np_form, Istring("form"));
    set(np_formula, Istring("formula"));
    set(np_foot, Istring("foot"));
    set(np_frame, Istring("pic-frame"));
    set(np_framed, Istring("framed"));
    set(np_french, Istring("french"));
    set(np_from, Istring("from"));
    set(np_frontmatter, Istring("frontmatter"));
    set(np_full, Istring("full"));
    set(np_full_first, Istring("prenomcomplet"));

    set(np_german, Istring("german"));
    set(np_glo_name, Istring("Glossary"));
    set(np_gloss, Istring("gloss"));
    set(np_graphics, Istring("graphics"));

    set(np_halfem, Istring("4.pt"));
    set(np_halign, Istring("halign"));
    set(np_hdr, Istring("hdr"));
    set(np_head, Istring("head"));
    set(np_header, Istring("headings"));
    set(np_height, Istring("height"));
    set(np_hfil, Istring("hfil"));
    set(np_hfill, Istring("hfill"));
    set(np_hfilneg, Istring("hfilneg"));
    set(np_html, Istring("html"));
    set(np_hss, Istring("hss"));

    set(np_id, Istring("id"));
    set(np_idtext, Istring("id-text"));
    set(np_index, Istring("index"));
    set(np_glossary, Istring("glossary"));
    set(np_inline, Istring("inline"));
    set(np_isproject, Istring("isproject"));
    set(np_item, Istring("item"));

    set(np_junior, Istring("junior"));

    set(np_key, Istring("key"));
    set(np_keywords, Istring("keywords"));

    set(np_label_glo, Istring("label"));
    set(np_label_item, Istring("label"));
    set(np_language, Istring());
    set(np_leaders, Istring("leaders"));
    set(np_left, Istring("left"));
    set(np_leftborder, Istring("left-border"));
    set(np_leg, Istring("leg"));
    set(np_level, Istring("level"));
    set(np_linethickness, Istring("linethickness"));
    set(np_lineC, Istring("line"));
    set(np_line, Istring("pic-line"));
    set(np_line_thickness, Istring("pic-linethickness"));
    set(np_list, Istring("list"));
    set(np_llap, Istring("llap"));
    set(np_rlap, Istring("rlap"));
    set(np_rasection, Istring(""));

    set(np_mainmatter, Istring("mainmatter"));
    set(np_mbox, Istring("mbox"));
    set(np_marginpar, Istring("marginpar"));
    set(np_minipage, Istring("minipage"));
    set(np_minipage_width, Istring("width"));
    set(np_module, Istring("module"));
    set(np_moreinfo, Istring("moreinfo"));
    set(np_movablelimits, Istring("movablelimits"));
    set(np_multiput, Istring("pic-multiput"));

    set(np_name, Istring("name"));
    set(np_nameA, Istring("nameA"));
    set(np_nameB, Istring("nameB"));

    set(np_node, Istring("node"));
    set(np_nodebox, Istring("nodebox"));
    set(np_nodecircle, Istring("nodecircle"));
    set(np_nodeconnect, Istring("nodeconnect"));
    set(np_nodecurve, Istring("nodecurve"));
    set(np_nodeoval, Istring("nodeoval"));
    set(np_nodetriangle, Istring("nodetriangle"));
    set(np_noindent, Istring("noindent"));
    set(np_nom, Istring("nom"));
    set(np_nonumber, Istring("nonumber"));

    set(np_open, Istring("open"));
    set(np_ordered, Istring("ordered"));
    set(np_oval, Istring("pic-oval"));
    set(np_overline, Istring("overline"));

    set(np_page, Istring("page"));
    set(np_part, Istring("part"));
    set(np_part0, Istring("participant"));
    set(np_part1, Istring("participante"));
    set(np_part2, Istring("participantes"));
    set(np_part3, Istring("participants"));
    set(np_particle, Istring("part"));
    set(np_pers, Istring("pers"));
    set(np_place, Istring("place"));
    set(np_picture, Istring("picture"));
    set(np_point, Istring("point"));
    set(np_pos, Istring("pos"));
    set(np_posA, Istring("posA"));
    set(np_posB, Istring("posB"));
    set(np_posi, Istring("inner-pos"));
    set(np_pre, Istring("pre"));
    set(np_prefix, Istring("prefix"));
    set(np_prenom, Istring("prenom"));
    set(np_prenote, Istring("prenote"));
    set(np_profession, Istring("profession"));
    set(np_research_centre, Istring("research-centre"));
    set(np_projet, Istring("projet"));
    set(np_projetdeveloppe, Istring("projetdeveloppe"));
    set(np_put, Istring("pic-put"));

    set(np_quoted, Istring("quoted"));

    set(np_r_angle, Istring("angle"));
    set(np_raisebox, Istring("raisebox"));
    set(np_ref, Istring("ref"));
    set(np_rend, Istring("rend"));
    set(np_repeat, Istring("repeat"));
    set(np_right, Istring("right"));
    set(np_rightborder, Istring("right-border"));
    set(np_rotatebox, Istring("pic-rotatebox"));
    set(np_row, Istring("row"));
    set(np_rule, Istring("rule"));
    set(np_rule_tsa, Istring("top_rule_space_above"));
    set(np_rule_tsb, Istring("top_rule_space_below"));
    set(np_rule_bsa, Istring("bottom_rule_space_above"));
    set(np_rule_bsb, Istring("bottom_rule_space_below"));
    set(np_rclist, Istring("UR"));
    set(np_rcval, Istring("+UR"));

    set(np_sbox, Istring("scalebox"));
    set(np_sc, Istring("sc"));
    set(np_scale, Istring("scale"));
    set(np_scaleput, Istring("pic-scaleput"));
    set(np_s_caps, Istring("caps"));
    set(np_s_hl, Istring("hl"));
    set(np_s_old, Istring("oldstyle"));
    set(np_s_scale, Istring("scale"));
    set(np_s_so, Istring("so"));
    set(np_s_st, Istring("st"));
    set(np_s_sub, Istring("sub"));
    set(np_s_sup, Istring("sup"));
    set(np_s_ul, Istring("ul"));
    set(np_separator, Istring(""));
    set(np_simple, Istring("simple"));
    set(np_simplemath, Istring("simplemath"));
    set(np_size, Istring("size"));
    set(np_spaceafter, Istring("spaceafter"));
    set(np_spacebefore, Istring("spacebefore"));
    set(np_specs, Istring("specs"));
    set(np_style, Istring("style"));
    set(np_starred, Istring("starred"));
    set(np_subfigure, Istring("subfigure"));
    set(np_stylesheet, Istring(""));
    set(np_stylesheet_type, Istring("text/css"));

    set(np_table, Istring("table"));
    set(np_Table, Istring("Table"));
    set(np_tabular, Istring("tabular"));
    set(np_tabular_star, Istring("tabular*"));
    set(np_tab_width, Istring("width"));
    set(np_tagcurve, Istring("pic-tagcurve"));
    set(np_target, Istring("target"));
    set(np_term, Istring("term"));
    set(np_texte, Istring("texte"));
    set(np_texmath, Istring("texmath"));
    set(np_theindex, Istring("theindex"));
    set(np_theglossary, Istring("theglossary"));
    set(np_theme, Istring("theme"));
    set(np_theorem, Istring(""));
    set(np_thick_lines, Istring("pic-thicklines"));
    set(np_thin_lines, Istring("pic-thinlines"));
    set(np_toc, Istring("tableofcontents"));
    set(np_toc1, Istring("listoftables"));
    set(np_toc2, Istring("listoffigures"));
    set(np_topborder, Istring("top-border"));
    set(np_totalwidth, Istring("totalwidth"));
    set(np_true, Istring("true"));
    set(np_type, Istring("type"));
    set(np_textype, Istring("textype"));

    set(np_underline, Istring("underline"));
    set(np_unknown, Istring("unknown"));
    set(np_unit_length, Istring("unit-length"));
    set(np_url, Istring("url"));
    set(np_userid, Istring("userid"));
    set(np_user_list, Istring("description"));

    set(np_val, Istring("val"));
    set(np_vector, Istring("pic-vector"));
    set(np_vfil, Istring("vfil"));
    set(np_vfill, Istring("vfill"));
    set(np_vfilneg, Istring("vfilneg"));
    set(np_vpos, Istring("vpos"));
    set(np_vss, Istring("vss"));

    set(np_width, Istring("width"));

    set(np_xscale, Istring("xscale"));
    set(np_xscaley, Istring("xscaley"));
    set(np_xdir, Istring("xdir"));
    set(np_xleaders, Istring("xleaders"));
    set(np_xpos, Istring("xpos"));
    set(np_xref, Istring("xref"));

    set(np_yscale, Istring("yscale"));
    set(np_yscalex, Istring("yscalex"));
    set(np_ydir, Istring("ydir"));
    set(np_ypos, Istring("ypos"));

    set(np_zerodim, Istring("0pt"));
    set(np_mml_normal, Istring("mml@font@normal"));
    set(np_mml_upright, Istring("mml@font@upright"));
    set(np_mml_bold, Istring("mml@font@bold"));
    set(np_mml_italic, Istring("mml@font@italic"));
    set(np_mml_bold_italic, Istring("mml@font@bolditalic"));
    set(np_mml_script, Istring("mml@font@script"));
    set(np_mml_bold_script, Istring("mml@font@boldscript"));
    set(np_mml_fraktur, Istring("mml@font@fraktur"));
    set(np_mml_doublestruck, Istring("mml@font@doublestruck"));
    set(np_mml_bold_fraktur, Istring("mml@font@boldfraktur"));
    set(np_mml_sansserif, Istring("mml@font@sansserif"));
    set(np_mml_bold_sansserif, Istring("mml@font@boldsansserif"));
    set(np_mml_sansserif_italic, Istring("mml@font@sansserifitalic"));
    set(np_mml_sansserif_bold_italic, Istring("mml@font@sansserifbolditalic"));
    set(np_mml_monospace, Istring("mml@font@monospace"));

    set(np_3pt, Istring("3pt"));
    set(np_6pt, Istring("6pt"));
    set(np_12pt, Istring("12pt"));
    set(np_letters_tl, Istring("tl"));
    set(np_letters_tr, Istring("tr"));
    set(np_letters_bl, Istring("bl"));
    set(np_letters_br, Istring("br"));
    std::array<char, 2> foo{};
    foo[1] = 0;
    for (char x = 'a'; x <= 'z'; x++) {
        foo[0] = x;
        set(to_unsigned(np_letter_a + x - 'a'), Istring(foo.data()));
    }
    for (char x = 'A'; x <= 'Z'; x++) {
        foo[0] = x;
        set(to_unsigned(np_letter_A + x - 'A'), Istring(foo.data()));
    }
}

auto NameMapper::assign_name(const std::string &A, const std::string &B) -> bool {
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

auto NameMapper::assign_att(const std::string &A, const std::string &B) -> bool {
    if (A == "angle") {
        set(np_angle, Istring(B));
        return true;
    }
    if (A == "affiliation") {
        set(np_affiliation, Istring(B));
        return true;
    }
    if (A == "box_pos") {
        set(np_box_pos, Istring(B));
        return true;
    }
    if (A == "box_scale") {
        set(np_s_scale, Istring(B));
        return true;
    }
    if (A == "box_width") {
        set(np_box_width, Istring(B));
        return true;
    }
    if (A == "boxed") {
        set(np_boxed, Istring(B));
        return true;
    }
    if (A == "bibkey") {
        set(np_bibkey, Istring(B));
        return true;
    }
    if (A == "border_bottom_width") {
        set(np_border_bottomw, Istring(B));
        return true;
    }
    if (A == "border_top_width") {
        set(np_border_topw, Istring(B));
        return true;
    }
    if (A == "border_left_width") {
        set(np_border_leftw, Istring(B));
        return true;
    }
    if (A == "border_right_width") {
        set(np_border_rightw, Istring(B));
        return true;
    }
    if (A == "centering") {
        set(np_center_etc1, Istring(B));
        return true;
    }
    if (A == "clip") {
        set(np_clip, Istring(B));
        return true;
    }
    if (A == "cols") {
        set(np_cols, Istring(B));
        return true;
    }
    if (A == "cell_left") {
        set(np_c_left, Istring(B));
        return true;
    }
    if (A == "cell_right") {
        set(np_c_right, Istring(B));
        return true;
    }
    if (A == "cell_center") {
        set(np_c_center, Istring(B));
        return true;
    }
    if (A == "cell_leftborder") {
        set(np_leftborder, Istring(B));
        return true;
    }
    if (A == "cell_rightborder") {
        set(np_rightborder, Istring(B));
        return true;
    }
    if (A == "cell_topborder") {
        set(np_topborder, Istring(B));
        return true;
    }
    if (A == "cell_bottomborder") {
        set(np_bottomborder, Istring(B));
        return true;
    }
    if (A == "curve_nbpts") {
        set(np_curve_nbpts, Istring(B));
        return true;
    }
    if (A == "display") {
        set(np_display, Istring(B));
        return true;
    }
    if (A == "dx") {
        set(np_dx, Istring(B));
        return true;
    }
    if (A == "dy") {
        set(np_dy, Istring(B));
        return true;
    }
    if (A == "depthA") {
        set(np_depthA, Istring(B));
        return true;
    }
    if (A == "depthB") {
        set(np_depthB, Istring(B));
        return true;
    }
    if (A == "depth") {
        set(np_depth, Istring(B));
        return true;
    }
    if (A == "encap") {
        set(np_encap, Istring(B));
        return true;
    }
    if (A == "framed") {
        set(np_framed, Istring(B));
        return true;
    }
    if (A == "full") {
        set(np_full, Istring(B));
        return true;
    }
    if (A == "file") {
        set(np_file, Istring(B));
        return true;
    }
    if (A == "file_extension") {
        set(np_fileextension, Istring(B));
        return true;
    }
    if (A == "fbox_rend") {
        set(np_b_rend, Istring(B));
        return true;
    }
    if (A == "flush_left") {
        set(np_center_etc4, Istring(B));
        return true;
    }
    if (A == "flush_right") {
        set(np_center_etc5, Istring(B));
        return true;
    }
    if (A == "foot_position") {
        set(np_foot, Istring(B));
        return true;
    }
    if (A == "full_first") {
        set(np_full_first, Istring(B));
        return true;
    }
    if (A == "gloss_type") {
        set(np_gloss, Istring(B));
        return true;
    }
    if (A == "height") {
        set(np_height, Istring(B));
        return true;
    }
    if (A == "halign") {
        set(np_halign, Istring(B));
        return true;
    }
    if (A == "hdr") {
        set(np_hdr, Istring(B));
        return true;
    }
    if (A == "inner_pos") {
        set(np_posi, Istring(B));
        return true;
    }
    if (A == "inline") {
        set(np_inline, Istring(B));
        return true;
    }
    if (A == "junior") {
        set(np_junior, Istring(B));
        return true;
    }
    if (A == "language") {
        set(np_language, Istring(B));
        return true;
    }
    if (A == "level") {
        set(np_level, Istring(B));
        return true;
    }
    if (A == "minipage_width") {
        set(np_minipage_width, Istring(B));
        return true;
    }
    if (A == "marginpar") {
        set(np_marginpar, Istring(B));
        return true;
    }
    if (A == "mathmlns") {
        set(cst_mathml, Istring(B));
        return true;
    }
    if (A == "marginpar") {
        set(np_marginpar, Istring(B));
        return true;
    }
    if (A == "mode") {
        set(cst_mode, Istring(B));
        return true;
    }
    if (A == "noindent") {
        set(np_noindent, Istring(B));
        return true;
    }
    if (A == "nonumber") {
        set(np_nonumber, Istring(B));
        return true;
    }
    if (A == "nom") {
        set(np_nom, Istring(B));
        return true;
    }
    if (A == "name") {
        set(np_name, Istring(B));
        return true;
    }
    if (A == "nameA") {
        set(np_nameA, Istring(B));
        return true;
    }
    if (A == "nameB") {
        set(np_nameB, Istring(B));
        return true;
    }
    if (A == "place") {
        set(np_place, Istring(B));
        return true;
    }
    if (A == "prenom") {
        set(np_prenom, Istring(B));
        return true;
    }
    if (A == "pre") {
        set(np_pre, Istring(B));
        return true;
    }
    if (A == "particule") {
        set(np_particle, Istring(B));
        return true;
    }
    if (A == "page") {
        set(np_page, Istring(B));
        return true;
    }
    if (A == "profession") {
        set(np_profession, Istring(B));
        return true;
    }
    if (A == "posA") {
        set(np_posA, Istring(B));
        return true;
    }
    if (A == "posB") {
        set(np_posB, Istring(B));
        return true;
    }
    if (A == "pos") {
        set(np_pos, Istring(B));
        return true;
    }
    if (A == "quote") {
        set(np_center_etc2, Istring(B));
        return true;
    }
    if (A == "quotation") {
        set(np_center_etc3, Istring(B));
        return true;
    }
    if (A == "rotate_angle") {
        set(np_r_angle, Istring(B));
        return true;
    }
    if (A == "rend") {
        set(np_rend, Istring(B));
        return true;
    }
    if (A == "row_spaceafter") {
        set(np_spaceafter, Istring(B));
        return true;
    }
    if (A == "repeat") {
        set(np_repeat, Istring(B));
        return true;
    }
    if (A == "scale") {
        set(np_scale, Istring(B));
        return true;
    }
    if (A == "space_before") {
        set(np_spacebefore, Istring(B));
        return true;
    }
    if (A == "size") {
        set(np_size, Istring(B));
        return true;
    }
    if (A == "starred") {
        set(np_starred, Istring(B));
        return true;
    }
    if (A == "table_width") {
        set(np_tab_width, Istring(B));
        return true;
    }
    if (A == "type") {
        set(np_type, Istring(B));
        return true;
    }
    if (A == "textype") {
        set(np_textype, Istring(B));
        return true;
    }
    if (A == "unit_length") {
        set(np_unit_length, Istring(B));
        return true;
    }
    if (A == "user_list") {
        set(np_user_list, Istring(B));
        return true;
    }
    if (A == "vpos") {
        set(np_vpos, Istring(B));
        return true;
    }
    if (A == "verse") {
        set(np_center_etc6, Istring(B));
        return true;
    }
    if (A == "width") {
        set(np_width, Istring(B));
        return true;
    }
    if (A == "xscale") {
        set(np_xscale, Istring(B));
        return true;
    }
    if (A == "xscaley") {
        set(np_xscaley, Istring(B));
        return true;
    }
    if (A == "xpos") {
        set(np_xpos, Istring(B));
        return true;
    }
    if (A == "xdir") {
        set(np_xdir, Istring(B));
        return true;
    }
    if (A == "yscale") {
        set(np_yscale, Istring(B));
        return true;
    }
    if (A == "yscalex") {
        set(np_yscalex, Istring(B));
        return true;
    }
    if (A == "ydir") {
        set(np_ydir, Istring(B));
        return true;
    }
    if (A == "ypos") {
        set(np_ypos, Istring(B));
        return true;
    }
    return false;
}
