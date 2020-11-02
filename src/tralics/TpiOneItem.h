#pragma once
#include "enums.h"
#include <string>

// One item if a titlepage
struct TpiOneItem {
    char        p1{};         // first character modifier
    char        p2{};         // second character modifier
    std::string value;        // the value
    tpi_vals    v{tpi_noval}; // the type (none, string, command, or XML element)

    [[nodiscard]] auto has_a_char() const -> bool { return p1 != 0 || p2 != 0; }
    [[nodiscard]] auto noval() const -> bool { return v == tpi_noval; }
    [[nodiscard]] auto is_elt() const -> bool { return v == tpi_elt; }
    [[nodiscard]] auto is_str() const -> bool { return v == tpi_str; }
    [[nodiscard]] auto is_cmd() const -> bool { return v == tpi_cmd; }
    [[nodiscard]] auto is_alias() const -> bool { return v == tpi_alias; }
    [[nodiscard]] auto only_dash() const -> bool { return p1 == '-' && p2 == 0; }
    [[nodiscard]] auto question() const -> bool { return p1 == '?' && p2 == 0; }
    [[nodiscard]] auto plus() const -> bool { return p1 == '+' && p2 == 0; }
    [[nodiscard]] auto quest_plus() const -> bool { return p1 == '?' && p2 == '+'; }
    [[nodiscard]] auto second_char() const -> bool { return p2 != 0; }
    void               reset();
};
