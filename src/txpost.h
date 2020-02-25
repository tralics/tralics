#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// for modules
class ModChecker {
    Istring id;              // the id of the module or section
    bool    is_mod{false};   // is this a module?
    bool    has_info{false}; // does this have some infos ?
public:
    ModChecker() : id(0) {}
    ModChecker(Istring I, bool mod) : id(I), is_mod(mod) {}
    void set(Istring I) {
        if (id == I) has_info = true;
    }
    void check(int *T) const {
        if (is_mod) {
            if (has_info)
                T[0]++;
            else
                T[1]++;
        } else {
            if (has_info)
                T[2]++;
            else
                T[3]++;
        }
    }
};

// temporary for recursion.
class XmlAction {
    Istring    match;      // the name of the object to match
    recur_type what;       // the type of the action to perform
    int        int_val;    // number of results, or xid
    Xml *      xml_val;    // input or output xml value
    Istring    string_val; // name of element ot work on
public:
    XmlAction(Istring M, recur_type w) : match(M), what(w), int_val(0), xml_val(nullptr), string_val(Istring()) {}
    XmlAction(Istring M, recur_type w, Xml *X) : match(M), what(w), int_val(0), xml_val(X), string_val(Istring()) {}
    XmlAction(Istring M, recur_type w, Istring X) : match(M), what(w), int_val(0), xml_val(nullptr), string_val(X) {}
    [[nodiscard]] auto get_what() const -> recur_type { return what; }
    void               incr_int_val() { int_val++; }
    void               mark_found() { int_val = 1; }
    auto               is_ok() -> bool { return int_val != 0; }
    [[nodiscard]] auto get_xml_val() const -> Xml * { return xml_val; }
    [[nodiscard]] auto get_int_val() const -> int { return int_val; }
    [[nodiscard]] auto get_string_val() const -> Istring { return string_val; }
    void               set_string_val(Istring s) { string_val = s; }
    void               set_xml_val(Xml *s) { xml_val = s; }
    void               set_int_val(int s) { int_val = s; }
    [[nodiscard]] auto get_match() const -> Istring { return match; }
};

// A class to count words...
class WordList {
    WordList *  next;
    std::string name;
    int         hash;
    int         freq;

public:
    void               set_next(WordList *n) { next = n; }
    [[nodiscard]] auto next_empty() const -> bool { return next == nullptr; }
    WordList(std::string s, int h, WordList *N) : next(N), name(s), hash(h), freq(1) {}
    auto               is_here(String s, int h) const -> bool { return hash == h && strcmp(name.c_str(), s) == 0; }
    void               incr_freq() { freq++; }
    [[nodiscard]] auto get_freq() const -> int { return freq; }
    [[nodiscard]] auto get_next() const -> WordList * { return next; }
    auto               dump(std::fstream *X, int i) -> bool {
        if (freq == i) {
            (*X) << freq << " " << name << "\n";
            return true;
        }
        return false;
    }
};
