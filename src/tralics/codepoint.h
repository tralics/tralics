#pragma once
#include "types.h"
#include <optional>

inline auto is_digit(char c) -> bool { return ('0' <= c && c <= '9'); }
inline auto is_space(char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto is_lower_case(char c) -> bool { return 'a' <= c && c <= 'z'; }
inline auto is_upper_case(char c) -> bool { return 'A' <= c && c <= 'Z'; }
inline auto is_letter(char c) -> bool { return is_upper_case(c) || is_lower_case(c); }
inline auto to_lower(uchar c) -> uchar {
    if ('A' <= c && c <= 'Z') return c + ('a' - 'A');
    return c;
}

/// \todo This might better just vanish (and we would use `char32_t` everywhere
/// in the code) if there is a reasonable path to do that while keeping all the
/// helper functions.

struct codepoint {
    char32_t value;

    explicit codepoint(unsigned x = 0) noexcept : value(x) {}
    explicit codepoint(size_t c) noexcept : value(static_cast<char32_t>(c)) {}
    explicit codepoint(uchar c) noexcept : value(c) {}
    explicit codepoint(char c) noexcept : value(static_cast<uchar>(c)) {}

    [[nodiscard]] auto is_ascii() const -> bool { return value < 128; }
    [[nodiscard]] auto is_big() const -> bool { return value > 65535; }
    [[nodiscard]] auto is_control() const -> bool { return value < 32; }
    [[nodiscard]] auto is_delete() const -> bool { return value == 127; }
    [[nodiscard]] auto is_digit() const -> bool { return '0' <= value && value <= '9'; }
    [[nodiscard]] auto is_Hex() const -> bool { return 'A' <= value && value <= 'F'; }
    [[nodiscard]] auto is_hex() const -> bool { return 'a' <= value && value <= 'f'; }
    [[nodiscard]] auto is_invalid() const -> bool { return value == 0xFFFF; }
    [[nodiscard]] auto is_letter() const -> bool { return is_ascii() && ::is_letter(static_cast<char>(value)); }
    [[nodiscard]] auto is_lower_case() const -> bool { return 'a' <= value && value <= 'z'; }
    [[nodiscard]] auto is_null() const -> bool { return value == 0; }
    [[nodiscard]] auto is_small() const -> bool { return value < 256; }
    [[nodiscard]] auto is_space() const -> bool { return value == ' ' || value == '\t' || value == '\n'; }
    [[nodiscard]] auto is_upper_case() const -> bool { return 'A' <= value && value <= 'Z'; }
    [[nodiscard]] auto is_verybig() const -> bool { return value > 0x1FFFF; }
    [[nodiscard]] auto non_null() const -> bool { return value != 0; }
    [[nodiscard]] auto val_as_digit() const -> unsigned { return value - '0'; }
    [[nodiscard]] auto val_as_Hex() const -> unsigned { return value - 'A' + 10; }
    [[nodiscard]] auto val_as_hex() const -> unsigned { return value - 'a' + 10; }

    [[nodiscard]] auto hex_val() const -> std::optional<unsigned> {
        if (is_digit()) return val_as_digit();
        if (is_hex()) return val_as_hex();
        return {};
    }

    [[nodiscard]] auto to_lower() const -> codepoint {
        if (is_upper_case()) return codepoint(value + ('a' - 'A'));
        return *this;
    }
};

inline auto operator==(const codepoint &a, const codepoint &b) -> bool { return a.value == b.value; }
inline auto operator!=(const codepoint &a, const codepoint &b) -> bool { return a.value != b.value; }
inline auto operator==(const codepoint &a, const unsigned char &b) -> bool { return a.value == unsigned(b); }
inline auto operator!=(const codepoint &a, const unsigned char &b) -> bool { return a.value != unsigned(b); }
