#pragma once
#include "../txid.h"
#include "../txscaled.h"
#include <cstring>

struct AttPair;
struct LinePtr;
class Xml;

/// a big structure \todo This is kind of a messy class, would be better off
/// using `std::string` as much as possible but we can't because of all the
/// zero-char manipulations. Or at least, replace wptr and vector::size() by
/// just size() and capacity() from the vector. Many methods do not belong in a
/// general class like that because they are specific to TeX.

class Buffer : public std::vector<char> {
public:
    size_t wptr{0}; ///< the write pointer
    size_t ptr{0};  ///< the read pointer
    size_t ptr1{0}; ///< a second read pointer

    Buffer() : std::vector<char>(1, 0) {}
    Buffer(const std::string &s) : Buffer() { push_back(s); }

    [[nodiscard]] auto at_eol() const -> bool { return wptr <= ptr; }         ///< Is the read pointer at the end?
    [[nodiscard]] auto c_str(size_t k = 0) const -> String;                   ///< Buffer contents as a char*
    [[nodiscard]] auto contains(const std::string &s) const -> bool;          ///< Does the buffer has s as a substring?
    [[nodiscard]] auto convert_to_latin1(bool nonascii) const -> std::string; ///< Convert to latin 1 or ASCII
    [[nodiscard]] auto convert_to_log_encoding() const -> std::string;        ///< Convert to logging encoding
    [[nodiscard]] auto convert_to_out_encoding() const -> std::string;        ///< Make a fresh copy with output encoding
    [[nodiscard]] auto empty() const -> bool { return size() == 0; }          ///< Is the write pointer at 0?
    [[nodiscard]] auto find_configuration(Buffer &aux) const -> bool;         ///< Extract config value \todo std::optional<std::string>
    [[nodiscard]] auto find_doctype() const -> size_t;                        ///< Figure out the doctype of the Buffer contents
    [[nodiscard]] auto hashcode(size_t prime) const -> size_t;                ///< Hash code of the string in the buffer
    [[nodiscard]] auto head() const -> char { return at(ptr); }               ///< The character under the read pointer
    [[nodiscard]] auto insert_space_here(size_t k) const -> bool;             ///< For typography
    [[nodiscard]] auto int_val() const -> std::optional<size_t>;              ///< Try to parse the contents as an integer
    [[nodiscard]] auto is_all_ascii() const -> bool;                          ///< Is everything ASCII and not CRLF?
    [[nodiscard]] auto is_and(size_t k) const -> bool;                        ///< Is the word at `k` an `and`?
    [[nodiscard]] auto is_good_ascii() const -> bool;                         ///< Is there no control or CRLF? (>128 ok, for UTF8)
    [[nodiscard]] auto is_spaceh(size_t j) const -> bool;                     ///< It the char at `j` a space?
    [[nodiscard]] auto is_special_end() const -> bool;                        ///< Is the current char `\\n`, `#` or `%`?
    [[nodiscard]] auto last_char() const -> char;                             ///< Last char if any, 0 if empty
    [[nodiscard]] auto last_slash() const -> std::optional<size_t>;           ///< Locate the last `/`, if any
    [[nodiscard]] auto next_non_space(size_t j) const -> size_t;              ///< Locate next non-space char after `j`
    [[nodiscard]] auto see_config_env() const -> int;                         ///< Do we start with `Begin` or `End`?
    [[nodiscard]] auto single_char() const -> char;                           ///< If only one (non-space) char, return it
    [[nodiscard]] auto single_character() const -> codepoint;                 ///< If only one (UTF8) character, return it
    [[nodiscard]] auto size() const -> size_t { return wptr; }                ///< Size of the contents \todo match vector::size()
    [[nodiscard]] auto special_exponent() const -> String;                    ///< Normalize contents as exponent name (th,nd...)
    [[nodiscard]] auto substring() const -> std::string;                      ///< Get the slice [ptr1,ptr)
    [[nodiscard]] auto to_string(size_t k = 0) const -> std::string;          ///< Buffer contents as a std::string

    void advance(size_t k = 1) { ptr += k; }          ///< Move the read pointer forward
    void alloc(size_t n);                             ///< Ensure that there is space for n+1 slots beyond wptr
    void dump_prefix(bool err, bool gbl, symcodes K); ///< Insert def qualifiers (`\global` etc.)
    void insert_string(const Buffer &s);              ///< Reset, insert s minus CRLF, remove trailing spaces
    void remove_last(size_t n = 1);                   ///< Drop `n` chars, provided size is large enough
    void remove_last_quote();                         ///< Drop last char if it is a `'`
    void reset(size_t k = 0);                         ///< Truncate buffer at `k` chars

    // Those have void return type but return parameters

    void extract_dtd(const std::string &a, std::string &b, std::string &c); ///< Get DTD name and file
    void extract_chars(vector<codepoint> &V);                               ///< Translate contents into codepoints
    void fill_table(bchar_type *table);                                     ///< Not sure what this does?
    void find_one_type(vector<std::string> &S);                             ///< Finds one type \todo [vb] what does that mean?

    // Those have void return type but involve global variables

    void convert_line(int l, size_t wc); ///< Convert a line to UTF8
    void find_top_atts();                ///< This does something with DocAttribs \todo [vb] sic
    void finish_xml_print();             ///< Flush the buffer, increment fp_len
    void insert_escape_char();           ///< Inserts the current escape char
    void insert_escape_char_raw();       ///< This one is for `\meaning`

    // Those are not const and have a return value, mostly they leave some
    // crucial info in ptr and ptr1 or just reset. \todo refactor all that

    [[nodiscard]] auto add_with_space(const std::string &s) -> std::string; ///< Weird RA stuff \todo remove
    [[nodiscard]] auto backup_space() -> bool;                              ///< Remove trailing spaces
    [[nodiscard]] auto contains_braced(const std::string &s) -> bool;       ///< Do we contain s with braces? (sets ptr after `}`)
    [[nodiscard]] auto contains_env(const std::string &env) -> bool;        ///< Do we contain `\end{env}`?
    [[nodiscard]] auto convert_line0(size_t wc) -> bool;                    ///< Convert to UTF8 into utf8_out
    [[nodiscard]] auto fetch_spec_arg() -> bool;                            ///< Try to read a braced argument
    [[nodiscard]] auto find_alias(const vector<std::string> &SL, std::string &res) -> bool; ///< Find one aliases in the config file.
    [[nodiscard]] auto find_and(const bchar_type *table) -> bool;                           ///< True iff we do not contain 'and'
    [[nodiscard]] auto find_documentclass(Buffer &aux) -> bool; ///< Extract the document class \todo optional tring
    [[nodiscard]] auto find_equals() -> bool;                   ///< Locate a `sth=` pattern into ptr and ptr1
    [[nodiscard]] auto get_machine_name() -> std::string;       ///< As name says \todo does not belong in Buffer
    [[nodiscard]] auto horner(size_t p) -> Digit;               ///< Read an integer at `ptr`, advance

    // Those are still unsorted as refactoring proceeds

    auto insert_break(const std::string &x) -> std::string;
    auto install_att(Xid idx, const Istring &match) -> bool;
    void interpret_aux(vector<Istring> &bib, vector<Istring> &bib2);
    void interpret_bibtex_list();
    void interpret_bibtex_extension_list();
    auto is_begin_something(String s) -> int;
    auto is_equal(String x) const -> bool { return strcmp(data(), x) == 0; }
    auto is_at_end(String s) const -> bool;
    auto is_here(String s) -> bool;
    auto is_here_case(String s) -> bool;
    auto look_at_space(const std::string &s) -> bool;
    void lowercase();
    void new_word();
    void next_bibtex_char();
    auto next_char() { return at(ptr++); }
    auto next_env_spec() -> bool;
    auto next_macro() -> bool;
    auto next_macro_spec() -> bool;
    auto next_utf8_byte() -> uchar;
    auto next_utf8_char() -> codepoint;
    auto next_utf8_char_aux() -> codepoint;
    void no_newline();
    void no_double_dot();
    void normalise_for_bibtex(String s);
    void optslash();
    void out_four_hats(codepoint ch);
    void out_log(codepoint ch, output_encoding_type T);
    auto pack_or_class(Buffer &aux) -> int;
    void pt_to_mu();
    void process_big_char(size_t n);
    void insert_token(Token T, bool sw);
    void push_back_alt(const AttPair &X);
    void push_back_braced(const std::string &s);
    void push_back_braced(String s);
    void push_back_def(String, std::string);
    void push_back_elt(const Istring &name, Xid id, int w);
    void push_back16(size_t n, bool uni);
    void push_back16l(bool hat, unsigned n);
    void push_back_ent(codepoint ch);
    void push_back_hex(unsigned c);
    void push_back_Hex(unsigned c);
    void push_back_math_token(const CmdChr &x, bool space);
    void push_back_math_tag(const CmdChr &x, int type);
    void push_back_math_tag(String s, int type);
    void push_back_math_aux(String s);
    void push_back_newline();
    auto push_back_newline_spec() -> bool;
    void push_back_open_brace() { push_back('{'); };
    void push_back_close_brace() { push_back('}'); };
    void push_back_roman(long n);
    void push_back_Roman(long n);
    void push_back_special_att(Xid id);
    void push_back_special_string(String s);
    void push_back_real_utf8(codepoint c);
    void push_back_xml_char(uchar c);
    void push_back_unless_punct(char c);
    void put_at_end(String s);
    auto remove_digits(const std::string &s) -> std::string;
    auto remove_space(const std::string &x) -> std::string;
    void remove_last_space();
    void remove_space_at_end();
    void remove_spec_chars(bool url, Buffer &B);
    auto reverse_horner() -> unsigned;
    auto see_config_kw(String s, bool c) -> String;
    auto see_equals(String s) -> bool;
    void show_uncomplete(String m);
    void skip_over_brace();
    void skip_letter();
    void skip_sp_tab(); // \todo skip(const std::string&)
    void skip_sp_tab_nl();
    void skip_sp_tab_comma();
    void skip_letter_dig();
    void skip_letter_dig_dot();
    void skip_letter_dig_dot_slash();
    auto slash_separated(std::string &a) -> bool;
    auto split_at_colon(std::string &before, std::string &after) -> bool;
    auto svn_id(std::string &name, std::string &date, std::string &version) -> bool;
    auto special_convert(bool init) -> std::string;
    void special_title(std::string s);
    auto str_toks(nl_to_tok nl) -> TokenList;
    auto str_toks11(bool nl) -> TokenList;
    auto string_delims() -> bool;
    auto convert_for_xml_err(Token t) -> Istring;
    auto tp_next_char(char &res) -> bool;
    auto tp_fetch_something() -> tpa_line;
    auto trace_scan_dimen(Token T, ScaledInt v, bool mu) -> String;
    void undo() { ptr--; }
    void uppercase();
    void utf8_error(bool first);
    auto xml_and_attrib(const std::string &s) -> Xml;
    auto find_char(char c) -> bool;
    void l3_fabricate_cond(const std::string &base, const std::string &sig, subtypes w);
    auto after_slash() -> bool;

    auto push_back(Token T) -> bool;

    void push_back(codepoint c);
    void push_back(char c);
    void push_back(uchar c);
    void push_back(const Buffer &b) { push_back(b.data()); }
    void push_back(const std::string &s);
    void push_back(const Istring &X);
    void push_back(ScaledInt v, glue_spec unit);
    void push_back(const SthInternal &x);
    void push_back(const Glue &x);
    void push_back(const AttList &Y);
    void push_back(const AttPair &X);
    void push_back(const Macro &x);
    void push_back(const Macro &x, bool sw);
    void push_back(const TokenList &L);
    // \todo push_back(char*,...) to do fmt::format
};

template <typename T> auto operator<<(Buffer &B, const T &t) -> Buffer & {
    B.push_back(t);
    return B;
}

inline auto operator<<(Buffer &B, void f(Buffer &)) -> Buffer & {
    f(B);
    return B;
}

inline void bf_reset(Buffer &B) { B.wptr = 0; }
inline void bf_optslash(Buffer &B) { B.optslash(); }
inline void bf_comma(Buffer &B) {
    if (!B.empty()) B.push_back(',');
}

extern Buffer Txbuf, err_buf, ssa2;
