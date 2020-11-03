#pragma once
#include "Line.h"

struct LineList : public std::list<Line> { // \todo use a vector instead
    int         cur_line{0};               // current line number
    std::string file_name;                 // file name associated to the lines
    size_t      encoding{1};               // current file encoding

    [[nodiscard]] auto dump_name() const -> std::string;

    void after_open();
    void before_close(bool sigforce);
    void clear_and_copy(LineList &X);
    void change_encoding(long wc);
    auto find_aliases(const std::vector<std::string> &SL, std::string &res) -> bool;
    void find_all_types(std::vector<std::string> &res);
    auto find_configuration() -> std::string;
    auto find_doctype() -> std::string;
    auto find_documentclass() -> std::string;
    void find_top_atts();
    auto find_top_val(const std::string &s, bool c) -> std::string;
    auto get_next_raw(Buffer &b) -> int;
    auto get_next_cv(Buffer &b, size_t w) -> int;
    auto get_next(Buffer &b) -> int;
    auto get_next(std::string &b, bool &cv) -> int;
    void insert(const std::string &c, bool cv = true);
    void insert_spec(int n, std::string c);
    void insert(const LineList &aux);
    void normalise_final_cr();
    void parse_and_extract_clean(const std::string &s);
    void parse_conf_toplevel() const;
    auto parse_and_extract(String s) const -> LineList;
    void print(std::ostream &outfile);
    void read(const std::string &x, int spec);
    auto read_from_tty(Buffer &b) -> int;
    void register_file();
    void reset(std::string x);
    auto skip_env(line_iterator_const C, Buffer &B) -> line_iterator_const;
    void splice_first(LineList &X); // \todo inline
    void split_string(std::string x, int l);
};

inline std::vector<LineList> file_pool; // pool managed by filecontents \todo static inline in LineList
