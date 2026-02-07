// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2006-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains ltclass.dtx

#include "tralics/KeyAndVal.h"
#include "tralics/LatexPackage.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

using namespace std::string_literals;

namespace {
    Buffer txclasses_local_buf;

    class ClassesData {
    public:
        std::vector<LatexPackage *> packages;
        OptionList                  unused_options;             // is \@unusedoptionlist in latex
        OptionList                  global_options;             // is \@classoptionslist is latex
        TokenList                   documentclass_hook;         // single hook for all classes
        bool                        seen_document_class{false}; // have we seen a \documentclass command
        bool                        using_default_class{false}; // inhibits warning

        ClassesData();
        auto        cur_pack() -> LatexPackage *;
        auto        find_package(const std::string &name, bool type, bool creat) -> size_t;
        static void remove_from_unused(const std::string &name);
        static void show_unused();
    };
} // namespace

namespace {
    ClassesData the_class_data;
    // global variable so that tralics/Parser.h does not need to know OptionList
    OptionList cur_opt_list;
} // namespace

namespace classes_ns {
    auto parse_version(const std::string &s) -> int;
    auto is_in_vector(const OptionList &V, const std::string &s, bool X) -> std::optional<size_t>;
    auto is_raw_option(const OptionList &V, String s) -> bool;
    auto is_in_option(const OptionList &V, const KeyAndVal &s) -> bool;
    auto make_options(TokenList &L) -> OptionList;
    auto compare_options(const OptionList &A, const OptionList &B) -> bool;
    void dump_options(const OptionList &A, String x);
    auto cur_options(bool star, TokenList &spec, bool normal) -> TokenList;
    auto make_keyval(TokenList &L) -> KeyAndVal;
    void register_key(const std::string &Key);
    void unknown_optionX(TokenList &cur, TokenList &action);
    void unknown_option(KeyAndVal &cur_keyval, TokenList &res, TokenList &spec, int X);
    void add_to_filelist(const std::string &s, const std::string &date);
    void add_sharp(TokenList &L);
} // namespace classes_ns

using namespace classes_ns;
using namespace token_ns;

// ---------------------------------------------------------
// Functions dealing with option lists

// Splits key=val in two parts
auto classes_ns::make_keyval(TokenList &key_val) -> KeyAndVal {
    TokenList key;
    Token     equals      = Token(other_t_offset, '=');
    bool      have_equals = split_at(equals, key_val, key);
    remove_first_last_space(key_val);
    remove_first_last_space(key);
    if (have_equals) key_val.push_front(equals);
    std::string key_name = the_parser.list_to_string_c(key, "Invalid option");
    std::string key_full = key_name;
    if (have_equals) {
        Buffer &B = txclasses_local_buf;
        B         = key_name;
        B << key_val; // \todo make TokenList formattable
        key_full = B;
    }
    return {key_name, key_val, key_full};
}

// Constructs an option list from a comma separated string.
auto classes_ns::make_options(TokenList &L) -> OptionList {
    OptionList res;
    TokenList  key;
    Token      comma = hash_table.comma_token;
    while (!L.empty()) {
        token_ns::split_at(comma, L, key);
        token_ns::remove_first_last_space(key);
        if (key.empty()) continue;
        res.push_back(make_keyval(key));
    }
    return res;
}

// Prints the options list on log and tty
void classes_ns::dump_options(const OptionList &A, String x) {
    Buffer &B = txclasses_local_buf;
    B.clear();
    auto n = A.size();
    for (size_t i = 0; i < n; i++) {
        if (i > 0) B += ",";
        B += A[i].full_name;
    }
    spdlog::info("{} {}", x, B);
}

// Adds a copy of L to the option list of the package.
void LatexPackage::add_options(const OptionList &L) {
    auto n = L.size();
    for (size_t j = 0; j < n; j++) Uoptions.push_back(L[j]);
}

// Returns true if S is in the option list (for check_builtin_class)
auto classes_ns::is_raw_option(const OptionList &V, String s) -> bool {
    return std::any_of(V.begin(), V.end(), [&s](const auto &i) { return i.name == s; });
}

// Returns true if slot is in the vector V with the same value
auto classes_ns::is_in_option(const OptionList &V, const KeyAndVal &slot) -> bool {
    const std::string &s = slot.name;
    auto               n = V.size();
    for (size_t i = 0; i < n; i++)
        if (V[i].name == s) return compare(slot.val, V[i].val);
    return false;
}

// Returns true if each element of B is in A, i.e. A contains B
auto classes_ns::compare_options(const OptionList &A, const OptionList &B) -> bool {
    auto n = B.size();
    for (size_t i = 0; i < n; i++)
        if (!is_in_option(A, B[i])) return false;
    return true;
}

// Like is_in_option, but returns a position
// If X true, checks the keyname
auto classes_ns::is_in_vector(const OptionList &V, const std::string &s, bool X) -> std::optional<size_t> {
    for (size_t i = 0; i < V.size(); i++)
        if (X ? (V[i].name == s) : (V[i].full_name == s)) return i;
    return {};
}

// ------------------------------------------------------------

// Find the index of a package in the list. Returns 0 in case of
// failure (slot 0 does not hold a valid package)
// Creates if creat is true.
auto ClassesData::find_package(const std::string &name, bool type, bool creat) -> size_t {
    std::string full_name = (type ? "C"s : "P"s) + name;
    auto        n         = packages.size();
    for (size_t i = 1; i < n; i++)
        if (packages[i]->has_name(full_name)) return i;
    if (!creat) return 0;
    packages.push_back(new LatexPackage(full_name));
    return n;
}

// Implements \@ifpackageloaded \@ifclassloaded.
// True if the date field is set (missing date in a package gives 0000/00/00)
void Parser::T_if_package_loaded(bool type) // true for class
{
    std::string name = sE_arg_nopar();
    auto        i    = the_class_data.find_package(name, type, false);
    bool        res  = false;
    if ((i != 0) && !the_class_data.packages[i]->date.empty()) res = true;
    one_of_two(res);
}

// Implements \@ifpackagelater \@ifclasslater.
// \@ifpackagelater{name}{YYYY/MM/DD}
void Parser::T_if_package_later(bool c) // true for class
{
    std::string name  = sE_arg_nopar();
    std::string date  = sE_arg_nopar();
    int         idate = parse_version(date);
    int         pdate = 0;
    auto        i     = the_class_data.find_package(name, c, false);
    if (i != 0) pdate = parse_version(the_class_data.packages[i]->date);
    one_of_two(pdate >= idate);
}

// Implements \@ifpackagewith \@ifclasswith.
// \@ifpackagewith{name}{option-list}
void Parser::T_if_package_with(bool c) // true for class
{
    std::string name    = sE_arg_nopar();
    TokenList   options = read_arg();
    OptionList  A;
    OptionList  B = make_options(options);
    auto        p = the_class_data.find_package(name, c, false);
    if (p != 0) A = the_class_data.packages[p]->Uoptions;
    bool res = compare_options(A, B);
    one_of_two(res);
}

// Class data ctor
ClassesData::ClassesData() { packages.push_back(new LatexPackage("Fdummy file")); }

// Returns data for current class or package
// Hack for InputClass, where N is negative
auto ClassesData::cur_pack() -> LatexPackage * {
    auto n = the_parser.get_cur_file_pos();
    if (n < 0) n = -n;
    return packages[to_unsigned(n)];
}

// Date is something like 2004/12/03 converted to 20041203
// Hack: we read at most 8 digits, ignore everything else
auto classes_ns::parse_version(const std::string &s) -> int {
    auto n = s.size();
    int  r = 0;
    int  k = 0;
    for (size_t i = 0; i < n; i++) {
        char c = s[i];
        if (std::isdigit(c) != 0) {
            r = 10 * r + (c - '0');
            k++;
            if (k == 8) break;
        }
    }
    return r;
}

// ------------------------------------------------------------

// Insert the \AtEndOfClass hook, and other stuff
// This is called by pop_input_stack when the end of a file is seen.
// The integer n is the value of cur_file_type
void Parser::insert_hook(long n) {
    auto k = the_class_data.packages.size();
    if (n <= 0 || n >= to_signed(k)) return;
    LatexPackage *C = the_class_data.packages[to_unsigned(n)];
    if (!C->seen_process && !C->Uoptions.empty())
        log_and_tty << "Warning: " << C->pack_or_class() << C->real_name() << " has no \\ProcessOptions\n";
    back_input(C->hook);
    if (parse_version(C->date) < parse_version(C->req_date)) {
        log_and_tty << "Warning: You have requested, on line " << get_cur_line() << ", version\n`" << C->req_date << "' of "
                    << C->pack_or_class() << C->real_name() << ",\n"
                    << "but only version\n`" << C->date << "' is available\n";
    }
}

// Store the file name (without dir) and date in the global_state.file_list buffer
void classes_ns::add_to_filelist(const std::string &s, const std::string &date) {
    auto n = s.size();
    long k = -1;
    for (size_t i = 0; i < n; i++)
        if (s[i] == '/') k = to_signed(i); // last slash \todo std::filesystem
    auto S = s.substr(to_unsigned(k + 1));
    global_state.file_list += fmt::format("{:>12}   {}\n", S, date);
}

// This implements \ProvidesPackage, \ProvidesClass (synonym)
// Also \ProvidesFile
void Parser::T_provides_package(bool c) // True for a file
{
    std::string name = sE_arg_nopar();
    std::string date = sE_optarg_nopar();
    add_to_filelist(get_cur_filename(), date);
    Logger::finish_seq();
    if (c) {
        the_log << "File: " << name << " " << date << "\n";
        return;
    }
    LatexPackage *cur = the_class_data.cur_pack();
    if (!date.empty()) cur->date = date;
    auto S = cur->real_name();
    if (name != S && !the_class_data.using_default_class) {
        log_and_tty << "Warning: " << cur->pack_or_class() << S << " claims to be " << name << ".\n";
    }
    Buffer &b = txclasses_local_buf;
    b         = fmt::format(cur->is_class() ? "Document class: {} {}\n" : "Package: {} {}\n", name, date);
    if (cur->is_class())
        log_and_tty << b;
    else
        the_log << b;
}

// This implements \PassOptionsToPackage, \PassOptionsToClass
// Latex says:
// % If the package has been loaded, we check that it was first loaded with
// % the options.  Otherwise we add the option list to that of the package.
// But the test seems to be missing

void Parser::T_pass_options(bool c) // true if a class
{
    TokenList   Lopt = read_arg();
    std::string name = sE_arg_nopar();
    OptionList  L    = make_options(Lopt);
    auto        p    = the_class_data.find_package(name, c, true);
    the_class_data.packages[p]->add_options(L);
}

// This implements \DeclareOption, \DeclareOption*
void Parser::T_declare_options() {
    bool star = remove_initial_star();
    if (star)
        T_declare_option_star();
    else {
        std::string   name = sE_arg_nopar();
        TokenList     L    = read_arg();
        LatexPackage *C    = the_class_data.cur_pack();
        C->Poptions.push_back(KeyAndVal(name, L, name));
    }
}

// The xkeyval package uses also this function

void classes_ns::register_key(const std::string &Key) {
    TokenList     L; // empty
    LatexPackage *C = the_class_data.cur_pack();
    C->Poptions.push_back(KeyAndVal(Key, L, Key));
}

// \DeclareOption*{foo}
// Same code for xkeyval and kvoption packages
void Parser::T_declare_option_star() {
    TokenList     L    = read_arg();
    LatexPackage *C    = the_class_data.cur_pack();
    C->has_a_default   = true;
    C->default_handler = L;
}

// This implements \OptionNotUsed
// This is the default option handler. In the case of a package, ignore,
// in the case of a class, remove the used flag
void Parser::T_option_not_used() {
    if (!the_class_data.cur_pack()->is_class()) return;
    OptionList &GO = the_class_data.global_options;
    expand_no_arg("CurrentOption");
    TokenList L = read_arg();
    KeyAndVal s = make_keyval(L);
    auto      j = is_in_vector(GO, s.full_name, true);
    if (j) GO[*j].used = false;
}

// Case \ProcessOptions* or  \ProcessOptionsX*
// in a package; we execute global options in order
void LatexPackage::check_global_options(TokenList &action, bool X) {
    OptionList &GO = the_class_data.global_options; // options of the class
    OptionList &DO = Poptions;                      // Known options
    for (auto &i : GO) {
        std::string nname = X ? i.name : i.full_name;
        auto        j     = find_option(nname);
        if (j <= 0) continue;
        if (DO[to_unsigned(j)].used) continue; // should not happen
        i.used = true;
        if (!txclasses_local_buf.empty()) txclasses_local_buf.push_back(',');
        txclasses_local_buf += nname;
        DO[to_unsigned(j)].use_and_kill(action, i, X);
    }
}

// Case \ProcessOptions or \ProcessOptionsX
// (no star) in class a package; we execute global options in order
void LatexPackage::check_local_options(TokenList &res, bool X) {
    OptionList &DO = Poptions;                      // Known options
    OptionList &GO = the_class_data.global_options; // options of the class
    OptionList &CO = Uoptions;                      // arg of \usepackage
    for (auto &i : DO) {
        const std::string &nname = i.name;
        if (i.used) continue; // should to happen
        auto j = is_in_vector(CO, nname, false);
        if (j) {
            ClassesData::remove_from_unused(nname);
            i.use_and_kill(res, CO[*j], X);
        } else if (is_class())
            continue;
        else {
            j = is_in_vector(GO, nname, X);
            if (j) {
                i.use_and_kill(res, GO[*j], X);
                GO[*j].used = true;
            } else
                continue;
        }
        if (!txclasses_local_buf.empty()) txclasses_local_buf.push_back(',');
        txclasses_local_buf += nname;
    }
}

// This is for \ProcessOptionX when an option is unknown,
//  but there is as default value.
// This is done at the end of ProcessOptionsX
void classes_ns::unknown_optionX(TokenList &cur, TokenList &action) {
    LatexPackage *C = the_class_data.cur_pack();
    if (!C->has_a_default) return; // should not happen
    KeyAndVal W = make_keyval(cur);
    TokenList unused;
    unknown_option(W, unused, action, 1); // insert default code
}

// General case.
void classes_ns::unknown_option(KeyAndVal &cur, TokenList &res, TokenList &spec, int X) {
    LatexPackage *     C    = the_class_data.cur_pack();
    const std::string &name = cur.full_name;
    if (!C->has_a_default) {
        if (C->is_class()) {
        } else
            log_and_tty << "Unknown option `" << name << "' for package `" << C->real_name() << "'\n";
    } else {
        TokenList u = cur.to_list();
        if (X == 1) {
            TokenList w;
            w = string_to_list(cur.name, true);
            spec.push_back(hash_table.def_token);
            spec.push_back(hash_table.CurrentOptionKey_token);
            spec.splice(spec.end(), w);

            w = cur.val;
            if (w.empty())
                w.push_back(hash_table.relax_token);
            else
                w.pop_front();
            w.brace_me();
            spec.push_back(hash_table.def_token);
            spec.push_back(hash_table.CurrentOptionValue_token);
            spec.splice(spec.end(), w);
            u.brace_me();
            spec.push_back(hash_table.def_token);
            spec.push_back(hash_table.CurrentOption_token);
            spec.splice(spec.end(), u);
            u = C->default_handler;
            spec.splice(spec.end(), u);
        } else if (X == 2) {
            if (!res.empty()) res.push_back(Token(other_t_offset, ','));
            res.splice(res.end(), u);
        } else {
            u.brace_me();
            res.push_back(hash_table.def_token);
            res.push_back(hash_table.CurrentOption_token);
            res.splice(res.end(), u);
            u = C->default_handler;
            res.splice(res.end(), u);
        }
    }
}

//
void LatexPackage::check_all_options(TokenList &action, TokenList &spec, int X) {
    OptionList &CO = Uoptions; // arg of \usepackage
    OptionList &DO = Poptions; // Known options
    for (auto &i : CO) {
        std::string nname = i.name;
        auto        j     = find_option(X != 0 ? nname : i.full_name);
        if (j == -1) {
            unknown_option(i, action, spec, X);
        } else {
            ClassesData::remove_from_unused(nname);
            if (DO[to_unsigned(j)].used) continue;
            if (!txclasses_local_buf.empty()) txclasses_local_buf.push_back(',');
            txclasses_local_buf += DO[to_unsigned(j)].name;
            DO[to_unsigned(j)].use_and_kill(action, i, X != 0);
        }
    }
    for (auto &i : DO) {
        i.val.clear();
        i.used = true;
    }
}

// This implements \ProcessOptions, \ProcessOptions*
void Parser::T_process_options() {
    bool          star     = remove_initial_star();
    LatexPackage *C        = the_class_data.cur_pack();
    bool          in_class = C->is_class();
    C->seen_process        = true;
    Buffer &b              = txclasses_local_buf;
    b.clear();
    TokenList action; // token list to evaluate
    if (star) {
        if (!in_class) C->check_global_options(action, false);
    } else
        C->check_local_options(action, false);
    TokenList unused;
    C->check_all_options(action, unused, 0);
    T_process_options_aux(action);
}

void Parser::T_execute_options() {
    TokenList     opt  = read_arg();
    LatexPackage *C    = the_class_data.cur_pack();
    OptionList &  pack = C->Poptions;
    TokenList     action;
    Buffer &      b = txclasses_local_buf;
    b.clear();
    OptionList L = make_options(opt);
    auto       n = L.size();
    for (size_t i = 0; i < n; i++) {
        const std::string &option = L[i].full_name;
        auto               k      = C->find_option(option);
        if (k >= 0) {
            if (!b.empty()) b.push_back(',');
            b += pack[to_unsigned(k)].name;
            pack[to_unsigned(k)].use(action);
        }
    }
    T_process_options_aux(action);
}

// Common code;
void Parser::T_process_options_aux(TokenList &action) {
    Logger::finish_seq();
    the_log << "{Options to execute->" << txclasses_local_buf << "}\n";
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << "{Options code to execute->" << action << "}\n";
    }
    back_input(action);
}

// This is used by \ProcessOptionsX and \ProcessKeyvalOptions
// It gets the list of all keyval pairs.
auto classes_ns::cur_options(bool star, TokenList &spec, bool normal) -> TokenList {
    LatexPackage *C        = the_class_data.cur_pack();
    bool          in_class = C->is_class();
    C->seen_process        = true;
    Buffer &b              = txclasses_local_buf;
    b.clear();
    TokenList action; // token list to evaluate
    if (star) {
        if (!in_class) C->check_global_options(action, true);
    } else
        C->check_local_options(action, true);
    C->check_all_options(action, spec, normal ? 1 : 2);
    Logger::finish_seq();
    the_log << "{Options to execute->" << txclasses_local_buf << "}\n";
    return action;
}

void Parser::T_inputclass() {
    std::string name = sE_arg_nopar();
    auto        res  = find_in_confdir(name + ".clt");
    if (!res) {
        parse_error(err_tok, "Cannot input " + name + ".clt", "");
    } else {
        auto k = cur_file_pos;
        open_tex_file(res->string(), true); // \todo fs::path
        if (k > 0) k = -k;
        set_cur_file_pos(k);
    }
}

// Implements \LoadClassWithOptions and \RequirePackageWithOptions
void Parser::T_load_with_options(bool c) // c is true for a class
{
    std::string name                        = sE_arg_nopar();
    the_class_data.cur_pack()->seen_process = true; // someone else processes
    std::string date                        = sE_optarg_nopar();
    cur_opt_list                            = the_class_data.cur_pack()->Uoptions;
    bool b                                  = check_builtin_pack(name);
    use_a_package(name, c, date, b);
}

// This implements \documentclass, and \LoadClass
// bad is true in not vertical mode of after \begin{document}
void Parser::T_documentclass(bool bad) {
    auto        c        = cur_cmd_chr.chr;
    Token       T        = cur_tok;
    auto        Loptions = read_optarg_nopar().value_or(TokenList{});
    std::string name     = sE_arg_nopar();
    std::string date     = sE_optarg_nopar();
    cur_opt_list         = make_options(Loptions);
    if (c == 0) { // else is LoadClass
        cur_tok = T;
        if (bad || the_class_data.seen_document_class) wrong_mode("Bad \\documentclass");
        the_class_data.seen_document_class = true;
    }
    check_builtin_class(); // handles builtin classes here
    if (c == 0) the_class_data.global_options = cur_opt_list;
    use_a_package(name, true, date, false);
}

// This implements \usepackage
void Parser::T_usepackage() {
    auto        Loptions = read_optarg_nopar().value_or(TokenList{});
    std::string name     = sE_arg_nopar(); // can be a list of names
    std::string date     = sE_optarg_nopar();
    cur_opt_list         = make_options(Loptions);
    for (const auto &pack : split_commas(name)) {
        bool b = check_builtin_pack(pack);
        use_a_package(pack, false, date, b);
    }
}

// This is done after the first \usepackage; may warn
void LatexPackage::reload() const {
    if (compare_options(Uoptions, cur_opt_list)) // true if A contains opt
        return;
    log_and_tty << "Option clash in \\usepackage " << real_name() << "\n";
    dump_options(Uoptions, "Old options: ");
    dump_options(cur_opt_list, "New options: ");
}

// This implements \@onefilewithoptions. Arguments are
// name of class/package,  class indicator, optional date, options.
void Parser::use_a_package(const std::string &name, bool type, const std::string &date, bool builtin) {
    auto          p   = the_class_data.find_package(name, type, true);
    LatexPackage *cur = the_class_data.packages[p];
    if (cur->checked) {
        cur->reload();
        return;
    }
    cur->checked = true;
    cur->add_options(cur_opt_list);
    cur->req_date                      = date;
    String T                           = type ? "Class" : "Package";
    auto   res                         = find_in_confdir(name + (type ? ".clt"s : ".plt"s));
    the_class_data.using_default_class = false;
    if (!res) {
        std::string D = the_main.default_class;
        if (type && !D.empty()) {
            res = find_in_confdir(D + ".clt");
            if (res) {
                log_and_tty << "Using default class " << D << "\n";
                the_class_data.using_default_class = true;
            }
        }
    }
    if (!res) {
        if (builtin) cur->date = "2006/01/01";
        Logger::finish_seq();
        the_log << T << " " << name << (builtin ? " builtin"s : " unknown"s) << "\n";
        return;
    }
    cur->date = "0000/00/00";
    open_tex_file(res->string(), true);
    set_cur_file_pos(to_signed(p));
    Buffer &b       = txclasses_local_buf;
    b               = name;
    TokenList cc    = b.str_toks11(false);
    Token     cctok = hash_table.locate("CurrentClass");
    new_macro(cc, cctok);
}

// Built-in package handler
auto Parser::check_builtin_pack(const std::string &pack) -> bool {
    if (pack == "calc") {
        calc_loaded = true;
        return false;
    }
    if (pack == "fp") {
        boot_fp();
        return true;
    }
    if (pack == "french" || pack == "frenchle") {
        set_default_language(1);
        return true;
    }
    if (pack == "german") {
        set_default_language(2);
        return true;
    }
    if (pack == "babel") {
        check_language();
        return true;
    }
    if (pack == "fancyhdr") {
        hash_table.boot_fancyhdr();
        return true;
    }
    return false;
}

// Built-in class handler. Class name unused
void Parser::check_builtin_class() {
    Xid doc_att(1);
    if (is_raw_option(cur_opt_list, "useallsizes")) the_main.use_all_sizes = true;
    if (is_raw_option(cur_opt_list, "french")) set_default_language(1);
    if (is_raw_option(cur_opt_list, "english")) set_default_language(0);
}

// Handles the language option in \usepackage[xx]{babel}
void Parser::check_language() {
    int  lang = -1;
    auto n    = cur_opt_list.size();
    for (size_t i = 0; i < n; i++) {
        const std::string &s = cur_opt_list[i].name;
        if (s == "french" || s == "francais" || s == "frenchb" || s == "acadian" || s == "canadien") {
            if (lang == -1) the_log << "babel options: ";
            lang = 1;
            the_log << "french ";
        }
        if (s == "english" || s == "american" || s == "british" || s == "canadian" || s == "UKenglish" || s == "USenglish") {
            if (lang == -1) the_log << "babel options: ";
            lang = 0;
            the_log << "english ";
        }
        if (s == "austrian" || s == "german" || s == "germanb" || s == "naustrian" || s == "ngerman") {
            if (lang == -1) the_log << "babel options: ";
            lang = 2;
            the_log << "german ";
        }
    }
    if (lang == -1) return;
    the_log << "\n";
    set_default_language(lang);
}

// This adds the language attribute to the main XML element
void Parser::add_language_att() const {
    std::string b = "cst_empty";
    int         D = get_def_language_num();
    if (D == 0)
        b = "english";
    else if (D == 1)
        b = "french";
    else if (D == 2)
        b = "german";
    Xid doc_att(1);
    if ((b != "cst_empty") && !the_names["language"].empty()) doc_att.get_att()[the_names["language"]] = the_names[b];
}

auto LatexPackage::find_option(const std::string &nname) -> long {
    auto n = Poptions.size();
    for (size_t i = 0; i < n; i++)
        if (Poptions[i].name == nname) return to_signed(i);
    return -1;
}

void ClassesData::remove_from_unused(const std::string &name) {
    OptionList &GO = the_class_data.global_options;
    auto        j  = is_in_vector(GO, name, true);
    if (j) GO[*j].used = true;
}

void show_unused_options() { ClassesData::show_unused(); }

void ClassesData::show_unused() {
    OptionList &GO = the_class_data.global_options;
    Buffer &    B  = txclasses_local_buf;
    B.clear();
    int k = 0;
    for (auto &i : GO) {
        if (i.used) continue;
        if (i.name == "useallsizes") continue;
        k++;
        if (!B.empty()) B += ',';
        B += i.full_name;
    }
    if (k == 0) return;
    log_and_tty << "Tralics Warning: Unused global option" << (k == 1 ? "" : "s") << "\n   " << B << ".\n";
}

// Implements \AtEndOfPackage \AtEndOfClass
void Parser::T_at_end_of_class() {
    TokenList L = read_arg();
    the_class_data.cur_pack()->add_to_hook(L);
}

// Implements \ClassError etc
void Parser::T_class_error(subtypes c) {
    if (c == messagebreak_code) return;
    static Token message_break_token = hash_table.locate("MessageBreak");
    std::string  prefix, prea;
    msg_type     what    = mt_none;
    bool         on_line = true;
    bool         std     = true;
    bool         simple  = false;
    int          skip    = 0;
    int          n       = 0;
    switch (c) {
    case packageerror_code:
        n    = 14;
        what = mt_error;
        prea = "Package";
        break;
    case packagewarning_code:
        n    = 16;
        what = mt_warning;
        prea = "Package";
        break;
    case packagewarningnoline_code:
        n       = 16;
        what    = mt_warning;
        prea    = "Package";
        on_line = false;
        break;
    case packageinfo_code:
        n    = 13;
        what = mt_info;
        prea = "Package";
        break;
    case classerror_code:
        n    = 12;
        what = mt_error;
        prea = "Class";
        break;
    case classwarning_code:
        n    = 14;
        what = mt_warning;
        prea = "Class";
        break;
    case classwarningnoline_code:
        n       = 14;
        what    = mt_warning;
        prea    = "Class";
        on_line = false;
        break;
    case classinfo_code:
        n    = 11;
        what = mt_info;
        prea = "Class";
        break;
    case latexerror_code:
        n      = 15;
        what   = mt_error;
        prea   = "Tralics";
        simple = true;
        break;
    case latexwarning_code:
        n      = 15;
        what   = mt_warning;
        prea   = "Tralics";
        simple = true;
        break;
    case latexwarningnoline_code:
        n       = 15;
        what    = mt_warning;
        prea    = "Tralics";
        simple  = true;
        on_line = false;
        break;
    case latexinfo_code:
        n    = 12;
        what = mt_info;
        prea = "Tralics";
        break;
    case latexinfonoline_code:
        n       = 12;
        what    = mt_info;
        prea    = "Tralics";
        on_line = false;
        break;
    case genericerror_code:
        std  = false;
        what = mt_error;
        skip = 2;
        break;
    case genericinfo_code:
        std  = false;
        what = mt_info;
        break;
    case genericwarning_code:
        std  = false;
        what = mt_warning;
        break;
    default:;
    }
    if (!simple) prefix = string_to_write(write18_slot + 1);
    Buffer &B = txclasses_local_buf;
    B.clear();
    if (std) {
        std::string name = prefix;
        if (!simple) B += "(" + prefix + ")";
        while (n > 0) {
            --n;
            B << ' ';
        }
        prefix = B;
        B      = prea;
        if (!simple) B += " " + name;
        const char *posta = "Info";
        if (what == mt_error)
            posta = "Error";
        else if (what == mt_warning)
            posta = "Warning";
        B += " " + encode(the_names[posta]) + ": ";
    }
    TokenList L = scan_general_text();
    L.push_back(hash_table.relax_token);
    read_toks_edef(L);
    if (!L.empty()) {
        if (L.back() == hash_table.relax_token)
            L.pop_back();
        else
            on_line = false;
    }
    auto C = L.begin();
    auto E = L.end();
    while (C != E) {
        if (*C == message_break_token)
            B += "\n" + prefix;
        else
            B.insert_token(*C, false);
        ++C;
    }
    if (on_line && what != mt_error) {
        B.format(" at line {}", get_cur_line());
        std::string f = get_cur_filename();
        if (!f.empty()) B += " of file " + f;
    }
    if (what != mt_error) B += ".\n";
    if (std && what == mt_error) skip = 1;
    if (skip != 0) ignore_arg();
    if (skip == 2) ignore_arg();
    out_warning(B, what);
}

void Parser::out_warning(Buffer &B, msg_type what) {
    std::string w = "Info";
    if (what == mt_error)
        w = "Error";
    else if (what == mt_warning)
        w = "Warning";
    if (!the_names["warning"].empty()) {
        flush_buffer();
        Xml *res = new Xml(the_names["warning"], new Xml(std::string(B)));
        res->id.add_attribute(the_names["c"], the_names[w]);
        res->id.add_attribute(the_names["l"], cur_line_to_istring());
        the_stack.add_last(res);
    }
    if (what == mt_none) return;
    auto res = B.convert_to_log_encoding();
    if (what == mt_error)
        parse_error(err_tok, res, "uerror");
    else if (what == mt_warning) {
        Logger::finish_seq();
        log_and_tty << res;
    } else
        the_log << res;
}

void Parser::T_change_element_name() {
    flush_buffer();
    bool        star  = remove_initial_star();
    std::string name  = special_next_arg();
    std::string value = sE_arg_nopar();
    if (star)
        the_names.set(name, value);
    else
        the_names.assign_name(name, value);
}

// -------------------------------------------------------------------
// Code for kvoptions

// Common function for all kv macros
void Parser::kvo_family(subtypes k) {
    switch (k) {
    case kvo_fam_set_code:
    case kvo_fam_get_code:
    case kvo_pre_set_code:
    case kvo_pre_get_code: kvo_family_etc(k); return;
    case kvo_bool_opt_code: kvo_bool_opt(); return;
    case kvo_comp_opt_code: kvo_comp_opt(); return;
    case kvo_boolkey_code: kvo_bool_key(); return;
    case kvo_voidkey_code: kvo_void_key(); return;
    case kvo_string_opt_code: kvo_string_opt(); return;
    case kvo_void_opt_code: kvo_void_opt(); return;
    case kvo_decdef_code: return;
    case kvo_process_code: kvo_process(); return;
    default: return;
    }
}

// Helper functions for kvo etc
// Insert {#1} in L
void classes_ns::add_sharp(TokenList &L) {
    L.push_front(make_char_token('}', 2));
    L.push_front(Token(other_t_offset, '1'));
    L.push_front(make_char_token('#', 6));
    L.push_front(make_char_token('{', 1));
}

// Puts \define@key{fam}{arg} in front of L
void Parser::call_define_key(TokenList &L, Token cmd, const std::string &arg, const std::string &fam) {
    TokenList aux = string_to_list(arg, true);
    L.splice(L.begin(), aux);
    aux = string_to_list(fam, true);
    L.splice(L.begin(), aux);
    L.push_front(hash_table.locate("define@key"));
    if (tracing_commands()) {
        Logger::finish_seq();
        the_log << cmd << "->" << L << "\n";
    }
    back_input(L);
}

// Generates
// \define@key{Fam}{arg}[true]{\KVO@boolkey{Pfoo}{fam}{arg}{##1}}
void Parser::finish_kvo_bool(Token T, const std::string &fam, const std::string &arg) {
    TokenList L, aux;
    classes_ns::register_key(arg);
    add_sharp(L);
    aux = string_to_list(arg, true);
    L.splice(L.begin(), aux);
    aux = string_to_list(fam, true);
    L.splice(L.begin(), aux);
    std::string s = the_class_data.cur_pack()->full_name();
    aux           = string_to_list(s, true);
    L.splice(L.begin(), aux);
    L.push_front(hash_table.locate("KVO@boolkey"));
    L.brace_me();
    aux = string_to_list("[true]", false);
    L.splice(L.begin(), aux);
    call_define_key(L, T, arg, fam);
}

// \KVO@boolkey{Pfoo}{fam}{arg}{val}
// checks that val is true/false and calls \fam@argval
void Parser::kvo_bool_key() {
    std::string A = sE_arg_nopar(); // package
    std::string C = sE_arg_nopar(); // prefix
    std::string D = sE_arg_nopar(); // key
    std::string d = sE_arg_nopar(); // val
    if (!(d == "true" || d == "false")) {
        Buffer &B = txclasses_local_buf;
        B         = fmt::format("Illegal boolean value {} ignored", d);
        parse_error(err_tok, B, "bad bool");
        log_and_tty << "Value  should be true or false in " << (A[0] == 'P' ? "package " : "class ") << A.substr(1) << ".\n";
        return;
    }
    txclasses_local_buf = C + '@' + D + d;
    back_input(hash_table.locate(txclasses_local_buf));
}

// \DeclareStringOption[a]{b}[c]
// is \define@key{P}{b}[c]{\def\p@b{##1}}

void Parser::kvo_string_opt() {
    Token       cmd   = cur_tok;
    auto        init  = read_optarg().value_or(TokenList{});
    std::string arg   = sE_arg_nopar();
    auto        deflt = read_optarg();
    classes_ns::register_key(arg);
    std::string fam = kvo_getfam();
    Buffer &    B   = txclasses_local_buf;
    B               = fam + "@" + arg;
    Token T         = hash_table.locate(B);
    if (!Hashtab::the_eqtb()[T.eqtb_loc()].val.is_undef_or_relax()) {
        parse_error(err_tok, "Cannot redefine ", T, "", "bad redef");
        return;
    }
    new_macro(init, T);
    TokenList L, aux;
    add_sharp(L);
    L.push_front(T);
    L.push_front(hash_table.locate("def"));
    L.brace_me();
    if (deflt) {
        deflt->brace_me();
        deflt->push_front(Token(other_t_offset, '['));
        deflt->push_back(Token(other_t_offset, ']'));
        L.splice(L.begin(), *deflt);
    }
    call_define_key(L, cmd, arg, fam);
}

// Signals an error if the option is not @VOID@
void Parser::kvo_void_key() {
    read_arg();                     // package (should appear in the error message)
    std::string C = sE_arg_nopar(); // current option
    std::string d = sE_arg_nopar(); // option value
    if (d == "@VOID@") return;
    parse_error(err_tok, "Option " + C + " takes no argument", "bad opt");
}

void Parser::kvo_process() {
    bool        ok  = remove_initial_star();
    std::string fam = ok ? kvo_getfam() : sE_arg_nopar();
    TokenList   spec;
    TokenList   L = classes_ns::cur_options(true, spec, true);
    L.brace_me();
    back_input(L);
    TokenList aux = string_to_list(fam, true);
    back_input(aux);
    back_input(hash_table.locate("setkeys"));
    back_input(spec);
}

void Parser::kvo_void_opt() {
    Token       cmd = cur_tok;
    std::string arg = sE_arg_nopar();
    std::string fam = kvo_getfam();
    Buffer &    B   = txclasses_local_buf;
    classes_ns::register_key(arg);
    B       = fam + "@" + arg;
    Token T = hash_table.locate(B);
    if (!Hashtab::the_eqtb()[T.eqtb_loc()].val.is_undef_or_relax()) {
        parse_error(err_tok, "Cannot redefine ", T, "", "bad redef");
        return;
    }
    back_input(T);
    M_def(false, false, user_cmd, rd_always);
    TokenList L, aux;
    add_sharp(L);
    L.push_back(T);
    aux = string_to_list(arg, true);
    L.splice(L.begin(), aux);
    std::string s = the_class_data.cur_pack()->full_name();
    aux           = string_to_list(s, true);
    L.splice(L.begin(), aux);
    L.push_front(hash_table.locate("KVO@voidkey"));
    L.brace_me();
    aux = string_to_list("[@VOID@]", false);
    L.splice(L.begin(), aux);
    call_define_key(L, cmd, arg, fam);
}

// Implements \DeclareBoolOption[def]{name}
// defined a boolean foo
void Parser::kvo_bool_opt() {
    Token       T   = cur_tok;
    std::string df  = sE_optarg_nopar();
    std::string arg = sE_arg_nopar();
    // Optional argument must be true or false
    if (!(df.empty() || df == "false" || df == "true")) { log_and_tty << "Bad option " << df << " of " << arg << " replaced by false\n"; }
    subtypes    v   = df == "true" ? if_true_code : if_false_code;
    std::string fam = kvo_getfam();
    std::string s   = fam + '@' + arg;
    if (!check_if_redef(s)) return;
    // This is \newif
    Token W = cur_tok;
    eq_define(W.eqtb_loc(), CmdChr(if_test_cmd, v), false);
    M_newif_aux(W, s, true);
    M_newif_aux(W, s, false);
    finish_kvo_bool(T, fam, arg);
}

// \DeclareComplementaryOption{new}{old}
void Parser::kvo_comp_opt() {
    Token       cmd  = cur_tok;
    std::string arg  = sE_arg_nopar();
    std::string comp = sE_arg_nopar();
    std::string fam  = kvo_getfam();
    Buffer &    B    = txclasses_local_buf;
    B                = "if" + fam + '@' + comp;
    Token T          = hash_table.locate(B);
    if (Hashtab::the_eqtb()[T.eqtb_loc()].val.is_undef()) {
        B = "Cannot generate code for `" + arg + "', no parent " + comp;
        parse_error(err_tok, B, "bad redef");
        return;
    }
    // make boolean old inverse of foo
    Token T1 = hash_table.locate(fam + '@' + comp + "true");
    Token T2 = hash_table.locate(fam + '@' + arg + "false");
    Token T3 = hash_table.locate(fam + '@' + comp + "false");
    Token T4 = hash_table.locate(fam + '@' + arg + "true");
    B        = fam + '@' + arg + "true"; // \todo useless?
    if (!Hashtab::the_eqtb()[T2.eqtb_loc()].val.is_undef_or_relax()) { parse_error(err_tok, "Cannot redefine ", T2, "", "bad redef"); }
    if (!Hashtab::the_eqtb()[T4.eqtb_loc()].val.is_undef_or_relax()) { parse_error(err_tok, "Cannot redefine ", T4, "", "bad redef"); }
    M_let_fast(T2, T1, true);
    M_let_fast(T4, T3, true);
    finish_kvo_bool(cmd, fam, arg);
}

// Get/set for family and prefix
void Parser::kvo_family_etc(subtypes k) {
    std::string s = the_class_data.cur_pack()->full_name();
    Buffer &    B = txclasses_local_buf;
    B             = "KVO@";
    if (k == kvo_fam_set_code || k == kvo_fam_get_code)
        B += "family@";
    else
        B += "prefix@";
    B += s;
    Token T = hash_table.locate(B);
    if (k == kvo_fam_set_code || k == kvo_pre_set_code) {
        TokenList L = read_arg();
        new_macro(L, T);
    } else if (Hashtab::the_eqtb()[T.eqtb_loc()].val.is_undef()) {
        B = s.substr(1);
        if (k == kvo_pre_get_code) B += "@";
        TokenList res = B.str_toks11(false);
        back_input(res);
    } else {
        back_input(T);
        expand_when_ok(true);
    }
}

// This gets prefix and family
auto Parser::kvo_getfam() -> std::string {
    back_input(hash_table.CB_token);
    kvo_family_etc(kvo_fam_get_code);
    back_input(hash_table.OB_token);
    return sE_arg_nopar();
}

// If arg is foo checks that \iffoo \footrue \foofalse
// are undefined . Puts \iffo in cur_tok
auto Parser::check_if_redef(const std::string &s) -> bool {
    Buffer &B = txclasses_local_buf;
    B         = s + "true";
    Token T2  = hash_table.locate(B);
    if (!Hashtab::the_eqtb()[T2.eqtb_loc()].val.is_undef_or_relax()) {
        parse_error(err_tok, "Cannot redefine ", T2, "", "bad redef");
        return false;
    }
    B        = s + "false";
    Token T3 = hash_table.locate(B);
    if (!Hashtab::the_eqtb()[T3.eqtb_loc()].val.is_undef_or_relax()) {
        parse_error(err_tok, "Cannot redefine ", T3, "", "bad redef");
        return false;
    }
    B        = "if" + s;
    Token T1 = hash_table.locate(B);
    if (!Hashtab::the_eqtb()[T1.eqtb_loc()].val.is_undef_or_relax()) {
        parse_error(err_tok, "Cannot redefine ", T1, "", "bad redef");
        return false;
    }
    cur_tok = T1;
    return true;
}

// --------------------------------------------------
// extensions for Xkeyval

// You can use <foo.sty> as optional argument
auto Parser::XKV_parse_filename() -> TokenList {
    skip_initial_space();
    if (cur_tok.is_valid()) back_input();
    if (cur_tok == Token(other_t_offset, '<')) {
        get_token();
        return read_until_nopar(Token(other_t_offset, '>'));
    }
    LatexPackage *C     = the_class_data.cur_pack();
    txclasses_local_buf = C->real_name() + (C->is_class() ? ".cls"s : ".sty"s);
    return txclasses_local_buf.str_toks11(false);
}
