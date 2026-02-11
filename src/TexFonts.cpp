#include "tralics/TexFonts.h"
#include "tralics/Parser.h"
#include <spdlog/fmt/fmt.h>

// Finds a font given by name and size, or creates one if needed
auto TexFonts::find_font(const TexFont &f) -> size_t {
    if (auto it = find(begin(), end(), f); it != end()) return to_unsigned(it - begin());
    push_back(f);
    return size() - 1;
}

// True if k is a valid font ID
// TODO: Replace that with size_t or std::optional<size_t>
auto TexFonts::is_valid(long k) -> bool { return (k >= 0) && (k < to_signed(size())); }

// Returns name of font
auto TexFonts::name(long k) -> std::string {
    if (!is_valid(k)) return "";
    return at(to_unsigned(k)).name;
}

// Returns name of font
void TexFonts::full_name(Buffer &B, long k) {
    if (!is_valid(k)) return;
    B.append(at(to_unsigned(k)).name);
    if (at(to_unsigned(k)).scaled_val != 0) B.format(" scaled {}", at(to_unsigned(k)).scaled_val);
    if (at(to_unsigned(k)).at_val != 0) {
        B.append(" at ");
        B.push_back(ScaledInt(at(to_unsigned(k)).at_val), glue_spec_pt);
    }
}

// Returns an integer parameter for a font
auto TexFonts::get_int_param(long ft, subtypes pos) -> long {
    if (!is_valid(ft)) return -1;
    if (pos == 0) return at(to_unsigned(ft)).hyphen_char;
    return at(to_unsigned(ft)).skew_char;
}

// Returns a dimension parameter for a font
auto TexFonts::get_dimen_param(long ft, long pos) -> ScaledInt {
    if (!is_valid(ft)) return ScaledInt{0};
    if (pos < 0 || to_unsigned(pos) >= at(to_unsigned(ft)).size()) return ScaledInt{0};
    return at(to_unsigned(ft))[to_unsigned(pos)];
}

// Sets an integer parameter for a font
void TexFonts::set_int_param(long ft, subtypes pos, long v) {
    if (!is_valid(ft)) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    if (pos == 0)
        at(to_unsigned(ft)).hyphen_char = v;
    else
        at(to_unsigned(ft)).skew_char = v;
}

// Sets a dimension parameter for a font
void TexFonts::set_dimen_param(long ft, long p, ScaledInt v) {
    if (!is_valid(ft) || p < 0 || p > 100000) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    auto up  = to_unsigned(p);
    auto uft = to_unsigned(ft);
    if (up >= at(uft).size()) at(uft).resize(up + 1);
    at(uft)[up] = v;
}
