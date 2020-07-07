// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2003-2011
// Functions on fonts for Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Parser.h"
#include "tralics/TexFonts.h"
#include <fmt/format.h>

// Font info stuff.

// ------------------------------------------------------
// tex fonts

// Finds a font given by name and size, or creates one if needed
auto TexFonts::find_font(const std::string &n, long a, long s) -> size_t {
    for (unsigned i = 0; i < size(); i++)
        if (at(i) == TexFont{n, a, s}) return i;
    return define_a_new_font(n, a, s);
}

// This allocates a new slot in the font list.
auto TexFonts::define_a_new_font(const std::string &n, long a, long s) -> size_t {
    if (size() >= 256) { /// \todo Perhaps remove this artificial limitation
        the_parser.parse_error("fatal: font table overflow");
        return 0;
    }
    emplace_back(n, a, s);
    return size() - 1;
}

/// True if k is a valid font ID
/// \todo Replace that with size_t or std::optional<size_t>
auto TexFonts::is_valid(long k) -> bool { return (k >= 0) && (k < to_signed(size())); }

// Returns name of font
auto TexFonts::name(long k) -> std::string {
    if (!is_valid(k)) return "";
    return at(to_unsigned(k)).name;
}

// Returns name of font
void TexFonts::full_name(Buffer &B, long k) {
    if (!is_valid(k)) return;
    B.append(at(to_unsigned(k)).name);
    if (at(to_unsigned(k)).scaled_val != 0) B.format(" scaled {}", at(to_unsigned(k)).scaled_val);
    if (at(to_unsigned(k)).at_val != 0) {
        B.append(" at ");
        B.push_back(ScaledInt(at(to_unsigned(k)).at_val), glue_spec_pt);
    }
}

// Returns an integer parameter for a font
auto TexFonts::get_int_param(long ft, int pos) -> long {
    if (!is_valid(ft)) return -1;
    if (pos == 0) return at(to_unsigned(ft)).hyphen_char;
    return at(to_unsigned(ft)).skew_char;
}

// Returns a dimension parameter for a font
auto TexFonts::get_dimen_param(long ft, long pos) -> ScaledInt {
    if (!is_valid(ft)) return 0;
    if (pos < 0 || to_unsigned(pos) >= at(to_unsigned(ft)).size()) return 0;
    return at(to_unsigned(ft))[to_unsigned(pos)];
}

// Sets an integer parameter for a font
void TexFonts::set_int_param(long ft, int pos, long v) {
    if (!is_valid(ft)) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    if (pos == 0)
        at(to_unsigned(ft)).hyphen_char = v;
    else
        at(to_unsigned(ft)).skew_char = v;
}

// Sets a dimension parameter for a font
void TexFonts::set_dimen_param(long ft, long p, ScaledInt v) {
    if (!is_valid(ft) || p < 0 || p > 100000) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    auto up  = to_unsigned(p);
    auto uft = to_unsigned(ft);
    if (up >= at(uft).size()) at(uft).resize(up + 1);
    at(uft)[up] = v;
}
