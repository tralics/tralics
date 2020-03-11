#pragma once
#include "../txvars.h"
#include "codepoint.h"
#include <string>
#include <vector>

class Buffer;
class LinePtr;

extern std::vector<std::string> conf_path;     //
extern std::vector<std::string> input_path;    //
extern std::vector<std::string> other_options; //

extern std::string log_name;        // Name of transcript file
extern std::string file_name;       // Job name, without directory
extern std::string out_dir;         // Output directory
extern std::string no_ext;          // file name without tex extension
extern std::string opt_doctype;     //
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
