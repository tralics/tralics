#include "tralics/XkvSetkeys.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"

namespace classes_ns {
    void unknown_optionX(TokenList &cur_keyval, TokenList &action);
} // namespace classes_ns

// Special case of ExecuteOptions
void XkvSetkeys::special_fams() {
    fams = the_parser.XKV_parse_filename();
    extract_keys(fams, Fams);
}

void XkvSetkeys::fetch_keys(bool c) {
    if (!c)
        keyvals = the_parser.get_mac_value(hash_table.locate("XKV@rm")); // case of \setrmkeys
    else
        keyvals = the_parser.read_arg();
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "setkeys -> " << keyvals << "\n";
    }
    extract_keys(keyvals, Keys);
}

void XkvSetkeys::dump_keys() {
    Logger::finish_seq();
    the_log << "{Options to execute->" << keyvals << "}\n";
}

// If i=-1 this is the normal case, else
// we are in preset and i is the family index
void XkvSetkeys::set_aux(TokenList &W, long idx) {
    TokenList val;
    XkvToken  cur;
    for (;;) {
        if (W.empty()) return;
        token_ns::split_at(hash_table.comma_token, W, val);
        token_ns::remove_initial_spaces(val);
        if (val.empty()) continue;
        cur.initial = std::move(val);
        cur.extract();
        if (cur.keyname.empty()) {
            if (!cur.value.empty()) the_parser.parse_error(the_parser.err_tok, "No key for a value");
            continue;
        }
        if (cur.ignore_this(Na)) continue;
        if (idx >= 0 && cur.ignore_this(Keys)) continue;
        bool found = false;
        auto N     = Fams.size();
        // if idx>=0, execute the loop once
        size_t i = 0;
        if (idx >= 0) {
            i = to_unsigned(idx);
            N = to_unsigned(idx + 1);
        }
        for (; i < N; i++) {
            if (!cur.is_defined(Fams[i])) continue;
            Token T = hash_table.last_tok;
            found   = true;
            run_key(T, cur, Fams[i]);
            if (!set_all && found) break;
        }
        if (!found) check_action(cur);
    }
}

// This is called when the key is defined
// What happens if you say \savevalue{keya} = \usevalue{keyb} ?
void XkvSetkeys::run_key(Token mac, XkvToken &cur, const std::string &fam) {
    cur.prepare(fam);
    more_action(cur.action);
    bool               s   = cur.check_save();
    const std::string &Key = cur.keyname;
    TokenList          L   = cur.value;
    if (s) save_key(Key, L);
    if (cur.value.empty())
        run_default(Key, mac, s);
    else {
        TokenList LL = cur.value;
        replace_pointers(LL);
        action.push_back(mac);
        LL.brace_me();
        more_action(LL);
    }
}

// This considers the case when \ProcessOptionsX has seen cur_opt
void XkvSetkeys::check_action(XkvToken &cur) {
    TokenList cur_opt = cur.initial;
    if (in_pox) {
        classes_ns::unknown_optionX(cur_opt, action);
        return;
    }
    // This is the normal case
    if (no_err) {
        delayed.splice(delayed.end(), cur_opt);
        delayed.push_back(hash_table.comma_token);
    } else
        the_parser.parse_error(the_parser.err_tok, "Undefined key: ", cur.keyname, "undefined key");
}
