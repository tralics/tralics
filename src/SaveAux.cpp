#include "tralics/SaveAux.h"
#include "tralics/globals.h"
#include "tralics/util.h"
#include <spdlog/spdlog.h>

namespace {
    Xml *                 the_box_to_end;
    std::optional<size_t> the_box_position;
} // namespace

// Consider the case of {\setbox0=\hbox{...}}
// We must call box_end, the function that fills box0 with the current box.
// The number of the box is set by SaveAuxBoxend into the_box_position
// But this function cannot call box_end (because we are still in the {...}
// group. Hence box_end must be called after cur_level is decremented.
SaveAuxBoundary::~SaveAuxBoundary() {
    the_parser.decr_cur_level();
    if (the_box_position) {
        if (the_box_to_end != nullptr) the_box_to_end->remove_last_empty_hi();
        the_parser.box_end(the_box_to_end, *the_box_position);
        the_box_position.reset();
    }
}

void SaveAuxBoundary::dump(int n) {
    std::string s = val == bt_semisimple ? "semi simple" : bt_to_string(val);
    spdlog::trace("### {} group (level {}) entered at line {}", s, n, line);
}

// Restore box. Called in the case {\setbox0=\hbox{...}}
// when we see the first closing brace. The box just created will be put in
// box0.
SaveAuxBoxend::~SaveAuxBoxend() {
    the_parser.flush_buffer();
    the_stack.pop(the_names["hbox"]);
    the_box_to_end   = val;
    the_box_position = pos;
}

// This done when we restore an integer value
SaveAuxInt::~SaveAuxInt() {
    bool rt = eqtb_int_table[pos].level != 1;
    if (rt) eqtb_int_table[pos] = {val, level};
}

// This done when we restore a string value
//
SaveAuxString::~SaveAuxString() {
    bool rt = the_parser.eqtb_string_table[pos].level != 1;
    if (rt) the_parser.eqtb_string_table[pos] = {val, level};
}

// This done when we restore a dimension value
SaveAuxDim::~SaveAuxDim() {
    bool rt = the_parser.eqtb_dim_table[pos].level != 1;
    if (rt) the_parser.eqtb_dim_table[pos] = {val, level};
}

// Restore glue
SaveAuxGlue::~SaveAuxGlue() {
    bool rt = the_parser.glue_table[pos].level != 1;
    if (rt) the_parser.glue_table[pos] = {val, level};
}

// Restore command. We have to take care to free memory for user commands.
SaveAuxCmd::~SaveAuxCmd() {
    auto lvl = Hashtab::the_eqtb()[cs].level;
    if (lvl == 1) { // retain old value, so kill val
        if (val.is_user()) the_parser.mac_table.delete_macro_ref(val.chr);
    } else {
        if (Hashtab::the_eqtb()[cs].val.is_user()) // kill cur and change
            the_parser.mac_table.delete_macro_ref(Hashtab::the_eqtb()[cs].val.chr);
        Hashtab::the_eqtb()[cs] = {val, level};
    }
}

// Restore token list.
SaveAuxToken::~SaveAuxToken() {
    bool rt = the_parser.toks_registers[pos].level != 1;
    if (rt) the_parser.toks_registers[pos] = {val, level};
}

// Restore box. Called in the case {\setbox0=\hbox{...}}
// when we see the last closing brace. This may restore box0.
SaveAuxBox::~SaveAuxBox() {
    bool rt = the_parser.box_table[pos].level != 1;
    if (rt) the_parser.box_table[pos] = {val, level};
}

// \aftergroup\foo{}: When the group is finished, the token \foo is
// pushed back into the input stream.
SaveAuxAftergroup::~SaveAuxAftergroup() { the_parser.back_input(val); }

// When we pop a level, the current font may change.
SaveAuxFont::~SaveAuxFont() {
    the_parser.flush_buffer();
    the_parser.cur_font.set_old_from_packed();
    the_parser.cur_font.set_level(level);
    the_parser.cur_font.set_packed(val);
    the_parser.cur_font.set_color(color);
    the_parser.cur_font.unpack();
    the_parser.font_has_changed1();
    the_parser.cur_font.update_old();
}
