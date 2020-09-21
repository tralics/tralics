#pragma once
#include "TokenList.h"
#include "types.h"
#include <array>

class Buffer;

struct FpNum {
    bool                        sign{true}; // sign of the number
    std::array<unsigned int, 4> data{};     // the digits

    FpNum(Digit a, Digit b, Digit c, Digit d) : data{a, b, c, d} {}
    FpNum() : FpNum(0, 0, 0, 0) {}

    void set(Digit a, Digit b, Digit c, Digit d) { data = {a, b, c, d}; }
    void reset() { set(0, 0, 0, 0); }

    [[nodiscard]] auto is_zero() const -> bool;
    [[nodiscard]] auto to_list() const -> TokenList;
    [[nodiscard]] auto to_string() const -> std::string;

    auto arcsincos_loop() -> bool;
    auto count_times(FpNum Y) -> size_t;
    auto create(Buffer &B) -> bool;
    auto large_exp() -> bool;
    auto octand() -> int;
    auto sincos_transform() -> bool;
    void add_abs(FpNum Y);
    void add_digit(size_t y);
    void add_int(FpNum Y);
    void add(FpNum X, FpNum Y);
    void add(FpNum Y);
    void arctan();
    void arctan1();
    void correct_sign();
    void cos0();
    void cos1(int k);
    void cosine();
    void croot();
    void div_by_10();
    void div(FpNum X, FpNum Y);
    void div(FpNum X);
    void div(int n);
    void divide2();
    void exec_exp();
    void exec_ln_a();
    void exec_ln();
    void finish_mul(bool xs, std::array<Digit, 24> &z);
    void mean(FpNum a, FpNum b);
    void mod2pi();
    void mul_by_10();
    void mul_by_2();
    void mul_split(std::array<Digit, 12> &T) const;
    void mul(FpNum X, FpNum Y);
    void mul(FpNum X, int y);
    void mul(FpNum X);
    void neg_sub_abs(FpNum Y);
    void neg();
    void pow(FpNum X, FpNum Y, subtypes i);
    void random();
    void round(long n);
    void round0();
    void sgn();
    void sin0();
    void sin1(int k);
    void sincos_loop(int n, FpNum x, FpNum p);
    void sqrt();
    void sub_abs(FpNum Y);
    void sub(FpNum X, FpNum Y);
    void trigo_inv();
    void trigo_sqrt();
    void trigo_xy(int x);
    void truncate(long n);
    void unsplit_mul4(const Digit *z);
};
