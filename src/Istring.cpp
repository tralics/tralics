#include "tralics/Buffer.h"
#include "tralics/LabelInfo.h"
#include "tralics/robin_hood.h"
#include <memory>

namespace {
    std::vector<std::string> SH{"", "", " "};

    auto find_or_insert(const std::string &s) -> size_t {
        static robin_hood::unordered_map<std::string, size_t> s_to_i{{"", 1U}, {" ", 2U}};
        if (auto tmp = s_to_i.find(s); tmp != s_to_i.end()) return tmp->second;
        s_to_i.emplace(s, SH.size());
        SH.push_back(s);
        return SH.size() - 1;
    }
} // namespace

Istring::Istring(size_t N) : id(N), name(SH[N]), value(Buffer(name).convert_to_out_encoding()) {}

Istring::Istring(const std::string &s) : id(find_or_insert(s)), name(s), value(Buffer(name).convert_to_out_encoding()) {}

auto Istring::labinfo() const -> LabelInfo * {
    static std::vector<LabelInfo> LL;
    if (LL.size() <= id) { LL.resize(id + 1); }
    return &LL[id];
}
