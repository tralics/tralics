#pragma once
#include "AttList.h"
#include "TokenList.h"
#include "Xid.h"

// Array management
class ArrayInfo {
    std::vector<AttList>   attribs; // positions attributes for each row
    std::vector<TokenList> u_table, v_table;

public:
    Xid    id;         // the id of the table
    size_t cell_no{0}; // current cell number, first on row is zero

    explicit ArrayInfo(Xid a1) : id(a1) {}

    [[nodiscard]] auto get_cell_atts(size_t k) const -> AttList;
    [[nodiscard]] auto get_u_or_v(bool u_or_v, size_t pos) const -> TokenList;
    void               add_uv(TokenList &u, TokenList &v, const AttList &At);
};
