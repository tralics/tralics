#include "tralics/globals.h"
#include "tralics/Parser.h"
#include "txinline.h"
#include <fmt/format.h>

std::vector<std::filesystem::path> conf_path;

std::string file_name;
std::string all_themes;
std::string everyjob_string;

std::vector<LinePtr> file_pool;

size_t leftquote_val{'`'};
size_t rightquote_val{'\''};

bool bad_minus{false};
bool compatibility{false};
bool nofloat_hack{false};
bool only_input_data{false};
bool raw_bib{false};
bool seen_enddocument{false};

std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;

void main_ns::register_file(LinePtr &&x) { file_pool.push_back(std::move(x)); }

auto main_ns::search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path> {
    for (auto i = conf_path.size(); i != 0; i--) {
        auto f = conf_path[i - 1] / s;
        if (tralics_ns::file_exists(f)) return f;
    }
    return {};
}

auto tralics_ns::exists(const std::vector<std::string> &ST, const std::string &d) -> bool {
    for (const auto &j : ST)
        if (j == d) return true;
    return false;
}

auto tralics_ns::get_short_jobname() -> std::string { return file_name; }

auto next_label_id() -> std::string {
    static size_t last_label_id = 0;
    return std::string(fmt::format("uid{}", ++last_label_id));
}

// Here we have to find the character c.
// only top-level characters are considered. Active chars are allowed.
// MX is decreased. Job aborted if it becomes negative.
auto token_ns::expand_nct(TokenList &L, size_t n, uchar c, int &MX, TokenList &body) -> bool {
    TokenList                 res;
    bool                      result = false;
    std::array<TokenList, 10> Table; // arguments of the commands
    while (!L.empty()) {
        Token t = L.front();
        L.pop_front();
        if (t.is_a_left_brace()) { // get a whole block
            L.push_front(t);       // re-enter the brace in the list
            token_ns::fast_get_block(L, res);
            continue;
        }
        if (t.cmd_val() == 10) continue;             // ignore spaces.
        if (!(t.is_a_char() && t.char_val() == c)) { // just copy
            res.push_back(t);
            continue;
        }
        result = true; // We found something
        MX--;
        if (MX < 0) return true;
        for (size_t k = 0; k < n; k++) Table[k + 1] = get_a_param(L, false);
        TokenList W = Parser::expand_mac_inner(body, Table.data());
        L.splice(L.begin(), W);
    }
    L.splice(L.end(), res);
    return result;
}
