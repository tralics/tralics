#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"

// Auto-registering package, see tipa.cpp

namespace xkv_ns {
    void find_aux(int c);
    auto find_key_of(const TokenList &L, int type) -> std::string;
    void makehd(const std::string &fam);
} // namespace xkv_ns

namespace {
    void T_xkeyval(subtypes c) {
        switch (c) {
        case boot_keyval_code: return;
        case xdefinekey_code: the_parser.T_define_key(true); return;
        case define_cmdkey_code:
        case define_cmdkeys_code: the_parser.define_cmd_key(c); return;
        case define_boolkey_code:
        case define_boolkeys_code: the_parser.define_bool_key(c); return;
        case define_choicekey_code: the_parser.define_choice_key(); return;
        case define_cc_code: the_parser.internal_choice_key(); return;
        case disable_keys_code: the_parser.disable_keys(); return;
        case key_ifundefined_code: the_parser.key_ifundefined(); return;
        case save_keys_code:
        case gsave_keys_code: {
            the_parser.xkv_save_keys_aux(false, 0);
            TokenList A = the_parser.read_arg();
            the_parser.xkv_merge(c == gsave_keys_code, 0, A, true);
            return;
        }
        case delsave_keys_code:
        case gdelsave_keys_code: {
            bool      bad = the_parser.xkv_save_keys_aux(false, 0);
            TokenList A   = the_parser.read_arg();
            if (!bad) the_parser.xkv_merge(c == gdelsave_keys_code, 0, A, false);
            return;
        }
        case unsave_keys_code:
        case gunsave_keys_code:
            if (the_parser.xkv_save_keys_aux(true, 0)) return;
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunsave_keys_code);
            return;
        case preset_keys_code:
        case gpreset_keys_code: {
            the_parser.xkv_save_keys_aux(false, 1);
            TokenList A = the_parser.read_arg();
            TokenList B = the_parser.read_arg();
            the_parser.xkv_merge(c == gpreset_keys_code, 1, A, true);
            the_parser.xkv_merge(c == gpreset_keys_code, 2, B, true);
            return;
        }
        case delpreset_keys_code:
        case gdelpreset_keys_code: {
            bool      bad = the_parser.xkv_save_keys_aux(true, 1);
            TokenList A   = the_parser.read_arg();
            TokenList B   = the_parser.read_arg();
            if (bad) return;
            the_parser.xkv_merge(c == gdelpreset_keys_code, 1, A, false);
            the_parser.xkv_merge(c == gdelpreset_keys_code, 2, B, false);
            return;
        }
        case unpreset_keys_code:
        case gunpreset_keys_code: {
            if (the_parser.xkv_save_keys_aux(true, 1)) return;
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
            xkv_ns::find_aux(2);
            the_parser.cur_tok = hash_table.locate(txparser2_local_buf);
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
            return;
        }
        case setrmkeys_code: the_parser.setkeys(false); return;
        case setkeys_code: the_parser.setkeys(true); return;
        case declare_optionsX_code: the_parser.xkv_declare_option(); return;
        case process_optionsX_code: the_parser.xkv_process_options(); return;
        case execute_optionsX_code: the_parser.xkv_execute_options(); return;
        default: return;
        }
    }

    void xkeyval() {
        hash_table.primitive_plain("define@boolkey", xkeyval_cmd, define_boolkey_code);
        hash_table.primitive_plain("define@boolkeys", xkeyval_cmd, define_boolkeys_code);
        hash_table.primitive_plain("define@choicekey", xkeyval_cmd, define_choicekey_code);
        hash_table.primitive_plain("define@cmdkey", xkeyval_cmd, define_cmdkey_code);
        hash_table.primitive_plain("define@cmdkeys", xkeyval_cmd, define_cmdkeys_code);
        hash_table.primitive_plain("define@key", xkeyval_cmd, xdefinekey_code);
        hash_table.primitive_plain("tralics@boot@keyval", xkeyval_cmd, boot_keyval_code);
        hash_table.primitive_plain("XKV@cc", xkeyval_cmd, define_cc_code);
        hash_table.primitive_plain("key@ifundefined", xkeyval_cmd, key_ifundefined_code);
        hash_table.primitive_plain("disable@keys", xkeyval_cmd, disable_keys_code);
        hash_table.primitive_plain("presetkeys", xkeyval_cmd, preset_keys_code);
        hash_table.primitive_plain("gpresetkeys", xkeyval_cmd, gpreset_keys_code);
        hash_table.primitive_plain("unpresetkeys", xkeyval_cmd, unpreset_keys_code);
        hash_table.primitive_plain("gunpresetkeys", xkeyval_cmd, gunpreset_keys_code);
        hash_table.primitive_plain("delpresetkeys", xkeyval_cmd, delpreset_keys_code);
        hash_table.primitive_plain("gdelpresetkeys", xkeyval_cmd, gdelpreset_keys_code);
        hash_table.primitive_plain("savekeys", xkeyval_cmd, save_keys_code);
        hash_table.primitive_plain("gsavekeys", xkeyval_cmd, gsave_keys_code);
        hash_table.primitive_plain("unsavekeys", xkeyval_cmd, unsave_keys_code);
        hash_table.primitive_plain("gunsavekeys", xkeyval_cmd, gunsave_keys_code);
        hash_table.primitive_plain("delsavekeys", xkeyval_cmd, delsave_keys_code);
        hash_table.primitive_plain("gdelsavekeys", xkeyval_cmd, gdelsave_keys_code);
        hash_table.primitive_plain("setkeys", xkeyval_cmd, setkeys_code);
        hash_table.primitive_plain("setrmkeys", xkeyval_cmd, setrmkeys_code);
        hash_table.primitive_plain("DeclareOptionX", xkeyval_cmd, declare_optionsX_code);
        hash_table.primitive_plain("ExecuteOptionsX", xkeyval_cmd, execute_optionsX_code);
        hash_table.primitive_plain("ProcessOptionsX", xkeyval_cmd, process_optionsX_code);
        Dispatcher::register_action_plain(xkeyval_cmd, T_xkeyval);
    }

    bool t = (xkeyval(), true);
} // namespace
