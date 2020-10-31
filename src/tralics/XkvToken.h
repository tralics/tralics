#pragma once
#include "TokenList.h"
#include <string>
#include <vector>

struct XkvToken {
    std::string keyname;
    TokenList   initial; // of the form \gsavevalue{foo}=bar
    TokenList   value;
    TokenList   action;
    bool        is_global{}, has_save{}, has_val{};

public:
    void extract();
    void prepare(const std::string &fam);

    [[nodiscard]] auto check_save() const -> bool;
    [[nodiscard]] auto ignore_this(std::vector<std::string> &igna) const -> bool;
    [[nodiscard]] auto is_defined(const std::string &fam) const -> bool;
};
