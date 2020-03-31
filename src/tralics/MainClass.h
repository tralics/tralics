#pragma once
#include "../txio.h"
#include "../txstring.h"

class Stack;

class MainClass {
    std::string infile;       ///< file argument given to the program
    std::string no_year;      // is miaou
    std::string raclass;      // is ra2003
    std::string dclass;       // documentclass of the file
    std::string type_option;  // the type option
    std::string dtd, dtdfile; // the dtd, and its external location
    std::string dtype;        // the doc type found in the configuration file
    std::string out_name;     // Name of output file
    std::string in_dir;       // Input directory
    std::string ult_name;     // absolute name of input.ult
    std::string tcf_file;     ///< File name of the `tcf` to use, if found \todo std::optional<std::string> instead of use_tcf?

    int year{9876};      // current year
    int env_number{0};   // number of environments seen
    int current_line{0}; // current line number
    int bibtex_size{0};
    int bibtex_extension_size{0};
    int dft{3}; // default dtd for standard classes
    int trivial_math{1};

    LinePtr input_content; // content of the tex source
    LinePtr tex_source;    // the data to be translated
    LinePtr config_file;   // content of configuratrion file
    LinePtr from_config;   // lines extracted from the configuration

    system_type cur_os{};

    std::vector<std::string> all_config_types;
    std::vector<std::string> after_conf;

    bool find_words{false};
    bool noconfig{false};
    bool nomathml{false};
    bool dualmath{false};
    bool silent{false}; // are we silent ?
    bool use_tcf{false};
    bool etex_enabled{true};
    bool multi_math_label{false};
    bool load_l3{false};
    bool verbose{false}; ///< Are we verbose ?

public:
    Stack * the_stack{}; ///< pointer to the stack
    StrHash SH;          ///< the XML hash table

    std::string default_class;     ///< The default class
    std::string short_date;        ///< Date of start of run (short format) \todo short_date and start_date seem to be identical
    std::string start_date;        ///< Date of start of run.
    std::string version{"2.15.4"}; ///< Version of tralics \todo set at a more reasonable place
    std::string year_string;       // is 2003

    std::vector<Istring> bibtex_fields_s;
    std::vector<Istring> bibtex_fields;
    std::vector<Istring> bibtex_extensions;
    std::vector<Istring> bibtex_extensions_s;

    size_t               fp_len{0};                ///< number of bytes sent to XML file
    size_t               input_encoding{1};        ///< Encoding of the input file \todo one type to rule all encodings
    output_encoding_type log_encoding{en_boot};    ///< Encoding of the log file \todo this should always be UTF-8
    output_encoding_type output_encoding{en_boot}; ///< Encoding of the XML output \todo this should always be UTF-8

    line_iterator doc_class_pos;

    int tpa_mode{3};

    bool distinguish_refer{true}; ///< Something to do with bibliographies in RA \todo remove
    bool double_quote_att{false}; ///< double quote as attribute value delimitor
    bool dverbose{false};         ///< Are we verbose at begin document ?
    bool footnote_hack{true};     ///< Not sure what this activates
    bool handling_ra{true};       ///< Are we handling the INRIA RA from the 2000s? \todo remove all references to the RA
    bool interactive_math{false}; ///< Are we in interactive mode? \todo get rid of interactive mode
    bool math_variant{false};
    bool no_entnames{false};
    bool no_undef_mac{false};
    bool no_xml{false}; ///< Are we in syntax-only mode (no XML output)?
    bool no_zerowidthelt{false};
    bool no_zerowidthspace{false};
    bool old_phi{false}; ///< Are we using `\phi` or `\varphi` to generate `&phi;`?
    bool pack_font_elt{false};
    bool prime_hack{false};
    bool shell_escape_allowed{false};
    bool use_all_sizes{false};
    bool use_font_elt{false};

    MainClass(); ///< This just adds `"../confdir"` to `conf_path` \todo should disappear

    auto check_for_tcf(const std::string &s) -> bool; ///< Look for a `.tcf` file, and if found set `tcf_file` and `use_tcf`

    void add_to_from_config(int n, Buffer &b); ///< Add contents to `from_config`
    void bad_year();                           ///< If the year is wrong, fail \todo this seems to be RA specific
    void run(int argc, char **argv);           ///< Do everything
    void set_ent_names(String s);              ///< Set no_entnames from a string saying yes or no
    void set_input_encoding(size_t wc);        ///< Set default input file encoding and log the action \todo remove?

    static auto check_theme(const std::string &s) -> std::string; ///< Check that theme is valid \todo RA specific?

private:
    void banner() const;                              ///< Prints the banner on the tty
    void check_section_use() const;                   ///< Not sure what this does, RA related
    void parse_args(int argc, char **argv);           ///< Parse the command-line arguments
    void parse_option(int &p, int argc, char **argv); ///< Interprets one command-line option, advances p
    void read_config_and_other();                     ///< Read the config file and extract all relevant information
    void set_tpa_status(String s);                    ///< Handles argument of -tpa_status switch

    auto append_nonempty_line() -> int;
    auto check_for_alias_type(bool vb) -> bool;
    auto check_for_arg(int &p, int argc, char **argv) -> String; ///< This gets foo, unless we are in the case tralics type=foo
    auto check_line_aux(Buffer &) -> bool;
    auto check_section() -> int;
    auto find_config_file() -> bool;   ///< Puts in main_ns::path_buffer the name of the config file
    auto find_document_type() -> bool; ///< Massage the output of get_doc_type
    auto find_opt_field(String info) -> bool;
    auto get_a_new_line() -> bool;
    auto split_one_arg(String a, int &p) -> String; ///< This considers the case of tralics conf_dir=foo
    void after_main_text();
    void append_non_eof_line(String, int);
    void bad_mod(int a, std::string b, Buffer &c);
    void boot_bibtex(bool inra);
    void call_dvips(std::string);
    void check_all();
    void check_before_begin(int k);
    void check_for_input(); ///< Reads the input file named in `infile`
    void check_kw(int, Buffer &);
    void check_line(Buffer &);
    void check_mod();
    void check_options();
    void check_presentation();
    void check_project(Buffer &a);
    void check_ra_dir();
    void check_year_string(int, bool);
    void dubious_command(int k, bool where);
    void end_document();
    void end_env(std::string);
    void end_mod();
    void end_with_help(int v);
    void find_dtd(); ///< Finds the DTD, create default if nothing given
    void find_field(String a);
    void finish_init() const; ///< RA related stuff
    void finish_xml();
    void get_doc_type(); ///< Determine document type from various sources
    void get_machine_name();
    void get_os();               ///< Sets cur_os to the current OS as a symbolic string
    void get_type_from_config(); ///< Extracts a type from the configuration file
    void handle_latex2init(String file_name);
    void ignore_text();
    void make_perl_script();
    void merge_bib();
    void mkcfg();
    void more_boot() const; ///< Finish bootstrapping
    void one_bib_file(bib_from pre, std::string bib);
    void open_config_file(); ///< Reads the config file b=named in `main_ns::path_buffer`
    void open_log();         ///< Opens the log file, prints the banner ann all information
    void open_main_file();
    void out_gathered_math();
    void out_sep();
    void out_xml(); ///< Ouput the XML and compute the word list
    void print_job();
    void print_mods_end_xml();
    void print_mods_end(std::fstream *);
    void print_mods_start_xml();
    void print_mods_start();
    void print_mods();
    void print_nb_error(int n);
    void read_one_file();
    void run_not_ra();
    void run_ra();
    void run_simple_ra();
    void see_aux_info(int k);
    void see_name(String s); ///< Extract versions of a filename with and without ext
    void see_name1();        ///< Post-process file names
    void show_input_size();
    void start_document(Buffer &a);
    void start_env(std::string);
    void start_error();
    void trans0(); ///< Start the latex to XML translation
};

extern MainClass *the_main;
