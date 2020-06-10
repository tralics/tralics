#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2002, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/BibEntry.h"
#include "tralics/BibMacro.h"
#include "tralics/CitationItem.h"
#include "tralics/CitationKey.h"
#include "tralics/Xml.h"
#include "txinline.h"

class Berror {};

class Bbl {
private:
    Buffer        B;
    std::ofstream file;
    bool          too_late{false};

public:
    friend class BibEntry;

    std::string name;
    LinePtr     lines;

    void               newline();
    void               push_back(String s) { B.push_back(s); }
    void               push_back(const std::string &s) { B.push_back(s); }
    void               push_back_braced(const std::string &s) { B.push_back_braced(s); }
    auto               non_empty_buf() -> bool { return !B.empty(); }
    void               reset() { B.clear(); }
    void               reset_lines() { lines.clear(); }
    [[nodiscard]] auto is_too_late() const -> bool { return too_late; }
    void               finish() { too_late = true; } // \todo should this be called ~Bbl ?
    void               open() {
        if (!file.is_open()) file = tralics_ns::open_file(name, true);
    }
};

inline Bbl bbl;
