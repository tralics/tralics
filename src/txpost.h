#pragma once
#include "tralics/enums.h"
#include <cstring>
#include <fstream>
#include <string>
#include <utility>

class Xml;

// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// temporary for recursion.
class XmlAction {
    std::string match;      // the name of the object to match
    recur_type  what;       // the type of the action to perform
    long        int_val;    // number of results, or xid
    Xml *       xml_val;    // input or output xml value
    std::string string_val; // name of element ot work on
public:
    XmlAction(std::string M, recur_type w) : match(std::move(M)), what(w), int_val(0), xml_val(nullptr) {}
    XmlAction(std::string M, recur_type w, Xml *X) : match(std::move(M)), what(w), int_val(0), xml_val(X) {}
    XmlAction(std::string M, recur_type w, std::string X)
        : match(std::move(M)), what(w), int_val(0), xml_val(nullptr), string_val(std::move(X)) {}
    [[nodiscard]] auto get_what() const -> recur_type { return what; }
    void               incr_int_val() { int_val++; }
    void               mark_found() { int_val = 1; }
    [[nodiscard]] auto is_ok() const -> bool { return int_val != 0; }
    [[nodiscard]] auto get_xml_val() const -> Xml * { return xml_val; }
    [[nodiscard]] auto get_int_val() const -> long { return int_val; }
    [[nodiscard]] auto get_string_val() const -> std::string { return string_val; }
    void               set_string_val(std::string s) { string_val = std::move(s); }
    void               set_xml_val(Xml *s) { xml_val = s; }
    void               set_int_val(long s) { int_val = s; }
    [[nodiscard]] auto get_match() const -> std::string { return match; }
};

// A class to count words... \todo where is that used? \todo chained list???
class WordList {
    WordList *  next;
    std::string name;
    size_t      hash;
    int         freq;

public:
    void               set_next(WordList *n) { next = n; }
    [[nodiscard]] auto next_empty() const -> bool { return next == nullptr; }
    WordList(std::string s, size_t h, WordList *N) : next(N), name(std::move(s)), hash(h), freq(1) {}
    [[nodiscard]] auto is_here(const std::string &s, size_t h) const -> bool { return (hash == h) && (name == s); }
    void               incr_freq() { freq++; }
    [[nodiscard]] auto get_freq() const -> int { return freq; }
    [[nodiscard]] auto get_next() const -> WordList * { return next; }
    auto               dump(std::ostream &X, int i) -> bool {
        if (freq == i) {
            X << freq << " " << name << "\n";
            return true;
        }
        return false;
    }
};
