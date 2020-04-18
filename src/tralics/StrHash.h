#pragma once
#include "consts.h"
#include <string>
#include <unordered_map>
#include <vector>

class LabelInfo;

struct StrHash_record {
    std::string name, value;
    LabelInfo * labinfo{nullptr};
};

class StrHash : public std::vector<StrHash_record> {
    std::unordered_map<std::string, size_t> s_to_i;

public:
    StrHash() : std::vector<StrHash_record>{{"", ""}, {"", ""}, {" ", " "}} {
        s_to_i.try_emplace("", 1);
        s_to_i.try_emplace(" ", 2);
    }

    auto find_or_insert(const std::string &s) -> size_t;
    auto labinfo(size_t k) -> LabelInfo *;
};
