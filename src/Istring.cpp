#include "tralics/Buffer.h"
#include "tralics/StrHash.h"

Istring::Istring(String s) : id(SH.find_or_insert(s)) {}

Istring::Istring(const Buffer &X) : id(SH.find_or_insert(X.c_str())) {}

Istring::Istring(const std::string &s) : id(SH.find_or_insert(s)) {}

Istring::Istring(const ScaledInt &i) {
    Buffer B;
    B.push_back(i, glue_spec_pt);
    id = SH.find_or_insert(B.to_string());
}

auto Istring::c_str() const -> String { return SH[id]; }

auto Istring::p_str() const -> std::string { return SH.at(id).value; }
