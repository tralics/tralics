#pragma once
#include "tralics/TitlePageAux.h"
#include "tralics/Xml.h"

// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002-2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains stuff for title-page element.

struct TitlePageFullLine;

// One item if a titlepage
struct TpiOneItem {
    char        p1{};         // first character modifier
    char        p2{};         // second character modifier
    std::string value;        // the value
    tpi_vals    v{tpi_noval}; // the type (none, string, command, or XML element)

    [[nodiscard]] auto has_a_char() const -> bool { return p1 != 0 || p2 != 0; }
    [[nodiscard]] auto noval() const -> bool { return v == tpi_noval; }
    [[nodiscard]] auto is_elt() const -> bool { return v == tpi_elt; }
    [[nodiscard]] auto is_str() const -> bool { return v == tpi_str; }
    [[nodiscard]] auto is_cmd() const -> bool { return v == tpi_cmd; }
    [[nodiscard]] auto is_alias() const -> bool { return v == tpi_alias; }
    [[nodiscard]] auto only_dash() const -> bool { return p1 == '-' && p2 == 0; }
    [[nodiscard]] auto question() const -> bool { return p1 == '?' && p2 == 0; }
    [[nodiscard]] auto plus() const -> bool { return p1 == '+' && p2 == 0; }
    [[nodiscard]] auto quest_plus() const -> bool { return p1 == '?' && p2 == '+'; }
    [[nodiscard]] auto second_char() const -> bool { return p2 != 0; }
    void               reset();
};

// temporary class, will bew copied into a TitlePageAux \todo do
struct TitlePageFullLine {
    TpiOneItem item1, item2, item3, item4; // the four items
    size_t     flags{};                    // the flags

    auto read() -> int;
    auto classify(int w, int state) -> tpi_vals;
    auto encode_flags(char c1, char c2) -> bool;
};

class TitlePage {
    size_t len2{1};       // size of bigtable and Data
    bool   valid{false};  // is this initialised and not killed ?
    int    size{0};       // allocated size of bigtable
    Xml ** Data{nullptr}; // the array of xml data
public:
    std::vector<TitlePageAux> bigtable; // the table
    int                       state{};  // current state of the parser
    TitlePage() = default;
    auto               operator[](size_t k) -> Xml *& { return Data[k]; }
    auto               get_bigtable(size_t k) -> TitlePageAux & { return bigtable[k]; }
    [[nodiscard]] auto get_len2() const -> size_t { return len2; }
    [[nodiscard]] auto is_valid() const -> bool { return valid; }
    void               make_invalid() { valid = false; }
    void               make_valid() { valid = true; }
    void               start_thing(bool verbose);
    void               parse();
    auto               increase_data() -> size_t { return len2++; }
    void               check_size();
    [[nodiscard]] auto find_UR(const std::string &s, const std::string &name) const -> size_t;
    [[nodiscard]] auto find_cmd(const std::string &s) const -> size_t;
};

inline TitlePage Titlepage; // title page info
