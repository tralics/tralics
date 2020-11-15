#include "tralics/Dispatcher.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"
#include "tralics/types.h"

namespace {
    const std::string list[] = {"sup", "sub", "oldstyle", "caps", "hl", "so", "st", "ul"};

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

[[nodiscard]] auto Parser::translate03() -> bool {
    auto guard  = SaveErrTok(cur_tok);
    auto [x, c] = cur_cmd_chr;

    if (x == underscore_catcode && global_in_load) {
        translate_char(cur_cmd_chr);
        return true;
    }

    if (auto it = actions.find(x); it != actions.end()) return it->second(x, c);

    switch (x) {
    case cst1_cmd:
        LC();
        T_cst1(c);
        return true;
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
    case letter_catcode: translate_char(cur_cmd_chr); return true;
    case other_catcode: translate_char(cur_cmd_chr); return true;
    case char_num_cmd: extended_chars(scan_27bit_int()); return true;
    case char_given_cmd: extended_chars(size_t(c)); return true;
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
    case dollar_catcode:
        flush_buffer();
        T_math(nomathenv_code);
        return true;
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
        if (global_in_load || is_pos_par(nomath_code))
            translate_char(CmdChr(letter_catcode, c));
        else
            parse_error(cur_tok, "Missing dollar not inserted, token ignored: ", cur_tok.tok_to_str(), "Missing dollar");
        return true;
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
    case sub_cmd: T_fonts(list[c]); return true;
    case soul_cmd: T_fonts(list[c]); return true;
    case arg_font_cmd: T_fonts("font_sc"); return true;
    case special_math_cmd:
        if (c == overline_code || c == underline_code)
            T_fonts(c == overline_code ? "overline" : "underline");
        else
            math_only();
        return true;
    case footnote_cmd: T_cap_or_note(false); return true;
    case ltfont_cmd:
        flush_buffer();
        cur_font.ltfont(sT_arg_nopar(), c);
        return true;
    case citation_cmd:
        // should be mode independent...
        T_citation();
        the_stack.add_nl();
        return true;
    case bibitem_cmd: c == 1 ? T_empty_bibitem() : T_bibitem(); return true;
    case end_citation_cmd: the_stack.pop(the_names["citation"]); return true;
    case ignoreA_cmd: T_ignoreA(); return true;
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
    case relax_cmd: return true;
    case eof_marker_cmd: return true;
    case ignore_one_argument_cmd:
        if (c == patterns_code || c == hyphenation_code || c == special_code) scan_left_brace_and_back_input();
        ignore_arg();
        return true;
    case ignore_two_argument_cmd:
        ignore_arg();
        ignore_arg();
        return true;
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
    case setlanguage_cmd: //  strange...
        scan_int(cur_tok);
        return true;
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
    case vglue_cmd:
        if (c == 0)
            T_par1();
        else
            leave_v_mode();
        T_scan_glue(c == 0 ? vskip_code : hskip_code);
        return true;
    case ifnextchar_cmd: T_ifnextchar(c == 0); return true;
    case newcounter_cmd: M_counter(true); return true;
    case listfiles_cmd: list_files_p = true; return true;
    case caption_cmd: T_cap_or_note(true); return true;
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
    case pass_options_cmd: T_pass_options(c == 0); return true;
    case needs_format_cmd:
        ignore_arg();
        ignore_optarg();
        return true;
    case label_cmd:
        flush_buffer();
        T_label(c);
        return true;
    case ref_cmd:
        leave_v_mode();
        T_ref(c == 0);
        return true;
    case eqref_cmd: // Case \XMLref
        Xid(read_elt_id(cur_tok)).add_ref(sT_arg_nopar());
        return true;
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
    case xthepage_cmd:
        flush_buffer();
        the_stack.add_last(the_page_xml);
        return true;
    case verb_cmd: T_verb(c != 0U ? verb_saved_char : char32_t(0U)); return true;
    case gloss_cmd: T_gloss(c == 0); return true;
    case only_preamble_cmd:
        get_r_token(true);
        onlypreamble.push_back(hash_table.let_token);
        onlypreamble.push_back(cur_tok);
        onlypreamble.push_back(hash_table.notprerr_token);
        return true;
    case loadlatex3_cmd: L3_load(false); return true;
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
    case end_keywords_cmd: the_stack.pop(the_names["keywords"]); return true;
    case center_cmd:
        leave_h_mode();     // finish the possibly not-centered paragraph
        the_stack.add_nl(); // needed ?
        word_define(incentering_code, c, false);
        return true;
    case end_center_cmd:
        leave_h_mode(); // finish centered paragraph
        return true;
    case figure_cmd: T_figure_table(x, c); return true;
    case table_cmd: T_figure_table(x, c); return true;
    case end_figure_cmd: T_figure_table_end(true); return true;
    case end_table_cmd: T_figure_table_end(false); return true;
    case solvecite_cmd: solve_cite(false); return true;
    case footcitepre_cmd: unprocessed_xml.push_back_unless_punct(' '); return true;
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
    case ignore_env_cmd: return true;
    case ignore_content_cmd: T_raw_env(false); return true;
    case raw_env_cmd: the_stack.add_last(new Xml(std::string(T_raw_env(true)))); return true;
    case math_env_cmd:
        cur_tok.kill();
        pop_level(bt_env); // IS THIS OK ?
        T_math(c);
        return true;
    case popmodule_cmd: the_stack.end_module(); return true;
    case end_picture_env_cmd: the_stack.pop(the_names["picture"]); return true;
    case end_ignore_env_cmd: return true;
    case subequations_cmd: T_subequations(true); return true;
    case end_subequations_cmd: T_subequations(false); return true;
    case end_minipage_cmd:
        flush_buffer();
        the_stack.pop_if_frame(the_names["cst_p"]);
        the_stack.pop_if_frame(the_names["item"]);
        the_stack.pop(the_names["minipage"]);
        return true;
    case end_ignore_content_cmd: parse_error(cur_tok, "missing \\begin environment ", cur_tok.tok_to_str(), "missing begin"); return true;
    case end_raw_env_cmd: parse_error(cur_tok, "missing \\begin environment ", cur_tok.tok_to_str(), "missing begin"); return true;
    case end_math_env_cmd: parse_error(cur_tok, "missing \\begin environment ", cur_tok.tok_to_str(), "missing begin"); return true;
    case mathinner_cmd:
        if (math_loc(c) == vdots_code) {
            back_input(hash_table.dollar_token);
            back_input(cur_tok);
            back_input(hash_table.dollar_token);
            return true;
        }
        math_only();
        return true;
    case self_insert_cmd:
        LC();
        unprocessed_xml.push_back(cur_tok);
        return true;
    default: undefined_mac(); return true;
    }
}
