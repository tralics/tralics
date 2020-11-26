#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"

// Auto-registering package, see tipa.cpp

namespace {
    auto token_xkeyval_name(subtypes chr) -> std::string {
        switch (chr) {
        case boot_keyval_code: return "tralics@boot@keyval";
        case definekey_code: return "define@key";
        case xdefinekey_code: return "define@key";
        case define_cmdkey_code: return "define@cmdkey";
        case define_cmdkeys_code: return "define@cmdkeys";
        case define_boolkey_code: return "define@boolkey";
        case define_boolkeys_code: return "define@boolkeys";
        case save_keys_code: return "savekeys";
        case gsave_keys_code: return "gsavekeys";
        case delsave_keys_code: return "delsavekeys";
        case gdelsave_keys_code: return "gdelsavekeys";
        case unsave_keys_code: return "unsavekeys";
        case gunsave_keys_code: return "gunsavekeys";
        case preset_keys_code: return "presetkeys";
        case gpreset_keys_code: return "gpresetkeys";
        case delpreset_keys_code: return "delpresetkeys";
        case gdelpreset_keys_code: return "gdelpresetkeys";
        case unpreset_keys_code: return "unpresetkeys";
        case gunpreset_keys_code: return "gunpresetkeys";
        case setkeys_code: return "setkeys";
        case setrmkeys_code: return "setrmkeys";
        case declare_optionsX_code: return "DeclareOptionX";
        case execute_optionsX_code: return "ExecuteOptionsX";
        case process_optionsX_code: return "ProcessOptionsX";
        default: return "";
        }
    }

    void xkeyval() {
        Symcode::get(xkeyval_cmd).name_fn = token_xkeyval_name;
        Dispatcher::register_action_plain(xkeyval_cmd, &Parser::T_xkeyval);
    }

    bool t = (xkeyval(), true);
} // namespace
