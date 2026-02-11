#pragma once
#include <ostream>
#include <string>

// A class to count words... TODO: where is that used? TODO: chained list???
struct WordList {
    WordList   *next;
    std::string name;
    size_t      hash;
    int         freq{1};

public:
    WordList(std::string s, size_t h, WordList *N) : next(N), name(std::move(s)), hash(h) {}

    [[nodiscard]] auto is_here(const std::string &s, size_t h) const -> bool { return (hash == h) && (name == s); }

    auto dump(std::ostream &X, int i) const -> bool {
        if (freq == i) {
            X << freq << " " << name << "\n";
            return true;
        }
        return false;
    }
};
