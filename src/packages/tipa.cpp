#include "../tralics/Dispatcher.h"

/* Auto-registering package
 *
 * This code is automatically running on startup (the static variable `t` is not
 * optimized away because it is initialized by a function with side-effects), we
 * are sure that `register_action` can be used because the map is hidden in the
 * `get_map` method of Dispatcher. This would fail if the code is compiled into
 * a static library but we will not be doing this.
 *
 * Good inclusion is tested by testhtml in the test suite.
 *
 * \todo move Parser::T_ipa here
 *
 * Reference: https://www.bfilipek.com/2018/02/static-vars-static-lib.html
 */

namespace {
    void mk_hi(String x, char c) {
        the_parser.LC();
        the_parser.unprocessed_xml.format("<hi rend='{}'>{}</hi>", x, c);
    }

    void tipa_star() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = size_t(the_parser.cur_cmd_chr.chr);
            switch (n) {
            case 'k': the_parser.extended_chars(0x29e); return;
            case 'f': mk_hi("turned", 'f'); return;
            case 't': the_parser.extended_chars(0x287); return;
            case 'r': the_parser.extended_chars(0x279); return;
            case 'w': the_parser.extended_chars(0x28d); return;
            case 'j': the_parser.extended_chars(0x25f); return;
            case 'n': the_parser.extended_chars(0x272); return;
            case 'h': the_parser.extended_chars(0x127); return;
            case 'l': the_parser.extended_chars(0x26c); return;
            case 'z': the_parser.extended_chars(0x26e); return;
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_semi() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = size_t(the_parser.cur_cmd_chr.chr);
            switch (n) {
            case 'E': mk_hi("sc", 'E'); return;
            case 'J': mk_hi("sc", 'J'); return;
            case 'A': mk_hi("sc", 'A'); return;
            case 'U': mk_hi("sc", 'U'); return;
            case 'H': the_parser.extended_chars(0x29c); return;
            case 'L': the_parser.extended_chars(0x29f); return;
            case 'B': the_parser.extended_chars(0x299); return;
            case 'G': the_parser.extended_chars(0x262); return;
            case 'N': the_parser.extended_chars(0x274); return;
            case 'R': the_parser.extended_chars(0x280); return;
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_colon() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = size_t(the_parser.cur_cmd_chr.chr);
            switch (n) {
            case 'd': the_parser.extended_chars(0x256); return;
            case 'l': the_parser.extended_chars(0x26d); return;
            case 'n': the_parser.extended_chars(0x273); return;
            case 'r': the_parser.extended_chars(0x27d); return;
            case 'R': the_parser.extended_chars(0x27b); return;
            case 's': the_parser.extended_chars(0x282); return;
            case 't': the_parser.extended_chars(0x288); return;
            case 'z': the_parser.extended_chars(0x290); return;
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_exclam() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = size_t(the_parser.cur_cmd_chr.chr);
            switch (n) {
            case 'G': the_parser.extended_chars(0x29b); return;
            case 'b': the_parser.extended_chars(0x253); return;
            case 'd': the_parser.extended_chars(0x257); return;
            case 'g': the_parser.extended_chars(0x260); return;
            case 'j': the_parser.extended_chars(0x284); return;
            case 'o': the_parser.extended_chars(0x298); return;
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_normal() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = size_t(the_parser.cur_cmd_chr.chr);
            switch (n) {
            case '0': the_parser.extended_chars(0x289); return;
            case '1': the_parser.extended_chars(0x268); return;
            case '2': the_parser.extended_chars(0x28c); return;
            case '3': the_parser.extended_chars(0x25c); return;
            case '4': the_parser.extended_chars(0x265); return;
            case '5': the_parser.extended_chars(0x250); return;
            case '6': the_parser.extended_chars(0x252); return;
            case '7': the_parser.extended_chars(0x264); return;
            case '8': the_parser.extended_chars(0x275); return;
            case '9': the_parser.extended_chars(0x258); return;
            case '@': the_parser.extended_chars(0x259); return;
            case 'A': the_parser.extended_chars(0x251); return;
            case 'B': the_parser.extended_chars(0x3b2); return;
            case 'C': the_parser.extended_chars(0x255); return;
            case 'D': the_parser.extended_chars(0x0f0); return;
            case 'E': the_parser.extended_chars(0x25B); return;
            case 'F': the_parser.extended_chars(0x278); return;
            case 'G': the_parser.extended_chars(0x263); return;
            case 'H': the_parser.extended_chars(0x266); return;
            case 'I': the_parser.extended_chars(0x26A); return;
            case 'J': the_parser.extended_chars(0x29D); return;
            case 'K': the_parser.extended_chars(0x261); return;
            case 'L': the_parser.extended_chars(0x28E); return;
            case 'M': the_parser.extended_chars(0x271); return;
            case 'N': the_parser.extended_chars(0x14b); return;
            case 'O': the_parser.extended_chars(0x254); return;
            case 'P': the_parser.extended_chars(0x294); return;
            case 'Q': the_parser.extended_chars(0x295); return;
            case 'R': the_parser.extended_chars(0x27E); return;
            case 'S': the_parser.extended_chars(0x283); return;
            case 'T': the_parser.extended_chars(0x3b8); return;
            case 'U': the_parser.extended_chars(0x28a); return;
            case 'V': the_parser.extended_chars(0x28b); return;
            case 'W': the_parser.extended_chars(0x26f); return;
            case 'X': the_parser.extended_chars(0x3c7); return;
            case 'Y': the_parser.extended_chars(0x28f); return;
            case 'Z': the_parser.extended_chars(0x292); return;
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void T_ipa(symcodes, subtypes c) {
        if (c == 0) tipa_star();
        if (c == 1) tipa_semi();
        if (c == 2) tipa_colon();
        if (c == 3) tipa_exclam();
        if (c == 4) tipa_normal();
    }

    auto tipa_name(subtypes chr) -> std::string {
        switch (chr) {
        case 0: return "tipa@star";
        case 1: return "tipa@semi";
        case 2: return "tipa@colon";
        case 3: return "tipa@exclam";
        case 4: return "tipa@normal";
        case 5: return "tipa@syllabic";
        case 6: return "tipa@subumlaut";
        case 7: return "tipa@subtilde";
        case 8: return "tipa@subring";
        case 9: return "tipa@dotacute";
        case 10: return "tipa@gravedot";
        case 11: return "tipa@acutemacron";
        case 12: return "tipa@circumdot";
        case 13: return "tipa@tildedot";
        case 14: return "tipa@brevemacro";
        default: return "";
        }
    }

    void tipa() {
        hash_table.primitive("tipa@star", ipa_cmd, subtypes(0));
        hash_table.primitive("tipa@semi", ipa_cmd, subtypes(1));
        hash_table.primitive("tipa@colon", ipa_cmd, subtypes(2));
        hash_table.primitive("tipa@exclam", ipa_cmd, subtypes(3));
        hash_table.primitive("tipa@normal", ipa_cmd, subtypes(4));
        hash_table.primitive("tipa@syllabic", ipa_cmd, subtypes(5));
        hash_table.primitive("tipa@subumlaut", ipa_cmd, subtypes(6));
        hash_table.primitive("tipa@subtilde", ipa_cmd, subtypes(7));
        hash_table.primitive("tipa@subring", ipa_cmd, subtypes(8));
        hash_table.primitive("tipa@dotacute", ipa_cmd, subtypes(9));
        hash_table.primitive("tipa@gravedot", ipa_cmd, subtypes(10));
        hash_table.primitive("tipa@acutemacron", ipa_cmd, subtypes(11));
        hash_table.primitive("tipa@circumdot", ipa_cmd, subtypes(12));
        hash_table.primitive("tipa@tildedot", ipa_cmd, subtypes(13));
        hash_table.primitive("tipa@brevemacro", ipa_cmd, subtypes(14));
        actions.register_action(ipa_cmd, T_ipa);
        actions.register_name(ipa_cmd, [](symcodes, subtypes x) { return tipa_name(x); });
    }
    bool t = (tipa(), true);
} // namespace
