// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2006-2011.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/types.h"
#include "tralics/util.h"
#include <array>
#include <cctype>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <sys/types.h>
#include <utility>
#include <vector>

#if defined(_WIN32)
#else
#include <termios.h>
#endif

namespace {
    const unsigned              buf_size = 2048;
    const unsigned              in_size  = 4096;
    bool                        term_set = false;
    std::array<size_t, in_size> word_beg{};
    std::array<size_t, in_size> word_end{};
    std::string                 cur_prompt{"> "};

    // Conditional code for windows

#if defined(_WIN32)
    // no termio
    void          RESET_TERMIO() {}
    void          SET_TERMIO() {}
    void          INIT_TERMIO() {}
    unsigned char check_for_control(unsigned char c) { return c; }
    bool          is_interrupt(char cc) { return cc == 3 || cc == 'y' || cc == 'Y'; }
#else
    struct termios orig_termio, rl_termio;
    void           RESET_TERMIO() { tcsetattr(0, TCSANOW, &orig_termio); }
    void           SET_TERMIO() { tcsetattr(0, TCSANOW, &rl_termio); }

    auto check_for_control(char c) -> char {
        if (c == orig_termio.c_cc[VERASE])
            c = 8; //^H
        else if (c == orig_termio.c_cc[VEOF])
            c = 4; // ^D
        else if (c == orig_termio.c_cc[VKILL])
            c = 11; // ^K
        else if (c == orig_termio.c_cc[VWERASE])
            c = 23; //^W
        else if (c == orig_termio.c_cc[VSUSP])
            c = 26; // ^Z
        else if (c == orig_termio.c_cc[VINTR])
            c = 3; // ^C
        else if (c == orig_termio.c_cc[VQUIT])
            c = 28;
        return c;
    }

    auto is_interrupt(char cc) -> bool {
        return cc == orig_termio.c_cc[VINTR] || cc == 3 || cc == orig_termio.c_cc[VQUIT] || cc == 'y' || cc == 'Y';
    }
    void INIT_TERMIO() {
        tcgetattr(0, &orig_termio);
        rl_termio = orig_termio;
        rl_termio.c_iflag &= ~to_unsigned((BRKINT | PARMRK | INPCK | IXOFF | IMAXBEL));
        rl_termio.c_iflag |= (IGNBRK | IGNPAR);
        rl_termio.c_lflag &= ~unsigned(ICANON | ECHO | ECHOE | ECHOK | ECHONL | NOFLSH);
        rl_termio.c_lflag |= (ISIG);
        rl_termio.c_cc[VMIN]   = 1;
        rl_termio.c_cc[VTIME]  = 0;
        rl_termio.c_cc[VSUSP]  = 0;
        rl_termio.c_cc[VINTR]  = 0;
        rl_termio.c_cc[VQUIT]  = 0;
        rl_termio.c_cc[VSTOP]  = 0;
        rl_termio.c_cc[VSTART] = 0;
#if defined(__ppc__)
        rl_termio.c_cc[VDSUSP] = 0; // needed on the mac
#endif
    }
#endif
} // namespace

void readline_newprompt(std::string s) { cur_prompt = std::move(s); }

namespace readline_ns {
    void make_edit();
    void set_termio();
    void reset_termio();
    void my_exit();
    void tybeep();
    auto skip_over_letters(String buf, size_t j) -> size_t;
    void tycleol();
    void right_fill();
    void shift_string(char *S, size_t len, size_t source, size_t target);
    void rl_delete0(char *s, size_t pos, size_t size, size_t w);
    auto find_word_beg(char *buf, size_t size) -> size_t;
} // namespace readline_ns

using namespace readline_ns;

class Slined { // \todo this should vanish at some point in refactoring
    std::unique_ptr<char[]>    m_inbuf_storage;
    char *                     m_inbuf{nullptr};
    std::array<char, buf_size> m_buffer{};
    std::string                m_killbuf;
    size_t                     m_inpos{0};
    size_t                     m_inmax{0};
    std::string                m_prompt{"> "};
    std::vector<std::string>   m_history;
    size_t                     m_hpos{0};
    size_t                     m_history_size;
    size_t                     m_mark{0};
    std::string                m_search;
    size_t                     m_left{0};
    size_t                     m_right{0};
    size_t                     m_pos{0};
    size_t                     m_max{0};
    size_t                     m_size{0};
    bool                       m_hack{false};
    bool                       cur_line_modified{false};
    bool                       done{false};
    std::array<char, buf_size> g_buffer{};

public:
    Slined(size_t sz, String P) {
        std::fill(m_buffer.begin(), m_buffer.end(), ' ');
        if (sz != 0) {
            m_inbuf_storage = std::make_unique<char[]>(sz);
            m_inbuf         = m_inbuf_storage.get();
        }
        if (P != nullptr) {
            m_history.emplace_back(P);
            m_history_size = 1; // first line is a comment
        }
    }

    Slined(const Slined &) = delete;
    ~Slined() = default;
    auto operator=(const Slined &) -> Slined & = delete;

    auto newpos(size_t x, size_t n) -> long;
    auto copystring(String string, size_t s, size_t inpos, bool sw) -> unsigned;
    void redisplay();
    void redisplay0();
    void redisplay1(size_t x);
    void redisplay_eol();
    void finish_redisplay(size_t pleft, size_t pright);
    void I_move(size_t pos, size_t x);
    void insert_in_image(size_t n, char c1, char c2);
    void delete_in_image(size_t w);
    void insert(size_t n, char c);
    void insert1(size_t n, char c);
    void rl_move(size_t y, size_t x);
    void rl_delete(bool sw);
    void toggle_char();
    void delete_string(int sw, size_t deb, size_t fn);
    void fast_ins(size_t n, String s, size_t l);
    void insert_substring(size_t n, String s, size_t l);
    void Hshow();
    auto Hfind(size_t n) -> bool;
    void Hfirst();
    void Hlast();
    void Hprevious(size_t n);
    void Hnext(size_t n);
    void search_string(size_t n);
    void forword(bool sw, size_t n);
    void backword(bool sw, size_t n);
    void replace_string();
    void store_line();
    void maybe_store_line();
    void do_esc_command(size_t n);
    void do_esc_command(size_t n, char c);
    void do_n_command(size_t n, char c);
    void do_command(unsigned n, int c);
    void initialise(char *buf, const std::string &prompt, size_t size);
    void run();
};

namespace {
    std::optional<Slined> the_editor, the_editor_c;
} // namespace

// Constructs two copies, one for showing the history.
// This function is called only once.
void readline_ns::make_edit() {
    the_editor_c.emplace(in_size, nullptr);
    the_editor.emplace(0, "%% start of history (say \\stop to exit)");
}

// Copy len characters from source to target. Strings may overlap.
void readline_ns::shift_string(char *S, size_t len, size_t source, size_t target) {
    if (len == 1)
        S[target] = S[source];
    else if (len == 2) {
        char x        = S[source + 1];
        S[target]     = S[source];
        S[target + 1] = x;
    } else if (target <= source) {
        for (size_t i = 0; i < len; i++, S++) S[target] = S[source];
    } else {
        S += len - 1;
        for (size_t i = 0; i < len; i++, S--) S[target] = S[source];
    }
}

// This assumes that characters with code >=128 are printable.
inline auto not_printable(char c) -> bool { return c == 127 || c < 32; }

// Returns the cursor position y of position x. Returns -1-y if the cursor
// is invisible. It is assumed that position n corresponds to m_pos.
auto Slined::newpos(size_t x, size_t n) -> long {
    size_t i = 0;
    size_t y = 0;
    if (x >= n) { // We can hack a bit.
        i = n;
        y = m_pos;
    }
    while (i < x) {
        if (not_printable(m_inbuf[i]))
            y += 2;
        else
            y++;
        i++;
    }
    if (y >= m_left && y < m_right) return to_signed(y);
    return -to_signed(1 + y);
}

// Beeps
void readline_ns::tybeep() { std::cerr.put(7); }

// Copy a string of size s into m_buffer or g_buffer, depending on sw.
// Control-A is copied as a two character sequence ^A. Returns
// the number of chars put in the buffer. If character at position
// inpos is copied, its location will be in m_pos.

auto Slined::copystring(String string, size_t s, size_t inpos, bool sw) -> unsigned {
    unsigned j   = 0;
    char *   buf = sw ? m_buffer.data() : g_buffer.data();
    for (size_t i = 0; i < s; i++) {
        if (i == inpos) m_pos = j;
        auto cn = string[i];
        if (not_printable(cn)) {
            buf[j] = '^';
            j++;
            cn = cn | 64;
        }
        buf[j] = cn;
        j++;
    }
    return j;
}

// Full redisplay.
void Slined::redisplay() {
    auto j = copystring(m_inbuf, m_inmax, m_inpos, true);
    if (m_inpos >= m_inmax) m_pos = j; // set cursor to EOL
    m_max = j;
    std::cerr << "\r" << m_prompt;
    tycleol();
    redisplay0();
}

// Redisplay after prompt.
void Slined::redisplay0() {
    size_t left{}, right{}; // position of left and right chars
    auto   s = m_size;
    auto   p = m_pos, m = m_max;
    if (m <= s) { // we have a short line, ok
        left  = 0;
        right = m;
    } else if (p < s - 2) { // cursor visible if we show first char
        left  = 0;
        right = s - 2;
    } else if (m - p < s - 2) { // cursor visible if we show last char
        right = m;
        left  = m - (s - 2);
    } else { // put the cursor in the middle
        right = p + (s - 4) / 2;
        left  = right - (s - 4);
    }
    m_left = left;
    if (left != 0) std::cerr << "> "; // show that there are unseen characters there
    finish_redisplay(left, right);
}

// clear to end of line.
void readline_ns::tycleol() {
    std::cerr.put(27);
    std::cerr.put('[');
    std::cerr.put('K');
}

// redisplay, set current pos to x.
void Slined::redisplay1(size_t x) {
    I_move(m_pos, m_left);
    m_pos = x;
    if (m_left != 0) {
        std::cerr.put(8); // backspace twice, because of the marker
        std::cerr.put(8);
    }
    tycleol();
    redisplay0();
}

// redisplay what's at right of cursor.
void Slined::redisplay_eol() {
    tycleol();
    auto f = m_left + m_size;
    if (m_left != 0) f -= 2;
    if (f >= m_max)
        f = m_max;
    else
        f -= 2;
    finish_redisplay(m_pos, f);
}

// Send out right marker plus two backspaces.
void readline_ns::right_fill() {
    std::cerr.put(' ');
    std::cerr.put('<');
    std::cerr.put(8);
    std::cerr.put(8);
}

// move the cursor to the right end, then back to target.
void Slined::finish_redisplay(size_t left, size_t right) {
    m_right = right;
    I_move(left, right);
    if (m_max != right) right_fill();
    I_move(right, m_pos);
}

// insert char c1 (or c1+c2) n times at current position
void Slined::insert_in_image(size_t n, char c1, char c2) {
    auto m = m_max, p = m_pos;
    auto size = m - p; // number of chars after cur pos
    auto k    = n;
    if (c1 != 0) k += n; // k is the number of chars to insert
    char *buf = m_buffer.data();
    buf       = buf + p;
    if (size != 0) shift_string(buf, size, 0, k); // make room
    for (unsigned i = 0; i < n; i++) {            // copy
        if (c1 != 0) *(buf++) = c1;
        *(buf++) = c2;
    }
    m += k;
    m_max = m; // new size
    p += k;
    m_pos   = p; // new position, cursor is after inserted chars
    auto d  = m_right + k;
    m_right = d; // new position of last char
    auto s  = m_size;
    if (m_left != 0) s -= 2;
    if (m != d) s -= 2; // s is number of free positions on line
    if (d - m_left > s) // cursor invisible, full redisplay needed
        redisplay();
    else {
        for (unsigned i = 0; i < n; i++) {
            if (c1 != 0) std::cerr.put(c1);
            std::cerr.put(c2);
        }
        if (p == m) {
        }                // we are at the right end
        else if (p == d) // we are at the boundary
            right_fill();
        else
            redisplay_eol(); // insert, and redisplay after position
    }
}

// delete a substring of size w
// in the string, at position pos, size is the string length
void readline_ns::rl_delete0(char *s, size_t pos, size_t size, size_t w) {
    auto t = size - pos - w;
    if (t > 0) shift_string(s, t, w + pos, pos);
}

// delete and redisplay
void Slined::delete_in_image(size_t w) {
    auto p = m_pos;
    rl_delete0(m_buffer.data(), p, m_max, w);
    m_right -= w;
    m_max -= w;
    if (p == m_right) { // deleted all chars after cursor
        tycleol();
        if (m_right != m_max) right_fill();
    } else
        redisplay_eol(); // non trivial case
}

// Insert a character n times
void Slined::insert(size_t n, char c) {
    if (n + m_inmax >= buf_size - 1) // overfull
        tybeep();
    else
        insert1(n, c);
}

// The real function that inserts a character c n times
void Slined::insert1(size_t n, char c) {
    auto  pos    = m_inpos;
    auto  size   = m_inmax;
    auto  aux    = size - pos;
    char *buffer = m_inbuf;
    buffer       = buffer + pos;
    if (aux != 0) shift_string(buffer, size, 0, n);
    for (unsigned i = 0; i < n; i++) buffer[i] = c; // insert
    size    = size + n;
    pos     = pos + n;
    m_inpos = pos;
    m_inmax = size;
    char c1 = 0, c2 = c;
    if (not_printable(c)) {
        c1 = '^';
        c2 |= 64;
    }
    insert_in_image(n, c1, c2);
    cur_line_modified = true;
}

// delete a character. Redisplay only if sw true.
void Slined::rl_delete(bool sw) {
    auto cn = m_inbuf[m_inpos];
    rl_delete0(m_inbuf, m_inpos, m_inmax, 1);
    m_inmax--;
    cur_line_modified = true;
    if (sw) delete_in_image(not_printable(cn) ? 2 : 1);
}

// Move the cursor from Y to X
void Slined::rl_move(size_t y, size_t x) {
    if (x == y) return;
    m_inpos = x;
    if (x == 0) {          // move to left
        if (m_left == 0) { // first char is visible
            I_move(m_pos, 0);
            m_pos = 0;
        } else
            redisplay1(0);      // else general case
    } else if (x == m_inmax) {  // move to last char
        if (m_right == m_max) { // last char is visible
            I_move(m_pos, m_right);
            m_pos = m_right;
        } else
            redisplay1(m_max); // else general case
    } else {
        auto yy = newpos(x, y);
        if (yy >= 0) { // target is visible
            I_move(m_pos, to_unsigned(yy));
            m_pos = to_unsigned(yy);
        } else
            redisplay1(to_unsigned(-1 - yy)); // target not visible
    }
}

// Move from a visible point to another visible point
void Slined::I_move(size_t pos, size_t x) {
    if (x == pos) return;
    if (x > pos)
        while (x > pos) {
            std::cerr.put(m_buffer[pos]); // advance -> output current char
            pos++;
        }
    else
        while (x < pos) {
            pos--;
            std::cerr.put(8); // otherwise, emit some backspaces
        }
}

// Toggle two chars: the character on the left and the character under
// the cursor and advance by one char.
void Slined::toggle_char() {
    auto max = m_inmax;
    auto pos = m_inpos;
    if (max < 2 || pos < 1) return;
    if (pos == max) { // go back by 1 character
        rl_move(pos, pos - 1);
        pos--;
    }
    auto c1 = m_inbuf[pos - 1], c2 = m_inbuf[pos];
    if (c1 == c2) // we can  hack
        rl_move(pos, pos + 1);
    else {
        rl_delete(true); // must delete 2 chars and insert 2 chars
        rl_move(pos, pos - 1);
        rl_delete(true);
        insert(1, c2);
        insert(1, c1);
    }
}

// this deletes a string between deb and fn.
void Slined::delete_string(int sw, size_t deb, size_t fn) {
    size_t debut{}, fin{};
    if (deb < fn) {
        debut = deb;
        fin   = fn;
    } else {
        debut = fn;
        fin   = deb;
    }
    if (fin >= m_inmax) fin = m_inmax - 1;

    auto taille = fin - debut + 1; // Size of what must be killed.
    m_killbuf   = std::string(m_inbuf + debut, taille);
    if (sw != 0) {
        rl_move(m_inpos, debut);
        for (size_t i = 0; i < taille; i++)
            if (m_inpos < m_inmax) rl_delete(false);
        redisplay();
    }
}

// inserts  a  string n times
void Slined::insert_substring(size_t n, String s, size_t l) {
    if (m_inmax + n * l >= buf_size - 1)
        tybeep();
    else if (n * l > 10)
        fast_ins(n, s, l);
    else {
        for (size_t j = 0; j < n; j++)
            for (size_t i = 0; i < l; i++) insert(1, s[i]);
    }
}

// insert in the buffer then redisplay all
void Slined::fast_ins(size_t n, String s, size_t l) {
    auto  pos    = m_inpos;
    auto  size   = m_inmax;
    char *buffer = m_inbuf;
    auto  aux    = size - pos;
    if (aux != 0) shift_string(buffer, aux, pos, pos + n * l); // make room
    for (size_t i = 0; i < n; i++) {
        strncpy(buffer + pos, s, l);
        pos += l;
        size += l;
    }
    m_inpos = pos;
    m_inmax = size;
    auto j  = copystring(s, l, size_t(-1), false);
    pos     = m_pos;
    buffer  = m_buffer.data();
    size    = m_max;
    aux     = size - pos;
    if (aux != 0) shift_string(buffer, aux, pos, pos + j * n); // make room here also
    for (size_t i = 0; i < n; i++) {
        strncpy(buffer + pos, g_buffer.data(), j);
        pos += j;
        size += j;
    }
    m_max = size;
    redisplay_eol();
    rl_move(m_pos, pos); // put cursor where it should be
}

// show history
void Slined::Hshow() {
    unsigned            n      = 0;
    auto                sz     = m_history.size();
    Slined &            new_ed = *the_editor_c;
    std::array<char, 4> p{};
    p[2]          = ' ';
    p[3]          = 0;
    new_ed.m_size = m_size + m_prompt.size() - 3;
    for (size_t i = 0; i < sz; i++) {
        n++;
        if (n >= 100) n = n - 100;
        if (n < 10)
            p[0] = ' ';
        else
            p[0] = '0' + static_cast<char>(n / 10);
        p[1]            = char('0' + to_signed(n % 10));
        new_ed.m_prompt = p.data();
        new_ed.m_inpos  = 0;
        String S        = m_history[i].c_str();
        auto   k        = strlen(S);
        if (k > buf_size - 2) k = buf_size - 2;
        new_ed.m_inmax = k;
        strncpy(new_ed.m_inbuf, S, k);
        new_ed.m_inbuf[k] = 0;
        new_ed.redisplay();
        std::cerr.put('\n');
    }
}

// search a string, going back n steps.
void Slined::search_string(size_t n) {
    if (m_search.empty()) { // searches current line in history
        m_inbuf[m_inmax] = 0;
        m_search         = m_inbuf;
    }
    if (!Hfind(n)) return;
    replace_string();
}

// Find a string in the history
auto Slined::Hfind(size_t n) -> bool {
    bool seen_once = false; // if seen at least once, do not kill
    auto pos       = m_hpos;
    while (pos > 0) {
        pos--;
        if (m_history[pos].find(m_search) != std::string::npos) {
            seen_once = true;
            n--;
            if (n == 0) {
                m_hpos = pos;
                return true;
            }
        }
    }
    tybeep();
    if (!seen_once) m_search = "";
    return false;
}

// set position to first line.
void Slined::Hfirst() {
    if (m_history_size == 1)
        m_hpos = 0; // first is a dummy, skip it.
    else
        m_hpos = 1;
}

// set position to last line
void Slined::Hlast() { m_hpos = m_history_size - 1; }

// go back n lines
void Slined::Hprevious(size_t n) {
    if (m_hpos == m_history_size - 1 && !m_hack) {
        n--;
        m_hack = true;
    }
    auto k = m_hpos > n ? m_hpos - n : 0;
    m_hpos = k;
}

// advance by n lines
void Slined::Hnext(size_t n) {
    auto k = m_history_size - 1;
    n += m_hpos;
    if (n > k) n = k;
    m_hpos = n;
}

auto readline_ns::skip_over_letters(String buf, size_t j) -> size_t {
    for (;;) {
        if (std::isalpha(buf[j]) == 0) return j;
        j++;
    }
}

// Find word boundaries
auto readline_ns::find_word_beg(char *buf, size_t size) -> size_t {
    size_t i = 0;
    size_t j = 0;
    for (;;) {
        buf[size] = '*';
        while (buf[j] == ' ') j++;
        if (j >= size) return i;
        word_beg[i] = j;
        buf[size]   = ' ';
        char c      = buf[j];
        j++;
        if (c == '\\') {
            c = buf[j];
            j++;
            if (std::isalpha(c) != 0) j = skip_over_letters(buf, j);
        } else if (std::isalpha(c) != 0)
            j = skip_over_letters(buf, j);
        word_end[i] = j;
        i++;
    }
}

// advance by n words. If sw false deletes the string
void Slined::forword(bool sw, size_t n) {
    auto   i = find_word_beg(m_inbuf, m_inmax);
    auto   y = m_inpos;
    size_t k = 0;
    while (k < i && y >= word_end[k]) k++; // k = index of cur word
    if (k >= i)
        y = m_inmax;
    else
        y = word_beg[k]; // y is here boundary
    k = k + n - 1;
    if (k >= i)
        k = m_inmax;
    else
        k = word_end[k]; // k is there boundary
    if (sw)
        rl_move(m_inpos, k);
    else if (k > y) // do nothing if k=0
        delete_string(1, y, k - 1);
}

// go backwards by n words. If sw false, delete the string.
// same code, with begin and end exchanged.
void Slined::backword(bool sw, size_t n) {
    auto i = find_word_beg(m_inbuf, m_inmax);
    auto y = m_inpos;
    auto k = i - 1;
    while (k != size_t(-1) && y <= word_beg[k]) k--;
    if (k == size_t(-1))
        y = 0;
    else
        y = word_end[k];
    if (k < n - 1)
        k = 0;
    else
        k = word_beg[k - n + 1];
    if (sw)
        rl_move(m_inpos, k);
    else if (k < y)
        delete_string(1, k, y - 1);
}

// Make s be the current string.
void Slined::replace_string() {
    maybe_store_line();
    cur_line_modified = false;
    std::string s     = m_hpos < m_history_size ? m_history[m_hpos] : "";
    auto        l     = s.size();
    strncpy(m_inbuf, s.c_str(), l);
    m_inmax = l;
    m_inpos = l;
    redisplay();
}

// If the line is not empty, it is stored in the history.
void Slined::store_line() {
    if (m_inmax > 0) {
        m_history.emplace_back(m_inbuf, m_inmax);
        m_history_size++;
    }
}

void Slined::maybe_store_line() {
    if (!cur_line_modified) return;
    store_line();
}

void Slined::do_esc_command(size_t n) {
    char c{};
    std::cin.get(c);
    if ('1' <= c && c <= '9') { // non zero digit; ignore n
        char cc{};
        std::cin.get(cc);
        do_command(to_unsigned(c - '0'), cc);
        return;
    }
    if ('A' <= c && c <= 'Z') // ignore case
        c = static_cast<char>(c + ('a' - 'A'));
    do_esc_command(n, c);
}

// Action associated to escape something
void Slined::do_esc_command(size_t n, char c) {
    switch (c) {
    case '0': // <esc>022 = repeat 22 times
    {
        unsigned count = 0;
        for (;;) {
            std::cin.get(c);
            if ('0' <= c && c <= '9')
                count = count * 10 + to_unsigned(c - '0');
            else
                break;
        }
        count = count % 1000; // <ESC>45679 = repeat 679
        do_command(count, c);
        return;
    }
    case 'b': backword(true, n); return;
    case 'f': forword(true, n); return;
    case 8:   // backspace
    case 127: // delete
        backword(false, n);
        return;
    case 'h':
        Hshow();
        redisplay();
        return;
    case 'd':
    case 4: // control-D
        forword(false, n);
        return;
    case 7: // control G
        tybeep();
        return;
    case '<':
        Hfirst();
        replace_string();
        return;
    case '>':
        Hlast();
        replace_string();
        return;
    case 27: search_string(n); return;
    case 'o': do_n_command(1, 15); return;
    case 's':
        m_search = "";
        search_string(n);
        return;
    case 'w': delete_string(0, m_mark, m_inpos); return;
    case '@': m_mark = m_inpos; return;
    case '?':
        std::cerr << "key bindings are more or less the same as in Emacs\n";
        std::cerr << "^R searches a string, <ESH>h shows history\n";
        std::cerr << "<ESC>n CMD : repeat the command CMD n times\n";
        std::cerr << "<ESC>0n CMD : repeat the command n times (case n>10)\n";
        redisplay();
        return;
    case 28: // escape control backslash
        std::cerr << "\nQUIT\n";
        my_exit();
        return;
    case '[': // ESC[A is up-arrow
    {
        char cc{};
        std::cin.get(cc);
        switch (cc) {
        case 'A': do_n_command(n, 16); return;
        case 'B': do_n_command(n, 14); return;
        case 'C': do_n_command(n, 6); return;
        case 'D': do_n_command(n, 2); return;
        case 'F': do_n_command(n, '>'); return;
        case 'H': do_n_command(n, '<'); return;
        default: tybeep();
        }
        return;
    }
    default: tybeep();
    }
}

void Slined::do_n_command(size_t n, char c) {
    auto   pos = m_inpos;
    auto   max = m_inmax;
    size_t aux = 0;
    c          = check_for_control(c);
    switch (c) {
    case 1: // ^A
        rl_move(pos, 0);
        return;
    case 2: // ^B
        aux = pos > n ? pos - n : 0;
        rl_move(pos, aux);
        return;
    case 5: // ^E
        rl_move(pos, max);
        return;
    case 6: // ^F
        aux = pos + n;
        if (aux > max) aux = max;
        rl_move(pos, aux);
        return;
    case 10:
    case 13:
        store_line();
        Hlast();
        done = true;
        return;
    case 15:
        store_line();
        Hnext(1);
        done = true;
        return;
    case 127:
    case 8:
        if (n > pos) n = pos;
        for (unsigned i = 0; i < n; i++) {
            rl_move(pos, pos - 1);
            pos--;
            rl_delete(true);
        }
        return;
    case 4:
        for (unsigned i = 0; i < n; i++) {
            if (pos < m_inmax) rl_delete(true);
        }
        return;
    case 11: delete_string(1, pos, max); return;
    case 21:
    case 24: delete_string(1, 0, pos - 1); return;
    case 25:
        if (!m_killbuf.empty())
            insert_substring(n, m_killbuf.c_str(), m_killbuf.size());
        else
            tybeep();
        return;
    case 16:
        Hprevious(n);
        replace_string();
        return;
    case 14:
        Hnext(n);
        replace_string();
        return;
    case 18: search_string(n); return;
    case 20: toggle_char(); return;
    case 12: redisplay(); return;
    case 17: {
        char cc{};
        std::cin.get(cc);
        insert(n, cc);
        return;
    }
    case 27: do_esc_command(n); return;
    case 0: m_mark = pos; return;
    case 23: delete_string(1, m_mark, pos); return;
    case 26:
        reset_termio();
        // kill(0, SIGTSTP);
        set_termio();
        redisplay();
        return;
    case 28:
        std::cerr << "\nQUIT\n";
        my_exit();
        return;
    case 3: {
        std::cerr << "\nreally quit ?\n";
        char cc{};
        std::cin.get(cc);
        if (is_interrupt(cc)) {
            reset_termio();
            exit(1);
        }
        redisplay();
    }
        return;
    default: insert(n, c);
    }
}

void Slined::do_command(unsigned n, int c) {
    if (c == EOF) {
        std::cerr << "EOF on tty\n";
        my_exit();
    }
    if (c >= 256 || c <= -256) {
        std::cerr << "???" << c << "???\n";
        return;
    }
    do_n_command(n, static_cast<char>(c));
}

void Slined::initialise(char *buffer, const std::string &prompt, size_t size) {
    m_inbuf_storage.reset();
    m_inbuf  = buffer;
    m_prompt = prompt;
    m_size   = size - prompt.size();
    if (m_hpos < m_history_size - 1)
        replace_string();
    else {
        m_inmax = 0;
        m_inpos = 0;
        redisplay();
    }
    m_hack = false;
}

void Slined::run() {
    cur_line_modified = false;
    done              = false;
    char c{};
    for (;;) {
        std::cin.get(c);
        do_command(1, c);
        if (done) break;
    }
    std::cerr.put('\n');
    m_inbuf[m_inmax] = 0;
}

void readline(char *buffer, size_t screen_size) {
    if (!the_editor) readline_ns::make_edit();
    the_editor->initialise(buffer, cur_prompt, screen_size);
    set_termio();
    the_editor->run();
    reset_termio();
}

void readline_ns::set_termio() {
    static bool term_copied = false;
    if (!term_copied) {
        INIT_TERMIO();
        term_copied = true;
    }
    if (!term_set) {
        SET_TERMIO();
        term_set = true;
    }
}

void readline_ns::reset_termio() {
    if (term_set) {
        RESET_TERMIO();
        term_set = false;
    }
}

// reset and quit (in case of error)
void readline_ns::my_exit() {
    reset_termio();
    exit(1);
}
