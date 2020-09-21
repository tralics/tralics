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
