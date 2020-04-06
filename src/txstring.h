#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Buffer.h"

// This file holds the hash table, and XML.
class LabelInfo;

// The string_hash table. It is like the hashtable below (Text and Next)
// but it is extensible (maybe, we should make the hashtable extensible)
// Each item has a pointer to a LabelInfo element. If the pointer is not null
// we consider the slot of `foo', that has a \label{foo} or \ref{foo}
// At bootstrap, we fill the table with true, false, spacebefore, center,...
// This uses utf8 encoding. The token can be dumped via its Value
// which can be ascii, utf8 or latin1 (XML syntax)

struct StrHash_record {
    String     name{nullptr};
    String     value{nullptr};
    LabelInfo *Labinfo{nullptr};
};

class StrHash {
    size_t                      hash_len{hash_size};
    std::vector<StrHash_record> data{hash_size};
    std::vector<size_t>         Next{std::vector<size_t>(hash_size)}; // Because {hash_size} gives size 1
    size_t                      hash_last{hash_prime + 1};            // last slot used
public:
    StrHash() {
        data[0] = {"", "", nullptr};
        data[1] = {"", "", nullptr};
        data[2] = {" ", " ", nullptr};
    }

    [[nodiscard]] auto p_str(size_t k) const -> String { return data[k].value; }

    void re_alloc() {
        auto k = hash_len + 10000;
        data.resize(k);
        Next.resize(k, 0);
        hash_len = k;
    }

    auto        hash_find(const std::string &s) -> size_t;
    auto        find(String s) -> size_t;
    auto        find(const std::string &s) -> size_t;
    auto        find(int s) -> size_t;
    auto        operator[](size_t k) const -> String { return data[k].name; }
    auto        lab_val(Istring k) -> LabelInfo * { return data[k.id].Labinfo; };
    auto        lab_val_check(Istring k) -> LabelInfo *;
    static auto next_label_id() -> Istring; // \todo Move outside
    auto        find_scaled(ScaledInt s) -> Istring;
};
