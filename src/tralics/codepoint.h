#pragma once
#include "types.h"
#include <optional>
#include <ostream>

inline auto is_digit(char c) -> bool { return ('0' <= c && c <= '9'); }
inline auto is_space(char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto is_lower_case(char c) -> bool { return 'a' <= c && c <= 'z'; }
inline auto is_upper_case(char c) -> bool { return 'A' <= c && c <= 'Z'; }
inline auto is_letter(char c) -> bool { return is_upper_case(c) || is_lower_case(c); }

struct codepoint {
    char32_t value{};

    explicit codepoint(unsigned x = 0) noexcept : value(x) {}
    explicit codepoint(size_t c) noexcept : value(static_cast<char32_t>(c)) {}
    explicit codepoint(uchar c) noexcept : value(c) {}
    explicit codepoint(char c) noexcept : value(static_cast<uchar>(c)) {}

    [[nodiscard]] auto to_lower() const -> codepoint {
        if ('A' <= value && value <= 'Z') return codepoint(value + ('a' - 'A'));
        return *this;
    }
};

inline auto operator==(const codepoint &a, const codepoint &b) -> bool { return a.value == b.value; }
inline auto operator!=(const codepoint &a, const codepoint &b) -> bool { return a.value != b.value; }
inline auto operator==(const codepoint &a, const unsigned char &b) -> bool { return a.value == unsigned(b); }
inline auto operator!=(const codepoint &a, const unsigned char &b) -> bool { return a.value != unsigned(b); }

auto operator<<(std::ostream &fp, const codepoint &x) -> std::ostream &;

inline auto is_ascii(codepoint c) -> bool { return c.value < 128; }
inline auto is_big(codepoint c) -> bool { return c.value > 65535; }
inline auto is_digit(codepoint c) -> bool { return '0' <= c.value && c.value <= '9'; }
inline auto is_letter(codepoint c) -> bool { return is_ascii(c) && is_letter(static_cast<char>(c.value)); }
inline auto is_small(codepoint c) -> bool { return c.value < 256; }
inline auto is_space(codepoint c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
