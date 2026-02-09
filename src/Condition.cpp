#include "tralics/Condition.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <spdlog/spdlog.h>
#include <sstream>

void Condition::dump() const {
    for (size_t i = size(); i > 0; i--) at(i - 1).dump(to_signed(i - 1));
}

void CondAux::dump(long i) const {
    std::ostringstream oss;
    oss << "### level " << i << " serial " << serial << ": ";
    auto T = cur_if;
    if (T >= unless_code) {
        T -= unless_code;
        oss << "\\unless";
    }
    CmdChr tmp(if_test_cmd, subtypes(T));
    oss << "\\" << tmp.name().value_or("");
    if (if_limit == fi_code) oss << "\\else";
    oss << " entered on line " << if_line;
    spdlog::trace("{}", oss.str());
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
    auto n = back().cur_if;
    if (n < unless_code) return to_signed(n + 1);

    return -to_signed(n - unless_code + 1);
}

// Pushes a new conditional
auto Condition::push(unsigned chr) -> size_t {
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
        the_parser.nb_errs++;
        spdlog::error("Unterminated \\if {}, started at line {}", top_serial(), top_line());
        pop_back();
    }
}
