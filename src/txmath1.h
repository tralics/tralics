#pragma once
#include "tralics/MathPAux.h"
#include "txmath.h"

// -*- C++ -*-
// TRALICS, copyright (C)INRIA/apics (Jose' Grimm) 2006, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Helper for finding start and end of <mrow>
class MathF {
    bool      state{true};
    bool      t_big{};
    int       next_change{-1};
    int       next_finish{-1};
    MathQList aux;
    MathList  res;
    Xml *     t{};

public:
    [[nodiscard]] auto in_mrow() const -> bool { return state; }
    [[nodiscard]] auto is_next_change(int i) const -> bool { return i == next_change; }
    auto               get_aux() -> MathQList & { return aux; }
    void               change_state();
    void               make_t_big() { t_big = true; }
    void               handle_t();
    void               push_in_t(Xml *x);
    void               push_in_res(const MathElt &x) { res.push_back(x); }
    void               push_in_res(Xml *x) { res.push_back(MathElt(x, -1, mt_flag_small)); }
    void               reset() {
        t     = nullptr;
        state = true;
        t_big = false;
    }
    void finish(MathList &value);
    void pop_last(Xml *xval);
};
