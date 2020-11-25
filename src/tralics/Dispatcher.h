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

    Dispatcher(); // \todo this method is too big, but easier to split up than translate03 was, do that

    static void register_action(symcodes x, const std::function<bool()> &f);                   // explicit action
    static void register_action(symcodes x, const std::function<bool(symcodes)> &f);           // explicit action
    static void register_action(symcodes x, const std::function<bool(subtypes)> &f);           // explicit action
    static void register_action(symcodes x, const std::function<bool(symcodes, subtypes)> &f); // explicit action

    static void register_action_plain(symcodes x, const std::function<void()> &f);                   // explicit action
    static void register_action_plain(symcodes x, const std::function<void(symcodes)> &f);           // explicit action
    static void register_action_plain(symcodes x, const std::function<void(subtypes)> &f);           // explicit action
    static void register_action_plain(symcodes x, const std::function<void(symcodes, subtypes)> &f); // explicit action

    static void register_action(symcodes x, parser_fn f);             // x triggers the_parser.f()
    static void register_action(symcodes x, parser_fn_with_x f);      // x triggers the_parser.f(x)
    static void register_action(symcodes x, parser_fn_with_c f);      // x triggers the_parser.f(c)
    static void register_action(symcodes x, parser_fn_with_xc f);     // x triggers the_parser.f(x,c)
    static void register_action(symcodes x, parser_fn_with_cmdchr f); // x triggers the_parser.f({x,c})

    static void register_action_plain(symcodes x, parser_fn_void f);             // x triggers the_parser.f()
    static void register_action_plain(symcodes x, parser_fn_with_x_void f);      // x triggers the_parser.f(x)
    static void register_action_plain(symcodes x, parser_fn_with_c_void f);      // x triggers the_parser.f(c)
    static void register_action_plain(symcodes x, parser_fn_with_xc_void f);     // x triggers the_parser.f(x,c)
    static void register_action_plain(symcodes x, parser_fn_with_cmdchr_void f); // x triggers the_parser.f({x,c})

    static void boot_math();
};

inline Dispatcher actions;
