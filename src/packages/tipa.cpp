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
            auto n = the_parser.cur_cmd_chr.chr;
            if (n > 0 && n < 128) {
                if (n == 'k') {
                    the_parser.extended_chars(0x29e);
                    return;
                }
                if (n == 'f') {
                    mk_hi("turned", 'f');
                    return;
                }
                if (n == 't') {
                    the_parser.extended_chars(0x287);
                    return;
                }
                if (n == 'r') {
                    the_parser.extended_chars(0x279);
                    return;
                }
                if (n == 'w') {
                    the_parser.extended_chars(0x28d);
                    return;
                }
                if (n == 'j') {
                    the_parser.extended_chars(0x25f);
                    return;
                }
                if (n == 'n') {
                    the_parser.extended_chars(0x272);
                    return;
                }
                if (n == 'h') {
                    the_parser.extended_chars(0x127);
                    return;
                }
                if (n == 'l') {
                    the_parser.extended_chars(0x26c);
                    return;
                }
                if (n == 'z') {
                    the_parser.extended_chars(0x26e);
                    return;
                }
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_semi() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = the_parser.cur_cmd_chr.chr;
            if (n > 0 && n < 128) {
                if (n == 'E' || n == 'J' || n == 'A' || n == 'U') {
                    mk_hi("sc", char(n));
                    return;
                }
                if (n == 'H') {
                    the_parser.extended_chars(0x29c);
                    return;
                }
                if (n == 'L') {
                    the_parser.extended_chars(0x29f);
                    return;
                }
                if (n == 'B') {
                    the_parser.extended_chars(0x299);
                    return;
                }
                if (n == 'G') {
                    the_parser.extended_chars(0x262);
                    return;
                }
                if (n == 'N') {
                    the_parser.extended_chars(0x274);
                    return;
                }
                if (n == 'R') {
                    the_parser.extended_chars(0x280);
                    return;
                }
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_colon() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = the_parser.cur_cmd_chr.chr;
            if (n > 0 && n < 128) {
                if (n == 'd') {
                    the_parser.extended_chars(0x256);
                    return;
                }
                if (n == 'l') {
                    the_parser.extended_chars(0x26d);
                    return;
                }
                if (n == 'n') {
                    the_parser.extended_chars(0x273);
                    return;
                }
                if (n == 'r') {
                    the_parser.extended_chars(0x27d);
                    return;
                }
                if (n == 'R') {
                    the_parser.extended_chars(0x27b);
                    return;
                }
                if (n == 's') {
                    the_parser.extended_chars(0x282);
                    return;
                }
                if (n == 't') {
                    the_parser.extended_chars(0x288);
                    return;
                }
                if (n == 'z') {
                    the_parser.extended_chars(0x290);
                    return;
                }
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_exclam() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = the_parser.cur_cmd_chr.chr;
            if (n > 0 && n < 128) {
                if (n == 'G') {
                    the_parser.extended_chars(0x29b);
                    return;
                }
                if (n == 'b') {
                    the_parser.extended_chars(0x253);
                    return;
                }
                if (n == 'd') {
                    the_parser.extended_chars(0x257);
                    return;
                }
                if (n == 'g') {
                    the_parser.extended_chars(0x260);
                    return;
                }
                if (n == 'j') {
                    the_parser.extended_chars(0x284);
                    return;
                }
                if (n == 'o') {
                    the_parser.extended_chars(0x298);
                    return;
                }
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void tipa_normal() {
        if (the_parser.get_token()) return; // should not happen
        auto cmd = the_parser.cur_cmd_chr.cmd;
        if (cmd == 12 || cmd == 11) {
            auto n = the_parser.cur_cmd_chr.chr;
            if (n > 0 && n < 128) {
                if (n == '0') {
                    the_parser.extended_chars(0x289);
                    return;
                }
                if (n == '1') {
                    the_parser.extended_chars(0x268);
                    return;
                }
                if (n == '2') {
                    the_parser.extended_chars(0x28c);
                    return;
                }
                if (n == '3') {
                    the_parser.extended_chars(0x25c);
                    return;
                }
                if (n == '4') {
                    the_parser.extended_chars(0x265);
                    return;
                }
                if (n == '5') {
                    the_parser.extended_chars(0x250);
                    return;
                }
                if (n == '6') {
                    the_parser.extended_chars(0x252);
                    return;
                }
                if (n == '7') {
                    the_parser.extended_chars(0x264);
                    return;
                }
                if (n == '8') {
                    the_parser.extended_chars(0x275);
                    return;
                }
                if (n == '9') {
                    the_parser.extended_chars(0x258);
                    return;
                }
                if (n == '@') {
                    the_parser.extended_chars(0x259);
                    return;
                }
                if (n == 'A') {
                    the_parser.extended_chars(0x251);
                    return;
                }
                if (n == 'B') {
                    the_parser.extended_chars(0x3b2);
                    return;
                } // beta
                if (n == 'C') {
                    the_parser.extended_chars(0x255);
                    return;
                }
                if (n == 'D') {
                    the_parser.extended_chars(0x0f0);
                    return;
                } // eth
                if (n == 'E') {
                    the_parser.extended_chars(0x25B);
                    return;
                }
                if (n == 'F') {
                    the_parser.extended_chars(0x278);
                    return;
                }
                if (n == 'G') {
                    the_parser.extended_chars(0x263);
                    return;
                }
                if (n == 'H') {
                    the_parser.extended_chars(0x266);
                    return;
                }
                if (n == 'I') {
                    the_parser.extended_chars(0x26A);
                    return;
                }
                if (n == 'J') {
                    the_parser.extended_chars(0x29D);
                    return;
                }
                if (n == 'K') {
                    the_parser.extended_chars(0x261);
                    return;
                }
                if (n == 'L') {
                    the_parser.extended_chars(0x28E);
                    return;
                }
                if (n == 'M') {
                    the_parser.extended_chars(0x271);
                    return;
                }
                if (n == 'N') {
                    the_parser.extended_chars(0x14b);
                    return;
                } // eng
                if (n == 'O') {
                    the_parser.extended_chars(0x254);
                    return;
                }
                if (n == 'P') {
                    the_parser.extended_chars(0x294);
                    return;
                }
                if (n == 'Q') {
                    the_parser.extended_chars(0x295);
                    return;
                }
                if (n == 'R') {
                    the_parser.extended_chars(0x27E);
                    return;
                }
                if (n == 'S') {
                    the_parser.extended_chars(0x283);
                    return;
                }
                if (n == 'T') {
                    the_parser.extended_chars(0x3b8);
                    return;
                } // theta
                if (n == 'U') {
                    the_parser.extended_chars(0x28a);
                    return;
                }
                if (n == 'V') {
                    the_parser.extended_chars(0x28b);
                    return;
                }
                if (n == 'W') {
                    the_parser.extended_chars(0x26f);
                    return;
                }
                if (n == 'X') {
                    the_parser.extended_chars(0x3c7);
                    return;
                } // chi
                if (n == 'Y') {
                    the_parser.extended_chars(0x28f);
                    return;
                }
                if (n == 'Z') {
                    the_parser.extended_chars(0x292);
                    return;
                }
            }
        }
        the_parser.back_input(the_parser.cur_tok);
    }

    void T_ipa(subtypes c) {
        if (c == 0) tipa_star();
        if (c == 1) tipa_semi();
        if (c == 2) tipa_colon();
        if (c == 3) tipa_exclam();
        if (c == 4) tipa_normal();
    }

    // Unicode characters not in the tables above.
    // 25a 25d 25e
    // 262 267 269 26b
    // 270 276 277 27a 27c 27f
    // 281 285 286
    // 291 x293 296 297 29a
    // 2a0 2a1 2a2 2a3 2a4 2a5 2a6 2a7 2a8 2a9 2aa 2ab 2ac 2ad 2ae 2af

    void tipa() {
        actions.register_action(ipa_cmd, [](symcodes, subtypes c) { T_ipa(c); });
    }
    bool t = (tipa(), true);
} // namespace
