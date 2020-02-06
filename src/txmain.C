// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007, 2008
// email tralics@sophia.inria.fr

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// 
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability. 
// 
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or 
// data to be ensured and,  more generally, to use and operate it in the 
// same conditions as regards security. 
// 
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.


#include "tralics.h"
#include "txpath.h"

const char* txmain_rcsid=
  "$Id: txmain.C,v 2.188 2015/11/10 17:32:46 grimm Exp $";

inline void MainClass::set_version ()
{ 
  version_string = "2.15.4";  // current version number
}

namespace {
  vector<string> after_conf;
  vector<string> conf_path;
  vector<string> input_path;
  vector<string> other_options;
  Buffer confbuf, bparse;
  int trivial_math=1;
  string user_config_file;
  string ult_name; // absolute name of input.ult
  string log_name; // Name of transcript file
  string file_name; // Job name, without directory
  string out_dir; // Output directory
  string no_ext; // file name without tex extension
  vector<LinePtr*> file_pool; // pool managed by filecontents
  int pool_position=-1;  // Position of file in pool
  string opt_doctype="";
  bool banner_printed = false;  // hack
  bool load_l3 = false;
  bool multi_math_label = false;
}

namespace tpage_ns {
  void after_conf_assign(vector<string>& V);
}

using main_ns::path_buffer;


bool nostraightquotes = false;
bool nofloat_hack = false;
uint leftquote_val='`';
uint rightquote_val='\'';
bool compatibility = false;
bool bad_minus = false;
bool only_input_data = false; // special hack
bool booted = false;
bool seen_enddocument = false;


namespace io_ns {
  void check_for_encoding();
  void show_encoding (int wc, const string& name);
}
namespace bib_ns {
  extern bool raw_bib;
}

namespace main_ns {
  void perl_assign(fstream*fp, String name, string value);
  void perl_assign(fstream*fp, String name, bool value);
  auto try_conf(const string &) -> bool;
  void find_conf_path();
  auto find_param_type(String s) -> param_args;
  auto search_in_confdir(const string &s) -> bool;
  void new_in_dir(String);
  void check_in_dir();
  auto hack_for_input(const string &s) -> string;
  auto search_in_pool(const string &name) -> bool;
  void register_file(LinePtr*);
  auto use_pool(LinePtr &L) -> bool;
  auto param_hack(String) -> bool;
  auto extract_year(Buffer &B, Buffer &C) -> int;
  void check_lowercase(Buffer&B);
  void check_year(int,Buffer&B,const string&,const string&);
}

extern void set_everyjob(const string& s);
using main_ns::log_and_tty;


// This funtion sets cur_os to the current operating system as a symbolic string
inline void MainClass::get_os ()
{
#if defined(__alpha)
  cur_os = st_decalpha; 
#elif defined(__sunsolaris)
  cur_os = st_solaris;
#elif defined(__linux)
  cur_os = st_linux;
#elif defined(__sgi)
  cur_os = st_solaris;
#elif defined(__hpux)
  cur_os = st_hp;
#elif defined(__APPLE__)
  cur_os = st_apple;
#elif defined(__CYGWIN__)
  cur_os = st_windows;
#elif defined(_MSC_VER)
  cur_os = st_windows;
#elif defined(_WIN32)
  cur_os = st_windows;
#else
  cur_os = st_unknown;
#endif
  machine = bparse.get_machine_name();
}

#ifdef _MSC_VER
#include <windows.h> // 'Sleep()'
void txsleep(int i) {
  Sleep(1000*i);
}
#else
#include <unistd.h> 
void   txsleep(int i) { sleep(i); }
#endif

#ifdef _MSC_VER
int tcgethostname(char *name , size_t len)
{
  return -1;
}
#else
auto txgethostname(char *name, size_t len) -> int {
  return gethostname(name,len);
}
#endif


// Converts the symbolic OS string to a real string
auto MainClass::print_os() const -> String {
  switch(cur_os) {
  case st_windows: return "Windows";
  case st_decalpha: return "Dec alpha";  
  case st_solaris: return "Solaris";
  case st_sgi: return "Sgi";  
  case st_apple: return "Apple";  
  case st_linux: return "Linux";
  default: return "Unknown";
  }
}

// Ctor of the main class
MainClass::MainClass() : infile(""), raweb_dir(TRALICSDIR) {
  conf_path.push_back(CONFDIR);
}

// ----------------------------------------------------------------------
// Setting paths


// Returns true if prefix is the path to he conf_path
// Tries to see if book.clt is there
auto main_ns::try_conf(const string &prefix) -> bool {
  int n = prefix.size(); 
  if(n==0) return false;
  Buffer& b = confbuf;
  b << bf_reset << prefix << bf_optslash << "book.clt";
  return tralics_ns::file_exists(b);
}

// The conf_path vector holds the default path, then user dirs
// If the default path is incorrect, tries to find a better path
void main_ns::find_conf_path()
{
  if(try_conf(conf_path[0])) return;
  String S = "/usr/share/tralics";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "/usr/lib/tralics/confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "/usr/local/lib/tralics/confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "/sw/share/tralics/confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "../confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "../../confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
  S = "/user/grimm/home/cvs/tralics/confdir";
  if(try_conf(S)) { conf_path.push_back(S); return; }
}

// Split a path
void main_ns::new_in_dir(String s)
{
  Buffer&b = path_buffer;
  b.reset();
  for(int i=0;;i++) {
    char c = s[i];
    if(c==0 || c==':') {
      if(b.last_char()=='/') b.remove_last();
      if(b.size()==1 && b[0] =='.') b.remove_last();
      input_path.push_back(b.to_string());
      b.reset();
      if(c==0) return;
    }
    else b.push_back(c);
  }
}

// Adds current directory in input path unless present
void main_ns::check_in_dir()
{
  int n = input_path.size();
  for(int i=0;i<n;i++)
    if(input_path[i].empty()) return;
  input_path.push_back("");
}

// If input file has the form foo/bar, then \jobname is foo/bar
// but in some case we prefer the short name bar
auto main_ns::hack_for_input(const string &s) -> string {
  Buffer&B = path_buffer;
  B << bf_reset << s;
  int k = B.last_slash();
  the_parser.set_job_name(no_ext);
  string path;
  string fn = s;
  if(k>0) {
    B.kill_at(k);
    path = B.to_string();
    if(out_dir.empty()) out_dir = path;
    fn = B.to_string(k+1);
  }
  B << bf_reset << fn;
  B.remove_last_n(4);
  file_name = B.to_string();
  if(log_name.empty()) log_name = file_name;
  if(k>0&&input_path.size()==1) {
    input_path[0] = path;
    input_path.push_back("");
    return fn;
  }
  return s;
}

// ----------------------------------------------------------------------
// Using paths

// There is a way to push the content of a file on a pool
void main_ns::register_file(LinePtr* x)
{
  file_pool.push_back(x);
}

auto main_ns::use_pool(LinePtr &L) -> bool {
  if(pool_position == -1) return false; // should not happen
  L.insert(*file_pool[pool_position]);
  pool_position = -1;
  return true;
}

// Returns true if the file is in the pool
auto main_ns::search_in_pool(const string &name) -> bool {
  int n = file_pool.size();
  pool_position = -1;
  for(int i=0;i<n;i++) {
    if(file_pool[i]->get_file_name()==name)  { 
      pool_position = i;
      return true;
    }
  }
  return false;
}

// Try to open the file, using alternate location if desired
// Resulting filename is in the buffer.
auto tralics_ns::find_in_confdir(const string &s, bool retry) -> bool {
  path_buffer << bf_reset << s;
  if(main_ns::search_in_pool(s)) return true;
  if(file_exists(path_buffer)) return true;
  if(!retry) return false;
  if(s.empty() || s[0]== '.' || s[0] == '/') return false;
  return main_ns::search_in_confdir(s);
}

// Searches only in conf_path
auto main_ns::search_in_confdir(const string &s) -> bool {
  int n = conf_path.size();
  for(int i=n-1; i>=0;i--) {
    path_buffer << bf_reset << conf_path[i] << bf_optslash << s;
    if(tralics_ns::file_exists(path_buffer)) return true;
  }
  return false;
}

auto tralics_ns::find_no_path(const string &s) -> bool {
  if(s.size()==0) return false;
  path_buffer << bf_reset << s;    
  return file_exists(path_buffer);
}

// This tries opens a TeX file
auto tralics_ns::find_in_path(const string &s) -> bool {
  if(s.size()==0) return false;
  path_buffer << bf_reset << s;    
  if(main_ns::search_in_pool(s))
    return true;
  if(s[0]== '.' || s[0] == '/')
    return file_exists(path_buffer);
  int n = input_path.size();
  for(int i=0;i<n;i++) {
    const string& p = input_path[i];
    if(p.empty())
      path_buffer << bf_reset << s;
    else 
      path_buffer << bf_reset << p << bf_optslash << s;
    if(file_exists(path_buffer)) return true;
  }
  return false;
}

// This reads the input file, it is in infile
// If you say tralics foo/bar; and no input path given
// this is the same as tralics -input_path=foo: -input_file=bar
// job name is 

void MainClass::check_for_input()
{
  main_ns::check_in_dir();
  string s = main_ns::hack_for_input(infile);
  if(!non_interactive()) {
    input_content.set_interactive(); 
    input_content.push_front(Clines(1,"foo",true));
    input_content.push_front(Clines(2,"foo",false));
    open_log();
    return;
  }
  if(!tralics_ns::find_in_path(s)) {
    cout << "Fatal error: Cannot open input file " << infile << "\n";
    exit(1);
  }
  s = path_buffer.to_string();
  path_buffer.decr_wptr();  path_buffer.decr_wptr();  path_buffer.decr_wptr();
  path_buffer.push_back("ult");
  ult_name = path_buffer.to_string();
  fstream* fp = new fstream(s.c_str(),std::ios::in);
  if (!fp) {
    cout << "Empty input file " << s << "\n";
    exit(1);
  }
  open_log();
  tralics_ns::read_a_file(input_content,s,4);
  if(input_content.is_empty()) {
    log_and_tty << "Empty input file " << s << "\n";
    exit(1);
  }
  {
    int wc = input_content.get_encoding();
    if (wc <0) {
      wc = the_main->get_input_encoding();
      input_content.set_encoding (wc);
    }
    io_ns::show_encoding (wc, "the main file");
  }

}

// ----------------------------------------------------------------------


// Fetches some version, OS and machine name, prints the banner on the tty
void MainClass::banner() 
{
  if(banner_printed) return;
  banner_printed = true;
  cout << "This is tralics "<< get_version() << ", a LaTeX to XML translator"
       << ", running on " << machine << "\n";
  cout << "Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm\n";
  cout << "Licensed under the CeCILL Free Software Licensing Agreement\n";
}


// This opens the log file, and prints the banner as well as all information
void MainClass::open_log()
{
  bool special = only_input_data;
  Buffer& B= b_after;
  B << bf_reset << out_dir << bf_optslash << log_name;
  B.put_at_end(".log");
  log_and_tty.init(B.to_string(), !special);
  main_ns::log_or_tty.L = main_ns::log_and_tty.L;
  main_ns::log_or_tty.verbose = !silent;
  if (special) main_ns::log_or_tty.verbose = false;
  if(output_encoding==en_boot)  output_encoding=en_utf8;
  if(log_encoding==en_boot)  log_encoding=output_encoding;
  the_log << "Transcript file of tralics " << get_version() << " for file " << infile <<"\n"
	  << "Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm\n"
	  << "Tralics is licensed under the CeCILL Free Software Licensing Agreement\n"
	  << start_date << "OS: " << print_os() << ", machine " << machine << "\n"<<lg_flush;
  if (special) 
    log_and_tty << "Starting translation of command line argument.\n";
  else
    log_and_tty << "Starting translation of file " << infile << ".\n";
  the_log << "Output encoding: ";
  if(output_encoding==en_utf8)
    the_log << "UTF8 ";
  else if(output_encoding==en_latin)
    the_log << "iso-8859-1";
  else if(output_encoding==en_ascii7)
    the_log << "iso-8859-1 (on 7bits)";
  else if(output_encoding==en_ascii8)
    the_log << "UTF8 (on 7bits)";
  else the_log << "random";
  if(log_encoding==output_encoding) the_log << " (idem transcript).\n";
  else if(log_encoding==en_utf8 ||log_encoding==en_ascii8)
    the_log << ", and UTF8 for transcript.\n";
  else if(log_encoding==en_latin || log_encoding==en_ascii7)
    the_log << ", and iso-8859-1 for transcript.\n";
  else the_log << ", and random for transcript.\n";
  Buffer& b = confbuf;
  b.reset();
  b << "Left quote is ";
  b.out_log(Utf8Char(leftquote_val), log_encoding);
  b <<" right quote is ";
  b.out_log(Utf8Char(rightquote_val), log_encoding);
  the_log << b << "\n";
  if(trivial_math) the_log << "\\notrivialmath=" << trivial_math << "\n";
  io_ns::check_for_encoding();
  if(!default_class.empty()) 
    the_log << "Default class is " << default_class << "\n";
  int n= input_path.size();
  if(n>1) {   
    b.reset();
    b <<  "Input path (";
    for(int i=0;i<n;i++) {
      if(i) b << ":";
      b << input_path[i];
    }
    the_log << b << ")\n"; 
  }
}

auto tralics_ns::exists(const vector<string> &ST, string d) -> bool {
  for(unsigned int j=0;j<ST.size(); j++)
    if(ST[j]==d) return true;
  return false;
}

void MainClass::set_ent_names(String s)
{
  if(strcmp(s,"true")==0) noent_names = false;
  else if(strcmp(s,"yes")==0) noent_names = false;
  else if(strcmp(s,"false")==0) noent_names = true;
  else if(strcmp(s,"no")==0) noent_names = true;
}

// This interprets the arguments of the Unix program
void MainClass::parse_args(int argc, char** argv)
{
  main_ns::find_conf_path();
  String s;
  if(argc==1) end_with_help(0);
  if(argc==2 && strcmp(argv[1],"-?")==0)
    usage_and_quit(0);
  for(int i=1;i<argc;i++) {
    s = argv[i];
    if(!s) continue;
    if(s[0]==0) continue;
    if(s[0]=='-') parse_option(i,argc,argv);
    else see_name(s);
  }
  if(infile.empty()) {
    banner ();
    cout << "Fatal: no source file given\n";
    end_with_help(1);
  }
  if(leftquote_val==0 || leftquote_val>= (1<<16))
    leftquote_val='`';
  if(rightquote_val==0 || rightquote_val>= (1<<16) )
    rightquote_val='\'';
}

// All these options take an argumet; You can say
// tralics type=foo, tralics type foo, tralics type = foo
// These are handled the same
auto main_ns::find_param_type(String s) -> param_args {
  if (strcmp(s, "entnames")==0) return pa_entnames;
  if (strcmp(s, "tpastatus")==0) return pa_tpastatus;
  if (strcmp(s, "dir")==0) return pa_dir;
  if (strcmp(s, "year")==0) return pa_year;
  if (strcmp(s, "type")==0)  return pa_type;
  if (strcmp(s, "configfile")==0) return pa_config;
  if (strcmp(s, "config")==0) return pa_config;
  if (strcmp(s, "distinguishreferinrabib")==0) return pa_refer;
  if (strcmp(s, "confdir")==0) return pa_confdir;
  if (strcmp(s, "externalprog")==0) return pa_external_prog;
  if (strcmp(s, "trivialmath")==0) return pa_trivialmath;
  if (strcmp(s, "leftquote")==0) return pa_leftquote;
  if (strcmp(s, "rightquote")==0) return pa_rightquote;
  if (strcmp(s, "defaultclass")==0) return pa_defaultclass;
  if(strcmp(s,"inputfile")==0) return pa_infile;
  if(strcmp(s,"inputdata")==0) return pa_indata;
  if(strcmp(s,"outputfile")==0) return pa_outfile;
  if(strcmp(s,"o")==0) return pa_outfile;
  if(strcmp(s,"inputdir")==0) return pa_indir;
  if(strcmp(s,"inputpath")==0) return pa_indir;
  if(strcmp(s,"outputdir")==0) return pa_outdir;
  if(strcmp(s,"logfile")==0) return pa_logfile;
  if(strcmp(s,"doctype")==0) return pa_dtd;
  if(strcmp(s,"param")==0) return pa_param;
  return pa_none;
}

// This considers the case of tralics conf_dir=foo
// puts confdir in the buffer, and returns that; sets p to the first 
// valid char after = sign, and 0 if none
auto MainClass::split_one_arg(String a, int &p) -> String {
  Buffer &B=bparse;
  p = 0;
  B.reset();
  int i = 0;
  for(;;) {
    char c = a[i];
    if(c==0) break;
    ++i;
    if(c=='=') { 
      while (a[i] && a[i]==' ') ++i;
      p = i;
      break;
    }
    if(c=='-' || c=='_') continue;
    B.push_back(c);
  }
  B.remove_space_at_end();
  if(B.empty()) {
    banner ();
    cout << "bad option " << a << "\n";
    usage_and_quit(1);
  } 
  return B.c_str();
}

// This gets foo, unless we are in the case tralics type=foo
// Here p is the position of the argument type.
auto MainClass::check_for_arg(int &p, int argc, char **argv) -> String {
  if(p>=argc-1)  {
    banner();
    cout << "Argument missing for option " << argv[p] << "\n";
    usage_and_quit(1);
  } 
  if(strcmp(argv[p+1],"=")==0) {
    if(p>=argc-2)  {
      banner();
      cout << "Argument missing for option " << argv[p] << "\n";
      usage_and_quit(1);
    } 
    p += 2;
    return argv[p];
  } else {
    ++ p;
    String a = argv[p];
    if(*a && *a==' ') ++a;
    return a;
  }
  return 0;
}

void obsolete(string s)
{
  cout << "Obsolete option `-" << s << "' ignored\n";
}


// This interprets one option. If the option takes k arguments
// it increments p by k
void MainClass::parse_option(int&p, int argc,char**argv)
{
  int eqpos=0;
  String s = split_one_arg(argv[p],eqpos);
  param_args special = main_ns::find_param_type(s);
  if(special != pa_none) {
    String a = argv[p] + eqpos;
    if(eqpos==0)  a = check_for_arg(p,argc,argv);
    switch(special) {
    case pa_entnames: set_ent_names(a); return;
    case pa_tpastatus: set_tpa_status(a);  return;
    case pa_dir: return;
    case pa_year:  year_string=a;  return;
    case pa_type:  type_option = a; return;
    case pa_config: user_config_file = a;  return;
    case pa_refer: 
      after_conf.push_back("distinguish_refer_in_rabib");
      after_conf.push_back(a);
      return;
    case pa_confdir: 
      if(a[0]=='0') return; // ignore empty component
      if(a[0] =='/' && a[1]=='0') return; // ignore root
      conf_path.push_back(a); 
      return;
    case pa_external_prog: obsolete(s);  return;
    case pa_trivialmath: trivial_math = atoi(a); return;
    case pa_leftquote: leftquote_val = strtol(a,NULL,16); return;
    case pa_rightquote: rightquote_val = strtol(a,NULL,16); return;
    case pa_defaultclass: default_class = a;  return;
    case pa_infile: see_name(a); return;
    case pa_indata: 
      interactive_math = true;
      only_input_data = true;
      see_name("texput");
      set_everyjob("\\usepackage{amsmath}" + string(a) + "\\stop");
      return;
    case pa_outfile: out_name = a; return;
    case pa_indir: main_ns::new_in_dir(a); return;
    case pa_outdir: out_dir = a; return;
    case pa_logfile: log_name = a; return;
    case pa_dtd: opt_doctype = a; return;
    case pa_param:
      if(main_ns::param_hack(a)) return;
      if(p>=argc-1) { return; }
      other_options.push_back(a);
      other_options.push_back(argv[p+1]);
      p++;
      return;      
    default:; // Should be pa_none
    }
  }
  if(eqpos) {
    banner ();
    cout << "Illegal equal sign in option " << argv[p] << "\n";
    usage_and_quit(1);
  } 
  else if(strcmp(s,"verbose")==0) verbose = true;
  else if(strcmp(s,"verbose-doc")==0) dverbose = true;
  else if(strcmp(s,"v")==0) verbose = true;
  else if(strcmp(s,"V")==0) dverbose = true;
  else if(strcmp(s,"silent")==0) silent = true;
  else if (strcmp(s,"version")==0) {banner (); exit(0); }
  else if (strcmp(s,"rawbib")==0) bib_ns::raw_bib=true;
  else if (strcmp(s, "radebug")==0) obsolete(s);
  else if (strcmp(s, "check")==0) obsolete(s);
  else if (strcmp(s, "ps")==0) obsolete(s);
  else if (strcmp(s, "utf8")==0) input_encoding = 0;
  else if (strcmp(s, "utf8output")==0) log_encoding=output_encoding = en_utf8;
  else if (strcmp(s, "oe8")==0) output_encoding = en_utf8;
  else if (strcmp(s, "oe8a")==0) output_encoding = en_ascii8;
  else if (strcmp(s, "oe1")==0) output_encoding = en_latin; 
  else if (strcmp(s, "oe1a")==0) output_encoding = en_ascii7;
  else if (strcmp(s, "te8")==0) log_encoding = en_utf8;
  else if (strcmp(s, "te8a")==0) log_encoding = en_ascii8;
  else if (strcmp(s, "te1")==0) log_encoding = en_latin; 
  else if (strcmp(s, "te1a")==0) log_encoding = en_ascii7;
  else if (strcmp(s, "latin1")==0) input_encoding = 1;
  else if (strcmp(s, "noentnames")==0) noent_names = true; 
  else if (strcmp(s, "nomultimathlabel")==0) multi_math_label = false; 
  else if (strcmp(s, "multimathlabel")==0) multi_math_label = true; 
  else if (strcmp(s, "nofloathack")==0) nofloat_hack = true; 
  else if (strcmp(s, "noprimehack")==0) prime_hack = false; 
  else if (strcmp(s, "primehack")==0) prime_hack = true; 
  else if (strcmp(s, "doublequoteatt")==0) double_quote_att = true; 
  else if (strcmp(s, "notrivialmath")==0) trivial_math = 0;
  else if (strcmp(s, "nozerowidthspace")==0) no_zerowidthspace = true;
  else if (strcmp(s, "nozerowidthelt")==0) no_zerowidthelt = true;
  else if (strcmp(s, "shellescape")==0) shell_escape_allowed=true;
  else if (strcmp(s, "xml")==0) todo_xml = true ;
  else if (strcmp(s, "allowbreak")==0) main_ns::bib_allow_break = true ;
  else if (strcmp(s, "noallowbreak")==0) main_ns::bib_allow_break = false ;
  else if (strcmp(s, "etex")==0) etex_enabled = true;
  else if (strcmp(s, "noetex")==0)etex_enabled = false;
  else if (strcmp(s, "noxmlerror")==0) main_ns::no_xml_error=true;
  else if (strcmp(s, "l3")==0) load_l3=true;
  else if (strcmp(s, "xmlfo")==0)  obsolete(s);
  else if (strcmp(s, "xmlhtml")==0)  obsolete(s);
  else if (strcmp(s, "xmltex")==0)  obsolete(s);
  else if (strcmp(s, "xmllint")==0)  obsolete(s);
  else if (strcmp(s, "interactivemath")==0 || strcmp(s, "i")==0 
	   || strcmp(s, "adventure")==0) {
    interactive_math = true;input_encoding = 0;
    see_name("texput");
    if(s[0]=='a') set_everyjob("\\usepackage{dunnet}\\run");
  } else if (strcmp(s, "xmlall")==0 || strcmp(s, "all")==0)  obsolete(s);
  else if (strcmp(s, "nobibyearerror")==0)obsolete(s);
  else if (strcmp(s, "nobibyearmodify")==0) obsolete(s);
  else if (strcmp(s, "noundefmac")==0)  no_undef_mac = true;
  else if (strcmp(s, "noconfig")==0) noconfig = true;
  else if (strcmp(s, "compatibility")==0) compatibility = true;
  else if (strcmp(s, "oldphi")==0) old_phi = true;
  else if (strcmp(s, "badminus")==0) bad_minus = true;
  else if (strcmp(s, "nostraightquotes")==0) rightquote_val = 0xB4;
  else if (strcmp(s, "usequotes")==0) main_ns::use_quotes = true;
  else if (strcmp(s, "mathvariant")==0) use_math_variant = true;
  else if (strcmp(s, "nomathvariant")==0) use_math_variant = false;
  else if (strcmp(s, "nomathml")==0)  nomathml = true;
  else if (strcmp(s, "dualmath")==0)  dualmath = true;
  else if (strcmp(s, "findwords")==0) find_words = true;
  else if(strcmp(s,"help")==0) {
    usage_and_quit(0);
  } else {
    banner ();
    cout <<"bad option " << argv[p] << "\n";
    usage_and_quit(1);
  } 
}

auto main_ns::param_hack(String a) -> bool {
  Buffer&B =bparse; B.reset();B.reset_ptr();
  B.push_back(a);
  if(!B.find_equals()) return false;
  int J =B.get_ptr1();
  if(!B.backup_space()) return false;
  B.advance();
  B.skip_sp_tab();
  other_options.push_back(B.to_string(J));
  other_options.push_back(B.to_string(B.get_ptr()));
  return true;
}

// This explains the syntax of the tralics command.
void MainClass::usage_and_quit (int v)
{
  cout << "Syntax:\n";
  cout << "   tralics [options] source\n";
  cout << "source is the name of the source file,\n";
  cout << "   (with or without a extension .tex), does not start with a hyphen\n";
  cout << "\n";
  cout << "All options start with a single or double hyphen, they are:\n";
  cout << "  -verbose: Prints much more things in the log file\n";
  cout << "  -silent: Prints less information on the terminal\n";
  cout << "  -input_file FILE: translates file FILE\n";
  cout << "  -log_file LOG: uses LOG as transcript file\n";
  cout << "  -input_path PATH: uses PATH as dir list for input\n";
  cout << "  -output_dir DIR: pute result files in directory DIR\n";
  cout << "  -type FOO: Uses FOO instead of the \\documentclass value\n";
  cout << "  -config FILE: use FILE instead of default configuration file\n";
  cout << "  -confdir : indicates where the configuration files are located\n";
  cout << "  -noconfig: no configuration file is used\n";
  cout << "  -interactivemath: reads from the terminal, \n";
  cout << "      and prints math formulas on the terminal\n";
  cout << "  -utf8: says that the source is encoded in utf8 instead of latin1\n";
  cout << "  -latin1: overrides -utf8\n";
  cout << "  -utf8output: same as -oe8\n";
  cout << "  -oe8, -oe1, -oe8a -oe1a: specifies output encoding\n";
  cout << "  -te8, -te1, -te8a -te1a: terminal and transcript encoding\n";
  cout << "  -(no)trivialmath: special math hacking\n";
  cout << "  -(no)etex; enable or disable e-TeX extensions\n";
  cout << "  -nozerowidthelt: Use  &#x200B; rather than <zws/>\n";
  cout << "  -nozerowidthspace: no special &#x200B; or <zws/> inserted\n";
  cout << "  -noentnames: result contains &#A0; rather than &nbsp;\n";
  cout << "  -entnames=true/false: says whether or not you want &nbsp;\n";
  cout << "  -nomathml: this disables mathml mode\n";
  cout << "  -dualmath: gives mathML and nomathML mode\n";
  cout << "  -(no)math_variant: for <mi mathvariant='script'>X</mi>\n";
  cout << "  -(no)multi_math_label: allows multiple labels in a formula \n";
  cout << "  -noundefmac: alternate XML output for undefined commands\n";
  cout << "  -noxmlerror: no XML element is generated in case of error\n";
  cout << "  -no_float_hack: Removes hacks for figures and tables\n";
  cout << "  -nostraightquotes: same as right_quote=B4\n";
  cout << "  -left_quote=2018: sets translation of ` to char U+2018\n";
  cout << "  -right_quote=2019: sets translation of ' to char U+2019\n"; 
  cout << "  -param foo bar: adds foo=\"bar\" to the configuratin file\n";
  cout << "  -doctype=A-B; specifies the XML DOCTYPE\n";
  cout << "  -usequotes: double quote gives two single quotes\n";
  cout << "  -shell-escape: enable \\write18{SHELL COMMAND}\n";
  cout << "  -tpa_status = title/all: translate all document or title only\n";
  cout << "  -default_class=xx: use xx.clt if current class is unknown\n";
  cout << "  -raw_bib: uses all bibtex fields\n";
  cout << "  -distinguish_refer_in_rabib= true/false: special raweb hack \n";
  cout << "  (the list of all options is avalaible at\n"
       << "    http://www-sop.inria.fr/marelle/tralics/options.html )\n";
  cout << "\n";
  cout << "Tralics homepage: http://www-sop.inria.fr/marelle/tralics\n";
  cout << "This software is governed by the CeCILL license that can be\n";
  cout << "found at http://www.cecill.info.\n";
  exit (v);
}

// Handles argument of -tpa_status switch
void MainClass::set_tpa_status(String s)
{
  if(!s || s[0]==0) return; //
  if(s[0]=='a' || s[0]=='A') tpa_mode = 1; // case 'all'
  else if(s[0]=='t' || s[0]=='T') tpa_mode = 2; // case title
  else if(s[0]=='c' || s[0]=='C')  tpa_mode = 3; // case config
  else tpa_mode = 0; // default
}

void MainClass::end_with_help(int v)
{
  banner ();
  cout << "Say tralics --help to get some help\n";
  exit(v);
}

auto MainClass::check_for_tcf(const string &s) -> bool {
  string tmp = s + ".tcf";
  if(tralics_ns::find_in_confdir(tmp,true)) {
    set_tcf_file(path_buffer.to_string());
    return true;
  }
  return false;
}

// This puts in path_buffer the name of the config file.
// Returns false if not found.
auto MainClass::find_config_file() -> bool {
  if(noconfig) return false;
  Buffer& B = path_buffer;
  if(!user_config_file.empty()) {
    B << bf_reset << user_config_file;
    the_log << "Trying config file from user specs: " << B <<"\n";
    if(B[0]== '.' || B[0] == '/') return tralics_ns::file_exists(B);
    if(!B.is_at_end(".tcf"))
      return main_ns::search_in_confdir(user_config_file + ".tcf");
    else
      return main_ns::search_in_confdir(user_config_file);
  }
  // If interactive, read config only if given as parameter
  if(!non_interactive()) return false;
  string xclass = input_content.find_configuration(B);
  if(!xclass.empty()) {
    the_log << "Trying config file from source file `" << xclass <<"'\n";
    if(xclass.find('.')== string::npos)
      xclass = xclass + ".tcf";
    if(tralics_ns::find_in_confdir(xclass,true)) return true;
  }
  B.reset();
  String rc = ".tralics_rc";
  if(cur_os==st_windows)
    rc = "tralics_rc";
  B << bf_reset << rc;
  if(tralics_ns::file_exists(B)) return true;
  // Lines commented out were used instead of these two lines
  if(main_ns::search_in_confdir(rc)) return true;
  B.reset();
  return true;
}

// This opens the configuration file
// Note the special case where the buffer is empty.
// if the file is foo/bar2004.tcf, sets dtype to bar
void MainClass::open_config_file()
{
  Buffer&B = path_buffer;
  if(B.empty()) {
    config_file.insert("#comment",true);
    log_and_tty << "Dummy default configuration file used.\n";
    return;
  }
  tralics_ns::read_a_file(config_file,B.to_string(),0);
  config_file.normalise_final_cr();
  main_ns::log_or_tty << "Read configuration file " << B << ".\n";
  if(!B.is_at_end(".tcf")) return;
  // special case where the config file is a tcf file
  use_tcf = true;
  B.remove_last_n(4);
  int n = B.size();
  int k = B.last_slash();
  for(int i=n-1;; i--) {
    if(i<=k+1) break; 
    if(!is_digit(B[i])) { B.kill_at(i+1); break; }
  }
  dtype = B.to_string(k+1);
  the_log << "Using tcf type " << dtype << "\n";
}


// Extracts a type from the configuration file.
void MainClass::get_type_from_config()
{
  dtype = config_file.find_top_val("Type",true);
  if(dtype.empty()) 
    the_log << "No type in configuration file\n";
  else
    the_log << "Configuration file has type " << dtype << "\n";
  if(strncmp(dtype.c_str(), "\\documentclass",14) ==0)
    dtype = "";
}

// The type is either the value given on the command line, or in the
// config file, or the documentclass. Sets dft if a standard class is given.
void MainClass::get_doc_type()
{
  get_type_from_config();
  if(dclass.empty()) the_log << "No \\documentclass in source file\n";
  else the_log << "Seen \\documentclass " << dclass << "\n";
  if(!type_option.empty())
    dtype = type_option;
  else if(dtype.empty()) {
    dtype = dclass;
    if(dtype=="book") dft = 0;
    else if(dtype=="article") dft = 0;
    else if(dtype=="report") dft = 0;
    else if(dtype=="minimal") dft = 0;
    else if(dtype=="") return; // dft is 3
  }
  if (dtype.empty()) dtype= "unknown";
  the_log << "Potential type is " << dtype <<"\n";
}

// Returns true if u is a type defined in ST, or aliased to something
// Result in dtype.
auto MainClass::check_for_alias_type(bool vb) -> bool {
  if(dtype.empty()) return false;
  if(!check_for_tcf(dtype)) { 
    if(vb) the_log << "Trying type " << dtype << "\n";
    if(tralics_ns::exists(all_config_types,dtype))
      return true;
    if(!config_file.find_aliases(all_config_types, dtype))
      return false;
  }
  if(use_tcf) {
    tralics_ns::read_a_file(config_file,tcf_file,0);
    config_file.normalise_final_cr();
    log_and_tty << "Read tcf file " << tcf_file << "\n";
  }
  return true;
}

// Puts in dtype the type to use (returns false if no type found).
// Puts in dft a maker in case of a standard class.
auto MainClass::find_document_type() -> bool {
  get_doc_type();
  if(config_file.is_empty()) return false;
  if(dtype.empty()) return false;
  config_file.find_all_types(all_config_types);
  if(check_for_alias_type(false))   return true;
  dtype = b_after.remove_digits(dtype);
  if(check_for_alias_type(true))  return true;
  if(!all_config_types.empty()) {
    dtype = all_config_types.front();
    the_log << "Chosing first defined type " << dtype << "\n";
    return true;
  }
  dtype = "";
  return false;
}

// Finds the DTD. If nothing given, creates a default, for instance
// report from report.dtd
void MainClass::find_dtd()
{
  string res = opt_doctype;
  if(handling_ra || res.empty()) 
     res = config_file.find_top_val("DocType",false);
  b_after.extract_dtd(res.c_str(), dtd, dtdfile);
  if(dtdfile.empty()) {
    if(dft==3) { dtd = "unknown" ;  dtdfile = "unknown.dtd"; }
    else { dtd = "std"; dtdfile = "classes.dtd"; }
  }
  if(handling_ra) 
    the_log << "dtd is " << dtd << " from " << dtdfile
	    << " (mode RAWEB" << year << ")\n";
  else
    the_log << "dtd is " << dtd << " from " << dtdfile <<" (standard mode)\n";
}

// This reads the configuration file, and extracts all relevant information
// including the dtd, the titlepage information and some latex definitions.
void MainClass::read_config_and_other()
{
  year = the_parser.get_ra_year();
  bool have_dclass = !dclass.empty();
  if(find_config_file()) open_config_file();
  else main_ns::log_or_tty << "No configuration file.\n";
  bool found_type = use_tcf;
  if(!use_tcf) {
    found_type = find_document_type();
    if(dtype.empty())
      found_type = false;
    if(found_type)
      the_log << "Using type " << dtype << "\n";
    else
      the_log << "Using some default type\n";
    if(use_tcf) {} // config says to use a tcf
    else if(found_type) 
      config_file.parse_and_extract_clean(dtype.c_str());
    else config_file.clear();
  }
  config_file.parse_conf_toplevel();
  tpage_ns::after_conf_assign(other_options);
  tpage_ns::after_conf_assign(after_conf);
  string tmp = b_after.remove_digits(dtype);
  if(!tmp.empty()) dtype = tmp;

  bool hr = dtype== "ra" || dtype=="RA" || (dtype.empty() && dft==4);
  if(dclass.empty()) hr = false;
  handling_ra = hr;
  find_dtd(); 
  see_name1(); // this sets year_string.
  the_parser.set_default_language((hr && year <=2002) ? 1 :0);
  LinePtr cmds = config_file.parse_and_extract("Commands");
  from_config.splice_end(cmds);
  if(hr) from_config.insert("\\AtBeginDocument{\\rawebstartdocument}\n",true);
  config_file.find_top_atts(b_after);
  LinePtr TP = config_file.parse_and_extract("TitlePage");
  tralics_ns::Titlepage_create(TP);
  if(have_dclass && !handling_ra)
    from_config.insert("\\InputIfFileExists*+{" + ult_name + "}{}{}\n",true);
  input_content.add_buffer(from_config,doc_class_pos);
  config_file.clear();
}

void MainClass::bad_year()
{
  cout << "Fatal error: Input file name must be team name followed by " << year 
       << "\n";
  log_and_tty << lg_fatal;
  end_with_help(1);
}

// When we parse arguments and see a input file name, we call this procedure.
// It fills no_ext and infile, that is no_ext plus tex extension.
void MainClass::see_name (String s)
{
  Buffer& B = b_after;
  if(!infile.empty()) {
    cout << "Fatal error: Seen two source files " << infile << " and " 
      << s <<"\n";
    exit(1);
  }
  B << bf_reset << s;
  if(B.is_at_end(".xml")) B.remove_last_n(4);
  B.put_at_end(".tex");
  infile = B.to_string();
  B.remove_last_n(4);
  no_ext = B.to_string();
}

// If B holds apics2006, puts apics in B,  2006 in C, returns 2006 as int
auto main_ns::extract_year(Buffer &B, Buffer &C) -> int {
  int m = B.size();
  int n = m;
  int k = 0; 
  while(k<4 && n>0 && is_digit(B[n-1])) { n--; k++; }
  int y = 0;
  for(int i=n;i<m;i++) {
    y = 10*y + B[i] -'0';
    C.push_back(B[i]);
  }
  B.set_last(n);
  return y;
}

// Here y C are as above. We check valididy
void main_ns::check_year(int y, Buffer&C, const string&dclass, const string&Y)
{
  if (y<2000 || y>=2100) the_main->bad_year();
  string raclass = string("ra") + C.to_string();
  if(dclass != raclass) {
    cout << "Illegal document class " << dclass << " should be "
	 << raclass << "\n"; 
    exit(1);
  }
  if(Y.empty()) return;
  if(Y==C.to_string()) return;
  log_and_tty << "Option -year=" << Y
	      << " incompatible with year in source file \n";
  log_and_tty << lg_fatal;
  exit(1);
}

// Checks that this is a valid team name 
void main_ns::check_lowercase(Buffer&B)
{
  int n  = B.size();
  if(n==0) {
    cout << "Illegal file name of the form safir/2002.tex\n";
    the_main->bad_year(); // never returns
  }
  for(int i=0;i<n;i++) 
    if(uint(B[i])<32 || uint(B[i])>127 || is_upper_case(B[i])) {
      cout << "Fatal error\n";
      cout << "Only lowercase letters allowed: " << B.c_str() <<" \n";
      exit(1);
    }
}


// This is done when all arguments have been parsed. 
// In the raweb case, we split and extract the year from /tmp/apics2003.tex
void MainClass::see_name1()
{
  Buffer&B = b_after;
  Buffer C;
  B << bf_reset << no_ext;
  int y = 0;
  if(handling_ra) { // find and check the year from the file name
    y = main_ns::extract_year(B,C);
    main_ns::check_year(y,C,dclass,year_string);
  }
  int k = B.last_slash(); // remove the directory part
  if(k>=0) {
    string s = B.to_string(k+1);
    B << bf_reset << s;
  }
  the_parser.set_projet_val(B.to_string()); // this is apics
  if(handling_ra) {
    main_ns::check_lowercase(B);
    year_string = C.to_string(); 
    out_name = B.to_string(); // This is apics
    year = y;
    the_parser.set_ra_year(y);
    return;
  }
  if(out_name.empty()) { // might be given as an option
    out_name = no_ext;
    B << bf_reset << no_ext; // remove the directory part
    int k = B.last_slash();
    if(k>=0) out_name = B.to_string(k+1); // This is apics2003
  }
  if(year_string.empty()) {  // might be given as an option
    year = the_parser.get_ra_year();
    C << year;
    year_string = C.to_string();
  } else {
    year = atoi(year_string.c_str());
    the_parser.set_ra_year(year);
  }
}



// This starts the latex to XML translation.
void MainClass::trans0()
{
  the_log << "Starting translation\n"; 
  the_log << lg_flush;
  the_parser.init_all(dtd);
  if(multi_math_label) the_parser.word_define(multimlabel_code,1,false);
  if(nomathml) the_parser.word_define(nomath_code,-1,false);
  if(dualmath) the_parser.word_define(nomath_code,-3,false);
  the_parser.word_define(notrivialmath_code,trivial_math,false);
  if(verbose) the_parser.M_tracingall();
  the_parser.load_latex();
  if(load_l3) the_parser.L3_load(true);
  tralics_ns::Titlepage_start(verbose);
  if(only_input_data) {
    log_and_tty.finish(main_ns::nb_errs);
    tralics_ns::close_file(log_and_tty.L.fp);
    exit(0);
  }
}

// returns output_dir+name
auto tralics_ns::get_out_dir(string name) -> String {
  Buffer&B= path_buffer;
  B << bf_reset << out_dir << bf_optslash << name;
  return B.c_str();
}

auto tralics_ns::get_short_jobname() -> string { return file_name; }

void MainClass::boot_bibtex(bool inra)
{
  string mybbl = out_name + "_.bbl";
  String fn= tralics_ns::get_out_dir(mybbl);
  tralics_ns::bibtex_boot(fn,year_string.c_str(),
			  out_name,inra, distinguish_refer);
}
// --------------------------------------------------

inline void MainClass::show_input_size()
{
  int n = input_content.get_last_line_no();
  if(n==1)
    the_log << "There is a single line\n";
  else
    the_log << "There are " << n << " lines\n";
}

// Creates the .tex file
void MainClass::mk_empty()
{
  LinePtr* res=0;
  res = new LinePtr;
  res->reset(".tex"); 
  res->insert(1, "\\message{File ignored^^J}\\endinput",false);
  main_ns::register_file(res); 
}

// Finish bootstrapping
void MainClass::more_boot()
{
  tralics_ns::boot_math(get_math_variant());
  if(etex_enabled) the_parser.hash_table.boot_etex();
  mk_empty();
  the_parser.my_stats.after_boot();
  the_parser.the_stack.set_xid_boot();
  //  the_parser.the_stack.dump_xml_table();
  booted = true;
}

// This is main(int, char**) as a function in a class.

void MainClass::run(int n, char** argv)
{
  set_version();
  get_os();
  the_parser.boot(); // create the hash table and all that
  parse_args(n, argv); // look at arguments
  if (!only_input_data) banner();  // print banner
  more_boot();  // finish bootstrap
  check_for_input(); // open the input file
  dclass = input_content.find_documentclass(b_after);
  input_content.find_doctype(b_after,opt_doctype);
  read_config_and_other();
  finish_init();
  the_log << "OK with the configuration file, dealing with the TeX file...\n";
  show_input_size();
  try {
    boot_bibtex(in_ra());
    trans0(); 
    if(in_ra()) {
      if(the_names[np_language].null())
	the_names[np_language] = Istring("language");
      the_parser.add_language_att();
    }
    the_parser.init(input_content);
    the_parser.translate_all();
  } catch(...) {}
  check_section_use ();
  the_parser.after_main_text();
  if(seen_enddocument) the_parser.the_stack.add_nl();
  the_parser.final_checks();
  //    the_parser.the_stack.dump_xml_table();
  if(todo_xml) {
    the_parser.my_stats.token_stats();
    the_parser.finish_images();
    out_xml();
    log_and_tty.finish(main_ns::nb_errs);
  }
  else log_and_tty << "Nothing written to " << out_name << ".xml.\n";
  cout.flush();
  tralics_ns::close_file(log_and_tty.L.fp);
}

// This ouputs the XML and compute the word list
void MainClass::out_xml()
{
  Buffer X;
  string u = tralics_ns::get_out_dir(out_name);
  X << bf_reset << u;
  X.put_at_end(".xml");
  string name = X.to_string();
  fstream* fp = tralics_ns::open_file(name,true);
  X.reset();
  int aux = 4;
  if(output_encoding==en_utf8 || output_encoding==en_ascii8)
    X << "<?xml version='1.0' encoding='UTF-8'?>\n";
  else
    X << "<?xml version='1.0' encoding='iso-8859-1'?>\n";
  Istring sl = the_names[np_stylesheet];
  if(!sl.empty()) {
    aux ++;
    X << "<?xml-stylesheet href=\"" << sl.p_str() << "\" type=\"" 
      << the_names[np_stylesheet_type].p_str() << "\"?>\n";
  }
  X << "<!DOCTYPE "<< dtd << " SYSTEM '"<< dtdfile << "'>\n";
  X << "<!-- Translated from latex by tralics " << get_version() 
    << ", date: " << short_date << "-->\n";
  int a = X.length();
#if defined(WINNT) || defined (__CYGWIN__) || defined(_WIN32)
  a += aux;
#endif
  set_fp_len(a+1); // for the \n that follows
  *fp << X;
  *fp << the_parser.the_stack.document_element();
  *fp << "\n";
  log_and_tty << "Output written on " << name << " (" << get_fp_len() 
	      << " bytes).\n";
  tralics_ns::close_file(fp);
  if(the_main->find_words) {
    u = tralics_ns::get_out_dir(out_name);
    X << bf_reset << u;
    name = X.to_string();
    the_parser.the_stack.document_element()->word_stats(name);
  }
}



