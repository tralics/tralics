#include "tralics/Dispatcher.h"

void Dispatcher::register_action(symcodes x, parser_fn f) {
    emplace(x, [f](symcodes, subtypes) { return std::invoke(f, the_parser); });
}

void Dispatcher::register_action(symcodes x, parser_fn_void f) {
    emplace(x, [f](symcodes, subtypes) {
        std::invoke(f, the_parser);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x f) {
    emplace(x, [f](symcodes x, subtypes) { return std::invoke(f, the_parser, x); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_x_void f) {
    emplace(x, [f](symcodes x, subtypes) {
        std::invoke(f, the_parser, x);
        return true;
    });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c f) {
    emplace(x, [f](symcodes, subtypes c) { return std::invoke(f, the_parser, c); });
}

void Dispatcher::register_action(symcodes x, parser_fn_with_c_void f) {
    emplace(x, [f](symcodes, subtypes c) {
        std::invoke(f, the_parser, c);
        return true;
    });
}

Dispatcher::Dispatcher() {
    register_action(cst2_cmd, &Parser::T_cst2);
    register_action(begin_cmd, &Parser::T_beginend);
    register_action(description_cmd, &Parser::T_listenv);
    register_action(end_cmd, &Parser::T_beginend);
    register_action(enumerate_cmd, &Parser::T_listenv);
    register_action(itemize_cmd, &Parser::T_listenv);
    register_action(list_cmd, &Parser::T_listenv);
    register_action(leave_v_mode_cmd, &Parser::leave_v_mode);
}
