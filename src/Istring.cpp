#include "tralics/Buffer.h"
#include "tralics/StrHash.h"

namespace {
    StrHash SH;
} // namespace

Istring::Istring(size_t N) : id(N), name(SH[N].name), value(Buffer(name).convert_to_out_encoding()) {}

Istring::Istring(const std::string &s) : id(SH.find_or_insert(s)), name(s), value(Buffer(name).convert_to_out_encoding()) {}

Istring::Istring(const ScaledInt &i)
    : Istring([&i] {
          Buffer B;
          B.push_back(i, glue_spec_pt);
          return B.to_string();
      }()) {}

auto Istring::labinfo() const -> LabelInfo * { return SH.labinfo(id); }
