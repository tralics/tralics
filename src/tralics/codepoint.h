#pragma once
#include "types.h"
#include <optional>
#include <ostream>
#include <sstream>
#include <utf8.h>

inline auto is_digit(char c) -> bool { return ('0' <= c && c <= '9'); }
inline auto is_space(char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto is_lower_case(char c) -> bool { return 'a' <= c && c <= 'z'; }
inline auto is_upper_case(char c) -> bool { return 'A' <= c && c <= 'Z'; }
inline auto is_letter(char c) -> bool { return is_upper_case(c) || is_lower_case(c); }

inline auto to_utf8(char32_t c) -> std::string {
    std::ostringstream o;
    utf8::append(c, std::ostream_iterator<char>(o));
    return o.str();
}

inline auto is_ascii(char32_t c) -> bool { return c < 128; }
inline auto is_big(char32_t c) -> bool { return c > 65535; }
inline auto is_digit(char32_t c) -> bool { return '0' <= c && c <= '9'; }
inline auto is_letter(char32_t c) -> bool { return is_ascii(c) && is_letter(static_cast<char>(c)); }
inline auto is_small(char32_t c) -> bool { return c < 256; }
inline auto is_space(char32_t c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }

inline auto to_lower(char32_t c) -> char32_t {
    if ('A' <= c && c <= 'Z') return char32_t(c + ('a' - 'A'));
    return c;
}
