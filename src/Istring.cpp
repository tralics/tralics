#include "tralics/Buffer.h"
#include "tralics/LabelInfo.h"
#include "tralics/consts.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace {
    struct StrHash_record {
        std::string name;
        LabelInfo * labinfo{nullptr};
    };

    class StrHash : public std::vector<StrHash_record> {
        std::unordered_map<std::string, size_t> s_to_i;

    public:
        StrHash() : std::vector<StrHash_record>{{""}, {""}, {" "}} {
            s_to_i.try_emplace("", 1);
            s_to_i.try_emplace(" ", 2);
        }

        auto find_or_insert(const std::string &s) -> size_t {
            if (auto tmp = s_to_i.find(s); tmp != s_to_i.end()) return tmp->second;
            s_to_i.emplace(s, size());
            push_back({s});
            return size() - 1;
        }
    };

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

auto Istring::labinfo() const -> LabelInfo * {
    if (SH[id].labinfo == nullptr) SH[id].labinfo = new LabelInfo;
    return SH[id].labinfo;
}
