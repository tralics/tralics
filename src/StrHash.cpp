#include "tralics/StrHash.h"
#include "txhash.h"
#include "txparser.h"
#include <fmt/format.h>

auto StrHash::labinfo(size_t k) -> LabelInfo * {
    if (at(k).labinfo == nullptr) at(k).labinfo = new LabelInfo(Istring(k));
    return at(k).labinfo;
}

// Find something in the StrHash table
auto StrHash::find_or_insert(const std::string &s) -> size_t {
    if (auto tmp = s_to_i.find(s); tmp != s_to_i.end()) return tmp->second;
    s_to_i.emplace(s, size());
    push_back({s, Buffer(s).convert_to_out_encoding()});
    return size() - 1;
}
