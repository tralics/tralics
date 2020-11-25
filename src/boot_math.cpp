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

    mk_accent("acute", "&acute;", "&#x000B4;", acute_code);
    mk_accent("grave", "&grave;", "&#x60;", grave_code);
    mk_accent("mathring", "&#x2DA;", "&#x2DA;", mathring_code);
    mk_accent("ddddot", "&#x20DC;", "&#x20DC;", ddddot_code);
    mk_accent("dddot", "&#x20DB;", "&#x20DB;", dddot_code);
    mk_accent("ddot", "&die;", "&#xA8;", ddot_code);
    mk_accent("tilde", "&tilde;", "&#x2DC;", tilde_code);
    mk_accent("widetilde", "&tilde;", "&#x2DC;", widetilde_code); // No wide....
    mk_accent("bar", "&OverBar;", "&#xAF;", bar_code);
    mk_accent("breve", "&breve;", "&#x2D8;", breve_code);
    mk_accent("check", "&Hacek;", "&#x2C7;", check_code);
    mk_accent("hat", "&Hat;", "&#x5E;", hat_code);
    mk_accent("widehat", "&Hat;", "&#x5E;", widehat_code); // No wide....
    mk_accent("vec", "&rightarrow;", "&#x02192;", vec_code);
    mk_accent("overrightarrow", "&rightarrow;", "&#x02192;", overrightarrow_code);
    mk_accent("overleftarrow", "&leftarrow;", "&#x02190;", overleftarrow_code);
    mk_accent("overleftrightarrow", "&leftrightarrow;", "&#x02194;", overleftrightarrow_code);
    mk_accent("underrightarrow", "&rightarrow;", "&#x02192;", underrightarrow_code);
    mk_accent("xrightarrow", "&rightarrow;", "&#x02192;", xrightarrow_code);
    mk_accent("underleftarrow", "&leftarrow;", "&#x02190;", underleftarrow_code);
    mk_accent("underleftrightarrow", "&leftrightarrow;", "&#x02194;", underleftrightarrow_code);
    mk_accent("xleftarrow", "&leftarrow;", "&#x02190;", xleftarrow_code);
    mk_accent("dot", "&dot;", "&#x002D9;", dot_code);

    hash_table.primitive_plain("@root", special_math_cmd, root_code);
    hash_table.primitive_plain("accentset", special_math_cmd, accentset_code);
    hash_table.primitive_plain("binom", special_math_cmd, binom_code);
    hash_table.primitive_plain("boxed", special_math_cmd, boxed_code);
    hash_table.primitive_plain("cellattribute", special_math_cmd, cell_attribute_code);
    hash_table.primitive_plain("cfrac", special_math_cmd, cfrac_code);
    hash_table.primitive_plain("dbinom", special_math_cmd, dbinom_code);
    hash_table.primitive_plain("dfrac", special_math_cmd, dfrac_code);
    hash_table.primitive_plain("formulaattribute", special_math_cmd, formula_attribute_code);
    hash_table.primitive_plain("frac", special_math_cmd, frac_code);
    hash_table.primitive_plain("genfrac", special_math_cmd, genfrac_code);
    hash_table.primitive_plain("hphantom", special_math_cmd, hphantom_code);
    hash_table.primitive_plain("mathattribute", special_math_cmd, math_attribute_code);
    hash_table.primitive_plain("mathbox", special_math_cmd, mathbox_code);
    hash_table.primitive_plain("mathchoice", special_math_cmd, mathchoice_code);
    hash_table.primitive_plain("mathci", special_math_cmd, mathci_code);
    hash_table.primitive_plain("mathcn", special_math_cmd, mathcn_code);
    hash_table.primitive_plain("mathcnothing", special_math_cmd, mathnothing_code);
    hash_table.primitive_plain("mathcsymbol", special_math_cmd, mathcsymbol_code);
    hash_table.primitive_plain("mathmi", special_math_cmd, mathmi_code);
    hash_table.primitive_plain("mathmn", special_math_cmd, mathmn_code);
    hash_table.primitive_plain("mathmo", special_math_cmd, mathmo_code);
    hash_table.primitive_plain("mmlmultiscripts", special_math_cmd, multiscripts_code);
    hash_table.primitive_plain("operatorname", special_math_cmd, operatorname_code);
    hash_table.primitive_plain("operatornamewithlimits", special_math_cmd, operatornamestar_code);
    hash_table.primitive_plain("overbrace", special_math_cmd, overbrace_code);
    hash_table.primitive_plain("overline", special_math_cmd, overline_code);
    hash_table.primitive_plain("overset", special_math_cmd, overset_code);
    hash_table.primitive_plain("phantom", special_math_cmd, phantom_code);
    hash_table.primitive_plain("qopname", special_math_cmd, qopname_code);
    hash_table.primitive_plain("rowattribute", special_math_cmd, row_attribute_code);
    hash_table.primitive_plain("smash", special_math_cmd, smash_code);
    hash_table.primitive_plain("sqrt", special_math_cmd, sqrt_code);
    hash_table.primitive_plain("stackrel", special_math_cmd, stackrel_code);
    hash_table.primitive_plain("tableattribute", special_math_cmd, table_attribute_code);
    hash_table.primitive_plain("tbinom", special_math_cmd, tbinom_code);
    hash_table.primitive_plain("tfrac", special_math_cmd, tfrac_code);
    hash_table.primitive_plain("thismathattribute", special_math_cmd, thismath_attribute_code);
    hash_table.primitive_plain("underaccent", special_math_cmd, underaccent_code);
    hash_table.primitive_plain("underbrace", special_math_cmd, underbrace_code);
    hash_table.primitive_plain("underline", special_math_cmd, underline_code);
    hash_table.primitive_plain("underset", special_math_cmd, underset_code);
    hash_table.primitive_plain("undertilde", special_math_cmd, undertilde_code);
    hash_table.primitive_plain("vphantom", special_math_cmd, vphantom_code);
    Symcode::get(special_math_cmd).name_sub[mathlabel_code]   = "anchorlabel";
    Symcode::get(special_math_cmd).name_sub[multicolumn_code] = "multicolumn";
    Symcode::get(special_math_cmd).name_sub[tfrac_code]       = "tfrac";
    register_action_plain(special_math_cmd, special_math);

    hash_table.primitive_plain("tag", tag_cmd);
    hash_table.primitive_plain("@xtag", tag_cmd, one_code);
    hash_table.primitive_plain("@ytag", tag_cmd, two_code);
    hash_table.primitive_plain("notag", tag_cmd, subtypes(3));
    hash_table.primitive_plain("nonumber", tag_cmd, subtypes(4));
    register_action_plain(tag_cmd, math_only);
}
