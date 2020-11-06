#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// \todo use maps and such for this

struct ParamDataList : public std::unordered_map<std::string, std::string> {
    std::string name;

    ParamDataList(std::string s) : name(std::move(s)) {}
};

class ConfigData : public std::vector<ParamDataList> {
public:
    auto find_list(const std::string &name, bool creat) -> ParamDataList *;
};

inline ConfigData config_data;
