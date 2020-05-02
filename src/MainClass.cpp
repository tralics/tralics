#include "tralics/Parser.h"
#include "tralics/globals.h"
#include <fmt/ostream.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace {
    constexpr auto usage = R"(
Syntax:
   tralics [options] source
source is the name of the source file,
   (with or without a extension .tex), does not start with a hyphen

All options start with a single or double hyphen, they are:
  -verbose: Prints much more things in the log file
  -silent: Prints less information on the terminal
  -input_file FILE: translates file FILE
  -log_file LOG: uses LOG as transcript file
  -input_path PATH: uses PATH as dir list for input
  -output_dir DIR: pute result files in directory DIR
  -type FOO: Uses FOO instead of the \documentclass value
  -config FILE: use FILE instead of default configuration file
  -confdir : indicates where the configuration files are located
  -noconfig: no configuration file is used
  -interactivemath: reads from the terminal, 
      and prints math formulas on the terminal
  -utf8: says that the source is encoded in utf8 instead of latin1
  -latin1: overrides -utf8
  -utf8output: same as -oe8
  -oe8, -oe1, -oe8a -oe1a: specifies output encoding
  -te8, -te1, -te8a -te1a: terminal and transcript encoding
  -(no)trivialmath: special math hacking
  -(no)etex; enable or disable e-TeX extensions
  -nozerowidthelt: Use  &#x200B; rather than <zws/>
  -nozerowidthspace: no special &#x200B; or <zws/> inserted
  -noentnames: result contains &#A0; rather than &nbsp;
  -entnames=true/false: says whether or not you want &nbsp;
  -nomathml: this disables mathml mode
  -dualmath: gives mathML and nomathML mode
  -(no)math_variant: for <mi mathvariant='script'>X</mi>
  -(no)multi_math_label: allows multiple labels in a formula 
  -noundefmac: alternate XML output for undefined commands
  -noxmlerror: no XML element is generated in case of error
  -no_float_hack: Removes hacks for figures and tables
  -nostraightquotes: same as right_quote=B4
  -left_quote=2018: sets translation of ` to char U+2018
  -right_quote=2019: sets translation of ' to char U+2019
  -param foo bar: adds foo="bar" to the configuratin file
  -doctype=A-B; specifies the XML DOCTYPE
  -usequotes: double quote gives two single quotes
  -shell-escape: enable \write18{SHELL COMMAND}
  -tpa_status = title/all: translate all document or title only
  -default_class=xx: use xx.clt if current class is unknown
  -raw_bib: uses all bibtex fields
  -distinguish_refer_in_rabib= true/false: special raweb hack 
  (the list of all options is avalaible at
    http://www-sop.inria.fr/marelle/tralics/options.html )

Tralics homepage: http://www-sop.inria.fr/marelle/tralics
This software is governed by the CeCILL license that can be
found at http://www.cecill.info.)";

    Buffer b_after;

    std::filesystem::path out_dir;

    std::string log_name;
    std::string machine;
    std::string no_ext;
    std::string opt_doctype;
    std::string user_config_file;

    std::vector<std::string> other_options;

    void after_conf_assign(std::vector<std::string> &V) {
        for (size_t i = 0; i < V.size(); ++i) {
            ssa2 << bf_reset << V[i];
            i++;
            Buffer local_buf;
            local_buf << V[i];
            bool res = assign(ssa2, local_buf);
            if (res) spdlog::trace("{}={}", ssa2, local_buf);
        }
    }

    void bad_conf(String s) {
        spdlog::critical("The configuration file for the RA is ra{}.tcf or ra.tcf\n", the_parser.get_ra_year());
        spdlog::critical("It must define a value for the parameter {}", s);
        spdlog::critical("No xml file generated");
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
            spdlog::critical("Illegal file name of the form safir/2002.tex");
            the_main->bad_year(); // never returns
        }
        for (size_t i = 0; i < n; i++)
            if (B[i] < 32 || B[i] > 127 || is_upper_case(B[i])) {
                spdlog::critical("Fatal: only lowercase letters allowed, {}", B);
                exit(1);
            }
    }

    void check_year(int y, Buffer &C, const std::string &dclass, const std::string &Y) {
        if (y < 2000 || y >= 2100) the_main->bad_year();
        std::string raclass = std::string("ra") + C.to_string();
        if (dclass != raclass) {
            spdlog::critical("Illegal document class {} should be {}", dclass, raclass);
            exit(1);
        }
        if (Y.empty()) return;
        if (Y == C.to_string()) return;
        spdlog::critical("Fatal: Option -year={} incompatible with year in source file", Y);
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

    /// Sometimes, we want `bar` if `\jobname` is `foo/bar`
    auto hack_for_input(const std::filesystem::path &s) -> std::string {
        std::filesystem::path path = s.parent_path();
        the_parser.set_job_name(no_ext);
        file_name = s.stem();
        if (out_dir.empty()) out_dir = path;
        if (log_name.empty()) log_name = file_name;
        if (input_path.size() == 1) {
            input_path[0] = path;
            if (!path.empty()) input_path.emplace_back("");
        }
        return s.filename();
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
        std::cout << usage;
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

    void obsolete(const std::string &s) { spdlog::warn("Obsolete option `-{}' ignored\n", s); }

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

    auto print_enc(output_encoding_type enc) -> std::string {
        switch (enc) {
        case en_utf8: return "UTF-8";
        case en_latin: return "ISO-8859-1";
        case en_ascii7: return "ISO-8859-1 on 7 bits";
        case en_ascii8: return "UTF-8 on 7 bits"; // whatever that means
        default: return "Unknown";
        }
    }

    auto split_one_arg(String a, int &p) -> std::string {
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
            the_main->banner();
            std::cout << "bad option " << a << "\n";
            usage_and_quit(1);
        }
        return B.to_string();
    }
} // namespace

auto tralics_ns::get_out_dir(const std::string &name) -> std::string {
    Buffer &B = main_ns::path_buffer;
    B << bf_reset << out_dir << bf_optslash << name;
    return B.to_string();
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
        spdlog::critical("Fatal error: Cannot open input file {}", infile);
        exit(1);
    }
    s = main_ns::path_buffer.to_string();
    main_ns::path_buffer.wptr -= 3;
    main_ns::path_buffer.push_back("ult");
    ult_name = main_ns::path_buffer.to_string();
    if (!std::filesystem::exists(s)) {
        spdlog::critical("Fatal: Nonexistent input file {}", s);
        exit(1);
    }
    open_log();
    tralics_ns::read_a_file(input_content, s, 4);
    if (input_content.empty()) {
        spdlog::critical("Fatal: Empty input file {}", s);
        exit(1);
    }

    auto wc = input_content.encoding;
    if (wc < 0) {
        wc = the_main->input_encoding;
        input_content.set_encoding(wc);
    }
    Logger::finish_seq();
    const std::string &wa = (wc == 0 ? "UTF-8" : wc == 1 ? "ISO-8859-1" : "custom");
    spdlog::trace("++ Input encoding: {} ({}) for the main file", wc, wa);
}

void MainClass::banner() const {
    static bool banner_printed = false;
    if (banner_printed) return;
    banner_printed = true;
    spdlog::info("This is tralics {}, a LaTeX to XML translator", version);
    spdlog::info("Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm");
    spdlog::info("Licensed under the CeCILL Free Software Licensing Agreement");
}

void MainClass::open_log() { // \todo spdlog etc
    auto f   = (std::filesystem::path(out_dir) / log_name).replace_extension("log");
    log_file = tralics_ns::open_file(f, true);
    if (output_encoding == en_boot) output_encoding = en_utf8;
    if (log_encoding == en_boot) log_encoding = output_encoding;

    spdlog::info("Transcript written to {}", f);
    spdlog::set_level(spdlog::level::trace);
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(f.replace_extension("spdlog"), true);
    spdlog::default_logger()->sinks().push_back(sink);
    spdlog::default_logger()->sinks()[0]->set_level(spdlog::level::info); // \todo Link this with verbose (later in startup)

    spdlog::trace("Transcript file of tralics {} for file {}", version, infile);
    spdlog::trace("Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\\'e Grimm");
    spdlog::trace("Tralics is licensed under the CeCILL Free Software Licensing Agreement");
    spdlog::trace("OS: {} running on {}", print_os(cur_os), machine);
    spdlog::trace("Output encoding: {}", print_enc(output_encoding));
    spdlog::trace("Transcript encoding: {}", print_enc(log_encoding));
    spdlog::trace("Left quote is '{}', right quote is '{}'", codepoint(leftquote_val), codepoint(rightquote_val));
    if (trivial_math != 0) spdlog::trace("\\notrivialmath={}", trivial_math);
    if (!default_class.empty()) spdlog::trace("Default class is {}", default_class);
    if (input_path.size() > 1) spdlog::trace("Input path: ({})", fmt::join(input_path, ","));

    if (only_input_data)
        spdlog::info("Starting translation of command line argument");
    else
        spdlog::info("Starting translation of file {}", infile);

    check_for_encoding(); // \todo this does not feel like it belongs here
}

void MainClass::set_ent_names(const std::string &s) { no_entnames = (s == "false") || (s == "no"); }

void MainClass::add_to_from_config(int n, Buffer &b) { from_config.add(n, b, true); }

void MainClass::parse_args(int argc, char **argv) {
    find_conf_path();
    if (argc == 1) end_with_help(0);
    if ((argc == 2) && (std::string(argv[1]) == "-?")) usage_and_quit(0);
    for (int i = 1; i < argc; i++) {
        auto *s = argv[i];
        if (s[0] == '-')
            parse_option(i, argc, argv);
        else
            see_name(s);
    }
    if (infile.empty()) {
        banner();
        spdlog::critical("Fatal: no source file given");
        end_with_help(1);
    }
    if (leftquote_val == 0 || leftquote_val >= (1 << 16)) leftquote_val = '`';
    if (rightquote_val == 0 || rightquote_val >= (1 << 16)) rightquote_val = '\'';
}

auto MainClass::check_for_arg(int &p, int argc, char **argv) const -> String {
    if (p >= argc - 1) {
        banner();
        spdlog::critical("Argument missing for option {}", argv[p]);
        usage_and_quit(1);
    }
    if (std::string(argv[p + 1]) == "=") { // \todo this allows weird syntax
        if (p >= argc - 2) {
            banner();
            spdlog::critical("Argument missing for option {}", argv[p]);
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

enum param_args {
    pa_none,
    pa_entnames,
    pa_tpastatus,
    pa_dir,
    pa_year,
    pa_type,
    pa_config,
    pa_distinguishreferinrabib,
    pa_confdir,
    pa_externalprog,
    pa_trivialmath,
    pa_leftquote,
    pa_rightquote,
    pa_defaultclass,
    pa_inputfile,
    pa_outputfile,
    pa_inputpath,
    pa_outputdir,
    pa_logfile,
    pa_doctype,
    pa_param,
    pa_inputdata
};

void MainClass::parse_option(int &p, int argc, char **argv) {
    int  eqpos = 0;
    auto s     = split_one_arg(argv[p], eqpos);

    if (s == "configfile") s = "config";
    if (s == "o") s = "outputfile";
    auto special = [](const std::string &s) -> param_args {
        if (s == "confdir") return pa_confdir;
        if (s == "config") return pa_config;
        if (s == "defaultclass") return pa_defaultclass;
        if (s == "dir") return pa_dir;
        if (s == "distinguishreferinrabib") return pa_distinguishreferinrabib;
        if (s == "doctype") return pa_doctype;
        if (s == "entnames") return pa_entnames;
        if (s == "externalprog") return pa_externalprog;
        if (s == "inputdata") return pa_inputdata;
        if (s == "inputdir") return pa_inputpath;
        if (s == "inputfile") return pa_inputfile;
        if (s == "inputpath") return pa_inputpath;
        if (s == "leftquote") return pa_leftquote;
        if (s == "logfile") return pa_logfile;
        if (s == "outputdir") return pa_outputdir;
        if (s == "outputfile") return pa_outputfile;
        if (s == "param") return pa_param;
        if (s == "rightquote") return pa_rightquote;
        if (s == "tpastatus") return pa_tpastatus;
        if (s == "trivialmath") return pa_trivialmath;
        if (s == "type") return pa_type;
        if (s == "year") return pa_year;
        return pa_none;
    }(s);

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
        case pa_distinguishreferinrabib:
            after_conf.emplace_back("distinguish_refer_in_rabib");
            after_conf.emplace_back(a);
            return;
        case pa_confdir:
            if (a[0] == '0') return;                // ignore empty component
            if (a[0] == '/' && a[1] == '0') return; // ignore root
            conf_path.emplace_back(a);
            return;
        case pa_externalprog: obsolete(s); return;
        case pa_trivialmath: trivial_math = atoi(a); return;
        case pa_leftquote: leftquote_val = std::stoul(a, nullptr, 16); return;
        case pa_rightquote: rightquote_val = std::stoul(a, nullptr, 16); return;
        case pa_defaultclass: default_class = a; return;
        case pa_inputfile: see_name(a); return;
        case pa_inputdata:
            interactive_math = true;
            only_input_data  = true;
            see_name("texput");
            everyjob_string = "\\usepackage{amsmath}" + std::string(a) + "\\stop";
            return;
        case pa_outputfile: out_name = a; return;
        case pa_inputpath: new_in_dir(a); return;
        case pa_outputdir: out_dir = a; return;
        case pa_logfile: log_name = a; return;
        case pa_doctype: opt_doctype = a; return;
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
        spdlog::error("Illegal equal sign in option {}", argv[p]);
        usage_and_quit(1);
    }
    if ((s == "v") || (s == "verbose")) {
        verbose = true;
        return;
    }
    if ((s == "V") || (s == "verbose-doc")) {
        dverbose = true;
        return;
    }
    if (s == "silent") {
        silent = true;
        return;
    }
    if (s == "version") {
        banner();
        exit(0);
    }
    if (s == "rawbib") {
        raw_bib = true;
        return;
    }
    if (s == "utf8") {
        input_encoding = 0;
        return;
    }
    if (s == "utf8output") {
        log_encoding = output_encoding = en_utf8;
        return;
    }
    if (s == "oe8") {
        output_encoding = en_utf8;
        return;
    }
    if (s == "oe8a") {
        output_encoding = en_ascii8;
        return;
    }
    if (s == "oe1") {
        output_encoding = en_latin;
        return;
    }
    if (s == "oe1a") {
        output_encoding = en_ascii7;
        return;
    }
    if (s == "te8") {
        log_encoding = en_utf8;
        return;
    }
    if (s == "te8a") {
        log_encoding = en_ascii8;
        return;
    }
    if (s == "te1") {
        log_encoding = en_latin;
        return;
    }
    if (s == "te1a") {
        log_encoding = en_ascii7;
        return;
    }
    if (s == "latin1") {
        input_encoding = 1;
        return;
    }
    if (s == "noentnames") {
        no_entnames = true;
        return;
    }
    if (s == "nomultimathlabel") {
        multi_math_label = false;
        return;
    }
    if (s == "multimathlabel") {
        multi_math_label = true;
        return;
    }
    if (s == "nofloathack") {
        nofloat_hack = true;
        return;
    }
    if (s == "noprimehack") {
        prime_hack = false;
        return;
    }
    if (s == "primehack") {
        prime_hack = true;
        return;
    }
    if (s == "doublequoteatt") {
        double_quote_att = true;
        return;
    }
    if (s == "notrivialmath") {
        trivial_math = 0;
        return;
    }
    if (s == "nozerowidthspace") {
        no_zerowidthspace = true;
        return;
    }
    if (s == "nozerowidthelt") {
        no_zerowidthelt = true;
        return;
    }
    if (s == "shellescape") {
        shell_escape_allowed = true;
        return;
    }
    if (s == "xml") {
        no_xml = false;
        return;
    }
    if (s == "allowbreak") {
        main_ns::bib_allow_break = true;
        return;
    }
    if (s == "noallowbreak") {
        main_ns::bib_allow_break = false;
        return;
    }
    if (s == "etex") {
        etex_enabled = true;
        return;
    }
    if (s == "noetex") {
        etex_enabled = false;
        return;
    }
    if (s == "noxmlerror") {
        main_ns::no_xml_error = true;
        return;
    }
    if (s == "l3") {
        load_l3 = true;
        return;
    }
    if ((s == "interactivemath") || (s == "i") || (s == "adventure")) {
        interactive_math = true;
        input_encoding   = 0;
        see_name("texput");
    }
    if (s == "noundefmac") {
        no_undef_mac = true;
        return;
    }
    if (s == "noconfig") {
        noconfig = true;
        return;
    }
    if (s == "compatibility") {
        compatibility = true;
        return;
    }
    if (s == "oldphi") {
        old_phi = true;
        return;
    }
    if (s == "badminus") {
        bad_minus = true;
        return;
    }
    if (s == "nostraightquotes") {
        rightquote_val = 0xB4;
        return;
    }
    if (s == "usequotes") {
        main_ns::use_quotes = true;
        return;
    }
    if (s == "mathvariant") {
        math_variant = true;
        return;
    }
    if (s == "nomathvariant") {
        math_variant = false;
        return;
    }
    if (s == "nomathml") {
        nomathml = true;
        return;
    }
    if (s == "dualmath") {
        dualmath = true;
        return;
    }
    if (s == "findwords") {
        find_words = true;
        return;
    }
    if ((s == "radebug") || (s == "check") || (s == "ps") || (s == "xmlfo") || (s == "xmlhtml") || (s == "xmltex") || (s == "xmllint") ||
        (s == "xmlall") || (s == "all") || (s == "nobibyearerror") || (s == "nobibyearmodify")) {
        obsolete(s);
        return;
    }
    if (s == "help") usage_and_quit(0);
    banner();
    spdlog::error("bad option {}", argv[p]);
    usage_and_quit(1);
}

void MainClass::set_tpa_status(const std::string &s) { // \todo Erk this is not good
    if (s.empty()) return;
    if (s[0] == 'a' || s[0] == 'A')
        tpa_mode = 1; // case 'all'
    else if (s[0] == 't' || s[0] == 'T')
        tpa_mode = 2; // case title
    else if (s[0] == 'c' || s[0] == 'C')
        tpa_mode = 3; // case config
    else
        tpa_mode = 0; // default
}

void MainClass::end_with_help(int v) const {
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
        if (!B.ends_with(".tcf")) return static_cast<bool>(main_ns::search_in_confdir(user_config_file + ".tcf"));
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

void MainClass::open_config_file() { // \todo filesystem
    Buffer &B = main_ns::path_buffer;
    if (B.empty()) {
        config_file.insert("#comment", true);
        spdlog::warn("Dummy default configuration file used.");
        return;
    }
    tralics_ns::read_a_file(config_file, B.to_string(), 0);
    config_file.normalise_final_cr();
    spdlog::trace("Read configuration file {}", B);
    if (!B.ends_with(".tcf")) return;
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
    if (dtype.starts_with("\\documentclass")) dtype = "";
}

void MainClass::get_doc_type() {
    get_type_from_config();
    if (dclass.empty())
        spdlog::trace("No \\documentclass in source file");
    else
        spdlog::trace("Seen \\documentclass {}", dclass);
    if (!type_option.empty())
        dtype = type_option;
    else if (dtype.empty()) {
        dtype = dclass;
        if ((dtype == "book") || (dtype == "article") || (dtype == "report") || (dtype == "minimal"))
            dft = 0;
        else if (dtype.empty())
            return; // dft is 3
    }
    if (dtype.empty()) dtype = "unknown";
    spdlog::trace("Potential type is {}", dtype);
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
        spdlog::info("Read tcf file {}", tcf_file);
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
        the_log << fmt::format("dtd is {} from {} (mode RAWEB{})\n", dtd, dtdfile, year);
    else
        the_log << "dtd is " << dtd << " from " << dtdfile << " (standard mode)\n";
}

void MainClass::read_config_and_other() {
    year             = the_parser.get_ra_year();
    bool have_dclass = !dclass.empty();
    if (find_config_file())
        open_config_file();
    else
        spdlog::trace("No configuration file.");
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
    spdlog::critical("Fatal: Input file name must be team name followed by {}", year);
    end_with_help(1);
}

void MainClass::see_name(String s) {
    Buffer &B = b_after;
    if (!infile.empty()) {
        std::cout << "Fatal error: Seen two source files " << infile << " and " << s << "\n";
        exit(1);
    }
    B << bf_reset << s;
    if (B.ends_with(".xml")) B.remove_last(4);
    B.append_unless_ends_with(".tex");
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
        Logger::log_finish();
        exit(0);
    }
}

void MainClass::boot_bibtex(bool inra) {
    std::string mybbl = out_name + "_.bbl";
    auto        fn    = tralics_ns::get_out_dir(mybbl);
    tralics_ns::bibtex_boot(fn, year_string.c_str(), out_name, inra, distinguish_refer);
}

void MainClass::show_input_size() {
    int n = input_content.get_last_line_no();
    if (n == 1)
        the_log << "There is a single line\n";
    else
        the_log << fmt::format("There are {} lines\n", n);
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
    } catch (...) {} // \todo probably this is wrong
    check_section_use();
    the_parser.after_main_text();
    if (seen_enddocument) the_parser.the_stack.add_nl();
    the_parser.final_checks();
    //    the_parser.the_stack.dump_xml_table();
    if (!no_xml) {
        the_parser.my_stats.token_stats();
        the_parser.finish_images();
        out_xml();
        Logger::log_finish();
    } else
        spdlog::warn("Nothing written to {}.xml.", out_name);
}

void MainClass::out_xml() {
    Buffer X;
    auto   u = tralics_ns::get_out_dir(out_name);
    X << bf_reset << u;
    X.append_unless_ends_with(".xml"); // \todo std::filesystem
    std::string name = X.to_string();
    auto        fp   = tralics_ns::open_file(name, true);
    X.reset();
    int aux = 4;
    if (output_encoding == en_utf8 || output_encoding == en_ascii8)
        X << "<?xml version='1.0' encoding='UTF-8'?>\n";
    else
        X << "<?xml version='1.0' encoding='iso-8859-1'?>\n";
    Istring sl = the_names[np_stylesheet];
    if (!sl.empty()) {
        aux++;
        X << "<?xml-stylesheet href=\"" << sl.value << "\" type=\"" << the_names[np_stylesheet_type].value << "\"?>\n";
    }
    X << "<!DOCTYPE " << dtd << " SYSTEM '" << dtdfile << "'>\n";
    X << "<!-- Translated from latex by tralics " << version << ", date: " << short_date << "-->\n";
    auto a = X.size();
#if defined(WINNT) || defined(__CYGWIN__) || defined(_WIN32)
    a += aux;
#endif
    fp_len = a + 1; // for the \n that follows
    fp << X;
    fp << the_parser.the_stack.document_element();
    fp << "\n";
    spdlog::info("Output written on {} ({} bytes).", name, fp_len);
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
        Logger::finish_seq();
        the_log << "++ Default input encoding changed to " << wc << "\n";
    }
}
