#include "tralics/Stats.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <spdlog/spdlog.h>

void Stats::io_convert_stats() {
    if (bad_chars != 0) spdlog::warn("Input conversion errors: {} char{}.", bad_chars, bad_chars > 1 ? "s" : "");
}

// Prints some statistics at end of run.
void Stats::token_stats() const {
    spdlog::trace("Math stats: {} formulas, {} kernels, {} trivial, {} \\mbox, {} large, {} small.", m_cv, m_k, m_trivial, m_spec_box,
                  m_large, m_small);
    if (nb_hdr != 0) spdlog::trace("Number of HdR: {}.", nb_hdr);
    spdlog::trace("Buffer merge {}.", m_merge); // \todo clean up
    spdlog::trace("Macros created {}, deleted {}; hash size {} + {}; footnotes {}.", nb_macros, nb_macros_del, hash_table.usage_normal,
                  hash_table.usage_unhashed, footnotes);
    spdlog::trace("Save stack +{} -{}.", level_up, level_down);
    spdlog::trace("Attribute list search {}({}) found {} in {} elements ({} at boot).", sh_find, sh_boot, sh_used,
                  the_main->the_stack->get_xid(), nb_xboot);
    spdlog::trace("Number of ref {}, of used labels {}, of defined labels {}, of ext. ref. {}.", nb_ref, nb_used_ref, nb_label_defined,
                  nb_href);
    if (the_parser.get_list_files()) {
        log_and_tty << " *File List*\n";
        log_and_tty << file_list;
        log_and_tty << " ***********\n";
    }
    io_convert_stats();
}
