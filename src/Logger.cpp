#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <spdlog/sinks/basic_file_sink.h>
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

void Logger::log_finish(int n) {
    log_is_open = false;
    if (n == 0)
        (Logger &)(*this) << "No error found.\n";
    else if (n == 1)
        (Logger &)(*this) << "There was one error.\n";
    else
        (Logger &)(*this) << "There were " << n << " errors.\n";
    (Logger &)(*this) << "(For more information, see transcript file " << filename << ")\n";
}

void Logger::log_init(const std::string &name, bool status) {
    filename    = name;
    log_file    = std::make_shared<std::ofstream>(tralics_ns::open_file(name, true));
    verbose     = status;
    log_is_open = true;

    spdlog::set_level(spdlog::level::trace);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(name + ".spdlog", true);
    spdlog::default_logger()->sinks().push_back(sink);
    spdlog::default_logger()->sinks()[0]->set_level(spdlog::level::info); // \todo Link this with verbose (later in startup)
}
