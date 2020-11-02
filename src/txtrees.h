#pragma once
#include "tralics/Indexer.h"
#include "tralics/OneIndex.h"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

class Xml;

// -*- C++ -*-
// Copyright (c)  INRIA/apics (Jose' Grimm) 2002, 2004, 2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

namespace tralics_ns {
    void add_ref(long v, const std::string &s, bool idx);
    void find_index_labels(std::vector<std::string> &W);

} // namespace tralics_ns

struct AllIndex : public std::vector<OneIndex *> {
    size_t last_index{0}, last_iid{0};

    AllIndex();

    auto find_index(const std::string &s) -> size_t;
    void mark_print(size_t g);
    void new_index(const std::string &s, const std::string &title);
};
