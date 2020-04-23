#pragma once
#include "codepoint.h"
#include <fstream>
#include <memory>
#include <string>

struct Logger {
    std::ofstream log_file; // the stream to which we print

    void log_init(const std::string &name);

    static void finish_seq();
    static void log_dump(const std::string &s);
    static void log_finish(); // \todo This belongs in the destructor but spdlog could die first
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

inline Logger  log_and_tty;
inline Logger &the_log = log_and_tty;
