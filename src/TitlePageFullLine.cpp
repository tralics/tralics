#include "tralics/TitlePageFullLine.h"
#include "tralics/Buffer.h"

namespace tpage_ns {
    auto scan_item(Buffer &in, Buffer &out, char del) -> bool;
} // namespace tpage_ns

namespace {
    TpiOneItem Toi;

    auto is_tp_delim(char c) -> bool { return c == '<' || c == '\\' || c == '"'; }

    // Scans an item: prefix plus value. Result in Toi
    // returns the type of the thing to create.
    auto next_item(Buffer &in, Buffer &out) -> tpi_vals {
        Toi.reset();
        char c = 0;
        if (!in.tp_next_char(c)) return tpi_noval;
        if (c == 'a' && in.skip_string("lias")) return tpi_alias;
        if (c == 'e' && in.skip_string("xecute")) return tpi_execute;
        if (c == 'a' && in.skip_string("ction")) return tpi_execute;
        if (!is_tp_delim(c)) {
            Toi.p1 = c;
            if (!in.tp_next_char(c)) return tpi_noval;
            if (!is_tp_delim(c)) {
                Toi.p2 = c;
                if (!in.tp_next_char(c)) return tpi_noval;
                if (!is_tp_delim(c)) return tpi_err;
            }
        }
        if (!tpage_ns::scan_item(in, out, c)) return tpi_err;
        Toi.value = out;
        if (c == '<') return tpi_elt;
        if (c == '\\') return tpi_cmd;
        return tpi_str;
    }
} // namespace

// This reads up to four items.
// returns a number that tells what types have been read.
auto TitlePageFullLine::read() -> int {
    item1.reset();
    item2.reset();
    item3.reset();
    item4.reset();
    int      res = 0;
    tpi_vals w   = next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item1   = Toi;
    item1.v = w;
    res += 1000 * (w - 1);
    if (w == tpi_noval) return res;
    w = next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item2   = Toi;
    item2.v = w;
    res += 100 * (w - 1);
    if (w == tpi_noval) return res;
    w = next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item3   = Toi;
    item3.v = w;
    res += 10 * (w - 1);
    if (w == tpi_noval) return res;
    w = next_item(tp_main_buf, tp_local_buf);
    if (w == tpi_err) return -1;
    item4   = Toi;
    item4.v = w;
    res += (w - 1);
    if (w == tpi_noval) return res;
    char c = 0;
    if (tp_main_buf.tp_next_char(c)) return -1;
    return res;
}

// First attempt at classification
// E=1 C=2 S=3 A=4 e=5
auto TitlePageFullLine::classify(int w, int state) -> tpi_vals {
    if (w == -1) return tpi_err; // forget about this
    flags = 0;
    if (item1.has_a_char() || item4.has_a_char()) return tpi_err;
    if (w == 0) return tpi_zero; // ignore this item
    if (w == 2133) {
        if (state != 0) return tpi_err;
    } else {
        if (state == 0) return tpi_err;
    }
    if (w == 1000) return item2.only_dash() ? tpi_E : tpi_err;
    if (w == 3000) return tpi_S;
    if (w == 4200 || w == 4233) {
        if (item2.has_a_char() || item3.has_a_char()) return tpi_err;
        if (state == 1 && w == 4233) return tpi_ACSS;
        if (state == 2 && w == 4200) return tpi_AC;
    } else if (w == 2230 || w == 2133) {
        if (item2.has_a_char() || item3.has_a_char()) return tpi_err;
        return w == 2230 ? tpi_CCS : tpi_CESS;
    } else if (w == 2100)
        return item2.quest_plus() && !item3.has_a_char() ? tpi_CE : tpi_err;
    else if (w == 5200)
        return !item2.has_a_char() ? tpi_EC : tpi_err;
    else if (w == 2110)
        return item2.question() && !item3.has_a_char() ? tpi_CEE : tpi_err;
    else if (w == 2113)
        return item2.plus() && !item3.has_a_char() ? tpi_CEES : tpi_err;
    if (w != 2130) return tpi_err;
    if (!encode_flags(item2.p1, item3.p1)) return tpi_err;
    return tpi_CES;
}

// Converts two characters into a flag.
auto TitlePageFullLine::encode_flags(char c1, char c2) -> bool {
    flags = 0;
    if (c1 == 'p')
        flags = tp_p_flag;
    else if (c1 == 'e')
        flags = tp_e_flag;
    else if (c1 == 'E')
        flags = tp_p_flag + tp_e_flag;
    else if (c1 == 'q')
        flags = tp_q_flag;
    else if (c1 != 0)
        return false;
    if (c2 == '+')
        flags += tp_plus_flag;
    else if (c2 == 'A')
        flags += tp_A_flag;
    else if (c2 == 'B')
        flags += tp_B_flag;
    else if (c2 == 'C')
        flags += tp_C_flag;
    else if (c2 != 0)
        return false;
    return true;
}
