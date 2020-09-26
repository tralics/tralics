#pragma once
#include "enums.h"
#include <optional>
#include <string>
#include <vector>

void readline(char *buffer, size_t screen_size);

auto bt_to_string(boundary_type v) -> std::string;
auto codepoints(const std::string &s) -> std::vector<char32_t>;
auto convert_to_latin1(const std::string &s, bool latin1) -> std::string;
auto convert_to_utf8(const std::string &s, size_t wc) -> std::string;
auto find_counter(const std::string &s) -> int;
auto is_m_font(symcodes cmd) -> bool;
auto only_digits(const std::string &s) -> bool;
auto only_space(const std::string &s) -> bool;
auto remove_digits(std::string s) -> std::string;
auto save_string_name(size_t n) -> String;
auto split_assign(std::string s) -> std::pair<std::string, std::string>;
auto split_commas(const std::string &S) -> std::vector<std::string>;
auto split_at_colon(const std::string &s) -> std::optional<std::pair<std::string, std::string>>;
auto to_utf8(char32_t c) -> std::string;
auto without_end_spaces(std::string s) -> std::string;

inline auto is_big(char32_t c) -> bool { return c > 65535; }
inline auto is_small(char32_t c) -> bool { return c < 256; }

auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;
