// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose Grimm) 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// (See the file COPYING in the main directory for details)

// Tralics math; boot part. This constructs all data structures.

#include "tralics.h"
#include "txmath.h"

const char* txmathboot_rcsid=
  "$Id: txmathboot.C,v 2.87 2012/07/06 10:21:07 grimm Exp $";

extern MathDataP math_data;
using namespace math_ns;
extern bool compatibility;
extern bool bad_minus;


// mathml variants: normal, bold, italic, bold-italic, double-struck, 
// bold-fraktur, script, bold-script, fraktur, sans-serif, bold-sans-serif,
// sans-serif-italic, sans-serif-bold-italic, monospace
typedef string math_char_slot [15];
math_char_slot math_chars[128];
Xmlp single_chars[128];

#define LANGLE "&#x02329;"
#define RANGLE "&#x0232A;"
//#define LANGLE "&#x27E8;"
//#define RANGLE "&#x27E9;"

string get_math_char(uchar c, int f)
{
  return math_chars[c][f];
}

void set_math_char(uchar c, int f,string s)
{
  math_chars[c][f] = s;
}

Xmlp math_ns::get_builtin_alt(int p)
{
  return math_data.get_builtin_alt(p);
}

inline void eval_let(String a, String b)
{ 
  the_parser.hash_table.eval_let(a,b);
}
void math_ns::fill_single_char()
{
  for(int i=0;i<128;i++) single_chars[i] = 0;
  for(uchar x ='a'; x<='z'; x++) {
    single_chars[x] = new Xml(the_names[np_letter_a+x-'a']);
  }
  for(uchar x ='A'; x<='Z'; x++) {
    single_chars[x] = new Xml(the_names[np_letter_A+x-'A']);
  }
}

// In the case of \Bbb A, we can use
//  &Aopf; or  &#x15D38; or mathvariant=doublestruck
// Here we fill the table with &Aopf;
void math_ns::fill_math_char_slots_ent()
{
  // from isomopf.ent
  // lower case letters from mmlextra.ent
  math_chars[uchar('A')][math_f_doublestruck]="&Aopf;";
  math_chars[uchar('B')][math_f_doublestruck]="&Bopf;";
  math_chars[uchar('C')][math_f_doublestruck]="&Copf;";
  math_chars[uchar('D')][math_f_doublestruck]="&Dopf;";
  math_chars[uchar('E')][math_f_doublestruck]="&Eopf;";
  math_chars[uchar('F')][math_f_doublestruck]="&Fopf;";
  math_chars[uchar('G')][math_f_doublestruck]="&Gopf;";
  math_chars[uchar('H')][math_f_doublestruck]="&Hopf;";
  math_chars[uchar('I')][math_f_doublestruck]="&Iopf;";
  math_chars[uchar('J')][math_f_doublestruck]="&Jopf;";
  math_chars[uchar('K')][math_f_doublestruck]="&Kopf;";
  math_chars[uchar('L')][math_f_doublestruck]="&Lopf;";
  math_chars[uchar('M')][math_f_doublestruck]="&Mopf;";
  math_chars[uchar('N')][math_f_doublestruck]="&Nopf;";
  math_chars[uchar('O')][math_f_doublestruck]="&Oopf;";
  math_chars[uchar('P')][math_f_doublestruck]="&Popf;";
  math_chars[uchar('Q')][math_f_doublestruck]="&Qopf;";
  math_chars[uchar('R')][math_f_doublestruck]="&Ropf;";
  math_chars[uchar('S')][math_f_doublestruck]="&Sopf;";
  math_chars[uchar('T')][math_f_doublestruck]="&Topf;";
  math_chars[uchar('U')][math_f_doublestruck]="&Uopf;";
  math_chars[uchar('V')][math_f_doublestruck]="&Vopf;";
  math_chars[uchar('W')][math_f_doublestruck]="&Wopf;";
  math_chars[uchar('X')][math_f_doublestruck]="&Xopf;";
  math_chars[uchar('Y')][math_f_doublestruck]="&Yopf;";
  math_chars[uchar('Z')][math_f_doublestruck]="&Zopf;";
  math_chars[uchar('a')][math_f_doublestruck]="&aopf;";
  math_chars[uchar('b')][math_f_doublestruck]="&bopf;";
  math_chars[uchar('c')][math_f_doublestruck]="&copf;";
  math_chars[uchar('d')][math_f_doublestruck]="&dopf;";
  math_chars[uchar('e')][math_f_doublestruck]="&eopf;";
  math_chars[uchar('f')][math_f_doublestruck]="&fopf;";
  math_chars[uchar('g')][math_f_doublestruck]="&gopf;";
  math_chars[uchar('h')][math_f_doublestruck]="&hopf;";
  math_chars[uchar('i')][math_f_doublestruck]="&iopf;";
  math_chars[uchar('j')][math_f_doublestruck]="&jopf;";
  math_chars[uchar('k')][math_f_doublestruck]="&kopf;";
  math_chars[uchar('l')][math_f_doublestruck]="&lopf;";
  math_chars[uchar('m')][math_f_doublestruck]="&mopf;";
  math_chars[uchar('n')][math_f_doublestruck]="&nopf;";
  math_chars[uchar('o')][math_f_doublestruck]="&oopf;";
  math_chars[uchar('p')][math_f_doublestruck]="&popf;";
  math_chars[uchar('q')][math_f_doublestruck]="&qopf;";
  math_chars[uchar('r')][math_f_doublestruck]="&ropf;";
  math_chars[uchar('s')][math_f_doublestruck]="&sopf;";
  math_chars[uchar('t')][math_f_doublestruck]="&topf;";
  math_chars[uchar('u')][math_f_doublestruck]="&uopf;";
  math_chars[uchar('v')][math_f_doublestruck]="&vopf;";
  math_chars[uchar('w')][math_f_doublestruck]="&wopf;";
  math_chars[uchar('x')][math_f_doublestruck]="&xopf;";
  math_chars[uchar('y')][math_f_doublestruck]="&yopf;";
  math_chars[uchar('z')][math_f_doublestruck]="&zopf;";

  // from isomscr.ent
  math_chars[uchar('A')][math_f_script]="&Ascr;";
  math_chars[uchar('B')][math_f_script]="&Bscr;";
  math_chars[uchar('C')][math_f_script]="&Cscr;";
  math_chars[uchar('D')][math_f_script]="&Dscr;";
  math_chars[uchar('E')][math_f_script]="&Escr;";
  math_chars[uchar('F')][math_f_script]="&Fscr;";
  math_chars[uchar('G')][math_f_script]="&Gscr;";
  math_chars[uchar('H')][math_f_script]="&Hscr;";
  math_chars[uchar('I')][math_f_script]="&Iscr;";
  math_chars[uchar('J')][math_f_script]="&Jscr;";
  math_chars[uchar('K')][math_f_script]="&Kscr;";
  math_chars[uchar('L')][math_f_script]="&Lscr;";
  math_chars[uchar('M')][math_f_script]="&Mscr;";
  math_chars[uchar('N')][math_f_script]="&Nscr;";
  math_chars[uchar('O')][math_f_script]="&Oscr;";
  math_chars[uchar('P')][math_f_script]="&Pscr;";
  math_chars[uchar('Q')][math_f_script]="&Qscr;";
  math_chars[uchar('R')][math_f_script]="&Rscr;";
  math_chars[uchar('S')][math_f_script]="&Sscr;";
  math_chars[uchar('T')][math_f_script]="&Tscr;";
  math_chars[uchar('U')][math_f_script]="&Uscr;";
  math_chars[uchar('V')][math_f_script]="&Vscr;";
  math_chars[uchar('W')][math_f_script]="&Wscr;";
  math_chars[uchar('X')][math_f_script]="&Xscr;";
  math_chars[uchar('Y')][math_f_script]="&Yscr;";
  math_chars[uchar('Z')][math_f_script]="&Zscr;";
  math_chars[uchar('a')][math_f_script]="&ascr;";
  math_chars[uchar('b')][math_f_script]="&bscr;";
  math_chars[uchar('c')][math_f_script]="&cscr;";
  math_chars[uchar('d')][math_f_script]="&dscr;";
  math_chars[uchar('e')][math_f_script]="&escr;";
  math_chars[uchar('f')][math_f_script]="&fscr;";
  math_chars[uchar('g')][math_f_script]="&gscr;";
  math_chars[uchar('h')][math_f_script]="&hscr;";
  math_chars[uchar('i')][math_f_script]="&iscr;";
  math_chars[uchar('j')][math_f_script]="&jscr;";
  math_chars[uchar('k')][math_f_script]="&kscr;";
  math_chars[uchar('l')][math_f_script]="&lscr;";
  math_chars[uchar('m')][math_f_script]="&mscr;";
  math_chars[uchar('n')][math_f_script]="&nscr;";
  math_chars[uchar('o')][math_f_script]="&oscr;";
  math_chars[uchar('p')][math_f_script]="&pscr;";
  math_chars[uchar('q')][math_f_script]="&qscr;";
  math_chars[uchar('r')][math_f_script]="&rscr;";
  math_chars[uchar('s')][math_f_script]="&sscr;";
  math_chars[uchar('t')][math_f_script]="&tscr;";
  math_chars[uchar('u')][math_f_script]="&uscr;";
  math_chars[uchar('v')][math_f_script]="&vscr;";
  math_chars[uchar('w')][math_f_script]="&wscr;";
  math_chars[uchar('x')][math_f_script]="&xscr;";
  math_chars[uchar('y')][math_f_script]="&yscr;";
  math_chars[uchar('z')][math_f_script]="&zscr;";

  // From isomfrk.ent
  math_chars[uchar('A')][math_f_fraktur]="&Afr;";
  math_chars[uchar('B')][math_f_fraktur]="&Bfr;";
  math_chars[uchar('C')][math_f_fraktur]="&Cfr;";
  math_chars[uchar('D')][math_f_fraktur]="&Dfr;";
  math_chars[uchar('E')][math_f_fraktur]="&Efr;";
  math_chars[uchar('F')][math_f_fraktur]="&Ffr;";
  math_chars[uchar('G')][math_f_fraktur]="&Gfr;";
  math_chars[uchar('H')][math_f_fraktur]="&Hfr;";
  math_chars[uchar('I')][math_f_fraktur]="&Ifr;";
  math_chars[uchar('J')][math_f_fraktur]="&Jfr;";
  math_chars[uchar('K')][math_f_fraktur]="&Kfr;";
  math_chars[uchar('L')][math_f_fraktur]="&Lfr;";
  math_chars[uchar('M')][math_f_fraktur]="&Mfr;";
  math_chars[uchar('N')][math_f_fraktur]="&Nfr;";
  math_chars[uchar('O')][math_f_fraktur]="&Ofr;";
  math_chars[uchar('P')][math_f_fraktur]="&Pfr;";
  math_chars[uchar('Q')][math_f_fraktur]="&Qfr;";
  math_chars[uchar('R')][math_f_fraktur]="&Rfr;";
  math_chars[uchar('S')][math_f_fraktur]="&Sfr;";
  math_chars[uchar('T')][math_f_fraktur]="&Tfr;";
  math_chars[uchar('U')][math_f_fraktur]="&Ufr;";
  math_chars[uchar('V')][math_f_fraktur]="&Vfr;";
  math_chars[uchar('W')][math_f_fraktur]="&Wfr;";
  math_chars[uchar('X')][math_f_fraktur]="&Xfr;";
  math_chars[uchar('Y')][math_f_fraktur]="&Yfr;";
  math_chars[uchar('Z')][math_f_fraktur]="&Zfr;";
  math_chars[uchar('a')][math_f_fraktur]="&afr;";
  math_chars[uchar('b')][math_f_fraktur]="&bfr;";
  math_chars[uchar('c')][math_f_fraktur]="&cfr;";
  math_chars[uchar('d')][math_f_fraktur]="&dfr;";
  math_chars[uchar('e')][math_f_fraktur]="&efr;";
  math_chars[uchar('f')][math_f_fraktur]="&ffr;";
  math_chars[uchar('g')][math_f_fraktur]="&gfr;";
  math_chars[uchar('h')][math_f_fraktur]="&hfr;";
  math_chars[uchar('i')][math_f_fraktur]="&ifr;";
  math_chars[uchar('j')][math_f_fraktur]="&jfr;";
  math_chars[uchar('k')][math_f_fraktur]="&kfr;";
  math_chars[uchar('l')][math_f_fraktur]="&lfr;";
  math_chars[uchar('m')][math_f_fraktur]="&mfr;";
  math_chars[uchar('n')][math_f_fraktur]="&nfr;";
  math_chars[uchar('o')][math_f_fraktur]="&ofr;";
  math_chars[uchar('p')][math_f_fraktur]="&pfr;";
  math_chars[uchar('q')][math_f_fraktur]="&qfr;";
  math_chars[uchar('r')][math_f_fraktur]="&rfr;";
  math_chars[uchar('s')][math_f_fraktur]="&sfr;";
  math_chars[uchar('t')][math_f_fraktur]="&tfr;";
  math_chars[uchar('u')][math_f_fraktur]="&ufr;";
  math_chars[uchar('v')][math_f_fraktur]="&vfr;";
  math_chars[uchar('w')][math_f_fraktur]="&wfr;";
  math_chars[uchar('x')][math_f_fraktur]="&xfr;";
  math_chars[uchar('y')][math_f_fraktur]="&yfr;";
  math_chars[uchar('z')][math_f_fraktur]="&zfr;";
}

void math_ns::fill_math_char_slots()
{
  for(int i=0;i<128;i++)
    for(int j=0;j<15;j++)
      math_chars[i][j]="";
  // Position 0 : normal
  math_chars[uchar('A')][math_f_normal]="A";
  math_chars[uchar('B')][math_f_normal]="B";
  math_chars[uchar('C')][math_f_normal]="C";
  math_chars[uchar('D')][math_f_normal]="D";
  math_chars[uchar('E')][math_f_normal]="E";
  math_chars[uchar('F')][math_f_normal]="F";
  math_chars[uchar('G')][math_f_normal]="G";
  math_chars[uchar('H')][math_f_normal]="H";
  math_chars[uchar('I')][math_f_normal]="I";
  math_chars[uchar('J')][math_f_normal]="J";
  math_chars[uchar('K')][math_f_normal]="K";
  math_chars[uchar('L')][math_f_normal]="L";
  math_chars[uchar('M')][math_f_normal]="M";
  math_chars[uchar('N')][math_f_normal]="N";
  math_chars[uchar('O')][math_f_normal]="O";
  math_chars[uchar('P')][math_f_normal]="P";
  math_chars[uchar('Q')][math_f_normal]="Q";
  math_chars[uchar('R')][math_f_normal]="R";
  math_chars[uchar('S')][math_f_normal]="S";
  math_chars[uchar('T')][math_f_normal]="T";
  math_chars[uchar('U')][math_f_normal]="U";
  math_chars[uchar('V')][math_f_normal]="V";
  math_chars[uchar('W')][math_f_normal]="W";
  math_chars[uchar('X')][math_f_normal]="X";
  math_chars[uchar('Y')][math_f_normal]="Y";
  math_chars[uchar('Z')][math_f_normal]="Z";
  math_chars[uchar('a')][math_f_normal]="a";
  math_chars[uchar('b')][math_f_normal]="b";
  math_chars[uchar('c')][math_f_normal]="c";
  math_chars[uchar('d')][math_f_normal]="d";
  math_chars[uchar('e')][math_f_normal]="e";
  math_chars[uchar('f')][math_f_normal]="f";
  math_chars[uchar('g')][math_f_normal]="g";
  math_chars[uchar('h')][math_f_normal]="h";
  math_chars[uchar('i')][math_f_normal]="i";
  math_chars[uchar('j')][math_f_normal]="j";
  math_chars[uchar('k')][math_f_normal]="k";
  math_chars[uchar('l')][math_f_normal]="l";
  math_chars[uchar('m')][math_f_normal]="m";
  math_chars[uchar('n')][math_f_normal]="n";
  math_chars[uchar('o')][math_f_normal]="o";
  math_chars[uchar('p')][math_f_normal]="p";
  math_chars[uchar('q')][math_f_normal]="q";
  math_chars[uchar('r')][math_f_normal]="r";
  math_chars[uchar('s')][math_f_normal]="s";
  math_chars[uchar('t')][math_f_normal]="t";
  math_chars[uchar('u')][math_f_normal]="u";
  math_chars[uchar('v')][math_f_normal]="v";
  math_chars[uchar('w')][math_f_normal]="w";
  math_chars[uchar('x')][math_f_normal]="x";
  math_chars[uchar('y')][math_f_normal]="y";
  math_chars[uchar('z')][math_f_normal]="z";

  // Position 1 : upright, not mathml
  math_chars[uchar('A')][math_f_upright]=" A ";
  math_chars[uchar('B')][math_f_upright]=" B ";
  math_chars[uchar('C')][math_f_upright]=" C ";
  math_chars[uchar('D')][math_f_upright]=" D ";
  math_chars[uchar('E')][math_f_upright]=" E ";
  math_chars[uchar('F')][math_f_upright]=" F ";
  math_chars[uchar('G')][math_f_upright]=" G ";
  math_chars[uchar('H')][math_f_upright]=" H ";
  math_chars[uchar('I')][math_f_upright]=" I ";
  math_chars[uchar('J')][math_f_upright]=" J ";
  math_chars[uchar('K')][math_f_upright]=" K ";
  math_chars[uchar('L')][math_f_upright]=" L ";
  math_chars[uchar('M')][math_f_upright]=" M ";
  math_chars[uchar('N')][math_f_upright]=" N ";
  math_chars[uchar('O')][math_f_upright]=" O ";
  math_chars[uchar('P')][math_f_upright]=" P ";
  math_chars[uchar('Q')][math_f_upright]=" Q ";
  math_chars[uchar('R')][math_f_upright]=" R ";
  math_chars[uchar('S')][math_f_upright]=" S ";
  math_chars[uchar('T')][math_f_upright]=" T ";
  math_chars[uchar('U')][math_f_upright]=" U ";
  math_chars[uchar('V')][math_f_upright]=" V ";
  math_chars[uchar('W')][math_f_upright]=" W ";
  math_chars[uchar('X')][math_f_upright]=" X ";
  math_chars[uchar('Y')][math_f_upright]=" Y ";
  math_chars[uchar('Z')][math_f_upright]=" Z ";
  math_chars[uchar('a')][math_f_upright]=" a ";
  math_chars[uchar('b')][math_f_upright]=" b ";
  math_chars[uchar('c')][math_f_upright]=" c ";
  math_chars[uchar('d')][math_f_upright]=" d ";
  math_chars[uchar('e')][math_f_upright]=" e ";
  math_chars[uchar('f')][math_f_upright]=" f ";
  math_chars[uchar('g')][math_f_upright]=" g ";
  math_chars[uchar('h')][math_f_upright]=" h ";
  math_chars[uchar('i')][math_f_upright]=" i ";
  math_chars[uchar('j')][math_f_upright]=" j ";
  math_chars[uchar('k')][math_f_upright]=" k ";
  math_chars[uchar('l')][math_f_upright]=" l ";
  math_chars[uchar('m')][math_f_upright]=" m ";
  math_chars[uchar('n')][math_f_upright]=" n ";
  math_chars[uchar('o')][math_f_upright]=" o ";
  math_chars[uchar('p')][math_f_upright]=" p ";
  math_chars[uchar('q')][math_f_upright]=" q ";
  math_chars[uchar('r')][math_f_upright]=" r ";
  math_chars[uchar('s')][math_f_upright]=" s ";
  math_chars[uchar('t')][math_f_upright]=" t ";
  math_chars[uchar('u')][math_f_upright]=" u ";
  math_chars[uchar('v')][math_f_upright]=" v ";
  math_chars[uchar('w')][math_f_upright]=" w ";
  math_chars[uchar('x')][math_f_upright]=" x ";
  math_chars[uchar('y')][math_f_upright]=" y ";
  math_chars[uchar('z')][math_f_upright]=" z ";
  // Position 2 : bold
  math_chars[uchar('A')][math_f_bold]="&#x1D400;";
  math_chars[uchar('B')][math_f_bold]="&#x1D401;";
  math_chars[uchar('C')][math_f_bold]="&#x1D402;";
  math_chars[uchar('D')][math_f_bold]="&#x1D403;";
  math_chars[uchar('E')][math_f_bold]="&#x1D404;";
  math_chars[uchar('F')][math_f_bold]="&#x1D405;";
  math_chars[uchar('G')][math_f_bold]="&#x1D406;";
  math_chars[uchar('H')][math_f_bold]="&#x1D407;";
  math_chars[uchar('I')][math_f_bold]="&#x1D408;";
  math_chars[uchar('J')][math_f_bold]="&#x1D409;";
  math_chars[uchar('K')][math_f_bold]="&#x1D40A;";
  math_chars[uchar('L')][math_f_bold]="&#x1D40B;";
  math_chars[uchar('M')][math_f_bold]="&#x1D40C;";
  math_chars[uchar('N')][math_f_bold]="&#x1D40D;";
  math_chars[uchar('O')][math_f_bold]="&#x1D40E;";
  math_chars[uchar('P')][math_f_bold]="&#x1D40F;";
  math_chars[uchar('Q')][math_f_bold]="&#x1D410;";
  math_chars[uchar('R')][math_f_bold]="&#x1D411;";
  math_chars[uchar('S')][math_f_bold]="&#x1D412;";
  math_chars[uchar('T')][math_f_bold]="&#x1D413;";
  math_chars[uchar('U')][math_f_bold]="&#x1D414;";
  math_chars[uchar('V')][math_f_bold]="&#x1D415;";
  math_chars[uchar('W')][math_f_bold]="&#x1D416;";
  math_chars[uchar('X')][math_f_bold]="&#x1D417;";
  math_chars[uchar('Y')][math_f_bold]="&#x1D418;";
  math_chars[uchar('Z')][math_f_bold]="&#x1D419;";
  math_chars[uchar('a')][math_f_bold]="&#x1D41A;";
  math_chars[uchar('b')][math_f_bold]="&#x1D41B;";
  math_chars[uchar('c')][math_f_bold]="&#x1D41C;";
  math_chars[uchar('d')][math_f_bold]="&#x1D41D;";
  math_chars[uchar('e')][math_f_bold]="&#x1D41E;";
  math_chars[uchar('f')][math_f_bold]="&#x1D41F;";
  math_chars[uchar('g')][math_f_bold]="&#x1D420;";
  math_chars[uchar('h')][math_f_bold]="&#x1D421;";
  math_chars[uchar('i')][math_f_bold]="&#x1D422;";
  math_chars[uchar('j')][math_f_bold]="&#x1D423;";
  math_chars[uchar('k')][math_f_bold]="&#x1D424;";
  math_chars[uchar('l')][math_f_bold]="&#x1D425;";
  math_chars[uchar('m')][math_f_bold]="&#x1D426;";
  math_chars[uchar('n')][math_f_bold]="&#x1D427;";
  math_chars[uchar('o')][math_f_bold]="&#x1D428;";
  math_chars[uchar('p')][math_f_bold]="&#x1D429;";
  math_chars[uchar('q')][math_f_bold]="&#x1D42A;";
  math_chars[uchar('r')][math_f_bold]="&#x1D42B;";
  math_chars[uchar('s')][math_f_bold]="&#x1D42C;";
  math_chars[uchar('t')][math_f_bold]="&#x1D42D;";
  math_chars[uchar('u')][math_f_bold]="&#x1D42E;";
  math_chars[uchar('v')][math_f_bold]="&#x1D42F;";
  math_chars[uchar('w')][math_f_bold]="&#x1D430;";
  math_chars[uchar('x')][math_f_bold]="&#x1D431;";
  math_chars[uchar('y')][math_f_bold]="&#x1D432;";
  math_chars[uchar('z')][math_f_bold]="&#x1D433;";
  // Position 3 : italic
  math_chars[uchar('A')][math_f_italic]="&#x1D434;";
  math_chars[uchar('B')][math_f_italic]="&#x1D435;";
  math_chars[uchar('C')][math_f_italic]="&#x1D436;";
  math_chars[uchar('D')][math_f_italic]="&#x1D437;";
  math_chars[uchar('E')][math_f_italic]="&#x1D438;";
  math_chars[uchar('F')][math_f_italic]="&#x1D439;";
  math_chars[uchar('G')][math_f_italic]="&#x1D43A;";
  math_chars[uchar('H')][math_f_italic]="&#x1D43B;";
  math_chars[uchar('I')][math_f_italic]="&#x1D43C;";
  math_chars[uchar('J')][math_f_italic]="&#x1D43D;";
  math_chars[uchar('K')][math_f_italic]="&#x1D43E;";
  math_chars[uchar('L')][math_f_italic]="&#x1D43F;";
  math_chars[uchar('M')][math_f_italic]="&#x1D440;";
  math_chars[uchar('N')][math_f_italic]="&#x1D441;";
  math_chars[uchar('O')][math_f_italic]="&#x1D442;";
  math_chars[uchar('P')][math_f_italic]="&#x1D443;";
  math_chars[uchar('Q')][math_f_italic]="&#x1D444;";
  math_chars[uchar('R')][math_f_italic]="&#x1D445;";
  math_chars[uchar('S')][math_f_italic]="&#x1D446;";
  math_chars[uchar('T')][math_f_italic]="&#x1D447;";
  math_chars[uchar('U')][math_f_italic]="&#x1D448;";
  math_chars[uchar('V')][math_f_italic]="&#x1D449;";
  math_chars[uchar('W')][math_f_italic]="&#x1D44A;";
  math_chars[uchar('X')][math_f_italic]="&#x1D44B;";
  math_chars[uchar('Y')][math_f_italic]="&#x1D44C;";
  math_chars[uchar('Z')][math_f_italic]="&#x1D44D;";
  math_chars[uchar('a')][math_f_italic]="&#x1D44E;";
  math_chars[uchar('b')][math_f_italic]="&#x1D44F;";
  math_chars[uchar('c')][math_f_italic]="&#x1D450;";
  math_chars[uchar('d')][math_f_italic]="&#x1D451;";
  math_chars[uchar('e')][math_f_italic]="&#x1D452;";
  math_chars[uchar('f')][math_f_italic]="&#x1D453;";
  math_chars[uchar('g')][math_f_italic]="&#x1D454;";
  math_chars[uchar('h')][math_f_italic]="&#x210E;"; // Plank
  math_chars[uchar('i')][math_f_italic]="&#x1D456;";
  math_chars[uchar('j')][math_f_italic]="&#x1D457;";
  math_chars[uchar('k')][math_f_italic]="&#x1D458;";
  math_chars[uchar('l')][math_f_italic]="&#x1D459;";
  math_chars[uchar('m')][math_f_italic]="&#x1D45A;";
  math_chars[uchar('n')][math_f_italic]="&#x1D45B;";
  math_chars[uchar('o')][math_f_italic]="&#x1D45C;";
  math_chars[uchar('p')][math_f_italic]="&#x1D45D;";
  math_chars[uchar('q')][math_f_italic]="&#x1D45E;";
  math_chars[uchar('r')][math_f_italic]="&#x1D45F;";
  math_chars[uchar('s')][math_f_italic]="&#x1D460;";
  math_chars[uchar('t')][math_f_italic]="&#x1D461;";
  math_chars[uchar('u')][math_f_italic]="&#x1D462;";
  math_chars[uchar('v')][math_f_italic]="&#x1D463;";
  math_chars[uchar('w')][math_f_italic]="&#x1D464;";
  math_chars[uchar('x')][math_f_italic]="&#x1D465;";
  math_chars[uchar('y')][math_f_italic]="&#x1D466;";
  math_chars[uchar('z')][math_f_italic]="&#x1D467;";
  // Position 4: bold-italics
  math_chars[uchar('A')][math_f_bold_italic]="&#x1D468;";
  math_chars[uchar('B')][math_f_bold_italic]="&#x1D469;";
  math_chars[uchar('C')][math_f_bold_italic]="&#x1D46A;";
  math_chars[uchar('D')][math_f_bold_italic]="&#x1D46B;";
  math_chars[uchar('E')][math_f_bold_italic]="&#x1D46C;";
  math_chars[uchar('F')][math_f_bold_italic]="&#x1D46D;";
  math_chars[uchar('G')][math_f_bold_italic]="&#x1D46E;";
  math_chars[uchar('H')][math_f_bold_italic]="&#x1D46F;";
  math_chars[uchar('I')][math_f_bold_italic]="&#x1D470;";
  math_chars[uchar('J')][math_f_bold_italic]="&#x1D471;";
  math_chars[uchar('K')][math_f_bold_italic]="&#x1D472;";
  math_chars[uchar('L')][math_f_bold_italic]="&#x1D473;";
  math_chars[uchar('M')][math_f_bold_italic]="&#x1D474;";
  math_chars[uchar('N')][math_f_bold_italic]="&#x1D475;";
  math_chars[uchar('O')][math_f_bold_italic]="&#x1D476;";
  math_chars[uchar('P')][math_f_bold_italic]="&#x1D477;";
  math_chars[uchar('Q')][math_f_bold_italic]="&#x1D478;";
  math_chars[uchar('R')][math_f_bold_italic]="&#x1D479;";
  math_chars[uchar('S')][math_f_bold_italic]="&#x1D47A;";
  math_chars[uchar('T')][math_f_bold_italic]="&#x1D47B;";
  math_chars[uchar('U')][math_f_bold_italic]="&#x1D47C;";
  math_chars[uchar('V')][math_f_bold_italic]="&#x1D47D;";
  math_chars[uchar('W')][math_f_bold_italic]="&#x1D47E;";
  math_chars[uchar('X')][math_f_bold_italic]="&#x1D47F;";
  math_chars[uchar('Y')][math_f_bold_italic]="&#x1D480;";
  math_chars[uchar('Z')][math_f_bold_italic]="&#x1D481;";
  math_chars[uchar('a')][math_f_bold_italic]="&#x1D482;";
  math_chars[uchar('b')][math_f_bold_italic]="&#x1D483;";
  math_chars[uchar('c')][math_f_bold_italic]="&#x1D484;";
  math_chars[uchar('d')][math_f_bold_italic]="&#x1D485;";
  math_chars[uchar('e')][math_f_bold_italic]="&#x1D486;";
  math_chars[uchar('f')][math_f_bold_italic]="&#x1D487;";
  math_chars[uchar('g')][math_f_bold_italic]="&#x1D488;";
  math_chars[uchar('h')][math_f_bold_italic]="&#x1D489;";
  math_chars[uchar('i')][math_f_bold_italic]="&#x1D48A;";
  math_chars[uchar('j')][math_f_bold_italic]="&#x1D48B;";
  math_chars[uchar('k')][math_f_bold_italic]="&#x1D48C;";
  math_chars[uchar('l')][math_f_bold_italic]="&#x1D48D;";
  math_chars[uchar('m')][math_f_bold_italic]="&#x1D48E;";
  math_chars[uchar('n')][math_f_bold_italic]="&#x1D48F;";
  math_chars[uchar('o')][math_f_bold_italic]="&#x1D490;";
  math_chars[uchar('p')][math_f_bold_italic]="&#x1D491;";
  math_chars[uchar('q')][math_f_bold_italic]="&#x1D492;";
  math_chars[uchar('r')][math_f_bold_italic]="&#x1D493;";
  math_chars[uchar('s')][math_f_bold_italic]="&#x1D494;";
  math_chars[uchar('t')][math_f_bold_italic]="&#x1D495;";
  math_chars[uchar('u')][math_f_bold_italic]="&#x1D496;";
  math_chars[uchar('v')][math_f_bold_italic]="&#x1D497;";
  math_chars[uchar('w')][math_f_bold_italic]="&#x1D498;";
  math_chars[uchar('x')][math_f_bold_italic]="&#x1D499;";
  math_chars[uchar('y')][math_f_bold_italic]="&#x1D49A;";
  math_chars[uchar('z')][math_f_bold_italic]="&#x1D49B;";
  // Position 5: Script
  math_chars[uchar('A')][math_f_script]="&#x1D49C;";
  math_chars[uchar('B')][math_f_script]="&#x212C;"; // Bernoulli
  math_chars[uchar('C')][math_f_script]="&#x1D49E;";
  math_chars[uchar('D')][math_f_script]="&#x1D49F;";
  math_chars[uchar('E')][math_f_script]="&#x2130;";  // Electromotrice force
  math_chars[uchar('F')][math_f_script]="&#x2131;"; // Fourier Transform
  math_chars[uchar('G')][math_f_script]="&#x1D4A2;";
  math_chars[uchar('H')][math_f_script]="&#x210B;"; // Hamiltonian
  math_chars[uchar('I')][math_f_script]="&#x2110;";
  math_chars[uchar('J')][math_f_script]="&#x1D4A5;";
  math_chars[uchar('K')][math_f_script]="&#x1D4A6;";
  math_chars[uchar('L')][math_f_script]="&#x2112;"; // Laplace
  math_chars[uchar('M')][math_f_script]="&#x2133;"; // M-matrix
  math_chars[uchar('N')][math_f_script]="&#x1D4A9;";
  math_chars[uchar('O')][math_f_script]="&#x1D4AA;";
  math_chars[uchar('P')][math_f_script]="&#x1D4AB;";
  math_chars[uchar('Q')][math_f_script]="&#x1D4AC;";
  math_chars[uchar('R')][math_f_script]="&#x211B;"; // Riemann Integral
  math_chars[uchar('S')][math_f_script]="&#x1D4AE;";
  math_chars[uchar('T')][math_f_script]="&#x1D4AF;";
  math_chars[uchar('U')][math_f_script]="&#x1D4B0;";
  math_chars[uchar('V')][math_f_script]="&#x1D4B1;";
  math_chars[uchar('W')][math_f_script]="&#x1D4B2;";
  math_chars[uchar('X')][math_f_script]="&#x1D4B3;";
  math_chars[uchar('Y')][math_f_script]="&#x1D4B4;";
  math_chars[uchar('Z')][math_f_script]="&#x1D4B5;";
  math_chars[uchar('a')][math_f_script]="&#x1D4B6;";
  math_chars[uchar('b')][math_f_script]="&#x1D4B7;";
  math_chars[uchar('c')][math_f_script]="&#x1D4B8;";
  math_chars[uchar('d')][math_f_script]="&#x1D4B9;";
  math_chars[uchar('e')][math_f_script]="&#x212F;"; // error
  math_chars[uchar('f')][math_f_script]="&#x1D4BB;";
  math_chars[uchar('g')][math_f_script]="&#x210A;";
  math_chars[uchar('h')][math_f_script]="&#x1D4BD;";
  math_chars[uchar('i')][math_f_script]="&#x1D4BE;";
  math_chars[uchar('j')][math_f_script]="&#x1D4BF;";
  math_chars[uchar('k')][math_f_script]="&#x1D4C0;";
  math_chars[uchar('l')][math_f_script]="&#x1D4C1;";
  math_chars[uchar('m')][math_f_script]="&#x1D4C2;";
  math_chars[uchar('n')][math_f_script]="&#x1D4C3;";
  math_chars[uchar('o')][math_f_script]="&#x2134;"; // order
  math_chars[uchar('p')][math_f_script]="&#x1D4C5;";
  math_chars[uchar('q')][math_f_script]="&#x1D4C6;";
  math_chars[uchar('r')][math_f_script]="&#x1D4C7;";
  math_chars[uchar('s')][math_f_script]="&#x1D4C8;";
  math_chars[uchar('t')][math_f_script]="&#x1D4C9;";
  math_chars[uchar('u')][math_f_script]="&#x1D4CA;";
  math_chars[uchar('v')][math_f_script]="&#x1D4CB;";
  math_chars[uchar('w')][math_f_script]="&#x1D4CC;";
  math_chars[uchar('x')][math_f_script]="&#x1D4CD;";
  math_chars[uchar('y')][math_f_script]="&#x1D4CE;";
  math_chars[uchar('z')][math_f_script]="&#x1D4CF;";
  // Position 6 bold script
  math_chars[uchar('A')][math_f_bold_script]="&#x1D4D0;";
  math_chars[uchar('B')][math_f_bold_script]="&#x1D4D1;";
  math_chars[uchar('C')][math_f_bold_script]="&#x1D4D2;";
  math_chars[uchar('D')][math_f_bold_script]="&#x1D4D3;";
  math_chars[uchar('E')][math_f_bold_script]="&#x1D4D4;";
  math_chars[uchar('F')][math_f_bold_script]="&#x1D4D5;";
  math_chars[uchar('G')][math_f_bold_script]="&#x1D4D6;";
  math_chars[uchar('H')][math_f_bold_script]="&#x1D4D7;";
  math_chars[uchar('I')][math_f_bold_script]="&#x1D4D8;";
  math_chars[uchar('J')][math_f_bold_script]="&#x1D4D9;";
  math_chars[uchar('K')][math_f_bold_script]="&#x1D4DA;";
  math_chars[uchar('L')][math_f_bold_script]="&#x1D4DB;";
  math_chars[uchar('M')][math_f_bold_script]="&#x1D4DC;";
  math_chars[uchar('N')][math_f_bold_script]="&#x1D4DD;";
  math_chars[uchar('O')][math_f_bold_script]="&#x1D4DE;";
  math_chars[uchar('P')][math_f_bold_script]="&#x1D4DF;";
  math_chars[uchar('Q')][math_f_bold_script]="&#x1D4E0;";
  math_chars[uchar('R')][math_f_bold_script]="&#x1D4E1;";
  math_chars[uchar('S')][math_f_bold_script]="&#x1D4E2;";
  math_chars[uchar('T')][math_f_bold_script]="&#x1D4E3;";
  math_chars[uchar('U')][math_f_bold_script]="&#x1D4E4;";
  math_chars[uchar('V')][math_f_bold_script]="&#x1D4E5;";
  math_chars[uchar('W')][math_f_bold_script]="&#x1D4E6;";
  math_chars[uchar('X')][math_f_bold_script]="&#x1D4E7;";
  math_chars[uchar('Y')][math_f_bold_script]="&#x1D4E8;";
  math_chars[uchar('Z')][math_f_bold_script]="&#x1D4E9;";
  math_chars[uchar('a')][math_f_bold_script]="&#x1D4EA;";
  math_chars[uchar('b')][math_f_bold_script]="&#x1D4EB;";
  math_chars[uchar('c')][math_f_bold_script]="&#x1D4EC;";
  math_chars[uchar('d')][math_f_bold_script]="&#x1D4ED;";
  math_chars[uchar('e')][math_f_bold_script]="&#x1D4EE;";
  math_chars[uchar('f')][math_f_bold_script]="&#x1D4EF;";
  math_chars[uchar('g')][math_f_bold_script]="&#x1D4F0;";
  math_chars[uchar('h')][math_f_bold_script]="&#x1D4F1;";
  math_chars[uchar('i')][math_f_bold_script]="&#x1D4F2;";
  math_chars[uchar('j')][math_f_bold_script]="&#x1D4F3;";
  math_chars[uchar('k')][math_f_bold_script]="&#x1D4F4;";
  math_chars[uchar('l')][math_f_bold_script]="&#x1D4F5;";
  math_chars[uchar('m')][math_f_bold_script]="&#x1D4F6;";
  math_chars[uchar('n')][math_f_bold_script]="&#x1D4F7;";
  math_chars[uchar('o')][math_f_bold_script]="&#x1D4F8;";
  math_chars[uchar('p')][math_f_bold_script]="&#x1D4F9;";
  math_chars[uchar('q')][math_f_bold_script]="&#x1D4FA;";
  math_chars[uchar('r')][math_f_bold_script]="&#x1D4FB;";
  math_chars[uchar('s')][math_f_bold_script]="&#x1D4FC;";
  math_chars[uchar('t')][math_f_bold_script]="&#x1D4FD;";
  math_chars[uchar('u')][math_f_bold_script]="&#x1D4FE;";
  math_chars[uchar('v')][math_f_bold_script]="&#x1D4FF;";
  math_chars[uchar('w')][math_f_bold_script]="&#x1D500;";
  math_chars[uchar('x')][math_f_bold_script]="&#x1D501;";
  math_chars[uchar('y')][math_f_bold_script]="&#x1D502;";
  math_chars[uchar('z')][math_f_bold_script]="&#x1D503;";
  // Position 7 Fraktur
  math_chars[uchar('A')][math_f_fraktur]="&#x1D504;";
  math_chars[uchar('B')][math_f_fraktur]="&#x1D505;";
  math_chars[uchar('C')][math_f_fraktur]="&#x212D;"; // complex
  math_chars[uchar('D')][math_f_fraktur]="&#x1D507;";
  math_chars[uchar('E')][math_f_fraktur]="&#x1D508;";
  math_chars[uchar('F')][math_f_fraktur]="&#x1D509;";
  math_chars[uchar('G')][math_f_fraktur]="&#x1D50A;";
  math_chars[uchar('H')][math_f_fraktur]="&#x210C;"; // Hilbert space
  math_chars[uchar('I')][math_f_fraktur]="&#x2111;"; // imaginary part
  math_chars[uchar('J')][math_f_fraktur]="&#x1D50D;";
  math_chars[uchar('K')][math_f_fraktur]="&#x1D50E;";
  math_chars[uchar('L')][math_f_fraktur]="&#x1D50F;";
  math_chars[uchar('M')][math_f_fraktur]="&#x1D510;";
  math_chars[uchar('N')][math_f_fraktur]="&#x1D511;";
  math_chars[uchar('O')][math_f_fraktur]="&#x1D512;";
  math_chars[uchar('P')][math_f_fraktur]="&#x1D513;";
  math_chars[uchar('Q')][math_f_fraktur]="&#x1D514;";
  math_chars[uchar('R')][math_f_fraktur]="&#x211C;"; // real part
  math_chars[uchar('S')][math_f_fraktur]="&#x1D516;";
  math_chars[uchar('T')][math_f_fraktur]="&#x1D517;";
  math_chars[uchar('U')][math_f_fraktur]="&#x1D518;";
  math_chars[uchar('V')][math_f_fraktur]="&#x1D519;";
  math_chars[uchar('W')][math_f_fraktur]="&#x1D51A;";
  math_chars[uchar('X')][math_f_fraktur]="&#x1D51B;";
  math_chars[uchar('Y')][math_f_fraktur]="&#x1D51C;";
  math_chars[uchar('Z')][math_f_fraktur]="&#x2128;"; // integers
  math_chars[uchar('a')][math_f_fraktur]="&#x1D51E;";
  math_chars[uchar('b')][math_f_fraktur]="&#x1D51F;";
  math_chars[uchar('c')][math_f_fraktur]="&#x1D520;";
  math_chars[uchar('d')][math_f_fraktur]="&#x1D521;";
  math_chars[uchar('e')][math_f_fraktur]="&#x1D522;";
  math_chars[uchar('f')][math_f_fraktur]="&#x1D523;";
  math_chars[uchar('g')][math_f_fraktur]="&#x1D524;";
  math_chars[uchar('h')][math_f_fraktur]="&#x1D525;";
  math_chars[uchar('i')][math_f_fraktur]="&#x1D526;";
  math_chars[uchar('j')][math_f_fraktur]="&#x1D527;";
  math_chars[uchar('k')][math_f_fraktur]="&#x1D528;";
  math_chars[uchar('l')][math_f_fraktur]="&#x1D529;";
  math_chars[uchar('m')][math_f_fraktur]="&#x1D52A;";
  math_chars[uchar('n')][math_f_fraktur]="&#x1D52B;";
  math_chars[uchar('o')][math_f_fraktur]="&#x1D52C;";
  math_chars[uchar('p')][math_f_fraktur]="&#x1D52D;";
  math_chars[uchar('q')][math_f_fraktur]="&#x1D52E;";
  math_chars[uchar('r')][math_f_fraktur]="&#x1D52F;";
  math_chars[uchar('s')][math_f_fraktur]="&#x1D530;";
  math_chars[uchar('t')][math_f_fraktur]="&#x1D531;";
  math_chars[uchar('u')][math_f_fraktur]="&#x1D532;";
  math_chars[uchar('v')][math_f_fraktur]="&#x1D533;";
  math_chars[uchar('w')][math_f_fraktur]="&#x1D534;";
  math_chars[uchar('x')][math_f_fraktur]="&#x1D535;";
  math_chars[uchar('y')][math_f_fraktur]="&#x1D536;";
  math_chars[uchar('z')][math_f_fraktur]="&#x1D537;";
  // Position  8 double struck
  math_chars[uchar('A')][math_f_doublestruck]="&#x1D538;";
  math_chars[uchar('B')][math_f_doublestruck]="&#x1D539;";
  math_chars[uchar('C')][math_f_doublestruck]="&#x2102;"; // complex numbers
  math_chars[uchar('D')][math_f_doublestruck]="&#x1D53B;";
  math_chars[uchar('E')][math_f_doublestruck]="&#x1D53C;";
  math_chars[uchar('F')][math_f_doublestruck]="&#x1D53D;";
  math_chars[uchar('G')][math_f_doublestruck]="&#x1D53E;";
  math_chars[uchar('H')][math_f_doublestruck]="&#x210D;"; // quaternions
  math_chars[uchar('I')][math_f_doublestruck]="&#x1D540;";
  math_chars[uchar('J')][math_f_doublestruck]="&#x1D541;";
  math_chars[uchar('K')][math_f_doublestruck]="&#x1D542;";
  math_chars[uchar('L')][math_f_doublestruck]="&#x1D543;";
  math_chars[uchar('M')][math_f_doublestruck]="&#x1D544;";
  math_chars[uchar('N')][math_f_doublestruck]="&#x2115;"; // natural numbers
  math_chars[uchar('O')][math_f_doublestruck]="&#x1D546;";
  math_chars[uchar('P')][math_f_doublestruck]="&#x2119;"; // projective
  math_chars[uchar('Q')][math_f_doublestruck]="&#x211A;"; // rational numbers
  math_chars[uchar('R')][math_f_doublestruck]="&#x211D;"; // real numbers
  math_chars[uchar('S')][math_f_doublestruck]="&#x1D54A;";
  math_chars[uchar('T')][math_f_doublestruck]="&#x1D54B;";
  math_chars[uchar('U')][math_f_doublestruck]="&#x1D54C;";
  math_chars[uchar('V')][math_f_doublestruck]="&#x1D54D;";
  math_chars[uchar('W')][math_f_doublestruck]="&#x1D54E;";
  math_chars[uchar('X')][math_f_doublestruck]="&#x1D54F;";
  math_chars[uchar('Y')][math_f_doublestruck]="&#x1D550;";
  math_chars[uchar('Z')][math_f_doublestruck]="&#x2124;"; // integers
  math_chars[uchar('a')][math_f_doublestruck]="&#x1D552;";
  math_chars[uchar('b')][math_f_doublestruck]="&#x1D553;";
  math_chars[uchar('c')][math_f_doublestruck]="&#x1D554;";
  math_chars[uchar('d')][math_f_doublestruck]="&#x1D555;";
  math_chars[uchar('e')][math_f_doublestruck]="&#x1D556;";
  math_chars[uchar('f')][math_f_doublestruck]="&#x1D557;";
  math_chars[uchar('g')][math_f_doublestruck]="&#x1D558;";
  math_chars[uchar('h')][math_f_doublestruck]="&#x1D559;";
  math_chars[uchar('i')][math_f_doublestruck]="&#x1D55A;";
  math_chars[uchar('j')][math_f_doublestruck]="&#x1D55B;";
  math_chars[uchar('k')][math_f_doublestruck]="&#x1D55C;";
  math_chars[uchar('l')][math_f_doublestruck]="&#x1D55D;";
  math_chars[uchar('m')][math_f_doublestruck]="&#x1D55E;";
  math_chars[uchar('n')][math_f_doublestruck]="&#x1D55F;";
  math_chars[uchar('o')][math_f_doublestruck]="&#x1D560;";
  math_chars[uchar('p')][math_f_doublestruck]="&#x1D561;";
  math_chars[uchar('q')][math_f_doublestruck]="&#x1D562;";
  math_chars[uchar('r')][math_f_doublestruck]="&#x1D563;";
  math_chars[uchar('s')][math_f_doublestruck]="&#x1D564;";
  math_chars[uchar('t')][math_f_doublestruck]="&#x1D565;";
  math_chars[uchar('u')][math_f_doublestruck]="&#x1D566;";
  math_chars[uchar('v')][math_f_doublestruck]="&#x1D567;";
  math_chars[uchar('w')][math_f_doublestruck]="&#x1D568;";
  math_chars[uchar('x')][math_f_doublestruck]="&#x1D569;";
  math_chars[uchar('y')][math_f_doublestruck]="&#x1D56A;";
  math_chars[uchar('z')][math_f_doublestruck]="&#x1D56B;";
  // Position 9 bold-fraktur
  math_chars[uchar('A')][math_f_bold_fraktur]="&#x1D56C;";
  math_chars[uchar('B')][math_f_bold_fraktur]="&#x1D56D;";
  math_chars[uchar('C')][math_f_bold_fraktur]="&#x1D56E;";
  math_chars[uchar('D')][math_f_bold_fraktur]="&#x1D56F;";
  math_chars[uchar('E')][math_f_bold_fraktur]="&#x1D570;";
  math_chars[uchar('F')][math_f_bold_fraktur]="&#x1D571;";
  math_chars[uchar('G')][math_f_bold_fraktur]="&#x1D572;";
  math_chars[uchar('H')][math_f_bold_fraktur]="&#x1D573;";
  math_chars[uchar('I')][math_f_bold_fraktur]="&#x1D574;";
  math_chars[uchar('J')][math_f_bold_fraktur]="&#x1D575;";
  math_chars[uchar('K')][math_f_bold_fraktur]="&#x1D576;";
  math_chars[uchar('L')][math_f_bold_fraktur]="&#x1D577;";
  math_chars[uchar('M')][math_f_bold_fraktur]="&#x1D578;";
  math_chars[uchar('N')][math_f_bold_fraktur]="&#x1D579;";
  math_chars[uchar('O')][math_f_bold_fraktur]="&#x1D57A;";
  math_chars[uchar('P')][math_f_bold_fraktur]="&#x1D57B;";
  math_chars[uchar('Q')][math_f_bold_fraktur]="&#x1D57C;";
  math_chars[uchar('R')][math_f_bold_fraktur]="&#x1D57D;";
  math_chars[uchar('S')][math_f_bold_fraktur]="&#x1D57E;";
  math_chars[uchar('T')][math_f_bold_fraktur]="&#x1D57F;";
  math_chars[uchar('U')][math_f_bold_fraktur]="&#x1D580;";
  math_chars[uchar('V')][math_f_bold_fraktur]="&#x1D581;";
  math_chars[uchar('W')][math_f_bold_fraktur]="&#x1D582;";
  math_chars[uchar('X')][math_f_bold_fraktur]="&#x1D583;";
  math_chars[uchar('Y')][math_f_bold_fraktur]="&#x1D584;";
  math_chars[uchar('Z')][math_f_bold_fraktur]="&#x1D585;";
  math_chars[uchar('a')][math_f_bold_fraktur]="&#x1D586;";
  math_chars[uchar('b')][math_f_bold_fraktur]="&#x1D587;";
  math_chars[uchar('c')][math_f_bold_fraktur]="&#x1D588;";
  math_chars[uchar('d')][math_f_bold_fraktur]="&#x1D589;";
  math_chars[uchar('e')][math_f_bold_fraktur]="&#x1D58A;";
  math_chars[uchar('f')][math_f_bold_fraktur]="&#x1D58B;";
  math_chars[uchar('g')][math_f_bold_fraktur]="&#x1D58C;";
  math_chars[uchar('h')][math_f_bold_fraktur]="&#x1D58D;";
  math_chars[uchar('i')][math_f_bold_fraktur]="&#x1D58E;";
  math_chars[uchar('j')][math_f_bold_fraktur]="&#x1D58F;";
  math_chars[uchar('k')][math_f_bold_fraktur]="&#x1D590;";
  math_chars[uchar('l')][math_f_bold_fraktur]="&#x1D591;";
  math_chars[uchar('m')][math_f_bold_fraktur]="&#x1D592;";
  math_chars[uchar('n')][math_f_bold_fraktur]="&#x1D593;";
  math_chars[uchar('o')][math_f_bold_fraktur]="&#x1D594;";
  math_chars[uchar('p')][math_f_bold_fraktur]="&#x1D595;";
  math_chars[uchar('q')][math_f_bold_fraktur]="&#x1D596;";
  math_chars[uchar('r')][math_f_bold_fraktur]="&#x1D597;";
  math_chars[uchar('s')][math_f_bold_fraktur]="&#x1D598;";
  math_chars[uchar('t')][math_f_bold_fraktur]="&#x1D599;";
  math_chars[uchar('u')][math_f_bold_fraktur]="&#x1D59A;";
  math_chars[uchar('v')][math_f_bold_fraktur]="&#x1D59B;";
  math_chars[uchar('w')][math_f_bold_fraktur]="&#x1D59C;";
  math_chars[uchar('x')][math_f_bold_fraktur]="&#x1D59D;";
  math_chars[uchar('y')][math_f_bold_fraktur]="&#x1D59E;";
  math_chars[uchar('z')][math_f_bold_fraktur]="&#x1D59F;";
  // Position 10 sans serif
  math_chars[uchar('A')][math_f_sansserif]="&#x1D5A0;";
  math_chars[uchar('B')][math_f_sansserif]="&#x1D5A1;";
  math_chars[uchar('C')][math_f_sansserif]="&#x1D5A2;";
  math_chars[uchar('D')][math_f_sansserif]="&#x1D5A3;";
  math_chars[uchar('E')][math_f_sansserif]="&#x1D5A4;";
  math_chars[uchar('F')][math_f_sansserif]="&#x1D5A5;";
  math_chars[uchar('G')][math_f_sansserif]="&#x1D5A6;";
  math_chars[uchar('H')][math_f_sansserif]="&#x1D5A7;";
  math_chars[uchar('I')][math_f_sansserif]="&#x1D5A8;";
  math_chars[uchar('J')][math_f_sansserif]="&#x1D5A9;";
  math_chars[uchar('K')][math_f_sansserif]="&#x1D5AA;";
  math_chars[uchar('L')][math_f_sansserif]="&#x1D5AB;";
  math_chars[uchar('M')][math_f_sansserif]="&#x1D5AC;";
  math_chars[uchar('N')][math_f_sansserif]="&#x1D5AD;";
  math_chars[uchar('O')][math_f_sansserif]="&#x1D5AE;";
  math_chars[uchar('P')][math_f_sansserif]="&#x1D5AF;";
  math_chars[uchar('Q')][math_f_sansserif]="&#x1D5B0;";
  math_chars[uchar('R')][math_f_sansserif]="&#x1D5B1;";
  math_chars[uchar('S')][math_f_sansserif]="&#x1D5B2;";
  math_chars[uchar('T')][math_f_sansserif]="&#x1D5B3;";
  math_chars[uchar('U')][math_f_sansserif]="&#x1D5B4;";
  math_chars[uchar('V')][math_f_sansserif]="&#x1D5B5;";
  math_chars[uchar('W')][math_f_sansserif]="&#x1D5B6;";
  math_chars[uchar('X')][math_f_sansserif]="&#x1D5B7;";
  math_chars[uchar('Y')][math_f_sansserif]="&#x1D5B8;";
  math_chars[uchar('Z')][math_f_sansserif]="&#x1D5B9;";
  math_chars[uchar('a')][math_f_sansserif]="&#x1D5BA;";
  math_chars[uchar('b')][math_f_sansserif]="&#x1D5BB;";
  math_chars[uchar('c')][math_f_sansserif]="&#x1D5BC;";
  math_chars[uchar('d')][math_f_sansserif]="&#x1D5BD;";
  math_chars[uchar('e')][math_f_sansserif]="&#x1D5BE;";
  math_chars[uchar('f')][math_f_sansserif]="&#x1D5BF;";
  math_chars[uchar('g')][math_f_sansserif]="&#x1D5C0;";
  math_chars[uchar('h')][math_f_sansserif]="&#x1D5C1;";
  math_chars[uchar('i')][math_f_sansserif]="&#x1D5C2;";
  math_chars[uchar('j')][math_f_sansserif]="&#x1D5C3;";
  math_chars[uchar('k')][math_f_sansserif]="&#x1D5C4;";
  math_chars[uchar('l')][math_f_sansserif]="&#x1D5C5;";
  math_chars[uchar('m')][math_f_sansserif]="&#x1D5C6;";
  math_chars[uchar('n')][math_f_sansserif]="&#x1D5C7;";
  math_chars[uchar('o')][math_f_sansserif]="&#x1D5C8;";
  math_chars[uchar('p')][math_f_sansserif]="&#x1D5C9;";
  math_chars[uchar('q')][math_f_sansserif]="&#x1D5CA;";
  math_chars[uchar('r')][math_f_sansserif]="&#x1D5CB;";
  math_chars[uchar('s')][math_f_sansserif]="&#x1D5CC;";
  math_chars[uchar('t')][math_f_sansserif]="&#x1D5CD;";
  math_chars[uchar('u')][math_f_sansserif]="&#x1D5CE;";
  math_chars[uchar('v')][math_f_sansserif]="&#x1D5CF;";
  math_chars[uchar('w')][math_f_sansserif]="&#x1D5D0;";
  math_chars[uchar('x')][math_f_sansserif]="&#x1D5D1;";
  math_chars[uchar('y')][math_f_sansserif]="&#x1D5D2;";
  math_chars[uchar('z')][math_f_sansserif]="&#x1D5D3;";
  // Position 11 bold sans serif
  math_chars[uchar('A')][math_f_bold_sansserif]="&#x1D5D4;";
  math_chars[uchar('B')][math_f_bold_sansserif]="&#x1D5D5;";
  math_chars[uchar('C')][math_f_bold_sansserif]="&#x1D5D6;";
  math_chars[uchar('D')][math_f_bold_sansserif]="&#x1D5D7;";
  math_chars[uchar('E')][math_f_bold_sansserif]="&#x1D5D8;";
  math_chars[uchar('F')][math_f_bold_sansserif]="&#x1D5D9;";
  math_chars[uchar('G')][math_f_bold_sansserif]="&#x1D5DA;";
  math_chars[uchar('H')][math_f_bold_sansserif]="&#x1D5DB;";
  math_chars[uchar('I')][math_f_bold_sansserif]="&#x1D5DC;";
  math_chars[uchar('J')][math_f_bold_sansserif]="&#x1D5DD;";
  math_chars[uchar('K')][math_f_bold_sansserif]="&#x1D5DE;";
  math_chars[uchar('L')][math_f_bold_sansserif]="&#x1D5DF;";
  math_chars[uchar('M')][math_f_bold_sansserif]="&#x1D5E0;";
  math_chars[uchar('N')][math_f_bold_sansserif]="&#x1D5E1;";
  math_chars[uchar('O')][math_f_bold_sansserif]="&#x1D5E2;";
  math_chars[uchar('P')][math_f_bold_sansserif]="&#x1D5E3;";
  math_chars[uchar('Q')][math_f_bold_sansserif]="&#x1D5E4;";
  math_chars[uchar('R')][math_f_bold_sansserif]="&#x1D5E5;";
  math_chars[uchar('S')][math_f_bold_sansserif]="&#x1D5E6;";
  math_chars[uchar('T')][math_f_bold_sansserif]="&#x1D5E7;";
  math_chars[uchar('U')][math_f_bold_sansserif]="&#x1D5E8;";
  math_chars[uchar('V')][math_f_bold_sansserif]="&#x1D5E9;";
  math_chars[uchar('W')][math_f_bold_sansserif]="&#x1D5EA;";
  math_chars[uchar('X')][math_f_bold_sansserif]="&#x1D5EB;";
  math_chars[uchar('Y')][math_f_bold_sansserif]="&#x1D5EC;";
  math_chars[uchar('Z')][math_f_bold_sansserif]="&#x1D5ED;";
  math_chars[uchar('a')][math_f_bold_sansserif]="&#x1D5EE;";
  math_chars[uchar('b')][math_f_bold_sansserif]="&#x1D5EF;";
  math_chars[uchar('c')][math_f_bold_sansserif]="&#x1D5F0;";
  math_chars[uchar('d')][math_f_bold_sansserif]="&#x1D5F1;";
  math_chars[uchar('e')][math_f_bold_sansserif]="&#x1D5F2;";
  math_chars[uchar('f')][math_f_bold_sansserif]="&#x1D5F3;";
  math_chars[uchar('g')][math_f_bold_sansserif]="&#x1D5F4;";
  math_chars[uchar('h')][math_f_bold_sansserif]="&#x1D5F5;";
  math_chars[uchar('i')][math_f_bold_sansserif]="&#x1D5F6;";
  math_chars[uchar('j')][math_f_bold_sansserif]="&#x1D5F7;";
  math_chars[uchar('k')][math_f_bold_sansserif]="&#x1D5F8;";
  math_chars[uchar('l')][math_f_bold_sansserif]="&#x1D5F9;";
  math_chars[uchar('m')][math_f_bold_sansserif]="&#x1D5FA;";
  math_chars[uchar('n')][math_f_bold_sansserif]="&#x1D5FB;";
  math_chars[uchar('o')][math_f_bold_sansserif]="&#x1D5FC;";
  math_chars[uchar('p')][math_f_bold_sansserif]="&#x1D5FD;";
  math_chars[uchar('q')][math_f_bold_sansserif]="&#x1D5FE;";
  math_chars[uchar('r')][math_f_bold_sansserif]="&#x1D5FF;";
  math_chars[uchar('s')][math_f_bold_sansserif]="&#x1D600;";
  math_chars[uchar('t')][math_f_bold_sansserif]="&#x1D601;";
  math_chars[uchar('u')][math_f_bold_sansserif]="&#x1D602;";
  math_chars[uchar('v')][math_f_bold_sansserif]="&#x1D603;";
  math_chars[uchar('w')][math_f_bold_sansserif]="&#x1D604;";
  math_chars[uchar('x')][math_f_bold_sansserif]="&#x1D605;";
  math_chars[uchar('y')][math_f_bold_sansserif]="&#x1D606;";
  math_chars[uchar('z')][math_f_bold_sansserif]="&#x1D607;";
  // Position 12 sans serif italics
  math_chars[uchar('A')][math_f_sansserif_italic]="&#x1D608;";
  math_chars[uchar('B')][math_f_sansserif_italic]="&#x1D609;";
  math_chars[uchar('C')][math_f_sansserif_italic]="&#x1D60A;";
  math_chars[uchar('D')][math_f_sansserif_italic]="&#x1D60B;";
  math_chars[uchar('E')][math_f_sansserif_italic]="&#x1D60C;";
  math_chars[uchar('F')][math_f_sansserif_italic]="&#x1D60D;";
  math_chars[uchar('G')][math_f_sansserif_italic]="&#x1D60E;";
  math_chars[uchar('H')][math_f_sansserif_italic]="&#x1D60F;";
  math_chars[uchar('I')][math_f_sansserif_italic]="&#x1D610;";
  math_chars[uchar('J')][math_f_sansserif_italic]="&#x1D611;";
  math_chars[uchar('K')][math_f_sansserif_italic]="&#x1D612;";
  math_chars[uchar('L')][math_f_sansserif_italic]="&#x1D613;";
  math_chars[uchar('M')][math_f_sansserif_italic]="&#x1D614;";
  math_chars[uchar('N')][math_f_sansserif_italic]="&#x1D615;";
  math_chars[uchar('O')][math_f_sansserif_italic]="&#x1D616;";
  math_chars[uchar('P')][math_f_sansserif_italic]="&#x1D617;";
  math_chars[uchar('Q')][math_f_sansserif_italic]="&#x1D618;";
  math_chars[uchar('R')][math_f_sansserif_italic]="&#x1D619;";
  math_chars[uchar('S')][math_f_sansserif_italic]="&#x1D61A;";
  math_chars[uchar('T')][math_f_sansserif_italic]="&#x1D61B;";
  math_chars[uchar('U')][math_f_sansserif_italic]="&#x1D61C;";
  math_chars[uchar('V')][math_f_sansserif_italic]="&#x1D61D;";
  math_chars[uchar('W')][math_f_sansserif_italic]="&#x1D61E;";
  math_chars[uchar('X')][math_f_sansserif_italic]="&#x1D61F;";
  math_chars[uchar('Y')][math_f_sansserif_italic]="&#x1D620;";
  math_chars[uchar('Z')][math_f_sansserif_italic]="&#x1D621;";
  math_chars[uchar('a')][math_f_sansserif_italic]="&#x1D622;";
  math_chars[uchar('b')][math_f_sansserif_italic]="&#x1D623;";
  math_chars[uchar('c')][math_f_sansserif_italic]="&#x1D624;";
  math_chars[uchar('d')][math_f_sansserif_italic]="&#x1D625;";
  math_chars[uchar('e')][math_f_sansserif_italic]="&#x1D626;";
  math_chars[uchar('f')][math_f_sansserif_italic]="&#x1D627;";
  math_chars[uchar('g')][math_f_sansserif_italic]="&#x1D628;";
  math_chars[uchar('h')][math_f_sansserif_italic]="&#x1D629;";
  math_chars[uchar('i')][math_f_sansserif_italic]="&#x1D62A;";
  math_chars[uchar('j')][math_f_sansserif_italic]="&#x1D62B;";
  math_chars[uchar('k')][math_f_sansserif_italic]="&#x1D62C;";
  math_chars[uchar('l')][math_f_sansserif_italic]="&#x1D62D;";
  math_chars[uchar('m')][math_f_sansserif_italic]="&#x1D62E;";
  math_chars[uchar('n')][math_f_sansserif_italic]="&#x1D62F;";
  math_chars[uchar('o')][math_f_sansserif_italic]="&#x1D630;";
  math_chars[uchar('p')][math_f_sansserif_italic]="&#x1D631;";
  math_chars[uchar('q')][math_f_sansserif_italic]="&#x1D632;";
  math_chars[uchar('r')][math_f_sansserif_italic]="&#x1D633;";
  math_chars[uchar('s')][math_f_sansserif_italic]="&#x1D634;";
  math_chars[uchar('t')][math_f_sansserif_italic]="&#x1D635;";
  math_chars[uchar('u')][math_f_sansserif_italic]="&#x1D636;";
  math_chars[uchar('v')][math_f_sansserif_italic]="&#x1D637;";
  math_chars[uchar('w')][math_f_sansserif_italic]="&#x1D638;";
  math_chars[uchar('x')][math_f_sansserif_italic]="&#x1D639;";
  math_chars[uchar('y')][math_f_sansserif_italic]="&#x1D63A;";
  math_chars[uchar('z')][math_f_sansserif_italic]="&#x1D63B;";
  // Position 13 sans serif bold italics
  math_chars[uchar('A')][math_f_sansserif_bold_italic]="&#x1D63C;";
  math_chars[uchar('B')][math_f_sansserif_bold_italic]="&#x1D63D;";
  math_chars[uchar('C')][math_f_sansserif_bold_italic]="&#x1D63E;";
  math_chars[uchar('D')][math_f_sansserif_bold_italic]="&#x1D63F;";
  math_chars[uchar('E')][math_f_sansserif_bold_italic]="&#x1D640;";
  math_chars[uchar('F')][math_f_sansserif_bold_italic]="&#x1D641;";
  math_chars[uchar('G')][math_f_sansserif_bold_italic]="&#x1D642;";
  math_chars[uchar('H')][math_f_sansserif_bold_italic]="&#x1D643;";
  math_chars[uchar('I')][math_f_sansserif_bold_italic]="&#x1D644;";
  math_chars[uchar('J')][math_f_sansserif_bold_italic]="&#x1D645;";
  math_chars[uchar('K')][math_f_sansserif_bold_italic]="&#x1D646;";
  math_chars[uchar('L')][math_f_sansserif_bold_italic]="&#x1D647;";
  math_chars[uchar('M')][math_f_sansserif_bold_italic]="&#x1D648;";
  math_chars[uchar('N')][math_f_sansserif_bold_italic]="&#x1D649;";
  math_chars[uchar('O')][math_f_sansserif_bold_italic]="&#x1D64A;";
  math_chars[uchar('P')][math_f_sansserif_bold_italic]="&#x1D64B;";
  math_chars[uchar('Q')][math_f_sansserif_bold_italic]="&#x1D64C;";
  math_chars[uchar('R')][math_f_sansserif_bold_italic]="&#x1D64D;";
  math_chars[uchar('S')][math_f_sansserif_bold_italic]="&#x1D64E;";
  math_chars[uchar('T')][math_f_sansserif_bold_italic]="&#x1D64F;";
  math_chars[uchar('U')][math_f_sansserif_bold_italic]="&#x1D650;";
  math_chars[uchar('V')][math_f_sansserif_bold_italic]="&#x1D651;";
  math_chars[uchar('W')][math_f_sansserif_bold_italic]="&#x1D652;";
  math_chars[uchar('X')][math_f_sansserif_bold_italic]="&#x1D653;";
  math_chars[uchar('Y')][math_f_sansserif_bold_italic]="&#x1D654;";
  math_chars[uchar('Z')][math_f_sansserif_bold_italic]="&#x1D655;";
  math_chars[uchar('a')][math_f_sansserif_bold_italic]="&#x1D656;";
  math_chars[uchar('b')][math_f_sansserif_bold_italic]="&#x1D657;";
  math_chars[uchar('c')][math_f_sansserif_bold_italic]="&#x1D658;";
  math_chars[uchar('d')][math_f_sansserif_bold_italic]="&#x1D659;";
  math_chars[uchar('e')][math_f_sansserif_bold_italic]="&#x1D65A;";
  math_chars[uchar('f')][math_f_sansserif_bold_italic]="&#x1D65B;";
  math_chars[uchar('g')][math_f_sansserif_bold_italic]="&#x1D65C;";
  math_chars[uchar('h')][math_f_sansserif_bold_italic]="&#x1D65D;";
  math_chars[uchar('i')][math_f_sansserif_bold_italic]="&#x1D65E;";
  math_chars[uchar('j')][math_f_sansserif_bold_italic]="&#x1D65F;";
  math_chars[uchar('k')][math_f_sansserif_bold_italic]="&#x1D660;";
  math_chars[uchar('l')][math_f_sansserif_bold_italic]="&#x1D661;";
  math_chars[uchar('m')][math_f_sansserif_bold_italic]="&#x1D662;";
  math_chars[uchar('n')][math_f_sansserif_bold_italic]="&#x1D663;";
  math_chars[uchar('o')][math_f_sansserif_bold_italic]="&#x1D664;";
  math_chars[uchar('p')][math_f_sansserif_bold_italic]="&#x1D665;";
  math_chars[uchar('q')][math_f_sansserif_bold_italic]="&#x1D666;";
  math_chars[uchar('r')][math_f_sansserif_bold_italic]="&#x1D667;";
  math_chars[uchar('s')][math_f_sansserif_bold_italic]="&#x1D668;";
  math_chars[uchar('t')][math_f_sansserif_bold_italic]="&#x1D669;";
  math_chars[uchar('u')][math_f_sansserif_bold_italic]="&#x1D66A;";
  math_chars[uchar('v')][math_f_sansserif_bold_italic]="&#x1D66B;";
  math_chars[uchar('w')][math_f_sansserif_bold_italic]="&#x1D66C;";
  math_chars[uchar('x')][math_f_sansserif_bold_italic]="&#x1D66D;";
  math_chars[uchar('y')][math_f_sansserif_bold_italic]="&#x1D66E;";
  math_chars[uchar('z')][math_f_sansserif_bold_italic]="&#x1D66F;";
  // Position 14 : monospace
  math_chars[uchar('A')][math_f_monospace]="&#x1D670;";
  math_chars[uchar('B')][math_f_monospace]="&#x1D671;";
  math_chars[uchar('C')][math_f_monospace]="&#x1D672;";
  math_chars[uchar('D')][math_f_monospace]="&#x1D673;";
  math_chars[uchar('E')][math_f_monospace]="&#x1D674;";
  math_chars[uchar('F')][math_f_monospace]="&#x1D675;";
  math_chars[uchar('G')][math_f_monospace]="&#x1D676;";
  math_chars[uchar('H')][math_f_monospace]="&#x1D677;";
  math_chars[uchar('I')][math_f_monospace]="&#x1D678;";
  math_chars[uchar('J')][math_f_monospace]="&#x1D679;";
  math_chars[uchar('K')][math_f_monospace]="&#x1D67A;";
  math_chars[uchar('L')][math_f_monospace]="&#x1D67B;";
  math_chars[uchar('M')][math_f_monospace]="&#x1D67C;";
  math_chars[uchar('N')][math_f_monospace]="&#x1D67D;";
  math_chars[uchar('O')][math_f_monospace]="&#x1D67E;";
  math_chars[uchar('P')][math_f_monospace]="&#x1D67F;";
  math_chars[uchar('Q')][math_f_monospace]="&#x1D680;";
  math_chars[uchar('R')][math_f_monospace]="&#x1D681;";
  math_chars[uchar('S')][math_f_monospace]="&#x1D682;";
  math_chars[uchar('T')][math_f_monospace]="&#x1D683;";
  math_chars[uchar('U')][math_f_monospace]="&#x1D684;";
  math_chars[uchar('V')][math_f_monospace]="&#x1D685;";
  math_chars[uchar('W')][math_f_monospace]="&#x1D686;";
  math_chars[uchar('X')][math_f_monospace]="&#x1D687;";
  math_chars[uchar('Y')][math_f_monospace]="&#x1D688;";
  math_chars[uchar('Z')][math_f_monospace]="&#x1D689;";
  math_chars[uchar('a')][math_f_monospace]="&#x1D68A;";
  math_chars[uchar('b')][math_f_monospace]="&#x1D68B;";
  math_chars[uchar('c')][math_f_monospace]="&#x1D68C;";
  math_chars[uchar('d')][math_f_monospace]="&#x1D68D;";
  math_chars[uchar('e')][math_f_monospace]="&#x1D68E;";
  math_chars[uchar('f')][math_f_monospace]="&#x1D68F;";
  math_chars[uchar('g')][math_f_monospace]="&#x1D690;";
  math_chars[uchar('h')][math_f_monospace]="&#x1D691;";
  math_chars[uchar('i')][math_f_monospace]="&#x1D692;";
  math_chars[uchar('j')][math_f_monospace]="&#x1D693;";
  math_chars[uchar('k')][math_f_monospace]="&#x1D694;";
  math_chars[uchar('l')][math_f_monospace]="&#x1D695;";
  math_chars[uchar('m')][math_f_monospace]="&#x1D696;";
  math_chars[uchar('n')][math_f_monospace]="&#x1D697;";
  math_chars[uchar('o')][math_f_monospace]="&#x1D698;";
  math_chars[uchar('p')][math_f_monospace]="&#x1D699;";
  math_chars[uchar('q')][math_f_monospace]="&#x1D69A;";
  math_chars[uchar('r')][math_f_monospace]="&#x1D69B;";
  math_chars[uchar('s')][math_f_monospace]="&#x1D69C;";
  math_chars[uchar('t')][math_f_monospace]="&#x1D69D;";
  math_chars[uchar('u')][math_f_monospace]="&#x1D69E;";
  math_chars[uchar('v')][math_f_monospace]="&#x1D69F;";
  math_chars[uchar('w')][math_f_monospace]="&#x1D6A0;";
  math_chars[uchar('x')][math_f_monospace]="&#x1D6A1;";
  math_chars[uchar('y')][math_f_monospace]="&#x1D6A2;";
  math_chars[uchar('z')][math_f_monospace]="&#x1D6A3;";
}

// Converts foo into <mspace width='foo'/>
Xmlp math_ns::mk_space(String a)
{
  Xmlp b = new Xml(cst_mspace,0);
  b->add_att(np_cst_width, Istring(a));
  return b;
}

Xmlp MathDataP::mk_mo(String a)
{
  Xmlp x = new Xml(Istring(a));
  return new Xml(cst_mo, x);
}

// Case of math env. Converts an integer into a string,
// that is put in the name field.
void Math::set_env_name(int xtype)
{
  String env;
  switch(xtype) {
  case eqnarray_code : env = "eqnarray"; break;
  case Beqnarray_code : env = "Beqnarray"; break;
  case multline_code : env = "multline"; break;
  case gather_code : env = "gather"; break;
  case align_code: env = "@align"; break;
  case align_star_code: env = "@align*"; break;
  case split_code: env = "split"; break;
  case aligned_code: env = "aligned"; break;
  case eqnarray_star_code: env = "eqnarray*"; break;
  case Beqnarray_star_code: env = "Beqnarray*"; break;
  case multline_star_code: env = "multline*"; break;
  case gather_star_code: env = "gather*"; break;
  case math_code: env = "math"; break;
  case equation_code: env = "equation"; break;
  case equation_star_code: env = "equation*"; break;
  case displaymath_code: env = "displaymath"; break;
  default: env = "strange";
  }
  name = env;
}

// Ams environments pmatrix, bmatrix, Bmatrix etc.
// Adds the fence adapted to the first character of the env.
// static function
void math_ns::special_fence(char s, int& open, int&close)
{
  switch(s) {
  case 'B' : open=del_open_brace; close=del_close_brace; break;
  case 'b' : open=del_open_bra; close=del_close_bra; break;
  case 'v' : open=del_bar; close=del_bar; break;
  case 'V' : open=del_Vert; close=del_Vert; break;
  default : open=del_open_par; close=del_close_par; break;
  }
}

// returns a delimiter position in the table
// static
del_pos math_ns::get_delimiter(CmdChr X)
{
  if(X.is_other()) {
    if(!X.char_val().is_ascii()) return del_invalid;
    switch(X.char_val().ascii_value()) {
    case '<' : return del_open_ket;
    case '>' : return del_close_ket;
    case '.' : return del_dot;
    case '(' : return del_open_par;
    case ')' : return del_close_par;
    case '[' : return del_open_bra;
    case ']' : return del_close_bra;
    case '|': return del_vert;
    case '/': return del_slash;
    default : return del_invalid;
    }
  }
  if(X.is_math_openclosebetween()) {
    switch(int (X.get_chr())) {
    case rangle_code:  return del_rangle;
    case langle_code:  return del_langle;
    case lbrace_code:  return del_lbrace;
    case rbrace_code:  return del_rbrace;
    case open_brace_code:  return del_lbrace;
    case close_brace_code:  return del_rbrace;
    case lceil_code:  return del_lceil;
    case rceil_code:  return del_rceil;
    case rfloor_code:  return del_rfloor;
    case lfloor_code:  return del_lfloor;
    case Vert_code: return del_Vert;
    case Vertx_code: return del_Vert;
    case vert_code: return del_vert;
    case lmoustache_code: return del_lmoustache;
    case rmoustache_code: return del_rmoustache;
    case uparrow_code: return del_uparrow;
    case downarrow_code: return del_downarrow;
    case updownarrow_code: return del_updownarrow;
    case Uparrow_code: return del_Uparrow;
    case Downarrow_code: return del_Downarrow;
    case Updownarrow_code: return del_Updownarrow;
    case rgroup_code: return del_rgroup;
    case lgroup_code: return del_lgroup;
    case backslash_code: return del_backslash;
    default:;
    }
  }
  if(X.is_cst1_cmd() && X.get_chr()==lbrace_chr) return del_lbrace;
  if(X.is_cst1_cmd() && X.get_chr()==rbrace_chr) return del_rbrace;
  return del_invalid;
}

del_pos math_ns::get_delimiter(int k)
{
  switch(k) {
  case rangle_code:  return del_rangle;
  case langle_code:  return del_langle;
  case lbrace_code:  return del_lbrace;
  case rbrace_code:  return del_rbrace;
  case open_brace_code:  return del_lbrace;
  case close_brace_code:  return del_rbrace;
  case lceil_code:  return del_lceil;
  case rceil_code:  return del_rceil;
  case rfloor_code:  return del_rfloor;
  case lfloor_code:  return del_lfloor;
  case Vertx_code: return del_Vert;
  case Vert_code: return del_Vert;
  case vert_code: return del_vert;
  case lmoustache_code: return del_lmoustache;
  case rmoustache_code: return del_rmoustache;
  case uparrow_code: return del_uparrow;
  case downarrow_code: return del_downarrow;
  case updownarrow_code: return del_updownarrow;
  case Uparrow_code: return del_Uparrow;
  case Downarrow_code: return del_Downarrow;
  case Updownarrow_code: return del_Updownarrow;
  case rgroup_code: return del_rgroup;
  case lgroup_code: return del_lgroup;
  case backslash_code: return del_backslash;
  default:;
  }
  int n = k- math_c_loc;
  if(0<n && n<127) {
    switch(n) {
    case '<' : return del_open_ket;
    case '>' : return del_close_ket;
    case '.' : return del_dot;
    case '(' : return del_open_par;
    case ')' : return del_close_par;
    case '[' : return del_open_bra;
    case ']' : return del_close_bra;
    case '|': return del_vert;
    case '/': return del_slash;
    default : return del_invalid;
    }
  }
  return del_invalid;
}

// This is a static function
// true if the constant is a space.
bool math_ns::math_space_code(int c)
{
  switch(c) {
  case quad_code: return true;
  case qquad_code: return true;
  case space_code: return true;
  case exclam_code: return true;
  case comma_code: return true;
  default: return false;
  }
}

// Returns the value of a constant,
Xmlp math_ns::math_constants(int c)
{
  switch(c) {
  case dots_code: return math_data.get_mc_table(5);
  case ldots_code: return math_data.get_mc_table(6);
  case quad_code: return math_data.get_mc_table(7);
  case qquad_code: return math_data.get_mc_table(8);
  case space_code: return math_data.get_mc_table(9);
  case dollar_code: return  math_data.get_mc_table(10);
  case percent_code: return  math_data.get_mc_table(11);
  case amp_code: return math_data.get_mc_table(12);
  case exclam_code: return math_data.get_mc_table(13);
  case comma_code: return math_data.get_mc_table(14);
  case lbrace_chr: return math_data.get_mc_table(15);
  case rbrace_chr: return math_data.get_mc_table(16);
  case i_code: return math_data.get_mc_table(17);
  case msharp_code: return math_data.get_mc_table(18);
  case natural_code: return math_data.get_mc_table(19);
  case flat_code: return math_data.get_mc_table(20);
  case underscore_code:  return math_data.get_mc_table(21);
  case sharp_code: return math_data.get_mc_table(22);
  case j_code: return math_data.get_mc_table(23);
  case tdagger_code: return math_data.get_mc_table(24);
  case tddagger_code: return math_data.get_mc_table(25);
  }
  return math_data.get_mc_table(0);
}

Token MathDataP::init_builtin(String name,math_loc pos, Xmlp x, symcodes t)
{
  init_builtin(pos,x);
  return the_parser.hash_table.primitive(name,t,subtypes(pos));
}

// This is for a general token. It defines name to be (t,pos)
// It adds defined math-location pos to be <bl>ent</bl>
// If hack is true, the formula $\name$ can be hacked to ent (i.e.
// $\alpha$ translates to &alpha;) If no entity names are desired
// then ent2 is used instead of ent, so that \alpha gives &#x3B1;

Token MathDataP::mk_gen(String name, String ent, String ent2, 
			  math_loc pos,
			  name_positions bl, symcodes t, bool hack)
{
  
  Xmlp x = new Xml(Istring(no_ent_names ? ent2 : ent));
  if(hack) built_in_table_alt[pos] = x;
  x = new Xml(bl,x);
  return init_builtin(name,pos, x, t);
}

// Special case where a bold variant exists
Token MathDataP::mk_gen(String name, String ent, String ent2, 
			math_loc pos, math_loc pos2,
			name_positions bl, symcodes t, bool hack)
{
  
  Xmlp x = new Xml(Istring(no_ent_names ? ent2 : ent));
  if(hack) built_in_table_alt[pos] = x;
  Xmlp bold = new Xml(bl,x);
  bold->add_att(cst_mathvariant,cstf_bold);
  init_builtin(pos2,bold);
  x = new Xml(bl,x);
  return init_builtin(name,pos, x, t);
}

// This defines name to be a math ord command. 
// the value is <mo>ent</mo> , with form=prefix, movable_limits = maybe
// This is currently unused
void MathDataP::mk_moc(String name, String ent, math_loc pos)
{
  Xmlp x = mk_mo(ent);
  x->add_att(np_form,np_prefix);
  init_builtin(name,pos,x,mathord_cmd);
}


// This defines name to be a math ord command. 
// the value is <mo>ent</mo> , with form=prefix, movable_limits = maybe
void MathDataP::mk_moo(String name, String ent, math_loc pos)
{
  Xmlp x = mk_mo(ent);
  x->add_att(np_form,np_prefix);
  symcodes T = mathopn_cmd;
  if(first_w_limit_code<=pos && pos <=last_w_limit_code) {
    T = mathop_cmd;
    x->add_att(np_movablelimits, np_true);
  }
  init_builtin(name,pos,x,T);
}

// This associates to the command name, a <mi> element with value ent/ent2
// with internal code pos. It is an Ordinary object.
// hack true
void MathDataP::mk_ic(String name, String ent, String ent2, math_loc pos)
{
  mk_gen(name,ent,ent2, pos,cst_mi,mathord_cmd,true);
}

// Case where the symbol has a bold variant
void MathDataP::mk_icb(String name, String ent, String ent2, math_loc pos)
{
  math_loc pos2 = math_loc(pos-alpha_code+alpha_bcode);
  mk_gen(name,ent,ent2, pos,pos2,cst_mi,mathord_cmd,true);
}

// This associates to the command named name, a <mo> element with value ent
// with internal code pos. It is an Ordinary object.
// these are hacked
void MathDataP::mk_oc(String name, String ent, String ent2, math_loc pos)
{
  mk_gen(name,ent,ent2,pos,cst_mo,mathord_cmd,true);
}
// Op no limits
void MathDataP::mk_oco(String name, String ent,  String ent2,math_loc pos)
{
  mk_gen(name,ent,ent2,pos,cst_mo,mathopn_cmd,true);
}
// Op display limits
void MathDataP::mk_ocol(String name, String ent,  String ent2,math_loc pos)
{
  mk_gen(name,ent,ent2,pos,cst_mo,mathop_cmd,true);
}
void MathDataP::mk_ocb(String name, String ent,  String ent2,math_loc pos)
{
  mk_gen(name,ent,ent2,pos,cst_mo,mathbin_cmd,true);
}
void MathDataP::mk_ocr(String name, String ent,  String ent2,math_loc pos)
{
  mk_gen(name,ent,ent2,pos,cst_mo,mathrel_cmd,true);
}

// This associates to the command named A, a <mo> element with value B
// with internal code pos. It is of type T.
void MathDataP::mk_oc(String name, String ent, String ent2, math_loc pos, symcodes t,bool hack)
{
  mk_gen(name,ent,ent2,pos,cst_mo,t,hack);
}

// This associates to the command named A, a <mo> element with value B
// with internal code pos. It is something like an accent.
void MathDataP::mk_accent(String name, String ent, String ent2, subtypes pos)
{
  mk_gen(name,ent,ent2, math_loc(pos),cst_mo,special_math_cmd,false);
}

// This is for a character: the single character in A
// is defined to be <mo> with value b.
void MathDataP::TM_mk(String a, String b, math_types c)
{
  int A = uchar(a[0]);
  init_builtin(A+math_c_loc, mk_mo(b));
  math_char_type[A] = c;
}

// For a command like \frac. 
Token mk_cmd(String name, subtypes pos)
{
  return the_parser.hash_table.primitive(name,special_math_cmd,pos);
}

// For a command like \enspace.
void mk_space(String name, int b)
{
  the_parser.hash_table.primitive(name,mathspace_cmd,subtypes(b));
}


void MathDataP::fill_lr(int a, String b, String c)
{
  xml_lr_ptable[a]  = Istring(no_ent_names ? c : b); 
}
inline void MathDataP::fill_lr(int a, String b)
{
  xml_lr_ptable[a]  = Istring(b); 
}

// This assumes that nb_mathchars is 128
void MathDataP::boot_xml_lr_tables()
{
  Buffer&B = the_main->SH.shbuf();
  for(uchar i=0;i<nb_mathchars;i++) {
    int k = i+math_c_loc;
    if(built_in_table[k]) continue;
    B.reset();
    B.push_back(i);
    built_in_table[k] = new Xml(cst_mo, 0);
    built_in_table[k]->push_back(new Xml(the_main->SH));
  }

  // fill_lr(del_Vert, "&Verbar;", "&#x02016;");
  fill_lr(del_Vert, "&parallel;", "&#x02225;");
  fill_lr(del_vert, "&vert;", "&#x7C;");
  fill_lr(del_rangle,"&rangle;", RANGLE);
  fill_lr(del_langle,"&langle;", LANGLE);
  fill_lr(del_close_ket,"&rangle;", RANGLE);
  fill_lr(del_open_ket,"&langle;", LANGLE);
  fill_lr(del_rbrace,"&rbrace;", "&#x0007D;");
  fill_lr(del_lbrace,"&lbrace;", "&#x0007B;");
  fill_lr(del_lceil, "&lceil;", "&#x02308;");
  fill_lr(del_rceil, "&rceil;", "&#x02309;");
  fill_lr(del_lfloor,"&lfloor;", "&#x0230A;");
  fill_lr(del_rfloor, "&rfloor;", "&#x0230B;");
  fill_lr(del_open_par,"(");
  fill_lr(del_close_par,")");
  fill_lr(del_open_brace,"{");
  fill_lr(del_close_brace,"}");
  fill_lr(del_open_bra,"[");
  fill_lr(del_close_bra,"]");
  fill_lr(del_bar,"|");
  //  fill_lr(del_open_ket,"&lt;");
  //  fill_lr(del_close_ket,"&gt;");
  fill_lr(del_dot,"");
  if(compatibility)  fill_lr(del_dot,".");
  fill_lr(del_slash,"/");
  fill_lr(del_backslash,"&#x2216;");
  fill_lr(del_lmoustache,"&lmoustache;", "&#x023B0;");
  fill_lr(del_rmoustache,"&rmoustache;", "&#x023B1;");
  fill_lr(del_arrowvert,"");
  fill_lr(del_Arrowvert,"");
  fill_lr(del_uparrow, "&uparrow;", "&#x02191;");
  fill_lr(del_downarrow,"&downarrow;", "&#x02193;");
  fill_lr(del_updownarrow,"&updownarrow;", "&#x02195;");
  fill_lr(del_Uparrow,"&Uparrow;","&#x021D1;");
  fill_lr(del_Downarrow,"&Downarrow;", "&#x021D3;");
  fill_lr(del_Updownarrow, "&Updownarrow;", "&#x021D5;");
  fill_lr(del_lgroup,"&#x3014;");
  fill_lr(del_rgroup,"&#x3015;");
  //  fill_lr(del_lgroup,"&#x2997;");
  // fill_lr(del_rgroup,"&#x2998;");
  fill_lr(del_bracevert,"");

  mc_table[0] = mk_mo("error unknown control sequence");
  mc_table[1] = mk_mo(no_ent_names ? "&#xAF;" : "&OverBar;");
  mc_table[2] = mk_mo(no_ent_names ? "&#x0FE37;" : "&OverBrace;");
  mc_table[3] = mk_mo(no_ent_names ? "&#x332;" : "&UnderBar;");
  mc_table[4] = mk_mo(no_ent_names ? "&#x0FE38;" : "&UnderBrace;");
  mc_table[5] = mk_mo(no_ent_names ? "&#x022EF;" : "&ctdot;");
  mc_table[6] = mk_mo("...");
  mc_table[7] = mk_space("1.em");
  mc_table[8] = mk_space("2.em");
  mc_table[9] = mk_space("4pt");
  mc_table[10] = new Xml(cst_mi, new Xml(Istring("$")));
  mc_table[11] = mk_mo("%");
  mc_table[12] = mk_mo("&amp;");
  mc_table[13] = mk_space("-0.166667em");
  mc_table[14] = mk_space("0.166667em");
  mc_table[15] = mk_mo("{");
  mc_table[16] = mk_mo("}");
  mc_table[17] = mk_mo(no_ent_names ? "&#x131;" : "&inodot;");
  mc_table[18] = mk_mo(no_ent_names ? "&#x0266F;" : "&sharp;");
  mc_table[19] = mk_mo(no_ent_names ? "&#x0266E;" : "&natur;");
  mc_table[20] = mk_mo(no_ent_names ? "&#x0266D;" : "&flat;");
  mc_table[21] = mk_mo( "_");
  mc_table[22] = mk_mo( "#");
  mc_table[23] = math_ns::mk_mi('j');
  mc_table[24] = mk_mo(no_ent_names ? "&#x02020;" : "&dagger;");
  mc_table[25] = mk_mo(no_ent_names ? "&#x02021;" : "&ddagger;");
  mc_table[26] = mk_mo(no_ent_names ? "&#x2DC;" : "&tilde;");
}



Xmlp math_ns::make_math_char(uchar c, int n)
{
  Buffer& B = the_main->SH.shbuf();
  B.reset();
  if(n<=1) B.push_back(c);
  else  B.push_back (math_chars[c][n]);
  Xmlp v = new Xml(the_main->SH);
  Xmlp res = new Xml(cst_mi, v);
  if(n==1) res->add_att(cst_mathvariant,cstf_normal);
  return res;
}

void MathDataP::boot_chars()
{
  for (int i = 0; i<=9;i++) {
    Istring K = the_names[cst_dig0+i];
    init_builtin(i+math_dig_loc, new Xml(cst_mn,new Xml(K)));
  }
  Buffer& B = the_main->SH.shbuf();
  
  for (uchar i = 'A'; i<='Z';i++)
    init_builtin(i+math_char_normal_loc, make_math_char(i,0));
  for (uchar i = 'a'; i<='z';i++) 
    init_builtin(i+math_char_normal_loc, make_math_char(i,0));
  for (uchar i = 'A'; i<='Z';i++)
    init_builtin(i+math_char_upright_loc, make_math_char(i,1));
  for (uchar i = 'a'; i<='z';i++) 
    init_builtin(i+math_char_upright_loc, make_math_char(i,1));
  
  init_builtin(xml_e_loc, new Xml(the_names[np_letter_e]));
  init_builtin(xml_o_loc, new Xml(the_names[np_letter_o]));
  // nb_simplemath should be 128
  for (uchar i = 0; i<nb_simplemath;i++) {
    B.reset();
    B.push_back(i);
    Xmlp res = new Xml(np_simplemath,new Xml(the_main->SH));
    Xmlp X = new Xml(np_formula,res);
    X->add_att(np_type,np_inline);
    simplemath_table[i] = X;
  }
}


// Creates complicated objects 
void MathDataP::boot2()
{
  // Define \colon 
  Xmlp colon = mk_mo(":");
  colon->add_att(Istring("lspace"),Istring("0pt"));
  init_builtin("colon",colon_code,colon,mathord_cmd);
  // Constructs varlim etc

  Xmlp lim_op= mk_mo("lim");
  lim_op->add_att(np_movablelimits, np_false);
  Xmlp x = xml2sons(Istring(cst_mover), lim_op,get_mc_table(1));
  init_builtin("varlimsup",varlimsup_code,x,mathop_cmd);

  x = xml2sons(Istring(cst_munder), lim_op,get_mc_table(3));
  init_builtin("varliminf",varliminf_code,x,mathop_cmd);

  x = xml2sons(Istring(cst_munder), lim_op,get_builtin(underrightarrow_code));
  x->add_att(cst_accentunder,np_true);
  init_builtin("varinjlim",varinjlim_code,x,mathop_cmd);

  x = xml2sons(Istring(cst_munder), lim_op,get_builtin(underleftarrow_code));
  x->add_att(cst_accentunder,np_true);
  init_builtin("varprojlim",varprojlim_code,x,mathop_cmd);

  x = mk_mo("(");
  x = new Xml(Istring(cst_mpadded),x);
  x->add_att(np_cst_width,np_zerodim);
  x=  new Xml(Istring(cst_mphantom),x);
  init_builtin("strut",strut_code,x,mathord_cmd);
  init_builtin("mathstrut",strut_code,x,mathord_cmd);

  Xmlp y = new Xml(Istring(cst_mpadded),get_builtin(int_code));
  y->add_att(np_cst_width,Istring("-3pt"));
  Xmlp z = get_builtin(xml_thickmu_space_loc);
  x = new Xml(cst_mrow, 0);
  x->push_back(z);
  x->push_back(y);
  x->push_back(y);
  x->push_back(z);
  init_builtin("iint",iint_code,x,mathop_cmd);
  x =  new Xml(cst_mrow, 0);
  x->push_back(z);
  x->push_back(y);
  x->push_back(y);
  x->push_back(y);
  x->push_back(z);
  init_builtin("iiint",iiint_code,x,mathop_cmd);
  x =  new Xml(cst_mrow, 0);
  x->push_back(z);
  x->push_back(y);
  x->push_back(y);
  x->push_back(y);
  x->push_back(y);
  x->push_back(z);
  init_builtin("iiiint",iiiint_code,x,mathop_cmd);
  x =  new Xml(cst_mrow, 0);
  x->push_back(get_builtin(int_code));
  x->push_back(math_data.get_mc_table(6));
  x->push_back(get_builtin(int_code));
  init_builtin("idotsint",idotsint_code,x,mathop_cmd);
  x =  new Xml(Istring("none"), 0);
  the_parser.hash_table.mmlnone_token 
    = init_builtin("mmlnone",mml_none_code,x,mathord_cmd);
  x =  new Xml(Istring("mprescripts"), 0);
  the_parser.hash_table.mmlprescripts_token 
    = init_builtin("mmlprescripts",mml_prescripts_code,x,mathord_cmd);
}


void MathDataP::boot()
{
  no_ent_names = the_main->use_noent_names();
  for(int i=0;i<last_math_loc;i++) built_in_table[i] = 0;
  for(int i=0;i<last_math_loc;i++) built_in_table_alt[i] = 0;
  boot_table();
  fill_single_char();
  fill_math_char_slots();
  if(!no_ent_names) fill_math_char_slots_ent();
  boot_chars();

  mk_icb("alpha", "&alpha;", "&#x3B1;", alpha_code);
  mk_icb("beta", "&beta;", "&#x3B2;", beta_code);
  mk_icb("gamma", "&gamma;", "&#x3B3;", gamma_code);
  mk_icb("delta", "&delta;", "&#x3B4;", delta_code);
  mk_icb("epsilon", "&straightepsilon;", "&#x3F5;", epsilon_code);
  mk_icb("varepsilon", "&varepsilon;", "&#x3B5;", varepsilon_code);
  mk_icb("zeta", "&zeta;", "&#x3B6;", zeta_code);
  mk_icb("eta", "&eta;", "&#x3B7;", eta_code);
  mk_icb("theta","&theta;", "&#x3B8;", theta_code);
  mk_icb("iota","&iota;", "&#x3B9;", iota_code);
  mk_icb("kappa","&kappa;","&#x3BA;", kappa_code);
  mk_icb("lambda", "&lambda;","&#x3BB;", lambda_code);
  mk_icb("mu","&mu;", "&#x3BC;", mu_code);
  mk_icb("nu","&nu;", "&#x3BD;", nu_code);
  mk_icb("xi", "&xi;","&#x3BE;", xi_code);
  mk_icb("pi", "&pi;","&#x3C0;", pi_code);
  mk_icb("rho", "&rho;", "&#x3C1;", rho_code);
  mk_icb("sigma", "&sigma;", "&#x3C3;", sigma_code);
  mk_icb("tau", "&tau;","&#x3C4;", tau_code);
  mk_icb("upsilon", "&upsi;", "&#x3C5;", upsilon_code);
  if(the_main -> use_old_phi()) {
   mk_icb("varphi", "&phi;", "&#x3D5;",  varphi_code);
   mk_icb("phi", "&phiv;", "&#x3C6;", phi_code);
  } else {
    mk_icb("phi", "&phiv;", "&#x3D5;", phi_code);
    mk_icb("varphi", "&phi;", "&#x3C6;",  varphi_code);
  }
  mk_icb("chi", "&chi;", "&#x3C7;", chi_code);
  mk_icb("psi", "&psi;", "&#x3C8;", psi_code);
  mk_icb("omega", "&omega;", "&#x3C9;", omega_code);
  mk_icb("digamma", "&digamma;", "&#x3DD;", digamma_code);
  mk_icb("varpi", "&piv;", "&#x3D6;", varpi_code);
  mk_icb("varrho", "&rhov;", "&#x3F1;", varrho_code);
  mk_icb("varsigma", "&sigmav;", "&#x3C2;", varsigma_code);
  mk_icb("vartheta", "&thetav;", "&#x3D1;", vartheta_code);
  mk_icb("varkappa", "&kappav;", "&#x3F0;", varkappa_code);
  mk_icb("Gamma", "&Gamma;", "&#x393;", Gamma_code);
  mk_icb("Delta", "&Delta;", "&#x394;", Delta_code);
  mk_icb("Theta", "&Theta;", "&#x398;", Theta_code);
  mk_icb("Lambda", "&Lambda;", "&#x39B;", Lambda_code);
  mk_icb("Xi", "&Xi;", "&#x39E;", Xi_code);
  mk_icb("Sigma", "&Sigma;", "&#x3A3;", Sigma_code);
  mk_icb("Upsilon", "&Upsi;", "&#x3A5;", Upsilon_code);
  mk_icb("Phi", "&Phi;", "&#x3A6;", Phi_code);
  mk_icb("Pi", "&Pi;", "&#x3A0;", Pi_code);
  mk_icb("Psi", "&Psi;", "&#x3A8;", Psi_code);
  mk_icb("Omega", "&Omega;", "&#x3A9;", Omega_code);
  mk_ic("hbar","&#x127;","&#x127;", hbar_code);
  mk_ic("hslash","&hbar;","&#x0210F;", hbar_code);
  mk_ic("ell","&ell;","&#x02113;", ell_code);          // pb
  mk_ic("wp","&wp;","&#x02118;", wp_code);
  mk_ic("Re","&Re;","&#x0211C;", Re_code);
  mk_ic("Im","&Im;","&#x02111;", Im_code);

  // Other mi alphabetic order
  mk_ic("aleph","&aleph;","&#x02135;", aleph_code);
  mk_ic("angle","&ang;","&#x02220;", angle_code);
  mk_ic("backprime","&backprime;","&#x2035;", backprime_code);
  mk_ic("beth","&beth;","&#x02136;", beth_code);
  mk_ic("bigstar","&bigstar;","&#x2605;", bigstar_code);
  mk_ic("blacklozenge","&blacklozenge;","&#x29EB;", blacklozenge_code);
  mk_ic("blacksquare","&blacksquare;","&#x25AA;", blacksquare_code);
  mk_ic("blacktriangledown","&blacktriangledown;","&#x25BE;", blacktriangledown_code);
  mk_ic("blacktriangle","&blacktriangle;","&#x25B4;", blacktriangle_code);
  mk_ic("bot","&bottom;","&#x022A5;", bot_code);
  mk_ic("bottom","&bottom;","&#x022A5;",bottom_code);    // eval_let ?????
  mk_ic("circledS","&circledS;","&#x24C8;",circledS_code);
  mk_ic("complement","&complement;","&#x02201;",complement_code);
  mk_ic("daleth","&daleth;","&#x2138;",daleth_code);
  mk_ic("diagdown","&setminus;","&#x2216;",diagdown_code);
  mk_ic("diagup","&#x2215;","&#x2215;",diagup_code);
  mk_ic("emptyset","&emptyset;","&#x2205;", emptyset_code);
  mk_ic("eth","&eth;","&#xF0;",eth_code);
  mk_ic("ExponentialE","&ExponentialE;","&#x2147;",exponentiale_code);
  mk_ic("Finv","&#x2132;","&#x2132;",Finv_code);
  mk_ic("Game","&#x2141;","&#x2141;",Game_code); 
  mk_ic("gimel","&gimel;","&#x2137;",gimel_code);
  mk_ic("ImaginaryI","&ImaginaryI;","&#x2148;",imaginaryi_code);
  mk_ic("infty","&infin;","&#x0221E;", infty_code);
  mk_ic("lightning","&#x2607;","&#x2607;", lightning_code);
  mk_ic("lozenge","&lozenge;","&#x25CA;", lozenge_code);
  mk_ic("nabla","&nabla;","&#x02207;", nabla_code);
  mk_ic("measuredangle","&measuredangle;","&#x2221;", measuredangle_code);
  mk_ic("mathparagraph","&para;","&#xB6;", mathparagraph_code);
  mk_ic("mathsection","&sect;","&#xA7;", mathsection_code);
  mk_ic("partial","&part;","&#x02202;", partial_code);
  mk_ic("nexists","&nexists;","&#x02204;", nexists_code);
  mk_ic("surd","&radic;","&#x0221A;", surd_code);
  mk_ic("sphericalangle","&angsph;","&#x2222;", sphericalangle_code);
  mk_ic("top","&top;","&#x022A4;", top_code);
  mk_ic("triangle","&triangle;","&#x025B5;",triangle_code);
  mk_ic("varnothing","&#x2300;","&#x2300;",varnothing_code);


  // mathop rm.. alphabetic order
  mk_moo("arccos","arccos",arccos_code);
  mk_moo("arcsin","arcsin",arcsin_code);
  mk_moo("arctan","arctan",arctan_code);
  mk_moo("arg","arg",arg_code);
  mk_moo("cos","cos",cos_code);
  mk_moo("cosh","cosh",cosh_code);
  mk_moo("cot","cot",cot_code);
  mk_moo("coth","coth",coth_code);
  mk_moo("csc","csc",csc_code);
  mk_moo("deg","deg",deg_code);
  mk_moo("det","det",det_code);
  mk_moo("dim","dim",dim_code);
  mk_moo("exp","exp",exp_code);
  mk_moo("gcd","gcd",gcd_code);
  mk_moo("hom","hom",hom_code);
  mk_moo("inf","inf",inf_code);
  mk_moo("injlim","inj lim",injlim_code);
  mk_moo("ker","ker",ker_code);
  mk_moo("lg","lg",lg_code);
  mk_moo("lim","lim",lim_code);
  mk_moo("liminf","lim inf",liminf_code);
  mk_moo("limsup","lim sup",limsup_code);
  mk_moo("ln","ln",ln_code);
  mk_moo("log","log",log_code);
  mk_moo("max","max",max_code);
  mk_moo("min","min",min_code);
  mk_moo("Pr","Pr",Pr_code);
  mk_moo("projlim","proj lim",projlim_code);
  mk_moo("sec","sec",sec_code);
  mk_moo("sin","sin",sin_code);
  mk_moo("@mod","mod",mod_code);
  mk_moo("sinh","sinh",sinh_code);
  mk_moo("sup","sup",sup_code);
  mk_moo("tan","tan",tan_code);
  mk_moo("tanh","tanh",tanh_code);

  mk_oc("forall","&forall;","&#x02200;", forall_code);
  mk_oc("exists","&exist;","&#x02203;", exists_code);

  // type large
  mk_ocol("coprod","&coprod;","&#x02210;", coprod_code);
  mk_ocol("bigvee","&bigvee;","&#x022C1;",bigvee_code);              // pb
  mk_ocol("bigwedge","&bigwedge;","&#x022C0;",bigwedge_code);          // pb
  mk_ocol("biguplus","&uplus;","&#x0228E;",biguplus_code);
  mk_ocol("bigcap","&bigcap;","&#x022C2;",bigcap_code);
  mk_ocol("bigcup","&bigcup;","&#x022C3;",bigcup_code);
  mk_oco("int","&int;","&#x0222B;",int_code);
  mk_oco("xiint","&#x0222C;","&#x0222C;",xiint_code);
  mk_oco("xiiint","&iiint;","&#x0222D;",xiiint_code);
  mk_oco("xiiiint","&iiiint;","&#x02A0C;",xiiiint_code);
  mk_ocol("sum","&sum;","&#x02211;",sum_code);
  mk_ocol("prod","&prod;","&#x0220F;",prod_code);
  mk_ocol("bigotimes","&bigotimes;","&#x02A02;",bigotimes_code);
  mk_ocol("bigoplus","&bigoplus;","&#x02A01;", bigoplus_code);
  mk_ocol("bigodot","&bigodot;","&#x02A00;",bigodot_code);
  mk_oco("oint","&oint;","&#x0222E;",oint_code);
  mk_ocol("bigsqcup","&bigsqcup;","&#x02A06;",bigsqcup_code);
  mk_oco("smallint","&int;","&#x0222B;",smallint_code);    // pas de small...

  // type bin
  mk_ocb("triangleq","&triangleq;","&#x0225C;",triangleq_code);
  mk_ocb("triangleleft","&triangleleft;","&#x025C3;",triangleleft_code);
  mk_ocb("triangleright","&triangleright;","&#x025B9;",triangleright_code);
  mk_ocb("bigtriangleup","&bigtriangleup;","&#x025B3;",bigtriangleup_code);
  mk_ocb("bigtriangledown","&bigtriangledown;","&#x025BD;",bigtriangledown_code);
  mk_ocb("wedge","&wedge;","&#x02227;",wedge_code);
  mk_ocb("land","&wedge;","&#x02227;",wedge_code);
  mk_ocb("vee","&vee;","&#x02228;",vee_code);
  mk_ocb("lor","&vee;","&#x02228;",vee_code);
  mk_ocb("cap","&cap;","&#x02229;",cap_code);
  mk_ocb("cup","&cup;","&#x0222A;", cup_code);
  mk_ocb("Cap","&Cap;","&#x022D2;",Cap_code);
  mk_ocb("Cup","&Cup;","&#x022D3;", Cup_code);
  mk_ocb("multimap","&multimap;","&#x022B8;", multimap_code);
  mk_ocb("dagger","&dagger;","&#x02020;", dagger_code);
  mk_ocb("ddagger","&ddagger;","&#x02021;", ddagger_code);
  mk_ocb("sqcap","&sqcap;","&#x02293;", sqcap_code);
  mk_ocb("sqcup","&sqcup;","&#x02294;", sqcup_code);
  mk_ocb("amalg","&amalg;","&#x02A3F;",almalg_code);
  mk_ocb("diamond","&diamond;","&#x022C4;",diamand_code);
  mk_ocb("Diamond","&diamond;","&#x022C4;",Diamond_code);
  mk_ocb("bullet","&bullet;","&#x02022;",bullet_code);
  mk_ocb("wr","&wr;","&#x02240;", wr_code);
  mk_ocb("div","&div;","&#xF7;", div_code);
  mk_ocb("odot","&odot;","&#x02299;", odot_code);
  mk_ocb("oslash","&osol;","&#x02298;",oslash_code);
  mk_ocb("otimes","&otimes;","&#x02297;",otimes_code);
  mk_ocb("ominus","&ominus;","&#x02296;" ,ominus_code);
  mk_ocb("oplus","&oplus;","&#x02295;",oplus_code);
  mk_ocb("uplus","&uplus;","&#x0228E;",uplus_code);
  mk_ocb("mp","&mp;","&#x02213;",mp_code);
  mk_ocb("pm","&pm;","&#xB1;",pm_code);
  mk_ocb("circ","&SmallCircle;","&#x02218;",circ_code);          // pb
  mk_ocb("bigcirc","&bigcirc;","&#x025EF;",bigcirc_code);    // pb
  mk_ocb("setminus","&setminus;","&#x02216;",setminus_code);
  mk_ocb("cdot","&middot;", "&#xB7;",cdot_code);          // pb
  mk_ocb("ast","&ast;","&#x2A;", ast_code);
  mk_ocb("times","&times;","&#xD7;",times_code);
  mk_ocb("star","&star;","&#x02606;" ,star_code);
  mk_ocb("*","&InvisibleTimes;","&#x02062;" ,starchar_code);
  mk_ocb("InvisibleTimes","&InvisibleTimes;","&#x2062;" ,starchar_code);
  mk_ocb("ApplyFunction","&ApplyFunction;","&#x2061;" ,applyfct_code);
  mk_ocb("InvisibleComma","&InvisibleComma;","&#x2063;" ,invisiblecomma_code);
  mk_ocb("baro","&ovbar;","&#x233D;", baro_code);
  mk_ocb("barwedge","&#x22BC;","&#x22BC;", barwedge_code);
  mk_ocb("curlyvee","&curlyvee;","&#x22CE;", curlyvee_code);
  mk_ocb("curlywedge","&curlywedge;","&#x22CF;", curlywedge_code);
  mk_ocb("divideontimes","&divideontimes;","&#x22C7;", divideontimes_code);
  mk_ocb("dotplus","&dotplus;","&#x2214;", dotplus_code);
  mk_ocb("gtrdot","&gtrdot;","&#x22D7;",gtrdot_code);
  mk_ocb("intercal","&intercal;","&#x22BA;", intercal_code);
  mk_ocb("interleave","&#x2AF4;","&#x2AF4;", interleave_code);
  mk_ocb("leftslice","&#x2AA6;","&#x2AA6;", leftslice_code);
  mk_ocb("rightslice","&#x2AA7;","&#x2AA7;", rightslice_code);
  mk_ocb("leftthreetimes","&leftthreetimes;","&#x22CB;", leftthreetimes_code);
  mk_ocb("rightthreetimes","&rightthreetimes;","&#x22CC;", rightthreetimes_code);
  mk_ocb("lessdot","&lessdot;","&#x22D6;", lessdot_code);
  mk_ocb("ltimes","&ltimes;","&#x22C9;", ltimes_code);
  mk_ocb("rtimes","&rtimes;","&#x22CA;", rtimes_code);
  mk_ocb("merge","&#x2A07;","&#x2A07;", merge_code);
  mk_ocb("sslash","&#x2AFD;","&#x2AFD;", sslash_code);
  mk_ocb("talloblong","&#x2AFF;","&#x2AFF;", talloblong_code);
  mk_ocb("boxast","&#x29C6;","&#x29C6;", boxast_code);
  mk_ocb("boxbox","&#x29C8;","&#x29C8;", boxbox_code);
  mk_ocb("boxbslash","&bsolb;","&#x29C5;", boxbslash_code);
  mk_ocb("boxcircle","&#x29C7;","&#x29C7;", boxcircle_code);
  mk_ocb("boxdot","&dotsquare;","&#x22A1;", boxdot_code);
  mk_ocb("boxminus","&boxminus;","&#x229F;", boxminus_code);
  mk_ocb("boxplus","&boxplus;","&#x229E;", boxplus_code);
  mk_ocb("boxslash","&solb;","&#x29C4;", boxslash_code);
  mk_ocb("boxtimes","&boxtimes;","&#x22A0;", boxtimes_code);
  mk_ocb("centerdot","&sdot;","&#x22C5;", centerdot_code);
  mk_ocb("circledast","&circledast;","&#x229B;", circledast_code);
  mk_ocb("circledcirc","&circledcirc;","&#x229A;", circledcirc_code);
  mk_ocb("circleddash","&circleddash;","&#x229D;", circleddash_code);
  mk_ocb("obar","&omid;","&#x29B6;", obar_code);
  mk_ocb("ogreaterthan","&ogt;","&#x29C1;", ogreaterthan_code);
  mk_ocb("olessthan","&olt;","&#x29C0;",olessthan_code);
  mk_ocb("varocircle","&#x29BE;","&#x29BE;", varocircle_code);


  // Rel
  mk_ocr("propto","&propto;","&#x0221D;",propto_code);
  mk_ocr("sqsubseteq","&sqsubseteq;","&#x02291;" , sqsubseteq_code);
  mk_ocr("sqsupseteq","&sqsupseteq;","&#x02292;",sqsupseteq_code);
  mk_ocr("sqsubset","&sqsubset;","&#x0228F;", sqsubset_code);
  mk_ocr("sqsupset","&sqsupset;","&#x02290;",sqsupset_code);
  mk_ocr("parallel","&parallel;","&#x02225;", parallel_code);
  mk_ocr("mid","&mid;","&#x02223;",mid_code);
  mk_ocr("dashv","&dashv;","&#x022A3;", dashv_code);
  mk_ocr("vdash","&vdash;","&#x022A2;",vdash_code);
  mk_ocr("Vdash","&Vdash;","&#x022A9;", Vdash_code);
  mk_ocr("vDash","&vDash;","&#x22A7;", vDash_code);
  mk_ocr("VDash","&VDash;","&#x22AB;", VDash_code);
  mk_ocr("Vvdash","&Vvdash;","&#x22AA;", Vvdash_code);
  mk_ocr("models","&models;","&#x022A7;",models_code);
  mk_ocr("nearrow","&nearrow;", "&#x02197;", nearrow_code);
  mk_ocr("searrow","&searrow;", "&#x02198;", searrow_code);
  mk_ocr("nwarrow","&nwarrow;", "&#x02196;", nwarrow_code);
  mk_ocr("swarrow","&swarrow;", "&#x02199;", swarrow_code);
  mk_ocr("Leftrightarrow","&Leftrightarrow;","&#x021D4;",Leftrightarrow_code);
  mk_ocr("Leftarrow","&Leftarrow;","&#x021D0;", Leftarrow_code);
  mk_ocr("Rightarrow","&Rightarrow;", "&#x021D2;",Rightarrow_code);
  mk_ocr("implies","&Rightarrow;", "&#x021D2;",Rightarrow_code);
  mk_ocr("ne","&ne;","&#x02260;",ne_code);
  mk_ocr("neq","&ne;","&#x02260;",ne_code);
  mk_ocr("le","&le;","&#x02264;",le_code);
  mk_ocr("leq","&le;","&#x02264;",le_code);
  mk_ocr("ge","&ge;","&#x2265;",ge_code);
  mk_ocr("geq","&ge;","&#x2265;",ge_code);
  mk_ocr("succ","&succ;","&#x0227B;",succ_code);
  mk_ocr("approx","&approx;","&#x02248;",approx_code);
  mk_ocr("succeq","&succeq;","&#x02AB0;",succeq_code);
  mk_ocr("preceq","&preceq;","&#x02AAF;" ,preceq_code);
  mk_ocr("prec","&prec;","&#x0227A;",prec_code);
  mk_ocr("doteq","&doteq;","&#x02250;",doteq_code);
  mk_ocr("supset","&supset;","&#x02283;", supset_code);
  mk_ocr("subset","&subset;","&#x02282;",subset_code);
  mk_ocr("supseteq","&supseteq;","&#x02287;",supseteq_code);
  mk_ocr("subseteq","&subseteq;","&#x02286;", subseteq_code);
  mk_ocr("bindnasrepma","&#x214B;","&#x214B;",bindnasrepma_code);
  mk_ocr("ni","&ni;","&#x0220B;" , ni_code);
  mk_ocr("owns","&ni;","&#x0220B;" ,ni_code);
  mk_ocr("gg","&gg;","&#x0226B;", gg_code);
  mk_ocr("ll","&ll;","&#x0226A;", ll_code);
  mk_ocr("gtrless","&gl;","&#x02277;", gtrless_code);
  mk_ocr("geqslant","&geqslant;","&#x02A7E;", geqslant_code);
  mk_ocr("leqslant","&leqslant;","&#x02A7D;", leqslant_code);
  mk_ocr("not","&not;","&#xAC;", not_code);        // pb
  mk_ocr("notin","&notin;","&#x02209;", notin_code);
  mk_ocr("leftrightarrow","&leftrightarrow;","&#x02194;" , leftrightarrow_code);
  mk_ocr("leftarrow","&leftarrow;","&#x02190;" , leftarrow_code);
  mk_ocr("gets","&leftarrow;","&#x02190;" , leftarrow_code);
  mk_ocr("rightarrow","&rightarrow;","&#x02192;", rightarrow_code);
  mk_ocr("to","&rightarrow;","&#x02192;",rightarrow_code);
  mk_ocr("mapsto","&mapsto;","&#x021A6;", mapsto_code);
  mk_ocr("sim","&sim;","&#x0223C;", sim_code);
  mk_ocr("simeq","&simeq;","&#x02243;", simeq_code);
  mk_ocr("perp","&perp;","&#x022A5;", perp_code);
  mk_ocr("equiv","&equiv;","&#x02261;", equiv_code);
  //  mk_ocr("asymp","&asymp;","&#x02248;",asymp_code);
  mk_ocr("asymp","&asympeq;","&#x0224D;",asymp_code);
  mk_ocr("smile","&smile;","&#x02323;", smile_code);
  mk_ocr("frown","&frown;","&#x02322;", frown_code);
  mk_ocr("leftharpoonup","&leftharpoonup;","&#x021BC;",leftharpoonup_code);
  mk_ocr("leftharpoondown","&leftharpoondown;","&#x021BD;",leftharpoondown_code);
  mk_ocr("rightharpoonup","&rightharpoonup;","&#x021C0;",rightharpoonup_code);
  mk_ocr("rightharpoondown","&rightharpoondown;","&#x021C1;", rightharpoondown_code);
  mk_ocr("hookrightarrow","&hookrightarrow;","&#x021AA;", hookrightarrow_code);
  mk_ocr("hookleftarrow","&hookleftarrow;",  "&#x021A9;", hookleftarrow_code);
  mk_ocr("bowtie","&bowtie;","&#x022C8;",bowtie_code);
  eval_let("Join","bowtie");
  mk_ocr("Longrightarrow","&Longrightarrow;","&#x027F9;",Longrightarrow_code);
  mk_ocr("longrightarrow","&longrightarrow;","&#x027F6;",longrightarrow_code);
  mk_ocr("longleftarrow","&longleftarrow;","&#x027F5;",longleftarrow_code);
  mk_ocr("Longleftarrow","&Longleftarrow;","&#x027F8;",Longleftarrow_code);
  //mk_ocr("longmapsto","&longmapsto;","&#x027FC;", longmapsto_code);
  // variant 
  // mk_ocr("longmapsto","&longmapsto;","&#x21a4;", longmapsto_code);
  mk_ocr("longleftrightarrow","&longleftrightarrow;","&#x027F7;", longleftrightarrow_code);
  mk_ocr("Longleftrightarrow","&Longleftrightarrow;","&#x027FA;",Longleftrightarrow_code);
  //  mk_ocr("iff","&iff;");   # merde
  // mk_ocr("iff","&Longleftrightarrow;","&#x027FA;", iff_code);
  mk_ocr("iff","&Leftrightarrow;","&#x021D4;", iff_code);
  mk_ocr("backepsilon","&backepsilon;","&#x3F6;", backepsilon_code);
  mk_ocr("because","&because;","&#x2235;", because_code);
  mk_ocr("between","&between;","&#x226C;", between_code);
  mk_ocr("nmid","&NotVerticalBar;","&#x2224;", nmid_code);
  mk_ocr("nparallel","&nparallel;","&#x2226;", nparallel_code);
  mk_ocr("nVDash","&nVDash;","&#x22AF;", nVDash_code);
  mk_ocr("nvDash","&nvDash;","&#x22AD;", nvDash_code);
  mk_ocr("nVdash","&nVdash;","&#x22AE;", nVdash_code);
  mk_ocr("nvdash","&nvdash;","&#x22AC;", nvdash_code);
  mk_ocr("pitchfork","&pitchfork;","&#x22D4;", pitchfork_code);
  mk_ocr("therefore","&therefore;","&#x2234;", therefore_code);
  mk_ocr("nLeftarrow","&nLeftarrow;","&#x21CD;", nLeftarrow_code);
  mk_ocr("nLeftrightarrow","&nLeftrightarrow;","&#x21CE;", nLeftrightarrow_code);
  mk_ocr("nRightarrow","&nRightarrow;","&#x21CF;", nRightarrow_code);
  mk_ocr("nleftarrow","&nleftarrow;","&#x219A;", nleftarrow_code);
  mk_ocr("nleftrightarrow","&nleftrightarrow;","&#x21AE;", nleftrightarrow_code);
  mk_ocr("nrightarrow","&nrightarrow;","&#x219B;", nrightarrow_code);
  mk_ocr("approxeq","&approxeq;","&#x224A;", approxeq_code);
  mk_ocr("Subset","&Subset;", "&#x22D0;", Subset_code);
  mk_ocr("Supset","&Supset;", "&#x22D1;",Supset_code);
  mk_ocr("blacktriangleleft","&blacktriangleleft;", "&#x25C2;",blacktriangleleft_code);
  mk_ocr("blacktriangleright","&blacktriangleright;", "&#x25B8;",blacktriangleright_code);
  mk_ocr("triangledown","&triangledown;", "&#x25BF;",triangledown_code);
  mk_ocr("circlearrowleft","&#x2940;", "&#x2940;",circlearrowleft_code);
  mk_ocr("circlearrowright","&#x2941;", "&#x2941;",circlearrowright_code);
  mk_ocr("curvearrowleft","&#x293A;", "&#x293A;",curvearrowleft_code);
  mk_ocr("curvearrowright","&#x293B;", "&#x293b;",curvearrowright_code);
  mk_ocr("dasharrow","&rBarr;", "&#x290F;",dasharrow_code);
  mk_ocr("dashleftarrow","&lBarr;", "&#x290E;",dashleftarrow_code);
  mk_ocr("dashrightarrow","&rBarr;", "&#x290F;",dashrightarrow_code);
  mk_ocr("fatsemi","&#x2A1F;", "&#x2A1F;",fatsemi_code);
  mk_ocr("downdownarrows","&downdownarrows;", "&#x21CA;",downdownarrows_code);
  mk_ocr("downharpoonright","&downharpoonright;", "&#x21C2;",downharpoonright_code);
  mk_ocr("downharpoonleft","&downharpoonleft;", "&#x21C3;",downharpoonleft_code);
  mk_ocr("leftarrowtail","&leftarrowtail;", "&#x21A2;",leftarrowtail_code);
  mk_ocr("rightarrowtail","&rightarrowtail;", "&#x21A3;",rightarrowtail_code);
  mk_ocr("leftarrowtriangle","&loarr;", "&#x21FD;",leftarrowtriangle_code);
  mk_ocr("rightarrowtriangle","&roarr;", "&#x21FE;",rightarrowtriangle_code);
mk_ocr("leftrightarrowtriangle","&hoarr;", "&#x21FF;",leftrightarrowtriangle_code);
 mk_ocr("leftleftarrows","&leftleftarrows;", "&#x21C7;",leftleftarrows_code);
 mk_ocr("upuparrows","&upuparrows;", "&#x21C8;",upuparrows_code);
 mk_ocr("rightrightarrows","&rightrightarrows;", "&#x21C9;",rightrightarrows_code);
 mk_ocr("leftrightarrows","&leftrightarrows;", "&#x21C6;",leftrightarrows_code);
 mk_ocr("leftrightharpoons","&leftrightharpoons;", "&#x21CB;",leftrightharpoons_code);
 mk_ocr("leftrightsquigarrow","&leftrightsquigarrow;", "&#x21AD;",leftrightsquigarrow_code);
 mk_ocr("Lleftarrow","&Lleftarrow;", "&#x21DA;",Lleftarrow_code);
 mk_ocr("Rrightarrow","&Rrightarrow;", "&#x21DB;",Rrightarrow_code);
 mk_ocr("Longmapsfrom","&#x27FD;", "&#x27FD;",Longmapsfrom_code);
 mk_ocr("longmapsfrom","&#x27FB;", "&#x27FB;",longmapsfrom_code);
 mk_ocr("Mapsfrom","&#x2906;", "&#x2906;",Mapsfrom_code);
 mk_ocr("Longmapsto","&#x27FE;", "&#x27FE;",Longmapsto_code);
 mk_ocr("Mapsto","&#x2907;", "&#x2907;",Mapsto_code);
 mk_ocr("mapsfrom","&mapstoleft;", "&#x21A4;",mapsfrom_code);
 mk_ocr("longmapsto","&longmapsto;", "&#x27FC;",longmapsto_code);
 mk_ocr("looparrowleft","&looparrowleft;", "&#x21AB;",looparrowleft_code);
 mk_ocr("looparrowright","&looparrowright;", "&#x21AC;",looparrowright_code);
 mk_ocr("Lsh","&Lsh;", "&#x21B0;",Lsh_code);
 mk_ocr("Rsh","&Rsh;", "&#x21B1;",Rsh_code);
 mk_ocr("rightleftarrows","&rightleftarrows;", "&#x21C4;",rightleftarrows_code);
 mk_ocr("rightleftharpoons","&rightleftharpoons;", "&#x21CC;",rightleftharpoons_code);
 mk_ocr("rightsquigarrow","&rightsquigarrow;", "&#x21DD;",rightsquigarrow_code);
 mk_ocr("twoheadleftarrow","&twoheadleftarrow;", "&#x219E;",twoheadleftarrow_code);
 mk_ocr("twoheadrightarrow","&twoheadrightarrow;", "&#x21A0;",twoheadrightarrow_code);
 mk_ocr("upharpoonleft","&upharpoonleft;", "&#x21BF;",upharpoonleft_code);
 mk_ocr("upharpoonright","&upharpoonright;", "&#x21BE;",upharpoonright_code);
 mk_ocr("varprime","&prime;", "&#x2032;",varprime_code); // vprime in isoamso,
 mk_ocr("Prime","&Prime;", "&#x2033;",Prime_code);
 mk_ocr("tprime","&tprime;", "&#x2034;",tprime_code);
 mk_ocr("bprime","&backprime;", "&#x2035;",bprime_code);
 mk_ocr("qprime","&qprime;", "&#x2057;",qprime_code);
 mk_ocr("veebar","&veebar;", "&#x22BB;",veebar_code);
 mk_ocr("backsimeq","&backsimeq;", "&#x22CD;",backsimeq_code);
mk_ocr("backsim","&backsim;", "&#x223D;",backsim_code);
 mk_ocr("Bumpeq","&Bumpeq;", "&#x224E;",Bumpeq_code);
 mk_ocr("bumpeq","&bumpeq;", "&#x224F;",bumpeq_code);
 mk_ocr("circeq","&circeq;", "&#x2257;",circeq_code);
 mk_ocr("curlyeqprec","&curlyeqprec;", "&#x22DE;",curlyeqprec_code);
 mk_ocr("curlyeqsucc","&curlyeqsucc;", "&#x22DF;",curlyeqsucc_code);
 mk_ocr("Doteq","&doteqdot;", "&#x2251;",Doteq_code);
 mk_ocr("doteqdot","&doteqdot;", "&#x2251;",doteqdot_code);
 mk_ocr("eqcirc","&eqcirc;", "&#x2256;",eqcirc_code);
 mk_ocr("eqsim","&eqsim;", "&#x2242;",eqsim_code);
 mk_ocr("eqslantgtr","&eqslantgtr;", "&#x2A96;",eqslantgtr_code);
 mk_ocr("eqslantless","&eqslantless;", "&#x2A95;",eqslantless_code);
 mk_ocr("fallingdotseq","&fallingdotseq;", "&#x2252;",fallingdotseq_code);
 mk_ocr("risingdotseq","&risingdotseq;", "&#x2253;",risingdotseq_code);
 mk_ocr("geqq","&geqq;", "&#x2267;",geqq_code);
 mk_ocr("leqq","&leqq;", "&#x2266;",leqq_code);
 mk_ocr("gggtr","&ggg;", "&#x22D9;",gggtr_code);
 mk_ocr("ggg","&ggg;", "&#x22D9;",ggg_code);
 mk_ocr("lll","&Ll;", "&#x22D8;",lll_code);
 mk_ocr("llless","&Ll;", "&#x22D8;",llless_code);
 mk_ocr("gtrapprox","&gtrapprox;", "&#x2A86;",gtrapprox_code);
 mk_ocr("lessapprox","&lessapprox;", "&#x2A85;",lessapprox_code);
 mk_ocr("gtreqless","&gtreqless;", "&#x22DB;",gtreqless_code);
 mk_ocr("gtreqqless","&gtreqqless;", "&#x2A8C;",gtreqqless_code);
 mk_ocr("lesseqgtr","&lesseqgtr;", "&#x22DA;",lesseqgtr_code);
 mk_ocr("lesseqqgtr","&lesseqqgtr;", "&#x2A8B;",lesseqqgtr_code);
 mk_ocr("lessgtr","&lessgtr;", "&#x2276;",lessgtr_code);
 mk_ocr("lesssim","&lesssim;", "&#x2272;",lesssim_code);
 mk_ocr("gtrsim","&gtrsim;", "&#x2273;",gtrsim_code);
 mk_ocr("precapprox","&precapprox;", "&#x2AB7;",precapprox_code);
 mk_ocr("succapprox","&succapprox;", "&#x2AB8;",succapprox_code);
 mk_ocr("succcurlyeq","&succcurlyeq;", "&#x227D;",succcurlyeq_code);
 mk_ocr("succsim","&succsim;", "&#x227F;",succsim_code);
 mk_ocr("preccurlyeq","&preccurlyeq;", "&#x227C;",preccurlyeq_code);
 // mk_ocr("leftrightarroweq","&gtrsim;", "&#x2273;",leftrightarroweq_code);
 mk_ocr("gnapprox","&gnapprox;", "&#x2A8A;",gnapprox_code);
 mk_ocr("lnapprox","&lnapprox;", "&#x2A89;",lnapprox_code);
 mk_ocr("gneqq","&gneqq;", "&#x2269;",gneqq_code);
 mk_ocr("lneqq","&lneqq;", "&#x2268;",lneqq_code);
 mk_ocr("gneq","&gneq;", "&#x2A88;",gneq_code);
 mk_ocr("lneq","&lneq;", "&#x2A87;",lneq_code);
 mk_ocr("gnsim","&gnsim;", "&#x22E7;",gnsim_code);
 mk_ocr("lnsim","&lnsim;", "&#x22E6;",lnsim_code);
 mk_ocr("ngeq","&ngeq;", "&#x2271;",ngeq_code);
 mk_ocr("ngtr","&ngtr;", "&#x226F;",ngtr_code);
 mk_ocr("nleq","&nleq;", "&#x2270;",nleq_code);
 mk_ocr("nless","&nless;", "&#x226E;",nless_code);
 mk_ocr("nprec","&nprec;", "&#x2280;",nprec_code);
 mk_ocr("nsim","&nsim;", "&#x2241;",nsim_code);
 mk_ocr("nsucc","&nsucc;", "&#x2281;",nsucc_code);
 mk_ocr("precnapprox","&precnapprox;", "&#x2AB9;",precnapprox_code);
 mk_ocr("precneqq","&precneqq;", "&#x2AB5;",precneqq_code);
 mk_ocr("precnsim","&precnsim;", "&#x22E8;",precnsim_code);
 mk_ocr("precsim","&precsim;", "&#x227E;",precsim_code);
 mk_ocr("succnapprox","&succnapprox;", "&#x2ABA;",succnapprox_code);
 mk_ocr("succneqq","&succneqq;", "&#x2AB6;",succneqq_code);
 mk_ocr("succnsim","&succnsim;", "&#x22E9;",succnsim_code);
 mk_ocr("inplus","&loplus;", "&#x2A2D;",inplus_code);
 mk_ocr("niplus","&roplus;", "&#x2A2E;",niplus_code);
 mk_ocr("ntrianglerighteqslant","&ntrianglerighteq;", "&#x22ED;",ntrianglerighteqslant_code);
 mk_ocr("ntrianglelefteqslant","&ntrianglelefteq;", "&#x22EC;",ntrianglelefteqslant_code);
 mk_ocr("subseteqq","&subseteqq;", "&#x2AC5;",subseteqq_code);
 mk_ocr("supseteqq","&supseteqq;", "&#x2AC6;",supseteqq_code);
 mk_ocr("vartriangleleft","&vartriangleleft;", "&#x22B2;",vartriangleleft_code);
 mk_ocr("vartriangleright","&vartriangleright;", "&#x22B3;",vartriangleright_code);
 mk_ocr("trianglerighteq","&trianglerighteq;", "&#x22B5;",trianglerighteq_code);
 mk_ocr("trianglelefteq","&trianglelefteq;", "&#x22B4;",trianglelefteq_code);
 mk_ocr("nsubseteq","&nsubseteq;", "&#x2288;",nsubseteq_code);
 mk_ocr("nsupseteq","&nsupseteq;", "&#x2289;",nsupseteq_code);
 mk_ocr("ntrianglelefteq","&ntrianglelefteq;", "&#x22EC;",ntrianglelefteq_code);
 mk_ocr("ntrianglerighteq","&ntrianglerighteq;", "&#x22ED;",ntrianglerighteq_code);
 mk_ocr("ntriangleleft","&ntriangleleft;", "&#x22EA;",ntriangleleft_code);
 mk_ocr("ntriangleright","&ntriangleright;", "&#x22EB;",ntriangleright_code);
 mk_ocr("subsetneqq","&subsetneqq;", "&#x2ACB;",subsetneqq_code);
 mk_ocr("supsetneqq","&supsetneqq;", "&#x2ACC;",supsetneqq_code);
 mk_ocr("subsetneq","&subsetneq;", "&#x228A;",subsetneq_code);
 mk_ocr("supsetneq","&supsetneq;", "&#x228B;",supsetneq_code);

  mk_oc("rrbracket","&RightDoubleBracket;","&#x301B;", rrbracket_code);
  mk_oc("llbracket","&LeftDoubleBracket;","&#x301A;", llbracket_code);
  mk_oc("binampersand","&amp;","&amp;", binampersand_code);
  // duplicate
  //  mk_oc("bindnasrepma","&#x214B;","&#x214B;", bindnasrepma_code);
  mk_oc("llparenthesis","&#x2987;","&#x2987;", llparenthesis_code);
  mk_oc("rrparenthesis","&#x2988;","&#x2988;", rrparenthesis_code);

  // mk_oc("ldots","...");
  mk_oc("cdots","&ctdot;","&#x022EF;",cdots_code,mathinner_cmd,false);
  //  mk_oc("dots","&ctdot;");
  mk_oc("hdots","&ctdot;","&#x022EF;", hdots_code,mathinner_cmd,false);
  mk_oc("vdots","&vellip;","&#x022EE;", vdots_code,mathinner_cmd,false);
  mk_oc("ddots","&dtdot;","&#x022F1;", ddots_code,mathinner_cmd,false);
  mk_oc("imath","&imath;","&#x00131;", imath_code,mathord_cmd,false);
  mk_oc("jmath","&jmath;","&#x0006A;", jmath_code,mathord_cmd,false);
  mk_oc("@Vert","&Verbar;","&#x02016;", Vertx_code,mathbetween_cmd,false);
  mk_oc("|","&parallel;","&#x02225;", Vert_code,mathbetween_cmd,false);
  mk_oc("Vert","&parallel;","&#x02225;", Vert_code,mathbetween_cmd,false);
  eval_let("Arrowvert","Vert");
  mk_oc("lVert","&parallel;","&#x02225;", Vert_code,mathopen_cmd,false);
  mk_oc("rVert","&parallel;","&#x02225;", Vert_code,mathclose_cmd,false);
  mk_oc("vert","&vert;","&#x7C;",vert_code,mathbetween_cmd,false);
  eval_let("arrowvert","vert");
  eval_let("bracevert","vert"); // U+23AA ?
  mk_oc("lvert","&vert;","&#x7C;",vert_code,mathopen_cmd,false);
  mk_oc("rvert","&vert;","&#x7C;",vert_code,mathclose_cmd,false);
  mk_oc("uparrow","&uparrow;","&#x02191;",uparrow_code,mathbetween_cmd,true);
  mk_oc("downarrow","&downarrow;","&#x02193;",downarrow_code,mathbetween_cmd,true);
  mk_oc("Uparrow","&Uparrow;","&#x021D1;",Uparrow_code,mathbetween_cmd,true);
  mk_oc("Downarrow","&Downarrow;","&#x021D3;",Downarrow_code,mathbetween_cmd,true);
  mk_oc("Updownarrow","&Updownarrow;","&#x021D5;",Updownarrow_code,mathbetween_cmd,true);
  mk_oc("updownarrow","&updownarrow;","&#x02195;",updownarrow_code,mathbetween_cmd,true);
  mk_oc("backslash","&Backslash;","&#x2216;", backslash_code,mathbetween_cmd,false);
  mk_oc("mho","&mho;","&#x02127;", mho_code);
  mk_oc("DifferentialD","&DifferentialD;","&#x2146;",differentiald_code);
  mk_oc("leadsto","&#x21DD;","&#x21DD;", leadsto_code);
  mk_oc("lhd","&#x22B2;","&#x22B2;", lhd_code);
  mk_oc("rhd","&#x22B3;","&#x22B3;", rhd_code);
  mk_oc("unlhd","&#x22B4;","&#x22B4;", unlhd_code);
  mk_oc("unrhd","&#x22B5;","&#x22B5;", unrhd_code);
  mk_oc("clubsuit","&clubsuit;","&#x02663;",clubsuit_code);
  mk_oc("diamondsuit","&diamondsuit;","&#x02666;", diamondsuit_code);
  mk_oc("heartsuit","&heartsuit;","&#x02665;",heartsuit_code);
  mk_oc("spadesuit","&spadesuit;","&#x02660;" , spadesuit_code);
  mk_oc("Box","&#x25A1;","&#x25A1;",Box_code);
  mk_oc("rangle","&rangle;",RANGLE,rangle_code,mathclose_cmd,true);
  mk_oc("langle","&langle;",LANGLE,langle_code,mathopen_cmd,true);
  mk_oc("varrangle","&rangle;","&#x27E9;",varrangle_code,mathclose_cmd,false);
  mk_oc("varlangle","&langle;", "&#x27E8;",varlangle_code,mathopen_cmd,false);
  mk_oc("rmoustache","&rmoustache;","&#x023B1;",rmoustache_code,mathclose_cmd,false);
  mk_oc("lmoustache","&lmoustache;","&#x023B0;",lmoustache_code,mathopen_cmd,false);
  mk_oc("rgroup","&#x3015;","&#x3015;",rgroup_code,mathclose_cmd,false);
  mk_oc("lgroup","&#x3014;","&#x3014;",lgroup_code,mathopen_cmd,false);
  mk_oc("rbrace","&rbrace;","&#x0007D;",rbrace_code,mathclose_cmd,true);
  mk_oc("lbrace","&lbrace;","&#x0007B;",lbrace_code,mathopen_cmd,true);
  mk_oc("lceil","&lceil;","&#x02308;",lceil_code,mathopen_cmd,false);
  mk_oc("rceil","&rceil;","&#x02309;",rceil_code,mathclose_cmd,false);
  mk_oc("lfloor","&lfloor;","&#x0230A;",lfloor_code,mathopen_cmd,false);
  mk_oc("rfloor","&rfloor;","&#x0230B;",rfloor_code,mathclose_cmd,false);

  mk_oc("square","&square;","&#x025A1;", square_code);
  mk_oc("cong","&cong;","&#x02245;", cong_code);
  mk_oc("ncong","&#x2247;","&#x02247;", ncong_code);
  mk_oc("lnot","&not;","&#xAC;", not_code);
  eval_let("neg","lnot");


  mk_ocb("in", "&Element;", "&#x02208;", in_code);
  the_parser.hash_table.math_OB_token=  
    mk_gen("math{","{","{",open_brace_code,cst_mo,mathopen_cmd,true);
  the_parser.hash_table.math_CB_token=  
    mk_gen("math}","}","}",close_brace_code,cst_mo,mathclose_cmd,true);
  //mk_oc("varprime", "&prime;","&#x02032;", prime_code);
  mk_oc("prime", "&apos;","&#x27;", prime_code);
  mc_table[27] = built_in_table[prime_code];

  TM_mk("<", "&lt;", mt_flag_small);
  TM_mk(">", "&gt;", mt_flag_small);
  TM_mk(",", ",", mt_flag_small);
  TM_mk(".", ".", mt_flag_small);
  TM_mk(":", ":", mt_flag_small);
  TM_mk(";", ";", mt_flag_small);
  TM_mk("*", "*", mt_flag_small);
  TM_mk("?", "?", mt_flag_small);
  TM_mk("!", "!", mt_flag_small);

  // Only ascii characters allowed here
  // TM_mk("\327", no_ent_names ? "&#x000D7;" : "&times;", mt_flag_small);


  TM_mk("(", "(", mt_flag_small_l);
  TM_mk(")", ")", mt_flag_small_r);
  TM_mk("[", "[", mt_flag_small_l);
  TM_mk("]", "]", mt_flag_small_r);
  TM_mk("|", "|", mt_flag_small_m);
  TM_mk("+", "+", mt_flag_bin);
  if(bad_minus)
    TM_mk("-", "&#x2013;", mt_flag_bin);
  else
    TM_mk("-", "-", mt_flag_bin);
  TM_mk("/", "/", mt_flag_bin);
  TM_mk("=", "=", mt_flag_rel);
  TM_mk("&", "&amp;", mt_flag_small);


  mk_accent("acute", "&acute;","&#x000B4;",acute_code);
  mk_accent("grave", "&grave;","&#x60;", grave_code);
  mk_accent("mathring",  "&#x2DA;","&#x2DA;", mathring_code);
  mk_accent("ddddot",  "&#x20DC;","&#x20DC;", ddddot_code);
  mk_accent("dddot",  "&#x20DB;", "&#x20DB;", dddot_code);
  mk_accent("ddot",  "&die;", "&#xA8;", ddot_code);
  mk_accent("tilde", "&tilde;", "&#x2DC;", tilde_code);
  mk_accent("widetilde", "&tilde;", "&#x2DC;", widetilde_code); // No wide....
  mk_accent("bar", "&OverBar;", "&#xAF;", bar_code);
  mk_accent("breve", "&breve;", "&#x2D8;", breve_code);
  mk_accent("check", "&Hacek;", "&#x2C7;", check_code);
  mk_accent("hat", "&Hat;", "&#x5E;", hat_code);
  mk_accent("widehat", "&Hat;", "&#x5E;", widehat_code);    // No wide....
  mk_accent("vec", "&rightarrow;", "&#x02192;", vec_code);
  mk_accent("overrightarrow", "&rightarrow;", "&#x02192;", overrightarrow_code);
  mk_accent("overleftarrow", "&leftarrow;", "&#x02190;", overleftarrow_code);
  mk_accent("overleftrightarrow", "&leftrightarrow;", "&#x02194;", overleftrightarrow_code);
  mk_accent("underrightarrow", "&rightarrow;", "&#x02192;", underrightarrow_code);
  mk_accent("xrightarrow", "&rightarrow;", "&#x02192;", xrightarrow_code);
  mk_accent("underleftarrow", "&leftarrow;", "&#x02190;", underleftarrow_code);
  mk_accent("underleftrightarrow", "&leftrightarrow;", "&#x02194;", underleftrightarrow_code);
  mk_accent("xleftarrow", "&leftarrow;", "&#x02190;", xleftarrow_code);
  mk_accent("dot", "&dot;","&#x002D9;", dot_code);
  eval_let("varbigtriangledown","bigtriangledown");
  eval_let("varbigtriangleup","bigtriangleup");
  eval_let("bigcurlyvee","curlyvee");
  eval_let("bigcurlywedge","curlywedge");
  eval_let("varcurlyvee","curlyvee");
  eval_let("varcurlywedge","curlywedge");
  eval_let("doublecup","Cup");
  eval_let("doublecap","Cap");
  eval_let("smallsetminus","setminus");
  eval_let("boxempty","square");
  eval_let("oblong","square");
  eval_let("oast","circledast");
  eval_let("ocircle","circledcirc");
  eval_let("varobar","obar");
  eval_let("varodot","odot");
  eval_let("varoast","oast");
  eval_let("varominus","ominus");
  eval_let("varoplus","oplus");
  eval_let("varotimes","otimes");
  eval_let("varoslash","oslash");
  eval_let("varbigcirc","bigcirc");
  eval_let("varobslash","oslash");
  eval_let("varogreaterthan","ogreaterthan");
  eval_let("varolessthan","olessthan");
  eval_let("bigbox","square");
  eval_let("biginterleave","interleave");
  eval_let("bignplus","nplus");
  eval_let("bigparallel","parallel");
  eval_let("bigsqcap","sqcap");
  eval_let("mathellipsis","ldots");
  eval_let("restriction", "upharpoonright");
  eval_let("shortdownarrow", "downarrow");
  eval_let("shortleftarrow", "leftarrow");
  eval_let("shortrightarrow", "rightarrow");
  eval_let("shortuparrow", "uparrow");
  eval_let("thickapprox", "approx");
  eval_let("thicksim", "sim");
  eval_let("vartimes", "times");
  mk_cmd("qopname", qopname_code);
  mk_cmd("operatorname", operatorname_code);
  mk_cmd("operatornamewithlimits", operatornamestar_code);
  mk_cmd("overline", overline_code);
  mk_cmd("underline", underline_code);
  mk_cmd("stackrel", stackrel_code);
  mk_cmd("underset", underset_code);
  mk_cmd("overset", overset_code);
  mk_cmd("mathchoice", mathchoice_code);
  mk_cmd("frac", frac_code);
  mk_cmd("overbrace", overbrace_code);
  mk_cmd("underbrace", underbrace_code);
  mk_cmd("undertilde", undertilde_code);
  mk_cmd("dfrac",dfrac_code);
  mk_cmd("cfrac",cfrac_code);
  mk_cmd("tfrac",tfrac_code);
  mk_cmd("boxed",boxed_code);
  mk_cmd("binom",binom_code);
  mk_cmd("dbinom",dbinom_code);
  mk_cmd("tbinom",tbinom_code);
  mk_cmd("sqrt",sqrt_code);
  mk_cmd("@root",root_code);
  mk_cmd("mathbox",mathbox_code);
  mk_cmd("genfrac",genfrac_code);
  mk_cmd("phantom",phantom_code);
  mk_cmd("vphantom",vphantom_code);
  mk_cmd("hphantom",hphantom_code);
  mk_cmd("smash",smash_code);
  mk_cmd("accentset",accentset_code);
  mk_cmd("underaccent",underaccent_code);
  init_builtin(xml_zero_space_loc, mk_space("0.3pt"));
  init_builtin(xml_thinmu_space_loc, mk_space("0.166667em"));
  init_builtin(xml_medmu_space_loc, mk_space("0.222222em"));
  init_builtin(xml_thickmu_space_loc, mk_space("0.277778em"));
  init_builtin(xml_thinmuneg_space_loc, mk_space("-0.166667em"));
  // This is not a quad, but the space of tilde
  init_builtin(xml_quad_space_loc, mk_space("3.33333pt"));
  //  mk_space("enspace", xml_thinmu_space_loc);
  // mk_space("zerospace", xml_zero_space_loc);
  mk_space(">",xml_medmu_space_loc);
  mk_space(";", xml_thickmu_space_loc);
  mk_space(":", xml_medmu_space_loc);
  init_builtin(math_c_loc+'~', xml_quad_space_loc);
  math_char_type[uchar('~')] = mt_flag_space;
  boot_xml_lr_tables();
  boot2();
}

// 338 (big ) 337 (small) negation postfix
