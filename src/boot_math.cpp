#include "tralics/Dispatcher.h"
#include "tralics/Symcode.h"
#include <spdlog/spdlog.h>

[[nodiscard]] auto token_math_name(subtypes c) -> std::string;

namespace {
    auto token_specialmath_name(subtypes chr) -> std::string {
        switch (chr) {
        case acute_code: return "acute";
        case grave_code: return "grave";
        case ddddot_code: return "ddddot";
        case mathring_code: return "mathring";
        case dddot_code: return "dddot";
        case ddot_code: return "ddot";
        case tilde_code: return "tilde";
        case widetilde_code: return "widetilde";
        case bar_code: return "bar";
        case breve_code: return "breve";
        case check_code: return "check";
        case hat_code: return "hat";
        case widehat_code: return "widehat";
        case vec_code: return "vec";
        case overrightarrow_code: return "overrightarrow";
        case overleftarrow_code: return "overleftarrow";
        case overleftrightarrow_code: return "overleftrightarrow";
        case underrightarrow_code: return "underrightarrow";
        case underleftarrow_code: return "underleftarrow";
        case underleftrightarrow_code: return "underleftrightarrow";
        case xleftarrow_code: return "xleftarrow";
        case xrightarrow_code: return "xrightarrow";
        case phantom_code: return "phantom";
        case vphantom_code: return "vphantom";
        case hphantom_code: return "hphantom";
        case smash_code: return "smash";
        case atsmash_code: return "@smash";
        case accentset_code: return "accentset";
        case underaccent_code: return "underaccent";
        case undertilde_code: return "undertilde";
        case dot_code: return "dot";
        case qopname_code: return "qopname";
        case operatorname_code: return "operatorname";
        case operatornamestar_code: return "operatornamewithlimits";
        case overline_code: return "overline";
        case underline_code: return "underline";
        case stackrel_code: return "stackrel";
        case underset_code: return "underset";
        case overset_code: return "overset";
        case mathchoice_code: return "mathchoice";
        case multicolumn_code: return "multicolumn";
        case frac_code: return "frac";
        case overbrace_code: return "overbrace";
        case underbrace_code: return "underbrace";
        case dfrac_code: return "dfrac";
        case tfrac_code: return "tfrac";
        case cfrac_code: return "cfrac";
        case binom_code: return "binom";
        case dbinom_code: return "dbinom";
        case tbinom_code: return "tbinom";
        case boxed_code: return "boxed";
        case sqrt_code: return "sqrt";
        case root_code: return "@root";
        case mathbox_code: return "mathbox";
        case genfrac_code: return "genfrac";
        case mathlabel_code: return "anchorlabel";
        case mathmi_code: return "mathmi";
        case mathmo_code: return "mathmo";
        case mathmn_code: return "mathmn";
        case mathci_code: return "mathco";
        case mathcn_code: return "mathcn";
        case mathcsymbol_code: return "mathcsymbol";
        case math_attribute_code: return "mathattribute";
        case multiscripts_code: return "mmlmultiscripts";
        case cell_attribute_code: return "cellattribute";
        case row_attribute_code: return "rowattribute";
        case thismath_attribute_code: return "thismathattribute";
        case formula_attribute_code: return "formulaattribute";
        case table_attribute_code: return "tableattribute";
        default: return "";
        }
    }

    void math_only() {
        if (!is_pos_par(nomath_code)) {
            the_parser.parse_error(the_parser.cur_tok, "Math only command ", the_parser.cur_tok, "", "Mathonly command");

            static bool first_time = true;
            if (first_time) {
                spdlog::warn("(Contrarily to TeX, Tralics does not switch to math mode in such a case.)");
                first_time = false;
            }
            return;
        }

        the_parser.LC();
        the_parser.unprocessed_xml << the_parser.cur_tok;
    }

    void mathinner(subtypes c) {
        if (math_loc(c) == vdots_code) {
            the_parser.back_input(hash_table.dollar_token);
            the_parser.back_input(the_parser.cur_tok);
            the_parser.back_input(hash_table.dollar_token);
        } else
            math_only();
    }

    void special_math(subtypes c) {
        if (c == overline_code || c == underline_code)
            the_parser.T_fonts(c == overline_code ? "overline" : "underline");
        else
            math_only();
    }
} // namespace

void Dispatcher::boot_math() {
    hash_table.primitive_plain("eqno", eqno_cmd, eqno_code);
    hash_table.primitive_plain("leqno", eqno_cmd, leqno_code);
    register_action_plain(eqno_cmd, math_only);

    hash_table.primitive_plain("left", left_cmd);
    register_action_plain(left_cmd, math_only);

    hash_table.primitive_plain("right", right_cmd);
    register_action_plain(right_cmd, math_only);

    hash_table.primitive_plain("cal", math_font_cmd, cal_code);
    hash_table.primitive_plain("mathtt", math_font_cmd, mathtt_code);
    hash_table.primitive_plain("mathcal", math_font_cmd, mathcal_code);
    hash_table.primitive_plain("mathbf", math_font_cmd, mathbf_code);
    hash_table.primitive_plain("mathrm", math_font_cmd, mathrm_code);
    hash_table.primitive_plain("mathit", math_font_cmd, mathit_code);
    hash_table.primitive_plain("mathbb", math_font_cmd, mathbb_code);
    hash_table.primitive_plain("mathsf", math_font_cmd, mathsf_code);
    hash_table.primitive_plain("mathfrak", math_font_cmd, mathfrak_code);
    hash_table.primitive_plain("mathnormal", math_font_cmd, mathnormal_code);
    register_action_plain(math_font_cmd, math_only);

    register_action_plain(math_list_cmd, math_only);
    register_action_plain(math_xml_cmd, math_only);

    Symcode::get(mathbetween_cmd).name_fn = token_math_name;
    register_action_plain(mathbetween_cmd, math_only);

    Symcode::get(mathbin_cmd).name_fn = token_math_name;
    register_action_plain(mathbin_cmd, math_only);

    Symcode::get(mathclose_cmd).name_fn = token_math_name;
    register_action_plain(mathclose_cmd, math_only);

    Symcode::get(mathinner_cmd).name_fn = token_math_name;
    register_action_plain(mathinner_cmd, mathinner);

    Symcode::get(mathop_cmd).name_fn = token_math_name;
    register_action_plain(mathop_cmd, math_only);

    hash_table.primitive_plain("mml@font@normal", mathfont_cmd, math_f_normal);
    hash_table.primitive_plain("mml@font@upright", mathfont_cmd, math_f_upright);
    hash_table.primitive_plain("mml@font@bold", mathfont_cmd, math_f_bold);
    hash_table.primitive_plain("mml@font@italic", mathfont_cmd, math_f_italic);
    hash_table.primitive_plain("mml@font@bolditalic", mathfont_cmd, math_f_bold_italic);
    hash_table.primitive_plain("mml@font@script", mathfont_cmd, math_f_script);
    hash_table.primitive_plain("mml@font@boldscript", mathfont_cmd, math_f_bold_script);
    hash_table.primitive_plain("mml@font@fraktur", mathfont_cmd, math_f_fraktur);
    hash_table.primitive_plain("mml@font@doublestruck", mathfont_cmd, math_f_doublestruck);
    hash_table.primitive_plain("mml@font@boldfraktur", mathfont_cmd, math_f_bold_fraktur);
    hash_table.primitive_plain("mml@font@sansserif", mathfont_cmd, math_f_sansserif);
    hash_table.primitive_plain("mml@font@boldsansserif", mathfont_cmd, math_f_bold_sansserif);
    hash_table.primitive_plain("mml@font@sansserifitalic", mathfont_cmd, math_f_sansserif_italic);
    hash_table.primitive_plain("mml@font@sansserifbolditalic", mathfont_cmd, math_f_sansserif_bold_italic);
    hash_table.primitive_plain("mml@font@monospace", mathfont_cmd, math_f_monospace);
    register_action_plain(mathfont_cmd, math_only);

    Symcode::get(mathopen_cmd).name_fn = token_math_name;
    register_action_plain(mathopen_cmd, math_only);

    Symcode::get(mathopn_cmd).name_fn = token_math_name;
    register_action_plain(mathopn_cmd, math_only);

    Symcode::get(mathord_cmd).name_fn = token_math_name;
    register_action_plain(mathord_cmd, math_only);

    Symcode::get(mathordb_cmd).name_fn = token_math_name;
    register_action_plain(mathordb_cmd, math_only);

    Symcode::get(mathrel_cmd).name_fn = token_math_name;
    register_action_plain(mathrel_cmd, math_only);

    Symcode::get(mathspace_cmd).name_fn = token_math_name;
    register_action_plain(mathspace_cmd, math_only);

    hash_table.primitive("mathattribute", special_math_cmd, math_attribute_code);
    hash_table.primitive("rowattribute", special_math_cmd, row_attribute_code);
    hash_table.primitive("cellattribute", special_math_cmd, cell_attribute_code);
    hash_table.primitive("formulaattribute", special_math_cmd, formula_attribute_code);
    hash_table.primitive("tableattribute", special_math_cmd, table_attribute_code);
    hash_table.primitive("thismathattribute", special_math_cmd, thismath_attribute_code);
    hash_table.primitive("mathmi", special_math_cmd, mathmi_code);
    hash_table.primitive("mathmn", special_math_cmd, mathmn_code);
    hash_table.primitive("mathmo", special_math_cmd, mathmo_code);
    hash_table.primitive("mathci", special_math_cmd, mathci_code);
    hash_table.primitive("mathcn", special_math_cmd, mathcn_code);
    hash_table.primitive("mathcsymbol", special_math_cmd, mathcsymbol_code);
    hash_table.primitive("mmlmultiscripts", special_math_cmd, multiscripts_code);
    hash_table.primitive("mathcnothing", special_math_cmd, mathnothing_code);
    Symcode::get(special_math_cmd).name_fn = token_specialmath_name;
    register_action_plain(special_math_cmd, special_math);

    register_action_plain(tag_cmd, math_only);
}
