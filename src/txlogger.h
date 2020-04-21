#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003,  2007, 2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Buffer.h"
#include "txid.h"
#include "txscaled.h"
#include <fstream>
#include <iostream>
#include <memory>

// This include file holds some declarations for printing objects
// and the classes that allow us to print either on the tty, the log file
// or both.

auto operator<<(std::ostream &fp, const Glue &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const Istring &L) -> std::ostream &;
auto operator<<(std::ostream &fp, const Macro &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const TokenList &L) -> std::ostream &;
auto operator<<(std::ostream &fp, const SthInternal &x) -> std::ostream &;
auto operator<<(std::ostream &fp, Token x) -> std::ostream &;
auto operator<<(std::ostream &fp, Xid X) -> std::ostream &;
auto operator<<(std::ostream &fp, const codepoint &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const Xml *T) -> std::ostream &;
auto operator<<(std::ostream &fp, const ScaledInt &x) -> std::ostream &;
auto operator<<(std::ostream &fp, const boundary_type &x) -> std::ostream &;

inline auto operator<<(std::ostream &fp, const Buffer &L) -> std::ostream & { return fp << L.c_str(); }

class Logger;
using logger_fn = void(Logger &);

class Logger {
    std::string filename; // the name of the log file
public:
    std::shared_ptr<std::ofstream> log_file; // the stream to which we print
    void                           finish_seq() const;
    static void                    out_single_char(codepoint c);
    void                           dump(String s);
    void                           dump0(String s);
    void                           set_file_name(std::string x) { filename = std::move(x); }
    static void                    abort();
    [[nodiscard]] auto             get_filename() const -> std::string { return filename; }
    auto                           operator<<(logger_fn f) -> Logger & {
        f(*this);
        return *this;
    }

    template <typename T> auto operator<<(const T &s) -> Logger & {
        *log_file << s;
        return *this;
    }
};

class FullLogger { // \todo public Logger
public:
    Logger L;
    bool   verbose{false};
    auto   operator<<(logger_fn f) -> FullLogger & {
        f(L);
        return *this;
    }
    void finish(int n);
    void init(std::string name, bool status);
    void unexpected_char(String s, int k);
};

// if X is of type logger, then X << lg_start; is the same as
// lg_start(X); and hence as  X.finish_seq();
// if Y is of type FullLogger, Y<< lg_start; is the same as
// lg_start(Y.L), hence Y.L.finish_seq();

inline void lg_flush(Logger &L) { (*(L.log_file)).flush(); }
inline void lg_start(Logger &L) { L.finish_seq(); }
inline void lg_start_io(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "++ ";
}
inline void lg_startstack(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "+stack: ";
}
inline void lg_startbrace(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "{";
}
inline void lg_startcond(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "+";
}
inline void lg_startif(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "{ifthenelse ";
}
inline void lg_startcalc(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "{calc ";
}
inline void lg_startbracebs(Logger &L) {
    L.finish_seq();
    *(L.log_file) << "{\\";
}
inline void lg_end(Logger &L) { *(L.log_file) << "\n"; }
inline void lg_endsentence(Logger &L) { *(L.log_file) << ".\n"; }
inline void lg_endbrace(Logger &L) { *(L.log_file) << "}\n"; }
inline void lg_arrow(Logger &L) { *(L.log_file) << "->"; }

auto operator<<(Logger &X, const ScaledInt &x) -> Logger &;
auto operator<<(Logger &X, const Glue &x) -> Logger &;
auto operator<<(Logger &X, const Macro &x) -> Logger &;
auto operator<<(Logger &X, const SthInternal &x) -> Logger &;
auto operator<<(Logger &fp, Token t) -> Logger &;
auto operator<<(Logger &fp, const codepoint &x) -> Logger &;
auto operator<<(FullLogger &fp, const codepoint &x) -> FullLogger &;
auto operator<<(FullLogger &fp, Token t) -> FullLogger &;
auto operator<<(FullLogger &fp, const Macro &x) -> FullLogger &;
auto operator<<(FullLogger &X, const ScaledInt &x) -> FullLogger &;
auto operator<<(FullLogger &X, String s) -> FullLogger &;
auto operator<<(FullLogger &X, const Istring &s) -> FullLogger &;
auto operator<<(FullLogger &X, int s) -> FullLogger &;
auto operator<<(FullLogger &X, long s) -> FullLogger &;
auto operator<<(FullLogger &X, size_t s) -> FullLogger &;
auto operator<<(FullLogger &X, const std::string &s) -> FullLogger &;
auto operator<<(FullLogger &X, char s) -> FullLogger &;
auto operator<<(FullLogger &X, unsigned char s) -> FullLogger &;
auto operator<<(FullLogger &X, const Buffer &s) -> FullLogger &;
auto operator<<(FullLogger &X, const TokenList &s) -> FullLogger &;
auto operator<<(FullLogger &X, const Xml *s) -> FullLogger &;

extern Logger &   the_log;
extern FullLogger log_and_tty;
