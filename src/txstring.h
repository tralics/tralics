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
    String     Text, Value;
    LabelInfo *Labinfo;
};

class StrHash {
    StrHash_record *data;
    size_t *        Next;      // the Next table
    size_t          hash_len;  // size of the table
    size_t          hash_last; // last slot used
    Buffer          mybuf;     // local buffer
public:
    StrHash();

    [[nodiscard]] auto p_str(size_t k) const -> String { return data[k].Value; }

    void re_alloc();
    auto hash_find() -> size_t;
    auto find(String s) -> size_t;
    auto find(const std::string &s) -> size_t;
    auto find(int s) -> size_t;
    auto operator[](size_t k) const -> String { return data[k].Text; }
    auto shbuf() -> Buffer & { return mybuf; }
    auto lab_val(Istring k) -> LabelInfo * { return data[k.id].Labinfo; };
    auto lab_val_check(Istring k) -> LabelInfo *;
    auto next_label_id() -> Istring;
    auto next_top_label_id() -> Istring;
    auto find_scaled(ScaledInt s) -> Istring;

    static void rlc_to_string(String s, std::vector<AttList> &res);
    static auto skip_val(int k) -> name_positions { return k == 0 ? np_3pt : k == 1 ? np_6pt : np_12pt; }
    static auto st_bool(bool x) -> name_positions { return x ? np_true : np_false; };
};
