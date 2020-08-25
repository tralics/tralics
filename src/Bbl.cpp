#include "tralics/Bbl.h"

void Bbl::flush() {
    append("\n");
    file << convert_to_log_encoding();
    lines.insert(*this, true);
    clear();
}
