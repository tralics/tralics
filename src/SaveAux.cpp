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
void SaveAuxBoundary::unsave(bool trace, Parser &P) {
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
void SaveAuxBoxend::unsave(bool trace, Parser &P) {
    if (trace) {
        Logger::finish_seq();
        the_log << "+stack: finish a box of type " << pos << "\n";
    }
    P.flush_buffer();
    P.the_stack.pop(the_names["hbox"]);
    the_box_to_end   = val;
    the_box_position = to_signed(pos);
}
