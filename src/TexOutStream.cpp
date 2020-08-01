#include "tralics/TexOutStream.h"
#include "txinline.h"

// This closes an output channel.
void TexOutStream::close(size_t chan) {
    if ((chan <= max_openout) && write_file[chan].is_open()) { write_file[chan].close(); }
}

// This opens an output channel.
// What if the file cannot be opened ?
void TexOutStream::open(size_t chan, const std::string &fn) {
    if (chan < 0 || chan > max_openout) return; // This cannot happen
    write_file[chan] = std::ofstream(tralics_ns::get_out_dir(fn));
}
