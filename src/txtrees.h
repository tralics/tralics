#include <utility>

#pragma once
// -*- C++ -*-
// Copyright (c)  INRIA/apics (Jose' Grimm) 2002, 2004, 2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

namespace tralics_ns {
    void add_ref(int v, const string &s, bool);
    void find_index_labels(vector<string> &W);

} // namespace tralics_ns

class Indexer;
namespace trees_ns {
    auto xless(Indexer *A, Indexer *B) -> bool;
}

// This  holds the association between the index and the main text.
// Seems to be unused
class RefIndexElt {
public:
    int     index_nb; // Number of the index, 1=glossary
    int     uid;
    Istring value;
    RefIndexElt(int n, int a, Istring b) : index_nb(n), uid(a), value(b) {}
};

//
class Indexer {
public:
    string             key; // sort key
    string             aux;
    Xml *              translation;
    int                level;
    int                iid; // index in the reference table
    [[nodiscard]] auto is_same(int l, const string &k) const -> bool { return level == l && k == aux; }
    Indexer(string k, string a, Xml *x, int l, int u) : key(std::move(k)), aux(std::move(a)), translation(x), level(l), iid(u) {}
};

class OneIndex {
    string            name;
    string            title;
    Xml *             position; // Position on the XML of the index
    int               AL;       // The attribute list index
    vector<Indexer *> value;

public:
    [[nodiscard]] auto size() const -> int { return value.size(); }
    auto               get_data() -> vector<Indexer *> & { return value; }
    void               do_sort() { sort(value.begin(), value.end(), trees_ns::xless); }
    [[nodiscard]] auto get_AL() const -> int { return AL; }
    [[nodiscard]] auto get_title() const -> const string & { return title; }
    [[nodiscard]] auto get_position() const -> Xml * { return position; }
    void               set_position(Xml *x) { position = x; }
    [[nodiscard]] auto get_translation(int i) const -> Xml * { return value[i]->translation; }
    [[nodiscard]] auto get_iid(int i) const -> int { return value[i]->iid; }
    [[nodiscard]] auto has_name(const string &s) const -> bool { return name == s; }
    OneIndex(string a, string b, int c) : name(std::move(a)), title(std::move(b)), position(nullptr), AL(c) {}
};

class AllIndex {
    int                last_index{0};
    int                last_iid{0};
    vector<OneIndex *> value;

public:
    auto               get_index(int j) -> OneIndex * { return value[j]; }
    auto               find_index(const string &) -> int;
    [[nodiscard]] auto size() const -> int { return value.size(); }
    auto               get_data(int i) -> vector<Indexer *> & { return value[i]->get_data(); }
    AllIndex();
    void mark_print(int g);
    auto next_iid() -> int {
        int i = last_iid;
        ++last_iid;
        return i;
    }
    auto next_index() -> int {
        ++last_index;
        return last_index;
    }
    [[nodiscard]] auto get_last_iid() const -> int { return last_iid; }
    void               new_index(const string &, const string &);
};
