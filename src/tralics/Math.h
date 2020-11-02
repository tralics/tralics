#pragma once
#include "Buffer.h"
#include "MathElt.h"
#include "MathF.h"
#include "MathP.h"
#include "MathQ.h"
#include "Xml.h"
#include <list>

struct AttList;

using const_math_iterator = std::list<MathElt>::const_iterator;

class Math : public std::list<MathElt> {
    friend class MathDataP;
    friend class MathElt;
    math_list_type type{invalid_cd};

public:
    subtypes    sname{nomathenv_code};
    std::string saved;

    using std::list<MathElt>::push_back;

    [[nodiscard]] auto        duplicate(bool nomath) const -> subtypes;
    auto                      chars_to_mb(Buffer &B, bool rec) const -> bool;
    auto                      chars_to_mb1(Buffer &B) const -> bool;
    auto                      chars_to_mb2(Buffer &B) const -> bool;
    auto                      chars_to_mb3() -> std::string;
    auto                      convert_math(math_style k) -> Xml *;
    auto                      convert_math_noML(bool spec) -> Xml *;
    void                      convert_math_noML0();
    void                      convert_math_noMLt0();
    [[nodiscard]] auto        convert_opname() const -> std::string;
    auto                      convert_this_to_string(Buffer &B) const -> std::string;
    auto                      find_parens(MathQList &res, bool verbose) const -> bool;
    auto                      get_arg1() -> Math { return front().get_list(); }
    [[nodiscard]] auto        get_arg2() const -> Math { return std::next(begin())->get_list(); }
    [[nodiscard]] auto        get_arg3() const -> Math { return std::next(begin(), 2)->get_list(); }
    [[nodiscard]] auto        get_type() const -> math_list_type { return type; }
    [[nodiscard]] auto        get_name() const -> std::string;
    [[nodiscard]] static auto get_list(size_t w) -> Math &;
    void                      hack_type(int);
    [[nodiscard]] auto        has_type(int x) const -> bool { return type == x; }
    void                      is_font_cmd1_list(const_math_iterator &B, const_math_iterator &E);
    auto                      M_array(bool numbered, math_style cms) -> Xml *;
    auto                      M_cv(math_style cms, int need_row) -> XmlAndType;
    void                      print() const;
    void                      push_back(CmdChr X, subtypes c, std::string s = "");
    void                      push_back_list(subtypes X, math_list_type c);
    void                      push_back_font(subtypes X, subtypes c);
    void                      push_back(CmdChr X);
    void                      push_back(Xml *A, long b, math_types c);
    void                      push_front(CmdChr X, subtypes c);
    void                      remove_initial_group();
    void                      remove_spaces();
    void                      set_display_type() { type = math_ddollar_cd; }
    void                      set_env_name(int);
    void                      set_nondisplay_type() { type = math_dollar_cd; }
    void                      set_type(math_list_type c) { type = c; }
    auto                      trivial_math(long action) -> Xml *;
    auto                      trivial_math_index(symcodes cmd) -> Xml *;
    auto                      check_align() -> int;

private:
    void               add_cur_cont();
    void               add_cur_font();
    auto               add_fence(bool final, MathF &M) -> bool;
    void               concat(Xml *res);
    void               concat_space(Xml *res);
    auto               convert_cell(size_t &n, std::vector<AttList> &table, math_style W) -> Xml;
    auto               convert_char_seq(const MathElt &W) -> MathElt;
    auto               convert_char_iseq(const MathElt &W, bool multiple) -> MathElt;
    void               fetch_rlc(std::vector<AttList> &table);
    void               find_paren0(MathP &aux) const;
    auto               finish_translate1(bool vb) -> bool;
    auto               finish_translate2() -> bool;
    void               handle_mbox(Math &res);
    void               handle_mbox_no();
    void               handle_mbox_not();
    [[nodiscard]] auto has_over() const -> bool;
    auto               is_font_cmd1() -> bool;
    auto               large1(MathElt &cl, math_style cms) -> Xml *;
    auto               M_cv0(math_style cms) -> XmlAndType;
    auto               M_cv3(math_style cms) -> Math;
    void               handle_cmd_Big(math_style cms);
    auto               handle_cmd_Big_aux(math_style cms) -> bool;
    void               cv_hspace_t(MathElt &cur);
    auto               M_cvaux() -> Math;
    auto               M_ref() -> Xml *;
    auto               M_mbox1(Buffer &B, subtypes &f) -> int;
    auto               only_digits(Buffer &B) const -> bool;
    void               push_back(Math &X);
    void               push_back_small(Xml *a);
    void               push_front(Math &X);
    void               skip_initial_space();
    [[nodiscard]] auto special1() const -> Xml *;
    void               special2(bool &ok, Xml *&res) const;
    auto               split_as_array(std::vector<AttList> &table, math_style W, bool numbered) -> Xml *;
    void               remove_opt_arg(bool star);
    auto               remove_req_arg() -> std::string;
    [[nodiscard]] auto remove_req_arg_noerr() const -> std::string;
};
