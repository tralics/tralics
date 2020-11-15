#include "tralics/Dispatcher.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"
#include "tralics/types.h"

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

// \todo make a hash table of methods instead of this huge mess, actions below is a proof of concept.

[[nodiscard]] bool Parser::translate03() {
    auto guard  = SaveErrTok(cur_tok);
    auto [x, c] = cur_cmd_chr;

    if (x == underscore_catcode && global_in_load) {
        translate_char(cur_cmd_chr);
        return true;
    }

    if (auto it = actions.find(x); it != actions.end()) return it->second(x, c);

    switch (x) {
    case cst1_cmd:
    case cst_cmd:
        LC();
        T_cst1(c);
        return true;
    case nobreakspace_cmd:
        LC();
        if (global_in_url)
            process_char('~');
        else
            process_char(0xA0);
        return true;
    case space_catcode:
        if (the_stack.in_v_mode() || the_stack.in_no_mode() || the_stack.in_bib_mode()) return true;
        process_char(char32_t(char32_t(c)));
        return true;
    case letter_catcode:
    case other_catcode: translate_char(cur_cmd_chr); return true;
    case char_num_cmd: extended_chars(scan_27bit_int()); return true;
    case char_given_cmd: extended_chars(c); return true;
    case fvset_cmd: special_fvset(); return true;
    case biblio_cmd: T_biblio(); return true;
    case omitcite_cmd: T_omitcite(); return true;
    case bibliographystyle_cmd: T_bibliostyle(); return true;
    case insertbibliohere_cmd: add_bib_marker(true); return true;
    case inhibit_xml_cmd:
        the_main.no_xml = true;
        log_and_tty << "\nsyntaxonly: no XML file will be produced\n";
        return true;
    case endcsname_cmd: parse_error("Extra \\endcsname"); return true;
    case xmllatex_cmd:
        LC();
        unprocessed_xml += T_xmllatex();
        return true;
    case scan_glue_cmd: T_scan_glue(c); return true;
    case kern_cmd:
        scan_dimen(c == 1, cur_tok);
        // so what ? we could append hspace or vspace here.
        return true;
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
        return true;
    case calc_cmd: exec_calc(); return true;
    case dollar_catcode:
        flush_buffer();
        T_math(nomathenv_code);
        return true;
    case alignment_catcode: T_ampersand(); return true;
    case open_catcode: push_level(bt_brace); return true;
    case close_catcode: pop_level(bt_brace); return true;
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
        return true;
    case hat_catcode:
    case underscore_catcode:
        if (global_in_load || is_pos_par(nomath_code))
            translate_char(CmdChr(letter_catcode, c));
        else
            parse_error(cur_tok, "Missing dollar not inserted, token ignored: ", cur_tok.tok_to_str(), "Missing dollar");
        return true;
    case backslash_cmd:
        if (c == 0)
            T_backslash();
        else
            T_newline();
        return true;
    case hline_cmd: // cline, hline, hlinee
        T_hline(c);
        return true;
    case skip_cmd: {
        int w = c == smallskip_code ? 3 : c == medskip_code ? 6 : 12;
        append_glue(cur_tok, w << 16, true);
        return true;
    }
    case hfill_cmd:
        leave_v_mode();
        the_stack.add_newid0(hfill_to_np(c));
        return true;
    case vfill_cmd:
        leave_h_mode();
        the_stack.add_newid0(vfill_to_np(c));
        return true;
    case sub_cmd:
    case soul_cmd: {
        static const std::string list[] = {"sup", "sub", "oldstyle", "caps", "hl", "so", "st", "ul"}; // \todo somewhere else
        T_fonts(list[c]);
        return true;
    }
    case trees_cmd: T_trees(c); return true;
    case matter_cmd: T_matter(c); return true;
    case arg_font_cmd: T_fonts("font_sc"); return true;
    case special_math_cmd:
        if (c == overline_code || c == underline_code)
            T_fonts(c == overline_code ? "overline" : "underline");
        else
            math_only();
        return true;
    case noargfont_cmd: see_font_change(c); return true;
    case argfont_cmd: arg_font(c); return true;
    case oldfont_cmd: old_font(); return true;
    case fontsize_cmd: translate_font_size(); return true;
    case fnhack_cmd: fnhack(); return true;
    case footnote_cmd: T_cap_or_note(false); return true;
    case selectfont_cmd: font_has_changed(); return true;
    case ltfont_cmd:
        flush_buffer();
        cur_font.ltfont(sT_arg_nopar(), c);
        return true;
    case usefont_cmd: T_usefont(); return true;
    case isin_cmd: T_isin(); return true;
    case citation_cmd:
        // should be mode independent...
        T_citation();
        the_stack.add_nl();
        return true;
    case bib_cmd: T_bauteursediteurs(c); return true;
    case line_cmd: T_line(c); return true;
    case bpers_cmd: T_bpers(); return true;
    case cititem_cmd: T_cititem(); return true;
    case bibitem_cmd: c == 1 ? T_empty_bibitem() : T_bibitem(); return true;
    case end_citation_cmd: the_stack.pop(the_names["citation"]); return true;
    case ignoreA_cmd:
        T_ignoreA();
        return true;
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
        return true;
    case relax_cmd:
    case eof_marker_cmd: return true;
    case nolinebreak_cmd: ignore_optarg(); return true;
    case ignore_one_argument_cmd:
        if (c == patterns_code || c == hyphenation_code || c == special_code) scan_left_brace_and_back_input();
        ignore_arg();
        return true;
    case ignore_two_argument_cmd:
        ignore_arg();
        ignore_arg();
        return true;
    case defineverbatimenv_cmd: T_define_verbatim_env(); return true;
    case saveverb_cmd: T_saveverb(); return true;
    case xkeyval_cmd: T_xkeyval(c); return true;
    case add_to_macro_cmd: T_addtomacro(c == 1); return true;
    case makeatletter_cmd: word_define('@', letter_catcode, false); return true;
    case makeatother_cmd: word_define('@', other_catcode, false); return true;
    case numberedverbatim_cmd: numbered_verbatim = true; return true;
    case unnumberedverbatim_cmd: numbered_verbatim = false; return true;
    case after_assignment_cmd:
        get_token();
        set_after_ass_tok(cur_tok);
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{\\afterassignment: " << cur_tok << "}\n";
        }
        return true;
    case last_item_cmd: parse_error(cur_tok, "Read only variable ", cur_tok, "", "readonly"); return true;
    case XML_swap_cmd: user_XML_swap(c); return true;
    case XML_fetch_cmd: user_XML_fetch(); return true;
    case XML_modify_cmd: user_XML_modify(c); return true;
    case un_box_cmd: T_un_box(c); return true;
    case extension_cmd: M_extension(c); return true;
    case setlanguage_cmd: //  strange...
        scan_int(cur_tok);
        return true;
    case xray_cmd: M_xray(c); return true;
    case move_cmd:
        scan_dimen(false, cur_tok); // ignore dimension
        scan_box(move_location);    // read a box and insert the value
        return true;
    case save_box_cmd: T_save_box(c == 0); return true;
    case make_box_cmd: begin_box(makebox_location, c); return true;
    case leader_ship_cmd:
        scan_box(c == shipout_code    ? shipout_location
                 : c == leaders_code  ? leaders_location
                 : c == cleaders_code ? cleaders_location
                                      : xleaders_location);
        return true;
    case tracingall_cmd: M_tracingall(); return true;
    case ifstar_cmd: T_ifstar(); return true;
    case vglue_cmd:
        if (c == 0)
            T_par1();
        else
            leave_v_mode();
        T_scan_glue(c == 0 ? vskip_code : hskip_code);
        return true;
    case for_cmd: T_xkv_for(c); return true;
    case ifnextchar_cmd: T_ifnextchar(c == 0); return true;
    case newif_cmd: M_newif(); return true;
    case newcount_cmd: new_constant(c); return true;
    case newboolean_cmd: M_newboolean(c); return true;
    case setboolean_cmd: set_boolean(); return true;
    case ifthenelse_cmd: T_ifthenelse(); return true;
    case whiledo_cmd: T_whiledo(); return true;
    case setmode_cmd: T_setmode(); return true;
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
    case set_interaction_cmd: M_prefixed(); return true;
    case shortverb_cmd: M_shortverb(c); return true;
    case usecounter_cmd: T_use_counter(); return true;
    case newcounter_cmd: M_counter(true); return true;
    case fp_cmd:
    case fpif_cmd: exec_fp_cmd(c); return true;
    case fpi_cmd: exec_fpi_cmd(c); return true;
    case aftergroup_cmd: T_aftergroup(); return true;
    case listfiles_cmd: list_files_p = true; return true;
    case ignorep_cmd:
    case par_cmd: T_par1(); return true;
    case start_par_cmd: implicit_par(c); return true;
    case caption_cmd: T_cap_or_note(true); return true;
    case cite_cmd: T_cite(c); return true;
    case item_cmd: T_item(c); return true;
    case doc_class_cmd: T_documentclass(!the_stack.in_v_mode() || seen_document); return true;
    case titlepage_cmd:
        if (!the_stack.in_v_mode()) wrong_mode("Bad titlepage command");
        T_titlepage(c);
        return true;
    case package_cmd:
        if (!the_stack.in_v_mode() || seen_document) wrong_mode("Bad \\usepackage command");
        T_usepackage();
        return true;
    case if_package_loaded_cmd: T_if_package_loaded(c == 0); return true;
    case if_package_later_cmd: T_if_package_later(c == 0); return true;
    case if_package_with_cmd: T_if_package_with(c == 0); return true;
    case provides_package_cmd: T_provides_package(c == 0); return true;
    case load_with_options_cmd: T_load_with_options(c == 0); return true;
    case inputclass_cmd: T_inputclass(); return true;
    case expandtwoargs_cmd: expand_twoargs(); return true;
    case removeelement_cmd: T_remove_element(); return true;
    case pass_options_cmd: T_pass_options(c == 0); return true;
    case declare_options_cmd: T_declare_options(); return true;
    case options_not_used_cmd: T_option_not_used(); return true;
    case process_options_cmd: T_process_options(); return true;
    case at_end_of_class_cmd: T_at_end_of_class(); return true;
    case execute_options_cmd: T_execute_options(); return true;
    case needs_format_cmd:
        ignore_arg();
        ignore_optarg();
        return true;
    case subfigure_cmd: T_subfigure(); return true;
    case pop_stack_cmd: pop_all_levels(); return true;
    case section_cmd: T_paras(c); return true;
    case label_cmd:
        flush_buffer();
        T_label(c);
        return true;
    case ref_cmd:
        leave_v_mode();
        T_ref(c == 0);
        return true;
    case hspace_cmd: T_hspace(c); return true;
    case eqref_cmd: // Case \XMLref
        Xid(read_elt_id(cur_tok)).add_ref(sT_arg_nopar());
        return true;
    case box_cmd: T_mbox(c); return true;
    case centering_cmd:
        word_define(incentering_code, c, false);
        if (c != 0U) the_stack.add_center_to_p();
        return true;
    case fbox_cmd:
        if (c == dashbox_code)
            T_fbox_dash_box();
        else if (c == rotatebox_code)
            T_fbox_rotate_box();
        else
            T_fbox(c);
        return true;
    case includegraphics_cmd: includegraphics(c); return true;
    case fancy_cmd: T_fancy(); return true;
    case xfancy_cmd: T_xfancy(); return true;
    case xthepage_cmd:
        flush_buffer();
        the_stack.add_last(the_page_xml);
        return true;
    case case_shift_cmd: T_case_shift(c); return true;
    case linebreak_cmd: ignore_optarg(); return true;
    case url_cmd: T_url(); return true;
    case hanl_cmd: T_hanl(c); return true;
    case error_cmd: T_error(); return true;
    case glo_cmd: T_glo(); return true;
    case reevaluate_cmd: T_reevaluate(); return true;
    case xmlelt_cmd: T_xmlelt(c); return true;
    case newcolumntype_cmd: T_newcolumn_type(); return true;
    case epsfbox_cmd: T_epsfbox(); return true;
    case put_cmd: T_put(c); return true;
    case curves_cmd: T_curves(c); return true;
    case dashline_cmd: T_dashline(c); return true;
    case bezier_cmd: T_bezier(c); return true;
    case grabenv_cmd: T_grabenv(); return true;
    case verb_cmd: T_verb(c != 0U ? verb_saved_char : char32_t(0U)); return true;
    case gloss_cmd: T_gloss(c == 0); return true;
    case only_preamble_cmd:
        get_r_token(true);
        onlypreamble.push_back(hash_table.let_token);
        onlypreamble.push_back(cur_tok);
        onlypreamble.push_back(hash_table.notprerr_token);
        return true;
    case l3_gen_from_sig_cmd: generate_from_sig(); return true;
    case l3_gen_from_ac_cmd: Tl3_gen_from_ac(c); return true;
    case loadlatex3_cmd: L3_load(false); return true;
    case GetIdInfo_cmd: L3_getid(); return true;
    case GetIdInfoLog_cmd: L3_logid(); return true;
    case l3_gen_cond_Npnn_cmd: L3_new_conditional_parm(c); return true;
    case l3_gen_cond_Nnn_cmd: L3_new_conditional(c); return true;
    case l3_gen_eq_cond_cmd: L3_eq_conditional(c); return true;
    case l3_check_cmd: L3_check_cmd(c); return true;
    case l3_generate_variant_cmd: l3_generate_variant(); return true;
    case l3_set_cat_cmd: L3_set_cat_code(c); return true;
    case l3_set_num_cmd: L3_set_num_code(c); return true;
    case tl_basic_cmd: l3_new_token_list(c); return true;
    case tl_concat_cmd: l3_tl_concat(c); return true;
    case tl_put_left_cmd: l3_tl_put_left(c); return true;
    case tl_set_cmd: l3_tl_set(c); return true;
    case l3_rescan_cmd: tl_set_rescan(c); return true;
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
        return true;
    }
    case index_cmd: T_index(c); return true;
    case document_cmd: T_begindocument(); return true;
    case end_document_cmd: T_enddocument(c); return true;
    case keywords_cmd: T_keywords(); return true;
    case end_keywords_cmd: the_stack.pop(the_names["keywords"]); return true;
    case center_cmd:
        leave_h_mode();     // finish the possibly not-centered paragraph
        the_stack.add_nl(); // needed ?
        word_define(incentering_code, c, false);
        return true;
    case end_center_cmd:
        leave_h_mode(); // finish centered paragraph
        return true;
    case float_cmd: T_float(c); return true;
    case figure_cmd:
    case table_cmd: T_figure_table(x, c); return true;
    case end_figure_cmd: T_figure_table_end(true); return true;
    case end_table_cmd: T_figure_table_end(false); return true;
    case thebibliography_cmd: T_start_the_biblio(); return true;
    case end_thebibliography_cmd: T_end_the_biblio(); return true;
    case solvecite_cmd: solve_cite(false); return true;
    case cite_one_cmd: T_cite_one(); return true;
    case ipa_cmd: T_ipa(c); return true;
    case footcitepre_cmd: unprocessed_xml.push_back_unless_punct(' '); return true;
    case ding_cmd: T_ding(); return true;
    case etex_cmd: T_etex(); return true;
    case rule_cmd: scan_rule(c); return true;
    case file_cmd: T_input(c); return true;
    case unimp_font_cmd: T_unimplemented_font(c); return true;
    case unimp_cmd: T_unimp(c); return true;
    case specimp_cmd: T_specimp(c); return true;
    case thickness_cmd: T_linethickness(c); return true;
    case thm_aux_cmd: {
        TokenList L = read_arg();
        token_list_define(c, L, false);
    }
        return true;
    case start_thm_cmd:
        if (c == 2)
            T_end_theorem();
        else
            T_start_theorem(c);
        return true;
    case atdocument_cmd: T_atdocument(c); return true;
    case glossaire_cmd: T_glossaire(); return true;
    case end_glossaire_cmd: T_glossaire_end(); return true;
    case end_list_cmd:
    case end_itemize_cmd:
    case end_enumerate_cmd:
    case end_description_cmd: T_listenv_end(); return true;
    case xmlelement_env_cmd: T_xmlenv(c); return true;
    case end_xmlelement_env_cmd: T_xmlenv_end(c); return true;
    case filecontents_env_cmd: T_filecontents(c); return true;
    case addatt_cmd: T_xmladdatt(c); return true;
    case ignore_env_cmd: return true;
    case ignore_content_cmd: T_raw_env(false); return true;
    case raw_env_cmd: the_stack.add_last(new Xml(std::string(T_raw_env(true)))); return true;
    case math_env_cmd:
        cur_tok.kill();
        pop_level(bt_env); // IS THIS OK ?
        T_math(c);
        return true;
    case popmodule_cmd: the_stack.end_module(); return true;
    case pushmodule_cmd: push_module(); return true;
    case tabular_env_cmd: T_start_tabular(c); return true;
    case end_tabular_env_cmd: T_end_tabular(c); return true;
    case verbatim_env_cmd: T_verbatim(); return true;
    case picture_env_cmd: T_picture(); return true;
    case end_picture_env_cmd: the_stack.pop(the_names["picture"]); return true;
    case end_ignore_env_cmd: return true;
    case subequations_cmd: T_subequations(true); return true;
    case end_subequations_cmd: T_subequations(false); return true;
    case minipage_cmd: T_minipage(); return true;
    case end_minipage_cmd:
        flush_buffer();
        the_stack.pop_if_frame(the_names["cst_p"]);
        the_stack.pop_if_frame(the_names["item"]);
        the_stack.pop(the_names["minipage"]);
        return true;
    case end_ignore_content_cmd:
    case end_raw_env_cmd:
    case end_math_env_cmd: parse_error(cur_tok, "missing \\begin environment ", cur_tok.tok_to_str(), "missing begin"); return true;
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
    case tag_cmd: math_only(); return true;
    case mathinner_cmd:
        if (math_loc(c) == vdots_code) {
            back_input(hash_table.dollar_token);
            back_input(cur_tok);
            back_input(hash_table.dollar_token);
            return true;
        }
        math_only();
        return true;
    case endv_cmd: T_endv(); return true;
    case cr_cmd: T_cr(); return true;
    case cons_cmd: M_cons(); return true;
    case self_insert_cmd:
        LC();
        unprocessed_xml.push_back(cur_tok);
        return true;
    case check_date_cmd: date_commands(c); return true;
    case typein_cmd: T_typein(); return true;
    case testopt_cmd: T_testopt(); return true;
    case selective_sanitize_cmd: selective_sanitize(); return true;
    case latex_error_cmd: T_class_error(c); return true;
    case change_element_name_cmd: T_change_element_name(); return true;
    case formatdate_cmd: formatdate(); return true;
    case numberwithin_cmd: numberwithin(); return true;
    case dblarg_cmd: dbl_arg(); return true;
    case ifdefinable_cmd: T_ifdefinable(); return true;
    case color_cmd: T_color(c); return true;
    case kvo_family_cmd: kvo_family(c); return true;
    default: undefined_mac(); return true;
    }
}
