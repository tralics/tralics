// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Tralics, math part II

#include "txmath1.h"
#include "tralics/Parser.h"
#include "txinline.h"
#include <algorithm>
#include <fmt/format.h>

enum { pbm_empty, pbm_start, pbm_end, pbm_att, pbm_att_empty };
static Buffer                 mathml_buffer;
static Buffer                 aux_buffer;
static Buffer                 att_buffer;
extern std::array<Xml *, 128> single_chars;
extern bool                   cmi_verbose;
extern MathDataP              math_data;

// Codes are explained in Math::find_paren0 below.
// This prints one token of the list.
void MathPAux::print(std::ostream &fp) const {
    fp << pos;
    switch (type) {
    case mt_flag_small_l: fp << 'l'; break;
    case mt_flag_small_m: fp << 'm'; break;
    case mt_flag_small_r: fp << 'r'; break;
    case mt_flag_big: fp << 'b'; break;
    case mt_flag_rel: fp << 'R'; break;
    case mt_flag_bin: fp << 'B'; break;
    default: fp << 'x';
    }
    fp << ' ';
}

// This prints the whole MathP list on the stream

auto operator<<(std::ostream &fp, const MathP &X) -> std::ostream & {
    auto C = X.value.begin();
    auto E = X.value.end();
    while (C != E) {
        C->print(fp);
        ++C;
    }
    return fp;
}

// This prints the whole MathQ list on the stream

auto operator<<(std::ostream &fp, const MathQList &X) -> std::ostream & {
    auto C = X.begin();
    auto E = X.end();
    while (C != E) {
        C->print(fp);
        ++C;
    }
    return fp;
}

// ------------------------------------------------------------------------

// Returns true if there is a small.
auto MathP::has_small() const -> bool {
    auto B = value.begin();
    auto E = value.end();
    auto C = std::find_if(B, E, [](const MathPAux &m) { return m.is_small(); });
    return C != E;
}

// Consider the expression \int_0^\infty f(x+y) dx = |z|b
// This will be translated to a list of 13 elements, the first one being
// \int_0^\infty. This element is marked big, and software like Amaya
// show the bars and parentheses with the same height as the integral.
// We want to add some <mrow>s in order to avoid this.

// The next function will return a list of the form
// 0b 2l 4B 6r 9R 10m 12m 13b.
// Here b means big (the integral, and there is a special marker at the end),
// l r m stand for small_l, small_r and small_m (the opening parenthesis,
// the closing parenthesis, and the bar that can be open or close).
// the letters B and R stand for binary and relation (the plus and the equal)

// Our problem is to add some <mrow> so that the small elements (l,r,m)
// will not change size in presence of a big element. The next function returns
// an empty list if there is no problem (no small or no big).
// Returns an empty list in the case $(\frac12)$.

void Math::find_paren0(MathP &aux) const {
    int  unbalance  = 0;
    bool seen_big   = false;
    bool inner_big  = false;
    bool seen_small = false;
    int  i          = 0;
    auto L          = value.begin();
    auto E          = value.end();
    int  nb_pairs   = 0;
    while (L != E) { // create the list of indices
        if (L->get_cmd() == math_xml_cmd) {
            math_types t    = L->get_xmltype();
            bool       keep = false;
            switch (t) {
            case mt_flag_small_l:
                nb_pairs++;
                unbalance++;
                keep       = true;
                seen_small = true;
                break;
            case mt_flag_small_r:
                unbalance--;
                keep       = true;
                seen_small = true;
                break;
            case mt_flag_small_m:
                keep       = true;
                seen_small = true;
                break;
            case mt_flag_bin: keep = true; break;
            case mt_flag_rel: keep = true; break;
            case mt_flag_big:
                if (unbalance <= 0) inner_big = true;
                seen_big = true;
                keep     = true;
                break;
            default: break;
            }
            if (keep) aux.push_back(MathPAux(i, t));
        }
        ++L;
        i++;
    }
    if (seen_big && nb_pairs > 1) inner_big = true;
    if (!inner_big || !seen_small) { // only small objects, or no parens
        aux.clear();
    } else
        aux.push_back(MathPAux(i, mt_flag_big)); // add the end marker.
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
    int  nb_small = 0;
    auto C        = value.begin();
    auto E        = value.end();
    while (C != E) {
        math_types t = C->get_type();
        ++C;
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
    value.remove_if([](const MathPAux &m) { return m.get_type() == mt_flag_bin; });
    if (analyse1(false)) return;
    value.remove_if([](const MathPAux &m) { return m.get_type() == mt_flag_rel; });
}

// The next function assumes that there is a big at the end of the list.
// Its splits the list in two parts, putting in k the position of the big.
auto MathP::find_big(int &k) -> MathP {
    MathP res;
    while (!value.empty()) {
        MathPAux N = value.front();
        value.pop_front();
        if (N.get_type() == mt_flag_big) {
            k = N.get_pos();
            return res;
        }
        res.value.push_back(N);
    }
    the_parser.signal_error("internal bug in find big");
    return MathP();
}

// Returns true if formula is ... left ... right ...
// 2 delims only, a big between them, no big ouside.
//  assumes that there is a big at the end of the list (ignored)
auto MathP::is_lbr(int &seen_d1, int &seen_d2) const -> bool {
    auto B        = value.begin();
    auto E        = value.end();
    seen_d1       = -1;
    seen_d2       = -1;
    bool seen_big = false;
    ;
    if (B == E) return false;
    for (;;) {
        if (B == E) break;
        MathPAux N = *B;
        ++B;
        if (B == E && N.get_type() == mt_flag_big) break;
        if (N.get_type() == mt_flag_big) {
            if (seen_d2 != -1) return false;
            if (seen_d1 == -1) return false;
            seen_big = true;
        } else if (N.is_small()) {
            if (seen_d2 != -1) return false;
            if (seen_d1 != -1)
                seen_d2 = N.get_pos();
            else
                seen_d1 = N.get_pos();
        }
    }
    if (!seen_big) return false;
    return seen_d2 != -1;
}

auto MathP::is_lbr2(int &seen_d1, int &seen_d2) const -> bool {
    auto B  = value.begin();
    auto E  = value.end();
    seen_d1 = -1;
    seen_d2 = -1;
    if (B == E) return false;
    for (;;) {
        if (B == E) break;
        MathPAux N = *B;
        ++B;
        if (N.is_small()) {
            if (seen_d2 != -1) return false;
            if (seen_d1 != -1)
                seen_d2 = N.get_pos();
            else
                seen_d1 = N.get_pos();
        }
    }
    return seen_d2 != -1;
}

// Return true if Pairing is OK.
// Handles the case of brakets (\langle  a \mid b\rangle)
auto MathP::find_paren_matched1() const -> bool {
    auto B         = value.begin();
    auto E         = value.end();
    bool is_out    = true;
    bool allow_mid = false;
    for (;;) {
        if (B == E) return is_out;
        if (B->get_type() == mt_flag_small_m) {
            if (!allow_mid) return false;
            allow_mid = false;
        } else if (is_out && B->get_type() == mt_flag_small_l) {
            is_out    = false;
            allow_mid = true;
        } else if (!is_out && B->get_type() == mt_flag_small_r) {
            is_out    = true;
            allow_mid = false;
        } else if (B->get_type() == mt_flag_small_l)
            return false;
        else if (B->get_type() == mt_flag_small_r)
            return false;
        ++B;
    }
}

void MathP::find_paren_matched2(MathQList &res) const {
    auto B = value.begin();
    auto E = value.end();
    int  k = 0;
    aux_buffer.reset();
    while (B != E) {
        if (B->get_type() == mt_flag_small_l) k = B->get_pos();
        if (B->get_type() == mt_flag_small_r) {
            res.push_back(MathQ(k, B->get_pos()));
            aux_buffer << fmt::format("{}, {} ", k, B->get_pos());
        }
        ++B;
    }
}

auto MathP::find_paren_rec(MathQList &res) const -> bool {
    auto B     = value.begin();
    auto E     = value.end();
    int  level = 0, p = -1;
    bool allow_mid = false;
    bool retval    = false;
    for (;;) {
        if (B == E) return retval;
        if (B->get_type() == mt_flag_small_m) {
            if (!allow_mid) p = -1;
            allow_mid = false;
        } else if (B->get_type() == mt_flag_small_l) {
            ++level;
            p         = B->get_pos();
            allow_mid = true;
        } else if (B->get_type() == mt_flag_small_r) {
            if (p >= 0) {
                res.push_back(MathQ(p, B->get_pos()));
                retval = true;
            }
            p = -1;
            if (level > 0) --level;
        }
        ++B;
    }
}

// The next function takes a math list, and returns a list of positions
// where to insert the <mrow> and </mrow>. In the case where find_paren0
// returns an empty list, there is nothing to do. Otherwise, the list is
// split into sublists, with the big as delimitors.
// For each sublist, if there is no small, nothing has to be done.
// After that, remove_binrel is called. In the example, we start with
// 0b 2l 4B 6r 9R 10m 12m 13b.
// The first sublist is empty, and the second will be
// start=0, 2l 6r 9R 10m 12m 13B
// (the start=0 means that the list starts after 0, the final 13B
// indicates that the list ends before the 13).
// Retval is false if rec, true otherwise
auto Math::find_parens(MathQList &res, bool verbose) const -> bool {
    MathP aux;
    find_paren0(aux);
    if (aux.empty()) return true;
    if (verbose) *(the_log.fp) << "MF: After find paren0 " << aux << "\n";
    if (aux.find_paren_rec(res)) {
        if (verbose) *(the_log.fp) << "MF: rec " << res << "\n";
        return false;
    }
    res.clear();
    int start = -1;
    int end   = -1;
    while (!aux.empty()) {
        if (aux.find_paren_matched1()) {
            aux.find_paren_matched2(res);
            if (verbose) the_log << "MF: matched " << aux_buffer << "\n";
            return true;
        }
        MathP content = aux.find_big(end);
        if (!content.has_small()) { // no small, look at next big
        } else {
            content.remove_binrel();                       // remove useless bin/rel in content
            content.push_back(MathPAux(end, mt_flag_rel)); // add end marker
            if (verbose) *(the_log.fp) << "MF: sublist start=" << start << ' ' << content << "\n";
            content.find_paren2(start, res, verbose);
        }
        start       = end;
        int seen_d1 = 0, seen_d2 = 0;
        if (aux.is_lbr2(seen_d1, seen_d2)) {
            if (verbose) *(the_log.fp) << "MF: LBR " << seen_d1 << ' ' << seen_d2 << "\n";
            res.push_back(MathQ(seen_d1, seen_d2));
            return true;
        }
    }
    return true;
}

// Same algo as find_big. We assume that the list is terminated by
// a rel. In the example above, the result will be
//  2l 6r k=9, and  10m 12m k=13
auto MathP::find_relbin(int &k) -> MathP {
    MathP res;
    while (!empty()) {
        MathPAux N = value.front();
        value.pop_front();
        if (N.get_type() == mt_flag_rel || N.get_type() == mt_flag_bin) {
            k = N.get_pos();
            return res;
        }
        res.value.push_back(N);
    }
    the_parser.signal_error("internal bug in find relbin");
    return MathP();
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
        if (verbose) *(the_log.fp) << "MF: Find paren2 k=" << k << " " << cur << "\n";
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
    if (verbose) *(the_log.fp) << "MF: Find paren1 (" << start1 << ", " << end1 << ") " << *this << "\n";
    while (!empty()) {
        int        i = 0;
        math_types k{};
        value.front().get_both(i, k);
        bool is_small = value.front().is_small();
        value.pop_front();
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
                t.push_back(MathQ(start_pos, i));
                if (verbose) *(the_log.fp) << "MF: OK " << start_pos << ' ' << i << ' ' << '\n';
            }
        }
    }
    if (t.empty()) failed = true;
    if (failed || state) {
        t.clear();
        t.push_back(MathQ(start1, end1));
        if (verbose) *(the_log.fp) << "MF: BB " << start1 << ' ' << end1 << '\n';
    }
    res.splice(res.end(), t);
}

// --------------------------------------------------------------------

// This calls find_parens, which indicates where we should add some <mrow>
// and replaces some values. Uses a class for this
// Returns true if the object is big, false if small.
auto Math::finish_translate1(bool vb) -> bool {
    if (empty()) return false;
    MathF M;
    bool  retval = false;
    for (;;) {
        M.reset();
        bool final = find_parens(M.get_aux(), vb);
        if (add_fence(final, M)) retval = true;
        if (final) return retval;
    }
}

// final is true for the final pass: calls remove_prefix when pushing in
auto Math::add_fence(bool final, MathF &M) -> bool {
    int i = 0; // current index in the list
    M.reset(); // state is now true
    M.change_state();
    bool ret_val     = false;
    bool after_dummy = false;
    while (!empty()) {
        math_types cur_type = front().get_xmltype();
        if (cur_type == mt_flag_big) {
            ret_val = true;
            if (M.in_mrow()) M.make_t_big();
        }
        if (after_dummy) {
            after_dummy = false;
            Xml *xval   = front().remove_prefix();
            if ((xval != nullptr) && !xval->is_xmlc() &&
                (xval->has_name(cst_msup) || xval->has_name(cst_msub) || xval->has_name(cst_msubsup))) {
                M.pop_last(xval);
            }
        }
        if (cur_type == mt_flag_dummy && (final || M.in_mrow())) after_dummy = true;
        if (M.is_next_change(i)) {
            Xml *xval = front().remove_prefix();
            if (cur_type == mt_flag_dummy) {
                if (M.in_mrow()) xval = nullptr;
                // elsestd::cout<< "bad dummy\n"; no error ??
            }
            if (xval != nullptr) M.push_in_t(xval);
            M.handle_t();
            M.change_state();
            if (M.is_next_change(i)) {
                M.handle_t();
                M.change_state();
            }
        } else if (cur_type == mt_flag_dummy) {
            if (!(final || M.in_mrow())) M.push_in_res(front());
        } else if (!M.in_mrow()) {
            if (final)
                M.push_in_res(front().remove_prefix());
            else
                M.push_in_res(front());
        } else
            M.push_in_t(front().remove_prefix());
        pop_front();
        ++i;
    }
    M.finish(value);
    return ret_val;
}

void MathF::pop_last(Xml *xval) {
    Xml *p{nullptr};
    if (!in_mrow() && !res.empty()) {
        p = res.back().remove_prefix();
        res.pop_back();
    } else if (in_mrow() && (t != nullptr))
        p = t->remove_last();
    else
        return; // should  not happen
    xval->replace_first(p);
}

// This is what we do when we see a start or end of mrow;
// state is true inside; next_change is position of next status change
void MathF::change_state() {
    state = !state;
    if (state)
        next_change = next_finish;
    else if (aux.empty())
        next_change = next_finish = -1;
    else {
        aux.front().get_both(next_change, next_finish);
        aux.pop_front();
    }
}

// This emits a mrow if needed
void MathF::handle_t() {
    if (state) {
        res.push_back(MathElt(new Xml(cst_mrow, t), -1, t_big ? mt_flag_big : mt_flag_small));
        t = nullptr;
        the_parser.my_stats.one_more_small();
    }
}

void MathF::push_in_t(Xml *x) {
    if (t == nullptr) t = new Xml(cst_temporary, nullptr);
    t->push_back_unless_nullptr(x);
}

void MathF::finish(MathList &value) {
    if ((t != nullptr) && !t->all_empty()) the_parser.signal_error("internal bug in finish_translate");
    value.swap(res);
}

MathF::MathF() = default;

// debug
void MathF::dump_aux() {
    if (aux.empty()) return;
    auto C = aux.begin();
    auto E = aux.end();
    while (C != E) {
        C->print(std::cout);
        ++C;
    }
    std::cout << "\n";
}

// --------------------------------------------------------------------

// Case no mathml wanted. The result is $\alpha _3$

// This converts a delimiter position into a Latex command
void math_ns::insert_delimiter(del_pos k) {
    Buffer &B = mathml_buffer;
    switch (k) {
    case del_open_ket: B.push_back("&lt;"); return;
    case del_close_ket: B.push_back("&gt;"); return;
    case del_dot: B.push_back("."); return;
    case del_open_par: B.push_back("("); return;
    case del_close_par: B.push_back(")"); return;
    case del_close_bra: B.push_back("]"); return;
    case del_open_bra: B.push_back("["); return;
    case del_vert: B.push_back("|"); return;
    case del_Vert: B.push_back("\\Vert "); return;
    case del_rangle: B.push_back("\\rangle "); return;
    case del_langle: B.push_back("\\langle "); return;
    case del_rbrace: B.push_back("\\rbrace "); return;
    case del_lbrace: B.push_back("\\lbrace "); return;
    case del_rceil: B.push_back("\\rceil "); return;
    case del_lceil: B.push_back("\\lceil "); return;
    case del_rfloor: B.push_back("\\rfloor "); return;
    case del_lfloor: B.push_back("\\lfloor "); return;
    case del_rgroup: B.push_back("\\rgroup "); return;
    case del_lgroup: B.push_back("\\lgroup "); return;
    case del_rmoustache: B.push_back("\\moustache "); return;
    case del_lmoustache: B.push_back("\\moustache "); return;
    case del_uparrow: B.push_back("\\uparrow "); return;
    case del_downarrow: B.push_back("\\downarrow "); return;
    case del_updownarrow: B.push_back("\\updownarrow "); return;
    case del_Uparrow: B.push_back("\\Uparrow "); return;
    case del_Downarrow: B.push_back("\\Downarrow "); return;
    case del_Updownarrow: B.push_back("\\Updownarrow "); return;
    default: B.push_back("\\bad.");
    }
}

void math_ns::insert_delimiter_t(del_pos k) {
    Buffer &B = att_buffer;
    switch (k) {
    case del_open_ket: B.push_back("&lt;"); return;
    case del_close_ket: B.push_back("&gt;"); return;
    case del_dot: B.push_back("."); return;
    case del_open_par: B.push_back("("); return;
    case del_close_par: B.push_back(")"); return;
    case del_close_bra: B.push_back("]"); return;
    case del_open_bra: B.push_back("["); return;
    case del_vert: B.push_back("|"); return;
    case del_Vert: B.push_back("Vert"); return;
    case del_rangle: B.push_back("rangle"); return;
    case del_langle: B.push_back("langle"); return;
    case del_rbrace: B.push_back("rbrace"); return;
    case del_lbrace: B.push_back("lbrace"); return;
    case del_rceil: B.push_back("rceil"); return;
    case del_lceil: B.push_back("lceil"); return;
    case del_rfloor: B.push_back("rfloor"); return;
    case del_lfloor: B.push_back("lfloor"); return;
    case del_rgroup: B.push_back("rgroup"); return;
    case del_lgroup: B.push_back("lgroup"); return;
    case del_rmoustache: B.push_back("moustache"); return;
    case del_lmoustache: B.push_back("moustache"); return;
    case del_uparrow: B.push_back("uparrow"); return;
    case del_updownarrow: B.push_back("updownarrow"); return;
    case del_downarrow: B.push_back("downarrow"); return;
    case del_Uparrow: B.push_back("Uparrow"); return;
    case del_Downarrow: B.push_back("Downarrow"); return;
    case del_Updownarrow: B.push_back("Updownarrow"); return;
    default: B.push_back("bad.");
    }
}

// This inserts something like \alpha, with a space at the end when needed
// if the boolean space is false, no space is added at the end.
void Buffer::push_back_math_token(const CmdChr &x, bool space) {
    if (x.cmd > 16) {
        push_back('\\');
        String s = x.name();
        if (s == nullptr) s = "unknown.";
        push_back_math_aux(s);
        if (!space) return;
        if (s[0] == 0) return;
        if (s[1] == 0 && !is_letter(s[0])) return;
        push_back(' ');
    } else
        push_back_real_utf8(x.char_val());
}

// This inserts something like <alpha>,
// Depending on the type we produce <math/> <math> or </math>
// We must take

void Buffer::push_back_math_tag(const CmdChr &x, int type) {
    if (x.cmd > 16) {
        String s = x.name();
        if (s == nullptr) s = "unknown.";
        push_back_math_tag(s, type);
    } else { // Let's hope no tag needed here
        if (type == pbm_end) return;
        push_back_real_utf8(x.char_val());
    }
}

// We must consider the case where the command is \& or the like
// We produce <elt name='&amp;'> otherwise
void Buffer::push_back_math_tag(String s, int type) {
    auto n  = strlen(s);
    bool ok = true; // true if letter
    for (size_t i = 0; i < n; i++) {
        auto c = s[i];
        if (!is_letter(c)) {
            if (strcmp(s, "@root") == 0) {
                s = "root";
                break;
            }
            ok = false;
            break;
        }
    }
    push_back('<');
    if (type == pbm_end) push_back('/');
    if (ok)
        push_back(s);
    else {
        push_back("elt");
        if (type != pbm_end) {
            push_back(" name='");
            for (size_t i = 0; i < n; i++) push_back_xml_char(uchar(s[i]));
            push_back("'");
        }
    }
    if (type == pbm_att || type == pbm_att_empty) push_back(att_buffer);
    if (type == pbm_empty || type == pbm_att_empty) push_back('/');
    push_back('>');
}

// Case of a string, we check if the string can be inserted directly.
void Buffer::push_back_math_aux(String s) {
    auto n  = strlen(s);
    bool ok = true;
    for (size_t i = 0; i < n; i++) {
        auto c = s[i];
        if (c == '<' || c == '>' || c == '&' || static_cast<uchar>(c) < 32) {
            ok = false;
            break;
        }
    }
    if (ok)
        push_back(s);
    else
        for (size_t i = 0; i < n; i++) push_back_xml_char(uchar(s[i]));
}

// Returns the number of arguments of the command.
auto math_ns::nb_args_for_cmd(int c) -> int {
    if (c >= first_maccent_code && c <= last_maccent_code) return 1;
    switch (c) {
    case mathchoice_code: return 4;
    case operatorname_code: return 1;
    case operatornamestar_code: return 1;
    case overline_code: return 1;
    case underline_code: return 1;
    case stackrel_code: return 2;
    case underset_code: return 2;
    case overset_code: return 2;
    case accentset_code: return 2;
    case underaccent_code: return 2;
    case frac_code: return 2;
    case overbrace_code: return 1;
    case underbrace_code: return 1;
    case undertilde_code: return 1;
    case mathmi_code: return 1;
    case mathmn_code: return 1;
    case mathmo_code: return 1;
    case mathci_code: return 1;
    case mathcn_code: return 1;
    case mathcsymbol_code: return 1;
    case math_attribute_code: return 2;
    case thismath_attribute_code: return 2;
    case formula_attribute_code: return 2;
    case mathlabel_code: return 2;
    case dfrac_code: return 2;
    case tfrac_code: return 2;
    case cfrac_code: return 2;
    case binom_code: return 2;
    case dbinom_code: return 2;
    case tbinom_code: return 2;
    case sqrt_code: return 1;
    case root_code: return 1;
    case genfrac_code: return 2;
    case multicolumn_code: return 3;
    case qopname_code: return 3;
    case mathbox_code: return 1;
    case multiscripts_code: return 1;
    case phantom_code: return 1;
    case hphantom_code: return 1;
    case vphantom_code: return 1;
    case smash_code: return 1;
    case boxed_code: return 1;
    default: return -1;
    }
}

// Not inline because of a bug of the Mac Compiler gcc4.2.1

auto MathElt::get_fml_subtype() const -> subtypes { return math_to_sub(get_lcmd()); }

// Handles the case of a command like \sqrt, \frac
void MathElt::cv_noML_special() const {
    subtypes c = get_fml_subtype();
    CmdChr   Val(special_math_cmd, c);
    Math &   L = get_list();
    if (c == formula_attribute_code || c == thismath_attribute_code) {
        static Buffer att_buffer;
        std::string   s1 = L.get_arg1().convert_this_to_string(att_buffer);
        std::string   s2 = L.get_arg2().convert_this_to_string(att_buffer);
        auto          A  = Istring(s1);
        auto          B  = Istring(s2);
        math_ns::add_attribute_spec(A, B);
        return;
    }
    mathml_buffer.push_back_math_token(Val, false);
    int n = math_ns::nb_args_for_cmd(c);
    if (n == -1) mathml_buffer.push_back("unknown");
    if (c == mathbox_code) {
        mathml_buffer.push_back("{");
        mathml_buffer.push_back(Istring(L.get_sname()));
        mathml_buffer.push_back("}");
    }
    if (c == mathmi_code || c == mathmo_code || c == mathmn_code || c == mathnothing_code || c == mathci_code || c == mathcn_code ||
        c == mathcsymbol_code || c == multiscripts_code || c == mathbox_code) {
        auto X = L.begin();
        auto Y = L.end();
        ++X;
        for (;;) {
            if (X == Y) break;
            mathml_buffer.push_back("[");
            X->get_list().convert_math_noML0();
            ++X;
            mathml_buffer.push_back("='");
            if (X == Y) {
            } // Should we signal an error ?
            else {
                X->get_list().convert_math_noML0();
                ++X;
            }
            mathml_buffer.push_back("']");
        }
    }
    if ((c == smash_code || c == cfrac_code) && !L.empty()) {
        if (!L.get_arg1().empty()) {
            mathml_buffer.push_back('[');
            L.get_arg1().convert_math_noML0();
            mathml_buffer.push_back(']');
        }
        L.pop_front();
    }
    if (c == root_code) {
        mathml_buffer.push_back(" ");
        if (!L.empty()) {
            L.get_arg1().convert_math_noML0();
            L.pop_front();
            mathml_buffer.push_back(" \\of ");
        }
    }
    if (c == genfrac_code) {
        if (L.empty()) return;
        auto k = del_pos(L.front().get_chr());
        if (k == del_dot)
            mathml_buffer.push_back("{}");
        else
            math_ns::insert_delimiter(k);
        L.pop_front();
        if (L.empty()) return;
        k = del_pos(L.front().get_chr());
        if (k == del_dot)
            mathml_buffer.push_back("{}");
        else
            math_ns::insert_delimiter(k);
        L.pop_front();
        auto sz = Istring(L.front().get_chr());
        mathml_buffer << '{' << sz << '}';
        L.pop_front();
        if (L.front().get_cmd() == style_cmd)
            mathml_buffer << fmt::format("{}", math_ns::style_level(L.front().get_chr()));
        else
            mathml_buffer.push_back("{}");
        L.pop_front();
    }
    if (c == xleftarrow_code || c == xrightarrow_code) {
        n = 1;
        if (!L.empty()) {
            if (!L.get_arg1().empty()) {
                mathml_buffer.push_back('[');
                L.get_arg1().convert_math_noML0();
                L.pop_front();
                mathml_buffer.push_back(']');
            }
        }
    }
    while (n > 0) {
        n--;
        mathml_buffer.push_back('{');
        if (!L.empty()) {
            L.get_arg1().convert_math_noML0();
            L.pop_front();
        }
        mathml_buffer.push_back('}');
    }
}

// Handles the case of a command like \sqrt, \frac
void MathElt::cv_noMLt_special() const {
    subtypes c = get_fml_subtype();
    CmdChr   Val(special_math_cmd, c);
    mathml_buffer.push_back_math_tag(Val, pbm_start);
    cv_noMLt_special0();
    mathml_buffer.push_back_math_tag(Val, pbm_end);
}

void MathElt::cv_noMLt_special0() const {
    subtypes c = get_fml_subtype();
    int      n = math_ns::nb_args_for_cmd(c);
    Math &   L = get_list();
    if (n == -1) mathml_buffer.push_back("unknown");
    if (c == mathbox_code) {
        mathml_buffer.push_back("<name>");
        mathml_buffer.push_back(Istring(L.get_sname()));
        mathml_buffer.push_back("</name>");
    }
    if (c == mathmi_code || c == mathmo_code || c == mathmn_code || c == mathnothing_code || c == mathci_code || c == mathcn_code ||
        c == mathcsymbol_code || c == multiscripts_code || c == mathbox_code) {
        auto X = L.begin();
        auto Y = L.end();
        ++X;
        mathml_buffer.remove_last();
        for (;;) {
            if (X == Y) break;
            mathml_buffer.push_back(" ");
            X->get_list().convert_math_noMLt0();
            ++X;
            mathml_buffer.push_back("='");
            if (X == Y) {
            } // Should we signal an error ?
            else {
                X->get_list().convert_math_noMLt0();
                ++X;
            }
            mathml_buffer.push_back("'");
        }
        mathml_buffer.push_back('>'); // re-insert
    }
    if ((c == smash_code || c == cfrac_code) && !L.empty()) {
        if (!L.get_arg1().empty()) {
            mathml_buffer.push_back("<oarg>");
            L.get_arg1().convert_math_noMLt0();
            mathml_buffer.push_back("</oarg>");
        }
        L.pop_front();
    }
    if (c == root_code) {
        if (!L.empty()) {
            mathml_buffer.push_back("<oarg>");
            L.get_arg1().convert_math_noMLt0();
            L.pop_front();
            mathml_buffer.push_back("</oarg>");
        }
    }
    if (c == genfrac_code) {
        if (L.empty()) return;
        mathml_buffer.remove_last();
        auto k = del_pos(L.front().get_chr());
        if (k != del_dot) {
            att_buffer.reset();
            math_ns::insert_delimiter_t(k);
            mathml_buffer << " left='" << att_buffer << "'";
        }
        L.pop_front();
        if (!L.empty()) {
            k = del_pos(L.front().get_chr());
            if (k != del_dot) {
                att_buffer.reset();
                math_ns::insert_delimiter_t(k);
                mathml_buffer << " right='" << att_buffer << "'";
            }
            L.pop_front();
        }
        if (!L.empty()) {
            auto sz = Istring(L.front().get_chr());
            att_buffer.reset();
            att_buffer << sz;
            if (!att_buffer.empty()) mathml_buffer << " size='" << att_buffer << "'";
            L.pop_front();
        }
        if (!L.empty()) {
            if (L.front().get_cmd() == style_cmd) mathml_buffer << fmt::format(" style='{}'", math_ns::style_level(L.front().get_chr()));
            L.pop_front();
        }
        mathml_buffer.push_back('>');
    }
    if (c == xleftarrow_code || c == xrightarrow_code) {
        n = 1;
        if (!L.empty()) {
            if (!L.get_arg1().empty()) {
                mathml_buffer.push_back("<oarg>");
                L.get_arg1().convert_math_noML0();
                L.pop_front();
                mathml_buffer.push_back("</oarg>");
            }
        }
    }
    // optimize this
    if (n == 1 && !L.empty()) {
        L.get_arg1().convert_math_noMLt0();
        L.pop_front();
        --n;
    }
    while (n > 0) {
        n--;
        mathml_buffer.push_back("<arg>");
        if (!L.empty()) {
            L.get_arg1().convert_math_noMLt0();
            L.pop_front();
        }
        mathml_buffer.push_back("</arg>");
    }
}

// Handles the case of a group
void MathElt::cv_noML_list() const {
    Math &         X = get_list();
    math_list_type T = get_lcmd();
    switch (T) {
    case math_open_cd:
        mathml_buffer.push_back('{');
        X.convert_math_noML0();
        mathml_buffer.push_back('}');
        return;
    case math_dollar_cd:
        mathml_buffer.push_back('$');
        X.convert_math_noML0();
        mathml_buffer.push_back('$');
        return;
    case math_LR_cd: X.convert_math_noML0(); return;
    case math_env_cd:
        mathml_buffer << "\\begin{" << X.get_name() << '}';
        X.convert_math_noML0();
        mathml_buffer << "\\end{" << X.get_name() << '}';
        return;
    case math_hbox_cd:
        mathml_buffer << "\\" << X.get_name() << "{";
        X.convert_math_noML0();
        mathml_buffer << "}";
        return;
    default: mathml_buffer << fmt::format("bad group{}", T);
    }
}

// Handles the case of a group
void MathElt::cv_noMLt_list() const {
    Math &         X = get_list();
    math_list_type T = get_lcmd();
    switch (T) {
    case math_open_cd:
        mathml_buffer.push_back("<arg>");
        X.convert_math_noMLt0();
        mathml_buffer.push_back("</arg>");
        return;
    case math_dollar_cd:
        mathml_buffer.push_back("<math>");
        X.convert_math_noMLt0();
        mathml_buffer.push_back("</math>");
        return;

    case math_LR_cd: X.convert_math_noMLt0(); return;
    case math_env_cd:
        mathml_buffer << "<env name='" << X.get_name() << "'>";
        X.convert_math_noMLt0();
        mathml_buffer << "</env>";
        return;
    case math_hbox_cd:
        mathml_buffer << "<" << X.get_name() << ">";
        X.convert_math_noMLt0();
        mathml_buffer << "</" << X.get_name() << ">";
        return;
    default: mathml_buffer.push_back("bad group");
    }
}

void Math::handle_mbox_no() {
    while (!empty()) {
        subtypes font = math_f_normal; // ignore font changes
        int      ok   = M_mbox1(aux_buffer, font);
        if (ok == 0) {
            mathml_buffer.push_back("bad hbox");
            return;
        }
        if (!aux_buffer.empty()) { mathml_buffer << "\\text{" << aux_buffer << "}"; }
        if (ok == 1) return;
        if (ok == 2) continue;
        if (ok == 4) {
            Math w = front().get_list();
            pop_front();
            std::string label = w.convert_opname();
            mathml_buffer.push_back("\\ref{");
            mathml_buffer.push_back(label);
            mathml_buffer.push_back("}");
        } else if (ok == 3)
            mathml_buffer.push_back(" ");
        else if (ok == 5)
            mathml_buffer.push_back("\\quad ");
        else if (ok == 6)
            mathml_buffer.push_back("\\qquad ");
        else if (ok == 7)
            mathml_buffer.push_back("\\!");
        else if (ok == 8)
            mathml_buffer.push_back("\\,");
        else if (ok == 9)
            mathml_buffer.push_back("\\:");
        else if (ok == 10)
            mathml_buffer.push_back("\\;");
        else if (ok == 10)
            mathml_buffer.push_back("\\;");
        else if (ok == 10)
            mathml_buffer.push_back("\\;");
        else if (ok == 10)
            mathml_buffer.push_back("\\hspace{0pt}");
        else {
            Math u = get_list(-ok); // was math_table[-ok];
            u.convert_math_noML0();
        }
    }
}

// Tag version of the previous
void Math::handle_mbox_not() {
    while (!empty()) {
        subtypes font = math_f_normal; // ignore font changes
        int      ok   = M_mbox1(aux_buffer, font);
        if (ok == 0) {
            mathml_buffer.push_back("<error>bad hbox</error>");
            return;
        }
        if (!aux_buffer.empty()) { mathml_buffer << "<text>" << aux_buffer << "</text>"; }
        if (ok == 1) return;
        if (ok == 2) continue;
        if (ok == 4) {
            Math w = front().get_list();
            pop_front();
            std::string label = w.convert_opname();
            mathml_buffer.push_back("<ref>");
            mathml_buffer.push_back(label);
            mathml_buffer.push_back("</ref>");
        } else if (ok == 3)
            mathml_buffer.push_back(" ");
        else if (ok == 5)
            mathml_buffer.push_back("<quad/>");
        else if (ok == 6)
            mathml_buffer.push_back("<qquad/>");
        else if (ok == 7)
            mathml_buffer.push_back("<mspace cmd='!'/>");
        else if (ok == 8)
            mathml_buffer.push_back("<mspace cmd=','/>");
        else if (ok == 9)
            mathml_buffer.push_back("<mspace cmd=':'/>");
        else if (ok == 10)
            mathml_buffer.push_back("<mspace cmd=';'/>");
        else if (ok == 11)
            mathml_buffer.push_back("<mspace cmd='hspace'/>");
        else {
            Math u = get_list(-ok); // was math_table[-ok];
            u.convert_math_noML0();
        }
    }
}

// Define these two macros, because of similar code
#define SELF_INSERT_CASES                                                                                                                  \
    case space_catcode:                                                                                                                    \
    case other_catcode:                                                                                                                    \
    case hat_catcode:                                                                                                                      \
    case underscore_catcode:                                                                                                               \
    case alignment_catcode:                                                                                                                \
    case char_given_cmd:                                                                                                                   \
    case math_given_cmd

#define NORMAL_CASES                                                                                                                       \
    case cst1_cmd:                                                                                                                         \
    case mathordb_cmd:                                                                                                                     \
    case mathord_cmd:                                                                                                                      \
    case mathbin_cmd:                                                                                                                      \
    case mathrel_cmd:                                                                                                                      \
    case mathinner_cmd:                                                                                                                    \
    case mathbetween_cmd:                                                                                                                  \
    case mathopen_cmd:                                                                                                                     \
    case mathop_cmd:                                                                                                                       \
    case mathopn_cmd:                                                                                                                      \
    case mathclose_cmd:                                                                                                                    \
    case big_cmd:                                                                                                                          \
    case mathspace_cmd:                                                                                                                    \
    case backslash_cmd:                                                                                                                    \
    case nonscript_cmd:                                                                                                                    \
    case style_cmd:                                                                                                                        \
    case math_comp_cmd:                                                                                                                    \
    case math_font_cmd:                                                                                                                    \
    case multicolumn_cmd:                                                                                                                  \
    case limits_cmd:                                                                                                                       \
    case over_cmd:                                                                                                                         \
    case ref_cmd

// This inserts a general token.
void MathElt::cv_noML() {
    switch (get_cmd()) {
    SELF_INSERT_CASES:
        mathml_buffer.push_back_real_utf8(get_char());
        return;
    case letter_catcode:
        if (get_char() == '&')
            mathml_buffer.push_back('&');
        else
            mathml_buffer.push_back_real_utf8(get_char());
        return;
    NORMAL_CASES:
        mathml_buffer.push_back_math_token(val, true);
        return;
    case mathfont_cmd: {
        int c = get_chr();
        if (c < 0 || c >= 15) c = 0;
        auto w = the_names[name_positions(np_mml_normal + c)];
        if (w.empty()) return;
        mathml_buffer.push_back("\\");
        mathml_buffer.push_back(w);
        mathml_buffer.push_back(' ');
        return;
    }
    case left_cmd: // left or right
    case right_cmd:
        mathml_buffer.push_back_math_token(val, false);
        math_ns::insert_delimiter(del_pos(get_chr()));
        return;
    case special_math_cmd: // \frac{}{}
        cv_noML_special();
        return;
    case math_list_cmd: // \left...\right
        cv_noML_list();
        return;
    case relax_cmd: {
        static Buffer rb;
        rb.reset();
        rb.push_back(Token(get_font()));
        if (rb.is_equal("\\relax")) return;
        mathml_buffer.push_back(rb.to_string());
        mathml_buffer.push_back(' ');
    }
        return;
    case hspace_cmd:
        mathml_buffer.push_back_math_token(get_cmd_chr(), false);
        mathml_buffer.push_back('{');
        mathml_buffer.push_back(ScaledInt(get_font()), glue_spec_pt);
        mathml_buffer.push_back('}');
        return;
    default: mathml_buffer.push_back_math_token(val, true); return;
    }
}

// This inserts a general token.
void MathElt::cv_noMLt() {
    switch (get_cmd()) {
    SELF_INSERT_CASES:
        mathml_buffer.push_back_real_utf8(get_char());
        return;
    case letter_catcode:
        if (get_char() == '&')
            mathml_buffer.push_back('&');
        else
            mathml_buffer.push_back_real_utf8(get_char());
        return;
    NORMAL_CASES:
        mathml_buffer.push_back_math_tag(val, pbm_empty);
        return;
    case mathfont_cmd: {
        int c = get_chr();
        if (c < 0 || c >= 15) c = 0;
        auto w = the_names[name_positions(np_mml_normal + c)];
        if (w.empty()) return;
        mathml_buffer << "<font name='" << w.value() << "'/>";
        return;
    }
    case left_cmd: // left or right
    case right_cmd:
        att_buffer << bf_reset << " del='";
        math_ns::insert_delimiter_t(del_pos(get_chr()));
        att_buffer << "'";
        mathml_buffer.push_back_math_tag(val, pbm_att_empty);
        return;
    case special_math_cmd: // \frac{}{}
        cv_noMLt_special();
        return;
    case math_list_cmd: // \left...\right
        cv_noMLt_list();
        return;
    case relax_cmd: {
        auto T = Token(get_font());
        auto x = T.hash_loc();
        auto s = the_parser.hash_table[x];
        mathml_buffer.push_back_math_tag(s.c_str(), pbm_empty);
        return;
    }
    case hspace_cmd: {
        int    chr  = get_chr();
        String name = chr == 1 || chr == 3 ? "vspace" : "hspace";
        if (chr >= 2) att_buffer << bf_reset << " star='true'";
        mathml_buffer.push_back_math_tag(name, chr < 2 ? pbm_start : pbm_att);
        mathml_buffer.push_back(ScaledInt(get_font()), glue_spec_pt);
        mathml_buffer.push_back_math_tag(name, pbm_end);
        return;
    }
    default: mathml_buffer.push_back_math_tag(val, pbm_empty); return;
    }
}

// This converts the whole formula. There are some funny exceptions.
void Math::convert_math_noML0() {
    while (!empty()) {
        MathElt cur = front();
        pop_front();
        int cmd = cur.get_cmd();
        if (cmd == mathfont_cmd && (empty() || front().get_cmd() == mathfont_cmd)) continue;
        cur.cv_noML();
    }
}

// Same with tags

void Math::convert_math_noMLt0() {
    while (!empty()) {
        MathElt cur = front();
        pop_front();
        int cmd = cur.get_cmd();
        if (cmd == mathfont_cmd && (empty() || front().get_cmd() == mathfont_cmd)) continue;
        if (!empty() && (cmd == hat_catcode || cmd == underscore_catcode)) {
            cur = front();
            pop_front();
            std::string s = cmd == hat_catcode ? "superscript" : "subscript";
            mathml_buffer << "<" << s << ">";
            cur.cv_noMLt();
            mathml_buffer << "</" << s << ">";
            continue;
        }
        cur.cv_noMLt();
    }
}

// Main function. Converts the buffer into XML, adds attributes.
// If spec is true, we produce <in/> otherwise \in
auto Math::convert_math_noML(bool spec) -> Xml * {
    mathml_buffer.reset();
    if (spec)
        convert_math_noMLt0();
    else
        convert_math_noML0();
    return new Xml(np_texmath, new Xml(Istring(mathml_buffer.to_string())));
}

// --------------------------------------------------
// Special hacks.
// This is to make the other file smaller

// True if the list contains only digits, that are put in the buffer.
auto Math::only_digits(Buffer &B) const -> bool {
    if (value.empty()) return false;
    for (const auto &L : value) {
        if (!L.is_digit()) { return false; }
        B.push_back(L.get_char());
    }
    return true;
}

// Put as many characters as possible in the buffer.
// Returns true if everything was inserted.
// If rec is true, the argument can contain groups; In the case the buffer
// is not reset !!
auto Math::chars_to_mb(Buffer &B, bool rec) const -> bool {
    if (!rec) B.reset();
    auto L = value.begin();
    auto E = value.end();
    for (;;) {
        if (L == E) return true;
        // DEBUG   std::cout<< "cmd:" << B << ".\n";
        CmdChr w = L->get_cmd_chr();
        if (w.is_space() || w.is_letter() || w.is_other()) {
            codepoint c = w.char_val();
            if (c == '<')
                B.push_back("&lt;");
            else if (c == '>')
                B.push_back("&gt;");
            else
                B.push_back(c);
        } else if (w.cmd == cst1_cmd && w.chr == sharp_code)
            B.push_back("#");
        else if (w.cmd == cst1_cmd && w.chr == underscore_code)
            B.push_back("_");
        else if (w.cmd == cst1_cmd && w.chr == amp_code)
            B.push_back("&amp;");
        else if (w.cmd == char_given_cmd)
            B.push_back_real_utf8(w.char_val());
        else if (w.cmd == relax_cmd)
            continue;
        else if (rec && w.cmd == math_list_cmd && L->get_font() == subtypes(math_open_cd)) {
            if (!L->get_list().chars_to_mb(B, true)) return false;
        } else {
            log_and_tty << lg_start << "First invalid token in math-to-string cmd=" << w.cmd << " chr=" << w.chr << lg_end;
            std::cout << "\n";
            return false;
        }
        ++L;
    }
}

// Slightly modified procedure.
auto Math::chars_to_mb1(Buffer &B) const -> bool {
    B.reset();
    auto L = value.begin();
    auto E = value.end();
    for (;;) {
        if (L == E) return true;
        CmdChr w = L->get_cmd_chr();
        if (w.is_space()) {
        } // spaces are ignored
        else if (w.is_letter() || w.is_other())
            B.push_back(w.char_val());
        else if (w.cmd == cst1_cmd && w.chr == comma_code)
            B.push_back(" ");
        else if (w.cmd == relax_cmd)
            continue;
        else if (w.cmd == mathspace_cmd)
            B.push_back(" ");
        else
            return false;
        ++L;
    }
}

// Slightly modified procedure. First token is ignored
auto Math::chars_to_mb2(Buffer &B) const -> bool {
    B.reset();
    auto L = value.begin();
    auto E = value.end();
    if (L == E) return false;
    ++L;
    if (L == E) return false;
    for (;;) {
        if (L == E) return true;
        if (!L->is_char()) return false;
        B.push_back(L->get_char());
        ++L;
    }
}

// Yet another procedure. Reads the dimension in a command like \above
// Something like -1,2cm  is OK
auto Math::chars_to_mb3() -> Istring {
    Buffer B;
    int    bc   = 0; // unit size
    int    sz   = 0; // current size
    bool   dot  = false;
    bool   sign = false;
    for (;;) {
        if (empty()) break;
        if (!front().is_char()) break;
        codepoint C = front().get_char();
        pop_front();
        if (C == '+' || C == '-') {
            if ((sz != 0) || (bc != 0)) {
                sz = 0;
                break;
            }
            if (C == '-') sign = !sign;
            continue;
        }
        if (C == ',') C = codepoint('.');
        if (C == '.') {
            if (dot || (bc != 0)) {
                sz = 0;
                break;
            }
            if (sz == 0) {
                if (sign) B << "-";
                B << "0";
            }
            B.push_back(C);
            dot = true;
            sz++;
            continue;
        }
        if (C.is_digit()) {
            if (bc != 0) {
                sz = 0;
                break;
            }
            if (sz == 0 && sign) B << "-";
            B.push_back(C);
            sz++;
        } else { // unit
            B.push_back(C);
            bc++;
            if (bc == 2) break;
        }
    }
    if (sz == 0 || bc != 2) {
        Buffer tmp;
        tmp.reset();
        tmp << "Error scanning width, so far got '" << B.to_string() << "'";
        the_parser.parse_error(the_parser.err_tok, tmp.c_str(), "bad dimension");
        B.reset();
        B.push_back("0pt");
    }
    return Istring(B.to_string());
}

// Procedure called in case of errors
void Buffer::show_uncomplete(String m) {
    the_parser.signal_error(m);
    if (empty())
        log_and_tty << "No character found\n";
    else
        log_and_tty << "So far, got " << c_str() << "\n";
    reset();
    push_back("error");
}

// Converts a whole math list into a string. May signal an error.
// In this case, the result is `error'.
auto Math::convert_this_to_string(Buffer &B) const -> std::string {
    B.reset();
    if (!chars_to_mb(B, true)) B.show_uncomplete("Bad character in conversion to string");
    return B.to_string();
}

// Use of the alternate command
auto Math::convert_opname() const -> std::string {
    Buffer &B = aux_buffer;
    if (!chars_to_mb1(B)) B.show_uncomplete("Bad character in conversion to string");
    return B.to_string();
}

// Case of \begin{foo}[bar]{gee}ETC, spaces are not yet removed
// This removes the optional argument of a list.
// If argument star is true, removes a *
// The value is in math_buffer, but not used at all.
void Math::remove_opt_arg(bool star) {
    Buffer &B = aux_buffer;
    skip_initial_space();
    if (star && !empty() && front().is_star()) {
        pop_front();
        skip_initial_space();
    }
    if (empty()) return;
    if (!front().is_bracket()) return;
    pop_front();
    while (!empty() && front().is_char()) {
        codepoint c = front().get_char();
        pop_front();
        if (c == ']') return;
        B.push_back(c);
    }
    the_parser.signal_error("missing ]");
}

// Case of \begin{foo}[bar]{gee}ETC, spaces are not yet removed
// Returns the value of a required argument, as a string.
// Here it can be "gee"; this is a sublist.
auto Math::remove_req_arg() -> std::string {
    skip_initial_space();
    if (empty() || !front().is_list()) {
        the_parser.signal_error("missing argument");
        return "error";
    }
    Math &L = front().get_list(); // the sublist containing the argument
    pop_front();
    return L.convert_this_to_string(aux_buffer);
}

auto Math::remove_req_arg_noerr() const -> std::string {
    auto C = value.begin();
    auto E = value.end();
    while (C != E && C->is_space()) ++C;
    if (C == E) return "empty";
    if (!C->is_list()) return "not-list";
    Math &L = C->get_list(); // the sublist containing the argument
    aux_buffer.reset();
    if (!L.chars_to_mb(aux_buffer, true)) return "bad";
    return aux_buffer.to_string();
}

// --------------------------------------------------

// The code in this section tries to replace $10^{i\`eme}$, or absurdities
// like that by 10\textsuperscript{e}.

// This realises the \textsuperscript, given the translation of the argument
auto math_ns::make_sup(Xml *xval) -> Xml * {
    Xml *tmp = Stack::fonts1(np_s_sup);
    tmp->push_back_unless_nullptr(xval);
    return tmp;
}

// This is the first step. Assumes OK false, res=0.
// Leaves OK false, and sets res to the translation of the formula) if only
// digits given.
// In case X^Y, where Y is the tail (can be a group), and X is formed
// of digits, OK is true, res the value of X.

void Math::special2(bool &ok, Xml *&res) const {
    Buffer &B = aux_buffer;
    B.reset();
    for (auto L = value.begin(); L != value.end(); ++L) {
        if (L->get_cmd() == hat_catcode) {
            ++L;                          // skip over hat
            if (L == value.end()) return; // a final hat should not appear
            ++L;
            if (L != value.end()) return; // hack only in case single object after hat
            ok = true;
            break;
        }
        if (L->is_digit())
            B.push_back(L->get_char());
        else if (L->is_list()) {
            if (!L->get_list().only_digits(B)) return;
        } else
            return;
    }
    if (!B.empty()) res = new Xml(Istring(B.to_string()));
}

// This handles the exponent. The case 10^e and 10^o is handled
// elsewhere. We consider here only 10^{something}
auto MathElt::special3() const -> Xml * {
    if (!is_list()) return nullptr;
    const_math_iterator L1;
    const_math_iterator E1;
    get_list().is_font_cmd1_list(L1, E1);
    return math_ns::special_exponent(L1, E1);
}

// Assumes that \textrm{toto}  gives {\rm toto} (wrong!)
// Contructs iterators to begin/end of the list, unless the list is
// a single \hbox, case where the sublist is used.
void Math::is_font_cmd1_list(const_math_iterator &B, const_math_iterator &E) {
    B = value.begin();
    E = value.end();
    if (B == E) return;
    ++B;
    bool ok = false;
    if (B != E) ok = true;
    --B;
    if (ok) return;
    if (value.front().is_hbox()) {
        Math &X = value.front().get_list();
        B       = X.value.begin();
        E       = X.value.end();
    }
}

// This handles the case of i{\`e}me. Removes initial font change,
// looks at letters. If OK, return a valid XML exponent.
auto math_ns::special_exponent(const_math_iterator L, const_math_iterator E) -> Xml * {
    if (L == E) return nullptr;
    if (L->get_cmd() == mathfont_cmd || L->get_cmd() == fontsize_cmd) ++L;
    if (L == E) return nullptr;
    if (L->get_cmd() == mathfont_cmd || L->get_cmd() == fontsize_cmd) ++L;
    Buffer &B = aux_buffer;
    B.reset();
    while (L != E) {
        if (L->is_char())
            B.push_back(L->get_char());
        else if (L->is_e_grave())
            B.push_back(codepoint(0350U));
        else
            return nullptr;
        ++L;
    }
    String expo = B.special_exponent();
    if (expo == nullptr) return nullptr;
    return new Xml(Istring(expo));
}

// True if it is a group containing \grave{e}
auto MathElt::is_e_grave() const -> bool {
    if (get_cmd() != special_math_cmd && get_font() != grave_code) return false;
    Math &  A = get_list();
    Buffer &B = mathml_buffer; // not aux_buffer !!
    if (!A.get_arg1().chars_to_mb(B, false)) return false;
    if (B.single_char() != 'e') return false;
    return true;
}

// This is the list of valid exponents and the conversions.
auto Buffer::special_exponent() const -> String {
    if (strcmp(data(), "th") == 0) return "th";
    if (strcmp(data(), "st") == 0) return "st";
    if (strcmp(data(), "rd") == 0) return "rd";
    if (strcmp(data(), "nd") == 0) return "nd";
    if (strcmp(data(), "e") == 0 || strcmp(data(), "ieme") == 0 || strcmp(data(), "eme") == 0 || strcmp(data(), "i\303\250me") == 0 ||
        strcmp(data(), "\303\250me") == 0)
        return "e";
    if (strcmp(data(), "ier") == 0 || strcmp(data(), "er") == 0) return "er";
    if (strcmp(data(), "iemes") == 0 || strcmp(data(), "i\303\250mes") == 0 || strcmp(data(), "es") == 0) return "es";
    if (strcmp(data(), "\303\250re") == 0 || strcmp(data(), "re") == 0) return "re";
    return nullptr;
}

// This is the main function.
auto Math::special1() const -> Xml * {
    bool ok = false;
    Xml *U{nullptr};
    special2(ok, U);
    if (!ok) return U;
    const MathElt &W = value.back();
    Xml *          xval{nullptr};
    if (W.get_cmd() == letter_catcode && W.get_char() == 'o')
        xval = math_ns::get_builtin(xml_o_loc);
    else if (W.get_cmd() == letter_catcode && W.get_char() == 'e')
        xval = math_ns::get_builtin(xml_e_loc);
    else {
        xval = W.special3();
        if (xval == nullptr) return nullptr;
    }
    xval = math_ns::make_sup(xval);
    if (U == nullptr) return xval;
    Xml *res = new Xml(cst_temporary, nullptr);
    res->push_back_unless_nullptr(U);
    res->push_back_unless_nullptr(xval);
    return res;
}

// ------------------------------------------------------------

// Math font handler.
// \mathcal{xy} gives \cal xy\oldfont, where \oldfont is the current font
// stored somewhere in eqtb. This does not interpret the fonts;
// It justs reads the tokens, and backinputs them
void Parser::TM_fonts() {
    Token *table = hash_table.my_mathfont_table.data();
    int    T     = 0;
    bool   bold  = is_pos_par(atmathversion_code);
    if (cur_cmd_chr.is_math_font()) {
        switch (cur_cmd_chr.chr) {
        case cal_code:
            back_input(table[bold ? math_f_bold_script : math_f_script]);
            back_input(hash_table.nomathsw0_token);
            return;
        case mathtt_code: T = math_f_monospace; break;
        case mathcal_code: T = bold ? math_f_bold_script : math_f_script; break;
        case mathbf_code: T = math_f_bold; break;
        case mathrm_code: T = math_f_upright; break;
        case mathfrak_code: T = bold ? math_f_bold_fraktur : math_f_fraktur; break;
        case mathit_code: T = bold ? math_f_bold_italic : math_f_italic; break;
        case mathnormal_code: T = bold ? math_f_bold : math_f_normal; break;
        case mathbb_code: T = math_f_doublestruck; break;
        case mathsf_code: T = bold ? math_f_bold_sansserif : math_f_sansserif; break;
        default: T = -1;
        }
    } else { // this is a textfont
        switch (cur_cmd_chr.chr) {
        case rm_family_code: T = math_f_upright; break;
        case sf_family_code: T = bold ? math_f_bold_sansserif : math_f_sansserif; break;
        case tt_family_code: T = math_f_monospace; break;
        case bf_series_code: T = math_f_bold; break;
        case it_shape_code:
            T = bold ? math_f_bold_italic : math_f_italic;
            break;
            //    case sc_shape_code:
        case sl_shape_code:
        case normalfont_code: T = math_f_normal; break;
        default: T = -1;
        }
    }
    if (cur_cmd_chr.cmd == oldfont_cmd || cur_cmd_chr.cmd == noargfont_cmd) {
        if (T != -1) {
            back_input(table[T]);
            back_input(hash_table.nomathsw0_token);
        }
        return;
    }
    TokenList L = read_arg();
    auto      c = eqtb_int_table[math_font_pos].val;
    back_input(table[c]);
    back_input(hash_table.nomathsw0_token);
    L.push_back(hash_table.nomathsw1_token);
    L.push_back(Token(other_t_offset, '}'));
    L.push_back(hash_table.endgroup_token);
    L.push_front(Token(other_t_offset, '{'));
    L.push_front(hash_table.nomathsw1_token);
    L.push_front(hash_table.begingroup_token);
    back_input(L);
    if (T != -1) {
        back_input(table[T]);
        back_input(hash_table.nomathsw0_token);
    }
}

// Convert the character c  into <mi>c</mi>
auto math_ns::mk_mi(codepoint c) -> Xml * {
    aux_buffer.reset();
    aux_buffer.push_back_real_utf8(c);
    Xml *x = new Xml(Istring(aux_buffer.to_string()));
    return new Xml(cst_mi, x);
}

// Converts a letter with a into into <mi mathvariant='foo'>X</mi>
// Assumes 2<=font<=14 and 'a'<=c<='z' || 'A'<=c<='Z'
auto math_ns::mk_mi(uchar c, int font) -> Xml * {
    Xml *x = single_chars[c];
    Xml *y = new Xml(cst_mi, x);
    y->add_att(cst_mathvariant, name_positions(cstf_normal + font));
    return y;
}

// True if this can form a sequence of characters to put in a <mi>
auto MathElt::maybe_seq() const -> bool {
    if (get_cmd() != letter_catcode) return false;
    if (get_font() == 0) return false;
    auto c = get_chr();
    return c < 128 && ::is_letter(char(uchar(c)));
}

// True is this can form a sequence of characters to put in a <mi>
// with the same font as F
auto MathElt::maybe_seq(subtypes f) const -> bool {
    if (get_cmd() != letter_catcode) return false;
    if (get_font() != f) return false;
    auto c = get_chr();
    return c < 128 && ::is_letter(char(uchar(c)));
}

// True is this can form a sequence of digits to put in a <mn>
auto MathElt::maybe_iseq() const -> bool {
    if (get_cmd() != other_catcode) return false;
    auto c = get_chr();
    return c < 128 && ::is_digit(char(uchar(c)));
}

// True is this can form a sequence of characters to put in a <mn>
// with the same font as F
auto MathElt::maybe_iseq(subtypes f) const -> bool {
    if (get_cmd() != other_catcode) return false;
    if (get_font() != f) return false;
    auto c = get_chr();
    return c < 128 && ::is_digit(char(uchar(c)));
}

// Converts a character sequence; first char W already removed from
// the list
auto Math::convert_char_seq(MathElt W) -> MathElt {
    subtypes f = W.get_font();
    auto     w = the_parser.eqtb_int_table[mathprop_ctr_code].val;
    Xml *    res{nullptr};
    Buffer & B = aux_buffer;
    B.reset();
    if (f == 1) B.push_back(' ');
    bool     spec = (f == 1) || ((w & (1 << f)) != 0);
    unsigned c    = W.get_chr();
    if (spec)
        B.push_back(char(uchar(c)));
    else
        B.push_back(get_math_char(uchar(c), f));
    for (;;) {
        if (empty()) break;
        if (!front().maybe_seq(f)) break;
        c = front().get_chr();
        if (spec)
            B.push_back(char(uchar(c)));
        else
            B.push_back(get_math_char(uchar(c), f));
        pop_front();
    }
    if (f == 1) B.push_back(' ');
    res = new Xml(Istring(B.to_string()));
    res = new Xml(cst_mi, res);
    if (f > 1 && spec) res->add_att(cst_mathvariant, name_positions(long(cstf_normal) + long(f)));
    return MathElt(res, mt_flag_small);
}

// Converts a character sequence; first char W already removed from
// the list; if bool true reads some chars, otherwise,
// reads only a single one.
auto Math::convert_char_iseq(MathElt W, bool multiple) -> MathElt {
    subtypes f = W.get_font();
    Buffer & B = aux_buffer;
    B.reset();
    unsigned c = W.get_chr();
    B.push_back(char(uchar(c)));
    if (multiple)
        for (;;) {
            if (empty()) break;
            if (!front().maybe_iseq(f)) break;
            c = front().get_chr();
            B.push_back(char(uchar(c)));
            pop_front();
        }
    Xml *res = new Xml(Istring(B.to_string()));
    res      = new Xml(cst_mn, res);
    if (f > 1) res->add_att(cst_mathvariant, name_positions(long(cstf_normal) + long(f)));
    return MathElt(res, mt_flag_small);
}
