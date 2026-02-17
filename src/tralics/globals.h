#pragma once
#include "Xml.h"
#include "enums.h"
#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class SaveAuxEnv;

// TODO: next are global functions, should we do something with them?

inline auto math_to_sub(math_list_type x) -> subtypes { return subtypes(x - fml_offset); }

namespace arith_ns {
    auto nx_plus_y(long n, long x, long y) -> long;
    auto n_times_x(int n, ScaledInt x) -> ScaledInt;
    void scaled_div(ScaledInt &x, int n);
    auto xn_over_d(long x, long n, long d, long &remainder) -> long;
    auto quotient(int n, int d) -> int;
    auto add_ovf(ScaledInt x, ScaledInt y) -> int;
} // namespace arith_ns

namespace main_ns {
    auto search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>; // Searches for a file in conf_path
} // namespace main_ns

namespace math_ns {
    void add_attribute_spec(const std::string &a, const std::string &b);
    auto handle_hspace(Buffer &B) -> Xml *;
    auto handle_space(Buffer &) -> Xml *;
    void insert_delimiter(del_pos k);
    void insert_delimiter_t(del_pos k);
    auto math_space_code(subtypes c) -> bool;
    auto make_sup(Xml *xval) -> Xml *;
    auto mk_mi(char32_t c) -> Xml *;
    auto mk_mi(uchar c, size_t font) -> Xml *;
    auto mk_space(const std::string &a) -> Xml *;
    void fill_math_char_slots();
    void fill_math_char_slots_ent();
    void fill_single_char();
    auto next_math_style(math_style x) -> math_style;
    auto next_frac_style(math_style x) -> math_style;
    auto special_fence(subtypes s, size_t &open, size_t &close) -> bool; // TODO: return a pair?
    auto style_level(subtypes tt) -> math_style;
    auto make_math_char(uchar c, size_t n) -> Xml *;
    auto xml2sons(std::string elt, Xml *first_arg, Xml *second_arg) -> Xml *;
} // namespace math_ns

namespace token_ns {
    auto is_sublist(TokenList::iterator A, TokenList::iterator B, int n) -> bool;
    auto check_brace(Token x, int &bl) -> bool;
    auto compare(const TokenList &A, const TokenList &B) -> bool;
} // namespace token_ns

namespace tralics_ns {
    auto math_env_name(subtypes c) -> std::optional<std::string_view>;
    void add_ref(long v, const std::string &s, bool idx);
    void find_index_labels(std::vector<std::string> &W);
} // namespace tralics_ns
