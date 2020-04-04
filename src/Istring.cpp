#include "txinline.h"

Istring::Istring(name_positions N) : value(the_names[N].value) {}

// True if the string holds only white space or &nbsp;
auto Istring::only_space_spec() const -> bool {
    int    i = 0;
    String s = c_str();
    while (s[i] != 0) {
        if (is_space(s[i]))
            i++;
        else if ((strncmp(s + i, "&nbsp;", 6) == 0) || (strncmp(s + i, "&#xA0;", 6) == 0) || (strncmp(s + i, "\302\240", 2) == 0))
            i += 6;
        else
            return false;
    }
    return true;
}
