#pragma once
#include <cstddef>

class Stats {
    int st_short{0};  // number of times the free token_list was shortened
    int st_inc{0};    // number of times it was increased
    int st_alloc{0};  // total number of totals in a list
    int stb_alloc{0}; // number of buffer realloc
    int nb_ref{0}, nb_used_ref{0}, nb_label_defined{0};
    int nb_href{0};
    int m_special{0};
    int m_allocated{0}, m_destroyed{0};

public:
    void one_more_shorten_list() { st_short++; }
    void one_more_increase_list() { st_inc++; }
    void one_more_alloc_list() { st_alloc++; }
    void one_more_ref() { nb_ref++; }
    void one_more_used_ref() { nb_used_ref++; }
    void one_more_label() { nb_label_defined++; }
    void one_more_href() { nb_href++; }
    void one_more_special() { m_special++; }
    void one_more_allocated() { m_allocated++; }
    void more_math_destroy(int k) { m_destroyed += k; }
    void token_stats() const;
    void print_module_stats();
};
