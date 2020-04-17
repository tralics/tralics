#pragma once
#include "consts.h"
#include <string>
#include <vector>

class LabelInfo;

struct StrHash_record {
    String      name{nullptr};
    std::string value;
    LabelInfo * labinfo{nullptr};
};

/// The string_hash table. It is like the hashtable below (Text and Next)
/// but it is extensible (maybe, we should make the hashtable extensible)
/// Each item has a pointer to a LabelInfo element. If the pointer is not null
/// we consider the slot of `foo`, that has a `\label{foo}` or `\ref{foo}`
/// At bootstrap, we fill the table with true, false, spacebefore, center,...
/// This uses utf8 encoding. The token can be dumped via its Value
/// which can be ascii, utf8 or latin1 (XML syntax)

class StrHash : public std::vector<StrHash_record> {
    std::vector<size_t> next;
    size_t              hash_last{hash_prime + 1}; // last slot used
public:
    StrHash();

    auto find_or_insert(const std::string &s) -> size_t;
    auto operator[](size_t k) const -> String { return at(k).name; }
    auto labinfo(size_t k) -> LabelInfo *;
};
