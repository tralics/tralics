#include "tralics/Bbl.h"

// This flushes the buffer.
void Bbl::newline() {
    B.push_back("\n");
    file << B.convert_to_log_encoding();
    lines.insert(B, true);
    B.clear();
}
