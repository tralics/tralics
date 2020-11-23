#pragma once
#include "CmdChr.h"
#include <utility>

class Xml;
class Math;

class MathElt : public CmdChr {
    subtypes    font{};
    std::string payload;

public:
    MathElt(CmdChr X, subtypes c, std::string s = "") : CmdChr(X), font(c), payload(std::move(s)) {}
    MathElt(subtypes a, math_types b) : CmdChr(math_xml_cmd, a), font(subtypes(b)) {}
    MathElt(Xml *x, math_types y);
    MathElt(Xml *A, long b, math_types c);

    [[nodiscard]] auto get_char() const -> char32_t { return char_val(); }
    [[nodiscard]] auto get_fml_subtype() const -> subtypes;
    [[nodiscard]] auto get_font() const -> subtypes { return font; }
    [[nodiscard]] auto get_lcmd() const -> math_list_type { return math_list_type(font); }
    [[nodiscard]] auto get_list() const -> Math &;
    [[nodiscard]] auto get_xml_val() const -> Xml *;
    [[nodiscard]] auto get_xmltype() const -> math_types { return math_types(font); }

    void set_xmltype(math_types x) { font = subtypes(x); }

    // some tests on the elements
    [[nodiscard]] auto is_list() const -> bool { return is_math_list() && font == subtypes(math_open_cd); }
    [[nodiscard]] auto is_hbox() const -> bool { return is_math_list() && font == subtypes(math_hbox_cd); }
    [[nodiscard]] auto is_digit() const -> bool;
    [[nodiscard]] auto is_char() const -> bool { return is_space() || is_letter_token() || is_other_token(); }
    [[nodiscard]] auto is_letter_token() const -> bool { return is_letter(); }
    [[nodiscard]] auto is_other_token() const -> bool { return is_other(); }
    [[nodiscard]] auto is_star() const -> bool { return is_other_token() && get_char() == '*'; }
    [[nodiscard]] auto is_bracket() const -> bool { return is_other_token() && get_char() == '['; }
    [[nodiscard]] auto maybe_seq() const -> bool;
    [[nodiscard]] auto maybe_seq(subtypes f) const -> bool;
    [[nodiscard]] auto maybe_iseq(subtypes f) const -> bool;
    [[nodiscard]] auto maybe_iseq() const -> bool;
    // other functions
    [[nodiscard]] auto large2() const -> del_pos;
    [[nodiscard]] auto remove_prefix() const -> Xml *;
    void               cv_noMLt();
    void               cv_noML();
    void               cv_noMLt_special() const;
    void               cv_noMLt_special0() const;
    void               cv_noMLt_list() const;
    void               cv_noML_special() const;
    void               cv_noML_list() const;
    auto               cv1(math_style cms, bool ph) -> MathElt;
    void               change_type(int t);
    [[nodiscard]] auto try_math_op() const -> Xml *;
    [[nodiscard]] auto is_e_grave() const -> bool;
    [[nodiscard]] auto special3() const -> Xml *;
    void               print() const;

private:
    void set_xml_subtype(math_types x) {
        set_xmltype(x);
        cmd = math_xml_cmd;
    }
    [[nodiscard]] auto cv_char() const -> MathElt;
    [[nodiscard]] auto cv_cst() const -> MathElt;
    auto               cv_special(math_style cms) -> MathElt;
    [[nodiscard]] auto cv_special1(math_style cms) const -> MathElt;
    auto               cv_list(math_style cms, bool ph) -> MathElt;
    auto               cv_list_no() -> MathElt;
    [[nodiscard]] auto cv_mi(math_style cms) const -> MathElt;
    void               cv1_err();
    void               dump_for_err() const;
    [[nodiscard]] auto is_accent() const -> bool { return is_mathml() && font == 0; }
};
