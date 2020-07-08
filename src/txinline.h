#pragma once
// -*- C++ -*-
// TRALICS, copyright (C)INRIA/apics (Jose' Grimm) 2003,2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains some inline functions and the tralics
// namespace declarations

// We also declare some global variables here

#include "tralics/MainClass.h"
#include "txparam.h"

// See comments in tralics.cpp where these variables are defined
namespace main_ns {
    inline int  nb_errs{0};
    inline bool no_xml_error{false};
    inline bool use_quotes{false};
    inline bool bib_allow_break{true};
} // namespace main_ns

namespace io_ns {
    auto find_encoding(const std::string &cl) -> int;
} // namespace io_ns

namespace tralics_ns {
    auto exists(const std::vector<std::string> &ST, const std::string &d) -> bool;
    auto file_exists(const std::string &name) -> bool;
    auto find_in_confdir(const std::string &s, bool retry)
        -> std::optional<std::filesystem::path>;                                     ///< Try to open the file, using alt location if needed
    auto find_in_path(const std::string &s) -> std::optional<std::filesystem::path>; ///< Tries to open a TeX file
    auto get_out_dir(const std::string &name) -> std::filesystem::path;              /// Returns output_dir+name
    auto get_short_jobname() -> std::string;
    auto is_leap_year(long y) -> bool;
    auto open_file(const std::string &name, bool f) -> std::ofstream;
    auto titlepage_is_valid() -> bool;

    void boot_math(bool mv);
    void read_a_file(LineList &L, const std::string &x, int spec);
    void Titlepage_create(LineList &lines);
    void Titlepage_start(bool verbose);
} // namespace tralics_ns

namespace err_ns {
    void fatal_error(String s);
} // namespace err_ns

namespace config_ns {
    auto find_one_key(const std::string &name, const std::string &key) -> std::string;
    auto pers_rc(const std::string &rc) -> std::string;
    void check_RC(Buffer &B, Xml *res);
    auto find_keys(const std::string &name) -> std::string;
    auto start_interpret(Buffer &B, String s) -> bool;
    void interpret_list(const std::string &a, Buffer &B);
} // namespace config_ns

namespace tpage_ns {
    void see_main_a(Buffer &in, Buffer &val);
} // namespace tpage_ns

inline auto is_vowel(char c) -> bool { return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'; }

// True if it is an accent character, like \^.
inline auto is_accent_char(char c) -> bool { return c == '\'' || c == '`' || c == '^' || c == '"' || c == '~' || c == '.' || c == '='; }

inline auto is_tp_delim(char c) -> bool { return c == '<' || c == '\\' || c == '"'; }

inline auto operator==(Token a, Token b) -> bool { return a.val == b.val; }

inline auto operator!=(Token a, Token b) -> bool { return a.val != b.val; }
