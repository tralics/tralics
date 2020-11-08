#include "tralics/XkvToken.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace xkv_ns {
    void find_aux(int c);
    auto find_key_of(const TokenList &L, int type) -> std::string;
    void makehd(const std::string &fam);
} // namespace xkv_ns

// Splits key=val into pieces
void XkvToken::extract() {
    TokenList key;
    value   = initial;
    has_val = token_ns::split_at(Token(other_t_offset, '='), value, key);
    token_ns::remove_first_last_space(key);
    token_ns::remove_first_last_space(value);
    token_ns::remove_ext_braces(value);
    token_ns::remove_ext_braces(value);
    keyname   = xkv_ns::find_key_of(key, 1);
    has_save  = xkv_is_save;
    is_global = xkv_is_global;
    token_ns::remove_first_last_space(value);
}

// True if the key is in the ignore list
auto XkvToken::ignore_this(std::vector<std::string> &igna) const -> bool {
    for (auto &i : igna) // \todo std::any_of
        if (keyname == i) return true;
    return false;
}

// Constructs the header, in xkv_header
// Constructs the 3 macros in action
void XkvToken::prepare(const std::string &fam) {
    Hashtab &H = hash_table;
    // We start constructing the three macros
    Buffer &B = txparser2_local_buf;
    action.push_back(H.def_token);
    action.push_back(H.xkv_tkey_token);
    B           = keyname;
    TokenList L = B.str_toks11(false);
    L.brace_me();
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.xkv_tfam_token);
    B = fam;
    L = B.str_toks11(false);
    L.brace_me();
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.xkv_header_token);
    xkv_ns::makehd(fam);
    L = B.str_toks11(false);
    L.brace_me();
    action.splice(action.end(), L);
}

// Returns true if the key is defined
auto XkvToken::is_defined(const std::string &fam) const -> bool {
    xkv_ns::makehd(fam);
    txparser2_local_buf += keyname;
    return hash_table.is_defined(txparser2_local_buf);
}

// Returns true if must be saved; may set xkv_is_global
auto XkvToken::check_save() const -> bool {
    if (has_save) {
        xkv_is_global = is_global;
        return true;
    }
    xkv_ns::find_aux(0);
    if (!hash_table.is_defined(txparser2_local_buf)) return false;
    return the_parser.find_a_save_key(keyname);
}
