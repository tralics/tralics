#include "tralics/NameMapper.h"
#include "tralics/MainClass.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"

auto NameMapper::operator[](const std::string &name) const -> Istring {
    if (name == "cst_invalid") return Istring(); // Only non-defined entry \todo get rid of it
    if (auto i = dict.find(name); i != dict.end()) return i->second;
    return Istring(name);
}

void NameMapper::set(const std::string &name, const std::optional<std::string> &value) { dict[name] = value ? Istring(*value) : Istring(); }

auto NameMapper::cstf(size_t i) -> Istring {
    static std::array<std::string, 15> fonts = {"normal",
                                                "cstf_upright",
                                                "bold",
                                                "italic",
                                                "bold-italic",
                                                "script",
                                                "bold-script",
                                                "fraktur",
                                                "double-struck",
                                                "bold-fraktur",
                                                "sans-serif",
                                                "bold-sans-serif",
                                                "sans-serif-italic",
                                                "sans-serif-bold-italic",
                                                "monospace"};
    return (*this)[fonts[i]];
}

auto NameMapper::mml(size_t i) -> Istring {
    static std::array<std::string, 15> fonts = {"mml@font@normal",
                                                "mml@font@upright",
                                                "mml@font@bold",
                                                "mml@font@italic",
                                                "mml@font@bolditalic",
                                                "mml@font@script",
                                                "mml@font@boldscript",
                                                "mml@font@fraktur",
                                                "mml@font@doublestruck",
                                                "mml@font@boldfraktur",
                                                "mml@font@sansserif",
                                                "mml@font@boldsansserif",
                                                "mml@font@sansserifitalic",
                                                "mml@font@sansserifbolditalic",
                                                "mml@font@monospace"};
    return (*this)[fonts[i]];
}

auto NameMapper::mi(size_t i) -> Istring {
    static std::array<std::string, 8> list = {"mi", "mn", "mo", "ci", "cn", "csymbol", "cst_nothing", "mmultiscripts"};
    return (*this)[list[i]];
}

auto NameMapper::center(size_t i) -> Istring {
    static std::array<std::string, 7> list = {"np_center_etc", "centering", "quote", "quotation", "flush_left", "flush_right", "verse"};
    return (*this)[list[i]];
}

auto NameMapper::cstdiv(size_t i) -> Istring {
    static std::array<std::string, 7> list = {"cst_div0", "cst_div1", "cst_div2", "cst_div3", "cst_div4", "cst_div5", "cst_div6"};
    return (*this)[list[i]];
}

auto NameMapper::npdiv(size_t i) -> Istring {
    static std::array<std::string, 7> list = {"div0", "div1", "div2", "div3", "div4", "div5", "div6"};
    return (*this)[list[i]];
}

void NameMapper::boot() {
    set("alt_caption", "alt_head");
    set("alt_section", "alt_head");
    set("arc", "pic-arc");
    set("bezier", "pic-bezier");
    set("bibkey", "bib-key");
    set("bigcircle", "pic-bigcircle");
    set("border_bottom_width", "border-bottom-width");
    set("border_left_width", "border-left-width");
    set("border_right_width", "border-right-width");
    set("border_top_width", "border-top-width");
    set("box_pos", "position");
    set("box_scale", "scale");
    set("box_width", "width");
    set("box", "pic-framebox");
    set("caption", "head");
    set("cell_bottomborder", "bottom-border");
    set("cell_center", "center");
    set("cell_left", "left");
    set("cell_leftborder", "left-border");
    set("cell_right", "right");
    set("cell_rightborder", "right-border");
    set("cell_topborder", "top-border");
    set("centering", "center");
    set("circle", "pic-circle");
    set("citetype", "type");
    set("closecurve", "pic-closecurve");
    set("composition_ra", "composition");
    set("cst_display", "display");
    set("cst_div0", "div0");
    set("cst_div1", "div1");
    set("cst_div2", "div2");
    set("cst_div3", "div3");
    set("cst_div4", "div4");
    set("cst_div5", "div5");
    set("cst_div6", "div6");
    set("cst_empty", "");
    set("cst_nothing", "");
    set("cst_p", "p");
    set("cstb_key", "key");
    set("cstb_language", "language");
    set("cstb_type", "type");
    set("cstb_unknown", "unknown");
    set("cstb_url", "url");
    set("cstf_upright", "");
    set("curve_nbpts", "nbsymb");
    set("curve", "pic-curve");
    set("fbox_rend", "rend");
    set("figure_env", "figure");
    set("file_extension", "extension");
    set("flush_left", "flushed-left");
    set("flush_right", "flushed-right");
    set("font_bold", "bold");
    set("font_boldextended", "boldextended");
    set("font_condensed", "condensed");
    set("font_it", "it");
    set("font_large", "large");
    set("font_large1", "large1");
    set("font_large2", "large2");
    set("font_large3", "large3");
    set("font_large4", "large4");
    set("font_large5", "large5");
    set("font_large6", "large6");
    set("font_medium", "medium");
    set("font_normalsize", "normalsize");
    set("font_roman", "roman");
    set("font_sansserif", "sansserif");
    set("font_sc", "sc");
    set("font_semibold", "semibold");
    set("font_slanted", "slanted");
    set("font_small", "small");
    set("font_small1", "small1");
    set("font_small2", "small2");
    set("font_small3", "small3");
    set("font_small4", "small4");
    set("font_small5", "small5");
    set("font_small6", "small6");
    set("font_tt", "tt");
    set("font_upright", "upright");
    set("foot_position", "foot");
    set("footnote", "note");
    set("full_first", "prenomcomplet");
    set("glo", "Glossary");
    set("gloitem", "label");
    set("gloss_type", "gloss");
    set("inner_pos", "inner-pos");
    set("labelitem", "label");
    set("language", "");
    set("line", "pic-line");
    set("lineC", "line");
    set("linethickness", "pic-linethickness");
    set("mathmlns", "http://www.w3.org/1998/Math/MathML");
    set("minipage_width", "width");
    set("multiput", "pic-multiput");
    set("natcit", "Cit");
    set("nb_rasection", "");
    set("np_center_etc", "center");
    set("np_center", "center");
    set("np_cst_width", "width");
    set("np_linethickness", "linethickness");
    set("np_separator", "");
    set("np_theorem", "");
    set("oval", "pic-oval");
    set("particule", "part");
    set("project", "projet");
    set("put", "pic-put");
    set("quotation", "quoted");
    set("quote", "quoted");
    set("rclist", "UR");
    set("rcval", "+UR");
    set("rotate_angle", "angle");
    set("rotatebox", "pic-rotatebox");
    set("row_spaceafter", "spaceafter");
    set("scaleput", "pic-scaleput");
    set("scaption", "caption");
    set("space_before", "spacebefore");
    set("stylesheet", "");
    set("stylesheettype", "text/css");
    set("table_env", "table");
    set("table_width", "width");
    set("tagcurve", "pic-tagcurve");
    set("theorem_head", "alt_head");
    set("thicklines", "pic-thicklines");
    set("thinlines", "pic-thinlines");
    set("unit_length", "unit-length");
    set("user_list", "description");
    set("vector", "pic-vector");
    set("warning", "");
}

void NameMapper::assign(const std::string &sa, const std::string &sb) {
    spdlog::trace("Assigning to `{}` the value \"{}\"", sa, sb);
    auto n = sa.size();

    if (sa.starts_with("elt_")) return the_names.assign_name(sa.substr(4), sb);
    if (sa.starts_with("xml_")) return the_names.assign_name(sa.substr(4, sa.ends_with("_name") ? n - 9 : n - 4), sb);
    if (sa.starts_with("att_")) return set(sa.substr(4), sb);

    if (sa == "lang_fr") { set("french", sb); }
    if (sa == "lang_en") { set("english", sb); }
    if (sa == "distinguish_refer_in_rabib") {
        if ((sb == "true") || (sb == "yes"))
            the_main->distinguish_refer = true;
        else if ((sb == "false") || (sb == "no"))
            the_main->distinguish_refer = false;
        else
            spdlog::warn("distinguish_refer_in_rabib = {} ignored");
    }
    if (sa == "entity_names") { the_main->set_ent_names(sb); }
    if (sa == "default_class") { the_main->default_class = sb; }
    if (sa == "alternate_item") {
        if (sb == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (sb == "true")
            the_parser.hash_table.eval_let("item", "@item");
    }
    if (sa == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << sb << "}";
        the_main->add_to_from_config(1, aux); // \todo no Buffer
    }
    if (sa == "everyjob") { everyjob_string = fmt::format("\\everyjob={{{}}}", sb); }
    if (sa == "no_footnote_hack") {
        if (sb == "true") the_main->footnote_hack = false;
        if (sb == "false") the_main->footnote_hack = true;
    }
    if (sa == "use_font_elt") {
        if (sb == "true") the_main->use_font_elt = true;
        if (sb == "false") the_main->use_font_elt = false;
    }
    if (sa == "use_all_sizes") {
        if (sb == "true") the_main->use_all_sizes = true;
        if (sb == "false") the_main->use_all_sizes = false;
    }
    if (sa == "bibtex_fields") {
        Buffer(sb).interpret_bibtex_list(); // \todo without Buffer
    }
    if (sa == "bibtex_extensions") { Buffer(sb).interpret_bibtex_extension_list(); }
    if (sa == "mfenced_separator_val") {
        if (sb == "NONE")
            set("np_separator", "");
        else
            set("np_separator", sb);
    }
    if (sa.ends_with("_vals")) {
        Buffer B(sb);
        config_ns::interpret_list(sa.substr(0, n - 5), B); // \todo without Buffer
    }
    if (sa == "mml_font_normal") { set("mml@font@normal", sb); }
    if (sa == "mml_font_upright") { set("mml@font@upright", sb); }
    if (sa == "mml_font_bold") { set("mml@font@bold", sb); }
    if (sa == "mml_font_italic") { set("mml@font@italic", sb); }
    if (sa == "mml_font_bold_italic") { set("mml@font@bolditalic", sb); }
    if (sa == "mml_font_script") { set("mml@font@script", sb); }
    if (sa == "mml_font_bold_script") { set("mml@font@boldscript", sb); }
    if (sa == "mml_font_fraktur") { set("mml@font@fraktur", sb); }
    if (sa == "mml_font_doublestruck") { set("mml@font@doublestruck", sb); }
    if (sa == "mml_font_bold_fraktur") { set("mml@font@boldfraktur", sb); }
    if (sa == "mml_font_sansserif") { set("mml@font@sansserif", sb); }
    if (sa == "mml_font_bold_sansserif") { set("mml@font@boldsansserif", sb); }
    if (sa == "mml_font_sansserif_italic") { set("mml@font@sansserifitalic", sb); }
    if (sa == "mml_font_sansserif_bold_italic") { set("mml@font@sansserifbolditalic", sb); }
    if (sa == "mml_font_monospace") { set("mml@font@monospace", sb); }
}

void NameMapper::assign_name(const std::string &A, const std::string &B) {
    spdlog::trace("Setting XML element name `{}' to \"{}\" (assign_name)", A, B);

    if (A == "pack_font_att") {
        if (B == "true") the_main->pack_font_elt = true;
        if (B == "false") the_main->pack_font_elt = false;
        return;
    }

    if (A == "rasection" && ra_ok) {
        set("nb_rasection", B);
        return;
    }

    if (A == "theorem") {
        if (B[0] == 0) { // reverst to old behavior
            set("np_theorem", B);
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else if (B[0] == ' ') { // special case
            set("np_theorem", B.substr(1));
            the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
        } else {
            set("np_theorem", B);
            the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
        }
        return;
    }

    if (A == "use_font_elt") {
        if (B == "true") the_main->use_font_elt = true;
        if (B == "false") the_main->use_font_elt = false;
        return;
    }

    if (A == "xtheorem") {
        set("np_theorem", B);
        return;
    }

    set(A, B);
}
