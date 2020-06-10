#pragma once
#include "CitationItem.h"

class Bibliography {
public:
    std::string               bib_style{"plain"}; // The bibliography style
    std::string               cmd;                // The bibliography command
    std::vector<CitationItem> citation_table;
    Xml *                     location{nullptr}; // location of biblio in the XML tree

private:
    std::vector<std::string> biblio_src;              // database file names
    bool                     nocite{false};           // have  we seen \nocite* ?
    bool                     biblio_loc_force{false}; // force location
    int                      last_bid{-1};            // current number for unique_bid

public:
    void               dump(Buffer &b);
    void               dump_bibtex();
    void               dump_data(Buffer &b);
    auto               get_bid(size_t n) { return citation_table[n].get_id(); }
    auto               find_citation_item(const Istring &from, const Istring &key, bool insert) -> std::optional<size_t>;
    auto               find_citation_star(const Istring &from, const Istring &key) -> size_t;
    [[nodiscard]] auto has_cmd() const -> bool { return !cmd.empty(); }
    [[nodiscard]] auto location_exists() const -> bool { return biblio_loc_force; }
    auto               number_of_data_bases() { return biblio_src.size(); }
    void               push_back_src(const std::string &x) { biblio_src.emplace_back(x); }
    [[nodiscard]] auto seen_nocite() const -> bool { return nocite; }
    void               set_cmd(std::string x) { cmd = std::move(x); }
    void               set_location(Xml *X, bool f) {
        location         = X;
        biblio_loc_force = f;
    }
    void set_nocite() { nocite = true; }
    void set_style(std::string x) { bib_style = std::move(x); }
    void stats();
    auto unique_bid() -> Istring;
};

inline Bibliography the_bibliography;
