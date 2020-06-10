#include "txstack.h"

void Stack::implement_cit(const std::string &b1, const Istring &b2, const std::string &a, const std::string &c) {
    add_att_to_last(the_names["userid"], Istring(b1));
    add_att_to_last(the_names["id"], b2);
    add_att_to_last(the_names["key"], Istring(a));
    add_att_to_last(the_names["from"], Istring(c));
}
