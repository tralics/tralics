#include "tralics/Condition.h"
#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

void Condition::dump() const {
    for (size_t i = size(); i > 0; i--) at(i - 1).dump(to_signed(i - 1));
}

void CondAux::dump(long i) const {
    Logger::finish_seq();
    the_log << "### level " << i << " serial " << serial << ": ";
    auto T = to_unsigned(cur_if);
    if (T >= unless_code) {
        T -= unless_code;
        the_log << "\\unless";
    }
    CmdChr tmp(if_test_cmd, subtypes(T));
    the_log << "\\" << tmp.name();
    if (if_limit == fi_code) the_log << "\\else";
    the_log << " entered on line " << if_line;
    the_log << "\n";
}

// for \currentifbranch
auto Condition::top_branch() const -> int {
    int k = top_limit();
    if (k == or_code || k == else_code) return 1;
    if (k == fi_code) return -1;
    return 0;
}

// for \currentiftype
auto Condition::top_type() const -> long {
    if (empty()) return 0;
    auto n = to_unsigned(back().cur_if);
    if (n < unless_code) return to_signed(n + 1);

    return -to_signed(n - unless_code + 1);
}

// Pushes a new conditional
auto Condition::push(int chr) -> size_t {
    if_serial++;
    push_back({if_code, chr, the_parser.get_cur_line(), if_serial});
    return size();
}

// Pops a conditional
void Condition::pop() {
    if (empty())
        spdlog::error("Error: attempt to pop empty conditional stack");
    else
        pop_back();
}

// This pops all conditions, signaling errors.
void Condition::terminate() {
    while (!empty()) {
        nb_errs++;
        spdlog::error("Unterminated \\if {}, started at line {}", top_serial(), top_line());
        pop_back();
    }
}
