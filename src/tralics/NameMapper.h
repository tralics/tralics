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

struct NameMapper {
    std::array<Istring, np_last>                    flat;
    std::unordered_map<std::string, name_positions> s_to_id;

    const Istring &operator[](name_positions i) const { return flat[i]; }
    const Istring &operator[](size_t i) const { return flat[i]; }

    void set(name_positions i, const std::string &s) { flat[i] = Istring(s); }
    void set(size_t i, const std::string &s) { flat[i] = Istring(s); }

    void def(const std::string &name, name_positions pos, const std::string &value);
    void def(name_positions i, const std::string &s);
    void boot();

    auto assign(const std::string &sa, const std::string &sb) -> bool;
    auto assign_att(const std::string &A, const std::string &B) -> bool;
    auto assign_name(const std::string &A, const std::string &B) -> bool;
};

inline NameMapper the_names;
