#pragma once
#include "codepoint.h"
#include <fstream>
#include <memory>
#include <string>

struct Logger {
    std::string   filename; // the name of the log file
    std::ofstream log_file; // the stream to which we print

    void log_finish() const; // \todo This belongs in the destructor but spdlog could die first
    void log_init(const std::string &name);

    static void finish_seq();
    static void log_dump(const std::string &s);
    static void out_single_char(codepoint c);
};

// By default, send things to the log file. In Logger, this defaulted to
// both console and file, those are all log_and_tty in the code for
// now. \todo So replace `the_log <<` with spdlog::trace, and `log_and_tty <<`
// with spdlog::info.
template <typename T> auto operator<<(Logger &L, const T &s) -> Logger & {
    L.log_file << s;
    return L;
}

// This one uses log encoding, vs utf8 for the ostream version \todo this is not
// really useful, we can decide that logs are in utf8
auto operator<<(Logger &fp, const codepoint &x) -> Logger &;

inline Logger  log_and_tty;
inline Logger &the_log = log_and_tty;
