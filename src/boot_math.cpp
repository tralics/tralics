#include "tralics/Dispatcher.h"
#include <spdlog/spdlog.h>

void Parser::math_only() {
    if (is_pos_par(nomath_code)) {
        LC();
        unprocessed_xml << cur_tok;
        return;
    }
    parse_error(cur_tok, "Math only command ", cur_tok, "", "Mathonly command");
    static bool first_time = true;
    if (first_time) {
        spdlog::warn("(Contrarily to TeX, Tralics does not switch to math mode in such a case.)");
        first_time = false;
    }
}

namespace {
    void mathinner(subtypes c) {
        if (math_loc(c) == vdots_code) {
            the_parser.back_input(hash_table.dollar_token);
            the_parser.back_input(the_parser.cur_tok);
            the_parser.back_input(hash_table.dollar_token);
        } else
            the_parser.math_only();
    }

    void special_math(subtypes c) {
        if (c == overline_code || c == underline_code)
            the_parser.T_fonts(c == overline_code ? "overline" : "underline");
        else
            the_parser.math_only();
    }
} // namespace

void Dispatcher::boot_math() {
    register_action_plain(eqno_cmd, &Parser::math_only);
    register_action_plain(left_cmd, &Parser::math_only);
    register_action_plain(math_font_cmd, &Parser::math_only);
    register_action_plain(math_list_cmd, &Parser::math_only);
    register_action_plain(math_xml_cmd, &Parser::math_only);
    register_action_plain(mathbetween_cmd, &Parser::math_only);
    register_action_plain(mathbin_cmd, &Parser::math_only);
    register_action_plain(mathclose_cmd, &Parser::math_only);
    register_action_plain(mathfont_cmd, &Parser::math_only);
    register_action_plain(mathinner_cmd, mathinner);
    register_action_plain(mathop_cmd, &Parser::math_only);
    register_action_plain(mathopen_cmd, &Parser::math_only);
    register_action_plain(mathopn_cmd, &Parser::math_only);
    register_action_plain(mathord_cmd, &Parser::math_only);
    register_action_plain(mathordb_cmd, &Parser::math_only);
    register_action_plain(mathrel_cmd, &Parser::math_only);
    register_action_plain(mathspace_cmd, &Parser::math_only);
    register_action_plain(right_cmd, &Parser::math_only);
    register_action_plain(special_math_cmd, special_math);
    register_action_plain(tag_cmd, &Parser::math_only);
}
