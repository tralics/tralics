#include "tralics/Parser.h"
#include <utf8.h>

namespace {
    auto single_char(const std::string &s) -> codepoint {
        auto it = s.begin();
        auto cp = utf8::next(it, s.end());
        return it == s.end() ? codepoint(cp) : codepoint(); // \todo the test should be it != end(), size() is wrong
    }
} // namespace

auto Hashtab::locate(const std::string &s) -> Token {
    if (s.empty()) return Token(null_tok_val);
    if (s.size() == 1) return Token(uchar(s[0]) + single_offset);
    if (auto c = single_char(s); c.non_null()) return Token(c.value + single_offset);
    return Token(hash_find(s) + hash_offset);
}

// Returns the hash location of the name in the buffer.
// If a new slot has to be created, uses the string name, if not empty.
auto Hashtab::hash_find(const std::string &s) -> size_t {
    if (auto i = map.find(s); i != map.end()) return i->second;
    usage_normal++;
    push_back(s);
    map.emplace(s, size() - 1);
    return size() - 1;
}

// Defines the command named a, but hash_find will not find it.
// This must be used at bootstrap code.
auto Hashtab::nohash_primitive(const std::string &a, CmdChr b) -> Token {
    usage_unhashed++;
    push_back(a);
    auto t = size() - 1 + hash_offset;
    eqtb[t - eqtb_offset].special_prim(b); // allows to define an undefined command
    return Token(t);
}

// This returns true if the token associated to the string in the buffer
// exists in the hash table and is not undefined.
// Sets last_tok to the result
auto Hashtab::is_defined(const Buffer &b) -> bool {
    auto i = map.find(b);
    if (i == map.end()) return false;

    size_t T = 0;
    if (b.empty())
        T = null_tok_val;
    else if (auto c = b.single_character(); c.non_null())
        T = c.value + single_offset;
    else
        T = i->second + hash_offset;

    last_tok = Token(T);
    return !eqtb[T - eqtb_offset].is_undef();
}

// Creates a primitive.
auto Hashtab::primitive(const std::string &s, symcodes c, subtypes v) -> Token {
    Token res = locate(s);
    eqtb[res.eqtb_loc()].primitive(CmdChr(c, v));
    return res;
}

// \global\let\firststring = \secondstring
auto Hashtab::eval_let(const std::string &a, const std::string &b) -> Token {
    Token Av   = locate(a);
    auto  A    = Av.eqtb_loc();
    auto  Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], true);
    return Av;
}

// \let\firststring = \secondstring
// Both strings must be permanent strings
void Hashtab::eval_let_local(const std::string &a, const std::string &b) {
    auto A    = locate(a).eqtb_loc();
    auto Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], false);
}

Hashtab::Hashtab() {
    for (auto &k : eqtb) k.cmd = undef_cmd;
    boot_base();
}
