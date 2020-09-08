#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"

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

// We can have Foo="bar'", printed as Foo='bar&apos;'
// The case Foo'bar="&gee" is not handled.
// nothing is printed in case the name starts with an apostrophe.
// Print in reverse order, because this was in the previous version
// \todo revert that inverse order thing

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream & {
    Buffer B;
    for (auto i = a.size(); i > 0; i--) B.push_back(a[i - 1]);
    return o << B;
}
