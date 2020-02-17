#pragma once

#include "types.h"

// This is the number of characters; see txtoken.h
// This is also the size of tables that remember character properties
inline constexpr uint nb_characters = 65536;

// When the parser sees a character C with a catcode B then
// We have then cur_tok = nb_characters* B + C  < eqtb_offset
inline constexpr uint eqtb_offset = 16 * nb_characters;

// In all other cases, cur_tok >= eqtb_offset
// and cur_tok-eqtb_offset is the eqtb location of cur_tok.
// In the case of an active character C, we have
// cur_tok=eqtb_offset +C < single_offset
inline constexpr uint single_offset = nb_characters + eqtb_offset;

// In the case of \C (mono char), we have
// cur_tok = single_offset +C  < first_multitok_val
// If cur_tok < single_offset, it is one of the above cases, and C can be
// obtained by taking the value modulo nb_characters.
inline constexpr uint first_multitok_val = nb_characters + single_offset;

// first_multitok_val is the location of an illegal token
// After that comes a token with an empty name
// In all other cases cur_tok >=   hash_offset
// and cur_tok - hash_offset is the hashtable address of the token
inline constexpr uint null_tok_val = first_multitok_val + 1;
inline constexpr uint hash_offset  = null_tok_val + 1;

// For \noexpand hack
inline constexpr uint special_relax = nb_characters + 1;

inline constexpr uint scan_char_num_max = nb_characters - 1; // nb_characters-1  is the max

// This is the number of other registers
inline constexpr uint nb_registers = 1024;
