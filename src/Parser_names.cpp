#include "tralics/Parser.h"
#include "tralics/Symcode.h"

[[nodiscard]] auto token_math_name(subtypes c) -> std::string;

void Parser::boot_symcode_names() {
    Symcode::get(mathbin_cmd).name_fn     = token_math_name;
    Symcode::get(mathrel_cmd).name_fn     = token_math_name;
    Symcode::get(mathinner_cmd).name_fn   = token_math_name;
    Symcode::get(mathbetween_cmd).name_fn = token_math_name;
    Symcode::get(mathopen_cmd).name_fn    = token_math_name;
    Symcode::get(mathclose_cmd).name_fn   = token_math_name;
    Symcode::get(mathspace_cmd).name_fn   = token_math_name;
    Symcode::get(mathord_cmd).name_fn     = token_math_name;
    Symcode::get(mathordb_cmd).name_fn    = token_math_name;
    Symcode::get(mathop_cmd).name_fn      = token_math_name;
    Symcode::get(mathopn_cmd).name_fn     = token_math_name;
}
