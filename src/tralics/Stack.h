#pragma once
#include "../txstack.h"
#include "ArrayInfo.h"

class Xml;

struct StackSlot {
    Xml *       obj;
    int         line;
    std::string frame;
    mode        md;
    std::string uid;
    bool        omit_cell{false};

    StackSlot(Xml *a, int b, std::string c, mode M, std::string u) : obj(a), line(b), frame(std::move(c)), md(M), uid(std::move(u)) {}

    void dump() const;
    void fulldump(size_t i) const;
};

class Stack : public std::vector<StackSlot> {
    long                   last_xid{-1}; // id of the last
    long                   xid_boot{0};
    std::string            cur_lid;    // the id to be pushed on uids[]
    std::vector<AttList>   attributes; // the main table of attributes
    std::vector<Xml *>     enames;     // the main table of element names
    Buffer                 mybuffer;   // a buffer
    std::vector<ArrayInfo> AI;         // the attributes for the current TeX arrays
    mode                   cur_mode{}; // the mode to be pushed on modes[]
public:
    Xml *newline_xml{};

    Stack();

    [[nodiscard]] auto first_frame() const -> std::string;
    [[nodiscard]] auto first_non_empty() const -> const StackSlot &;
    [[nodiscard]] auto get_cur_id() const -> std::string { return cur_lid; }
    [[nodiscard]] auto get_cur_par() const -> Xml *;
    [[nodiscard]] auto get_mode() const -> mode { return cur_mode; }
    [[nodiscard]] auto get_xid() const -> Xid {
        assert(last_xid == enames.size() - 1);
        return last_xid;
    }
    [[nodiscard]] auto in_v_mode() const -> bool { return get_mode() == mode_v; }
    [[nodiscard]] auto in_h_mode() const -> bool { return get_mode() == mode_h; }
    [[nodiscard]] auto in_no_mode() const -> bool { return get_mode() == mode_none; }
    [[nodiscard]] auto in_bib_mode() const -> bool { return get_mode() == mode_bib; }
    [[nodiscard]] auto in_array_mode() const -> bool { return get_mode() == mode_array; }
    [[nodiscard]] auto is_frame(const std::string &s) const -> bool;
    [[nodiscard]] auto is_frame2(const std::string &S) const -> bool;
    [[nodiscard]] auto last_att_list() const -> AttList & { return get_xid().get_att(); }

    auto add_anchor(const std::string &s, bool spec) -> std::string;
    void add_att_to_last(const std::string &A, const std::string &B, bool force);
    void add_att_to_last(const std::string &A, const std::string &B);
    void add_att_to_cur(const std::string &A, const std::string &B);
    void add_att_to_cur(const std::string &A, const std::string &B, bool force);
    void add_border(long a, long b);
    void add_center_to_p() const;
    void add_last(Xml *x);
    void add_last_string(const Buffer &B);
    auto add_new_anchor() -> std::string;
    auto add_new_anchor_spec() -> std::string;
    void add_nl();
    auto add_newid0(const std::string &x) -> AttList &;
    void check_font();
    void create_new_anchor(Xid xid, const std::string &id, const std::string &idtext);
    auto cur_xid() -> Xid { return top_stack()->id; }
    auto get_top_id() -> Xid { return top_stack()->id; }
    void delete_table_atts();
    void dump();
    auto document_element() -> Xml * { return at(0).obj; }
    auto elt_from_id(size_t n) { return enames[n]; }
    void end_module();
    auto fetch_by_id(size_t n) -> Xml *;
    auto find_cell_props(Xid id) -> ArrayInfo *;
    void find_cid_rid_tid(Xid &cid, Xid &rid, Xid &tid);
    auto find_ctrid(subtypes m) -> long;
    auto find_parent(Xml *x) -> Xml *;
    void finish_cell(int w);
    void fonts0(const std::string &x);
    auto get_att_list(size_t k) -> AttList & { return attributes[k]; }
    auto get_my_table(Xid &cid) -> ArrayInfo *;
    auto get_u_or_v(bool u_or_v) -> TokenList;
    void hack_for_hanl();
    void implement_cit(const std::string &b1, const std::string &b2, const std::string &a, const std::string &c);
    void init_all(const std::string &a);
    void ipush(std::string fr, Xml *V);
    auto is_float() -> bool;
    auto is_omit_cell() -> bool { return back().omit_cell; }
    void mark_omit_cell();
    auto new_array_info(Xid i) -> ArrayInfo &;
    auto next_xid(Xml *elt) -> Xid;
    void para_aux(int x);
    void pop(const std::string &a);
    void pop_if_frame(const std::string &x);
    void push(std::string fr, Xml *V);
    void push1(std::string name, std::string x);
    void push1(const std::string &x);
    auto push_hbox(std::string name) -> Xml *;
    void push_pop_cell(int dir);
    void push_trace();
    auto push_par(size_t k) -> Xid;
    auto remove_last() -> Xml *;
    void remove_last_space();
    void set_arg_mode() { cur_mode = mode_argument; }
    void set_array_mode() { cur_mode = mode_array; }
    void set_bib_mode() { cur_mode = mode_bib; }
    void set_cur_id(std::string k) { cur_lid = std::move(k); }
    void set_h_mode() { cur_mode = mode_h; }
    void set_m_mode() { cur_mode = mode_math; }
    void set_mode(mode x) { cur_mode = x; }
    void set_no_mode() { cur_mode = mode_none; }
    void set_v_mode() { cur_mode = mode_v; }
    void set_xid_boot() {
        assert(last_xid == enames.size() - 1);
        xid_boot = last_xid;
    }
    void T_ampersand();
    void T_hline();
    auto temporary() -> Xml *;
    auto top_stack() -> Xml * { return back().obj; }
    void trace_pop(bool sw);
    void trace_stack();
    void unbox(Xml *x);

    static auto fonts1(const std::string &x) -> Xml *;
    static auto xml2_space(std::string elt, const std::string &b1, Xml *first_arg, Xml *second_arg) -> gsl::not_null<Xml *>;
};
