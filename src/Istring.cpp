#include "tralics/Buffer.h"

Istring::Istring(const std::string &s) : name(s), id(SH.find_or_insert(s)) {}

Istring::Istring(const ScaledInt &i) {
    Buffer B;
    B.push_back(i, glue_spec_pt);
    name = B.to_string();
    id   = SH.find_or_insert(name);
}
