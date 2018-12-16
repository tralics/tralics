// -*- C++ -*-
// $Id: txmath.h,v 2.69 2011/08/05 14:08:18 grimm Exp $
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
typedef std::list<MathQ> MathQList;
 
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

inline subtypes math_to_sub(math_list_type x) 
{
  return subtypes(x-fml_offset);
} 
inline math_list_type sub_to_math(subtypes x)
{
  return math_list_type(x+fml_offset);
}

inline bool is_m_font (symcodes cmd)
{
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
  math_types get_xmltype() const { return math_types (Font);}
  void set_xmltype(math_types x) { Font = subtypes(x); }
  subtypes get_font() const { return Font;}
  math_list_type get_lcmd() const { return math_list_type(Font);}
  subtypes get_fml_subtype() const;
  const CmdChr& get_cmd_chr() const { return val; }
  Utf8Char get_char() const { return val.char_val();}
  subtypes get_chr() const { return val.get_chr();}
  void set_chr(subtypes c) { val.set_chr(c); }
  symcodes get_cmd() const { return val.get_cmd(); }
  void set_cmd(symcodes c) { val.set_cmd(c); }
  Math& get_list() const;
  Xmlp get_xml_val() const;
  
    // some tests on the elements
  bool is_list() const { return val.is_math_list() && Font==subtypes(math_open_cd);}
  bool is_hbox() const { return val.is_math_list() && Font==subtypes(math_hbox_cd);}
  bool is_space() const { return val.is_space(); }
  bool is_digit() const;
  bool is_char() const { return is_space() || is_letter_token() || is_other_token(); }
  bool is_letter_token() const { return val.is_letter(); }
  bool is_other_token() const { return val.is_other(); }
  bool is_star() const { return is_other_token() && get_char()=='*'; }
  bool is_bracket() const { return is_other_token() && get_char()=='['; }
  bool maybe_seq() const ;
  bool maybe_seq(subtypes) const;
  bool maybe_iseq(subtypes) const;
  bool maybe_iseq() const ;
  // other functions
  del_pos large2(); 
  Xmlp remove_prefix() const;
  void cv_noMLt(); 
  void cv_noML(); 
  void cv_noMLt_special(); 
  void cv_noMLt_special0(); 
  void cv_noMLt_list(); 
  void cv_noML_special(); 
  void cv_noML_list(); 
  MathElt cv1(math_style,bool);
  void change_type(int t);
  Xmlp try_math_op ();
  bool is_e_grave() const;
  Xmlp special3() const;
  void print() const;
  int val_as_digit() const { return val.val_as_digit(); }

private:
  void set_xml_subtype(math_types x) { set_xmltype(x); val.set_mathml(); }
  MathElt cv_char();
  MathElt cv_cst();
  MathElt cv_special(math_style);    
  MathElt cv_special1(math_style);    
  MathElt cv_list(math_style,bool);
  MathElt cv_list_no();
  MathElt cv_mi(math_style);
  void cv1_err();
  void dump_for_err() const;
  bool is_accent() const { return val.is_mathml() && Font == 0; }
};

class MathIsSpace: public unary_function<MathElt,bool> {
public:
  explicit MathIsSpace() {}
  bool operator() (const MathElt& m) { 
    return m.is_space();
  }
};

class MathIsDollar: public unary_function<MathElt,bool> {
public:
  explicit MathIsDollar() {}
  bool operator() (const Token& m) { 
    return m.is_math_shift();
  }
};


class MathIsOver: public unary_function<MathElt,bool> {
public:
  explicit MathIsOver() {}
  bool operator() (const MathElt& m) { 
    return m.get_cmd()==over_cmd;
  }
};


typedef std::list<MathElt> MathList;
typedef std::list<MathElt>::const_iterator const_math_iterator;
typedef std::list<MathElt>::iterator math_iterator;

class Math {
  friend class MathHelper;
  friend class MathDataP;
  friend class MathElt;
  MathList value;
  math_list_type type;
  string name;
 public:
  Math() : type(invalid_cd),name("") {}
 public:

  subtypes duplicate (bool) const;
  MathElt& back() { return value.back(); }
  const_math_iterator begin () const { return value.begin(); }
  bool chars_to_mb(Buffer&,bool) const;
  bool chars_to_mb1(Buffer&) const;
  bool chars_to_mb2(Buffer&) const;
  Istring chars_to_mb3();
  void clear() { value.clear(); }
  Xmlp convert_math(math_style);
  Xmlp convert_math_noML(name_positions,bool);
  void convert_math_noML0();
  void convert_math_noMLt0();
  string convert_opname();
  string convert_this_to_string(Buffer&);
  void destroy();
  bool empty() const { return value.empty(); }
  const_math_iterator end () const { return value.end(); }
  bool find_parens (MathQList&,bool) const;
  MathElt& front() { return value.front(); }
  const MathElt& front() const { return value.front(); }
  Math get_arg1 () { return front().get_list(); }
  Math get_arg2 () { return second_element().get_list(); }
  Math get_arg3 () { return third_element().get_list(); }
  math_list_type get_type() const { return type; }
  string get_name() const { return name; }
  Math& get_list(int) const;
  string get_real_name() const;
  void hack_type(int);
  bool has_type(int x) const { return type==x;}
  bool has_one_element() const;
  bool has_two_elements() const;
  bool is_align_ext() const { return name == "@align"; }
  bool is_align_ext_star() const { return name == "@align*"; }
  bool is_aligned_ext() const { return name == "aligned"; }
  bool is_array_env() const;
  bool is_array_ext() const { return name == "array" || name == "array*"; }
  bool is_external_array() const;
  void is_font_cmd1_list(const_math_iterator&B,const_math_iterator&E);
  bool is_bordermatrix () const {return name == "bordermatrix "; }
  bool is_matrix_ext() const { return strncmp(name.c_str()+1,"matrix",6)==0; }
  bool is_matrix() const { return name == "matrix"; }
  bool is_multline_ext() const 
  { return name == "multline" || name == "multline*"; }
  bool is_gather_ext() const 
  { return name == "gather" || name == "gather*"; }
  bool is_split_ext() const { return name == "split"; }
  bool length_one() const { return value.size() ==1; }
  Xmlp M_array (math_style);
  XmlAndType M_cv (math_style,int need_row);
  void pop_back() { value.pop_back(); }
  void pop_front() { value.pop_front(); }
  void pop3 ();
  void print() const;
  void push_back(CmdChr,subtypes);
  void push_back_list(subtypes X,math_list_type c);
  void push_back_font(subtypes X,subtypes c);
  void push_back(CmdChr);
  void push_back(MathElt x) { value.push_back(x); }
  void push_back(Xmlp,int,math_types);
  void push_front(CmdChr,subtypes);
  void remove_initial_group();
  void remove_last();
  void remove_spaces();
  const MathElt& second_element() const;
  void set_display_type() { type = math_ddollar_cd;  } 
  void set_env_name(int);
  void set_name(string X) { name = X;}
  void set_nondisplay_type() { type = math_dollar_cd;  } 
  void set_type(math_list_type c) { type = c; }
  const MathElt& third_element() const;
  Xmlp trivial_math(int);
  Xmlp trivial_math_index(symcodes);
  int check_align();
 private:
  
  void add_cur_cont();
  void add_cur_font();
  bool add_fence(bool,MathF&);
  void concat(Xmlp);
  void concat_space(Xmlp);
  Xmlp convert_cell(int&n,vector<AttList>& table, math_style);
  MathElt convert_char_seq(MathElt W);
  MathElt convert_char_iseq(MathElt W,bool);
  void fetch_rlc(vector<AttList>&);
  void find_paren0 (MathP&) const;
  bool finish_translate1(bool);
  bool finish_translate2();
  void handle_mbox(Math&);
  void handle_mbox_no();
  void handle_mbox_not();
  bool has_over() const;
  bool is_font_cmd1();
  Xmlp large1(MathElt&,math_style);
  XmlAndType M_cv0(math_style);
  Math M_cv3(math_style);
  void handle_cmd_Big(math_style);
  bool handle_cmd_Big_aux(math_style);
  void cv_hspace_t(MathElt& cur);
  Math M_cvaux();
  Xmlp M_ref();
  int M_mbox1 (Buffer&, subtypes&);
  bool only_digits(Buffer&) const;
  void push_back(Math&);
  void push_back_small(Xmlp a);
  void push_front(Math&);
  void skip_initial_space();
  Xmlp special1() const;
  void special2(bool&,Xmlp&) const;
  Xmlp split_as_array(vector<AttList>& table,bool,bool,math_style);
  void remove_opt_arg (bool star);
  string remove_req_arg ();
  string remove_req_arg_noerr () const;
};



// This is a global object for math handling
// but math handling is not recursive. reset() is called on every formula.
class MathHelper {
  MathElt* free_list;  // free list
  bool current_mode; // display or not, needed for \label
  name_positions pos_att; // position attribute, inline or display
  bool need_tag; // has this equation a number
  bool seen_label; // do we see already have a label
  bool warned_label; // was the used waerned for labels on this formula ?
  string label_val; // name of the label
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
 public:
  MathHelper() {math_env_ctr = 0; all_env_ctr =0; }
  void new_label(string s);
  void finish_math_mem();
  void set_type(bool);
  bool has_label() const { return seen_label || need_tag; }
  bool need_eqnum() const { return need_tag; }
  string get_label_val() const { return label_val; }
  void stats();
  name_positions get_pos_att() const { return pos_att; }
  void reset(bool);
  MathElt* get_free_list() const { return free_list; }
  void set_free_list(MathElt* x)  { free_list = x; }
  bool is_inline() const  { return current_mode; }
  bool has_tag () const { return !tag_list.empty(); }
  void reset_tags() { tag_list = TokenList(); }
  void handle_tags();
  void add_tag(TokenList &L);
  void update_all_env_ctr(bool open) 
  { if(open) all_env_ctr ++; else all_env_ctr--; }
  void update_math_env_ctr(bool open) 
  { if(open) math_env_ctr ++; else math_env_ctr--; }
  int get_math_env_ctr() const { return math_env_ctr; }
  int get_all_env_ctr() const { return all_env_ctr; }
  void add_attribute(Istring,Istring,subtypes);
  Xid get_cid() const { return cur_cell_id; }
  Xid get_rid() const { return cur_row_id; }
  Xid get_mid() const { return cur_math_id; }
  Xid get_fid() const { return cur_formula_id; }
  Xid get_tid() const { return cur_texmath_id; }
  Xid get_taid() const { return cur_table_id; }
  void set_cid(Xid i) { cur_cell_id = i; }
  void set_rid(Xid i) { cur_row_id = i; }
  void set_taid(Xid i) { cur_table_id = i; }
  void starred_tag() { is_tag_starred = true; }
  void check_for_eqnum(int type);
 private:
  void set_label(string s) { label_val = s; seen_label = true; }
  void initialise();
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
  Token mk_gen(String a, String b, String c, math_loc pos, name_positions bl,
	       symcodes t, bool hack);
  Token mk_gen(String a, String b, String c, math_loc pos, math_loc pos2, 
	       name_positions bl, symcodes t, bool hack);
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
  Token init_builtin(String,math_loc, Xmlp, symcodes);
public:
  void boot();
  void realloc_list0();
  void realloc_list();
  void realloc_xml();
  subtypes find_math_location(math_list_type k);
  subtypes find_xml_location();
  subtypes find_xml_location(Xmlp);
  Xmlp make_mfenced(int open, int close, Xmlp val);
  Xmlp add_style(int lvl,Xmlp val);
  void TM_mk(String a, String b, math_types c);
  void finish_math_mem();
  Xmlp get_mc_table(int i) { return mc_table[i]; }
  Xmlp get_builtin(int p) { return built_in_table[p];}
  Xmlp get_builtin_alt(int p) { return built_in_table_alt[p];}
  void init_builtin(int i, Xmlp X) { built_in_table[i] = X; }
  void init_builtin(int i, int j) { built_in_table[i] = built_in_table[j]; }
  void init_builtin(int i, Buffer&B){  built_in_table[i] = new Xml(B); }
  Xmlp get_xml_val(int i) {
    if(i<m_offset) return built_in_table[i]; 
    else return xml_math_table[i-m_offset];
  }
  Math& get_list(int k) { return math_table[k]; }
  void push_back(int k, CmdChr, subtypes c);
  Xmlp get_simplemath_val(int i) { return simplemath_table[i]; }
  Istring get_fence(int k) { return xml_lr_ptable[k]; }
  math_types get_math_char_type(int i) { return math_char_type[i]; }
  Xmlp mk_mo(String a);
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
  symcodes get_cmd() const { return object.front().get_cmd(); }
  subtypes get_chr() const { return object.front().get_chr(); }
  math_types get_xmltype() { return object.front().get_xmltype(); }
  bool empty() { return object.empty(); }
 public:
  int state;
  Cv3Helper(Math X) :  object(X),special(false) {}
  void reinit();
  void non_script();
  void find_kernel(); 
  void find_index(math_style);
  void add_kernel(math_style);
  name_positions find_operator(math_style);
  Math finish() { return res; }
};


namespace math_ns {
  void add_attribute_spec(Istring a, Istring b);
  name_positions cv_special_string (int c);
  Xmlp get_builtin(int p);
  Xmlp get_builtin_alt(int p);
  del_pos get_delimiter(CmdChr X);
  del_pos get_delimiter(int X);
  Xmlp handle_hspace(Buffer& B);
  Xmlp handle_space(Buffer&);
  void insert_delimiter(del_pos k);
  void insert_delimiter_t(del_pos k);
  Xmlp math_constants(int c);
  bool math_space_code(int c);
  Xmlp make_sup(Xmlp xval);
  Xmlp mk_mi(Utf8Char c);
  Xmlp mk_mi(uchar c, int font);
  Xmlp mk_space(String);
  int nb_args_for_cmd(int);
  void fill_math_char_slots();
  void fill_math_char_slots_ent();
  void fill_single_char();
  math_style next_math_style(math_style x);
  math_style next_frac_style(math_style x);
  Xmlp special_exponent(const_math_iterator L,const_math_iterator E);
  void special_fence(char s, int& open, int&close);  
  math_style style_level(subtypes tt); 
  Xmlp make_math_char(uchar,int);
  Xmlp xml2sons(Istring elt, Xmlp first_arg, Xmlp second_arg);
};

//---------------------------------------------------------------------
//  Some inline functions

inline bool Math::has_two_elements() const 
{ 
  const_math_iterator X = value.begin();
  if(X==value.end()) return false;
  ++X;
  if(X==value.end()) return false;
  return true;
}


inline bool Math::has_one_element() const 
{ 
  const_math_iterator X = value.begin();
  if(X==value.end()) return false;
  ++X;
  if(X==value.end()) return true;
  return false;
}

inline const MathElt& Math::second_element()  const
{ 
  const_math_iterator X = value.begin();
  ++X;
  return *X; 
}

inline const MathElt& Math::third_element() const
{ 
  const_math_iterator X = value.begin();
  ++X; 
  ++X;
  return *X; 
}


inline bool Math::is_external_array() const 
{
  return name == "eqnarray" || name == "eqnarray*" ||
    name == "Beqnarray" || name == "Beqnarray*" ||
    name == "multline" || name == "multline*" ||
    name == "gather" || name == "gather*" || 
    name == "@align" || name == "@align*" ;
}
