#include "tralics/StrHash.h"
#include "txhash.h"
#include "txparser.h"
#include <fmt/format.h>

StrHash::StrHash() : std::vector<StrHash_record>(hash_size) {
    at(0) = {"", "", nullptr, 0};
    at(1) = {"", "", nullptr, 0};
    at(2) = {" ", " ", nullptr, 0};
}

auto StrHash::labinfo(size_t k) -> LabelInfo * {
    if (at(k).labinfo == nullptr) at(k).labinfo = new LabelInfo(Istring(k));
    return at(k).labinfo;
}

// Find something in the StrHash table
auto StrHash::find_or_insert(const std::string &s) -> size_t {
    the_parser.my_stats.one_more_sh_find();
    if (s.empty()) return 1;
    auto p = Buffer(s).hashcode(hash_prime) + 3; // skip the special values 0, 1 and 2
    for (;;) {
        if ((at(p).name != nullptr) && s == at(p).name) return p;
        if (at(p).next == 0) break;
        p = at(p).next;
    }
    String name  = (new std::string(s))->c_str();
    auto   value = Buffer(s).convert_to_out_encoding();
    if (at(p).name == nullptr) {
        the_parser.my_stats.one_more_sh_used();
        at(p).name  = name;
        at(p).value = value;
        return p;
    }
    if (hash_last >= size()) resize(size() + 10'000); // \todo simplify using push_back
    auto k = hash_last;
    hash_last++;
    the_parser.my_stats.one_more_sh_used();
    at(k).name  = name;
    at(k).value = value;
    at(p).next  = k;
    return k;
}
