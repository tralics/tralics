#include "tralics/Dispatcher.h"

void Dispatcher::register_action(symcodes x, parser_fn f) {
    emplace(x, [f](symcodes /*unused*/, subtypes /*unused*/) { return std::invoke(f, the_parser); });
}

void Dispatcher::register_action(symcodes x, parser_fn_void f) {
    emplace(x, [f](symcodes /*unused*/, subtypes /*unused*/) {
        std::invoke(f, the_parser);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x f) {
    emplace(x, [f](symcodes x, subtypes /*unused*/) { return std::invoke(f, the_parser, x); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x_void f) {
    emplace(x, [f](symcodes x, subtypes /*unused*/) {
        std::invoke(f, the_parser, x);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c f) {
    emplace(x, [f](symcodes /*unused*/, subtypes c) { return std::invoke(f, the_parser, c); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c_void f) {
    emplace(x, [f](symcodes /*unused*/, subtypes c) {
        std::invoke(f, the_parser, c);
        return true;
    });
}

Dispatcher::Dispatcher() {
    register_action(cst2_cmd, &Parser::T_cst2);
    register_action(begin_cmd, &Parser::T_beginend);
    register_action(description_cmd, &Parser::T_listenv);
    register_action(end_cmd, &Parser::T_beginend);
    register_action(enumerate_cmd, &Parser::T_listenv);
    register_action(itemize_cmd, &Parser::T_listenv);
    register_action(list_cmd, &Parser::T_listenv);
    register_action(leave_v_mode_cmd, &Parser::leave_v_mode);
    register_action(scan_glue_cmd, &Parser::T_scan_glue);
    register_action(trees_cmd, &Parser::T_trees);
    register_action(matter_cmd, &Parser::T_matter);
    register_action(noargfont_cmd, &Parser::see_font_change);
    register_action(argfont_cmd, &Parser::arg_font);
    register_action(bib_cmd, &Parser::T_bauteursediteurs);
    register_action(line_cmd, &Parser::T_line);
    register_action(xkeyval_cmd, &Parser::T_xkeyval);
    register_action(XML_swap_cmd, &Parser::user_XML_swap);
    register_action(XML_modify_cmd, &Parser::user_XML_modify);
    register_action(un_box_cmd, &Parser::T_un_box);
    register_action(extension_cmd, &Parser::M_extension);
    register_action(xray_cmd, &Parser::M_xray);
    register_action(for_cmd, &Parser::T_xkv_for);
    register_action(newcount_cmd, &Parser::new_constant);
    register_action(newboolean_cmd, &Parser::M_newboolean);
    register_action(shortverb_cmd, &Parser::M_shortverb);
    register_action(fp_cmd, &Parser::exec_fp_cmd);
    register_action(fpif_cmd, &Parser::exec_fp_cmd);
    register_action(fpi_cmd, &Parser::exec_fpi_cmd);
    register_action(start_par_cmd, &Parser::implicit_par);
    register_action(cite_cmd, &Parser::T_cite);
    register_action(item_cmd, &Parser::T_item);
    register_action(section_cmd, &Parser::T_paras);
    register_action(hspace_cmd, &Parser::T_hspace);
    register_action(box_cmd, &Parser::T_mbox);
    register_action(includegraphics_cmd, &Parser::includegraphics);
    register_action(case_shift_cmd, &Parser::T_case_shift);
    register_action(hanl_cmd, &Parser::T_hanl);
    register_action(xmlelt_cmd, &Parser::T_xmlelt);
    register_action(put_cmd, &Parser::T_put);
    register_action(curves_cmd, &Parser::T_curves);
    register_action(dashline_cmd, &Parser::T_dashline);
    register_action(bezier_cmd, &Parser::T_bezier);
    register_action(l3_gen_from_ac_cmd, &Parser::Tl3_gen_from_ac);
    register_action(l3_gen_cond_Npnn_cmd, &Parser::L3_new_conditional_parm);
    register_action(l3_gen_cond_Nnn_cmd, &Parser::L3_new_conditional);
    register_action(l3_gen_eq_cond_cmd, &Parser::L3_eq_conditional);
    register_action(l3_check_cmd, &Parser::L3_check_cmd);
    register_action(l3_set_cat_cmd, &Parser::L3_set_cat_code);
    register_action(l3_set_num_cmd, &Parser::L3_set_num_code);
    register_action(tl_basic_cmd, &Parser::l3_new_token_list);
    register_action(tl_concat_cmd, &Parser::l3_tl_concat);
    register_action(tl_put_left_cmd, &Parser::l3_tl_put_left);
    register_action(tl_set_cmd, &Parser::l3_tl_set);
    register_action(l3_rescan_cmd, &Parser::tl_set_rescan);
    register_action(index_cmd, &Parser::T_index);
    register_action(end_document_cmd, &Parser::T_enddocument);
    register_action(float_cmd, &Parser::T_float);
    register_action(ipa_cmd, &Parser::T_ipa);
    register_action(rule_cmd, &Parser::scan_rule);
    register_action(file_cmd, &Parser::T_input);
    register_action(unimp_font_cmd, &Parser::T_unimplemented_font);
    register_action(unimp_cmd, &Parser::T_unimp);
    register_action(specimp_cmd, &Parser::T_specimp);
    register_action(thickness_cmd, &Parser::T_linethickness);
    register_action(atdocument_cmd, &Parser::T_atdocument);
    register_action(xmlelement_env_cmd, &Parser::T_xmlenv);
    register_action(end_xmlelement_env_cmd, &Parser::T_xmlenv_end);
    register_action(filecontents_env_cmd, &Parser::T_filecontents);
    register_action(addatt_cmd, &Parser::T_xmladdatt);
    register_action(tabular_env_cmd, &Parser::T_start_tabular);
    register_action(end_tabular_env_cmd, &Parser::T_end_tabular);
    register_action(check_date_cmd, &Parser::date_commands);
    register_action(latex_error_cmd, &Parser::T_class_error);
    register_action(color_cmd, &Parser::T_color);
    register_action(kvo_family_cmd, &Parser::kvo_family);
    register_action(fvset_cmd, &Parser::special_fvset);
    register_action(biblio_cmd, &Parser::T_biblio);
    register_action(omitcite_cmd, &Parser::T_omitcite);
    register_action(bibliographystyle_cmd, &Parser::T_bibliostyle);
    register_action(calc_cmd, &Parser::exec_calc);
    register_action(alignment_catcode, &Parser::T_ampersand);
    register_action(oldfont_cmd, &Parser::old_font);
    register_action(fontsize_cmd, &Parser::translate_font_size);
    register_action(fnhack_cmd, &Parser::fnhack);
    register_action(selectfont_cmd, &Parser::font_has_changed);
    register_action(usefont_cmd, &Parser::T_usefont);
    register_action(isin_cmd, &Parser::T_isin);
    register_action(bpers_cmd, &Parser::T_bpers);
    register_action(cititem_cmd, &Parser::T_cititem);
    register_action(nolinebreak_cmd, &Parser::ignore_optarg);
    register_action(defineverbatimenv_cmd, &Parser::T_define_verbatim_env);
    register_action(saveverb_cmd, &Parser::T_saveverb);
    register_action(XML_fetch_cmd, &Parser::user_XML_fetch);
    register_action(tracingall_cmd, &Parser::M_tracingall);
    register_action(ifstar_cmd, &Parser::T_ifstar);
    register_action(newif_cmd, &Parser::M_newif);
    register_action(setboolean_cmd, &Parser::set_boolean);
    register_action(ifthenelse_cmd, &Parser::T_ifthenelse);
    register_action(whiledo_cmd, &Parser::T_whiledo);
    register_action(setmode_cmd, &Parser::T_setmode);
    register_action(toks_register_cmd, &Parser::M_prefixed);
    register_action(assign_toks_cmd, &Parser::M_prefixed);
    register_action(assign_int_cmd, &Parser::M_prefixed);
    register_action(assign_dimen_cmd, &Parser::M_prefixed);
    register_action(assign_glue_cmd, &Parser::M_prefixed);
    register_action(assign_mu_glue_cmd, &Parser::M_prefixed);
    register_action(assign_font_dimen_cmd, &Parser::M_prefixed);
    register_action(assign_font_int_cmd, &Parser::M_prefixed);
    register_action(assign_enc_char_cmd, &Parser::M_prefixed);
    register_action(set_aux_cmd, &Parser::M_prefixed);
    register_action(set_prev_graf_cmd, &Parser::M_prefixed);
    register_action(set_page_dimen_cmd, &Parser::M_prefixed);
    register_action(set_page_int_cmd, &Parser::M_prefixed);
    register_action(set_box_dimen_cmd, &Parser::M_prefixed);
    register_action(set_shape_cmd, &Parser::M_prefixed);
    register_action(def_code_cmd, &Parser::M_prefixed);
    register_action(def_family_cmd, &Parser::M_prefixed);
    register_action(set_font_cmd, &Parser::M_prefixed);
    register_action(set_mathprop_cmd, &Parser::M_prefixed);
    register_action(set_mathchar_cmd, &Parser::M_prefixed);
    register_action(def_font_cmd, &Parser::M_prefixed);
    register_action(register_cmd, &Parser::M_prefixed);
    register_action(advance_cmd, &Parser::M_prefixed);
    register_action(multiply_cmd, &Parser::M_prefixed);
    register_action(divide_cmd, &Parser::M_prefixed);
    register_action(prefix_cmd, &Parser::M_prefixed);
    register_action(let_cmd, &Parser::M_prefixed);
    register_action(shorthand_def_cmd, &Parser::M_prefixed);
    register_action(read_to_cs_cmd, &Parser::M_prefixed);
    register_action(def_cmd, &Parser::M_prefixed);
    register_action(set_box_cmd, &Parser::M_prefixed);
    register_action(set_interaction_cmd, &Parser::M_prefixed);
    register_action(usecounter_cmd, &Parser::T_use_counter);
    register_action(aftergroup_cmd, &Parser::T_aftergroup);
    register_action(ignorep_cmd, &Parser::T_par1);
    register_action(par_cmd, &Parser::T_par1);
    register_action(inputclass_cmd, &Parser::T_inputclass);
    register_action(expandtwoargs_cmd, &Parser::expand_twoargs);
    register_action(removeelement_cmd, &Parser::T_remove_element);
    register_action(declare_options_cmd, &Parser::T_declare_options);
    register_action(options_not_used_cmd, &Parser::T_option_not_used);
    register_action(process_options_cmd, &Parser::T_process_options);
    register_action(at_end_of_class_cmd, &Parser::T_at_end_of_class);
    register_action(execute_options_cmd, &Parser::T_execute_options);
    register_action(subfigure_cmd, &Parser::T_subfigure);
    register_action(pop_stack_cmd, &Parser::pop_all_levels);
    register_action(fancy_cmd, &Parser::T_fancy);
    register_action(xfancy_cmd, &Parser::T_xfancy);
    register_action(linebreak_cmd, &Parser::ignore_optarg);
    register_action(url_cmd, &Parser::T_url);
    register_action(error_cmd, &Parser::T_error);
    register_action(glo_cmd, &Parser::T_glo);
    register_action(reevaluate_cmd, &Parser::T_reevaluate);
    register_action(newcolumntype_cmd, &Parser::T_newcolumn_type);
    register_action(epsfbox_cmd, &Parser::T_epsfbox);
    register_action(grabenv_cmd, &Parser::T_grabenv);
    register_action(l3_gen_from_sig_cmd, &Parser::generate_from_sig);
    register_action(GetIdInfo_cmd, &Parser::L3_getid);
    register_action(GetIdInfoLog_cmd, &Parser::L3_logid);
    register_action(l3_generate_variant_cmd, &Parser::l3_generate_variant);
    register_action(document_cmd, &Parser::T_begindocument);
    register_action(keywords_cmd, &Parser::T_keywords);
    register_action(thebibliography_cmd, &Parser::T_start_the_biblio);
    register_action(end_thebibliography_cmd, &Parser::T_end_the_biblio);
    register_action(cite_one_cmd, &Parser::T_cite_one);
    register_action(ding_cmd, &Parser::T_ding);
    register_action(etex_cmd, &Parser::T_etex);
    register_action(glossaire_cmd, &Parser::T_glossaire);
    register_action(end_glossaire_cmd, &Parser::T_glossaire_end);
    register_action(end_list_cmd, &Parser::T_listenv_end);
    register_action(end_itemize_cmd, &Parser::T_listenv_end);
    register_action(end_enumerate_cmd, &Parser::T_listenv_end);
    register_action(end_description_cmd, &Parser::T_listenv_end);
    register_action(pushmodule_cmd, &Parser::push_module);
    register_action(verbatim_env_cmd, &Parser::T_verbatim);
    register_action(picture_env_cmd, &Parser::T_picture);
    register_action(minipage_cmd, &Parser::T_minipage);
    register_action(eqno_cmd, &Parser::math_only);
    register_action(mathordb_cmd, &Parser::math_only);
    register_action(mathord_cmd, &Parser::math_only);
    register_action(mathbin_cmd, &Parser::math_only);
    register_action(mathrel_cmd, &Parser::math_only);
    register_action(mathbetween_cmd, &Parser::math_only);
    register_action(mathopen_cmd, &Parser::math_only);
    register_action(mathclose_cmd, &Parser::math_only);
    register_action(mathop_cmd, &Parser::math_only);
    register_action(mathopn_cmd, &Parser::math_only);
    register_action(mathspace_cmd, &Parser::math_only);
    register_action(mathfont_cmd, &Parser::math_only);
    register_action(math_font_cmd, &Parser::math_only);
    register_action(math_list_cmd, &Parser::math_only);
    register_action(math_xml_cmd, &Parser::math_only);
    register_action(left_cmd, &Parser::math_only);
    register_action(right_cmd, &Parser::math_only);
    register_action(tag_cmd, &Parser::math_only);
    register_action(endv_cmd, &Parser::T_endv);
    register_action(cr_cmd, &Parser::T_cr);
    register_action(cons_cmd, &Parser::M_cons);
    register_action(typein_cmd, &Parser::T_typein);
    register_action(testopt_cmd, &Parser::T_testopt);
    register_action(selective_sanitize_cmd, &Parser::selective_sanitize);
    register_action(change_element_name_cmd, &Parser::T_change_element_name);
    register_action(formatdate_cmd, &Parser::formatdate);
    register_action(numberwithin_cmd, &Parser::numberwithin);
    register_action(dblarg_cmd, &Parser::dbl_arg);
    register_action(ifdefinable_cmd, &Parser::T_ifdefinable);
}
