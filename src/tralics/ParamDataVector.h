#pragma once
#include "ParamDataList.h"

class ParamDataVector : public std::vector<ParamDataList *> {
public:
    ParamDataVector();
    auto find_list(const std::string &name, bool creat) -> ParamDataList *;
};

inline ParamDataVector config_data;
