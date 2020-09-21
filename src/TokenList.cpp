#include "tralics/TokenList.h"
#include "tralics/Parser.h"
#include "txtokenlist.h"

void TokenList::add_env(const std::string &name) {
    TokenList res;
    res.push_back(the_parser.hash_table.begin_token);
    res.splice(res.end(), token_ns::string_to_list(name, true)); // \todo res.append()
    res.splice(res.end(), *this);
    res.push_back(the_parser.hash_table.end_token);
    res.splice(res.end(), token_ns::string_to_list(name, true));
    swap(res);
}

// A number N gives: {\verbatimnumberfont{N}}\space
void TokenList::add_verbatim_number(const Hashtab &H, long n) {
    push_back(H.OB_token);
    push_back(H.verbatim_number_font);
    push_back(H.OB_token);
    token_ns::push_back_i(*this, n);
    push_back(H.CB_token);
    push_back(H.CB_token);
    push_back(H.space_token);
}

// Assumes that the list starts with a brace.
// Returns the number of tokens in sublist with its braces.
// If the sublist is everything, returns -1.
// in case of problem, returns -2.
auto TokenList::block_size() const -> int {
    int res = 0;
    int bl  = 0;
    for (auto C = begin(); C != end();) {
        Token t = *(C++);
        ++res;
        if (token_ns::check_brace(t, bl)) return (C == end()) ? -1 : res;
    }
    return -2;
}

// Here we have to find the character c.
// only top-level characters are considered. Active chars are allowed.
// MX is decreased. Job aborted if it becomes negative.
auto TokenList::expand_nct(size_t n, uchar c, int &MX, TokenList &body) -> bool {
    TokenList                 res;
    bool                      result = false;
    std::array<TokenList, 10> Table; // arguments of the commands
    while (!empty()) {
        auto t = front();
        pop_front();
        if (t.is_a_left_brace()) { // get a whole block
            push_front(t);         // re-enter the brace in the list
            token_ns::fast_get_block(*this, res);
            continue;
        }
        if (t.cmd_val() == 10) continue;             // ignore spaces.
        if (!(t.is_a_char() && t.char_val() == c)) { // just copy
            res.push_back(t);
            continue;
        }
        result = true; // We found something
        MX--;
        if (MX < 0) return true;
        for (size_t k = 0; k < n; k++) Table[k + 1] = token_ns::get_a_param(*this, false);
        TokenList W = Parser::expand_mac_inner(body, Table.data());
        splice(begin(), W);
    }
    splice(end(), res);
    return result;
}

// Interprets *{3}{abc} as 3 copies of abc
// This is used for expanding the header of a table
void TokenList::expand_star() {
    TokenList res;
    while (!empty()) {
        Token t = front();
        if (t.is_a_left_brace()) {
            token_ns::fast_get_block(*this, res);
        } else if (!t.is_star_token()) {
            pop_front();
            res.push_back(t);
        } else {
            pop_front();
            TokenList u = token_ns::fast_get_block(*this);
            TokenList v = token_ns::fast_get_block(*this);
            token_ns::remove_ext_braces(u);
            token_ns::remove_ext_braces(v);
            size_t n = 0;
            while (!u.empty()) {
                Token q = u.front();
                u.pop_front();
                if (!q.is_digit_token()) break;
                n = 10 * n + q.val_as_digit();
                if (n > 1000000) break; // bug?
            }
            while (n > 0) {
                TokenList w = v;
                splice(begin(), w);
                n--;
            }
        }
    }
    swap(res);
}
