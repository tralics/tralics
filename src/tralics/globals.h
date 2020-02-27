#pragma once
#include "../txvars.h"
#include "codepoint.h"
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

extern uint leftquote_val, rightquote_val;

extern bool bad_minus;
extern bool compatibility;
extern bool nofloat_hack;
extern bool only_input_data;
extern bool raw_bib;
extern bool seen_enddocument;

extern std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;

void after_conf_assign(std::vector<std::string> &V);
auto assign(Buffer &a, Buffer &b) -> bool;
void bad_conf(String s);
void check_for_encoding();
void obsolete(const std::string &s);
void set_everyjob(const std::string &s);
void show_encoding(int wc, const std::string &name);

namespace main_ns {
    void check_in_dir();                                      ///< Adds current directory in input path unless present
    void check_lowercase(Buffer &B);                          ///< Checks that name is non-empty and all lowercase
    auto extract_year(Buffer &B, Buffer &C) -> int;           ///< If B holds apics2006, puts apics in B, 2006 in C, returns 2006
    void find_conf_path();                                    ///< Locate the config dir, using a few sources
    auto hack_for_input(const std::string &s) -> std::string; ///< Sometimes, we want `bar` if `\jobname` is `foo/bar`
    void new_in_dir(String s);                                ///< Split a `:`-separated path list into paths
    void register_file(LinePtr *x);                           ///< Push a file onto the pool
    auto search_in_confdir(const std::string &s) -> bool;     ///< Searches for a file in conf_path
    auto search_in_pool(const std::string &name) -> bool;     ///< Returns true if the file is in the pool
    auto try_conf(const std::string &prefix) -> bool;         ///< Returns true if prefix is the path to the conf_path
    auto use_pool(LinePtr &L) -> bool;                        ///< Use a file from the pool

    auto find_param_type(String s) -> param_args;
    auto param_hack(String a) -> bool;
    void check_year(int y, Buffer &C, const std::string &dclass, const std::string &Y);
} // namespace main_ns

#ifdef _MSC_VER
#include <windows.h> // 'Sleep()'
inline void txsleep(int i) { Sleep(1000 * i); }
#else
#include <unistd.h>
inline void txsleep(int i) { sleep(i); }
#endif
