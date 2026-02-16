#include "tralics/Token.h"
#include "tralics/Buffer.h"
#include "tralics/MainClass.h"

// Returns a temporary string: the name of the token
// This is used for printing errors in the transcript file
// Uses the function below, except for characters
auto Token::tok_to_str() const -> std::string {
    Buffer B;
    if (!is_a_char() || cmd_val() == eol_catcode) {
        B.push_back(*this);
        return std::move(B);
    }
    unsigned cat      = cmd_val();
    char32_t c        = char_val();
    bool     good_cat = false;
    if ((c >= 128) && cat == 12) good_cat = true;
    if ((std::isalpha(static_cast<int>(c)) != 0) && cat == 11) good_cat = true;
    if (good_cat)
        B.out_log(c);
    else {
        B.append("{Character ");
        B.out_log(c);
        B.format(" of catcode {}}}", cat);
    }
    return std::move(B);
}

// If this is a TeX token representing a valid digit in base radix,
// returns the value,
// otherwise return -1.
auto Token::tex_is_digit(unsigned radix) const -> long {
    auto w = val_as_other();
    if ('0' <= w && w <= radix + '0' && w <= '9') // do not use is_digit...
        return to_signed(w) - '0';
    if (radix != 16) return -1;
    if (w <= 'F' && w >= 'A') return to_signed(w) - 'A' + 10;
    w = val_as_letter(); // allow category code 11 digits
    if (w <= 'F' && w >= 'A') return to_signed(w) - 'A' + 10;
    return -1;
}

// True if the Token is upper case or lower case x
auto Token::no_case_letter(char x) const -> bool {
    if (cmd_val() != letter_catcode) return false;
    auto c = to_signed(val_as_letter());
    if (c == x) return true;
    if (c == x + 'A' - 'a') return true;
    return false;
}

auto operator<<(std::ostream &fp, Token x) -> std::ostream & { return fp << x.tok_to_str(); }
