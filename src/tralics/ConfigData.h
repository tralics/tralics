#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct ParamDataList : public std::unordered_map<std::string, std::string> {
    void interpret(const std::string &b);
};

struct ConfigData : public std::unordered_map<std::string, ParamDataList> {};

inline ConfigData config_data; // \todo make static?
