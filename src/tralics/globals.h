#pragma once
#include "WordList.h"
#include "Xml.h"
#include "enums.h"
#include <array>
#include <filesystem>
#include <gsl/gsl>
#include <optional>
#include <string>
#include <vector>

class LabelInfo;

inline bool                                                         bad_minus{false};
inline bool                                                         bib_allow_break{true};
inline bool                                                         compatibility{false};
inline bool                                                         cur_sec_no_topic = false;
inline bool                                                         have_default_ur  = false;
inline bool                                                         nofloat_hack{false};
inline bool                                                         only_input_data{false};
inline bool                                                         raw_bib{false};
inline bool                                                         seen_enddocument{false};
inline bool                                                         distinguish_refer = false;
inline bool                                                         global_in_load    = false;
inline bool                                                         global_in_url     = false;
inline bool                                                         in_hlinee, have_above, have_below;
inline bool                                                         no_xml_error{false};
inline bool                                                         old_ra = false;
inline bool                                                         ra_ok{true};
inline bool                                                         use_quotes{false};
inline bool                                                         xkv_is_global;
inline bool                                                         xkv_is_save;
inline char32_t                                                     leftquote_val{'`'};
inline char32_t                                                     rightquote_val{'\''};
inline int                                                          bad_chars{0};
inline int                                                          cur_entry_line;    // position of entry in source file
inline int                                                          cur_file_line{0};  // current line number
inline int                                                          init_file_pos = 0; // position in init file
inline int                                                          nb_errs{0};
inline int                                                          nb_words = 0;
inline long                                                         cline_first, cline_last;
inline long                                                         composition_section = -1;
inline size_t                                                       ur_size{0};
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
inline std::string                                                  the_default_rc; // \todo RA stuff
inline std::string                                                  the_tag;
inline std::string                                                  xkv_header;
inline std::string                                                  xkv_prefix;
inline std::vector<std::filesystem::path>                           conf_path{"../confdir"};
inline std::vector<std::filesystem::path>                           input_path;
inline std::vector<std::pair<size_t, std::string>>                  ref_list;       // list of all \ref
inline std::vector<std::pair<std::string, LabelInfo *>>             defined_labels; // list of all \label
inline std::vector<std::pair<String, std::string>>                  removed_labels; // list of all \label removed
inline std::vector<std::string>                                     omitcite_list;

// \todo next are global functions, should we do something with them?

auto        next_label_id() -> std::string;
auto        null_cs_name() -> std::string;
inline auto math_to_sub(math_list_type x) -> subtypes { return subtypes(x - fml_offset); }

namespace main_ns {
    auto search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>; ///< Searches for a file in conf_path
} // namespace main_ns

namespace math_ns {
    void add_attribute_spec(const std::string &a, const std::string &b);
    auto cv_special_string(int c) -> std::string;
    auto get_builtin(size_t p) -> Xml *;
    auto get_builtin_alt(size_t p) -> Xml *;
    auto get_delimiter(CmdChr X) -> del_pos;
    auto get_delimiter(int k) -> del_pos;
    auto handle_hspace(Buffer &B) -> Xml *;
    auto handle_space(Buffer &) -> Xml *;
    void insert_delimiter(del_pos k);
    void insert_delimiter_t(del_pos k);
    auto math_constants(int c) -> Xml *;
    auto math_space_code(int c) -> bool;
    auto make_sup(Xml *xval) -> Xml *;
    auto mk_mi(char32_t c) -> Xml *;
    auto mk_mi(uchar c, size_t font) -> Xml *;
    auto mk_space(const std::string &a) -> Xml *;
    auto nb_args_for_cmd(int c) -> int;
    void fill_math_char_slots();
    void fill_math_char_slots_ent();
    void fill_single_char();
    auto next_math_style(math_style x) -> math_style;
    auto next_frac_style(math_style x) -> math_style;
    auto special_fence(subtypes s, size_t &open, size_t &close) -> bool; // \todo return a pair?
    auto style_level(subtypes tt) -> math_style;
    auto make_math_char(uchar c, size_t n) -> Xml *;
    auto xml2sons(std::string elt, gsl::not_null<Xml *> first_arg, gsl::not_null<Xml *> second_arg) -> Xml;
} // namespace math_ns

namespace tralics_ns {
    auto math_env_name(subtypes c) -> String;
} // namespace tralics_ns
