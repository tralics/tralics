#pragma once
#include <string>

class Xml;

struct Indexer {
    std::string key; // sort key
    std::string aux;
    Xml *       translation;
    int         level;
    size_t      iid; // index in the reference table

    Indexer(std::string k, std::string a, Xml *x, int l, size_t u)
        : key(std::move(k)), aux(std::move(a)), translation(x), level(l), iid(u) {}

    [[nodiscard]] auto is_same(int l, const std::string &k) const -> bool { return level == l && aux == k; }
};
