#pragma once
#include "CitationKey.h"
#include "MainClass.h"
#include <array>
#include <vector>

// Consider for instance the name list :
// Carrasco, J. and MahEEvas, S. and Rubino, G. and Su\~nEE, V.
// (where EE stands for \'e on 8 bits)
// value will be \bpers{}\bpers{}\bpers{}\bpers (concatenation of 4 value)
// long_key will be the concatenation of 4?
// short_key will be CMRS
struct BibtexName {
    std::string value, long_key, short_key, name_key;
};

// A bibliographic entry TODO: derive from CitationKey?
class BibEntry {
    friend class Bibtex;

    BibEntry                           *crossref{nullptr};      // In case of a crossref
    BibEntry                           *crossref_from{nullptr}; // reverse crossref
    entry_type                          type_int{type_unknown}; // the type of the entry
    CitationKey                         cite_key;               // the cite_key structure
    bib_creator                         why_me{};               // reason why this entry is considered
    std::array<std::string, fp_unknown> all_fields;             // all the fields
    BibtexName                          author_data;            // already processed author data
    BibtexName                          editor_data;            // already processed editor data

    int                      extra_num{0};     // extra char added to the label
    int                      cur_year{0};      // current year, if year field can be parsed
    std::string              lab1, lab2, lab3; // two temporaries.
    size_t                   id{0};
    bool                     explicit_cit{false};
    c_primaire               main_c_type{};
    c_secondaire             second_c_type{};
    int                      first_line{-1};
    std::vector<std::string> user_fields;
    size_t                   is_extension{};

public:
    std::string label, sort_label, aux_label; // cite label and sort label
    std::string unique_id;

    BibEntry() : user_fields(the_main.bibtex_fields.size()) {}

private:
    void out_something(field_pos p);
    void out_something(field_pos p, size_t w);
    void set_explicit_cit() { explicit_cit = true; }
    void use_extra_num();
    void numeric_label(size_t i);
    void call_type();
    void call_type_all();
    void call_type_special();
    void format_series_etc(bool pa);
    void sort_author(bool au);
    void presort(long serial);
    void format_author(bool au);
    void forward_pass(std::string &previous_label, int &last_num);
    void reverse_pass(int &next_extra);
    void normalise();
    void un_crossref();
    void copy_from(BibEntry *Y);
    void copy_from(BibEntry *Y, size_t k);
    auto store_field(field_pos where) -> bool;
    void parse_crossref();
    void work(long serial);
};
