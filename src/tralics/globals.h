#pragma once
#include "Xml.h"
#include "enums.h"
#include "fmt_compat.h"
#include <array>
#include <filesystem>
#include <gsl/gsl>
#include <optional>
#include <string>
#include <vector>

class SaveAuxEnv;

// \todo next are global functions, should we do something with them?

auto        next_label_id() -> std::string;
inline auto math_to_sub(math_list_type x) -> subtypes { return subtypes(x - fml_offset); }
auto        fonts1(const std::string &x) -> Xml *;
auto        first_boundary() -> boundary_type;
auto        is_env_on_stack(const std::string &s) -> SaveAuxEnv *;

namespace arith_ns {
    auto nx_plus_y(long n, long x, long y) -> long;
    auto n_times_x(int n, ScaledInt x) -> ScaledInt;
    void scaled_div(ScaledInt &x, int n);
    auto xn_over_d(long x, long n, long d, long &remainder) -> long;
    auto quotient(int n, int d) -> int;
    auto add_ovf(ScaledInt x, ScaledInt y) -> int;
} // namespace arith_ns

namespace main_ns {
    auto search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path>; ///< Searches for a file in conf_path
} // namespace main_ns

namespace math_ns {
    void add_attribute_spec(const std::string &a, const std::string &b);
    auto get_builtin(size_t p) -> Xml *;
    auto get_builtin_alt(size_t p) -> Xml *;
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
    auto special_fence(subtypes s, size_t &open, size_t &close) -> bool; // \todo return a pair?
    auto style_level(subtypes tt) -> math_style;
    auto make_math_char(uchar c, size_t n) -> Xml *;
    auto xml2sons(std::string elt, gsl::not_null<Xml *> first_arg, gsl::not_null<Xml *> second_arg) -> Xml;
} // namespace math_ns

namespace token_ns {
    auto get_unique(TokenList &L) -> Token;
    void get_unique(TokenList &L, Token &t1, Token &t2);
    auto has_a_single_token(const TokenList &L) -> bool;
    auto has_a_single_token(const TokenList &L, Token t) -> bool;
    auto posint_to_list(long n) -> TokenList;
    void push_back_i(TokenList &L, long n);
    void remove_ext_braces(TokenList &L);
    void remove_initial_spaces(TokenList &L);
    void remove_first_last_space(TokenList &L);
    void replace(TokenList &A, Token x1, Token x2);
    auto replace_space(TokenList &A, Token x2, Token x3) -> int;
    void show(const TokenList &);
    auto split_at(Token e, Token m, Token m1, TokenList &L, TokenList &z, bool s) -> bool;
    auto string_to_list(const std::string &s, bool b) -> TokenList;
    void double_hack(TokenList &key);
    auto split_at(Token m, TokenList &L, TokenList &z) -> bool;
    auto is_sublist(TokenList::iterator A, TokenList::iterator B, int n) -> bool;
    void normalise_list(char c, TokenList &L);
    void sanitize_one(TokenList &L, uchar c);
    void sanitize_one(TokenList &L, TokenList &s, long n);
    void sanitize_one(TokenList &L);
    auto check_brace(Token x, int &bl) -> bool;
    auto compare(const TokenList &A, const TokenList &B) -> bool;
} // namespace token_ns

namespace tralics_ns {
    auto math_env_name(subtypes c) -> String;
    void add_ref(long v, const std::string &s, bool idx);
    void find_index_labels(std::vector<std::string> &W);
} // namespace tralics_ns
