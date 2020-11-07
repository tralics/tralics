#pragma once
#include "TokenList.h"
#include "Xid.h"
#include "Xml.h"
#include <string>
#include <vector>

// This is a global object for math handling
// but math handling is not recursive. reset() is called on every formula.
class MathHelper {
public:
    Xid         cur_cell_id;    // Id of current cell
    Xid         cur_row_id;     // Id of current row
    Xid         cur_formula_id; // Id of current formula
    Xid         cur_math_id;    // Id of current math element
    Xid         cur_texmath_id; // Id of current texmath
    Xid         cur_table_id;   // Id of current table
    int         all_env_ctr{};
    int         eqnum_status{}; // how many numbers for this equation?
    int         math_env_ctr{}; // two counters for environments
    std::string pos_att;        // position attribute, inline or display
    std::string label_val;      // name of the label

private:
    bool                     current_mode{}; // display or not, needed for \label
    bool                     seen_label{};   // do we see already have a label
    bool                     warned_label{}; // was the used warned for labels on this formula ?
    std::vector<int>         multi_labels_type;
    std::vector<std::string> multi_labels;
    TokenList                tag_list;         // value of \tag{foo}, if given
    bool                     is_tag_starred{}; // \tag or \tag* ?
    size_t                   last_ml_pos{0};

public:
    void reset_last_ml_pos() { last_ml_pos = 0; }
    auto end_of_row() -> bool;
    void dump_labels();
    void ml_check_labels();
    void new_label(const std::string &s, bool a);
    void ml_second_pass(Xml *row, bool vb);
    void ml_last_pass(bool vb);
    void insert_special_tag(std::string s) { multi_labels[last_ml_pos - 2] = std::move(s); }
    void new_multi_label(const std::string &s, int t) {
        multi_labels.push_back(s);
        multi_labels_type.push_back(t);
    }
    void               set_type(bool b);
    [[nodiscard]] auto has_label() const -> bool { return seen_label || eqnum_status == 1 || eqnum_status == 3; }
    void               stats();
    void               reset(bool dual);
    [[nodiscard]] auto is_inline() const -> bool { return current_mode; }
    [[nodiscard]] auto has_tag() const -> bool { return !tag_list.empty(); }
    void               reset_tags() { tag_list = TokenList(); }
    void               handle_tags();
    void               add_tag(TokenList &L);
    void               update_all_env_ctr(bool open) {
        if (open)
            all_env_ctr++;
        else
            all_env_ctr--;
    }
    void update_math_env_ctr(bool open) {
        if (open)
            math_env_ctr++;
        else
            math_env_ctr--;
    }
    void add_attribute(const std::string &a, const std::string &b, subtypes c) const;
    void starred_tag() { is_tag_starred = true; }
    void check_for_eqnum(subtypes type, bool multi);

private:
    void set_label(std::string s) {
        label_val  = std::move(s);
        seen_label = true;
    }
};

inline MathHelper cmi; // Data structure holding some global values
