#include "tralics/BibEntry.h"
#include "tralics/Bbl.h"
#include "tralics/Bibtex.h"
#include "tralics/Logger.h"
#include "tralics/NameMapper.h"
#include "tralics/NameSplitter.h"
#include "tralics/globals.h"

namespace {
    std::array<std::string, 30> bib_xml_name{"bcrossref", "bkey",     "baddress",      "bauthors",     "bbooktitle",   "bchapter",
                                             "bedition",  "beditors", "bhowpublished", "binstitution", "bjournal",     "bmonth",
                                             "bnote",     "bnumber",  "borganization", "bpages",       "bpublisher",   "bschool",
                                             "bseries",   "btitle",   "btype",         "burl",         "bvolume",      "byear",
                                             "bdoi",      "bsubtype", "bunite",        "bequipe",      "bidentifiant", "bunknown"};

    // In the case of `Lo{\"i}c', returns  `Lo{\"i}'.
    auto first_three(const std::string &s) -> std::string {
        Buffer &B = biblio_buf1;
        B.clear();
        B.push_back(s);
        B.ptrs.b = 0;
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == '\\') return s;
        B.next_bibtex_char();
        if (B.head() == 0) return s;
        B.resize(B.ptrs.b);
        return B;
    }

    // In the case of `Lo{\"i}c', returns  `{\"i}c' for k=2.
    // In the case of `Lo\"i c', returns the whole string.
    auto last_chars(const std::string &s, int k) -> std::string {
        Buffer B;
        B.clear();
        B.push_back(s);
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
} // namespace

// This returns a prefix from ra_pretable, according to from and type_int.
auto BibEntry::ra_prefix() const -> String {
    if (get_cite_prefix() == from_refer) return ra_pretable[0];
    if (get_cite_prefix() == from_foot) return ra_pretable[1];
    switch (type_int) {
    case type_book:
    case type_booklet:
    case type_proceedings: return ra_pretable[2];
    case type_phdthesis: return ra_pretable[3];
    case type_article:
    case type_inbook:
    case type_incollection: return ra_pretable[4];
    case type_conference:
    case type_inproceedings: return ra_pretable[5];
    case type_manual:
    case type_techreport:
    case type_coursenotes: return ra_pretable[6];
    case type_masterthesis:
    case type_misc:
    case type_unpublished:
    default: return ra_pretable[7];
    }
}

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
    bib_creator bc = the_bibtex.auto_cite() ? because_all : because_crossref;
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
    for (size_t i = k; i < fp_unknown; i++) {
        if (all_fields[i].empty()) all_fields[i] = Y->all_fields[i];
    }
    auto n = the_main->bibtex_fields.size();
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
    B << bf_reset << label;
    if (extra_num <= 25)
        B << char('a' + extra_num);
    else
        B << fmt::format("{}", extra_num);
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
void BibEntry::numeric_label(long i) {
    Buffer &B = biblio_buf1;
    B << bf_reset << '[' << label << ']';
    aux_label = B;
    B << bf_reset << std::to_string(i);
    label = B;
}

// -----------------------------------------------------------------------
// printing the bbl.

void BibEntry::out_something(field_pos p, const std::string &s) {
    bbl.push_back("\\cititem");
    bbl.push_back_braced(bib_xml_name[p]);
    bbl.push_back_braced(s);
    bbl.newline();
}

// output a generic field as \cititem{k}{value}
// If no value, and w>0, a default value will be used.
void BibEntry::out_something(field_pos p, size_t w) {
    std::string s = all_fields[p];
    if (s.empty()) s = my_constant_table[w - 1];
    out_something(p, s);
}

void BibEntry::out_something(field_pos p) {
    std::string s = all_fields[p];
    if (s.empty()) return;
    out_something(p, s);
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
    bbl.push_back("\\" + bib_xml_name[p]);
    bbl.push_back_braced(data);
    bbl.newline();
}

void BibEntry::call_type() {
    bbl.reset();
    bbl.push_back("%");
    bbl.newline();
    //  bbl.push_back("%%%");bbl.push_back(sort_label); bbl.newline();
    bbl.push_back("\\citation");
    bbl.push_back_braced(label);
    bbl.push_back_braced(cite_key.full_key);
    bbl.push_back_braced(unique_id.name);
    bbl.push_back_braced(from_to_string());
    auto my_name = (is_extension > 0) ? the_main->bibtex_extensions[is_extension - 1].name : the_names[type_to_string(type_int)].name;
    bbl.push_back_braced(my_name);
    bbl.push_back(aux_label);
    bbl.newline();
    if (type_int == type_extension || raw_bib)
        call_type_all();
    else
        call_type_special();

    out_something(fp_doi);
    std::string s = all_fields[fp_url];
    if (!s.empty()) {
        Buffer &B = biblio_buf1;
        if (s.starts_with("\\rrrt"))
            bbl.push_back(s);
        else {
            bbl.push_back("\\csname @href\\endcsname");
            //    string S = hack_bib_space(s);
            bbl.push_back_braced(B.remove_space(s));
            bbl.push_back(B.insert_break(s));
        }
        bbl.newline();
    }
    std::vector<Istring> &Bib        = the_main->bibtex_fields;
    auto                  additional = Bib.size();
    for (size_t i = 0; i < additional; i++) {
        auto ss = user_fields[i];
        if (!ss.empty()) {
            bbl.push_back("\\cititem");
            bbl.push_back_braced(Bib[i].name);
            bbl.push_back_braced(ss);
            bbl.newline();
        }
    }
    out_something(fp_note);
    bbl.push_back("\\endcitation");
    bbl.newline();
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
            if (!is_digit(s[0])) s = "";
        }
        label = lab1 + s;
    }
    B.clear();
    if (the_main->handling_ra) B << ra_prefix() << lab3;
    B << label << lab2 << "    " << y << "    ";
    B.special_title(all_fields[fp_title]);
    B.lowercase();
    B << fmt::format("{:05d}", serial);
    sort_label = B;
}

void BibEntry::handle_one_namelist(std::string &src, BibtexName &X) {
    if (src.empty()) return;
    biblio_buf1.clear();
    biblio_buf2.clear();
    biblio_buf3.clear();
    biblio_buf4.clear();
    name_buffer.normalise_for_bibtex(src.c_str());
    auto         n     = name_buffer.size() + 1;
    auto *       table = new bchar_type[n];
    NameSplitter W(table);
    name_buffer.fill_table(table);
    W.handle_the_names();
    delete[] table;
    X.value     = biblio_buf1;
    X.long_key  = biblio_buf2;
    X.name_key  = biblio_buf4;
    X.short_key = biblio_buf3;
}

void BibEntry::normalise() {
    handle_one_namelist(all_fields[fp_author], author_data);
    handle_one_namelist(all_fields[fp_editor], editor_data);
    std::string y = all_fields[fp_year];
    auto        n = y.size();
    if (n == 0) return;
    cur_year = -1;
    int res  = 0;
    for (size_t i = 0; i < n; i++) {
        char c = y[i];
        if (!is_digit(c)) return;
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
