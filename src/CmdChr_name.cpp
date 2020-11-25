#include "tralics/CmdChr.h"
#include "tralics/Symcode.h"
#include "tralics/globals.h"

namespace {
    auto make_name16(const std::string &x, size_t y) -> std::string { return fmt::format("{}\"{:X}", x, y); }

    // This converts `endfoo' to `foo'. It does not check that the
    // string starts with `end', just that is is not zero.
    auto strip_end(String s) -> String {
        if (s == nullptr) return nullptr;
        return s + 3;
    }
} // namespace

// This returns the name of a CmdChr pair.
// The result is a UTF8 string
auto CmdChr::name() const -> std::string {
    if (auto res = Symcode::get(cmd).name(chr)) return *res;

    switch (cmd) {
    case section_cmd: return token_section_name();
    case label_cmd: return chr == 0 ? "label" : (chr == 1 ? "anchor" : "anchorlabel");
    case ref_cmd: return chr == 0 ? "ref" : "pageref";
    case eqref_cmd: return "XMLref";
    case par_cmd: return "par";
    case start_par_cmd: return chr == 0 ? "noindent" : "indent";
    case endcsname_cmd: return "endcsname";
    case multicolumn_cmd: return "multicolumn";
    case atdocument_cmd: return chr == 0 ? "AtBeginDocument" : "AtEndDocument";
    case fvset_cmd: return "fvset";
    case calc_cmd: return "calc ";
    case biblio_cmd: return "bibliography";
    case aparaitre_cmd: return "aparaitre";
    case citation_cmd: return "citation";
    case end_citation_cmd: return "endcitation";
    case thebibliography_cmd: return "thebibliography";
    case end_thebibliography_cmd: return "endthebibliography";
    case bpers_cmd: return "bpers";
    case cititem_cmd: return "cititem";
    case bibitem_cmd: return "bibitem";
    case cite_one_cmd: return chr == 0 ? "cite@one" : "cite@simple";
    case cite_cmd: return token_cite_name();
    case solvecite_cmd: return "XMLsolvecite";
    case bib_cmd: return chr == 0 ? "bauthors" : "beditors";
    case backslash_cmd: return chr == 0 ? "\\" : "newline";
    case omitcite_cmd: return "omitcite";
    case hline_cmd:
        if (chr == one_code) return "cline";
        if (chr == two_code) return "hlinee";
        return "hline";
    case skip_cmd:
        if (chr == smallskip_code) return "smallskip";
        if (chr == medskip_code) return "medskip";
        if (chr == bigskip_code) return "bigskip";
        return nullptr;
    case hfill_cmd:
        if (chr == hfil_code) return "hfil";
        if (chr == hfill_code) return "hfill";
        if (chr == hfilneg_code) return "hfilneg";
        if (chr == hss_code) return "hss";
        return nullptr;
    case vfill_cmd:
        if (chr == vfil_code) return "vfil";
        if (chr == vfill_code) return "vfill";
        if (chr == vfilneg_code) return "vfilneg";
        if (chr == vss_code) return "vss";
        return nullptr;
    case soul_cmd:
    case sub_cmd: return token_soul_name();
    case oldfont_cmd: return token_oldfont_name();
    case argfont_cmd: return token_argfont_name();
    case noargfont_cmd: return token_noargfont_name();
    case fontsize_cmd: return token_fontsize_name();
    case item_cmd: return chr == 0 ? "item" : "@item";
    case usecounter_cmd: return "usecounter";
    case get_config_cmd: return chr == 0 ? "tralics@find@config" : "tralics@getd@config";
    case kvo_family_cmd:
        switch (chr) {
        case kvo_fam_set_code: return "KVO@family@set";
        case kvo_fam_get_code: return "KVO@family";
        case kvo_pre_set_code: return "KVO@prefix@set";
        case kvo_pre_get_code: return "KVO@prefix";
        case kvo_bool_opt_code: return "DeclareBoolOption";
        case kvo_comp_opt_code: return "DeclareComplementaryOption";
        case kvo_boolkey_code: return "KVO@boolkey";
        case kvo_voidkey_code: return "KVO@voidkey";
        case kvo_string_opt_code: return "DeclareStringOption";
        case kvo_void_opt_code: return "DeclareVoidOption";
        case kvo_disable_code: return "DisableKeyvalOption";
        case kvo_decdef_code: return "DeclareDefaultOption";
        case kvo_process_code: return "ProcessKeyvalOptions";
        default: return nullptr;
        }
    case doc_class_cmd: return chr == 0 ? "documentclass" : "LoadClass";
    case package_cmd: return "usepackage";
    case inputclass_cmd: return "InputClass";
    case needs_format_cmd: return "NeedsTeXFormat";
    case if_package_loaded_cmd: return chr == 0 ? "@ifclassloaded" : "@ifpackageloaded";
    case if_package_later_cmd: return chr == 0 ? "@ifclasslater" : "@ifpackagelater";
    case if_package_with_cmd: return chr == 0 ? "@ifclasswith" : "@ifpackagewith";
    case provides_package_cmd: return chr == 0 ? "ProvidesFile" : chr == 1 ? "ProvidesPackage" : "ProvidesClass";
    case pass_options_cmd: return chr == 0 ? "PassOptionsToClass" : "PassOptionsToPackage";
    case declare_options_cmd: return "DeclareOption";
    case options_not_used_cmd: return "OptionNotUsed";
    case process_options_cmd: return "ProcessOptions";
    case execute_options_cmd: return "ExecuteOptions";
    case at_end_of_class_cmd: return chr == 0 ? "AtEndOfClass" : "AtEndOfPackage";
    case load_with_options_cmd: return chr == 0 ? "LoadClassWithOptions" : "RequirePackageWithOptions";
    case float_cmd:
        switch (chr) {
        case 0: return "@float";
        case 1: return "@dblfloat";
        case 2: return "float@end";
        case 3: return "float@dblend";
        default: return nullptr;
        }
    case subfigure_cmd: return "subfigure";
    case pop_stack_cmd: return "pop@stack";
    case box_cmd: return token_box_name();
    case math_comp_cmd: return token_mathcomp_name();
    case big_cmd: return token_big_name();
    case style_cmd: return token_style_name();
    case limits_cmd: return token_limits_name();
    case ensuremath_cmd: return "ensuremath";
    case nonumber_cmd: return "nonumber";
    case nonscript_cmd: return "nonscript";
    case line_cmd: return token_line_name();
    case linebreak_cmd: return token_linebreak_name();
    case fancy_cmd: return token_fancy_name();
    case xfancy_cmd: return "fancyinternal";
    case xthepage_cmd: return "inert@thepage";
    case url_cmd: return "url";
    case hanl_cmd: return chr == 0 ? "htmladdnormallink" : (chr == 1 ? "htmladdnormallinkfoot" : "href");
    case addatt_cmd:
        switch (chr) {
        case addatt_to_cur_code: return "AddAttToCurrent";
        case addatt_to_last_code: return "AddAttToLast";
        case addatt_to_doc_code: return "addattributetodocument";
        case addatt_to_code: return "XMLaddatt";
        case addatt_to_index_code: return "addattributetoindex";
        default: return nullptr;
        }
    case over_cmd: return token_over_name();
    case begingroup_cmd: return chr == 0 ? "begingroup" : chr == 1 ? "endgroup" : "endenv";
    case startprojet_cmd: return "RAstartprojet";
    case declaretopics_cmd: return "declaretopic";
    case footnote_cmd: return "footnote";
    case popmodule_cmd: return "tralics@pop@module";
    case pushmodule_cmd: return "tralics@push@module";
    case fnhack_cmd: return "tralics@fnhack";
    case caption_cmd: return "caption";
    case centering_cmd: return token_centering_name();
    case save_box_cmd: return chr == 0 ? "sbox" : "savebox";
    case fbox_cmd: return token_fbox_name();
    case color_cmd: return token_color_name();
    case hspace_cmd:
        switch (chr) {
        case 0: return "hspace";
        case 1: return "vspace";
        case 2: return "hspace*";
        default: return "vspace*";
        }
    case includegraphics_cmd: return chr == 0 ? "includegraphics" : "psfig";
    case error_cmd: return chr == 1 ? "@notprerr" : "error";
    case glo_cmd: return "glo";
    case reevaluate_cmd: return "@reevaluate";
    case xmllatex_cmd: return "xmllatex";
    case specimp_cmd:
    case unimp_cmd: return token_unimp_name();
    case unimp_font_cmd: return token_unimp_font_name();
    case trees_cmd: return token_trees_name();
    case index_cmd: return token_index_name();
    case gloss_cmd: return chr == 0 ? "cgloss@gll" : "cgloss@glll";
    case only_preamble_cmd: return "@onlypreamble";
    case toc_cmd: {
        if (chr == 1) return "listoftables";
        if (chr == 2) return "listoffigures";
        return "tableofcontents";
    }
    case random_cmd: return "tralics@random";
    case pdfstrcmp_cmd: return "pdfstrcmp";
    case ot2enc_cmd: {
        if (chr == 1) return "OT2-encoding";
        if (chr == 2) return "OT1-encoding";
        return "T1-encoding";
    }
    case mathversion_cmd: return "mathversion";
    case matter_cmd:
        switch (chr) {
        case mainmatter_code: return "mainmatter";
        case frontmatter_code: return "frontmatter";
        case backmatter_code: return "backmatter";
        default: return nullptr;
        }
    case cr_cmd:
        switch (chr) {
        case cr_code: return "cr";
        case mycr_code: return "cr";
        case crcr_code: return "crcr";
        case crwithargs_code: return "cr withargs";
        default: return nullptr;
        }
    case scan_glue_cmd:
        if (chr == hskip_code) return "hskip";
        if (chr == vskip_code) return "vskip";
        return "mskip";
    case case_shift_cmd: return token_caseshift_name();
    case kern_cmd: return chr == 0 ? "kern" : "mkern";
    case make_box_cmd: return token_makebox_name();
    case XML_swap_cmd:
        if (chr == 0) return "XML@moveAB";
        if (chr == 1) return "XML@moveBA";
        return "XML@swapAB";
    case XML_fetch_cmd: return "XML@fetch";
    case XML_modify_cmd:
        switch (chr) {
        case xml_get_code: return "XML@get";
        case xml_ins_code: return "XML@insert";
        case xml_del_code: return "XML@delete";
        case xml_set_code: return "XML@set";
        case xml_parent_code: return "XML@parent";
        case xml_setA_code: return "XML@setA";
        case xml_setB_code: return "XML@setB";
        default: return nullptr;
        }
    case char_num_cmd: return "char";
    case cst1_cmd:
    case cst2_cmd:
    case cst_cmd: return token_cst_name();
    case specchar_cmd: return specchar_cmd_name();
    case nobreakspace_cmd: return "nobreakspace";
    case input_cmd:
    case file_cmd: return token_file_name();
    case xmlelt_cmd: return chr == 0 ? "xmlelt" : (chr == 1 ? "xmlemptyelt" : "xmlcomment");
    case newcolumntype_cmd: return "newcolumntype";
    case arg_font_cmd: return chr == lsc_code ? "lsc" : "fsc";
    case rule_cmd: return chr == hrule_code ? "hrule" : chr == vrule_code ? "vrule" : "rule";
    case document_cmd: return "document";
    case keywords_cmd: return "motscle";
    case center_cmd: return strip_end(token_ecenter_name());
    case figure_cmd: return strip_end(token_efigure_name());
    case table_cmd: return chr == 0 ? "table" : chr == 2 ? "wraptable" : "table*";
    case glossaire_cmd: return "glossaire";
    case itemize_cmd: return "itemize";
    case list_cmd: return "list";
    case enumerate_cmd: return "enumerate";
    case description_cmd: return "description";
    case listfiles_cmd: return "listfiles";
    case ignore_env_cmd: return strip_end(token_eignore_name());
    case ignore_content_cmd: return strip_end(token_eignorec_name());
    case raw_env_cmd: return "rawxml";
    case math_env_cmd: return strip_end(tralics_ns::math_env_name(chr));
    case tabular_env_cmd: return chr == 0 ? "tabular" : "tabular*";
    case verbatim_env_cmd: return chr == 0 ? "verbatim" : chr == 1 ? "Verbatim" : "@verbatim";
    case minipage_cmd: return "minipage";
    case subequations_cmd: return "subequations";
    case picture_env_cmd: return "picture";
    case xmlelement_env_cmd: return chr == 0 ? "xmlelement" : "xmlelement*";
    case filecontents_env_cmd: return chr == 0 ? "filecontents" : chr == 1 ? "filecontents*" : chr == 2 ? "filecontents+" : "filecontents";
    case end_document_cmd: return chr == 0 ? "enddocument" : "real-enddocument";
    case end_keywords_cmd: return "endmotscle";
    case end_center_cmd: return token_ecenter_name();
    case end_figure_cmd: return token_efigure_name();
    case end_table_cmd: return chr == 0 ? "endtable" : chr == 2 ? "endwraptable" : "endtable*";
    case end_glossaire_cmd: return "endglossaire";
    case end_itemize_cmd: return "enditemize";
    case end_list_cmd: return "endlist";
    case end_enumerate_cmd: return "endenumerate";
    case end_description_cmd: return "enddescription";
    case end_ignore_env_cmd: return token_eignore_name();
    case end_ignore_content_cmd: return token_eignorec_name();
    case end_raw_env_cmd: return "endrawxml";
    case end_math_env_cmd: return tralics_ns::math_env_name(chr);
    case end_tabular_env_cmd: return chr == 0 ? "endtabular" : "endtabular*";
    case end_verbatim_env_cmd: return chr == 0 ? "endverbatim" : chr == 1 ? "endVerbatim" : "end@verbatim";
    case end_minipage_cmd: return "endminipage";
    case end_subequations_cmd: return "endsubequations";
    case end_picture_env_cmd: return "endpicture";
    case end_xmlelement_env_cmd: return chr == 0 ? "endxmlelement" : "endxmlelement*";
    case end_filecontents_env_cmd: return chr == 0 ? "endfilecontents" : "endfilecontents*";
    case begin_cmd: return "begin";
    case end_cmd: return "end";
    case leave_v_mode_cmd: return "leavevmode";
    case epsfbox_cmd: return "epsfbox";
    case put_cmd: return token_put_name();
    case bezier_cmd: return chr == 0 ? "bezier" : "qbezier";
    case dashline_cmd: return token_dashline_name();
    case ignore_cmd:
    case ignoreA_cmd:
    case ignorep_cmd: return token_ignore_name();
    case relax_cmd: return "relax";
    case ltfont_cmd: return token_ltfont_name();
    case ignore_one_argument_cmd: return token_ign1_name();
    case ignore_two_argument_cmd: return token_ign2_name();
    case defineverbatimenv_cmd: return "DefineVerbatimEnvironment";
    case saveverb_cmd: return "SaveVerb";
    case add_to_macro_cmd: return chr == 1 ? "tralics@addtolist@o" : "tralics@addtolist@n";
    case makeatletter_cmd: return "makeatletter";
    case makeatother_cmd: return "makeatother";
    case numberedverbatim_cmd: return "numberedverbatim";
    case unnumberedverbatim_cmd: return "unnumberedverbatim";
    case after_assignment_cmd: return "afterassignment";
    case un_box_cmd: return token_unbox_name();
    case extension_cmd: return token_extension_name();
    case setlanguage_cmd: return "setlanguage";
    case xray_cmd: return token_xray_name();
    case move_cmd: return token_move_name();
    case leader_ship_cmd: return token_leader_name();
    case selectfont_cmd: return "selectfont";
    case usefont_cmd: return "usefont";
    case isin_cmd: return "in@";
    case tracingall_cmd: return "tracingall";
    case newif_cmd: return "newif";
    case newcount_cmd: return token_newcount_name();
    case newboolean_cmd: return chr == 0 ? "newboolean" : "provideboolean";
    case setboolean_cmd: return "setboolean";
    case ifthenelse_cmd: return "ifthenelse";
    case whiledo_cmd: return "whiledo";
    case expandtwoargs_cmd: return "@expandtwoargs";
    case removeelement_cmd: return "@removeelement";
    case shortverb_cmd: return chr == 0 ? "DefineShortVerb" : "UndefineShortVerb";
    case newcounter_cmd: return "newcounter";
    case aftergroup_cmd: return "aftergroup";
    case latex_ctr_cmd: return token_latexctr_name();
    case ifundefined_cmd: return chr == 0 ? "@ifundefined" : "tralics@ifundefined";
    case ifstar_cmd: return "@ifstar";
    case ifnextchar_cmd: return chr == 0 ? "@ifnextchar" : "@ifnextcharacter";
    case ifempty_cmd: return chr == 0 ? "@iftempty" : "@ifbempty";
    case for_cmd: return token_for_name();
    case fp_cmd: return token_fp_names();
    case fpif_cmd: return token_fp_names();
    case fpi_cmd: return token_fpi_names();
    case verb_cmd: return chr == 0 ? "verb" : " verb";
    case last_item_cmd: return token_lastitem_name();
    case toks_register_cmd: return "toks";
    case assign_toks_cmd: return token_assigntoks_name();
    case assign_int_cmd: return token_assignint_name();
    case assign_dimen_cmd: return token_assigndimen_name();
    case assign_glue_cmd: return token_assignglue_name();
    case assign_mu_glue_cmd: return token_assignmuglue_name();
    case assign_font_dimen_cmd: return "fontdimen";
    case assign_enc_char_cmd: return "input@encoding@val";
    case assign_font_int_cmd: return chr == 0 ? "hyphenchar" : "skewchar";
    case set_aux_cmd:
        switch (chr) {
        case 0: return "spacefactor";
        case 1: return "prevdepth";
        case 2: return "input@encoding";
        case 4: return "@xfontsize";
        default: return "input@encoding@default";
        }
    case set_prev_graf_cmd: return "prevgraf";
    case set_page_dimen_cmd: return token_setpagedimen_name();
    case set_page_int_cmd:
        if (chr == deadcycles_code) return "deadcycles";
        if (chr == interactionmode_code) return "interactionmode";
        return "insertpenalties";
    case set_box_dimen_cmd: return token_setboxdimen_name();
    case thm_aux_cmd:
        if (chr == theorem_style_code) return "theoremstyle";
        if (chr == theorem_bodyfont_code) return "theorembodyfont";
        if (chr == theorem_headerfont_code) return "theoremheaderfont";
        return nullptr;
    case start_thm_cmd: return chr == 2 ? "@endtheorem" : "@begintheorem";
    case setmode_cmd: return "@setmode";
    case set_shape_cmd: return token_shape_name();
    case def_code_cmd: return token_defcode_name();
    case def_family_cmd: return token_deffamily_name();
    case set_font_cmd: return "nullfont";
    case def_font_cmd: return "font";
    case set_mathprop_cmd: return "mathfontproperty";
    case set_mathchar_cmd: return "setmathchar";
    case register_cmd: return token_register_name();
    case advance_cmd: return "advance";
    case multiply_cmd: return "multiply";
    case divide_cmd: return "divide";
    case prefix_cmd: return token_prefix_name();
    case let_cmd:
        switch (chr) {
        case let_code: return "let";
        case futurelet_code: return "futurelet";
        case letNN_code: return "cs_set_eq:NN";
        case letcN_code: return "cs_set_eq:cN";
        case letNc_code: return "cs_set_eq:Nc";
        case letcc_code: return "cs_set_eq:cc";
        case gletNN_code: return "cs_gset_eq:NN";
        case gletcN_code: return "cs_gset_eq:cN";
        case gletNc_code: return "cs_gset_eq:Nc";
        case gletcc_code: return "cs_gset_eq:cc";
        case nletNN_code: return "cs_new_eq:NN";
        case nletcN_code: return "cs_new_eq:cN";
        case nletNc_code: return "cs_new_eq:Nc";
        case nletcc_code: return "cs_new_eq:cc";
        case undef_code: return "cs_undefine:N";
        case undefc_code: return "cs_undefine:c";
        default: return nullptr;
        }
    case shorthand_def_cmd: return token_shorthand_name();
    case read_to_cs_cmd: return chr == 0 ? "read" : "readline";
    case def_cmd: return token_def_name();
    case set_box_cmd: return "setbox";
    case set_interaction_cmd: return token_setinteraction_name();
    case xspace_cmd: return "xspace";
    case strippt_cmd: return "strip@pt";
    case the_cmd:
        if (chr == the_code)
            return "the";
        else if (chr == unexpanded_code)
            return "unexpanded";
        else
            return "detokenize";
    case convert_cmd: return token_convert_name();
    case obracket_cmd: return chr == 0 ? "[" : "]";
    case oparen_cmd: return chr == 0 ? "(" : ")";
    case csname_cmd: return "csname";
    case usename_cmd: return chr == 0 ? "use:c" : (chr == 1 ? "exp_args:Nc" : "exp_args:cc");
    case l3expand_aux_cmd: return l3_expand_aux_name();
    case l3expand_base_cmd: return l3_expand_base_name();
    case l3_ifx_cmd: return l3_ifx_name();
    case tl_basic_cmd: return l3_tl_basic_name();
    case tl_concat_cmd: return tl_concat_name();
    case tl_set_cmd: return tl_set_name();
    case l3_rescan_cmd: return l3_rescan_name();
    case tl_put_left_cmd: return tl_put_left_name();
    case l3str_ifeq_cmd: return l3str_ifeq_name();
    case l3str_case_cmd: return l3str_case_name();
    case l3_set_cat_cmd: return l3_set_cat_name();
    case cat_ifeq_cmd: return cat_ifeq_name();
    case token_if_cmd: return token_if_name();
    case l3_set_num_cmd:
    case l3E_set_num_cmd: // these two are shared
        return l3_set_num_name();
    case l3noexpand_cmd:
        if (chr == l3expc_code) return "exp_not:c";
        if (chr == l3expo_code) return "exp_not:o";
        if (chr == l3expf_code) return "exp_not:f";
        if (chr == l3expv_code) return "exp_not:v";
        if (chr == l3expV_code) return "exp_not:V";
        return nullptr;
    case expandafter_cmd: return chr == 0 ? "expandafter" : "unless";
    case titlepage_cmd: return "titlepage";
    case noexpand_cmd: return "noexpand";
    case scan_up_down_cmd: return "@scanupdown";
    case sideset_cmd: return "sideset";
    case split_cmd: return "tralics@split";
    case a_cmd: return "a";
    case accent_cmd: return token_accent_name();
    case loop_cmd: return "loop";
    case counter_cmd: return token_counter_name();
    case setlength_cmd: return chr == 0 ? "setlength" : "addtolength";
    case useverb_cmd: return "UseVerb";
    case all_of_one_cmd:
        switch (chr) {
        case 0: return "@firstofone";
        case 1: return "use:n";
        case 2: return "use:nn";
        case 3: return "use:nnn";
        case 4: return "use:nnnn";
        default: return "use_i_ii:nnn";
        }
    case ignore_n_args_cmd:
        switch (chr) {
        case 1: return "use_none:n";
        case 2: return "use_none:nn";
        case 3: return "use_none:nnn";
        case 4: return "use_none:nnnn";
        case 5: return "use_none:nnnnn";
        case 6: return "use_none:nnnnnn";
        case 7: return "use_none:nnnnnnn";
        case 8: return "use_none:nnnnnnnn";
        default: return "use_none:nnnnnnnnn";
        }
    case l3_gen_cond_Npnn_cmd:
        switch (chr) {
        case L3_set_code: return "prg_set_conditional:Npnn";
        case L3_new_code: return "prg_new_conditional:Npnn";
        case L3_setp_code: return "prg_set_protected_conditional:Npnn";
        default: return "prg_new_protected_conditional:Npnn";
        }
    case l3_gen_cond_Nnn_cmd:
        switch (chr) {
        case L3_set_code: return "prg_set_conditional:Nnn";
        case L3_new_code: return "prg_new_conditional:Nnn";
        case L3_setp_code: return "prg_set_protected_conditional:Nnn";
        default: return "prg_new_protected_conditional:Nnn";
        }
    case l3_gen_eq_cond_cmd: return (chr == L3_set_code ? "prg_set_eq_conditional:NNn" : "prg_new_eq_conditional:NNn");
    case l3_check_cmd:
        switch (chr) {
        case 0: return "__chk_if_free_cs:N";
        case 1: return "__chk_if_free_cs:c";
        case 2: return "__chk_if_exist_cs:N";
        case 3: return "__chk_if_exist_cs:c";
        default: return nullptr;
        }
    case l3_gen_from_ac_cmd:
        return (chr == 0) ? "cs_generate_from_arg_count:NNnn"
                          : (chr == 1 ? "cs_generate_from_arg_count:cNnn" : "cs_generate_from_arg_count:Ncnn");
    case l3_generate_variant_cmd: return "cs_generate_variant:Nn";
    case l3_gen_from_sig_cmd: return "__cs_generate_from_signature:NNn";
    case GetIdInfo_cmd: return "GetIdInfo_cmd";
    case splitfun_cmd: return chr == 0 ? "__cs_get_function_name:N" : "__cs_get_function_signature:N";
    case first_of_two_cmd: return chr == 1 ? "@firstoftwo" : "@secondoftwo";
    case prg_return_cmd: return chr == 0 ? "prg_return_true:" : "prg_return_false:";
    case first_of_three_cmd: return chr == 1 ? "use_i:nnn" : chr == 2 ? "use_ii:nnn" : "use_iii:nnn";
    case first_of_four_cmd:
        switch (chr) {
        case 1: return "use_i:nnnn";
        case 2: return "use_ii:nnnn";
        case 3: return "use_iii:nnnn";
        default: return "use_iv:nnnn";
        }
    case if_test_cmd: return token_iftest_name();
    case fi_or_else_cmd: return token_fiorelse_name();
    case top_bot_mark_cmd: return token_mark_name();
    case char_given_cmd: return make_name16("char", chr);
    case math_given_cmd: return make_name16("mathchar", chr);
    case undef_cmd: return "undefined";
    case endv_cmd: return "endtemplate";
    case ding_cmd: return "ding";
    case grabenv_cmd: return "grabenv";
    case formatdate_cmd: return "formatdate";
    case thickness_cmd:
        if (chr == thicklines_code) return "thicklines";
        if (chr == thinlines_code) return "thinlines";
        if (chr == linethickness_code) return "linethickness";
        return nullptr;
    case bibliographystyle_cmd: return "bibliographystyle";
    case insertbibliohere_cmd: return "insertbibliohere";
    case inhibit_xml_cmd: return "syntaxonly";
    case footcitepre_cmd: return "footcitepre";
    case car_cmd: return chr == 0 ? "@car" : "@cdr";
    case latex_error_cmd: return token_error_name();
    case xkeyval_cmd: return token_xkeyval_name();
    case curves_cmd:
        switch (chr) {
        case arc_code: return "arc";
        case bigcircle_code: return "bigcircle";
        case closecurve_code: return "closecurve";
        case curve_code: return "curve";
        case tagcurve_code: return "tagcurve";
        default: return nullptr;
        }
    case check_date_cmd:
        switch (chr) {
        case 0: return "checkdate";
        case 1: return "countdays";
        case 2: return "nextdate";
        case 3: return "prevdate";
        case 4: return "datebynumber";
        default: return nullptr;
        }
    case refstepcounter_cmd: return "refstepcounter";
    case month_day_cmd: return token_monthday_name();
    case change_element_name_cmd: return "ChangeElementName";
    case multispan_cmd: return "multispan";
    case stripprefix_cmd: return "strip@prefix";
    case hexnumber_cmd: return "hexnumber@";
    case zapspace_cmd: return chr == 0 ? "zap@space" : "zap@fl@space";
    case vglue_cmd: return chr == 0 ? "vglue" : "hglue";
    case typein_cmd: return "typein";
    case cons_cmd: return "@cons";
    case afterfi_cmd: return "@afterfi";
    case afterelsefi_cmd: return "@afterelsefi";
    case testopt_cmd: return "@testopt";
    case gobble_cmd: return chr == 1 ? "@gobble" : chr == 2 ? "@gobbletwo" : "@gobblefour";
    case selective_sanitize_cmd: return "@selective@sanitize";
    case numberwithin_cmd: return "numberwithin";
    case ifdefinable_cmd: return "@ifdefinable";
    case dblarg_cmd: return "@dblarg";
    case makelabel_cmd: return "tralics@makelabel";
    default: return "";
    }
}
