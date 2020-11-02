#pragma once
#include "Indexer.h"
#include <string>
#include <vector>

class Xml;

struct OneIndex : public std::vector<Indexer> {
    std::string name;
    std::string title;
    size_t      AL;         // The attribute list index
    Xml *       position{}; // Position on the XML of the index

    OneIndex(std::string a, std::string b, size_t c) : name(std::move(a)), title(std::move(b)), AL(c) {}
};
