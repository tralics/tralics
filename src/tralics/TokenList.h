#pragma once
#include "Token.h"
#include <array>

struct Hashtab;

struct TokenList : public std::list<Token> {
    [[nodiscard]] auto block_size() const -> int;
    [[nodiscard]] auto expand_mac_inner(const TokenList *arguments) const -> TokenList;
    template <size_t N> [[nodiscard]] auto expand_mac_inner(const std::array<TokenList, N> &arguments) const -> TokenList {
        return expand_mac_inner(arguments.data());
    }

    void add_env(const std::string &name);
    void add_verbatim_number(const Hashtab &H, long n);
    void append(TokenList &L) { splice(end(), L); }
    void brace_me();
    void expand_star();
    void fast_get_block(TokenList &res); // TODO: rename
    void latex_ctr_fnsymbol(long n);
    void normalise();
    void prepend(TokenList &L) { splice(begin(), L); }
    void reevaluate0(bool in_env);
    void url_hack();

    auto contains(TokenList &A, bool remove, int &is_in_skipped) -> bool;
    auto expand_nct(size_t n, uchar c, int &MX, const TokenList &body) -> bool;
    auto fast_get_block() -> TokenList;
    auto find(TokenList &A, Token t, bool sw, int &n) -> bool;
    auto get_a_param() -> TokenList;
};

inline TokenList onlypreamble; // token-list allowed only in preamble

auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;

using EqtbToken = EQTB<TokenList>;
