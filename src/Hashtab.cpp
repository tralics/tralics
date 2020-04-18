#include "txlogger.h"
#include "txparser.h"

auto Hashtab::locate(const std::string &s) -> Token {
    if (s.size() == 1) return Token(uchar(s[0]) + single_offset);
    return locate(Buffer(s));
}

// This returns the token associated to the string in the buffer.
auto Hashtab::locate(const Buffer &b) -> Token {
    if (b.empty()) return Token(null_tok_val);
    codepoint c = b.single_character();
    if (c.non_null()) return Token(c.value + single_offset);
    return Token(hash_find(b.to_string()) + hash_offset);
}

// Returns the hash location of the name in the buffer.
// If a new slot has to be created, uses the string name, if not empty.
auto Hashtab::hash_find(const std::string &s) -> size_t {
    if (auto i = map.find(s); i != map.end()) return i->second;
    auto p = Buffer(s).hashcode(hash_prime);
    while (Next[p] != 0) p = Next[p];

    if (at(p)) {
        for (;;) {
            hash_used--;
            if (hash_used <= 0) {
                log_and_tty << "Size of hash table is " << hash_size << "\n";
                log_and_tty << "Value of hash_prime is " << hash_prime << "\n";
                log_and_tty << "hash table full\n" << lg_fatal;
                abort();
            }
            if (!at(hash_used)) break;
        }
        at(hash_used) = s;
        hash_bad++;
        Next[p] = hash_used;
        map.try_emplace(s, hash_used);
        return hash_used;
    }
    hash_usage++;
    at(p) = s;
    map.try_emplace(s, p);
    return p;
}

// Defines the command named a, but hash_find will not find it.
// This must be used at bootstrap code.
auto Hashtab::nohash_primitive(const std::string &a, CmdChr b) -> Token {
    hash_used--;
    auto p = hash_used;
    assert(!at(p));
    assert(p >= hash_prime);
    hash_bad++;
    at(p)  = a;
    auto t = p + hash_offset;
    eqtb[t - eqtb_offset].special_prim(b); // allows to define an undefined command
    return Token(t);
}

// This returns true if the token associated to the string in the buffer
// exists in the hash table and is not undefined.
// Sets last_tok to the result
auto Hashtab::is_defined(const Buffer &b) -> bool {
    auto i = map.find(b.to_string());
    if (i == map.end()) return false;

    size_t T = 0;
    if (b.empty())
        T = null_tok_val;
    else {
        codepoint c = b.single_character();
        if (c.non_null())
            T = c.value + single_offset;
        else {
            T = i->second + hash_offset;
        }
    }
    last_tok = Token(T);
    return !eqtb[T - eqtb_offset].is_undef();
}

// Creates a primitive.
// The string s must be a permanent string
auto Hashtab::primitive(String s, symcodes c, subtypes v) -> Token {
    Token res = locate(s);
    auto  w   = res.eqtb_loc();
    eqtb[w].primitive(CmdChr(c, v));
    return res;
}

// \global\let\firststring = \secondstring
// Both strings must be permanent strings
void Hashtab::eval_let(String a, String b) {
    auto A    = locate(a).eqtb_loc();
    auto Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], true);
}

auto Hashtab::eval_letv(String a, String b) -> Token {
    Token Av   = locate(a);
    auto  A    = Av.eqtb_loc();
    auto  Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], true);
    return Av;
}

// \let\firststring = \secondstring
// Both strings must be permanent strings
void Hashtab::eval_let_local(String a, String b) {
    auto A    = locate(a).eqtb_loc();
    auto Bval = locate(b).eqtb_loc();
    the_parser.eq_define(A, eqtb[Bval], false);
}

// This is the BIG function.
// It enters all primitives in the hash table.
Hashtab::Hashtab() {
    for (auto &k : eqtb) k.reset();
    boot_base();
}
