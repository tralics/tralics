#pragma once
#include "Istring.h"
#include <array>

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
    std::array<Istring, np_last> flat;

    Istring operator[](size_t i) const { return flat[i]; }

    Istring &operator[](name_positions i) { return flat[i]; }

    void set(size_t i, const Istring &s) { flat[i] = s; }
};

inline NameMapper the_names; // \todo static in Istring and make is a map from std::string
