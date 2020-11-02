#include "tralics/FpGenList.h"
#include "tralics/Buffer.h"
#include "tralics/Parser.h"
#include <spdlog/spdlog.h>

// ---------------------------------------------------------
// If the token list contains a string of N characters, is followed by X
// returns X and sets n to N. Return null token in case all tokens in the
// list are characters.
auto FpGenList::find_str(int &n) const -> Token {
    tkbuf.clear();
    auto C = value.begin();
    auto E = value.end();
    n      = 0;
    while (C != E) {
        Token x = *C;
        ++C;
        if (x.is_lowercase_token()) {
            tkbuf.push_back(static_cast<char>(x.val_as_letter()));
            n++;
        } else
            return x;
    }
    return Token(0);
}

// Adds at the end of *this a space and S (of type letter)
// Assumes that S is ascii
void FpGenList::add_last_space(String S) {
    push_back(the_parser.hash_table.space_token);
    for (size_t i = 0;; i++) {
        auto c = S[i];
        if (c == 0) return;
        if (static_cast<uchar>(c) > 128) {
            spdlog::critical("Add last space: internal error");
            abort();
        }
        push_back(Token(letter_t_offset, static_cast<uchar>(c)));
    }
}

// Adds at the end of *this a space, W, a second space and S (of type letter)
// Assumes that S is ascii
void FpGenList::add_last_space(TokenList &W, String S) {
    value.push_back(the_parser.hash_table.space_token);
    push_back(W);
    add_last_space(S);
}

// splits at X. What is before is put in z. Cur element disappears.

void FpGenList::split_after(TokenList::iterator X, TokenList &z) {
    z.clear();
    z.splice(z.begin(), value, X, value.end());
    z.pop_front();
    z.swap(value);
}

// Finds the first x1 or x2. Splits there. before is put in z.
// Returns x1 or x2 (the token that is found) or 0 (if nothing is found).
auto FpGenList::split_at(Token x1, Token x2, TokenList &z) -> Token {
    auto X = value.begin();
    auto E = value.end();
    for (;;) {
        if (X == E) return Token(0);
        Token x = *X;
        if (x == x1 || x == x2) {
            split_after(X, z);
            return x;
        }
        ++X;
    }
}

// The first N tokens are put in z.
// the Token that follows is recycled
void FpGenList::split_after(int n, TokenList &z) {
    auto X = value.begin();
    while (n > 0) {
        ++X;
        n--;
    }
    split_after(X, z);
}

// In the case of word space something, converts to something space word
// provided that word is in the list.
// For instance sin 25 gives 25 sin
void FpGenList::fp_gen_app() {
    while (!value.empty() && value.front().is_space_token()) value.pop_front();
    int   n = 0;
    Token x = find_str(n);
    if (!x.is_space_token()) return;
    auto &S = tkbuf;
    if ((S == "add") || (S == "sub") || (S == "mul") || (S == "div") || (S == "abs") || (S == "neg") || (S == "sgn") || (S == "min") ||
        (S == "max") || (S == "round") || (S == "trunc") || (S == "clip") || (S == "exp") || (S == "ln") || (S == "pow") || (S == "root") ||
        (S == "seed") || (S == "random") || (S == "sin") || (S == "cos") || (S == "sincos") || (S == "tan") || (S == "cot") ||
        (S == "tancot") || (S == "arcsin") || (S == "arccos") || (S == "arcsincos") || (S == "arctan") || (S == "arccot") ||
        (S == "arctancot") || (S == "pop") || (S == "swap") || (S == "copy")) {
        push_back(the_parser.hash_table.space_token);
        TokenList z;
        split_after(n, z);
        push_back(z);
    }
}

// Converts A^B into B A pow. If there is no ^, then fp_gen_app is called
// Note that `sin 1^cos 2' gives `2 cos 1 sin pow'. Note order of args.
void FpGenList::fp_gen_exp() {
    Token     x(hat_t_offset, '^');
    TokenList aux;
    if (!split_at(x, x, aux).is_null()) {
        fp_gen_exp(); // handle B
        FpGenList Aux(aux);
        Aux.fp_gen_exp();                 // handle A
        add_last_space(Aux.value, "pow"); // insert operator and after
    } else
        fp_gen_app();
}

// Converts A via fp_gen_exp, A*B into A B mul, where A and B are handled
// and A*B*C gives A B mul C mul. Handles division also.
// Order is preserved.
void FpGenList::fp_gen_mul() {
    static const Token x1 = the_parser.hash_table.star_token;
    static const Token x2(other_t_offset, '/');
    for (;;) {
        TokenList aux;
        Token     y = split_at(x1, x2, aux);
        if (y.is_null()) {
            fp_gen_exp();
            return;
        } // return if no star
        TokenList aux2;
        Token     y2 = split_at(x1, x2, aux2);
        if (y2.is_null()) { // cas a*B
            fp_gen_exp();   // handle  B
            FpGenList Aux(aux);
            Aux.fp_gen_exp(); // handle A
            Aux.add_last_space(value, y == x1 ? "mul" : "div");
            value.swap(Aux.value);
            return;
        }
        FpGenList Aux(aux);
        Aux.fp_gen_exp(); // case A*B*C, handle A
        FpGenList Aux2(aux2);
        Aux2.fp_gen_exp();                                       // handle B
        Aux.add_last_space(Aux2.value, y == x1 ? "mul" : "div"); // we have A B mul
        Aux.push_back(the_parser.hash_table.space_token);
        Aux.push_back(y2); // insert the second op
        Aux.push_back(value);
        value.swap(Aux.value);
    }
}

// This handles addition subtraction. in the same fashion as fp_gen_mul
void FpGenList::fp_gen_add() {
    Token x1 = the_parser.hash_table.plus_token;
    Token x2 = the_parser.hash_table.minus_token;
    for (;;) {
        TokenList aux;
        Token     y = split_at(x1, x2, aux);
        if (y.is_null()) {
            fp_gen_mul();
            return;
        }
        TokenList aux2;
        Token     y2 = split_at(x1, x2, aux2);
        if (y2.is_null()) {
            fp_gen_mul();
            FpGenList Aux(aux);
            Aux.fp_gen_mul();
            Aux.add_last_space(value, y == x1 ? "add" : "sub");
            value.swap(Aux.value);
            return;
        }
        FpGenList Aux(aux);
        Aux.fp_gen_mul();
        FpGenList Aux2(aux2);
        Aux2.fp_gen_mul();

        Aux.add_last_space(Aux2.value, y == x1 ? "add" : "sub");
        Aux.push_back(the_parser.hash_table.space_token);
        Aux.push_back(y2);
        Aux.push_back(value);
        value.swap(Aux.value);
    }
}

// This replaces foo,bar or foo:bar by  foo bar.
// If a character has been replaced, a space is added at the end.
void FpGenList::fp_gen_komma() {
    bool  need_space = false;
    auto  X          = value.begin();
    auto  E          = value.end();
    Token x1         = the_parser.hash_table.comma_token;
    Token x2         = Token(other_t_offset, ':');
    while (X != E) {
        if (*X == x1 || *X == x2) {
            need_space = true;
            *X         = the_parser.hash_table.space_token;
        }
        ++X;
    }
    if (need_space) push_back(the_parser.hash_table.space_token);
    fp_gen_add();
}

// In case of X(Y)Z, puts X in A, Y in B, leaves Z in *this
// First closing parenthesis is considered. Then last opening one is taken.
auto FpGenList::split_at_p(TokenList &A, TokenList &B) -> bool {
    Token x1 = Token(other_t_offset, '(');
    Token x2 = Token(other_t_offset, ')');
    auto  X  = value.begin();
    auto  E  = value.end();
    auto  p1 = E; // at open paren
    auto  p2 = E; // at close paren
    for (;;) {
        if (X == E) return false;
        Token x = *X;
        if (x == x2) {
            if (p2 != E) return false; // failed
            p2 = X;
            break;
        }
        if (x == x1) p1 = X;
        ++X;
    }
    // close paren seen
    if (p1 == E) return false;
    A.splice(A.begin(), value, value.begin(), p1);
    value.pop_front(); // remove open parenn
    B.splice(B.begin(), value, value.begin(), p2);
    value.pop_front(); // remove close paren
    return true;
}

// In case of sin(a+b)+cos(c+d), B is a+b then c+d
// converted to: a b plus and c d plus.
// The argument of fp_gen_comma is: sin a b plus and cos c d plus.
void FpGenList::to_postfix() {
    fp_check_paren();
    for (;;) {
        TokenList A, B;
        if (split_at_p(A, B)) {
            FpGenList BB(B);
            BB.fp_gen_komma(); //
            value.swap(A);     // A holds: +cos(c+d), and value holds sin
            push_back(the_parser.hash_table.space_token);
            push_back(BB.value);
            push_back(A);
        } else
            break;
    }
    fp_gen_komma();
}

// In the case of (+3+4)*(-5-6),
// this  gives    (3+4)*(0-5-6),
void FpGenList::fp_check_paren() {
    Token x0(other_t_offset, '(');
    Token x1 = the_parser.hash_table.minus_token;
    Token x2 = the_parser.hash_table.plus_token;
    auto  X  = value.begin();
    auto  E  = value.end();
    while (X != E) {
        Token x = *X;
        if (x == x0) {
            auto Y = X;
            ++Y;
            if (Y == E) return;
            Token y = *Y;
            if (y.is_space_token() || y == x2)
                value.erase(Y);
            else if (y == x1)
                value.insert(Y, the_parser.hash_table.zero_token);
        }
        ++X;
    }
}

// This removes the first N tokens from the list
void FpGenList::remove_first_n(int n) {
    while (n > 0) {
        value.pop_front();
        n--;
    }
}

void FpGenList::remove_spaces() {
    TokenList &L = value;
    while (!L.empty() && L.front().is_space_token()) L.pop_front();
    while (!L.empty() && L.back().is_space_token()) L.pop_back();
}
