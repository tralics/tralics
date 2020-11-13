#include "tralics/Bibtex.h"
#include "tralics/Bbl.h"
#include "tralics/Bibliography.h"
#include "tralics/Logger.h"
#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

namespace {
    int similar_entries;

    bool start_comma = true; // should we scan for an initial comma ?

    class [[deprecated]] Berror {};

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

    // Finds the type of an entry (or comment, string, preamble). \todo without the_names?
    auto find_type(const std::string &s) -> entry_type {
        if (s.empty()) return type_comment; // in case of error.

        std::vector<std::string> &Bib2 = the_main.bibtex_extensions_s;
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

        std::vector<std::string> &Bib = the_main.bibtex_extensions;
        for (size_t i = 0; i < Bib.size(); i++)
            if (Bib[i] == s) return entry_type(type_extension + i + 1);
        return type_unknown;
    }

    // Return an integer associated to a field position.
    auto find_field_pos(const std::string &s) -> field_pos {
        auto S = std::string(s);
        // Check is this has to be ignored
        std::vector<std::string> &Bib_s = the_main.bibtex_fields_s;
        for (auto &b : Bib_s)
            if (b == s) return fp_unknown;

        // Check is this is standard
        if (s == the_names["address"]) return fp_address;
        if (s == the_names["author"]) return fp_author;
        if (s == the_names["booktitle"]) return fp_booktitle;
        if (s == the_names["chapter"]) return fp_chapter;
        if (s == the_names["doi"]) return fp_doi;
        if (s == the_names["edition"]) return fp_edition;
        if (s == the_names["editor"]) return fp_editor;
        if (s == the_names["howpublished"]) return fp_howpublished;
        if (s == the_names["institution"]) return fp_institution;
        if (s == the_names["isbn"]) return fp_isbn;
        if (s == the_names["issn"]) return fp_issn;
        if (s == the_names["isrn"]) return fp_isrn;
        if (s == the_names["journal"]) return fp_journal;
        if (s == the_names["cstb_key"]) return fp_key;
        if (s == the_names["month"]) return fp_month;
        if (s == the_names["langue"]) return fp_langue;
        if (s == the_names["cstb_language"]) return fp_langue;
        if (s == the_names["note"]) return fp_note;
        if (s == the_names["number"]) return fp_number;
        if (s == the_names["organization"]) return fp_organization;
        if (s == the_names["pages"]) return fp_pages;
        if (s == the_names["publisher"]) return fp_publisher;
        if (s == the_names["school"]) return fp_school;
        if (s == the_names["series"]) return fp_series;
        if (s == the_names["title"]) return fp_title;
        if (s == the_names["cstb_type"]) return fp_type;
        if (s == the_names["cstb_url"]) return fp_url;
        if (s == the_names["volume"]) return fp_volume;
        if (s == the_names["year"]) return fp_year;
        if (s == the_names["crossref"]) return fp_crossref;
        // Check is this is additional
        std::vector<std::string> &Bib = the_main.bibtex_fields;
        for (size_t i = 0; i < Bib.size(); i++)
            if (Bib[i] == s) return field_pos(fp_unknown + i + 1);
        return fp_unknown;
    }
} // namespace

// This takes a file name. It handles the case where the file has a suffix
// like miaou+foot. Prints a warning if this is a bad name.
void Bibtex::read_bib_file(const std::string &s) {
    if (auto of = find_in_path(s + (s.ends_with(".bib") ? "" : ".bib")); of) {
        spdlog::trace("Found BIB file: {}", *of);
        bbl.format("% reading source {}", *of);
        bbl.flush();
        in_lines.read(*of, 1);
        last_ok_line = 0;
        reset_input();
        try {
            while (parse_one_item()) {};
        } catch (Berror x) {} // \todo bad to stop on exception like this

        if (!bbl.empty()) {
            bbl.append("%");
            bbl.flush();
        }
    } else
        spdlog::warn("Bibtex Info: no biblio file {}", s);
}

auto Bibtex::look_for_macro(const std::string &name) -> std::optional<size_t> {
    for (size_t i = 0; i < all_macros.size(); i++)
        if (all_macros[i].is_same(name)) return i;
    return {};
}

// This looks for a macro named name or xname. If the macro is not found
// and insert is true, we construct a new macro. If xname is true, then
// the macro is initialised to val.
// [ if xname true, we define a system macro,
//   otherwise we define/redefine user one]
auto Bibtex::find_a_macro(Buffer &name, bool insert, String xname, String val) -> std::optional<size_t> {
    if (xname != nullptr) name = xname;
    auto lfm = look_for_macro(name);
    if (lfm || !insert) return lfm;
    auto res = all_macros.size();
    if (xname != nullptr)
        all_macros.emplace_back(xname, val);
    else
        all_macros.emplace_back(name);
    return res;
}

// This defines a system macro.
void Bibtex::define_a_macro(String name, String value) {
    Buffer B;
    find_a_macro(B, true, name, value);
}

// This finds a citation that matches exactly S
auto Bibtex::find_entry(const CitationKey &s) -> BibEntry * {
    for (auto &e : all_entries)
        if (e->cite_key.is_same(s)) return e;
    return nullptr;
}

// This finds a citation whose lowercase equivalent is S.
// Puts in N the number of citations found.
auto Bibtex::find_lower_case(const CitationKey &s, int &n) -> BibEntry * {
    n = 0;
    BibEntry *res{nullptr};
    for (auto &entry : all_entries)
        if (entry->cite_key.is_same_lower(s)) {
            res = entry;
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

auto Bibtex::exec_bibitem(const std::string &b) -> std::string {
    BibEntry *X = find_entry(b, because_all);
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

// Returns next line of the .bib file. Error if EOF and what.
// Throws if EOF.
bool Bibtex::next_line(bool what) {
    Buffer scratch;
    auto   n = in_lines.get_next(scratch);
    if (!n) {
        if (what) err_in_file("Illegal end of bibtex database file", true);
        return false;
    }
    cur_bib_line = *n;
    the_parser.set_cur_line(*n);
    inbuf.insert_without_crlf(scratch);
    input_line     = codepoints(inbuf);
    input_line_pos = 0;
    return true;
}

// Skip over a space. Error in case of EOF.
// This is the only function that reads a new line.
[[nodiscard]] bool Bibtex::skip_space() {
    for (;;) {
        if (at_eol()) {
            if (!next_line(true)) return false;
        } else {
            if (std::isspace(static_cast<int>(cur_char())) != 0)
                advance();
            else
                return true;
        }
    }
}

// Reads until the next @ sign. This is the only function
// that accepts to read an EOF without error.
[[nodiscard]] bool Bibtex::scan_for_at() {
    for (;;) {
        if (at_eol()) {
            if (!next_line(false)) return false;
        } else {
            char32_t c = next_char();
            if (c == '@') return true;
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
        if (!skip_space()) throw Berror();
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
    if ((c >= 128) || (std::isdigit(static_cast<int>(c)) != 0) || get_class(c) != legal_id_char) return wrong_first_char(c, what);
    for (;;) {
        if (at_eol()) break;
        c = next_char();
        if ((c >= 128) || get_class(c) != legal_id_char) {
            input_line_pos--; // c is to be read again
            break;
        }
        c = static_cast<char32_t>(std::tolower(static_cast<int>(c)));
        B.push_back(c);
    }
    // a field part.
    if (what == 0) return check_val_end();
    if (at_eol() || (std::isspace(static_cast<int>(c)) != 0)) {
        if (!skip_space()) throw Berror();
    }
    if (what == 1) return check_entry_end(); // case of @foo
    auto out = check_field_end(what);
    if (!out) throw Berror();
    return *out;
}

// A bunch of functions called when we see the end of an identifier.
// We start with a function that complains if first character is wrong.
auto Bibtex::wrong_first_char(char32_t c, size_t what) const -> int {
    err_in_file(scan_msgs[what], false);
    if (std::isdigit(static_cast<int>(c)) != 0)
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
    if (!skip_space()) throw Berror();
    return k;
}

// We have seen foo in foo=bar. We have skipped over spaces
// Returns 0 if OK. We try to read some characters on the current line
// in case of error
auto Bibtex::check_field_end(size_t what) -> std::optional<int> {
    if (cur_char() == '=') {
        advance();
        if (!skip_space()) return {};
        return 0;
    }
    err_in_file(scan_msgs[what], false);
    log_and_tty << "\nexpected `=' sign";
    for (;;) {
        if (at_eol()) return what == 2 ? 5 : -4;
        if (cur_char() == '=') {
            advance();
            if (!skip_space()) return {};
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
    if ((std::isspace(static_cast<int>(c)) != 0) || c == '#' || c == ',' || c == char32_t(right_outer_delim)) return 0;
    err_in_file(scan_msgs[0], false);
    log_and_tty << fmt::format("\nit cannot end with `{}'\n", to_utf8(c)) << "expecting `,', `#' or `" << right_outer_delim << "'";
    return 4;
}

// Reads someththing like foo=bar, and stores the result in X if ok
// The first field is preceded by the key and a comma.
// Thus, we start reading the comma (unless previous fiels has a syntax err).
// A null pointer means an entry to be discarded
[[nodiscard]] bool Bibtex::parse_one_field(BibEntry *X) {
    if (start_comma) {
        if (cur_char() != ',')
            err_in_file("expected comma before a field", true);
        else
            advance();
    }
    start_comma = true;
    if (!skip_space()) return false;
    if (cur_char() == char32_t(right_outer_delim)) return true;
    if (scan_identifier(3)) return true;
    field_pos where = fp_unknown;
    bool      store = false;
    if (X != nullptr) { // if X null, we just read, but store nothing
        cur_field_name = token_buf;
        where          = find_field_pos(token_buf);
        if (where != fp_unknown) store = true;
    }
    if (!read_field(store)) return false;
    if (!store) return true;
    bool ok = X->store_field(where);
    if (!ok) {
        err_in_file("", false);
        log_and_tty << "duplicate field `" << cur_field_name << "' ignored.\n";
        return true;
    }
    if (where != fp_crossref) return true;
    X->parse_crossref(); // special action
    return true;
}

// This parses an @something.
bool Bibtex::parse_one_item() {
    cur_entry_name = "";
    cur_entry_line = -1;
    if (!scan_for_at()) return false;
    last_ok_line = cur_bib_line;
    if (!skip_space()) return false;
    bool k = scan_identifier(1);
    if (k) return true;
    entry_type cn = find_type(token_buf);
    if (cn == type_comment) return true;
    if (cn == type_preamble) {
        if (!read_field(true)) return false;
        bbl.append(field_buf);
    } else if (cn == type_string) {
        k = scan_identifier(2);
        if (k) return true;
        auto X = *find_a_macro(token_buf, true, nullptr, nullptr);
        mac_def_val(X);
        if (!read_field(true)) return false;
        mac_set_val(X, field_buf.special_convert(false));
    } else {
        cur_entry_line = cur_bib_line;
        Buffer A;
        if (!skip_space()) return false;
        for (;;) {
            if (at_eol()) break;
            char32_t c = cur_char();
            if (c == ',' || (std::isspace(static_cast<int>(c)) != 0)) break;
            A << c;
            next_char();
        }
        if (!skip_space()) return false;
        cur_entry_name = A;
        BibEntry *X    = see_new_entry(cn, last_ok_line);
        int       m    = similar_entries;
        while (cur_char() != ')' && cur_char() != '}')
            if (!parse_one_field(X)) return false;
        if (m > 1) handle_multiple_entries(X);
    }
    char32_t c = cur_char();
    if (c == ')' || c == '}') advance();
    if (c != char32_t(right_outer_delim)) err_in_file("bad end delimiter", true);
    return true;
}

void Bibtex::handle_multiple_entries(BibEntry *Y) {
    CitationKey s = Y->cite_key;
    for (auto *entry : all_entries)
        if (entry->cite_key.is_similar(s)) {
            if (entry != Y) entry->copy_from(Y);
        }
}

// This finds entry named s, or case-equivalent. If create is true,
// creates entry if not found.
// Used by see_new_entry (create=auto_cite), or parse_crossref
// prefix can be from_year;
// If create is true, it is either a crossref, or \nocite{*}+from_year
// or or bug.

auto Bibtex::find_entry(const std::string &s, bool create, bib_creator bc) -> BibEntry * {
    CitationKey key(s);
    similar_entries = 1;
    int       n     = 0;
    BibEntry *X     = find_entry(key);
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
    BibEntry *X = find_entry(cur_entry_name, nocitestar, because_all);
    if (X == nullptr) return X;
    if (X->type_int != type_unknown) {
        err_in_file("duplicate entry ignored", true);
        return nullptr;
    }
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
[[nodiscard]] bool Bibtex::read_field(bool store) {
    field_buf.clear();
    for (;;) {
        if (!read_one_field(store)) return false;
        if (!skip_space()) return false;
        if (cur_char() != '#') return true;
        advance();
        if (!skip_space()) return false;
    }
}

// This reads a single field
[[nodiscard]] bool Bibtex::read_one_field(bool store) {
    char32_t c = cur_char();
    if (c == '{' || c == '\"') {
        uchar delimiter = c == '{' ? '}' : '\"';
        advance(); // reads left delimiter
        int bl = 0;
        for (;;) {
            if (at_eol()) {
                field_buf.push_back(' ');
                if (!next_line(true)) return false;
                continue;
            }
            c = cur_char();
            advance();
            if (c == '\\') {
                field_buf.push_back(c);
                if (at_eol()) {
                    field_buf.push_back(' ');
                    if (!next_line(true)) return false;
                    continue;
                }
                field_buf.push_back(cur_char());
                advance();
                continue;
            }
            if (c == delimiter && bl == 0) return true;
            if (c == '}' && bl == 0) {
                err_in_file("illegal closing brace", true);
                continue;
            }
            field_buf.push_back(c);
            if (c == '}') bl--;
            if (c == '{') bl++;
        }
    } else if (std::isdigit(static_cast<int>(c)) != 0) {
        for (;;) {
            if (at_eol()) return true;
            c = cur_char();
            if (std::isdigit(static_cast<int>(c)) == 0) return true;
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
    return true;
}

// This finds entry named s, or case-equivalent.
// creates entry if not found. This is used by exec_bibitem
auto Bibtex::find_entry(const std::string &s, bib_creator bc) -> BibEntry * {
    CitationKey key(s);
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
    if (all_entries.empty()) return;
    if (!bbl.empty()) bbl.flush();
    all_entries_table.reserve(all_entries.size());
    for (auto &all_entrie : all_entries) all_entrie->un_crossref();
    for (size_t i = 0; i < all_entries.size(); i++) all_entries[i]->work(to_signed(i));
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

void Bibtex::boot(std::string S) {
    no_year      = std::move(S);
    want_numeric = false;
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
    if (cur_lang_fr()) { // french
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
    } else if (cur_lang_german()) { //	      german
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
