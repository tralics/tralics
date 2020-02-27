#include "tralics/globals.h"
#include "txinline.h"
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

int pool_position{-1};

uint leftquote_val{'`'};
uint rightquote_val{'\''};

bool bad_minus{false};
bool compatibility{false};
bool nofloat_hack{false};
bool only_input_data{false};
bool raw_bib{false};
bool seen_enddocument{false};

std::array<std::array<codepoint, lmaxchar>, max_encoding - 2> custom_table;

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
    size_t n = V.size(), i = 0;
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

void obsolete(const std::string &s) { std::cout << "Obsolete option `-" << s << "' ignored\n"; }

// Initialises encoding tables
void check_for_encoding() {
    for (auto &i : custom_table)
        for (unsigned j = 0; j < lmaxchar; ++j) i[j] = codepoint(j);
}

void show_encoding(size_t wc, const std::string &name) {
    const std::string &wa = (wc == 0 ? " (UTF8)" : (wc == 1 ? " (iso-8859-1)" : " (custom)"));
    the_log << lg_start_io << "Input encoding is " << wc << wa << " for " << name << lg_end;
}

auto main_ns::try_conf(const std::string &prefix) -> bool {
    if (prefix.size() == 0) return false;
    Buffer b;
    b << prefix << bf_optslash << "book.clt";
    return tralics_ns::file_exists(b);
}

void main_ns::find_conf_path() {
    if (try_conf(conf_path[0])) return;
    String S = "/usr/share/tralics";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "/usr/lib/tralics/confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "/usr/local/lib/tralics/confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "/sw/share/tralics/confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "../confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "../../confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
    S = "/user/grimm/home/cvs/tralics/confdir";
    if (try_conf(S)) {
        conf_path.emplace_back(S);
        return;
    }
}

void main_ns::new_in_dir(String s) {
    Buffer &b = main_ns::path_buffer;
    b.reset();
    for (int i = 0;; i++) {
        char c = s[i];
        if (c == 0 || c == ':') {
            if (b.last_char() == '/') b.remove_last();
            if (b.size() == 1 && b[0] == '.') b.remove_last();
            input_path.push_back(b.to_string());
            b.reset();
            if (c == 0) return;
        } else
            b.push_back(c);
    }
}

void main_ns::check_in_dir() {
    for (auto &i : input_path)
        if (i.empty()) return;
    input_path.emplace_back("");
}

auto main_ns::hack_for_input(const std::string &s) -> std::string {
    Buffer B;
    B << s;
    int k = B.last_slash();
    the_parser.set_job_name(no_ext);
    std::string path;
    std::string fn = s;
    if (k > 0) {
        B.kill_at(static_cast<size_t>(k));
        path = B.to_string();
        if (out_dir.empty()) out_dir = path;
        fn = B.to_string(static_cast<size_t>(k) + 1);
    }
    B << bf_reset << fn;
    B.remove_last_n(4);
    file_name = B.to_string();
    if (log_name.empty()) log_name = file_name;
    if (k > 0 && input_path.size() == 1) {
        input_path[0] = path;
        input_path.emplace_back("");
        return fn;
    }
    return s;
}

void main_ns::register_file(LinePtr *x) { file_pool.push_back(x); }

auto main_ns::use_pool(LinePtr &L) -> bool {
    if (pool_position == -1) return false; // should not happen
    L.insert(*file_pool[static_cast<size_t>(pool_position)]);
    pool_position = -1;
    return true;
}

auto main_ns::search_in_pool(const std::string &name) -> bool {
    size_t n      = file_pool.size();
    pool_position = -1;
    for (unsigned i = 0; i < n; i++) {
        if (file_pool[i]->get_file_name() == name) {
            pool_position = static_cast<int>(i);
            return true;
        }
    }
    return false;
}

auto tralics_ns::find_in_confdir(const std::string &s, bool retry) -> bool {
    main_ns::path_buffer << bf_reset << s;
    if (main_ns::search_in_pool(s)) return true;
    if (file_exists(main_ns::path_buffer)) return true;
    if (!retry) return false;
    if (s.empty() || s[0] == '.' || s[0] == '/') return false;
    return main_ns::search_in_confdir(s);
}

auto main_ns::search_in_confdir(const std::string &s) -> bool {
    auto n = conf_path.size();
    for (auto i = n; i != 0; i--) {
        main_ns::path_buffer << bf_reset << conf_path[i - 1] << bf_optslash << s;
        if (tralics_ns::file_exists(main_ns::path_buffer)) return true;
    }
    return false;
}

auto tralics_ns::find_no_path(const std::string &s) -> bool {
    if (s.empty()) return false;
    main_ns::path_buffer << bf_reset << s;
    return file_exists(main_ns::path_buffer);
}

auto tralics_ns::find_in_path(const std::string &s) -> bool {
    if (s.empty()) return false;
    main_ns::path_buffer << bf_reset << s;
    if (main_ns::search_in_pool(s)) return true;
    if (s[0] == '.' || s[0] == '/') return file_exists(main_ns::path_buffer);
    auto n = input_path.size();
    for (size_t i = 0; i < n; i++) {
        const std::string &p = input_path[i];
        if (p.empty())
            main_ns::path_buffer << bf_reset << s;
        else
            main_ns::path_buffer << bf_reset << p << bf_optslash << s;
        if (file_exists(main_ns::path_buffer)) return true;
    }
    return false;
}

auto tralics_ns::exists(const std::vector<std::string> &ST, const std::string &d) -> bool {
    for (const auto &j : ST)
        if (j == d) return true;
    return false;
}

auto main_ns::find_param_type(String s) -> param_args {
    if (strcmp(s, "entnames") == 0) return pa_entnames;
    if (strcmp(s, "tpastatus") == 0) return pa_tpastatus;
    if (strcmp(s, "dir") == 0) return pa_dir;
    if (strcmp(s, "year") == 0) return pa_year;
    if (strcmp(s, "type") == 0) return pa_type;
    if (strcmp(s, "configfile") == 0) return pa_config;
    if (strcmp(s, "config") == 0) return pa_config;
    if (strcmp(s, "distinguishreferinrabib") == 0) return pa_refer;
    if (strcmp(s, "confdir") == 0) return pa_confdir;
    if (strcmp(s, "externalprog") == 0) return pa_external_prog;
    if (strcmp(s, "trivialmath") == 0) return pa_trivialmath;
    if (strcmp(s, "leftquote") == 0) return pa_leftquote;
    if (strcmp(s, "rightquote") == 0) return pa_rightquote;
    if (strcmp(s, "defaultclass") == 0) return pa_defaultclass;
    if (strcmp(s, "inputfile") == 0) return pa_infile;
    if (strcmp(s, "inputdata") == 0) return pa_indata;
    if (strcmp(s, "outputfile") == 0) return pa_outfile;
    if (strcmp(s, "o") == 0) return pa_outfile;
    if (strcmp(s, "inputdir") == 0) return pa_indir;
    if (strcmp(s, "inputpath") == 0) return pa_indir;
    if (strcmp(s, "outputdir") == 0) return pa_outdir;
    if (strcmp(s, "logfile") == 0) return pa_logfile;
    if (strcmp(s, "doctype") == 0) return pa_dtd;
    if (strcmp(s, "param") == 0) return pa_param;
    return pa_none;
}

auto main_ns::param_hack(String a) -> bool {
    Buffer B;
    B.reset_ptr();
    B.push_back(a);
    if (!B.find_equals()) return false;
    if (!B.backup_space()) return false;
    B.advance();
    B.skip_sp_tab();
    other_options.push_back(B.to_string(B.ptr1));
    other_options.push_back(B.to_string(B.ptr));
    return true;
}

auto main_ns::extract_year(Buffer &B, Buffer &C) -> int {
    size_t m = B.size(), n = m, k = 0;
    while (k < 4 && n > 0 && is_digit(B[n - 1])) {
        n--;
        k++;
    }
    int y = 0;
    for (auto i = n; i < m; i++) {
        y = 10 * y + B[i] - '0';
        C.push_back(B[i]);
    }
    B.set_last(n);
    return y;
}

void main_ns::check_year(int y, Buffer &C, const std::string &dclass, const std::string &Y) {
    if (y < 2000 || y >= 2100) the_main->bad_year();
    std::string raclass = std::string("ra") + C.to_string();
    if (dclass != raclass) {
        std::cout << "Illegal document class " << dclass << " should be " << raclass << "\n";
        exit(1);
    }
    if (Y.empty()) return;
    if (Y == C.to_string()) return;
    log_and_tty << "Option -year=" << Y << " incompatible with year in source file \n";
    log_and_tty << lg_fatal;
    exit(1);
}

void main_ns::check_lowercase(Buffer &B) {
    auto n = B.size();
    if (n == 0) {
        std::cout << "Illegal file name of the form safir/2002.tex\n";
        the_main->bad_year(); // never returns
    }
    for (size_t i = 0; i < n; i++)
        if (B[i] < 32 || B[i] > 127 || is_upper_case(B[i])) {
            std::cout << "Fatal error\n";
            std::cout << "Only lowercase letters allowed: " << B.c_str() << " \n";
            exit(1);
        }
}

auto tralics_ns::get_out_dir(const std::string &name) -> String {
    Buffer &B = main_ns::path_buffer;
    B << bf_reset << out_dir << bf_optslash << name;
    return B.c_str();
}

auto tralics_ns::get_short_jobname() -> std::string { return file_name; }
