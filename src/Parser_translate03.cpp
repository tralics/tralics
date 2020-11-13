#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"

namespace {
    auto hfill_to_np(subtypes c) -> std::string {
        if (c == hfill_code) return "hfill";
        if (c == hfilneg_code) return "hfilneg";
        if (c == hss_code) return "hss";
        return "hfil";
    }

    auto vfill_to_np(subtypes c) -> std::string {
        if (c == vfill_code) return "vfill";
        if (c == vfilneg_code) return "vfilneg";
        if (c == vss_code) return "vss";
        return "vfil";
    }
} // namespace

void Parser::translate03() {
    auto guard = SaveErrTok(cur_tok);

    if (cur_cmd_chr.cmd == underscore_catcode && global_in_load) {
        translate_char(cur_cmd_chr);
        return;
    }
    auto [x, c] = cur_cmd_chr;
    switch (x) {
    case cst1_cmd:
    case cst_cmd:
        LC();
        T_cst1(c);
        return;
    case cst2_cmd: T_cst2(c); return;
    case nobreakspace_cmd:
        LC();
        if (global_in_url)
            process_char('~');
        else
            process_char(0xA0);
        return;
    case leave_v_mode_cmd: leave_v_mode(); return;
    case space_catcode:
        if (the_stack.in_v_mode() || the_stack.in_no_mode() || the_stack.in_bib_mode()) return;
        process_char(char32_t(char32_t(c)));
        return;
    case letter_catcode:
    case other_catcode: translate_char(cur_cmd_chr); return;
    case char_num_cmd: extended_chars(scan_27bit_int()); return;
    case char_given_cmd: extended_chars(c); return;
    case fvset_cmd: special_fvset(); return;
    case biblio_cmd: T_biblio(); return;
    case omitcite_cmd: T_omitcite(); return;
    case bibliographystyle_cmd: T_bibliostyle(); return;
    case insertbibliohere_cmd: add_bib_marker(true); return;
    case inhibit_xml_cmd:
        the_main.no_xml = true;
        log_and_tty << "\nsyntaxonly: no XML file will be produced\n";
        return;
    case endcsname_cmd: parse_error("Extra \\endcsname"); return;
    case xmllatex_cmd:
        LC();
        unprocessed_xml += T_xmllatex();
        return;
    case scan_glue_cmd: T_scan_glue(c); return;
    case kern_cmd:
        scan_dimen(c == 1, cur_tok);
        // so what ? we could append hspace or vspace here.
        return;
    case aparaitre_cmd:
        LC();
        // FIXME this is latex not xml
        if (eqtb_int_table[language_code].val == 1) {
            process_char(char32_t(0xE0U));
            process_string(" para");
            process_char(char32_t(0xEEU));
            process_string("tre");
        } else
            process_string("to appear");
        return;
    case calc_cmd: exec_calc(); return;
    case dollar_catcode:
        flush_buffer();
        T_math(nomathenv_code);
        return;
    case alignment_catcode: T_ampersand(); return;
    case open_catcode: push_level(bt_brace); return;
    case close_catcode: pop_level(bt_brace); return;
    case begingroup_cmd:
        flush_buffer();
        if (c == 0)
            push_level(bt_semisimple);
        else if (c == 1)
            pop_level(bt_semisimple);
        else {
            get_token();
            pop_level(bt_env);
        }
        return;
    case hat_catcode:
    case underscore_catcode:
        if (global_in_load || is_pos_par(nomath_code))
            translate_char(CmdChr(letter_catcode, c));
        else
            parse_error(cur_tok, "Missing dollar not inserted, token ignored: ", cur_tok.tok_to_str(), "Missing dollar");
        return;
    case backslash_cmd:
        if (c == 0)
            T_backslash();
        else
            T_newline();
        return;
    case hline_cmd: // cline, hline, hlinee
        T_hline(c);
        return;
    case skip_cmd: {
        int w = c == smallskip_code ? 3 : c == medskip_code ? 6 : 12;
        append_glue(cur_tok, w << 16, true);
        return;
    }
    case hfill_cmd:
        leave_v_mode();
        the_stack.add_newid0(hfill_to_np(c));
        return;
    case vfill_cmd:
        leave_h_mode();
        the_stack.add_newid0(vfill_to_np(c));
        return;
    case sub_cmd:
    case soul_cmd: {
        static const std::string list[] = {"sup", "sub", "oldstyle", "caps", "hl", "so", "st", "ul"}; // \todo somewhere else
        T_fonts(list[c]);
        return;
    }
    case trees_cmd: T_trees(c); return;
    case matter_cmd: T_matter(c); return;
    case arg_font_cmd: T_fonts("font_sc"); return;
    case special_math_cmd:
        if (c == overline_code || c == underline_code)
            T_fonts(c == overline_code ? "overline" : "underline");
        else
            math_only();
        return;
    case noargfont_cmd: see_font_change(c); return;
    case argfont_cmd: arg_font(c); return;
    case oldfont_cmd: old_font(); return;
    case fontsize_cmd: translate_font_size(); return;
    case fnhack_cmd: fnhack(); return;
    case footnote_cmd: T_cap_or_note(false); return;
    case selectfont_cmd: font_has_changed(); return;
    case ltfont_cmd:
        flush_buffer();
        cur_font.ltfont(sT_arg_nopar(), c);
        return;
    case usefont_cmd: T_usefont(); return;
    case isin_cmd: T_isin(); return;
    case citation_cmd:
        // should be mode independent...
        T_citation();
        the_stack.add_nl();
        return;
    case bib_cmd: T_bauteursediteurs(c); return;
    case line_cmd: T_line(c); return;
    case bpers_cmd: T_bpers(); return;
    case cititem_cmd: T_cititem(); return;
    case bibitem_cmd: c == 1 ? T_empty_bibitem() : T_bibitem(); return;
    case end_citation_cmd: the_stack.pop(the_names["citation"]); return;
    case ignoreA_cmd:
        T_ignoreA();
        return;
        // these are mode independent...
    case ignore_cmd:
        if (c == addnl_code) {
            flush_buffer();
            the_stack.add_nl();
        } else if (c == unskip_code) {
            if (unprocessed_xml.empty())
                the_stack.remove_last_space();
            else
                unprocessed_xml.remove_last_space();
        }
        return;
    case relax_cmd:
    case eof_marker_cmd: return;
    case nolinebreak_cmd: ignore_optarg(); return;
    case ignore_one_argument_cmd:
        if (c == patterns_code || c == hyphenation_code || c == special_code) scan_left_brace_and_back_input();
        ignore_arg();
        return;
    case ignore_two_argument_cmd:
        ignore_arg();
        ignore_arg();
        return;
    case defineverbatimenv_cmd: T_define_verbatim_env(); return;
    case saveverb_cmd: T_saveverb(); return;
    case xkeyval_cmd: T_xkeyval(c); return;
    case add_to_macro_cmd: T_addtomacro(c == 1); return;
    case makeatletter_cmd: word_define('@', letter_catcode, false); return;
    case makeatother_cmd: word_define('@', other_catcode, false); return;
    case numberedverbatim_cmd: numbered_verbatim = true; return;
    case unnumberedverbatim_cmd: numbered_verbatim = false; return;
    case after_assignment_cmd:
        get_token();
        set_after_ass_tok(cur_tok);
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{\\afterassignment: " << cur_tok << "}\n";
        }
        return;
    case last_item_cmd: parse_error(cur_tok, "Read only variable ", cur_tok, "", "readonly"); return;
    case XML_swap_cmd: user_XML_swap(c); return;
    case XML_fetch_cmd: user_XML_fetch(); return;
    case XML_modify_cmd: user_XML_modify(c); return;
    case un_box_cmd: T_un_box(c); return;
    case extension_cmd: M_extension(c); return;
    case setlanguage_cmd: //  strange...
        scan_int(cur_tok);
        return;
    case xray_cmd: M_xray(c); return;
    case move_cmd:
        scan_dimen(false, cur_tok); // ignore dimension
        scan_box(move_location);    // read a box and insert the value
        return;
    case save_box_cmd: T_save_box(c == 0); return;
    case make_box_cmd: begin_box(makebox_location, c); return;
    case leader_ship_cmd:
        scan_box(c == shipout_code    ? shipout_location
                 : c == leaders_code  ? leaders_location
                 : c == cleaders_code ? cleaders_location
                                      : xleaders_location);
        return;
    case tracingall_cmd: M_tracingall(); return;
    case ifstar_cmd: T_ifstar(); return;
    case vglue_cmd:
        if (c == 0)
            T_par1();
        else
            leave_v_mode();
        T_scan_glue(c == 0 ? vskip_code : hskip_code);
        return;
    case for_cmd: T_xkv_for(c); return;
    case ifnextchar_cmd: T_ifnextchar(c == 0); return;
    case newif_cmd: M_newif(); return;
    case newcount_cmd: new_constant(c); return;
    case newboolean_cmd: M_newboolean(c); return;
    case setboolean_cmd: set_boolean(); return;
    case ifthenelse_cmd: T_ifthenelse(); return;
    case whiledo_cmd: T_whiledo(); return;
    case setmode_cmd: T_setmode(); return;
    case toks_register_cmd:
    case assign_toks_cmd:
    case assign_int_cmd:
    case assign_dimen_cmd:
    case assign_glue_cmd:
    case assign_mu_glue_cmd:
    case assign_font_dimen_cmd:
    case assign_font_int_cmd:
    case assign_enc_char_cmd:
    case set_aux_cmd:
    case set_prev_graf_cmd:
    case set_page_dimen_cmd:
    case set_page_int_cmd:
    case set_box_dimen_cmd:
    case set_shape_cmd:
    case def_code_cmd:
    case def_family_cmd:
    case set_font_cmd:
    case set_mathprop_cmd:
    case set_mathchar_cmd:
    case def_font_cmd:
    case register_cmd:
    case advance_cmd:
    case multiply_cmd:
    case divide_cmd:
    case prefix_cmd:
    case let_cmd:
    case shorthand_def_cmd:
    case read_to_cs_cmd:
    case def_cmd:
    case set_box_cmd:
    case set_interaction_cmd: M_prefixed(); return;
    case shortverb_cmd: M_shortverb(c); return;
    case usecounter_cmd: T_use_counter(); return;
    case newcounter_cmd: M_counter(true); return;
    case fp_cmd:
    case fpif_cmd: exec_fp_cmd(c); return;
    case fpi_cmd: exec_fpi_cmd(c); return;
    case aftergroup_cmd: T_aftergroup(); return;
    case listfiles_cmd: list_files_p = true; return;
    case ignorep_cmd:
    case par_cmd: T_par1(); return;
    case start_par_cmd: implicit_par(c); return;
    case caption_cmd: T_cap_or_note(true); return;
    case cite_cmd: T_cite(c); return;
    case item_cmd: T_item(c); return;
    case doc_class_cmd: T_documentclass(!the_stack.in_v_mode() || seen_document); return;
    case titlepage_cmd:
        if (!the_stack.in_v_mode()) wrong_mode("Bad titlepage command");
        T_titlepage(c);
        return;
    case package_cmd:
        if (!the_stack.in_v_mode() || seen_document) wrong_mode("Bad \\usepackage command");
        T_usepackage();
        return;
    case if_package_loaded_cmd: T_if_package_loaded(c == 0); return;
    case if_package_later_cmd: T_if_package_later(c == 0); return;
    case if_package_with_cmd: T_if_package_with(c == 0); return;
    case provides_package_cmd: T_provides_package(c == 0); return;
    case load_with_options_cmd: T_load_with_options(c == 0); return;
    case inputclass_cmd: T_inputclass(); return;
    case expandtwoargs_cmd: expand_twoargs(); return;
    case removeelement_cmd: T_remove_element(); return;
    case pass_options_cmd: T_pass_options(c == 0); return;
    case declare_options_cmd: T_declare_options(); return;
    case options_not_used_cmd: T_option_not_used(); return;
    case process_options_cmd: T_process_options(); return;
    case at_end_of_class_cmd: T_at_end_of_class(); return;
    case execute_options_cmd: T_execute_options(); return;
    case needs_format_cmd:
        ignore_arg();
        ignore_optarg();
        return;
    case subfigure_cmd: T_subfigure(); return;
    case pop_stack_cmd: pop_all_levels(); return;
    case section_cmd: T_paras(c); return;
    case label_cmd:
        flush_buffer();
        T_label(c);
        return;
    case ref_cmd:
        leave_v_mode();
        T_ref(c == 0);
        return;
    case hspace_cmd: T_hspace(c); return;
    case eqref_cmd: // Case \XMLref
        Xid(read_elt_id(cur_tok)).add_ref(sT_arg_nopar());
        return;
    case box_cmd: T_mbox(c); return;
    case centering_cmd:
        word_define(incentering_code, c, false);
        if (c != 0U) the_stack.add_center_to_p();
        return;
    case fbox_cmd:
        if (c == dashbox_code)
            T_fbox_dash_box();
        else if (c == rotatebox_code)
            T_fbox_rotate_box();
        else
            T_fbox(c);
        return;
    case includegraphics_cmd: includegraphics(c); return;
    case fancy_cmd: T_fancy(); return;
    case xfancy_cmd: T_xfancy(); return;
    case xthepage_cmd:
        flush_buffer();
        the_stack.add_last(the_page_xml);
        return;
    case case_shift_cmd: T_case_shift(c); return;
    case linebreak_cmd: ignore_optarg(); return;
    case url_cmd: T_url(); return;
    case hanl_cmd: T_hanl(c); return;
    case error_cmd: T_error(); return;
    case glo_cmd: T_glo(); return;
    case reevaluate_cmd: T_reevaluate(); return;
    case xmlelt_cmd: T_xmlelt(c); return;
    case newcolumntype_cmd: T_newcolumn_type(); return;
    case epsfbox_cmd: T_epsfbox(); return;
    case put_cmd: T_put(c); return;
    case curves_cmd: T_curves(c); return;
    case dashline_cmd: T_dashline(c); return;
    case bezier_cmd: T_bezier(c); return;
    case grabenv_cmd: T_grabenv(); return;
    case verb_cmd: T_verb(c != 0U ? verb_saved_char : char32_t(0U)); return;
    case gloss_cmd: T_gloss(c == 0); return;
    case only_preamble_cmd:
        get_r_token(true);
        onlypreamble.push_back(hash_table.let_token);
        onlypreamble.push_back(cur_tok);
        onlypreamble.push_back(hash_table.notprerr_token);
        return;
    case l3_gen_from_sig_cmd: generate_from_sig(); return;
    case l3_gen_from_ac_cmd: Tl3_gen_from_ac(c); return;
    case loadlatex3_cmd: L3_load(false); return;
    case GetIdInfo_cmd: L3_getid(); return;
    case GetIdInfoLog_cmd: L3_logid(); return;
    case l3_gen_cond_Npnn_cmd: L3_new_conditional_parm(c); return;
    case l3_gen_cond_Nnn_cmd: L3_new_conditional(c); return;
    case l3_gen_eq_cond_cmd: L3_eq_conditional(c); return;
    case l3_check_cmd: L3_check_cmd(c); return;
    case l3_generate_variant_cmd: l3_generate_variant(); return;
    case l3_set_cat_cmd: L3_set_cat_code(c); return;
    case l3_set_num_cmd: L3_set_num_code(c); return;
    case tl_basic_cmd: l3_new_token_list(c); return;
    case tl_concat_cmd: l3_tl_concat(c); return;
    case tl_put_left_cmd: l3_tl_put_left(c); return;
    case tl_set_cmd: l3_tl_set(c); return;
    case l3_rescan_cmd: tl_set_rescan(c); return;
    case toc_cmd: { // insert <tableofcontents/>
        std::string np = "tableofcontents";
        if (c == 1) np = "listoftables";
        if (c == 2) np = "listoffigures";
        remove_initial_star();
        leave_h_mode();
        the_stack.push1(the_names[np]);
        if (c == 0) {
            static bool inserted = false;
            if (!inserted) the_stack.top_stack()->id = 4;
            inserted = true;
            auto k   = eqtb_int_table[42 + count_reg_offset].val;
            the_stack.add_att_to_cur(std::string("depth"), std::string(std::to_string(k)));
        }
        the_stack.pop(the_names[np]);
        return;
    }
    case begin_cmd:
    case end_cmd: T_beginend(x); return;
    case index_cmd: T_index(c); return;
    case document_cmd: T_begindocument(); return;
    case end_document_cmd: T_enddocument(c); return;
    case keywords_cmd: T_keywords(); return;
    case end_keywords_cmd: the_stack.pop(the_names["keywords"]); return;
    case center_cmd:
        leave_h_mode();     // finish the possibly not-centered paragraph
        the_stack.add_nl(); // needed ?
        word_define(incentering_code, c, false);
        return;
    case end_center_cmd:
        leave_h_mode(); // finish centered paragraph
        return;
    case float_cmd: T_float(c); return;
    case figure_cmd:
    case table_cmd: T_figure_table(x, c); return;
    case end_figure_cmd: T_figure_table_end(true); return;
    case end_table_cmd: T_figure_table_end(false); return;
    case thebibliography_cmd: T_start_the_biblio(); return;
    case end_thebibliography_cmd: T_end_the_biblio(); return;
    case solvecite_cmd: solve_cite(false); return;
    case cite_one_cmd: T_cite_one(); return;
    case ipa_cmd: T_ipa(c); return;
    case footcitepre_cmd: unprocessed_xml.push_back_unless_punct(' '); return;
    case ding_cmd: T_ding(); return;
    case etex_cmd: T_etex(); return;
    case rule_cmd: scan_rule(c); return;
    case file_cmd: T_input(c); return;
    case unimp_font_cmd: T_unimplemented_font(c); return;
    case unimp_cmd: T_unimp(c); return;
    case specimp_cmd: T_specimp(c); return;
    case thickness_cmd: T_linethickness(c); return;
    case thm_aux_cmd: {
        TokenList L = read_arg();
        token_list_define(c, L, false);
    }
        return;
    case start_thm_cmd:
        if (c == 2)
            T_end_theorem();
        else
            T_start_theorem(c);
        return;
    case atdocument_cmd: T_atdocument(c); return;
    case glossaire_cmd: T_glossaire(); return;
    case end_glossaire_cmd: T_glossaire_end(); return;
    case list_cmd:
    case itemize_cmd:
    case enumerate_cmd:
    case description_cmd: T_listenv(x); return;
    case end_list_cmd:
    case end_itemize_cmd:
    case end_enumerate_cmd:
    case end_description_cmd: T_listenv_end(); return;
    case xmlelement_env_cmd: T_xmlenv(c); return;
    case end_xmlelement_env_cmd: T_xmlenv_end(c); return;
    case filecontents_env_cmd: T_filecontents(c); return;
    case addatt_cmd: T_xmladdatt(c); return;
    case ignore_env_cmd: return;
    case ignore_content_cmd: T_raw_env(false); return;
    case raw_env_cmd: the_stack.add_last(new Xml(std::string(T_raw_env(true)))); return;
    case math_env_cmd:
        cur_tok.kill();
        pop_level(bt_env); // IS THIS OK ?
        T_math(c);
        return;
    case popmodule_cmd: the_stack.end_module(); return;
    case pushmodule_cmd: push_module(); return;
    case tabular_env_cmd: T_start_tabular(c); return;
    case end_tabular_env_cmd: T_end_tabular(c); return;
    case verbatim_env_cmd: T_verbatim(); return;
    case picture_env_cmd: T_picture(); return;
    case end_picture_env_cmd: the_stack.pop(the_names["picture"]); return;
    case end_ignore_env_cmd: return;
    case subequations_cmd: T_subequations(true); return;
    case end_subequations_cmd: T_subequations(false); return;
    case minipage_cmd: T_minipage(); return;
    case end_minipage_cmd:
        flush_buffer();
        the_stack.pop_if_frame(the_names["cst_p"]);
        the_stack.pop_if_frame(the_names["item"]);
        the_stack.pop(the_names["minipage"]);
        return;
    case end_ignore_content_cmd:
    case end_raw_env_cmd:
    case end_math_env_cmd: parse_error(cur_tok, "missing \\begin environment ", cur_tok.tok_to_str(), "missing begin"); return;
    case eqno_cmd:

    case mathordb_cmd:
    case mathord_cmd:
    case mathbin_cmd:
    case mathrel_cmd:
    case mathbetween_cmd:
    case mathopen_cmd:
    case mathclose_cmd:
    case mathop_cmd:
    case mathopn_cmd:
    case mathspace_cmd:
    case mathfont_cmd:
    case math_font_cmd:
    case math_list_cmd:
    case math_xml_cmd:
    case left_cmd:
    case right_cmd:
    case tag_cmd: math_only(); return;
    case mathinner_cmd:
        if (math_loc(c) == vdots_code) {
            back_input(hash_table.dollar_token);
            back_input(cur_tok);
            back_input(hash_table.dollar_token);
            return;
        }
        math_only();
        return;
    case endv_cmd: T_endv(); return;
    case cr_cmd: T_cr(); return;
    case cons_cmd: M_cons(); return;
    case self_insert_cmd:
        LC();
        unprocessed_xml.push_back(cur_tok);
        return;
    case check_date_cmd: date_commands(c); return;
    case typein_cmd: T_typein(); return;
    case testopt_cmd: T_testopt(); return;
    case selective_sanitize_cmd: selective_sanitize(); return;
    case latex_error_cmd: T_class_error(c); return;
    case change_element_name_cmd: T_change_element_name(); return;
    case formatdate_cmd: formatdate(); return;
    case numberwithin_cmd: numberwithin(); return;
    case dblarg_cmd: dbl_arg(); return;
    case ifdefinable_cmd: T_ifdefinable(); return;
    case color_cmd: T_color(c); return;
    case kvo_family_cmd: kvo_family(c); return;
    default: undefined_mac();
    }
}
