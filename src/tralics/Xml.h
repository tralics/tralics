#pragma once
#include "Buffer.h"
#include "NameMapper.h"
#include <gsl/gsl>

class XmlAction;

class Xml : public std::vector<gsl::not_null<Xml *>> { // \todo value semantics
public:
    Xid     id{0}; ///< id of the objet
    Istring name;  ///< name of the element

    Xml(Istring n = {}) : name(std::move(n)) {}
    Xml(const std::string &n) : name(Istring(n)) {}
    Xml(Istring N, Xml *z);
    Xml(name_positions N, Xml *z);
    [[deprecated]] Xml(name_positions x, Xid n) : id(n), name(the_names[x]) {}
    Xml(const std::string &s, Xid n) : id(n), name(the_names[s]) {}

    [[nodiscard]] auto             all_empty() const -> bool;
    [[nodiscard]] auto             back_or_nullptr() const -> Xml * { return empty() ? nullptr : back().get(); }
    [[nodiscard]] auto             get_cell_span() const -> long;
    [[nodiscard]] auto             has_name(const Istring &s) const -> bool { return name == s; }
    [[nodiscard, deprecated]] auto has_name(name_positions s) const -> bool { return name == the_names[s]; }
    [[nodiscard]] auto             has_name_of(const std::string &s) const -> bool { return name == the_names[s]; }
    [[nodiscard]] auto             is_anchor() const -> bool { return !is_xmlc() && name == the_names["anchor"]; }
    [[nodiscard]] auto             is_whitespace() const -> bool;
    [[nodiscard]] auto             is_empty_p() const -> bool;
    [[nodiscard]] auto             is_xmlc() const -> bool { return id.value <= 0; }
    [[nodiscard]] auto             last_addr() const -> Xml *;
    [[nodiscard]] auto             last_is_string() const -> bool;
    [[nodiscard]] auto             only_hi() const -> bool;
    [[nodiscard]] auto             only_recur_hi() const -> bool;
    [[nodiscard]] auto             all_xmlc() const -> bool;
    [[nodiscard]] auto             real_size() const -> long;
    [[nodiscard]] auto             single_non_empty() const -> Xml *;
    [[nodiscard]] auto             single_son() const -> Xml *;
    [[nodiscard]] auto             tail_is_anchor() const -> bool;
    [[nodiscard]] auto             spec_copy() const -> Xml *;

    auto convert_to_string() -> std::string;
    auto deep_copy() -> gsl::not_null<Xml *>;
    auto find_on_tree(Xml *check, Xml **res) const -> bool;
    auto get_first_env(name_positions name) -> Xml *;
    auto how_many_env(Istring match) -> long;

    void                add_att(const Istring &a, const Istring &b) const { id.add_attribute(a, b); }
    void                add_att(name_positions a, name_positions b) const { id.add_attribute(a, b); }
    void                add_first(Xml *x);
    void                add_ref(std::string s);
    void                add_tmp(gsl::not_null<Xml *> x);
    void                add_last_nl(Xml *x);
    void                add_last_string(const Buffer &B);
    void                add_nl();
    void                add_non_empty_to(Xml *res);
    [[deprecated]] void change_name(name_positions s) { name = the_names[s]; }
    void                change_name(const std::string &s) { name = the_names[s]; }
    void                compo_special();
    void                convert_to_string(Buffer &B);
    void                insert_at(size_t pos, Xml *x);
    void                insert_bib(Xml *bib, Xml *match);
    auto                is_child(Xml *x) const -> bool;
    void                kill_name() { name = Istring(); }
    auto                last_box() -> Xml *;
    void                last_to_SH() const;
    void                move(Istring match, Xml *res);
    void                one_fig_tab(bool is_fig);
    auto                par_is_empty() -> bool;
    void                postprocess_fig_table(bool is_fig);
    auto                prev_sibling(Xml *x) -> Xml *;
    auto                put_at(long n, gsl::not_null<Xml *> x) -> bool;
    void                put_in_buffer(Buffer &b);
    void                push_back_unless_nullptr(Xml *x);
    void                push_back_list(Xml *x);
    void                recurse(XmlAction &X);
    void                recurse0(XmlAction &X);
    void                remove_empty_par();
    auto                remove_at(long n) -> bool;
    auto                remove_last() -> Xml *;
    void                remove_last_empty_hi();
    void                remove_last_space();
    void                remove_par_bal_if_ok();
    void                rename(Istring old_name, Istring new_name);
    void                set_id(long i) { id = i; }
    void                subst_env0(Istring match, Xml *vl);
    void                swap_x(Xml *x);
    void                to_buffer(Buffer &b) const;
    auto                total_span(long &res) const -> bool;
    auto                try_cline(bool action) -> bool;
    auto                try_cline_again(bool action) -> bool;
    void                unbox(Xml *x);
    auto                value_at(long n) -> Xml *;
    void                word_stats(const std::string &match);
    void                word_stats_i();
    void                replace_first(Xml *x) {
        if (!empty()) at(0) = gsl::not_null{x};
    }
    void bordermatrix();
};

struct XmlAndType {
    Xml *      value;
    math_types type;
};

auto read_xml(const std::string &s) -> Xml *;

auto operator<<(std::ostream &fp, const Xml *T) -> std::ostream &;
