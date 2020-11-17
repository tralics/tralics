#include "tralics/CitationKey.h"
#include "tralics/Buffer.h"
#include "tralics/globals.h"

CitationKey::CitationKey(const std::string &b) { make_key(b); }

// Common code of the Ctor. Argument is "Knuth", cite_prefix is OK.
void CitationKey::make_key(const std::string &s) {
    cite_key = s;
    Buffer B1(s);
    B1.lowercase();
    lower_cite_key = B1;

    full_key.clear();
    full_key.append("cite:");
    full_key.append(s);
}
