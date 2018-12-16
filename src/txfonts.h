// -*- C++ -*-
// $Id: txfonts.h,v 2.11 2008/11/12 09:43:56 grimm Exp $
// TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2003, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// This is how Tralics interprets a font
class FontInfo {
  int size;  // size, between 1 and 11 times 2048
  int tsize; // is fi_normal_size, etc 
  int shape; // it, sl, sc, or normal
  int family;// tt, sf, or normal
  int series;// bf or normal
  int packed;// packed value of the font
  int level; // the level, as for any EQTB object
  int old;   // previous value
  Istring color; // current color
  Istring old_color; // previous color
  bool stackval; // is the value on the stack ok ?
 public:
  FontInfo() : size(6*2048), tsize(0), shape(0), family(0), series(0), 
	       packed(0), level(level_zero), color(Istring()), stackval(true) {}
  name_positions shape_change() const;
  String shape_name() const;
  name_positions size_change() const;
  String size_name() const;
  name_positions family_change() const;
  String family_name() const;
  name_positions series_change() const;
  String series_name() const ;
  void not_on_stack() { stackval = false; }
  void is_on_stack() { stackval = true; }
  void update_old() { old = packed; old_color = color;}
  bool is_ok () { return  (old&fi_data_mask) == (packed&fi_data_mask) 
      && stackval &&color==old_color;}
  void pack() { packed = tsize+shape+family+series+size;}
  void unpack();
  void change_size(int);
  void kill() { shape =0; family = 0; series = 0;}
  void see_font_change(subtypes);
  String show_font();
  int get_level() { return level; }
  int get_size() { return size>>11; }
  void set_level(int k) { level = k;}
  void set_packed(int k) { packed = k;}
  int get_old() { return old; }
  Istring get_old_color() { return old_color; }
  int get_packed() { return packed; }
  void set_old_from_packed() { old = packed;}
  void set_color(Istring c) { color = c; }
  Istring get_color() const  { return color; }
  void ltfont(string s, subtypes c);
};

class TeXChar {
 public:
  short int width_idx;  // index in the width_table of this char
  short int height_idx; // index in the height_table of this char
  short int depth_idx;  // index in the depth_table of this char
  short int italic_idx; // index in the italics_table of this char
  short int tag;        // explains how to interpret remainder
  short int remainder;  // the remainder field
};

class TexFont {
 public:
  int smallest_char;
  int largest_char;  
  int width_len;
  int height_len;
  int depth_len;
  int italic_len;
  int ligkern_len;
  int kern_len;
  int exten_len;
  int param_len;
  TeXChar* char_table;
  int* width_table;
  int* height_table;
  int* depth_table;
  int* italic_table;
  int* ligkern_table;
  int* kern_table;
  int* exten_table;
  ScaledInt* param_table;
  int hyphen_char;
  int skew_char;
  string name;
  int scaled_val;
  int at_val;

  TexFont(string, int,int); 

  void realloc_param(int);
  bool its_me(string, int,int) const;
  void make_null();
  void load();
};

class TexFonts {
  uint last_idx;
 public:
  TexFonts() { data[0] = new TexFont("",0,0); last_idx = 0;}
  TexFont* data[nb_tex_fonts];
  bool is_valid(int);
  string name(int);
  void full_name(Buffer &,int k);
  int get_int_param(int k, int m);
  ScaledInt get_dimen_param(int k, int m);
  void set_int_param(int,int,int);
  void set_dimen_param(int,int,ScaledInt);
  int find_font(string,int,int);
  int define_a_new_font(string n, int a, int s);
};
