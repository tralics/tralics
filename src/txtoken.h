// -*- C++ -*-
// $Id: txtoken.h,v 2.37 2015/11/09 10:02:25 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007, 2015

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)



// Notes about tokens and characters.
// Since version 2.8.6, the internal encoding is 16 bit UTF8
// A character produced by ^^^^ABCD has the same status as an ASCII
// character concerning tokens; for this reason, the implementation changed

// This constant holds the number of possible 16 bit characters.
// It is defined in txvars.h
//    static const uint nb_characters = 65536;
// In Tex, the eqtb location of cur_tok is in cur_cs. This is 0 if the token is
// has no eqtb location. This global variable is not needed in Tralics anymore;
// so the first slot in eqtb is at position 0.
// The following are no more needed in Tralics version 2.15.4 
//    static const uint active_base = 1;
//    static const uint single_base = active_base+nb_characters;
//    static const uint bad_cs = single_base+nb_characters;
//    static const uint null_cs = bad_cs +1;
//    static const uint hash_base = null_cs+1;

// When the parser sees a character C with a catcode B then
// We have then cur_tok = nb_characters* B + C  < eqtb_offset
static const uint eqtb_offset = 16*nb_characters;

// In all other cases, cur_tok >= eqtb_offset
// and cur_tok-eqtb_offset is the eqtb location of cur_tok.
// In the case of an active character C, we have
// cur_tok=eqtb_offset +C < single_offset 

static const uint single_offset = nb_characters + eqtb_offset;

// In the case of \C (mono char), we have
// cur_tok = single_offset +C  < first_multitok_val
// If cur_tok < single_offset, it is one of the above cases, and C can be
// obtained by taking the value modulo nb_characters.
static const uint first_multitok_val = nb_characters + single_offset;

// first_multitok_val is the location of an illegal token
// After that comes a token with an empty name
// In all other cases cur_tok >=   hash_offset
// and cur_tok - hash_offset is the hashtable address of the token
static const uint null_tok_val = first_multitok_val + 1;
static const uint hash_offset = null_tok_val +1;


// For \noexpand hack
static const uint special_relax = nb_characters+1;

static const uint scan_char_num_max=nb_characters-1; // nb_characters-1  is the max

// nb_characters*cur_cmd for catcode 11 and 12.
enum spec_offsets {
  letter_t_offset = letter_catcode*nb_characters,
  other_t_offset = other_catcode*nb_characters,
  hat_t_offset = hat_catcode*nb_characters,
  space_t_offset = space_catcode*nb_characters,
  ampersand_t_offset = alignment_catcode*nb_characters,
  RB_limit = 3*nb_characters,
  CB_t_offset = 2*nb_characters,
  OB_t_offset = 1*nb_characters,
  eol_t_offset = nb_characters*eol_catcode,
  dollar_t_offset = nb_characters*dollar_catcode,
  dollar_limit = nb_characters*(dollar_catcode+1),
};
static const uint space_token_val = space_t_offset + ' ';
static const uint newline_token_val = space_t_offset + '\n';


// cmd, chr pair; The subtype can have 16bits
class CmdChr {
  symcodes cmd{invalid_cmd};
  subtypes chr{zero_code};

public:
  CmdChr(symcodes a, subtypes b) : cmd(a), chr(b) {}
  explicit CmdChr(uchar b) : cmd(other_catcode), chr(subtypes(b)) {}
  CmdChr() {}
  void reset() { cmd = undef_cmd; chr = zero_code; }
  void kill() { cmd = invalid_cmd; chr = zero_code; }
  auto get_cmd() const -> symcodes { return cmd; }
  auto get_chr() const -> subtypes { return chr; }
  void set_cmd(symcodes A) { cmd = A; }
  void set_chr(subtypes B) { chr = B; }
  void set_cmd_chr(symcodes A, subtypes B) { cmd = A; chr = B; }
  auto char_val() const -> Utf8Char { return Utf8Char(chr); }
  auto is_letter() const -> bool { return cmd == letter_catcode; }
  auto is_digit() const -> bool { return '0' <= chr && chr <= '9'; }
  auto is_other() const -> bool { return cmd == other_catcode; }
  auto is_space() const -> bool { return cmd == space_catcode; }
  auto is_letter_other() const -> bool {
    return cmd == other_catcode || cmd == letter_catcode;
  }
  auto is_relax() const -> bool { return cmd == relax_cmd; }
  auto is_undef() const -> bool { return cmd == undef_cmd; }
  auto is_undef_or_relax() const -> bool {
    return cmd == undef_cmd || cmd == relax_cmd;
  }
  auto is_outer() const -> bool { return cmd >= usero_cmd; }
  auto is_open_brace() const -> bool { return cmd == open_catcode; }
  auto is_parameter() const -> bool { return cmd == parameter_catcode; }
  auto is_minus_sign() const -> bool {
    return cmd == other_catcode && chr == '-';
  }
  auto is_user() const -> bool { return cmd >= user_cmd; }
  auto is_cst1_cmd() const -> bool { return cmd == cst1_cmd; }
  auto is_math_font() const -> bool { return cmd == math_font_cmd; }
  auto is_math_list() const -> bool { return cmd == math_list_cmd; }
  auto is_mathml() const -> bool { return cmd == math_xml_cmd; }
  auto is_expandable() const -> bool { return cmd >= max_command; }
  auto is_ok_for_the() const -> bool {
    return cmd > min_internal && cmd < max_internal;
  }
  auto is_ok_for_xspace() const -> bool;
  auto is_protected() const -> bool {
    return cmd==userp_cmd || cmd==userlp_cmd || cmd==userpo_cmd || cmd==userlpo_cmd;
  }
  auto val_as_digit() const -> int { return chr - '0'; }
  void set_mathml() {  cmd = math_xml_cmd; }
  auto is_math_openclosebetween() const -> bool {
    return cmd == mathbetween_cmd || cmd == mathopen_cmd ||
           cmd == mathclose_cmd;
  }
  auto special_name() const -> String;
  auto name() const -> String;
  auto is_single_quote() const -> bool {
    return cmd == other_catcode && chr == '\'';
  }

 private:
   auto token_error_name() const -> String;
   auto token_fbox_name() const -> String;
   auto token_fancy_name() const -> String;
   auto token_linebreak_name() const -> String;
   auto token_fp_names() const -> String;
   auto token_fpi_names() const -> String;
   auto token_math_name() const -> String;
   auto token_unimp_name() const -> String;
   auto token_unimp_font_name() const -> String;
   auto token_trees_name() const -> String;
   auto token_soul_name() const -> String;
   auto token_oldfont_name() const -> String;
   auto token_noargfont_name() const -> String;
   auto token_argfont_name() const -> String;
   auto token_fontsize_name() const -> String;
   auto token_ignore_name() const -> String;
   auto token_ign1_name() const -> String;
   auto token_ign2_name() const -> String;
   auto token_line_name() const -> String;
   auto token_newcount_name() const -> String;
   auto token_index_name() const -> String;
   auto token_unbox_name() const -> String;
   auto token_extension_name() const -> String;
   auto token_xray_name() const -> String;
   auto token_move_name() const -> String;
   auto token_makebox_name() const -> String;
   auto token_leader_name() const -> String;
   auto token_assigntoks_name() const -> String;
   auto token_assignint_name() const -> String;
   auto token_assigndimen_name() const -> String;
   auto token_assignglue_name() const -> String;
   auto token_assignmuglue_name() const -> String;
   auto token_setpagedimen_name() const -> String;
   auto token_setboxdimen_name() const -> String;
   auto token_shape_name() const -> String;
   auto token_defcode_name() const -> String;
   auto token_deffamily_name() const -> String;
   auto token_register_name() const -> String;
   auto token_prefix_name() const -> String;
   auto token_shorthand_name() const -> String;
   auto token_def_name() const -> String;
   auto token_setinteraction_name() const -> String;
   auto token_specialmath_name() const -> String;
   auto token_mathfont1_name() const -> String;
   auto token_mathfont2_name() const -> String;
   auto token_latexctr_name() const -> String;
   auto token_cite_name() const -> String;
   auto token_section_name() const -> String;
   auto token_box_name() const -> String;
   auto token_centering_name() const -> String;
   auto token_caseshift_name() const -> String;
   auto token_put_name() const -> String;
   auto token_dashline_name() const -> String;
   auto token_file_name() const -> String;
   auto token_ecenter_name() const -> String;
   auto token_efigure_name() const -> String;
   auto token_eignore_name() const -> String;
   auto token_eignorec_name() const -> String;
   auto token_eparticipant_name() const -> String;
   auto token_emath_name() const -> String;
   auto token_mathcomp_name() const -> String;
   auto token_big_name() const -> String;
   auto token_color_name() const -> String;
   auto token_style_name() const -> String;
   auto token_limits_name() const -> String;
   auto token_over_name() const -> String;
   auto token_lastitem_name() const -> String;
   auto token_convert_name() const -> String;
   auto token_accent_name() const -> String;
   auto token_counter_name() const -> String;
   auto token_iftest_name() const -> String;
   auto token_mark_name() const -> String;
   auto token_cst_name() const -> String;
   auto token_xkeyval_name() const -> String;
   auto token_ltfont_name() const -> String;
   auto cmd_special_name() const -> String;
   auto specchar_cmd_name() const -> String;
   auto token_for_name() const -> String;
   auto token_monthday_name() const -> String;
   auto l3_expand_aux_name() const -> String;
   auto l3_expand_base_name() const -> String;
   auto l3_ifx_name() const -> String;
   auto l3str_ifeq_name() const -> String;
   auto l3str_case_name() const -> String;
   auto tipa_name() const -> String;
   auto l3_set_cat_name() const -> String;
   auto l3_set_num_name() const -> String;
   auto token_if_name() const -> String;
   auto cat_ifeq_name() const -> String;
   auto l3_tl_basic_name() const -> String;
   auto tl_concat_name() const -> String;
   auto tl_set_name() const -> String;
   auto tl_put_left_name() const -> String;
   auto l3_rescan_name() const -> String;

 public:
   auto token_fiorelse_name() const -> String;
};


class Token {
  uint val{0};

public:
  explicit Token(uint x) : val(x) {}
  Token(spec_offsets a, Utf8Char b) : val(a+b.get_value()) {}
  Token(spec_offsets a, uchar b) : val(a+b) {}
  explicit Token(Utf8Char c) : val( c.get_value() +single_offset) {}
  Token() {}

  auto get_val() const -> uint { return val; }

  void kill() {  val = 0; }
  void from_cmd_chr(CmdChr X) { 
    val = nb_characters*X.get_cmd()+X.char_val().get_value(); }
  void active_char(uint cs) { val = cs+eqtb_offset; }
  auto eqtb_loc() const -> int { return val - eqtb_offset; }
  auto hash_loc() const -> int { return val - hash_offset; }
  auto is_in_hash() const -> bool { return val >= hash_offset; }
  auto cmd_val() const -> symcodes { return symcodes(val / nb_characters); }
  auto chr_val() const -> subtypes { return subtypes(val % nb_characters); }
  auto char_val() const -> Utf8Char { return Utf8Char(val % nb_characters); }
  auto is_a_brace() const -> bool {
    return OB_t_offset <= val && val < RB_limit;
  }
  auto is_a_left_brace() const -> bool {
    return OB_t_offset <= val && val < CB_t_offset;
  }
  auto is_OB_token() const -> bool {
    return OB_t_offset <= val && val < CB_t_offset;
  }
  auto is_CB_token() const -> bool {
    return CB_t_offset <= val && val < RB_limit;
  }
  auto is_math_shift() const -> bool {
    return dollar_t_offset <= val && val < dollar_limit;
  }
  auto is_space_token() const -> bool {
    return val == space_token_val || val == newline_token_val;
  }
  auto is_same_token(const Token &x) const -> bool {
    return val== x.val || (is_space_token() && x.is_space_token());
  }
  auto is_only_space_token() const -> bool { return val == space_token_val; }
  auto is_zero_token() const -> bool { return val == other_t_offset + '0'; }
  auto is_equal_token() const -> bool { return val == other_t_offset + '='; }
  auto is_star_token() const -> bool { return val == other_t_offset + '*'; }
  auto is_slash_token() const -> bool { return val == other_t_offset + '/'; }
  auto is_exclam_token() const -> bool { return val == other_t_offset + '!'; }
  auto is_one_token() const -> bool { return val == other_t_offset + '1'; }
  auto is_plus_token() const -> bool { return val == other_t_offset + '+'; }
  auto is_minus_token() const -> bool { return val == other_t_offset + '-'; }
  auto is_hat_token() const -> bool { return val == hat_t_offset + '^'; }
  auto is_comma_token() const -> bool { return val == other_t_offset + ','; }
  auto is_singlequote() const -> bool { return val == other_t_offset + '\''; }
  auto is_doublequote() const -> bool { return val == other_t_offset + '\"'; }
  auto is_backquote() const -> bool { return val == other_t_offset + '`'; }
  auto is_open_bracket() const -> bool { return val == other_t_offset + '['; }
  auto is_open_paren() const -> bool { return val == other_t_offset + '('; }
  auto is_close_paren() const -> bool { return val == other_t_offset + ')'; }
  auto is_bs_oparen() const -> bool { return val == single_offset + '('; }
  auto is_bs_cparen() const -> bool { return val == single_offset + ')'; }
  auto is_dot() const -> bool { return val == other_t_offset + '.'; }
  auto is_letter(uchar c) const -> bool {
    return val == uint(letter_t_offset) + c;
  }
  auto is_digit_token() const -> bool {
    return val>=other_t_offset+'0' && val<=other_t_offset+'9';
  }
  auto is_lowercase_token() const -> bool {
    return val>=letter_t_offset+'a' && val<=letter_t_offset+'z';
  }
  auto is_null() const -> bool { return val == 0; }
  auto is_invalid() const -> bool { return val == 0; }
  auto is_valid() const -> bool { return val != 0; }
  auto not_a_cmd() const -> bool { return val < eqtb_offset; }
  auto is_a_char() const -> bool { return val < eqtb_offset; }
  auto active_or_single() const -> bool { return val < first_multitok_val; }
  auto char_or_active() const -> bool { return val < single_offset; }
  auto val_as_other() const -> int { return val - other_t_offset; }
  auto val_as_digit() const -> int { return val - other_t_offset - '0'; }
  auto val_as_letter() const -> int { return val - letter_t_offset; }
  auto no_case_letter(char) const -> bool;
  auto tex_is_digit(int) -> int;
  auto is_dec_separator() const -> bool {
    return val==other_t_offset+',' || val==other_t_offset+'.';
  }
  auto to_string() const -> String;
  auto tok_to_str() const -> String;
  auto tok_to_str1() const -> String;
  auto get_ival() const -> int { return val; }
  void testpach();
};

inline auto make_char_token(unsigned char c, int cat) -> Token {
  return Token(nb_characters*cat+c);
}
