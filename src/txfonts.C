// Tralics, a LaTeX to XML translator.
// Copyright (C) INRIA/apics/marelle (Jose' Grimm) 2003-2011
// Functions on fonts for Tralics.

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)


#include "tralics.h"
const char* txfonts_rcsid=
  "$Id: txfonts.C,v 2.27 2011/06/28 08:15:25 grimm Exp $";
// Font info stuff.

// This returns the position of the name of a font attribute
// for use as an XML attribute. 
// This function deals with the size.
name_positions FontInfo::size_change() const
{
  if(tsize == fi_small_size) return np_font_small;
  else if(tsize == fi_big_size) return np_font_large;
  else if(tsize==0) return cst_empty;
  else if(tsize == fi_small_size1) return np_font_small1;
  else if(tsize == fi_small_size2) return np_font_small2;
  else if(tsize == fi_small_size3) return np_font_small3;
  else if(tsize == fi_small_size4) return np_font_small4;
  else if(tsize == fi_small_size5) return np_font_small5;
  else if(tsize == fi_small_size6) return np_font_small6;
  else if(tsize == fi_big_size1) return np_font_large1;
  else if(tsize == fi_big_size2) return np_font_large2;
  else if(tsize == fi_big_size3) return np_font_large3;
  else if(tsize == fi_big_size4) return np_font_large4;
  else if(tsize == fi_big_size5) return np_font_large5;
  else if(tsize == fi_big_size6) return np_font_large6;
  else return cst_empty;
  // maybe np_font_normalsize
}

// Idem. This function deals with the shape.
name_positions FontInfo::shape_change() const
{
  if(shape == fi_it_shape) return np_font_it;
  else if(shape == fi_sl_shape) return np_font_slanted; 
  else if(shape == fi_sc_shape) return np_font_sc;
  else return cst_empty;
  // could be np_font_upright
}

// Idem. This function deals with the family.
name_positions FontInfo::family_change() const
{
  if(family == fi_sf_family) return np_font_sansserif;
  else if(family == fi_tt_family) return np_font_tt;
  else return cst_empty;
 // could be np_font_roman
}

// Idem. This function deals with the series.
name_positions FontInfo::series_change() const
{ 
  if(series == fi_bf_series) return np_font_bold;
  else if(series == fi_bx_series) return np_font_boldextended;
  else if(series == fi_sb_series) return np_font_semibold;
  else if(series == fi_c_series) return np_font_condensed;
  else return cst_empty;  // could be np_font_medium
}

// This function returns a font attribute, as a LaTeX command.

String FontInfo::size_name() const
{
  if(tsize == fi_small_size) return "\\small";
  else if(tsize == fi_small_size1) return "\\small";
  else if(tsize == fi_big_size) return "\\large";
  else if(tsize == fi_big_size1) return "\\large";
  else if(tsize == fi_small_size2) return "\\footnotesize";
  else if(tsize == fi_small_size3) return "\\scriptsize";
  else if(tsize == fi_small_size4) return "\\tiny";
  else if(tsize == fi_small_size5) return "\\Tiny";
  else if(tsize == fi_small_size6) return "\\TINY";
  else if(tsize == fi_big_size2) return "\\Large";
  else if(tsize == fi_big_size3) return "\\LARGE";
  else if(tsize == fi_big_size4) return "\\huge";
  else if(tsize == fi_big_size5) return "\\Huge";
  else if(tsize == fi_big_size6) return "\\HUGE";
  else return 0;
}

// Idem. This function deals with the shape.
String FontInfo::shape_name() const
{
  if(shape == fi_it_shape) return "\\itshape";
  else if(shape == fi_sl_shape) return "\\slshape";
  else if(shape == fi_sc_shape) return "\\scshape";
  else return 0;
}

// Idem. This function deals with the family.
String FontInfo::family_name() const
{
  if(family == fi_sf_family) return "\\sffamily";
  else if(family == fi_tt_family) return "\\ttfamily";
  else return 0;
}

// Idem. This function deals with the series. (the function is inline).
String FontInfo::series_name() const 
{ 
  if(series==fi_bf_series) return "\\bfseries";
  else if(series==fi_bx_series) return "\\boldextendedseries";
  else if(series==fi_sb_series) return "\\semiboldseries";
  else if(series==fi_c_series) return "\\condensedseries";
  return 0;
}

// This prints everything.
ostream& operator<<(ostream&fp, const FontInfo& L)
{
  String s;
  s = L.size_name();
  if(s) fp << s;
  s = L.shape_name();
  if(s) fp << s;
  s = L.family_name();
  if(s) fp << s;
  s = L.series_name();
  if(s) fp << s;
  return fp;
}

Logger& operator <<(Logger& X,  const FontInfo&x)
{
  *(X.fp) << x;
  return X;
}


// This unpacks the font. 
void FontInfo::unpack()
{
  size = packed &  fi_size_mask;
  tsize = packed & fi_tsize_mask;
  shape = packed & fi_shape_mask;
  family = packed & fi_family_mask;
  series = packed & fi_series_mask;
}

// This implements a command like \small.
void FontInfo::change_size(int c)
{
  if(c<0) c = 0;
  if(c>12) c = 12;
  size = c<< 11;
  if(!the_main->use_all_sizes()) {
    if(c<6) tsize = fi_small_size;
    else if (c>6) tsize = fi_big_size; 
    else tsize = 0;
    return;
  }
  switch(c) {
  case 0: tsize = fi_small_size6; return;
  case 1: tsize = fi_small_size5; return;
  case 2: tsize = fi_small_size4; return;
  case 3: tsize = fi_small_size3; return;
  case 4: tsize = fi_small_size2; return;
  case 5: tsize = fi_small_size1; return;
  case 7: tsize = fi_big_size1; return;
  case 8: tsize = fi_big_size2; return;
  case 9: tsize = fi_big_size3; return;
  case 10: tsize = fi_big_size4; return;
  case 11: tsize = fi_big_size5; return;
  case 12: tsize = fi_big_size6; return;
  default:tsize = 0;
  }
}


// This implements a command like \rmfamily: one parameter is changed.
void FontInfo::see_font_change(subtypes c)
{
  switch(c) {
  case rm_family_code: family = 0; return;
  case sf_family_code: family = fi_sf_family; return;
  case tt_family_code: family = fi_tt_family; return;
  case md_series_code: series = 0; return;
  case bf_series_code: series = fi_bf_series; return;
  case up_shape_code: shape = 0; return;
  case it_shape_code: shape = fi_it_shape; return;
  case sl_shape_code: shape = fi_sl_shape; return;
  case sc_shape_code: shape = fi_sc_shape; return;
  case em_code:
    if(shape) shape = 0; else shape = fi_it_shape;
    return;
  default:
    kill(); // should be \normalfont
  }
}

// This implements \fontfamily etc
void FontInfo::ltfont(string s,subtypes c)
{
  switch(c) {
  case fontencoding_code:
    return; // Output encoding is always Unicode
  case fontfamily_code: // rm, sf, or tt
    if(s=="cmr" || s=="ptm" ) family= 0;
    else if(s=="cmss" || s=="phv") family= fi_sf_family;
    else if(s=="cmtt" || s=="pcr") family= fi_tt_family;
    else { 
      family = 0; 
      main_ns::log_and_tty << lg_start << "Unknown font family " << s << lg_end; }
    return;
  case fontseries_code: // md bf
    if(s=="m") series=0;
    else if(s=="b") series=fi_bf_series;
    else if(s=="bx") series=fi_bx_series;
    else if(s=="sb") series=fi_sb_series;
    else if(s=="c") series=fi_c_series;
    else { series = 0; 
      main_ns::log_and_tty << lg_start << "Unknown font series " << s << lg_end; }
    return;
  case fontshape_code: // it sl sc
    if(s=="n") shape=0;
    else if(s=="it") shape=fi_it_shape;
    else if(s=="sl") shape=fi_sl_shape;
    else if(s=="sc") shape=fi_sc_shape;
    else { shape = 0; 
      main_ns::log_and_tty << lg_start << "Unknown font shape " << s << lg_end; }
    return;
  default: return; // impossible case
  }
}

// ------------------------------------------------------
// tex fonts

// Finds a font given by name and size, or creates one if needed
int TexFonts::find_font(string n, int a, int s) 
{
  for(uint i=0;i<nb_tex_fonts;i++)
    if(data[i] && data[i]->its_me(n,a,s)) return i;
  return define_a_new_font(n,a,s);
}

// Ctor(name, at_value, scaled).
// In TeX, only one of at_value and scaled can be given. Unused in Tralics.
TexFont::TexFont(string n, int a, int s) 
{
  make_null();
  name = n; 
  at_val =a;
  scaled_val =s;
  if(n.empty()) return; // case of null font
  load();
}

// This allocates a new slot in the font list.
int TexFonts::define_a_new_font(string n, int a, int s) 
{
  if(last_idx+1>=nb_tex_fonts) {
    the_parser.parse_error("fatal: font table overflow");
    return 0;
  }
  last_idx++;
  int k = last_idx;
  data[k] = new TexFont(n,a,s);
  return k;
}

// not yet done.
void TexFont::load()
{
}

// This compares two fonts
bool TexFont::its_me(string n, int a, int s) const
{
  return name==n && at_val == a && scaled_val ==s;
}

// This kills all tables.
void TexFont::make_null()
{
  smallest_char=0;
  largest_char=0;
  width_len=0;
  height_len=0;
  depth_len=0;
  italic_len=0;
  ligkern_len=0;
  kern_len=0;
  exten_len=0;
  param_len=0;
  char_table=0;
  width_table=0;
  height_table=0;
  depth_table=0;
  italic_table=0;
  ligkern_table=0;
  kern_table=0;
  exten_table=0;
  param_table=0;
  hyphen_char=0;
  skew_char=0;
  name="";
  scaled_val=0;
  at_val=0;
}

// True if k is a valid font ID
bool TexFonts::is_valid(int k)
{
  if(k<0 || k>= int(nb_tex_fonts)) return false;
  if(!data[k]) return false;
  return true;
}

// Returns name of font
string TexFonts::name(int k)
{
  if(k==0) return "nullfont";
  if(!is_valid(k)) return "";
  return data[k]->name;
}
// Returns name of font
void TexFonts::full_name(Buffer& B,int k)
{
  if(!is_valid(k)) return;
  B.push_back(data[k]->name);
  if(data[k]->scaled_val) { 
    B.push_back(" scaled "); 
    B.push_back_int(data[k]->scaled_val);
  }
  if(data[k]->at_val) { 
    B.push_back(" at "); 
    B.push_back(ScaledInt(data[k]->at_val),glue_spec_pt);
  }
}

// Returns an integer parameter for a font
int TexFonts::get_int_param(int ft, int pos)
{
  if(!is_valid(ft)) return -1;
  if(pos==0) return data[ft]->hyphen_char;
  else return data[ft]->skew_char;
}

// Returns a dimension parameter for a font
ScaledInt TexFonts::get_dimen_param(int ft, int pos)
{
  if(!is_valid(ft)) return 0;
  if(pos<0 || pos>= data[ft]->param_len) return 0;
  return data[ft]->param_table[pos];
}

// Sets an integer parameter for a font
void TexFonts::set_int_param(int ft, int pos, int v)
{
  if(!is_valid(ft)) {
    the_parser.parse_error("attempt to modify unexistent font param");
    return ;
  }
  if(pos==0) data[ft]->hyphen_char=v; 
  else data[ft]->skew_char = v;
}

// Sets a dimension parameter for a font
void TexFonts::set_dimen_param(int ft, int p, ScaledInt v)
{
  if(!is_valid(ft) || p<0 || p>100000) {
    the_parser.parse_error("attempt to modify unexistent font param");
    return ;
  }
  if(p>data[ft]->param_len) data[ft]->realloc_param(p);
  data[ft]->param_table[p]= v;
}

// All fonts can be resized...
void TexFont::realloc_param(int p)
{
  int k =1;
  while(p>=k) k *=2;
  ScaledInt* T = new ScaledInt[k];
  for(int i=0;i<k;i++) T[i].set_value(0);
  for(int i=0;i<param_len;i++) T[i] = param_table[i];
  if(param_table) delete[] param_table;
  param_table = T;
  param_len = k;
}
