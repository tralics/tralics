#pragma once
#include "OneIndex.h"
#include <string>

struct AllIndex : public std::vector<OneIndex> { // \todo unordered_map perhaps
    size_t last_index{0}, last_iid{0};

    AllIndex();

    auto find_index(const std::string &s) -> OneIndex &;
    static void mark_print(OneIndex &g);
    void new_index(const std::string &s, const std::string &title);
};

inline AllIndex the_index;
