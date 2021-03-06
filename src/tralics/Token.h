#pragma once
#include "CmdChr.h"
#include <array>
#include <list>

struct Token {
    size_t val{0};

    explicit Token(size_t x) : val(x) {}
    Token(spec_offsets a, char32_t b) : val(a + b) {}
    Token() = default;

    void kill() { val = 0; }
    void from_cmd_chr(CmdChr X) { val = nb_characters * X.cmd + X.char_val(); }
    void active_char(size_t cs) { val = cs + eqtb_offset; }

    [[nodiscard]] auto active_or_single() const -> bool { return val < first_multitok_val; }
    [[nodiscard]] auto char_or_active() const -> bool { return val < single_offset; }
    [[nodiscard]] auto char_val() const -> char32_t { return char32_t(val % nb_characters); }
    [[nodiscard]] auto chr_val() const -> subtypes { return subtypes(val % nb_characters); }
    [[nodiscard]] auto cmd_val() const -> symcodes { return symcodes(val / nb_characters); }
    [[nodiscard]] auto eqtb_loc() const -> size_t { return val - eqtb_offset; }
    [[nodiscard]] auto hash_loc() const -> size_t { return val - hash_offset; }
    [[nodiscard]] auto is_a_brace() const -> bool { return OB_t_offset <= val && val < RB_limit; }
    [[nodiscard]] auto is_a_char() const -> bool { return val < eqtb_offset; }
    [[nodiscard]] auto not_a_cmd() const -> bool { return val < eqtb_offset; }
    [[nodiscard]] auto is_a_left_brace() const -> bool { return OB_t_offset <= val && val < CB_t_offset; }
    [[nodiscard]] auto is_backquote() const -> bool { return val == other_t_offset + '`'; }
    [[nodiscard]] auto is_bs_cparen() const -> bool { return val == single_offset + ')'; }
    [[nodiscard]] auto is_bs_oparen() const -> bool { return val == single_offset + '('; }
    [[nodiscard]] auto is_CB_token() const -> bool { return CB_t_offset <= val && val < RB_limit; }
    [[nodiscard]] auto is_close_paren() const -> bool { return val == other_t_offset + ')'; }
    [[nodiscard]] auto is_comma_token() const -> bool { return val == other_t_offset + ','; }
    [[nodiscard]] auto is_dec_separator() const -> bool { return val == other_t_offset + ',' || val == other_t_offset + '.'; }
    [[nodiscard]] auto is_digit_token() const -> bool { return val >= other_t_offset + '0' && val <= other_t_offset + '9'; }
    [[nodiscard]] auto is_dot() const -> bool { return val == other_t_offset + '.'; }
    [[nodiscard]] auto is_doublequote() const -> bool { return val == other_t_offset + '\"'; }
    [[nodiscard]] auto is_equal_token() const -> bool { return val == other_t_offset + '='; }
    [[nodiscard]] auto is_exclam_token() const -> bool { return val == other_t_offset + '!'; }
    [[nodiscard]] auto is_hat_token() const -> bool { return val == hat_t_offset + '^'; }
    [[nodiscard]] auto is_in_hash() const -> bool { return val >= hash_offset; }
    [[nodiscard]] auto is_invalid() const -> bool { return val == 0; }
    [[nodiscard]] auto is_letter(uchar c) const -> bool { return val == size_t(letter_t_offset) + c; }
    [[nodiscard]] auto is_lowercase_token() const -> bool { return val >= letter_t_offset + 'a' && val <= letter_t_offset + 'z'; }
    [[nodiscard]] auto is_math_shift() const -> bool { return dollar_t_offset <= val && val < dollar_limit; }
    [[nodiscard]] auto is_minus_token() const -> bool { return val == other_t_offset + '-'; }
    [[nodiscard]] auto is_null() const -> bool { return val == 0; }
    [[nodiscard]] auto is_OB_token() const -> bool { return OB_t_offset <= val && val < CB_t_offset; }
    [[nodiscard]] auto is_one_token() const -> bool { return val == other_t_offset + '1'; }
    [[nodiscard]] auto is_only_space_token() const -> bool { return val == space_token_val; }
    [[nodiscard]] auto is_open_bracket() const -> bool { return val == other_t_offset + '['; }
    [[nodiscard]] auto is_open_paren() const -> bool { return val == other_t_offset + '('; }
    [[nodiscard]] auto is_plus_token() const -> bool { return val == other_t_offset + '+'; }
    [[nodiscard]] auto is_same_token(const Token &x) const -> bool { return val == x.val || (is_space_token() && x.is_space_token()); }
    [[nodiscard]] auto is_singlequote() const -> bool { return val == other_t_offset + '\''; }
    [[nodiscard]] auto is_slash_token() const -> bool { return val == other_t_offset + '/'; }
    [[nodiscard]] auto is_space_token() const -> bool { return val == space_token_val || val == newline_token_val; }
    [[nodiscard]] auto is_star_token() const -> bool { return val == other_t_offset + '*'; }
    [[nodiscard]] auto is_valid() const -> bool { return val != 0; }
    [[nodiscard]] auto is_zero_token() const -> bool { return val == other_t_offset + '0'; }
    [[nodiscard]] auto no_case_letter(char x) const -> bool;
    [[nodiscard]] auto tex_is_digit(unsigned radix) const -> long;
    [[nodiscard]] auto to_string() const -> String;
    [[nodiscard]] auto tok_to_str() const -> std::string;
    [[nodiscard]] auto val_as_digit() const -> size_t { return val - other_t_offset - '0'; }
    [[nodiscard]] auto val_as_letter() const -> size_t { return val - letter_t_offset; }
    [[nodiscard]] auto val_as_other() const -> size_t { return val - other_t_offset; }
};

auto        operator<<(std::ostream &fp, Token x) -> std::ostream &;
inline auto operator==(Token a, Token b) -> bool { return a.val == b.val; }
inline auto operator!=(Token a, Token b) -> bool { return a.val != b.val; }

inline Token T_use1, T_use2, T_usen, T_use_nonen, T_exp_notN, T_exp_notn, T_empty, gen_from_sig_tok, expargsnc_tok, T3col_tok, Tc_zero,
    Tc_true_bool, Tc_false_bool, cmd_name, token_to_split;
inline std::array<Token, 20> special_double;

inline auto make_char_token(unsigned char c, unsigned cat) -> Token { return Token(nb_characters * cat + c); }
