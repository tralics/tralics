#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/NameMapper.h"

auto AttList::lookup(const Istring &x) const -> std::optional<size_t> {
    for (size_t i = 0; i < size(); ++i)
        if (at(i).name == x) return i;
    return {};
}

void AttList::push_back(const Istring &name, const Istring &value, bool force) {
    if (value.null()) return;
    if (auto T = lookup(name)) {
        if (force) at(*T).value = value;
        return;
    }
    push_back({name, value});
}

void AttList::push_back(name_positions name, name_positions value, bool force) { push_back(the_names[name], the_names[value], force); }

void AttList::push_back(name_positions N, const Istring &v) { push_back(the_names[N], v, true); }

void AttList::delete_att(name_positions a) {
    if (auto i = lookup(the_names[a])) at(*i).name = Istring(); // \todo delete entry instead
}

// Prints an att list on a buffer, then a stream.
void AttList::print(std::ostream &fp) const {
    Buffer B;
    B.push_back(*this);
    fp << B;
}
