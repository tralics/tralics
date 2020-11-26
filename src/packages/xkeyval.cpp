#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"

// Auto-registering package, see tipa.cpp

namespace {
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
        Dispatcher::register_action_plain(xkeyval_cmd, &Parser::T_xkeyval);
    }

    bool t = (xkeyval(), true);
} // namespace
