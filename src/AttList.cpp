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

auto operator<<(std::ostream &o, const AttList &l) -> std::ostream & {
    for (const auto &[name, value] : l) {
        if (name[0] == '\'') continue;
        const char  quote = the_main.double_quote_att ? '\"' : '\'';
        const char *repl  = the_main.double_quote_att ? "&quot;" : "&apos;";

        fmt::print(o, " {}={}", name, quote);
        for (char c : encode(value)) {
            if (c == quote)
                o << repl;
            else
                o << c;
        }
        o << quote;
    }
    return o;
}
