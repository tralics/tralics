#pragma once
#include "enums.h"
#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// \todo move from extern to inline

struct LineList;
class WordList;
class LabelInfo;

inline bool                                                         bad_minus{false};
inline bool                                                         compatibility{false};
inline bool                                                         nofloat_hack{false};
inline bool                                                         only_input_data{false};
inline bool                                                         raw_bib{false};
inline bool                                                         seen_enddocument{false};
inline bool                                                         distinguish_refer = false;
inline bool                                                         global_in_load    = false;
inline bool                                                         global_in_url     = false;
inline bool                                                         in_hlinee, have_above, have_below;
inline bool                                                         old_ra = false;
inline bool                                                         ra_ok{true};
inline char32_t                                                     leftquote_val{'`'};
inline char32_t                                                     rightquote_val{'\''};
inline int                                                          bad_chars{0};
inline int                                                          cur_entry_line;    // position of entry in source file
inline int                                                          cur_file_line{0};  // current line number
inline int                                                          init_file_pos = 0; // position in init file
inline int                                                          nb_errs{0};
inline int                                                          nb_words = 0;
inline long                                                         cline_first, cline_last;
inline std::array<std::array<char32_t, lmaxchar>, max_encoding - 2> custom_table;
inline std::array<std::array<std::string, 15>, 128>                 math_chars;
inline std::array<std::string, 8>                                   ra_pretable;
inline std::array<String, 3>                                        my_constant_table;
inline std::array<WordList *, 100>                                  WL0;
inline std::optional<size_t>                                        pool_position;  // \todo this is a static variable that should disappear
inline std::string                                                  all_themes;     //
inline std::string                                                  cur_entry_name; // name of entry under construction.
inline std::string                                                  cur_file_name{"tty"};
inline std::string                                                  everyjob_string; //
inline std::string                                                  file_name;       // Job name, without directory
inline std::string                                                  hlinee_above, hlinee_width, hlinee_below;
inline std::string                                                  tralics_version{"2.15.4"};
inline std::vector<std::filesystem::path>                           conf_path{"../confdir"};
inline std::vector<std::filesystem::path>                           input_path;
inline std::vector<std::pair<size_t, std::string>>                  ref_list;       // list of all \ref
inline std::vector<std::pair<std::string, LabelInfo *>>             defined_labels; // list of all \label
inline std::vector<std::pair<String, std::string>>                  removed_labels; // list of all \label removed
inline std::vector<std::string>                                     omitcite_list;

auto next_label_id() -> std::string;
auto null_cs_name() -> std::string;

namespace main_ns {
    auto search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>; ///< Searches for a file in conf_path
} // namespace main_ns
