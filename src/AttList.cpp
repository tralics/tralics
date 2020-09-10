#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include <fmt/ostream.h>

auto AttList::lookup(const std::string &x) const -> std::optional<size_t> {
    for (size_t i = 0; i < size(); ++i)
        if (at(i).name == x) return i;
    return {};
}

void AttList::push_back(const std::string &name, const std::string &value, bool force) {
    if (auto T = lookup(name)) {
        if (force) at(*T).value = value;
        return;
    }
    push_back({name, value});
}

auto operator<<(std::ostream &o, const AttPair &a) -> std::ostream & {
    if (a.name[0] == '\'') return o;
    const char  quote = the_main->double_quote_att ? '\"' : '\'';
    const char *repl  = the_main->double_quote_att ? "&quot;" : "&apos;";

    fmt::print(o, " {}={}", a.name, quote);
    for (char c : Buffer(a.value).convert_to_out_encoding()) {
        if (c == quote)
            o << repl;
        else
            o << c;
    }
    return o << quote;
}

// We can have Foo="bar'", printed as Foo='bar&apos;'
// The case Foo'bar="&gee" is not handled.
// nothing is printed in case the name starts with an apostrophe.
// Print in reverse order, because this was in the previous version
// \todo revert that inverse order thing

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream & {
    for (auto i = a.size(); i > 0; i--) o << a[i - 1];
    return o;
}
