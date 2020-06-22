#pragma once
#include "Istring.h"
#include <utility>

// Data structure for label and references.
class LabelInfo {
public:
    Istring     id;             // value of the ID
    std::string filename;       // file of definition
    int         lineno{0};      // line of definition
    bool        used{false};    // is this ID used ?
    bool        defined{false}; // is this ID defined ?

    auto set_used() -> bool { return std::exchange(used, true); }
    auto set_defined() -> bool { return std::exchange(defined, true); }
};

[[nodiscard]] auto labinfo(const std::string &s) -> LabelInfo *;
