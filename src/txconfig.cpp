// Tralics, a LaTeX to XML translator.
// Copright INRIA/apics/marelle (Jose' Grimm) 2008-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// The file contains code for configurating the Raweb

#include "tralics/Logger.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include "txinline.h"
#include "txparam.h"

ParamDataVector config_data;

namespace {
    auto is_good_ur(const std::string &x) -> bool {
        std::vector<ParamDataSlot> &ur_list = config_data.data[0]->data;
        auto                        n       = ur_list.size();
        if (ur_size == 0) {
            for (size_t i = 0; i < n; i++) ur_list[i].mark_used();
            ur_size = n;
        }
        for (size_t i = 0; i < n; i++)
            if (ur_list[i].matches(x)) return true;
        return false;
    }

    // Interprets the RC argument of a pers command \todo RA
    // This returns the short name, said otherwise, the argument.
    // Notice the space case when argument is empty, or +foo or =foo.
    auto pers_rc(const std::string &rc) -> std::string {
        if (rc.empty()) {
            if (have_default_ur) return the_default_rc;
            if (the_main->handling_ra && the_parser.get_ra_year() > 2006) {
                // signal error, make a default
                the_parser.parse_error("No default Research Centre defined");
                the_default_rc = "unknown";
            }
            have_default_ur = true;
            return the_default_rc;
        }
        if (rc[0] == '+') { return rc.substr(1); }
        bool spec = (rc.size() >= 2 && rc[0] == '=');
        auto RC   = spec ? rc.substr(1) : rc;
        if (!is_good_ur(RC)) {
            err_buf                       = "Invalid Unit Centre " + rc + "\nUse one of:";
            std::vector<ParamDataSlot> &V = config_data.data[0]->data;
            for (auto &i : V)
                if (i.is_used) err_buf += " " + i.key;
            the_parser.signal_error(the_parser.err_tok, "illegal data");
        }
        if (spec) {
            the_default_rc  = RC;
            have_default_ur = true;
        }
        return RC;
    }
} // namespace

namespace config_ns {
    bool cur_sec_no_topic = false;
} // namespace config_ns

namespace config_ns {
    auto check_section(const std::string &s) -> std::string;
    auto check_spec_section(const std::string &s) -> std::string;
} // namespace config_ns

using namespace config_ns;

namespace config_ns {
    // We add a final slash, or double slash, this makes parsing easier;
    // We also remove an initial slash (This is not done if the separator is a
    // space, case where s is empty).
    // An initial plus sign means: append the line to the vector, else reset.

    auto start_interpret(Buffer &B, String s) -> bool {
        bool ret_val = false;
        B.append(s);
        B.ptrs.b = 0;
        if (B.head() == '+') {
            B.advance();
            the_log << "+";
        } else
            ret_val = true;
        if ((s[0] != 0) && B.head() == '/') B.advance();
        return ret_val;
    }
} // namespace config_ns

// --------------------------------------------------

// Configuration file option lists.
// The configuration file may contains lines of the form
// FOO_vals="/A/B/C", or FOO_vals="/A/a/B/b/C/c".
// transformed into a vector of data, named FOO, with some execptions.

// This error is fatal
// We make sure these four items always exist
ParamDataVector::ParamDataVector() {
    data.push_back(new ParamDataList("ur"));
    data.push_back(new ParamDataList("sections"));
    data.push_back(new ParamDataList("profession"));
    data.push_back(new ParamDataList("affiliation"));
}

// We may add a special slot at the end
void ParamDataList::check_other() {
    if ((std::islower(name[0]) != 0) && !empty()) push_back(ParamDataSlot("Other", "Other"));
}

// --------------------------------------------------

// --------------------------------------------------

// Return the data associated to name, may create depend on creat
auto ParamDataVector::find_list(const std::string &name, bool creat) -> ParamDataList * {
    auto n = data.size();
    for (size_t i = 0; i < n; i++)
        if (data[i]->its_me(name)) return data[i];
    if (!creat) return nullptr;
    auto *res = new ParamDataList(name);
    data.push_back(res);
    return res;
}

void ParamDataList::keys_to_buffer(Buffer &B) const {
    for (const auto &i : data) B.format(" {}", i.key);
}

// Return the value of the key in a list.
auto config_ns::find_one_key(const std::string &name, const std::string &key) -> std::string {
    if (name == "ur") return pers_rc(key);
    if (name == "theme") return MainClass::check_theme(key);
    if (name == "fullsection") return check_section(key);
    if (name == "section") return check_spec_section(key);
    ParamDataList *X = config_data.find_list(name, false);
    if (X == nullptr) {
        the_parser.parse_error(the_parser.err_tok, "Configuration file does not define ", name, "no list");
        return "";
    }
    auto n = X->size();
    for (size_t i = 0; i < n; i++)
        if (X->data[i].key == key) return X->data[i].value;
    err_buf = fmt::format("Illegal value '{}' for {}\nUse one of:", key, name);
    X->keys_to_buffer(err_buf);
    the_parser.signal_error(the_parser.err_tok, "illegal data");
    return "";
}

// return non-empty string only if section is new
auto config_ns::check_section(const std::string &s) -> std::string {
    static long cur_section = -1;
    long        k           = -1;
    err_buf.clear();
    std::vector<ParamDataSlot> &X = config_data.data[1]->data;
    auto                        n = X.size(); // number of sections
    if (s.empty())
        k = cur_section;
    else
        for (size_t i = 0; i < n; i++)
            if (X[i].key == s) {
                k = to_signed(i + 1);
                break;
            }
    if (k > 0 && k < cur_section) {
        err_buf.format("Bad section {} after {}\nOrder of sections is{}", s, X[to_unsigned(cur_section - 1)].key, sec_buffer);
        the_parser.signal_error();
    } else if (k == -1) {
        if (n == 0) {
            the_parser.parse_error("Illegal access to fullsection list.");
            return "";
        }
        if (!s.empty()) {
            err_buf.format("Invalid section {}\nValid sections are{}", s, sec_buffer);
            the_parser.signal_error();
            if (cur_section < 0) cur_section = 1;
        }
    } else
        cur_section = k;
    if (cur_section < 0) {
        the_parser.parse_error("No default section");
        cur_section = 1;
    }
    if (cur_section == composition_section) {
        static bool first_module = true;
        if (first_module)
            first_module = false;
        else {
            the_parser.parse_error("Only one module accepted in composition");
            cur_section++;
            return "";
        }
    }
    static long prev = -1;
    if (prev == cur_section) return "";
    prev             = cur_section;
    cur_sec_no_topic = X[to_unsigned(cur_section - 1)].no_topic();
    X[to_unsigned(cur_section - 1)].mark_used(); // incompatible with topics
    return X[to_unsigned(cur_section - 1)].value;
}

// Special command. We assume that cur_sec_no_topic
// is correctlty set.
auto config_ns::check_spec_section(const std::string &s) -> std::string {
    if (cur_sec_no_topic) return "";
    if (s.empty()) return "default";
    return s;
}

// -----------------------------------------------------------

// --------------------------------------------------

// If str is, say `Cog A', this puts ` cog ' in the buffer, returns `cog'.
auto Buffer::add_with_space(const std::string &s) -> std::string {
    size_t i = 0;
    while (s[i] == ' ') ++i;
    clear();
    push_back(' ');
    while ((s[i] != 0) && (s[i] != ' ')) push_back(s[i++]);
    lowercase();
    std::string res = substr(1);
    push_back(' ');
    return res;
}

// --------------------------------------------------

// This creates the file foo_.bbl and initiates the bibtex translation.

// In the case of "foo bar gee", puts foo, bar and gee in the vector.
// Initial + means append, otherwise replace.

void Buffer::interpret_aux(std::vector<std::string> &bib, std::vector<std::string> &bib2) {
    if (start_interpret(*this, "")) {
        bib.resize(0);
        bib2.resize(0);
    }
    for (;;) {
        bool keep = true;
        skip_sp_tab();
        if (head() == 0) break;
        auto a = ptrs.b;
        if (head() == '-') {
            keep = false;
            advance();
            if (head() == 0) break; // final dash ignored
            a++;
            the_log << "--";
        }
        while ((head() != 0) && (std::isspace(head()) == 0)) advance();
        ptrs.a        = a;
        std::string k = substring();
        if (keep)
            bib.emplace_back(k);
        else
            bib2.emplace_back(k);
        the_log << k << " ";
    }
    the_log << "\n";
}
