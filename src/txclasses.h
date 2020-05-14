#pragma once
// -*- C++ -*-
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Buffer.h"

void show_unused_options();

// This holds the translation of \DeclareOption{foo}{bar}
// or \usepackage[foo=bar]{pack}
//  name foo, val bar

// In the case of \DeclareOption{foo}{bar}, then name=full_name =foo
// and val is bar or empty. In the case \usepackage, this is different
class KeyAndVal {
private:
    bool used{false}; // set to true when option is used
public:
    std::string name;      // name of the option, e.g. 'key'
    std::string full_name; // In case 'key = {\val,etc}' holds all chars
    TokenList   val;       // value of the option, eg '={\val,etc}'.

    KeyAndVal() = default;
    KeyAndVal(std::string a, TokenList b, std::string all) : name(std::move(a)), full_name(std::move(all)), val(std::move(b)) {}
    [[nodiscard]] auto has_name(const std::string &x) const -> bool { return name == x; }
    [[nodiscard]] auto has_full_name(const std::string &x) const -> bool { return full_name == x; }
    auto               has_name(String x) const -> bool { return name == x; }
    void               use(TokenList &A) const;
    void               kill() {
        val  = TokenList();
        used = true;
    }
    void               use_and_kill(TokenList &L, KeyAndVal &U, bool X);
    [[nodiscard]] auto to_list() const -> TokenList;
    [[nodiscard]] auto is_used() const -> bool { return used; }
    void               mark_used() { used = true; }
    void               mark_un_used() { used = false; }
};

inline auto operator<<(std::ostream &os, const KeyAndVal &kv) -> std::ostream & { return os << kv.full_name; }

using OptionList = std::vector<KeyAndVal>;

// This defines what is in a package or a class
class LatexPackage {
public:
    std::string name;            // name, with prefix P or C
    std::string date;            // date and version
    std::string req_date;        // date argument in the call
    OptionList  Uoptions;        // options of \usepackage
    OptionList  Poptions;        // options defined by the package
    bool        has_a_default;   // is there a default error handler ?
    TokenList   default_handler; // the token lists of the default handler
    TokenList   hook;            // for \AtEndOfPackage
    bool        seen_process;    // have we seen a \ProcessOptions
    bool        checked;         // set by \usepakage (for re-use)

    LatexPackage(std::string A);
    void               add_to_hook(TokenList &L) { hook.splice(hook.end(), L); }
    void               add_options(const OptionList &L);
    auto               find_option(const std::string &name) -> long;
    [[nodiscard]] auto is_class() const -> bool { return name[0] == 'C'; }
    [[nodiscard]] auto real_name() const -> std::string { return name.substr(1); }
    [[nodiscard]] auto full_name() const -> std::string { return name; }
    [[nodiscard]] auto pack_or_class() const -> String { return is_class() ? "class " : "package "; }
    [[nodiscard]] auto has_name(const std::string &s) const -> bool { return name == s; }
    void               print_options();
    void               check_global_options(TokenList &action, bool X);
    void               check_local_options(TokenList &res, bool X);
    void               check_all_options(TokenList &action, TokenList &spec, int X);
    void               reload() const;
};

class ClassesData {
public:
    std::vector<LatexPackage *> packages;
    OptionList                  unused_options;             // is \@unusedoptionlist in latex
    OptionList                  global_options;             // is \@classoptionslist is latex
    TokenList                   documentclass_hook;         // single hook for all classes
    bool                        seen_document_class{false}; // have we seen a \documentclass command
    bool                        using_default_class{false}; // inhibits warning

    ClassesData();
    auto        cur_pack() -> LatexPackage *;
    auto        find_package(const std::string &name, bool type, bool creat) -> size_t;
    void        new_unused_global_option(const KeyAndVal &s) { unused_options.push_back(s); }
    static void remove_from_unused(const std::string &name);
    static void show_unused();
};

// The  date parser
class FormatDate { // \todo c++ standard class for date?
    Token       err_tok;
    auto        scan_a_field(Buffer &B, int &res) -> bool;
    static auto scan_a_month(Buffer &B, int &res) -> bool;
    auto        scan_next(Buffer &B, int &res) -> bool;
    auto        sort() -> bool;
    static auto next_format_char(Buffer &B) -> int;
    auto        parse_format(Buffer &B) -> bool;
    auto        parse(Buffer &B) -> bool;

public:
    int month{0}, day{0}, year{0};

    auto interpret(const std::string &s, Token T) -> bool;
};
