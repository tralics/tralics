#include "tralics/RealNumber.h"

// put x in this, but change the sign is *this<0
void RealNumber::from_int(long x) {
    if (negative) x = -x;
    if (x > 0) {
        ipart    = x;
        fpart    = 0;
        negative = false;
    } else {
        ipart    = -x;
        negative = true;
        fpart    = 0;
    }
}

// Assume that we have read x=0.142. i.e. k=3 digits  in the table.
// This computes y=x* 2^{17}, then (y+1)/2.
void RealNumber::convert_decimal_part(size_t k, const long *table) {
    long f = 0;
    while (k > 0) {
        k--;
        f = (f + (table[k] << 17)) / 10;
    }
    fpart = (f + 1) / 2;
}
