// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose' Grimm) 2002-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Tralics, math part I

#include "txmath.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <algorithm>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace {
    Buffer             math_buffer;
    Buffer             special_buffer;
    Buffer             math_buffer1;
    Buffer             Trace;
    size_t             old_pos = 0;       // pointer into trace
    MathHelper         cmi;               // Data structure holding some global values
    bool               trace_needs_space; // bool  for: \frac\pi y
    bool               old_need = false;  // prev value of trace_needs_space
    Xml *              xmlspace;          // Const, xml element containing a space
    Token              fct_caller;
    std::string        the_tag;
    std::vector<Xml *> all_maths;

    auto is_m_font(symcodes cmd) -> bool {
        return cmd == math_font_cmd || cmd == oldfont_cmd || cmd == argfont_cmd || cmd == noargfont_cmd;
    }

    auto sub_to_math(subtypes x) -> math_list_type { return math_list_type(long(x) + long(fml_offset)); }

    // Creates a table for array specifications.
    auto rlc_to_string(const std::string &s) -> std::vector<AttList> {
        std::vector<AttList> res;
        for (char c : s) {
            if (c != 'r' && c != 'l' && c != 'c') continue;
            AttList L;
            if (c == 'l')
                L.push_back(np_columnalign, np_left);
            else if (c == 'r')
                L.push_back(np_columnalign, np_right);
            res.push_back(L);
        }
        return res;
    }
} // namespace

MathDataP math_data;

using namespace math_ns;
namespace math_ns {
    void add_to_trace(Token T);
    void add_to_trace(char x);
    void add_to_trace(const std::string &x);
    void remove_from_trace();
    void bad_math_warn(Buffer &B);
    auto finish_cv_special(bool isfrac, Istring s, size_t pos, Xml *a, Xml *b, const Istring &sz, int numalign, int denalign, int style,
                           size_t open, size_t close) -> Xml *;
} // namespace math_ns

namespace tralics_ns {
    auto math_env_name(subtypes c) -> String;
    auto math_env_props(subtypes c) -> int;
} // namespace tralics_ns

using tralics_ns::math_env_props;

// This duplicates the formula.
// It removes tokens preceeded by the special marker.

auto Math::duplicate(bool nomath) const -> subtypes {
    subtypes k  = math_data.find_math_location(type, sname);
    Math &   cp = math_data.get_list(k);
    int      sp = 0, sm = 0;
    bool     skipping  = false;
    bool     skip_next = false;
    auto     C         = value.begin();
    auto     E         = value.end();
    while (C != E) {
        skip_next = skipping;
        if (C->get_cmd() == nomath_cmd) {
            int s = C->get_chr();
            ++C;
            if (C == E) break;
            if (s == 0)
                skip_next = nomath;
            else if (s == 1)
                skip_next = !nomath;
            else {
                if (s == 2)
                    sp++;
                else if (s == 3) {
                    if (sp > 0) sp--;
                    sm++;
                } else if (s == 4) {
                    if (sm > 0) sm--;
                } else {
                    sp = sm = 0;
                } // should not happen, clear the stack
                skipping = false;
                if (sp > 0 && nomath) skipping = true;
                if (sm > 0 && !nomath) skipping = true;
                skip_next = skipping;
            }
        }
        if (skip_next) {
        } else if (C->get_cmd() == math_list_cmd) {
            Math &   v  = C->get_list();
            subtypes k1 = v.duplicate(nomath);
            cp.push_back_list(k1, C->get_lcmd());
        } else if (C->get_cmd() == special_math_cmd) {
            Math &   v  = C->get_list();
            subtypes k1 = v.duplicate(nomath);
            cp.push_back(CmdChr(special_math_cmd, k1), C->get_font());
        } else
            cp.push_back(*C);
        ++C;
    }
    return k;
}

void Parser::select_math_font() {
    bool bold = is_pos_par(atmathversion_code);
    word_define(math_font_pos, bold ? math_f_bold : math_f_normal, false);
}

void math_ns::remove_from_trace() {
    trace_needs_space = old_need;
    Trace.resize(old_pos);
}

// Three functions that add something to the trace.
// We want spaces line this: \frac\pi x\bar\alpha

void math_ns::add_to_trace(Token T) {
    old_need = trace_needs_space;
    old_pos  = Trace.size();
    auto x   = T.val;
    if (trace_needs_space && T.cmd_val() == letter_catcode) Trace.push_back(' ');
    if (T.is_a_char() && T.char_val() == '\n')
        Trace.push_back("^^J");
    else
        Trace.push_back(T);
    trace_needs_space = false;
    if (x >= single_offset) trace_needs_space = true;
}

void math_ns::add_to_trace(const std::string &x) {
    Trace.push_back_braced(x);
    trace_needs_space = false;
}

void math_ns::add_to_trace(char x) {
    Trace.push_back(x);
    trace_needs_space = false;
}

auto math_ns::get_builtin(size_t p) -> Xml * { return math_data.get_builtin(p); } // \todo why a separate global method?

// -----------------------------------------------------------------------
// Math environments. The following are recognised.

auto tralics_ns::math_env_name(subtypes chr) -> String {
    switch (chr) {
    case eqnarray_code: return "endeqnarray";
    case eqnarray_star_code: return "endeqnarray*";
    case Beqnarray_code: return "endBeqnarray";
    case Beqnarray_star_code: return "endBeqnarray*";
    case multline_code: return "endmultline";
    case multline_star_code: return "endmultline*";
    case gather_code: return "endgather";
    case gather_star_code: return "endgather*";
    case equation_code: return "endequation";
    case equation_star_code: return "endequation*";
    case align_code: return "endalign";
    case align_star_code: return "endalign*";
    case flalign_code: return "endflalign";
    case flalign_star_code: return "endflalign*";
    case alignat_code: return "endalignat";
    case alignat_star_code: return "endalignat*";
    case xalignat_code: return "endxalignat";
    case xalignat_star_code: return "endxalignat*";
    case xxalignat_code: return "endxxalignat";
    case xxalignat_star_code:
        return "endxxalignat*";
        // special math
    case math_code: return "endmath";
    case displaymath_code:
        return "enddisplaymath";
        // inner math
    case array_code: return "endarray";
    case split_code: return "endsplit";
    case aligned_code: return "endaligned";
    case gathered_code: return "endgathered";
    case matrix_code: return "endmatrix";
    case bordermatrix_code: return "endbordermatrix "; // space ?
    case matrixb_code: return "endbmatrix";
    case matrixB_code: return "endBmatrix";
    case matrixp_code: return "endpmatrix";
    case matrixV_code: return "endVmatrix";
    case matrixv_code:
        return "endvmatrix";
        // special
    case mbox_S_code: return "endmbox";
    case text_S_code: return "endtext";
    case fbox_S_code: return "endfbox";
    case hbox_S_code: return "endhbox";
    default: return nullptr;
    }
}

// symbolic properties
// 1 means: external (otherwise internal)
// 2 means: one equation number (implies 1)
// 4 means: one equation number per row (implies 1)
// 8 means: array
// 16 has an optional ignored arg
// 32 has an required ignored

auto tralics_ns::math_env_props(subtypes chr) -> int {
    switch (chr) {
    case eqnarray_code:
    case Beqnarray_code:
    case gather_code:
    case align_code:
    case flalign_code: return 1 + 4 + 8;
    case eqnarray_star_code:
    case Beqnarray_star_code:
    case multline_star_code:
    case gather_star_code:
    case align_star_code:
    case flalign_star_code: return 1 + 8;
    case multline_code: return 1 + 2 + 8;
    case equation_code: return 1 + 2;
    case alignat_code:
    case xalignat_code:
    case xxalignat_code: return 1 + 4 + 8 + 32;
    case alignat_star_code:
    case xalignat_star_code:
    case xxalignat_star_code: return 1 + 8 + 32;
    case math_code:
    case displaymath_code:
    case equation_star_code: return 1;
    case array_code:
    case split_code:
    case matrix_code:
    case bordermatrix_code:
    case matrixB_code:
    case matrixp_code:
    case matrixV_code:
    case matrixv_code:
    case matrixb_code: return 8;
    case aligned_code:
    case gathered_code: return 8 + 16;
    default: return 0;
    }
}

// Name of a math object. In a previous version, the name was a character string.
// Now it is a subtype. The name has the form "endmath" and we remove the prefix
auto Math::get_name() const -> String {
    subtypes w = get_sname();
    if (w == nomathenv_code) return "";
    String S = tralics_ns::math_env_name(w);
    if (S != nullptr) return S + 3;
    return "bad";
}

// --------------------------------------------------

// Interprets the meta data of the list
auto operator<<(std::ostream &X, math_list_type y) -> std::ostream & {
    switch (y) {
    case invalid_cd: X << "Invalid list"; break;
    case math_open_cd: X << "simple group"; break;
    case math_dollar_cd: X << "Inline formula"; break;
    case math_ddollar_cd: X << "Display formula"; break;
    case math_env_cd: X << "Environment"; break;
    case math_LR_cd: X << "Left/right pair"; break;
    case math_hbox_cd: X << "Hbox"; break;
    case math_argument_cd: X << "Argument"; break;
    default: {
        CmdChr x(special_math_cmd, math_to_sub(y));
        X << "Argument list for \\" << x.name();
        break;
    }
    }
    return X;
}

auto operator<<(Buffer &X, math_list_type y) -> Buffer & {
    switch (y) {
    case invalid_cd: X << "Invalid list"; break;
    case math_open_cd: X << "Simple group"; break;
    case math_dollar_cd: X << "Inline formula"; break;
    case math_ddollar_cd: X << "Display formula"; break;
    case math_env_cd: X << "Environment"; break;
    case math_LR_cd: X << "Left/right pair"; break;
    case math_hbox_cd: X << "Hbox"; break;
    case math_argument_cd: X << "Argument"; break;
    default: {
        CmdChr x(special_math_cmd, math_to_sub(y));
        X << "Argument list for \\" << x.name();
        break;
    }
    }
    return X;
}

// Prints a list into the Trace buffer
void Math::print() const {
    Trace << "{" << type;
    if (sname == nomathenv_code)
        Trace << "\n";
    else
        Trace << " name= " << get_name() << "\n";
    if (empty())
        Trace << "empty";
    else {
        int  k = 0;
        auto C = value.begin();
        auto E = value.end();
        while (C != E) {
            k++;
            Trace << fmt::format("{} ", k);
            C->print();
            ++C;
        }
    }
    Trace << "}\n";
}

// This prints a math element
void MathElt::print() const {
    int cmd = get_cmd();
    int chr = get_chr();
    if (cmd == nomath_cmd) {
        Trace << "only for " << (chr == zero_code ? "math\n" : "nomath\n");
        return;
    }
    Trace << fmt::format("ME {} - ", cmd);
    if (32 < chr && chr < 128)
        Trace << fmt::format("char {}", uchar(chr));
    else
        Trace << fmt::format("{}", chr);
    // is this secure ???
    //  if(cmd>16) Trace << " - " <<  Token(get_font());
    if (cmd == mathfont_cmd || is_m_font(symcodes(cmd)))
        Trace << " - " << Token(get_font()) << "\n";
    else
        Trace << fmt::format(" - {}\n", get_font());
    if (cmd == math_list_cmd || cmd == special_math_cmd) get_list().print(); // recurse
}

// -----------------------------------------------------------------------
// Basic code generator

// General fence around val.
auto MathDataP::make_mfenced(size_t open, size_t close, gsl::not_null<Xml *> val) -> gsl::not_null<Xml *> {
    auto res = gsl::not_null{new Xml(cst_mfenced, nullptr)};
    res->add_att(the_names["close"], xml_lr_ptable[close]);
    res->add_att(the_names["open"], xml_lr_ptable[open]);
    bool single_elt = val->size() == 1;
    if (the_names["np_separator"] == the_names["mrow"]) {
        if (!single_elt) val = gsl::not_null{new Xml(cst_mrow, val)};
    } else if (the_names["np_separator"].empty() && single_elt) {
    } else
        res->add_att(cst_separators, np_separator);
    res->add_tmp(val);
    return res;
}

// This adds a style element above res.
auto MathDataP::add_style(int lvl, gsl::not_null<Xml *> res) -> gsl::not_null<Xml *> {
    if (lvl < 0) return res; // special case
    res = gsl::not_null{new Xml(cst_mstyle, res)};
    if (lvl == 0) {
        res->add_att(cst_displaystyle, np_true);
        res->add_att(cst_scriptlevel, cst_dig0);
    } else {
        res->add_att(cst_displaystyle, np_false);
        res->add_att(cst_scriptlevel, name_positions(cst_dig0 + lvl - 1));
    }
    return res;
}

// Implements \cellattribute
void MathHelper::add_attribute(const Istring &a, const Istring &b, subtypes c) {
    Xid w;
    if (c == cell_attribute_code)
        w = cur_cell_id;
    else if (c == row_attribute_code)
        w = cur_row_id;
    else if (c == table_attribute_code)
        w = cur_table_id;
    else if (c == thismath_attribute_code)
        w = cur_math_id;
    else if (c == formula_attribute_code)
        w = cur_formula_id;
    else
        return;
    w.add_attribute(a, b, true);
}

void math_ns::add_attribute_spec(const Istring &a, const Istring &b) { cmi.get_tid().add_attribute(a, b, true); }

// Adds a label to the formula X
void Parser::add_math_label(Xml *res) {
    if (cmi.get_eqnum_status() == 1) {
        cmi.ml_last_pass(tracing_math());
        if (the_tag.empty()) return;
    }
    Istring my_id = next_label_id();
    if (the_tag.empty()) {
        static int mid = 0;
        mid++;
        math_buffer << bf_reset << fmt::format("mid{}", mid);
        the_tag = math_buffer;
    }
    the_stack.create_new_anchor(res->id, my_id, Istring(the_tag));
    const std::string &label = cmi.get_label_val();
    if (!label.empty()) create_label(label, my_id);
}

// Generates <elt>first_arg second_arg</elt>
auto math_ns::xml2sons(Istring elt, gsl::not_null<Xml *> first_arg, gsl::not_null<Xml *> second_arg) -> Xml {
    Xml tmp(std::move(elt), nullptr);
    tmp.add_tmp(first_arg);
    tmp.push_back_unless_nullptr(xmlspace);
    tmp.add_tmp(second_arg);
    return tmp;
}

// As above, but if B1 is not empty, adds b1 as attribute with value true.
auto Stack::xml2_space(Istring elt, const Istring &b1, Xml *first_arg, Xml *second_arg) -> gsl::not_null<Xml *> {
    auto tmp = gsl::not_null{new Xml(std::move(elt), nullptr)};
    if (!b1.null()) tmp->add_att(b1, the_names["true"]);
    tmp->add_tmp(gsl::not_null{first_arg});
    tmp->push_back_unless_nullptr(xmlspace);
    tmp->add_tmp(gsl::not_null{second_arg});
    return tmp;
}

// -----------------------------------------------------------------------
// Some trivial functions on lists

// This reallocates the math_table
void MathDataP::realloc_list() {
    auto  k = 2 * lmath_size;
    Math *T = new Math[k];
    for (size_t i = 0; i < lmath_size; i++) T[i] = math_table[i];
    delete[] math_table;
    math_table = T;
    lmath_size = k;
    the_log << "Realloc math table to " << k << "\n";
}

// Makes sure there is enough place for two copies
void MathDataP::realloc_list0() {
    auto n = lmath_pos + 1;
    if (n + n + n > lmath_size) realloc_list();
    if (n + n + n > lmath_size) realloc_list();
}

// Appends the list X at the end, and destroys X.
void Math::push_front(Math &X) { value.splice(value.begin(), X.value); }

void Math::destroy() { value.clear(); }

// Adds X to the end of *this, and kills X.
void Math::push_back(Math &X) { value.splice(value.end(), X.value); }

auto MathElt::get_list() const -> Math & { return math_data.get_list(get_chr()); }

auto Math::get_list(int w) -> Math & { return math_data.get_list(w); }

// Adds a token to the list
void Math::push_back(CmdChr X, subtypes c) { push_back(MathElt(X, c)); }

void Math::push_back_list(subtypes X, math_list_type c) { push_back(MathElt(CmdChr(math_list_cmd, X), subtypes(c))); }

void Math::push_back_font(subtypes X, subtypes c) { push_back(MathElt(CmdChr(math_font_cmd, X), c)); }

// Adds a token to the list at position k
void MathDataP::push_back(int k, CmdChr X, subtypes c) { get_list(k).push_back(MathElt(X, c)); }

// Adds a token to the list
void Math::push_front(CmdChr X, subtypes c) { value.push_front(MathElt(X, c)); }

// Adds a character (cmd+chr). Uses current math font.
void Math::push_back(CmdChr X) {
    auto font = subtypes(the_parser.eqtb_int_table[math_font_pos].val);
    push_back(MathElt(X, font));
}

// Add an xml element. If b>=0, this element is at position b in the table.
void Math::push_back(Xml *A, int b, math_types c) { push_back(MathElt(A, b, c)); }

MathElt::MathElt(Xml *A, int b, math_types c) {
    subtypes pos{};
    if (b >= 0)
        pos = subtypes(b);
    else
        pos = math_data.find_xml_location(A);
    val = CmdChr(math_xml_cmd, pos);
    set_xmltype(c);
}

// Add an xml element, of type small, to the list.
void Math::push_back_small(Xml *A) {
    subtypes pos = math_data.find_xml_location(A);
    push_back(MathElt(pos, mt_flag_small));
}

// Returns the next style in the list D,T, S, SS.
// D and T give S, S gives SS, and SS gives SS.
// Knuth uses: 2*(x/4)+1+1 for sub,  2*(x/4)+1+ (x mod2) for sub
auto math_ns::next_math_style(math_style x) -> math_style {
    switch (x) {
    case ms_T:
    case ms_D: return ms_S;
    default: return ms_SS;
    }
}

// Knuth uses: x+2-2*(x/6)  for numerator x*(x/2)+1+2-2*(x/6) for denominator
auto math_ns::next_frac_style(math_style x) -> math_style {
    switch (x) {
    case ms_D: return ms_T;
    case ms_T: return ms_S;
    default: return ms_SS;
    }
}

// Takes the code of a command like \textstyle, returns ms_T
auto math_ns::style_level(subtypes tt) -> math_style {
    if (tt == displaystyle_code) return ms_D;
    if (tt == textstyle_code) return ms_T;
    if (tt == scriptstyle_code) return ms_S;
    if (tt == scriptscriptstyle_code) return ms_SS;
    return ms_T;
}

// ------------------------------------------------------------------

// This creates the two tables math_table and xml_math_table
void MathDataP::boot_table() {
    lmath_size = 10;
    math_table = new Math[lmath_size];
    xml_math_table.resize(10, nullptr);
    xmath_pos = 0;
    lmath_pos = 0;
}

inline auto MathElt::get_xml_val() const -> Xml * { return math_data.get_xml_val(get_chr()); }

// --------------------------------------------------------------------

// This is called before every math formula.
void MathHelper::reset(bool dual) {
    current_mode   = false;
    pos_att        = cst_dig0;
    seen_label     = false;
    warned_label   = false;
    label_val      = "";
    tag_list       = TokenList();
    is_tag_starred = false;
    math_env_ctr   = 0;
    all_env_ctr    = 0;
    cur_math_id    = the_main->the_stack->next_xid(nullptr);
    cur_formula_id = the_main->the_stack->next_xid(nullptr);
    if (dual)
        cur_texmath_id = the_main->the_stack->next_xid(nullptr);
    else
        cur_texmath_id = cur_math_id;
    multi_labels.clear();
    multi_labels_type.clear();
    last_ml_pos = 0;
}

// Sets the type (display or not), and prepares the pos attribute.
void MathHelper::set_type(bool b) {
    current_mode = b;
    pos_att      = b ? np_inline : np_display;
}

// This finds a free position in the table of math lists.
// Note: the return value is never zero.
auto MathDataP::find_math_location(math_list_type c, subtypes n) -> subtypes {
    lmath_pos++;
    auto res = lmath_pos;
    if (res >= lmath_size) realloc_list();
    math_table[res].set_type(c);
    math_table[res].set_name(n);
    return subtypes(res);
}

// This finds a free position in the xml table.
// Note that there is a free position in case of overflow (see next function)
auto MathDataP::find_xml_location() -> subtypes {
    xmath_pos++;
    if (xmath_pos >= xml_math_table.size()) xml_math_table.resize(xmath_pos + 1, nullptr);
    return subtypes(m_offset + xmath_pos - 1);
}

// This finds a free position, puts Y in it, and returns the position.
auto MathDataP::find_xml_location(Xml *y) -> subtypes {
    xml_math_table[xmath_pos] = y;
    return find_xml_location();
}

MathElt::MathElt(Xml *x, math_types y) : val(CmdChr(math_xml_cmd, zero_code)) {
    set_xmltype(y);
    set_chr(math_data.find_xml_location(x));
}

// Destroys all math lists, and resets pointers,
// for use with another math object
void MathHelper::finish_math_mem() { math_data.finish_math_mem(); }

// This kills the math elements
void MathDataP::finish_math_mem() {
    for (size_t i = 0; i <= lmath_pos; i++) math_table[i].destroy();
    lmath_pos = 0;
    xmath_pos = 0;
}

// True if this is a digit with category 12
auto MathElt::is_digit() const -> bool { return get_cmd() == 12 && val.is_digit(); }

// If we see something like {} in a math formula, say cur_math,
// we open a group, create a new mathlist res, add it to cur_math.
// Note that the type c appears both as type of res, and a field of cur_math

auto Parser::new_math_list(int cur_math, math_list_type c, subtypes s) -> subtypes {
    subtypes k = math_data.find_math_location(c, s);
    math_data.get_list(cur_math).push_back_list(k, c);
    scan_math3(k, c, 1);
    return k;
}

// This reads a character (delimiter) after a \left or a \right.
auto Parser::math_lr_value() -> del_pos {
    remove_initial_space_relax();
    add_to_trace(cur_tok);
    del_pos k = get_delimiter(cur_cmd_chr);
    if (k != del_invalid) return k;
    parse_error(err_tok, "Invalid character in \\left or \\right\n", cur_tok, "", "bad left/right");
    return del_dot;
}

// This is done to the main math list U when we start reading a formula.

// Second argument is nomathenv_code if a dollar has been read,
// the subtype of the token otherwise;
// Note: \( and \[ expand to $$, in all other cases we have an environment
// for instance \begin{math} or \begin{equation}. We compute and insert
// the name of the environment.
// Sets and returns the inline/non-inline flag

auto Parser::start_scan_math(Math &u, subtypes type) -> bool {
    u.set_name(type);
    if (type == nomathenv_code) { // case of $...
        add_to_trace(cur_tok);
        get_token(); // no expansion
        if (cur_cmd_chr.cmd == 3) {
            add_to_trace(cur_tok);
            u.set_display_type();
            return false;
        }
        if (cur_tok != hash_table.relax_token) back_input();
        u.set_nondisplay_type();
        return true;
    }
    // case of \begin{...}
    u.set_type(math_env_cd);
    if ((math_env_props(type) & 1) == 0) {
        math_buffer << bf_reset;
        math_buffer << "Environment " << u.get_name() << " should only be used in math mode";
        parse_error(math_buffer);
    }
    if ((math_env_props(type) & 16) != 0) ignore_optarg();
    if ((math_env_props(type) & 32) != 0) read_arg();
    return type == math_code;
}

// Defines how many equation numbers are to be created
// If multi is true, more than one is allowed
void MathHelper::check_for_eqnum(subtypes type, bool multi) {
    int fl = math_env_props(type);
    if ((fl & 2) != 0)
        eqnum_status = 1;
    else if ((fl & 4) != 0)
        eqnum_status = multi ? 2 : 3;
    else
        eqnum_status = 0;
}

// This piece of code is executed when the math formula has been read
// and translated. If the formula is inline, we execute \leavevmode
// Otherwise, it is a display math formula, we finish the current paragraph
// and look at the next (expanded) token. If it is no \par, we insert
// a \noindent.
void Parser::after_math(bool is_inline) {
    cmi.finish_math_mem();
    if (is_inline)
        leave_v_mode();
    else if (the_stack.is_frame(np_fbox))
        return;
    else {
        bool w = the_stack.in_h_mode() || the_stack.in_v_mode();
        leave_h_mode();
        if (the_main->interactive_math) return; // no need to be subtle
        remove_initial_space_and_back_input();
        if (w && cur_cmd_chr.cmd != par_cmd) back_input(hash_table.noindent_token);
    }
}

// This is called if no MathML should be generated.
void Parser::finish_no_mathml(bool is_inline, int vp) {
    Math &      u  = math_data.get_list(vp);
    Xid         id = cmi.get_mid();
    std::string S  = u.get_name();
    auto        s  = Istring(S);
    if (S.empty()) s = the_names[is_inline ? "inline" : "display"];
    id.add_attribute(np_type, cmi.get_pos_att());
    id.add_attribute(np_textype, s);
    Xml *res = u.convert_math_noML(eqtb_int_table[nomath_code].val == -2);
    res->id  = id;
    if (the_main->interactive_math) std::cout << res << "\n";
    after_math(is_inline);
    the_stack.top_stack()->push_back_unless_nullptr(res);
    if (cmi.has_label()) add_math_label(res);
    if (!is_inline) the_stack.add_nl();
}

// Case of a trivial math formula that translates to res
// Always inline
void Parser::finish_trivial_math(Xml *res) {
    if (tracing_math()) {
        Logger::finish_seq();
        the_log << "formula was math\n";
    }
    the_parser.my_stats.one_more_trivial();
    if (the_main->interactive_math) std::cout << res << "\n";
    leave_v_mode();
    cmi.finish_math_mem();
    the_stack.top_stack()->add_tmp(gsl::not_null{res});
}

// Needed for implementation of \ifinner
auto Parser::is_inner_math() -> bool { return cmi.is_inline(); }

// Toplevel function. Reads and translates a formula.
// Argument as in start_scan_math
void Parser::T_math(subtypes type) {
    auto nm = eqtb_int_table[nomath_code].val;
    cmi.reset(nm == -3);
    Trace.clear();
    trace_needs_space = false;
    Math &u1          = math_data.get_list(0);
    bool  is_inline   = start_scan_math(u1, type);
    cmi.set_type(is_inline);
    cmi.check_for_eqnum(type, eqtb_int_table[multimlabel_code].val != 0);
    if (type == nomathenv_code || type == math_code || type == displaymath_code) {
        size_t    position  = is_inline ? everymath_code : everydisplay_code;
        TokenList everymath = toks_registers[position].val;
        if (!everymath.empty()) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{" << (is_inline ? "<everymath> " : "<everydisplay> ") << everymath << "}\n";
            }
            back_input(everymath);
        }
    }
    select_math_font();
    scan_math3(0, math_data.get_list(0).get_type(), 0);
    if (tracing_math()) {
        Logger::finish_seq();
        the_log << "Math: " << Trace << "\n";
        Trace.clear();
        math_data.get_list(0).print();
        Logger::finish_seq();
        the_log << Trace;
    }
    // Test for the no-mathml mode
    math_data.realloc_list0();
    int loc_of_cp = 0;
    if (nm < 0) {
        loc_of_cp = math_data.get_list(0).duplicate(true);
        if (nm != -3) {
            finish_no_mathml(is_inline, loc_of_cp);
            return;
        }
    }
    Xml *       alter{nullptr};
    std::string textype = math_data.get_list(0).get_name();
    if (nm == -3) {
        Math &w   = math_data.get_list(loc_of_cp);
        alter     = w.convert_math_noML(false);
        alter->id = cmi.get_tid();
    }
    loc_of_cp = math_data.get_list(0).duplicate(false);
    Math &u   = math_data.get_list(loc_of_cp);
    // Translate the formula into res
    Xml *res{nullptr};
    if ((math_env_props(type) & 8) == 0) u.set_type(math_open_cd);
    if (u.has_type(math_env_cd)) {
        res = u.M_array(cmi.get_eqnum_status() == 2, is_inline ? ms_T : ms_D);
    } else {
        u.remove_spaces();
        if (is_inline) {
            auto k = eqtb_int_table[notrivialmath_code].val;
            res    = u.trivial_math(k);
            if (res != nullptr) {
                finish_trivial_math(res);
                return;
            }
        }
        res = u.convert_math(is_inline ? ms_T : ms_D);
    }
    after_math(is_inline);
    // Insert the result in the tree.
    Xml *x = new Xml(cst_math, nullptr);
    x->id  = cmi.get_mid();
    x->add_att(cst_xmlns, cst_mathml);
    x->add_tmp(gsl::not_null{res});
    if (!is_inline) x->add_att(cst_mode, cst_display);
    Xml *res1 = new Xml(np_formula, x);
    if (alter != nullptr) res1->push_back_unless_nullptr(alter);

    res1->id = cmi.get_fid();
    res1->add_att(np_type, cmi.get_pos_att());
    if (!textype.empty()) res1->add_att(the_names["textype"], Istring(textype));
    if (cmi.has_label()) add_math_label(res1);
    if (the_main->interactive_math) {
        if (only_input_data)
            std::cout << x << "\n";
        else
            std::cout << res1 << "\n";
    }
    the_stack.add_last(res1);
    if (!is_inline) the_stack.add_nl();
}

// --------------------------------------------------
// Scanning math

// x is index of result, t is type of math list,
// m is 0 for outer math, 1 normally, 2 in \hbox
void Parser::scan_math3(int x, math_list_type t, int m) {
    Token xfct_caller = fct_caller;
    fct_caller        = hash_table.relax_token;
    mode om           = the_stack.get_mode();
    if (t == math_open_cd) {
    } // do not change the mode
    else if (m == 2)
        the_stack.set_h_mode();
    else
        the_stack.set_m_mode();
    boundary_type aux = bt_math;
    {
        Math &u = math_data.get_list(x);
        if ((math_env_props(u.get_sname()) & 8) != 0)
            aux = bt_cell;
        else if (t == math_open_cd)
            aux = bt_brace;
    }
    push_level(aux);
    if (m == 0 && cmi.get_eqnum_status() == 3) refstepcounter("equation", false);
    scan_math(x, t);
    if (m == 0 && (cmi.get_eqnum_status() == 2 || cmi.get_eqnum_status() == 1)) {
        if (!cmi.end_of_row()) {
            refstepcounter("equation", false);
            cmi.insert_special_tag(the_parser.eqtb_string_table[0].val);
        }
        if (tracing_math()) cmi.dump_labels();
        cmi.ml_check_labels();
    }
    pop_level(aux);
    the_stack.set_mode(om);
    fct_caller = xfct_caller;
}

// nnewt item in the list of tags
void MathHelper::add_tag(TokenList &L) {
    if (!tag_list.empty()) tag_list.push_back(the_parser.hash_table.comma_token);
    tag_list.splice(tag_list.end(), L);
}

// passes the list of tag as argument to \x@tag or \y@tag
void MathHelper::handle_tags() {
    TokenList L = tag_list;
    token_ns::remove_first_last_space(L);
    the_parser.brace_me(L);
    L.push_front(is_tag_starred ? the_parser.hash_table.ytag1_token : the_parser.hash_table.xtag1_token);
    the_parser.back_input(L);
    tag_list = TokenList();
}

// Reads a new token.
// Return  0 if end-of data, 1 if token should be ignored, 2 otherwise
auto Parser::scan_math1(int res) -> int {
    get_x_token();
    if (cur_tok.is_invalid()) {
        parse_error("End of input reached");
        return 0;
    }
    symcodes T = cur_cmd_chr.cmd;
    if (T == nobreakspace_cmd) {
        back_input(Token(other_t_offset, '~'));
        return 1;
    }
    // replace \{ by \lbrace
    if (T == cst1_cmd && cur_cmd_chr.chr == rbrace_chr) {
        back_input(hash_table.rbrace_token);
        return 1;
    }
    if (T == cst1_cmd && cur_cmd_chr.chr == lbrace_chr) {
        back_input(hash_table.lbrace_token);
        return 1;
    }
    if (T == 3) { // dollar sign hack a bit this
        if (cmi.has_tag()) {
            back_input(Token(dollar_t_offset, '$'));
            cmi.handle_tags();
            return 1;
        }
    }
    if (T == 11 || T == 12 || T == char_given_cmd) {
        // FIXME what is the purpose of this math mode test
        if (the_stack.get_mode() == mode_math) {
            auto c = cur_cmd_chr.chr;
            if (c > 0 && c < nb_characters) {
                auto u = eqtb_int_table[c + math_code_offset].val;
                if (u == 32768) {
                    cur_tok.active_char(c);
                    back_input();
                    return 1;
                }
            }
        }
    }
    if (is_m_font(T)) {
        if (eqtb_int_table[nomath_code].val == -2) {
            math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
            return 1;
        }
        math_data.push_back(res, CmdChr(nomath_cmd, one_code), zero_code);
        math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
        TM_fonts();
        return 1;
    }
    add_to_trace(cur_tok);
    return 2;
}

// The function that reads a math formula. Read tokens until the end
// of the current group is seen. Fills the list number res, of type type
void Parser::scan_math(int res, math_list_type type) {
    for (;;) {
        int w = scan_math1(res);
        if (w == 0) return;
        if (w == 1) continue;
        symcodes T = cur_cmd_chr.cmd;
        subtypes c = cur_cmd_chr.chr;
        Token    t = cur_tok;
        switch (T) {
        case 1: // open brace, read a group
            new_math_list(res, math_open_cd, nomathenv_code);
            continue;
        case 2: // close brace, end group
            if (type == math_open_cd || type == math_argument_cd || type == math_hbox_cd) return;
            parse_error("Unexpected }");
            return;
        case 3: // dollar, open or close ?
            if (scan_math_dollar(res, type)) return;
            continue;
        case par_cmd:
            err_buf << bf_reset << "Unexpected \\par";
            if (type == math_argument_cd) err_buf << " while scanning argument of " << fct_caller.tok_to_str();
            signal_error(err_tok, "Unexpected par");
            return;
        case eqno_cmd: scan_eqno(type); continue;
        case tag_cmd: scan_math_tag(c); continue;
        case label_cmd:
            if (c == 1) { // anchor
                remove_initial_star();
                sT_optarg_nopar();
                parse_error("Illegal anchor in math mode");
                continue;
            } else if (c == 2) { // anchorlabel
                bool        h = 0;
                std::string s = scan_anchor(h);
                std::string a = special_next_arg();
                if (h) {
                    c           = mathlabel_code;
                    TokenList L = token_ns::string_to_list(a, true);
                    // brace_me(L);
                    back_input(L);
                    TokenList L1 = token_ns::string_to_list(s, true);
                    // brace_me(L1);
                    back_input(L1);
                    math_list_type cc = sub_to_math(c);
                    Token          ct = cur_tok;
                    subtypes       k  = math_data.find_math_location(cc, nomathenv_code);
                    CmdChr         W  = CmdChr(special_math_cmd, k);
                    math_data.set_type(k, cc);
                    subtypes r1 = math_argument(0, ct); // should be s
                    subtypes r2 = math_argument(0, ct); // should be a
                    Math &   u  = math_data.get_list(k);
                    u.push_back_list(r1, math_argument_cd);
                    u.push_back_list(r2, math_argument_cd);
                    math_data.push_back(res, W, subtypes(u.get_type()));
                } else {
                    the_tag = s;
                    cmi.new_label(a, true);
                }
                continue; // case anchorlabel ??
            }
            {
                std::string s = special_next_arg();
                cmi.new_label(s, false);
            }
            continue;
        case ensuremath_cmd: E_all_of_one(t, zero_code); continue;
        case begingroup_cmd:
            if (c == 0)
                push_level(bt_semisimple);
            else
                pop_level(bt_semisimple);
            continue;
        case begin_cmd:
        case end_cmd:
            if (scan_math_env(res, type)) return;
            continue;
        case left_cmd: {
            del_pos k   = math_lr_value();
            int     tmp = new_math_list(res, math_LR_cd, nomathenv_code);
            Math &  ww  = math_data.get_list(tmp);
            ww.push_front(CmdChr(T, subtypes(k)), zero_code);
            if (ww.back().get_cmd() != right_cmd) {
                parse_error("Missing \\right. inserted");
                ww.push_back(CmdChr(right_cmd, subtypes(del_dot)), zero_code);
            }
            continue;
        }
        case right_cmd: {
            del_pos k = math_lr_value();
            if (type == math_LR_cd) {
                math_data.get_list(res).push_back(CmdChr(right_cmd, subtypes(k)));
                return;
            }
            parse_error("Unexpected \\right");
            continue;
        }
        case special_math_cmd: interpret_math_cmd(res, c); continue;
        case multicolumn_cmd: interpret_math_cmd(res, multicolumn_code); continue;
        case specmath_cmd: // Tralics commands like \mathmi
            interpret_math_cmd(res, c);
            continue;
        case make_box_cmd:
            if (c == hbox_code)
                scan_math_hbox(res, hbox_S_code);
            else
                math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
            continue;
        case fbox_cmd:
            if (c == fbox_code)
                scan_math_hbox(res, fbox_S_code);
            else
                math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
            continue;
        case box_cmd:
            if (c == text_code)
                scan_hbox(res, text_S_code);
            else
                scan_hbox(res, mbox_S_code); // what if it is makebox ? [FIXME]
            continue;
        case alignment_catcode: // case & and \\ in a table
        case backslash_cmd:
            if (stack_math_in_cell()) {
                if (scan_math_endcell(t)) return;

                continue;
            }
            scan_math_endcell_ok(res);
            continue;
        case mathfont_cmd:
            math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
            word_define(math_font_pos, c, false);
            continue;
        case undef_cmd: undefined_mac(); continue;
        case mathrel_cmd: scan_math_rel(c, res); continue;
        case unimp_cmd: T_unimp(c); continue;
        case specimp_cmd:
            if (c == allowbreak_code) continue;
            T_specimp(c);
            continue;
        case kern_cmd:
        case scan_glue_cmd:
        case hspace_cmd: {
            ScaledInt val = scan_math_kern(T, c);
            CmdChr    ww  = CmdChr(hspace_cmd, c);
            math_data.push_back(res, ww, subtypes(val.value));
            continue;
        }
        case start_par_cmd:
            // no error ?
            continue;
        case char_num_cmd: {
            auto   C  = scan_27bit_int();
            CmdChr ww = CmdChr(char_given_cmd, subtypes(C));
            math_data.push_back(res, ww, subtypes(0));
        }
            continue;
        case char_given_cmd:
        case math_given_cmd: math_data.push_back(res, cur_cmd_chr, subtypes(0)); continue;
        case relax_cmd: math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val)); continue;
        case mathord_cmd: // may be bold
        {
            auto   F = subtypes(cur_tok.val);
            CmdChr R = cur_cmd_chr;
            if (math_loc(c) > first_inline_hack && math_loc(c) < last_inline_bhack && is_pos_par(atmathversion_code))
                R = CmdChr(mathordb_cmd, c);
            math_data.push_back(res, R, F);
            continue;
        }
        default:
            if (T == 12 && c == '\'' && type != math_hbox_cd) {
                if (!Trace.empty() && Trace.back() == '\'') Trace.remove_last();
                scan_prime();
                continue;
            }
            if (T >= first_mode_independent) {
                remove_from_trace();
                translate01();
                continue;
            }
            if (T < 16 && cur_tok.not_a_cmd()) {
                // in the case \let\A=B, we replace \A by B (avoids core dump)
                cur_cmd_chr = CmdChr(T, c);
            }
            if (T < 16) { // Case of a character
                auto font = subtypes(eqtb_int_table[math_font_pos].val);
                math_data.push_back(res, cur_cmd_chr, font);
            } else
                math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
        }
    }
}

// We have seen & or \\. Possibly inserts v-part
auto Parser::scan_math_endcell(Token t) -> bool {
    if (the_stack.is_frame(np_cell) && !the_stack.is_omit_cell()) {
        TokenList L = the_stack.get_u_or_v(false);
        if (!L.empty()) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{template v-part " << L << "}\n";
            }
            back_input(t);
            back_input(L);
            the_stack.mark_omit_cell();
            return false;
        }
    }
    parse_error(err_tok, "unexpected command ", t, "; math mode aborted", "bad math");
    back_input(t);
    return true; // bad
}

// debug
void MathHelper::dump_labels() {
    auto n = multi_labels.size();
    for (size_t i = 0; i < n; i++) {
        int v = multi_labels_type[i];
        if (v == 0)
            the_log << "\n";
        else if (v == 4)
            the_log << "notag,";
        else if (v == 1)
            the_log << "label " << multi_labels[i] << ",";
        else if (v == -1)
            the_log << "bad label " << multi_labels[i] << ",";
        else if (v == 2)
            the_log << "tag " << multi_labels[i] << ",";
        else if (v == 3)
            the_log << "tag* " << multi_labels[i] << ",";
        else if (v == -2)
            the_log << "bad tag " << multi_labels[i] << ",";
        else if (v == -3)
            the_log << "bad tag* " << multi_labels[i] << ",";
        else if (v == 5)
            the_log << "eqno " << multi_labels[i] << ",";
    }
}

//
void MathHelper::ml_check_labels() {
    auto        n      = multi_labels.size();
    int         l      = 1;
    Buffer &    B      = math_buffer;
    static bool warned = false;
    for (size_t i = 0; i < n; i++) {
        int v = multi_labels_type[i];
        if (v == 0)
            l++;
        else if (v == -1) {
            B << bf_reset << "Multiple \\label " << multi_labels[i];
            if (eqnum_status == 1)
                B << " for the current the formula";
            else {
                B << fmt::format(" on row {} of the formula", l);
                if (!warned) B << "\n(at most one \\label and at most one \\tag allowed per row)";
                warned = true;
            }
            the_parser.parse_error(the_parser.err_tok, B, "duplicate label");
        }
        if (v == -2 || v == -3) {
            B << bf_reset << "Multiple \\tag " << multi_labels[i];
            if (eqnum_status == 1)
                B << " for the current formula";
            else {
                B << fmt::format(" on row {} of formula", l);
                if (!warned) B << "\n(at most one \\label and at most one \\tag allowed per row)";
                warned = true;
            }
            the_parser.parse_error(the_parser.err_tok, B, "duplicate tag");
        }
    }
}

// true if no equation number has to be produced
// leaves a hole for the label
auto MathHelper::end_of_row() -> bool {
    auto k           = last_ml_pos;
    auto n           = multi_labels.size();
    bool sseen_label = false;
    bool seen_notag  = false;
    bool seen_tag    = false;
    for (size_t i = k; i < n; i++) {
        int v = multi_labels_type[i];
        if (v == 0) continue; // boundary
        if (v == 1) {         // a label
            if (sseen_label) { multi_labels_type[i] = -v; }
            sseen_label = true;
        } else if (v == 2 || v == 3) { // a tag
            if (seen_tag) { multi_labels_type[i] = -v; }
            seen_tag = true;
        } else if (v == 4)
            seen_notag = true;
    }
    bool ok = seen_tag || seen_notag;
    if (seen_notag && sseen_label && !seen_tag) ok = false; // ??
    if (!ok) new_multi_label("", 5);
    new_multi_label("", 0); // add a new boundary
    last_ml_pos = multi_labels.size();
    return ok;
}

void MathHelper::ml_second_pass(Xml *row, bool vb) {
    bool        slabel = false, stag = false;
    std::string label;
    std::string tag;
    auto        n = multi_labels.size();
    size_t      i = 0;
    static int  N = 0;
    if (last_ml_pos == 0) N = 0;
    N++;
    for (;;) {
        if (last_ml_pos >= n) break;
        i = last_ml_pos;
        last_ml_pos++;
        int j = (multi_labels_type[i]);
        if (j == 0) break;
        if (j == 1) {
            slabel = true;
            label  = multi_labels[i];
        } else if (j == 2 || j == 3 || j == 5) {
            stag = true;
            tag  = multi_labels[i];
        }
    }
    if (vb) {
        if (slabel) the_log << "label on row " << N << " " << label << ".\n";
        if (stag) the_log << "tag on row " << N << " " << tag << ".\n";
    }
    if (stag) {
        Istring id = next_label_id();
        the_parser.the_stack.create_new_anchor(row->id, id, Istring(tag));
        if (slabel) the_parser.create_label(label, id);
    } else if (slabel)
        the_parser.parse_error("Internal error");
}

void MathHelper::ml_last_pass(bool vb) {
    bool        slabel = false, stag = false;
    std::string label;
    std::string tag;
    auto        n = multi_labels.size();
    for (size_t i = 0; i < n; i++) {
        int j = (multi_labels_type[i]);
        if (j == 0) continue;
        if (j == 1) {
            slabel = true;
            label  = multi_labels[i];
        } else if (j == 2 || j == 3 || j == 5) {
            stag = true;
            tag  = multi_labels[i];
        }
    }
    if (slabel) label_val = label;
    the_tag = tag;
    if (vb) {
        the_log << "Check labels \n";
        dump_labels();
        if (slabel) the_log << "label for formula " << label << ".\n";
        if (stag) the_log << "tag for formula " << tag << ".\n";
    }
}

// We have seen & or \\. Must interpret it
void Parser::scan_math_endcell_ok(int res) {
    math_data.push_back(res, cur_cmd_chr, subtypes(cur_tok.val));
    if (cur_cmd_chr.cmd == backslash_cmd && res == 0 && cmi.get_eqnum_status() == 2) {
        bool w = cmi.end_of_row();
        if (!w) {
            refstepcounter("equation", false);
            cmi.insert_special_tag(the_parser.eqtb_string_table[0].val);
        }
    }
    pop_level(bt_cell); // start-end a group for the next cell
    push_level(bt_cell);
}

// We have seen \begin or \end
// return false if parsing continues, true otherwise (case of \end)
auto Parser::scan_math_env(int res, math_list_type type) -> bool {
    symcodes T        = cur_cmd_chr.cmd;
    bool     at_start = T == begin_cmd;
    // Check if v-part of template has to be inserted here
    if (!at_start && stack_math_in_cell()) {
        if (the_stack.is_frame(np_cell) && !the_stack.is_omit_cell()) {
            TokenList L = the_stack.get_u_or_v(false);
            if (!L.empty()) {
                if (tracing_commands()) {
                    Logger::finish_seq();
                    the_log << "{template v-part " << L << "}\n";
                }
                back_input(cur_tok);
                back_input(L);
                the_stack.mark_omit_cell();
                return false;
            }
        }
    }
    cmi.update_all_env_ctr(at_start);
    auto s = fetch_name0();
    add_to_trace(s);
    Token  eenv    = find_env_token(s, false); // \endfoo
    CmdChr end_val = cur_cmd_chr;              // value of \endfoo
    Token  benv    = find_env_token(s, true);  // \foo
    if (end_val.is_user()) {                   // user defined env, expand
        if (at_start) {
            back_input(benv);
            back_input(hash_table.OB_token); // Insert OB
            cmi.update_math_env_ctr(true);
        } else { // end something
            if (cmi.get_math_env_ctr() > 0) {
                back_input(hash_table.CB_token); // matches the OB above
                cmi.update_math_env_ctr(false);
                back_input(eenv);
            } else {
                // FIXME
                T_end(s);
                return false;
            }
        }
        return false;
    }
    // non-user env
    subtypes et = nomathenv_code;
    if (cur_cmd_chr.cmd == math_env_cmd) et = cur_cmd_chr.chr;
    if (at_start) {
        if (et == nomathenv_code || ((math_env_props(et) & 1) != 0)) {
            Buffer &B = math_buffer;
            B << bf_reset << "Illegal \\begin{" << s << "} found in math mode";
            parse_error(err_tok, B, "bad env");
        }
        if ((math_env_props(et) & 16) != 0) ignore_optarg();
        new_math_list(res, math_env_cd, et);
        return false;
    }
    // Case \end{foo}
    bool at_level_zero = cmi.get_all_env_ctr() == -1;
    if (at_level_zero && cmi.has_tag()) {
        TokenList L = token_ns::string_to_list(s, true);
        back_input(L);
        back_input(hash_table.end_token);
        cmi.handle_tags();
        return false;
    }
    if (type == math_env_cd && et == math_data.get_list(res).get_sname()) {
        if (res == 0) { // end of main formula
            if (cmi.get_eqnum_status() > 1) the_tag = the_parser.eqtb_string_table[0].val;
        }
        return true;
    }
    {
        Buffer &B = math_buffer;
        B << bf_reset << "Bad \\end{" << s << "}; expected ";
        if (type == math_env_cd)
            B << "\\end{" << math_data.get_list(res).get_name() << "}";
        else if (type == math_dollar_cd)
            B << "$";
        else if (type == math_ddollar_cd)
            B << "$$";
        else if (type == math_LR_cd)
            B << "\\right";
        else
            B << "}";
        parse_error(err_tok, B, "bad end");
    }
    return true;
}

// We have seen a dollar sign, expected closing delimiter type
// Returns  true if Ok, false if parsing continues
auto Parser::scan_math_dollar(int res, math_list_type type) -> bool {
    switch (type) {
    case math_dollar_cd: return true;
    case math_ddollar_cd: // We want $$ or equivalent
        get_x_token();
        add_to_trace(cur_tok);
        if (cur_cmd_chr.cmd != 3) parse_error("Display math should end with $$");
        return true;
    case math_open_cd:
        if (!the_stack.in_h_mode()) {
            parse_error(err_tok, "Out of scope $ ignored, maybe a } is missing here", "extra $");
            return false;
        }
        // fall through
    case math_hbox_cd: {
        // it's a math formula inside a formula
        TokenList everymath = toks_registers[everymath_code].val;
        if (!everymath.empty()) {
            if (tracing_commands()) {
                Logger::finish_seq();
                the_log << "{<everymath> " << everymath << "}\n";
            }
            back_input(everymath);
        }
        select_math_font();
        new_math_list(res, math_dollar_cd, nomathenv_code);
        return false;
    }
    case math_LR_cd:
        back_input();
        parse_error(err_tok, "Missing \\right. inserted", "missing \\right");
        math_data.get_list(res).push_back(CmdChr(right_cmd, subtypes(del_dot)));
        return true;
    case math_argument_cd: parse_error(err_tok, "Extra $ ignored while scanning argument of ", fct_caller, "", "extra $"); return false;
    default: parse_error("Extra $ ignored..."); return false;
    }
}

// We have seen \tag, \@xtag o<r \@ytag
// 0: \tag, 1=\@xtag 2=\@ytag 3=\notag 4=\nonumber
void Parser::scan_math_tag(subtypes c) {
    if (c == 3 || c == 4) {
        if (cmi.get_eqnum_status() == 2 || cmi.get_eqnum_status() == 1) {
            cmi.new_multi_label("", 4);
        } else if (cmi.get_eqnum_status() == 0) {
            parse_error("Illegal \\notag");
        } else { // decrement equation number
            auto v = eqtb_int_table[equation_ctr_pos].val;
            word_define(equation_ctr_pos, v - 1, true);
        }
        return;
    }
    bool is_star = false;
    if (c == 0) is_star = remove_initial_star();
    TokenList L = read_arg();
    if (c == 0 && (cmi.get_eqnum_status() == 2 || cmi.get_eqnum_status() == 1)) {
        L.remove_if([](const Token &m) { return m.is_math_shift(); });
        L.push_front(hash_table.relax_token);
        L.push_front(hash_table.ref_token);
        L.push_front(hash_table.let_token);
        brace_me(L);
        std::string val = sT_translate(L);
        cmi.new_multi_label(val, (is_star ? 3 : 2));
        return;
    }
    if (c == 0 && (cmi.get_eqnum_status() == 0)) {
        parse_error("Illegal \\tag");
        return;
    }
    if (c == 0) {
        brace_me(L);
        L.remove_if([](const Token &m) { return m.is_math_shift(); });
        L.push_front(is_star ? hash_table.ytag_token : hash_table.xtag_token);
        back_input(L);
    } else {
        if (c == 2) cmi.starred_tag();
        cmi.add_tag(L);
    }
}

// Scans the arguments of \eqno; interprets \char`\^
void Parser::scan_eqno(math_list_type type) {
    if (type != math_ddollar_cd) {
        parse_error(err_tok, "Command \\eqno allowed only in display math", "bad \\eqno");
        return;
    }
    name_positions w = cur_cmd_chr.chr == leqno_code ? np_left : np_right;
    cmi.get_fid().add_attribute(np_eqnpos, w, true);
    TokenList L;
    int       balance = 0;
    for (;;) {
        get_x_token();
        if (cur_tok.is_invalid()) {
            parse_error("End of input reached while scanning \\eqno");
            return;
        }
        if (cur_cmd_chr.cmd == char_num_cmd) {
            auto C  = scan_char_num();
            cur_tok = Token(other_t_offset, static_cast<uchar>(C));
            back_input();
            continue;
        }
        if (cur_cmd_chr.cmd == 12 && cur_cmd_chr.chr == '\'') cur_tok = hash_table.apostrophe_token;
        check_brace(balance);
        if (balance == 0 && cur_cmd_chr.cmd == 3) break;
        L.push_back(cur_tok);
    }
    back_input();
    brace_me(L);
    L.push_front(hash_table.ytag_token);
    back_input(L);
}

// Case of a \kern, or something like that. We scan the value, convert it
// in pt.
auto Parser::scan_math_kern(symcodes T, subtypes &c) -> ScaledInt {
    ScaledInt value = 0;
    bool      is_mu = false;
    Token     ct    = cur_tok;
    if (T == kern_cmd) {
        is_mu = c == 1;
        c     = zero_code;
        scan_dimen(is_mu, ct);
        value = cur_val.get_int_val();
    } else if (T == hspace_cmd) {
        if (remove_initial_star()) c = subtypes(c + 2);
        scan_glue(it_glue, ct, false);
        value = cur_val.get_glue_width();
    } else {
        is_mu = c == mskip_code;
        if (c == vskip_code)
            c = one_code;
        else
            c = zero_code;
        scan_glue(is_mu ? it_mu : it_glue, ct);
        value = cur_val.get_glue_width();
    }
    if (is_mu) value.times_10_18();
    return value;
}

// Case of a \not\in read as \notin, \not= read as \ne
// What should we do in other cases?
void Parser::scan_math_rel(subtypes c, int res) {
    auto w = subtypes(cur_tok.val);
    if (c == subtypes(not_code)) {
        Token not_token = cur_tok;
        get_x_token();
        if (cur_tok.is_invalid()) {
        } else if (cur_tok.not_a_cmd() && cur_cmd_chr.chr == '=') {
            add_to_trace(cur_tok);
            cur_cmd_chr.set(mathrel_cmd, subtypes(ne_code));
            w = subtypes(not_token.val);
        } else if (cur_cmd_chr.cmd == mathbin_cmd && cur_cmd_chr.chr == subtypes(in_code)) {
            add_to_trace(cur_tok);
            cur_cmd_chr.set(mathbin_cmd, subtypes(notin_code));
            w = subtypes(not_token.val);
        } else {
            back_input();
            cur_tok = not_token;
            cur_cmd_chr.set(mathord_cmd, subtypes(not_code));
        }
    }
    math_data.push_back(res, cur_cmd_chr, w);
}

// Case of a \hbox; like \mbox, but inserts \everyhbox tokens
void Parser::scan_math_hbox(int res, subtypes c) {
    TokenList L = toks_registers[everyhbox_code].val;
    if (!L.empty()) {
        if (before_mac_arg()) back_input(hash_table.CB_token);
        ;
        if (tracing_commands()) {
            Logger::finish_seq();
            the_log << "{<everyhbox> " << L << "}\n";
        }
        back_input(L);
        back_input(hash_table.OB_token);
    }
    scan_hbox(res, c);
}

// Scans a mbox or a hbox
void Parser::scan_hbox(int ptr, subtypes c) {
    if (before_mac_arg()) back_input(hash_table.CB_token);
    add_to_trace('{');
    subtypes k = math_data.find_math_location(math_hbox_cd, nomathenv_code);
    math_data.get_list(k).set_name(c);
    scan_math3(k, math_hbox_cd, 2);
    math_data.push_back(ptr, CmdChr(math_list_cmd, k), subtypes(math_hbox_cd));
}

// Scans an argument of a procedure.
// Case w=0 or w = 1: creates a scope
// case w=1 or w = 3: scans a relax
// Same command with relax hacking
// Changes in 2.10.9: always create a scope
auto Parser::math_argument(int w, Token t) -> subtypes {
    Token xfct_caller = fct_caller;
    fct_caller        = t;
    if ((w & 1) != 0) {
        remove_initial_space_relax();
        if (!cur_tok.is_invalid()) back_input();
    }
    if (before_mac_arg()) back_input(hash_table.CB_token);
    add_to_trace('{');
    subtypes      k   = math_data.find_math_location(math_argument_cd, nomathenv_code);
    boundary_type aux = bt_brace;
    push_level(aux);
    scan_math(k, math_argument_cd);
    pop_level(aux);
    fct_caller = xfct_caller;
    return k;
}

// This reads a number and returns \displaystyle, \textstyle
// scriptstyle \scriptscriptstyle if the result is 0 1 2 3
// It returns \relax otherwise. Should we call scanint ?
auto Parser::scan_style() -> Token {
    TokenList L = read_arg();
    Token     t = token_ns::get_unique(L);
    size_t    p = 4;
    if (t.cmd_val() == other_catcode) {
        auto tt = t.val_as_digit();
        if (tt <= 3) p = tt;
    }
    return hash_table.genfrac_mode[p];
}

// This parses something like
// \genfrac (){0pt}3{foo}{bar}
void Parser::interpret_genfrac_cmd(int res, subtypes k, CmdChr W) {
    Token     ct = cur_tok;
    del_pos   k1{}, k2{};
    TokenList L1 = read_arg();
    if (L1.empty())
        k1 = del_dot;
    else {
        back_input(L1);
        k1 = math_lr_value();
    }
    TokenList L2 = read_arg();
    if (L2.empty())
        k2 = del_dot;
    else {
        back_input(L2);
        k2 = math_lr_value();
    }
    TokenList L3    = read_arg();
    long      dmres = 0;
    if (!L3.empty()) {
        back_input(L3);
        scan_dimen(false, ct);
        dmres = cur_val.get_int_val();
        Trace.push_back(ScaledInt(dmres), glue_spec_pt);
        Buffer B;
        B.push_back(ScaledInt(dmres), glue_spec_pt);
        dmres = to_signed(Istring(B).id);
    }
    Token m = scan_style();
    add_to_trace(m);
    subtypes r1 = math_argument(0, ct);
    subtypes r2 = math_argument(0, ct);
    Math &   u  = math_data.get_list(k);
    u.push_back(CmdChr(left_cmd, subtypes(k1)), zero_code);
    u.push_back(CmdChr(right_cmd, subtypes(k2)), zero_code);
    u.push_back(CmdChr(right_cmd, subtypes(dmres)), zero_code);

    token_from_list(m);
    cur_tok = m;
    u.push_back(cur_cmd_chr, subtypes(cur_tok.val));
    u.push_back_list(r1, math_argument_cd);
    u.push_back_list(r2, math_argument_cd);
    math_data.push_back(res, W, subtypes(u.get_type()));
}

// Handles \mathmi[foo][bar][a][b]{etc} and friends
void Parser::scan_math_mi(int res, subtypes c, subtypes k, CmdChr W) {
    Token       ct = cur_tok;
    std::string s;
    if (c == mathbox_code) {
        s = group_to_string(); // name of the env (no expand here ?)
        add_to_trace(s);
    }
    std::vector<MathElt> T; // the optional arguments
    for (;;) {
        remove_initial_space_and_back_input();
        if (!cur_tok.is_open_bracket()) break;
        TokenList L;
        read_optarg_nopar(L);
        brace_me(L);
        back_input(L);
        subtypes r1 = math_argument(0, ct);
        T.emplace_back(CmdChr(math_list_cmd, r1), subtypes(math_argument_cd));
    }
    auto n      = T.size();
    n           = n / 2;
    n           = n + n; // Ignore last if odd
    subtypes r1 = math_argument(0, ct);
    Math &   u  = math_data.get_list(k);
    if (c == mathbox_code) {
        auto ss = Istring(s);
        u.set_name(subtypes(ss.id));
    }
    u.push_back_list(r1, math_argument_cd);
    for (size_t i = 0; i < n; i++) u.push_back(T[i]);
    math_data.push_back(res, W, subtypes(u.get_type()));
}

// Case of \mathchoice{}{}{}{}
void Parser::interpret_mathchoice_cmd(int res, subtypes k, CmdChr W) {
    Token    ct = cur_tok;
    subtypes r1 = math_argument(1, ct);
    subtypes r2 = math_argument(1, ct);
    subtypes r3 = math_argument(1, ct);
    subtypes r4 = math_argument(1, ct);
    Math &   u  = math_data.get_list(k);
    u.push_back_list(r1, math_argument_cd);
    u.push_back_list(r2, math_argument_cd);
    u.push_back_list(r3, math_argument_cd);
    u.push_back_list(r4, math_argument_cd);
    math_data.push_back(res, W, subtypes(u.get_type()));
}

// This replaces [foo] by {foo}
void Parser::opt_to_mandatory() {
    TokenList L;
    read_optarg(L);
    brace_me(L);
    back_input(L);
}

// Scans a command with some arguments.
void Parser::interpret_math_cmd(int res, subtypes c) {
    Token    ct = cur_tok;
    subtypes k  = math_data.find_math_location(sub_to_math(c), nomathenv_code);
    CmdChr   W  = CmdChr(special_math_cmd, k);
    switch (c) {
    case genfrac_code: interpret_genfrac_cmd(res, k, W); return;
    case mathchoice_code: interpret_mathchoice_cmd(res, k, W); return;
    case sqrt_code:
        remove_initial_space_and_back_input();
        if (cur_tok.is_open_bracket()) {
            c = root_code;
            opt_to_mandatory();
        }
        break;
    case xleftarrow_code:
    case xrightarrow_code: opt_to_mandatory(); break;
    case smash_code:
    case cfrac_code: opt_to_mandatory(); break;
    case mathmi_code:
    case mathmo_code:
    case mathmn_code:
    case mathnothing_code:
    case mathci_code:
    case mathcn_code:
    case mathcsymbol_code:
    case multiscripts_code:
    case mathbox_code: scan_math_mi(res, c, k, W); return;
    case operatorname_code:
        if (remove_initial_star()) c = operatornamestar_code;
        break;
    default:; // nothing special
    }
    math_data.set_type(k, sub_to_math(c)); // may be needed in some cases
    // bool is_math_accent = c>=first_maccent_code && c<=last_maccent_code;
    // int first_spec = is_math_accent?1:0;
    // if(c==sqrt_code) first_spec = 1;
    //  subtypes r1 = math_argument(first_spec,ct);
    subtypes r1 = math_argument(0, ct);
    subtypes r2 = zero_code;
    subtypes r3 = zero_code;
    if (c == cfrac_code || c == qopname_code || c == multicolumn_code) {
        r2 = math_argument(0, ct);
        r3 = math_argument(0, ct);
    } else if (c <= last_marg_code || c > last_maccent_code)
        r2 = math_argument(0, ct);
    else if (c == xleftarrow_code || c == xrightarrow_code || c == smash_code)
        r2 = math_argument(0, ct);
    Math &u = math_data.get_list(k);
    u.push_back_list(r1, math_argument_cd);
    if (r2 != 0U) u.push_back_list(r2, math_argument_cd);
    if (r3 != 0U) u.push_back_list(r3, math_argument_cd);
    math_data.push_back(res, W, subtypes(u.get_type()));
}

// --------------------------
// Math array handling
void Math::skip_initial_space() {
    while (!empty() && front().is_space()) pop_front();
}

// Constructs the attributes list for a table.
// eqnarray is RCL, align is RL, and matrix is C*.
// For aligned it is rlrlrlrl
// Otherwise we must fetch an argument.
void Math::fetch_rlc(std::vector<AttList> &table) {
    std::string rlc = "rcl";
    switch (sname) {
    case gather_code:
    case gather_star_code:
    case gathered_code:
    case multline_code:
    case multline_star_code: rlc = "c"; break;
    case bordermatrix_code: rlc = ""; break;
    case split_code: rlc = "rl"; break;
    case aligned_code:
    case align_code:
    case align_star_code:
    case flalign_code:
    case flalign_star_code:
    case alignat_code:
    case alignat_star_code:
    case xalignat_code:
    case xalignat_star_code:
    case xxalignat_code:
    case xxalignat_star_code: rlc = "rlrlrlrlrl"; break;
    case matrix_code:
    case matrixb_code:
    case matrixB_code:
    case matrixp_code:
    case matrixV_code:
    case matrixv_code: rlc = ""; break;
    case array_code:
        remove_opt_arg(false);
        rlc = remove_req_arg();
        break;
    default:;
    }
    table = rlc_to_string(rlc);
}

// Converts a cell. Updates n, the index of the cell in the row.
auto Math::convert_cell(size_t &n, std::vector<AttList> &table, math_style W) -> Xml {
    Xml res(cst_mtd, nullptr);
    if (empty()) {
        n++; // empty cell, no atts needed.
        return res;
    }
    Xid id        = res.id;
    int tbl_align = 0;
    cmi.set_cid(id);
    Math args = *this;
    if (!(front().get_cmd() == special_math_cmd && front().get_lcmd() == sub_to_math(multicolumn_code))) {
        auto m = table.size();
        if (n < m) id.add_attribute(table[n], true);
        n++;
    } else {
        Math L = front().get_list();
        pop_front();
        skip_initial_space();
        if (!empty()) the_parser.parse_error(the_parser.err_tok, "Cell contains garbage after \\multicolumn", "multicol garbage");
        L.get_arg1().convert_this_to_string(math_buffer); // get the span
        int k = 0;
        try {
            k = std::stoi(math_buffer);
        } catch (...) { spdlog::warn("Could not parse `{}' as an integer", math_buffer); }
        if (k <= 0)
            n++;
        else {
            id.add_attribute(np_columnspan, Istring(std::to_string(k)));
            n += to_unsigned(k);
        }
        L.get_arg2().convert_this_to_string(math_buffer);
        auto c = math_buffer.single_char();
        if (c == 'l') tbl_align = 1;
        if (c == 'r') tbl_align = 2;
        args = L.get_arg3();
    }
    int k = args.check_align();
    if (k != 0) tbl_align = k;
    if (tbl_align == 1)
        id.add_attribute(np_columnalign, np_left);
    else if (tbl_align == 2)
        id.add_attribute(np_columnalign, np_right);
    else if (tbl_align == 3)
        id.add_attribute(np_columnalign, np_center);
    res.add_tmp(gsl::not_null{args.convert_math(W)});
    return res;
}

// Converts an array.
auto Math::split_as_array(std::vector<AttList> &table, math_style W, bool numbered) -> Xml * {
    Math cell;
    bool is_multline = sname == multline_code || sname == multline_star_code;
    bool needs_dp    = (math_env_props(sname) & 1) != 0;
    if (sname == aligned_code) needs_dp = true; // OK FIXME
    if (sname == split_code) needs_dp = true;   // OK FIXME
    size_t n    = 0;                            // index of cell in row.
    Xml *  res  = new Xml(cst_mtable, nullptr);
    Xml *  row  = new Xml(cst_mtr, nullptr);
    Xid    rid  = cmi.get_rid(); // old rid, to be restored at the end
    Xid    cid  = cmi.get_cid();
    Xid    taid = cmi.get_taid();
    cmi.set_taid(res->id);
    cmi.set_rid(row->id);
    if (needs_dp) W = ms_D;
    res->push_back_unless_nullptr(row);
    bool first_cell = is_multline;
    if (numbered) cmi.reset_last_ml_pos();
    while (!empty()) {
        symcodes cmd = front().get_cmd();
        if (cmd == alignment_catcode) { // finish cell
            pop_front();
            row->push_back(gsl::not_null{new Xml(cell.convert_cell(n, table, W))});
            cmi.set_cid(cid);
            cell.clear();
            first_cell = false;
        } else if (cmd == backslash_cmd) { // finish row and cell
            pop_front();
            remove_opt_arg(true); // optional argument ignored.
            row->push_back(gsl::not_null{new Xml(cell.convert_cell(n, table, W))});
            if (first_cell) cmi.get_cid().add_attribute(np_columnalign, np_left);
            cmi.set_cid(cid);
            cell.clear();
            first_cell = false;
            if (numbered) cmi.ml_second_pass(row, the_parser.tracing_math());

            n   = 0;
            row = new Xml(cst_mtr, nullptr);
            cmi.set_rid(row->id);
            res->push_back_unless_nullptr(row);
        } else if (cmd == space_catcode && cell.empty()) {
            pop_front();
        } else {
            cell.push_back(front());
            pop_front();
        }
    }
    if (!cell.empty()) {
        row->push_back(gsl::not_null{new Xml(cell.convert_cell(n, table, W))});
        if (is_multline) cmi.get_cid().add_attribute(np_columnalign, np_right);
        cmi.set_cid(cid);
    }
    if (row->empty()) // kill the last empty row
        res->pop_back();
    else {
        if (numbered) cmi.ml_second_pass(row, the_parser.tracing_math());
    }

    Xid w = the_main->the_stack->next_xid(res);
    w.add_attribute(cmi.get_taid()); // move the attributes
    cmi.set_rid(rid);
    cmi.set_taid(taid);
    res->id = w;
    if (needs_dp) res->add_att(cst_displaystyle, np_true);
    return res;
}

// Converts an object that holds an array or something.
auto Math::M_array(bool numbered, math_style cms) -> Xml * {
    std::vector<AttList> table;
    fetch_rlc(table);
    Xml *res = split_as_array(table, cms, numbered);
    if (sname == bordermatrix_code) {
        res->bordermatrix();
        return res;
    }
    size_t open = 0, close = 0;
    bool   nf = special_fence(sname, open, close);
    if (nf) {
        res = new Xml(the_names["mfenced"], res);
        res->add_att(the_names["close"], math_data.get_fence(close));
        res->add_att(the_names["open"], math_data.get_fence(open));
    }
    return res;
}

void Xml::bordermatrix() {
    if (size() <= 1) return;
    auto n = size() - 1;
    auto F = front();
    if ((F != nullptr) && !F->is_xmlc() && F->size() > 1) { F->insert_at(1, new Xml(cst_mtd, nullptr)); }
    auto att    = Istring("rowspan");
    auto attval = Istring(std::to_string(n));
    F           = at(1);
    if ((F != nullptr) && !F->is_xmlc() && F->size() > 1) {
        Xml *aux = new Xml(cst_mtd, MathDataP::mk_mo("("));
        aux->add_att(att, attval);
        F->insert_at(1, aux);
        aux = new Xml(cst_mtd, MathDataP::mk_mo(")"));
        aux->add_att(att, attval);
        F->push_back_unless_nullptr(aux);
    }
}

// -----------------------------------------------------------

// We assume here that the formula has two tokens. The first is underscore
// or caret, code in cmd. In the case of $_x$ or $_{x}$ or $^{y}$
// return a non-mathml expression in case the argument is OK.
auto Math::trivial_math_index(symcodes cmd) -> Xml * {
    Buffer B;
    auto   L = value.begin();
    ++L;
    name_positions loc       = cmd == underscore_catcode ? np_s_sub : np_s_sup;
    CmdChr         w         = L->get_cmd_chr();
    bool           have_font = false;
    name_positions font_pos  = cst_empty;
    if (w.is_letter() || w.is_other())
        B.push_back(w.char_val());
    else if (L->is_list()) {
        const Math &A = L->get_list();
        auto        C = A.value.begin();
        auto        E = A.value.end();
        if (C == E) return nullptr;
        if (C->get_cmd() == mathfont_cmd) {
            have_font  = true;
            subtypes s = C->get_chr();
            if (s == math_f_italic)
                font_pos = np_font_it;
            else if (s == math_f_upright)
                have_font = false;
            else if (s == math_f_sansserif)
                font_pos = np_font_sansserif;
            else if (s == math_f_monospace)
                font_pos = np_font_tt;
            else if (s == math_f_bold)
                font_pos = np_font_bold;
            else
                return nullptr;
            ++C;
        }
        while (C != E) {
            if (C->is_char())
                B.push_back(C->get_char());
            else if (C->get_cmd() == mathfont_cmd) {
                ++C;
                if (C == E) break;
                return nullptr;
            } else
                return nullptr;
            ++C;
        }
    } else
        return nullptr;
    Xml *tmp  = Stack::fonts1(loc);
    Xml *xval = new Xml(Istring(B));
    if (have_font) {
        Xml *tmp2 = Stack::fonts1(font_pos);
        tmp2->push_back_unless_nullptr(xval);
        xval = tmp2;
    }
    tmp->push_back_unless_nullptr(xval);
    return tmp;
}

// If this is a number, returns the number, with ok = 1;
// if it starts with a number, followed by a hat, returns the number,
// and reads the hat.
// Sets ok=2 (if ok), 0 otherwise.

// If only digits are given the result is the formula as an XML elemet.
// Otherwise, we expect $1^{er}$, or things like that.
// Only digits can appear before the hat. Only one token (or a list)
// can appear after the hat. This should be e, o or a special exponent.
// The result is as if you said: 1\textsuperscript{er} (without $).
// Some hacks
// Handles the case $1$, the case $x$.
// We handle also the case of $_{foo}$ that gives \textsubscript{foo}
// provided that a special switch is true.

auto Math::trivial_math(long action) -> Xml * {
    action = action % 8;
    if (action == 0) return nullptr;
    auto L = value.begin();
    if (L == value.end()) return nullptr; // empty formula is never trivial
    int len = 1;
    ++L;
    if (L != value.end()) {
        ++L;
        len = 2;
        if (L != value.end()) len = 3;
    }
    symcodes cmd = front().get_cmd();
    Xml *    res{nullptr};
    if (((action & 4) != 0) && len == 2 && (cmd == underscore_catcode || cmd == hat_catcode)) res = trivial_math_index(cmd);
    if (res != nullptr) return res;
    if ((action & 1) != 0) res = special1();
    if (res != nullptr) return res;
    if (len != 1) return nullptr;
    if ((action & 2) == 0) return nullptr;
    if (front().is_digit()) {
        Istring sval = the_names[std::to_string(cst_dig0 + front().val_as_digit())];
        return new Xml(sval);
    }
    if (front().is_letter_token()) {
        auto c = front().get_char().value;
        if (c < nb_simplemath) return math_data.get_simplemath_val(c);
    }
    if (front().is_other_token() && front().get_char() == '-') return new Xml(Istring("&#x2013;"));
    if (front().get_cmd() == mathord_cmd || front().get_cmd() == mathordb_cmd || front().get_cmd() == mathbin_cmd ||
        front().get_cmd() == mathop_cmd || front().get_cmd() == mathopn_cmd || front().get_cmd() == mathrel_cmd ||
        front().get_cmd() == mathinner_cmd || front().get_cmd() == mathbetween_cmd || front().get_cmd() == mathopen_cmd ||
        front().get_cmd() == mathclose_cmd) {
        size_t c = front().get_chr();
        if (first_inline_hack <= c && c <= last_inline_hack) return math_ns::get_builtin_alt(c);
    }
    return nullptr;
}

// Inserts the current font in the list
void Math::add_cur_font() {
    auto c = the_parser.eqtb_int_table[math_font_pos].val;
    push_back_font(subtypes(c), zero_code);
}

// Insert the font in the list and saves the font.
void Parser::TM_math_fonts(Math &x) const {
    subtypes cur_chr = cur_cmd_chr.chr;
    the_parser.word_define(math_font_pos, cur_chr, false);
    x.push_back_font(cur_chr, zero_code);
}

auto Parser::is_not_a_math_env(String s) -> bool {
    find_env_token(s, false);
    return cur_cmd_chr.is_user();
}

// This is done when we see a label in a math formula.
// If anchor is true, we are in the case of \anchorlabel.
// Only one label is allowed.
void MathHelper::new_label(const std::string &s, bool anchor) {
    if (eqnum_status == 2 || eqnum_status == 1) {
        if (anchor) the_parser.parse_error("Illegal \\anchorlabel");
        cmi.new_multi_label(s, 1);
        return;
    }
    bool ok = true;
    if (anchor)
        ok = (eqnum_status == 0);
    else {
        if (eqnum_status == 0) ok = false; // TEMP
    }
    if (seen_label) ok = false;
    if (ok)
        set_label(s);
    else
        the_parser.parse_error(the_parser.err_tok, "Label will be lost: ", s, "label-lost");
}

// This removes the spaces.
void Math::remove_spaces() {
    value.remove_if([](const MathElt &v) { return v.is_space(); });
}

// Returns true if there is an \over or something like that in the list.
auto Math::has_over() const -> bool {
    auto ovr = std::count_if(value.begin(), value.end(), [](const MathElt &m) { return m.get_cmd() == over_cmd; });
    if (ovr > 1) the_parser.parse_error("Too many commands of type \\over");
    return ovr > 0;
}

// Case of \mathop{\rm sin}. The this in the procedure is what follows the
// \mathop.
auto MathElt::try_math_op() const -> Xml * {
    if (!is_list()) return nullptr;
    Math &X = get_list();
    if (X.empty()) return nullptr;
    if (!(X.front().get_cmd() == mathfont_cmd && X.front().get_chr() == math_f_upright)) return nullptr;
    if (!X.chars_to_mb2(math_buffer)) return nullptr;
    Xml *s = new Xml(cst_mo, new Xml(Istring(math_buffer)));
    s->add_att(np_form, np_prefix);
    return s;
}

// This converts a character into a MathML object
auto MathElt::cv_char() const -> MathElt {
    unsigned c  = get_chr();
    int      a  = 0;
    auto     mt = mt_flag_small;
    auto     F  = get_font();
    if (c >= nb_mathchars) return MathElt(math_ns::mk_mi(codepoint(c)), mt_flag_small);
    if (::is_digit(static_cast<char>(c)))
        a = to_signed(c) - '0' + math_dig_loc;
    else if (::is_letter(static_cast<char>(c)) && F < 2) {
        a = math_char_normal_loc + F * to_signed(nb_mathchars) + to_signed(c);
    } else if (::is_letter(static_cast<char>(c))) {
        auto w = the_parser.eqtb_int_table[mathprop_ctr_code].val;
        if ((w & (1 << F)) != 0) return MathElt(math_ns::mk_mi(static_cast<uchar>(c), F), mt);
        return MathElt(math_ns::make_math_char(static_cast<uchar>(c), F), mt);
    } else {
        a  = to_signed(c) + math_c_loc;
        mt = math_data.get_math_char_type(c);
    }
    return MathElt(subtypes(a), mt);
}

// This converts a constant.
auto MathElt::cv_cst() const -> MathElt {
    subtypes   c  = get_chr();
    Xml *      s  = math_constants(c);
    math_types mt = math_space_code(c) ? mt_flag_space : mt_flag_small;
    return MathElt(s, mt);
}

// This converts a list
auto MathElt::cv_list(math_style cms, bool ph) -> MathElt {
    Math &X = get_list();
    if (get_lcmd() == math_open_cd) { // case of {x+y}
        XmlAndType res = X.M_cv(cms, ph ? 2 : 1);
        return MathElt(res.value, res.type);
    }
    if (get_lcmd() == math_LR_cd) { // case \left(x+y\right)
        auto a = X.front().get_chr();
        auto b = X.back().get_chr();
        X.pop_front();
        X.pop_back();
        XmlAndType res  = X.M_cv(cms, 0);
        Xml *      res2 = math_data.make_mfenced(a, b, gsl::not_null{res.value});
        return MathElt(res2, mt_flag_big);
    }
    if (get_lcmd() == math_env_cd) // case \begin{array}...
        return MathElt(X.M_array(false, cms), mt_flag_big);
    cv1_err();
    return MathElt(CmdChr(error_cmd, zero_code), zero_code);
}

// Return the name of the element associated to the command c.
auto math_ns::cv_special_string(int c) -> std::string {
    if (c >= first_maccent_code && c <= last_maccent_code) return c >= first_under_accent_code ? "munder" : "mover";
    if (c == overline_code || c == overbrace_code || c == stackrel_code || c == overset_code || c == accentset_code) return "mover";
    if (c == underline_code || c == underbrace_code || c == underset_code || c == undertilde_code || c == underaccent_code) return "munder";
    if (c == root_code) return "mroot";
    if (c == frac_code || c == dfrac_code || c == tfrac_code || c == genfrac_code || c == cfrac_code || c == binom_code ||
        c == dbinom_code || c == tbinom_code)
        return "mfrac";
    return "unknown";
}

// Return 1 if the list is left aligned, 2 if right aligned, 0 if centered
auto Math::check_align() -> int {
    int a = 0, b = 0;
    if (!empty() && front().get_cmd() == hfill_cmd) {
        a = front().get_chr();
        pop_front();
    }
    if (!empty() && back().get_cmd() == hfill_cmd) {
        b = back().get_chr();
        pop_back();
    }
    if (a == b && a == hfill_code) return 3; // clearly centered
    if (a == b) return 0;                    // clearly centered
    if (a == hfill_code) return 2;
    if (b == hfill_code) return 1;
    if (a == hfil_code) return 2;
    if (b == hfil_code) return 1;
    return 0; // remaining cases are \hss and \hfilneg
}

// Create <mi>...</mi> and friends
auto MathElt::cv_mi(math_style cms) const -> MathElt {
    Math &   L = get_list();
    subtypes c = get_fml_subtype();
    auto     X = L.begin();
    auto     Y = L.end();
    Xml *    res{nullptr};
    if (c == mathbox_code) {
        Xml *xs = X->get_list().M_cv(cms, 0).value;    // OK
        res     = new Xml(Istring(L.get_sname()), xs); // OK
    } else if (c == multiscripts_code) {
        Xml *xs = X->get_list().M_cv(cms, 0).value;
        auto w  = name_positions(c - mathmi_code + cst_mi);
        res     = new Xml(w, xs);
    } else {
        std::string s  = X->get_list().convert_this_to_string(math_buffer);
        Xml *       xs = new Xml(Istring(s));
        auto        w  = name_positions(c - mathmi_code + cst_mi);
        res            = new Xml(w, xs);
    }
    ++X;
    for (;;) {
        if (X == Y) break;
        std::string s1 = X->get_list().convert_this_to_string(math_buffer);
        ++X;
        std::string s2;
        if (X == Y) {
        } // Should we signal an error ?
        else {
            s2 = X->get_list().convert_this_to_string(math_buffer);
            ++X;
        }
        res->add_att(Istring(s1), Istring(s2));
    }
    return MathElt(res, mt_flag_small);
}

//  This converts commands.
auto MathElt::cv_special(math_style cms) -> MathElt {
    subtypes c = get_fml_subtype();
    Math &   L = get_list();
    switch (c) {
    case mathchoice_code: {
        std::array<Math, 4> table;
        if (!L.empty()) {
            table[0] = L.get_arg1();
            L.pop_front();
        }
        if (!L.empty()) {
            table[1] = L.get_arg1();
            L.pop_front();
        }
        if (!L.empty()) {
            table[2] = L.get_arg1();
            L.pop_front();
        }
        if (!L.empty()) {
            table[3] = L.get_arg1();
            L.pop_front();
        }
        Math w = table[cms];
        return MathElt(w.convert_math(cms), mt_flag_small); // flag ok ?
    }
    case operatorname_code:
    case operatornamestar_code: {
        std::string s   = L.get_arg1().convert_opname();
        Xml *       xs  = new Xml(Istring(s));
        Xml *       res = new Xml(cst_mo, xs);
        res->add_att(np_form, np_prefix);
        return MathElt(res, c == operatornamestar_code ? mt_flag_opD : mt_flag_opN);
    }
    case qopname_code: {
        // arg 1 is currently ignored
        std::string s   = L.get_arg3().convert_opname();
        std::string o   = L.get_arg2().convert_opname();
        Xml *       xs  = new Xml(Istring(s));
        Xml *       res = new Xml(cst_mo, xs);
        res->add_att(np_form, np_prefix);
        return MathElt(res, (o == "o") ? mt_flag_opN : mt_flag_opD);
    }
    case mathmi_code:
    case mathmo_code:
    case mathmn_code:
    case mathnothing_code:
    case mathci_code:
    case mathcn_code:
    case mathcsymbol_code:
    case multiscripts_code:
    case mathbox_code: return cv_mi(cms);
    case math_attribute_code:
    case cell_attribute_code:
    case row_attribute_code:
    case table_attribute_code:
    case formula_attribute_code:
    case thismath_attribute_code: {
        std::string s1 = L.get_arg1().convert_this_to_string(math_buffer);
        std::string s2 = L.get_arg2().convert_this_to_string(math_buffer);
        auto        A  = Istring(s1);
        auto        B  = Istring(s2);
        if (c == math_attribute_code)
            the_main->the_stack->add_att_to_last(A, B, true);
        else
            cmi.add_attribute(A, B, c);
        return MathElt(CmdChr(error_cmd, zero_code), zero_code);
    }
    case mathlabel_code: {
        std::string s1  = L.get_arg1().convert_this_to_string(math_buffer);
        std::string s2  = L.get_arg2().convert_this_to_string(math_buffer);
        Xml *       x   = new Xml(cst_mrow, nullptr);
        Istring     id  = next_label_id();
        Xid         xid = x->id;
        the_parser.the_stack.create_new_anchor(xid, id, Istring(s1));
        the_parser.create_label(s2, id);
        return MathElt(x, mt_flag_small);
    }
    case boxed_code: {
        Xml *x = L.get_arg1().M_cv(cms, 0).value;
        x      = new Xml(cst_mtd, x);
        x      = new Xml(cst_mtr, x);
        x      = new Xml(cst_mtable, x);
        x->add_att(Istring("frame"), Istring("solid"));
        return MathElt(x, mt_flag_small);
    }
    case phantom_code:
    case hphantom_code:
    case vphantom_code: {
        Xml *A = L.get_arg1().M_cv(cms, 0).value;
        if (c == hphantom_code || c == vphantom_code) {
            A = new Xml(cst_mpadded, A);
            if (c == vphantom_code) A->add_att(np_width, np_zerodim);
            if (c == hphantom_code) A->add_att(np_height, np_zerodim);
            if (c == hphantom_code) A->add_att(np_depth, np_zerodim);
        }
        Xml *R = new Xml(cst_mphantom, A);
        return MathElt(R, mt_flag_small);
    }
    case smash_code: {
        Xml *A = L.get_arg2().M_cv(cms, 0).value;
        Xml *R = new Xml(cst_mpadded, A);
        L.get_arg1().convert_this_to_string(math_buffer);
        char w = math_buffer[0];
        if (w != 'b' && w != 't' && w != 'w') w = 'c';
        if (w == 'b' || w == 'c') R->add_att(np_depth, np_zerodim);
        if (w == 't' || w == 'c') R->add_att(np_height, np_zerodim);
        if (w == 'w') R->add_att(np_width, np_zerodim);
        return MathElt(R, mt_flag_small);
    }
    default: return cv_special1(cms);
    }
}

auto MathElt::cv_special1(math_style cms) const -> MathElt {
    subtypes c        = get_fml_subtype();
    int      numalign = 0, denalign = 0;
    Math &   L = get_list();
    if (c == cfrac_code) {
        L.get_arg1().convert_this_to_string(math_buffer);
        L.pop_front();
        cms    = ms_D;
        char w = math_buffer[0];
        if (w == 'l') numalign = 1;
        if (w == 'r') numalign = 2;
        c = frac_code;
    }
    int     style = -1;                      // style to add to XML
    size_t  open = del_dot, close = del_dot; // delimiters, in case
    Istring sz;                              // fraction rule width
    if (c == genfrac_code) {
        open = L.front().get_chr();
        L.pop_front();
        close = L.front().get_chr();
        L.pop_front();
        sz = Istring(L.front().get_chr());
        L.pop_front();
        if (L.front().get_cmd() == style_cmd) {
            cms   = style_level(L.front().get_chr());
            style = cms;
        }
        L.pop_front();
    } else {
        if (c == dfrac_code) {
            cms   = ms_D;
            style = 0;
        }
        if (c == tfrac_code) {
            cms   = ms_T;
            style = 1;
        }
        if (c == dbinom_code) {
            cms   = ms_D;
            style = 0;
        }
        if (c == tbinom_code) {
            cms   = ms_T;
            style = 1;
        }
    }
    if (c == dbinom_code || c == tbinom_code || c == binom_code) {
        open  = del_open_par;
        close = del_close_par;
        sz    = the_names["0pt"];
    }
    if (c == frac_code || c == binom_code || c == genfrac_code || c == dfrac_code || c == dbinom_code) {
        cms = next_frac_style(cms);
    } else
        cms = next_math_style(cms);
    Math tmp = L.get_arg1();
    int  k   = tmp.check_align();
    if (numalign == 0) numalign = k;
    Xml *A1 = tmp.convert_math(cms);
    if (c == sqrt_code) return MathElt(new Xml(cst_msqrt, A1), mt_flag_big);
    Xml *          A2{nullptr};
    auto           ns          = cv_special_string(c);
    Istring        s           = the_names[ns];
    bool           is_fraction = ns == "mfrac";
    bool           is_mathop   = false;
    name_positions pos         = cst_empty;
    if (c == xleftarrow_code || c == xrightarrow_code) {
        tmp = L.get_arg2();
        tmp.check_align();
        A2      = tmp.convert_math(cms);
        Xml *A3 = get_builtin(c);
        if (L.get_arg1().empty()) {
            A1 = A3;
            s  = the_names["mover"];
        } else if (L.get_arg2().empty()) {
            A2 = A1;
            A1 = A3;
            s  = the_names["munder"];
        } else {
            Xml *tmp2 = new Xml(the_names["munderover"], nullptr);
            tmp2->add_tmp(gsl::not_null{A3});
            tmp2->push_back_unless_nullptr(xmlspace);
            tmp2->add_tmp(gsl::not_null{A1});
            tmp2->push_back_unless_nullptr(xmlspace);
            tmp2->add_tmp(gsl::not_null{A2});
            return MathElt(tmp2, mt_flag_big);
        }
    } else if (c >= first_maccent_code && c <= last_maccent_code) {
        A2  = get_builtin(c);
        pos = c >= first_under_accent_code ? cst_accentunder : cst_accent;
    } else if (c == overline_code) {
        A2  = math_data.get_mc_table(1);
        pos = cst_empty;
        //    pos = cst_accent;
    } else if (c == overbrace_code) {
        A2        = math_data.get_mc_table(2);
        pos       = cst_accent;
        is_mathop = true;
    } else if (c == underline_code) {
        A2  = math_data.get_mc_table(3);
        pos = cst_empty;
    } else if (c == undertilde_code) {
        A2  = math_data.get_mc_table(26);
        pos = cst_accentunder;
    } else if (c == underbrace_code) {
        A2        = math_data.get_mc_table(4);
        pos       = cst_accentunder;
        is_mathop = true;
    } else {
        tmp      = L.get_arg2();
        denalign = tmp.check_align();
        A2       = tmp.convert_math(cms);
        if (c == accentset_code) pos = cst_accent;
        if (c == underaccent_code) pos = cst_accentunder;
        if (c == stackrel_code || c == underset_code || c == overset_code || c == root_code || c == accentset_code ||
            c == underaccent_code) {
            Xml *X   = A1;
            A1       = A2;
            A2       = X;
            int xx   = numalign;
            numalign = denalign;
            denalign = xx; // swap this also
        }
    }
    Xml *res = finish_cv_special(is_fraction, s, pos, A1, A2, sz, numalign, denalign, style, open, close);
    return MathElt(res, is_mathop ? mt_flag_opD : mt_flag_big);
}

// First pass: convert characters.
auto MathElt::cv1(math_style cms, bool ph) -> MathElt {
    subtypes c = get_chr();
    switch (get_cmd()) {
    case space_catcode:
    case letter_catcode:
    case other_catcode:
    case char_given_cmd:
    case math_given_cmd: return cv_char();
    case hat_catcode:
    case underscore_catcode:
    case math_xml_cmd: // ok ??
    case left_cmd:
    case right_cmd: return *this;
    case cst1_cmd: return cv_cst();
    case mathbin_cmd: return MathElt(c, mt_flag_bin);
    case mathrel_cmd: return MathElt(c, mt_flag_rel);
    case mathordb_cmd: return MathElt(subtypes(long(c) - long(alpha_code) + long(alpha_bcode)), mt_flag_small);
    case mathord_cmd:
    case mathinner_cmd: return MathElt(c, mt_flag_small);
    case mathbetween_cmd: return MathElt(c, mt_flag_small_m);
    case mathopen_cmd: return MathElt(c, mt_flag_small_l);
    case mathclose_cmd: return MathElt(c, mt_flag_small_r);
    case mathop_cmd: return MathElt(c, mt_flag_opD);
    case mathopn_cmd: return MathElt(c, mt_flag_opN);
    case special_math_cmd: return cv_special(cms);
    case math_list_cmd: return cv_list(cms, ph);
    case mathspace_cmd: return MathElt(c, mt_flag_space);
    case relax_cmd: return MathElt(CmdChr(error_cmd, zero_code), zero_code);
    default: cv1_err(); return MathElt(CmdChr(error_cmd, zero_code), zero_code);
    }
}

void MathElt::dump_for_err() const {
    Logger::finish_seq();
    the_log << int(get_cmd()) << " - " << int(get_chr()) << " - " << int(get_font()) << "\n";
}

void MathElt::cv1_err() {
    dump_for_err();
    Logger::finish_seq();
    log_and_tty << "--- " << Token(get_font()) << "\n";
    the_parser.signal_error("Bad math expression");
}

// Converts flags from small to large.
void MathElt::change_type(int t) {
    if (get_cmd() != math_xml_cmd) return;
    math_types T = get_xmltype();
    if (T == mt_flag_small) {
        del_pos w = get_delimiter(get_chr());
        if (w == del_invalid) return;
        if (t == 0) {
            if (w == del_open_ket)
                T = mt_flag_large_l;
            else if (w == del_close_ket)
                T = mt_flag_large_r;
            else
                return;
        } else if (t == 1)
            T = mt_flag_large_l;
        else if (t == 2)
            T = mt_flag_large_r;
        else
            T = mt_flag_large_m;
        set_xmltype(T);
        return;
    }
    if (!(T == mt_flag_small_l || T == mt_flag_small_r || T == mt_flag_small_m)) return;
    if (t == 0)
        T = math_types(T - mt_flag_small_l + mt_flag_large_l);
    else if (t == 1)
        T = mt_flag_large_l;
    else if (t == 2)
        T = mt_flag_large_r;
    else
        T = mt_flag_large_m;
    set_xmltype(T);
}

// We have an \over somewhere...
auto Math::M_cv0(math_style cms) -> XmlAndType {
    Math A;
    auto c = subtypes(atopwithdelims_code + 1);
    cms    = next_frac_style(cms);
    while (!empty()) {
        if (front().get_cmd() == over_cmd) {
            c = front().get_chr();
            break;
        }
        A.push_back(front());
        pop_front();
    }
    pop_front();
    del_pos open = del_dot, close = del_dot;
    int     numalign = 0, denalign = 0;
    if (c == atopwithdelims_code || c == overwithdelims_code || c == abovewithdelims_code) {
        while (!empty()) {
            if (front().get_cmd() == relax_cmd)
                pop_front();
            else
                break;
        }
        if (empty())
            the_parser.parse_error("Problem finding opening delim");
        else {
            open = get_delimiter(front().get_cmd_chr());
            if (open == del_invalid) the_parser.parse_error(the_parser.err_tok, "Invalid character in open", "bad delims");
            pop_front();
        }
        while (!empty()) {
            if (front().get_cmd() == relax_cmd)
                pop_front();
            else
                break;
        }
        if (empty())
            the_parser.parse_error(the_parser.err_tok, "Problem finding closing delim");
        else {
            close = get_delimiter(front().get_cmd_chr());
            if (close == del_invalid) the_parser.parse_error(the_parser.err_tok, "Invalid character in close", "bad delims");
            pop_front();
        }
    }
    Istring sz;
    if (c == above_code || c == abovewithdelims_code) {
        if (empty()) the_parser.parse_error(the_parser.err_tok, "Invalid width", "bad delims");
        sz = chars_to_mb3();
    }
    if (c == atop_code || c == atopwithdelims_code) sz = the_names["0pt"];
    numalign = A.check_align();
    denalign = check_align();
    Xml *a   = A.M_cv(cms, 1).value;
    Xml *b   = M_cv(cms, 1).value;
    Xml *res = finish_cv_special(true, the_names["mfrac"], cst_empty, a, b, sz, numalign, denalign, -1, open, close);
    return {res, mt_flag_big};
}

auto math_ns::finish_cv_special(bool isfrac, Istring s, size_t pos, Xml *a, Xml *b, const Istring &sz, int numalign, int denalign,
                                int style, size_t open, size_t close) -> Xml * {
    Istring Pos;
    if (pos != 0) Pos = the_names[name_positions(pos)];
    auto R = the_main->the_stack->xml2_space(std::move(s), Pos, a, b);
    if (!sz.null()) R->add_att(the_names["np_linethickness"], sz);
    if (isfrac) {
        if (numalign == 1) R->add_att(cst_numalign, np_left);
        if (numalign == 2) R->add_att(cst_numalign, np_right);
        if (denalign == 1) R->add_att(cst_denalign, np_left);
        if (denalign == 2) R->add_att(cst_denalign, np_right);
    }
    if (style >= 0) R = math_data.add_style(style, R);
    if (!(open == del_dot && close == del_dot)) R = math_data.make_mfenced(open, close, R);
    return R;
}

auto Math::M_cv(math_style cms, int need_row) -> XmlAndType {
    the_parser.my_stats.one_more_convert();
    if (has_over()) return M_cv0(cms); // there was an over somewhere
    Math res;
    int  i           = 0;
    bool prev_is_hat = false; //  must increment cms if true
    bool cur_is_hat  = false;
    bool seen_style  = false;
    while (!empty()) {
        i++;
        MathElt cur = front();
        pop_front();
        if (cur.is_space()) continue; // ignore this
        prev_is_hat  = cur_is_hat;
        symcodes cmd = cur.get_cmd();
        subtypes chr = cur.get_chr();
        if (cmd == hat_catcode || cmd == underscore_catcode) {
            cur_is_hat = true;
            res.push_back(cur);
            continue;
        }
        cur_is_hat = false;
        if (cur.is_hbox()) {
            cur.get_list().handle_mbox(res);
            continue;
        }
        if (cmd == hspace_cmd) {
            if (chr == one_code || chr == three_code) continue;
            Buffer &B = Trace;
            B.clear();
            int n = cur.get_font();
            B.push_back(ScaledInt(n), glue_spec_pt);
            Xml *v = mk_space(B);
            res.push_back(MathElt(v, mt_flag_space));
            continue;
        }
        if (cmd == style_cmd) {
            cms        = style_level(chr);
            seen_style = true;
            continue;
        }
        if (cmd == ref_cmd) {
            res.push_back_small(M_ref());
            continue;
        }

        int        next_action = 0;
        math_types new_type    = mt_flag_rel;
        int        t           = 0;
        if (cmd == nonscript_cmd) {
            if (cms == ms_T || (ms_D != 0U)) continue;
            res.push_back(cur);
            continue;
        }
        if (cmd == limits_cmd) {
            res.push_back(cur);
            continue;
        }
        if (cmd == math_comp_cmd && (chr == bin_noad || chr == rel_noad) && !empty()) {
            next_action = 1;
            if (chr == bin_noad) new_type = mt_flag_bin;
            cur = front();
            pop_front();
        } else if (cmd == math_comp_cmd) {
            res.push_back(cur);
            continue;
        } else if (cmd == big_cmd && !empty()) {
            cur = front();
            pop_front();
            t           = chr / 4; //  empty, L, R, et M.
            next_action = 2;
        } else if (cmd == ensuremath_cmd || cmd == nolinebreak_cmd || cmd == mathfont_cmd)
            continue;
        bool next_is_hat = false;
        if (cmd == math_list_cmd) {
            while (!empty() && front().is_space()) pop_front();
            if (!empty() && (front().get_cmd() == hat_catcode || front().get_cmd() == hat_catcode)) next_is_hat = true;
        }
        math_style cmss = cms;
        if (prev_is_hat) cmss = next_math_style(cms);
        if (cur.maybe_seq() && !empty() && front().maybe_seq(cur.get_font()) && next_action == 0 && !prev_is_hat) {
            res.push_back(convert_char_seq(cur));
        } else {
            MathElt Cur = cur.maybe_iseq() ? convert_char_iseq(cur, !prev_is_hat) : cur.cv1(cmss, next_is_hat);
            if (next_action == 1) Cur.set_xmltype(new_type);
            if (next_action == 2) Cur.change_type(t);
            if (Cur.get_cmd() == error_cmd) continue;
            res.push_back(Cur);
        }
    }
    res.handle_cmd_Big(cms);
    Math       res1     = res.M_cv3(cms);
    math_types res_type = mt_flag_small;
    if (res1.finish_translate1(the_parser.tracing_math())) res_type = mt_flag_big;
    if (res1.length_one()) {
        Xml *W = res1.front().get_xml_val();
        if (need_row == 2) W = new Xml(cst_mrow, W);
        if (!seen_style) return {W, res_type};

        Xml *res2 = math_data.add_style(cms, gsl::not_null{W});
        return {res2, res_type};
    }
    Xml *tmp = new Xml(cst_temporary, nullptr);
    res1.concat(tmp);
    Xml *res22{nullptr};
    if (need_row != 0)
        res22 = new Xml(cst_mrow, tmp);
    else
        res22 = tmp;
    if (seen_style) res22 = math_data.add_style(cms, gsl::not_null{res22});
    return {res22, res_type};
}

// Translate \ref
auto Math::M_ref() -> Xml * {
    Math w = front().get_list();
    pop_front();
    std::string label = w.convert_opname();
    Xml *       X     = new Xml(cst_mref, nullptr);
    X->id.add_ref(label);
    return X;
}

// Case of a box in math.
// Gathers characters in the buffer. Return 0 in case of error
// 1 in case everything is OK, -N if a math formula has been seen,
// and 3 if we have seen a space. Return 2 in case of a font command
// Sets f to the font

static ScaledInt cur_math_space;

void math_ns::bad_math_warn(Buffer &B) {
    Logger::finish_seq();
    the_log << "Bad token in argument of \\text-like command\n";
    if (B.empty())
        the_log << "Error occured at start of list.\n";
    else
        the_log << "Error occured after scanning " << B << ".\n";
}

auto Math::M_mbox1(Buffer &B, subtypes &f) -> int {
    B.clear();
    while (!empty()) {
        symcodes cmd = front().get_cmd();
        auto     chr = front().get_chr();
        subtypes fn  = front().get_font();
        MathElt  old = front();
        pop_front();
        if (cmd == space_catcode || (cmd == other_catcode && chr == '~')) return 3;
        if (cmd == cst1_cmd && math_space_code(chr)) {
            switch (chr) {
            case quad_code: return 5;
            case qquad_code: return 6;
            case exclam_code: return 7;
            case comma_code: return 8;
            default: return 3;
            }
        } else if (cmd == 11 || cmd == 12) {
            B.push_back_real_utf8(codepoint(char32_t(chr)));
            f = fn;
            continue;
        } else if (cmd == mathfont_cmd)
            return 2;
        else if (cmd == fontsize_cmd)
            return 2;
        else if (is_m_font(cmd))
            continue;
        else if (cmd == ref_cmd) {
            if (front().get_cmd() == math_list_cmd && front().get_list().type == math_open_cd) return 4;
            return 2; // Should signal an error
        } else if (cmd == char_given_cmd || cmd == math_given_cmd) {
            B.push_back_real_utf8(codepoint(char32_t(chr)));
            continue;
        } else if (cmd == relax_cmd)
            continue;
        else if (cmd == mathspace_cmd) {
            if (chr == subtypes(xml_thickmu_space_loc)) return 10;
            if (chr == subtypes(xml_thinmu_space_loc)) return 7;
            if (chr == subtypes(xml_medmu_space_loc)) return 9;
            return 0;
        } else if (cmd == kern_cmd || cmd == scan_glue_cmd || cmd == hspace_cmd) {
            cur_math_space = ScaledInt(old.get_font());
            return 11;
        } else if (cmd == special_math_cmd) {
            math_ns::bad_math_warn(B);
            math_list_type y = old.get_list().type;
            CmdChr         x(special_math_cmd, math_to_sub(y));
            the_log << "Offending command = \\" << x.name() << "\n";
            return 0;
        } else if (cmd != math_list_cmd) {
            math_ns::bad_math_warn(B);
            the_log << "Offending command = \\" << old.get_cmd_chr().name() << "\n";
            return 0;
        } else if (old.get_list().type == math_dollar_cd)
            return -int(chr);
        else if (old.get_list().type == math_open_cd)
            push_front(old.get_list()); // insert the sublist here
        else {
            math_ns::bad_math_warn(B);
            the_log << "Subformula is of type " << old.get_list().type << "\n";
            return 0;
        }
    }
    return 1;
}

void Math::handle_mbox(Math &res) {
    while (!empty()) {
        subtypes font = math_f_normal;
        int      ok   = M_mbox1(math_buffer, font);
        if (ok == 0) {
            the_parser.signal_error("bad hbox (see transcript file for details)");
            Xml *Text = new Xml(cst_mtext, new Xml(Istring("bad hbox")));
            res.push_back_small(new Xml(Istring("merror"), Text));
            return;
        }
        if (!math_buffer.empty()) {
            auto s    = math_buffer;
            Xml *Text = new Xml(cst_mtext, new Xml(Istring(s)));
            if (int(font) > 1) Text->add_att(cst_mathvariant, name_positions(long(cstf_normal) + long(font)));
            res.push_back_small(Text);
            the_parser.my_stats.one_more_mbox();
        }
        if (ok == 1) return;
        if (ok == 2) continue;
        if (ok == 4) { // This is a \ref
            res.push_back_small(M_ref());
        } else if (ok == 3 || ok == 5 || ok == 6 || ok == 7 || ok == 8 || ok == 9 || ok == 10 || ok == 11) {
            Xml *b{nullptr};
            if (ok == 5)
                b = math_data.get_mc_table(7);
            else if (ok == 6)
                b = math_data.get_mc_table(8);
            else if (ok == 7)
                b = math_data.get_mc_table(13);
            else if (ok == 8)
                b = math_data.get_mc_table(14);
            else if (ok == 9)
                b = get_builtin(xml_medmu_space_loc);
            else if (ok == 10)
                b = get_builtin(xml_thickmu_space_loc);
            else if (ok == 11) {
                Buffer &B = Trace;
                B.clear();
                B.push_back(cur_math_space, glue_spec_pt);
                b = mk_space(B);
            } else {
                b = new Xml(cst_mspace, nullptr);
                b->add_att(np_cst_width, np_halfem);
            }
            res.push_back_small(b);
        } else {
            Math u = math_data.get_list(-ok);
            res.push_back_small(u.convert_math(ms_T));
        }
    }
}

auto MathElt::remove_prefix() const -> Xml * {
    if (get_cmd() == math_xml_cmd) return get_xml_val();
    dump_for_err();
    log_and_tty << "bad math token " << Token(get_font()) << int(right_cmd) << "\n";
    return new Xml(Istring("BAD"));
}

void Cv3Helper::reinit() {
    state       = 2;
    index       = nullptr;
    exponent    = nullptr;
    p           = nullptr;
    ploc        = -1;
    ptype       = mt_flag_small;
    prefix_type = mt_flag_zero;
    special     = 0;
}

// Case where current token is \nonscript. If the mode is D or T, the token
// is removed in the first pass. Otherwise we remove it in the second pass
// together with a space that follows
void Cv3Helper::non_script() {
    pop_front();
    if (!empty()) {
        if (get_cmd() == math_xml_cmd && get_xmltype() == mt_flag_space) pop_front();
    }
}

void Cv3Helper::find_kernel() {
    if (empty()) {
        state = 3;
        return;
    }
    symcodes C = get_cmd();
    if (C == hat_catcode || C == underscore_catcode) {
        p     = new Xml(the_names["mrow"], nullptr); // Insert <mrow/> before
        state = 1;
        return;
    }
    if (C == nonscript_cmd) {
        non_script();
        return;
    }
    if (C == math_comp_cmd) {
        subtypes q = get_chr();
        switch (q) {
        case between_noad: prefix_type = mt_flag_small_m; break;
        case ord_noad: prefix_type = mt_flag_small; break;
        case op_noad: prefix_type = mt_flag_opD; break;
        case bin_noad: prefix_type = mt_flag_bin; break;
        case rel_noad: prefix_type = mt_flag_rel; break;
        case open_noad: prefix_type = mt_flag_small_l; break;
        case close_noad: prefix_type = mt_flag_small_r; break;
        default: prefix_type = mt_flag_small; break;
        }
        pop_front();
        return;
    }
    if (C != math_xml_cmd) {
        the_parser.signal_error("unexpected obj");
        pop_front();
        return;
    }
    ploc  = get_chr();
    ptype = get_xmltype();
    if (prefix_type != 0) ptype = prefix_type;
    if (ptype == mt_flag_opD) special = 1;
    if (ptype == mt_flag_opN) special = 2;
    p = object.front().get_xml_val();
    pop_front();
    state = 1;
}

void Cv3Helper::find_index(math_style cms) {
    if (empty()) {
        state = 2;
        return;
    }
    symcodes C = get_cmd();
    if (C == limits_cmd) {
        int k = get_chr(); // displaylimits, nolimits, limits
        pop_front();
        if ((index != nullptr) || (exponent != nullptr)) return; // allow x_2\limits^3
        if (special == 0) return;
        if (k == 0) k = cms == ms_D ? 2 : 1;
        special = special & 7; // clear flags
        if (k == 2)
            special += 8;
        else
            special += 16;
        return;
    }
    if (C == nonscript_cmd) {
        non_script();
        return;
    }
    if (C == underscore_catcode && (index == nullptr)) {
        pop_front();
        if (empty()) {
            the_parser.parse_error("Math formula should not finish with _");
        } else if (get_cmd() == underscore_catcode || get_cmd() == hat_catcode)
            the_parser.parse_error("Two consecutive _ or ^ characters");
        else {
            index = object.front().remove_prefix();
            pop_front();
        }
        return;
    }
    if (C == hat_catcode && (exponent == nullptr)) {
        pop_front();
        if (empty()) {
            the_parser.parse_error("Math formula should not finish with ^");
        } else if (get_cmd() == underscore_catcode || get_cmd() == hat_catcode)
            the_parser.parse_error("Two consecutive _ or ^ characters");
        else {
            exponent = object.front().remove_prefix();
            pop_front();
        }
        return;
    }
    if (C == underscore_catcode) {
        the_parser.parse_error("double subscript");
        pop_front();
        state = 2;
        return;
    }
    if (C == hat_catcode) {
        the_parser.parse_error("double superscript");
        pop_front();
        state = 2;
        return;
    }
    state = 2;
}

// x_1 and \sum_1 produce <msub> or <munder>
// This is a bit complicated
// Special is A+B, A=8(limits) or 16(nolimits)
// B=0 (not a big op), B=1 (\sum, \lim, \mathop), B=2(\int, \sin)
auto Cv3Helper::find_operator(math_style cms) -> name_positions {
    int what = 0;
    if (index != nullptr) what++;
    if (exponent != nullptr) what += 2;
    name_positions bl = cst_msub;
    if (what == 3)
        bl = cst_msubsup;
    else if (what == 2)
        bl = cst_msup;

    if (special > 16)
        special = 0; //  explicit \nolimits
    else if (special == 2)
        special = 0; // implicit \nolimits
    else if (special == 1 && cms != ms_D)
        special = 0; // implicit \displaylimits

    if (special == 0) return bl;
    if (special > 8) { // \lim\limits_1 : define movablelimits='false'
        Xml *q = p->spec_copy();
        if (q != nullptr) {
            p = q;
            p->add_att(np_movablelimits, np_false);
        }
    }
    if (what == 3)
        bl = cst_munderover;
    else if (what == 2)
        bl = cst_mover;
    else
        bl = cst_munder;
    return bl;
}

void Cv3Helper::add_kernel(math_style cms) {
    if ((index == nullptr) && (exponent == nullptr)) {
        res.push_back(p, ploc, ptype);
        return;
    }
    the_parser.my_stats.one_more_kernel();
    name_positions bl = find_operator(cms);
    if (the_main->prime_hack && exponent == math_data.get_mc_table(27) && bl == cst_msup) {
        bl       = cst_mrow;
        exponent = get_builtin(varprime_code);
    }
    Xml *tmp = new Xml(bl, nullptr);
    // case a_b_c. If we do nothing, the mathml interpreter will barf
    if ((p != nullptr) && !p->is_xmlc() && (p->has_name_of("msup") || p->has_name_of("msub") || p->has_name_of("msubsup")))
        p = new Xml(cst_mrow, p);
    if (ptype == mt_flag_small_l || ptype == mt_flag_small_r || ptype == mt_flag_small_m) {
        res.push_back(p, ploc, ptype);
        Xml *q = p;
        res.push_back(q, -1, mt_flag_dummy);
    }

    // case {\sum}_1
    tmp->add_tmp(gsl::not_null{p});
    tmp->push_back_unless_nullptr(xmlspace);
    if (index != nullptr) {
        tmp->add_tmp(gsl::not_null{index});
        tmp->push_back_unless_nullptr(xmlspace);
    }
    if (exponent != nullptr) {
        tmp->add_tmp(gsl::not_null{exponent});
        tmp->push_back_unless_nullptr(xmlspace);
    }
    p = tmp;
    res.push_back(p, -1, mt_flag_big);
}

auto Math::M_cv3(math_style cms) -> Math {
    Cv3Helper W(*this);
    for (;;) {
        W.reinit();
        for (;;) {
            W.find_kernel();
            if (W.state == 3) return W.finish();
            if (W.state == 2) continue;
            break;
        }
        for (;;) { // if state = 1, I have a kernel p, otherwise looking for it
            W.find_index(cms);
            if (W.state == 2) break;
        }
        W.add_kernel(cms);
    }
}

// returns the element with a new id, if it's a <mo> and has a np_movablelimits
// attributes; return null otherwise.
auto Xml::spec_copy() const -> Xml * {
    if (name != the_names["mo"]) return nullptr;
    AttList &X = id.get_att();
    auto     i = X.has_value(the_names["movablelimits"]);
    if (i < 0) return nullptr;
    Xml *res                                               = new Xml(name, nullptr);
    static_cast<std::vector<gsl::not_null<Xml *>> &>(*res) = *this;
    res->id.add_attribute(X, true);
    return res;
}

void Math::concat_space(Xml *res) {
    while (!empty()) {
        res->push_back_unless_nullptr(front().get_xml_val());
        pop_front();
        if (!empty()) res->push_back_unless_nullptr(xmlspace);
    }
}

void Math::concat(Xml *res) {
    while (!empty()) {
        res->push_back_unless_nullptr(front().get_xml_val());
        pop_front();
    }
}

auto MathElt::large2() const -> del_pos { return get_delimiter(get_chr()); }

// The list holds something like [a+b, et cl is ]
// returns <mfenced open=[ close=]>a+b</mfenced>
auto Math::large1(MathElt &cl, math_style cms) -> Xml * {
    the_parser.my_stats.one_more_large();
    size_t close = cl.large2();      // ok ??
    size_t open  = front().large2(); // ok ??
    bool   bad   = false;
    if (open == del_invalid || close == del_invalid) {
        bad = true;
        push_back(cl);
    } else
        pop_front();
    Math res0 = M_cv3(cms);
    res0.finish_translate1(the_parser.tracing_math());
    Xml *res1 = new Xml(cst_temporary, nullptr);
    res0.concat_space(res1);
    if (bad) {
        Xml *res = new Xml(cst_mrow, nullptr);
        res->add_tmp(gsl::not_null{res1});
        return res;
    }
    return math_data.make_mfenced(open, close, gsl::not_null{res1});
}

// This piece of code tries to add some <mfenced> commands
// whenever we have a \big something
void Math::handle_cmd_Big(math_style cms) {
    for (;;) {
        bool ok = handle_cmd_Big_aux(cms);
        if (!ok) return;
    }
}

auto Math::handle_cmd_Big_aux(math_style cms) -> bool {
    MathList res;
    Math     aux;
    bool     state     = false;
    bool     ok        = false; // true if we have added a fence
    bool     try_again = false; // true if formula has unused \big
    while (!empty()) {
        int        cmd = front().get_cmd();
        math_types t   = front().get_xmltype();
        if (cmd == math_xml_cmd && t == mt_flag_large_l) {
            if (state) {
                try_again = true;
                res.splice(res.end(), aux.value);
            }
            aux.push_back(front());
            state = true;
        } else if (cmd == math_xml_cmd && t == mt_flag_large_r) {
            if (!state) {
                try_again = true;
                res.push_back(front());
            } else {
                Xml *x = aux.large1(front(), cms);
                res.push_back(MathElt(x, -1, mt_flag_big));
                aux.clear();
                ok    = true;
                state = false;
            }
        } else if (!state)
            res.push_back(front());
        else
            aux.push_back(front());
        pop_front();
    }
    value.swap(res);
    value.splice(value.end(), aux.value);
    return ok && try_again;
}

auto Math::convert_math(math_style k) -> Xml * { return M_cv(k, 1).value; }

// Removes an an initial group that is the consequence of \refstepcounter
void Math::remove_initial_group() {
    bool initial_relax = false;
    auto B             = value.begin();
    auto E             = value.end();
    if (B == E) return;
    if (B->get_cmd() == relax_cmd) {
        initial_relax = true;
        ++B;
    }
    if (B == E) return;
    if (B->get_cmd() != math_list_cmd) return;
    if (B->get_lcmd() != math_open_cd) return;
    Math &X = B->get_list();
    if (!X.value.empty()) return;
    if (initial_relax) value.pop_front();
    value.pop_front();
}

void tralics_ns::boot_math(bool mv) {
    xmlspace = new Xml(Istring(" "));
    math_data.boot();
    if (mv) {
        int w = (2 << 15) - 1;
        the_parser.word_define(mathprop_ctr_code, w, true);
    }
}
