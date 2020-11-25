#include "tralics/Dispatcher.h"
#include "tralics/Symcode.h"
#include <spdlog/spdlog.h>

namespace {
    void math_only() {
        if (is_pos_par(nomath_code)) {
            the_parser.LC();
            the_parser.unprocessed_xml << the_parser.cur_tok;
            return;
        }
        the_parser.parse_error(the_parser.cur_tok, "Math only command ", the_parser.cur_tok, "", "Mathonly command");

        static bool first_time = true;
        if (first_time) {
            spdlog::warn("(Contrarily to TeX, Tralics does not switch to math mode in such a case.)");
            first_time = false;
        }
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
    register_action_plain(mathbetween_cmd, math_only);
    register_action_plain(mathbin_cmd, math_only);
    register_action_plain(mathclose_cmd, math_only);
    register_action_plain(mathfont_cmd, math_only);
    register_action_plain(mathinner_cmd, mathinner);
    register_action_plain(mathop_cmd, math_only);
    register_action_plain(mathopen_cmd, math_only);
    register_action_plain(mathopn_cmd, math_only);
    register_action_plain(mathord_cmd, math_only);
    register_action_plain(mathordb_cmd, math_only);
    register_action_plain(mathrel_cmd, math_only);
    register_action_plain(mathspace_cmd, math_only);
    register_action_plain(special_math_cmd, special_math);
    register_action_plain(tag_cmd, math_only);
}
