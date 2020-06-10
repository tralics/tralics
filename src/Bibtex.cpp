#include "tralics/Bibtex.h"
#include "txbib.h"
#include "txinline.h"
#include <spdlog/spdlog.h>

// This reads conditionally a file. Returns true if the file exists.
auto Bibtex::read0(Buffer &B, bib_from ct) -> bool {
    B.push_back(".bib");
    if (auto of = tralics_ns::find_in_path(B); of) {
        read(*of, ct);
        return true;
    }
    return false;
}

// This takes a file name. It handles the case where the file has a suffix
// like miaou+foot. Prints a warning if this is a bad name.
void Bibtex::read1(const std::string &cur) {
    Buffer &Tbuf = biblio_buf4;
    Tbuf.clear();
    Tbuf.push_back(cur);
    auto n = Tbuf.size();
    if (read0(Tbuf, from_year)) return;
    if (Tbuf.ends_with("+foot.bib")) {
        Tbuf.resize(n - 5);
        if (read0(Tbuf, from_foot)) return;
    }
    if (Tbuf.ends_with("+year.bib")) {
        Tbuf.resize(n - 5);
        if (read0(Tbuf, from_year)) return;
    }
    if (Tbuf.ends_with("+all.bib")) {
        Tbuf.resize(n - 4);
        if (read0(Tbuf, from_any)) return;
    }
    if (Tbuf.ends_with("+refer.bib")) {
        Tbuf.resize(n - 6);
        if (read0(Tbuf, from_refer)) return;
    }
    if (Tbuf.ends_with(".bib")) {
        Tbuf.resize(n - 4);
        if (read0(Tbuf, from_year)) return;
    }
    spdlog::warn("Bibtex Info: no biblio file {}", Tbuf);
}

// Handles one bib file for the raweb. Returns true if the file exists.
// Extension can be foot, year or refer. New in Tralics 2.9.3 it can be any
auto Bibtex::read2(bib_from pre) -> bool {
    Buffer &B = biblio_buf4;
    B.clear();
    B.push_back(no_year);
    if (pre == from_foot)
        B.push_back("_foot");
    else if (pre == from_refer)
        B.push_back("_refer");
    else if (pre == from_any)
        B.push_back("_all");
    B.push_back(default_year);
    return read0(B, pre);
}

// This loads all three files, if we are compiling the raweb. Otherwise,
// there is no default.
// We read apics2006_all.bib if this exists
void Bibtex::read_ra() {
    if (in_ra) {
        bbl.open();
        if (read2(from_any)) return;
        read2(from_year);
        read2(from_foot);
        read2(from_refer);
    }
}
