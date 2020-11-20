#include "tralics/Dispatcher.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"
#include "tralics/types.h"

// \todo make a hash table of methods instead of this huge mess, actions below is a proof of concept.

[[nodiscard]] auto Parser::translate03() -> bool {
    auto guard  = SaveErrTok(cur_tok);
    auto [x, c] = cur_cmd_chr;

    if (x == underscore_catcode && global_in_load) {
        translate_char(cur_cmd_chr);
        return true;
    }

    if (auto res = Dispatcher::call(x, c)) return *res;

    switch (x) {
    case start_thm_cmd:
        if (c == 2)
            T_end_theorem();
        else
            T_start_theorem(c);
        return true;
    case ignore_env_cmd: return true;
    case math_env_cmd:
        cur_tok.kill();
        pop_level(bt_env);
        T_math(c);
        return true;
    case end_ignore_env_cmd: return true;
    case end_minipage_cmd:
        flush_buffer();
        the_stack.pop_if_frame(the_names["cst_p"]);
        the_stack.pop_if_frame(the_names["item"]);
        the_stack.pop(the_names["minipage"]);
        return true;
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
