#include "tralics/TitlePageAux.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/TitlePage.h"
#include "tralics/TitlePageFullLine.h"

// Ctor from a TitlePageFullLine. It's just a copy.
TitlePageAux::TitlePageAux(TitlePageFullLine &X) {
    T1     = X.item1.value;
    T2     = X.item2.value;
    T3     = X.item3.value;
    T4     = X.item4.value;
    xflags = X.flags;
}

// More classification. This allocates memory, and modifies state
auto TitlePageAux::classify(tpi_vals w, int &state) -> bool {
    switch (w) {
    case tpi_CESS: // \makeRR <rrstart> "ok" "ok"
        state = 1;
        idx   = 0;
        type  = tpi_rt_tp;
        return true;
    case tpi_ACSS: // alias \makeRT "ok" "ok"
        T1   = T2;
        T2   = Titlepage.bigtable[0].T2;
        type = tpi_rt_tp;
        return true; // state is still 1
    case tpi_CES:    // \title <title> "notitle"
        state = 2;
        type  = tpi_rt_normal;
        T4    = T3;
        T3    = T2;
        idx   = Titlepage.increase_data();
        if (get_flags2() == tp_A_flag) {
            Buffer B;
            B = "\\" + T1;
            B.format("{{{}}}", T4);
            B.append("%\n");
            the_main->add_to_from_config(1, B);
        }
        return true;
    case tpi_CEES: // \author +<aulist> <au> "default"
        state = 2;
        type  = tpi_rt_list;
        idx   = Titlepage.increase_data();
        return true;
    case tpi_E:    //  <UR> -
        state = 3; // next line cannot be alias
        type  = tpi_rt_urlist;
        idx   = Titlepage.increase_data();
        T2    = T1;
        return true;
    case tpi_CE: // case \URsop ?+ <UR>
    {
        auto k = Titlepage.find_UR(T2, T1);
        if (k == 0) return false;
        state = 2;
        type  = tpi_rt_ur;
        idx   = k;
        T2    = tp_local_buf;
        return true;
    }
    case tpi_S: // case "<foo/>"
        type = tpi_rt_constant;
        idx  = Titlepage.increase_data();
        return true;
    case tpi_EC: // case execute \foo
        type = tpi_rt_exec;
        idx  = Titlepage.increase_data();
        return true;
    case tpi_CEE: // \Paris ?<UR> <Rocqu>
    {
        auto k = Titlepage.find_UR(T2, "");
        if (k == 0) return false;
        state = 2;
        type  = tpi_rt_ur;
        idx   = k;
        T2    = T3;
        T3    = "";
        return true;
    }
    case tpi_CCS: // case \myself \author "JG"
    {
        auto k = Titlepage.find_cmd(T2);
        if (k == 0) return false;
        TitlePageAux &R = Titlepage.bigtable[k];
        state           = 2;
        idx             = R.idx;
        xflags          = k; // hack...
        T2              = T3;
        T3              = R.T3;
        type            = R.type == tpi_rt_normal ? tpi_rt_normal_def : tpi_rt_list_def;
    }
        return true;
    case tpi_AC:
        type = tpi_rt_alias;
        T1   = T2;
        T2   = Titlepage.bigtable.back().T1;
        return true;
    default: return false;
    }
}

// Evaluates at start of translation
void TitlePageAux::exec_start(size_t k) {
    if (type == tpi_rt_urlist) { // no command here
        Titlepage[idx] = convert(2, new Xml(the_names["\n"]));
        return;
    }
    if (type == tpi_rt_alias) { // \let\this\that
        hash_table.eval_let(T1, T2);
        return;
    }
    // in all other cases define this as (titlepage,k)
    hash_table.primitive(T1, titlepage_cmd, subtypes(k));
    if (type == tpi_rt_list) { // initialise the list
        Titlepage[idx] = convert(2, convert(3, std::string(T4)));
        return;
    }
    // remaining types are:  tp, ur, normal_def, list_def, and normal
    if (type != tpi_rt_normal) return;
    auto fl = get_flags2(); // this is 0,1, 2 or 3
    if (fl == 0) {
        Titlepage[idx] = convert(2, std::string(T4));
        return;
    }
    Buffer &B = tp_local_buf;
    B         = "\\" + T1;
    B.format("{{{}}}", T4);
    Titlepage[idx] = new Xml(std::string("empty"));
    B.append("%\n");
    if (fl == tp_A_flag) {
    } else if (fl == tp_B_flag)
        the_parser.add_buffer_to_document_hook(B, "(tpa init)");
    else
        T4 = B;
}

// This is executed when we see the \Titlepage cmd
void TitlePageAux::exec_post() const {
    if (type == tpi_rt_constant) Titlepage[idx] = new Xml(std::string(T1));
    if (type == tpi_rt_exec) Titlepage[idx] = the_parser.tpa_exec(T2);
    if (type != tpi_rt_normal) return;
    if (get_flags2() == tp_C_flag) the_parser.titlepage_evaluate(T4, T1);
    if (!has_plus_flags()) return;
    if (has_u_flags()) return;
    the_parser.parse_error(the_parser.err_tok, "No value given for command \\", T1, "");
}

// This is executed when the user asks for a titlepage command.
void TitlePageAux::exec(size_t v, bool vb) {
    if (vb) {
        Logger::finish_seq();
        the_log << "{\\titlepage " << v << "=\\" << T1 << "}\n";
    }
    if (type == tpi_rt_tp) {
        the_parser.T_titlepage_finish(v);
        return;
    }
    if (type == tpi_rt_ur) { // easy case
        Titlepage[idx]->add_last_nl(convert(2));
        return;
    }
    Xml *R{nullptr};
    if (type == tpi_rt_normal_def || type == tpi_rt_list_def)
        R = convert(3, std::string(T2));
    else { // we have to read the argument.
        R = the_parser.special_tpa_arg(T1, T3, has_p_flags(), has_e_flags(), has_q_flags());
    }
    bool replace = (type == tpi_rt_normal || type == tpi_rt_normal_def);
    if (increment_flag()) {
        if (!replace) Titlepage[idx]->remove_last();
    }
    if (replace)
        Titlepage[idx] = R;
    else
        Titlepage[idx]->add_last_nl(R);
}

// Finds the real flag to increment.
// If 0, increments it and returns true, otherwise return false.
// In the case of =tpi_rt_*_def, the flag is a pointer !
auto TitlePageAux::increment_flag() -> bool {
    if (type == tpi_rt_normal_def || type == tpi_rt_list_def) return Titlepage.bigtable[xflags].increment_flag();
    if (!has_u_flags()) {
        xflags++;
        return true;
    }
    return false;
}

auto TitlePageAux::convert(int i, const std::string &s) -> Xml * { return convert(i, new Xml(s)); }

// For the case CEE, \Paris ?<UR flags> <Rocq>
// s is the string without attribs, and n is the length
auto TitlePageAux::find_UR(const std::string &s, size_t n) const -> size_t {
    if (type != tpi_rt_urlist) return 0;
    if (T2.substr(0, n) == s.substr(0, n)) return idx;
    return 0;
}

// true if this OK for CCS and the nams is right.
auto TitlePageAux::find_cmd(const std::string &s) const -> bool {
    if (type != tpi_rt_normal && type != tpi_rt_list) return false;
    if (T1 != s) return false;
    return true;
}

// Returns the index for a CCS.
auto TitlePage::find_cmd(const std::string &s) const -> size_t {
    for (unsigned int k = 0; k < bigtable.size(); k++) {
        if (bigtable[k].find_cmd(s)) return k;
    }
    return 0;
}

// This prints the flags, in a symbolic way.
void TitlePageAux::decode_flags() const {
    auto f2 = xflags / 16;
    auto f1 = xflags % 16;
    f1      = f1 / 2;
    if (f1 == 0 && f2 == 0) return;
    the_log << " (flags";
    if (f1 == 1) the_log << " +par";
    if (f1 == 2) the_log << " +env";
    if (f1 == 3) the_log << " +par +env";
    if (f1 == 4) the_log << " -par";
    if (f2 == 1) the_log << " +list";
    if (f2 == 2) the_log << " +A";
    if (f2 == 4) the_log << " +B";
    if (f2 == 6) the_log << " +C";
    the_log << ")";
}

// This prints a slot.
void TitlePageAux::dump(size_t k) const {
    tpi_vals t = type;
    if (t == tpi_rt_alias) {
        Logger::finish_seq();
        the_log << "Defining \\" << T1 << " as alias to \\" << T2 << "\n";
        return;
    }
    if (t == tpi_rt_constant) {
        Logger::finish_seq();
        the_log << "Inserting the string " << T1 << "\n";
        return;
    }
    if (t == tpi_rt_exec) {
        Logger::finish_seq();
        the_log << "Inserting the command \\" << T2 << "\n";
        return;
    }
    Logger::finish_seq();
    the_log << "Defining \\" << T1 << " as \\TitlePageCmd " << k << "\n";
    if (t == tpi_rt_normal)
        the_log << "   usual <" << T3 << "/>";
    else if (t == tpi_rt_normal_def)
        the_log << "   usual? <" << T2 << "/>"
                << " " << T3;
    else if (t == tpi_rt_list)
        the_log << "   list <" << T2 << "/> and <" << T3 << "/>";
    else if (t == tpi_rt_list_def)
        the_log << "   list? <" << T2 << "/> and <" << T3 << "/>";
    else if (t == tpi_rt_urlist)
        the_log << "   ur_list <" << T2 << "/>";
    else if (t == tpi_rt_ur)
        the_log << "   ur <" << T2 << "/>";
    else if (t == tpi_rt_tp)
        the_log << "   main <" << T2 << " " << T3 << " -- " << T4 << "/>"; //   id??
    else
        the_log << "   random";
    if (t != tpi_rt_normal_def && t != tpi_rt_list_def) decode_flags(); // otherwise flags are meaningless
    the_log << "\n";
}

// Converts one of the strings into an empty XML element
auto TitlePageAux::convert(int i) -> Xml * {
    std::string s = i == 1 ? T1 : i == 2 ? T2 : i == 3 ? T3 : T4;
    return new Xml(tp_local_buf.xml_and_attrib(s));
}

// Converts one of the strings into a XML element containing R.
auto TitlePageAux::convert(int i, Xml *r) -> Xml * {
    std::string s   = i == 1 ? T1 : i == 2 ? T2 : i == 3 ? T3 : T4;
    Xml *       res = new Xml(tp_local_buf.xml_and_attrib(s));
    res->push_back_unless_nullptr(r);
    return res;
}
