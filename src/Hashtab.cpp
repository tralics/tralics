#include "tralics/Dispatcher.h"
#include "tralics/Parser.h"
#include "tralics/Symcode.h"
#include <utf8.h>

namespace {
    auto single_char(const std::string &s) -> std::optional<char32_t> {
        auto it = s.begin();
        auto cp = utf8::next(it, s.end());
        if (it == s.end()) return char32_t(cp);
        return {};
    }
} // namespace

auto Hashtab::the_map() -> std::unordered_map<std::string, size_t> & {
    static std::unordered_map<std::string, size_t> m;
    return m;
}

auto Hashtab::the_eqtb() -> std::unordered_map<size_t, EqtbCmdChr> & {
    static std::unordered_map<size_t, EqtbCmdChr> m;
    return m;
}

auto Hashtab::locate(std::string_view s) -> Token {
    if (s.empty()) return Token(null_tok_val);
    if (s.size() == 1) return Token(uchar(s[0]) + single_offset);
    if (auto c = single_char(std::string(s))) return Token(*c + single_offset);
    return Token(hash_find(std::string(s)) + hash_offset);
}

// Returns the hash location of the name in the buffer.
// If a new slot has to be created, uses the string name, if not empty.
auto Hashtab::hash_find(const std::string &s) -> size_t {
    static auto &map = the_map();
    if (auto i = map.find(s); i != map.end()) return i->second;
    push_back(s);
    map.emplace(s, size() - 1);
    return size() - 1;
}

// Defines the command named a, but hash_find will not find it.
// This must be used at bootstrap code.
auto Hashtab::nohash_primitive(const std::string &a, CmdChr b) -> Token {
    push_back(a);
    auto t                          = size() - 1 + hash_offset;
    the_eqtb()[Token(t).eqtb_loc()] = {b, b.is_undef() ? 0 : 1}; // allows to define an undefined command
    return Token(t);
}

// This returns true if the token associated to the string in the buffer
// exists in the hash table and is not undefined.
// Sets last_tok to the result
auto Hashtab::is_defined(const std::string &b) -> bool {
    static auto &map = the_map();
    auto         i   = map.find(b);
    if (i == map.end()) return false;

    size_t T = 0;
    if (b.empty())
        T = null_tok_val;
    else if (auto c = single_char(b))
        T = *c + single_offset;
    else
        T = i->second + hash_offset;

    last_tok = Token(T);
    return !the_eqtb()[Token(T).eqtb_loc()].val.is_undef();
}

// Creates a primitive.
auto Hashtab::primitive(const std::string &s, symcodes c, subtypes v) -> Token {
    Token res                  = locate(s);
    the_eqtb()[res.eqtb_loc()] = {{c, v}, 1};
    return res;
}

auto Hashtab::primitive_plain(const std::string &s, symcodes c, subtypes v) -> Token {
    Symcode::get(c).name_sub[v] = s;
    return primitive(s, c, v);
}

auto Hashtab::primitive_plain(const std::string &s, symcodes c) -> Token {
    Symcode::get(c).name_str = s;
    return primitive(s, c, zero_code);
}

auto Hashtab::primitive_and_action_plain(const std::string &s, const std::function<void()> &f, symcodes c) -> Token {
    Dispatcher::register_action_plain(c, f);
    return primitive_plain(s, c);
}

auto Hashtab::primitive_and_action_plain(const std::string &s, const std::function<void()> &f, symcodes c, subtypes v) -> Token {
    Dispatcher::register_action_plain(c, f);
    return primitive_plain(s, c, v);
}

auto Hashtab::primitive_and_action_plain(const std::string &s, const std::function<void(subtypes)> &f, symcodes c, subtypes v) -> Token {
    Dispatcher::register_action_plain(c, f);
    return primitive_plain(s, c, v);
}

// \global\let\firststring = \secondstring
auto Hashtab::eval_let(const std::string &a, const std::string &b) -> Token {
    Token Av   = locate(a);
    auto  A    = Av.eqtb_loc();
    auto  Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, the_eqtb()[Bval].val, true);
    return Av;
}
