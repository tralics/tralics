#include "tralics/Dispatcher.h"

void Dispatcher::register_void(symcodes x, parser_fn f) {
    emplace(x, [f](symcodes, subtypes) { return std::invoke(f, the_parser); });
}

void Dispatcher::register_void(symcodes x, parser_fn_void f) {
    emplace(x, [f](symcodes, subtypes) {
        std::invoke(f, the_parser);
        return true;
    });
}

void Dispatcher::register_with_x(symcodes x, parser_fn_with_x f) {
    emplace(x, [f](symcodes x, subtypes) { return std::invoke(f, the_parser, x); });
}

void Dispatcher::register_with_x(symcodes x, parser_fn_with_x_void f) {
    emplace(x, [f](symcodes x, subtypes) {
        std::invoke(f, the_parser, x);
        return true;
    });
}

void Dispatcher::register_with_c(symcodes x, parser_fn_with_c f) {
    emplace(x, [f](symcodes, subtypes c) { return std::invoke(f, the_parser, c); });
}

void Dispatcher::register_with_c(symcodes x, parser_fn_with_c_void f) {
    emplace(x, [f](symcodes, subtypes c) {
        std::invoke(f, the_parser, c);
        return true;
    });
}

Dispatcher::Dispatcher() {
    register_with_c(cst2_cmd, &Parser::T_cst2);
    register_with_x(begin_cmd, &Parser::T_beginend);
    register_with_x(description_cmd, &Parser::T_listenv);
    register_with_x(end_cmd, &Parser::T_beginend);
    register_with_x(enumerate_cmd, &Parser::T_listenv);
    register_with_x(itemize_cmd, &Parser::T_listenv);
    register_with_x(list_cmd, &Parser::T_listenv);
}
