#pragma once
#include "Istring.h"
#include <array>

/*
 * TODO This is work in progress. We need to be able to change XML output and so
 * far there is a huge list of name_positions, but eventually it would be better
 * to have a map from std::string to Istring instead. In the interim we will
 * build a map from std::string to name_positions, then replace progressively.
 */

struct NameMapper {
    std::array<Istring, np_last> flat;

    Istring &operator[](name_positions i) { return flat[i]; }
    Istring &operator[](size_t i) { return flat[i]; }
};

inline NameMapper the_names; // \todo static in Istring and make is a map from std::string
