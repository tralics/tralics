#pragma once
#include "Istring.h"
#include "codepoint.h"
#include <filesystem>
#include <vector>

// \todo move from extern to inline

struct LinePtr;
class WordList;

extern bool                                                          bad_minus;
extern bool                                                          compatibility;
extern bool                                                          nofloat_hack;
extern bool                                                          only_input_data;
extern bool                                                          ra_ok;
extern bool                                                          raw_bib;
extern bool                                                          seen_enddocument;
extern size_t                                                        leftquote_val, rightquote_val;
extern std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;
extern std::string                                                   all_themes;      //
extern std::string                                                   everyjob_string; //
extern std::string                                                   file_name;       // Job name, without directory
extern std::vector<LinePtr>                                          file_pool;       // pool managed by filecontents
extern std::vector<std::filesystem::path>                            conf_path;
inline bool                                                          distinguish_refer = false;
inline bool                                                          global_in_load    = false;
inline bool                                                          global_in_url     = false;
inline bool                                                          old_ra            = false;
inline int                                                           cur_entry_line;    // position of entry in source file
inline int                                                           init_file_pos = 0; // position in init file
inline int                                                           nb_words      = 0;
inline long                                                          cline_first, cline_last;
inline std::array<std::string, 8>                                    ra_pretable;
inline std::array<String, 3>                                         my_constant_table;
inline std::array<WordList *, 100>                                   WL0;
inline std::ostream *                                                cur_fp;         // the XML file
inline std::string                                                   cur_entry_name; // name of entry under construction.
inline std::string                                                   tralics_version{"2.15.4"};
inline std::vector<std::filesystem::path>                            input_path;
inline std::vector<std::pair<Istring, LabelInfo *>>                  defined_labels; // list of all \label
inline std::vector<std::pair<int, Istring>>                          ref_list;       // list of all \ref
inline std::vector<std::pair<String, Istring>>                       removed_labels; // list of all \label removed
inline std::vector<std::string>                                      omitcite_list;

auto next_label_id() -> Istring;
auto null_cs_name() -> std::string;

namespace main_ns {
    void register_file(LinePtr &&x);                                                      ///< Push a file onto the pool
    auto search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>; ///< Searches for a file in conf_path
} // namespace main_ns
