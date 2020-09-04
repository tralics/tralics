#include "tralics/LineList.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txinline.h"
#include <ctre.hpp>

namespace {
    Buffer local_buf;

    void find_one_type(const std::string &s, std::vector<std::string> &S) { // \todo regexp?
        if (!s.starts_with("BeginType")) return;

        size_t b = 9;
        while (s[b] == ' ' || s[b] == '\t') b++;
        if (b == 9) return; // bad

        size_t a = b;
        while (is_letter(s[b])) b++;
        if (b == a) return; // bad

        auto str = s.substr(a, b - a);
        S.push_back(str);
        spdlog::trace("Defined type: {}", str);
    }
} // namespace

void LineList::change_encoding(long wc) {
    if (wc >= 0 && wc < to_signed(max_encoding)) {
        encoding = to_unsigned(wc);
        Logger::finish_seq();
        spdlog::trace("++ Input encoding changed to {} for {}", wc, file_name);
    }
}

void LineList::set_interactive() {
    interactive = true;
    file_name   = "tty";
    encoding    = the_main->input_encoding;
}

// interface with the line editor.
auto LineList::read_from_tty(Buffer &B) -> int {
    static bool                   prev_line = false; // was previous line non-blank ?
    static std::array<char, 4096> m_ligne;
    readline(m_ligne.data(), 78);
    if (std::string(m_ligne.data()) == "\\stop") return -1;
    cur_line++;
    B = m_ligne.data();
    B.append("\n");
    if (B.size() == 1) {
        if (!prev_line) std::cout << "Say \\stop when finished, <ESC>-? for help.\n";
        prev_line = false;
    } else
        prev_line = true;
    if (B[0] == '%') { // debug
        if (auto k = io_ns::find_encoding(B)) encoding = *k;
    }
    return cur_line;
}

// inserts a copy of aux
void LineList::insert(const LineList &aux) {
    encoding = 0;
    for (auto L : aux) {
        L.converted = false;
        push_back(L);
    }
}

// If a line ends with \, we take the next line, and append it to this one
void LineList::normalise_final_cr() {
    auto prev = end();
    for (auto C = begin(); C != end(); ++C) {
        std::string &s       = *C;
        auto         n       = s.size();
        bool         special = s.ends_with("\\\n");
        std::string  normal  = s;
        if (special) normal = std::string(s, 0, n - 2);
        if (prev != end()) {
            prev->append(normal);
            s = "\n";
        }
        if (special) {
            if (prev == end()) {
                prev                       = C;
                prev->std::string::operator=(normal);
            }
        } else
            prev = end();
    }
}

void LineList::reset(std::string x) {
    cur_line    = 0;
    encoding    = 0;
    interactive = false;
    clear();
    file_name = std::move(x);
}

// Insert a line at the end of the file, incrementing the line no
void LineList::insert(const std::string &c, bool cv) { emplace_back(++cur_line, c, cv); }

// Insert a line at the end of the file, incrementing the line no
// We assume that the const char* is ascii 7 bits
void LineList::insert(String c) { emplace_back(++cur_line, c, true); }

// Like insert, but we do not insert an empty line after an empty line.
// Used by the raweb preprocessor, hence already converted
void LineList::insert_spec(int n, std::string c) {
    if (!empty() && back()[0] == '\n' && c[0] == '\n') return;
    emplace_back(n, c, true);
}

// insert a file at the start
void LineList::splice_first(LineList &X) { splice(begin(), X); }

// Copy X here,
void LineList::clear_and_copy(LineList &X) {
    clear();
    splice(begin(), X);
    encoding  = X.encoding;
    file_name = X.file_name;
}

auto LineList::dump_name() const -> std::string { return file_name.empty() ? "virtual file" : "file " + file_name; }

// Whenever a TeX file is opened for reading, we print this in the log
void LineList::after_open() {
    Logger::finish_seq();
    the_log << "++ Opened " << dump_name();
    if (empty())
        the_log << "; it is empty\n";
    else {
        int n = front().number;
        int m = back().number;
        if (n == 1) {
            if (m == 1)
                the_log << "; it has 1 line\n";
            else
                the_log << "; it has " << m << " lines\n";
        } else
            the_log << "; line range is " << n << "-" << m << "\n";
    }
}

// Whenever a TeX file is closed, we call this. If sigforce is true
// we say if this was closed by a \endinput command.
void LineList::before_close(bool sigforce) {
    Logger::finish_seq();
    the_log << "++ End of " << dump_name();
    if (sigforce && !empty()) the_log << " (forced by \\endinput)";
    the_log << "\n";
}

// Puts in b the next line of input.
// return -1 at EOF, the line number otherwise.
auto LineList::get_next(Buffer &b) -> int {
    int  n         = -1;
    bool converted = false;
    if (interactive) {
        n = read_from_tty(b);
        if (n == -1) interactive = false;
    } else {
        if (empty()) return -1;
        b.append(front());
        n         = front().number;
        converted = front().converted;
        pop_front();
    }
    if (!converted) {
        cur_file_name = file_name;
        b.convert_line(n, encoding);
    }
    return n;
}

auto LineList::get_next_cv(Buffer &b, int w) -> int {
    if (empty()) return -1;
    b.append(front());
    auto n = front().number;
    pop_front();
    if (w != 0) {
        cur_file_name = file_name;
        b.convert_line(n, to_unsigned(w));
    }
    return n;
}

// same as get_next, without conversion
auto LineList::get_next_raw(Buffer &b) -> int {
    if (empty()) return -1;
    b.append(front());
    auto n = front().number;
    pop_front();
    return n;
}

// Puts the line in the string, instead of the buffer.
auto LineList::get_next(std::string &b, bool &cv) -> int {
    if (empty()) return -1;
    b      = front();
    cv     = front().converted;
    auto n = front().number;
    pop_front();
    return n;
}

/// This finds a line with documentclass in it
auto LineList::find_documentclass() -> std::string {
    static constexpr auto pattern = ctll::fixed_string{R"(.*\\documentclass.*\{([a-zA-Z0-9]+)\}.*)"};
    for (auto C = begin(); C != end(); ++C) {
        if (C->find("%%") != std::string::npos) continue;
        if (auto match = ctre::match<pattern>(*C)) {
            the_main->doc_class_pos = C;
            return match.get<1>().to_string();
        }
    }
    the_main->doc_class_pos = end();
    return "";
}

// This finds a line with documentclass in it
auto LineList::find_configuration() -> std::string {
    static constexpr auto pattern = ctll::fixed_string{"^%.*ralics configuration file[^']*'([^']+)'.*$"};
    int                   N       = 0;
    for (auto &C : *this) {
        if (++N > 100) break;
        if (auto m = ctre::match<pattern>(C)) return m.get<1>().to_string();
    }
    return "";
}

// This finds a line with document type in it
auto LineList::find_doctype() -> std::string {
    static constexpr auto pattern = ctll::fixed_string{"%.*[Tt]ralics DOCTYPE [ =]*([^ =].*)$"};
    int                   N       = 0;
    for (auto &C : *this) {
        if (++N > 100) break;
        if (auto m = ctre::match<pattern>(C)) return m.get<1>().to_string();
    }
    return "";
}

// Splits a string at \n, creates a list of lines with l as first
// line number.
// This is used by \scantokens and \reevaluate, assumes UTF8
void LineList::split_string(std::string x, int l) {
    Buffer   B;
    LineList L;
    L.cur_line = l;
    for (size_t i = 0;; ++i) {
        char c    = x[i];
        bool emit = false;
        if (c == '\n')
            emit = true;
        else if (c == 0) {
            if (!B.empty()) emit = true;
        } else
            B.push_back(c);
        if (emit) {
            B.push_back_newline();
            L.insert(B, true);
            B.clear();
        }
        if (c == 0) break;
    }
    splice_first(L);
}

void LineList::print(std::ostream &outfile) {
    for (auto &C : *this) outfile << C;
}

// This find a toplevel attributes. Real job done by next function.
void LineList::find_top_atts() {
    Buffer B;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B = *C;
        B.find_top_atts();
    }
}

// A loop to find all types  and put them in res.
void LineList::find_all_types(std::vector<std::string> &res) {
    Buffer &B = local_buf;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        init_file_pos = C->number;
        B             = *C;
        find_one_type(B, res);
    }
}

// This find a toplevel value.
auto LineList::find_top_val(String s, bool c) -> std::string {
    Buffer &B = local_buf;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B          = *C;
        String res = B.see_config_kw(s, c);
        if (res != nullptr) return res;
    }
    return "";
}

// We remove everything that is not of type S.
void LineList::parse_and_extract_clean(const std::string &s) {
    LineList res;
    int      b    = 0;
    Buffer & B    = local_buf;
    bool     keep = true;
    for (auto &C : *this) {
        B         = C;
        auto n    = C.number;
        auto cv   = C.converted;
        int  open = B.see_config_env();
        b += open;
        if (b < 0) {
            b = 0;
            continue;
        }                           // ignore bogus lines
        if (b == 0 && open == -1) { // cur env has closed
            keep = true;
            continue;
        }
        if (b == 1 && open == 1) { // something new started
            int v = B.is_begin_something(s);
            if (v == 1) {
                keep = false;
                continue;
            }
            if (v == 3) {
                keep = true;
                continue;
            }
        }
        if (keep) res.emplace_back(n, B, cv);
    }
    clear();
    splice_first(res);
}

// Returns all line in a begin/end block named s
auto LineList::parse_and_extract(String s) const -> LineList {
    LineList res;
    int      b    = 0;
    Buffer & B    = local_buf;
    bool     keep = false;
    for (const auto &C : *this) {
        B        = C;
        int open = B.see_config_env();
        b += open;
        if (open != 0) keep = false; // status changed
        if (b < 0) {
            b = 0;
            continue;
        }                          // ignore bogus lines
        if (b == 1 && open == 1) { // something new started
            if (B.is_begin_something(s) == 4) keep = true;
            continue;
        }
        if (keep) res.push_back(C);
    }
    return res;
}

// Execute all lines that are not in an block via see_main_a
void LineList::parse_conf_toplevel() const {
    int    b = 0;
    Buffer B;
    for (const auto &C : *this) {
        B             = C;
        init_file_pos = C.number;
        b += B.see_config_env();
        if (b == 0) tpage_ns::see_main_a(B, local_buf);
    }
}

// This skips over an environment.
auto LineList::skip_env(line_iterator_const C, Buffer &B) -> line_iterator_const {
    ++C;
    int b = B.see_config_env();
    if (b != 1) return C;
    auto E = end();
    while (C != E) {
        B = *C;
        ++C;
        b += B.see_config_env();
        if (b == 0) return C;
    }
    return C;
}

// Find all aliases in the config file.
auto LineList::find_aliases(const std::vector<std::string> &SL, std::string &res) -> bool {
    Buffer &B        = local_buf;
    bool    in_alias = false;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B = *C;
        if (in_alias) {
            if (B.find_alias(SL, res)) return true;
        }
        if (B.starts_with("End"))
            in_alias = false;
        else if (B.starts_with("BeginAlias")) {
            in_alias = true;
            ++C;
            continue;
        }
    }
    return false;
}
