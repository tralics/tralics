#pragma once
#include "Istring.h"
#include <array>
#include <unordered_map>

/*
 * TODO This is work in progress. We need to be able to change XML output and so
 * far there is a huge list of name_positions, but eventually it would be better
 * to have a map from std::string to Istring instead. So we will need a few
 * things temporarily:
 *
 * - A map from std::string to name_positions, from the default value to the id
 * - A map from name_position to std::string, to the default value
 *
 * Problem is if two name_positions initially are mapped to the same string.
 */

class NameMapper {
    std::array<std::string, np_last>         id_to_name;
    std::unordered_map<std::string, Istring> dict;

public:
    [[deprecated]] auto operator[](name_positions i) const -> Istring;
    auto                operator[](const std::string &name) const -> Istring;

    void set(name_positions i, const std::string &s);
    void set(const std::string &name, const std::string &value);

    void def(const std::string &name, name_positions pos, const std::optional<std::string> &value = std::nullopt);
    void def(name_positions i, const std::string &s);
    void boot();

    void assign(const std::string &sa, const std::string &sb);
    void assign_name(const std::string &A, const std::string &B);
};

inline NameMapper the_names;
