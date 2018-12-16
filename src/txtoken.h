// -*- C++ -*-
// $Id: txtoken.h,v 2.27 2008/11/12 09:43:57 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)



// Notes about tokens and characters.
// Since version 2.8.6, the internql encoding is 16 bit UTF8
// A character produced by ^^^^ABCD has the same status as an ASCII
// character concerning tokens; for this reason, the implementation changed

// This constant holds the number of possible 16 bit characters.
// It is defined in txvars.h
//    static const uint nb_characters = 65536;
// This is the maximum value of a char+cat pair 
static const uint Lastchar = 16*nb_characters-1;
// We follow Knuth's notations, and define eqtb_offset differently later
// but the value is the same as shown here
//   static const uint eqtb_offset = 16*nb_characters;

// When the parser sees a character with a catcode,
// it puts the character in cur_chr, the catcode in cur_cmd, and cur_cs is 0.
// We have then cur_tok = nb_characters*chur_cmd + cur_chr

// In the case of a command we have cur_tok>Lastchar.
// and cur_cs = cur_tok-Lastchar (this is >0).
// The quantity cur_tok-eqtb_offset is the location in the eqtb table.
// In the case of an active character C, we have
// cur_tok=eqtb_offset +C
// in the case of \C (mono char), we have
// cur_tok = eqtb_offset+nb_characters +C.

// In the case of a normal character, an active character, a macro of the form
// \C, then cur_tok%nb_characters is the character.
// In the case of an empty macro, we have c
// cur_tok=eqtb_offset+nb_characters+nb_characters+1
// the value cur_tok=eqtb_offset+nb_characters+nb_characters+1 is special

// For \noexpand hack
static const uint special_relax = nb_characters+1;

static const uint scan_char_num_max=nb_characters-1; // nb_characters-1  is the max


// these values have to be added to cur_cs
static const uint active_base = 1;
static const uint single_base = active_base+nb_characters;
static const uint bad_cs = single_base+nb_characters;
static const uint null_cs = bad_cs +1;
static const uint hash_base = null_cs+1;

static const uint cs_token_flag = Lastchar;
static const uint cs_offset = hash_base + cs_token_flag;
static const uint single_offset = single_base + cs_token_flag;
static const uint eqtb_offset = active_base + cs_token_flag;

extern Token invalid_token;
extern Token runawayed_token;

// Max token value allowed in `\x.
static const int last_ok_for_char_cst=  Lastchar+nb_characters+nb_characters;

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
  symcodes cmd;
  subtypes chr;
  public:
  CmdChr(symcodes a, subtypes b) : cmd(a), chr(b) {}
  explicit CmdChr(uchar b) : cmd(other_catcode), chr(subtypes(b)) {}
  CmdChr() :cmd (invalid_cmd), chr(zero_code) {}
  void reset() { cmd = undef_cmd; chr = zero_code; }
  symcodes get_cmd() const { return cmd; }
  subtypes get_chr() const { return chr; }
  void set_cmd(symcodes A) { cmd = A; }
  void set_chr(subtypes B) { chr = B; }
  void set_cmd_chr(symcodes A, subtypes B) { cmd = A; chr = B; }
  Utf8Char char_val() const { return Utf8Char(chr); }
  bool is_letter() const { return cmd==letter_catcode; }
  bool is_digit() const { return '0'<= chr && chr <= '9'; }
  bool is_other() const { return cmd==other_catcode; }
  bool is_space() const { return cmd==space_catcode; }
  bool is_letter_other() const { return cmd==other_catcode ||cmd==letter_catcode ;  }
  bool is_relax() const { return cmd==relax_cmd; }
  bool is_open_brace() const { return cmd==open_catcode; }
  bool is_parameter() const { return cmd==parameter_catcode; }
  bool is_minus_sign() const { return cmd==other_catcode && chr=='-'; }
  bool is_undef() const { return cmd==undef_cmd; }
  bool is_user() const { return cmd>=user_cmd;}
  bool is_cst1_cmd() const { return cmd== cst1_cmd; }
  bool is_math_font() const { return cmd==math_font_cmd; }
  bool is_math_list() const { return cmd==math_list_cmd; }
  bool is_mathml() const { return cmd == math_xml_cmd; }
  bool is_expandable() const { return cmd>=max_command; }
  bool is_ok_for_the() const { return cmd>min_internal && cmd<max_internal; }
  bool is_ok_for_xspace() const;
  int val_as_digit() const { return chr-'0'; }
  void set_mathml() {  cmd = math_xml_cmd; }
  bool is_math_openclosebetween() const 
  { return cmd==mathbetween_cmd || cmd == mathopen_cmd || 
      cmd==mathclose_cmd; }
  String special_name() const;
  String name() const;
 private:
  String token_error_name() const;
  String token_fbox_name() const;
  String token_fancy_name() const;
  String token_linebreak_name() const;
  String token_fp_names() const;
  String token_fpi_names() const;
  String token_math_name() const;
  String token_unimp_name() const;
  String token_unimp_font_name() const;
  String token_trees_name() const;
  String token_soul_name() const;
  String token_oldfont_name() const;
  String token_noargfont_name() const;
  String token_argfont_name() const;
  String token_fontsize_name() const;
  String token_ignore_name() const;
  String token_ign1_name() const;
  String token_ign2_name() const;
  String token_line_name() const;
  String token_newcount_name() const;
  String token_index_name() const;
  String token_unbox_name() const;
  String token_extension_name() const;
  String token_xray_name() const;
  String token_move_name() const;
  String token_makebox_name() const;
  String token_leader_name() const;
  String token_assigntoks_name() const;
  String token_assignint_name() const;
  String token_assigndimen_name() const;
  String token_assignglue_name() const;
  String token_assignmuglue_name() const;
  String token_setpagedimen_name() const;
  String token_setboxdimen_name() const;
  String token_shape_name() const;
  String token_defcode_name() const;
  String token_deffamily_name() const;
  String token_register_name() const;
  String token_prefix_name() const;
  String token_shorthand_name() const;
  String token_def_name() const;
  String token_setinteraction_name() const;
  String token_specialmath_name() const;
  String token_mathfont1_name() const;
  String token_mathfont2_name() const;
  String token_latexctr_name() const;
  String token_cite_name() const;
  String token_section_name() const;
  String token_box_name() const;
  String token_centering_name() const;
  String token_caseshift_name() const;
  String token_put_name() const;
  String token_dashline_name() const;
  String token_file_name() const;
  String token_ecenter_name() const;
  String token_efigure_name() const;
  String token_eignore_name() const;
  String token_eignorec_name() const;
  String token_eparticipant_name() const;
  String token_emath_name() const;
  String token_mathcomp_name() const;
  String token_big_name() const;
  String token_color_name() const;
  String token_style_name() const;
  String token_limits_name() const;
  String token_over_name() const;
  String token_lastitem_name() const;
  String token_convert_name() const;
  String token_accent_name() const;
  String token_counter_name() const;
  String token_iftest_name() const;
  String token_mark_name() const;
  String token_cst_name() const;
  String token_xkeyval_name() const;
  String token_ltfont_name() const;
  String cmd_special_name() const;
  String specchar_cmd_name() const;
  String token_for_name() const;
  String token_monthday_name() const;
 public:
  String token_fiorelse_name() const;

};


class Token {
  uint val;
 public:
  explicit Token(uint x) : val(x) {}
  Token(spec_offsets a, Utf8Char b) : val(a+b.get_value()) {}
  Token(spec_offsets a, uchar b) : val(a+b) {}
  explicit Token(Utf8Char c) : val( c.get_value() +single_base + cs_token_flag) {}
  Token() : val(0) {}

  uint get_val() const { return val; } 

  void kill() {  val = 0; }
  void from_cmd_chr(CmdChr X) { 
    val = nb_characters*X.get_cmd()+X.char_val().get_value(); }
  void from_cs(uint cs) { val = cs+cs_token_flag; }
  void from_hash(uint cs) { val = cs+cs_offset; }
  uint to_cs() const { return val-cs_token_flag; }
  int eqtb_loc() const { return val-eqtb_offset; }
  int hash_loc() const { return val-cs_offset; }
  symcodes cmd_val() const { return symcodes(val/nb_characters); }
  subtypes chr_val() const { return subtypes(val%nb_characters); }
  Utf8Char char_val() const { return Utf8Char (val%nb_characters); }
  bool is_a_brace() const { return OB_t_offset<=val && val<RB_limit; }
  bool is_a_left_brace() const { return OB_t_offset<=val && val< CB_t_offset; }
  bool is_OB_token() const { return OB_t_offset<=val && val< CB_t_offset; }
  bool is_CB_token() const { return CB_t_offset<=val && val< RB_limit; }
  bool is_math_shift() const { return dollar_t_offset<=val && val<dollar_limit;}
  bool is_space_token() const { 
    return val == space_token_val || val == newline_token_val;}
  bool is_only_space_token() const { return val == space_token_val; }
  bool is_zero_token() const { return val == other_t_offset+'0'; }
  bool is_equal_token() const { return val == other_t_offset+'='; }
  bool is_star_token() const { return val == other_t_offset+'*'; }
  bool is_slash_token() const { return val == other_t_offset+'/'; }
  bool is_exclam_token() const { return val == other_t_offset+'!'; }
  bool is_one_token() const { return val == other_t_offset+'1'; }
  bool is_plus_token() const { return val == other_t_offset+'+'; }
  bool is_minus_token() const { return val == other_t_offset+'-'; }
  bool is_hat_token() const { return val == hat_t_offset+'^'; }
  bool is_comma_token() const { return val == other_t_offset+','; }
  bool is_singlequote() const { return val == other_t_offset+'\''; }
  bool is_doublequote() const { return val == other_t_offset+'\"'; }
  bool is_backquote() const { return val == other_t_offset+'`'; }
  bool is_open_bracket() const { return val == other_t_offset+'['; }
  bool is_open_paren() const { return val == other_t_offset+'('; }
  bool is_close_paren() const { return val == other_t_offset+')'; }
  bool is_bs_oparen() const { return val == single_base +cs_token_flag+'('; }
  bool is_bs_cparen() const { return val == single_base +cs_token_flag+')'; }
  bool is_dot() const { return val == other_t_offset +'.'; }
  bool is_letter(uchar c) const { return val== uint(letter_t_offset) + c ; }
  bool is_digit_token() const { 
    return val>=other_t_offset+'0' && val<=other_t_offset+'9'; }
  bool is_lowercase_token() const { 
    return val>=letter_t_offset+'a' && val<=letter_t_offset+'z'; }
  bool is_null() const { return val==0; }
  bool is_invalid() const { return val==invalid_token.val; }
  bool is_valid() const { return val!=invalid_token.val; }

  bool not_a_cmd() const { return val < eqtb_offset; }
  bool is_a_char() const { return val < eqtb_offset; }
  bool char_or_active() const { return val <= single_offset; }
  int val_as_other() const { return val-other_t_offset; }
  int val_as_digit() const { return val-other_t_offset-'0'; }
  int val_as_letter() const { return val-letter_t_offset; }
  bool no_case_letter(char) const;
  int tex_is_digit(int);
  bool is_dec_separator() const { 
    return val==other_t_offset+',' || val==other_t_offset+'.'; }
  String to_string() const;
  String tok_to_str() const;
  String tok_to_str1() const;
  int get_ival() const { return val; }
  void testpach();
};


inline Token make_char_token(unsigned char c, int cat)
{
  return Token(nb_characters*cat+c);
}
