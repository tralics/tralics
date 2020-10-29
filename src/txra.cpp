// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002, 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// This file contains commands needed for the RA

#include "tralics/Logger.h"
#include "tralics/ParamDataVector.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

namespace {
    Buffer Tbuf;

    // User says \UR{foo,bar}
    // returns -1 if there no other RC in the buffer.
    // returns -2 if the RC is invalid
    // returns location in the table otherwise
    auto next_RC_in_buffer(Buffer &B, std::string &sname, std::string &lname) -> long {
        std::vector<ParamDataSlot> &ur_list = config_data[0];
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
                sname              = ur_list[j].key;
                lname              = ur_list[j].value;
                ur_list[j].is_used = true;
                return to_signed(j);
            }
        return -2;
    }

    // This is the function used since 2007, when definining the
    // Research Centers of the team;
    // May return  <URRocquencourt/><URSophia/>
    void check_RC(Buffer &B, Xml *res) {
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
        config_data[0].keys_to_buffer(err_buf);
        the_parser.signal_error();
    }
} // namespace

namespace ra_ns {
    void fnhack(TokenList &c, TokenList &d, TokenList &aux);
} // namespace ra_ns

auto Parser::next_arg_is_project() -> bool {
    TokenList L = read_arg();
    if (L.empty()) return true;
    return L.front().is_one_token();
}

// --------------------------------------------------

// If c has no \footnote, copies it in aux;
// else copies only what is before, what is after is appended to d
void ra_ns::fnhack(TokenList &c, TokenList &d, TokenList &aux) {
    Hashtab &H  = the_parser.hash_table;
    int      bl = 0;
    while (!c.empty()) {
        Token x = c.front();
        c.pop_front();
        token_ns::check_brace(x, bl);
        if (x == H.footnote_token && bl == 0) {
            if (!d.empty()) {
                d.push_front(H.space_token);
                d.push_front(H.comma_token);
            }
            d.splice(d.begin(), c);
        } else {
            aux.push_back(x);
        }
    }
}

// User function \fn@hack\foo\bar
void Parser::fnhack() {
    Token     A = get_r_token();
    Token     B = get_r_token();
    TokenList La, Lb = get_mac_value(B), Li = get_mac_value(A);
    ra_ns::fnhack(Li, Lb, La);
    new_macro(La, A, false);
    new_macro(Lb, B, false);
}

// User function associated to \UR in the RA
void Parser::interpret_rc() {
    TokenList L = read_arg();
    Tbuf.clear();
    Tbuf << L; // \todo make TokenList formattable
    Xml *res = new Xml(the_names["rclist"], nullptr);
    the_stack.add_last(res);
    check_RC(Tbuf, res);
}

// --------------------------------------------------
// \end{RAsection} or \tralics@pop@section
Xid  compo_id = Xid(decltype(Xid::value)(-1));
void Parser::T_rasection_end() {
    Xml *in = the_stack.top_stack();
    if (in->id == compo_id) in->compo_special();
    the_stack.pop(the_names["RAsection"]);
}

// \begin{RAsection} or \tralics@push@section
void Parser::T_rasection() {
    std::string name     = sT_arg_nopar();
    std::string elt_name = the_names["nb_rasection"];
    auto        iname    = name;
    leave_h_mode();
    the_stack.add_nl();
    Xml *cur = new Xml(elt_name.empty() ? iname : elt_name, nullptr);
    if (!elt_name.empty()) cur->id.add_attribute(the_names["name"], iname);
    if (iname == the_names["composition_ra"]) compo_id = cur->id;
    the_stack.push(the_names["RAsection"], cur);
    string_define(0, name, false);
    std::string id = the_stack.add_new_anchor();
    create_label("section:" + name, id);
}

void Parser::push_module() {
    std::string aux = sT_arg_nopar();
    push_module(aux);
}

void Parser::push_module(const std::string &aux) {
    if (the_stack.in_h_mode()) { parse_error("Invalid \\begin{module} in a paragraph"); }
    leave_h_mode();
    the_stack.add_nl();
    the_stack.push1(the_names["module"]);
    //  refstepcounter("module",false); Assume done by the package
    start_paras(8, aux, false);
}
