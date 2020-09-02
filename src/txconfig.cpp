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

namespace config_ns {
    Buffer      sec_buffer;
    bool        have_default_ur = false;
    std::string the_default_rc;
    size_t      ur_size{0};
    long        composition_section = -1;
    bool        cur_sec_no_topic    = false;
} // namespace config_ns

namespace config_ns {
    void interpret_section_list(Buffer &B, bool new_syntax);
    void interpret_data_list(Buffer &B, const std::string &name);
    void interpret_theme_list(const Buffer &B);
    auto is_good_ur(const std::string &x) -> bool;
    auto next_RC_in_buffer(Buffer &B, std::string &sname, std::string &lname) -> long;
    auto check_section(const std::string &s) -> std::string;
    auto check_spec_section(const std::string &s) -> std::string;
} // namespace config_ns

using namespace config_ns;

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
    if (is_lower_case(name[0]) && !empty()) push_back(ParamDataSlot("Other", "Other"));
}

// --------------------------------------------------
// This interprets theme_vals = "foo bar"
// and all consequences

void config_ns::interpret_list(const std::string &a, Buffer &b) {
    if (a == "section")
        interpret_section_list(b, false);
    else if (a == "fullsection")
        interpret_section_list(b, true);
    else if (a == "theme")
        interpret_theme_list(b);
    else if (a.empty()) {
    } else
        interpret_data_list(b, a);
}

// Function called when theme_vals is seen in the config file.
void config_ns::interpret_theme_list(const Buffer &B) {
    all_themes = " " + B + " ";
    for (char &c : all_themes)
        if (is_upper_case(c)) c += 'a' - 'A';
}

// --------------------------------------------------

// Interprets Sections ="/foo/bar*/gee".
// In 2007, we have  NewSections ="/foo/FOO/bar/BAR/gee/GEE"
// This is indicated by new_syntax
// Puts foo bar+ gee in the transcript file (we use + instead of *, meaning
// that the character was interpreted). Three RaSection objects are
// created, named foo, bar and gee. The second is marked: not for topic.
// In simplified mode, a section has a name and a number.
// Otherwise it has additional fields.
void config_ns::interpret_section_list(Buffer &B, bool new_syntax) {
    ParamDataList *V = config_data.data[1];
    if (start_interpret(B, "//")) {
        V->reset();
        sec_buffer.clear();
        composition_section = -1;
    }
    std::string s, r;
    for (;;) {
        if (!B.slash_separated(s)) break;
        if (new_syntax) {
            if (!B.slash_separated(r)) break;
        } else
            r = "";
        if (s.empty()) continue;
        bool star = false;
        auto ns   = s.size();
        if (ns > 1 && s[ns - 1] == '*') {
            s    = std::string(s, 0, ns - 1);
            star = true;
        }
        if (r.empty()) r = s;
        the_log << "Section: " << s << (star ? "+" : "") << " -> " << r << "\n";
        if (s == "composition") composition_section = to_signed(V->size() + 1);
        sec_buffer += " " + s;
        V->push_back(ParamDataSlot(s, r, !star));
    }
}

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

void ParamDataSlot::to_buffer(Buffer &B) const { B.format("{}={},", key, value); }

void ParamDataList::keys_to_buffer(Buffer &B) const {
    for (const auto &i : data) B.format(" {}", i.key);
}

// Converts the whole data struture as foo1=bar1,foo2=bar2,
auto config_ns::find_keys(const std::string &name) -> std::string {
    ParamDataList *X = config_data.find_list(name, false);
    if (X == nullptr) return "";
    Buffer B;
    auto   n = X->size();
    for (size_t i = 0; i < n; i++) X->data[i].to_buffer(B);
    if (n > 0) B.pop_back();
    return std::move(B);
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

// We add a final slash, or double slash, this makes parsing easier;
// We also remove an initial slash (This is not done if the separator is a
// space, case where s is empty).
// An initial plus sign means: append the line to the vector, else reset.

auto config_ns::start_interpret(Buffer &B, String s) -> bool {
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

// This interprets Foo="Visiteur/foo/Chercheur//Enseignant//Technique//"
// This creates four slots in a table indexed by k.
void config_ns::interpret_data_list(Buffer &B, const std::string &name) {
    ParamDataList *V = config_data.find_list(name, true);
    if (start_interpret(B, "//")) V->reset();
    for (;;) {
        std::string r1, r2;
        if (!B.slash_separated(r1)) return;
        if (!B.slash_separated(r2)) return;
        if (r1.empty()) continue;
        if (r2.empty()) r2 = r1;
        the_log << name << ": " << r1 << " -> " << r2 << "\n";
        V->push_back(ParamDataSlot(r1, r2));
    }
}

// -----------------------------------------------------------

// User says \UR{foo,bar}
// returns -1 if there no other RC in the buffer.
// returns -2 if the RC is invalid
// returns location in the table otherwise
auto config_ns::next_RC_in_buffer(Buffer &B, std::string &sname, std::string &lname) -> long {
    std::vector<ParamDataSlot> &ur_list = config_data.data[0]->data;
    B.skip_sp_tab_comma();
    if (B.head() == 0) return -1;
    if (B.substr(B.ptrs.b, 3) == "\\UR") {
        static bool warned = false;
        if (!warned && the_parser.get_ra_year() > 2006) {
            log_and_tty << "You should use Lille instead of \\URLille,\n";
            log_and_tty << "Nancy instead of \\URNancy, etc.\n";
            warned = true;
        }
        B.advance(3);
    }
    B.ptrs.a = B.ptrs.b;
    B.skip_letter();
    auto k = ur_list.size();
    for (size_t j = 0; j < k; j++)
        if (B.substring() == ur_list[j].key) {
            sname = ur_list[j].key;
            lname = ur_list[j].value;
            ur_list[j].mark_used();
            return to_signed(j);
        }
    return -2;
}

// This is the function used since 2007, when definining the
// Research Centers of the team;
// May return  <URRocquencourt/><URSophia/>
void config_ns::check_RC(Buffer &B, Xml *res) {
    const std::string &tmp      = the_names["rcval"];
    bool               need_elt = tmp[0] == '+'; // Hack
    std::string        str;
    if (need_elt) str = tmp.substr(1);
    Buffer      temp2;
    std::string sname, lname;
    temp2.clear();
    std::vector<int> vals;
    size_t           nb = 0;
    B.ptrs.b            = 0;
    for (;;) {
        auto j = next_RC_in_buffer(B, sname, lname);
        if (j == -1) break;
        if (j == -2) {
            nb = 0;
            break;
        }
        Xml *new_elt{nullptr};
        if (need_elt)
            new_elt = new Xml(std::string(str + sname), nullptr);
        else {
            new_elt = new Xml(the_names["rcval"], nullptr);
            new_elt->id.add_attribute(std::string("name"), std::string(sname));
        }
        res->push_back_unless_nullptr(new_elt);
        temp2 += sname + " ";
        the_default_rc = sname;
        nb++;
    }
    ur_size = nb;
    if (nb == 1) have_default_ur = true;
    if (nb != 0) {
        the_log << "Localisation " << temp2 << "\n";
        return;
    }

    if (B.empty())
        err_buf = "Empty localisation value\n";
    else
        err_buf = "Illegal localisation value: " + B + "\n";
    err_buf += "Use one or more of:";
    config_data.data[0]->keys_to_buffer(err_buf);
    the_parser.signal_error();
}

// Interprets the RC argument of a pers command \todo RA
// This returns the short name, said otherwise, the argument.
// Notice the space case when argument is empty, or +foo or =foo.
auto config_ns::pers_rc(const std::string &rc) -> std::string {
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

auto config_ns::is_good_ur(const std::string &x) -> bool {
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
    if (config_ns::start_interpret(*this, "")) {
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
        while ((head() != 0) && !is_space(head())) advance();
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
