#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics/marelle (Jose' Grimm) 2006 2015

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "txbuffer.h"
#include <fstream>

void readline(char *buffer, size_t screen_size); ///< Read a line from standart input (readline.cpp)

// Implements:  FileForInput, LinePtr, TexOutStream
// InputStack converter

// Class used for conversion to and from UTF8

class Converter {
public:
    std::string cur_file_name;        // file name to be converted
    int         cur_file_line{0};     // current line number
    bool        global_error{false};  // Is this line OK?
    bool        local_error{false};   // is this char OK ?
    bool        line_is_ascii{false}; // is this line ascii 7bit
    int         lines_converted{0};   // number of lines converted
    int         bad_lines{0};         // number of lines with errors
    int         bad_chars{0};         // number of errors
public:
    Converter();
    auto new_error() -> bool;
    void start_convert(int l);
};

// Whenever Tralics reads a file, it puts the result in a structure like this
struct Clines {            // \todo rename to Line or something
    int         number;    // the number of the current line
    std::string chars;     // the characters on the line
    bool        converted; // true if line is already converted

    Clines(int n, std::string c = "", bool C = true) : number(n), chars(std::move(c)), converted(C) {}

    auto to_string(std::string &C, bool &cv) const -> int {
        C  = chars;
        cv = converted;
        return number;
    }
    auto to_buffer(Buffer &b, bool &C) const -> int {
        b.push_back(chars);
        C = converted;
        return number;
    }
    auto starts_with(String x) const -> bool;
    void convert_line(size_t wc);
    void clear_converted() { converted = false; }
};

using line_iterator_const = std::list<Clines>::const_iterator;
using line_iterator       = std::list<Clines>::iterator;

struct LinePtr : public std::list<Clines> { // \todo rename to LineList or something
    int         cur_line{0};                // current line number
    bool        interactive{false};         // is this file or a tty ? \todo get rid of interactive mode
    std::string file_name;                  // file name associated to the lines
    size_t      encoding{1};                // current file encoding

    [[nodiscard]] auto dump_name() const -> String;

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
    void incr_cur_line() { cur_line++; }
    void insert(int n, const std::string &c, bool cv); // \todo is emplace_back
    void insert(const std::string &c, bool cv);
    void insert(String c);
    void insert_spec(int n, std::string c);
    void insert(const LinePtr &aux);
    void normalise_final_cr();
    void parse_and_extract_clean(String s);
    void parse_conf_toplevel() const;
    auto parse_and_extract(String s) const -> LinePtr;
    void print();
    void print(std::fstream *outfile);
    void print1(std::fstream *);
    void reset(std::string x);
    auto read_from_tty(Buffer &b) -> int;
    void set_cur_line(int x) { cur_line = x; }
    void set_encoding(size_t k) { encoding = k; }
    void set_file_name(std::string s) { file_name = std::move(s); }
    void set_interactive();
    auto skip_env(line_iterator_const C, Buffer &B) -> line_iterator_const;
    void splice_end(LinePtr &X);   // \todo inline
    void splice_first(LinePtr &X); // \todo inline
    void split_string(String x, int l);
};

// This allows us to temporarily read from elsewhere
class InputStack {
    std::vector<codepoint> B;
    states                 s;          // copy of scanner state
    LinePtr                L;          // the lines
    int                    line_no;    // the current line number
    TokenList              TL;         // saved token list
    std::string            name;       // name of the current file
    long                   restore_at; // catcode of @ to restore
    long                   file_pos;   // file position to restore
    size_t                 Bpos;       // position in B
    bool                   every_eof;  // True if every_eof_token can be inserted
    bool                   eof_outer;  // True if eof is outer
public:
    [[nodiscard]] auto get_name() const -> const std::string & { return name; }
    [[nodiscard]] auto get_line_no() const -> int { return line_no; }
    void               set_state(states X) { s = X; }
    auto               get_TL() -> TokenList & { return TL; }
    auto               get_B() -> std::vector<codepoint> & { return B; }
    [[nodiscard]] auto get_state() const -> states { return s; }
    [[nodiscard]] auto get_line_pos() const -> size_t { return Bpos; }
    void               set_line_pos(size_t x) { Bpos = x; }
    void               set_line_vector(const std::vector<codepoint> &x) { B = x; }
    [[nodiscard]] auto get_at_val() const -> long { return restore_at; }
    void               set_at_val(long x) { restore_at = x; }
    [[nodiscard]] auto get_file_pos() const -> long { return file_pos; }
    [[nodiscard]] auto get_every_eof() const -> bool { return every_eof; }
    [[nodiscard]] auto get_eof_outer() const -> bool { return eof_outer; }
    void               destroy();
    void               set_line_ptr(LinePtr &X) {
        L.clear_and_copy(X);
        X.set_file_name("");
        L.set_interactive(X.interactive);
    }
    void get_line_ptr(LinePtr &X) {
        X.clear_and_copy(L);
        X.set_interactive(L.interactive);
    }
    InputStack(std::string N, int l, states S, long cfp, bool eof, bool eof_o)
        : s(S), line_no(l), name(std::move(N)), restore_at(-1), file_pos(cfp), Bpos(0), every_eof(eof), eof_outer(eof_o) {}
};

// data structure associated to \input3=some_file.
class FileForInput {
    bool    open_flag{false}; // is this file active ?
    LinePtr the_lines;        // the lines that not yet read by TeX
    Buffer  cur_line;         // this holds the current line
    int     line_no{0};       // this holds the current line number
public:
    void open(const std::string &file, bool action);
    void close();
    FileForInput() = default;

    auto is_open() -> bool { return open_flag; }
    auto get_lines() -> LinePtr & { return the_lines; }
    void set_lines(LinePtr X) { the_lines = std::move(X); }
    auto get_buffer() -> Buffer & { return cur_line; }
    auto get_line_no() -> int & { return line_no; }
};

// From TeX:
// The sixteen possible \write streams are represented by the |write_file|
// array. The |j|th file is open if and only if |write_open[j]=true|. The last
// two streams are special; |write_open[16]| represents a stream number
// greater than 15, while |write_open[17]| represents a negative stream number,
// and both of these variables are always |false|.
// Since \write18 is special, we added another slot in write_open
class TexOutStream {
    std::array<std::fstream *, nb_input_channels> write_file;
    std::array<bool, nb_output_channels>          write_open;

public:
    TexOutStream();
    void               close(size_t chan);
    void               open(size_t chan, const std::string &file_name);
    [[nodiscard]] auto is_open(size_t i) const -> bool { return write_open[i]; }
    void               write(size_t chan, const std::string &s) { *(write_file[chan]) << s; }
};
