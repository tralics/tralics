#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <spdlog/spdlog.h>

namespace {
    Buffer buffer_for_log2; // Only used in the following 2 functions
} // namespace

// finishes a sequence of characters.
void Logger::finish_seq() {
    if (!buffer_for_log2.empty()) {
        spdlog::trace("{}.", buffer_for_log2.convert_to_log_encoding());
        buffer_for_log2.reset();
    }
}

// starts a sequence of characters if needed, adds character c
void Logger::out_single_char(codepoint c) {
    if (buffer_for_log2.empty()) buffer_for_log2 << "Character sequence: ";
    buffer_for_log2 << c;
}

void Logger::log_dump(const std::string &s) {
    finish_seq();
    spdlog::trace("{{\\{}}}", s);
}

void Logger::log_finish() {
    if (main_ns::nb_errs == 0)
        spdlog::info("No error found.");
    else if (main_ns::nb_errs == 1)
        spdlog::warn("There was one error.");
    else
        spdlog::warn("There were {} errors.", main_ns::nb_errs);
}

void Logger::log_init(const std::string &name) { log_file = tralics_ns::open_file(name, true); }
