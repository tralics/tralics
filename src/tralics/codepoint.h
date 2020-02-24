#pragma once

// \todo This might better just vanish (and we would use char32_t everywhere in
// the code) if there is a reasonable path to do that while keeping all the
// helper functions.

struct codepoint {
    char32_t value;

    explicit codepoint(unsigned int x = 0) : value(x) {}

    [[nodiscard]] auto is_ascii() const -> bool { return value < 128; }
    [[nodiscard]] auto is_big() const -> bool { return value > 65535; }
    [[nodiscard]] auto is_control() const -> bool { return value < 32; }
    [[nodiscard]] auto is_delete() const -> bool { return value == 127; }
    [[nodiscard]] auto is_digit() const -> bool { return '0' <= value && value <= '9'; }
    [[nodiscard]] auto is_Hex() const -> bool { return 'A' <= value && value <= 'F'; }
    [[nodiscard]] auto is_hex() const -> bool { return 'a' <= value && value <= 'f'; }
    [[nodiscard]] auto is_invalid() const -> bool { return value == 0xFFFF; }
    [[nodiscard]] auto is_letter() const -> bool { return is_ascii() && ::is_letter(value); }
    [[nodiscard]] auto is_lower_case() const -> bool { return 'a' <= value && value <= 'z'; }
    [[nodiscard]] auto is_null() const -> bool { return value == 0; }
    [[nodiscard]] auto is_small() const -> bool { return value < 256; }
    [[nodiscard]] auto is_space() const -> bool { return value == ' ' || value == '\t' || value == '\n'; }
    [[nodiscard]] auto is_upper_case() const -> bool { return 'A' <= value && value <= 'Z'; }
    [[nodiscard]] auto is_verybig() const -> bool { return value > 0x1FFFF; }
    [[nodiscard]] auto non_null() const -> bool { return value != 0; }
    [[nodiscard]] auto val_as_digit() const -> uint { return value - '0'; }
    [[nodiscard]] auto val_as_Hex() const -> uint { return value - 'A' + 10; }
    [[nodiscard]] auto val_as_hex() const -> uint { return value - 'a' + 10; }

    [[nodiscard]] auto hex_val() const -> int {
        if (is_digit()) return val_as_digit();
        if (is_hex()) return val_as_hex();
        return -1;
    }

    [[nodiscard]] auto to_lower() const -> codepoint {
        if (is_upper_case()) return codepoint(value + ('a' - 'A'));
        return *this;
    }

    void make_invalid() { value = 0xFFFF; } // Not a Unicode char
};

inline auto operator==(const codepoint &a, const codepoint &b) -> bool { return a.value == b.value; }
inline auto operator!=(const codepoint &a, const codepoint &b) -> bool { return a.value != b.value; }
inline auto operator==(const codepoint &a, const unsigned char &b) -> bool { return a.value == uint(b); }
inline auto operator!=(const codepoint &a, const unsigned char &b) -> bool { return a.value != uint(b); }
