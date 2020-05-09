#pragma once
#include "Buffer.h"
#include "Clines.h"
#include <list>
#include <string>

struct LinePtr : public std::list<Clines> { // \todo rename to LineList or something \todo use a vector instead
    int         cur_line{0};                // current line number
    bool        interactive{false};         // is this file or a tty ? \todo get rid of interactive mode
    std::string file_name;                  // file name associated to the lines
    size_t      encoding{1};                // current file encoding

    [[nodiscard]] auto dump_name() const -> std::string;

    void add(int n, Buffer &b, bool cv);
    void add_buffer(Buffer &B, line_iterator C);
    void after_open();
    void before_close(bool sigforce);
    void clear_and_copy(LinePtr &X);
    void change_encoding(long wc);
    void find_tex_encoding();
    auto find_aliases(const std::vector<std::string> &SL, std::string &res) -> bool;
    void find_all_types(std::vector<std::string> &res);
    auto find_configuration(Buffer &B) -> std::string;
    void find_doctype(Buffer &B, std::string &res);
    auto find_documentclass(Buffer &B) -> std::string;
    void find_top_atts(Buffer &B);
    auto find_top_val(String s, bool c) -> std::string;
    void set_interactive(bool sw) { interactive = sw; }
    auto get_last_line_no() -> int { return back().number; }
    auto get_next_raw(Buffer &b) -> int;
    auto get_next_cv(Buffer &b, int w) -> int;
    auto get_next(Buffer &b) -> int;
    auto get_next(std::string &b, bool &cv) -> int;
    void insert(const std::string &c, bool cv);
    void insert(String c);
    void insert_spec(int n, std::string c);
    void insert(const LinePtr &aux);
    void normalise_final_cr();
    void parse_and_extract_clean(String s);
    void parse_conf_toplevel() const;
    auto parse_and_extract(String s) const -> LinePtr;
    void print(std::ostream &outfile);
    void reset(std::string x);
    auto read_from_tty(Buffer &b) -> int;
    void set_interactive();
    auto skip_env(line_iterator_const C, Buffer &B) -> line_iterator_const;
    void splice_end(LinePtr &X);   // \todo inline
    void splice_first(LinePtr &X); // \todo inline
    void split_string(std::string x, int l);
};
