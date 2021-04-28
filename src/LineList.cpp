#include "tralics/LineList.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <ctre.hpp>

namespace tpage_ns {
    auto scan_item(Buffer &in, Buffer &out, char del) -> bool;
} // namespace tpage_ns

namespace {
    Buffer local_buf;

    auto find_encoding(const std::string &cl) -> std::optional<size_t> {
        if (cl.find("-*-") != std::string::npos) {
            if (cl.find("coding: utf-8") != std::string::npos) return 0;
            if (cl.find("coding: utf8") != std::string::npos) return 0;
            if (cl.find("coding: latin1") != std::string::npos) return 1;
            if (cl.find("coding: iso-8859-1") != std::string::npos) return 1;
        }
        if (cl.find("iso-8859-1") != std::string::npos) return 1;
        if (cl.find("utf8-encoded") != std::string::npos) return 0;
        if (cl.find("%&TEX encoding = UTF-8") != std::string::npos) return 0;
        auto kk = cl.find("tralics-encoding:");
        if (kk == std::string::npos) return {};
        if (std::isdigit(cl[kk + 17]) == 0) return {};
        int k = cl[kk + 17] - '0';
        if (std::isdigit(cl[kk + 18]) != 0) { k = 10 * k + cl[kk + 18] - '0'; }
        if (k < to_signed(max_encoding)) return k;
        return {};
    }

    void find_one_type(const std::string &s, std::vector<std::string> &S) { // \todo regexp?
        if (!s.starts_with("BeginType")) return;

        size_t b = 9;
        while (s[b] == ' ' || s[b] == '\t') b++;
        if (b == 9) return; // bad

        size_t a = b;
        while (std::isalpha(s[b]) != 0) b++;
        if (b == a) return; // bad

        auto str = s.substr(a, b - a);
        S.push_back(str);
        spdlog::trace("Defined type: {}", str);
    }

    // Returns 0, unless we see A="B", fills the buffers A and B.
    // return 2 if there is a space in A, 1 otherwise.
    auto see_an_assignment(Buffer &in, Buffer &key, Buffer &val) -> int {
        if (in.tp_fetch_something() != tl_normal) return 0;
        for (;;) {
            if (in.is_special_end()) return 0;
            if (in.head() == '=') break;
            key.push_back(in.head());
            in.advance();
        }
        in.advance();
        in.skip_sp_tab();
        if (in.head() != '\"') return 0;
        in.advance();
        tpage_ns::scan_item(in, val, '\"');
        key.ptrs.b = 0;
        key.remove_space_at_end();
        for (;;) {
            if (key.head() == 0) return 1;
            if (std::isspace(key.head()) != 0) return 2;
            key.advance();
        }
    }

    // This is called for all lines, outside groups.
    void see_main_a(Buffer &in, Buffer &val) {
        Buffer B;
        val.clear();
        int k = see_an_assignment(in, B, val);
        if (k == 1) the_names.assign(B, val);
    }
} // namespace

void LineList::change_encoding(long wc) {
    if (wc >= 0 && wc < to_signed(max_encoding)) {
        encoding = to_unsigned(wc);
        Logger::finish_seq();
        spdlog::trace("++ Input encoding changed to {} for {}", wc, file_name);
    }
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
    cur_line = 0;
    encoding = 0;
    clear();
    file_name = std::move(x);
}

// Insert a line at the end of the file, incrementing the line no
void LineList::insert(const std::string &c, bool cv) { emplace_back(++cur_line, c, cv); }

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
// return the line number, or std::nullopt if EOF.
auto LineList::get_next(Buffer &b) -> std::optional<int> {
    if (empty()) return {};
    int  n         = front().number;
    bool converted = front().converted;
    b.append(front());
    pop_front();
    if (!converted) {
        cur_file_name = file_name;
        b.convert_line(n, encoding);
    }
    return n;
}

auto LineList::get_next_cv(Buffer &b, size_t w) -> int {
    if (empty()) return -1;
    b.append(front());
    auto n = front().number;
    pop_front();
    if (w != 0) {
        cur_file_name = file_name;
        b.convert_line(n, w);
    }
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
            the_main.doc_class_pos = C;
            return match.get<1>().to_string();
        }
    }
    the_main.doc_class_pos = end();
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
    splice(begin(), L);
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
auto LineList::find_top_val(const std::string &s, bool c) -> std::string {
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
    splice(begin(), res);
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
        if (b == 0) see_main_a(B, local_buf);
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

// This reads the file named x.
// If spec is 0, we are reading the config file.
// If 2 it's a tex file, and the file is converted later.
// If 3, no conversion  done
// If 4, its is the main file, log not yet open.
void LineList::read(const std::string &x, int spec) { // \todo take a std::filesystem::path
    reset(x);
    if (pool_position) {
        insert(file_pool[*pool_position]);
        pool_position.reset();
        return;
    }

    std::ifstream fp(x);
    std::string   old_name = cur_file_name;
    cur_file_name          = x;
    Buffer B;
    encoding       = the_main.input_encoding;
    bool converted = spec < 2;
    int  co_try    = spec == 3 ? 0 : 20;
    for (;;) {
        int  c    = fp.get();
        bool emit = false;
        if (c == '\r') { // pc or mac ?
            emit = true;
            c    = fp.peek();
            if (c == '\n') fp.ignore();
        } else if (c == '\n')
            emit = true;
        else if (c == EOF) {
            if (!B.empty()) emit = true;
            cur_file_name = old_name;
        } else
            B.push_back(static_cast<char>(c));
        if (emit) {
            if (spec == 0) // special case of config file
                emit = B.push_back_newline_spec();
            else
                B.push_back_newline();
            if (co_try != 0) {
                co_try--;
                if (auto k = find_encoding(B)) {
                    encoding = *k;
                    co_try   = 0;
                    Logger::finish_seq();
                    spdlog::trace("++ Input encoding number {} detected  at line {} of file {}", *k, cur_line + 1, x);
                }
            }
            if (converted) B.convert_line(cur_line + 1, encoding);
            if (emit)
                insert(B, converted);
            else
                ++cur_line;
            B.clear();
        }
        if (c == EOF) break;
    }
}

void LineList::register_file() { file_pool.push_back(std::move(*this)); }
