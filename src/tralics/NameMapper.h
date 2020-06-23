#pragma once
#include <array>
#include <string>
#include <unordered_map>

/*
 * TODO This is work in progress. We need to be able to change XML output and so
 * far there is a huge list of name_positions, but eventually it would be better
 * to have a map from std::string to std::string instead. So we will need a few
 * things temporarily:
 *
 * - A map from std::string to name_positions, from the default value to the id
 * - A map from name_position to std::string, to the default value
 *
 * Problem is if two name_positions initially are mapped to the same string.
 */

class NameMapper {
    std::unordered_map<std::string, std::string> dict;

public:
    auto operator[](const std::string &name) const -> std::string;

    void set(const std::string &name, const std::optional<std::string> &value);

    void boot();

    void assign(const std::string &sa, const std::string &sb);
    void assign_name(const std::string &A, const std::string &B);

    auto cstf(size_t i) -> std::string;
    auto mml(size_t i) -> std::string;
    auto mi(size_t i) -> std::string;
    auto center(size_t i) -> std::string;
    auto cstdiv(size_t i) -> std::string;
    auto npdiv(size_t i) -> std::string;
};

inline NameMapper the_names;
