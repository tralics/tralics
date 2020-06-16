#include "tralics/SaveAux.h"
#include "tralics/Logger.h"
#include "tralics/globals.h"
#include "tralics/util.h"

namespace {
    Xml *the_box_to_end;
}

// If you say: \let\foo=1 \global\let\foo=2
// the first \let saves the old value on the stack. The \global makes this
// irrelevant.
void SaveAux::restore_or_retain(bool rt, String s) {
    Logger::finish_seq();
    the_log << "+stack: " << (rt ? "restoring " : "retaining ") << s;
}

// Consider the case of {\setbox0=\hbox{...}}
// We must call box_end, the function that fills box0 with the current box.
// The number of the box is set by SaveAuxBoxend into the_box_position
// But this function cannot call box_end (because we are still in the {...}
// group. Hence box_end must be called after cur_level is decremented.
void SaveAuxBoundary::unsave(bool trace) {
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: level - " << P.get_cur_level() << " for " << val << " from line " << line << "\n";
    }
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
void SaveAuxBoxend::unsave(bool trace) {
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: finish a box of type " << pos << "\n";
    }
    P.flush_buffer();
    P.the_stack.pop(the_names["hbox"]);
    the_box_to_end   = val;
    the_box_position = to_signed(pos);
}

// This done when we restore an integer value
void SaveAuxInt::unsave(bool trace) {
    bool rt = P.eqtb_int_table[pos].level != 1;
    if (trace) {
        restore_or_retain(rt, "\\");
        CmdChr tmp(assign_int_cmd, subtypes(pos));
        the_log << tmp.name() << "=" << val << ".\n";
    }
    if (rt) P.eqtb_int_table[pos] = {val, level};
}

// This done when we restore a string value
//
void SaveAuxString::unsave(bool trace) {
    bool rt = P.eqtb_string_table[pos].level != 1;
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: restoring " << save_string_name(pos) << "=" << val << ".\n";
    }
    if (rt) P.eqtb_string_table[pos] = {val, level};
}

// This done when we restore a dimension value
void SaveAuxDim::unsave(bool trace) {
    bool rt = P.eqtb_dim_table[pos].level != 1;
    if (trace) {
        restore_or_retain(rt, "\\");
        CmdChr tmp(assign_dimen_cmd, subtypes(pos));
        the_log << tmp.name() << "=" << val << ".\n";
    }
    if (rt) P.eqtb_dim_table[pos] = {val, level};
}

// Restore glue
void SaveAuxGlue::unsave(bool trace) {
    bool rt = P.glue_table[pos].level != 1;
    if (trace) {
        Thbuf1 << bf_reset << val;
        if (pos >= thinmuskip_code) Thbuf1.pt_to_mu();
        restore_or_retain(rt, "\\");
        CmdChr tmp(assign_glue_cmd, subtypes(pos));
        the_log << tmp.name() << "=" << Thbuf1 << ".\n";
    }
    if (rt) P.glue_table[pos] = {val, level};
}

// Restore command. We have to take care to free memory for user commands.
void SaveAuxCmd::unsave(bool trace) {
    int lvl = P.hash_table.eqtb[cs].level;
    if (trace) {
        String S = lvl == 1 ? "retaining " : (val.is_undef() ? "killing " : "restoring ");
        Logger::finish_seq();
        the_log << "+stack: " << S << Token(cs + eqtb_offset);
        if (lvl > 1 && !val.is_undef()) {
            the_log << "=";
            if (val.is_user())
                P.token_for_show(val);
            else
                Parser::print_cmd_chr(val);
        }
        the_log << ".\n";
    }
    if (lvl == 1) { // retain old value, so kill val
        if (val.is_user()) P.mac_table.delete_macro_ref(val.chr);
    } else {
        if (P.hash_table.eqtb[cs].is_user()) // kill cur and change
            P.mac_table.delete_macro_ref(P.hash_table.eqtb[cs].chr);
        P.hash_table.eqtb[cs].set(val, level);
    }
}

// Restore token list.
void SaveAuxToken::unsave(bool trace) {
    bool rt = P.toks_registers[pos].level != 1;
    if (trace) {
        restore_or_retain(rt, "\\");
        CmdChr tmp(assign_toks_cmd, subtypes(pos));
        the_log << tmp.name() << "=" << val << ".\n";
    }
    if (rt) P.toks_registers[pos] = {val, level};
}

// Restore box. Called in the case {\setbox0=\hbox{...}}
// when we see the last closing brace. This may restore box0.
void SaveAuxBox::unsave(bool trace) {
    bool rt = P.box_table[pos].level != 1;
    if (trace) {
        restore_or_retain(rt, "\\box");
        the_log << pos << ".\n";
    }
    if (rt) P.box_table[pos] = {val, level};
}

// \aftergroup\foo{}: When the group is finished, the token \foo is
// pushed back into the input stream.
void SaveAuxAftergroup::unsave(bool trace) {
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: after group " << value << "\n";
    }
    P.back_input(value);
}

// This is executed when we pop a slot containing restore-foo-env
void SaveAuxEnv::unsave(bool trace) {
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: ending environment " << name << "; resuming " << oldname << ".\n";
    }
    P.set_cur_env_name(oldname, line);
}

// When we pop a level, the current font may change.
void SaveAuxFont::unsave(bool trace) {
    P.flush_buffer();
    P.cur_font.set_old_from_packed();
    P.cur_font.set_level(level);
    P.cur_font.set_packed(value);
    P.cur_font.set_color(color);
    P.cur_font.unpack();
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: restoring current font " << P.cur_font << ".\n";
    }
    P.font_has_changed1();
    P.cur_font.update_old();
}
