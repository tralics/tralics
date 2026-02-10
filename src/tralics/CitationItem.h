#pragma once
#include "Buffer.h"

class Xml;

struct CitationItem {
    std::string key, from, id;
    Xml        *solved{};

    CitationItem(std::string A, std::string B) : key(std::move(A)), from(std::move(B)) {}

    void dump(Buffer &b) const;

    [[nodiscard]] auto is_solved() const -> bool { return solved != nullptr; }
    [[nodiscard]] auto match(const std::string &A, const std::string &B) const -> bool;
    [[nodiscard]] auto match_star(const std::string &A) const -> bool;

    auto get_id() -> std::string;
    void dump_bibtex();
};
