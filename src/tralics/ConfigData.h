#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct ParamDataList : public std::unordered_map<std::string, std::string> {
    void interpret(const std::string &b);
};

struct ConfigData : public std::unordered_map<std::string, ParamDataList> {
    [[nodiscard]] auto find_one_key(const std::string &name, const std::string &key) const -> std::string; // \todo lots of RA stuff
    [[nodiscard]] auto format_keys(const std::string &name) const -> std::string;
};

inline ConfigData config_data; // \todo make static?
