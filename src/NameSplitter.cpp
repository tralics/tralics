#include "tralics/NameSplitter.h"
#include "tralics/Bibtex.h"
#include "tralics/Logger.h"

// This handles all the names in the list.
void NameSplitter::handle_the_names() {
    bool   is_first_name = true;
    int    serial        = 1;
    size_t pos           = 1; // there is a space at position 0
    main_data.init(table);
    for (;;) {
        name_buffer.ptrs.b = pos;
        bool is_last_name  = name_buffer.find_and(table);
        auto k             = name_buffer.ptrs.b;
        main_data.init(pos, k);
        handle_one_name(is_first_name, is_last_name, serial);
        if (is_last_name) return;
        is_first_name = false;
        serial++;
        pos = k + 3;
    }
}

// Very complicated function. Assume that the names are Alpha,
// Bravo, Charlie, Delta, usw. The key will be
// Alp, AB, ABC, ABCD, ABC+, if there are 1,2,3,4,... authors.
// The name `others' is handled specially if last, not first.

void NameSplitter::handle_one_name(bool ifn, bool iln, int serial) {
    first_name.init(table);
    last_name.init(table);
    jr_name.init(table);
    main_data.remove_junk();
    auto   F  = main_data.first;
    auto   L  = main_data.last;
    size_t fc = 0, lc = 0, hm = 0;
    main_data.find_a_comma(fc, lc, hm);
    if (hm >= 2) {
        last_name.init(F, fc);
        jr_name.init(fc + 1, lc);
        first_name.init(lc + 1, L);
        if (hm > 2) {
            the_bibtex->err_in_entry("");
            log_and_tty << "too many commas (namely " << hm << ") in name\n" << name_buffer << ".\n";
        }
    } else if (hm == 1) {
        first_name.init(fc + 1, L);
        last_name.init(F, fc);
    } else { // no comma
        auto k = main_data.find_a_lower();
        if (k == L) {
            k = main_data.find_a_space();
            if (k == L) {
                main_data.invent_spaces();
                k = main_data.find_a_space();
            }
        }
        if (k == L)
            last_name.init(F, L);
        else {
            first_name.init(F, k);
            last_name.init(k + 1, L);
        }
    }
    first_name.remove_junk();
    last_name.remove_junk();
    jr_name.remove_junk();
    if (first_name.empty() && last_name.empty() && jr_name.empty()) {
        the_bibtex->err_in_entry("empty name in\n");
        log_and_tty << name_buffer << ".\n";
        return;
    }
    bool handle_key = want_handle_key(serial, iln);
    bool is_other   = is_this_other();
    if (is_other) {
        if (iln && !ifn) {
            biblio_buf1.push_back("\\cititem{etal}{}");
            biblio_buf2.push_back("etal");
            biblio_buf4.push_back("etal");
            if (handle_key) biblio_buf3.push_back("+");
        } else {
            biblio_buf1.push_back("\\bpers{}{}{others}{}");
            biblio_buf2.push_back("others");
            biblio_buf4.push_back("others");
            if (handle_key) biblio_buf3.push_back(iln ? "oth" : "o");
        }
        return;
    }
    if (handle_key) last_name.make_key(!(ifn && iln), biblio_buf3);
    biblio_buf1.push_back("\\bpers[");
    biblio_buf1 << first_name;
    biblio_buf1.push_back("]{");
    biblio_buf2.push_back(" ");
    first_name.print_first_name(biblio_buf1, biblio_buf2);
    biblio_buf1 << "}{}{" << last_name << "}{" << jr_name << "}";
    biblio_buf2 << " " << last_name << " " << jr_name << " ";
    biblio_buf4 << last_name << " " << jr_name << " ";
}

auto NameSplitter::want_handle_key(int s, bool last) -> bool {
    if (s < 4) return true;
    if (s > 4) return false;
    if (last) return true;
    biblio_buf3.push_back("+");
    return false;
}

auto NameSplitter::is_this_other() -> bool {
    if (!first_name.empty()) return false;
    if (!jr_name.empty()) return false;
    auto a = last_name.first;
    auto b = last_name.last;
    return b - a == 6 && name_buffer.substr(a, 6) == "others";
}
