#include "tralics/AttList.h"
#include "tralics/Buffer.h"
#include "tralics/NameMapper.h"

// Returns a pointer to the pair x=... if it exists, -1 otherwise
auto AttList::has_value(const Istring &x) const -> std::optional<size_t> {
    for (size_t i = 0; i < val.size(); ++i)
        if (val[i].name == x) return i;
    return {};
}

// adds a=b, unless there is already an a in the list.
// In this case, if force is true, removes the old value,
// otherwise does nothing
// Does nothing if b is null (ok if b is empty).
void AttList::push_back(const Istring &a, const Istring &b, bool force) {
    if (b.null()) return;
    auto T = has_value(a);
    if (T) {
        if (force) val[*T].value = b;
        return;
    }
    val.push_back({a, b});
}

// Same function with a name_positions instead of an istring
// Note: istring(a) is the same as the_names[a];
void AttList::push_back(name_positions a, name_positions b, bool force) { push_back(the_names[a], the_names[b], force); }

// Same functions, without a third argument (default is force).
void AttList::push_back(const Istring &n, const Istring &v) { push_back(n, v, true); }

void AttList::push_back(name_positions N, name_positions V) { push_back(the_names[N], the_names[V], true); }

void AttList::push_back(name_positions N, const Istring &v) { push_back(the_names[N], v, true); }

// We should remove the slot....instead of replacing
void AttList::delete_att(name_positions a) {
    auto i = has_value(the_names[a]);
    if (i && (*i > 0)) val[*i].name = Istring();
}

// Prints an att list on a buffer, then a stream.
void AttList::print(std::ostream &fp) const {
    Buffer B;
    B.push_back(*this);
    fp << B;
}
