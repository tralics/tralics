#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"

// Auto-registering package, see tipa.cpp

namespace {
    auto token_xkeyval_name(subtypes chr) -> std::string {
        switch (chr) {
        case declare_optionsX_code: return "DeclareOptionX";
        case delpreset_keys_code: return "delpresetkeys";
        case delsave_keys_code: return "delsavekeys";
        case execute_optionsX_code: return "ExecuteOptionsX";
        case gdelpreset_keys_code: return "gdelpresetkeys";
        case gdelsave_keys_code: return "gdelsavekeys";
        case gpreset_keys_code: return "gpresetkeys";
        case gsave_keys_code: return "gsavekeys";
        case gunpreset_keys_code: return "gunpresetkeys";
        case gunsave_keys_code: return "gunsavekeys";
        case preset_keys_code: return "presetkeys";
        case process_optionsX_code: return "ProcessOptionsX";
        case save_keys_code: return "savekeys";
        case setkeys_code: return "setkeys";
        case setrmkeys_code: return "setrmkeys";
        case unpreset_keys_code: return "unpresetkeys";
        case unsave_keys_code: return "unsavekeys";
        default: return "";
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
        Symcode::get(xkeyval_cmd).name_fn = token_xkeyval_name;
        Dispatcher::register_action_plain(xkeyval_cmd, &Parser::T_xkeyval);
    }

    bool t = (xkeyval(), true);
} // namespace
