#pragma once
// -*- C++ -*-
// Copyright INRIA/apics (Jose' Grimm)  2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/FpStack.h"
#include "tralics/Token.h"
#include "tralics/TokenList.h"
#include <string>
#include <utility>

struct FpNum;
class Hashtab;

//

// This represents the value of a user-defined command
class Macro {
    std::array<TokenList, 10> delimiters; // delimiters between arguments
public:
    size_t    nbargs{0};       // number of arguments
    def_type  type{dt_normal}; // type of macro
    TokenList body;            // the body

    Macro() = default;
    Macro(TokenList L) : body(std::move(L)) { correct_type(); }

    void               set_nbargs(size_t n) { nbargs = n; } // \todo remove
    void               set_type(def_type n) { type = n; }
    [[nodiscard]] auto is_same(const Macro &aux) const -> bool;
    auto               operator[](size_t n) const -> const TokenList & { return delimiters[n]; }
    void               set_delimiters(size_t k, TokenList L) { delimiters[k] = std::move(L); }
    void               correct_type();
};

auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream &;
