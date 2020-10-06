#include "tralics/MathPAux.h"

// Codes are explained in Math::find_paren0.
auto operator<<(std::ostream &fp, const MathPAux &x) -> std::ostream & {
    fp << x.pos;
    switch (x.type) {
    case mt_flag_small_l: fp << 'l'; break;
    case mt_flag_small_m: fp << 'm'; break;
    case mt_flag_small_r: fp << 'r'; break;
    case mt_flag_big: fp << 'b'; break;
    case mt_flag_rel: fp << 'R'; break;
    case mt_flag_bin: fp << 'B'; break;
    default: fp << 'x';
    }
    return fp << ' ';
}
