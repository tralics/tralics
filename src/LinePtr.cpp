#include "tralics/LinePtr.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"
#include "txinline.h"

namespace {
    Buffer local_buf;
} // namespace

void LinePtr::change_encoding(long wc) {
    if (wc >= 0 && wc < to_signed(max_encoding)) {
        encoding = to_unsigned(wc);
        Logger::finish_seq();
        spdlog::trace("++ Input encoding changed to {} for {}", wc, file_name);
    }
}

void LinePtr::set_interactive() {
    interactive = true;
    file_name   = "tty";
    encoding    = the_main->input_encoding;
}

// interface with the line editor.
auto LinePtr::read_from_tty(Buffer &B) -> int {
    static bool                   prev_line = false; // was previous line non-blank ?
    static std::array<char, 4096> m_ligne;
    readline(m_ligne.data(), 78);
    if (std::string(m_ligne.data()) == "\\stop") return -1;
    cur_line++;
    B.reset();
    B << m_ligne.data() << "\n";
    if (B.size() == 1) {
        if (!prev_line) std::cout << "Say \\stop when finished, <ESC>-? for help.\n";
        prev_line = false;
    } else
        prev_line = true;
    if (B[0] == '%') { // debug
        int k = io_ns::find_encoding(B.to_string());
        if (k >= 0) encoding = to_unsigned(k);
    }
    return cur_line;
}

// inserts a copy of aux
void LinePtr::insert(const LinePtr &aux) {
    encoding = 0;
    for (auto L : aux) {
        L.converted = false;
        push_back(L);
    }
}

// If a line ends with \, we take the next line, and append it to this one
void LinePtr::normalise_final_cr() {
    line_iterator prev{end()};
    for (auto C = begin(); C != end(); ++C) {
        std::string &s       = *C;
        auto         n       = s.size();
        bool         special = s.ends_with("\\\n");
        std::string  normal  = s;
        if (special) normal = std::string(s, 0, n - 2);
        if (prev != end()) {
            prev->append(normal);
            s = "\n"; // \todo could this create an empty line and cause a `\par`? should drop?
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

void LinePtr::reset(std::string x) {
    cur_line    = 0;
    encoding    = 0;
    interactive = false;
    clear();
    file_name = std::move(x);
}

// Insert a line at the end of the file, incrementing the line no
void LinePtr::insert(const std::string &c, bool cv) { emplace_back(++cur_line, c, cv); }

// Insert a line at the end of the file, incrementing the line no
// We assume that the const char* is ascii 7 bits
void LinePtr::insert(String c) { emplace_back(++cur_line, c, true); }

// Like insert, but we do not insert an empty line after an empty line.
// Used by the raweb preprocessor, hence already converted
void LinePtr::insert_spec(int n, std::string c) {
    if (!empty() && back()[0] == '\n' && c[0] == '\n') return;
    emplace_back(n, c, true);
}

// Inserts the buffer, with a newline at the end.
void LinePtr::add(int n, Buffer &b, bool cv) { emplace_back(n, b.to_string() + "\n", cv); }

// insert a file at the start
void LinePtr::splice_first(LinePtr &X) { splice(begin(), X); }

// insert at the end
void LinePtr::splice_end(LinePtr &X) { splice(end(), X); }

// Copy X here,
void LinePtr::clear_and_copy(LinePtr &X) {
    clear();
    splice(begin(), X);
    encoding = X.encoding;
    set_file_name(X.file_name);
}

auto LinePtr::dump_name() const -> std::string { return file_name.empty() ? "virtual file" : "file " + file_name; }

// Whenever a TeX file is opened for reading, we print this in the log
void LinePtr::after_open() {
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
void LinePtr::before_close(bool sigforce) {
    Logger::finish_seq();
    the_log << "++ End of " << dump_name();
    if (sigforce && !empty()) the_log << " (forced by \\endinput)";
    the_log << "\n";
}

// Puts in b the next line of input.
// return -1 at EOF, the line number otherwise.
auto LinePtr::get_next(Buffer &b) -> int {
    int  n         = -1;
    bool converted = false;
    if (interactive) {
        n = read_from_tty(b);
        if (n == -1) interactive = false;
    } else {
        if (empty()) return -1;
        n = front().to_buffer(b, converted);
        pop_front();
    }
    if (!converted) {
        the_converter.cur_file_name = file_name;
        b.convert_line(n, encoding);
    }
    return n;
}

auto LinePtr::get_next_cv(Buffer &b, int w) -> int {
    if (empty()) return -1;
    bool converted = false; // \todo unused
    int  n         = front().to_buffer(b, converted);
    pop_front();
    if (w != 0) {
        the_converter.cur_file_name = file_name;
        b.convert_line(n, to_unsigned(w));
    }
    return n;
}

// same as get_next, without conversion
auto LinePtr::get_next_raw(Buffer &b) -> int {
    if (empty()) return -1;
    bool unused = false;
    int  n      = front().to_buffer(b, unused);
    pop_front();
    return n;
}

// Puts the line in the string, instead of the buffer.
auto LinePtr::get_next(std::string &b, bool &cv) -> int {
    if (empty()) return -1;
    int n = front().to_string(b, cv);
    pop_front();
    return n;
}

/// This finds a line with documentclass in it
// uses B and the buffer.
auto LinePtr::find_documentclass(Buffer &B) -> std::string {
    the_main->doc_class_pos = end();
    for (auto C = begin(); C != end(); ++C) {
        B.reset();
        B.push_back(*C);
        if (B.find_documentclass(buf)) {
            the_main->doc_class_pos = C;
            return buf.to_string();
        }
    }
    return "";
}

// This inserts B into *this, before C
// If C is the end pointer, B is inserted at the start.
// The idea is to insert text from the config file to the main file
// It is assumed that the inserted line is already converted.
void LinePtr::add_buffer(Buffer &B, line_iterator C) {
    if (C == end())
        push_front(Clines(1, B.to_string(), true));
    else
        std::list<Clines>::insert(C, Clines(1, B.to_string(), true)); // \todo ew
}

// This finds a line with documentclass in it
// uses B and the buffer.
auto LinePtr::find_configuration(Buffer &B) -> std::string {
    int N = 0;
    for (auto & C : *this) {
        B.reset();
        B.push_back(C);
        if (B.find_configuration(buf)) return buf.to_string();
        if (++N > 100) break;
    }
    return "";
}
// This finds a line with document type in it
// uses B and the buffer.
void LinePtr::find_doctype(Buffer &B, std::string &res) {
    if (!res.empty()) return; // use command line option if given
    int N = 0;
    for (auto & C : *this) {
        B.reset();
        B.push_back(C);
        auto k = B.find_doctype();
        if (k != 0) {
            res = B.to_string(k);
            return;
        }
        if (++N > 100) return;
    }
}

// Splits a string at \n, creates a list of lines with l as first
// line number.
// This is used by \scantokens and \reevaluate, assumes UTF8
void LinePtr::split_string(std::string x, int l) {
    Buffer &B = buf;
    LinePtr L;
    L.set_cur_line(l);
    B.reset();
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
            L.insert(B.to_string(), true);
            B.reset();
        }
        if (c == 0) break;
    }
    splice_first(L);
}

void LinePtr::print(std::ostream &outfile) {
    for (auto &C : *this) outfile << C;
}

// This find a toplevel attributes. Real job done by next function.
void LinePtr::find_top_atts(Buffer &B) {
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B << bf_reset << *C;
        B.find_top_atts();
    }
}

// A loop to find all types  and put them in res.
void LinePtr::find_all_types(std::vector<std::string> &res) {
    Buffer &B = local_buf;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        init_file_pos = C->number;
        B << bf_reset << *C;
        B.find_one_type(res);
    }
}

// This find a toplevel value.
auto LinePtr::find_top_val(String s, bool c) -> std::string {
    Buffer &B = local_buf;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B << bf_reset << *C;
        String res = B.see_config_kw(s, c);
        if (res != nullptr) return res;
    }
    return "";
}

// We remove everything that is not of type S.
void LinePtr::parse_and_extract_clean(String s) {
    LinePtr res;
    int     b    = 0;
    Buffer &B    = local_buf;
    bool    keep = true;
    bool    cv   = true;
    for (auto & C : *this) {
        B.reset();
        int n    = C.to_buffer(B, cv);
        int open = B.see_config_env();
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
        if (keep) res.emplace_back(n, B.to_string(), cv);
    }
    clear();
    splice_first(res);
}

// Returns all line in a begin/end block named s
auto LinePtr::parse_and_extract(String s) const -> LinePtr {
    LinePtr res;
    int     b    = 0;
    Buffer &B    = local_buf;
    bool    keep = false;
    bool    cv   = 0; // unused.
    for (auto C = begin(); C != end(); ++C) {
        B.reset();
        C->to_buffer(B, cv);
        auto W    = C;
        int  open = B.see_config_env();
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
        if (keep) res.push_back(*W);
    }
    return res;
}

// Execute all lines that are not in an block via see_main_a
void LinePtr::parse_conf_toplevel() const {
    int    b  = 0;
    bool   cv = 0; // unused. We assume that the line is always converted
    Buffer B;
    for (const auto & C : *this) {
        B.reset();
        init_file_pos = C.to_buffer(B, cv);
        b += B.see_config_env();
        if (b == 0) tpage_ns::see_main_a(B, ssa2, local_buf);
    }
}

// This skips over an environment.
auto LinePtr::skip_env(line_iterator_const C, Buffer &B) -> line_iterator_const {
    ++C;
    int b = B.see_config_env();
    if (b != 1) return C;
    auto E = end();
    while (C != E) {
        B << bf_reset << *C;
        ++C;
        b += B.see_config_env();
        if (b == 0) return C;
    }
    return C;
}

// Find all aliases in the config file.
auto LinePtr::find_aliases(const std::vector<std::string> &SL, std::string &res) -> bool {
    Buffer &B        = local_buf;
    bool    in_alias = false;
    for (auto C = cbegin(); C != cend(); C = skip_env(C, B)) {
        B << bf_reset << *C;
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
