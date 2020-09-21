#include "tralics/globals.h"
#include "tralics/Parser.h"
#include "txinline.h"
#include <fmt/format.h>

auto main_ns::search_in_confdir(const std::string &s) -> std::optional<std::filesystem::path> {
    for (auto i = conf_path.size(); i != 0; i--) {
        auto f = conf_path[i - 1] / s;
        if (tralics_ns::file_exists(f)) return f;
    }
    return {};
}

auto next_label_id() -> std::string {
    static size_t last_label_id = 0;
    return std::string(fmt::format("uid{}", ++last_label_id));
}
