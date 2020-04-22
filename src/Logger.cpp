#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace {
    Buffer buffer_for_log2; // Only used in the following 2 functions
} // namespace

// finishes a sequence of characters.
void Logger::finish_seq() const {
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

void Logger::log_dump(const std::string &s) const {
    finish_seq();
    spdlog::trace("{{\\{}}}", s);
}

void Logger::log_finish() {
    log_is_open = false;
    if (main_ns::nb_errs == 0)
        spdlog::info("No error found.");
    else if (main_ns::nb_errs == 1)
        spdlog::warn("There was one error.");
    else
        spdlog::warn("There were {} errors.", main_ns::nb_errs);
    if (!filename.empty()) spdlog::info("For more information, see transcript file {}", filename);
}

void Logger::log_init(const std::string &name, bool /*status*/) {
    filename    = name;
    log_file    = std::make_shared<std::ofstream>(tralics_ns::open_file(name, true));
    log_is_open = true;

    spdlog::set_level(spdlog::level::trace);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(name + ".spdlog", true);
    spdlog::default_logger()->sinks().push_back(sink);
    spdlog::default_logger()->sinks()[0]->set_level(spdlog::level::info); // \todo Link this with verbose (later in startup)
}
