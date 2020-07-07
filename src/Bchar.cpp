#include "tralics/Bchar.h"
#include "tralics/Bibtex.h"
#include "tralics/Logger.h"
#include "tralics/util.h"

auto operator<<(std::ostream &X, const Bchar &Y) -> std::ostream & {
    for (auto k = Y.first; k < Y.last; ++k)
        if (Y.table[k] != bct_bad) X << name_buffer[k];
    return X;
}

void Bchar::find_a_comma(size_t &first_c, size_t &second_c, size_t &howmany) const {
    for (auto i = first; i < last; i++)
        if (table[i] == bct_comma) {
            if (howmany == 0) first_c = i;
            if (howmany == 1) second_c = i;
            howmany++;
        }
}

auto Bchar::find_a_space() const -> size_t {
    for (auto i = last; i > first; i--)
        if (like_special_space(i - 1)) return i - 1;
    return last;
}

auto Bchar::find_a_lower() const -> size_t {
    for (auto i = first; i < last - 1; i++) {
        if (!like_space(i)) continue;
        char c = '0';
        if (table[i + 1] == bct_extended) continue; // too complicated a case
        if (table[i + 1] == bct_normal) c = name_buffer[i + 1];
        if (table[i + 1] == bct_cmd) c = name_buffer[i + 3];
        if (is_lower_case(c)) return i;
    }
    return last;
}

void Bchar::invent_spaces() const {
    for (auto i = first; i < last; i++)
        if (table[i] == bct_normal && name_buffer.insert_space_here(i)) table[i] = bct_dot;
}

// Returns true if character can be removed (between names)
auto Bchar::is_junk(size_t i) const -> bool {
    bchar_type b = table[i];
    if (b == bct_comma) {
        Bibtex::err_in_entry("misplaced comma in bibtex name\n");
        log_and_tty << "you should say \"{},{},foo\", instead of  \",,foo\" in \n" << name_buffer << ".\n";
    }
    if (b == bct_space || b == bct_tilde || b == bct_dash || b == bct_comma) return true;
    if (b == bct_bad || b == bct_continuation) return true;
    return false;
}

// Remove initial and final junk (space, dash, tilde).
void Bchar::remove_junk() {
    if (empty()) return;
    for (auto i = first; i < last; i++)
        if (is_junk(i))
            first = i + 1;
        else
            break;
    for (auto j = last; j > first; j--) {
        if (table[j - 1] == bct_continuation || table[j - 1] == bct_bad) continue;
        if (is_junk(j - 1))
            last = j - 1;
        else
            break;
    }
}

// We use the fact that first cannot be zero
void Bchar::make_key(bool sw, Buffer &B) {
    auto       oldfirst = first;
    auto       w        = first - 1;
    bchar_type old      = table[w];
    table[w]            = bct_space;
    make_key_aux(sw, B);
    table[w] = old;
    first    = oldfirst;
}

void Bchar::make_key_aux(bool sw, Buffer &B) {
    int k = 0;
    for (auto i = first; i < last; i++)
        if (is_name_start(i)) k++;
    if (k >= 3 || sw) {
        while (first < last) {
            if (is_name_start(first)) { print_for_key(B); }
            first++;
        }
    } else {
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
        if (first >= last) return;
        if (is_printable())
            first = print_for_key(B);
        else
            first++;
    }
}

auto Bchar::is_name_start(size_t i) const -> bool {
    bchar_type A = table[i - 1];
    bchar_type B = table[i];
    if (A != bct_space && A != bct_dash && A != bct_tilde) return false;
    if (B != bct_normal && B != bct_cmd && B != bct_brace) return false;
    return true;
}

auto Bchar::print_for_key(Buffer &X) -> size_t {
    auto i = first;
    while (i < last && table[i] == bct_bad) i++;
    if (i >= last) return i;
    if (table[i] == bct_brace && is_letter(name_buffer[i + 1])) {
        X.push_back(name_buffer[i + 1]);
        i++;
        while (table[i] == bct_continuation) i++;
        return i;
    }
    if (table[i] == bct_cmd) {
        X.push_back(name_buffer[i + 2]);
        return i + 3;
    }
    return print(X);
}

auto Bchar::print(Buffer &X) const -> size_t {
    auto i = first;
    while (i < last && table[i] == bct_bad) i++;
    if (i >= last) return i;
    X.push_back(name_buffer[i]);
    i++;
    while (i < last && table[i] == bct_continuation) {
        X.push_back(name_buffer[i]);
        i++;
    }
    return i;
}

auto Bchar::special_print(Buffer &X, bool sw) -> size_t {
    auto i = print(X);
    if (sw) X.push_back('.');
    if (X.ends_with("..") || X.ends_with(".}.")) X.pop_back();
    return i;
}

void Bchar::print_first_name(Buffer &B1, Buffer &B2) {
    bool print_next = true;
    while (first < last) {
        bchar_type c = table[first];
        if (c == bct_space || c == bct_tilde || c == bct_dash || c == bct_dot) {
            auto i = special_print(B1, false);
            special_print(B2, false);
            print_next = true;
            first      = i;
        } else if (print_next) {
            auto i = special_print(B1, true);
            special_print(B2, true);
            print_next = false;
            first      = i;
        } else
            first++;
    }
}
