#include "tralics/Dispatcher.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/Saver.h"
#include "tralics/globals.h"
#include "tralics/types.h"

// \todo make a hash table of methods instead of this huge mess, actions below is a proof of concept.

[[nodiscard]] auto Parser::translate03() -> bool {
    auto guard  = SaveErrTok(cur_tok);
    auto [x, c] = cur_cmd_chr;

    if (x == underscore_catcode && global_in_load) {
        translate_char(cur_cmd_chr);
        return true;
    }

    if (auto res = Dispatcher::call(x, c)) return *res;

    switch (x) {
    default: undefined_mac(); return true;
    }
}
