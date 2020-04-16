#pragma once
#include "../txscaled.h"
#include <vector>

class LabelInfo;

struct StrHash_record {
    String      name{nullptr};
    std::string value;
    LabelInfo * Labinfo{nullptr};
    size_t      next{0};
};

/// The string_hash table. It is like the hashtable below (Text and Next)
/// but it is extensible (maybe, we should make the hashtable extensible)
/// Each item has a pointer to a LabelInfo element. If the pointer is not null
/// we consider the slot of `foo`, that has a `\label{foo}` or `\ref{foo}`
/// At bootstrap, we fill the table with true, false, spacebefore, center,...
/// This uses utf8 encoding. The token can be dumped via its Value
/// which can be ascii, utf8 or latin1 (XML syntax)

class StrHash : public std::vector<StrHash_record> {
    size_t hash_last{hash_prime + 1}; // last slot used
public:
    StrHash() : std::vector<StrHash_record>(hash_size) {
        at(0) = {"", "", nullptr, 0};
        at(1) = {"", "", nullptr, 0};
        at(2) = {" ", " ", nullptr, 0};
    }

    [[nodiscard]] auto p_str(size_t k) -> std::string { return at(k).value; }

    auto        find(const std::string &s) -> size_t;
    auto        find(int s) -> size_t;
    auto        operator[](size_t k) const -> String { return at(k).name; }
    auto        lab_val(Istring k) -> LabelInfo * { return at(k.id).Labinfo; };
    auto        lab_val_check(Istring k) -> LabelInfo *;
    static auto next_label_id() -> Istring; // \todo Move outside
    auto        find_scaled(ScaledInt s) -> Istring;
};

inline StrHash SH; // \todo This would fit better elsewhere
