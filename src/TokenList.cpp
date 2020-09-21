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
