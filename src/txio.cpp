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

#include "tralics/Saver.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <sstream>
#include <utf8.h>

namespace {
    std::optional<size_t> pool_position; // \todo this is a static variable that should disappear

    /// Use a file from the pool
    auto use_pool(LinePtr &L) -> bool {
        if (!pool_position) return false; // should not happen
        L.insert(file_pool[*pool_position]);
        pool_position = {};
        return true;
    }

    void utf8_ovf(size_t n) { // \todo inline
        Converter &T = the_converter;
        buf.reset();
        buf.push_back16(n, true);
        spdlog::error("UTF-8 parsing overflow (char {}, line {}, file {})", buf, T.cur_file_line, T.cur_file_name);
        T.bad_chars++;
        T.new_error();
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
    auto how_many_bytes(char C) -> size_t;
    auto plural(int n) -> String;
    void set_enc_param(long enc, long pos, long v);
    auto get_enc_param(long enc, long pos) -> long;
} // namespace io_ns

// ---------------------------------------------------------

// Prints an att list on a buffer, then a stream.
void AttList::print(std::ostream &fp) const {
    buf.reset();
    buf.push_back(*this);
    fp << buf;
}

// Prints an att list on a stream.
auto operator<<(std::ostream &fp, Xid X) -> std::ostream & {
    X.get_att().print(fp);
    return fp;
}

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

// returns false if some unprintable characters appear
// Non-ascii chars are printable (assumes buffer is valid UTF8).
auto Buffer::is_good_ascii() const -> bool {
    for (size_t i = 0; i < size(); i++) {
        auto c = at(i);
        if (c < 32 && c != '\t' && c != '\n') return false;
    }
    return true;
}

// ------------------------------------------------------------------------
// Functions that extract utf8 characters from streams and buffers

auto io_ns::plural(int n) -> String { return n > 1 ? "s" : ""; }

void Stats::io_convert_stats() {
    int bl = the_converter.bad_lines; // \todo why is this twice what it should be?
    int bc = the_converter.bad_chars;
    int lc = the_converter.lines_converted;
    if (bl != 0) spdlog::warn("Input conversion errors: {} line{}, {} char{}.", bl, io_ns::plural(bl), bc, io_ns::plural(bc));
    if (lc != 0) spdlog::info("Input conversion: {} line{} converted.", lc, io_ns::plural(lc));
}

// If an error is signaled on current line, we do not signal again
// We mark current char as invalid
auto Converter::new_error() -> bool {
    if (global_error) return true;
    bad_lines++;
    global_error = true;
    return false;
}

// Action when starting conversion of line l of current file
void Converter::start_convert(int l) {
    cur_file_line = l;
    global_error  = false;
    line_is_ascii = true;
}

// This returns the number of bytes in a UTF8 character
// given the first byte. Returns 0 in case of error
auto io_ns::how_many_bytes(char c) -> size_t { return to_unsigned(utf8::internal::sequence_length(&c)); }

// Returns 0 at end of line or error
// This complains if the character is greater than 1FFFF
auto Buffer::next_utf8_char() -> codepoint {
    auto      it = begin() + to_signed(ptrs.b), it0 = it;
    codepoint cp;
    try {
        cp = codepoint(utf8::next(it, end()));
    } catch (utf8::invalid_utf8) {
        Converter &T = the_converter;
        T.bad_chars++;
        T.new_error();
        spdlog::warn("{}:{}:{}: UTF-8 parsing error, ignoring char", T.cur_file_name, T.cur_file_line, ptrs.b + 1);
        ++ptrs.b;
        return codepoint();
    }
    auto nn = to_unsigned(it - it0);
    if (nn != 1) the_converter.line_is_ascii = false;
    ptrs.b += nn;
    if (cp.is_verybig()) {
        utf8_ovf(cp.value);
        return codepoint();
    }
    return cp;
}

// If the buffer contains a unique character, return it
// Otherwise return 0. No error signaled
auto Buffer::single_character() const -> codepoint {
    auto it = begin();
    auto cp = utf8::next(it, end());
    return it == begin() + to_signed(size()) ? codepoint(cp) : codepoint(); // \todo the test should be it != end(), size() is wrong
}

// This converts a line in UTF8 format. Returns true if no conversion needed
auto Buffer::convert_line0(size_t wc) -> std::pair<bool, std::string> {
    Buffer utf8_out;
    ptrs.b = 0;
    codepoint c;
    for (;;) {
        if (wc == 0)
            c = next_utf8_char();
        else {
            auto C = static_cast<uchar>(next_char());
            if (wc == 1)
                c = codepoint(C);
            else
                c = custom_table[wc - 2][C];
            if (!(c.is_ascii() && c == C)) the_converter.line_is_ascii = false;
        }
        if (c.non_null()) utf8_out.push_back(c); // \todo use codepoint::to_utf8 when it exists
        if (at_eol()) break;
    }
    return {the_converter.line_is_ascii, utf8_out.to_string()};
}

// This converts a line in UTF8 format
// Result of conversion is pushed back in the buffer
void Buffer::convert_line(int l, size_t wc) {
    the_converter.start_convert(l);
    auto [o, s] = convert_line0(wc);
    if (o) return;
    the_converter.lines_converted++;
    reset();
    push_back(s);
}

// Why is v limited to 16bit chars?
void io_ns::set_enc_param(long enc, long pos, long v) {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) {
        buf << bf_reset << fmt::format("Illegal encoding {}", enc);
        the_parser.parse_error(buf.to_string());
        return;
    }
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) {
        buf << bf_reset << fmt::format("Illegal encoding position {}", pos);
        the_parser.parse_error(buf.to_string());
        return;
    }
    if (0 < v && v < int(nb_characters))
        custom_table[to_unsigned(enc)][to_unsigned(pos)] = codepoint(to_unsigned(v));
    else
        custom_table[to_unsigned(enc)][to_unsigned(pos)] = codepoint(to_unsigned(pos));
}

auto io_ns::get_enc_param(long enc, long pos) -> long {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) return pos;
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) return pos;
    return to_signed(custom_table[to_unsigned(enc)][to_unsigned(pos)].value);
}

auto io_ns::find_encoding(const std::string &cl) -> int {
    if (cl.find("-*-") != std::string::npos) {
        if (cl.find("coding: utf-8") != std::string::npos) return 0;
        if (cl.find("coding: utf8") != std::string::npos) return 0;
        if (cl.find("coding: latin1") != std::string::npos) return 1;
        if (cl.find("coding: iso-8859-1") != std::string::npos) return 1;
    }
    if (cl.find("iso-8859-1") != std::string::npos) return 1;
    if (cl.find("utf8-encoded") != std::string::npos) return 0;
    if (cl.find("%&TEX encoding = UTF-8") != std::string::npos) return 0; // \todo VB: check, this was 1 but that was dubious
    auto kk = cl.find("tralics-encoding:");
    if (kk == std::string::npos) return -1;
    if (!is_digit(cl[kk + 17])) return -1;
    int k = cl[kk + 17] - '0';
    if (is_digit(cl[kk + 18])) { k = 10 * k + cl[kk + 18] - '0'; }
    if (k < to_signed(max_encoding)) return k;
    return -1;
}

// This reads the file named x.
// If spec is 0, we are reading the config file.
// If 2 it's a tex file, and the file is converted later.
// If 3, no conversion  done
// If 4, its is the main file, log not yet open.
void tralics_ns::read_a_file(LinePtr &L, const std::string &x, int spec) {
    L.reset(x);
    if (use_pool(L)) return;
    std::ifstream fp(x);
    std::string   old_name      = the_converter.cur_file_name;
    the_converter.cur_file_name = x;
    Buffer B;
    auto   wc        = the_main->input_encoding;
    bool   converted = spec < 2;
    L.encoding       = the_main->input_encoding;
    int co_try       = spec == 3 ? 0 : 20;
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
            the_converter.cur_file_name = old_name;
        } else
            B.push_back(static_cast<char>(c));
        if (emit) {
            if (spec == 0) // special case of config file
                emit = B.push_back_newline_spec();
            else
                B.push_back_newline();
            if (co_try != 0) {
                co_try--;
                int k = io_ns::find_encoding(B.to_string());
                if (k >= 0) {
                    wc         = to_unsigned(k);
                    L.encoding = wc;
                    co_try     = 0;
                    Logger::finish_seq();
                    spdlog::trace("++ Input encoding number {} detected  at line {} of file {}", k, L.cur_line + 1, x);
                }
            }
            if (converted) B.convert_line(L.cur_line + 1, wc);
            if (emit)
                L.insert(B.to_string(), converted);
            else
                ++L.cur_line;
            B.reset();
        }
        if (c == EOF) break;
    }
}

// This puts x into the buffer in utf8 form \todo codepoint::to_utf8()
void Buffer::push_back(codepoint c) {
    auto x = c.value;
    if (x < 128) {
        push_back(static_cast<char>(x));
        return;
    }
    auto x1 = x >> 18;
    auto x2 = (x >> 12) & 63;
    auto x3 = (x >> 6) & 63;
    auto x4 = x & 63;
    if (x1 > 0 || x2 >= 16) {
        push_back(static_cast<char>(x1 + 128 + 64 + 32 + 16));
        push_back(static_cast<char>(x2 + 128));
        push_back(static_cast<char>(x3 + 128));
        push_back(static_cast<char>(x4 + 128));
    } else if (x2 > 0 || x3 >= 32) {
        push_back(static_cast<char>(x2 + 128 + 64 + 32));
        push_back(static_cast<char>(x3 + 128));
        push_back(static_cast<char>(x4 + 128));
    } else {
        push_back(static_cast<char>(x3 + 128 + 64));
        push_back(static_cast<char>(x4 + 128));
    }
}

inline void Buffer::push_back_hex(unsigned c) {
    if (c < 10)
        push_back(static_cast<char>(c + '0'));
    else
        push_back(static_cast<char>(c + 'a' - 10));
}

inline void Buffer::push_back_Hex(unsigned c) {
    if (c < 10)
        push_back(static_cast<char>(c + '0'));
    else
        push_back(static_cast<char>(c + 'A' - 10));
}

// Converts in uppercase hex. If uni is ptrue, produces U+00AB
void Buffer::push_back16(size_t n, bool uni) { // \todo fmt
    static std::array<unsigned, 9> dig;
    size_t                         k = 0;
    for (;;) { // construct the list of digits
        dig[k] = n % 16;
        n      = n / 16;
        k++;
        if (n == 0) break;
    }
    if (uni) {
        push_back("U+");
        if (k < 4) push_back('0'); // print at least 4 digit
        if (k < 3) push_back('0');
        if (k < 2) push_back('0');
        if (k < 1) push_back('0');
    }
    while (k > 0) push_back_Hex(dig[--k]);
}

// Converts number in lower hex form (assumes n>=16, so k>=2)
// if hat is true, inserts hats before
void Buffer::push_back16l(bool hat, unsigned n) {
    static std::array<unsigned, 9> dig;
    size_t                         k = 0;
    for (;;) { // construct the list of digits
        dig[k] = n % 16;
        n      = n / 16;
        k++;
        if (n == 0) break;
    }
    if (hat) {
        auto res = k;
        while (k > 0) {
            k--;
            push_back('^');
        }
        if (res == 3) {
            push_back('^');
            push_back('0');
        }
        k = res;
    }
    while (k > 0) push_back_hex(dig[--k]);
}

// This puts a 16bit char in the form ^^^^abcd in the buffer.
// Uses ^^ab notation if better
void Buffer::out_four_hats(codepoint ch) {
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
    unsigned c = ch.value;
    if (ch.is_control()) {
        push_back("^^");
        push_back(static_cast<char>(c + 64));
    } else if (ch.is_delete())
        push_back("^^?");
    else if (ch.is_ascii())
        push_back(static_cast<char>(c));
    else
        push_back16l(true, c);
}

// This inserts &#xabc;
void Buffer::push_back_ent(codepoint ch) {
    auto c = ch.value;
    if (c == 65534 || c == 65535) return; // these chars are illegal
    push_back('&');
    push_back('#');
    push_back('x');
    push_back16(c, false);
    push_back(';');
}

// This handles the case of \char 1234, where the number is at least 2^16
// Uses hex representation.
void Buffer::process_big_char(size_t n) {
    push_back('&');
    push_back('#');
    push_back('x');
    push_back16(n, false);
    push_back(';');
}

// This is the function that puts a character into the buffer  as XML
// We must handle some character. We use entities in case of big values
// or control characters.

void Parser::process_char(codepoint c) {
    if (c.is_null())
        unprocessed_xml.push_back(""); // may be required
    else if (c == '\n')
        unprocessed_xml.push_back('\n');
    else if (c == '\r')
        unprocessed_xml.push_back('\r');
    else if (c == '\t')
        unprocessed_xml.push_back('\t');
    else if (c == '<')
        unprocessed_xml.push_back("&lt;");
    else if (c == '>')
        unprocessed_xml.push_back("&gt;");
    else if (c == '&')
        unprocessed_xml.push_back("&amp;");
    else if (c.is_control() || c.is_big())
        unprocessed_xml.push_back_ent(c);
    else
        unprocessed_xml.push_back(c);
}

void Buffer::push_back_real_utf8(codepoint c) {
    if (c.is_null())
        push_back(""); // may be required
    else if (c == '\n')
        push_back('\n');
    else if (c == '\r')
        push_back('\r');
    else if (c == '\t')
        push_back('\t');
    else if (c == '<')
        push_back("&lt;");
    else if (c == '>')
        push_back("&gt;");
    else if (c == '&')
        push_back("&amp;");
    else if (c.is_control() || c.is_big())
        push_back_ent(c);
    else
        push_back(c);
}

// Assumes that c is not a special char
void Parser::process_char(uchar c) {
    if (c < 128)
        unprocessed_xml.push_back(static_cast<char>(c));
    else
        process_char(codepoint(c));
}

// This dumps a single character using log method
void Buffer::out_log(codepoint ch, output_encoding_type T) {
    if (ch == '\n')
        push_back('\n');
    else if (ch == '\r')
        push_back("^^M");
    else if (ch == '\t')
        push_back('\t');
    else if (ch.is_control())
        out_four_hats(ch);
    else if (ch.is_ascii())
        push_back(static_cast<char>(ch.value));
    else if (T == en_utf8)
        push_back(ch);
    else if (ch.is_small() && T == en_latin)
        push_back(static_cast<char>(ch.value));
    else
        out_four_hats(ch);
}

// Converts the buffer to the output encoding
auto Buffer::convert_to_out_encoding() const -> std::string {
    auto T = the_main->output_encoding;
    if (T == en_boot || T == en_utf8 || is_all_ascii()) return to_string(); // \todo use std::string
    return convert_to_latin1(T == en_latin);
}

// Convert to latin 1 or ASCII
auto Buffer::convert_to_latin1(bool nonascii) const -> std::string {
    Buffer B; // \todo do it without temporary buffer
    B.push_back(data());
    Buffer O;
    the_converter.global_error = false;
    for (;;) {
        codepoint c = B.next_utf8_char();
        if (c.is_null() && B.at_eol()) break;
        if (c.is_null()) continue;
        if (c.is_ascii())
            O.push_back(static_cast<char>(c.value));
        else if (c.is_small() && nonascii)
            O.push_back(static_cast<char>(c.value));
        else
            O.push_back_ent(c);
    }
    return O.to_string();
}

auto Buffer::convert_to_log_encoding() -> std::string {
    output_encoding_type T = the_main->log_encoding;
    if (is_all_ascii() || (T == en_utf8 && is_good_ascii())) return to_string();
    auto old_ptr               = ptrs.b;
    ptrs.b                     = 0;
    the_converter.global_error = false;
    Buffer utf8_out;
    for (;;) {
        codepoint c = next_utf8_char();
        if (c == 0) {
            if (at_eol()) break;
            utf8_out << "<null>";
        } else if (c == '\r')
            utf8_out << "^^M";
        else
            utf8_out.out_log(c, T);
    }
    ptrs.b = old_ptr;
    return utf8_out.to_string();
}

auto Buffer::codepoints() -> std::vector<codepoint> { // \todo use at more places, and exploit utfcpp
    std::vector<codepoint> V;
    the_converter.start_convert(the_parser.get_cur_line());
    ptrs.b = 0;
    for (;;) {
        codepoint c = next_utf8_char();
        if (c == 0 && at_eol()) return V;
        V.push_back(c);
    }
}

// --------------------------------------------

// This can be used to check if the main file exists. In this case the
// transcript file is not yet open.
auto tralics_ns::file_exists(const std::string &name) -> bool {
    auto e = std::filesystem::exists(name);
    Logger::finish_seq();
    spdlog::trace("++ file {} {}.", name, e ? "exists" : "does not exist");
    return e;
}

// This exits if the file cannot be opened and argument is true
auto tralics_ns::open_file(const std::string &name, bool fatal) -> std::ofstream {
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
        LinePtr res;
        res.reset(filename);
        main_ns::register_file(std::move(res));
        if (spec == 3) is_encoded = false;
    } else if (auto of = tralics_ns::find_in_path(filename); of) {
        Logger::finish_seq();
        spdlog::warn("File {} already exists, not generating from source.", *of);
    } else {
        auto fn = tralics_ns::get_out_dir(filename);
        outfile = tralics_ns::open_file(fn, false);
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
            file_pool.back().emplace_back(n, input_buffer.to_string(), is_encoded);
        }
        kill_line();
    }
    kill_line(); // who knows
    cur_tok.kill();
    pop_level(bt_env);
}

auto tralics_ns::find_in_confdir(const std::string &s, bool retry) -> std::optional<std::filesystem::path> {
    pool_position = search_in_pool(s);
    if (pool_position) return s;
    if (file_exists(s)) return s;
    if (!retry) return {};
    if (s.empty() || s[0] == '.' || s[0] == '/') return {};
    return main_ns::search_in_confdir(s);
}

auto tralics_ns::find_in_path(const std::string &s) -> std::optional<std::filesystem::path> {
    if (s.empty()) return {};
    pool_position = search_in_pool(s);
    if (pool_position) return s;
    if (s[0] == '.' || s[0] == '/') {
        if (file_exists(s)) return s;
        return {};
    }
    for (const auto &p : input_path) {
        auto ss = p.empty() ? std::filesystem::path(s) : p / s;
        if (file_exists(ss)) return ss;
    }
    return {};
}
