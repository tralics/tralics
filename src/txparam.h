// -*- C++ -*-
// $Id: txparam.h,v 2.47 2012/09/03 08:45:48 grimm Exp $
// TRALICS, copyright (C)  INRIA/apics (Jose' Grimm) 2002,2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This is one include  file for the tralics software

class MainClass
{
  //  MS ms;  // the ms data, see txcheck
  //  ProjectData project; // the project
  string infile;   // file argument given to the program
  string raweb_dir;  // the main tralics dir
  string raweb_dir_src; // is raweb_dir + "/src/"
  string no_year; // is miaou
  string raclass; // is ra2003
  string year_string; // is 2003
  string dclass; // documentclass of the file
  string type_option; // the type option
  string dtd,dtdfile; // the dtd, and its external location
  string dtype; // the doc type found in the configuration file
  string start_date;  // string with date of start of run.
  string short_date;  // date of start of run.
  string tcf_file;
  string machine;
  string out_name; // Name of output file
  string in_dir; // Input directory
  string default_class; //
  int year;        // current year
  int env_number;     // number of environments seen
  int current_line;      // current line number
  int bibtex_size;
  int bibtex_extension_size;
  int tpa_mode;
  int dft; // default dtd for standard classes
  int cur_fp_len; // number of bytes sent to XML file
  LinePtr input_content;// content of the tex source
  LinePtr tex_source;   // the data to be translated
  LinePtr config_file;  // content of configuratrion file
  LinePtr from_config; // lines extracted from the configuration
  line_iterator doc_class_pos;
  system_type cur_os;
  // vector <EnvList> the_env; // the list of environments for checks
  vector <Istring> bibtex_fields_s;
  vector <Istring> bibtex_fields;
  vector <Istring> bibtex_extensions;
  vector <Istring> bibtex_extensions_s;
  vector <string> all_config_types;
  Buffer line_buffer; // buffer for current line

  Buffer b_after, b_current; // aux buffers.
  String external_prog;
  String version_string;
  int input_encoding;
  output_encoding_type output_encoding;
  output_encoding_type log_encoding;

  bool no_zerowidthspace;
  bool no_zerowidthelt;
  bool footnote_hack;
  bool prime_hack;
  bool use_all_sizes_sw;
  bool noent_names;
  bool interactive_math;
  bool shell_escape_allowed;
  bool find_words;
  bool handling_ra;
  bool no_undef_mac;
  bool use_font_elt_sw;
  bool pack_font_elt_sw;
  bool distinguish_refer;
  bool noconfig;
  bool nomathml;
  bool dualmath;
  bool old_phi;
  bool verbose;    // are we verbose ?
  bool dverbose;    // are we verbose at begin document ?
  bool silent;    // are we silent ?
  bool double_quote_att; // double quote as attribute value delimitor
  bool use_tcf;
  bool etex_enabled;
  bool use_math_variant;
  bool simplified_ra;
  bool todo_xml;
public:
  Stack* the_stack;   // pointer to the stack 
  StrHash SH;        // the XML hash table
  
public:
  MainClass();
  void add_to_from_config(int n,Buffer&b) { from_config.add(n,b,true); }
  void bad(string,string);
  void bad1(string,string);
  void bad_char_before_brace(int k, String s, String info);
  void bad_end_env(string,int);
  void bad_ignore_char(int k, String s);
  bool check_for_tcf(const string&);
  void check_section_use();
  string check_theme(const string&);
  void check_ur(const Buffer&);
  vector <Istring>& get_bibtex_fields() { return  bibtex_fields; }
  vector <Istring>& get_bibtex_fields_s() { return  bibtex_fields_s; }
  vector <Istring>& get_bibtex_extensions() { return  bibtex_extensions; }
  vector <Istring>& get_bibtex_extensions_s() { return  bibtex_extensions_s; }
  int get_fp_len() { return cur_fp_len; }
  bool get_footnote_hack() { return footnote_hack; }
  bool get_no_undef_mac() { return no_undef_mac; }
  string get_no_year() { return no_year; }
  string get_year_string() { return year_string; }
  int get_input_encoding() const { return input_encoding; }
  output_encoding_type get_output_encoding() const { return output_encoding;}
  output_encoding_type get_log_encoding() const { return log_encoding;}
  int get_tpa_mode() const { return tpa_mode; }
  bool get_prime_hack() const { return prime_hack; }
  bool get_math_variant() const { return use_math_variant; }
  bool get_zws_mode() const { return no_zerowidthspace; }
  bool get_zws_elt() const { return no_zerowidthelt; }
  String get_version() const { return version_string; }
  void handle_one_bib_file(string);
  void incr_cur_fp_len(int a) { cur_fp_len += a; }
  bool in_ra() const {  return handling_ra;}
  bool in_simple_ra() const {  return simplified_ra;}
  void inhibit_xml () { todo_xml = false; }
  bool is_verbose() const { return verbose; }
  bool is_interactive_math() const { return interactive_math; }
  bool is_shell_escape_allowed() const { return shell_escape_allowed; }
  bool non_interactive() const{ return !interactive_math;}
  void parse_args(int argc, char** argv);
  void parse_option(int&,int,char** argv);
  void banner();
  String print_os() const;
  void read_config_and_other();
  void RRbib(String);
  void run(); 
  void run(int n, char** argv);
  void set_distinguish(bool b) { distinguish_refer = b; } 
  void set_doc_class_pos(line_iterator x) { doc_class_pos = x; }
  void set_ent_names(String);
  void set_foot_hack(bool b) { footnote_hack = b; }
  void set_fp_len(int a) { cur_fp_len = a; }
  void set_input_encoding(int a);
  void set_tcf_file(string s) { tcf_file = s; use_tcf = true;}
  void set_use_font(bool b) { use_font_elt_sw = b; }
  void set_pack_font(bool b) { pack_font_elt_sw = b; }
  void set_use_sizes(bool b) { use_all_sizes_sw = b; }
  void set_start_date(string s) { start_date = s; } 
  void set_short_date(string s) { short_date = s; } 
  void set_default_class(string s) { default_class = s; }
  string get_short_date() const { return short_date; } 
  string get_default_class() const { return default_class; } 
  void set_tpa_status(String);
  bool use_all_sizes() const { return use_all_sizes_sw; }
  bool use_noent_names() const { return noent_names; }
  bool use_font_elt() const { return use_font_elt_sw; }
  bool use_old_phi() { return old_phi; }
  bool use_double_quote_att () {return  double_quote_att; }
  bool pack_font_elt() const { return pack_font_elt_sw; }
  void unexpected_eof(string,int);
  bool d_verbose() const { return dverbose; }
  void bad_year();
private:
  void after_main_text();
  int  append_checked_line ();
  void append_non_eof_line (String,int);
  int  append_nonempty_line ();
  void bad_mod (int a, string b, Buffer& c);
  void boot_bibtex(bool);
  void call_dvips (string);
  void check_all ();
  void check_before_begin(int k);
  bool check_for_alias_type(bool vb);
  String check_for_arg(int&p, int argc,char**argv);
  void check_for_input();
  void check_kw(int, Buffer&);
  void check_line(Buffer&);
  bool check_line_aux(Buffer&);
  void check_mod();
  void check_options();
  void check_presentation();
  void check_project(Buffer& a);
  void check_ra_dir();
  int  check_section();
  void check_year_string(int, bool);
  void dubious_command(int k, bool where);
  void end_document();
  void end_env(string);
  void end_mod();
  void end_with_help(int);
  bool find_config_file();
  bool find_document_type();
  void find_dtd();
  void find_field(String a);
  bool find_opt_field(String info);
  void finish_init();
  void finish_xml();
  bool get_a_new_line();
  void get_doc_type();
  void get_machine_name();
  void get_os();
  void get_type_from_config();
  void handle_latex2init(String file_name);
  void ignore_text();
  void make_perl_script();
  void merge_bib();
  void mkcfg();
  void mk_empty();
  void more_boot();
  bool need_script() 
  { return in_ra() && !in_simple_ra(); }
  void one_bib_file (bib_from pre, string bib);
  void open_main_file();
  void open_config_file();
  void out_gathered_math();
  void out_sep();
  void out_xml();
  void print_job ();
  void print_mods ();
  void print_mods_end (fstream*);
  void print_mods_end_xml ();
  void print_mods_start();
  void print_mods_start_xml();
  void print_nb_error(int n);
  void read_one_file();
  void run_ra();
  void run_simple_ra();
  void run_not_ra();
  void see_aux_info(int k);
  void see_name (String s);
  void see_name1();
  void see_name2();
  void see_name3();
  void see_project();
  void see_topic();
  void set_version();
  void show_input_size();
  String split_one_arg(String,int&);
  void start_document(Buffer& a);
  void start_env(string);
  void start_error();
  void open_log();
  void trans0();
  void usage_and_quit(int);
};


// parameterised data
class ParamDataSlot {
 public:
  string key;
  string value;
  bool is_used;
  ParamDataSlot(string a, string b) : key(a), value(b),is_used(false) {}
  ParamDataSlot(string a, string b,bool c) : key(a), value(b),is_used(c) {}
  void mark_used() { is_used = true; }
  bool matches(const string& x) { return is_used && x==key; } 
  bool no_topic() const { return !is_used; } 
  void to_buffer(Buffer&) const;
  void key_to_buffer(Buffer&) const;
};

// We maintain a list of ParamDataSlot.
class ParamDataList {
public:
  string name;
  vector <ParamDataSlot> data;
public:
  void check_other();
  bool its_me(const string& s) { return name == s; }
  ParamDataList(string s) : name (s) {}
  bool empty() const { return data.empty(); }
  void push_back(ParamDataSlot x) { data.push_back(x); }
  int size() const { return data.size(); }
  void keys_to_buffer(Buffer&) const;
  void reset() { data = vector <ParamDataSlot>(); }
};

class ParamDataVector {
public:
  vector<ParamDataList*> data;
  ParamDataVector();
  ParamDataList* find_list(const string& name,bool creat);
};

