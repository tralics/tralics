// The file contains the main data structures, and code to fill the tables.

#include "tralics/MainClass.h"
#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <ctime>
#include <fmt/format.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// This is the main function. It does the following: boot the parser and the
// math part, and execute the main prog.
auto main(int argc, char **argv) -> int {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    auto logger = std::make_shared<spdlog::logger>("tralics", spdlog::sinks_init_list{console_sink});
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("This is tralics {}, a LaTeX to XML translator", the_main.tralics_version);
    spdlog::info("Licensed under the CeCILL Free Software Licensing Agreement");
    the_parser.boot(); // create the hash table and all that
    the_main.run(argc, argv);
    return the_parser.nb_errs == 0 ? 0 : 2;
}
