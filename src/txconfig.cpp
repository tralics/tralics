// Tralics, a LaTeX to XML translator.
// Copright INRIA/apics/marelle (Jose' Grimm) 2008-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"

namespace config_ns {
    // We add a final slash, or double slash, this makes parsing easier;
    // We also remove an initial slash (This is not done if the separator is a
    // space, case where s is empty).
    // An initial plus sign means: append the line to the vector, else reset.

    auto start_interpret(Buffer &B, String s) -> bool {
        bool ret_val = false;
        B.append(s);
        B.ptrs.b = 0;
        if (B.head() == '+') {
            B.advance();
            the_log << "+";
        } else
            ret_val = true;
        if ((s[0] != 0) && B.head() == '/') B.advance();
        return ret_val;
    }
} // namespace config_ns

// --------------------------------------------------

// Configuration file option lists.
// The configuration file may contains lines of the form
// FOO_vals="/A/B/C", or FOO_vals="/A/a/B/b/C/c".
// transformed into a vector of data, named FOO, with some execptions.

// This error is fatal
// We make sure these four items always exist

// -----------------------------------------------------------

// This creates the file foo_.bbl and initiates the bibtex translation.

// In the case of "foo bar gee", puts foo, bar and gee in the vector.
// Initial + means append, otherwise replace.

void Buffer::interpret_aux(std::vector<std::string> &bib, std::vector<std::string> &bib2) {
    if (config_ns::start_interpret(*this, "")) {
        bib.resize(0);
        bib2.resize(0);
    }
    for (;;) {
        bool keep = true;
        skip_sp_tab();
        if (head() == 0) break;
        auto a = ptrs.b;
        if (head() == '-') {
            keep = false;
            advance();
            if (head() == 0) break; // final dash ignored
            a++;
            the_log << "--";
        }
        while ((head() != 0) && (std::isspace(head()) == 0)) advance();
        ptrs.a        = a;
        std::string k = substring();
        if (keep)
            bib.emplace_back(k);
        else
            bib2.emplace_back(k);
        the_log << k << " ";
    }
    the_log << "\n";
}
