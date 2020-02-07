// -*- C++ -*-
// $Id: txmath.h,v 2.76 2015/11/24 17:08:44 grimm Exp $
// TRALICS, copyright (C) 2003-2004 2006, 2007 INRIA, Jos\'e Grimm

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

class MathQ;
class MathP;
class MathF;
using MathQList = std::list<MathQ>;
 
// A math object is a strange thing:
// a math_elt object has 4 fields, one is a pointer to another object.
// Other 3 objects are : cmd, chr, token in some cases.
// it can be cmd, chr, font (if cmd==12 or cmd==11), and font is one of
//  math_f_it,math_f_rm,math_f_cal,math_f_bb,math_f_sf,math_f_tt,math_f_bf,
// it can be math_list_cmd, pos, type, where pos is a position into a
//    table of math lists, and type is one of
//     math_open_cd, math_dollar_cd, math_ddollar_cd,math_env_cd, math_LR_cd,
//     math_hbox_cd,math_argument_cd,
// it can be math_xml_cmd, pos, type, where pos is a position into a
//    table of XML elements.

inline auto math_to_sub(math_list_type x) -> subtypes {
  return subtypes(x-fml_offset);
}
inline auto sub_to_math(subtypes x) -> math_list_type {
  return math_list_type(x+fml_offset);
}

inline auto is_m_font(symcodes cmd) -> bool {
  return cmd==math_font_cmd || cmd==oldfont_cmd || cmd==argfont_cmd || 
    cmd==noargfont_cmd;
}

class MathElt {
  CmdChr val;
  subtypes Font;
public:
  MathElt(CmdChr X, subtypes c) :  val(X),Font(c) {}
  MathElt(subtypes a,math_types b) : 
    val(CmdChr(math_xml_cmd,a)), Font(subtypes(b))   {  }
  MathElt(Xmlp, math_types);
  MathElt(Xmlp A, int b, math_types c);
  // access to elements
  auto get_xmltype() const -> math_types { return math_types(Font); }
  void set_xmltype(math_types x) { Font = subtypes(x); }
  auto get_font() const -> subtypes { return Font; }
  auto get_lcmd() const -> math_list_type { return math_list_type(Font); }
  auto get_fml_subtype() const -> subtypes;
  auto get_cmd_chr() const -> const CmdChr & { return val; }
  auto get_char() const -> Utf8Char { return val.char_val(); }
  auto get_chr() const -> subtypes { return val.get_chr(); }
  void set_chr(subtypes c) { val.set_chr(c); }
  auto get_cmd() const -> symcodes { return val.get_cmd(); }
  void set_cmd(symcodes c) { val.set_cmd(c); }
  auto get_list() const -> Math &;
  auto get_xml_val() const -> Xmlp;

  // some tests on the elements
  auto is_list() const -> bool {
    return val.is_math_list() && Font == subtypes(math_open_cd);
  }
  auto is_hbox() const -> bool {
    return val.is_math_list() && Font == subtypes(math_hbox_cd);
  }
  auto is_space() const -> bool { return val.is_space(); }
  auto is_digit() const -> bool;
  auto is_char() const -> bool {
    return is_space() || is_letter_token() || is_other_token();
  }
  auto is_letter_token() const -> bool { return val.is_letter(); }
  auto is_other_token() const -> bool { return val.is_other(); }
  auto is_star() const -> bool { return is_other_token() && get_char() == '*'; }
  auto is_bracket() const -> bool {
    return is_other_token() && get_char() == '[';
  }
  auto maybe_seq() const -> bool;
  auto maybe_seq(subtypes) const -> bool;
  auto maybe_iseq(subtypes) const -> bool;
  auto maybe_iseq() const -> bool;
  // other functions
  auto large2() -> del_pos;
  auto remove_prefix() const -> Xmlp;
  void cv_noMLt(); 
  void cv_noML(); 
  void cv_noMLt_special(); 
  void cv_noMLt_special0(); 
  void cv_noMLt_list(); 
  void cv_noML_special(); 
  void cv_noML_list();
  auto cv1(math_style, bool) -> MathElt;
  void change_type(int t);
  auto try_math_op() -> Xmlp;
  auto is_e_grave() const -> bool;
  auto special3() const -> Xmlp;
  void print() const;
  auto val_as_digit() const -> int { return val.val_as_digit(); }

private:
  void set_xml_subtype(math_types x) { set_xmltype(x); val.set_mathml(); }
  auto cv_char() -> MathElt;
  auto cv_cst() -> MathElt;
  auto cv_special(math_style) -> MathElt;
  auto cv_special1(math_style) -> MathElt;
  auto cv_list(math_style, bool) -> MathElt;
  auto cv_list_no() -> MathElt;
  auto cv_mi(math_style) -> MathElt;
  void cv1_err();
  void dump_for_err() const;
  auto is_accent() const -> bool { return val.is_mathml() && Font == 0; }
};

class MathIsSpace: public unary_function<MathElt,bool> {
public:
  explicit MathIsSpace() {}
  auto operator()(const MathElt &m) -> bool { return m.is_space(); }
};

class MathIsDollar: public unary_function<MathElt,bool> {
public:
  explicit MathIsDollar() {}
  auto operator()(const Token &m) -> bool { return m.is_math_shift(); }
};


class MathIsOver: public unary_function<MathElt,bool> {
public:
  explicit MathIsOver() {}
  auto operator()(const MathElt &m) -> bool { return m.get_cmd() == over_cmd; }
};


using MathList = std::list<MathElt>;
using const_math_iterator = std::list<MathElt>::const_iterator;
using math_iterator = std::list<MathElt>::iterator;

class Math {
  friend class MathHelper;
  friend class MathDataP;
  friend class MathElt;
  MathList value;
  math_list_type type{invalid_cd};
  subtypes sname{nomathenv_code};

public:
  Math() {}

public:
  auto duplicate(bool) const -> subtypes;
  auto back() -> MathElt & { return value.back(); }
  auto begin() const -> const_math_iterator { return value.begin(); }
  auto chars_to_mb(Buffer &, bool) const -> bool;
  auto chars_to_mb1(Buffer &) const -> bool;
  auto chars_to_mb2(Buffer &) const -> bool;
  auto chars_to_mb3() -> Istring;
  void clear() { value.clear(); }
  auto convert_math(math_style) -> Xmlp;
  auto convert_math_noML(name_positions, bool) -> Xmlp;
  void convert_math_noML0();
  void convert_math_noMLt0();
  auto convert_opname() -> string;
  auto convert_this_to_string(Buffer &) -> string;
  void destroy();
  auto empty() const -> bool { return value.empty(); }
  auto end() const -> const_math_iterator { return value.end(); }
  auto find_parens(MathQList &, bool) const -> bool;
  auto front() -> MathElt & { return value.front(); }
  auto front() const -> const MathElt & { return value.front(); }
  auto get_arg1() -> Math { return front().get_list(); }
  auto get_arg2() -> Math { return second_element().get_list(); }
  auto get_arg3() -> Math { return third_element().get_list(); }
  auto get_type() const -> math_list_type { return type; }
  auto get_sname() const -> subtypes { return sname; }
  auto get_name() const -> String;
  auto get_list(int) const -> Math &;
  void hack_type(int);
  auto has_type(int x) const -> bool { return type == x; }
  auto has_one_element() const -> bool;
  auto has_two_elements() const -> bool;
  void is_font_cmd1_list(const_math_iterator &B, const_math_iterator &E);
  auto length_one() const -> bool { return value.size() == 1; }
  auto M_array(bool, math_style) -> Xmlp;
  auto M_cv(math_style, int need_row) -> XmlAndType;
  void pop_back() { value.pop_back(); }
  void pop_front() { value.pop_front(); }
  void print() const;
  void push_back(CmdChr, subtypes);
  void push_back_list(subtypes X, math_list_type c);
  void push_back_font(subtypes X, subtypes c);
  void push_back(CmdChr);
  void push_back(MathElt x) { value.push_back(x); }
  void push_back(Xmlp, int, math_types);
  void push_front(CmdChr, subtypes);
  void remove_initial_group();
  void remove_last();
  void remove_spaces();
  auto second_element() const -> const MathElt &;
  void set_display_type() { type = math_ddollar_cd; }
  void set_env_name(int);
  void set_name(subtypes X) { sname = X; }
  void set_nondisplay_type() { type = math_dollar_cd; }
  void set_type(math_list_type c) { type = c; }
  auto third_element() const -> const MathElt &;
  auto trivial_math(int) -> Xmlp;
  auto trivial_math_index(symcodes) -> Xmlp;
  auto check_align() -> int;

private:
  
  void add_cur_cont();
  void add_cur_font();
  auto add_fence(bool, MathF &) -> bool;
  void concat(Xmlp);
  void concat_space(Xmlp);
  auto convert_cell(int &n, vector<AttList> &table, math_style) -> Xmlp;
  auto convert_char_seq(MathElt W) -> MathElt;
  auto convert_char_iseq(MathElt W, bool) -> MathElt;
  void fetch_rlc(vector<AttList>&);
  void find_paren0 (MathP&) const;
  auto finish_translate1(bool) -> bool;
  auto finish_translate2() -> bool;
  void handle_mbox(Math&);
  void handle_mbox_no();
  void handle_mbox_not();
  auto has_over() const -> bool;
  auto is_font_cmd1() -> bool;
  auto large1(MathElt &, math_style) -> Xmlp;
  auto M_cv0(math_style) -> XmlAndType;
  auto M_cv3(math_style) -> Math;
  void handle_cmd_Big(math_style);
  auto handle_cmd_Big_aux(math_style) -> bool;
  void cv_hspace_t(MathElt& cur);
  auto M_cvaux() -> Math;
  auto M_ref() -> Xmlp;
  auto M_mbox1(Buffer &, subtypes &) -> int;
  auto only_digits(Buffer &) const -> bool;
  void push_back(Math&);
  void push_back_small(Xmlp a);
  void push_front(Math&);
  void skip_initial_space();
  auto special1() const -> Xmlp;
  void special2(bool&,Xmlp&) const;
  auto split_as_array(vector<AttList> &table, math_style, bool) -> Xmlp;
  void remove_opt_arg (bool star);
  auto remove_req_arg() -> string;
  auto remove_req_arg_noerr() const -> string;
};



// This is a global object for math handling
// but math handling is not recursive. reset() is called on every formula.
class MathHelper {
  MathElt* free_list;  // free list
  bool current_mode; // display or not, needed for \label
  name_positions pos_att; // position attribute, inline or display
  bool seen_label; // do we see already have a label
  bool warned_label; // was the used warned for labels on this formula ?
  string label_val; // name of the label
  vector<string> multi_labels;
  vector<int> multi_labels_type;
  TokenList tag_list; // value of \tag{foo}, if given 
  bool is_tag_starred; // \tag or \tag* ?
  Xid cur_cell_id; // Id of current cell
  Xid cur_row_id; // Id of current row
  Xid cur_table_id; // Id of current table
  Xid cur_math_id; // Id of current math element
  Xid cur_formula_id; // Id of current formula
  Xid cur_texmath_id; // Id of current texmath
  int math_env_ctr; // two counters for environments
  int all_env_ctr;
  int last_ml_pos; 
  int eqnum_status; // how many numbers for this equation?
 public:
  MathHelper() {math_env_ctr = 0; all_env_ctr =0; }
  void reset_last_ml_pos () { last_ml_pos = 0; }
  auto end_of_row() -> bool;
  void dump_labels();
  void ml_check_labels();
  void new_label(string s, bool a);
  void ml_second_pass(Xmlp row,bool);
  void ml_last_pass(bool);
  void insert_special_tag(string s) {
    multi_labels [last_ml_pos-2] = s;
  }
  void new_multi_label(string s, int t) {
    multi_labels.push_back(s); multi_labels_type.push_back(t);  }
  auto get_multi_labels() -> vector<string> & { return multi_labels; }
  void finish_math_mem();
  void set_type(bool);
  auto get_eqnum_status() const -> int { return eqnum_status; }
  auto has_label() const -> bool {
    return seen_label || eqnum_status == 1 || eqnum_status == 3;
  }
  auto get_label_val() const -> string { return label_val; }
  void stats();
  auto get_pos_att() const -> name_positions { return pos_att; }
  void reset(bool);
  auto get_free_list() const -> MathElt * { return free_list; }
  void set_free_list(MathElt* x)  { free_list = x; }
  auto is_inline() const -> bool { return current_mode; }
  auto has_tag() const -> bool { return !tag_list.empty(); }
  void reset_tags() { tag_list = TokenList(); }
  void handle_tags();
  void add_tag(TokenList &L);
  void update_all_env_ctr(bool open) 
  { if(open) all_env_ctr ++; else all_env_ctr--; }
  void update_math_env_ctr(bool open) 
  { if(open) math_env_ctr ++; else math_env_ctr--; }
  auto get_math_env_ctr() const -> int { return math_env_ctr; }
  auto get_all_env_ctr() const -> int { return all_env_ctr; }
  void add_attribute(Istring,Istring,subtypes);
  auto get_cid() const -> Xid { return cur_cell_id; }
  auto get_rid() const -> Xid { return cur_row_id; }
  auto get_mid() const -> Xid { return cur_math_id; }
  auto get_fid() const -> Xid { return cur_formula_id; }
  auto get_tid() const -> Xid { return cur_texmath_id; }
  auto get_taid() const -> Xid { return cur_table_id; }
  void set_cid(Xid i) { cur_cell_id = i; }
  void set_rid(Xid i) { cur_row_id = i; }
  void set_taid(Xid i) { cur_table_id = i; }
  void starred_tag() { is_tag_starred = true; }
  void check_for_eqnum(subtypes, bool);
 private:
  void set_label(string s) { label_val = s; seen_label = true; }
};

// This is a global object for math handling
class MathDataP {
  static const int m_offset = 10000;
  Xmlp built_in_table [last_math_loc]; // the static math table
  Xmlp built_in_table_alt [last_math_loc]; // the static math table
  Xml** xml_math_table;  // the dynamic math table
  int xmath_size; // size of the dynamic table
  int xmath_pos; // number of slots used in the dynamic table

  Math* math_table; // the table of math lists
  int lmath_size; // the size of the math table
  int lmath_pos; // number of slots used in the math table
  Istring xml_lr_ptable [del_tablesize]; // table of fence attributes 
  math_types math_char_type[nb_mathchars];  // the math type for +, = etc
  Xmlp simplemath_table[nb_simplemath]; // translation of $x$ etc
  Xmlp mc_table[29];
  bool no_ent_names;
  Token nomathsw0; // says next token is for nomathml only
  Token nomathsw1; // says next token is for normal mode only
private:
  void boot_table();
  void boot2();
  void boot_chars();
  void boot_xml_lr_tables();
  auto mk_gen(String a, String b, String c, math_loc pos, name_positions bl,
              symcodes t, bool hack) -> Token;
  auto mk_gen(String a, String b, String c, math_loc pos, math_loc pos2,
              name_positions bl, symcodes t, bool hack) -> Token;
  void mk_ic(String a,String b,String b2,math_loc pos);
  void mk_icb(String a,String b,String b2,math_loc pos);
  void mk_oc(String a,String b,String b2,math_loc pos);
  void mk_oco(String a,String b,String b2,math_loc pos);
  void mk_ocol(String a,String b,String b2,math_loc pos);
  void mk_ocb(String a,String b,String b2,math_loc pos);
  void mk_ocr(String a,String b,String b2,math_loc pos);
  void mk_oc(String a,String b,String b2,math_loc pos, symcodes t,bool);
  void mk_moc(String a,String b,math_loc pos);
  void mk_moo(String a,String b,math_loc pos);
  void mk_accent(String a,String b,String b2,subtypes pos);
  void fill_lr(int,String,String);
  void fill_lr(int,String);
  auto init_builtin(String, math_loc, Xmlp, symcodes) -> Token;

public:
  void boot();
  void realloc_list0();
  void realloc_list();
  void realloc_xml();
  auto find_math_location(math_list_type k, subtypes s) -> subtypes;
  auto find_xml_location() -> subtypes;
  auto find_xml_location(Xmlp) -> subtypes;
  auto make_mfenced(int open, int close, Xmlp val) -> Xmlp;
  auto add_style(int lvl, Xmlp val) -> Xmlp;
  void TM_mk(String a, String b, math_types c);
  void finish_math_mem();
  auto get_mc_table(int i) -> Xmlp { return mc_table[i]; }
  auto get_builtin(int p) -> Xmlp { return built_in_table[p]; }
  auto get_builtin_alt(int p) -> Xmlp { return built_in_table_alt[p]; }
  void init_builtin(int i, Xmlp X) { built_in_table[i] = X; }
  void init_builtin(int i, int j) { built_in_table[i] = built_in_table[j]; }
  void init_builtin(int i, Buffer&B){  built_in_table[i] = new Xml(B); }
  auto get_xml_val(int i) -> Xmlp {
    if(i<m_offset) return built_in_table[i]; 
    else return xml_math_table[i-m_offset];
  }
  auto get_list(int k) -> Math & { return math_table[k]; }
  void push_back(int k, CmdChr, subtypes c);
  auto get_simplemath_val(int i) -> Xmlp { return simplemath_table[i]; }
  auto get_fence(int k) -> Istring { return xml_lr_ptable[k]; }
  auto get_math_char_type(int i) -> math_types { return math_char_type[i]; }
  auto mk_mo(String a) -> Xmlp;
  void set_type(int k, math_list_type c) {math_table[k].set_type(c); }
};

class Cv3Helper{
  Math res;
  Xmlp p;
  int ploc;
  math_types ptype;
  math_types prefix_type;
  Xmlp index, exponent;
  Math object;
  int special; // Sum or product
  void pop_front() { object.pop_front(); }
  auto get_cmd() const -> symcodes { return object.front().get_cmd(); }
  auto get_chr() const -> subtypes { return object.front().get_chr(); }
  auto get_xmltype() -> math_types { return object.front().get_xmltype(); }
  auto empty() -> bool { return object.empty(); }

public:
  int state;
  Cv3Helper(Math X) :  object(X),special(false) {}
  void reinit();
  void non_script();
  void find_kernel(); 
  void find_index(math_style);
  void add_kernel(math_style);
  auto find_operator(math_style) -> name_positions;
  auto finish() -> Math { return res; }
};


namespace math_ns {
  void add_attribute_spec(Istring a, Istring b);
  auto cv_special_string(int c) -> name_positions;
  auto get_builtin(int p) -> Xmlp;
  auto get_builtin_alt(int p) -> Xmlp;
  auto get_delimiter(CmdChr X) -> del_pos;
  auto get_delimiter(int X) -> del_pos;
  auto handle_hspace(Buffer &B) -> Xmlp;
  auto handle_space(Buffer &) -> Xmlp;
  void insert_delimiter(del_pos k);
  void insert_delimiter_t(del_pos k);
  auto math_constants(int c) -> Xmlp;
  auto math_space_code(int c) -> bool;
  auto make_sup(Xmlp xval) -> Xmlp;
  auto mk_mi(Utf8Char c) -> Xmlp;
  auto mk_mi(uchar c, int font) -> Xmlp;
  auto mk_space(String) -> Xmlp;
  auto nb_args_for_cmd(int) -> int;
  void fill_math_char_slots();
  void fill_math_char_slots_ent();
  void fill_single_char();
  auto next_math_style(math_style x) -> math_style;
  auto next_frac_style(math_style x) -> math_style;
  auto special_exponent(const_math_iterator L, const_math_iterator E) -> Xmlp;
  auto special_fence(subtypes s, int &open, int &close) -> bool;
  auto style_level(subtypes tt) -> math_style;
  auto make_math_char(uchar, int) -> Xmlp;
  auto xml2sons(Istring elt, Xmlp first_arg, Xmlp second_arg) -> Xmlp;
};

//---------------------------------------------------------------------
//  Some inline functions

inline auto Math::has_two_elements() const -> bool {
  const_math_iterator X = value.begin();
  if(X==value.end()) return false;
  ++X;
  if(X==value.end()) return false;
  return true;
}

inline auto Math::has_one_element() const -> bool {
  const_math_iterator X = value.begin();
  if(X==value.end()) return false;
  ++X;
  if(X==value.end()) return true;
  return false;
}

inline auto Math::second_element() const -> const MathElt & {
  const_math_iterator X = value.begin();
  ++X;
  return *X;
}

inline auto Math::third_element() const -> const MathElt & {
  const_math_iterator X = value.begin();
  ++X; 
  ++X;
  return *X;
}
