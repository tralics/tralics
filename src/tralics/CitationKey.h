#pragma once
#include "Buffer.h"
#include "enums.h"
#include <string>

// This is the unique identifier of a bibliography element
// from the bibtex point of view. If used, a unique id (an integer will be
// computed). For the moment being it has a cite key, and a prefix.
struct CitationKey {
    std::string cite_key;                       // cite key, say Foo
    std::string lower_cite_key;                 // cite key, lower case, say foo
    std::string full_key;                       // something like footcite:Foo
    void        make_key(const std::string &s); // creates the key.

    CitationKey() = default;
    CitationKey(const std::string &b);

    [[nodiscard]] auto is_similar(const CitationKey &w) const -> bool { return cite_key == w.cite_key; }
    [[nodiscard]] auto is_similar_lower(const CitationKey &w) const -> bool { return lower_cite_key == w.lower_cite_key; }
    [[nodiscard]] auto is_same(const CitationKey &w) const -> bool { return is_similar(w); } // \todo merge all these
    [[nodiscard]] auto is_same_lower(const CitationKey &w) const -> bool { return is_similar_lower(w); }
};
