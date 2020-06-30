#include "tralics/CitationKey.h"
#include "tralics/Buffer.h"
#include "tralics/globals.h"

// Ctor via "foot" and "Knuth"
CitationKey::CitationKey(const std::string &a, const std::string &b) {
    if (a == "foot")
        cite_prefix = from_foot;
    else if (a == "refer")
        cite_prefix = from_refer;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Ctor via from_foot and "Knuth".
CitationKey::CitationKey(bib_from a, const std::string &b) {
    if (a == from_foot || a == from_refer)
        cite_prefix = a;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Common code of the Ctor. Argument is "Knuth", cite_prefix is OK.
void CitationKey::make_key(const std::string &s) {
    if (!distinguish_refer && cite_prefix == from_refer) cite_prefix = from_year;
    Buffer &B = biblio_buf2;
    B.clear();
    B.append(s);
    B.lowercase();
    cite_key       = s;
    lower_cite_key = B;
    B.clear();
    if (cite_prefix == from_foot)
        B.append("foot");
    else if (cite_prefix == from_refer && !old_ra)
        B.append("refer");
    B.append("cite:");
    B.append(s);
    full_key = B;
}

auto CitationKey::is_same_lower_old(const CitationKey &w) const -> bool {
    if (!is_similar_lower(w)) return false;
    if (cite_prefix == w.cite_prefix) return true;
    if (cite_prefix == from_foot || w.cite_prefix == from_foot) return false;
    return true;
}

auto CitationKey::from_to_string() const -> std::string {
    if (cite_prefix == from_year) return "year";
    if (cite_prefix == from_refer) return "refer";
    return "foot";
}
