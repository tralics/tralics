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
    if (cite_prefix == from_refer) cite_prefix = from_year;
    cite_key = s;
    Buffer B1(s);
    B1.lowercase();
    lower_cite_key = B1;

    full_key.clear();
    if (cite_prefix == from_foot)
        full_key = "foot";
    else if (cite_prefix == from_refer)
        full_key = "refer";
    full_key.append("cite:");
    full_key.append(s);
}

auto CitationKey::from_to_string() const -> std::string {
    if (cite_prefix == from_year) return "year";
    if (cite_prefix == from_refer) return "refer";
    return "foot";
}
