#include "tralics/Buffer.h"
#include "tralics/NameMapper.h"
#include "tralics/globals.h"
#include "txid.h"

void Xid::add_top_rule() const {
    add_attribute(the_names["cell_topborder"], the_names["true"]);
    if (in_hlinee) {
        add_attribute(the_names["border_top_width"], hlinee_width);
        if (have_above) add_attribute(the_names["top_rule_space_above"], hlinee_above);
        if (have_below) add_attribute(the_names["top_rule_space_below"], hlinee_below);
    }
}
void Xid::add_bottom_rule() const {
    add_attribute(the_names["cell_bottomborder"], the_names["true"]);
    if (in_hlinee) {
        add_attribute(the_names["border_bottom_width"], hlinee_width);
        if (have_above) add_attribute(the_names["bottom_rule_space_above"], hlinee_above);
        if (have_below) add_attribute(the_names["bottom_rule_space_below"], hlinee_below);
    }
}

// adds a span value of n to the current cell
void Xid::add_span(long n) const {
    if (n == 1) return;
    errbuf.clear();
    errbuf << std::to_string(n);
    add_attribute(the_names["cols"], Istring(errbuf));
}
