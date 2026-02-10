#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace ra_ns {
    void fnhack(TokenList &c, TokenList &d, TokenList &aux) {
        Hashtab &H  = hash_table;
        int      bl = 0;
        while (!c.empty()) {
            Token x = c.front();
            c.pop_front();
            token_ns::check_brace(x, bl);
            if (x == H.footnote_token && bl == 0) {
                if (!d.empty()) {
                    d.push_front(H.space_token);
                    d.push_front(H.comma_token);
                }
                d.splice(d.begin(), c);
            } else {
                aux.push_back(x);
            }
        }
    }
} // namespace ra_ns

// User function \fn@hack\foo\bar
void Parser::fnhack() {
    Token     A = get_r_token();
    Token     B = get_r_token();
    TokenList La, Lb = get_mac_value(B), Li = get_mac_value(A);
    ra_ns::fnhack(Li, Lb, La);
    new_macro(La, A, false);
    new_macro(Lb, B, false);
}

auto Parser::push_module() -> bool { return push_module(sT_arg_nopar()); }

auto Parser::push_module(const std::string &aux) -> bool {
    if (the_stack.in_h_mode()) { parse_error("Invalid \\begin{module} in a paragraph"); }
    leave_h_mode();
    the_stack.add_nl();
    the_stack.push1(the_names["module"]);
    //  refstepcounter("module",false); Assume done by the package
    if (!start_paras(8, aux, false)) return false;
    return true;
}
