#include "tralics/Image.h"
#include "txinline.h"

// This checks all possible extensions and remembers them
void Image::check_existence() {
    flags = 0;
    if (tralics_ns::file_exists(name + ".ps")) flags |= 1;
    if (tralics_ns::file_exists(name + ".eps")) flags |= 2;
    if (tralics_ns::file_exists(name + ".epsi")) flags |= 4;
    if (tralics_ns::file_exists(name + ".epsf")) flags |= 8;
    if (tralics_ns::file_exists(name + ".pdf")) flags |= 16;
    if (tralics_ns::file_exists(name + ".png")) flags |= 32;
    if (tralics_ns::file_exists(name + ".gif")) flags |= 64;
}

// This checks that there is a unique source for the image
void Image::check(Buffer &B1, Buffer &B2) const {
    int a = (flags & 1) != 0 ? 1 : 0;
    int b = (flags & 2) != 0 ? 1 : 0;
    int c = (flags & 4) != 0 ? 1 : 0;
    int d = (flags & 8) != 0 ? 1 : 0;
    int e = a + b + c + d;
    if (e > 1) {
        if (!B1.empty()) B1.append(", ");
        B1.append(name);
    }
    if (flags == 0) {
        if (!B2.empty()) B2.append(", ");
        B2.append(name);
    }
}
