#pragma once
#include "TitlePageFullLine.h"
#include "enums.h"
#include <string>

class Xml;

// data for a titlepage item
struct TitlePageAux {
    std::string T1, T2, T3, T4; // the four strings
    size_t      idx{0};         // index into titlepage::Data
    size_t      xflags{0};      // flags associated to the object
    tpi_vals    type{tpi_zero}; // type of object

    TitlePageAux() = default;

    explicit TitlePageAux(const TitlePageFullLine &X)
        : T1(X.item1.value), T2(X.item2.value), T3(X.item3.value), T4(X.item4.value), xflags(X.flags) {}

    [[nodiscard]] auto get_flags2() const -> size_t { return 32U * (xflags / 32U); }
    [[nodiscard]] auto has_u_flags() const -> bool { return (xflags & tp_u_flag) != 0; }
    [[nodiscard]] auto has_p_flags() const -> bool { return (xflags & tp_p_flag) != 0; }
    [[nodiscard]] auto has_e_flags() const -> bool { return (xflags & tp_e_flag) != 0; }
    [[nodiscard]] auto has_q_flags() const -> bool { return (xflags & tp_q_flag) != 0; }
    [[nodiscard]] auto has_plus_flags() const -> bool { return (xflags & tp_plus_flag) != 0; }
    [[nodiscard]] auto find_UR(const std::string &s, size_t n) const -> size_t;
    [[nodiscard]] auto find_cmd(const std::string &s) const -> bool;

    auto classify(tpi_vals w, int &state) -> bool;
    auto convert(int i) -> Xml *;
    auto convert(int i, Xml *r) -> Xml *;
    auto convert(int i, const std::string &s) -> Xml *;
    void dump(size_t k) const;
    void exec_start(size_t k);
    void exec_post() const;
    void exec(size_t v, bool vb);
    auto increment_flag() -> bool;
    void decode_flags() const;

    void move_T1T2(std::string x) {
        T1 = T2;
        T2 = std::move(x);
    }
};
