#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "txinline.h"
#include <fmt/format.h>

auto operator<<(std::ostream &X, const Image &Y) -> std::ostream &; // \todo elsewhere

// In case of error, we add the current line number as attribute
// via this function
auto Parser::cur_line_to_istring() const -> Istring { return Istring(fmt::format("{}", get_cur_line())); }

// This is the TeX command \string ; if esc is false, no escape char is inserted
void Parser::tex_string(Buffer &B, Token T, bool esc) const {
    if (T.not_a_cmd())
        B.push_back(T.char_val());
    else {
        auto x = T.val;
        if (esc && x >= single_offset) B.insert_escape_char_raw();
        if (x >= hash_offset)
            B.push_back(hash_table[T.hash_loc()]);
        else if (x < first_multitok_val)
            B.push_back(T.char_val());
        else
            B.push_back(null_cs_name());
    }
}

// Enter a new image file, if ok is false, do not increase the occ count
void Parser::enter_file_in_table(const std::string &nm, bool ok) {
    for (auto &X : the_images) {
        if (X.name == nm) {
            if (ok) X.occ++;
            return;
        }
    }
    the_images.emplace_back(nm, ok ? 1 : 0);
}

// finish handling the images,
void Parser::finish_images() {
    if (the_images.empty()) return;
    std::string   name = tralics_ns::get_short_jobname() + ".img";
    String        wn   = tralics_ns::get_out_dir(name);
    std::ofstream fp(wn);
    fp << "# images info, 1=ps, 2=eps, 4=epsi, 8=epsf, 16=pdf, 32=png, 64=gif\n";
    check_image1.reset();
    check_image2.reset();
    for (auto &the_image : the_images) {
        if (the_image.occ != 0) {
            the_image.check_existence();
            the_image.check();
            fp << the_image;
        }
    }
    if (the_images.empty())
        main_ns::log_or_tty << "There was no image.\n";
    else
        main_ns::log_or_tty << fmt::format("There were {} images.\n", the_images.size());
    if (!check_image1.empty()) main_ns::log_or_tty << "Following images have multiple PS source: " << check_image1.c_str() << ".\n";
    if (!check_image2.empty()) main_ns::log_or_tty << "Following images not defined: " << check_image2.c_str() << ".\n";
}
