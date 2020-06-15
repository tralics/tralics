#pragma once
#include "KeyAndVal.h"
#include <vector>

using OptionList = std::vector<KeyAndVal>;

// This defines what is in a package or a class
class LatexPackage {
public:
    std::string name;                 // name, with prefix P or C
    std::string date;                 // date and version
    std::string req_date;             // date argument in the call
    OptionList  Uoptions;             // options of \usepackage
    OptionList  Poptions;             // options defined by the package
    TokenList   default_handler;      // the token lists of the default handler
    TokenList   hook;                 // for \AtEndOfPackage
    bool        has_a_default{false}; // is there a default error handler ?
    bool        seen_process{false};  // have we seen a \ProcessOptions
    bool        checked{false};       // set by \usepakage (for re-use)

    LatexPackage(std::string A) : name(std::move(A)) {}

    [[nodiscard]] auto is_class() const -> bool { return name[0] == 'C'; }
    [[nodiscard]] auto real_name() const -> std::string { return name.substr(1); }
    [[nodiscard]] auto full_name() const -> std::string { return name; }
    [[nodiscard]] auto pack_or_class() const -> String { return is_class() ? "class " : "package "; }
    [[nodiscard]] auto has_name(const std::string &s) const -> bool { return name == s; }

    void add_to_hook(TokenList &L) { hook.splice(hook.end(), L); }
    void add_options(const OptionList &L);
    auto find_option(const std::string &name) -> long;
    void check_global_options(TokenList &action, bool X);
    void check_local_options(TokenList &res, bool X);
    void check_all_options(TokenList &action, TokenList &spec, int X);
    void reload() const;
};
