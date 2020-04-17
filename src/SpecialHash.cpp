#include "tralics/SpecialHash.h"
#include "tralics/Splitter.h"
#include "txhash.h"
#include "txinline.h"
#include "txparser.h"

namespace {
    auto counter_val(int k) -> int { return (0 <= k && k < to_signed(nb_registers)) ? k : -1; }
} // namespace

SpecialHash::SpecialHash(const std::string &str) {
    auto     s = str.c_str();
    Splitter S(s);
    auto     n = S.count();
    key.reserve(n);
    value.reserve(n);
    size = n;
    for (size_t j = 0; j < size; j++) {
        auto [a, b] = S.extract_keyval();
        key.push_back(a);
        value.push_back(b);
    }
}

// Return the value associated to the key x, or empty string if not found.
auto SpecialHash::find(String x) const -> std::string {
    for (size_t i = 0; i < size; i++)
        if (key[i] == x) return value[i];
    return "";
}

// Hack. Returns -1 in case no counter was given
// counter=12345 was given, this is out_of_range,  counter=foo is given
// but \c@foo is not a counter.
// Otherwise, returns the register number of the counter
auto SpecialHash::find_counter() const -> int {
    std::string s = find("counter");
    if (s.empty()) return -1;
    if (tralics_ns::only_digits(s)) return counter_val(std::stoi(s));
    Buffer &B = the_parser.hash_table.my_buffer();
    B << bf_reset << "c@" << s;
    Token t  = the_parser.hash_table.locate(B);
    auto  cs = t.eqtb_loc();
    if (the_parser.hash_table.eqtb[cs].cmd != assign_int_cmd) return -1;
    return counter_val(the_parser.hash_table.eqtb[cs].chr - count_reg_offset);
}
