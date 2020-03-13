#pragma once
// -*- C++ -*-
// Copyright (c)  INRIA/apics (Jose' Grimm) 2002, 2004, 2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include <algorithm>
#include <utility>

namespace tralics_ns {
    void add_ref(long v, const std::string &s, bool idx);
    void find_index_labels(std::vector<std::string> &W);

} // namespace tralics_ns

class Indexer;
namespace trees_ns {
    auto xless(Indexer *A, Indexer *B) -> bool;
} // namespace trees_ns

//
class Indexer {
public:
    std::string        key; // sort key
    std::string        aux;
    Xml *              translation;
    int                level;
    size_t             iid; // index in the reference table
    [[nodiscard]] auto is_same(int l, const std::string &k) const -> bool { return level == l && k == aux; }
    Indexer(std::string k, std::string a, Xml *x, int l, size_t u)
        : key(std::move(k)), aux(std::move(a)), translation(x), level(l), iid(u) {}
};

class OneIndex {
    std::string            name;
    std::string            title;
    Xml *                  position; // Position on the XML of the index
    size_t                 AL;       // The attribute list index
    std::vector<Indexer *> value;

public:
    OneIndex(std::string a, std::string b, size_t c) : name(std::move(a)), title(std::move(b)), position(nullptr), AL(c) {}

    [[nodiscard]] auto size() const { return value.size(); }
    [[nodiscard]] auto get_title() const -> const std::string & { return title; }
    [[nodiscard]] auto get_position() const -> Xml * { return position; }
    [[nodiscard]] auto get_translation(size_t i) const -> Xml * { return value[i]->translation; }
    [[nodiscard]] auto get_iid(size_t i) const -> size_t { return value[i]->iid; }
    [[nodiscard]] auto has_name(const std::string &s) const -> bool { return name == s; }
    [[nodiscard]] auto get_AL() const -> size_t { return AL; }

    void set_position(Xml *x) { position = x; }
    auto get_data() -> std::vector<Indexer *> & { return value; }
    void do_sort() { std::sort(value.begin(), value.end(), trees_ns::xless); }
};

class AllIndex {
    int                     last_index{0};
    size_t                  last_iid{0};
    std::vector<OneIndex *> value;

public:
    AllIndex();

    [[nodiscard]] auto size() const { return value.size(); }
    [[nodiscard]] auto get_last_iid() const -> size_t { return last_iid; }

    auto get_index(size_t j) -> OneIndex * { return value[j]; }
    auto find_index(const std::string &s) -> size_t;
    auto get_data(size_t i) -> std::vector<Indexer *> & { return value[i]->get_data(); }
    void mark_print(size_t g);
    auto next_iid() -> size_t { return last_iid++; }
    auto next_index() -> int { return ++last_index; }
    void new_index(const std::string &s, const std::string &title);
};
