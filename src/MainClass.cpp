#include "tralics/globals.h"
#include "txinline.h"
#include "txparser.h"
#include <filesystem>
#include <spdlog/spdlog.h>

namespace {
    Buffer b_after;

    std::string log_name;
    std::string machine;
    std::string no_ext;
    std::string opt_doctype;
    std::string out_dir;
    std::string user_config_file;

    std::vector<std::string> other_options;

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

    void bad_conf(String s) {
        log_and_tty << "The configuration file for the RA is ra" << the_parser.get_ra_year() << ".tcf or ra.tcf\n"
                    << "It must define a value for the parameter " << s << "\n"
                    << "See transcript file " << the_log.get_filename() << " for details\n"
                    << "No xml file generated\n"
                    << lg_fatal;
        exit(1);
    }

    /// Initialises encoding tables
    void check_for_encoding() {
        for (auto &i : custom_table)
            for (unsigned j = 0; j < lmaxchar; ++j) i[j] = codepoint(j);
    }

    /// Checks that name is non-empty and all lowercase
    void check_lowercase(Buffer &B) {
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

    void check_year(int y, Buffer &C, const std::string &dclass, const std::string &Y) {
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

    /// If B holds apics2006, puts apics in B, 2006 in C, returns 2006
    auto extract_year(Buffer &B, Buffer &C) -> int {
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
        B.reset(n);
        return y;
    }

    auto find_param_type(String s) -> param_args {
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

    /// Sometimes, we want `bar` if `\jobname` is `foo/bar`
    auto hack_for_input(const std::string &s) -> std::string {
        Buffer B;
        B << s;
        auto k = B.last_slash();
        the_parser.set_job_name(no_ext);
        std::string path;
        std::string fn = s;
        if (k && (*k > 0)) {
            B.at(*k) = 0;
            path     = B.to_string();
            if (out_dir.empty()) out_dir = path;
            fn = B.to_string(*k + 1);
        }
        B << bf_reset << fn;
        B.remove_last(4);
        file_name = B.to_string();
        if (log_name.empty()) log_name = file_name;
        if (k > 0 && input_path.size() == 1) {
            input_path[0] = path;
            input_path.emplace_back("");
            return fn;
        }
        return s;
    }

    /// Create an empty TeX file
    void mk_empty() {
        LinePtr res;
        res.reset(".tex");
        res.insert(1, "\\message{File ignored^^J}\\endinput", false);
        main_ns::register_file(std::move(res));
    }

    /// Display usage message, then exit the program \todo Manage this centrally
    void usage_and_quit(int v) {
        std::cout << "Syntax:\n";
        std::cout << "   tralics [options] source\n";
        std::cout << "source is the name of the source file,\n";
        std::cout << "   (with or without a extension .tex), does not start with a hyphen\n";
        std::cout << "\n";
        std::cout << "All options start with a single or double hyphen, they are:\n";
        std::cout << "  -verbose: Prints much more things in the log file\n";
        std::cout << "  -silent: Prints less information on the terminal\n";
        std::cout << "  -input_file FILE: translates file FILE\n";
        std::cout << "  -log_file LOG: uses LOG as transcript file\n";
        std::cout << "  -input_path PATH: uses PATH as dir list for input\n";
        std::cout << "  -output_dir DIR: pute result files in directory DIR\n";
        std::cout << "  -type FOO: Uses FOO instead of the \\documentclass value\n";
        std::cout << "  -config FILE: use FILE instead of default configuration file\n";
        std::cout << "  -confdir : indicates where the configuration files are located\n";
        std::cout << "  -noconfig: no configuration file is used\n";
        std::cout << "  -interactivemath: reads from the terminal, \n";
        std::cout << "      and prints math formulas on the terminal\n";
        std::cout << "  -utf8: says that the source is encoded in utf8 instead of latin1\n";
        std::cout << "  -latin1: overrides -utf8\n";
        std::cout << "  -utf8output: same as -oe8\n";
        std::cout << "  -oe8, -oe1, -oe8a -oe1a: specifies output encoding\n";
        std::cout << "  -te8, -te1, -te8a -te1a: terminal and transcript encoding\n";
        std::cout << "  -(no)trivialmath: special math hacking\n";
        std::cout << "  -(no)etex; enable or disable e-TeX extensions\n";
        std::cout << "  -nozerowidthelt: Use  &#x200B; rather than <zws/>\n";
        std::cout << "  -nozerowidthspace: no special &#x200B; or <zws/> inserted\n";
        std::cout << "  -noentnames: result contains &#A0; rather than &nbsp;\n";
        std::cout << "  -entnames=true/false: says whether or not you want &nbsp;\n";
        std::cout << "  -nomathml: this disables mathml mode\n";
        std::cout << "  -dualmath: gives mathML and nomathML mode\n";
        std::cout << "  -(no)math_variant: for <mi mathvariant='script'>X</mi>\n";
        std::cout << "  -(no)multi_math_label: allows multiple labels in a formula \n";
        std::cout << "  -noundefmac: alternate XML output for undefined commands\n";
        std::cout << "  -noxmlerror: no XML element is generated in case of error\n";
        std::cout << "  -no_float_hack: Removes hacks for figures and tables\n";
        std::cout << "  -nostraightquotes: same as right_quote=B4\n";
        std::cout << "  -left_quote=2018: sets translation of ` to char U+2018\n";
        std::cout << "  -right_quote=2019: sets translation of ' to char U+2019\n";
        std::cout << "  -param foo bar: adds foo=\"bar\" to the configuratin file\n";
        std::cout << "  -doctype=A-B; specifies the XML DOCTYPE\n";
        std::cout << "  -usequotes: double quote gives two single quotes\n";
        std::cout << "  -shell-escape: enable \\write18{SHELL COMMAND}\n";
        std::cout << "  -tpa_status = title/all: translate all document or title only\n";
        std::cout << "  -default_class=xx: use xx.clt if current class is unknown\n";
        std::cout << "  -raw_bib: uses all bibtex fields\n";
        std::cout << "  -distinguish_refer_in_rabib= true/false: special raweb hack \n";
        std::cout << "  (the list of all options is avalaible at\n"
                  << "    http://www-sop.inria.fr/marelle/tralics/options.html )\n";
        std::cout << "\n";
        std::cout << "Tralics homepage: http://www-sop.inria.fr/marelle/tralics\n";
        std::cout << "This software is governed by the CeCILL license that can be\n";
        std::cout << "found at http://www.cecill.info.\n";
        exit(v);
    }

    auto try_conf(const std::filesystem::path &dir) -> bool { return tralics_ns::file_exists(dir / "book.clt"); }

    /// Locate the config dir, using a few sources \todo do it better
    void find_conf_path() {
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

    /// Split a `:`-separated path list into paths
    void new_in_dir(String s) {
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

    void obsolete(const std::string &s) { spdlog::info("Obsolete option `-{}' ignored\n", s); }

    auto param_hack(String a) -> bool {
        Buffer B;
        B.ptr = 0;
        B.push_back(a);
        if (!B.find_equals()) return false;
        if (!B.backup_space()) return false;
        B.advance();
        B.skip_sp_tab();
        other_options.push_back(B.to_string(B.ptr1));
        other_options.push_back(B.to_string(B.ptr));
        return true;
    }

    auto print_os(system_type os) -> std::string {
        switch (os) {
        case st_windows: return "Windows";
        case st_decalpha: return "Dec alpha";
        case st_solaris: return "Solaris";
        case st_sgi: return "Sgi";
        case st_apple: return "Apple";
        case st_linux: return "Linux";
        default: return "Unknown";
        }
    }

    void show_encoding(size_t wc, const std::string &name) {
        const std::string &wa = (wc == 0 ? " (UTF8)" : (wc == 1 ? " (iso-8859-1)" : " (custom)"));
        the_log << lg_start_io << "Input encoding is " << wc << wa << " for " << name << lg_end;
    }
} // namespace

auto tralics_ns::get_out_dir(const std::string &name) -> String {
    Buffer &B = main_ns::path_buffer;
    B << bf_reset << out_dir << bf_optslash << name;
    return B.c_str();
}

MainClass::MainClass() {
#ifdef CONFDIR
    conf_path.emplace_back(CONFDIR);
#else
    conf_path.emplace_back("../confdir");
#endif
}

void MainClass::get_os() {
#if defined(__alpha)
    cur_os = st_decalpha;
#elif defined(__sunsolaris)
    cur_os = st_solaris;
#elif defined(__linux)
    cur_os = st_linux;
#elif defined(__sgi)
    cur_os = st_solaris;
#elif defined(__hpux)
    cur_os = st_hp;
#elif defined(__APPLE__)
    cur_os = st_apple;
#elif defined(__CYGWIN__)
    cur_os = st_windows;
#elif defined(_MSC_VER)
    cur_os = st_windows;
#elif defined(_WIN32)
    cur_os = st_windows;
#else
    cur_os = st_unknown;
#endif
    machine = Buffer().get_machine_name();
}

void MainClass::check_for_input() {
    if (std::none_of(input_path.begin(), input_path.end(), [](const auto &s) { return s.empty(); })) input_path.emplace_back("");

    std::string s = hack_for_input(infile);
    if (interactive_math) {
        input_content.set_interactive();
        input_content.push_front(Clines(1, "foo", true));
        input_content.push_front(Clines(2, "foo", false));
        open_log();
        return;
    }
    if (!tralics_ns::find_in_path(s)) {
        std::cout << "Fatal error: Cannot open input file " << infile << "\n";
        exit(1);
    }
    s = main_ns::path_buffer.to_string();
    main_ns::path_buffer.wptr -= 3;
    main_ns::path_buffer.push_back("ult");
    ult_name = main_ns::path_buffer.to_string();
    if (!std::filesystem::exists(s)) {
        std::cout << "Empty input file " << s << "\n";
        exit(1);
    }
    open_log();
    tralics_ns::read_a_file(input_content, s, 4);
    if (input_content.empty()) {
        log_and_tty << "Empty input file " << s << "\n";
        exit(1);
    }
    {
        auto wc = input_content.encoding;
        if (wc < 0) {
            wc = the_main->input_encoding;
            input_content.set_encoding(wc);
        }
        show_encoding(wc, "the main file");
    }
}

void MainClass::banner() const {
    static bool banner_printed = false;
    if (banner_printed) return;
    banner_printed = true;
    std::cout << "This is tralics " << version << ", a LaTeX to XML translator"
              << ", running on " << machine << "\n";
    std::cout << "Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm\n";
    std::cout << "Licensed under the CeCILL Free Software Licensing Agreement\n";
}

void MainClass::open_log() {
    bool    special = only_input_data;
    Buffer &B       = b_after;
    B << bf_reset << out_dir << bf_optslash << log_name;
    B.put_at_end(".log");
    log_and_tty.init(B.to_string(), !special);
    main_ns::log_or_tty.L       = log_and_tty.L;
    main_ns::log_or_tty.verbose = !silent;
    if (special) main_ns::log_or_tty.verbose = false;
    if (output_encoding == en_boot) output_encoding = en_utf8;
    if (log_encoding == en_boot) log_encoding = output_encoding;
    the_log << "Transcript file of tralics " << version << " for file " << infile << "\n"
            << "Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm\n"
            << "Tralics is licensed under the CeCILL Free Software Licensing Agreement\n"
            << start_date << "OS: " << print_os(cur_os) << ", machine " << machine << "\n"
            << lg_flush;
    if (special)
        log_and_tty << "Starting translation of command line argument.\n";
    else
        log_and_tty << "Starting translation of file " << infile << ".\n";
    the_log << "Output encoding: ";
    if (output_encoding == en_utf8)
        the_log << "UTF8 ";
    else if (output_encoding == en_latin)
        the_log << "iso-8859-1";
    else if (output_encoding == en_ascii7)
        the_log << "iso-8859-1 (on 7bits)";
    else if (output_encoding == en_ascii8)
        the_log << "UTF8 (on 7bits)";
    else
        the_log << "random";
    if (log_encoding == output_encoding)
        the_log << " (idem transcript).\n";
    else if (log_encoding == en_utf8 || log_encoding == en_ascii8)
        the_log << ", and UTF8 for transcript.\n";
    else if (log_encoding == en_latin || log_encoding == en_ascii7)
        the_log << ", and iso-8859-1 for transcript.\n";
    else
        the_log << ", and random for transcript.\n";
    Buffer b;
    b << "Left quote is ";
    b.out_log(codepoint(char32_t(leftquote_val)), log_encoding);
    b << " right quote is ";
    b.out_log(codepoint(char32_t(rightquote_val)), log_encoding);
    the_log << b << "\n";
    if (trivial_math != 0) the_log << "\\notrivialmath=" << trivial_math << "\n";
    check_for_encoding();
    if (!default_class.empty()) the_log << "Default class is " << default_class << "\n";
    auto n = input_path.size();
    if (n > 1) {
        b.reset();
        b << "Input path (";
        for (size_t i = 0; i < n; i++) {
            if (i != 0) b << ":";
            b << input_path[i];
        }
        the_log << b << ")\n";
    }
}

void MainClass::set_ent_names(String s) { // \todo bool is_yes(String)
    if (strcmp(s, "true") == 0)
        no_entnames = false;
    else if (strcmp(s, "yes") == 0)
        no_entnames = false;
    else if (strcmp(s, "false") == 0)
        no_entnames = true;
    else if (strcmp(s, "no") == 0)
        no_entnames = true;
}

void MainClass::add_to_from_config(int n, Buffer &b) { from_config.add(n, b, true); }

void MainClass::parse_args(int argc, char **argv) {
    find_conf_path();
    if (argc == 1) end_with_help(0);
    if (argc == 2 && strcmp(argv[1], "-?") == 0) usage_and_quit(0);
    for (int i = 1; i < argc; i++) {
        auto s = argv[i];
        if (s[0] == '-')
            parse_option(i, argc, argv);
        else
            see_name(s);
    }
    if (infile.empty()) {
        banner();
        std::cout << "Fatal: no source file given\n";
        end_with_help(1);
    }
    if (leftquote_val == 0 || leftquote_val >= (1 << 16)) leftquote_val = '`';
    if (rightquote_val == 0 || rightquote_val >= (1 << 16)) rightquote_val = '\'';
}

auto MainClass::split_one_arg(String a, int &p) -> String {
    static Buffer B;
    B.reset();
    p     = 0;
    int i = 0;
    for (;;) {
        char c = a[i];
        if (c == 0) break;
        ++i;
        if (c == '=') {
            while (a[i] == ' ') ++i;
            p = i;
            break;
        }
        if (c == '-' || c == '_') continue;
        B.push_back(c);
    }
    B.remove_space_at_end();
    if (B.empty()) {
        banner();
        std::cout << "bad option " << a << "\n";
        usage_and_quit(1);
    }
    return B.c_str();
}

auto MainClass::check_for_arg(int &p, int argc, char **argv) -> String {
    if (p >= argc - 1) {
        banner();
        std::cout << "Argument missing for option " << argv[p] << "\n";
        usage_and_quit(1);
    }
    if (strcmp(argv[p + 1], "=") == 0) {
        if (p >= argc - 2) {
            banner();
            std::cout << "Argument missing for option " << argv[p] << "\n";
            usage_and_quit(1);
        }
        p += 2;
        return argv[p];
    }
    ++p;
    String a = argv[p];
    if (*a == ' ') ++a;
    return a;
}

void MainClass::parse_option(int &p, int argc, char **argv) {
    int        eqpos   = 0;
    String     s       = split_one_arg(argv[p], eqpos);
    param_args special = find_param_type(s);
    if (special != pa_none) {
        String a = argv[p] + eqpos;
        if (eqpos == 0) a = check_for_arg(p, argc, argv);
        switch (special) {
        case pa_entnames: set_ent_names(a); return;
        case pa_tpastatus: set_tpa_status(a); return;
        case pa_dir: return;
        case pa_year: year_string = a; return;
        case pa_type: type_option = a; return;
        case pa_config: user_config_file = a; return;
        case pa_refer:
            after_conf.emplace_back("distinguish_refer_in_rabib");
            after_conf.emplace_back(a);
            return;
        case pa_confdir:
            if (a[0] == '0') return;                // ignore empty component
            if (a[0] == '/' && a[1] == '0') return; // ignore root
            conf_path.emplace_back(a);
            return;
        case pa_external_prog: obsolete(s); return;
        case pa_trivialmath: trivial_math = atoi(a); return;
        case pa_leftquote: leftquote_val = std::stoul(a, nullptr, 16); return;
        case pa_rightquote: rightquote_val = std::stoul(a, nullptr, 16); return;
        case pa_defaultclass: default_class = a; return;
        case pa_infile: see_name(a); return;
        case pa_indata:
            interactive_math = true;
            only_input_data  = true;
            see_name("texput");
            everyjob_string = "\\usepackage{amsmath}" + std::string(a) + "\\stop";
            return;
        case pa_outfile: out_name = a; return;
        case pa_indir: new_in_dir(a); return;
        case pa_outdir: out_dir = a; return;
        case pa_logfile: log_name = a; return;
        case pa_dtd: opt_doctype = a; return;
        case pa_param:
            if (param_hack(a)) return;
            if (p >= argc - 1) { return; }
            other_options.emplace_back(a);
            other_options.emplace_back(argv[p + 1]);
            p++;
            return;
        default:; // Should be pa_none
        }
    }
    if (eqpos != 0) {
        banner();
        std::cout << "Illegal equal sign in option " << argv[p] << "\n";
        usage_and_quit(1);
    } else if (strcmp(s, "verbose") == 0)
        verbose = true;
    else if (strcmp(s, "verbose-doc") == 0)
        dverbose = true;
    else if (strcmp(s, "v") == 0)
        verbose = true;
    else if (strcmp(s, "V") == 0)
        dverbose = true;
    else if (strcmp(s, "silent") == 0)
        silent = true;
    else if (strcmp(s, "version") == 0) {
        banner();
        exit(0);
    } else if (strcmp(s, "rawbib") == 0)
        raw_bib = true;
    else if (strcmp(s, "radebug") == 0)
        obsolete(s);
    else if (strcmp(s, "check") == 0)
        obsolete(s);
    else if (strcmp(s, "ps") == 0)
        obsolete(s);
    else if (strcmp(s, "utf8") == 0)
        input_encoding = 0;
    else if (strcmp(s, "utf8output") == 0)
        log_encoding = output_encoding = en_utf8;
    else if (strcmp(s, "oe8") == 0)
        output_encoding = en_utf8;
    else if (strcmp(s, "oe8a") == 0)
        output_encoding = en_ascii8;
    else if (strcmp(s, "oe1") == 0)
        output_encoding = en_latin;
    else if (strcmp(s, "oe1a") == 0)
        output_encoding = en_ascii7;
    else if (strcmp(s, "te8") == 0)
        log_encoding = en_utf8;
    else if (strcmp(s, "te8a") == 0)
        log_encoding = en_ascii8;
    else if (strcmp(s, "te1") == 0)
        log_encoding = en_latin;
    else if (strcmp(s, "te1a") == 0)
        log_encoding = en_ascii7;
    else if (strcmp(s, "latin1") == 0)
        input_encoding = 1;
    else if (strcmp(s, "noentnames") == 0)
        no_entnames = true;
    else if (strcmp(s, "nomultimathlabel") == 0)
        multi_math_label = false;
    else if (strcmp(s, "multimathlabel") == 0)
        multi_math_label = true;
    else if (strcmp(s, "nofloathack") == 0)
        nofloat_hack = true;
    else if (strcmp(s, "noprimehack") == 0)
        prime_hack = false;
    else if (strcmp(s, "primehack") == 0)
        prime_hack = true;
    else if (strcmp(s, "doublequoteatt") == 0)
        double_quote_att = true;
    else if (strcmp(s, "notrivialmath") == 0)
        trivial_math = 0;
    else if (strcmp(s, "nozerowidthspace") == 0)
        no_zerowidthspace = true;
    else if (strcmp(s, "nozerowidthelt") == 0)
        no_zerowidthelt = true;
    else if (strcmp(s, "shellescape") == 0)
        shell_escape_allowed = true;
    else if (strcmp(s, "xml") == 0)
        no_xml = false;
    else if (strcmp(s, "allowbreak") == 0)
        main_ns::bib_allow_break = true;
    else if (strcmp(s, "noallowbreak") == 0)
        main_ns::bib_allow_break = false;
    else if (strcmp(s, "etex") == 0)
        etex_enabled = true;
    else if (strcmp(s, "noetex") == 0)
        etex_enabled = false;
    else if (strcmp(s, "noxmlerror") == 0)
        main_ns::no_xml_error = true;
    else if (strcmp(s, "l3") == 0)
        load_l3 = true;
    else if (strcmp(s, "xmlfo") == 0)
        obsolete(s);
    else if (strcmp(s, "xmlhtml") == 0)
        obsolete(s);
    else if (strcmp(s, "xmltex") == 0)
        obsolete(s);
    else if (strcmp(s, "xmllint") == 0)
        obsolete(s);
    else if (strcmp(s, "interactivemath") == 0 || strcmp(s, "i") == 0 || strcmp(s, "adventure") == 0) {
        interactive_math = true;
        input_encoding   = 0;
        see_name("texput");
        if (s[0] == 'a') everyjob_string = "\\usepackage{dunnet}\\run";
    } else if (strcmp(s, "xmlall") == 0 || strcmp(s, "all") == 0)
        obsolete(s);
    else if (strcmp(s, "nobibyearerror") == 0)
        obsolete(s);
    else if (strcmp(s, "nobibyearmodify") == 0)
        obsolete(s);
    else if (strcmp(s, "noundefmac") == 0)
        no_undef_mac = true;
    else if (strcmp(s, "noconfig") == 0)
        noconfig = true;
    else if (strcmp(s, "compatibility") == 0)
        compatibility = true;
    else if (strcmp(s, "oldphi") == 0)
        old_phi = true;
    else if (strcmp(s, "badminus") == 0)
        bad_minus = true;
    else if (strcmp(s, "nostraightquotes") == 0)
        rightquote_val = 0xB4;
    else if (strcmp(s, "usequotes") == 0)
        main_ns::use_quotes = true;
    else if (strcmp(s, "mathvariant") == 0)
        math_variant = true;
    else if (strcmp(s, "nomathvariant") == 0)
        math_variant = false;
    else if (strcmp(s, "nomathml") == 0)
        nomathml = true;
    else if (strcmp(s, "dualmath") == 0)
        dualmath = true;
    else if (strcmp(s, "findwords") == 0)
        find_words = true;
    else if (strcmp(s, "help") == 0) {
        usage_and_quit(0);
    } else {
        banner();
        std::cout << "bad option " << argv[p] << "\n";
        usage_and_quit(1);
    }
}

void MainClass::set_tpa_status(String s) {   // \todo Erk this is not good
    if ((s == nullptr) || s[0] == 0) return; //
    if (s[0] == 'a' || s[0] == 'A')
        tpa_mode = 1; // case 'all'
    else if (s[0] == 't' || s[0] == 'T')
        tpa_mode = 2; // case title
    else if (s[0] == 'c' || s[0] == 'C')
        tpa_mode = 3; // case config
    else
        tpa_mode = 0; // default
}

void MainClass::end_with_help(int v) {
    banner();
    std::cout << "Say tralics --help to get some help\n";
    exit(v);
}

auto MainClass::check_for_tcf(const std::string &s) -> bool {
    std::string tmp = s + ".tcf";
    if (tralics_ns::find_in_confdir(tmp, true)) {
        tcf_file = main_ns::path_buffer.to_string(); // \todo without using path_buffer?
        use_tcf  = true;
        return true;
    }
    return false;
}

auto MainClass::find_config_file() -> bool {
    if (noconfig) return false;
    Buffer &B = main_ns::path_buffer;
    if (!user_config_file.empty()) {
        B << bf_reset << user_config_file;
        the_log << "Trying config file from user specs: " << B << "\n";
        if (B[0] == '.' || B[0] == '/') return tralics_ns::file_exists(B.to_string());
        if (!B.is_at_end(".tcf")) return static_cast<bool>(main_ns::search_in_confdir(user_config_file + ".tcf"));
        return static_cast<bool>(main_ns::search_in_confdir(user_config_file));
    }
    // If interactive, read config only if given as parameter
    if (interactive_math) return false;
    std::string xclass = input_content.find_configuration(B);
    if (!xclass.empty()) {
        the_log << "Trying config file from source file `" << xclass << "'\n";
        if (xclass.find('.') == std::string::npos) xclass = xclass + ".tcf";
        if (tralics_ns::find_in_confdir(xclass, true)) return true;
    }
    B.reset();
    String rc = ".tralics_rc";
    if (cur_os == st_windows) rc = "tralics_rc";
    B << bf_reset << rc;
    if (tralics_ns::file_exists(B.to_string())) return true;
    // Lines commented out were used instead of these two lines
    if (main_ns::search_in_confdir(rc)) return true;
    B.reset();
    return true;
}

void MainClass::open_config_file() {
    Buffer &B = main_ns::path_buffer;
    if (B.empty()) {
        config_file.insert("#comment", true);
        log_and_tty << "Dummy default configuration file used.\n";
        return;
    }
    tralics_ns::read_a_file(config_file, B.to_string(), 0);
    config_file.normalise_final_cr();
    main_ns::log_or_tty << "Read configuration file " << Istring(B) << ".\n";
    if (!B.is_at_end(".tcf")) return;
    // special case where the config file is a tcf file
    use_tcf = true;
    B.remove_last(4);
    auto n  = B.size();
    auto k  = B.last_slash();
    auto kk = k ? *k + 1 : 0UL;
    for (size_t i = n - 1;; i--) {
        if (i <= kk) break;
        if (!is_digit(B[i])) {
            B.at(i + 1) = 0;
            break;
        }
    }
    dtype = B.to_string(kk);
    the_log << "Using tcf type " << dtype << "\n";
}

void MainClass::get_type_from_config() {
    dtype = config_file.find_top_val("Type", true);
    if (dtype.empty())
        the_log << "No type in configuration file\n";
    else
        the_log << "Configuration file has type " << dtype << "\n";
    if (strncmp(dtype.c_str(), "\\documentclass", 14) == 0) dtype = "";
}

void MainClass::get_doc_type() {
    get_type_from_config();
    if (dclass.empty())
        the_log << "No \\documentclass in source file\n";
    else
        the_log << "Seen \\documentclass " << dclass << "\n";
    if (!type_option.empty())
        dtype = type_option;
    else if (dtype.empty()) {
        dtype = dclass;
        if (dtype == "book")
            dft = 0;
        else if (dtype == "article")
            dft = 0;
        else if (dtype == "report")
            dft = 0;
        else if (dtype == "minimal")
            dft = 0;
        else if (dtype.empty())
            return; // dft is 3
    }
    if (dtype.empty()) dtype = "unknown";
    the_log << "Potential type is " << dtype << "\n";
}

auto MainClass::check_for_alias_type(bool vb) -> bool {
    if (dtype.empty()) return false;
    if (!check_for_tcf(dtype)) {
        if (vb) the_log << "Trying type " << dtype << "\n";
        if (tralics_ns::exists(all_config_types, dtype)) return true;
        if (!config_file.find_aliases(all_config_types, dtype)) return false;
    }
    if (use_tcf) {
        tralics_ns::read_a_file(config_file, tcf_file, 0);
        config_file.normalise_final_cr();
        log_and_tty << "Read tcf file " << tcf_file << "\n";
    }
    return true;
}

auto MainClass::find_document_type() -> bool {
    get_doc_type();
    if (config_file.empty()) return false;
    if (dtype.empty()) return false;
    config_file.find_all_types(all_config_types);
    if (check_for_alias_type(false)) return true;
    dtype = b_after.remove_digits(dtype);
    if (check_for_alias_type(true)) return true;
    if (!all_config_types.empty()) {
        dtype = all_config_types.front();
        the_log << "Chosing first defined type " << dtype << "\n";
        return true;
    }
    dtype = "";
    return false;
}

void MainClass::find_dtd() {
    std::string res = opt_doctype;
    if (handling_ra || res.empty()) res = config_file.find_top_val("DocType", false);
    b_after.extract_dtd(res, dtd, dtdfile);
    if (dtdfile.empty()) {
        if (dft == 3) {
            dtd     = "unknown";
            dtdfile = "unknown.dtd";
        } else {
            dtd     = "std";
            dtdfile = "classes.dtd";
        }
    }
    if (handling_ra)
        the_log << "dtd is " << dtd << " from " << dtdfile << " (mode RAWEB" << year << ")\n";
    else
        the_log << "dtd is " << dtd << " from " << dtdfile << " (standard mode)\n";
}

void MainClass::read_config_and_other() {
    year             = the_parser.get_ra_year();
    bool have_dclass = !dclass.empty();
    if (find_config_file())
        open_config_file();
    else
        main_ns::log_or_tty << "No configuration file.\n";
    if (!use_tcf) {
        bool found_type = find_document_type();
        if (dtype.empty()) found_type = false;
        if (found_type)
            the_log << "Using type " << dtype << "\n";
        else
            the_log << "Using some default type\n";
        if (use_tcf) {
        } // config says to use a tcf
        else if (found_type)
            config_file.parse_and_extract_clean(dtype.c_str());
        else
            config_file.clear();
    }
    config_file.parse_conf_toplevel();
    after_conf_assign(other_options);
    after_conf_assign(after_conf);
    std::string tmp = b_after.remove_digits(dtype);
    if (!tmp.empty()) dtype = tmp;

    bool hr = dtype == "ra" || dtype == "RA" || (dtype.empty() && dft == 4);
    if (dclass.empty()) hr = false;
    handling_ra = hr;
    find_dtd();
    see_name1(); // this sets year_string.
    the_parser.set_default_language((hr && year <= 2002) ? 1 : 0);
    LinePtr cmds = config_file.parse_and_extract("Commands");
    from_config.splice_end(cmds);
    if (hr) from_config.insert("\\AtBeginDocument{\\rawebstartdocument}\n", true);
    config_file.find_top_atts(b_after);
    LinePtr TP = config_file.parse_and_extract("TitlePage");
    tralics_ns::Titlepage_create(TP);
    if (have_dclass && !handling_ra) from_config.insert("\\InputIfFileExists*+{" + ult_name + "}{}{}\n", true);
    input_content.splice(doc_class_pos, from_config);
    config_file.clear();
}

void MainClass::bad_year() {
    std::cout << "Fatal error: Input file name must be team name followed by " << year << "\n";
    log_and_tty << lg_fatal;
    end_with_help(1);
}

void MainClass::see_name(String s) {
    Buffer &B = b_after;
    if (!infile.empty()) {
        std::cout << "Fatal error: Seen two source files " << infile << " and " << s << "\n";
        exit(1);
    }
    B << bf_reset << s;
    if (B.is_at_end(".xml")) B.remove_last(4);
    B.put_at_end(".tex");
    infile = B.to_string();
    B.remove_last(4);
    no_ext = B.to_string();
}

void MainClass::see_name1() {
    Buffer &B = b_after;
    Buffer  C;
    B << bf_reset << no_ext;
    int y = 0;
    if (handling_ra) { // find and check the year from the file name
        y = extract_year(B, C);
        check_year(y, C, dclass, year_string);
    }
    auto k = B.last_slash(); // remove the directory part
    if (k) {
        std::string s = B.to_string(*k + 1);
        B << bf_reset << s;
    }
    the_parser.set_projet_val(B.to_string()); // this is apics
    if (handling_ra) {                        // \todo handling_ra should disappear from tralics alltogether
        check_lowercase(B);
        year_string = C.to_string();
        out_name    = B.to_string(); // This is apics
        year        = y;
        the_parser.set_ra_year(y);
        return;
    }
    if (out_name.empty()) { // might be given as an option
        out_name = no_ext;
        B << bf_reset << no_ext; // remove the directory part
        auto kk = B.last_slash();
        if (kk) out_name = B.to_string(*kk + 1); // This is apics2003
    }
    if (year_string.empty()) { // might be given as an option
        year = the_parser.get_ra_year();
        C << fmt::format("{}", year);
        year_string = C.to_string();
    } else {
        year = atoi(year_string.c_str());
        the_parser.set_ra_year(year);
    }
}

void MainClass::trans0() {
    the_log << "Starting translation\n";
    the_log << lg_flush;
    the_parser.init_all(dtd);
    if (multi_math_label) the_parser.word_define(multimlabel_code, 1, false);
    if (nomathml) the_parser.word_define(nomath_code, -1, false);
    if (dualmath) the_parser.word_define(nomath_code, -3, false);
    the_parser.word_define(notrivialmath_code, trivial_math, false);
    if (verbose) the_parser.M_tracingall();
    the_parser.load_latex();
    if (load_l3) the_parser.L3_load(true);
    tralics_ns::Titlepage_start(verbose);
    if (only_input_data) {
        log_and_tty.finish(main_ns::nb_errs);
        tralics_ns::close_file(log_and_tty.L.fp);
        exit(0);
    }
}

void MainClass::boot_bibtex(bool inra) {
    std::string mybbl = out_name + "_.bbl";
    String      fn    = tralics_ns::get_out_dir(mybbl);
    tralics_ns::bibtex_boot(fn, year_string.c_str(), out_name, inra, distinguish_refer);
}

void MainClass::show_input_size() {
    int n = input_content.get_last_line_no();
    if (n == 1)
        the_log << "There is a single line\n";
    else
        the_log << "There are " << n << " lines\n";
}

void MainClass::more_boot() const {
    tralics_ns::boot_math(math_variant);
    if (etex_enabled) the_parser.hash_table.boot_etex();
    mk_empty();
    the_parser.my_stats.after_boot();
    the_parser.the_stack.set_xid_boot();
    //  the_parser.the_stack.dump_xml_table();
}

void MainClass::run(int argc, char **argv) {
    get_os();
    the_parser.boot();              // create the hash table and all that
    parse_args(argc, argv);         // look at arguments
    if (!only_input_data) banner(); // print banner
    more_boot();                    // finish bootstrap
    check_for_input();              // open the input file
    dclass = input_content.find_documentclass(b_after);
    input_content.find_doctype(b_after, opt_doctype);
    read_config_and_other();
    finish_init();
    the_log << "OK with the configuration file, dealing with the TeX file...\n";
    show_input_size();
    try {
        boot_bibtex(handling_ra);
        trans0();
        if (handling_ra) {
            if (the_names[np_language].null()) the_names[np_language] = Istring("language");
            the_parser.add_language_att();
        }
        the_parser.init(input_content);
        the_parser.translate_all();
    } catch (...) {}
    check_section_use();
    the_parser.after_main_text();
    if (seen_enddocument) the_parser.the_stack.add_nl();
    the_parser.final_checks();
    //    the_parser.the_stack.dump_xml_table();
    if (!no_xml) {
        the_parser.my_stats.token_stats();
        the_parser.finish_images();
        out_xml();
        log_and_tty.finish(main_ns::nb_errs);
    } else
        log_and_tty << "Nothing written to " << out_name << ".xml.\n";
    std::cout.flush();
    tralics_ns::close_file(log_and_tty.L.fp);
}

void MainClass::out_xml() {
    Buffer      X;
    std::string u = tralics_ns::get_out_dir(out_name);
    X << bf_reset << u;
    X.put_at_end(".xml");
    std::string   name = X.to_string();
    std::fstream *fp   = tralics_ns::open_file(name, true);
    X.reset();
    int aux = 4;
    if (output_encoding == en_utf8 || output_encoding == en_ascii8)
        X << "<?xml version='1.0' encoding='UTF-8'?>\n";
    else
        X << "<?xml version='1.0' encoding='iso-8859-1'?>\n";
    Istring sl = the_names[np_stylesheet];
    if (!sl.empty()) {
        aux++;
        X << "<?xml-stylesheet href=\"" << sl.p_str() << "\" type=\"" << the_names[np_stylesheet_type].p_str() << "\"?>\n";
    }
    X << "<!DOCTYPE " << dtd << " SYSTEM '" << dtdfile << "'>\n";
    X << "<!-- Translated from latex by tralics " << version << ", date: " << short_date << "-->\n";
    auto a = X.size();
#if defined(WINNT) || defined(__CYGWIN__) || defined(_WIN32)
    a += aux;
#endif
    fp_len = a + 1; // for the \n that follows
    *fp << X;
    *fp << the_parser.the_stack.document_element();
    *fp << "\n";
    log_and_tty << "Output written on " << name << " (" << fp_len << " bytes).\n";
    tralics_ns::close_file(fp);
    if (the_main->find_words) {
        u = tralics_ns::get_out_dir(out_name);
        X << bf_reset << u;
        name = X.to_string();
        the_parser.the_stack.document_element()->word_stats(name);
    }
}

void MainClass::finish_init() const {
    if (handling_ra) {
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
        auto n = config_data.data[1]->size();
        if (n == 0) bad_conf("sections_vals");
        if (n < 2) bad_conf("Config file did not provide sections");
    }
    auto n = config_data.data.size();
    for (size_t i = 2; i < n; i++) config_data.data[i]->check_other();
}

auto MainClass::check_theme(const std::string &s) -> std::string {
    std::string res = Txbuf.add_with_space(s);
    if (all_themes.find(Txbuf.to_string()) == std::string::npos) {
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

void MainClass::check_section_use() const {
    if (handling_ra) {
        std::vector<ParamDataSlot> &X = config_data.data[1]->data;
        auto                        n = X.size(); // number of sections
        for (size_t i = 0; i < n; i++)
            if (X[i].no_topic()) the_parser.parse_error(Token(), "No module in section ", X[i].key, "no module");
    }
}

void MainClass::set_input_encoding(size_t wc) {
    if (wc < max_encoding) {
        input_encoding = wc;
        the_log << lg_start_io << "Default input encoding changed to " << wc << lg_end;
    }
}
