// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2006-2015

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Functions on files and characters;
// Handle also utf8 input output

#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Saver.h"
#include "tralics/util.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <utf8.h>

namespace {
    void utf8_ovf(size_t n) { // \todo inline
        spdlog::error("UTF-8 parsing overflow (char U+{:04X}, line {}, file {})", n, cur_file_line, cur_file_name);
        bad_chars++;
    }
    /// Look for a file in the pool
    auto search_in_pool(const std::string &name) -> std::optional<size_t> {
        for (size_t i = 0; i < file_pool.size(); i++)
            if (file_pool[i].file_name == name) return i;
        return {};
    }
} // namespace

namespace io_ns {
    void print_ascii(std::ostream &fp, char c);
    auto plural(int n) -> String;
    void set_enc_param(long enc, long pos, long v);
    auto get_enc_param(long enc, long pos) -> long;
} // namespace io_ns

// ---------------------------------------------------------
// Output methods for characters

// This prints a character in the form \230 if not ascii
// Can be used in case where encoding is strange

void io_ns::print_ascii(std::ostream &fp, char c) {
    if (32 <= c && c < 127)
        fp << c;
    else {
        auto     C = static_cast<uchar>(c);
        unsigned z = C & 7;
        unsigned y = (C >> 3) & 7;
        unsigned x = (C >> 6) & 7;
        fp << "\\" << uchar(x + '0') << uchar(y + '0') << uchar(z + '0');
    }
}

// returns true if only ascii 7 bits in the buffer
auto Buffer::is_all_ascii() const -> bool {
    for (size_t i = 0; i < size(); i++) {
        auto c = at(i);
        if (static_cast<uchar>(c) >= 128) return false;
        if (c < 32 && c != '\t' && c != '\n') return false;
    }
    return true;
}

// ------------------------------------------------------------------------
// Functions that extract utf8 characters from streams and buffers

auto io_ns::plural(int n) -> String { return n > 1 ? "s" : ""; }

void Stats::io_convert_stats() {
    if (bad_chars != 0) spdlog::warn("Input conversion errors: {} char{}.", bad_chars, io_ns::plural(bad_chars));
}

// Returns 0 at end of line or error
// This complains if the character is greater than 1FFFF
auto Buffer::next_utf8_char() -> char32_t {
    auto     it = begin() + to_signed(ptrs.b), it0 = it;
    char32_t cp = 0;
    try {
        cp = it == end() ? char32_t(0U) : char32_t(utf8::next(it, end())); // \todo just if
    } catch (utf8::invalid_utf8 &) {
        bad_chars++;
        spdlog::warn("{}:{}:{}: UTF-8 parsing error, ignoring char", cur_file_name, cur_file_line, ptrs.b + 1);
        ++ptrs.b;
        return char32_t();
    }
    auto nn = to_unsigned(it - it0);
    ptrs.b += nn;
    if (cp > 0x1FFFF) {
        utf8_ovf(cp);
        return char32_t(); // \todo nullopt
    }
    return cp;
}

// This converts a line to UTF8
// Result of conversion is pushed back in the buffer
void Buffer::convert_line(int l, size_t wc) {
    cur_file_line = l;
    if (wc != 0) *this = convert_to_utf8(*this, wc);
}

// Why is v limited to 16bit chars?
void io_ns::set_enc_param(long enc, long pos, long v) {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) {
        the_parser.parse_error(fmt::format("Illegal encoding {}", enc));
        return;
    }
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) {
        the_parser.parse_error(fmt::format("Illegal encoding position {}", pos));
        return;
    }
    if (0 < v && v < int(nb_characters))
        custom_table[to_unsigned(enc)][to_unsigned(pos)] = char32_t(to_unsigned(v));
    else
        custom_table[to_unsigned(enc)][to_unsigned(pos)] = char32_t(to_unsigned(pos));
}

auto io_ns::get_enc_param(long enc, long pos) -> long {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) return pos;
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) return pos;
    return to_signed(custom_table[to_unsigned(enc)][to_unsigned(pos)]);
}

// This puts x into the buffer in utf8 form
void Buffer::push_back(char32_t c) { utf8::append(c, std::back_inserter(*this)); }

// This puts a 16bit char in the form ^^^^abcd in the buffer.
// Uses ^^ab notation if better
void Buffer::out_four_hats(char32_t ch) {
    if (ch == '\n') {
        push_back('\n');
        return;
    }
    if (ch == '\r') {
        push_back('\r');
        return;
    }
    if (ch == '\t') {
        push_back('\t');
        return;
    }
    unsigned c = ch;
    if (ch < 32) {
        append("^^");
        push_back(static_cast<char>(c + 64));
    } else if (ch == 127)
        append("^^?");
    else if (ch < 128)
        push_back(static_cast<char>(c));
    else {
        auto s = fmt::format("{:x}", c);
        for (size_t i = 0; i < s.size(); ++i) push_back('^');
        if (s.size() == 3) append("^0");
        append(s);
    }
}

// This is the function that puts a character into the buffer  as XML
// We must handle some character. We use entities in case of big values
// or control characters.

void Parser::process_char(char32_t c) { unprocessed_xml.push_back_real_utf8(c); }

// Assumes that c is not a special char
void Parser::process_char(uchar c) {
    if (c < 128)
        unprocessed_xml.push_back(static_cast<char>(c));
    else
        process_char(char32_t(c));
}

// This dumps a single character using log method
void Buffer::out_log(char32_t ch, output_encoding_type T) {
    if (ch == '\n')
        push_back('\n');
    else if (ch == '\r')
        append("^^M");
    else if (ch == '\t')
        push_back('\t');
    else if (ch < 32)
        out_four_hats(ch);
    else if (ch < 128)
        push_back(static_cast<char>(ch));
    else if (T == en_utf8)
        push_back(ch);
    else if (ch < 256 && T == en_latin)
        push_back(static_cast<char>(ch));
    else
        out_four_hats(ch);
}

// Converts the buffer to the output encoding
auto Buffer::convert_to_out_encoding() const -> std::string {
    auto T = the_main->output_encoding;
    if (T == en_boot || T == en_utf8 || is_all_ascii()) return data();
    return convert_to_latin1(*this, T == en_latin);
}

auto Buffer::convert_to_log_encoding() const -> std::string {
    auto T = the_main->log_encoding;
    if (T == en_utf8 || is_all_ascii()) return data();

    Buffer B = *this;
    B.ptrs.b = 0;
    Buffer utf8_out;
    for (;;) {
        char32_t c = B.next_utf8_char();
        if (c == 0) {
            if (B.at_eol()) break;
            utf8_out += "<null>";
        } else if (c == '\r')
            utf8_out += "^^M";
        else
            utf8_out.out_log(c, T);
    }
    return std::move(utf8_out);
}

// --------------------------------------------

// This exits if the file cannot be opened and argument is true
auto open_file(const std::string &name, bool fatal) -> std::ofstream {
    std::ofstream fp(name);
    if (!fp && fatal) {
        spdlog::critical("Fatal: Cannot open file {} for output.", name);
        exit(1);
    }
    if (!fp) spdlog::error("Cannot open file {} for output.", name);
    return fp;
}

// This implements the filecontent environment.
// \begin{filecontents}{name} some lines of code \end{filecontents}
// spec=0 normal, =1 star, =2 plus
void Parser::T_filecontents(int spec) {
    std::string filename;
    {
        flush_buffer();
        auto guard = InLoadHandler();
        filename   = sT_arg_nopar();
    }
    int           action = 0;
    std::ofstream outfile;
    bool          is_encoded = true;
    if (spec == 2 || spec == 3) {
        action = 2;
        LineList res;
        res.reset(filename);
        res.register_file();
        if (spec == 3) is_encoded = false;
    } else if (auto of = find_in_path(filename); of) {
        Logger::finish_seq();
        spdlog::warn("File {} already exists, not generating from source.", *of);
    } else {
        auto fn = get_out_dir(filename);
        outfile = open_file(fn, false);
        Logger::finish_seq();
        log_and_tty << "Writing file `" << fn << "'\n";
        if (!outfile)
            parse_error("unable to open file for writing");
        else {
            action = 1;
            if (spec == 0)
                outfile << "%% LaTeX2e file `" << filename << "' utf8-encoded\n"
                        << "%% generated by the `filecontents' environment\n"
                        << "%% from source `" << get_job_name() << "' on " << the_main->short_date << ".\n%%\n";
        }
    }
    kill_line();
    for (;;) {
        if (is_verbatim_end()) break;
        if (at_eol()) {
            parse_error("bad end of filecontents env");
            break;
        }
        input_buffer.push_back_newline();
        if (action == 1) outfile << input_buffer;
        if (action == 2) {
            int n = get_cur_line();
            file_pool.back().emplace_back(n, input_buffer, is_encoded);
        }
        kill_line();
    }
    kill_line(); // who knows
    cur_tok.kill();
    pop_level(bt_env);
}

// \todo the next three function are kind of misleadingly named

auto main_ns::search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path> {
    for (auto i = conf_path.size(); i != 0; i--) {
        auto f = conf_path[i - 1] / s;
        if (std::filesystem::exists(f)) {
            spdlog::trace("Found in configuration path: {}", f);
            return f;
        }
    }
    spdlog::warn("File {} not found in configuration path", s);
    return {};
}

auto find_in_confdir(const std::string &s) -> std::optional<std::filesystem::path> {
    pool_position = search_in_pool(s);
    if (pool_position) return s;
    if (std::filesystem::exists(s)) return s;
    if (s.empty() || s[0] == '.' || s[0] == '/') return {};
    return main_ns::search_in_confdir(s);
}

auto find_in_path(const std::string &s) -> std::optional<std::filesystem::path> {
    if (s.empty()) return {};
    pool_position = search_in_pool(s);
    if (pool_position) return s;
    if (s[0] == '.' || s[0] == '/') {
        if (std::filesystem::exists(s)) return s;
        return {};
    }
    for (const auto &p : input_path) {
        auto ss = p.empty() ? std::filesystem::path(s) : p / s;
        if (std::filesystem::exists(ss)) return ss;
    }
    return {};
}
