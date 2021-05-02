#include "tralics/ConfigData.h"
#include "tralics/Parser.h"

auto ConfigData::find_one_key(const std::string &name, const std::string &key) const -> std::optional<std::string> {
    auto it = find(name);
    if (it == end()) {
        the_parser.parse_error(the_parser.err_tok, "Configuration file does not define ", name, "no list");
        return {};
    }
    const auto &X = it->second;
    if (auto it2 = X.find(key); it2 != X.end()) return it2->second;
    err_buf = fmt::format("Illegal value '{}' for {}, use one of:", key, name);
    err_buf = std::accumulate(X.begin(), X.end(), err_buf, [](const auto &c, const auto &i) { return c + " " + i.first; });
    the_parser.signal_error(the_parser.err_tok, "illegal data");
    return {};
}

auto ConfigData::format_keys(const std::string &name) const -> std::string {
    auto it = find(name);
    if (it == end()) return "";
    std::string res;
    for (const auto &d : it->second) res.append(fmt::format("{}={},", d.first, d.second));
    if (!res.empty()) res.pop_back();
    return res;
}
