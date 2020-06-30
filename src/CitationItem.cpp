#include "tralics/CitationItem.h"
#include "tralics/BibEntry.h"
#include "tralics/Bibliography.h"
#include "tralics/Bibtex.h"
#include "tralics/Parser.h"

// This returns a bid. It may create one.
auto CitationItem::get_id() -> std::string {
    if (id.empty()) id = the_bibliography.unique_bid();
    return id;
}

// This prints an unsolved reference in a buffer, we will put it in
// the aux file.
void CitationItem::dump(Buffer &b) const {
    if (!is_solved()) b.format("\\citation{{{}}}\n", encode(key));
}

// This prints an unsolved reference for use by Tralics.
void CitationItem::dump_bibtex() {
    if (is_solved()) return;
    CitationKey ref(from, key);
    BibEntry *  X = the_bibtex.find_entry(ref);
    if (X != nullptr) {
        err_buf = "Conflicts with tralics bib" + ref.full_key;
        the_parser.signal_error(the_parser.err_tok, "bib");
        return;
    }
    the_bibtex.make_entry(ref, get_id());
}

// Returns true if this is the same object.
// returns false for \cite{Knuth} and \footcite{Knuth}
auto CitationItem::match(const std::string &A, const std::string &B) const -> bool { return key == A && from == B; }

// Case of solve{?}{Knuth}. We return true if the key is Knuth, whatever the
// from field, but only if the entry is unsolved.
auto CitationItem::match_star(const std::string &A) const -> bool { return key == A && !is_solved(); }
