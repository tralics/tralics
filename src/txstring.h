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
    StrHash_record *data;
    size_t *        Next;      // the Next table
    size_t          hash_len;  // size of the table
    size_t          hash_last; // last slot used
public:
    StrHash() {
        hash_len = hash_size;

        data = new StrHash_record[hash_len];
        for (size_t i = 0; i < hash_len; i++) { data[i] = {}; }

        Next = new size_t[hash_len];
        for (size_t i = 0; i < hash_len; i++) { Next[i] = 0; }

        hash_last = hash_prime + 1;

        data[0].name  = ""; // make sure these are allocated.
        data[1].name  = "";
        data[2].name  = " ";
        data[0].value = ""; // make sure these are allocated.
        data[1].value = "";
        data[2].value = " ";
    }

    [[nodiscard]] auto p_str(size_t k) const -> String { return data[k].value; }

    void re_alloc() { // \todo use vectors instead of reinventing the wheel
        auto k = hash_len + 10000;

        auto *T1 = new StrHash_record[k];
        for (size_t i = 0; i < hash_len; i++) { T1[i] = data[i]; }
        delete[] data;
        data = T1;

        auto *T2 = new size_t[k];
        for (size_t i = 0; i < hash_len; i++) { T2[i] = Next[i]; }
        delete[] Next;
        Next = T2;

        for (size_t i = hash_len; i < k; i++) {
            data[i] = {};
            Next[i] = 0;
        }

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
