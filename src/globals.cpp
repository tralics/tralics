#include "tralics/globals.h"
#include "txinline.h"
#include "txparser.h"

std::vector<std::string> conf_path;
std::vector<std::string> input_path;
std::vector<std::string> other_options;

std::string log_name;
std::string file_name;
std::string out_dir;
std::string no_ext;
std::string opt_doctype;
std::string all_themes;
std::string everyjob_string;

std::vector<LinePtr *> file_pool;

std::optional<size_t> pool_position;

size_t leftquote_val{'`'};
size_t rightquote_val{'\''};

bool bad_minus{false};
bool compatibility{false};
bool nofloat_hack{false};
bool only_input_data{false};
bool raw_bib{false};
bool seen_enddocument{false};

std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;

namespace {
    /// Returns true if the file is in the pool
    auto search_in_pool(const std::string &name) -> bool {
        size_t n      = file_pool.size();
        pool_position = {};
        for (size_t i = 0; i < n; i++) {
            if (file_pool[i]->get_file_name() == name) {
                pool_position = i;
                return true;
            }
        }
        return false;
    }
} // namespace

void main_ns::register_file(LinePtr *x) { file_pool.push_back(x); }

auto tralics_ns::find_in_confdir(const std::string &s, bool retry) -> bool {
    main_ns::path_buffer << bf_reset << s;
    if (search_in_pool(s)) return true;
    if (file_exists(main_ns::path_buffer)) return true;
    if (!retry) return false;
    if (s.empty() || s[0] == '.' || s[0] == '/') return false;
    return main_ns::search_in_confdir(s);
}

auto main_ns::search_in_confdir(const std::string &s) -> bool {
    auto n = conf_path.size();
    for (auto i = n; i != 0; i--) {
        main_ns::path_buffer << bf_reset << conf_path[i - 1] << bf_optslash << s;
        if (tralics_ns::file_exists(main_ns::path_buffer)) return true;
    }
    return false;
}

auto tralics_ns::find_no_path(const std::string &s) -> bool {
    if (s.empty()) return false;
    main_ns::path_buffer << bf_reset << s;
    return file_exists(main_ns::path_buffer);
}

auto tralics_ns::find_in_path(const std::string &s) -> bool {
    if (s.empty()) return false;
    main_ns::path_buffer << bf_reset << s;
    if (search_in_pool(s)) return true;
    if (s[0] == '.' || s[0] == '/') return file_exists(main_ns::path_buffer);
    auto n = input_path.size();
    for (size_t i = 0; i < n; i++) {
        const std::string &p = input_path[i];
        if (p.empty())
            main_ns::path_buffer << bf_reset << s;
        else
            main_ns::path_buffer << bf_reset << p << bf_optslash << s;
        if (file_exists(main_ns::path_buffer)) return true;
    }
    return false;
}

auto tralics_ns::exists(const std::vector<std::string> &ST, const std::string &d) -> bool {
    for (const auto &j : ST)
        if (j == d) return true;
    return false;
}

auto tralics_ns::get_out_dir(const std::string &name) -> String {
    Buffer &B = main_ns::path_buffer;
    B << bf_reset << out_dir << bf_optslash << name;
    return B.c_str();
}

auto tralics_ns::get_short_jobname() -> std::string { return file_name; }
