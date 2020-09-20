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

#include "tralics/LineList.h"
#include "tralics/MainClass.h"
#include "txparam.h"
#include <filesystem>

// See comments in tralics.cpp where these variables are defined
namespace main_ns {
    inline bool no_xml_error{false};
    inline bool use_quotes{false};
    inline bool bib_allow_break{true};
} // namespace main_ns

namespace io_ns {
    auto find_encoding(const std::string &cl) -> std::optional<size_t>;
} // namespace io_ns

namespace tralics_ns {
    auto file_exists(const std::string &name) -> bool;
    auto find_in_confdir(const std::string &s, bool retry)
        -> std::optional<std::filesystem::path>;                                     ///< Try to open the file, using alt location if needed
    auto find_in_path(const std::string &s) -> std::optional<std::filesystem::path>; ///< Tries to open a TeX file
    auto get_out_dir(const std::string &name) -> std::filesystem::path;              /// Returns output_dir+name
    auto is_leap_year(long y) -> bool;
    auto open_file(const std::string &name, bool f) -> std::ofstream;
    auto titlepage_is_valid() -> bool;

    void boot_math(bool mv);
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
    void interpret_list(const std::string &a, Buffer &B);
} // namespace config_ns
