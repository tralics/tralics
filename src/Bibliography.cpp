#include "tralics/Bibliography.h"
#include "tralics/Bbl.h"
#include "tralics/Bibtex.h"
#include "tralics/globals.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// This creates a unique ID, named bid1, bid2, etc.
auto Bibliography::unique_bid() -> std::string { return std::string(fmt::format("bid{}", ++last_bid)); }

// This creates the full aux file, for use with bibtex.
void Bibliography::dump(Buffer &b) {
    if (seen_nocite()) b.append("\\citation{*}\n");
    size_t n = citation_table.size();
    for (size_t i = 0; i < n; i++) citation_table[i].dump(b);
}

// This dumps the whole biblio for use by Tralics.
void Bibliography::dump_bibtex() {
    if (seen_nocite()) the_bibtex.nocitestar_true();
    size_t n = citation_table.size();
    if (n != 0) bbl.open();
    for (size_t i = 0; i < n; i++) citation_table[i].dump_bibtex();
    n = biblio_src.size();
    if (n > 0) {
        bbl.open();
        for (size_t i = 0; i < n; i++) the_bibtex.read1(biblio_src[i]);
    } else
        the_bibtex.read_ra();
}

void Bibliography::stats() {
    auto solved = std::count_if(citation_table.begin(), citation_table.end(), [](auto &c) { return c.is_solved(); });
    spdlog::trace("Bib stats: seen {}{} entries.", citation_table.size(), solved > 0 ? fmt::format("({})", solved) : "");
}

// This dumps the whole biblio for use by bibtex.
void Bibliography::dump_data(Buffer &b) {
    auto n = biblio_src.size();
    b.format("\\bibstyle{{{}}}\n", bib_style);
    b.append("\\bibdata{");
    if (n == 0) b.append(file_name);
    for (size_t i = 0; i < n; i++) {
        if (i > 0) b.append(",");
        b.append(biblio_src[i]);
    }
    b.append("}\n");
}

// This finds a citation in the table. In the case \footcite{Kunth},
// the first two arguments are the Istrings associated to foot and Knuth.
// If not found, we may insert a new item (normal case),
// or return -1 (in case of failure)
auto Bibliography::find_citation_item(const std::string &from, const std::string &key, bool insert) -> std::optional<size_t> {
    auto n = citation_table.size();
    for (size_t i = 0; i < n; i++)
        if (citation_table[i].match(key, from)) return i;
    if (!insert) return {};
    citation_table.emplace_back(key, from);
    return n;
}

// This is like the function above. In the case \footcite{Kunth},
// the two arguments are the Istrings associated to foot and Knuth.
// If not found, we try \cite[?]{Kunth}, using any possibility for the first
// argument (this matches only unsolved references). In case of failure
// a new entry is added, of type FROM.
auto Bibliography::find_citation_star(const std::string &from, const std::string &key) -> size_t {
    if (auto n = find_citation_item(from, key, false)) return *n;
    auto n = citation_table.size();
    for (size_t i = 0; i < n; i++)
        if (citation_table[i].match_star(key)) return i;
    citation_table.emplace_back(key, from);
    return n;
}
