#include "tralics/globals.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

auto next_label_id() -> std::string {
    static size_t last_label_id = 0;
    return std::string(fmt::format("uid{}", ++last_label_id));
}

auto fonts1(const std::string &x) -> Xml * {
    bool     w   = the_main->use_font_elt;
    Xml *    res = new Xml(the_names[w ? x : "hi"], nullptr);
    AttList &W   = res->id.get_att();
    if (!w) W.push_back(the_names["rend"], the_names[x]);
    return res;
}
