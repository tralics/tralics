#include "tralics/globals.h"
#include "tralics/Bbl.h"
#include "tralics/Bibliography.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/SaveAux.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

auto next_label_id() -> std::string {
    static size_t last_label_id = 0;
    return std::string(fmt::format("uid{}", ++last_label_id));
}

auto fonts1(const std::string &x) -> Xml * {
    bool     w   = the_main.use_font_elt;
    Xml *    res = new Xml(the_names[w ? x : "hi"], nullptr);
    AttList &W   = res->id.get_att();
    if (!w) W[the_names["rend"]] = the_names[x];
    return res;
}

auto first_boundary() -> boundary_type {
    for (size_t i = the_save_stack.size(); i > 0; i--) {
        auto &p = the_save_stack[i - 1];
        if (!p) continue;
        if (p->type != st_boundary) continue;
        the_parser.first_boundary_loc = p->line;
        return dynamic_cast<SaveAuxBoundary *>(p.get())->val;
    }
    return bt_impossible;
}

// Returns the slot associated to the env S
auto is_env_on_stack(const std::string &s) -> SaveAuxEnv * {
    for (size_t i = the_save_stack.size(); i > 0; i--) {
        auto &p = the_save_stack[i - 1];
        if (!p) continue; // \todo this should never happen but it does on linux+clang9
        if (p->type != st_env) continue;
        auto *q = dynamic_cast<SaveAuxEnv *>(p.get());
        if (q->name == s) return q;
    }
    return nullptr;
}
