#pragma once
#include "LineList.h"
#include <array>
#include <filesystem>
#include <optional>

class Stack;

class MainClass {
    std::filesystem::path                infile;   // file argument given to the program
    std::optional<std::filesystem::path> tcf_file; // File name of the `tcf` to use, if found
    std::filesystem::path                executable_path; // path to argv[0], normalized if possible

    std::string no_year;     // is miaou
    std::string raclass;     // is ra2003
    std::string dclass;      // documentclass of the file
    std::string type_option; // the type option
    std::string dtd;         // the DTD
    std::string dtype;       // the doc type found in the configuration file
    std::string out_name;    // Name of output file
    std::string in_dir;      // Input directory
    std::string ult_name;    // absolute name of input.ult

    int year{9876};      // current year
    int dft{3}; // default dtd for standard classes
    int trivial_math{1};

    LineList input_content; // content of the tex source
    LineList tex_source;    // the data to be translated
    LineList config_file;   // content of configuratrion file
    LineList from_config;   // lines extracted from the configuration

    system_type cur_os{};

    std::vector<std::string> all_config_types;
    std::vector<std::string> after_conf;

    bool noconfig{false};
    bool nomathml{false};
    bool dualmath{false};
    bool silent{false}; // are we silent ?
    bool etex_enabled{true};
    bool multi_math_label{false};
    bool load_l3{false};
    bool verbose{false}; // Are we verbose ?

public:
    std::vector<std::filesystem::path> conf_path;
    std::vector<std::filesystem::path> input_path;
    std::string                        file_name; // Job name, without directory
    std::string                        file_list; // TODO: vector of std::fs::path

    std::array<std::array<char32_t, lmaxchar>, max_encoding - 2> custom_table{};

    std::string tralics_version{"2.15.4"};

    std::string default_class; // The default class
    std::string short_date;    // Date of start of run (short format)

    std::vector<std::string> bibtex_fields_s;
    std::vector<std::string> bibtex_fields;
    std::vector<std::string> bibtex_extensions;
    std::vector<std::string> bibtex_extensions_s;

    size_t               input_encoding{1}; // Encoding of the input file TODO: one type to rule all encodings

    line_iterator doc_class_pos;

    int tpa_mode{3};
    int init_file_pos{0}; // position in init file
    int bad_chars{0};

    bool     double_quote_att{false}; // double quote as attribute value delimitor
    bool     dverbose{false};         // Are we verbose at begin document ?
    bool     footnote_hack{true};     // Not sure what this activates
    bool     math_variant{false};
    bool     no_entnames{false};
    bool     no_undef_mac{false};
    bool     no_xml{false}; // Are we in syntax-only mode (no XML output)?
    bool     no_zerowidthelt{false};
    bool     no_zerowidthspace{false};
    bool     pack_font_elt{false};
    bool     prime_hack{false};
    bool     shell_escape_allowed{false};
    bool     use_all_sizes{false};
    bool     use_font_elt{false};
    bool     bad_minus{false};
    bool     compatibility{false};
    bool     nofloat_hack{false};
    bool     no_xml_error{false};
    bool     use_quotes{false};
    char32_t leftquote_val{'`'};
    char32_t rightquote_val{'\''};

    auto check_for_tcf(const std::string &s) -> bool; // Look for a `.tcf` file, and if found set `tcf_file` and `use_tcf`
    [[nodiscard]] auto get_executable_path() const -> const std::filesystem::path & { return executable_path; }

    void add_to_from_config(int n, const std::string &b); // Add contents to `from_config`
    void run(int argc, char **argv);                      // Do everything
    void set_ent_names(const std::string &s);             // Set no_entnames from a string saying yes or no
    void set_input_encoding(size_t wc);                   // Set default input file encoding and log the action TODO: remove?
    auto search_in_confdir(const std::string &s) const -> std::optional<std::filesystem::path>; // Search for a file in conf_path

private:
    void parse_args(int argc, char **argv);           // Parse the command-line arguments
    void parse_option(int &p, int argc, char **argv); // Interprets one command-line option, advances p
    void read_config_and_other();                     // Read the config file and extract all relevant information
    void set_tpa_status(const std::string &s);
    auto check_for_alias_type(bool vb) -> bool;
    auto find_config_file() -> std::optional<std::filesystem::path>; // TODO: static in MainClass.cpp
    auto find_document_type() -> bool;
    void boot_bibtex();
    void check_for_input();
    void find_dtd();
    void get_doc_type();         // Determine document type from various sources
    void get_os();               // Sets cur_os to the current OS as a symbolic string
    void get_type_from_config();
    void more_boot() const; // Finish bootstrapping
    void open_config_file(std::filesystem::path f);
    void open_log();
    void out_xml();
    void see_name(std::filesystem::path s); // Extract versions of a filename with and without ext
    void show_input_size();
    auto trans0() -> bool; // Start the latex to XML translation
};

inline MainClass the_main;
