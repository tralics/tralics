#pragma once
#include "enums.h"
#include <vector>

struct CondAux {
    int if_limit; // specifies the largest code of a fi_or_else command
    int cur_if;   // is the name of the current type of conditional
    int if_line;  // is the line number at which it began
    int serial;   // is the serial number

    void dump(long i) const;
};

// This is used to implement \if, \else, \fi
struct Condition : public std::vector<CondAux> {
    int if_serial;

    Condition() { if_serial = 0; }

    [[nodiscard]] auto top_serial() const -> int { return empty() ? -1 : back().serial; }
    [[nodiscard]] auto top_branch() const -> int;
    [[nodiscard]] auto top_type() const -> long;
    [[nodiscard]] auto top_level() const { return size(); };
    [[nodiscard]] auto top_limit() const -> int { return empty() ? 0 : back().if_limit; }
    [[nodiscard]] auto top_line() const -> int { return empty() ? 0 : back().if_line; }
    [[nodiscard]] auto is_this_if(size_t n) const -> bool { return size() == n; }

    void pop();
    auto push(int chr) -> size_t;
    void wait_for_fi() { back().if_limit = fi_code; }
    void terminate();
    void set_limit(size_t n, int l) { at(n - 1).if_limit = l; }
    void dump() const;
};
