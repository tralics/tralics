#pragma once
#include "types.h"
#include <string>

// This class returns foo then bar then gee from `foo,bar,gee'
class Splitter {
    std::string S;   // the string to split
    size_t      pos; // current position
    size_t      size;

public:
    Splitter(std::string w) : S(std::move(w)), pos(0) { size = S.size(); }
    [[nodiscard]] auto at_end() const -> bool { return pos == size; }
    [[nodiscard]] auto count() const -> size_t;
    auto               get_next_raw() -> String;
    auto               get_next() -> std::string;
    void               extract_keyval(std::string &key, std::string &val);
};
