#include "tralics/ConfigData.h"
#include <cctype>
#include <string_view>

namespace {
    class SlashParser {
    public:
        explicit SlashParser(std::string_view input) : s(input) {}

        auto next(std::string &out) -> bool {
            std::string res;
            size_t      p = 0;
            while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])) != 0) ++i;
            if (i >= s.size()) return false;
            for (;;) {
                if (i >= s.size()) return false;
                char c = s[i++];
                if (c == '/') break;
                if (c == '\\') {
                    if (i >= s.size()) return false;
                    if (s[i] == ' ') p = res.size() + 1;
                    c = s[i++];
                }
                res.push_back(c);
            }
            auto b = res.size();
            while (b > p && (std::isspace(static_cast<unsigned char>(res[b - 1])) != 0)) b--;
            res.resize(b);
            out = std::move(res);
            return true;
        }

    private:
        std::string_view s;
        size_t           i{0};
    };
} // namespace

void ParamDataList::interpret(const std::string &b) {
    std::string input = b + "//";
    size_t      i     = 0;
    bool        reset = true;
    if (i < input.size() && input[i] == '+') {
        reset = false;
        ++i;
    }
    if (i < input.size() && input[i] == '/') ++i;
    if (reset) clear();
    SlashParser B(std::string_view(input).substr(i));
    for (;;) {
        std::string r1;
        std::string r2;
        if (!B.next(r1)) return;
        if (!B.next(r2)) return;
        if (r1.empty()) continue;
        if (r2.empty()) r2 = r1;
        (*this)[r1] = r2;
    }
}
