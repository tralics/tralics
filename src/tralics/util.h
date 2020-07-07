#pragma once
#include "enums.h"
#include <string>
#include <vector>

auto bt_to_string(boundary_type v) -> std::string;
auto find_counter(const std::string &s) -> int;
auto is_m_font(symcodes cmd) -> bool;
auto only_digits(const std::string &s) -> bool;
auto only_space(const std::string &s) -> bool;
auto save_string_name(size_t n) -> String;
auto split_assign(std::string s) -> std::pair<std::string, std::string>;
auto split_commas(const std::string &S) -> std::vector<std::string>;
auto to_utf8(char32_t c) -> std::string;
auto without_end_spaces(std::string s) -> std::string;

inline auto is_digit(char c) -> bool { return ('0' <= c && c <= '9'); }
inline auto is_space(char c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto is_lower_case(char c) -> bool { return 'a' <= c && c <= 'z'; }
inline auto is_upper_case(char c) -> bool { return 'A' <= c && c <= 'Z'; }
inline auto is_letter(char c) -> bool { return is_upper_case(c) || is_lower_case(c); }
inline auto is_ascii(char32_t c) -> bool { return c < 128; }
inline auto is_big(char32_t c) -> bool { return c > 65535; }
inline auto is_digit(char32_t c) -> bool { return '0' <= c && c <= '9'; }
inline auto is_letter(char32_t c) -> bool { return is_ascii(c) && is_letter(static_cast<char>(c)); }
inline auto is_small(char32_t c) -> bool { return c < 256; }
inline auto is_space(char32_t c) -> bool { return c == ' ' || c == '\t' || c == '\n'; }
inline auto to_lower(char32_t c) -> char32_t { return 'A' <= c && c <= 'Z' ? char32_t(c + ('a' - 'A')) : c; }

auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;
