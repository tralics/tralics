#include "tralics/globals.h"
#include "txlogger.h"
#include "txparser.h"

std::vector<std::string> conf_path;
std::vector<std::string> input_path;
std::vector<std::string> other_options;

std::string log_name;
std::string file_name;
std::string out_dir;
std::string no_ext;
std::string opt_doctype;
std::string all_themes;
std::string everyjob_string;

std::vector<LinePtr *> file_pool;

int pool_position = -1;

void bad_conf(String s) {
    log_and_tty << "The configuration file for the RA is ra" << the_parser.get_ra_year() << ".tcf or ra.tcf\n"
                << "It must define a value for the parameter " << s << "\n"
                << "See transcript file " << the_log.get_filename() << " for details\n"
                << "No xml file generated\n"
                << lg_fatal;
    exit(1);
}

void set_everyjob(const std::string &s) { everyjob_string = s; }

void after_conf_assign(std::vector<std::string> &V) {
    int n = V.size();
    int i = 0;
    for (;;) {
        if (i >= n) return;
        ssa2 << bf_reset << V[i];
        i++;
        Buffer local_buf;
        local_buf << bf_reset << V[i];
        i++;
        bool res = assign(ssa2, local_buf);
        if (res) the_log << ssa2.c_str() << "=" << local_buf.c_str() << "\n";
    }
}
