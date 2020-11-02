#pragma once
#include "Indexer.h"
#include <string>
#include <vector>

class Xml;

class OneIndex {
    std::string            name;
    std::string            title;
    Xml *                  position{}; // Position on the XML of the index
    size_t                 AL;         // The attribute list index
    std::vector<Indexer *> value;

public:
    OneIndex(std::string a, std::string b, size_t c) : name(std::move(a)), title(std::move(b)), AL(c) {}

    [[nodiscard]] auto size() const { return value.size(); }
    [[nodiscard]] auto get_title() const -> const std::string & { return title; }
    [[nodiscard]] auto get_position() const -> Xml * { return position; }
    [[nodiscard]] auto get_translation(size_t i) const -> Xml * { return value[i]->translation; }
    [[nodiscard]] auto get_iid(size_t i) const -> size_t { return value[i]->iid; }
    [[nodiscard]] auto has_name(const std::string &s) const -> bool { return name == s; }
    [[nodiscard]] auto get_AL() const -> size_t { return AL; }

    void set_position(Xml *x) { position = x; }
    auto get_data() -> std::vector<Indexer *> & { return value; }
    void do_sort() {
        std::sort(value.begin(), value.end(), [](auto A, auto B) { return A->key < B->key; });
    }
};
