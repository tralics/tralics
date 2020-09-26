#include "tralics/Bibtex.h"
#include "tralics/Bbl.h"
#include "tralics/Bibliography.h"
#include "tralics/Logger.h"
#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txinline.h"
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

namespace {
    int similar_entries;

    bool start_comma = true; // should we scan for an initial comma ?

    class Berror {};

    const std::array<String, 9> scan_msgs{
        "bad syntax for a field type",
        "bad syntax for an entry type",
        "bad syntax for a string name",
        "bad syntax for a field name",
        "\nremaining characters on current line will be ignored.\n", // 4
        "\nall characters up to next @ will be ignored.\n",          // 5
        "\nlet's assume that the brace ends the entry.\n",           // 6
        "\nall characters up to next `('  or `{' ignored.\n",        // 7
        "\nall characters up to next `=' ignored.\n"                 // 8
    };

    void boot_ra_prefix(String s) {
        std::array<char, 3 * 8> tmp{};
        for (unsigned i = 0; i < 8; i++) {
            size_t j   = i * 3;
            tmp[j]     = s[1];
            tmp[j + 1] = static_cast<char>('A' + to_signed(i));
            tmp[j + 2] = 0;
        }
        tmp[0] = s[0];
        tmp[3] = s[2];
        for (unsigned i = 0; i < 8; i++) { ra_pretable[i] = tmp.data() + i * 3; }
    }

    // Finds the type of an entry (or comment, string, preamble). \todo without the_names?
    auto find_type(const std::string &s) -> entry_type {
        if (s.empty()) return type_comment; // in case of error.

        std::vector<std::string> &Bib2 = the_main->bibtex_extensions_s;
        for (auto &i : Bib2)
            if (i == s) return type_comment;
        if (s == the_names["article"]) return type_article;
        if (s == the_names["book"]) return type_book;
        if (s == the_names["booklet"]) return type_booklet;
        if (s == the_names["conference"]) return type_conference;
        if (s == the_names["coursenotes"]) return type_coursenotes;
        if (s == the_names["comment"]) return type_comment;
        if (s == the_names["inbook"]) return type_inbook;
        if (s == the_names["incollection"]) return type_incollection;
        if (s == the_names["inproceedings"]) return type_inproceedings;
        if (s == the_names["manual"]) return type_manual;
        if (s == the_names["masterthesis"]) return type_masterthesis;
        if (s == the_names["mastersthesis"]) return type_masterthesis;
        if (s == the_names["misc"]) return type_misc;
        if (s == the_names["phdthesis"]) return type_phdthesis;
        if (s == the_names["proceedings"]) return type_proceedings;
        if (s == the_names["preamble"]) return type_preamble;
        if (s == the_names["techreport"]) return type_techreport;
        if (s == the_names["string"]) return type_string;
        if (s == the_names["unpublished"]) return type_unpublished;

        std::vector<std::string> &Bib = the_main->bibtex_extensions;
        for (size_t i = 0; i < Bib.size(); i++)
            if (Bib[i] == s) return entry_type(type_extension + i + 1);
        return type_unknown;
    }
} // namespace

// This reads conditionally a file. Returns true if the file exists.
auto Bibtex::read0(Buffer &B, bib_from ct) -> bool {
    B.append(".bib");
    if (auto of = tralics_ns::find_in_path(B); of) {
        spdlog::trace("Found BIB file: {}", *of);
        read(*of, ct);
        return true;
    }
    return false;
}

// This takes a file name. It handles the case where the file has a suffix
// like miaou+foot. Prints a warning if this is a bad name.
void Bibtex::read1(const std::string &cur) {
    Buffer B;
    B.append(cur);
    auto n = B.size();
    if (read0(B, from_year)) return;
    if (B.ends_with("+foot.bib")) {
        B.resize(n - 5);
        if (read0(B, from_foot)) return;
    }
    if (B.ends_with("+year.bib")) {
        B.resize(n - 5);
        if (read0(B, from_year)) return;
    }
    if (B.ends_with("+all.bib")) {
        B.resize(n - 4);
        if (read0(B, from_any)) return;
    }
    if (B.ends_with("+refer.bib")) {
        B.resize(n - 6);
        if (read0(B, from_refer)) return;
    }
    if (B.ends_with(".bib")) {
        B.resize(n - 4);
        if (read0(B, from_year)) return;
    }
    spdlog::warn("Bibtex Info: no biblio file {}", B);
}

// Handles one bib file for the raweb. Returns true if the file exists.
// Extension can be foot, year or refer. New in Tralics 2.9.3 it can be any
auto Bibtex::read2(bib_from pre) -> bool {
    Buffer B;
    B.append(no_year);
    if (pre == from_foot)
        B.append("_foot");
    else if (pre == from_refer)
        B.append("_refer");
    else if (pre == from_any)
        B.append("_all");
    B.append(default_year);
    return read0(B, pre);
}

// This loads all three files, if we are compiling the raweb. Otherwise,
// there is no default.
// We read apics2006_all.bib if this exists
void Bibtex::read_ra() {
    if (in_ra) {
        bbl.open();
        if (read2(from_any)) return;
        read2(from_year);
        read2(from_foot);
        read2(from_refer);
    }
}

// Returns the index of the macro named name if it exists,  not_found otherwise.
auto Bibtex::look_for_macro(const Buffer &name) -> std::optional<size_t> { return look_for_macro(name.hashcode(bib_hash_mod), name); }

auto Bibtex::look_for_macro(size_t h, const std::string &name) -> std::optional<size_t> {
    for (size_t i = 0; i < all_macros.size(); i++)
        if (all_macros[i].is_same(h, name)) return i;
    return {};
}

// This looks for a macro named name or xname. If the macro is not found
// and insert is true, we construct a new macro. If xname is true, then
// the macro is initialised to val.
// [ if xname true, we define a system macro,
//   otherwise we define/redefine user one]
auto Bibtex::find_a_macro(Buffer &name, bool insert, String xname, String val) -> std::optional<size_t> {
    if (xname != nullptr) name = xname;
    auto h   = name.hashcode(bib_hash_mod);
    auto lfm = look_for_macro(h, name);
    if (lfm || !insert) return lfm;
    auto res = all_macros.size();
    if (xname != nullptr)
        all_macros.emplace_back(h, xname, val);
    else
        all_macros.emplace_back(h, name);
    return res;
}

// This defines a system macro.
void Bibtex::define_a_macro(String name, String value) {
    Buffer B;
    find_a_macro(B, true, name, value);
}

// Return an integer associated to a field position.
auto Bibtex::find_field_pos(const std::string &s) -> field_pos {
    auto S = std::string(s);
    // Check is this has to be ignored
    std::vector<std::string> &Bib_s        = the_main->bibtex_fields_s;
    size_t                    additional_s = Bib_s.size();
    for (size_t i = 0; i < additional_s; i++)
        if (Bib_s[i] == S) return fp_unknown;

    // Check is this is standard
    if (S == the_names["address"]) return fp_address;
    if (S == the_names["author"]) return fp_author;
    if (S == the_names["booktitle"]) return fp_booktitle;
    if (S == the_names["chapter"]) return fp_chapter;
    if (S == the_names["doi"]) return fp_doi;
    if (S == the_names["edition"]) return fp_edition;
    if (S == the_names["editor"]) return fp_editor;
    if (S == the_names["howpublished"]) return fp_howpublished;
    if (S == the_names["institution"]) return fp_institution;
    if (S == the_names["isbn"]) return fp_isbn;
    if (S == the_names["issn"]) return fp_issn;
    if (S == the_names["isrn"]) return fp_isrn;
    if (S == the_names["journal"]) return fp_journal;
    if (S == the_names["cstb_key"]) return fp_key;
    if (S == the_names["month"]) return fp_month;
    if (S == the_names["langue"]) return fp_langue;
    if (S == the_names["cstb_language"]) return fp_langue;
    if (S == the_names["note"]) return fp_note;
    if (S == the_names["number"]) return fp_number;
    if (S == the_names["organization"]) return fp_organization;
    if (S == the_names["pages"]) return fp_pages;
    if (S == the_names["publisher"]) return fp_publisher;
    if (S == the_names["school"]) return fp_school;
    if (S == the_names["series"]) return fp_series;
    if (S == the_names["title"]) return fp_title;
    if (S == the_names["cstb_type"]) return fp_type;
    if (S == the_names["cstb_url"]) return fp_url;
    if (S == the_names["volume"]) return fp_volume;
    if (S == the_names["year"]) return fp_year;
    if (S == the_names["crossref"]) return fp_crossref;
    // Check is this is additional
    std::vector<std::string> &Bib        = the_main->bibtex_fields;
    size_t                    additional = Bib.size();
    for (size_t i = 0; i < additional; i++)
        if (Bib[i] == S) return field_pos(fp_unknown + i + 1);
    return fp_unknown;
}

// This finds a citation that matches exactly S
auto Bibtex::find_entry(const CitationKey &s) -> BibEntry * {
    auto len = all_entries.size();
    if (old_ra) {
        for (size_t i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same_old(s)) return all_entries[i];
    } else {
        for (size_t i = 0; i < len; i++)
            if (all_entries[i]->cite_key.is_same(s)) return all_entries[i];
    }
    return nullptr;
}

// This finds a citation whose lowercase equivalent is S.
// Puts in N the number of citations found.
auto Bibtex::find_lower_case(const CitationKey &s, int &n) -> BibEntry * {
    n = 0;
    BibEntry *res{nullptr};
    if (old_ra) {
        for (auto &all_entrie : all_entries)
            if (all_entrie->cite_key.is_same_lower_old(s)) {
                res = all_entrie;
                n++;
            }
    } else
        for (auto &all_entrie : all_entries)
            if (all_entrie->cite_key.is_same_lower(s)) {
                res = all_entrie;
                n++;
            }
    return res;
}

// This command is used in the case of apics_all.
// Assume that the citation is Knuth
// Returns -2 if we have \cite{Knuth}, \footcite{Knuth}
// Returns -2 if we have \cite{knuth}, \footcite{knuth}
// Returns 2  if we have \cite{KNUTH}, \footcite{knuth} or other mismatch
// Returns 0 if nothing was found.
auto Bibtex::find_similar(const CitationKey &s, int &n) -> BibEntry * {
    n = 0;
    BibEntry *res{nullptr};
    for (auto &all_entrie : all_entries)
        if (all_entrie->cite_key.is_similar(s)) {
            res = all_entrie;
            n++;
        }
    if (res != nullptr) {
        n = -n;
        return res;
    }
    bool bad = false;
    for (auto &all_entrie : all_entries)
        if (all_entrie->cite_key.is_similar_lower(s)) {
            n++;
            if (res == nullptr) {
                res = all_entrie;
                continue;
            }
            if (!all_entrie->cite_key.is_similar(res->cite_key)) bad = true;
        }
    if (!bad) n = -n;
    return res;
}

// This makes a new entry.
auto Bibtex::make_new_entry(const CitationKey &a, bib_creator b) -> BibEntry * { return make_entry(a, b, the_bibliography.unique_bid()); }

// Copy from the biblio
void Bibtex::make_entry(const CitationKey &a, std::string myid) { make_entry(a, because_cite, std::move(myid)); }

// Generic code
auto Bibtex::make_entry(const CitationKey &a, bib_creator b, std::string myid) -> BibEntry * {
    auto *X      = new BibEntry;
    X->cite_key  = a;
    X->why_me    = b;
    X->id        = all_entries.size();
    X->unique_id = std::move(myid);
    all_entries.push_back(X);
    return X;
}

auto Bibtex::exec_bibitem(const std::string &w, const std::string &b) -> std::string {
    BibEntry *X = find_entry(b, w, because_all);
    if (X->type_int != type_unknown) {
        the_parser.parse_error("Duplicate bibliography entry ignored");
        return std::string();
    }
    X->type_int = type_article;
    X->set_explicit_cit();
    return X->unique_id;
}

// Signals an error while reading the file.
// We do not use parse_error here
void Bibtex::err_in_file(String s, bool last) const {
    nb_errs++;
    Logger::finish_seq();
    log_and_tty << "Error detected at line " << cur_bib_line << " of bibliography file " << in_lines.file_name << "\n";
    if (!cur_entry_name.empty()) log_and_tty << "in entry " << cur_entry_name << " started at line " << last_ok_line << "\n";
    log_and_tty << s;
    if (last) log_and_tty << ".\n";
}

void Bibtex::err_in_entry(String a) {
    nb_errs++;
    log_and_tty << "Error signaled while handling entry " << cur_entry_name;
    if (cur_entry_line >= 0) log_and_tty << " (line " << cur_entry_line << ")";
    log_and_tty << "\n" << a;
}

void Bibtex::err_in_name(String a, long i) {
    err_in_entry(a);
    log_and_tty << "\nbad syntax in author or editor name\n";
    log_and_tty << "error occurred at character position " << i << " in the string\n" << name_buffer << ".\n";
}

// Returns next line of the .bib file. Error if EOF and what.
// Throws if EOF.
void Bibtex::next_line(bool what) {
    Buffer scratch;
    int    n = in_lines.get_next(scratch);
    if (n > 0)
        cur_bib_line = n;
    else {
        if (what) err_in_file("Illegal end of bibtex database file", true);
        throw Berror();
    }
    the_parser.set_cur_line(n);
    inbuf.insert_without_crlf(scratch);
    input_line     = codepoints(inbuf);
    input_line_pos = 0;
}

// Skip over a space. Error in case of EOF.
// This is the only function that reads a new line.
void Bibtex::skip_space() {
    for (;;) {
        if (at_eol())
            next_line(true);
        else {
            if (is_space(cur_char()))
                advance();
            else
                return;
        }
    }
}

// Reads until the next @ sign. This is the only function
// that accepts to read an EOF without error.
void Bibtex::scan_for_at() {
    for (;;) {
        if (at_eol())
            next_line(false);
        else {
            char32_t c = next_char();
            if (c == '@') return;
        }
    }
}

// Calls scan_identifier0, and interprets the return message.
// that should be 0, or at least 4 in absolute value
// If the retval of scan_identifier0 is <=0, but not -4
// Note: The error message must be output before skip_space,
// in case we are at EOF.
auto Bibtex::scan_identifier(size_t what) -> bool {
    int ret = scan_identifier0(what);
    if (ret != 0) log_and_tty << scan_msgs[to_unsigned(ret > 0 ? ret : -ret)];
    if (ret == 4 || ret == -4) {
        start_comma = false;
        reset_input();
        skip_space();
    }
    return ret > 0;
}

// Scans an identifier. It will be in lower case in token_buf.
// Scans also something after it. Invariant: at_eol() is false on entry.
// it is also false on exit
auto Bibtex::scan_identifier0(size_t what) -> int {
    Buffer &B = token_buf;
    B.clear();
    char32_t c = cur_char();
    if (!is_ascii(c) || std::isdigit(static_cast<int>(c)) || get_class(c) != legal_id_char) return wrong_first_char(c, what);
    for (;;) {
        if (at_eol()) break;
        c = next_char();
        if (!is_ascii(c) || get_class(c) != legal_id_char) {
            input_line_pos--; // c is to be read again
            break;
        }
        c = to_lower(c);
        B.push_back(c);
    }
    // a field part.
    if (what == 0) return check_val_end();
    if (at_eol() || is_space(c)) skip_space();
    if (what == 1) return check_entry_end(); // case of @foo
    return check_field_end(what);
}

// A bunch of functions called when we see the end of an identifier.
// We start with a function that complains if first character is wrong.
auto Bibtex::wrong_first_char(char32_t c, size_t what) const -> int {
    err_in_file(scan_msgs[what], false);
    if (std::isdigit(static_cast<int>(c)))
        log_and_tty << "\nit cannot start with a digit";
    else
        log_and_tty << fmt::format("\nit cannot start with `{}'", to_utf8(c));
    if (c == '%') log_and_tty << "\n(A percent sign is not a comment character in bibtex)";
    if (what == 1 || what == 2) return 5;
    if (what == 0) {
        if (c == '}') { // this brace might be the end of the entry
            return 6;
        }
    }
    return 4;
}

// We have seen @article, plus space, plus c. We want paren or brace.
// Return 0 if OK, a negative value otherwise.
// We read c; maybe additional characters in case of error
// @comment(etc) is ignored
auto Bibtex::check_entry_end() -> int {
    if (token_buf == "comment") return 0; // don't complain
    char32_t c = cur_char();
    if (c == '(' || c == '{') return check_entry_end(0);
    err_in_file(scan_msgs[1], false);
    log_and_tty << "\nexpected `('  or `{'";
    for (;;) {
        c = cur_char();
        if (c == '(' || c == '{') return check_entry_end(-7);
        advance();
        if (at_eol()) return -4;
    }
}

// We store in right_outer_delim the closing delimiter.
// associated to the current char. We skip over spaces.
auto Bibtex::check_entry_end(int k) -> int {
    right_outer_delim = cur_char() == '(' ? ')' : '}';
    advance();
    skip_space();
    return k;
}

// We have seen foo in foo=bar. We have skipped over spaces
// Returns 0 if OK. We try to read some characters on the current line
// in case of error
auto Bibtex::check_field_end(size_t what) -> int {
    if (cur_char() == '=') {
        advance();
        skip_space();
        return 0;
    }
    err_in_file(scan_msgs[what], false);
    log_and_tty << "\nexpected `=' sign";
    for (;;) {
        if (at_eol()) return what == 2 ? 5 : -4;
        if (cur_char() == '=') {
            advance();
            skip_space();
            return -8;
        }
        advance();
    }
}

// We have something like year=foo. After foo we can have a # (year is foo
// concatenated with something), a comma (there are more fields after foo)
// or brace/paren indicating the end of the entry. Of course spaces are allowed.
// Returns 0 ik OK, 4 otherwise.
auto Bibtex::check_val_end() -> int {
    if (at_eol()) return 0;
    char32_t c = cur_char();
    if (is_space(c) || c == '#' || c == ',' || c == char32_t(right_outer_delim)) return 0;
    err_in_file(scan_msgs[0], false);
    log_and_tty << fmt::format("\nit cannot end with `{}'\n", to_utf8(c)) << "expecting `,', `#' or `" << right_outer_delim << "'";
    return 4;
}

// Reads someththing like foo=bar, and stores the result in X if ok
// The first field is preceded by the key and a comma.
// Thus, we start reading the comma (unless previous fiels has a syntax err).
// A null pointer means an entry to be discarded
void Bibtex::parse_one_field(BibEntry *X) {
    if (start_comma) {
        if (cur_char() != ',')
            err_in_file("expected comma before a field", true);
        else
            advance();
    }
    start_comma = true;
    skip_space();
    if (cur_char() == char32_t(right_outer_delim)) return;
    if (scan_identifier(3)) return;
    field_pos where = fp_unknown;
    bool      store = false;
    if (X != nullptr) { // if X null, we just read, but store nothing
        cur_field_name = token_buf;
        where          = find_field_pos(token_buf);
        if (where != fp_unknown) store = true;
    }
    read_field(store);
    if (!store) return;
    bool ok = X->store_field(where);
    if (!ok) {
        err_in_file("", false);
        log_and_tty << "duplicate field `" << cur_field_name << "' ignored.\n";
        return;
    }
    if (where != fp_crossref) return;
    X->parse_crossref(); // special action
}

// This parses an @something.
void Bibtex::parse_one_item() {
    cur_entry_name = "";
    cur_entry_line = -1;
    scan_for_at();
    last_ok_line = cur_bib_line;
    skip_space();
    bool k = scan_identifier(1);
    if (k) return;
    entry_type cn = find_type(token_buf);
    if (cn == type_comment) return;
    if (cn == type_preamble) {
        read_field(true);
        bbl.append(field_buf);
    } else if (cn == type_string) {
        k = scan_identifier(2);
        if (k) return;
        auto X = *find_a_macro(token_buf, true, nullptr, nullptr);
        mac_def_val(X);
        read_field(true);
        mac_set_val(X, field_buf.special_convert(false));
    } else {
        cur_entry_line = cur_bib_line;
        Buffer A;
        skip_space();
        for (;;) {
            if (at_eol()) break;
            char32_t c = cur_char();
            if (c == ',' || is_space(c)) break;
            A << c;
            next_char();
        }
        skip_space();
        cur_entry_name = A;
        BibEntry *X    = see_new_entry(cn, last_ok_line);
        int       m    = similar_entries;
        while (cur_char() != ')' && cur_char() != '}') parse_one_field(X);
        if (m > 1) handle_multiple_entries(X);
    }
    char32_t c = cur_char();
    if (c == ')' || c == '}') advance();
    if (c != char32_t(right_outer_delim)) err_in_file("bad end delimiter", true);
}

void Bibtex::handle_multiple_entries(BibEntry *Y) {
    CitationKey s   = Y->cite_key;
    auto        len = all_entries.size();
    for (size_t i = 0; i < len; i++)
        if (all_entries[i]->cite_key.is_similar(s)) {
            BibEntry *X = all_entries[i];
            if (X == Y) continue;
            X->copy_from(Y);
        }
}

// This finds entry named s, or case-equivalent. If create is true,
// creates entry if not found.
// Used by see_new_entry (create=auto_cite), or parse_crossref
// prefix can be from_year, from_refer, from_foot, from_any;
// If create is true, it is either a crossref, or \nocite{*}+from_year
// or from_refer, or bug; if prefix is from_any, we use from_year instead.
//

auto Bibtex::find_entry(const std::string &s, bool create, bib_creator bc) -> BibEntry * {
    bib_from prefix = default_prefix();
    if (create && prefix == from_any) prefix = from_year;
    CitationKey key(prefix, s);
    similar_entries = 1;
    int n           = 0;
    if (prefix == from_any) {
        BibEntry *X = find_similar(key, n);
        if (n > 1) err_in_file("more than one lower case key equivalent", true);
        if (n < 0) similar_entries = -n;
        return X;
    }
    BibEntry *X = find_entry(key);
    if (X != nullptr) return X;
    X = find_lower_case(key, n);
    if (n > 1) err_in_file("more than one lower case key equivalent", true);
    if ((X == nullptr) && create) return make_new_entry(key, bc);
    return X;
}

// This command is called when we have see @foo{bar,... on line lineno
// The type of FOO is in cn, the value BAR in cur_entry_name.
// the entry is ignored if teh name is in the omit list (with the same case)
// This does not create a new entry, complains if the entry exists
// and is not empty. If OK, we start to fill the entry.

auto Bibtex::see_new_entry(entry_type cn, int lineno) -> BibEntry * {
    for (const auto &i : omitcite_list)
        if (i == cur_entry_name) {
            Logger::finish_seq();
            the_log << "bib: Omitting " << cur_entry_name << "\n";
            return nullptr;
        }
    BibEntry *X = find_entry(cur_entry_name, auto_cite(), because_all);
    if (X == nullptr) return X;
    if (X->type_int != type_unknown) {
        err_in_file("duplicate entry ignored", true);
        return nullptr;
    }
    if (old_ra && default_prefix() == from_refer) X->cite_key.move_to_refer();
    if (cn > type_extension) {
        X->is_extension = cn - type_extension;
        cn              = type_extension;
    } else
        X->is_extension = 0;
    X->type_int   = cn;
    X->first_line = lineno;
    return X;
}

// This reads a field into field_buf.
// This can be "foo" # {bar} # 1234 # macro.
// If store is false, we do not look at the value of a macro
void Bibtex::read_field(bool store) {
    field_buf.clear();
    for (;;) {
        read_one_field(store);
        skip_space();
        if (cur_char() != '#') return;
        advance();
        skip_space();
    }
}

// This reads a single field
void Bibtex::read_one_field(bool store) {
    char32_t c = cur_char();
    if (c == '{' || c == '\"') {
        uchar delimiter = c == '{' ? '}' : '\"';
        advance(); // reads left delimiter
        int bl = 0;
        for (;;) {
            if (at_eol()) {
                field_buf.push_back(' ');
                next_line(true);
                continue;
            }
            c = cur_char();
            advance();
            if (c == '\\') {
                field_buf.push_back(c);
                if (at_eol()) {
                    field_buf.push_back(' ');
                    next_line(true);
                    continue;
                }
                field_buf.push_back(cur_char());
                advance();
                continue;
            }
            if (c == delimiter && bl == 0) return;
            if (c == '}' && bl == 0) {
                err_in_file("illegal closing brace", true);
                continue;
            }
            field_buf.push_back(c);
            if (c == '}') bl--;
            if (c == '{') bl++;
        }
    } else if (std::isdigit(static_cast<int>(c))) {
        for (;;) {
            if (at_eol()) return;
            c = cur_char();
            if (!std::isdigit(static_cast<int>(c))) return;
            field_buf.push_back(c);
            advance();
        }
    } else {
        bool k = scan_identifier(0);
        if (store && !k) {
            auto macro = find_a_macro(token_buf, false, nullptr, nullptr);
            if (!macro) {
                err_in_file("", false);
                log_and_tty << "undefined macro " << token_buf << ".\n";
            } else
                field_buf += all_macros[*macro].value;
        }
    }
}

// Returns true if because of \nocite{*}
auto Bibtex::auto_cite() const -> bool {
    if (refer_biblio) return true;
    if (normal_biblio && nocitestar) return true;
    return false;
}

// This finds entry named s, or case-equivalent.
// creates entry if not found. This is used by exec_bibitem
auto Bibtex::find_entry(const std::string &s, const std::string &prefix, bib_creator bc) -> BibEntry * {
    CitationKey key(prefix, s);
    BibEntry *  X = find_entry(key);
    if (X != nullptr) return X;
    int n = 0;
    X     = find_lower_case(key, n);
    if (n > 1) err_in_file("more than one lower case key equivalent", true);
    if (X == nullptr) return make_new_entry(key, bc);
    return X;
}

// This is the main function.
void Bibtex::work() {
    auto n = all_entries.size();
    if (n == 0) return;
    if (!bbl.empty()) bbl.flush();
    boot_ra_prefix("ABC");
    all_entries_table.reserve(n);
    for (size_t i = 0; i < n; i++) all_entries[i]->un_crossref();
    for (size_t i = 0; i < n; i++) all_entries[i]->work(to_signed(i));
    auto nb_entries = all_entries_table.size();
    spdlog::trace("Seen {} bibliographic entries.", nb_entries);
    // Sort the entries
    std::sort(all_entries_table.begin(), all_entries_table.end(), [](auto a, auto b) { return a->sort_label < b->sort_label; });
    std::string previous_label;
    int         last_num = 0;
    for (size_t i = 0; i < nb_entries; i++) all_entries_table[i]->forward_pass(previous_label, last_num);
    int next_extra = 0;
    for (size_t i = nb_entries; i > 0; i--) all_entries_table[i - 1]->reverse_pass(next_extra);
    if (want_numeric)
        for (size_t i = 0; i < nb_entries; i++) all_entries_table[i]->numeric_label(i + 1);
    for (size_t i = 0; i < nb_entries; i++) all_entries_table[i]->call_type();
    bbl.too_late = true;
}

// Signals an error if the year is invalid in the case of refer.
auto Bibtex::wrong_class(int y, const std::string &Y, bib_from from) -> bool {
    if (from != from_refer) return false;
    if (old_ra) return false;
    int ry = the_parser.get_ra_year();
    if (y <= 0 || y > ry || (!distinguish_refer && y == ry)) {
        the_bibtex.err_in_entry("");
        log_and_tty << "entry moved from refer to year because\n";
        if (y == 0)
            log_and_tty << "the year field of this entry is missing.\n";
        else if (y < 0)
            log_and_tty << "the year field of this entry is invalid";
        else if (y == ry)
            log_and_tty << "it is from this year";
        else
            log_and_tty << "it is unpublished";
        if (!Y.empty()) log_and_tty << " (year field is `" << Y << "').\n";
        return true;
    }
    return false;
}

void Bibtex::bad_year(const std::string &given, String wanted) {
    the_bibtex.err_in_entry("");
    log_and_tty << "the year field of this entry should be " << wanted << ", ";
    if (given.empty())
        log_and_tty << "it is missing.\n";
    else
        log_and_tty << "it is `" << given << "'.\n";
}

// Faire un hack: s'il y a un extrabib, et normal_biblio
// le rajouter a la fin, et virer le extrabib
// Plus le parser propement

void Bibtex::read(const std::string &src, bib_from ct) {
    bbl.append("% reading source ");
    bbl.append(src);
    bbl.flush();
    entry_prefix  = ct;
    normal_biblio = ct == from_year;
    refer_biblio  = ct == from_refer;
    in_lines.read(src, 1);
    interactive = false;

    last_ok_line = 0;
    reset_input();
    try {
        for (;;) parse_one_item();
    } catch (Berror x) {}

    if (!bbl.empty()) {
        bbl.append("%");
        bbl.flush();
    }
}

void Bibtex::boot(std::string S, bool inra) {
    no_year      = std::move(S);
    in_ra        = inra;
    want_numeric = false;
    if (the_main->handling_ra) want_numeric = true;
    for (auto &id_clas : id_class) id_clas = legal_id_char;
    for (size_t i = 0; i < 32; i++) id_class[i] = illegal_id_char;
    id_class[static_cast<unsigned char>(' ')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('\t')] = illegal_id_char;
    id_class[static_cast<unsigned char>('"')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('#')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('%')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('\'')] = illegal_id_char;
    id_class[static_cast<unsigned char>('(')]  = illegal_id_char;
    id_class[static_cast<unsigned char>(')')]  = illegal_id_char;
    id_class[static_cast<unsigned char>(',')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('=')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('{')]  = illegal_id_char;
    id_class[static_cast<unsigned char>('}')]  = illegal_id_char;
    bootagain();
}

void Bibtex::bootagain() {
    old_ra = the_parser.get_ra_year() < 2006; // \todo we should really not keep this around

    if (the_parser.cur_lang_fr()) { // french
        define_a_macro("jan", "janvier");
        define_a_macro("feb", "f\\'evrier");
        define_a_macro("mar", "mars");
        define_a_macro("apr", "avril");
        define_a_macro("may", "mai");
        define_a_macro("jun", "juin");
        define_a_macro("jul", "juillet");
        define_a_macro("aug", "ao\\^ut");
        define_a_macro("sep", "septembre");
        define_a_macro("oct", "octobre");
        define_a_macro("nov", "novembre");
        define_a_macro("dec", "d\\'ecembre");
        my_constant_table[0] = "th\\`ese de doctorat";
        my_constant_table[1] = "rapport technique";
        my_constant_table[2] = "m\\'emoire";
    } else if (the_parser.cur_lang_german()) { //	      german
        define_a_macro("jan", "Januar");
        define_a_macro("feb", "Februar");
        define_a_macro("mar", "M\\\"arz");
        define_a_macro("apr", "April");
        define_a_macro("may", "Mai");
        define_a_macro("jun", "Juni");
        define_a_macro("jul", "Juli");
        define_a_macro("aug", "August");
        define_a_macro("sep", "September");
        define_a_macro("oct", "Oktober");
        define_a_macro("nov", "November");
        define_a_macro("dec", "Dezember");
        my_constant_table[0] = "Ph. D. Thesis";
        my_constant_table[1] = "Technical report";
        my_constant_table[2] = "Masters thesis";
    } else { // non french, assume english
        define_a_macro("jan", "January");
        define_a_macro("feb", "February");
        define_a_macro("mar", "March");
        define_a_macro("apr", "April");
        define_a_macro("may", "May");
        define_a_macro("jun", "June");
        define_a_macro("jul", "July");
        define_a_macro("aug", "August");
        define_a_macro("sep", "September");
        define_a_macro("oct", "October");
        define_a_macro("nov", "November");
        define_a_macro("dec", "December");
        my_constant_table[0] = "Ph. D. Thesis";
        my_constant_table[1] = "Technical report";
        my_constant_table[2] = "Masters thesis";
    }
}
