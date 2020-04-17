#pragma once
#include "util.h"
#include <string>
#include <unordered_map>

struct SpecialHash : public std::unordered_map<std::string, std::string> {
    SpecialHash(const std::string &str) {
        for (const auto &s : split_commas(str)) {
            auto [a, b] = split_assign(s);
            try_emplace(a, b);
        }
    }

    [[nodiscard]] auto get(const std::string &x) const -> std::string {
        if (auto i = find(x); i != end()) return i->second;
        return {};
    }
};
