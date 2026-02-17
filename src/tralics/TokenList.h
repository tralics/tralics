#pragma once
#include "Token.h"
#include <array>
#include <string_view>

struct Hashtab;

struct TokenList : public std::list<Token> {
    TokenList() = default;
    explicit TokenList(long n); // decimal digits as catcode-12 tokens
    TokenList(std::string_view s, bool braced);

    [[nodiscard]] auto block_size() const -> int;
    [[nodiscard]] auto expand_mac_inner(const TokenList *arguments) const -> TokenList;
    template <size_t N> [[nodiscard]] auto expand_mac_inner(const std::array<TokenList, N> &arguments) const -> TokenList {
        return expand_mac_inner(arguments.data());
    }

    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
    void double_hack();
    void append(TokenList &L) { splice(end(), L); }
    void brace_me();
    void expand_star();
    void fast_get_block(TokenList &res); // TODO: rename
    void latex_ctr_fnsymbol(long n);
    void normalise();
    void remove_ext_braces();
    void remove_first_last_space();
    void remove_initial_spaces();
    void show() const;
    void replace_at_toplevel(Token x1, Token x2);
    void push_back_i(long n);
    void sanitize_toplevel(uchar c);
    void sanitize_chars_only();
    void sanitize_with_chars(TokenList &s, long n);
    void prepend(TokenList &L) { splice(begin(), L); }
    void reevaluate0(bool in_env);
    void url_hack();

    auto contains(TokenList &A, bool remove, int &is_in_skipped) -> bool;
    auto expand_nct(size_t n, uchar c, int &MX, const TokenList &body) -> bool;
    auto fast_get_block() -> TokenList;
    auto find(TokenList &A, Token t, bool sw, int &n) -> bool;
    auto get_unique() -> Token;
    void get_unique(Token &t1, Token &t2);
    [[nodiscard]] auto has_a_single_token() const -> bool;
    [[nodiscard]] auto has_a_single_token(Token t) const -> bool;
    auto replace_space(Token x2, Token x3) -> int;
    auto split_at(Token e, Token m, Token m1, TokenList &z, bool s) -> bool;
    auto split_at(Token m, TokenList &z) -> bool;
    auto get_a_param() -> TokenList;
};

inline TokenList onlypreamble; // token-list allowed only in preamble

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;

using EqtbToken = EQTB<TokenList>;
