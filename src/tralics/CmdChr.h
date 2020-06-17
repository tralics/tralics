#pragma once
#include "codepoint.h"
#include "enums.h"

// cmd, chr pair; The subtype can have 16bits
class CmdChr {
public:
    symcodes cmd{undef_cmd};
    subtypes chr{zero_code};

    CmdChr(symcodes a, subtypes b) : cmd(a), chr(b) {}
    explicit CmdChr(uchar b) : cmd(other_catcode), chr(subtypes(b)) {}
    CmdChr() = default;

    void set(symcodes A, subtypes B) {
        cmd = A;
        chr = B;
    }

    void reset() { set(undef_cmd, zero_code); }
    void kill() { set(invalid_cmd, zero_code); }
    void set_mathml() { cmd = math_xml_cmd; }

    [[nodiscard]] auto char_val() const -> codepoint { return codepoint(unsigned(chr)); }
    [[nodiscard]] auto is_letter() const -> bool { return cmd == letter_catcode; }
    [[nodiscard]] auto is_digit() const -> bool { return '0' <= chr && chr <= '9'; }
    [[nodiscard]] auto is_other() const -> bool { return cmd == other_catcode; }
    [[nodiscard]] auto is_space() const -> bool { return cmd == space_catcode; }
    [[nodiscard]] auto is_letter_other() const -> bool { return cmd == other_catcode || cmd == letter_catcode; }
    [[nodiscard]] auto is_relax() const -> bool { return cmd == relax_cmd; }
    [[nodiscard]] auto is_undef() const -> bool { return cmd == undef_cmd; }
    [[nodiscard]] auto is_undef_or_relax() const -> bool { return cmd == undef_cmd || cmd == relax_cmd; }
    [[nodiscard]] auto is_outer() const -> bool { return cmd >= usero_cmd; }
    [[nodiscard]] auto is_open_brace() const -> bool { return cmd == open_catcode; }
    [[nodiscard]] auto is_parameter() const -> bool { return cmd == parameter_catcode; }
    [[nodiscard]] auto is_minus_sign() const -> bool { return cmd == other_catcode && chr == '-'; }
    [[nodiscard]] auto is_user() const -> bool { return cmd >= user_cmd; }
    [[nodiscard]] auto is_cst1_cmd() const -> bool { return cmd == cst1_cmd; }
    [[nodiscard]] auto is_math_font() const -> bool { return cmd == math_font_cmd; }
    [[nodiscard]] auto is_math_list() const -> bool { return cmd == math_list_cmd; }
    [[nodiscard]] auto is_mathml() const -> bool { return cmd == math_xml_cmd; }
    [[nodiscard]] auto is_expandable() const -> bool { return cmd >= max_command; }
    [[nodiscard]] auto is_ok_for_the() const -> bool { return cmd > min_internal && cmd < max_internal; }
    [[nodiscard]] auto is_ok_for_xspace() const -> bool;
    [[nodiscard]] auto is_protected() const -> bool {
        return cmd == userp_cmd || cmd == userlp_cmd || cmd == userpo_cmd || cmd == userlpo_cmd;
    }
    [[nodiscard]] auto val_as_digit() const -> unsigned { return chr - '0'; }
    [[nodiscard]] auto is_math_openclosebetween() const -> bool {
        return cmd == mathbetween_cmd || cmd == mathopen_cmd || cmd == mathclose_cmd;
    }
    [[nodiscard]] auto special_name() const -> String;
    [[nodiscard]] auto name() const -> std::string;
    [[nodiscard]] auto is_single_quote() const -> bool { return cmd == other_catcode && chr == '\''; }
    [[nodiscard]] auto token_fiorelse_name() const -> String;

private:
    [[nodiscard]] auto cat_ifeq_name() const -> String;
    [[nodiscard]] auto cmd_special_name() const -> String;
    [[nodiscard]] auto l3_expand_aux_name() const -> String;
    [[nodiscard]] auto l3_expand_base_name() const -> String;
    [[nodiscard]] auto l3_ifx_name() const -> String;
    [[nodiscard]] auto l3_rescan_name() const -> String;
    [[nodiscard]] auto l3_set_cat_name() const -> String;
    [[nodiscard]] auto l3_set_num_name() const -> String;
    [[nodiscard]] auto l3_tl_basic_name() const -> String;
    [[nodiscard]] auto l3str_case_name() const -> String;
    [[nodiscard]] auto l3str_ifeq_name() const -> String;
    [[nodiscard]] auto specchar_cmd_name() const -> std::string;
    [[nodiscard]] auto tipa_name() const -> String;
    [[nodiscard]] auto tl_concat_name() const -> String;
    [[nodiscard]] auto tl_put_left_name() const -> String;
    [[nodiscard]] auto tl_set_name() const -> String;
    [[nodiscard]] auto token_accent_name() const -> String;
    [[nodiscard]] auto token_argfont_name() const -> String;
    [[nodiscard]] auto token_assigndimen_name() const -> std::string;
    [[nodiscard]] auto token_assignglue_name() const -> std::string;
    [[nodiscard]] auto token_assignint_name() const -> std::string;
    [[nodiscard]] auto token_assignmuglue_name() const -> std::string;
    [[nodiscard]] auto token_assigntoks_name() const -> std::string;
    [[nodiscard]] auto token_big_name() const -> String;
    [[nodiscard]] auto token_box_name() const -> String;
    [[nodiscard]] auto token_caseshift_name() const -> String;
    [[nodiscard]] auto token_centering_name() const -> String;
    [[nodiscard]] auto token_cite_name() const -> String;
    [[nodiscard]] auto token_color_name() const -> String;
    [[nodiscard]] auto token_convert_name() const -> String;
    [[nodiscard]] auto token_counter_name() const -> String;
    [[nodiscard]] auto token_cst_name() const -> std::string;
    [[nodiscard]] auto token_dashline_name() const -> String;
    [[nodiscard]] auto token_def_name() const -> String;
    [[nodiscard]] auto token_defcode_name() const -> String;
    [[nodiscard]] auto token_deffamily_name() const -> String;
    [[nodiscard]] auto token_ecenter_name() const -> String;
    [[nodiscard]] auto token_efigure_name() const -> String;
    [[nodiscard]] auto token_eignore_name() const -> String;
    [[nodiscard]] auto token_eignorec_name() const -> String;
    [[nodiscard]] auto token_emath_name() const -> String;
    [[nodiscard]] auto token_eparticipant_name() const -> String;
    [[nodiscard]] auto token_error_name() const -> String;
    [[nodiscard]] auto token_extension_name() const -> String;
    [[nodiscard]] auto token_fancy_name() const -> String;
    [[nodiscard]] auto token_fbox_name() const -> String;
    [[nodiscard]] auto token_file_name() const -> String;
    [[nodiscard]] auto token_fontsize_name() const -> String;
    [[nodiscard]] auto token_for_name() const -> String;
    [[nodiscard]] auto token_fp_names() const -> String;
    [[nodiscard]] auto token_fpi_names() const -> String;
    [[nodiscard]] auto token_if_name() const -> String;
    [[nodiscard]] auto token_iftest_name() const -> String;
    [[nodiscard]] auto token_ign1_name() const -> String;
    [[nodiscard]] auto token_ign2_name() const -> String;
    [[nodiscard]] auto token_ignore_name() const -> String;
    [[nodiscard]] auto token_index_name() const -> String;
    [[nodiscard]] auto token_lastitem_name() const -> String;
    [[nodiscard]] auto token_latexctr_name() const -> String;
    [[nodiscard]] auto token_leader_name() const -> String;
    [[nodiscard]] auto token_limits_name() const -> String;
    [[nodiscard]] auto token_line_name() const -> String;
    [[nodiscard]] auto token_linebreak_name() const -> String;
    [[nodiscard]] auto token_ltfont_name() const -> String;
    [[nodiscard]] auto token_makebox_name() const -> String;
    [[nodiscard]] auto token_mark_name() const -> String;
    [[nodiscard]] auto token_math_name() const -> String;
    [[nodiscard]] auto token_mathcomp_name() const -> String;
    [[nodiscard]] auto token_mathfont1_name() const -> String;
    [[nodiscard]] auto token_mathfont2_name() const -> String;
    [[nodiscard]] auto token_monthday_name() const -> String;
    [[nodiscard]] auto token_move_name() const -> String;
    [[nodiscard]] auto token_newcount_name() const -> String;
    [[nodiscard]] auto token_noargfont_name() const -> String;
    [[nodiscard]] auto token_oldfont_name() const -> String;
    [[nodiscard]] auto token_over_name() const -> String;
    [[nodiscard]] auto token_prefix_name() const -> String;
    [[nodiscard]] auto token_put_name() const -> String;
    [[nodiscard]] auto token_register_name() const -> String;
    [[nodiscard]] auto token_section_name() const -> String;
    [[nodiscard]] auto token_setboxdimen_name() const -> String;
    [[nodiscard]] auto token_setinteraction_name() const -> String;
    [[nodiscard]] auto token_setpagedimen_name() const -> String;
    [[nodiscard]] auto token_shape_name() const -> String;
    [[nodiscard]] auto token_shorthand_name() const -> String;
    [[nodiscard]] auto token_soul_name() const -> String;
    [[nodiscard]] auto token_specialmath_name() const -> String;
    [[nodiscard]] auto token_style_name() const -> String;
    [[nodiscard]] auto token_trees_name() const -> String;
    [[nodiscard]] auto token_unbox_name() const -> String;
    [[nodiscard]] auto token_unimp_font_name() const -> String;
    [[nodiscard]] auto token_unimp_name() const -> String;
    [[nodiscard]] auto token_xkeyval_name() const -> String;
    [[nodiscard]] auto token_xray_name() const -> String;
};
