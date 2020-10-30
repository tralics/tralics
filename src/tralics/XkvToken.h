#pragma once
#include "TokenList.h"

struct XkvToken {
    std::string keyname;
    TokenList   initial; // of the form \gsavevalue{foo}=bar
    TokenList   value;
    TokenList   action;
    bool        is_global{}, has_save{}, has_val{};

public:
    void extract();
    void prepare(const std::string &fam);

    auto check_save() -> bool;
    auto ignore_this(std::vector<std::string> &igna) -> bool;
    auto is_defined(const std::string &fam) -> bool;
};
