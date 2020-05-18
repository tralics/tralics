// tralics, a LaTeX to XML translator
// Copyright INRIA (Jose Grimm) 2002, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// InterDepositDigitalNumber = IDDN.FR.001.510030.000.S.P.2002.000.31235
// for version 1.6, dated 05/12/2002.
// InterDepositDigitalNumber = IDDN.FR.001.510030.001.S.P.2002.000.31235
// for version 2.9.4, dated 18/02/2007.

// The file contains the main data structures, and code to fill the tables.

#include "tralics/NameMapper.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <ctime>
#include <fmt/format.h>

namespace main_ns {
    int  nb_errs         = 0;     // number of errors seen so far
    bool no_xml_error    = false; // should an error produce an XML element ?
    bool use_quotes      = false; // How to translate quotes
    bool bib_allow_break = true;  // <allowbreak> in bibliography
} // namespace main_ns

MainClass *the_main;     // the main class data structure
bool       ra_ok = true; // inhibits  redefinitions

// This is the main function. It does the following: boot the parser and the
// math part, and execute the main prog.
auto main(int argc, char **argv) -> int {
    spdlog::info("This is tralics {}, a LaTeX to XML translator", tralics_version);
    MainClass M;
    the_main    = &M;
    M.the_stack = &the_parser.the_stack;
    spdlog::info("Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm");
    the_parser.boot(); // create the hash table and all that
    spdlog::info("Licensed under the CeCILL Free Software Licensing Agreement");
    M.run(argc, argv);
    return main_ns::nb_errs == 0 ? 0 : 2;
}

// Function called when A=B is seen in the configuration file.
// Returns true if A is recognised
auto assign(const std::string &sa, const std::string &sb) -> bool {
    auto n = sa.size();

    if (sa.starts_with("elt_")) return the_names.assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return the_names.assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
    if (sa.starts_with("att_")) return the_names.assign_att(sa.substr(4), sb);

    if (sa == "lang_fr") {
        the_names.set(np_french, Istring(sb));
        return true;
    }
    if (sa == "lang_en") {
        the_names.set(np_english, Istring(sb));
        return true;
    }
    if (sa == "distinguish_refer_in_rabib") {
        if ((sb == "true") || (sb == "yes"))
            the_main->distinguish_refer = true;
        else if ((sb == "false") || (sb == "no"))
            the_main->distinguish_refer = false;
        else
            spdlog::warn("distinguish_refer_in_rabib = {} ignored");
        return true;
    }
    if (sa == "entity_names") {
        the_main->set_ent_names(sb);
        return true;
    }
    if (sa == "default_class") {
        the_main->default_class = sb;
        return true;
    }
    if (sa == "alternate_item") {
        if (sb == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (sb == "true")
            the_parser.hash_table.eval_let("item", "@item");
        return true;
    }
    if (sa == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << sb << "}";
        the_main->add_to_from_config(1, aux); // \todo no Buffer
        return true;
    }
    if (sa == "everyjob") {
        everyjob_string = fmt::format("\\everyjob={{{}}}", sb);
        return true;
    }
    if (sa == "no_footnote_hack") {
        if (sb == "true") the_main->footnote_hack = false;
        if (sb == "false") the_main->footnote_hack = true;
        return true;
    }
    if (sa == "use_font_elt") {
        if (sb == "true") the_main->use_font_elt = true;
        if (sb == "false") the_main->use_font_elt = false;
        return true;
    }
    if (sa == "use_all_sizes") {
        if (sb == "true") the_main->use_all_sizes = true;
        if (sb == "false") the_main->use_all_sizes = false;
        return true;
    }
    if (sa == "bibtex_fields") {
        Buffer(sb).interpret_bibtex_list(); // \todo without Buffer
        return false;
    }
    if (sa == "bibtex_extensions") {
        Buffer(sb).interpret_bibtex_extension_list();
        return false;
    }
    if (sa == "mfenced_separator_val") {
        if (sb == "NONE")
            the_names.set(np_separator, Istring());
        else
            the_names.set(np_separator, Istring(sb));
        return true;
    }
    if (sa.ends_with("_vals")) {
        Buffer B(sb);
        config_ns::interpret_list(sa.substr(0, n - 5), B); // \todo without Buffer
        return true;
    }
    if (sa == "mml_font_normal") {
        the_names.set(np_mml_normal, Istring(sb));
        return true;
    }
    if (sa == "mml_font_upright") {
        the_names.set(np_mml_upright, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold") {
        the_names.set(np_mml_bold, Istring(sb));
        return true;
    }
    if (sa == "mml_font_italic") {
        the_names.set(np_mml_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_italic") {
        the_names.set(np_mml_bold_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_script") {
        the_names.set(np_mml_script, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_script") {
        the_names.set(np_mml_bold_script, Istring(sb));
        return true;
    }
    if (sa == "mml_font_fraktur") {
        the_names.set(np_mml_fraktur, Istring(sb));
        return true;
    }
    if (sa == "mml_font_doublestruck") {
        the_names.set(np_mml_doublestruck, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_fraktur") {
        the_names.set(np_mml_bold_fraktur, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif") {
        the_names.set(np_mml_sansserif, Istring(sb));
        return true;
    }
    if (sa == "mml_font_bold_sansserif") {
        the_names.set(np_mml_bold_sansserif, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif_italic") {
        the_names.set(np_mml_sansserif_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_sansserif_bold_italic") {
        the_names.set(np_mml_sansserif_bold_italic, Istring(sb));
        return true;
    }
    if (sa == "mml_font_monospace") {
        the_names.set(np_mml_monospace, Istring(sb));
        return true;
    }
    return false;
}
