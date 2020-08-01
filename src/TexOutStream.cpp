#include "tralics/TexOutStream.h"
#include "txinline.h"

TexOutStream::TexOutStream() {
    for (bool &i : write_open) i = false;
}

// This closes an output channel.
void TexOutStream::close(size_t chan) {
    if ((chan <= max_openout) && write_open[chan]) {
        write_file[chan].close();
        write_open[chan] = false;
    }
}

// This opens an output channel.
// What if the file cannot be opened ?
void TexOutStream::open(size_t chan, const std::string &fn) {
    if (chan < 0 || chan > max_openout) return; // This cannot happen
    write_file[chan] = std::ofstream(tralics_ns::get_out_dir(fn));
    write_open[chan] = true;
}
