#include "tralics/Dispatcher.h"

void Dispatcher::register_with_x(symcodes x, parser_fn_with_x f) {
    emplace(x, [f](symcodes x, subtypes) { return std::invoke(f, the_parser, x); });
}

void Dispatcher::register_with_x(symcodes x, parser_fn_with_x_void f) {
    emplace(x, [f](symcodes x, subtypes) {
        std::invoke(f, the_parser, x);
        return true;
    });
}

Dispatcher::Dispatcher() {
    actions.register_with_x(begin_cmd, &Parser::T_beginend);
    actions.register_with_x(end_cmd, &Parser::T_beginend);
}
