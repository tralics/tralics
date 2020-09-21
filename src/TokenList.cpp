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
