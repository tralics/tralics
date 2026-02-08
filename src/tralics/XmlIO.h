#pragma once
#include "Buffer.h"
#include "LineList.h"
#include <string_view>

class XmlIO {
    enum x_type { xt_letter, xt_space, xt_digit, xt_ok_first, xt_other, xt_invalid };

    struct EntityDef {
        std::string name, value;
    };

    Buffer                  B; // holds current element
    Buffer                  aux;
    Buffer                  line_buffer; // holds current line
    LineList                lines;       // input file
    int                     cur_line{};
    std::array<x_type, 128> Type{};
    std::vector<char32_t>   input_line;    // current line
    std::vector<char32_t>   reread_list;   // current line
    char32_t                cur_char{' '}; // current character in some cases

    Xml                   *cur_xml{};
    std::vector<Xml *>     cur_stack;
    size_t                 cur_line_len{};
    size_t                 input_line_pos{};
    size_t                 encoding{}; // 0=utf8, 1=latin1
    void                   error(const std::string &s) const;
    std::vector<EntityDef> entities;
    int                    nb_cond{};
    bool                   eof_ok{};

private:
    auto               peek_char() -> char32_t;
    void               skip_char();
    auto               next_char() -> char32_t;
    void               skip_space();
    auto               next_line() -> bool;
    [[nodiscard]] auto at_eol() const -> bool { return input_line_pos >= cur_line_len; }
    void               scan_name();
    void               scan_name(uchar c);
    void               scan_name(uchar c1, uchar c2);
    void               parse_lt();
    void               parse_end();
    void               parse_dec();
    void               parse_dec_comment();
    void               parse_dec_entity();
    void               parse_dec_element();
    void               parse_dec_attlist();
    void               parse_dec_doctype();
    void               parse_dec_cdata();
    void               parse_dec_conditional();
    void               parse_dec_notation();
    auto               parse_sys_pub() -> bool;
    void               expect(std::string_view s);
    void               parse_pi();
    void               parse_tag();
    void               parse_attributes();
    void               parse_att_val();
    void               parse_quoted();
    void               pop_this();
    void               flush_buffer();
    auto               expand_PEReference() -> bool;
    void               run();

public:
    auto init(const std::string &name) -> bool;
    auto prun() -> Xml *;
};
