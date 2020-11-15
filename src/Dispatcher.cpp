#include "tralics/Dispatcher.h"

void Dispatcher::register_action(symcodes x, parser_fn f) {
    emplace(x, [f](symcodes, subtypes) { return std::invoke(f, the_parser); });
}

void Dispatcher::register_action(symcodes x, parser_fn_void f) {
    emplace(x, [f](symcodes, subtypes) {
        std::invoke(f, the_parser);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x f) {
    emplace(x, [f](symcodes x, subtypes) { return std::invoke(f, the_parser, x); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x_void f) {
    emplace(x, [f](symcodes x, subtypes) {
        std::invoke(f, the_parser, x);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c f) {
    emplace(x, [f](symcodes, subtypes c) { return std::invoke(f, the_parser, c); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c_void f) {
    emplace(x, [f](symcodes, subtypes c) {
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
}
