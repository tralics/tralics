#include "../tralics/Dispatcher.h"
#include <spdlog/spdlog.h>

namespace {
    void tipa() { actions.register_action(ipa_cmd, &Parser::T_ipa); }
    bool t = (tipa(), true);
} // namespace
