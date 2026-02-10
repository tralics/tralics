#pragma once
#include "AttList.h"
#include "Buffer.h"
#include "NameMapper.h"
#include <cassert>

struct XmlAction;

class Xml : public std::vector<Xml *> { // \todo value semantics
public:
    size_t      id{0};  ///< id of the object
    std::string name;   ///< name of the element
    AttList     att;    ///< attributes of the element

    explicit Xml(const std::string &n) : name(std::string(n)) {}
    Xml(const std::string &s, size_t n) : id(n), name(the_names[s]) {}
    Xml(std::string N, Xml *z);
    Xml(const Xml &) = delete;
    Xml &operator=(const Xml &) = delete;
    Xml(Xml &&)                 = delete;
    Xml &operator=(Xml &&)      = delete;

    [[nodiscard]] auto all_empty() const -> bool;
    [[nodiscard]] auto back_or_nullptr() const -> Xml * { return empty() ? nullptr : back(); }
    [[nodiscard]] auto get_cell_span() const -> long;
    [[nodiscard]] auto has_name(const std::string &s) const -> bool { return name == s; }
    [[nodiscard]] auto has_name_of(const std::string &s) const -> bool { return name == the_names[s]; }
    [[nodiscard]] auto is_anchor() const -> bool { return is_element() && name == the_names["anchor"]; }
    [[nodiscard]] auto is_element() const -> bool { return to_signed(id) > 0; }
    [[nodiscard]] auto is_whitespace() const -> bool;
    [[nodiscard]] auto is_empty_p() const -> bool;
    [[nodiscard]] auto last_addr() const -> Xml *;
    [[nodiscard]] auto last_is_string() const -> bool;
    [[nodiscard]] auto only_hi() const -> bool;
    [[nodiscard]] auto only_recur_hi() const -> bool;
    [[nodiscard]] auto real_size() const -> long;
    [[nodiscard]] auto single_non_empty() const -> Xml *;
    [[nodiscard]] auto single_son() const -> Xml *;
    [[nodiscard]] auto tail_is_anchor() const -> bool;
    [[nodiscard]] auto spec_copy() const -> Xml *;

    auto convert_to_string() -> std::string;
    auto deep_copy() -> Xml *;
    auto find_on_tree(Xml *check, Xml *&res) const -> bool;
    auto get_first_env(const std::string &name) -> Xml *;

    void add_att(const std::string &a, const std::string &b, bool force = true) {
        if (force)
            att[a] = b;
        else
            att.emplace(a, b);
    }
    void add_att(const AttList &L, bool f) {
        for (const auto &i : L) {
            if (f)
                att[i.first] = i.second;
            else
                att.emplace(i.first, i.second);
        }
    }
    void add_att(Xml *b) { add_att(b->att, true); }
    void add_att_but_rend(Xml *b) {
        for (const auto &i : b->att)
            if (i.first != the_names["rend"]) att[i.first] = i.second;
    }
    auto has_att(const std::string &n) -> std::string {
        if (auto *i = att.lookup(n)) return *i;
        return {};
    }
    auto is_font_change() const -> bool { return att.lookup(the_names["'hi_flag"]) != nullptr; }
    void add_span(long n);
    void add_top_rule();
    void add_bottom_rule();
    void add_ref(const std::string &s);
    void add_special_att(const std::string &S, Buffer &B);
    void take_id(Xml *from);  // take ownership of a pre-allocated element's id and attributes
    void add_first(Xml *x);
    void add_tmp(Xml *x);
    void add_last_nl(Xml *x);
    void add_last_string(const std::string &B);
    void add_nl();
    void add_non_empty_to(Xml *res);
    void change_name(const std::string &s) { name = the_names[s]; }
    void convert_to_string(Buffer &B);
    void insert_at(size_t pos, Xml *x);
    void insert_bib(Xml *bib, Xml *match);
    auto is_child(Xml *x) const -> bool;
    void kill_name() { name = std::string(); }
    auto last_box() -> Xml *;
    void last_to_SH() const;
    void move(std::string match, Xml *res);
    auto par_is_empty() -> bool;
    void postprocess_fig_table(bool is_fig);
    auto put_at(long n, Xml *x) -> bool;
    void put_in_buffer(Buffer &b);
    void push_back_unless_nullptr(Xml *x);
    void push_back_list(Xml *x);
    void recurse(XmlAction &X);
    void recurse0(XmlAction &X);
    void remove_empty_par();
    auto remove_at(long n) -> bool;
    auto remove_last() -> Xml *;
    void remove_last_empty_hi();
    void remove_last_space();
    void remove_par_bal_if_ok();
    void subst_env0(std::string match, Xml *vl);
    void swap_x(Xml *x);
    void print_on(std::ostream &o) const;
    auto total_span(long &res) const -> bool;
    auto try_cline(bool action) -> bool;
    auto try_cline_again(bool action) -> bool;
    void unbox(Xml *x);
    auto value_at(long n) -> Xml *;
    void replace_first(Xml *x) {
        if (!empty()) {
            assert(x != nullptr);
            at(0) = x;
        }
    }
    void bordermatrix();
};

inline Xml *the_page_xml;

struct XmlAndType {
    Xml       *value;
    math_types type;
};

auto read_xml(const std::string &s) -> Xml *;

auto operator<<(std::ostream &fp, const Xml *T) -> std::ostream &;

auto fetch_att(Xml *x, const std::string &m) -> std::optional<std::string>;

using EqtbBox = EQTB<Xml *>;
