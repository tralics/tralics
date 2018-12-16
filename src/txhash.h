// -*- C++ -*-
// $Id: txhash.h,v 2.39 2012/04/27 15:10:05 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


// This is the main hash table. If a token like \foo has hashcode p,
// then foo is in Text[p], or in Text[Next[p]] or in Text[Next[Next[p]]] ...
// The data structure holds the location of tokens like \par
// that are known only after bootstrap
class Hashtab {
 public:
  Token par_token,OB_token, CB_token,dollar_token;
  Token verb_token,noindent_token,small_token, textvisiblespace_token;
  Token end_token,frozen_dont_expand, relax_token,frozen_relax_token,
    sendgroup_token,textbf_token,frozen_endcsname,begin_token;
  Token frozen_undef_token,equals_token,char_token;
  Token number_token,roman_numeral_token,Roman_numeral_token;
  Token advance_token, global_token,atalph_token,atAlph_token,fnsymbol_token;
  Token roman_token, Roman_token,alph_token,Alph_token;
  Token iftrue_token,iffalse_token,let_token,urlfont_token;
  Token fi_token, iterate_token,repeat_token,expandafter_token,endcsname_token;
  Token csname_token,xkv_cc_token;
  Token isodd_token,lengthtest_token,boolean_token,equal_token;
  Token or_token,and_token,not_token,ratio_token,real_token,calc_token;
  Token OR_token, AND_token, NOT_token,isundefined_token;
  Token frozen_protection, prime_token,elt_token,killcounter_token;
  Token math_OB_token,math_CB_token,text_OB_token,text_CB_token;
  Token real_end_token,error_token,tt_token,dagger_token, ddagger_token;
  Token ensuremath_token,vbar_token,atbd_token,notprerr_token;
  Token hat_token,zero_token,one_token,comma_token,dot_token,star_token;
  Token plus_token, minus_token,tilda_token,space_token,newline_token;
  Token verbatim_number_font,verbatim_font, temp_token,unitlength_token;
  Token verb_prefix_token, verbatim_prefix_token,citeone_token,citesimple_token;
  Token end_natcite_token,at_one_token,at_zero_token,at_align_token;
  Token alt_rapers_token,alt_pers_token,crwithargs_token,of_token;
  Token ifdim_token, ifnum_token,iwhilenum_token, iwhiledim_token, 
    iwhilesw_token, do_token,bth_token, eth_token, arabic_token,
    gobble_token, gobbletwo_token,else_token,endv_token,cr_token,
    refstepcounter_token,hspace_token, footcite_sep_token, footcite_pre_token,
    cite_punct_token,operatorname_token, itemlabel_token, vspace_token;
  Token xscale_token, yscale_token, xscaley_token,yscalex_token,refname_token ;
  Token cite_type_token, cite_prenote_token,lbrace_token,rbrace_token;
  Token backslash_token,headercr_token,mathversion_token,tag_token,qquad_token,
    xtag_token,ytag_token, xtag1_token,ytag1_token,apostrophe_token,
    cite_e_token,  cite_i_token,ref_i_token,label_i_token,ensuremath_i_token,
    nocase_e_token,  nocase_i_token, ref_token,label_token, infalse_token,
    intrue_token,nil_token,CurrentOption_token,xkv_for_i_token,
    CurrentOptionKey_token,CurrentOptionValue_token,
    forloop_token,doubleat_token,def_token,iforloop_token,tforloop_token;
  Token footnote_token,dblarg_token;
  Token mmlprescripts_token, mmlmultiscripts_token,mmlnone_token,
    mathop_token,limits_token,begingroup_token,endgroup_token,allowbreak_token;
  Token increqnum_token, makelabel_token,stepcounter_token;
  Token nomathsw0_token, nomathsw1_token;
  Token my_mathfont_table[15];
  Token genfrac_mode[5];
  Token xkv_resa_token, xkv_warn_token, xkv_prefix_token,
    xkv_fams_token, xkv_na_token, xkv_rm_token, xkv_tfam_token, usevalue_token,
    xkv_header_token, xkv_tkey_token, gsavevalue_token,savevalue_token,
    empty_token,composite_token;
  Token last_tok;
  // the big tables
  Equivalent eqtb[eqtb_size];

private:
  String Text[hash_size]; // the strings
  int Next[hash_size];    // points to next
  Buffer B; // internal buffer
  int hash_used; // all places above this one are used
  int hash_usage; // number of commands in the table
 private:
  int find_empty(String s); // find an empty slot
  int find_aux(int p,String name);
 public:
  Hashtab ();
  Buffer& my_buffer() { return B; }
  Token locate(String s); // used by primitive, etc
  Token locate(const string& s); // used by primitive, etc
  Token locate(const Buffer&); // used by primitive, etc
  int get_hash_usage()const { return hash_usage; }
  int hash_find(const Buffer&b, String name);
  int hash_find();
  Token primitive (String s, symcodes c,subtypes v=zero_code);
  Token nohash_primitive(String a, CmdChr);
  void eval_let(String, String);  
  void eval_let_local(String, String);
  String operator[] (int k) const { return Text[k]; }
  void dump();
  void boot_fancyhdr();
  void boot_etex();
  void boot_keyval();
  Token locate_active(Utf8Char s);
  Token locate_mono(Utf8Char s);
  bool is_defined(const Buffer&b);
};

// This is an association table. We could use a standard C++ class here
class SpecialHash 
{
  int size;  // number of terms
  vector<string> key; // the keys
  vector<string> value; // the values
 public:
  SpecialHash(string s) : size(0) { create(s.c_str()); }
  void create(String s);
  string find(String) const;
  int get_size() const { return size;}
  void get_pair(int k, string&a, string &b) {
    if(k<size) {a = key[k];b=value[k];} else {a="";b="";}
  }
  int find_true_false(String s);
  static int counter_val(int);
  int find_counter();
};


// Data structure for label and references.
class LabelInfo
{
  bool used;    // is this ID used ?
  bool defined; // is this ID defined ?
  Istring id;       // value of the ID
  Istring name;     // name of the ID (pointer into SH)
  int lineno;   // line of definition
  string file_name; // file of definition
 public:
  LabelInfo() : used(false), defined(false), id(Istring()), name(Istring()), lineno(0), file_name("") {}
  LabelInfo(Istring k) : used(false), defined(false), id(Istring()), name(k), lineno(0),file_name("") {}
  bool is_used() const { return used; }
  bool is_defined() const { return defined; }
  bool set_used() { if(used) return true; used = true; return false; }
  bool set_defined() { if (defined) return true; defined = true; return false;}
  void set_undefined() { defined = false;}
  Istring get_id() const { return id; }
  Istring get_name() const { return name; }
  void set_id(Istring i) {id = i; }
  int get_lineno() const { return lineno; }
  string get_filename() const { return file_name; }
  void set_lineno(int i) { lineno = i; }
  void set_filename(string i) { file_name = i; }
};


// This class returns foo then bar then gee from `foo,bar,gee'
class Splitter {
  string S; // the string to split
  int pos; // current position
  int size;
 public:
  Splitter(const string& w) : S(w), pos(0) { size = S.size(); }
  bool at_end() const { return pos==size; }
  int count() const;
  String get_next_raw();
  String get_next();
  void extract_keyval(string&,string&);
};


// This is used in order to extract things from \documentclass[]{}
// and \usepackage[]{}
// Not used anymore
class PackMatcher {
  string attrib; // attribute to set
  string value;  // value of attribute
  string package;  // package name
  string options;  // option
  bool wild_pack;  // wild card in package
  bool att_star;    // one star in attribute
  bool match_document; // usepackage? or \documentclass ?
  bool att_plus; // 
  bool used; // did we use this rule ?
 public:
  PackMatcher(bool c=false):
    wild_pack(false), att_star(false), match_document(c), att_plus(false),
    used(false) {}
  bool ok_for_match(bool c) { return c==match_document && !used; }
  void set_match_doc(bool X) { match_document = X; }
  int set_wild_pack(bool X) { wild_pack = X; return X ? 1 : 0; }
  void set_package(string X) { package = X; }
  void set_options(string X) { options = X; }
  void print();
  string get_attrib() { return attrib; }
  bool find_att(string cur_pack, string cur_options);
  Istring find_att_val(string cur_pack, string cur_options);
  void initialise(string A, string B);
};

