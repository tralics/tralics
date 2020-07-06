#pragma once
#include "types.h"
#include <optional>
#include <ostream>

// \todo move these to util.h

inline auto is_digit(char c) -> bool { return ('0' <= c && c <= '9'); }
inline auto is_space(char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto is_lower_case(char c) -> bool { return 'a' <= c && c <= 'z'; }
inline auto is_upper_case(char c) -> bool { return 'A' <= c && c <= 'Z'; }
inline auto is_letter(char c) -> bool { return is_upper_case(c) || is_lower_case(c); }

/// \todo This might better just vanish (and we would use `char32_t` everywhere
/// in the code) if there is a reasonable path to do that while keeping all the
/// helper functions.

struct codepoint {
    char32_t value{};

    explicit codepoint(unsigned x = 0) noexcept : value(x) {}
    explicit codepoint(size_t c) noexcept : value(static_cast<char32_t>(c)) {}
    explicit codepoint(uchar c) noexcept : value(c) {}
    explicit codepoint(char c) noexcept : value(static_cast<uchar>(c)) {}

    [[nodiscard]] auto is_control() const -> bool { return value < 32; }
    [[nodiscard]] auto is_digit() const -> bool { return '0' <= value && value <= '9'; }
    [[nodiscard]] auto is_letter() const -> bool { return value < 128 && ::is_letter(static_cast<char>(value)); }
    [[nodiscard]] auto is_small() const -> bool { return value < 256; }
    [[nodiscard]] auto is_space() const -> bool { return value == ' ' || value == '\t' || value == '\n'; }

    [[nodiscard]] auto hex_val() const -> std::optional<unsigned> {
        if (is_digit()) return value - '0';
        if ('a' <= value && value <= 'f') return value - 'a' + 10;
        return {};
    }

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
