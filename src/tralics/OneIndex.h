#pragma once
#include <string>
#include <vector>

class Xml;

struct Indexer {
    std::string key; // sort key
    std::string aux;
    Xml        *translation;
    int         level;
    size_t      iid; // index in the reference table

    [[nodiscard]] auto is_same(int l, const std::string &k) const -> bool { return level == l && aux == k; }
};

struct OneIndex : public std::vector<Indexer> { // \todo unordered_map
    std::string name;
    std::string title;
    Xml        *AL;         // Pre-allocated element for index attributes
    Xml        *position{}; // Position on the XML of the index

    OneIndex(std::string a, std::string b, Xml *c) : name(std::move(a)), title(std::move(b)), AL(c) {}
};
