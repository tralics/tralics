#pragma once
#include <string>

// True if the string holds only white space
auto only_space(const std::string &s) -> bool;

// True if the string holds only white space or &nbsp;
auto only_space_spec(const std::string &s) -> bool;