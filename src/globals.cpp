#include "tralics/globals.h"
#include "txinline.h"
#include "txparser.h"

std::vector<std::filesystem::path> conf_path;
std::vector<std::string>           input_path;

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

auto main_ns::search_in_confdir(const std::string &s) -> bool { // \todo return std::optional<path>
    for (auto i = conf_path.size(); i != 0; i--) {
        main_ns::path_buffer << bf_reset << conf_path[i - 1] << bf_optslash << s;
        if (tralics_ns::file_exists(conf_path[i - 1] / s)) return true;
    }
    return false;
}

auto tralics_ns::exists(const std::vector<std::string> &ST, const std::string &d) -> bool {
    for (const auto &j : ST)
        if (j == d) return true;
    return false;
}

auto tralics_ns::get_short_jobname() -> std::string { return file_name; }
