#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <spdlog/spdlog.h>

void Logger::log_dump(const std::string &s) {
    finish_seq();
    spdlog::trace("{{\\{}}}", s);
}

void Logger::log_finish() {
    if (nb_errs == 0)
        spdlog::info("No error found.");
    else if (nb_errs == 1)
        spdlog::warn("There was one error.");
    else
        spdlog::warn("There were {} errors.", nb_errs);
}
