#pragma once
#include <string>
#include <vector>

// \todo use maps and such for this

struct ParamDataSlot {
    std::string key, value;
};

struct ParamDataList : public std::vector<ParamDataSlot> {
    std::string name;

    ParamDataList(std::string s) : name(std::move(s)) {}
};

class ConfigData : public std::vector<ParamDataList> {
public:
    auto find_list(const std::string &name, bool creat) -> ParamDataList *;
};

inline ConfigData config_data;
