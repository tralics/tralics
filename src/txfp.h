#pragma once
// -*- C++ -*-
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

#include "tralics/Token.h"
#include <array>

class Buffer;

// for the fp package. this implements fixed point arithmetic
class FpNum {
public:
    bool                        sign{true}; // sign of the number
    std::array<unsigned int, 4> data{};     // the digits

    FpNum(Digit a, Digit b, Digit c, Digit d) {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
        sign    = true;
    }
    FpNum() { data[0] = data[1] = data[2] = data[3] = 0; }
    void init(Digit a, Digit b, Digit c, Digit d) {
        data[0] = a;
        data[1] = b;
        data[2] = c;
        data[3] = d;
    }
    [[nodiscard]] auto is_zero() const -> bool;
    void               reset() { data[0] = data[1] = data[2] = data[3] = 0; }
    void               correct_sign();
    auto               create(Buffer &B) -> bool;
    [[nodiscard]] auto to_list() const -> TokenList;
    void               add(FpNum Y);
    static void        set_xmax(const Digit *x, int &xmin, int &xmax); // \todo size_t& ? std::pair<size_t,size_t> even better
    static void        prop_carry(Digit *z);
    void               finish_mul(bool xs, Digit *z);
    void               unsplit_mul4(const Digit *z);
    void               add_int(FpNum Y);
    void               div(int n);
    void               sin0();
    void               cos0();
    void               sincos_loop(int n, FpNum x, FpNum p);
    void               add_abs(FpNum Y);
    void               sub_abs(FpNum Y);
    void               neg_sub_abs(FpNum Y);
    void               mod2pi();
    auto               count_times(FpNum Y) -> size_t;
    [[nodiscard]] auto to_string() const -> String;
    void               neg();
    void               mul_split(unsigned int *T) const;
    void               mul_by_10();
    void               mul_by_2();
    void               mul(FpNum X);
    void               div(FpNum X);
    void               div_by_10();
    void               add_digit(size_t y);
    void               sgn();
    void               truncate(long n);
    void               round(long n);
    void               round0();
    void               exec_ln();
    void               exec_ln_a();
    void               exec_exp();
    auto               large_exp() -> bool;
    void               mul(FpNum X, FpNum Y);
    void               mul(FpNum X, int y);
    void               div(FpNum X, FpNum Y);
    void               pow(FpNum X, FpNum Y, subtypes i);
    void               divide2();
    auto               octand() -> int;
    void               sin1(int k);
    void               cos1(int k);
    void               mean(FpNum a, FpNum b);
    auto               sincos_transform() -> bool;
    auto               arcsincos_loop() -> bool;
    void               trigo_xy(int x);
    void               trigo_inv();
    void               trigo_sqrt();
    void               arctan();
    void               arctan1();
    void               sqrt();
    void               croot();
    void               sub(FpNum X, FpNum Y);
    void               add(FpNum X, FpNum Y);
    void               cosine();
    void               random();
};
