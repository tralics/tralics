#include "tralics/TexFonts.h"
#include "tralics/Parser.h"
#include <optional>
#include <spdlog/fmt/fmt.h>

namespace {
    auto font_index(long k, size_t n) -> std::optional<size_t> {
        if (k < 0) return std::nullopt;
        auto idx = to_unsigned(k);
        if (idx >= n) return std::nullopt;
        return idx;
    }
} // namespace

// Finds a font given by name and size, or creates one if needed
auto TexFonts::find_font(const TexFont &f) -> size_t {
    if (auto it = find(begin(), end(), f); it != end()) return to_unsigned(it - begin());
    push_back(f);
    return size() - 1;
}

// Returns name of font
auto TexFonts::name(long k) -> std::string {
    auto idx = font_index(k, size());
    if (!idx) return "";
    return at(*idx).name;
}

// Returns name of font
void TexFonts::full_name(Buffer &B, long k) {
    auto idx = font_index(k, size());
    if (!idx) return;
    B.append(at(*idx).name);
    if (at(*idx).scaled_val != 0) B.format(" scaled {}", at(*idx).scaled_val);
    if (at(*idx).at_val != 0) {
        B.append(" at ");
        B.push_back(ScaledInt(at(*idx).at_val), glue_spec_pt);
    }
}

// Returns an integer parameter for a font
auto TexFonts::get_int_param(long ft, subtypes pos) -> long {
    auto idx = font_index(ft, size());
    if (!idx) return -1;
    if (pos == 0) return at(*idx).hyphen_char;
    return at(*idx).skew_char;
}

// Returns a dimension parameter for a font
auto TexFonts::get_dimen_param(long ft, long pos) -> ScaledInt {
    auto idx = font_index(ft, size());
    if (!idx) return ScaledInt{0};
    if (pos < 0 || to_unsigned(pos) >= at(*idx).size()) return ScaledInt{0};
    return at(*idx)[to_unsigned(pos)];
}

// Sets an integer parameter for a font
void TexFonts::set_int_param(long ft, subtypes pos, long v) {
    auto idx = font_index(ft, size());
    if (!idx) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    if (pos == 0)
        at(*idx).hyphen_char = v;
    else
        at(*idx).skew_char = v;
}

// Sets a dimension parameter for a font
void TexFonts::set_dimen_param(long ft, long p, ScaledInt v) {
    auto idx = font_index(ft, size());
    if (!idx || p < 0 || p > 100000) {
        the_parser.parse_error("attempt to modify unexistent font param");
        return;
    }
    auto up  = to_unsigned(p);
    if (up >= at(*idx).size()) at(*idx).resize(up + 1);
    at(*idx)[up] = v;
}
