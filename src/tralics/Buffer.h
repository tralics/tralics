#pragma once
#include "../txscaled.h"
#include "Xid.h"
#include <cstring>
#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <vector>

struct AttPair;
class Xml;

/// A utility structure which is basically a std::string with a marked range,
/// and many methods specific to TeX.

class Buffer : public std::string {
public:
    struct span {
        size_t a{0}, b{0};
    };

    span ptrs;

    Buffer() = default;
    Buffer(const std::string &s) : std::string(s) {}

    using std::string::string;
    using std::string::operator=;
    using std::string::push_back;

    // Standard const methods
    [[nodiscard]] auto at_eol() const -> bool { return ptrs.b >= size(); } ///< Is the read pointer at the end?
    [[nodiscard]] auto convert_to_log_encoding() const -> std::string;     ///< Convert to logging encoding
    [[nodiscard]] auto convert_to_out_encoding() const -> std::string;     ///< Make a fresh copy with output encoding
    [[nodiscard]] auto hashcode(size_t prime) const -> size_t;             ///< Hash code of the string in the buffer
    [[nodiscard]] auto head() const -> char { return (*this)[ptrs.b]; }    ///< The character under the read pointer
    [[nodiscard]] auto insert_space_here(size_t k) const -> bool;          ///< For typography
    [[nodiscard]] auto int_val() const -> std::optional<size_t>;           ///< Try to parse the contents as an integer
    [[nodiscard]] auto is_all_ascii() const -> bool;                       ///< Is everything ASCII and not CRLF?
    [[nodiscard]] auto is_and(size_t k) const -> bool;                     ///< Is the word at `k` an `and`?
    [[nodiscard]] auto is_spaceh(size_t j) const -> bool;                  ///< It the char at `j` a space?
    [[nodiscard]] auto is_special_end() const -> bool;                     ///< Is the current char `\\n`, `#` or `%`?
    [[nodiscard]] auto next_non_space(size_t j) const -> size_t;           ///< Locate next non-space char after `j`
    [[nodiscard]] auto see_config_env() const -> int;                      ///< Do we start with `Begin` or `End`?
    [[nodiscard]] auto single_char() const -> char;                        ///< If only one (non-space) char, return it
    [[nodiscard]] auto special_exponent() const -> String;                 ///< Normalize contents as exponent name (th,nd...)
    [[nodiscard]] auto substring() const -> std::string;                   ///< Get the slice [ptrs.a,ptrs.b)

    // Mutating methods, affecting the data but not ptrs
    void dump_prefix(bool err, bool gbl, symcodes K); ///< Insert def qualifiers (`\global` etc.)
    void lowercase();
    void remove_last(size_t n); ///< Drop `n` chars, provided size is large enough
    void uppercase();

    // Mutating methods, affecting ptrs but not the data, as intended
    void advance(size_t k = 1) { ptrs.b += k; } ///< Move the read pointer forward
    auto next_char() { return (*this)[ptrs.b++]; }

    // Mutating methods, affecting ptrs but not the data, but morally const
    // \todo all those should be const

    // Mutating methods, affecting both the data and ptrs
    void insert_without_crlf(const std::string &s); ///< Reset, insert s minus CRLF, remove trailing spaces

    // Those have void return type but return parameters
    void fill_table(bchar_type *table); ///< Not sure what this does?

    // Those have void return type but involve global variables
    void convert_line(int l, size_t wc); ///< Convert a line to UTF8
    void find_top_atts();                ///< This does something with DocAttribs \todo [vb] sic
    void finish_xml_print();             ///< Flush the buffer
    void insert_escape_char();           ///< Inserts the current escape char
    void insert_escape_char_raw();       ///< This one is for `\meaning`

    // Those are not const and have a return value, mostly they leave some
    // crucial info in ptrs.b and ptrs.a or just reset. \todo refactor all that
    [[nodiscard]] auto add_with_space(const std::string &s) -> std::string; ///< Weird RA stuff \todo remove
    [[nodiscard]] auto backup_space() -> bool;                              ///< Remove trailing spaces
    [[nodiscard]] auto contains_braced(const std::string &s) -> bool;       ///< Do we contain s with braces? (sets ptrs.b after `}`)
    [[nodiscard]] auto contains_env(const std::string &env) -> bool;        ///< Do we contain `\end{env}`?
    [[nodiscard]] auto fetch_spec_arg() -> bool;                            ///< Try to read a braced argument
    [[nodiscard]] auto find_alias(const std::vector<std::string> &SL, std::string &res) -> bool; ///< Find one aliases in the config file.
    [[nodiscard]] auto find_and(const bchar_type *table) -> bool;                                ///< True iff we do not contain 'and'
    [[nodiscard]] auto find_equals() -> bool;     ///< Locate a `sth=` pattern into ptrs.b and ptrs.a
    [[nodiscard]] auto horner(size_t p) -> Digit; ///< Read an integer at `ptrs.b`, advance

    // Those are still unsorted as refactoring proceeds

    auto is_begin_something(const std::string &s) -> int;
    auto look_at_space(const std::string &s) -> bool;
    auto next_utf8_char() -> char32_t;
    auto push_back_newline_spec() -> bool;
    auto reverse_horner() -> unsigned;
    auto see_config_kw(String s, bool c) -> String;
    auto see_equals(String s) -> bool;
    auto skip_string(const std::string &s) -> bool;
    auto skip_word_ci(const std::string &s) -> bool;
    auto slash_separated(std::string &a) -> bool;
    auto special_convert(bool init) -> std::string;
    auto split_at_colon(std::string &before, std::string &after) -> bool;
    auto str_toks(nl_to_tok nl) -> TokenList;
    auto str_toks11(bool nl) -> TokenList;
    auto string_delims() -> bool;
    auto svn_id(std::string &name, std::string &date, std::string &version) -> bool;
    auto tp_fetch_something() -> tpa_line;
    auto tp_next_char(char &res) -> bool;
    auto xml_and_attrib(const std::string &s) -> Xml;

    void insert_token(Token T, bool sw);
    void interpret_aux(std::vector<std::string> &bib, std::vector<std::string> &bib2);
    void l3_fabricate_cond(const std::string &base, const std::string &sig, subtypes w);
    void new_word();
    void next_bibtex_char();
    void normalise_for_bibtex(String s);
    void out_four_hats(char32_t ch);
    void out_log(char32_t ch, output_encoding_type T);
    void pt_to_mu();
    void push_back_alt(const AttPair &X);
    void push_back_elt(const std::string &name, Xid id, int w);
    void push_back_ent(char32_t ch);
    void push_back_math_aux(std::string s);
    void push_back_math_tag(const CmdChr &x, int type);
    void push_back_math_tag(std::string s, int type);
    void push_back_math_token(const CmdChr &x, bool space);
    void push_back_newline();
    void push_back_real_utf8(char32_t c);
    void push_back_roman(long n);
    void push_back_Roman(long n);
    void push_back_special_att(Xid id);
    void push_back_unless_punct(char c);
    void push_back_xml_char(uchar c);
    void remove_last_space();
    void remove_space_at_end();
    void show_uncomplete(String m);
    void skip_letter_dig_dot_slash();
    void skip_letter_dig_dot();
    void skip_letter_dig();
    void skip_letter();
    void skip_over_brace();
    void skip_sp_tab_comma();
    void skip_sp_tab_nl();
    void skip_sp_tab(); // \todo skip(const std::string&)
    void special_title(std::string s);

    auto push_back(Token T) -> bool;
    void push_back(char32_t c);
    void push_back(uchar c);
    void push_back(ScaledInt v, glue_spec unit);
    void push_back(const SthInternal &x);
    void push_back(const Glue &x);
    void push_back(const AttList &Y);
    void push_back(const AttPair &X);
    void push_back(const Macro &x);
    void push_back(const Macro &x, bool sw);
    void push_back(const TokenList &L);

    template <typename... Args> void format(const char *f, Args &&... args) { append(fmt::format(f, args...)); }
};

template <typename T> auto operator<<(Buffer &B, const T &t) -> Buffer & {
    B.push_back(t);
    return B;
}

inline auto encode(const std::string &s) -> std::string { return Buffer(s).convert_to_out_encoding(); }

inline Buffer err_buf, name_buffer, field_buf, shbuf, scbuf, Thbuf1, errbuf, Trace;
