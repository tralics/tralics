#include "tralics/Image.h"
#include "txinline.h"

// returns true if the file exists with extension s.
auto Image::extension_exists(const std::string &s) const -> bool { return tralics_ns::file_exists(name + '.' + s); }

// This checks all possible extensions and remembers them
void Image::check_existence() {
    flags = 0;
    if (extension_exists("ps")) flags |= 1;
    if (extension_exists("eps")) flags |= 2;
    if (extension_exists("epsi")) flags |= 4;
    if (extension_exists("epsf")) flags |= 8;
    if (extension_exists("pdf")) flags |= 16;
    if (extension_exists("png")) flags |= 32;
    if (extension_exists("gif")) flags |= 64;
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
