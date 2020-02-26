#include "tralics/MainClass.h"
#include "txlogger.h"
#include "txparam.h"
#include "txparser.h"

extern std::string all_themes;

void bad_conf(String s) {
    log_and_tty << "The configuration file for the RA is ra" << the_parser.get_ra_year() << ".tcf or ra.tcf\n"
                << "It must define a value for the parameter " << s << "\n"
                << "See transcript file " << the_log.get_filename() << " for details\n"
                << "No xml file generated\n"
                << lg_fatal;
    exit(1);
}

// There are 5 values required for the RA. In the case of themes,
// we just store a string with initial and final space, and use strstr
// for finding. The case of section is defined in txcheck.
// In the case of profession and affiliation, we add Other at the end.

void MainClass::finish_init() {
    if (in_ra()) {
        if (year <= 2003) all_themes = " 1a 1b 1c 2a 2b 3a 3b 4a 4b ";
        if (year <= 2014 && all_themes.empty()) bad_conf("theme_vals");
        if (config_data.data[0]->empty()) bad_conf("ur_vals");
        if (year >= 2007) {
            if (config_data.data[2]->empty()) bad_conf("profession_vals");
            if (year >= 2013)
                config_data.data[3]->reset(); // kill this
            else if (config_data.data[3]->empty())
                bad_conf("affiliation_vals");
        }
        int n = config_data.data[1]->size();
        if (n == 0) bad_conf("sections_vals");
        if (n < 2) bad_conf("Config file did not provide sections");
    }
    int n = config_data.data.size();
    for (int i = 2; i < n; i++) config_data.data[i]->check_other();
}

// This function is called when we translate a theme value.

auto MainClass::check_theme(const std::string &s) -> std::string {
    std::string res = Txbuf.add_with_space(s.c_str());
    if (strstr(all_themes.c_str(), Txbuf.c_str()) == nullptr) {
        err_buf.reset();
        if (s.empty())
            err_buf << "Empty or missing theme\n";
        else
            err_buf << "Invalid theme " << s << "\n";
        if (all_themes.empty())
            err_buf << "Configuration file defines nothing";
        else
            err_buf << "Valid themes are" << all_themes;
        the_parser.signal_error(the_parser.err_tok, "Bad theme");
    }
    return res;
}
