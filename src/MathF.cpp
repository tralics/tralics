#include "tralics/MathF.h"
#include "tralics/Parser.h"

void MathF::pop_last(Xml *xval) {
    Xml *p{nullptr};
    if (!in_mrow && !res.empty()) {
        p = res.back().remove_prefix();
        res.pop_back();
    } else if (in_mrow && (t != nullptr))
        p = t->remove_last();
    else
        return; // should  not happen
    xval->replace_first(p);
}

// This is what we do when we see a start or end of mrow;
// state is true inside; next_change is position of next status change
void MathF::change_state() {
    in_mrow = !in_mrow;
    if (in_mrow)
        next_change = next_finish;
    else if (aux.empty())
        next_change = next_finish = -1;
    else {
        std::tie(next_change, next_finish) = aux.front();
        aux.pop_front();
    }
}

// This emits a mrow if needed
void MathF::handle_t() {
    if (in_mrow) {
        res.push_back(MathElt(new Xml(the_names["mrow"], t), -1, t_big ? mt_flag_big : mt_flag_small));
        t = nullptr;
    }
}

void MathF::push_in_t(Xml *x) {
    if (t == nullptr) t = new Xml(the_names["temporary"], nullptr);
    t->push_back_unless_nullptr(x);
}

void MathF::finish(std::list<MathElt> &value) {
    if ((t != nullptr) && !t->all_empty()) the_parser.signal_error("internal bug in finish_translate");
    value.swap(res);
}
