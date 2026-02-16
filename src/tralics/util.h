#pragma once
#include "enums.h"
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

auto bt_to_string(boundary_type v) -> std::optional<std::string>;
auto codepoints(const std::string &s) -> std::vector<char32_t>;
auto convert_to_latin1(const std::string &s, bool latin1) -> std::string;
auto convert_to_utf8(const std::string &s, size_t wc) -> std::string;
auto encode(const std::string &s) -> std::string;
auto find_counter(const std::string &s) -> int;
auto find_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>;
auto find_in_path(const std::string &s) -> std::optional<std::filesystem::path>;
auto get_out_dir(const std::string &name) -> std::filesystem::path;
auto is_all_ascii(const std::string &s) -> bool;
auto is_leap_year(long y) -> bool; // TODO: std::chrono::year::is_leap
auto is_m_font(symcodes cmd) -> bool;
auto only_digits(const std::string &s) -> bool;
auto only_space(const std::string &s) -> bool;
auto open_file(const std::string &name, bool f) -> std::ofstream;
auto open_file(const std::filesystem::path &name, bool f) -> std::ofstream;
auto remove_digits(std::string s) -> std::string;
auto save_string_name(size_t n) -> std::string_view;
auto split_assign(std::string s) -> std::pair<std::string, std::string>;
auto split_commas(const std::string &S) -> std::vector<std::string>;
auto split_at_colon(const std::string &s) -> std::optional<std::pair<std::string, std::string>>;
auto to_utf8(char32_t c) -> std::string;
auto without_end_spaces(std::string s) -> std::string;
void append_unless_punct(std::string &s, char c);
void append_roman_upper(std::string &s, long n);
void append_roman_lower(std::string &s, long n);
auto special_title(std::string_view s) -> std::string;
auto is_noopsort(std::string_view s, size_t i) -> bool;

auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;
