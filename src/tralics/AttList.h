#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>

struct AttPair {
    std::string name, value;
};

struct AttList : public std::vector<AttPair> { // \todo unordered_map
    [[nodiscard]] auto lookup(const std::string &x) -> AttPair *;

    using std::vector<AttPair>::push_back;
    void push_back(const std::string &name, const std::string &value, bool force = true);
};

auto operator<<(std::ostream &o, const AttPair &a) -> std::ostream &;
auto operator<<(std::ostream &o, const AttList &a) -> std::ostream &;
