#include "tralics/AttList.h"
#include "tralics/Buffer.h"
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

auto operator<<(std::ostream &o, const AttList &a) -> std::ostream & {
    Buffer B;
    B.push_back(a);
    return o << B;
}
