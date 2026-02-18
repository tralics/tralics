// Functions on files and characters;
// Handle also utf8 input output

#include "tralics/LineList.h"
#include "tralics/MainClass.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <utf8.h>

namespace {
    // Look for a file in the pool
    auto search_in_pool(const std::string &name) -> std::optional<size_t> {
        for (size_t i = 0; i < LineList::file_pool.size(); i++)
            if (LineList::file_pool[i].file_name == name) return i;
        return {};
    }
} // namespace

namespace io_ns {
    void set_enc_param(long enc, long pos, long v);
    auto get_enc_param(long enc, long pos) -> long;
} // namespace io_ns

// ------------------------------------------------------------------------
// Functions that extract utf8 characters from streams and buffers

// Returns 0 at end of line or error
// This complains if the character is greater than 1FFFF
auto Buffer::next_utf8_char() -> std::optional<char32_t> {
    auto     it = begin() + to_signed(ptrs.b), it0 = it;
    char32_t cp = 0;
    try {
        if (it == end())
            cp = char32_t(0U);
        else
            cp = char32_t(utf8::next(it, end()));
    } catch (utf8::invalid_utf8 &) {
        the_main.bad_chars++;
        spdlog::warn("{}:{}:{}: UTF-8 parsing error, ignoring char", the_parser.cur_file_name, the_parser.cur_file_line, ptrs.b + 1);
        ++ptrs.b;
        return std::nullopt;
    }
    auto nn = to_unsigned(it - it0);
    ptrs.b += nn;
    if (cp > 0x1FFFF) {
        spdlog::error("UTF-8 parsing overflow (char U+{:04X}, line {}, file {})", size_t(cp), the_parser.cur_file_line,
                      the_parser.cur_file_name);
        the_main.bad_chars++;
        return std::nullopt;
    }
    return cp;
}

// This converts a line to UTF8
// Result of conversion is pushed back in the buffer
void Buffer::convert_line(int l, size_t wc) {
    the_parser.cur_file_line = l;
    if (wc != 0) *this = convert_to_utf8(*this, wc);
}

// Why is v limited to 16bit chars?
void io_ns::set_enc_param(long enc, long pos, long v) {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) {
        the_parser.parse_error(fmt::format("Illegal encoding {}", enc));
        return;
    }
    static bool warned_custom_input_encoding = false;
    if (!warned_custom_input_encoding) {
        warned_custom_input_encoding = true;
        spdlog::warn("Input encoding tables >1 are deprecated; prefer UTF-8 or ISO-8859-1 source files.");
    }
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) {
        the_parser.parse_error(fmt::format("Illegal encoding position {}", pos));
        return;
    }
    if (0 < v && v < int(nb_characters))
        the_main.custom_table[to_unsigned(enc)][to_unsigned(pos)] = char32_t(to_unsigned(v));
    else
        the_main.custom_table[to_unsigned(enc)][to_unsigned(pos)] = char32_t(to_unsigned(pos));
}

auto io_ns::get_enc_param(long enc, long pos) -> long {
    if (!(enc >= 2 && enc < to_signed(max_encoding))) return pos;
    enc -= 2;
    if (!(pos >= 0 && pos < lmaxchar)) return pos;
    return to_signed(the_main.custom_table[to_unsigned(enc)][to_unsigned(pos)]);
}

// This puts x into the buffer in utf8 form
void Buffer::push_back(char32_t c) { utf8::append(c, std::back_inserter(*this)); }

// This is the function that puts a character into the buffer  as XML
// We must handle some character. We use entities in case of big values
// or control characters.

void Parser::process_char(char32_t c) { unprocessed_xml.append_with_xml_escaping(c); }
void Parser::process_char(int s) { process_char(char32_t(s)); }
void Parser::process_char(size_t c) { process_char(char32_t(c)); }

// Assumes that c is not a special char
void Parser::process_char(uchar c) {
    if (c < 128)
        unprocessed_xml.push_back(static_cast<char>(c));
    else
        process_char(char32_t(c));
}

// This dumps a single character using log method
void Buffer::out_log(char32_t ch) {
    if (ch == '\n')
        push_back('\n');
    else if (ch == '\r')
        append("^^M");
    else if (ch == '\t')
        push_back('\t');
    else if (ch < 32)
        append(four_hats(ch));
    else if (ch < 128)
        push_back(static_cast<char>(ch));
    else
        push_back(ch);
}

auto Buffer::convert_to_log_encoding() const -> std::string {
    if (is_all_ascii(*this)) return data();

    Buffer B = *this;
    B.ptrs.b = 0;
    Buffer utf8_out;
    for (;;) {
        auto c = B.next_utf8_char();
        if (!c)
            utf8_out += "<null>";
        else if (*c == 0) {
            if (B.at_eol()) break;
            utf8_out += "<null>";
        } else if (*c == '\r')
            utf8_out += "^^M";
        else
            utf8_out.out_log(*c);
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

auto open_file(const std::filesystem::path &name, bool fatal) -> std::ofstream { return open_file(name.string(), fatal); }

// This implements the filecontent environment.
// \begin{filecontents}{name} some lines of code \end{filecontents}
// spec=0 normal, =1 star, =2 plus
auto Parser::T_filecontents(subtypes spec) -> bool {
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
        spdlog::warn("File {} already exists, not generating from source.", of->string());
    } else {
        auto fn = get_out_dir(filename);
        outfile = open_file(fn, false);
        spdlog::info("Writing file `{}`", fn.string());
        if (!outfile)
            parse_error("unable to open file for writing");
        else {
            action = 1;
            if (spec == 0)
                outfile << "%% LaTeX2e file `" << filename << "' utf8-encoded\n"
                        << "%% generated by the `filecontents' environment\n"
                        << "%% from source `" << get_job_name() << "' on " << the_main.short_date << ".\n%%\n";
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
            LineList::file_pool.back().emplace_back(n, input_buffer, is_encoded);
        }
        kill_line();
    }
    kill_line(); // who knows
    cur_tok.kill();
    if (!pop_level(bt_env)) return false;
    return true;
}

// Note: the next three function are kind of misleadingly named

auto MainClass::search_in_confdir(const std::string &s) const -> std::optional<std::filesystem::path> {
    for (auto i = conf_path.size(); i != 0; i--) {
        auto f = conf_path[i - 1] / s;
        if (std::filesystem::exists(f)) {
            spdlog::trace("Found in configuration path: {}", f.string());
            return f;
        }
    }

    spdlog::warn("File {} not found in configuration path", s);
    return {};
}

auto find_in_confdir(const std::string &s) -> std::optional<std::filesystem::path> {
    LineList::pool_position = search_in_pool(s);
    if (LineList::pool_position) return s;
    if (std::filesystem::exists(s)) return s;
    if (s.empty() || s[0] == '.' || s[0] == '/') return {};
    return the_main.search_in_confdir(s);
}

auto find_in_path(const std::string &s) -> std::optional<std::filesystem::path> {
    if (s.empty()) return {};
    LineList::pool_position = search_in_pool(s);
    if (LineList::pool_position) return s;
    if (s[0] == '.' || s[0] == '/') {
        if (std::filesystem::exists(s)) return s;
        return {};
    }
    for (const auto &p : the_main.input_path) {
        auto ss = p.empty() ? std::filesystem::path(s) : p / s;
        if (std::filesystem::exists(ss)) return ss;
    }
    if (auto kpse = find_in_kpathsea(s)) {
        spdlog::trace("Found in kpathsea: {}", kpse->string());
        return kpse;
    }
    return {};
}
