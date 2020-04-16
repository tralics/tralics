#include "tralics/StrHash.h"
#include "txhash.h"
#include "txparser.h"
#include <fmt/format.h>

StrHash::StrHash() : std::vector<StrHash_record>(hash_size) {
    next.resize(size(), 0);
    at(0) = {"", "", nullptr};
    at(1) = {"", "", nullptr};
    at(2) = {" ", " ", nullptr};
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
    for (;; p = next[p]) {
        if ((at(p).name != nullptr) && s == at(p).name) return p;
        if (next[p] == 0) break;
    }
    the_parser.my_stats.one_more_sh_used();
    if (at(p).name != nullptr) {
        if (hash_last >= size()) {
            resize(size() + 10'000);
            next.resize(size(), 0);
        }
        auto k = hash_last;
        hash_last++;
        next[p] = k;
        p       = k;
    }
    at(p).name  = (new std::string(s))->c_str();
    at(p).value = Buffer(s).convert_to_out_encoding();
    return p;
}
