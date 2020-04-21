#pragma once
#include "codepoint.h"
#include <fstream>
#include <memory>
#include <string>

struct Logger {
    std::string                    filename; // the name of the log file
    std::shared_ptr<std::ofstream> log_file; // the stream to which we print
    void                           finish_seq() const;
    static void                    out_single_char(codepoint c);
    void                           dump(String s) const;
    void                           dump0(String s) const;
    static void                    abort();
};

class FullLogger : public Logger {
public:
    bool verbose{false};
    void finish(int n);
    void init(const std::string &name, bool status);
    void unexpected_char(String s, int k);
};

using logger_fn = void(Logger &);

// By default, send things to the log file. In FullLogger, this defaulted to
// both console and file, those are all (Logger &)log_and_tty in the code for
// now. \todo So replace `the_log <<` with spdlog::trace, and `log_and_tty <<`
// with spdlog::info.
template <typename T> auto operator<<(Logger &L, const T &s) -> Logger & {
    *(L.log_file) << s;
    return L;
}

inline auto operator<<(Logger &L, logger_fn f) -> Logger & {
    f(L);
    return L;
}

inline auto operator<<(FullLogger &L, logger_fn f) -> FullLogger & {
    f(L);
    return L;
}

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
auto operator<<(Logger &fp, const codepoint &x) -> Logger &;

inline FullLogger log_and_tty;
inline Logger &   the_log = log_and_tty;
