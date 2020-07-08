#pragma once
#include "../txinline.h"

class Bbl {
private:
    Buffer        B;
    std::ofstream file;
    bool          too_late{false};

public:
    friend class BibEntry;

    std::string name;
    LineList    lines;

    void               newline();
    void               push_back(String s) { B.append(s); }
    void               push_back(const std::string &s) { B.append(s); }
    void               push_back_braced(const std::string &s) { B.push_back_braced(s); }
    auto               non_empty_buf() -> bool { return !B.empty(); }
    void               reset() { B.clear(); }
    void               reset_lines() { lines.clear(); }
    [[nodiscard]] auto is_too_late() const -> bool { return too_late; }
    void               finish() { too_late = true; } // \todo should this be called ~Bbl ?
    void               open() {
        if (!file.is_open()) file = tralics_ns::open_file(name, true);
    }
};

inline Bbl bbl;
