#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/util.h"
#include <fmt/ostream.h>

auto AttList::lookup(const std::string &x) -> std::string * {
    if (auto i = find(x); i != end()) return &(i->second);
    return nullptr;
}

auto operator<<(std::ostream &o, const AttList &l) -> std::ostream & {
    for (const auto &[name, value] : l) {
        if (name[0] == '\'') continue;
        const char  quote = the_main.double_quote_att ? '\"' : '\'';
        const char *repl  = the_main.double_quote_att ? "&quot;" : "&apos;";
        std::string out;
        for (char c : encode(value)) {
            if (c == quote)
                out += repl;
            else
                out += c;
        }
        fmt::print(o, " {}={}{}{}", name, quote, out, quote);
    }
    return o;
}
