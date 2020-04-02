#pragma once
#include "codepoint.h"
#include "enums.h"
#include <array>
#include <string>
#include <vector>

// \todo move from extern to inline

class Buffer;
struct LinePtr;

extern std::vector<std::string> conf_path;  //
extern std::vector<std::string> input_path; //

extern std::string file_name;       // Job name, without directory
extern std::string all_themes;      //
extern std::string everyjob_string; //

extern std::vector<LinePtr *> file_pool; // pool managed by filecontents

extern std::optional<size_t> pool_position; // Position of file in pool

extern size_t leftquote_val, rightquote_val;

extern bool bad_minus;
extern bool compatibility;
extern bool nofloat_hack;
extern bool only_input_data;
extern bool raw_bib;
extern bool seen_enddocument;

extern std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;

auto assign(Buffer &a, Buffer &b) -> bool;

namespace main_ns {
    void register_file(LinePtr *x);                       ///< Push a file onto the pool
    auto search_in_confdir(const std::string &s) -> bool; ///< Searches for a file in conf_path
} // namespace main_ns
