#include "tralics/XkvSetkeys.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"

namespace classes_ns {
    void unknown_optionX(TokenList &cur_keyval, TokenList &action);
} // namespace classes_ns

namespace xkv_ns {
    auto find_key_of(const TokenList &L, int type) -> std::string;
    auto is_Gin(const TokenList &x) -> bool;
    void makehd(const std::string &fam);
} // namespace xkv_ns

// Ctor
XkvSetkeys::XkvSetkeys(Parser *P) : P(P) {
    comma_token     = P->hash_table.comma_token;
    equals_token    = P->hash_table.equals_token;
    na_token        = P->hash_table.xkv_na_token;
    fams_token      = P->hash_table.xkv_fams_token;
    rm_token        = P->hash_table.xkv_rm_token;
    savevalue_token = P->hash_table.savevalue_token;
    usevalue_token  = P->hash_table.usevalue_token;
}

// This reads and manages the list of families
void XkvSetkeys::fetch_fams() {
    fams = P->read_arg();
    extract_keys(fams, Fams);
}

// Special case of ExecuteOptions
void XkvSetkeys::special_fams() {
    fams = P->XKV_parse_filename();
    extract_keys(fams, Fams);
}

// Reads the optional list of keys that should not be set
void XkvSetkeys::fetch_na() {
    P->read_optarg_nopar(na);
    extract_keys(na, Na);
}

void XkvSetkeys::fetch_keys(bool c) {
    if (!c)
        keyvals = P->get_mac_value(rm_token); // case of \setrmkeys
    else
        keyvals = P->read_arg();
    if (the_parser.tracing_commands()) {
        Logger::finish_seq();
        the_log << "setkeys -> " << keyvals << "\n";
    }
    extract_keys(keyvals, Keys);
}

// Evaluate now everything
void XkvSetkeys::finish() {
    P->new_macro(xkv_prefix, P->hash_table.xkv_prefix_token);
    P->new_macro(fams, fams_token);
    P->new_macro(na, na_token);
    if (!delayed.empty()) delayed.pop_back(); // remove trailing comma
    if (the_parser.tracing_commands()) {
        Logger::finish_seq();
        the_log << "setkeys <- " << action << "\n";
    }
    P->new_macro(delayed, rm_token);
    P->back_input(action);
}

// Extract the keys from a list, result in the vector R
// Will store a normalised version of the list in L
void XkvSetkeys::extract_keys(TokenList &L, std::vector<std::string> &R) {
    if (L.empty()) return;
    Token     T = comma_token;
    TokenList res;
    TokenList z;
    int       bl = 0;
    L.push_back(T);
    for (;;) {
        if (L.empty()) break;
        Token x = L.front();
        L.pop_front();
        token_ns::check_brace(x, bl);
        if (bl == 0 && x.is_a_char() && x.char_val() == ',') {
            token_ns::remove_first_last_space(z);
            if (z.empty()) continue;
            std::string s = xkv_ns::find_key_of(z, 1);
            R.push_back(s);
            res.splice(res.end(), z);
            res.push_back(T);
        } else
            z.push_back(x);
    }
    if (!res.empty()) res.pop_back(); // remove final comma
    L.swap(res);
}

void XkvSetkeys::dump_keys() {
    Logger::finish_seq();
    the_log << "{Options to execute->" << keyvals << "}\n";
}

void XkvSetkeys::run(bool c) {
    no_err  = P->remove_initial_plus(false);
    set_all = P->remove_initial_plus(true);
    P->xkv_fetch_prefix();
    fams = P->read_arg();
    if (xkv_ns::is_Gin(fams)) {
        TokenList L = the_parser.read_arg();
        L.push_back(comma_token);
        the_parser.new_macro(L, the_parser.hash_table.locate("Gin@keys"));
        return;
    }
    extract_keys(fams, Fams);
    fetch_na();
    fetch_keys(c);
    check_preset("preseth");
    set_aux(keyvals, -1);
    check_preset("presett");
    finish();
}

void XkvSetkeys::check_preset(String s) {
    Buffer &B = txparser2_local_buf;
    auto    N = Fams.size();
    for (size_t i = 0; i < N; i++) {
        xkv_ns::makehd(Fams[i]);
        B = "XKV@" + xkv_header + s;
        if (P->hash_table.is_defined(B)) {
            Token     T = P->hash_table.locate(B);
            TokenList W = P->get_mac_value(T);
            set_aux(W, to_signed(i));
        }
    }
}

// If i=-1 this is the normal case, else
// we are in preset and i is the family index
void XkvSetkeys::set_aux(TokenList &W, long idx) {
    TokenList val;
    XkvToken  cur;
    for (;;) {
        if (W.empty()) return;
        token_ns::split_at(comma_token, W, val);
        token_ns::remove_initial_spaces(val);
        if (val.empty()) continue;
        cur.initial = std::move(val);
        cur.extract();
        if (cur.keyname.empty()) {
            if (!cur.value.empty()) P->parse_error(P->err_tok, "No key for a value");
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
            Token T = P->hash_table.last_tok;
            found   = true;
            run_key(T, cur, Fams[i]);
            if (!set_all && found) break;
        }
        if (!found) check_action(cur);
    }
}

// This is called if the value must be saved
void XkvSetkeys::save_key(const std::string &Key, TokenList &L) {
    txparser2_local_buf = "XKV@" + xkv_header + Key + "@value";
    Token T             = P->hash_table.locate(txparser2_local_buf);
    P->new_macro(L, T, xkv_is_global);
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
        P->brace_me(LL);
        more_action(LL);
    }
}

// Interprets \usevalue{foo} in the list L
void XkvSetkeys::replace_pointers(TokenList &L) {
    Buffer &  B = txparser2_local_buf;
    TokenList res;
    int       n = 1000;
    for (;;) {
        if (L.empty()) break;
        Token t = L.front();
        if (t.is_a_left_brace()) {
            L.fast_get_block(res);
            continue;
        }
        L.pop_front();
        if (t != usevalue_token) {
            res.push_back(t);
            continue;
        }
        if (!L.empty()) t = L.front();
        if (!t.is_a_left_brace()) {
            P->parse_error(P->err_tok, "Invalid \\usevalue token", "invalid usevalue");
            continue;
        }
        --n;
        if (n < 0) {
            P->parse_error(P->err_tok, "Replace pointer aborted, (infinite loop?)", "key loop");
            res.clear();
            break;
        }
        TokenList key = L.fast_get_block();
        token_ns::remove_ext_braces(key);
        std::string Key     = P->list_to_string_c(key, "Argument of \\savevalue");
        txparser2_local_buf = "XKV@" + xkv_header + Key + "@value";
        if (P->hash_table.is_defined(B)) {
            TokenList w = P->get_mac_value(P->hash_table.last_tok);
            L.splice(L.begin(), w);
        } else {
            B = "No value recorded for key `" + Key + "'; ignored";
            P->parse_error(P->err_tok, B, "no val recorded");
        }
    }
    L.swap(res);
}

//
void XkvSetkeys::run_default(const std::string &Key, Token mac, bool s) {
    Buffer &B = txparser2_local_buf;
    B         = xkv_header + Key + "@default";
    if (!P->hash_table.is_defined(B)) {
        B = "No value specified for key `" + Key + "'";
        P->parse_error(P->err_tok, B);
        return;
    }
    Token     T = P->hash_table.locate(B);
    TokenList L = P->get_mac_value(T);
    if (L.empty() || L.front() != mac) {
        // no hack needed
        action.push_back(T);
        return;
    }
    L.pop_front();
    // We apply mac to L via setkeys and not the macro
    TokenList args;
    if (!L.empty()) {
        if (L.front().is_a_left_brace()) { // get the block without the braces
            L.fast_get_block(args);
            args.pop_front();
            args.pop_back();
        } else { // otherwise get just a token
            args.push_back(L.front());
            L.pop_front();
        }
    }
    // We have now in args the real argument.
    if (s) save_key(Key, args);
    replace_pointers(args);
    action.push_back(mac);
    P->brace_me(args);
    more_action(args);
}

// This considers the case when \ProcessOptionsX has seen cur_opt
void XkvSetkeys::check_action(XkvToken &cur) {
    TokenList cur_opt = cur.initial;
    if (in_pox) {
        classes_ns::unknown_optionX(cur_opt, action);
        return;
    }
    // This is the normal case
    if (no_err)
        new_unknown(cur_opt);
    else
        P->parse_error(P->err_tok, "Undefined key: ", cur.keyname, "undefined key");
}