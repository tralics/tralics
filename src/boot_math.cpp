#include "tralics/Dispatcher.h"
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
    register_action_plain(eqno_cmd, math_only);
    register_action_plain(left_cmd, math_only);
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
    register_action_plain(right_cmd, math_only);
    register_action_plain(special_math_cmd, special_math);
    register_action_plain(tag_cmd, math_only);
}
