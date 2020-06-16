#include "tralics/SaveAux.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"
#include "tralics/util.h"

namespace {
    Xml *the_box_to_end;
} // namespace

// Consider the case of {\setbox0=\hbox{...}}
// We must call box_end, the function that fills box0 with the current box.
// The number of the box is set by SaveAuxBoxend into the_box_position
// But this function cannot call box_end (because we are still in the {...}
// group. Hence box_end must be called after cur_level is decremented.
SaveAuxBoundary::~SaveAuxBoundary() {
    P.decr_cur_level();
    if (the_box_position >= 0) {
        if (the_box_to_end != nullptr) the_box_to_end->remove_last_empty_hi();
        P.box_end(the_box_to_end, to_unsigned(the_box_position));
        the_box_position = -1;
    }
}

void SaveAuxBoundary::dump(int n) {
    String s = val == bt_semisimple ? "semi simple" : bt_to_string(val);
    the_log << "### " << s << " group (level " << n << ") entered at line " << line << "\n";
}

// Restore box. Called in the case {\setbox0=\hbox{...}}
// when we see the first closing brace. The box just created will be put in
// box0.
SaveAuxBoxend::~SaveAuxBoxend() {
    P.flush_buffer();
    P.the_stack.pop(the_names["hbox"]);
    the_box_to_end   = val;
    the_box_position = to_signed(pos);
}

// This done when we restore an integer value
SaveAuxInt::~SaveAuxInt() {
    bool rt = P.eqtb_int_table[pos].level != 1;
    if (rt) P.eqtb_int_table[pos] = {val, level};
}

// This done when we restore a string value
//
SaveAuxString::~SaveAuxString() {
    bool rt = P.eqtb_string_table[pos].level != 1;
    if (rt) P.eqtb_string_table[pos] = {val, level};
}

// This done when we restore a dimension value
SaveAuxDim::~SaveAuxDim() {
    bool rt = P.eqtb_dim_table[pos].level != 1;
    if (rt) P.eqtb_dim_table[pos] = {val, level};
}

// Restore glue
SaveAuxGlue::~SaveAuxGlue() {
    bool rt = P.glue_table[pos].level != 1;
    if (rt) P.glue_table[pos] = {val, level};
}

// Restore command. We have to take care to free memory for user commands.
SaveAuxCmd::~SaveAuxCmd() {
    auto lvl = P.hash_table.eqtb[cs].level;
    if (lvl == 1) { // retain old value, so kill val
        if (val.is_user()) P.mac_table.delete_macro_ref(val.chr);
    } else {
        if (P.hash_table.eqtb[cs].is_user()) // kill cur and change
            P.mac_table.delete_macro_ref(P.hash_table.eqtb[cs].chr);
        P.hash_table.eqtb[cs].set(val, level);
    }
}

// Restore token list.
SaveAuxToken::~SaveAuxToken() {
    bool rt = P.toks_registers[pos].level != 1;
    if (rt) P.toks_registers[pos] = {val, level};
}

// Restore box. Called in the case {\setbox0=\hbox{...}}
// when we see the last closing brace. This may restore box0.
SaveAuxBox::~SaveAuxBox() {
    bool rt = P.box_table[pos].level != 1;
    if (rt) P.box_table[pos] = {val, level};
}

// \aftergroup\foo{}: When the group is finished, the token \foo is
// pushed back into the input stream.
SaveAuxAftergroup::~SaveAuxAftergroup() { P.back_input(val); }

// When we pop a level, the current font may change.
SaveAuxFont::~SaveAuxFont() {
    P.flush_buffer();
    P.cur_font.set_old_from_packed();
    P.cur_font.set_level(level);
    P.cur_font.set_packed(val);
    P.cur_font.set_color(color);
    P.cur_font.unpack();
    P.font_has_changed1();
    P.cur_font.update_old();
}
