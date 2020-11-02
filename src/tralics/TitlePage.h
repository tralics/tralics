#pragma once
#include <vector>

struct TitlePageAux;
class Xml;

struct TitlePage : public std::vector<Xml *> {
    size_t                    len2{1};      // size of bigtable and *this
    bool                      valid{false}; // is this initialised and not killed ?
    int                       size{0};      // allocated size of bigtable
    std::vector<TitlePageAux> bigtable;     // the table
    int                       state{};      // current state of the parser

    [[nodiscard]] auto get_len2() const -> size_t { return len2; }
    [[nodiscard]] auto is_valid() const -> bool { return valid; }
    [[nodiscard]] auto find_UR(const std::string &s, const std::string &name) const -> size_t;
    [[nodiscard]] auto find_cmd(const std::string &s) const -> size_t;

    auto get_bigtable(size_t k) -> TitlePageAux & { return bigtable[k]; }
    void make_invalid() { valid = false; }
    void make_valid() { valid = true; }
    void start_thing(bool verbose);
    void parse();
    auto increase_data() -> size_t { return len2++; }
    void check_size();
};

inline TitlePage Titlepage;
