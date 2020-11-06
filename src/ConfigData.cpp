#include "tralics/ConfigData.h"
#include "tralics/Parser.h"

namespace {
    // Interprets the RC argument of a pers command \todo RA
    // This returns the short name, said otherwise, the argument.
    // Notice the space case when argument is empty, or +foo or =foo.
    auto pers_rc(const std::string &rc) -> std::string {
        static std::string the_default_rc; // \todo remove
        if (rc.empty()) return the_default_rc;
        if (rc[0] == '+') return rc.substr(1);
        bool spec = (rc.size() >= 2 && rc[0] == '=');
        auto RC   = spec ? rc.substr(1) : rc;
        if (spec) the_default_rc = RC;
        return RC;
    }
} // namespace

auto ConfigData::find_one_key(const std::string &name, const std::string &key) const -> std::string {
    if (name == "ur") return pers_rc(key);
    if (name == "section") return key.empty() ? "default" : key;
    auto it = find(name);
    if (it == end()) {
        the_parser.parse_error(the_parser.err_tok, "Configuration file does not define ", name, "no list");
        return "";
    }
    auto &X = it->second;
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
