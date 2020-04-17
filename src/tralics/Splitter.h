#pragma once
#include <string>
#include <vector>

auto split_commas(const std::string &S) -> std::vector<std::string>;
auto split_assign(std::string s) -> std::pair<std::string, std::string>;
auto without_end_spaces(std::string s) -> std::string;
