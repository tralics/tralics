#include "tralics/ArrayInfo.h"
#include "tralics/AttList.h"
#include "tralics/Token.h"

// Adds a new <u>, <v>, and att-list slot
void ArrayInfo::add_uv(TokenList &u, TokenList &v, const AttList &At) {
    u_table.push_back(u);
    v_table.push_back(v);
    attribs.push_back(At);
}

// This gets u-part or v-part
auto ArrayInfo::get_u_or_v(bool u_or_v, size_t pos) const -> TokenList {
    if (pos >= attribs.size()) return {};
    return u_or_v ? u_table[pos] : v_table[pos];
}

auto ArrayInfo::get_cell_atts(size_t k) const -> AttList {
    if (k >= attribs.size()) return {};
    return attribs[k];
}
