#pragma once
#include "OneIndex.h"
#include <string>

struct AllIndex : public std::vector<OneIndex> { // \todo unordered_map perhaps
    size_t last_index{0}, last_iid{0};

    AllIndex();

    static AllIndex &the_index();

    auto find(const std::string &s) -> OneIndex &;
    void insert(const std::string &s, const std::string &title);
};
