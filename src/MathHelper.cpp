#include "tralics/MathHelper.h"
#include "tralics/Logger.h"
#include "tralics/MainClass.h"
#include "tralics/MathDataP.h"
#include "tralics/Parser.h"
#include "tralics/Stack.h"
#include "tralics/globals.h"

namespace tralics_ns {
    auto math_env_props(subtypes c) -> int; // \todo elsewhere
} // namespace tralics_ns

using tralics_ns::math_env_props;

// Implements \cellattribute
void MathHelper::add_attribute(const std::string &a, const std::string &b, subtypes c) const {
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

// This is called before every math formula.
void MathHelper::reset(bool dual) {
    current_mode   = false;
    pos_att        = "0";
    seen_label     = false;
    warned_label   = false;
    label_val      = "";
    tag_list       = TokenList();
    is_tag_starred = false;
    math_env_ctr   = 0;
    all_env_ctr    = 0;
    cur_math_id    = the_stack.next_xid(nullptr);
    cur_formula_id = the_stack.next_xid(nullptr);
    if (dual)
        cur_texmath_id = the_stack.next_xid(nullptr);
    else
        cur_texmath_id = cur_math_id;
    multi_labels.clear();
    multi_labels_type.clear();
    last_ml_pos = 0;
}

// Sets the type (display or not), and prepares the pos attribute.
void MathHelper::set_type(bool b) {
    current_mode = b;
    pos_att      = b ? "inline" : "display";
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

// nnewt item in the list of tags
void MathHelper::add_tag(TokenList &L) {
    if (!tag_list.empty()) tag_list.push_back(hash_table.comma_token);
    tag_list.splice(tag_list.end(), L);
}

// passes the list of tag as argument to \x@tag or \y@tag
void MathHelper::handle_tags() {
    TokenList L = tag_list;
    token_ns::remove_first_last_space(L);
    L.brace_me();
    L.push_front(is_tag_starred ? hash_table.ytag1_token : hash_table.xtag1_token);
    the_parser.back_input(L);
    tag_list = TokenList();
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
    auto        n = multi_labels.size();
    int         l = 1;
    std::string B;
    static bool warned = false;
    for (size_t i = 0; i < n; i++) {
        int v = multi_labels_type[i];
        if (v == 0)
            l++;
        else if (v == -1) {
            B = "Multiple \\label " + multi_labels[i];
            if (eqnum_status == 1)
                B += " for the current the formula";
            else {
                B += fmt::format(" on row {} of the formula", l);
                if (!warned) B += "\n(at most one \\label and at most one \\tag allowed per row)";
                warned = true;
            }
            the_parser.parse_error(the_parser.err_tok, B, "duplicate label");
        }
        if (v == -2 || v == -3) {
            B = "Multiple \\tag " + multi_labels[i];
            if (eqnum_status == 1)
                B += " for the current formula";
            else {
                B += fmt::format(" on row {} of formula", l);
                if (!warned) B += "\n(at most one \\label and at most one \\tag allowed per row)";
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
        std::string id = next_label_id();
        the_stack.create_new_anchor(row->id, id, std::string(tag));
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
