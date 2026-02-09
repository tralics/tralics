#include "../tralics/Dispatcher.h"
#include "../tralics/Symcode.h"
#include "../tralics/globals.h"
#include "../tralics/util.h"
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

// Auto-registering package, see tipa.cpp for details

namespace classes_ns {
    auto cur_options(bool, TokenList &, bool) -> TokenList;
    void register_key(const std::string &);
    void unknown_optionX(TokenList &cur_keyval, TokenList &action);
} // namespace classes_ns

namespace {
    bool        xkv_is_global, xkv_is_save;
    std::string xkv_header, xkv_prefix;

    struct XkvToken {
        std::string keyname;
        TokenList   initial, value, action;
        bool        is_global{}, has_save{}, has_val{};

    public:
        void extract();
        void prepare(const std::string &fam);

        [[nodiscard]] auto check_save() const -> bool;
        [[nodiscard]] auto is_defined(const std::string &fam) const -> bool;
    };

    class XkvSetkeys {
        std::vector<std::string> Fams;           // the list of all families
        std::vector<std::string> Na;             // the list of keys not to set
        std::vector<std::string> Keys;           // the list of keys
        TokenList                fams;           // the list of families
        TokenList                na;             // the list of keys that should not be set
        TokenList                keyvals;        // the keylist to set
        TokenList                delayed;        // unknown ketyvalue pairs
        TokenList                action;         // expansion of \setkeys
        bool                     no_err{false};  // Error when undefined ?
        bool                     set_all{false}; // set key in all families ?
        bool                     in_pox{false};  // are in in \ProcessOptionsX ?

    public:
        void run(bool c);
        void check_preset(std::string_view s);
        void special_fams();
        void fetch_keys(bool c);
        void check_action(const XkvToken &cur);
        void run_key(Token mac, XkvToken &cur, const std::string &fam);
        void run_default(const std::string &Key, Token mac, bool s);
        void more_action(TokenList &L) { action.splice(action.end(), L); }
        void finish();
        void set_aux(TokenList &W, long idx);
        void set_aux() { set_aux(keyvals, -1); }
        void set_inpox() { in_pox = true; }
        void dump_keys();
    };

    // This is called if the value must be saved
    void save_key(const std::string &Key, TokenList &L) {
        Token T = hash_table.locate("XKV@" + xkv_header + Key + "@value");
        the_parser.new_macro(L, T, xkv_is_global);
    }

    [[nodiscard]] auto xkv_find_aux(int c) -> std::string {
        std::string res = "XKV@" + xkv_header;
        res += (c == 0 ? "save" : (c == 1 ? "preseth" : "presett"));
        return res;
    }

    [[nodiscard]] auto xkv_makehd_s(const std::string &fam) -> std::string {
        std::string res = xkv_prefix + fam;
        if (!fam.empty()) res.push_back('@');
        xkv_header = res;
        return res;
    }

    // The following function takes in L one item and puts the key in x.
    // If type is 0, we are looking for \global, and there is no equals
    // Otherwise we look for \savevalue or \gsavevalue, skip equals.
    // We set some booleans
    auto xkv_find_key_of(const TokenList &L, int type) -> std::string {
        Hashtab  &H      = hash_table;
        Token     equals = H.equals_token;
        auto      C      = L.begin();
        auto      E      = L.end();
        TokenList x; // will hold the tokens
        while (C != E) {
            if (*C == equals) break;
            x.push_back(*C);
            ++C;
        }
        xkv_is_save   = false;
        xkv_is_global = false;
        if (x.empty()) return "";
        Token first = x.front();
        if (type == 0) {
            if (first == H.global_token) {
                x.pop_front();
                xkv_is_global = true;
            }
        } else {
            if (first == H.locate("savevalue")) {
                x.pop_front();
                xkv_is_save = true;
            } else if (first == H.locate("gsavevalue")) {
                x.pop_front();
                xkv_is_save   = true;
                xkv_is_global = true;
            }
        }
        token_ns::remove_ext_braces(x);
        return the_parser.list_to_string_c(x, "Invalid key name");
    }

    // Interprets \usevalue{foo} in the list L
    void replace_pointers(TokenList &L) {
        TokenList res;
        int       n = 1000;
        for (;;) {
            if (L.empty()) break;
            Token tt = L.front();
            if (tt.is_a_left_brace()) {
                L.fast_get_block(res);
                continue;
            }
            L.pop_front();
            if (tt != hash_table.locate("usevalue")) {
                res.push_back(tt);
                continue;
            }
            if (!L.empty()) tt = L.front();
            if (!tt.is_a_left_brace()) {
                the_parser.parse_error(the_parser.err_tok, "Invalid \\usevalue token", "invalid usevalue");
                continue;
            }
            --n;
            if (n < 0) {
                the_parser.parse_error(the_parser.err_tok, "Replace pointer aborted, (infinite loop?)", "key loop");
                res.clear();
                break;
            }
            TokenList key = L.fast_get_block();
            token_ns::remove_ext_braces(key);
            std::string Key           = the_parser.list_to_string_c(key, "Argument of \\savevalue");
            auto        xkv_local_buf = "XKV@" + xkv_header + Key + "@value";
            if (hash_table.is_defined(xkv_local_buf)) {
                TokenList w = the_parser.get_mac_value(hash_table.last_tok);
                L.splice(L.begin(), w);
            } else {
                xkv_local_buf = "No value recorded for key `" + Key + "'; ignored";
                the_parser.parse_error(the_parser.err_tok, xkv_local_buf, "no val recorded");
            }
        }
        L.swap(res);
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
            if (std::find(Na.begin(), Na.end(), cur.keyname) != Na.end()) continue;
            if (idx >= 0 && std::find(Keys.begin(), Keys.end(), cur.keyname) != Keys.end()) continue;
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
                if (!set_all) break;
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

    // Evaluate now everything
    void XkvSetkeys::finish() {
        the_parser.new_macro(xkv_prefix, hash_table.locate("XKV@prefix"));
        the_parser.new_macro(fams, hash_table.locate("XKV@fams"));
        the_parser.new_macro(na, hash_table.locate("XKV@na"));
        if (!delayed.empty()) delayed.pop_back(); // remove trailing comma
        if (tracing_commands()) { spdlog::trace("setkeys <- {}", fmt::streamed(action)); }
        the_parser.new_macro(delayed, hash_table.locate("XKV@rm"));
        the_parser.back_input(action);
    }

    // Splits key=val into pieces
    void XkvToken::extract() {
        TokenList key;
        value   = initial;
        has_val = token_ns::split_at(Token(other_t_offset, '='), value, key);
        token_ns::remove_first_last_space(key);
        token_ns::remove_first_last_space(value);
        token_ns::remove_ext_braces(value);
        token_ns::remove_ext_braces(value);
        keyname   = xkv_find_key_of(key, 1);
        has_save  = xkv_is_save;
        is_global = xkv_is_global;
        token_ns::remove_first_last_space(value);
    }

    void XkvSetkeys::check_preset(std::string_view s) {
        auto N = Fams.size();
        for (size_t i = 0; i < N; i++) {
            auto xkv_local_buf = "XKV@" + xkv_makehd_s(Fams[i]) + std::string(s);
            if (hash_table.is_defined(xkv_local_buf)) {
                Token     T = hash_table.locate(xkv_local_buf);
                TokenList W = the_parser.get_mac_value(T);
                set_aux(W, to_signed(i));
            }
        }
    }

    // This considers the case when \ProcessOptionsX has seen cur_opt
    void XkvSetkeys::check_action(const XkvToken &cur) {
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

    void XkvSetkeys::run_default(const std::string &Key, Token mac, bool s) {
        auto xkv_local_buf = xkv_header + Key + "@default";
        if (!hash_table.is_defined(xkv_local_buf)) {
            xkv_local_buf = "No value specified for key `" + Key + "'";
            the_parser.parse_error(the_parser.err_tok, xkv_local_buf);
            return;
        }
        Token     T = hash_table.locate(xkv_local_buf);
        TokenList L = the_parser.get_mac_value(T);
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
        args.brace_me();
        more_action(args);
    }

    // Reads optional prefix, and family, and handles them
    void xkv_fetch_prefix() {
        auto opt = the_parser.read_optarg();
        if (!opt) {
            xkv_prefix = "KV@";
            return;
        }
        Buffer xkv_local_buf;
        token_ns::remove_first_last_space(*opt);
        if (the_parser.list_to_string(*opt, xkv_local_buf)) {
            the_parser.parse_error(the_parser.err_tok, "Bad command ", the_parser.cur_tok, " in XKV prefix (more errors may follow)",
                                   "bad kv prefix");
            xkv_local_buf.clear();
        }
        if (xkv_local_buf == "XKV") {
            the_parser.parse_error(the_parser.err_tok, "xkeyval: `XKV' prefix is not allowed");
            xkv_local_buf.clear();
        }
        if (!xkv_local_buf.empty()) xkv_local_buf.push_back('@');
        xkv_prefix = xkv_local_buf;
    }

    // Implements ExecuteOptionsX
    void xkv_execute_options() {
        XkvSetkeys data;
        xkv_fetch_prefix();
        data.special_fams();
        data.fetch_keys(true);
        data.dump_keys();
        data.set_aux();
        data.finish();
    }

    // Implements ProcessOptionsX
    void xkv_process_options() {
        XkvSetkeys data;
        //  data.no_err = remove_initial_plus(false);
        bool s = the_parser.remove_initial_star(); // we should do something with this
        xkv_fetch_prefix();
        data.set_inpox();
        data.special_fams();
        TokenList uo;
        TokenList L = classes_ns::cur_options(s, uo, false);
        L.brace_me();
        the_parser.back_input(L);
        data.fetch_keys(true);
        data.set_aux();
        data.finish();
    }

    // May set \XKV@prefix \XKV@fams \XKV@tfam \XKV@header \XKV@tkey \XKV@na
    void setkeys(bool c) {
        XkvSetkeys data;
        data.run(c);
    }

    // This deletes L from W. Here L is a simple list of keys
    void xkv_ns_remove(TokenList &W, TokenList &L, int type) {
        Buffer    B     = the_parser.list_to_string_c(L, ",", ",", "Invalid key name list");
        Token     comma = hash_table.comma_token;
        TokenList tmp;
        TokenList key;
        W.swap(tmp);
        while (!tmp.empty()) {
            token_ns::split_at(comma, tmp, key);
            std::string key_name      = xkv_find_key_of(key, type);
            auto        xkv_local_buf = "," + key_name + ",";
            if (B.find(xkv_local_buf) == std::string::npos) {
                if (!W.empty()) W.push_back(comma);
                W.splice(W.end(), key);
            }
        }
    }

    // This merges L into W; both lists have the form \global{key}=value
    // or, depenfing on the type, are lists of keys
    void xkv_ns_merge(TokenList &W, TokenList &L, int type) {
        Token     comma = hash_table.comma_token;
        TokenList key;
        TokenList tmp;
        if (W.empty()) {
            while (!L.empty()) {
                token_ns::split_at(comma, L, key);
                token_ns::remove_first_last_space(key);
                if (!W.empty()) W.push_back(comma);
                W.splice(W.end(), key);
            }
            return;
        }
        while (!L.empty()) {
            token_ns::split_at(comma, L, key);
            token_ns::remove_first_last_space(key);
            std::string key_name = xkv_find_key_of(key, type);
            bool        found    = false; // Was key in W ?
            TokenList   z;
            W.swap(tmp);
            while (!tmp.empty()) {
                token_ns::split_at(comma, tmp, z);
                std::string zname = xkv_find_key_of(z, type);
                if (!W.empty()) W.push_back(comma);
                if (key_name == zname) {
                    found = true; // replace in W old value z by new value
                    W.splice(W.end(), key);
                } else
                    W.splice(W.end(), z);
            }
            if (!found) {
                if (!W.empty()) W.push_back(comma);
                W.splice(W.end(), key); // Insert key at the end
            }
        }
    }

    // Merges or deletes depending on mg globally if gbl is true
    // the keys in L from the variable depending on type
    // Implements preset or save depending on type
    void xkv_merge(bool gbl, int type, TokenList &L, bool mg) {
        token_ns::sanitize_one(L, '=');
        token_ns::sanitize_one(L, ',');
        Token     T = hash_table.locate(xkv_find_aux(type));
        TokenList W = the_parser.get_mac_value(T);
        if (mg)
            xkv_ns_merge(W, L, type);
        else
            xkv_ns_remove(W, L, type);
        the_parser.new_macro(W, T, gbl);
    }

    // Implements \key@ifundefined
    void key_ifundefined() {
        xkv_fetch_prefix();
        TokenList   fams      = the_parser.read_arg();
        bool        undefined = true;
        TokenList   key       = the_parser.read_arg();
        std::string Key       = the_parser.list_to_string_c(key, "problem scanning key");
        std::string Fams      = the_parser.list_to_string_c(fams, "Problem with the families");
        std::string fam;
        for (const auto &f : split_commas(Fams)) {
            fam                = f;
            auto xkv_local_buf = xkv_prefix + fam;
            if (!fam.empty()) xkv_local_buf.push_back('@');
            xkv_local_buf.append(Key);
            if (hash_table.is_defined(xkv_local_buf)) {
                undefined = false;
                break;
            }
        }
        the_parser.new_macro(fam, hash_table.locate("XKV@tfam"));
        the_parser.one_of_two(undefined);
    }

    void lower_case(TokenList &L) {
        for (auto &a : L) {
            if (a.val < single_offset) {
                auto b  = a.chr_val();
                auto cx = eqtb_int_table[b + lc_code_offset].val;
                if (cx != 0) a = Token(a.val - b + to_unsigned(cx));
            }
        }
    }

    // Implements \XKV@cc
    void internal_choice_key() {
        bool      if_star = the_parser.remove_initial_plus(false);
        bool      if_plus = the_parser.remove_initial_plus(true);
        auto      bin     = the_parser.read_optarg_nopar().value_or(TokenList{});
        TokenList input   = the_parser.read_arg();
        TokenList allowed = the_parser.read_arg();
        TokenList ok_code = the_parser.read_arg();
        TokenList bad_code;
        if (if_plus) bad_code = the_parser.read_arg();
        if (if_star) {
            lower_case(input);
            lower_case(allowed);
        }
        Token relax = hash_table.relax_token;
        Token B1 = relax, B2 = relax;
        if (!bin.empty()) {
            B1 = bin.front();
            bin.pop_front();
        }
        if (!bin.empty()) {
            B2 = bin.front();
            bin.pop_front();
        }
        if (B1.not_a_cmd()) B1 = relax;
        if (B2.not_a_cmd()) B2 = relax;
        if (B1 != relax) {
            TokenList u = input;
            the_parser.new_macro(u, B1);
        }
        TokenList xinput = input;
        int       k      = 0;
        bool      found  = allowed.find(xinput, hash_table.comma_token, false, k);
        if (B2 != relax) {
            TokenList u = Buffer{fmt::format("{}", k)}.str_toks(nlt_cr); // Should be irrelevant ?
            the_parser.new_macro(u, B2);
        }
        if (found)
            the_parser.back_input(ok_code);
        else if (if_plus) // invalid, but catched
            the_parser.back_input(bad_code);
        else {
            the_parser.parse_error(the_parser.err_tok, "XKV: value is not allowed");
            spdlog::error(" {}", fmt::streamed(input));
        }
    }

    void internal_define_key_default(const std::string &s, Token T, TokenList &L) {
        L.brace_me();
        L.push_front(T);
        the_parser.cur_tok = hash_table.locate(s + "@default");
        the_parser.new_macro(L, the_parser.cur_tok);
    }

    // This is like \def\T#1, optimised
    void internal_define_key(Token T) {
        auto *X = new Macro;
        X->set_nbargs(1);
        X->set_type(dt_normal);
        the_parser.read_mac_body(X->body, false, 1);
        X->correct_type();
        the_parser.mac_define(T, X, false, rd_always, user_cmd);
    }

    void xkv_makehd(TokenList &L) {
        token_ns::remove_first_last_space(L);
        Buffer xkv_local_buf{xkv_prefix};
        auto   k = xkv_local_buf.size();
        if (the_parser.list_to_string(L, xkv_local_buf)) {
            the_parser.parse_error(the_parser.err_tok, "Bad command ", the_parser.cur_tok, " in XKV family (more errors may follow)",
                                   "bad kv family");
            xkv_local_buf.resize(k);
        }
        if (xkv_local_buf.size() != k) xkv_local_buf.push_back('@');
        xkv_header = xkv_local_buf;
    }

    void xkv_declare_option() {
        // \let\@fileswith@pti@ns\@badrequireerror
        bool star = the_parser.remove_initial_star();
        if (star) {
            the_parser.T_declare_option_star();
            return;
        }
        xkv_fetch_prefix();
        TokenList FL = the_parser.XKV_parse_filename();
        xkv_makehd(FL);
        TokenList   key  = the_parser.read_arg();
        TokenList   xkey = key;
        std::string Key  = the_parser.list_to_string_c(xkey, "Invalid option");
        classes_ns::register_key(Key);
        auto  xkv_local_buf = the_parser.list_to_string_c(key, xkv_header, "", "bad option name");
        Token T             = hash_table.locate(xkv_local_buf);
        auto  opt           = the_parser.read_optarg().value_or(TokenList{});
        internal_define_key_default(xkv_local_buf, T, opt);
        internal_define_key(T);
    }

    void xkv_fetch_prefix_family() {
        xkv_fetch_prefix();
        TokenList M = the_parser.read_arg();
        xkv_makehd(M);
    }

    // Find saved or preset keys, depending on c2. If not found:
    // signals a an error if c is true (creates otherwise), return true.
    // Creates cur_tok if needed
    auto xkv_save_keys_aux(bool c, int c2) -> bool {
        xkv_fetch_prefix_family();
        auto xkv_local_buf = xkv_find_aux(c2);
        bool ret           = !hash_table.is_defined(xkv_local_buf);
        if (c && ret) {
            xkv_local_buf = (c2 != 0 ? "No presets defined for `" : "No save keys defined for `") + xkv_header + "'";
            the_parser.parse_error(the_parser.err_tok, xkv_local_buf);
            return true;
        }
        the_parser.cur_tok = hash_table.last_tok;
        return ret;
    }

    void disable_keys() {
        xkv_fetch_prefix_family(); // read prefix and family
        TokenList   keys = the_parser.read_arg();
        std::string Keys = the_parser.list_to_string_c(keys, "problem scanning keys");
        for (const auto &Key : split_commas(Keys)) {
            Buffer xkv_local_buf{xkv_header + Key};
            if (hash_table.is_defined(xkv_local_buf)) {
                Token T = hash_table.last_tok;
                xkv_local_buf.append("@default");
                if (hash_table.is_defined(xkv_local_buf)) {
                    TokenList L;
                    L.brace_me();
                    L.push_front(T);
                    the_parser.new_macro(L, hash_table.last_tok);
                }
                xkv_local_buf = "Key `" + Key + "' has been disabled";
                TokenList L   = xkv_local_buf.str_toks(nlt_space); // should be irrelevant
                L.brace_me();
                L.push_front(hash_table.locate("XKV@warn"));
                auto *X = new Macro(L);
                X->set_nbargs(1);
                X->set_type(dt_normal);
                the_parser.mac_define(T, X, false, rd_always, user_cmd);
            } else
                the_parser.parse_error(the_parser.err_tok, "Undefined key cannot be disabled: ", Key, "");
        }
    }

    void define_bool_key(subtypes c) {
        the_parser.remove_initial_plus(false);
        bool if_plus = the_parser.remove_initial_plus(true);
        if (c != define_boolkey_code) if_plus = false;
        xkv_fetch_prefix_family(); // read prefix and family
        Buffer xkv_local_buf;
        if (auto L = the_parser.read_optarg_nopar()) {
            xkv_local_buf = the_parser.list_to_string_c(*L, "", "", "Problem scanning macro prefix");
        } else
            xkv_local_buf = xkv_header;
        std::string mp      = xkv_local_buf;
        TokenList   keytoks = the_parser.read_arg();
        auto        dft     = the_parser.read_optarg_nopar();
        // construct the key or key list
        std::string Keys = the_parser.list_to_string_c(keytoks, "Problem scanning key");
        for (const auto &Key : split_commas(Keys)) {
            if (Key.empty()) continue;
            xkv_local_buf = mp + Key;
            TokenList u   = xkv_local_buf.str_toks11(false);
            xkv_local_buf = "if" + mp + Key;
            the_parser.back_input(hash_table.locate(xkv_local_buf));
            the_parser.M_newif();
            xkv_local_buf = "true,false";
            TokenList v   = xkv_local_buf.str_toks11(false);
            xkv_local_buf = xkv_header + Key;
            Token T       = hash_table.locate(xkv_local_buf);
            if (dft) { internal_define_key_default(xkv_local_buf, T, *dft); }
            u.push_front(hash_table.csname_token);
            u.push_back(hash_table.locate("XKV@resa"));
            u.push_back(hash_table.endcsname_token);
            if (c == define_boolkey_code) {
                TokenList add1 = the_parser.read_arg();
                u.splice(u.end(), add1);
            }
            u.brace_me();
            TokenList LL;
            LL.splice(LL.end(), u);
            if (if_plus) {
                TokenList add2 = the_parser.read_arg();
                add2.brace_me();
                LL.splice(LL.end(), add2);
            }
            v.brace_me();
            LL.splice(LL.begin(), v);
            LL.push_front(hash_table.CB_token);
            LL.push_front(Token(other_t_offset, '1'));
            LL.push_front(make_char_token('#', 6));
            LL.push_front(hash_table.OB_token);
            LL.push_front(Token(other_t_offset, ']'));
            LL.push_front(hash_table.locate("XKV@resa"));
            LL.push_front(Token(other_t_offset, '['));
            if (if_plus) LL.push_front(Token(other_t_offset, '+'));
            LL.push_front(Token(other_t_offset, '*'));
            LL.push_front(hash_table.locate("XKV@cc"));
            LL.brace_me();
            the_parser.back_input(LL);
            auto *X = new Macro;
            X->set_nbargs(1);
            X->set_type(dt_normal);
            the_parser.read_mac_body(X->body, false, 1);
            X->correct_type();
            the_parser.mac_define(T, X, false, rd_always, user_cmd);
        }
    }

    void T_define_key() {
        xkv_fetch_prefix_family();
        TokenList key           = the_parser.read_arg();
        auto      xkv_local_buf = the_parser.list_to_string_c(key, xkv_header, "", "bad key name");
        Token     T             = hash_table.locate(xkv_local_buf);
        if (auto opt = the_parser.read_optarg()) internal_define_key_default(xkv_local_buf, T, *opt);
        internal_define_key(T);
    }

    void define_choice_key() {
        bool if_star = the_parser.remove_initial_plus(false);
        bool if_plus = the_parser.remove_initial_plus(true);
        xkv_fetch_prefix_family();
        TokenList keytoks       = the_parser.read_arg();
        auto      xkv_local_buf = the_parser.list_to_string_c(keytoks, xkv_header, "", "bad key name");
        Token     T             = hash_table.locate(xkv_local_buf);
        auto      storage_bin   = the_parser.read_optarg_nopar().value_or(TokenList{});
        TokenList allowed       = the_parser.read_arg();
        if (auto opt = the_parser.read_optarg()) internal_define_key_default(xkv_local_buf, T, *opt);
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
        internal_define_key(T);
    }

    // We make the assumption that a key does not contain a comma
    void define_cmd_key(subtypes c) {
        xkv_fetch_prefix_family(); // read prefix and family
        std::string xkv_local_buf;
        if (auto L = the_parser.read_optarg_nopar()) {
            xkv_local_buf = the_parser.list_to_string_c(*L, "", "", "Problem scanning macro prefix");
        } else
            xkv_local_buf = "cmd" + xkv_header;
        std::string mp      = xkv_local_buf;
        TokenList   keytoks = the_parser.read_arg();
        auto        opt     = the_parser.read_optarg();
        // construct the key or key list
        std::string Keys = the_parser.list_to_string_c(keytoks, "problem scanning key");
        for (const auto &Key : split_commas(Keys)) {
            if (Key.empty()) continue;
            xkv_local_buf = mp + Key;
            Token cmd     = hash_table.locate(xkv_local_buf);
            xkv_local_buf = xkv_header + Key;
            Token T       = hash_table.locate(xkv_local_buf);
            if (opt) {
                TokenList D = *opt;
                internal_define_key_default(xkv_local_buf, T, D);
            }
            TokenList LL;
            if (c == define_cmdkey_code) { // case of cmdkey
                TokenList u = the_parser.read_arg();
                LL.splice(LL.end(), u);
            }
            LL.push_front(hash_table.CB_token);
            LL.push_front(Token(other_t_offset, '1'));
            LL.push_front(make_char_token('#', 6));
            LL.push_front(hash_table.OB_token);
            LL.push_front(cmd);
            LL.push_front(hash_table.locate("def"));
            LL.brace_me();
            the_parser.back_input(LL);
            auto *X = new Macro;
            X->set_nbargs(1);
            X->set_type(dt_normal);
            the_parser.read_mac_body(X->body, false, 1);
            X->correct_type();
            the_parser.mac_define(T, X, false, rd_always, user_cmd);
        }
    }

    void T_xkeyval(subtypes c) {
        switch (c) {
        case boot_keyval_code: return;
        case xdefinekey_code: T_define_key(); return;
        case define_cmdkey_code:
        case define_cmdkeys_code: define_cmd_key(c); return;
        case define_boolkey_code:
        case define_boolkeys_code: define_bool_key(c); return;
        case define_choicekey_code: define_choice_key(); return;
        case define_cc_code: internal_choice_key(); return;
        case disable_keys_code: disable_keys(); return;
        case key_ifundefined_code: key_ifundefined(); return;
        case save_keys_code:
        case gsave_keys_code: {
            xkv_save_keys_aux(false, 0);
            TokenList A = the_parser.read_arg();
            xkv_merge(c == gsave_keys_code, 0, A, true);
            return;
        }
        case delsave_keys_code:
        case gdelsave_keys_code: {
            bool      bad = xkv_save_keys_aux(false, 0);
            TokenList A   = the_parser.read_arg();
            if (!bad) xkv_merge(c == gdelsave_keys_code, 0, A, false);
            return;
        }
        case unsave_keys_code:
        case gunsave_keys_code:
            if (xkv_save_keys_aux(true, 0)) return;
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunsave_keys_code);
            return;
        case preset_keys_code:
        case gpreset_keys_code: {
            xkv_save_keys_aux(false, 1);
            TokenList A = the_parser.read_arg();
            TokenList B = the_parser.read_arg();
            xkv_merge(c == gpreset_keys_code, 1, A, true);
            xkv_merge(c == gpreset_keys_code, 2, B, true);
            return;
        }
        case delpreset_keys_code:
        case gdelpreset_keys_code: {
            bool      bad = xkv_save_keys_aux(true, 1);
            TokenList A   = the_parser.read_arg();
            TokenList B   = the_parser.read_arg();
            if (bad) return;
            xkv_merge(c == gdelpreset_keys_code, 1, A, false);
            xkv_merge(c == gdelpreset_keys_code, 2, B, false);
            return;
        }
        case unpreset_keys_code:
        case gunpreset_keys_code: {
            if (xkv_save_keys_aux(true, 1)) return;
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
            the_parser.cur_tok = hash_table.locate(xkv_find_aux(2));
            the_parser.M_let_fast(the_parser.cur_tok, hash_table.frozen_undef_token, c == gunpreset_keys_code);
            return;
        }
        case setrmkeys_code: setkeys(false); return;
        case setkeys_code: setkeys(true); return;
        case declare_optionsX_code: xkv_declare_option(); return;
        case process_optionsX_code: xkv_process_options(); return;
        case execute_optionsX_code: xkv_execute_options(); return;
        default: return;
        }
    }

    auto is_Gin(const TokenList &x) -> bool {
        auto C = x.begin();
        auto E = x.end();
        if (C == E) return false;
        if (*C != Token(letter_t_offset, 'G')) return false;
        ++C;
        if (C == E) return false;
        if (*C != Token(letter_t_offset, 'i')) return false;
        ++C;
        if (C == E) return false;
        if (*C != Token(letter_t_offset, 'n')) return false;
        ++C;
        return C == E;
    }

    // Extract the keys from a list, result in the vector R
    // Will store a normalised version of the list in L
    void extract_keys(TokenList &L, std::vector<std::string> &R) {
        if (L.empty()) return;
        Token     T = hash_table.comma_token;
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
                std::string s = xkv_find_key_of(z, 1);
                R.push_back(s);
                res.splice(res.end(), z);
                res.push_back(T);
            } else
                z.push_back(x);
        }
        if (!res.empty()) res.pop_back(); // remove final comma
        L.swap(res);
    }

    // Returns true if must be saved; may set xkv_is_global
    auto XkvToken::check_save() const -> bool {
        if (has_save) {
            xkv_is_global = is_global;
            return true;
        }
        auto xkv_local_buf = xkv_find_aux(0);
        if (!hash_table.is_defined(xkv_local_buf)) return false;

        TokenList W = the_parser.get_mac_value(hash_table.locate(xkv_local_buf));
        TokenList key;
        while (!W.empty()) {
            token_ns::split_at(hash_table.comma_token, W, key);
            std::string key_name = xkv_find_key_of(key, 0);
            if (key_name == keyname) return true;
        }
        return false;
    }

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
        if (tracing_commands()) { spdlog::trace("setkeys -> {}", fmt::streamed(keyvals)); }
        extract_keys(keyvals, Keys);
    }

    void XkvSetkeys::dump_keys() { spdlog::trace("{{Options to execute->{}}}", fmt::streamed(keyvals)); }

    void XkvSetkeys::run(bool c) {
        no_err  = the_parser.remove_initial_plus(false);
        set_all = the_parser.remove_initial_plus(true);
        xkv_fetch_prefix();
        fams = the_parser.read_arg();
        if (is_Gin(fams)) {
            TokenList L = the_parser.read_arg();
            L.push_back(hash_table.comma_token);
            the_parser.new_macro(L, hash_table.locate("Gin@keys"));
            return;
        }
        extract_keys(fams, Fams);
        if (auto L = the_parser.read_optarg_nopar()) na.append(*L);
        extract_keys(na, Na);
        fetch_keys(c);
        check_preset("preseth");
        set_aux(keyvals, -1);
        check_preset("presett");
        finish();
    }

    // Constructs the header, in xkv_header
    // Constructs the 3 macros in action
    void XkvToken::prepare(const std::string &fam) {
        Hashtab &H = hash_table;
        // We start constructing the three macros
        action.push_back(H.def_token);
        action.push_back(H.locate("XKV@tkey"));
        Buffer    xkv_local_buf{keyname};
        TokenList L = xkv_local_buf.str_toks11(false);
        L.brace_me();
        action.splice(action.end(), L);
        action.push_back(H.def_token);
        action.push_back(H.locate("XKV@tfam"));
        xkv_local_buf = fam;
        L             = xkv_local_buf.str_toks11(false);
        L.brace_me();
        action.splice(action.end(), L);
        action.push_back(H.def_token);
        action.push_back(H.locate("XKV@header"));
        xkv_local_buf = xkv_makehd_s(fam);
        L             = xkv_local_buf.str_toks11(false);
        L.brace_me();
        action.splice(action.end(), L);
    }

    // Returns true if the key is defined
    auto XkvToken::is_defined(const std::string &fam) const -> bool { return hash_table.is_defined(xkv_makehd_s(fam) + keyname); }

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
