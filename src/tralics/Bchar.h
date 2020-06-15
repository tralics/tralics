#pragma once
#include "Buffer.h"

class Bchar {
public:
    size_t      first; // index of first char
    size_t      last;  // index of one-after-last char
    bchar_type *table;

    [[nodiscard]] auto empty() const -> bool { return first >= last; }
    [[nodiscard]] auto find_a_lower() const -> size_t;
    [[nodiscard]] auto find_a_space() const -> size_t;

    void init(size_t i, size_t j) {
        first = i;
        last  = j;
    }
    void init(bchar_type *T) {
        first = 0;
        last  = 0;
        table = T;
    }
    void print_first_name(Buffer &B1, Buffer &B2);
    void make_key(bool sw, Buffer &B);
    void remove_junk();
    void invent_spaces() const;
    void find_a_comma(size_t &first_c, size_t &second_c, size_t &howmany) const;

private:
    [[nodiscard]] auto is_junk(size_t i) const -> bool;
    [[nodiscard]] auto is_name_start(size_t i) const -> bool;

    [[nodiscard]] auto like_space(size_t i) const -> bool {
        bchar_type T = table[i];
        return T == bct_space || T == bct_tilde || T == bct_dash;
    }
    [[nodiscard]] auto like_special_space(size_t i) const -> bool {
        bchar_type T = table[i];
        return T == bct_space || T == bct_tilde || T == bct_dot;
    }
    [[nodiscard]] auto is_printable() const -> bool {
        bchar_type b = table[first];
        return b == bct_normal || b == bct_cmd || b == bct_brace || b == bct_extended;
    }

    void make_key_aux(bool sw, Buffer &B);
    auto print_for_key(Buffer &X) -> size_t;
    auto special_print(Buffer &X, bool sw) -> size_t;
    auto print(Buffer &X) const -> size_t;
};

auto operator<<(Buffer &X, const Bchar &Y) -> Buffer &;
