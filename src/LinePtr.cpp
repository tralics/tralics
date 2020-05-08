#include "tralics/LinePtr.h"
#include "tralics/Logger.h"

void LinePtr::change_encoding(long wc) {
    if (wc >= 0 && wc < to_signed(max_encoding)) {
        encoding = to_unsigned(wc);
        Logger::finish_seq();
        the_log << "++ Input encoding changed to " << wc << " for " << file_name << "\n";
    }
}
