#pragma once
#include "types.h"

inline constexpr uint max_newcolumn_loops = 10000; // max nb of iterations
inline constexpr uint nb_characters       = 65536; // Number of characters and size of char-index tables
inline constexpr uint nb_mathchars        = 128;   // This is the number of slots for Mathml characters
inline constexpr uint nb_newcolumn        = 128;   // Max char for newcolumntype
inline constexpr uint nb_registers        = 1024;  // This is the number of other registers
inline constexpr uint nb_shortverb_values = 256;   // This is the size of the table for shortverb
inline constexpr uint nb_simplemath       = 128;   // Max char for which $x$ is trivial math
inline constexpr uint nb_xspace_values    = 128;   // This is the size of the table ok_for_xspace

inline constexpr int bib_hash_mod  = 6397;
inline constexpr int lmaxchar      = 256;
inline constexpr int max_dimension = 07777777777;
inline constexpr int max_encoding  = 34;
inline constexpr int max_integer   = 017777777777;

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

inline constexpr uint special_relax     = nb_characters + 1; // For \noexpand hack
inline constexpr uint scan_char_num_max = nb_characters - 1; // nb_characters-1  is the max

// This is the size of the accent table.
// If you want to put an accent on a character with code >128,
// you must remap it
inline constexpr uint nb_accents = 128;

// There are two hash tables. In the current version, they have the same size
// Size of eqtb should be the same (??)
inline constexpr int hash_prime = 40009;
inline constexpr int hash_size  = 50000;
// Size of eqtb. This +100 is strange.
// The big part of the table
inline constexpr int eqtb_size = hash_size + 2 * nb_characters + 100;
