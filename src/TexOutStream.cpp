#include "tralics/TexOutStream.h"
#include "tralics/util.h"

// This closes an output channel.
void TexOutStream::close(size_t chan) {
    if ((chan <= max_openout) && at(chan).is_open()) { at(chan).close(); }
}

// This opens an output channel.
// What if the file cannot be opened ?
void TexOutStream::open(size_t chan, const std::string &fn) {
    if (chan < 0 || chan > max_openout) return; // This cannot happen
    at(chan) = std::ofstream(get_out_dir(fn));
}
