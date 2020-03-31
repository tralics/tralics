#pragma once
// -*- C++ -*-
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2002,2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This is one include  file for the tralics software

#include "txio.h"

// parameterised data
class ParamDataSlot {
public:
    std::string key;
    std::string value;
    bool        is_used;

    ParamDataSlot(std::string a, std::string b) : key(std::move(a)), value(std::move(b)), is_used(false) {}
    ParamDataSlot(std::string a, std::string b, bool c) : key(std::move(a)), value(std::move(b)), is_used(c) {}

    void               mark_used() { is_used = true; }
    [[nodiscard]] auto matches(const std::string &x) const -> bool { return is_used && x == key; }
    [[nodiscard]] auto no_topic() const -> bool { return !is_used; }
    void               to_buffer(Buffer &B) const;
    void               key_to_buffer(Buffer &B) const;
};

// We maintain a list of ParamDataSlot.
class ParamDataList {
public:
    std::string                name;
    std::vector<ParamDataSlot> data;

    void check_other();
    auto its_me(const std::string &s) const -> bool { return name == s; }
    ParamDataList(std::string s) : name(std::move(s)) {}
    [[nodiscard]] auto empty() const -> bool { return data.empty(); }
    void               push_back(const ParamDataSlot &x) { data.push_back(x); }
    [[nodiscard]] auto size() const { return data.size(); }
    void               keys_to_buffer(Buffer &B) const;
    void               reset() { data = std::vector<ParamDataSlot>(); }
};

class ParamDataVector {
public:
    std::vector<ParamDataList *> data;
    ParamDataVector();
    auto find_list(const std::string &name, bool creat) -> ParamDataList *;
};

extern ParamDataVector config_data;
