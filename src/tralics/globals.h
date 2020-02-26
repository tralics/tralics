#pragma once
#include "types.h"
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

extern int pool_position; // Position of file in pool

void after_conf_assign(std::vector<std::string> &V);
auto assign(Buffer &a, Buffer &b) -> bool;
void bad_conf(String s);
void set_everyjob(const std::string &s);
