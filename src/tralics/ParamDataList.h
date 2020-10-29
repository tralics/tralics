#pragma once
#include "ParamDataSlot.h"
#include <string>
#include <vector>

struct ParamDataList : public std::vector<ParamDataSlot> {
    std::string name;

    ParamDataList(std::string s) : name(std::move(s)) {}

    void keys_to_buffer(std::string &B) const;
    void check_other();
};
