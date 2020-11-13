#include "tralics/BibEntry.h"
#include "tralics/Bbl.h"
#include "tralics/Bchar.h"
#include "tralics/Bibtex.h"
#include "tralics/Logger.h"
#include "tralics/NameMapper.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/ostream.h>

namespace {
    Buffer biblio_buf1, biblio_buf2, biblio_buf3, biblio_buf4;

    auto want_handle_key(int s, bool last) -> bool {
        if (s < 4) return true;
        if (s > 4) return false;
        if (last) return true;
        biblio_buf3.append("+");
        return false;
    }

    class NameSplitter {
        bchar_type *table;
        Bchar       first_name{};
        Bchar       last_name{};
        Bchar       jr_name{};
        Bchar       main_data{};

    public:
        NameSplitter(bchar_type *T) : table(T) {}

        void handle_the_names() {
            bool   is_first_name = true;
            int    serial        = 1;
            size_t pos           = 1; // there is a space at position 0
            main_data.init(table);
            for (;;) {
                name_buffer.ptrs.b = pos;
                bool is_last_name  = name_buffer.find_and(table);
                auto k             = name_buffer.ptrs.b;
                main_data.init(pos, k);
                handle_one_name(is_first_name, is_last_name, serial);
                if (is_last_name) return;
                is_first_name = false;
                serial++;
                pos = k + 3;
            }
        };

        void handle_one_name(bool ifn, bool iln, int serial) {
            first_name.init(table);
            last_name.init(table);
            jr_name.init(table);
            main_data.remove_junk();
            auto   F  = main_data.first;
            auto   L  = main_data.last;
            size_t fc = 0, lc = 0, hm = 0;
            main_data.find_a_comma(fc, lc, hm);
            if (hm >= 2) {
                last_name.init(F, fc);
                jr_name.init(fc + 1, lc);
                first_name.init(lc + 1, L);
                if (hm > 2) {
                    Bibtex::err_in_entry("");
                    log_and_tty << "too many commas (namely " << hm << ") in name\n" << name_buffer << ".\n";
                }
            } else if (hm == 1) {
                first_name.init(fc + 1, L);
                last_name.init(F, fc);
            } else { // no comma
                auto k = main_data.find_a_lower();
                if (k == L) {
                    k = main_data.find_a_space();
                    if (k == L) {
                        main_data.invent_spaces();
                        k = main_data.find_a_space();
                    }
                }
                if (k == L)
                    last_name.init(F, L);
                else {
                    first_name.init(F, k);
                    last_name.init(k + 1, L);
                }
            }
            first_name.remove_junk();
            last_name.remove_junk();
            jr_name.remove_junk();
            if (first_name.empty() && last_name.empty() && jr_name.empty()) {
                Bibtex::err_in_entry("empty name in\n");
                log_and_tty << name_buffer << ".\n";
                return;
            }
            bool handle_key = want_handle_key(serial, iln);
            bool is_other   = is_this_other();
            if (is_other) {
                if (iln && !ifn) {
                    biblio_buf1.append("\\cititem{etal}{}");
                    biblio_buf2.append("etal");
                    biblio_buf4.append("etal");
                    if (handle_key) biblio_buf3.append("+");
                } else {
                    biblio_buf1.append("\\bpers{}{}{others}{}");
                    biblio_buf2.append("others");
                    biblio_buf4.append("others");
                    if (handle_key) biblio_buf3.append(iln ? "oth" : "o");
                }
                return;
            }
            if (handle_key) last_name.make_key(!(ifn && iln), biblio_buf3);
            biblio_buf1.format("\\bpers[{}]{{", first_name);
            biblio_buf2.append(" ");
            first_name.print_first_name(biblio_buf1, biblio_buf2);
            biblio_buf1.format("}}{{}}{{{}}}{{{}}}", last_name, jr_name);
            biblio_buf2.format(" {} {} ", last_name, jr_name);
            biblio_buf4.format("{} {} ", last_name, jr_name);
        };

        auto is_this_other() -> bool {
            if (!first_name.empty()) return false;
            if (!jr_name.empty()) return false;
            auto a = last_name.first;
            auto b = last_name.last;
            return b - a == 6 && name_buffer.substr(a, 6) == "others";
        };
    };

    std::array<std::string, 30> bib_xml_name{"bcrossref", "bkey",     "baddress",      "bauthors",     "bbooktitle",   "bchapter",
                                             "bedition",  "beditors", "bhowpublished", "binstitution", "bjournal",     "bmonth",
                                             "bnote",     "bnumber",  "borganization", "bpages",       "bpublisher",   "bschool",
                                             "bseries",   "btitle",   "btype",         "burl",         "bvolume",      "byear",
                                             "bdoi",      "bsubtype", "bunite",        "bequipe",      "bidentifiant", "bunknown"};

    // In the case of `Lo{\"i}c', returns  `Lo{\"i}'.
    auto first_three(const std::string &s) -> std::string {
        Buffer B = s;
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == 0) return s;
        B.resize(B.ptrs.b);
        return std::move(B);
    }

    // In the case of `Lo{\"i}c', returns  `{\"i}c' for k=2.
    // In the case of `Lo\"i c', returns the whole string.
    auto last_chars(const std::string &s, int k) -> std::string {
        Buffer B;
        B.clear();
        B.append(s);
        B.ptrs.b = 0;
        int n    = -k;
        while (B.head() != 0) {
            n++;
            B.next_bibtex_char();
        }
        if (n <= 0) return s;
        B.ptrs.b = 0;
        while (n > 0) {
            n--;
            B.next_bibtex_char();
        }
        return B.substr(B.ptrs.b);
    }

    // Converts cite:foo into foo, with some heuristic tests.
    auto skip_dp(const std::string &str) -> std::string {
        size_t i = 0;
        while (i < str.size() && str[i] != ':') ++i;
        if (i + 1 < str.size()) return str.substr(i + 1);
        return str;
    }

    // Dual function. Returns the name of the thing.
    auto type_to_string(entry_type x) -> std::string {
        switch (x) {
        case type_article: return "article";
        case type_book: return "book";
        case type_booklet: return "booklet";
        case type_conference: return "conference";
        case type_coursenotes: return "coursenotes";
        case type_inbook: return "inbook";
        case type_incollection: return "incollection";
        case type_inproceedings: return "inproceedings";
        case type_manual: return "manual";
        case type_masterthesis: return "mastersthesis";
        case type_misc: return "misc";
        case type_phdthesis: return "phdthesis";
        case type_proceedings: return "proceedings";
        case type_techreport: return "techreport";
        case type_unpublished: return "unpublished";
        default: return "cstb_unknown";
        }
    }

    auto remove_space(const std::string &s) -> std::string {
        std::string res;
        for (auto c : s)
            if (c != ' ') res.push_back(c);
        return res;
    }

    auto insert_break(const std::string &x) -> std::string {
        std::string res = "{\\url{";
        for (auto c : x) {
            if (c == ' ' && bib_allow_break) res.append("\\allowbreak");
            res.push_back(c);
        }
        res.append("}}");
        return res;
    }

    void handle_one_namelist(std::string &src, BibtexName &X) {
        if (src.empty()) return;
        biblio_buf1.clear();
        biblio_buf2.clear();
        biblio_buf3.clear();
        biblio_buf4.clear();
        name_buffer.normalise_for_bibtex(src.c_str());
        auto                    n = name_buffer.size() + 1;
        std::vector<bchar_type> table(n);
        NameSplitter            W(table.data());
        name_buffer.fill_table(table);
        W.handle_the_names();
        X.value     = biblio_buf1;
        X.long_key  = biblio_buf2;
        X.short_key = biblio_buf3;
        X.name_key  = biblio_buf4;
    }

    void out_something_s(field_pos p, const std::string &s) {
        bbl.format("\\cititem{{{}}}{{{}}}", bib_xml_name[p], s);
        bbl.flush();
    }
} // namespace

// This is non trivial, because we have a fixed-sized array and a varying size
// return true if the field is not already filled.
auto BibEntry::store_field(field_pos where) -> bool {
    if (where < fp_unknown) {
        if (all_fields[where].empty()) {
            all_fields[where] = field_buf.special_convert(true);
            return true;
        }
        return false;
    }
    auto k = size_t(where - fp_unknown - 1);
    if (user_fields[k].empty()) {
        user_fields[k] = field_buf.special_convert(true);
        return true;
    }
    return false;
}

// When we see crossref=y, we expect to see entry Y later
// We make Y to be cited, and link X to Y and Y to X.

void BibEntry::parse_crossref() {
    const auto &name = all_fields[fp_crossref];
    if (name[0] == 0) return;
    bib_creator bc = the_bibtex.nocitestar ? because_all : because_crossref;
    BibEntry *  Y  = the_bibtex.find_entry(name, true, bc);
    if (this == Y) return; /// should not happen
    crossref = Y;
    if (Y->crossref_from == nullptr) Y->crossref_from = this;
}

// Assume that X has a crossref to Y. Then all empty fields will be
// replaced by the fields of Y, except crossref and key.
void BibEntry::un_crossref() {
    if (crossref == nullptr) return;
    copy_from(crossref, 2);
}

void BibEntry::copy_from(BibEntry *Y) {
    if (type_int != type_unknown) {
        the_bibtex.err_in_file("duplicate entry ignored", true);
        return;
    }
    the_log << "Copy Entry " << Y->cite_key.full_key << " into " << cite_key.full_key << "\n";
    is_extension = Y->is_extension;
    type_int     = Y->type_int;
    first_line   = Y->first_line;
    copy_from(Y, 0);
}

void BibEntry::copy_from(BibEntry *Y, size_t k) {
    if (Y->type_int == type_unknown) {
        log_and_tty << "Unknown reference in crossref " << Y->cite_key.full_key << "\n";
        return; // Should signal an error
    }
    for (size_t i = k; i < all_fields.size(); i++) {
        if (all_fields[i].empty()) all_fields[i] = Y->all_fields[i];
    }
    auto n = the_main.bibtex_fields.size();
    for (size_t i = 0; i < n; i++)
        if (user_fields[i].empty()) user_fields[i] = Y->user_fields[i];
}

// Some entries are discarded, others are pushed in all_entries_table.
// For these we call normalise and presort
void BibEntry::work(long serial) {
    cur_entry_line = -1;
    cur_entry_name = cite_key.full_key;
    if (type_int == type_unknown) {
        Bibtex::err_in_entry("undefined reference.\n");
        if (crossref_from != nullptr) log_and_tty << "This entry was crossref'd from " << crossref_from->cite_key.full_key << "\n";
        return;
    }
    if (explicit_cit) return;
    if (why_me == because_crossref) return;
    the_bibtex.enter_in_table(this);
    cur_entry_line = first_line;
    normalise();
    presort(serial);
}

// This computes the extra_num field. It's k if this is the k-th entry
// with the same label (0 for the first entry).
void BibEntry::forward_pass(std::string &previous_label, int &last_num) {
    if (label == previous_label) {
        last_num++;
        extra_num = last_num;
    } else {
        last_num       = 0;
        previous_label = label;
        extra_num      = 0;
    }
}

// Uses the extra-num value : 0 gives a, 1 gives b, etc.
// -1 gives nothing.
void BibEntry::use_extra_num() {
    if (extra_num == -1) return;
    Buffer &B = biblio_buf1;
    B         = label;
    if (extra_num <= 25)
        B << char('a' + extra_num);
    else
        B.format("{}", extra_num);
    label = B;
}

// This must be done after the forward pass. We know if an extra num of 0
// must be converted to -1. We can compute the full label.
void BibEntry::reverse_pass(int &next_extra) {
    if (extra_num == 0 && next_extra != 1) {
        extra_num  = -1;
        next_extra = 0;
    } else {
        next_extra = extra_num;
        use_extra_num();
    }
}

// Creates a numeric version of the label, and (optional) alpha one.
void BibEntry::numeric_label(size_t i) {
    aux_label = fmt::format("[{}]", label);
    label     = fmt::format("{}", i);
}

// -----------------------------------------------------------------------
// printing the bbl.

// output a generic field as \cititem{k}{value}
// If no value, and w>0, a default value will be used.
void BibEntry::out_something(field_pos p, size_t w) {
    std::string s = all_fields[p];
    if (s.empty()) s = my_constant_table[w - 1];
    out_something_s(p, s);
}

void BibEntry::out_something(field_pos p) {
    std::string s = all_fields[p];
    if (s.empty()) return;
    out_something_s(p, s);
}

// Outputs a part of the thing.
void BibEntry::format_series_etc(bool pa) {
    out_something(fp_series);
    out_something(fp_number);
    out_something(fp_volume);
    if (pa) {
        out_something(fp_publisher);
        out_something(fp_address);
    }
}

// Outputs author or editor.
void BibEntry::format_author(bool au) {
    field_pos p = au ? fp_author : fp_editor;
    if (all_fields[p].empty()) return;
    std::string data = au ? author_data.value : editor_data.value;
    bbl.append("\\" + bib_xml_name[p]);
    bbl.format("{{{}}}", data);
    bbl.flush();
}

void BibEntry::call_type() {
    bbl.clear();
    bbl.append("%");
    bbl.flush();
    //  bbl.push_back("%%%");bbl.push_back(sort_label); bbl.newline();
    bbl.append("\\citation");
    bbl.format("{{{}}}", label);
    bbl.format("{{{}}}", cite_key.full_key);
    bbl.format("{{{}}}", unique_id);
    bbl.format("{{{}}}", "year"); // \todo [[deprecated]]
    const auto &my_name = (is_extension > 0) ? the_main.bibtex_extensions[is_extension - 1] : the_names[type_to_string(type_int)];
    bbl.format("{{{}}}", my_name);
    bbl.append(aux_label);
    bbl.flush();
    if (type_int == type_extension || raw_bib)
        call_type_all();
    else
        call_type_special();

    out_something(fp_doi);
    std::string s = all_fields[fp_url];
    if (!s.empty()) {
        bbl.append("\\csname @href\\endcsname");
        //    string S = hack_bib_space(s);
        bbl.format("{{{}}}", remove_space(s));
        bbl.append(insert_break(s));
        bbl.flush();
    }
    std::vector<std::string> &Bib = the_main.bibtex_fields;
    for (size_t i = 0; i < Bib.size(); i++) {
        auto ss = user_fields[i];
        if (!ss.empty()) {
            bbl.append("\\cititem");
            bbl.format("{{{}}}", Bib[i]);
            bbl.format("{{{}}}", ss);
            bbl.flush();
        }
    }
    out_something(fp_note);
    bbl.append("\\endcitation");
    bbl.flush();
}

void BibEntry::call_type_all() {
    format_author(true);
    out_something(fp_title);
    format_author(false);
    out_something(fp_journal);
    out_something(fp_edition);
    format_series_etc(true);
    out_something(fp_howpublished);
    out_something(fp_booktitle);
    out_something(fp_organization);
    out_something(fp_pages);
    out_something(fp_chapter);
    if (type_int == type_masterthesis)
        out_something(fp_type, 3);
    else if (type_int == type_phdthesis)
        out_something(fp_type, 1);
    else if (type_int == type_techreport)
        out_something(fp_type, 2);
    else
        out_something(fp_type);
    out_something(fp_school);
    out_something(fp_institution);
    out_something(fp_month);
    out_something(fp_year);
}

void BibEntry::call_type_special() {
    if (type_int != type_proceedings) format_author(true);
    if (type_int == type_book || type_int == type_inbook) format_author(false);
    out_something(fp_title);
    if (type_int == type_proceedings || type_int == type_incollection) format_author(false);
    switch (type_int) {
    case type_article:
        out_something(fp_journal);
        out_something(fp_number);
        out_something(fp_volume);
        break;
    case type_book:
    case type_inbook:
        out_something(fp_edition);
        format_series_etc(true);
        break;
    case type_booklet:
        out_something(fp_howpublished);
        out_something(fp_address);
        break;
    case type_incollection:
        out_something(fp_booktitle);
        format_series_etc(true);
        break;
    case type_inproceedings:
    case type_conference:
        out_something(fp_booktitle);
        format_series_etc(false);
        out_something(fp_organization);
        out_something(fp_publisher);
        format_author(false);
        out_something(fp_pages);
        out_something(fp_address);
        break;
    case type_manual:
        out_something(fp_organization);
        out_something(fp_edition);
        out_something(fp_address);
        break;
    case type_masterthesis:
    case type_coursenotes:
    case type_phdthesis:
        if (type_int == type_coursenotes)
            out_something(fp_type);
        else if (type_int == type_phdthesis)
            out_something(fp_type, 1);
        else
            out_something(fp_type, 3);
        out_something(fp_school);
        out_something(fp_address);
        break;
    case type_misc:
        out_something(fp_howpublished);
        format_author(false);
        out_something(fp_booktitle);
        format_series_etc(true);
        break;
    case type_proceedings:
        out_something(fp_organization);
        format_series_etc(true);
        break;
    case type_techreport:
        out_something(fp_type, 2);
        out_something(fp_number);
        out_something(fp_institution);
        out_something(fp_address);
        break;
    default:;
    }
    out_something(fp_month);
    out_something(fp_year);
    if (type_int == type_inbook || type_int == type_incollection) out_something(fp_chapter);
    switch (type_int) {
    case type_inproceedings:
    case type_conference: break;
    default: out_something(fp_pages);
    }
}

// Handle author or editor (depending on au), for sort key.
void BibEntry::sort_author(bool au) {
    if (!lab1.empty()) return;
    if (au && !all_fields[fp_author].empty()) {
        lab1 = author_data.short_key;
        lab2 = author_data.long_key;
        lab3 = author_data.name_key;
    }
    if (!au && !all_fields[fp_editor].empty()) {
        lab1 = editor_data.short_key;
        lab2 = editor_data.long_key;
        lab3 = editor_data.name_key;
    }
}

void BibEntry::presort(long serial) {
    Buffer &B = biblio_buf1;
    sort_author(type_int != type_proceedings);
    sort_author(type_int == type_proceedings);
    if (lab1.empty()) {
        std::string s = all_fields[fp_key];
        if (s.empty()) s = skip_dp(cite_key.full_key);
        lab1 = first_three(s);
        lab2 = s;
        lab3 = s;
    }
    std::string y = all_fields[fp_year];
    {
        std::string s = last_chars(y, 2);
        if (!s.empty()) {
            if (std::isdigit(s[0]) == 0) s = "";
        }
        label = lab1 + s;
    }
    B.clear();
    B.format("{}{}    {}    ", label, lab2, y);
    B.special_title(all_fields[fp_title]);
    B.lowercase();
    B.format("{:05d}", serial);
    sort_label = B;
}

void BibEntry::normalise() {
    handle_one_namelist(all_fields[fp_author], author_data);
    handle_one_namelist(all_fields[fp_editor], editor_data);
    std::string y = all_fields[fp_year];
    auto        n = y.size();
    if (y.empty()) return;
    cur_year = -1;
    int res  = 0;
    for (char c : y) {
        if (std::isdigit(c) == 0) return;
        res = res * 10 + (c - '0');
        if (res > 10000) return;
    }
    if (n == 2) {
        if (res > 50) {
            res += 1900;
            all_fields[fp_year] = "19" + y;
        } else {
            res += 2000;
            all_fields[fp_year] = "20" + y;
        }
    }
    cur_year = res;
}
