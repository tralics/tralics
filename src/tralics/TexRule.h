#pragma once
#include "AttList.h"
#include "ScaledInt.h"

struct TexRule {
    ScaledInt rule_w, rule_h, rule_d;

    void reset();
    void convert(AttList &res) const;
    void init_vrule() { rule_w = 26214; }
    void init_hrule() {
        rule_h = 26214;
        rule_d = 0;
    }
    void adjust() {
        rule_h += rule_d;
        rule_d.neg();
    }
};
