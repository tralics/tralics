#include "../tralics/Dispatcher.h"
#include <spdlog/spdlog.h>

/* Auto-registering package
 *
 * This code is automatically running on startup (the static variable `t` is not
 * optimized away because it is initialized by a function with side-effects), we
 * are sure that `register_action` can be used because the map is hidden in the
 * `get_map` method of Dispatcher. This would fail if the code is compiled into
 * a static library but we will not be doing this.
 *
 * Good inclusion is tested by testhtml in the test suite.
 *
 * Reference: https://www.bfilipek.com/2018/02/static-vars-static-lib.html
 */

namespace {
    void tipa() { actions.register_action(ipa_cmd, &Parser::T_ipa); }
    bool t = (tipa(), true);
} // namespace
