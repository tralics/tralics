// -*- C++ -*-
// $Id: txparser.h,v 2.145 2015/07/29 16:10:40 grimm Exp $
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2003-2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


// This file holds the definition of the Parser class, which is the main
// data structure of the Tralics translator.

class Parser
{
  // Make all tables public
  friend class XkvSetkeys;
 public: 
  Hashtab hash_table;  // the hash table
  Mactab mac_table;    // the table of macros
  Stack the_stack;         // the stack
  EqtbString eqtb_string_table[10];  // eqtb strings
  EqtbInt eqtb_int_table[integer_table_size]; // EQTB, integers
  EqtbDim eqtb_dim_table[dimension_table_size]; // EQTB, dimensions
  EqtbToken toks_registers[toks_register_size]; // EQTB, token lists
  EqtbBox box_table[nb_registers]; // EQTB, boxes
  EqtbGlue glue_table[glue_table_size]; // EQTB, glue
  Token verbatim_chars[nb_characters]; // value of a character in verbatim mode
  int old_catcode[nb_shortverb_values]; // catcodes for undefineshortverb
  bool ok_for_xspace[nb_xspace_values]; // status of char w.r.t. \xspace
  Token uclc_list[22]; // upper, lowert case equivalent of \ij etc
  FontInfo cur_font;  // info for the current font
  vector<Image> the_images; // file data for images
  vector<Xmlp> all_heads;
  Stats my_stats;    // for the statistics
private:
  bool unexpected_seen_hi;  // check for wrongly placed font changes
  bool calc_loaded;    // did we see \usepackage{calc} ?
  bool numbered_verbatim; // has this verbatim line numbers ?
  bool restricted;     // are we in restricted mode ?
  bool force_eof;      // did we see \endinput ?
  bool no_new_file;    // can we pop the input stack ?
  bool chapter_has_star; // true in frontmatter, backmatter
  bool use_quotes;
  bool list_files_p;   // Should we list the files at the end ?
  int old_nberrs;      // previous number of errors
  int cur_line;        // current input line number
  int begin_env_line;  // input line number of 
  int ra_year;    // default year if none given as argument
  Token after_assignment_token; // token for \after assignment
  int default_language_num; // default language
  int cur_level;       // current level on the execution stack
  states state;        // current state of the scanner
  Token cur_tok;       // current token
  unsigned int cur_cs; // control-sequence value of current token
  CmdChr cur_cmd_chr;   // current command code and modifier
  int sectionning_offset; // what is the main section, part, chapter ?
  Token name_for_error;  // used in somne error messages
  l_state long_state;  // Error recovery handling (\long)
  scan_stat scanner_status; // Error recovery handling (\outer)
  int cur_in_chan;     // if get_token call get_a_new_line
  int cur_file_pos;    // pos of file in the package list (0= none)
  
  string cur_env_name; // name of current environment
  string the_url_val;  // this may be <URSophia>, raweb only
  string the_projetval;// this could be miaou
  string year_string;  // the year (effective number)
  string job_name;     // the name, without extensions

  Buffer input_buffer; // input buffer
  Buffer mac_buffer;   // buffer the current macro
  Buffer group_buffer; // buffer for arg of \begin{...} \end(...)
  Buffer unprocessed_xml; // chars to be converted into an XML element

  LinePtr lines;      // the lines to  be read
  TokenList TL;       // list of tokens to be read again
  Condition conditions;// condition stack for current \if
  SthInternal cur_val;// result of scan_something internal
  TokenList document_hook; // the document-hook
  TokenList end_document_hook; // the end-document-hook
  Utf8Char verb_saved_char;  // Char to use for verb by ShortVewrb
  vector<Utf8Char> input_line; // input line converted to chars
  uint input_line_pos;  // position in input_line
  Xmlp the_xmlA, the_xmlB; // for XML tree manipulations
  // private inline functions
 private: 
  bool at_eol () { return input_line_pos>=input_line.size(); }
  symcodes fast_catcode(int x) const {
    return symcodes(eqtb_int_table[x].get_val()); }
  Token get_after_ass_tok() { 
    Token x = after_assignment_token;
    after_assignment_token.kill(); 
    return x; 
  }
  string& get_cur_env_name() { return cur_env_name; }
  int get_def_language_num()const { return default_language_num; }
  string get_projet_val() const { return the_projetval; }
  string get_ur_val() { return the_url_val; }
  bool is_pos_par(int k) const { return eqtb_int_table[k].get_val()>0; }
  void kill_line() { input_line.clear(); }
  void look_at_mac() { 
    cur_cmd_chr = hash_table.eqtb[cur_tok.eqtb_loc()].get_cmdchr();
  }
  void see_non_mac_token(Token t)  {
    cur_cmd_chr.set_cmd(t.cmd_val());
    cur_cmd_chr.set_chr(t.chr_val());
  }
  void set_after_ass_tok(Token x) { after_assignment_token = x; }
  void set_cat(int c, int v) { eqtb_int_table[c].set_val(v); }
  void set_def_language_num(int x) { default_language_num = x; }
  bool tracing_io() const { return is_pos_par(tracingoutput_code); }
  bool tracing_macros() const { return is_pos_par(tracingmacros_code); }
  // public inline functions
 public:
  void back_input() { TL.push_front(cur_tok); }
  void back_input(Token t) { TL.push_front(t); }
  void back_input(TokenList& L) { TL.splice(TL.begin(),L); }
  void brace_me(TokenList&L);
  int cur_centering() const { 
    return eqtb_int_table[incentering_code].get_val(); }
  bool cur_lang_fr() const {
    return eqtb_int_table[language_code].get_val() ==1; }
  bool cur_lang_german() const {
    return eqtb_int_table[language_code].get_val() ==2; }
  Istring cur_line_to_istring();
  void decr_cur_level() { cur_level--; }
  string get_cur_filename() { return lines.get_file_name(); }
  int get_cur_file_pos() const { return cur_file_pos; }
  int get_cur_level() { return cur_level; }
  int get_cur_line()const { return cur_line; }
  SthInternal& get_cur_val() { return cur_val; }
  string get_job_name() const { return job_name; }
  bool get_list_files()const { return list_files_p; }
  int  get_ra_year()const { return ra_year; }
  string get_year_string() const { return year_string; }
  void init (LinePtr x) { lines = x; }
  void remember_ur(string s) { the_url_val = s; }
  void set_cur_line(int x) { cur_line = x; }
  void set_cur_file_pos(int k) { cur_file_pos = k; }
  void set_cur_env_name(string s) { 
    cur_env_name = s; 
    begin_env_line = cur_line;
  } 
  void set_cur_env_name(string s,int x) { 
    cur_env_name = s;
    begin_env_line = x;
  } 
  void set_ra_year(int x) { ra_year = x; }
  void set_job_name(string s) { job_name = s; }
  void set_projet_val(string s) { the_projetval=s; }
  void set_year_string(string s) { year_string=s; }
  bool tracing_commands() const { return is_pos_par(tracingcommands_code); }
  bool tracing_assigns() const { return is_pos_par(tracingassigns_code); }
  bool tracing_math() const { return is_pos_par(tracingmath_code); }
  bool tracing_stack() const { return is_pos_par(tracingrestores_code); }
  void unexpected_font() { unexpected_seen_hi = true; }

  // public functions
 public:  
  Parser();

  void add_buffer_to_document_hook(Buffer&b,const string&);
  void add_language_att();
  void after_main_text();
  void boot();
  void boot_special_names();
  void box_end(Xmlp,int);
  bool csname_aux(String s1, String s2, TokenList& L, bool cs,Buffer&b);
  bool csname_aux(TokenList& L, bool cs,Buffer&b);
  void eq_define (int a, CmdChr, bool gbl);
  void evaluate_now(String s,const string&);
  void evaluate_now(Buffer& s,const string&);
  void final_checks();
  void finish_images();
  void flush_buffer();
  void font_has_changed1();
  FpNum fp_read_value();
  void fp_send_one_arg (TokenList& res);
  scan_stat get_scanner_status() const { return scanner_status; }
  l_state get_long_state() const { return long_state; }
  bool has_letter_catcode(int x)const { return fast_catcode(x)==letter_catcode;}
  void init_all(string);
  void load_latex();
  TokenList mac_arg ();
  TokenList mac_arg_long();
  int nct_aux(Token);
  Istring nT_next_arg();
  void parse_error(const string&,TokenList&);
  void parse_error(const string&);
  void parse_error(String);
  void parse_error(const string&,const string&);
  void parse_error(const string&,Token,const string&,const string&);
  void parse_error(const string&,const string&,const string&);
  void print_cmd_chr(CmdChr);
  void remove_junk();
  void scan_eqno(math_list_type);
  void scan_glue(internal_type level,Token T);
  void scan_glue(internal_type level,Token T,bool);
  void scan_glue(internal_type level,Token T,String);
  void set_default_language(int);
  void set_scanner_status(scan_stat c) { scanner_status=c; }
  void set_long_state(l_state c) { long_state=c; }
  void signal_error(String);
  TokenList special_expand(TokenList* args);
  Xmlp special_tpa_arg(String n,String y, bool par,bool env,bool qf);
  void T_titlepage_finish (int v);
  Xmlp tpa_exec(String);
  void tracing_all();
  void translate0();
  void translate_all();
  void word_define (int a, int c, bool gbl);
  bool find_a_save_key(const string& mykey);
  Token get_name_for_error() { return name_for_error; }

  // private functions, alphabetic order
 private:
  void accent_a ();
  void accent ();
  void accent_err1();
  void accent_err2(Token Y);
  void accent_err3();
  void accent_err4();
  void add_bib_marker(bool);
  void add_math_label(Xmlp);
  void add_buffer_to_list(Buffer& b,TokenList&X,const string&);
  void add_id(const string&);
  void add_vspace(ScaledInt,Xid);
  void after_parameter(bool exp,int);
  void after_math(bool);
  void append_glue(ScaledInt,bool);
  void arg_font(subtypes);
  void assign_def_something(bool gbl);
  void back_input_braced(TokenList&);
  void back_input_pt(bool);
  void back_input_unless_space();
  void bad_counter0();
  void bad_counter1(const Buffer&,Equivalent&);
  void bad_csname();
  void bad_csname(String);
  void bad_definition(int nb, Token name);
  void bad_delimited(int,Token name);
  void bad_end_env(int);
  void bad_end_env(String s,bool);
  void bad_group_char (bool test);
  void bad_macro_prefix(Token,Token);
  void bad_nbargs(int k);
  void bad_number();
  void bad_redefinition(int,Token);
  bool before_mac_arg (Token);
  void begin_box(int,subtypes);
  void boot_NAO();
  void boot_fp();
  void boot_time();
  void boot_uclc();
  void boot_verbatim();
  void boot_xspace();
  void box_define (int a, Xmlp c, bool gbl);
  void calc_aux(SthInternal& A);
  void calc_main(internal_type type,SthInternal& res, TokenList& B);
  void calc_mul_aux (SthInternal&res);
  void calc_primitive(SthInternal&A);
  void calc_ratio_eval(int num, int den,SthInternal& res);
  void calc_spec_mul(RealNumber,SthInternal& res);
  void call_define_key(TokenList&L,Token cmd,const string&arg,const string&fam);
  void change_case(int);
  void check_all_ids();
  bool check_brace(int&);
  bool check_builtin_pack(const string&);
  void check_builtin_class(const string&);
  bool check_if_redef(const string& s);
  void check_language();
  void check_module_title(TokenList&L);
  void check_outer_validity();
  void check_outer_validity0();
  void conditional(subtypes,bool);
  void convert();
  void count_days();
  bool counter(bool def);
  void counter_commands(int);
  bool counter_aux(string,String,Token T);
  void counter_boot(String,String);
  bool counter_check(Buffer&,bool,Token);
  int  counter_read_opt(String);
  void counter_overflow(int,int);
  void close_all();
  void create_aux_file_and_run_pgm();
  void csname();
  int cur_group_type();
  void date_commands(int);
  void datebynumber();
  void dbl_arg();
  void declare_math_operator();
  void declaretopics();
  void default_bp(Buffer& B,Token T,TokenList&val);
  void define_bool_key(subtypes);
  void define_choice_key();
  void define_cmd_key(subtypes);
  void define_new_box();
  void define_new_counter();
  void define_new_dimen();
  void define_new_language();
  void define_new_length();
  void define_new_muskip();
  void define_new_read();
  void define_new_something(subtypes);
  void define_new_something(String ,int, int ,symcodes c);
  void define_new_toks();
  void define_new_write();
  void define_something(int,bool,symcodes);
  Utf8Char delimiter_for_verb(bool&,bool&);
  Utf8Char delimiter_for_saveverb(bool&);
  Istring dimen_attrib(ScaledInt);
  ScaledInt dimen_from_list(Token, TokenList&);
  void dimen_from_list0(Token, TokenList&);
  void dim_define (int a, ScaledInt c, bool gbl);
  void disable_keys();
  int do_register_arg(int q,int&p,Token&);
  void do_register_command(bool gbl);
  void dump_save_stack();
  bool edef_aux(TokenList& L);
  void english_quotes(CmdChr);
  void enter_file_in_table(const string&, bool);
  SaveAuxEnv* env_helper (const string& s);
  void exec_calc();
  void exec_fp_cmd(subtypes);
  void exec_fpi_cmd(subtypes);
  void expand();
  void expand_car(bool);
  void expand_first(TokenList& L);
  bool expand_ifthenelse(Token);
  bool expand_ifx ();
  void expand_mac (Macro& X);
  void expand_mark(subtypes);
  TokenList expand_mac_inner (Macro& X,TokenList*);
  void expand_no_arg0 (Token);
  void expand_no_arg (const string&);
  void expand_spaces ();
  void expand_twoargs ();
  void expand_verbatim (int,Token,Token,Token);
  void expand_verb (unsigned char t);
  void expand_verb0(Utf8Char);
  void expand_verb1(TokenList&);
  void expand_when_ok(bool);
  string exp_token_list_to_string(TokenList& L);
  void extended_chars(unsigned int);
  void extra_close_brace(int cl);
  void extra_fi_or_else();
  void extract_keys(TokenList,vector<string>&);
  bool eval_condition(subtypes);
  void eval_let(Token,Token,bool);
  bool false_end_tabular(const string&);
  void fast_new_macro(TokenList& L,Token name);
  void fetch_box_id(Xmlp);
  String fetch_name0();
  String fetch_name1(TokenList& L);
  void fetch_name2();
  String fetch_name_opt();
  Token find_env_token(const string& s, bool beg);
  void find_in_config(int c);
  void finish_a_cell(Token T, Istring a);
  void finish_counter_cmd(Token, TokenList& L);
  void finish_csname(const Buffer&,String);
  void finish_csname(const Buffer&);
  void finish_fi(); 
  void finish_index(); 
  void finish_color(); 
  void finish_pers(); 
  void finish_kvo_bool(Token,const string&,const string&);
  void finish_iwhile(TokenList& A,Token D);
  void finish_par_cmd(bool,Istring);
  void finish_trivial_math(Xmlp);
  void finish_no_mathml(bool,int); 
  boundary_type first_boundary();
  void first_of_one(Token);
  void flush_buffer0();
  void flush_buffer1();
  void fnhack();
  void font_has_changed();
  void formatdate();
  bool four_hats(Utf8Char cc);
  void fp_boolean(bool);
  void fp_calla(Token);
  void fp_callb(Token);
  void fp_callc(Token);
  void fp_calld(Token);
  void fp_calle(Token);
  void fp_e_arcsin(subtypes);
  void fp_e_csolve();
  void fp_e_eval();
  void fp_e_lsolve();
  void fp_e_pascal();
  void fp_e_qsolve();
  void fp_e_qqsolve();
  void fp_e_upn();
  void fp_eval_lt(subtypes);
  void fp_eval_unarytest(subtypes);
  void fp_finish(FpNum X);
  void fp_parse_error(Token,Token);
  void fp_prepare();
  void fp_print();
  int  fp_read_int();
  void fp_set();
  void fp_setseed();
  void fp_special_expand(TokenList& L);
  void french_punctuation(CmdChr);
  bool get_a_new_line();
  string get_attval();
  name_positions get_ctb_opt(bool);
  name_positions get_trees_opt();
  Istring get_c_val(Token);
  void get_counter(Token, int&);
  int get_index_value();
  void get_date_ctrs(int&,int&,int&);
  void get_def_nbargs (Macro* X,Token t);
  string get_env();
  name_positions get_lrcs_opt(bool);
  Token get_macro();
  TokenList get_mac_value(Token);
  TokenList get_mac_value(const string&);
  int get_nbargs ();
  void get_new_command (rd_flag redef,bool gbl);
  void get_new_command_aux(const TokenList&);
  void get_new_env (rd_flag redef,bool gbl);
  Macro* get_new_mac();
  Istring get_opt_dim(Token);
  bool get_token();
  bool get_token_o();
  int get_r_token(bool br=false);
  bool get_x_token();
  void get_x_token_or_active_char();
  void glue_define (int a, Glue c, bool gbl);
  void grab_env(TokenList&,bool,bool&);
  void grab_env(TokenList&);
  string group_to_string ();
  string group_to_string_spec (bool);
  void iexpand();
  void ifdefinable();
  void if_empty();
  void if_nextchar(bool);
  void if_star();
  void if_undefined();
  void ifthenelse ();
  bool ifthenelse (TokenList&);
  string ignore_env(bool);
  void ignore_group0 ();
  void ignore_next_arg ();
  void ignore_next_optarg();
  Xid  ileave_v_mode();
  void implicit_par(subtypes);
  void improper_alpha();
  void includegraphics(subtypes);
  int  index_aux(TokenList& L,int father,int);
  void initialise_font();
  void ins_the_toks();
  void insert_endline_char();
  void insert_every_bib();
  void insert_relax ();
  void insert_hook (int);
  void internal_choice_key();
  void internal_define_key(Token T);
  void internal_define_key_default(Token T,TokenList& L);
  Xmlp internal_makebox();
  void interpret_genfrac_cmd(int res,subtypes,CmdChr);
  void interpret_mathchoice_cmd(int res,subtypes,CmdChr);
  void interpret_math_cmd(int res,subtypes);
  void interpret_rc();
  void invalid_key(string,const TokenList&);
  bool is_delimiter(const TokenList& L);
  void is_date_valid();
  SaveAuxEnv* is_env_on_stack(const string& s);
  bool is_in_open();
  bool is_inner_math();
  bool is_not_a_math_env(String s); 
  bool is_verbatim_end ();
  void iwhile_num(subtypes);
  void key_ifundefined();
  void kvo_bool_key();
  void kvo_bool_opt();
  void kvo_comp_opt();
  void kvo_family(subtypes k);
  void kvo_family_etc(subtypes k);
  string kvo_getfam();
  void kvo_process();
  void kvo_string_opt();
  void kvo_void_opt();
  void kvo_void_key();
  AttList& last_att_list();
  void latex_ctr();
  string latex_input(int);
  void LC();
  void leave_h_mode();
  void leave_v_mode();
  void lost_if(int);
  void mac_define(Token a, Macro* b,bool gbl,rd_flag redef,symcodes);
  void make_catcodes();
  Xmlp make_cit_ref(Istring,Istring);
  void make_constants();
  void makelabel();
  void make_token(String);
  void make_uclc_table ();
  subtypes math_argument(int,Token);
  int math_dimen_attrib(Token C, String s); 
  del_pos math_lr_value();
  void math_only();
  void mathversion();
  void minus_sign(CmdChr);
  void missing_argument();
  void missing_close_brace(int);
  void missing_equals();
  void missing_flush();
  void missing_open_brace();
  void mklcuc(int c, int lc, int uc);
  void mklcuc(int c, int);
  void mk_hi(String,char);
  void month_day(subtypes);
  void more_bootstrap();
  void multiple_label(String, int,string);
  void multiply_dim(RealNumber val,int v);
  void multispan();
  bool my_csname(String s1, String s2, TokenList& L, String s,bool cs);
  int nb_env_on_stack(const string& s);
  int nb_env_on_stack();
  void need_array_mode();
  void need_bib_mode();
  bool next_arg_is_project();
  void next_date();
  void next_day();
  void next_from_list();
  bool next_from_line();
  bool next_from_line0();
  bool next_optarg(TokenList&);
  bool next_optarg_long(TokenList&);
  void new_boolean(subtypes);
  void newif();
  void newif_aux(Token T, string s, bool b);
  void new_font ();
  bool new_line_for_read(bool);
  void new_macro(const string& L,Token name);
  void new_macro(TokenList& L,Token name,bool gbl);
  void new_macro(TokenList& L,Token name);
  subtypes new_math_list(int,math_list_type,string s);
  void new_prim(String, TokenList&);
  void new_prim(String, String);
  void new_prim(Token, Token);
  void new_primx(String, String);
  void new_prim(Token, TokenList&);
  void new_xref(Xmlp val, string v, bool err);
  void no_arg_font();
  void no_extension(AttList&, const string&);
  Istring nT_next_optarg();
  void numberwithin();
  bool ok_to_define(Token a, rd_flag redef);
  void old_font();
  void one_of_two(TokenList&, TokenList&, bool);
  void one_of_two(bool);
  void open_tex_file(bool);
  void opt_to_mandatory();
  bool optional_enumerate(TokenList& L,String);
  void out_warning(Buffer&B, msg_type);
  void par_shape(subtypes m);
  void pass_text(); 
  void prefixed_aux(bool,symcodes);
  void prefixed_command();
  void prefix_error(bool b_global, symcodes K);
  void pop_input_stack(bool);
  void pop_level(bool,boundary_type);
  void pop_all_levels();
  void prev_date();
  void print_token(ostream& fp,Token x);
  void process_char(uchar c);
  void process_string(String c);
  void process_char(Utf8Char c);
  void process_char(uint c);
  void process_char(int c);
  void push_input_stack(const string&,bool);
  void push_level(boundary_type);
  void push_module();
  void push_module(const string&aux);
  Xid push_par();
  void push_save_stack(SaveAux *v);
  void push_tpa();
  void ratio_evaluate(TokenList& A, TokenList& B,SthInternal&res);
  void read_a_TokenList(bool gbl);
  TokenList read_delimited (const TokenList& L);
  int read_elt_id(Token T);
  Token read_for_variable();
  TokenList read_from_file(int,bool);
  TokenList read_mac_body (bool exp);
  void read_mac_body (TokenList&,bool,int);
  void read_one_space();
  void read_until(TokenList&,Token x);
  TokenList read_until(Token x);
  TokenList read_until_long(Token x);
  void read_into(TokenList&X);
  int read_unit();
  void refstepcounter();
  void refstepcounter(const string&,bool);
  void refstepcounter(TokenList&,bool);
  void remove_element(TokenList& A, TokenList& B, Token C);
  bool remove_initial_plus (bool);
  void remove_initial_space();
  void remove_initial_space_and_back_input();
  void remove_initial_space_relax();
  bool remove_initial_star ();
  void restore_the_state (SaveState& x);
  string rT_next_arg();
  string rT_next_optarg();
  void runaway(int);
  void save_font();
  void save_the_state(SaveState& x);
  int scan_27bit_int();
  int scan_alpha();
  void scan_box(int);
  int scan_braced_int(Token);
  int scan_char_num  ();
  Istring scan_color(const string& opt,const string& name);  
  bool scan_date_ctrs();
  bool scan_dim_helper(bool mu,bool ai);
  bool scan_dim2(RealNumber&,bool,Token T);
  void scan_dimen(bool,Token T);
  void scan_dimen(bool mu, bool inf, glue_spec&,bool shortcut,Token T);
  bool scan_dimen1(bool mu, bool inf, glue_spec&,bool shortcut,Token T);
  void scan_double(Token T,RealNumber&res);
  int scan_eight_bit_int();
  void scan_expr(subtypes);
  bool scan_expr(Token,internal_type);
  void scan_expr_arg(internal_type, Token T);
  scan_expr_t scan_expr_next(bool);
  int scan_fifteen_bit_int();
  string scan_file_name();
  int  scan_font_ident ();
  bool scan_for_eval(Buffer&B, bool sw);
  TokenList scan_general_text();
  bool scan_group0 (TokenList&,int);
  bool scan_group1(TokenList&L, int&b,int cl);
  void scan_group2(TokenList&);
  void scan_group3 (TokenList&,int,bool,int);
  void scan_group4 (TokenList&,int);
  bool scan_group_opt(TokenList&L);
  void scan_group_del(TokenList&L,const TokenList&);
  void scan_hbox(int);
  int  scan_int(TokenList&, Token);
  int  scan_int(Token);
  int  scan_int(Token,int, String);
  int  scan_int_digs();
  int  scan_int_internal();
  bool scan_keyword(String s);
  void scan_left_brace();
  void scan_left_brace_and_bi();
  void scan_math(int,math_list_type);
  int  scan_math1(int);
  void scan_math2(int, math_list_type);
  void scan_math3(int, math_list_type, bool);
  bool scan_math_dollar(int,math_list_type);
  bool scan_math_env(int,math_list_type);
  int  scan_mathfont_ident ();
  ScaledInt scan_math_kern(symcodes,subtypes&);
  void scan_math_hbox(int);
  void scan_math_mi(int, subtypes,subtypes,CmdChr);
  void scan_math_rel(subtypes,int);
  void scan_math_tag(subtypes);
  void scan_optional_equals();
  bool scan_pair_ints (Token T, TokenList&L);
  void scan_prime();
  void scan_rule(int);
  void scan_sideset();
  void scan_split();
  bool scan_sign();
  void scan_something_internal(internal_type,bool);
  void scan_something_internal(internal_type);
  int  scan_special_int_d(Token T, int d);
  Token scan_style();
  void scan_toks_def();
  void scan_toks_edef(TokenList&);
  void scan_toks_absorb();
  void scan_toks_absorb_expand();
  int  scan_twenty_seven_bit_int();
  void scan_updown(TokenList&,TokenList&,TokenList&,TokenList&);
  void scan_updown();
  void scan_unit(RealNumber f);
  void see_font_change(subtypes);
  void see_future_let(bool);
  void see_let(bool);
  void see_new_thm ();
  void see_new_def(bool edef,bool gbl,symcodes);
  void see_new_id_spec(bool);
  void selective_sanitize();
  void select_math_font();
  void setlength_command(int c);
  void set_boolean();
  void set_counter(Token, int);
  void set_date_ctrs(int,int,int);
  void setkeys(bool);
  void shorthand_define(int cmd,bool gbl);
  void shorthand_gdefine(int cmd, String,int);
  void short_verb(int x);
  void short_verb_error(Token t, int x);
  void show_box(Xmlp);
  void skip_group (TokenList&);
  void skip_group0 (TokenList&);
  void skip_initial_space();
  void skip_initial_space_and_back_input();
  void skip_over_parens();
  void skip_prefix (const TokenList& L);
  void solve_cite(bool);
  void special_fvset();
  string special_next_arg();
  void special_read_mac_body(TokenList& L);
  void special_verbatim();
  void start_a_cell(bool);
  void start_a_row(int);
  bool start_scan_math(Math&,int);
  void start_paras(int,string,bool);
  void store_new_line(int, bool);
  void string_define (int a, const string& c, bool gbl);
  void strip_pt();
  string sT_next_arg();
  string sT_next_optarg();
  string sT_translate(TokenList& L);

  void T_addtomacro(bool);
  void T_add_to_reset();
  void T_aftergroup();
  void T_afterfi();
  void T_afterelsefi();
  void T_ampersand();
  void T_anchor();
  void T_arg1(name_positions);
  void T_atdocument(subtypes);
  void T_at_end_of_class();
  void T_backslash();
  void T_bezier(int);
  void T_bauteursediteurs(subtypes);
  void T_bibitem();
  void T_biblio();
  void T_bibliostyle();
  void T_bpers();
  void T_beginend(symcodes);
  void T_begindocument();
  void T_begin(const string&);
  void T_cap_or_note(bool);
  void T_change_element_name();
  void T_catperso ();
  void T_citation ();
  void T_cite(subtypes);
  void T_cite(subtypes,TokenList&,Istring&);
  void T_cite_one();
  void T_cite_type();
  void T_cititem();
  void T_class_error(subtypes);
  void T_cline();
  void T_cons();
  void T_cons(Token, TokenList&);
  void T_cr();
  void T_cst1(int);
  void T_cst2(int);
  void T_curves(int);
  void T_dashline(subtypes);
  void T_declare_options();
  void T_declare_option_star();
  void T_define_verbatim_env();
  void T_define_key(bool);
  void T_ding();
  void T_documentclass(bool);
  void T_empty_bibitem();
  void T_end_tabular (subtypes);
  void T_end(const string&);
  void T_enddocument(subtypes);
  void T_endv();
  void T_end_the_biblio();
  void T_end_theorem();
  void T_ensuremath();
  void T_epsfbox();
  void T_error();
  void T_etex(subtypes);
  void T_execute_options();
  void T_expandafter();
  void T_figure_table(symcodes,subtypes);
  void T_color(subtypes);
  void T_figure_table_end(bool);
  void T_fancy(String, TokenList&);
  void T_fancy();
  void T_filecontents(int);
  void T_fbox(subtypes);
  void T_fbox_dash_box();
  void T_fbox_rotate_box();
  void T_float(subtypes);
  void T_fonts(name_positions x);
  void T_glo ();
  void T_glossaire ();
  void T_gloss (bool);
  void T_glossaire_end ();
  void T_grabenv();
  void T_hanl(subtypes);
  Xmlp T_hanl_text();
  Xmlp T_hanl_url();
  void T_hline(subtypes);
  int  T_hline_parse(subtypes);
  void T_hspace(subtypes);
  void T_if_package_later(bool c);
  void T_if_package_loaded(bool c);
  void T_if_package_with(bool c);
  void T_ignoreA();
  void T_index(subtypes);
  void T_input(int);
  void T_inputclass();
  void T_ipa(subtypes);
  void T_isin();
  void T_item (int);
  Istring T_item_label(int);
  void T_keywords();
  void T_label();
  void T_line(subtypes);
  void T_listenv(symcodes);
  void T_listenv_end(symcodes);
  void T_linethickness(int);
  void T_loop();
  void T_load_with_options(bool);
  void T_makebox(bool,Token);
  void T_math (int);
  void T_matter (subtypes);
  void T_mbox(subtypes);
  void T_minipage();
  void T_moreinfo_end();
  void T_multicolumn ();
  void T_multiput();
  void T_next_arg_local();
  void T_next_arg();
  void T_next_optarg();
  void T_newline();
  void T_newcolumn_type();
  void T_newthheorem();
  void T_node();
  void T_nodepoint();
  void T_nodeconnect(name_positions);
  void T_nodecurve(name_positions);
  void T_nodebox(name_positions);
  void T_nodetriangle(name_positions);
  void T_nodecircle(name_positions);
  void T_barnodeconnect(name_positions);
  void T_option_not_used();
  void T_par1(Istring);
  void T_par1();
  void T_paras(subtypes x);
  void T_parse_encoding(bool, subtypes);
  void T_participants(subtypes x);
  void T_participants_end();
  void T_pass_options(bool c);
  void T_pers ();
  void T_pers_ra ();
  void T_picture();
  void T_process_options();
  void T_process_options_aux(TokenList&);
  void T_provides_package(bool c);
  void T_put(subtypes);
  void T_raisebox();
  void T_random();
  void T_rasection();
  void T_ra_startdoc();
  void T_rasection_end();
  void T_reevaluate();
  void T_ref(bool);
  void T_reevaluate0(TokenList&, bool);
  void T_remove_element();
  void T_save_box(bool);
  void T_save_use_verb(bool save);
  void T_scan_glue(subtypes c);
  void T_setmode();
  void T_specimp(int);
  void T_subfigure();
  void T_startprojet(String proj, String loc);
  void T_start_tabular (subtypes);
  void T_start_the_biblio();
  void T_start_theorem(int);
  void T_testopt();
  void T_thm_aux(int);
  void T_titlepage (int v);
  void T_trees(int);
  void T_translate(TokenList&);
  void T_typein();
  void T_twodims(Istring&,Istring&,Token);
  void T_twoints(Istring&,Istring&,Token);
  void T_twoints(TokenList&A,TokenList&B);
  void T_un_box(subtypes c); 
  void T_unimp(subtypes c);
  void T_unimplemented_font(subtypes c);
  void T_unless();
  void T_url(subtypes);
  void T_usefont();
  void T_usepackage();
  void T_use_counter(const string&);
  void T_xmlelt(subtypes);
  void T_xmlenv(subtypes);
  void T_xmlenv_end(subtypes);
  void T_xmladdatt(subtypes c);
  void T_xfancy();
  void T_xkeyval(subtypes);
  void T_xspace();
  void TM_fonts();
  void TM_math_fonts(Math& x);
  void TM_tabular_arg (Xid id);

  void Tat_pers ();
  void Tat_pers_ra ();
  void testoptd(string s);
  void tex_extension (int);
  String tex_write(int);
  TokenList the_toks();
  void tipa_acutemacron();
  void tipa_brevemacro();
  void tipa_circumdot();
  void tipa_colon();
  void tipa_dotacute();
  void tipa_exclam();
  void tipa_gravedot();
  void tipa_normal();
  void tipa_semi();
  void tipa_star();
  void tipa_subumlaut();
  void tipa_subtilde();
  void tipa_subring();
  void tipa_tildedot();
  void tipa_syllabic();
  void token_from_list(Token t);
  void token_list_define(int p, TokenList& c, bool gbl);
  ScaledInt token_list_to_dim(TokenList& a,Token C,bool);
  Istring token_list_to_att(TokenList& a,Token C,bool);
  void token_show(int, Buffer&B);
  void token_show(bool lg, const CmdChr&,Buffer&B);
  void token_show(const CmdChr&);
  void trace_count_def(String,CmdChr);
  void trace_if(int);
  void trace_if(String,int,String);
  void trace_if(String,int,int);
  void trace_scan_expr(String s, const SthInternal, char t,Token T);
  void translate01();
  void translate02();
  void translate03();
  void translate1();
  void translate_char (CmdChr);
  void translate_char (uchar,uchar);
  Xmlp translate_list(TokenList& L);
  void translate_font_size();
  void typeout(int);
  void umlaut();
  void umlaut_bad();
  void undefined_env(const string&);
  void undefined_mac();
  void unexpected_close_brace();
  void unskip_group(String s);
  void upn_eval(TokenList&);
  void url_hack(TokenList&);
  void use_a_package(const string&,bool,const string&,bool);
  void user_XML_swap (subtypes c);
  void user_XML_modify (subtypes c);
  void user_XML_fetch ();
  void vb_tokens(bool& ok, Utf8Char test,TokenList&L,bool);
  void verb_error(bool,int);
  void while_do();  
  void while_num(subtypes);  
  void wrong_mode(String);
  void wrong_pop(String, String);
  void x_input(int);
  void xgetfontsize();
  void xkv_checksanitize(Token A, TokenList&B,bool c);
  void xkv_checksanitize(bool c);
  void xkv_fetch_prefix_family();
  void xkv_for(subtypes c);
  void xkv_makehd(TokenList& L);
  void xkv_makehd(); 
  void xkv_merge(bool gbl, int,TokenList& L,bool);
  TokenList XKV_parse_filename();
  void XKV_pox();
  bool xkv_save_keys_aux(bool c, int c2);
  void xkv_testopta();
  void xkv_unsave_keys(bool c);
  void xkv_fetch_prefix();
  void xkv_class_prefix();
  void XKV_process_execute(bool psw);
  void xkv_declare_option();
  void xkv_process_options();
  void xkv_execute_options();
  void xkv_pass_options(bool c);
  void xml_name (Xmlp x);
  string xmllatex();
  void xsetfontsize();
  Xmlp xT_next_arg();
  Xmlp xT_next_optarg();
  void xray(subtypes);
  void zapspace();
};

