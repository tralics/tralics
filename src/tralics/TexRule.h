#pragma once
#include "AttList.h"
#include "ScaledInt.h"

struct TexRule {
    ScaledInt rule_w, rule_h, rule_d;

    void reset();
    void convert(AttList &res) const;
    void init_vrule() { rule_w = ScaledInt{26214}; }
    void init_hrule() {
        rule_h = ScaledInt{26214};
        rule_d = ScaledInt{0};
    }
    void adjust() {
        rule_h += rule_d;
        rule_d.neg();
    }
};
