#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"
#include "../tralics/globals.h"

// Auto-registering package, see tipa.cpp

namespace xkv_ns {
    void find_aux(int c);
    auto find_key_of(const TokenList &L, int type) -> std::string;
    void makehd(const std::string &fam);
} // namespace xkv_ns

namespace {
    void T_define_key() {
        Buffer &B = txparser2_local_buf;
        the_parser.xkv_fetch_prefix_family();
        TokenList key = the_parser.read_arg();
        the_parser.list_to_string_c(key, xkv_header, "", "bad key name", B);
        Token     T = hash_table.locate(B);
        TokenList opt;
        if (the_parser.read_optarg(opt)) the_parser.internal_define_key_default(T, opt);
        the_parser.internal_define_key(T);
    }

    // Implements \define@choicekey
    void define_choice_key() {
        bool if_star = the_parser.remove_initial_plus(false);
        bool if_plus = the_parser.remove_initial_plus(true);
        the_parser.xkv_fetch_prefix_family();
        TokenList keytoks = the_parser.read_arg();
        the_parser.list_to_string_c(keytoks, xkv_header, "", "bad key name", txparser2_local_buf);
        Token     T = hash_table.locate(txparser2_local_buf);
        TokenList storage_bin;
        the_parser.read_optarg_nopar(storage_bin);
        TokenList allowed = the_parser.read_arg();
        TokenList opt;
        if (the_parser.read_optarg(opt)) the_parser.internal_define_key_default(T, opt);
        TokenList F;
        if (if_plus) {
            TokenList x = the_parser.read_arg();
            TokenList y = the_parser.read_arg();
            x.brace_me();
            y.brace_me();
            F.splice(F.end(), x);
            F.splice(F.end(), y);
        } else {
            F = the_parser.read_arg();
            F.brace_me();
        }
        TokenList body;
        body.push_back(hash_table.locate("XKV@cc"));
        if (if_star) body.push_back(Token(other_t_offset, '*'));
        if (if_plus) body.push_back(Token(other_t_offset, '+'));
        storage_bin.push_front(Token(other_t_offset, '['));
        storage_bin.push_back(Token(other_t_offset, ']'));
        body.splice(body.end(), storage_bin);
        TokenList argument;
        argument.push_back(make_char_token('#', 6));
        argument.push_back(Token(other_t_offset, '1'));
        argument.brace_me();
        body.splice(body.end(), argument);
        allowed.brace_me();
        body.splice(body.end(), allowed);
        body.splice(body.end(), F);
        body.brace_me();
        the_parser.back_input(body);
        the_parser.internal_define_key(T);
    }

    void T_xkeyval(subtypes c) {
        switch (c) {
        case boot_keyval_code: return;
        case xdefinekey_code: T_define_key(); return;
        case define_cmdkey_code:
        case define_cmdkeys_code: the_parser.define_cmd_key(c); return;
        case define_boolkey_code:
        case define_boolkeys_code: the_parser.define_bool_key(c); return;
        case define_choicekey_code: define_choice_key(); return;
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
