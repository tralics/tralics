#pragma once
#include "Parser.h"
#include "enums.h"
#include <unordered_map>

class Dispatcher : public std::unordered_map<symcodes, std::function<bool(symcodes, subtypes)>> {
public:
    using parser_fn_with_x      = bool (Parser::*)(symcodes x);
    using parser_fn_with_x_void = void (Parser::*)(symcodes x);

    Dispatcher();

    void register_with_x(symcodes x, parser_fn_with_x f);      // x triggers the_parser.f(x)
    void register_with_x(symcodes x, parser_fn_with_x_void f); // x triggers the_parser.f(x)
};

inline Dispatcher actions;
