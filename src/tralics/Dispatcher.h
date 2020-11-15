#pragma once
#include "Parser.h"
#include "enums.h"
#include <unordered_map>

class Dispatcher : public std::unordered_map<symcodes, std::function<bool(symcodes, subtypes)>> {
public:
    using parser_fn             = bool (Parser::*)();
    using parser_fn_void        = void (Parser::*)();
    using parser_fn_with_x      = bool (Parser::*)(symcodes x);
    using parser_fn_with_x_void = void (Parser::*)(symcodes x);
    using parser_fn_with_c      = bool (Parser::*)(subtypes c);
    using parser_fn_with_c_void = void (Parser::*)(subtypes c);

    Dispatcher();

    void register_action(symcodes x, parser_fn f);             // x triggers the_parser.f()
    void register_action(symcodes x, parser_fn_void f);        // x triggers the_parser.f()
    void register_action(symcodes x, parser_fn_with_x f);      // x triggers the_parser.f(x)
    void register_action(symcodes x, parser_fn_with_x_void f); // x triggers the_parser.f(x)
    void register_action(symcodes x, parser_fn_with_c f);      // x triggers the_parser.f(c)
    void register_action(symcodes x, parser_fn_with_c_void f); // x triggers the_parser.f(c)
};

inline Dispatcher actions;
