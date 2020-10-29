#pragma once
#include "ParamDataSlot.h"
#include <string>
#include <vector>

struct ParamDataList : public std::vector<ParamDataSlot> {
    std::string name;

    ParamDataList(std::string s) : name(std::move(s)) {}

    [[nodiscard]] auto its_me(const std::string &s) const -> bool { return name == s; }
    void               keys_to_buffer(Buffer &B) const;

    void check_other();
};
