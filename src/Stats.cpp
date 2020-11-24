#include "tralics/Stats.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <spdlog/spdlog.h>

// Prints some statistics at end of run. // \todo clean up
void Stats::token_stats() const {
    spdlog::trace("Number of ref {}, of used labels {}, of defined labels {}, of ext. ref. {}.", nb_ref, nb_used_ref, nb_label_defined,
                  nb_href);
    if (the_parser.get_list_files()) {
        log_and_tty << " *File List*\n";
        log_and_tty << file_list;
        log_and_tty << " ***********\n";
    }
    if (bad_chars != 0) spdlog::warn("Input conversion errors: {} char{}.", bad_chars, bad_chars > 1 ? "s" : "");
}
