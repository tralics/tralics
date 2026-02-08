#include "tralics/Dispatcher.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/Symcode.h"
#include "tralics/globals.h"
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

void Dispatcher::register_action(symcodes x, const std::function<bool(subtypes)> &f) { Symcode::get(x).action = f; }

void Dispatcher::register_action(symcodes x, const std::function<bool()> &f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return f(); };
}

void Dispatcher::register_action_plain(symcodes x, const std::function<void()> &f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return f(), true; };
}

void Dispatcher::register_action(symcodes x, const std::function<bool(symcodes)> &f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return f(x); };
}

void Dispatcher::register_action_plain(symcodes x, const std::function<void(symcodes)> &f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return f(x), true; };
}

void Dispatcher::register_action_plain(symcodes x, const std::function<void(subtypes)> &f) {
    Symcode::get(x).action = [f](subtypes c) { return f(c), true; };
}

void Dispatcher::register_action(symcodes x, const std::function<bool(symcodes, subtypes)> &f) {
    Symcode::get(x).action = [=](subtypes c) { return f(x, c); };
}

void Dispatcher::register_action_plain(symcodes x, const std::function<void(symcodes, subtypes)> &f) {
    Symcode::get(x).action = [=](subtypes c) { return f(x, c), true; };
}

void Dispatcher::register_action(symcodes x, parser_fn f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return std::invoke(f, the_parser); };
}

void Dispatcher::register_action_plain(symcodes x, parser_fn_void f) {
    Symcode::get(x).action = [f](subtypes /*unused*/) { return std::invoke(f, the_parser), true; };
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return std::invoke(f, the_parser, x); };
}

void Dispatcher::register_action_plain(symcodes x, parser_fn_with_x_void f) {
    Symcode::get(x).action = [=](subtypes /*unused*/) { return std::invoke(f, the_parser, x), true; };
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c f) {
    Symcode::get(x).action = [f](subtypes c) { return std::invoke(f, the_parser, c); };
}

void Dispatcher::register_action_plain(symcodes x, parser_fn_with_c_void f) {
    Symcode::get(x).action = [f](subtypes c) { return std::invoke(f, the_parser, c), true; };
}

void Dispatcher::register_action(symcodes x, parser_fn_with_xc f) {
    Symcode::get(x).action = [=](subtypes c) { return std::invoke(f, the_parser, x, c); };
}

void Dispatcher::register_action_plain(symcodes x, parser_fn_with_xc_void f) {
    Symcode::get(x).action = [=](subtypes c) { return std::invoke(f, the_parser, x, c), true; };
}

void Dispatcher::register_action(symcodes x, parser_fn_with_cmdchr f) {
    Symcode::get(x).action = [=](subtypes c) { return std::invoke(f, the_parser, CmdChr{x, c}); };
}

void Dispatcher::register_action_plain(symcodes x, parser_fn_with_cmdchr_void f) {
    Symcode::get(x).action = [=](subtypes c) { return std::invoke(f, the_parser, CmdChr{x, c}), true; };
}

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

void Dispatcher::boot() {
    boot_math();

    register_action_plain(add_to_macro_cmd, [](subtypes c) { the_parser.T_addtomacro(c == 1); });
    register_action_plain(addatt_cmd, &Parser::T_xmladdatt);
    register_action_plain(advance_cmd, &Parser::M_prefixed);
    register_action_plain(aftergroup_cmd, &Parser::T_aftergroup);
    register_action_plain(alignment_catcode, &Parser::T_ampersand);
    register_action_plain(arg_font_cmd, [] { the_parser.T_fonts("font_sc"); });
    register_action_plain(argfont_cmd, &Parser::arg_font);
    register_action_plain(assign_dimen_cmd, &Parser::M_prefixed);
    register_action_plain(assign_enc_char_cmd, &Parser::M_prefixed);
    register_action_plain(assign_font_dimen_cmd, &Parser::M_prefixed);
    register_action_plain(assign_font_int_cmd, &Parser::M_prefixed);
    register_action_plain(assign_glue_cmd, &Parser::M_prefixed);
    register_action_plain(assign_int_cmd, &Parser::M_prefixed);
    register_action_plain(assign_mu_glue_cmd, &Parser::M_prefixed);
    register_action_plain(assign_toks_cmd, &Parser::M_prefixed);
    register_action_plain(at_end_of_class_cmd, &Parser::T_at_end_of_class);
    register_action_plain(atdocument_cmd, &Parser::T_atdocument);
    register_action_plain(bezier_cmd, &Parser::T_bezier);
    register_action_plain(bib_cmd, &Parser::T_bauteursediteurs);
    register_action_plain(bibitem_cmd, [](subtypes c) { c == 1 ? the_parser.T_empty_bibitem() : the_parser.T_bibitem(); });
    register_action_plain(biblio_cmd, &Parser::T_biblio);
    register_action_plain(bibliographystyle_cmd, &Parser::T_bibliostyle);
    register_action_plain(box_cmd, &Parser::T_mbox);
    register_action_plain(bpers_cmd, &Parser::T_bpers);
    register_action_plain(calc_cmd, &Parser::exec_calc);
    register_action_plain(caption_cmd, [] { the_parser.T_cap_or_note(true); });
    register_action_plain(case_shift_cmd, &Parser::T_case_shift);
    register_action_plain(change_element_name_cmd, &Parser::T_change_element_name);
    register_action_plain(char_given_cmd, [](subtypes c) { the_parser.extended_chars(size_t(c)); });
    register_action_plain(char_num_cmd, [] { the_parser.extended_chars(the_parser.scan_27bit_int()); });
    register_action_plain(check_date_cmd, &Parser::date_commands);
    register_action_plain(citation_cmd, [] { the_parser.T_citation(), the_stack.add_nl(); });
    register_action_plain(cite_cmd, &Parser::T_cite);
    register_action_plain(cite_one_cmd, &Parser::T_cite_one);
    register_action_plain(cititem_cmd, &Parser::T_cititem);
    register_action_plain(close_catcode, [] {
        if (!the_parser.pop_level(bt_brace)) throw EndOfData();
    });
    register_action_plain(color_cmd, &Parser::T_color);
    register_action_plain(cons_cmd, &Parser::M_cons);
    register_action_plain(cr_cmd, &Parser::T_cr);
    register_action_plain(cst_cmd, &Parser::T_cst1);
    register_action_plain(cst1_cmd, &Parser::T_cst1);
    register_action_plain(cst2_cmd, &Parser::T_cst2);
    register_action_plain(curves_cmd, &Parser::T_curves);
    register_action_plain(dashline_cmd, &Parser::T_dashline);
    register_action_plain(dblarg_cmd, &Parser::dbl_arg);
    register_action_plain(declare_options_cmd, &Parser::T_declare_options);
    register_action_plain(def_cmd, &Parser::M_prefixed);
    register_action_plain(def_code_cmd, &Parser::M_prefixed);
    register_action_plain(def_family_cmd, &Parser::M_prefixed);
    register_action_plain(def_font_cmd, &Parser::M_prefixed);
    register_action_plain(defineverbatimenv_cmd, &Parser::T_define_verbatim_env);
    register_action_plain(description_cmd, &Parser::T_listenv);
    register_action_plain(ding_cmd, &Parser::T_ding);
    register_action_plain(divide_cmd, &Parser::M_prefixed);
    register_action_plain(doc_class_cmd, [] { the_parser.T_documentclass(!the_stack.in_v_mode() || the_parser.seen_document); });
    register_action_plain(document_cmd, &Parser::T_begindocument);
    register_action_plain(dollar_catcode, [] { return the_parser.flush_buffer(), the_parser.T_math(nomathenv_code); });
    register_action_plain(end_center_cmd, [] { the_parser.leave_h_mode(); });
    register_action_plain(end_citation_cmd, [] { the_stack.pop(::the_names["citation"]); });
    register_action_plain(end_description_cmd, &Parser::T_listenv_end);
    register_action_plain(end_document_cmd, &Parser::T_enddocument);
    register_action_plain(end_enumerate_cmd, &Parser::T_listenv_end);
    register_action_plain(end_figure_cmd, [] { the_parser.T_figure_table_end(true); });
    register_action_plain(end_glossaire_cmd, &Parser::T_glossaire_end);
    register_action_plain(end_ignore_env_cmd, [] {});
    register_action_plain(end_itemize_cmd, &Parser::T_listenv_end);
    register_action_plain(end_keywords_cmd, [] { the_stack.pop(::the_names["keywords"]); });
    register_action_plain(end_list_cmd, &Parser::T_listenv_end);
    register_action_plain(end_picture_env_cmd, [] { the_stack.pop(::the_names["picture"]); });
    register_action_plain(end_subequations_cmd, [] { the_parser.T_subequations(false); });
    register_action_plain(end_table_cmd, [] { the_parser.T_figure_table_end(false); });
    register_action_plain(end_tabular_env_cmd, &Parser::T_end_tabular);
    register_action_plain(end_thebibliography_cmd, &Parser::T_end_the_biblio);
    register_action_plain(end_xmlelement_env_cmd, &Parser::T_xmlenv_end);
    register_action_plain(endcsname_cmd, [] { the_parser.parse_error("Extra \\endcsname"); });
    register_action_plain(endv_cmd, [] {
        if (!the_parser.T_endv()) throw EndOfData();
    });
    register_action_plain(enumerate_cmd, &Parser::T_listenv);
    register_action_plain(eof_marker_cmd, [] {});
    register_action_plain(epsfbox_cmd, &Parser::T_epsfbox);
    register_action_plain(eqref_cmd, [] { Xid(the_parser.read_elt_id(the_parser.cur_tok)).add_ref(the_parser.sT_arg_nopar()); });
    register_action_plain(error_cmd, &Parser::T_error);
    register_action_plain(etex_cmd, &Parser::T_etex);
    register_action_plain(execute_options_cmd, &Parser::T_execute_options);
    register_action_plain(expandtwoargs_cmd, &Parser::expand_twoargs);
    register_action_plain(extension_cmd, &Parser::M_extension);
    register_action_plain(fancy_cmd, &Parser::T_fancy);
    register_action_plain(figure_cmd, [](symcodes x, subtypes c) { the_parser.T_figure_table(x, c); });
    register_action_plain(file_cmd, &Parser::T_input);
    register_action_plain(filecontents_env_cmd, &Parser::T_filecontents);
    register_action_plain(float_cmd, &Parser::T_float);
    register_action_plain(fnhack_cmd, &Parser::fnhack);
    register_action_plain(fontsize_cmd, &Parser::translate_font_size);
    register_action_plain(footcitepre_cmd, [] { the_parser.unprocessed_xml.push_back_unless_punct(' '); });
    register_action_plain(footnote_cmd, [] { the_parser.T_cap_or_note(false); });
    register_action_plain(for_cmd, &Parser::T_xkv_for);
    register_action_plain(formatdate_cmd, &Parser::formatdate);
    register_action_plain(fp_cmd, &Parser::exec_fp_cmd);
    register_action_plain(fpi_cmd, &Parser::exec_fpi_cmd);
    register_action_plain(fpif_cmd, &Parser::exec_fp_cmd);
    register_action_plain(fvset_cmd, &Parser::special_fvset);
    register_action_plain(GetIdInfo_cmd, &Parser::L3_getid);
    register_action_plain(GetIdInfoLog_cmd, &Parser::L3_logid);
    register_action_plain(glo_cmd, &Parser::T_glo);
    register_action_plain(gloss_cmd, [](subtypes c) { the_parser.T_gloss(c == 0); });
    register_action_plain(glossaire_cmd, &Parser::T_glossaire);
    register_action_plain(grabenv_cmd, &Parser::T_grabenv);
    register_action_plain(hanl_cmd, &Parser::T_hanl);
    register_action_plain(hfill_cmd, [](subtypes c) { the_parser.leave_v_mode(), the_stack.add_newid0(hfill_to_np(c)); });
    register_action_plain(hline_cmd, [](subtypes c) { the_parser.T_hline(c); });
    register_action_plain(hspace_cmd, &Parser::T_hspace);
    register_action_plain(if_package_later_cmd, [](subtypes c) { the_parser.T_if_package_later(c == 0); });
    register_action_plain(if_package_loaded_cmd, [](subtypes c) { the_parser.T_if_package_loaded(c == 0); });
    register_action_plain(if_package_with_cmd, [](subtypes c) { the_parser.T_if_package_with(c == 0); });
    register_action_plain(ifdefinable_cmd, &Parser::T_ifdefinable);
    register_action_plain(ifnextchar_cmd, [](subtypes c) { the_parser.T_ifnextchar(c == 0); });
    register_action_plain(ifstar_cmd, &Parser::T_ifstar);
    register_action_plain(ifthenelse_cmd, &Parser::T_ifthenelse);
    register_action_plain(ignore_content_cmd, [] { the_parser.T_raw_env(false); });
    register_action_plain(ignore_env_cmd, [] {});
    register_action_plain(ignore_two_argument_cmd, [] { the_parser.ignore_arg(), the_parser.ignore_arg(); });
    register_action_plain(ignoreA_cmd, [] { the_parser.T_ignoreA(); });
    register_action_plain(ignorep_cmd, &Parser::T_par1);
    register_action_plain(includegraphics_cmd, &Parser::includegraphics);
    register_action_plain(index_cmd, &Parser::T_index);
    register_action_plain(inputclass_cmd, &Parser::T_inputclass);
    register_action_plain(insertbibliohere_cmd, [] { Parser::add_bib_marker(true); });
    register_action_plain(isin_cmd, &Parser::T_isin);
    register_action_plain(item_cmd, &Parser::T_item);
    register_action_plain(itemize_cmd, &Parser::T_listenv);
    register_action_plain(kern_cmd, [](subtypes c) { the_parser.scan_dimen(c == 1, the_parser.cur_tok); });
    register_action_plain(keywords_cmd, &Parser::T_keywords);
    register_action_plain(kvo_family_cmd, &Parser::kvo_family);
    register_action_plain(l3_check_cmd, &Parser::L3_check_cmd);
    register_action_plain(l3_gen_cond_Nnn_cmd, &Parser::L3_new_conditional);
    register_action_plain(l3_gen_cond_Npnn_cmd, &Parser::L3_new_conditional_parm);
    register_action_plain(l3_gen_eq_cond_cmd, &Parser::L3_eq_conditional);
    register_action_plain(l3_gen_from_ac_cmd, &Parser::Tl3_gen_from_ac);
    register_action_plain(l3_gen_from_sig_cmd, &Parser::generate_from_sig);
    register_action_plain(l3_generate_variant_cmd, &Parser::l3_generate_variant);
    register_action_plain(l3_rescan_cmd, &Parser::tl_set_rescan);
    register_action_plain(l3_set_cat_cmd, &Parser::L3_set_cat_code);
    register_action_plain(l3_set_num_cmd, &Parser::L3_set_num_code);
    register_action_plain(label_cmd, [](subtypes c) { the_parser.flush_buffer(), the_parser.T_label(c); });
    register_action_plain(latex_error_cmd, &Parser::T_class_error);
    register_action_plain(leave_v_mode_cmd, &Parser::leave_v_mode);
    register_action_plain(let_cmd, &Parser::M_prefixed);
    register_action_plain(letter_catcode, &Parser::translate_char);
    register_action_plain(line_cmd, &Parser::T_line);
    register_action_plain(linebreak_cmd, &Parser::ignore_optarg);
    register_action_plain(list_cmd, &Parser::T_listenv);
    register_action_plain(listfiles_cmd, [] { the_parser.list_files_p = true; });
    register_action_plain(load_with_options_cmd, [](subtypes c) { the_parser.T_load_with_options(c == 0); });
    register_action_plain(loadlatex3_cmd, [] {
        if (!the_parser.L3_load(false)) throw EndOfData();
    });
    register_action_plain(make_box_cmd, [](subtypes c) { the_parser.begin_box(makebox_location, c); });
    register_action_plain(makeatletter_cmd, [] { the_parser.word_define('@', letter_catcode, false); });
    register_action_plain(makeatother_cmd, [] { the_parser.word_define('@', other_catcode, false); });
    register_action_plain(matter_cmd, &Parser::T_matter);
    register_action_plain(minipage_cmd, &Parser::T_minipage);
    register_action_plain(move_cmd, [] { the_parser.scan_dimen(false, the_parser.cur_tok), the_parser.scan_box(move_location); });
    register_action_plain(multiply_cmd, &Parser::M_prefixed);
    register_action_plain(needs_format_cmd, [] { the_parser.ignore_arg(), the_parser.ignore_optarg(); });
    register_action_plain(newboolean_cmd, &Parser::M_newboolean);
    register_action_plain(newcolumntype_cmd, &Parser::T_newcolumn_type);
    register_action_plain(newcount_cmd, &Parser::new_constant);
    register_action_plain(newcounter_cmd, [] { the_parser.M_counter(true); });
    register_action_plain(newif_cmd, &Parser::M_newif);
    register_action_plain(noargfont_cmd, &Parser::see_font_change);
    register_action_plain(nobreakspace_cmd, [] { the_parser.LC(), the_parser.process_char(the_parser.global_in_url ? '~' : 0xA0); });
    register_action_plain(nolinebreak_cmd, &Parser::ignore_optarg);
    register_action_plain(numberedverbatim_cmd, [] { the_parser.numbered_verbatim = true; });
    register_action_plain(numberwithin_cmd, &Parser::numberwithin);
    register_action_plain(oldfont_cmd, &Parser::old_font);
    register_action_plain(omitcite_cmd, &Parser::T_omitcite);
    register_action_plain(open_catcode, [] { the_parser.push_level(bt_brace); });
    register_action_plain(options_not_used_cmd, &Parser::T_option_not_used);
    register_action_plain(other_catcode, &Parser::translate_char);
    register_action_plain(par_cmd, &Parser::T_par1);
    register_action_plain(pass_options_cmd, [](subtypes c) { the_parser.T_pass_options(c == 0); });
    register_action_plain(picture_env_cmd, &Parser::T_picture);
    register_action_plain(pop_stack_cmd, &Parser::pop_all_levels);
    register_action_plain(popmodule_cmd, [] { the_stack.end_module(); });
    register_action_plain(prefix_cmd, &Parser::M_prefixed);
    register_action_plain(process_options_cmd, &Parser::T_process_options);
    register_action_plain(provides_package_cmd, [](subtypes c) { the_parser.T_provides_package(c == 0); });
    register_action_plain(pushmodule_cmd, &Parser::push_module);
    register_action_plain(put_cmd, &Parser::T_put);
    register_action_plain(raw_env_cmd, [] { the_stack.add_last(new Xml(std::string(the_parser.T_raw_env(true)))); });
    register_action_plain(read_to_cs_cmd, &Parser::M_prefixed);
    register_action_plain(reevaluate_cmd, &Parser::T_reevaluate);
    register_action_plain(ref_cmd, [](subtypes c) { the_parser.leave_v_mode(), the_parser.T_ref(c == 0); });
    register_action_plain(register_cmd, &Parser::M_prefixed);
    register_action_plain(relax_cmd, [] {});
    register_action_plain(removeelement_cmd, &Parser::T_remove_element);
    register_action_plain(rule_cmd, &Parser::scan_rule);
    register_action_plain(save_box_cmd, [](subtypes c) { the_parser.T_save_box(c == 0); });
    register_action_plain(saveverb_cmd, &Parser::T_saveverb);
    register_action_plain(scan_glue_cmd, &Parser::T_scan_glue);
    register_action_plain(section_cmd, &Parser::T_paras);
    register_action_plain(selectfont_cmd, &Parser::font_has_changed);
    register_action_plain(selective_sanitize_cmd, &Parser::selective_sanitize);
    register_action_plain(self_insert_cmd, [] { the_parser.LC(), the_parser.unprocessed_xml.push_back(the_parser.cur_tok); });
    register_action_plain(set_aux_cmd, &Parser::M_prefixed);
    register_action_plain(set_box_cmd, &Parser::M_prefixed);
    register_action_plain(set_box_dimen_cmd, &Parser::M_prefixed);
    register_action_plain(set_font_cmd, &Parser::M_prefixed);
    register_action_plain(set_interaction_cmd, &Parser::M_prefixed);
    register_action_plain(set_mathchar_cmd, &Parser::M_prefixed);
    register_action_plain(set_mathprop_cmd, &Parser::M_prefixed);
    register_action_plain(set_page_dimen_cmd, &Parser::M_prefixed);
    register_action_plain(set_page_int_cmd, &Parser::M_prefixed);
    register_action_plain(set_prev_graf_cmd, &Parser::M_prefixed);
    register_action_plain(set_shape_cmd, &Parser::M_prefixed);
    register_action_plain(setboolean_cmd, &Parser::set_boolean);
    register_action_plain(setlanguage_cmd, [] { the_parser.scan_int(the_parser.cur_tok); });
    register_action_plain(setmode_cmd, &Parser::T_setmode);
    register_action_plain(shorthand_def_cmd, &Parser::M_prefixed);
    register_action_plain(shortverb_cmd, &Parser::M_shortverb);
    register_action_plain(solvecite_cmd, [] { the_parser.solve_cite(false); });
    register_action_plain(soul_cmd, &Parser::T_fonts);
    register_action_plain(specimp_cmd, &Parser::T_specimp);
    register_action_plain(start_par_cmd, &Parser::implicit_par);
    register_action_plain(sub_cmd, &Parser::T_fonts);
    register_action_plain(subequations_cmd, [] { the_parser.T_subequations(true); });
    register_action_plain(subfigure_cmd, &Parser::T_subfigure);
    register_action_plain(table_cmd, [](symcodes x, subtypes c) { the_parser.T_figure_table(x, c); });
    register_action_plain(tabular_env_cmd, &Parser::T_start_tabular);
    register_action_plain(testopt_cmd, &Parser::T_testopt);
    register_action_plain(thebibliography_cmd, &Parser::T_start_the_biblio);
    register_action_plain(thickness_cmd, &Parser::T_linethickness);
    register_action_plain(tl_basic_cmd, &Parser::l3_new_token_list);
    register_action_plain(tl_concat_cmd, &Parser::l3_tl_concat);
    register_action_plain(tl_put_left_cmd, &Parser::l3_tl_put_left);
    register_action_plain(tl_set_cmd, &Parser::l3_tl_set);
    register_action_plain(toks_register_cmd, &Parser::M_prefixed);
    register_action_plain(tracingall_cmd, &Parser::M_tracingall);
    register_action_plain(trees_cmd, &Parser::T_trees);
    register_action_plain(typein_cmd, &Parser::T_typein);
    register_action_plain(un_box_cmd, &Parser::T_un_box);
    register_action_plain(unimp_cmd, &Parser::T_unimp);
    register_action_plain(unimp_font_cmd, &Parser::T_unimplemented_font);
    register_action_plain(unnumberedverbatim_cmd, [] { the_parser.numbered_verbatim = false; });
    register_action_plain(url_cmd, &Parser::T_url);
    register_action_plain(usecounter_cmd, &Parser::T_use_counter);
    register_action_plain(usefont_cmd, &Parser::T_usefont);
    register_action_plain(verb_cmd, [](subtypes c) { the_parser.T_verb(c != 0U ? the_parser.verb_saved_char : char32_t(0U)); });
    register_action_plain(verbatim_env_cmd, &Parser::T_verbatim);
    register_action_plain(vfill_cmd, [](subtypes c) { the_parser.leave_h_mode(), the_stack.add_newid0(vfill_to_np(c)); });
    register_action_plain(whiledo_cmd, &Parser::T_whiledo);
    register_action_plain(xfancy_cmd, &Parser::T_xfancy);
    register_action_plain(XML_fetch_cmd, &Parser::user_XML_fetch);
    register_action_plain(XML_modify_cmd, &Parser::user_XML_modify);
    register_action_plain(XML_swap_cmd, &Parser::user_XML_swap);
    register_action_plain(xmlelement_env_cmd, &Parser::T_xmlenv);
    register_action_plain(xmlelt_cmd, &Parser::T_xmlelt);
    register_action_plain(xray_cmd, &Parser::M_xray);
    register_action_plain(xthepage_cmd, [] { the_parser.flush_buffer(), the_stack.add_last(the_page_xml); });

    register_action(begin_cmd, &Parser::T_beginend);
    register_action(end_cmd, &Parser::T_beginend);

    // \todo wrap the next ones in functions to fit on one line

    register_action_plain(end_ignore_content_cmd, [] {
        the_parser.parse_error(the_parser.cur_tok, "missing \\begin environment ", the_parser.cur_tok.tok_to_str(), "missing begin");
    });

    register_action_plain(end_math_env_cmd, [] {
        the_parser.parse_error(the_parser.cur_tok, "missing \\begin environment ", the_parser.cur_tok.tok_to_str(), "missing begin");
    });

    register_action_plain(end_raw_env_cmd, [] {
        the_parser.parse_error(the_parser.cur_tok, "missing \\begin environment ", the_parser.cur_tok.tok_to_str(), "missing begin");
    });

    register_action_plain(last_item_cmd,
                          [] { the_parser.parse_error(the_parser.cur_tok, "Read only variable ", the_parser.cur_tok, "", "readonly"); });

    register_action_plain(space_catcode, [](subtypes c) {
        if (!the_stack.in_v_mode() && !the_stack.in_no_mode() && !the_stack.in_bib_mode()) the_parser.process_char(char32_t(c));
    });

    register_action_plain(inhibit_xml_cmd, [] {
        the_main.no_xml = true;
        spdlog::warn("syntaxonly: no XML file will be produced");
    });

    register_action_plain(xmllatex_cmd, [] {
        the_parser.LC();
        the_parser.unprocessed_xml += the_parser.T_xmllatex();
    });

    register_action_plain(aparaitre_cmd, [] { // \todo multilingual stuff somewhere
        the_parser.LC();
        if (eqtb_int_table[language_code].val == 1) {
            the_parser.process_char(char32_t(0xE0U));
            the_parser.process_string(" para");
            the_parser.process_char(char32_t(0xEEU));
            the_parser.process_string("tre");
        } else
            the_parser.process_string("to appear");
    });

    register_action_plain(begingroup_cmd, [](subtypes c) {
        the_parser.flush_buffer();
        if (c == 0) {
            the_parser.push_level(bt_semisimple);
        } else if (c == 1) {
            if (!the_parser.pop_level(bt_semisimple)) throw EndOfData();
        } else {
            the_parser.get_token();
            if (!the_parser.pop_level(bt_env)) throw EndOfData();
        }
    });

    register_action_plain(hat_catcode, [](subtypes c) {
        if (the_parser.global_in_load || is_pos_par(nomath_code))
            the_parser.translate_char(CmdChr(letter_catcode, c));
        else
            the_parser.parse_error(the_parser.cur_tok, "Missing dollar not inserted, token ignored: ", the_parser.cur_tok.tok_to_str(),
                                   "Missing dollar");
    });

    register_action_plain(underscore_catcode, [](subtypes c) {
        if (the_parser.global_in_load || is_pos_par(nomath_code))
            the_parser.translate_char(CmdChr(letter_catcode, c));
        else
            the_parser.parse_error(the_parser.cur_tok, "Missing dollar not inserted, token ignored: ", the_parser.cur_tok.tok_to_str(),
                                   "Missing dollar");
    });

    register_action_plain(backslash_cmd, [](subtypes c) {
        if (c == 0)
            the_parser.T_backslash();
        else
            the_parser.T_newline();
    });

    register_action_plain(skip_cmd, [](subtypes c) {
        the_parser.append_glue(the_parser.cur_tok, ScaledInt{(c == smallskip_code ? 3 : c == medskip_code ? 6 : 12) << 16}, true);
    });

    register_action_plain(ltfont_cmd, [](subtypes c) {
        the_parser.flush_buffer();
        the_parser.cur_font.ltfont(the_parser.sT_arg_nopar(), c);
    });

    register_action_plain(ignore_cmd, [](subtypes c) {
        if (c == addnl_code) {
            the_parser.flush_buffer();
            the_stack.add_nl();
        } else if (c == unskip_code) {
            if (the_parser.unprocessed_xml.empty())
                the_stack.remove_last_space();
            else
                the_parser.unprocessed_xml.remove_last_space();
        }
    });

    register_action_plain(ignore_one_argument_cmd, [](subtypes c) {
        if (c == patterns_code || c == hyphenation_code || c == special_code) the_parser.scan_left_brace_and_back_input();
        the_parser.ignore_arg();
    });

    register_action_plain(after_assignment_cmd, [] {
        the_parser.get_token();
        the_parser.set_after_ass_tok(the_parser.cur_tok);
        if (tracing_commands()) {
            spdlog::trace("{{\\afterassignment: {}}}", fmt::streamed(the_parser.cur_tok));
        }
    });

    register_action_plain(leader_ship_cmd, [](subtypes c) {
        the_parser.scan_box(c == shipout_code    ? shipout_location
                            : c == leaders_code  ? leaders_location
                            : c == cleaders_code ? cleaders_location
                                                 : xleaders_location);
    });

    register_action_plain(vglue_cmd, [](subtypes c) {
        if (c == 0)
            the_parser.T_par1();
        else
            the_parser.leave_v_mode();
        the_parser.T_scan_glue(c == 0 ? vskip_code : hskip_code);
    });

    register_action_plain(titlepage_cmd, [](subtypes c) {
        if (!the_stack.in_v_mode()) the_parser.wrong_mode("Bad titlepage command");
        Parser::T_titlepage(c);
    });

    register_action_plain(package_cmd, [] {
        if (!the_stack.in_v_mode() || the_parser.seen_document) the_parser.wrong_mode("Bad \\usepackage command");
        the_parser.T_usepackage();
    });

    register_action_plain(centering_cmd, [](subtypes c) {
        the_parser.word_define(incentering_code, c, false);
        if (c != 0U) the_stack.add_center_to_p();
    });

    register_action_plain(fbox_cmd, [](subtypes c) {
        if (c == dashbox_code)
            the_parser.T_fbox_dash_box();
        else if (c == rotatebox_code)
            the_parser.T_fbox_rotate_box();
        else
            the_parser.T_fbox(c);
    });

    register_action_plain(only_preamble_cmd, [] {
        the_parser.get_r_token(true);
        onlypreamble.push_back(hash_table.let_token);
        onlypreamble.push_back(the_parser.cur_tok);
        onlypreamble.push_back(hash_table.notprerr_token);
    });

    register_action_plain(toc_cmd, [](subtypes c) {
        std::string np = "tableofcontents";
        if (c == 1) np = "listoftables";
        if (c == 2) np = "listoffigures";
        the_parser.remove_initial_star();
        the_parser.leave_h_mode();
        the_stack.push1(::the_names[np]);
        if (c == 0) {
            static bool inserted = false;
            if (!inserted) the_stack.top_stack()->id = 4;
            inserted = true;
            auto k   = eqtb_int_table[42 + count_reg_offset].val;
            the_stack.add_att_to_cur(std::string("depth"), std::string(std::to_string(k)));
        }
        the_stack.pop(::the_names[np]);
    });

    register_action_plain(center_cmd, [](subtypes c) {
        the_parser.leave_h_mode();
        the_stack.add_nl();
        the_parser.word_define(incentering_code, c, false);
    });

    register_action_plain(thm_aux_cmd, [](subtypes c) {
        TokenList L = the_parser.read_arg();
        the_parser.token_list_define(c, L, false);
    });

    register_action_plain(start_thm_cmd, [](subtypes c) {
        if (c == 2)
            the_parser.T_end_theorem();
        else
            the_parser.T_start_theorem(c);
    });

    register_action_plain(math_env_cmd, [](subtypes c) {
        the_parser.cur_tok.kill();
        if (!the_parser.pop_level(bt_env)) throw EndOfData();
        return the_parser.T_math(c);
    });

    register_action_plain(end_minipage_cmd, [] {
        the_parser.flush_buffer();
        the_stack.pop_if_frame(::the_names["cst_p"]);
        the_stack.pop_if_frame(::the_names["item"]);
        the_stack.pop(::the_names["minipage"]);
    });
}
