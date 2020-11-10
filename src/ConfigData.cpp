#include "tralics/ConfigData.h"
#include "tralics/Parser.h"

auto ConfigData::find_one_key(const std::string &name, const std::string &key) const -> std::string {
    if (name == "section") return key.empty() ? "default" : key;
    auto it = find(name);
    if (it == end()) {
        the_parser.parse_error(the_parser.err_tok, "Configuration file does not define ", name, "no list");
        return "";
    }
    const auto &X = it->second;
    if (auto it2 = X.find(key); it2 != X.end()) return it2->second;
    err_buf = fmt::format("Illegal value '{}' for {}\nUse one of:", key, name);
    for (const auto &i : X) err_buf += " " + i.first;
    the_parser.signal_error(the_parser.err_tok, "illegal data");
    return "";
}

auto ConfigData::format_keys(const std::string &name) const -> std::string {
    auto it = find(name);
    if (it == end()) return "";
    std::string res;
    for (const auto &d : it->second) res.append(fmt::format("{}={},", d.first, d.second));
    if (!res.empty()) res.pop_back();
    return res;
}
