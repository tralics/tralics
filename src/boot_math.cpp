#include "tralics/Dispatcher.h"
#include "tralics/MainClass.h"
#include "tralics/MathDataP.h"
#include "tralics/Symcode.h"
#include <spdlog/spdlog.h>

[[nodiscard]] auto token_math_name(subtypes c) -> std::string;

namespace {
    void mk_accent(const std::string &name, const std::string &ent, const std::string &hex, subtypes pos) {
        auto *x = new Xml(the_names["mo"], new Xml(the_main.no_entnames ? hex : ent));
        math_data.init_builtin(math_loc(pos), x);
        hash_table.primitive_plain(name, special_math_cmd, pos);
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
    hash_table.primitive_and_action_plain("@root", special_math, special_math_cmd, root_code);
    hash_table.primitive_and_action_plain("@xtag", math_only, tag_cmd, one_code);
    hash_table.primitive_and_action_plain("@ytag", math_only, tag_cmd, two_code);
    hash_table.primitive_and_action_plain("accentset", special_math, special_math_cmd, accentset_code);
    hash_table.primitive_and_action_plain("binom", special_math, special_math_cmd, binom_code);
    hash_table.primitive_and_action_plain("boxed", special_math, special_math_cmd, boxed_code);
    hash_table.primitive_and_action_plain("cal", math_only, math_font_cmd, cal_code);
    hash_table.primitive_and_action_plain("cellattribute", special_math, special_math_cmd, cell_attribute_code);
    hash_table.primitive_and_action_plain("cfrac", special_math, special_math_cmd, cfrac_code);
    hash_table.primitive_and_action_plain("dbinom", special_math, special_math_cmd, dbinom_code);
    hash_table.primitive_and_action_plain("dfrac", special_math, special_math_cmd, dfrac_code);
    hash_table.primitive_and_action_plain("eqno", math_only, eqno_cmd, eqno_code);
    hash_table.primitive_and_action_plain("formulaattribute", special_math, special_math_cmd, formula_attribute_code);
    hash_table.primitive_and_action_plain("frac", special_math, special_math_cmd, frac_code);
    hash_table.primitive_and_action_plain("genfrac", special_math, special_math_cmd, genfrac_code);
    hash_table.primitive_and_action_plain("hphantom", special_math, special_math_cmd, hphantom_code);
    hash_table.primitive_and_action_plain("left", math_only, left_cmd);
    hash_table.primitive_and_action_plain("leqno", math_only, eqno_cmd, leqno_code);
    hash_table.primitive_and_action_plain("mathattribute", special_math, special_math_cmd, math_attribute_code);
    hash_table.primitive_and_action_plain("mathbb", math_only, math_font_cmd, mathbb_code);
    hash_table.primitive_and_action_plain("mathbf", math_only, math_font_cmd, mathbf_code);
    hash_table.primitive_and_action_plain("mathbox", special_math, special_math_cmd, mathbox_code);
    hash_table.primitive_and_action_plain("mathcal", math_only, math_font_cmd, mathcal_code);
    hash_table.primitive_and_action_plain("mathchoice", special_math, special_math_cmd, mathchoice_code);
    hash_table.primitive_and_action_plain("mathci", special_math, special_math_cmd, mathci_code);
    hash_table.primitive_and_action_plain("mathcn", special_math, special_math_cmd, mathcn_code);
    hash_table.primitive_and_action_plain("mathcnothing", special_math, special_math_cmd, mathnothing_code);
    hash_table.primitive_and_action_plain("mathcsymbol", special_math, special_math_cmd, mathcsymbol_code);
    hash_table.primitive_and_action_plain("mathfrak", math_only, math_font_cmd, mathfrak_code);
    hash_table.primitive_and_action_plain("mathit", math_only, math_font_cmd, mathit_code);
    hash_table.primitive_and_action_plain("mathmi", special_math, special_math_cmd, mathmi_code);
    hash_table.primitive_and_action_plain("mathmn", special_math, special_math_cmd, mathmn_code);
    hash_table.primitive_and_action_plain("mathmo", special_math, special_math_cmd, mathmo_code);
    hash_table.primitive_and_action_plain("mathnormal", math_only, math_font_cmd, mathnormal_code);
    hash_table.primitive_and_action_plain("mathrm", math_only, math_font_cmd, mathrm_code);
    hash_table.primitive_and_action_plain("mathsf", math_only, math_font_cmd, mathsf_code);
    hash_table.primitive_and_action_plain("mathtt", math_only, math_font_cmd, mathtt_code);
    hash_table.primitive_and_action_plain("mml@font@bold", math_only, mathfont_cmd, math_f_bold);
    hash_table.primitive_and_action_plain("mml@font@boldfraktur", math_only, mathfont_cmd, math_f_bold_fraktur);
    hash_table.primitive_and_action_plain("mml@font@bolditalic", math_only, mathfont_cmd, math_f_bold_italic);
    hash_table.primitive_and_action_plain("mml@font@boldsansserif", math_only, mathfont_cmd, math_f_bold_sansserif);
    hash_table.primitive_and_action_plain("mml@font@boldscript", math_only, mathfont_cmd, math_f_bold_script);
    hash_table.primitive_and_action_plain("mml@font@doublestruck", math_only, mathfont_cmd, math_f_doublestruck);
    hash_table.primitive_and_action_plain("mml@font@fraktur", math_only, mathfont_cmd, math_f_fraktur);
    hash_table.primitive_and_action_plain("mml@font@italic", math_only, mathfont_cmd, math_f_italic);
    hash_table.primitive_and_action_plain("mml@font@monospace", math_only, mathfont_cmd, math_f_monospace);
    hash_table.primitive_and_action_plain("mml@font@normal", math_only, mathfont_cmd, math_f_normal);
    hash_table.primitive_and_action_plain("mml@font@sansserif", math_only, mathfont_cmd, math_f_sansserif);
    hash_table.primitive_and_action_plain("mml@font@sansserifbolditalic", math_only, mathfont_cmd, math_f_sansserif_bold_italic);
    hash_table.primitive_and_action_plain("mml@font@sansserifitalic", math_only, mathfont_cmd, math_f_sansserif_italic);
    hash_table.primitive_and_action_plain("mml@font@script", math_only, mathfont_cmd, math_f_script);
    hash_table.primitive_and_action_plain("mml@font@upright", math_only, mathfont_cmd, math_f_upright);
    hash_table.primitive_and_action_plain("mmlmultiscripts", special_math, special_math_cmd, multiscripts_code);
    hash_table.primitive_and_action_plain("nonumber", math_only, tag_cmd, subtypes(4));
    hash_table.primitive_and_action_plain("notag", math_only, tag_cmd, subtypes(3));
    hash_table.primitive_and_action_plain("operatorname", special_math, special_math_cmd, operatorname_code);
    hash_table.primitive_and_action_plain("operatornamewithlimits", special_math, special_math_cmd, operatornamestar_code);
    hash_table.primitive_and_action_plain("overbrace", special_math, special_math_cmd, overbrace_code);
    hash_table.primitive_and_action_plain("overline", special_math, special_math_cmd, overline_code);
    hash_table.primitive_and_action_plain("overset", special_math, special_math_cmd, overset_code);
    hash_table.primitive_and_action_plain("phantom", special_math, special_math_cmd, phantom_code);
    hash_table.primitive_and_action_plain("qopname", special_math, special_math_cmd, qopname_code);
    hash_table.primitive_and_action_plain("right", math_only, right_cmd);
    hash_table.primitive_and_action_plain("rowattribute", special_math, special_math_cmd, row_attribute_code);
    hash_table.primitive_and_action_plain("smash", special_math, special_math_cmd, smash_code);
    hash_table.primitive_and_action_plain("sqrt", special_math, special_math_cmd, sqrt_code);
    hash_table.primitive_and_action_plain("stackrel", special_math, special_math_cmd, stackrel_code);
    hash_table.primitive_and_action_plain("tableattribute", special_math, special_math_cmd, table_attribute_code);
    hash_table.primitive_and_action_plain("tag", math_only, tag_cmd);
    hash_table.primitive_and_action_plain("tbinom", special_math, special_math_cmd, tbinom_code);
    hash_table.primitive_and_action_plain("tfrac", special_math, special_math_cmd, tfrac_code);
    hash_table.primitive_and_action_plain("thismathattribute", special_math, special_math_cmd, thismath_attribute_code);
    hash_table.primitive_and_action_plain("underaccent", special_math, special_math_cmd, underaccent_code);
    hash_table.primitive_and_action_plain("underbrace", special_math, special_math_cmd, underbrace_code);
    hash_table.primitive_and_action_plain("underline", special_math, special_math_cmd, underline_code);
    hash_table.primitive_and_action_plain("underset", special_math, special_math_cmd, underset_code);
    hash_table.primitive_and_action_plain("undertilde", special_math, special_math_cmd, undertilde_code);
    hash_table.primitive_and_action_plain("vphantom", special_math, special_math_cmd, vphantom_code);

    mk_accent("acute", "&acute;", "&#x000B4;", acute_code);
    mk_accent("bar", "&OverBar;", "&#xAF;", bar_code);
    mk_accent("breve", "&breve;", "&#x2D8;", breve_code);
    mk_accent("check", "&Hacek;", "&#x2C7;", check_code);
    mk_accent("ddddot", "&#x20DC;", "&#x20DC;", ddddot_code);
    mk_accent("dddot", "&#x20DB;", "&#x20DB;", dddot_code);
    mk_accent("ddot", "&die;", "&#xA8;", ddot_code);
    mk_accent("dot", "&dot;", "&#x002D9;", dot_code);
    mk_accent("grave", "&grave;", "&#x60;", grave_code);
    mk_accent("hat", "&Hat;", "&#x5E;", hat_code);
    mk_accent("mathring", "&#x2DA;", "&#x2DA;", mathring_code);
    mk_accent("overleftarrow", "&leftarrow;", "&#x02190;", overleftarrow_code);
    mk_accent("overleftrightarrow", "&leftrightarrow;", "&#x02194;", overleftrightarrow_code);
    mk_accent("overrightarrow", "&rightarrow;", "&#x02192;", overrightarrow_code);
    mk_accent("tilde", "&tilde;", "&#x2DC;", tilde_code);
    mk_accent("underleftarrow", "&leftarrow;", "&#x02190;", underleftarrow_code);
    mk_accent("underleftrightarrow", "&leftrightarrow;", "&#x02194;", underleftrightarrow_code);
    mk_accent("underrightarrow", "&rightarrow;", "&#x02192;", underrightarrow_code);
    mk_accent("vec", "&rightarrow;", "&#x02192;", vec_code);
    mk_accent("widehat", "&Hat;", "&#x5E;", widehat_code);        // No wide....
    mk_accent("widetilde", "&tilde;", "&#x2DC;", widetilde_code); // No wide....
    mk_accent("xleftarrow", "&leftarrow;", "&#x02190;", xleftarrow_code);
    mk_accent("xrightarrow", "&rightarrow;", "&#x02192;", xrightarrow_code);

    register_action_plain(math_list_cmd, math_only);
    register_action_plain(math_xml_cmd, math_only);
    register_action_plain(mathbetween_cmd, math_only);
    register_action_plain(mathbin_cmd, math_only);
    register_action_plain(mathclose_cmd, math_only);
    register_action_plain(mathinner_cmd, mathinner);
    register_action_plain(mathop_cmd, math_only);
    register_action_plain(mathopen_cmd, math_only);
    register_action_plain(mathopn_cmd, math_only);
    register_action_plain(mathord_cmd, math_only);
    register_action_plain(mathordb_cmd, math_only);
    register_action_plain(mathrel_cmd, math_only);
    register_action_plain(mathspace_cmd, math_only);

    Symcode::get(mathbetween_cmd).name_fun = token_math_name;
    Symcode::get(mathbin_cmd).name_fun     = token_math_name;
    Symcode::get(mathclose_cmd).name_fun   = token_math_name;
    Symcode::get(mathinner_cmd).name_fun   = token_math_name;
    Symcode::get(mathop_cmd).name_fun      = token_math_name;
    Symcode::get(mathopen_cmd).name_fun    = token_math_name;
    Symcode::get(mathopn_cmd).name_fun     = token_math_name;
    Symcode::get(mathord_cmd).name_fun     = token_math_name;
    Symcode::get(mathordb_cmd).name_fun    = token_math_name;
    Symcode::get(mathrel_cmd).name_fun     = token_math_name;
    Symcode::get(mathspace_cmd).name_fun   = token_math_name;

    Symcode::get(special_math_cmd).name_sub[mathlabel_code]   = "anchorlabel";
    Symcode::get(special_math_cmd).name_sub[multicolumn_code] = "multicolumn";
    Symcode::get(special_math_cmd).name_sub[tfrac_code]       = "tfrac";
}
