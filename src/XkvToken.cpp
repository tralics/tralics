#include "tralics/XkvToken.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace xkv_ns {
    void find_aux(int c);
    void makehd(const std::string &fam);
} // namespace xkv_ns

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
    action.push_back(H.locate("XKV@tkey"));
    B           = keyname;
    TokenList L = B.str_toks11(false);
    L.brace_me();
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.locate("XKV@tfam"));
    B = fam;
    L = B.str_toks11(false);
    L.brace_me();
    action.splice(action.end(), L);
    action.push_back(H.def_token);
    action.push_back(H.locate("XKV@header"));
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
