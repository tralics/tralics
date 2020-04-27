#include "tralics/Buffer.h"
#include "tralics/StrHash.h"

namespace {
    StrHash SH;
} // namespace

Istring::Istring(size_t N) : name(SH[N].name), id(N) {}

Istring::Istring(const std::string &s) : name(s), id(SH.find_or_insert(s)) {}

Istring::Istring(const ScaledInt &i) {
    Buffer B;
    B.push_back(i, glue_spec_pt);
    name = B.to_string();
    id   = SH.find_or_insert(name);
}

auto Istring::value() const -> std::string { return SH[id].value; }

auto Istring::labinfo() const -> LabelInfo * { return SH.labinfo(id); }
