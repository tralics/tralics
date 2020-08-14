#pragma once
#include <cassert>
#include <optional>
#include <string>
#include <vector>

struct AttPair {
    std::string name, value;
};

struct AttList : public std::vector<AttPair> {
    [[nodiscard]] auto get_val(size_t i) const -> std::string { return at(i).value; }
    [[nodiscard]] auto lookup(const std::string &x) const -> std::optional<size_t>;

    using std::vector<AttPair>::push_back;
    void push_back(const std::string &name, const std::string &value, bool force = true);
};

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream &;
