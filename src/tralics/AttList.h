#pragma once
#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>

struct [[deprecated]] AttPair {
    std::string name, value;
};

struct AttList : public std::map<std::string, std::string> {          // map and not unordered_map for reproducible XML output
    [[nodiscard]] auto lookup(const std::string &x) -> std::string *; // \todo use map API

    [[deprecated]] void push_back(const std::string &name, const std::string &value, bool force = true);
};

[[deprecated]] auto operator<<(std::ostream &o, const AttPair &a) -> std::ostream &;
auto                operator<<(std::ostream &o, const AttList &a) -> std::ostream &;
