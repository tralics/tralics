#pragma once
#include "enums.h"
#include <string>
#include <vector>

auto find_counter(const std::string &s) -> int;
auto only_digits(const std::string &s) -> bool;
auto only_space(const std::string &s) -> bool;
auto split_commas(const std::string &S) -> std::vector<std::string>;
auto split_assign(std::string s) -> std::pair<std::string, std::string>;
auto without_end_spaces(std::string s) -> std::string;
auto bt_to_string(boundary_type v) -> std::string;
auto save_string_name(size_t n) -> String;
auto is_m_font(symcodes cmd) -> bool;

auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;
