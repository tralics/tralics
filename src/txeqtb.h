#pragma once
#include "txscaled.h"

// -*- C++ -*-
// Copyright INRIA/apics (Jose' Grimm) 2002-2004, 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

class Parser;
class Xml;

// this saves two values
class SaveState {
    TokenList L; // the token list to be restored
public:
    bool restricted{}; // the restricted flag to be restored

    void copy_and_reset(TokenList &X) {
        L.clear();
        L.splice(L.begin(), X);
    }
    void restore(TokenList &X) {
        X.clear();
        X.splice(X.begin(), L);
    }
    void set_restricted(bool b) { restricted = b; }
};
