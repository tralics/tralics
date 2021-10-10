#include "tralics/MathP.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"

auto MathP::has_small() const -> bool {
    return std::any_of(begin(), end(), [](const MathPAux &m) { return m.is_small(); });
}

// The idea is to split the list into sublists, L1, L2, L3, etc.
// Separators are big, rel or bin. We say that a formula is acceptable if
// it has either no small, or at least two small. For instance f(a+b)=c
// is acceptable if we split at =, not if we split at +. Something like
// ]a,b[ is acceptable, even though there is a right delimiter
// followed by a left one.

// The next function returns true if the formula is acceptable if we split
// at:  rel and bin if w is true, rel if w is false.
auto MathP::analyse1(bool w) const -> bool {
    int nb_small = 0;
    for (const auto &x : *this) {
        math_types t = x.type;
        if (t == mt_flag_small_m || t == mt_flag_small_l || t == mt_flag_small_r) nb_small++;
        if (t == mt_flag_rel || (w && t == mt_flag_bin)) {
            if (nb_small == 1) return false;
            nb_small = 0;
        }
    }
    return nb_small != 1;
}

// This removes all bin, and then all rel, in order to get an acceptable
// formula.  The result in the example will be
// 0b 2l 6r 9R 10m 12m 13b.
void MathP::remove_binrel() {
    if (analyse1(true)) return;
    remove_if([](const MathPAux &m) { return m.type == mt_flag_bin; });
    if (analyse1(false)) return;
    remove_if([](const MathPAux &m) { return m.type == mt_flag_rel; });
}

// The next function assumes that there is a big at the end of the list.
// Its splits the list in two parts, putting in k the position of the big.
auto MathP::find_big(int &k) -> MathP {
    MathP res;
    while (!empty()) {
        MathPAux N = front();
        pop_front();
        if (N.type == mt_flag_big) {
            k = N.pos;
            return res;
        }
        res.push_back(N);
    }
    the_parser.signal_error("internal bug in find big");
    return {};
}

auto MathP::is_lbr(int &seen_d1, int &seen_d2) const -> bool {
    seen_d1 = -1;
    seen_d2 = -1;
    for (const auto &N : *this) {
        if (!N.is_small()) continue;
        if (seen_d2 != -1) return false;
        if (seen_d1 != -1)
            seen_d2 = N.pos;
        else
            seen_d1 = N.pos;
    }
    return seen_d2 != -1;
}

// Return true if Pairing is OK.
// Handles the case of brakets (\langle  a \mid b\rangle)
auto MathP::find_paren_matched1() const -> bool {
    bool is_out    = true;
    bool allow_mid = false;
    for (const auto &B : *this) {
        if (B.type == mt_flag_small_m) {
            if (!allow_mid) return false;
            allow_mid = false;
        } else if (is_out && B.type == mt_flag_small_l) {
            is_out    = false;
            allow_mid = true;
        } else if (!is_out && B.type == mt_flag_small_r) {
            is_out    = true;
            allow_mid = false;
        } else if (B.type == mt_flag_small_l)
            return false;
        else if (B.type == mt_flag_small_r)
            return false;
    }
    return is_out;
}

void MathP::find_paren_matched2(MathQList &res) const {
    int k = 0;
    aux_buffer.clear();
    for (const auto &B : *this) {
        if (B.type == mt_flag_small_l) k = B.pos;
        if (B.type == mt_flag_small_r) {
            res.push_back({k, B.pos});
            aux_buffer.format("{}, {} ", k, B.pos);
        }
    }
}

auto MathP::find_paren_rec(MathQList &res) const -> bool {
    int  level = 0, p = -1;
    bool allow_mid = false;
    bool retval    = false;
    for (const auto &B : *this) {
        if (B.type == mt_flag_small_m) {
            if (!allow_mid) p = -1;
            allow_mid = false;
        } else if (B.type == mt_flag_small_l) {
            ++level;
            p         = B.pos;
            allow_mid = true;
        } else if (B.type == mt_flag_small_r) {
            if (p >= 0) {
                res.push_back({p, B.pos});
                retval = true;
            }
            p = -1;
            if (level > 0) --level;
        }
    }
    return retval;
}

// Same algo as find_big. We assume that the list is terminated by
// a rel. In the example above, the result will be
//  2l 6r k=9, and  10m 12m k=13
auto MathP::find_relbin(int &k) -> MathP {
    MathP res;
    while (!empty()) {
        MathPAux N = front();
        pop_front();
        if (N.type == mt_flag_rel || N.type == mt_flag_bin) {
            k = N.pos;
            return res;
        }
        res.push_back(N);
    }
    the_parser.signal_error("internal bug in find relbin");
    return {};
}

// As explained above, the current list will be split in smaller
// lists. In the example above, it will be  2l 6r and  10m 12m.
// We call find_paren1 with the additional argument [1,8] and [10,12]
// which are the intervals containing the elements of the list
// (first and last included).
void MathP::find_paren2(int start, MathQList &res, bool verbose) {
    while (!empty()) {
        int   k   = 0;
        MathP cur = find_relbin(k);
        if (verbose) log_file << "MF: Find paren2 k=" << k << " " << cur << "\n";
        if (cur.has_small()) cur.find_paren1(start + 1, k - 1, res, verbose);
        start = k;
    }
}

// This is the last part of find_paren.  Our example
// calls the function with
// (start1,end1) = (1,8), L = 2l 6r
// and   (start1,end1) =(10,12) and L = 10m 12m.
// In the first case, the function adds to res the pair (2,6),
// because this is a left-right. It could add more than one left-right pairs
// In the second case, it adds (10,12), because no left-right pair is found
// and this is the (start1,end1) pair.

void MathP::find_paren1(int start1, int end1, MathQList &res, bool verbose) {
    MathQList t;
    bool      state     = false;
    bool      failed    = false;
    int       start_pos = -1;
    if (verbose) log_file << "MF: Find paren1 (" << start1 << ", " << end1 << ") " << *this << "\n";
    while (!empty()) {
        auto [i, k]   = front();
        bool is_small = front().is_small();
        pop_front();
        if (!is_small) continue;
        if (!state) { // no opening seen
            if (k != mt_flag_small_l) {
                failed = true;
                clear();
            } else {
                start_pos = i;
                state     = true;
            }
        } else { // we have seen an opening
            if (k == mt_flag_small_l) {
                failed = true;
                clear();
            }
            if (k == mt_flag_small_r) {
                state = false;
                t.push_back({start_pos, i});
                if (verbose) log_file << "MF: OK " << start_pos << ' ' << i << ' ' << '\n';
            }
        }
    }
    if (t.empty()) failed = true;
    if (failed || state) {
        t.clear();
        t.push_back({start1, end1});
        if (verbose) log_file << "MF: BB " << start1 << ' ' << end1 << '\n';
    }
    res.splice(res.end(), t);
}
