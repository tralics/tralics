#include "tralics/CitationKey.h"
#include "tralics/Buffer.h"
#include "tralics/globals.h"

// Ctor via "foot" and "Knuth"
CitationKey::CitationKey(const std::string &a, const std::string &b) {
    if (a == "foot")
        cite_prefix = from_foot;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Ctor via from_foot and "Knuth".
CitationKey::CitationKey(bib_from a, const std::string &b) {
    if (a == from_foot)
        cite_prefix = a;
    else
        cite_prefix = from_year;
    make_key(b);
}

// Common code of the Ctor. Argument is "Knuth", cite_prefix is OK.
void CitationKey::make_key(const std::string &s) {
    cite_key = s;
    Buffer B1(s);
    B1.lowercase();
    lower_cite_key = B1;

    full_key.clear();
    if (cite_prefix == from_foot) full_key = "foot";
    full_key.append("cite:");
    full_key.append(s);
}

auto CitationKey::from_to_string() const -> std::string {
    if (cite_prefix == from_year) return "year";
    return "foot";
}
