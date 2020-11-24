#pragma once
#include <cstddef>

class Stats {
    int st_short{0};                    // number of times the free token_list was shortened
    int st_inc{0};                      // number of times it was increased
    int st_alloc{0};                    // total number of totals in a list
    int stb_alloc{0};                   // number of buffer realloc
    int nb_macros{0}, nb_macros_del{0}; // number of macross added end deleted
    int level_up{0}, level_down{0};     // number of push and pop nest.
    int sh_boot{0}, sh_find{0}, sh_used{0};
    int nb_ref{0}, nb_used_ref{0}, nb_label_defined{0};
    int nb_href{0};
    int m_special{0};
    int m_allocated{0}, m_destroyed{0}, m_merge{0};
    int footnotes{0};

    size_t nb_xboot{0}; ///< number of elements created at boot tme.

public:
    void one_more_shorten_list() { st_short++; }
    void one_more_increase_list() { st_inc++; }
    void one_more_alloc_list() { st_alloc++; }
    void one_more_macro() { nb_macros++; }
    void one_less_macro() { nb_macros_del++; }
    void one_more_up() { level_up++; }
    void one_more_down() { level_down++; }
    void after_boot() { sh_boot = sh_find; }
    void one_more_sh_find() { sh_find++; }
    void one_more_sh_used() { sh_used++; }
    void one_more_ref() { nb_ref++; }
    void one_more_used_ref() { nb_used_ref++; }
    void one_more_label() { nb_label_defined++; }
    void one_more_href() { nb_href++; }
    void set_nb_xboot(size_t k) { nb_xboot = k; }
    void one_more_special() { m_special++; }
    void one_more_footnote() { footnotes++; }
    void one_more_allocated() { m_allocated++; }
    void more_math_destroy(int k) { m_destroyed += k; }
    void one_more_merge() { m_merge++; }
    void token_stats() const;
    void print_module_stats();
};
