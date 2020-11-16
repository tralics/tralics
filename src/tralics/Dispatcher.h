#pragma once
#include "Parser.h"
#include "enums.h"
#include <unordered_map>

class Dispatcher {
public:
    using parser_fn                  = bool (Parser::*)();
    using parser_fn_void             = void (Parser::*)();
    using parser_fn_with_x           = bool (Parser::*)(symcodes x);
    using parser_fn_with_x_void      = void (Parser::*)(symcodes x);
    using parser_fn_with_c           = bool (Parser::*)(subtypes c);
    using parser_fn_with_c_void      = void (Parser::*)(subtypes c);
    using parser_fn_with_xc          = bool (Parser::*)(symcodes x, subtypes c);
    using parser_fn_with_xc_void     = void (Parser::*)(symcodes x, subtypes c);
    using parser_fn_with_cmdchr      = bool (Parser::*)(CmdChr cc);
    using parser_fn_with_cmdchr_void = void (Parser::*)(CmdChr cc);

    Dispatcher();

    auto lookup_and_call(symcodes x, subtypes c) -> std::optional<bool>;
    auto name(symcodes x, subtypes c) -> std::string;

    void register_action(symcodes x, std::function<bool(symcodes, subtypes)> f); // explicit action
    void register_action(symcodes x, std::function<void(symcodes, subtypes)> f); // explicit action, return true

    void register_action(symcodes x, parser_fn f);                  // x triggers the_parser.f()
    void register_action(symcodes x, parser_fn_void f);             // x triggers the_parser.f(), return true
    void register_action(symcodes x, parser_fn_with_x f);           // x triggers the_parser.f(x)
    void register_action(symcodes x, parser_fn_with_x_void f);      // x triggers the_parser.f(x), return true
    void register_action(symcodes x, parser_fn_with_c f);           // x triggers the_parser.f(c)
    void register_action(symcodes x, parser_fn_with_c_void f);      // x triggers the_parser.f(c), return true
    void register_action(symcodes x, parser_fn_with_xc f);          // x triggers the_parser.f(x,c)
    void register_action(symcodes x, parser_fn_with_xc_void f);     // x triggers the_parser.f(x,c), return true
    void register_action(symcodes x, parser_fn_with_cmdchr f);      // x triggers the_parser.f({x,c})
    void register_action(symcodes x, parser_fn_with_cmdchr_void f); // x triggers the_parser.f({x,c}), return true

    void register_name(symcodes x, std::function<std::string(symcodes, subtypes)> f);

private:
    std::unordered_map<symcodes, std::function<bool(symcodes, subtypes)>> &       the_actions();
    std::unordered_map<symcodes, std::function<std::string(symcodes, subtypes)>> &the_names();
};

inline Dispatcher actions;
