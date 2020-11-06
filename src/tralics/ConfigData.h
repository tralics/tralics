#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using ParamDataList = std::unordered_map<std::string, std::string>;

class ConfigData : public std::unordered_map<std::string, ParamDataList> {
public:
    [[deprecated]] auto find_list(const std::string &name, bool creat) -> ParamDataList *;
};

inline ConfigData config_data; // \todo make static?
