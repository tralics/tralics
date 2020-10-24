#pragma once
#include "types.h"

inline constexpr unsigned max_encoding        = 34;
inline constexpr unsigned max_newcolumn_loops = 10000; // max nb of iterations
inline constexpr unsigned nb_characters       = 65536; // Number of characters and size of char-index tables
inline constexpr unsigned nb_mathchars        = 128;   // This is the number of slots for Mathml characters
inline constexpr unsigned nb_newcolumn        = 128;   // Max char for newcolumntype
inline constexpr unsigned nb_registers        = 1024;  // This is the number of other registers
inline constexpr unsigned nb_shortverb_values = 256;   // This is the size of the table for shortverb
inline constexpr unsigned nb_simplemath       = 128;   // Max char for which $x$ is trivial math
inline constexpr unsigned nb_xspace_values    = 128;   // This is the size of the table ok_for_xspace

inline constexpr int bib_hash_mod  = 6397;
inline constexpr int fi_family_mul = 64;
inline constexpr int fi_series_mul = 256;
inline constexpr int fi_shape_mul  = 16;
inline constexpr int fi_size_mul   = 2048;
inline constexpr int lmaxchar      = 256;
inline constexpr int max_dimension = 07777777777;
inline constexpr int max_integer   = 017777777777;

// When the parser sees a character C with a catcode B then
// We have then cur_tok = nb_characters* B + C  < eqtb_offset
inline constexpr unsigned eqtb_offset = 16 * nb_characters;

// In all other cases, cur_tok >= eqtb_offset
// and cur_tok-eqtb_offset is the eqtb location of cur_tok.
// In the case of an active character C, we have
// cur_tok=eqtb_offset +C < single_offset
inline constexpr unsigned single_offset = nb_characters + eqtb_offset;

// In the case of \C (mono char), we have
// cur_tok = single_offset +C  < first_multitok_val
// If cur_tok < single_offset, it is one of the above cases, and C can be
// obtained by taking the value modulo nb_characters.
inline constexpr unsigned first_multitok_val = nb_characters + single_offset;

// first_multitok_val is the location of an illegal token
// After that comes a token with an empty name
// In all other cases cur_tok >=   hash_offset
// and cur_tok - hash_offset is the hashtable address of the token
inline constexpr unsigned null_tok_val = first_multitok_val + 1;
inline constexpr unsigned hash_offset  = null_tok_val + 1;

inline constexpr unsigned special_relax     = nb_characters + 1; // For \noexpand hack
inline constexpr unsigned scan_char_num_max = nb_characters - 1; // nb_characters-1  is the max

// This is the size of the accent table.
// If you want to put an accent on a character with code >128,
// you must remap it
inline constexpr unsigned nb_accents = 128;
