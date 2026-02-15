#include "tralics/Bbl.h"
#include "tralics/Buffer.h"

void Bbl::flush() {
    buffer.append("\n");
    Buffer tmp(buffer);
    file << tmp.convert_to_log_encoding();
    lines.insert(buffer, true);
    buffer.clear();
}
