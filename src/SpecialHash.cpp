#include "tralics/SpecialHash.h"
#include "tralics/util.h"
#include "txhash.h"
#include "txinline.h"
#include "txparser.h"

SpecialHash::SpecialHash(const std::string &str) {
    for (const auto &s : split_commas(str)) kv.push_back(split_assign(s));
}

// Return the value associated to the key x, or empty string if not found.
auto SpecialHash::find(const std::string &x) const -> std::string {
    for (const auto &i : kv)
        if (i.first == x) return i.second;
    return "";
}
