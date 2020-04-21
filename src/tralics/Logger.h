#pragma once
#include "codepoint.h"
#include <fstream>
#include <memory>
#include <string>

struct Logger;
class ScaledInt;
class Glue;
class Macro;
class SthInternal;
struct Token;

using logger_fn = void(Logger &);

struct Logger {
    std::string                    filename; // the name of the log file
    std::shared_ptr<std::ofstream> log_file; // the stream to which we print
    void                           finish_seq() const;
    static void                    out_single_char(codepoint c);
    void                           dump(String s) const;
    void                           dump0(String s) const;
    static void                    abort();
    auto                           operator<<(logger_fn f) -> Logger & {
        f(*this);
        return *this;
    }
};

template <typename T> auto operator<<(Logger &L, const T &s) -> Logger & {
    *(L.log_file) << s;
    return L;
}

class FullLogger : public Logger {
public:
    bool verbose{false};
    auto operator<<(logger_fn f) -> FullLogger & {
        f(*this);
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

extern Logger &   the_log;
extern FullLogger log_and_tty;
