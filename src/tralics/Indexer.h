#pragma once
#include <string>

class Xml;

struct Indexer {
    std::string key; // sort key
    std::string aux;
    Xml *       translation;
    int         level;
    size_t      iid; // index in the reference table

    [[nodiscard]] auto is_same(int l, const std::string &k) const -> bool { return level == l && aux == k; }
};
