#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/util.h"
#include <fmt/ostream.h>

auto AttList::lookup(const std::string &x) -> std::string * {
    auto i = find(x);
    if (i != end()) return &(i->second);
    return nullptr;
}

auto operator<<(std::ostream &o, const AttPair &a) -> std::ostream & {
    if (a.name[0] == '\'') return o;
    const char  quote = the_main.double_quote_att ? '\"' : '\'';
    const char *repl  = the_main.double_quote_att ? "&quot;" : "&apos;";

    fmt::print(o, " {}={}", a.name, quote);
    for (char c : encode(a.value)) {
        if (c == quote)
            o << repl;
        else
            o << c;
    }
    return o << quote;
}

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream & {
    for (const auto &i : a) o << AttPair{i.first, i.second};
    return o;
}
