// This file contains the code for accents.
// for instance, \'e expands to the character 0351 (internal encoding is
// iso-8859-1)

// Note: The first 256 characters of Unicode are identical to Latin-1. So in
// fact internal encoding is always unicode, latin1 is a red herring here.

#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace accent_ns {
    auto double_a_accent(int acc3) -> unsigned;
    auto double_e_accent(int acc3) -> unsigned;
    auto double_o_accent(int acc3) -> unsigned;
    auto double_u_accent(int acc3) -> unsigned;
    auto double_other_accent(int a, int acc3) -> unsigned;
    void special_acc_hack(TokenList &y);
} // namespace accent_ns

// Fetches the position of a double accent on capital O
auto accent_ns::double_o_accent(int acc3) -> unsigned {
    switch (acc3) {
    case 1: return Odiamacro_cc;
    case 5: return Odotmacro_cc;
    case 6: return Otilmacro_cc;
    case 7: return Oogonmacron_cc;
    case 9: return Otildeaigu_cc;
    case 10: return Obaraigu_cc;
    case 12: return Ociracute_cc;
    case 13: return Obargrave_cc;
    case 16: return Oacuteyy_cc;
    case 18: return Ograveyy_cc;
    case 21: return Ociracute_cc;
    case 15: return Ocirgrave_cc;
    case 23: return Ocirxx_cc;
    case 24: return Ocirtilde_cc;
    case 25: return Ocirdot_cc;
    case 27: return Otildedp_cc;
    case 28: return Oxxyy_cc;
    case 29: return Otildeyy_cc;
    case 30: return Odotyy_cc;
    default: return unused_accent_even_cc;
    }
}

// Fetches the position of a double accent on capital E
auto accent_ns::double_e_accent(int acc3) -> unsigned {
    if (acc3 == 10) return Ebaracute_cc;
    if (acc3 == 12) return Eciracute_cc;
    if (acc3 == 13) return Ebargrave_cc;
    if (acc3 == 26) return Ecedrond_cc;
    if (acc3 == 15) return Ecirgrave_cc;
    if (acc3 == 23) return Ecirxx_cc;
    if (acc3 == 24) return Ecirtilde_cc;
    if (acc3 == 25) return Ecirdot_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on capital A
auto accent_ns::double_a_accent(int acc3) -> unsigned {
    if (acc3 == 1) return Adiamacro_cc;
    if (acc3 == 5) return Adotmacro_cc;
    if (acc3 == 12) return Aciracute_cc;
    if (acc3 == 15) return Acirgrave_cc;
    if (acc3 == 25) return Acirdot_cc;
    if (acc3 == 23) return Acirxx_cc;
    if (acc3 == 24) return Acirtilde_cc;
    if (acc3 == 34) return Abreveacute_cc;
    if (acc3 == 22) return Abrevegrave_cc;
    if (acc3 == 31) return Abrevexx_cc;
    if (acc3 == 32) return Abrevetilde_cc;
    if (acc3 == 33) return Abrevedot_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on capital U
auto accent_ns::double_u_accent(int acc3) -> unsigned {
    if (acc3 == 1) return Udiamacro_cc;
    if (acc3 == 2) return Udiaacute_cc;
    if (acc3 == 3) return Udiacaron_cc;
    if (acc3 == 4) return Udiagrave_cc;
    if (acc3 == 9) return Utildeacute_cc;
    if (acc3 == 16) return Uacuteyy_cc;
    if (acc3 == 18) return Ugraveyy_cc;
    if (acc3 == 28) return Uxxyy_cc;
    if (acc3 == 29) return Utildeyy_cc;
    if (acc3 == 30) return Udotyy_cc;
    return unused_accent_even_cc;
}

// Fetches the position of a double accent on a, not in the list above
auto accent_ns::double_other_accent(int a, int acc3) -> unsigned {
    if (a == 'I') {
        if (acc3 == 2) return Itremaacute_cc;
    }
    if (a == 'S') {
        if (acc3 == 11) return Sdotacute_cc;
        if (acc3 == 19) return Sdotcaron_cc;
        if (acc3 == 20) return Sdotdot_cc;
    }
    if (a == 'C') {
        if (acc3 == 17) return Ccedilacute_cc;
    }
    if (a == 'L') {
        if (acc3 == 14) return Ldotbar_cc;
    }
    if (a == 'R') {
        if (acc3 == 14) return Rdotbar_cc;
    }
    if ('a' <= a && a <= 'z') return double_other_accent(a - 'a' + 'A', acc3) + 1;
    return unused_accent_even_cc;
}

// removes some useless braces.
void accent_ns::special_acc_hack(TokenList &y) {
    if (y.empty()) return;
    if (y.front().is_space_token()) y.pop_front();
    if (y.empty()) return;
    Token T = y.front();
    if (!T.is_OB_token()) return;
    y.pop_front();
    if (y.empty()) return; // this should not happen
    Token T2 = y.front();
    y.pop_front();
    if (y.empty()) return; // this should not happen
    if (y.front().is_CB_token()) {
        y.pop_front();
        y.push_front(T2);
    } else {
        y.push_front(T2);
        y.push_front(T);
    }
}
