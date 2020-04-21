#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include <spdlog/spdlog.h>

namespace {
    Buffer buffer_for_log2; // Only used in the following 2 functions
} // namespace

// This is executed when there is a critical error. \todo not really useful anymore
void Logger::abort() { spdlog::critical("Fatal_error for {}", the_parser.get_job_name()); }

// finishes a sequence of characters.
void Logger::finish_seq() const {
    if (!buffer_for_log2.empty()) {
        *log_file << buffer_for_log2.convert_to_log_encoding() << ".\n";
        buffer_for_log2.reset();
    }
}

// starts a sequence of characters if needed, adds character c
void Logger::out_single_char(codepoint c) {
    if (buffer_for_log2.empty()) buffer_for_log2 << "Character sequence: ";
    buffer_for_log2 << c;
}

void Logger::dump(String s) const {
    finish_seq();
    *log_file << "{\\" << s << "}\n";
}

void Logger::dump0(String s) const {
    finish_seq();
    *log_file << "{" << s << "}\n";
}
