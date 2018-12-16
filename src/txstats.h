// -*- C++ -*-
// $Id: txstats.h,v 2.5 2008/02/21 17:45:59 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


class Stats {
  int st_short; // number of times the free token_list was shortened
  int st_inc;   // number of times it was increased
  int st_alloc; // total number of totals in a list
  int stb_alloc; // number of buffer realloc
  int st_nb_string; // number of strings created
  int str_length; // total size of string created
  int nb_macros, nb_macros_del; // number of macross added end deleted
  int level_up, level_down; // number of push and pop nest.
  int sh_boot, sh_find,sh_used;
  int nb_xboot; // number of elements created at boot tme.
  int nb_ref, nb_used_ref,nb_label_defined;
  int nb_href;
  int nb_hdr;
  int m_cv, m_k, m_large, m_small;
  int m_special;
  int m_trivial;
  int m_spec_box;
  int m_allocated,m_destroyed,m_merge;
  int footnotes;
  public:
  void one_more_mbox() { m_spec_box++; }
  void one_more_shorten_list() { st_short++; }
  void one_more_increase_list() { st_inc++; }
  void one_more_alloc_list() { st_alloc++; }
  void one_more_buffer_realloc() { stb_alloc++; }
  void one_more_string(int n) { st_nb_string++; str_length += n;}
  void one_more_macro() { nb_macros ++; }
  void one_less_macro() { nb_macros_del ++; }
  void one_more_up() { level_up++; }
  void one_more_down() { level_down++; }
  void after_boot() { sh_boot = sh_find; }
  void one_more_sh_find() { sh_find++; }
  void one_more_sh_used() { sh_used++; }
  void one_more_ref() { nb_ref++; }
  void one_more_used_ref() { nb_used_ref++; }
  void one_more_label() {nb_label_defined++;}
  void one_more_href() { nb_href++;}
  void one_more_hdr() { nb_hdr++;}
  void set_nb_xboot(int k) { nb_xboot = k; }
  void one_more_small() { m_small ++; }
  void one_more_kernel() { m_k ++; }
  void one_more_large() { m_large ++; }
  void one_more_convert() { m_cv ++; }
  void one_more_trivial() { m_trivial ++; }
  void one_more_special() { m_special ++; }
  void one_more_footnote() { footnotes++; }
  void one_more_allocated() { m_allocated ++; }
  void more_math_destroy(int k) { m_destroyed+= k; }
  void one_more_merge () { m_merge++; }
  Stats(): st_short(0),st_inc(0), st_alloc(0),stb_alloc(0), st_nb_string(0),
	   nb_macros(0),nb_macros_del(0), level_up(0), level_down(0),
	   sh_boot(0), sh_find(0),sh_used(0),nb_xboot(0), nb_ref(0), 
	   nb_used_ref(0), nb_label_defined(0),nb_href(0),nb_hdr(0),
	   m_cv(0), m_k(0), m_large(0), m_small(0), m_special(0),
	   m_trivial(0), m_spec_box(0),
	   m_allocated (0), m_destroyed (0),m_merge(0),footnotes(0) {};
  void token_stats();
  void print_module_stats();
};


