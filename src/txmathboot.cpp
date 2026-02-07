// Tralics, a LaTeX to XML translator.
// Copyright INRIA/apics/marelle (Jose Grimm) 2004, 2007-2011

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// Tralics math; boot part. This constructs all data structures.

#include "tralics/Dispatcher.h"
#include "tralics/MainClass.h"
#include "tralics/MathDataP.h"
#include "tralics/Parser.h"
#include "tralics/globals.h"
#include "tralics/util.h"

using namespace math_ns;

// mathml variants: normal, bold, italic, bold-italic, double-struck,
// bold-fraktur, script, bold-script, fraktur, sans-serif, bold-sans-serif,
// sans-serif-italic, sans-serif-bold-italic, monospace
std::array<Xml *, 128> single_chars{};

#define LANGLE "&#x02329;"
#define RANGLE "&#x0232A;"
//#define LANGLE "&#x27E8;"
//#define RANGLE "&#x27E9;"

auto math_ns::get_builtin_alt(size_t p) -> Xml * { return math_data.get_builtin_alt(p); } // \todo Why a global function?

inline void eval_let(String a, String b) { hash_table.eval_let(a, b); }
void        math_ns::fill_single_char() {
    for (uchar x = 'a'; x <= 'z'; x++) { single_chars[x] = new Xml(std::string(1, char(x))); }
    for (uchar x = 'A'; x <= 'Z'; x++) { single_chars[x] = new Xml(std::string(1, char(x))); }
}

// In the case of \Bbb A, we can use
//  &Aopf; or  &#x15D38; or mathvariant=doublestruck
// Here we fill the table with &Aopf;
void math_ns::fill_math_char_slots_ent() {
    // from isomopf.ent
    // lower case letters from mmlextra.ent
    global_state.math_chars[uchar('A')][math_f_doublestruck] = "&Aopf;";
    global_state.math_chars[uchar('B')][math_f_doublestruck] = "&Bopf;";
    global_state.math_chars[uchar('C')][math_f_doublestruck] = "&Copf;";
    global_state.math_chars[uchar('D')][math_f_doublestruck] = "&Dopf;";
    global_state.math_chars[uchar('E')][math_f_doublestruck] = "&Eopf;";
    global_state.math_chars[uchar('F')][math_f_doublestruck] = "&Fopf;";
    global_state.math_chars[uchar('G')][math_f_doublestruck] = "&Gopf;";
    global_state.math_chars[uchar('H')][math_f_doublestruck] = "&Hopf;";
    global_state.math_chars[uchar('I')][math_f_doublestruck] = "&Iopf;";
    global_state.math_chars[uchar('J')][math_f_doublestruck] = "&Jopf;";
    global_state.math_chars[uchar('K')][math_f_doublestruck] = "&Kopf;";
    global_state.math_chars[uchar('L')][math_f_doublestruck] = "&Lopf;";
    global_state.math_chars[uchar('M')][math_f_doublestruck] = "&Mopf;";
    global_state.math_chars[uchar('N')][math_f_doublestruck] = "&Nopf;";
    global_state.math_chars[uchar('O')][math_f_doublestruck] = "&Oopf;";
    global_state.math_chars[uchar('P')][math_f_doublestruck] = "&Popf;";
    global_state.math_chars[uchar('Q')][math_f_doublestruck] = "&Qopf;";
    global_state.math_chars[uchar('R')][math_f_doublestruck] = "&Ropf;";
    global_state.math_chars[uchar('S')][math_f_doublestruck] = "&Sopf;";
    global_state.math_chars[uchar('T')][math_f_doublestruck] = "&Topf;";
    global_state.math_chars[uchar('U')][math_f_doublestruck] = "&Uopf;";
    global_state.math_chars[uchar('V')][math_f_doublestruck] = "&Vopf;";
    global_state.math_chars[uchar('W')][math_f_doublestruck] = "&Wopf;";
    global_state.math_chars[uchar('X')][math_f_doublestruck] = "&Xopf;";
    global_state.math_chars[uchar('Y')][math_f_doublestruck] = "&Yopf;";
    global_state.math_chars[uchar('Z')][math_f_doublestruck] = "&Zopf;";
    global_state.math_chars[uchar('a')][math_f_doublestruck] = "&aopf;";
    global_state.math_chars[uchar('b')][math_f_doublestruck] = "&bopf;";
    global_state.math_chars[uchar('c')][math_f_doublestruck] = "&copf;";
    global_state.math_chars[uchar('d')][math_f_doublestruck] = "&dopf;";
    global_state.math_chars[uchar('e')][math_f_doublestruck] = "&eopf;";
    global_state.math_chars[uchar('f')][math_f_doublestruck] = "&fopf;";
    global_state.math_chars[uchar('g')][math_f_doublestruck] = "&gopf;";
    global_state.math_chars[uchar('h')][math_f_doublestruck] = "&hopf;";
    global_state.math_chars[uchar('i')][math_f_doublestruck] = "&iopf;";
    global_state.math_chars[uchar('j')][math_f_doublestruck] = "&jopf;";
    global_state.math_chars[uchar('k')][math_f_doublestruck] = "&kopf;";
    global_state.math_chars[uchar('l')][math_f_doublestruck] = "&lopf;";
    global_state.math_chars[uchar('m')][math_f_doublestruck] = "&mopf;";
    global_state.math_chars[uchar('n')][math_f_doublestruck] = "&nopf;";
    global_state.math_chars[uchar('o')][math_f_doublestruck] = "&oopf;";
    global_state.math_chars[uchar('p')][math_f_doublestruck] = "&popf;";
    global_state.math_chars[uchar('q')][math_f_doublestruck] = "&qopf;";
    global_state.math_chars[uchar('r')][math_f_doublestruck] = "&ropf;";
    global_state.math_chars[uchar('s')][math_f_doublestruck] = "&sopf;";
    global_state.math_chars[uchar('t')][math_f_doublestruck] = "&topf;";
    global_state.math_chars[uchar('u')][math_f_doublestruck] = "&uopf;";
    global_state.math_chars[uchar('v')][math_f_doublestruck] = "&vopf;";
    global_state.math_chars[uchar('w')][math_f_doublestruck] = "&wopf;";
    global_state.math_chars[uchar('x')][math_f_doublestruck] = "&xopf;";
    global_state.math_chars[uchar('y')][math_f_doublestruck] = "&yopf;";
    global_state.math_chars[uchar('z')][math_f_doublestruck] = "&zopf;";

    // from isomscr.ent
    global_state.math_chars[uchar('A')][math_f_script] = "&Ascr;";
    global_state.math_chars[uchar('B')][math_f_script] = "&Bscr;";
    global_state.math_chars[uchar('C')][math_f_script] = "&Cscr;";
    global_state.math_chars[uchar('D')][math_f_script] = "&Dscr;";
    global_state.math_chars[uchar('E')][math_f_script] = "&Escr;";
    global_state.math_chars[uchar('F')][math_f_script] = "&Fscr;";
    global_state.math_chars[uchar('G')][math_f_script] = "&Gscr;";
    global_state.math_chars[uchar('H')][math_f_script] = "&Hscr;";
    global_state.math_chars[uchar('I')][math_f_script] = "&Iscr;";
    global_state.math_chars[uchar('J')][math_f_script] = "&Jscr;";
    global_state.math_chars[uchar('K')][math_f_script] = "&Kscr;";
    global_state.math_chars[uchar('L')][math_f_script] = "&Lscr;";
    global_state.math_chars[uchar('M')][math_f_script] = "&Mscr;";
    global_state.math_chars[uchar('N')][math_f_script] = "&Nscr;";
    global_state.math_chars[uchar('O')][math_f_script] = "&Oscr;";
    global_state.math_chars[uchar('P')][math_f_script] = "&Pscr;";
    global_state.math_chars[uchar('Q')][math_f_script] = "&Qscr;";
    global_state.math_chars[uchar('R')][math_f_script] = "&Rscr;";
    global_state.math_chars[uchar('S')][math_f_script] = "&Sscr;";
    global_state.math_chars[uchar('T')][math_f_script] = "&Tscr;";
    global_state.math_chars[uchar('U')][math_f_script] = "&Uscr;";
    global_state.math_chars[uchar('V')][math_f_script] = "&Vscr;";
    global_state.math_chars[uchar('W')][math_f_script] = "&Wscr;";
    global_state.math_chars[uchar('X')][math_f_script] = "&Xscr;";
    global_state.math_chars[uchar('Y')][math_f_script] = "&Yscr;";
    global_state.math_chars[uchar('Z')][math_f_script] = "&Zscr;";
    global_state.math_chars[uchar('a')][math_f_script] = "&ascr;";
    global_state.math_chars[uchar('b')][math_f_script] = "&bscr;";
    global_state.math_chars[uchar('c')][math_f_script] = "&cscr;";
    global_state.math_chars[uchar('d')][math_f_script] = "&dscr;";
    global_state.math_chars[uchar('e')][math_f_script] = "&escr;";
    global_state.math_chars[uchar('f')][math_f_script] = "&fscr;";
    global_state.math_chars[uchar('g')][math_f_script] = "&gscr;";
    global_state.math_chars[uchar('h')][math_f_script] = "&hscr;";
    global_state.math_chars[uchar('i')][math_f_script] = "&iscr;";
    global_state.math_chars[uchar('j')][math_f_script] = "&jscr;";
    global_state.math_chars[uchar('k')][math_f_script] = "&kscr;";
    global_state.math_chars[uchar('l')][math_f_script] = "&lscr;";
    global_state.math_chars[uchar('m')][math_f_script] = "&mscr;";
    global_state.math_chars[uchar('n')][math_f_script] = "&nscr;";
    global_state.math_chars[uchar('o')][math_f_script] = "&oscr;";
    global_state.math_chars[uchar('p')][math_f_script] = "&pscr;";
    global_state.math_chars[uchar('q')][math_f_script] = "&qscr;";
    global_state.math_chars[uchar('r')][math_f_script] = "&rscr;";
    global_state.math_chars[uchar('s')][math_f_script] = "&sscr;";
    global_state.math_chars[uchar('t')][math_f_script] = "&tscr;";
    global_state.math_chars[uchar('u')][math_f_script] = "&uscr;";
    global_state.math_chars[uchar('v')][math_f_script] = "&vscr;";
    global_state.math_chars[uchar('w')][math_f_script] = "&wscr;";
    global_state.math_chars[uchar('x')][math_f_script] = "&xscr;";
    global_state.math_chars[uchar('y')][math_f_script] = "&yscr;";
    global_state.math_chars[uchar('z')][math_f_script] = "&zscr;";

    // From isomfrk.ent
    global_state.math_chars[uchar('A')][math_f_fraktur] = "&Afr;";
    global_state.math_chars[uchar('B')][math_f_fraktur] = "&Bfr;";
    global_state.math_chars[uchar('C')][math_f_fraktur] = "&Cfr;";
    global_state.math_chars[uchar('D')][math_f_fraktur] = "&Dfr;";
    global_state.math_chars[uchar('E')][math_f_fraktur] = "&Efr;";
    global_state.math_chars[uchar('F')][math_f_fraktur] = "&Ffr;";
    global_state.math_chars[uchar('G')][math_f_fraktur] = "&Gfr;";
    global_state.math_chars[uchar('H')][math_f_fraktur] = "&Hfr;";
    global_state.math_chars[uchar('I')][math_f_fraktur] = "&Ifr;";
    global_state.math_chars[uchar('J')][math_f_fraktur] = "&Jfr;";
    global_state.math_chars[uchar('K')][math_f_fraktur] = "&Kfr;";
    global_state.math_chars[uchar('L')][math_f_fraktur] = "&Lfr;";
    global_state.math_chars[uchar('M')][math_f_fraktur] = "&Mfr;";
    global_state.math_chars[uchar('N')][math_f_fraktur] = "&Nfr;";
    global_state.math_chars[uchar('O')][math_f_fraktur] = "&Ofr;";
    global_state.math_chars[uchar('P')][math_f_fraktur] = "&Pfr;";
    global_state.math_chars[uchar('Q')][math_f_fraktur] = "&Qfr;";
    global_state.math_chars[uchar('R')][math_f_fraktur] = "&Rfr;";
    global_state.math_chars[uchar('S')][math_f_fraktur] = "&Sfr;";
    global_state.math_chars[uchar('T')][math_f_fraktur] = "&Tfr;";
    global_state.math_chars[uchar('U')][math_f_fraktur] = "&Ufr;";
    global_state.math_chars[uchar('V')][math_f_fraktur] = "&Vfr;";
    global_state.math_chars[uchar('W')][math_f_fraktur] = "&Wfr;";
    global_state.math_chars[uchar('X')][math_f_fraktur] = "&Xfr;";
    global_state.math_chars[uchar('Y')][math_f_fraktur] = "&Yfr;";
    global_state.math_chars[uchar('Z')][math_f_fraktur] = "&Zfr;";
    global_state.math_chars[uchar('a')][math_f_fraktur] = "&afr;";
    global_state.math_chars[uchar('b')][math_f_fraktur] = "&bfr;";
    global_state.math_chars[uchar('c')][math_f_fraktur] = "&cfr;";
    global_state.math_chars[uchar('d')][math_f_fraktur] = "&dfr;";
    global_state.math_chars[uchar('e')][math_f_fraktur] = "&efr;";
    global_state.math_chars[uchar('f')][math_f_fraktur] = "&ffr;";
    global_state.math_chars[uchar('g')][math_f_fraktur] = "&gfr;";
    global_state.math_chars[uchar('h')][math_f_fraktur] = "&hfr;";
    global_state.math_chars[uchar('i')][math_f_fraktur] = "&ifr;";
    global_state.math_chars[uchar('j')][math_f_fraktur] = "&jfr;";
    global_state.math_chars[uchar('k')][math_f_fraktur] = "&kfr;";
    global_state.math_chars[uchar('l')][math_f_fraktur] = "&lfr;";
    global_state.math_chars[uchar('m')][math_f_fraktur] = "&mfr;";
    global_state.math_chars[uchar('n')][math_f_fraktur] = "&nfr;";
    global_state.math_chars[uchar('o')][math_f_fraktur] = "&ofr;";
    global_state.math_chars[uchar('p')][math_f_fraktur] = "&pfr;";
    global_state.math_chars[uchar('q')][math_f_fraktur] = "&qfr;";
    global_state.math_chars[uchar('r')][math_f_fraktur] = "&rfr;";
    global_state.math_chars[uchar('s')][math_f_fraktur] = "&sfr;";
    global_state.math_chars[uchar('t')][math_f_fraktur] = "&tfr;";
    global_state.math_chars[uchar('u')][math_f_fraktur] = "&ufr;";
    global_state.math_chars[uchar('v')][math_f_fraktur] = "&vfr;";
    global_state.math_chars[uchar('w')][math_f_fraktur] = "&wfr;";
    global_state.math_chars[uchar('x')][math_f_fraktur] = "&xfr;";
    global_state.math_chars[uchar('y')][math_f_fraktur] = "&yfr;";
    global_state.math_chars[uchar('z')][math_f_fraktur] = "&zfr;";
}

void math_ns::fill_math_char_slots() {
    for (auto &math_char : global_state.math_chars) std::fill(math_char.begin(), math_char.end(), "");
    // Position 0 : normal
    global_state.math_chars[uchar('A')][math_f_normal] = "A";
    global_state.math_chars[uchar('B')][math_f_normal] = "B";
    global_state.math_chars[uchar('C')][math_f_normal] = "C";
    global_state.math_chars[uchar('D')][math_f_normal] = "D";
    global_state.math_chars[uchar('E')][math_f_normal] = "E";
    global_state.math_chars[uchar('F')][math_f_normal] = "F";
    global_state.math_chars[uchar('G')][math_f_normal] = "G";
    global_state.math_chars[uchar('H')][math_f_normal] = "H";
    global_state.math_chars[uchar('I')][math_f_normal] = "I";
    global_state.math_chars[uchar('J')][math_f_normal] = "J";
    global_state.math_chars[uchar('K')][math_f_normal] = "K";
    global_state.math_chars[uchar('L')][math_f_normal] = "L";
    global_state.math_chars[uchar('M')][math_f_normal] = "M";
    global_state.math_chars[uchar('N')][math_f_normal] = "N";
    global_state.math_chars[uchar('O')][math_f_normal] = "O";
    global_state.math_chars[uchar('P')][math_f_normal] = "P";
    global_state.math_chars[uchar('Q')][math_f_normal] = "Q";
    global_state.math_chars[uchar('R')][math_f_normal] = "R";
    global_state.math_chars[uchar('S')][math_f_normal] = "S";
    global_state.math_chars[uchar('T')][math_f_normal] = "T";
    global_state.math_chars[uchar('U')][math_f_normal] = "U";
    global_state.math_chars[uchar('V')][math_f_normal] = "V";
    global_state.math_chars[uchar('W')][math_f_normal] = "W";
    global_state.math_chars[uchar('X')][math_f_normal] = "X";
    global_state.math_chars[uchar('Y')][math_f_normal] = "Y";
    global_state.math_chars[uchar('Z')][math_f_normal] = "Z";
    global_state.math_chars[uchar('a')][math_f_normal] = "a";
    global_state.math_chars[uchar('b')][math_f_normal] = "b";
    global_state.math_chars[uchar('c')][math_f_normal] = "c";
    global_state.math_chars[uchar('d')][math_f_normal] = "d";
    global_state.math_chars[uchar('e')][math_f_normal] = "e";
    global_state.math_chars[uchar('f')][math_f_normal] = "f";
    global_state.math_chars[uchar('g')][math_f_normal] = "g";
    global_state.math_chars[uchar('h')][math_f_normal] = "h";
    global_state.math_chars[uchar('i')][math_f_normal] = "i";
    global_state.math_chars[uchar('j')][math_f_normal] = "j";
    global_state.math_chars[uchar('k')][math_f_normal] = "k";
    global_state.math_chars[uchar('l')][math_f_normal] = "l";
    global_state.math_chars[uchar('m')][math_f_normal] = "m";
    global_state.math_chars[uchar('n')][math_f_normal] = "n";
    global_state.math_chars[uchar('o')][math_f_normal] = "o";
    global_state.math_chars[uchar('p')][math_f_normal] = "p";
    global_state.math_chars[uchar('q')][math_f_normal] = "q";
    global_state.math_chars[uchar('r')][math_f_normal] = "r";
    global_state.math_chars[uchar('s')][math_f_normal] = "s";
    global_state.math_chars[uchar('t')][math_f_normal] = "t";
    global_state.math_chars[uchar('u')][math_f_normal] = "u";
    global_state.math_chars[uchar('v')][math_f_normal] = "v";
    global_state.math_chars[uchar('w')][math_f_normal] = "w";
    global_state.math_chars[uchar('x')][math_f_normal] = "x";
    global_state.math_chars[uchar('y')][math_f_normal] = "y";
    global_state.math_chars[uchar('z')][math_f_normal] = "z";

    // Position 1 : upright, not mathml
    global_state.math_chars[uchar('A')][math_f_upright] = " A ";
    global_state.math_chars[uchar('B')][math_f_upright] = " B ";
    global_state.math_chars[uchar('C')][math_f_upright] = " C ";
    global_state.math_chars[uchar('D')][math_f_upright] = " D ";
    global_state.math_chars[uchar('E')][math_f_upright] = " E ";
    global_state.math_chars[uchar('F')][math_f_upright] = " F ";
    global_state.math_chars[uchar('G')][math_f_upright] = " G ";
    global_state.math_chars[uchar('H')][math_f_upright] = " H ";
    global_state.math_chars[uchar('I')][math_f_upright] = " I ";
    global_state.math_chars[uchar('J')][math_f_upright] = " J ";
    global_state.math_chars[uchar('K')][math_f_upright] = " K ";
    global_state.math_chars[uchar('L')][math_f_upright] = " L ";
    global_state.math_chars[uchar('M')][math_f_upright] = " M ";
    global_state.math_chars[uchar('N')][math_f_upright] = " N ";
    global_state.math_chars[uchar('O')][math_f_upright] = " O ";
    global_state.math_chars[uchar('P')][math_f_upright] = " P ";
    global_state.math_chars[uchar('Q')][math_f_upright] = " Q ";
    global_state.math_chars[uchar('R')][math_f_upright] = " R ";
    global_state.math_chars[uchar('S')][math_f_upright] = " S ";
    global_state.math_chars[uchar('T')][math_f_upright] = " T ";
    global_state.math_chars[uchar('U')][math_f_upright] = " U ";
    global_state.math_chars[uchar('V')][math_f_upright] = " V ";
    global_state.math_chars[uchar('W')][math_f_upright] = " W ";
    global_state.math_chars[uchar('X')][math_f_upright] = " X ";
    global_state.math_chars[uchar('Y')][math_f_upright] = " Y ";
    global_state.math_chars[uchar('Z')][math_f_upright] = " Z ";
    global_state.math_chars[uchar('a')][math_f_upright] = " a ";
    global_state.math_chars[uchar('b')][math_f_upright] = " b ";
    global_state.math_chars[uchar('c')][math_f_upright] = " c ";
    global_state.math_chars[uchar('d')][math_f_upright] = " d ";
    global_state.math_chars[uchar('e')][math_f_upright] = " e ";
    global_state.math_chars[uchar('f')][math_f_upright] = " f ";
    global_state.math_chars[uchar('g')][math_f_upright] = " g ";
    global_state.math_chars[uchar('h')][math_f_upright] = " h ";
    global_state.math_chars[uchar('i')][math_f_upright] = " i ";
    global_state.math_chars[uchar('j')][math_f_upright] = " j ";
    global_state.math_chars[uchar('k')][math_f_upright] = " k ";
    global_state.math_chars[uchar('l')][math_f_upright] = " l ";
    global_state.math_chars[uchar('m')][math_f_upright] = " m ";
    global_state.math_chars[uchar('n')][math_f_upright] = " n ";
    global_state.math_chars[uchar('o')][math_f_upright] = " o ";
    global_state.math_chars[uchar('p')][math_f_upright] = " p ";
    global_state.math_chars[uchar('q')][math_f_upright] = " q ";
    global_state.math_chars[uchar('r')][math_f_upright] = " r ";
    global_state.math_chars[uchar('s')][math_f_upright] = " s ";
    global_state.math_chars[uchar('t')][math_f_upright] = " t ";
    global_state.math_chars[uchar('u')][math_f_upright] = " u ";
    global_state.math_chars[uchar('v')][math_f_upright] = " v ";
    global_state.math_chars[uchar('w')][math_f_upright] = " w ";
    global_state.math_chars[uchar('x')][math_f_upright] = " x ";
    global_state.math_chars[uchar('y')][math_f_upright] = " y ";
    global_state.math_chars[uchar('z')][math_f_upright] = " z ";
    // Position 2 : bold
    global_state.math_chars[uchar('A')][math_f_bold] = "&#x1D400;";
    global_state.math_chars[uchar('B')][math_f_bold] = "&#x1D401;";
    global_state.math_chars[uchar('C')][math_f_bold] = "&#x1D402;";
    global_state.math_chars[uchar('D')][math_f_bold] = "&#x1D403;";
    global_state.math_chars[uchar('E')][math_f_bold] = "&#x1D404;";
    global_state.math_chars[uchar('F')][math_f_bold] = "&#x1D405;";
    global_state.math_chars[uchar('G')][math_f_bold] = "&#x1D406;";
    global_state.math_chars[uchar('H')][math_f_bold] = "&#x1D407;";
    global_state.math_chars[uchar('I')][math_f_bold] = "&#x1D408;";
    global_state.math_chars[uchar('J')][math_f_bold] = "&#x1D409;";
    global_state.math_chars[uchar('K')][math_f_bold] = "&#x1D40A;";
    global_state.math_chars[uchar('L')][math_f_bold] = "&#x1D40B;";
    global_state.math_chars[uchar('M')][math_f_bold] = "&#x1D40C;";
    global_state.math_chars[uchar('N')][math_f_bold] = "&#x1D40D;";
    global_state.math_chars[uchar('O')][math_f_bold] = "&#x1D40E;";
    global_state.math_chars[uchar('P')][math_f_bold] = "&#x1D40F;";
    global_state.math_chars[uchar('Q')][math_f_bold] = "&#x1D410;";
    global_state.math_chars[uchar('R')][math_f_bold] = "&#x1D411;";
    global_state.math_chars[uchar('S')][math_f_bold] = "&#x1D412;";
    global_state.math_chars[uchar('T')][math_f_bold] = "&#x1D413;";
    global_state.math_chars[uchar('U')][math_f_bold] = "&#x1D414;";
    global_state.math_chars[uchar('V')][math_f_bold] = "&#x1D415;";
    global_state.math_chars[uchar('W')][math_f_bold] = "&#x1D416;";
    global_state.math_chars[uchar('X')][math_f_bold] = "&#x1D417;";
    global_state.math_chars[uchar('Y')][math_f_bold] = "&#x1D418;";
    global_state.math_chars[uchar('Z')][math_f_bold] = "&#x1D419;";
    global_state.math_chars[uchar('a')][math_f_bold] = "&#x1D41A;";
    global_state.math_chars[uchar('b')][math_f_bold] = "&#x1D41B;";
    global_state.math_chars[uchar('c')][math_f_bold] = "&#x1D41C;";
    global_state.math_chars[uchar('d')][math_f_bold] = "&#x1D41D;";
    global_state.math_chars[uchar('e')][math_f_bold] = "&#x1D41E;";
    global_state.math_chars[uchar('f')][math_f_bold] = "&#x1D41F;";
    global_state.math_chars[uchar('g')][math_f_bold] = "&#x1D420;";
    global_state.math_chars[uchar('h')][math_f_bold] = "&#x1D421;";
    global_state.math_chars[uchar('i')][math_f_bold] = "&#x1D422;";
    global_state.math_chars[uchar('j')][math_f_bold] = "&#x1D423;";
    global_state.math_chars[uchar('k')][math_f_bold] = "&#x1D424;";
    global_state.math_chars[uchar('l')][math_f_bold] = "&#x1D425;";
    global_state.math_chars[uchar('m')][math_f_bold] = "&#x1D426;";
    global_state.math_chars[uchar('n')][math_f_bold] = "&#x1D427;";
    global_state.math_chars[uchar('o')][math_f_bold] = "&#x1D428;";
    global_state.math_chars[uchar('p')][math_f_bold] = "&#x1D429;";
    global_state.math_chars[uchar('q')][math_f_bold] = "&#x1D42A;";
    global_state.math_chars[uchar('r')][math_f_bold] = "&#x1D42B;";
    global_state.math_chars[uchar('s')][math_f_bold] = "&#x1D42C;";
    global_state.math_chars[uchar('t')][math_f_bold] = "&#x1D42D;";
    global_state.math_chars[uchar('u')][math_f_bold] = "&#x1D42E;";
    global_state.math_chars[uchar('v')][math_f_bold] = "&#x1D42F;";
    global_state.math_chars[uchar('w')][math_f_bold] = "&#x1D430;";
    global_state.math_chars[uchar('x')][math_f_bold] = "&#x1D431;";
    global_state.math_chars[uchar('y')][math_f_bold] = "&#x1D432;";
    global_state.math_chars[uchar('z')][math_f_bold] = "&#x1D433;";
    // Position 3 : italic
    global_state.math_chars[uchar('A')][math_f_italic] = "&#x1D434;";
    global_state.math_chars[uchar('B')][math_f_italic] = "&#x1D435;";
    global_state.math_chars[uchar('C')][math_f_italic] = "&#x1D436;";
    global_state.math_chars[uchar('D')][math_f_italic] = "&#x1D437;";
    global_state.math_chars[uchar('E')][math_f_italic] = "&#x1D438;";
    global_state.math_chars[uchar('F')][math_f_italic] = "&#x1D439;";
    global_state.math_chars[uchar('G')][math_f_italic] = "&#x1D43A;";
    global_state.math_chars[uchar('H')][math_f_italic] = "&#x1D43B;";
    global_state.math_chars[uchar('I')][math_f_italic] = "&#x1D43C;";
    global_state.math_chars[uchar('J')][math_f_italic] = "&#x1D43D;";
    global_state.math_chars[uchar('K')][math_f_italic] = "&#x1D43E;";
    global_state.math_chars[uchar('L')][math_f_italic] = "&#x1D43F;";
    global_state.math_chars[uchar('M')][math_f_italic] = "&#x1D440;";
    global_state.math_chars[uchar('N')][math_f_italic] = "&#x1D441;";
    global_state.math_chars[uchar('O')][math_f_italic] = "&#x1D442;";
    global_state.math_chars[uchar('P')][math_f_italic] = "&#x1D443;";
    global_state.math_chars[uchar('Q')][math_f_italic] = "&#x1D444;";
    global_state.math_chars[uchar('R')][math_f_italic] = "&#x1D445;";
    global_state.math_chars[uchar('S')][math_f_italic] = "&#x1D446;";
    global_state.math_chars[uchar('T')][math_f_italic] = "&#x1D447;";
    global_state.math_chars[uchar('U')][math_f_italic] = "&#x1D448;";
    global_state.math_chars[uchar('V')][math_f_italic] = "&#x1D449;";
    global_state.math_chars[uchar('W')][math_f_italic] = "&#x1D44A;";
    global_state.math_chars[uchar('X')][math_f_italic] = "&#x1D44B;";
    global_state.math_chars[uchar('Y')][math_f_italic] = "&#x1D44C;";
    global_state.math_chars[uchar('Z')][math_f_italic] = "&#x1D44D;";
    global_state.math_chars[uchar('a')][math_f_italic] = "&#x1D44E;";
    global_state.math_chars[uchar('b')][math_f_italic] = "&#x1D44F;";
    global_state.math_chars[uchar('c')][math_f_italic] = "&#x1D450;";
    global_state.math_chars[uchar('d')][math_f_italic] = "&#x1D451;";
    global_state.math_chars[uchar('e')][math_f_italic] = "&#x1D452;";
    global_state.math_chars[uchar('f')][math_f_italic] = "&#x1D453;";
    global_state.math_chars[uchar('g')][math_f_italic] = "&#x1D454;";
    global_state.math_chars[uchar('h')][math_f_italic] = "&#x210E;"; // Plank
    global_state.math_chars[uchar('i')][math_f_italic] = "&#x1D456;";
    global_state.math_chars[uchar('j')][math_f_italic] = "&#x1D457;";
    global_state.math_chars[uchar('k')][math_f_italic] = "&#x1D458;";
    global_state.math_chars[uchar('l')][math_f_italic] = "&#x1D459;";
    global_state.math_chars[uchar('m')][math_f_italic] = "&#x1D45A;";
    global_state.math_chars[uchar('n')][math_f_italic] = "&#x1D45B;";
    global_state.math_chars[uchar('o')][math_f_italic] = "&#x1D45C;";
    global_state.math_chars[uchar('p')][math_f_italic] = "&#x1D45D;";
    global_state.math_chars[uchar('q')][math_f_italic] = "&#x1D45E;";
    global_state.math_chars[uchar('r')][math_f_italic] = "&#x1D45F;";
    global_state.math_chars[uchar('s')][math_f_italic] = "&#x1D460;";
    global_state.math_chars[uchar('t')][math_f_italic] = "&#x1D461;";
    global_state.math_chars[uchar('u')][math_f_italic] = "&#x1D462;";
    global_state.math_chars[uchar('v')][math_f_italic] = "&#x1D463;";
    global_state.math_chars[uchar('w')][math_f_italic] = "&#x1D464;";
    global_state.math_chars[uchar('x')][math_f_italic] = "&#x1D465;";
    global_state.math_chars[uchar('y')][math_f_italic] = "&#x1D466;";
    global_state.math_chars[uchar('z')][math_f_italic] = "&#x1D467;";
    // Position 4: bold-italics
    global_state.math_chars[uchar('A')][math_f_bold_italic] = "&#x1D468;";
    global_state.math_chars[uchar('B')][math_f_bold_italic] = "&#x1D469;";
    global_state.math_chars[uchar('C')][math_f_bold_italic] = "&#x1D46A;";
    global_state.math_chars[uchar('D')][math_f_bold_italic] = "&#x1D46B;";
    global_state.math_chars[uchar('E')][math_f_bold_italic] = "&#x1D46C;";
    global_state.math_chars[uchar('F')][math_f_bold_italic] = "&#x1D46D;";
    global_state.math_chars[uchar('G')][math_f_bold_italic] = "&#x1D46E;";
    global_state.math_chars[uchar('H')][math_f_bold_italic] = "&#x1D46F;";
    global_state.math_chars[uchar('I')][math_f_bold_italic] = "&#x1D470;";
    global_state.math_chars[uchar('J')][math_f_bold_italic] = "&#x1D471;";
    global_state.math_chars[uchar('K')][math_f_bold_italic] = "&#x1D472;";
    global_state.math_chars[uchar('L')][math_f_bold_italic] = "&#x1D473;";
    global_state.math_chars[uchar('M')][math_f_bold_italic] = "&#x1D474;";
    global_state.math_chars[uchar('N')][math_f_bold_italic] = "&#x1D475;";
    global_state.math_chars[uchar('O')][math_f_bold_italic] = "&#x1D476;";
    global_state.math_chars[uchar('P')][math_f_bold_italic] = "&#x1D477;";
    global_state.math_chars[uchar('Q')][math_f_bold_italic] = "&#x1D478;";
    global_state.math_chars[uchar('R')][math_f_bold_italic] = "&#x1D479;";
    global_state.math_chars[uchar('S')][math_f_bold_italic] = "&#x1D47A;";
    global_state.math_chars[uchar('T')][math_f_bold_italic] = "&#x1D47B;";
    global_state.math_chars[uchar('U')][math_f_bold_italic] = "&#x1D47C;";
    global_state.math_chars[uchar('V')][math_f_bold_italic] = "&#x1D47D;";
    global_state.math_chars[uchar('W')][math_f_bold_italic] = "&#x1D47E;";
    global_state.math_chars[uchar('X')][math_f_bold_italic] = "&#x1D47F;";
    global_state.math_chars[uchar('Y')][math_f_bold_italic] = "&#x1D480;";
    global_state.math_chars[uchar('Z')][math_f_bold_italic] = "&#x1D481;";
    global_state.math_chars[uchar('a')][math_f_bold_italic] = "&#x1D482;";
    global_state.math_chars[uchar('b')][math_f_bold_italic] = "&#x1D483;";
    global_state.math_chars[uchar('c')][math_f_bold_italic] = "&#x1D484;";
    global_state.math_chars[uchar('d')][math_f_bold_italic] = "&#x1D485;";
    global_state.math_chars[uchar('e')][math_f_bold_italic] = "&#x1D486;";
    global_state.math_chars[uchar('f')][math_f_bold_italic] = "&#x1D487;";
    global_state.math_chars[uchar('g')][math_f_bold_italic] = "&#x1D488;";
    global_state.math_chars[uchar('h')][math_f_bold_italic] = "&#x1D489;";
    global_state.math_chars[uchar('i')][math_f_bold_italic] = "&#x1D48A;";
    global_state.math_chars[uchar('j')][math_f_bold_italic] = "&#x1D48B;";
    global_state.math_chars[uchar('k')][math_f_bold_italic] = "&#x1D48C;";
    global_state.math_chars[uchar('l')][math_f_bold_italic] = "&#x1D48D;";
    global_state.math_chars[uchar('m')][math_f_bold_italic] = "&#x1D48E;";
    global_state.math_chars[uchar('n')][math_f_bold_italic] = "&#x1D48F;";
    global_state.math_chars[uchar('o')][math_f_bold_italic] = "&#x1D490;";
    global_state.math_chars[uchar('p')][math_f_bold_italic] = "&#x1D491;";
    global_state.math_chars[uchar('q')][math_f_bold_italic] = "&#x1D492;";
    global_state.math_chars[uchar('r')][math_f_bold_italic] = "&#x1D493;";
    global_state.math_chars[uchar('s')][math_f_bold_italic] = "&#x1D494;";
    global_state.math_chars[uchar('t')][math_f_bold_italic] = "&#x1D495;";
    global_state.math_chars[uchar('u')][math_f_bold_italic] = "&#x1D496;";
    global_state.math_chars[uchar('v')][math_f_bold_italic] = "&#x1D497;";
    global_state.math_chars[uchar('w')][math_f_bold_italic] = "&#x1D498;";
    global_state.math_chars[uchar('x')][math_f_bold_italic] = "&#x1D499;";
    global_state.math_chars[uchar('y')][math_f_bold_italic] = "&#x1D49A;";
    global_state.math_chars[uchar('z')][math_f_bold_italic] = "&#x1D49B;";
    // Position 5: Script
    global_state.math_chars[uchar('A')][math_f_script] = "&#x1D49C;";
    global_state.math_chars[uchar('B')][math_f_script] = "&#x212C;"; // Bernoulli
    global_state.math_chars[uchar('C')][math_f_script] = "&#x1D49E;";
    global_state.math_chars[uchar('D')][math_f_script] = "&#x1D49F;";
    global_state.math_chars[uchar('E')][math_f_script] = "&#x2130;"; // Electromotrice force
    global_state.math_chars[uchar('F')][math_f_script] = "&#x2131;"; // Fourier Transform
    global_state.math_chars[uchar('G')][math_f_script] = "&#x1D4A2;";
    global_state.math_chars[uchar('H')][math_f_script] = "&#x210B;"; // Hamiltonian
    global_state.math_chars[uchar('I')][math_f_script] = "&#x2110;";
    global_state.math_chars[uchar('J')][math_f_script] = "&#x1D4A5;";
    global_state.math_chars[uchar('K')][math_f_script] = "&#x1D4A6;";
    global_state.math_chars[uchar('L')][math_f_script] = "&#x2112;"; // Laplace
    global_state.math_chars[uchar('M')][math_f_script] = "&#x2133;"; // M-matrix
    global_state.math_chars[uchar('N')][math_f_script] = "&#x1D4A9;";
    global_state.math_chars[uchar('O')][math_f_script] = "&#x1D4AA;";
    global_state.math_chars[uchar('P')][math_f_script] = "&#x1D4AB;";
    global_state.math_chars[uchar('Q')][math_f_script] = "&#x1D4AC;";
    global_state.math_chars[uchar('R')][math_f_script] = "&#x211B;"; // Riemann Integral
    global_state.math_chars[uchar('S')][math_f_script] = "&#x1D4AE;";
    global_state.math_chars[uchar('T')][math_f_script] = "&#x1D4AF;";
    global_state.math_chars[uchar('U')][math_f_script] = "&#x1D4B0;";
    global_state.math_chars[uchar('V')][math_f_script] = "&#x1D4B1;";
    global_state.math_chars[uchar('W')][math_f_script] = "&#x1D4B2;";
    global_state.math_chars[uchar('X')][math_f_script] = "&#x1D4B3;";
    global_state.math_chars[uchar('Y')][math_f_script] = "&#x1D4B4;";
    global_state.math_chars[uchar('Z')][math_f_script] = "&#x1D4B5;";
    global_state.math_chars[uchar('a')][math_f_script] = "&#x1D4B6;";
    global_state.math_chars[uchar('b')][math_f_script] = "&#x1D4B7;";
    global_state.math_chars[uchar('c')][math_f_script] = "&#x1D4B8;";
    global_state.math_chars[uchar('d')][math_f_script] = "&#x1D4B9;";
    global_state.math_chars[uchar('e')][math_f_script] = "&#x212F;"; // error
    global_state.math_chars[uchar('f')][math_f_script] = "&#x1D4BB;";
    global_state.math_chars[uchar('g')][math_f_script] = "&#x210A;";
    global_state.math_chars[uchar('h')][math_f_script] = "&#x1D4BD;";
    global_state.math_chars[uchar('i')][math_f_script] = "&#x1D4BE;";
    global_state.math_chars[uchar('j')][math_f_script] = "&#x1D4BF;";
    global_state.math_chars[uchar('k')][math_f_script] = "&#x1D4C0;";
    global_state.math_chars[uchar('l')][math_f_script] = "&#x1D4C1;";
    global_state.math_chars[uchar('m')][math_f_script] = "&#x1D4C2;";
    global_state.math_chars[uchar('n')][math_f_script] = "&#x1D4C3;";
    global_state.math_chars[uchar('o')][math_f_script] = "&#x2134;"; // order
    global_state.math_chars[uchar('p')][math_f_script] = "&#x1D4C5;";
    global_state.math_chars[uchar('q')][math_f_script] = "&#x1D4C6;";
    global_state.math_chars[uchar('r')][math_f_script] = "&#x1D4C7;";
    global_state.math_chars[uchar('s')][math_f_script] = "&#x1D4C8;";
    global_state.math_chars[uchar('t')][math_f_script] = "&#x1D4C9;";
    global_state.math_chars[uchar('u')][math_f_script] = "&#x1D4CA;";
    global_state.math_chars[uchar('v')][math_f_script] = "&#x1D4CB;";
    global_state.math_chars[uchar('w')][math_f_script] = "&#x1D4CC;";
    global_state.math_chars[uchar('x')][math_f_script] = "&#x1D4CD;";
    global_state.math_chars[uchar('y')][math_f_script] = "&#x1D4CE;";
    global_state.math_chars[uchar('z')][math_f_script] = "&#x1D4CF;";
    // Position 6 bold script
    global_state.math_chars[uchar('A')][math_f_bold_script] = "&#x1D4D0;";
    global_state.math_chars[uchar('B')][math_f_bold_script] = "&#x1D4D1;";
    global_state.math_chars[uchar('C')][math_f_bold_script] = "&#x1D4D2;";
    global_state.math_chars[uchar('D')][math_f_bold_script] = "&#x1D4D3;";
    global_state.math_chars[uchar('E')][math_f_bold_script] = "&#x1D4D4;";
    global_state.math_chars[uchar('F')][math_f_bold_script] = "&#x1D4D5;";
    global_state.math_chars[uchar('G')][math_f_bold_script] = "&#x1D4D6;";
    global_state.math_chars[uchar('H')][math_f_bold_script] = "&#x1D4D7;";
    global_state.math_chars[uchar('I')][math_f_bold_script] = "&#x1D4D8;";
    global_state.math_chars[uchar('J')][math_f_bold_script] = "&#x1D4D9;";
    global_state.math_chars[uchar('K')][math_f_bold_script] = "&#x1D4DA;";
    global_state.math_chars[uchar('L')][math_f_bold_script] = "&#x1D4DB;";
    global_state.math_chars[uchar('M')][math_f_bold_script] = "&#x1D4DC;";
    global_state.math_chars[uchar('N')][math_f_bold_script] = "&#x1D4DD;";
    global_state.math_chars[uchar('O')][math_f_bold_script] = "&#x1D4DE;";
    global_state.math_chars[uchar('P')][math_f_bold_script] = "&#x1D4DF;";
    global_state.math_chars[uchar('Q')][math_f_bold_script] = "&#x1D4E0;";
    global_state.math_chars[uchar('R')][math_f_bold_script] = "&#x1D4E1;";
    global_state.math_chars[uchar('S')][math_f_bold_script] = "&#x1D4E2;";
    global_state.math_chars[uchar('T')][math_f_bold_script] = "&#x1D4E3;";
    global_state.math_chars[uchar('U')][math_f_bold_script] = "&#x1D4E4;";
    global_state.math_chars[uchar('V')][math_f_bold_script] = "&#x1D4E5;";
    global_state.math_chars[uchar('W')][math_f_bold_script] = "&#x1D4E6;";
    global_state.math_chars[uchar('X')][math_f_bold_script] = "&#x1D4E7;";
    global_state.math_chars[uchar('Y')][math_f_bold_script] = "&#x1D4E8;";
    global_state.math_chars[uchar('Z')][math_f_bold_script] = "&#x1D4E9;";
    global_state.math_chars[uchar('a')][math_f_bold_script] = "&#x1D4EA;";
    global_state.math_chars[uchar('b')][math_f_bold_script] = "&#x1D4EB;";
    global_state.math_chars[uchar('c')][math_f_bold_script] = "&#x1D4EC;";
    global_state.math_chars[uchar('d')][math_f_bold_script] = "&#x1D4ED;";
    global_state.math_chars[uchar('e')][math_f_bold_script] = "&#x1D4EE;";
    global_state.math_chars[uchar('f')][math_f_bold_script] = "&#x1D4EF;";
    global_state.math_chars[uchar('g')][math_f_bold_script] = "&#x1D4F0;";
    global_state.math_chars[uchar('h')][math_f_bold_script] = "&#x1D4F1;";
    global_state.math_chars[uchar('i')][math_f_bold_script] = "&#x1D4F2;";
    global_state.math_chars[uchar('j')][math_f_bold_script] = "&#x1D4F3;";
    global_state.math_chars[uchar('k')][math_f_bold_script] = "&#x1D4F4;";
    global_state.math_chars[uchar('l')][math_f_bold_script] = "&#x1D4F5;";
    global_state.math_chars[uchar('m')][math_f_bold_script] = "&#x1D4F6;";
    global_state.math_chars[uchar('n')][math_f_bold_script] = "&#x1D4F7;";
    global_state.math_chars[uchar('o')][math_f_bold_script] = "&#x1D4F8;";
    global_state.math_chars[uchar('p')][math_f_bold_script] = "&#x1D4F9;";
    global_state.math_chars[uchar('q')][math_f_bold_script] = "&#x1D4FA;";
    global_state.math_chars[uchar('r')][math_f_bold_script] = "&#x1D4FB;";
    global_state.math_chars[uchar('s')][math_f_bold_script] = "&#x1D4FC;";
    global_state.math_chars[uchar('t')][math_f_bold_script] = "&#x1D4FD;";
    global_state.math_chars[uchar('u')][math_f_bold_script] = "&#x1D4FE;";
    global_state.math_chars[uchar('v')][math_f_bold_script] = "&#x1D4FF;";
    global_state.math_chars[uchar('w')][math_f_bold_script] = "&#x1D500;";
    global_state.math_chars[uchar('x')][math_f_bold_script] = "&#x1D501;";
    global_state.math_chars[uchar('y')][math_f_bold_script] = "&#x1D502;";
    global_state.math_chars[uchar('z')][math_f_bold_script] = "&#x1D503;";
    // Position 7 Fraktur
    global_state.math_chars[uchar('A')][math_f_fraktur] = "&#x1D504;";
    global_state.math_chars[uchar('B')][math_f_fraktur] = "&#x1D505;";
    global_state.math_chars[uchar('C')][math_f_fraktur] = "&#x212D;"; // complex
    global_state.math_chars[uchar('D')][math_f_fraktur] = "&#x1D507;";
    global_state.math_chars[uchar('E')][math_f_fraktur] = "&#x1D508;";
    global_state.math_chars[uchar('F')][math_f_fraktur] = "&#x1D509;";
    global_state.math_chars[uchar('G')][math_f_fraktur] = "&#x1D50A;";
    global_state.math_chars[uchar('H')][math_f_fraktur] = "&#x210C;"; // Hilbert space
    global_state.math_chars[uchar('I')][math_f_fraktur] = "&#x2111;"; // imaginary part
    global_state.math_chars[uchar('J')][math_f_fraktur] = "&#x1D50D;";
    global_state.math_chars[uchar('K')][math_f_fraktur] = "&#x1D50E;";
    global_state.math_chars[uchar('L')][math_f_fraktur] = "&#x1D50F;";
    global_state.math_chars[uchar('M')][math_f_fraktur] = "&#x1D510;";
    global_state.math_chars[uchar('N')][math_f_fraktur] = "&#x1D511;";
    global_state.math_chars[uchar('O')][math_f_fraktur] = "&#x1D512;";
    global_state.math_chars[uchar('P')][math_f_fraktur] = "&#x1D513;";
    global_state.math_chars[uchar('Q')][math_f_fraktur] = "&#x1D514;";
    global_state.math_chars[uchar('R')][math_f_fraktur] = "&#x211C;"; // real part
    global_state.math_chars[uchar('S')][math_f_fraktur] = "&#x1D516;";
    global_state.math_chars[uchar('T')][math_f_fraktur] = "&#x1D517;";
    global_state.math_chars[uchar('U')][math_f_fraktur] = "&#x1D518;";
    global_state.math_chars[uchar('V')][math_f_fraktur] = "&#x1D519;";
    global_state.math_chars[uchar('W')][math_f_fraktur] = "&#x1D51A;";
    global_state.math_chars[uchar('X')][math_f_fraktur] = "&#x1D51B;";
    global_state.math_chars[uchar('Y')][math_f_fraktur] = "&#x1D51C;";
    global_state.math_chars[uchar('Z')][math_f_fraktur] = "&#x2128;"; // integers
    global_state.math_chars[uchar('a')][math_f_fraktur] = "&#x1D51E;";
    global_state.math_chars[uchar('b')][math_f_fraktur] = "&#x1D51F;";
    global_state.math_chars[uchar('c')][math_f_fraktur] = "&#x1D520;";
    global_state.math_chars[uchar('d')][math_f_fraktur] = "&#x1D521;";
    global_state.math_chars[uchar('e')][math_f_fraktur] = "&#x1D522;";
    global_state.math_chars[uchar('f')][math_f_fraktur] = "&#x1D523;";
    global_state.math_chars[uchar('g')][math_f_fraktur] = "&#x1D524;";
    global_state.math_chars[uchar('h')][math_f_fraktur] = "&#x1D525;";
    global_state.math_chars[uchar('i')][math_f_fraktur] = "&#x1D526;";
    global_state.math_chars[uchar('j')][math_f_fraktur] = "&#x1D527;";
    global_state.math_chars[uchar('k')][math_f_fraktur] = "&#x1D528;";
    global_state.math_chars[uchar('l')][math_f_fraktur] = "&#x1D529;";
    global_state.math_chars[uchar('m')][math_f_fraktur] = "&#x1D52A;";
    global_state.math_chars[uchar('n')][math_f_fraktur] = "&#x1D52B;";
    global_state.math_chars[uchar('o')][math_f_fraktur] = "&#x1D52C;";
    global_state.math_chars[uchar('p')][math_f_fraktur] = "&#x1D52D;";
    global_state.math_chars[uchar('q')][math_f_fraktur] = "&#x1D52E;";
    global_state.math_chars[uchar('r')][math_f_fraktur] = "&#x1D52F;";
    global_state.math_chars[uchar('s')][math_f_fraktur] = "&#x1D530;";
    global_state.math_chars[uchar('t')][math_f_fraktur] = "&#x1D531;";
    global_state.math_chars[uchar('u')][math_f_fraktur] = "&#x1D532;";
    global_state.math_chars[uchar('v')][math_f_fraktur] = "&#x1D533;";
    global_state.math_chars[uchar('w')][math_f_fraktur] = "&#x1D534;";
    global_state.math_chars[uchar('x')][math_f_fraktur] = "&#x1D535;";
    global_state.math_chars[uchar('y')][math_f_fraktur] = "&#x1D536;";
    global_state.math_chars[uchar('z')][math_f_fraktur] = "&#x1D537;";
    // Position  8 double struck
    global_state.math_chars[uchar('A')][math_f_doublestruck] = "&#x1D538;";
    global_state.math_chars[uchar('B')][math_f_doublestruck] = "&#x1D539;";
    global_state.math_chars[uchar('C')][math_f_doublestruck] = "&#x2102;"; // complex numbers
    global_state.math_chars[uchar('D')][math_f_doublestruck] = "&#x1D53B;";
    global_state.math_chars[uchar('E')][math_f_doublestruck] = "&#x1D53C;";
    global_state.math_chars[uchar('F')][math_f_doublestruck] = "&#x1D53D;";
    global_state.math_chars[uchar('G')][math_f_doublestruck] = "&#x1D53E;";
    global_state.math_chars[uchar('H')][math_f_doublestruck] = "&#x210D;"; // quaternions
    global_state.math_chars[uchar('I')][math_f_doublestruck] = "&#x1D540;";
    global_state.math_chars[uchar('J')][math_f_doublestruck] = "&#x1D541;";
    global_state.math_chars[uchar('K')][math_f_doublestruck] = "&#x1D542;";
    global_state.math_chars[uchar('L')][math_f_doublestruck] = "&#x1D543;";
    global_state.math_chars[uchar('M')][math_f_doublestruck] = "&#x1D544;";
    global_state.math_chars[uchar('N')][math_f_doublestruck] = "&#x2115;"; // natural numbers
    global_state.math_chars[uchar('O')][math_f_doublestruck] = "&#x1D546;";
    global_state.math_chars[uchar('P')][math_f_doublestruck] = "&#x2119;"; // projective
    global_state.math_chars[uchar('Q')][math_f_doublestruck] = "&#x211A;"; // rational numbers
    global_state.math_chars[uchar('R')][math_f_doublestruck] = "&#x211D;"; // real numbers
    global_state.math_chars[uchar('S')][math_f_doublestruck] = "&#x1D54A;";
    global_state.math_chars[uchar('T')][math_f_doublestruck] = "&#x1D54B;";
    global_state.math_chars[uchar('U')][math_f_doublestruck] = "&#x1D54C;";
    global_state.math_chars[uchar('V')][math_f_doublestruck] = "&#x1D54D;";
    global_state.math_chars[uchar('W')][math_f_doublestruck] = "&#x1D54E;";
    global_state.math_chars[uchar('X')][math_f_doublestruck] = "&#x1D54F;";
    global_state.math_chars[uchar('Y')][math_f_doublestruck] = "&#x1D550;";
    global_state.math_chars[uchar('Z')][math_f_doublestruck] = "&#x2124;"; // integers
    global_state.math_chars[uchar('a')][math_f_doublestruck] = "&#x1D552;";
    global_state.math_chars[uchar('b')][math_f_doublestruck] = "&#x1D553;";
    global_state.math_chars[uchar('c')][math_f_doublestruck] = "&#x1D554;";
    global_state.math_chars[uchar('d')][math_f_doublestruck] = "&#x1D555;";
    global_state.math_chars[uchar('e')][math_f_doublestruck] = "&#x1D556;";
    global_state.math_chars[uchar('f')][math_f_doublestruck] = "&#x1D557;";
    global_state.math_chars[uchar('g')][math_f_doublestruck] = "&#x1D558;";
    global_state.math_chars[uchar('h')][math_f_doublestruck] = "&#x1D559;";
    global_state.math_chars[uchar('i')][math_f_doublestruck] = "&#x1D55A;";
    global_state.math_chars[uchar('j')][math_f_doublestruck] = "&#x1D55B;";
    global_state.math_chars[uchar('k')][math_f_doublestruck] = "&#x1D55C;";
    global_state.math_chars[uchar('l')][math_f_doublestruck] = "&#x1D55D;";
    global_state.math_chars[uchar('m')][math_f_doublestruck] = "&#x1D55E;";
    global_state.math_chars[uchar('n')][math_f_doublestruck] = "&#x1D55F;";
    global_state.math_chars[uchar('o')][math_f_doublestruck] = "&#x1D560;";
    global_state.math_chars[uchar('p')][math_f_doublestruck] = "&#x1D561;";
    global_state.math_chars[uchar('q')][math_f_doublestruck] = "&#x1D562;";
    global_state.math_chars[uchar('r')][math_f_doublestruck] = "&#x1D563;";
    global_state.math_chars[uchar('s')][math_f_doublestruck] = "&#x1D564;";
    global_state.math_chars[uchar('t')][math_f_doublestruck] = "&#x1D565;";
    global_state.math_chars[uchar('u')][math_f_doublestruck] = "&#x1D566;";
    global_state.math_chars[uchar('v')][math_f_doublestruck] = "&#x1D567;";
    global_state.math_chars[uchar('w')][math_f_doublestruck] = "&#x1D568;";
    global_state.math_chars[uchar('x')][math_f_doublestruck] = "&#x1D569;";
    global_state.math_chars[uchar('y')][math_f_doublestruck] = "&#x1D56A;";
    global_state.math_chars[uchar('z')][math_f_doublestruck] = "&#x1D56B;";
    // Position 9 bold-fraktur
    global_state.math_chars[uchar('A')][math_f_bold_fraktur] = "&#x1D56C;";
    global_state.math_chars[uchar('B')][math_f_bold_fraktur] = "&#x1D56D;";
    global_state.math_chars[uchar('C')][math_f_bold_fraktur] = "&#x1D56E;";
    global_state.math_chars[uchar('D')][math_f_bold_fraktur] = "&#x1D56F;";
    global_state.math_chars[uchar('E')][math_f_bold_fraktur] = "&#x1D570;";
    global_state.math_chars[uchar('F')][math_f_bold_fraktur] = "&#x1D571;";
    global_state.math_chars[uchar('G')][math_f_bold_fraktur] = "&#x1D572;";
    global_state.math_chars[uchar('H')][math_f_bold_fraktur] = "&#x1D573;";
    global_state.math_chars[uchar('I')][math_f_bold_fraktur] = "&#x1D574;";
    global_state.math_chars[uchar('J')][math_f_bold_fraktur] = "&#x1D575;";
    global_state.math_chars[uchar('K')][math_f_bold_fraktur] = "&#x1D576;";
    global_state.math_chars[uchar('L')][math_f_bold_fraktur] = "&#x1D577;";
    global_state.math_chars[uchar('M')][math_f_bold_fraktur] = "&#x1D578;";
    global_state.math_chars[uchar('N')][math_f_bold_fraktur] = "&#x1D579;";
    global_state.math_chars[uchar('O')][math_f_bold_fraktur] = "&#x1D57A;";
    global_state.math_chars[uchar('P')][math_f_bold_fraktur] = "&#x1D57B;";
    global_state.math_chars[uchar('Q')][math_f_bold_fraktur] = "&#x1D57C;";
    global_state.math_chars[uchar('R')][math_f_bold_fraktur] = "&#x1D57D;";
    global_state.math_chars[uchar('S')][math_f_bold_fraktur] = "&#x1D57E;";
    global_state.math_chars[uchar('T')][math_f_bold_fraktur] = "&#x1D57F;";
    global_state.math_chars[uchar('U')][math_f_bold_fraktur] = "&#x1D580;";
    global_state.math_chars[uchar('V')][math_f_bold_fraktur] = "&#x1D581;";
    global_state.math_chars[uchar('W')][math_f_bold_fraktur] = "&#x1D582;";
    global_state.math_chars[uchar('X')][math_f_bold_fraktur] = "&#x1D583;";
    global_state.math_chars[uchar('Y')][math_f_bold_fraktur] = "&#x1D584;";
    global_state.math_chars[uchar('Z')][math_f_bold_fraktur] = "&#x1D585;";
    global_state.math_chars[uchar('a')][math_f_bold_fraktur] = "&#x1D586;";
    global_state.math_chars[uchar('b')][math_f_bold_fraktur] = "&#x1D587;";
    global_state.math_chars[uchar('c')][math_f_bold_fraktur] = "&#x1D588;";
    global_state.math_chars[uchar('d')][math_f_bold_fraktur] = "&#x1D589;";
    global_state.math_chars[uchar('e')][math_f_bold_fraktur] = "&#x1D58A;";
    global_state.math_chars[uchar('f')][math_f_bold_fraktur] = "&#x1D58B;";
    global_state.math_chars[uchar('g')][math_f_bold_fraktur] = "&#x1D58C;";
    global_state.math_chars[uchar('h')][math_f_bold_fraktur] = "&#x1D58D;";
    global_state.math_chars[uchar('i')][math_f_bold_fraktur] = "&#x1D58E;";
    global_state.math_chars[uchar('j')][math_f_bold_fraktur] = "&#x1D58F;";
    global_state.math_chars[uchar('k')][math_f_bold_fraktur] = "&#x1D590;";
    global_state.math_chars[uchar('l')][math_f_bold_fraktur] = "&#x1D591;";
    global_state.math_chars[uchar('m')][math_f_bold_fraktur] = "&#x1D592;";
    global_state.math_chars[uchar('n')][math_f_bold_fraktur] = "&#x1D593;";
    global_state.math_chars[uchar('o')][math_f_bold_fraktur] = "&#x1D594;";
    global_state.math_chars[uchar('p')][math_f_bold_fraktur] = "&#x1D595;";
    global_state.math_chars[uchar('q')][math_f_bold_fraktur] = "&#x1D596;";
    global_state.math_chars[uchar('r')][math_f_bold_fraktur] = "&#x1D597;";
    global_state.math_chars[uchar('s')][math_f_bold_fraktur] = "&#x1D598;";
    global_state.math_chars[uchar('t')][math_f_bold_fraktur] = "&#x1D599;";
    global_state.math_chars[uchar('u')][math_f_bold_fraktur] = "&#x1D59A;";
    global_state.math_chars[uchar('v')][math_f_bold_fraktur] = "&#x1D59B;";
    global_state.math_chars[uchar('w')][math_f_bold_fraktur] = "&#x1D59C;";
    global_state.math_chars[uchar('x')][math_f_bold_fraktur] = "&#x1D59D;";
    global_state.math_chars[uchar('y')][math_f_bold_fraktur] = "&#x1D59E;";
    global_state.math_chars[uchar('z')][math_f_bold_fraktur] = "&#x1D59F;";
    // Position 10 sans serif
    global_state.math_chars[uchar('A')][math_f_sansserif] = "&#x1D5A0;";
    global_state.math_chars[uchar('B')][math_f_sansserif] = "&#x1D5A1;";
    global_state.math_chars[uchar('C')][math_f_sansserif] = "&#x1D5A2;";
    global_state.math_chars[uchar('D')][math_f_sansserif] = "&#x1D5A3;";
    global_state.math_chars[uchar('E')][math_f_sansserif] = "&#x1D5A4;";
    global_state.math_chars[uchar('F')][math_f_sansserif] = "&#x1D5A5;";
    global_state.math_chars[uchar('G')][math_f_sansserif] = "&#x1D5A6;";
    global_state.math_chars[uchar('H')][math_f_sansserif] = "&#x1D5A7;";
    global_state.math_chars[uchar('I')][math_f_sansserif] = "&#x1D5A8;";
    global_state.math_chars[uchar('J')][math_f_sansserif] = "&#x1D5A9;";
    global_state.math_chars[uchar('K')][math_f_sansserif] = "&#x1D5AA;";
    global_state.math_chars[uchar('L')][math_f_sansserif] = "&#x1D5AB;";
    global_state.math_chars[uchar('M')][math_f_sansserif] = "&#x1D5AC;";
    global_state.math_chars[uchar('N')][math_f_sansserif] = "&#x1D5AD;";
    global_state.math_chars[uchar('O')][math_f_sansserif] = "&#x1D5AE;";
    global_state.math_chars[uchar('P')][math_f_sansserif] = "&#x1D5AF;";
    global_state.math_chars[uchar('Q')][math_f_sansserif] = "&#x1D5B0;";
    global_state.math_chars[uchar('R')][math_f_sansserif] = "&#x1D5B1;";
    global_state.math_chars[uchar('S')][math_f_sansserif] = "&#x1D5B2;";
    global_state.math_chars[uchar('T')][math_f_sansserif] = "&#x1D5B3;";
    global_state.math_chars[uchar('U')][math_f_sansserif] = "&#x1D5B4;";
    global_state.math_chars[uchar('V')][math_f_sansserif] = "&#x1D5B5;";
    global_state.math_chars[uchar('W')][math_f_sansserif] = "&#x1D5B6;";
    global_state.math_chars[uchar('X')][math_f_sansserif] = "&#x1D5B7;";
    global_state.math_chars[uchar('Y')][math_f_sansserif] = "&#x1D5B8;";
    global_state.math_chars[uchar('Z')][math_f_sansserif] = "&#x1D5B9;";
    global_state.math_chars[uchar('a')][math_f_sansserif] = "&#x1D5BA;";
    global_state.math_chars[uchar('b')][math_f_sansserif] = "&#x1D5BB;";
    global_state.math_chars[uchar('c')][math_f_sansserif] = "&#x1D5BC;";
    global_state.math_chars[uchar('d')][math_f_sansserif] = "&#x1D5BD;";
    global_state.math_chars[uchar('e')][math_f_sansserif] = "&#x1D5BE;";
    global_state.math_chars[uchar('f')][math_f_sansserif] = "&#x1D5BF;";
    global_state.math_chars[uchar('g')][math_f_sansserif] = "&#x1D5C0;";
    global_state.math_chars[uchar('h')][math_f_sansserif] = "&#x1D5C1;";
    global_state.math_chars[uchar('i')][math_f_sansserif] = "&#x1D5C2;";
    global_state.math_chars[uchar('j')][math_f_sansserif] = "&#x1D5C3;";
    global_state.math_chars[uchar('k')][math_f_sansserif] = "&#x1D5C4;";
    global_state.math_chars[uchar('l')][math_f_sansserif] = "&#x1D5C5;";
    global_state.math_chars[uchar('m')][math_f_sansserif] = "&#x1D5C6;";
    global_state.math_chars[uchar('n')][math_f_sansserif] = "&#x1D5C7;";
    global_state.math_chars[uchar('o')][math_f_sansserif] = "&#x1D5C8;";
    global_state.math_chars[uchar('p')][math_f_sansserif] = "&#x1D5C9;";
    global_state.math_chars[uchar('q')][math_f_sansserif] = "&#x1D5CA;";
    global_state.math_chars[uchar('r')][math_f_sansserif] = "&#x1D5CB;";
    global_state.math_chars[uchar('s')][math_f_sansserif] = "&#x1D5CC;";
    global_state.math_chars[uchar('t')][math_f_sansserif] = "&#x1D5CD;";
    global_state.math_chars[uchar('u')][math_f_sansserif] = "&#x1D5CE;";
    global_state.math_chars[uchar('v')][math_f_sansserif] = "&#x1D5CF;";
    global_state.math_chars[uchar('w')][math_f_sansserif] = "&#x1D5D0;";
    global_state.math_chars[uchar('x')][math_f_sansserif] = "&#x1D5D1;";
    global_state.math_chars[uchar('y')][math_f_sansserif] = "&#x1D5D2;";
    global_state.math_chars[uchar('z')][math_f_sansserif] = "&#x1D5D3;";
    // Position 11 bold sans serif
    global_state.math_chars[uchar('A')][math_f_bold_sansserif] = "&#x1D5D4;";
    global_state.math_chars[uchar('B')][math_f_bold_sansserif] = "&#x1D5D5;";
    global_state.math_chars[uchar('C')][math_f_bold_sansserif] = "&#x1D5D6;";
    global_state.math_chars[uchar('D')][math_f_bold_sansserif] = "&#x1D5D7;";
    global_state.math_chars[uchar('E')][math_f_bold_sansserif] = "&#x1D5D8;";
    global_state.math_chars[uchar('F')][math_f_bold_sansserif] = "&#x1D5D9;";
    global_state.math_chars[uchar('G')][math_f_bold_sansserif] = "&#x1D5DA;";
    global_state.math_chars[uchar('H')][math_f_bold_sansserif] = "&#x1D5DB;";
    global_state.math_chars[uchar('I')][math_f_bold_sansserif] = "&#x1D5DC;";
    global_state.math_chars[uchar('J')][math_f_bold_sansserif] = "&#x1D5DD;";
    global_state.math_chars[uchar('K')][math_f_bold_sansserif] = "&#x1D5DE;";
    global_state.math_chars[uchar('L')][math_f_bold_sansserif] = "&#x1D5DF;";
    global_state.math_chars[uchar('M')][math_f_bold_sansserif] = "&#x1D5E0;";
    global_state.math_chars[uchar('N')][math_f_bold_sansserif] = "&#x1D5E1;";
    global_state.math_chars[uchar('O')][math_f_bold_sansserif] = "&#x1D5E2;";
    global_state.math_chars[uchar('P')][math_f_bold_sansserif] = "&#x1D5E3;";
    global_state.math_chars[uchar('Q')][math_f_bold_sansserif] = "&#x1D5E4;";
    global_state.math_chars[uchar('R')][math_f_bold_sansserif] = "&#x1D5E5;";
    global_state.math_chars[uchar('S')][math_f_bold_sansserif] = "&#x1D5E6;";
    global_state.math_chars[uchar('T')][math_f_bold_sansserif] = "&#x1D5E7;";
    global_state.math_chars[uchar('U')][math_f_bold_sansserif] = "&#x1D5E8;";
    global_state.math_chars[uchar('V')][math_f_bold_sansserif] = "&#x1D5E9;";
    global_state.math_chars[uchar('W')][math_f_bold_sansserif] = "&#x1D5EA;";
    global_state.math_chars[uchar('X')][math_f_bold_sansserif] = "&#x1D5EB;";
    global_state.math_chars[uchar('Y')][math_f_bold_sansserif] = "&#x1D5EC;";
    global_state.math_chars[uchar('Z')][math_f_bold_sansserif] = "&#x1D5ED;";
    global_state.math_chars[uchar('a')][math_f_bold_sansserif] = "&#x1D5EE;";
    global_state.math_chars[uchar('b')][math_f_bold_sansserif] = "&#x1D5EF;";
    global_state.math_chars[uchar('c')][math_f_bold_sansserif] = "&#x1D5F0;";
    global_state.math_chars[uchar('d')][math_f_bold_sansserif] = "&#x1D5F1;";
    global_state.math_chars[uchar('e')][math_f_bold_sansserif] = "&#x1D5F2;";
    global_state.math_chars[uchar('f')][math_f_bold_sansserif] = "&#x1D5F3;";
    global_state.math_chars[uchar('g')][math_f_bold_sansserif] = "&#x1D5F4;";
    global_state.math_chars[uchar('h')][math_f_bold_sansserif] = "&#x1D5F5;";
    global_state.math_chars[uchar('i')][math_f_bold_sansserif] = "&#x1D5F6;";
    global_state.math_chars[uchar('j')][math_f_bold_sansserif] = "&#x1D5F7;";
    global_state.math_chars[uchar('k')][math_f_bold_sansserif] = "&#x1D5F8;";
    global_state.math_chars[uchar('l')][math_f_bold_sansserif] = "&#x1D5F9;";
    global_state.math_chars[uchar('m')][math_f_bold_sansserif] = "&#x1D5FA;";
    global_state.math_chars[uchar('n')][math_f_bold_sansserif] = "&#x1D5FB;";
    global_state.math_chars[uchar('o')][math_f_bold_sansserif] = "&#x1D5FC;";
    global_state.math_chars[uchar('p')][math_f_bold_sansserif] = "&#x1D5FD;";
    global_state.math_chars[uchar('q')][math_f_bold_sansserif] = "&#x1D5FE;";
    global_state.math_chars[uchar('r')][math_f_bold_sansserif] = "&#x1D5FF;";
    global_state.math_chars[uchar('s')][math_f_bold_sansserif] = "&#x1D600;";
    global_state.math_chars[uchar('t')][math_f_bold_sansserif] = "&#x1D601;";
    global_state.math_chars[uchar('u')][math_f_bold_sansserif] = "&#x1D602;";
    global_state.math_chars[uchar('v')][math_f_bold_sansserif] = "&#x1D603;";
    global_state.math_chars[uchar('w')][math_f_bold_sansserif] = "&#x1D604;";
    global_state.math_chars[uchar('x')][math_f_bold_sansserif] = "&#x1D605;";
    global_state.math_chars[uchar('y')][math_f_bold_sansserif] = "&#x1D606;";
    global_state.math_chars[uchar('z')][math_f_bold_sansserif] = "&#x1D607;";
    // Position 12 sans serif italics
    global_state.math_chars[uchar('A')][math_f_sansserif_italic] = "&#x1D608;";
    global_state.math_chars[uchar('B')][math_f_sansserif_italic] = "&#x1D609;";
    global_state.math_chars[uchar('C')][math_f_sansserif_italic] = "&#x1D60A;";
    global_state.math_chars[uchar('D')][math_f_sansserif_italic] = "&#x1D60B;";
    global_state.math_chars[uchar('E')][math_f_sansserif_italic] = "&#x1D60C;";
    global_state.math_chars[uchar('F')][math_f_sansserif_italic] = "&#x1D60D;";
    global_state.math_chars[uchar('G')][math_f_sansserif_italic] = "&#x1D60E;";
    global_state.math_chars[uchar('H')][math_f_sansserif_italic] = "&#x1D60F;";
    global_state.math_chars[uchar('I')][math_f_sansserif_italic] = "&#x1D610;";
    global_state.math_chars[uchar('J')][math_f_sansserif_italic] = "&#x1D611;";
    global_state.math_chars[uchar('K')][math_f_sansserif_italic] = "&#x1D612;";
    global_state.math_chars[uchar('L')][math_f_sansserif_italic] = "&#x1D613;";
    global_state.math_chars[uchar('M')][math_f_sansserif_italic] = "&#x1D614;";
    global_state.math_chars[uchar('N')][math_f_sansserif_italic] = "&#x1D615;";
    global_state.math_chars[uchar('O')][math_f_sansserif_italic] = "&#x1D616;";
    global_state.math_chars[uchar('P')][math_f_sansserif_italic] = "&#x1D617;";
    global_state.math_chars[uchar('Q')][math_f_sansserif_italic] = "&#x1D618;";
    global_state.math_chars[uchar('R')][math_f_sansserif_italic] = "&#x1D619;";
    global_state.math_chars[uchar('S')][math_f_sansserif_italic] = "&#x1D61A;";
    global_state.math_chars[uchar('T')][math_f_sansserif_italic] = "&#x1D61B;";
    global_state.math_chars[uchar('U')][math_f_sansserif_italic] = "&#x1D61C;";
    global_state.math_chars[uchar('V')][math_f_sansserif_italic] = "&#x1D61D;";
    global_state.math_chars[uchar('W')][math_f_sansserif_italic] = "&#x1D61E;";
    global_state.math_chars[uchar('X')][math_f_sansserif_italic] = "&#x1D61F;";
    global_state.math_chars[uchar('Y')][math_f_sansserif_italic] = "&#x1D620;";
    global_state.math_chars[uchar('Z')][math_f_sansserif_italic] = "&#x1D621;";
    global_state.math_chars[uchar('a')][math_f_sansserif_italic] = "&#x1D622;";
    global_state.math_chars[uchar('b')][math_f_sansserif_italic] = "&#x1D623;";
    global_state.math_chars[uchar('c')][math_f_sansserif_italic] = "&#x1D624;";
    global_state.math_chars[uchar('d')][math_f_sansserif_italic] = "&#x1D625;";
    global_state.math_chars[uchar('e')][math_f_sansserif_italic] = "&#x1D626;";
    global_state.math_chars[uchar('f')][math_f_sansserif_italic] = "&#x1D627;";
    global_state.math_chars[uchar('g')][math_f_sansserif_italic] = "&#x1D628;";
    global_state.math_chars[uchar('h')][math_f_sansserif_italic] = "&#x1D629;";
    global_state.math_chars[uchar('i')][math_f_sansserif_italic] = "&#x1D62A;";
    global_state.math_chars[uchar('j')][math_f_sansserif_italic] = "&#x1D62B;";
    global_state.math_chars[uchar('k')][math_f_sansserif_italic] = "&#x1D62C;";
    global_state.math_chars[uchar('l')][math_f_sansserif_italic] = "&#x1D62D;";
    global_state.math_chars[uchar('m')][math_f_sansserif_italic] = "&#x1D62E;";
    global_state.math_chars[uchar('n')][math_f_sansserif_italic] = "&#x1D62F;";
    global_state.math_chars[uchar('o')][math_f_sansserif_italic] = "&#x1D630;";
    global_state.math_chars[uchar('p')][math_f_sansserif_italic] = "&#x1D631;";
    global_state.math_chars[uchar('q')][math_f_sansserif_italic] = "&#x1D632;";
    global_state.math_chars[uchar('r')][math_f_sansserif_italic] = "&#x1D633;";
    global_state.math_chars[uchar('s')][math_f_sansserif_italic] = "&#x1D634;";
    global_state.math_chars[uchar('t')][math_f_sansserif_italic] = "&#x1D635;";
    global_state.math_chars[uchar('u')][math_f_sansserif_italic] = "&#x1D636;";
    global_state.math_chars[uchar('v')][math_f_sansserif_italic] = "&#x1D637;";
    global_state.math_chars[uchar('w')][math_f_sansserif_italic] = "&#x1D638;";
    global_state.math_chars[uchar('x')][math_f_sansserif_italic] = "&#x1D639;";
    global_state.math_chars[uchar('y')][math_f_sansserif_italic] = "&#x1D63A;";
    global_state.math_chars[uchar('z')][math_f_sansserif_italic] = "&#x1D63B;";
    // Position 13 sans serif bold italics
    global_state.math_chars[uchar('A')][math_f_sansserif_bold_italic] = "&#x1D63C;";
    global_state.math_chars[uchar('B')][math_f_sansserif_bold_italic] = "&#x1D63D;";
    global_state.math_chars[uchar('C')][math_f_sansserif_bold_italic] = "&#x1D63E;";
    global_state.math_chars[uchar('D')][math_f_sansserif_bold_italic] = "&#x1D63F;";
    global_state.math_chars[uchar('E')][math_f_sansserif_bold_italic] = "&#x1D640;";
    global_state.math_chars[uchar('F')][math_f_sansserif_bold_italic] = "&#x1D641;";
    global_state.math_chars[uchar('G')][math_f_sansserif_bold_italic] = "&#x1D642;";
    global_state.math_chars[uchar('H')][math_f_sansserif_bold_italic] = "&#x1D643;";
    global_state.math_chars[uchar('I')][math_f_sansserif_bold_italic] = "&#x1D644;";
    global_state.math_chars[uchar('J')][math_f_sansserif_bold_italic] = "&#x1D645;";
    global_state.math_chars[uchar('K')][math_f_sansserif_bold_italic] = "&#x1D646;";
    global_state.math_chars[uchar('L')][math_f_sansserif_bold_italic] = "&#x1D647;";
    global_state.math_chars[uchar('M')][math_f_sansserif_bold_italic] = "&#x1D648;";
    global_state.math_chars[uchar('N')][math_f_sansserif_bold_italic] = "&#x1D649;";
    global_state.math_chars[uchar('O')][math_f_sansserif_bold_italic] = "&#x1D64A;";
    global_state.math_chars[uchar('P')][math_f_sansserif_bold_italic] = "&#x1D64B;";
    global_state.math_chars[uchar('Q')][math_f_sansserif_bold_italic] = "&#x1D64C;";
    global_state.math_chars[uchar('R')][math_f_sansserif_bold_italic] = "&#x1D64D;";
    global_state.math_chars[uchar('S')][math_f_sansserif_bold_italic] = "&#x1D64E;";
    global_state.math_chars[uchar('T')][math_f_sansserif_bold_italic] = "&#x1D64F;";
    global_state.math_chars[uchar('U')][math_f_sansserif_bold_italic] = "&#x1D650;";
    global_state.math_chars[uchar('V')][math_f_sansserif_bold_italic] = "&#x1D651;";
    global_state.math_chars[uchar('W')][math_f_sansserif_bold_italic] = "&#x1D652;";
    global_state.math_chars[uchar('X')][math_f_sansserif_bold_italic] = "&#x1D653;";
    global_state.math_chars[uchar('Y')][math_f_sansserif_bold_italic] = "&#x1D654;";
    global_state.math_chars[uchar('Z')][math_f_sansserif_bold_italic] = "&#x1D655;";
    global_state.math_chars[uchar('a')][math_f_sansserif_bold_italic] = "&#x1D656;";
    global_state.math_chars[uchar('b')][math_f_sansserif_bold_italic] = "&#x1D657;";
    global_state.math_chars[uchar('c')][math_f_sansserif_bold_italic] = "&#x1D658;";
    global_state.math_chars[uchar('d')][math_f_sansserif_bold_italic] = "&#x1D659;";
    global_state.math_chars[uchar('e')][math_f_sansserif_bold_italic] = "&#x1D65A;";
    global_state.math_chars[uchar('f')][math_f_sansserif_bold_italic] = "&#x1D65B;";
    global_state.math_chars[uchar('g')][math_f_sansserif_bold_italic] = "&#x1D65C;";
    global_state.math_chars[uchar('h')][math_f_sansserif_bold_italic] = "&#x1D65D;";
    global_state.math_chars[uchar('i')][math_f_sansserif_bold_italic] = "&#x1D65E;";
    global_state.math_chars[uchar('j')][math_f_sansserif_bold_italic] = "&#x1D65F;";
    global_state.math_chars[uchar('k')][math_f_sansserif_bold_italic] = "&#x1D660;";
    global_state.math_chars[uchar('l')][math_f_sansserif_bold_italic] = "&#x1D661;";
    global_state.math_chars[uchar('m')][math_f_sansserif_bold_italic] = "&#x1D662;";
    global_state.math_chars[uchar('n')][math_f_sansserif_bold_italic] = "&#x1D663;";
    global_state.math_chars[uchar('o')][math_f_sansserif_bold_italic] = "&#x1D664;";
    global_state.math_chars[uchar('p')][math_f_sansserif_bold_italic] = "&#x1D665;";
    global_state.math_chars[uchar('q')][math_f_sansserif_bold_italic] = "&#x1D666;";
    global_state.math_chars[uchar('r')][math_f_sansserif_bold_italic] = "&#x1D667;";
    global_state.math_chars[uchar('s')][math_f_sansserif_bold_italic] = "&#x1D668;";
    global_state.math_chars[uchar('t')][math_f_sansserif_bold_italic] = "&#x1D669;";
    global_state.math_chars[uchar('u')][math_f_sansserif_bold_italic] = "&#x1D66A;";
    global_state.math_chars[uchar('v')][math_f_sansserif_bold_italic] = "&#x1D66B;";
    global_state.math_chars[uchar('w')][math_f_sansserif_bold_italic] = "&#x1D66C;";
    global_state.math_chars[uchar('x')][math_f_sansserif_bold_italic] = "&#x1D66D;";
    global_state.math_chars[uchar('y')][math_f_sansserif_bold_italic] = "&#x1D66E;";
    global_state.math_chars[uchar('z')][math_f_sansserif_bold_italic] = "&#x1D66F;";
    // Position 14 : monospace
    global_state.math_chars[uchar('A')][math_f_monospace] = "&#x1D670;";
    global_state.math_chars[uchar('B')][math_f_monospace] = "&#x1D671;";
    global_state.math_chars[uchar('C')][math_f_monospace] = "&#x1D672;";
    global_state.math_chars[uchar('D')][math_f_monospace] = "&#x1D673;";
    global_state.math_chars[uchar('E')][math_f_monospace] = "&#x1D674;";
    global_state.math_chars[uchar('F')][math_f_monospace] = "&#x1D675;";
    global_state.math_chars[uchar('G')][math_f_monospace] = "&#x1D676;";
    global_state.math_chars[uchar('H')][math_f_monospace] = "&#x1D677;";
    global_state.math_chars[uchar('I')][math_f_monospace] = "&#x1D678;";
    global_state.math_chars[uchar('J')][math_f_monospace] = "&#x1D679;";
    global_state.math_chars[uchar('K')][math_f_monospace] = "&#x1D67A;";
    global_state.math_chars[uchar('L')][math_f_monospace] = "&#x1D67B;";
    global_state.math_chars[uchar('M')][math_f_monospace] = "&#x1D67C;";
    global_state.math_chars[uchar('N')][math_f_monospace] = "&#x1D67D;";
    global_state.math_chars[uchar('O')][math_f_monospace] = "&#x1D67E;";
    global_state.math_chars[uchar('P')][math_f_monospace] = "&#x1D67F;";
    global_state.math_chars[uchar('Q')][math_f_monospace] = "&#x1D680;";
    global_state.math_chars[uchar('R')][math_f_monospace] = "&#x1D681;";
    global_state.math_chars[uchar('S')][math_f_monospace] = "&#x1D682;";
    global_state.math_chars[uchar('T')][math_f_monospace] = "&#x1D683;";
    global_state.math_chars[uchar('U')][math_f_monospace] = "&#x1D684;";
    global_state.math_chars[uchar('V')][math_f_monospace] = "&#x1D685;";
    global_state.math_chars[uchar('W')][math_f_monospace] = "&#x1D686;";
    global_state.math_chars[uchar('X')][math_f_monospace] = "&#x1D687;";
    global_state.math_chars[uchar('Y')][math_f_monospace] = "&#x1D688;";
    global_state.math_chars[uchar('Z')][math_f_monospace] = "&#x1D689;";
    global_state.math_chars[uchar('a')][math_f_monospace] = "&#x1D68A;";
    global_state.math_chars[uchar('b')][math_f_monospace] = "&#x1D68B;";
    global_state.math_chars[uchar('c')][math_f_monospace] = "&#x1D68C;";
    global_state.math_chars[uchar('d')][math_f_monospace] = "&#x1D68D;";
    global_state.math_chars[uchar('e')][math_f_monospace] = "&#x1D68E;";
    global_state.math_chars[uchar('f')][math_f_monospace] = "&#x1D68F;";
    global_state.math_chars[uchar('g')][math_f_monospace] = "&#x1D690;";
    global_state.math_chars[uchar('h')][math_f_monospace] = "&#x1D691;";
    global_state.math_chars[uchar('i')][math_f_monospace] = "&#x1D692;";
    global_state.math_chars[uchar('j')][math_f_monospace] = "&#x1D693;";
    global_state.math_chars[uchar('k')][math_f_monospace] = "&#x1D694;";
    global_state.math_chars[uchar('l')][math_f_monospace] = "&#x1D695;";
    global_state.math_chars[uchar('m')][math_f_monospace] = "&#x1D696;";
    global_state.math_chars[uchar('n')][math_f_monospace] = "&#x1D697;";
    global_state.math_chars[uchar('o')][math_f_monospace] = "&#x1D698;";
    global_state.math_chars[uchar('p')][math_f_monospace] = "&#x1D699;";
    global_state.math_chars[uchar('q')][math_f_monospace] = "&#x1D69A;";
    global_state.math_chars[uchar('r')][math_f_monospace] = "&#x1D69B;";
    global_state.math_chars[uchar('s')][math_f_monospace] = "&#x1D69C;";
    global_state.math_chars[uchar('t')][math_f_monospace] = "&#x1D69D;";
    global_state.math_chars[uchar('u')][math_f_monospace] = "&#x1D69E;";
    global_state.math_chars[uchar('v')][math_f_monospace] = "&#x1D69F;";
    global_state.math_chars[uchar('w')][math_f_monospace] = "&#x1D6A0;";
    global_state.math_chars[uchar('x')][math_f_monospace] = "&#x1D6A1;";
    global_state.math_chars[uchar('y')][math_f_monospace] = "&#x1D6A2;";
    global_state.math_chars[uchar('z')][math_f_monospace] = "&#x1D6A3;";
}

// Converts foo into <mspace width='foo'/>
auto math_ns::mk_space(const std::string &a) -> Xml * {
    Xml *b = new Xml(the_names["mspace"], nullptr);
    b->add_att(the_names["np_cst_width"], std::string(a));
    return b;
}

auto MathDataP::mk_mo(String a) -> gsl::not_null<Xml *> {
    Xml *x = new Xml(std::string(a));
    return gsl::not_null{new Xml(the_names["mo"], x)};
}

// Case of math env. Converts an integer into a string,
// that is put in the name field.
// void Math::set_env_name(int xtype)
// {
//   String env;
//   switch(xtype) {
//   case eqnarray_code : env = "eqnarray"; break;
//   case Beqnarray_code : env = "Beqnarray"; break;
//   case multline_code : env = "multline"; break;
//   case gather_code : env = "gather"; break;
//   case align_code: env = "align"; break;
//   case align_star_code: env = "align*"; break;
//   case split_code: env = "split"; break;
//   case aligned_code: env = "aligned"; break;
//   case gathered_code: env = "gathered"; break;
//   case eqnarray_star_code: env = "eqnarray*"; break;
//   case Beqnarray_star_code: env = "Beqnarray*"; break;
//   case multline_star_code: env = "multline*"; break;
//   case gather_star_code: env = "gather*"; break;
//   case math_code: env = "math"; break;
//   case equation_code: env = "equation"; break;
//   case equation_star_code: env = "equation*"; break;
//   case displaymath_code: env = "displaymath"; break;
//   default: env = "strange";
//   }
//   name = env;
// }

// void Math::set_env_name(int xtype)
// {
//   String env;
//   switch(xtype) {
//   case eqnarray_code: env =  "eqnarray";break;
//   case Beqnarray_code: env =  "Beqnarray";break;
//   case multline_code: env =  "multline";break;
//   case eqnarray_star_code: env =  "eqnarray*";break;
//   case Beqnarray_star_code: env =  "Beqnarray*";break;
//   case multline_star_code: env =  "multline*";break;
//   case gather_code: env =  "gather";break;
//   case gather_star_code: env =  "gather*";break;
//   case split_code: env =  "split";break;
//   case aligned_code: env =  "aligned";break;
//   case gathered_code: env =  "gathered";break;
//   case equation_code: env =  "equation";break;
//   case equation_star_code: env =  "equation*";break;
//   case math_code: env =  "math";break;
//   case displaymath_code: env =  "displaymath";break;
//   case align_code: env = "align";break;
//   case align_star_code: env = "align*"; break;
//   case flalign_code: env = "flalign"; break;
//   case flalign_star_code: env = "flalign*"; break;
//   case alignat_code:  env = "alignat"; break;
//   case alignat_star_code:  env = "alignat*"; break;
//   case xalignat_code:  env = "xalignat"; break;
//   case xalignat_star_code:  env = "xalignat*"; break;
//   case xxalignat_code:  env = "xxalignat"; break;
//   case xxalignat_star_code:  env = "xxalignat*"; break;
//   default: env = "strange";
//   }
//   name = env;
// }

// Ams environments pmatrix, bmatrix, Bmatrix etc.
// returns the fence  values
auto math_ns::special_fence(subtypes s, size_t &open, size_t &close) -> bool {
    switch (s) {
    case matrixB_code:
        open  = del_open_brace;
        close = del_close_brace;
        break;
    case matrixb_code:
        open  = del_open_bra;
        close = del_close_bra;
        break;
    case matrixv_code:
        open  = del_bar;
        close = del_bar;
        break;
    case matrixV_code:
        open  = del_Vert;
        close = del_Vert;
        break;
    case matrixp_code:
        open  = del_open_par;
        close = del_close_par;
        break;
    default: return false; // no fence
    }
    return true;
}

// This is a static function
// true if the constant is a space.
auto math_ns::math_space_code(subtypes c) -> bool {
    switch (c) {
    case quad_code: return true;
    case qquad_code: return true;
    case space_code: return true;
    case exclam_code: return true;
    case comma_code: return true;
    default: return false;
    }
}

auto MathDataP::init_builtin(const std::string &name, math_loc pos, Xml *x, symcodes t) -> Token {
    init_builtin(pos, x);
    return hash_table.primitive(name, t, subtypes(pos));
}

// This is for a general token. It defines name to be (t,pos)
// It adds defined math-location pos to be <bl>ent</bl>
// If hack is true, the formula $\name$ can be hacked to ent (i.e.
// $\alpha$ translates to &alpha;) If no entity names are desired
// then ent2 is used instead of ent, so that \alpha gives &#x3B1;

auto MathDataP::mk_gen(String name, String ent, String ent2, math_loc pos, const std::string &bl, symcodes t, bool hack) -> Token {
    Xml *x = new Xml(std::string(no_ent_names ? ent2 : ent));
    if (hack) built_in_table_alt[pos] = x;
    x = new Xml(the_names[bl], x);
    return init_builtin(name, pos, x, t);
}

// Special case where a bold variant exists
auto MathDataP::mk_gen(String name, String ent, String ent2, math_loc pos, math_loc pos2, const std::string &bl, symcodes t, bool hack)
    -> Token {
    Xml *x = new Xml(std::string(no_ent_names ? ent2 : ent));
    if (hack) built_in_table_alt[pos] = x;
    Xml *bold = new Xml(the_names[bl], x);
    bold->add_att(the_names["mathvariant"], the_names["bold"]);
    init_builtin(pos2, bold);
    x = new Xml(the_names[bl], x);
    return init_builtin(name, pos, x, t);
}

// This defines name to be a math ord command.
// the value is <mo>ent</mo> , with form=prefix, movable_limits = maybe
void MathDataP::mk_moo(String name, String ent, math_loc pos) {
    Xml *x = mk_mo(ent);
    x->add_att(the_names["form"], the_names["prefix"]);
    symcodes T = mathopn_cmd;
    if (first_w_limit_code <= pos && pos <= last_w_limit_code) {
        T = mathop_cmd;
        x->add_att(the_names["movablelimits"], the_names["true"]);
    }
    init_builtin(name, pos, x, T);
}

// This associates to the command name, a <mi> element with value ent/ent2
// with internal code pos. It is an Ordinary object.
// hack true
void MathDataP::mk_ic(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mi", mathord_cmd, true); }

// Case where the symbol has a bold variant
void MathDataP::mk_icb(String name, String ent, String ent2, math_loc pos) {
    auto pos2 = math_loc(pos - alpha_code + alpha_bcode);
    mk_gen(name, ent, ent2, pos, pos2, "mi", mathord_cmd, true);
}

// This associates to the command named name, a <mo> element with value ent
// with internal code pos. It is an Ordinary object.
// these are hacked
void MathDataP::mk_oc(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mo", mathord_cmd, true); }
// Op no limits
void MathDataP::mk_oco(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mo", mathopn_cmd, true); }
// Op display limits
void MathDataP::mk_ocol(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mo", mathop_cmd, true); }
void MathDataP::mk_ocb(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mo", mathbin_cmd, true); }
void MathDataP::mk_ocr(String name, String ent, String ent2, math_loc pos) { mk_gen(name, ent, ent2, pos, "mo", mathrel_cmd, true); }

// This associates to the command named A, a <mo> element with value B
// with internal code pos. It is of type T.
void MathDataP::mk_oc(String name, String ent, String ent2, math_loc pos, symcodes t, bool hack) {
    mk_gen(name, ent, ent2, pos, "mo", t, hack);
}

// This is for a character: the single character in A
// is defined to be <mo> with value b.
void MathDataP::TM_mk(String a, String b, math_types c) {
    auto A = uchar(a[0]);
    init_builtin(A + math_c_loc, mk_mo(b));
    math_char_type[A] = c;
}

// For a command like \enspace.
void mk_space(const std::string &name, int b) { hash_table.primitive(name, mathspace_cmd, subtypes(b)); }

void        MathDataP::fill_lr(size_t a, String b, String c) { xml_lr_ptable[a] = std::string(no_ent_names ? c : b); }
inline void MathDataP::fill_lr(size_t a, String b) { xml_lr_ptable[a] = std::string(b); }

// This assumes that nb_mathchars is 128
void MathDataP::boot_xml_lr_tables() {
    for (uchar i = 0; i < nb_mathchars; i++) {
        size_t k = i + math_c_loc;
        if (built_in_table[k] != nullptr) continue;
        Buffer B;
        B.push_back(static_cast<char>(i));
        built_in_table[k] = new Xml(the_names["mo"], nullptr);
        built_in_table[k]->push_back_unless_nullptr(new Xml(B));
    }

    // fill_lr(del_Vert, "&Verbar;", "&#x02016;");
    fill_lr(del_Vert, "&parallel;", "&#x02225;");
    fill_lr(del_vert, "&vert;", "&#x7C;");
    fill_lr(del_rangle, "&rangle;", RANGLE);
    fill_lr(del_langle, "&langle;", LANGLE);
    fill_lr(del_close_ket, "&rangle;", RANGLE);
    fill_lr(del_open_ket, "&langle;", LANGLE);
    fill_lr(del_rbrace, "&rbrace;", "&#x0007D;");
    fill_lr(del_lbrace, "&lbrace;", "&#x0007B;");
    fill_lr(del_lceil, "&lceil;", "&#x02308;");
    fill_lr(del_rceil, "&rceil;", "&#x02309;");
    fill_lr(del_lfloor, "&lfloor;", "&#x0230A;");
    fill_lr(del_rfloor, "&rfloor;", "&#x0230B;");
    fill_lr(del_open_par, "(");
    fill_lr(del_close_par, ")");
    fill_lr(del_open_brace, "{");
    fill_lr(del_close_brace, "}");
    fill_lr(del_open_bra, "[");
    fill_lr(del_close_bra, "]");
    fill_lr(del_bar, "|");
    //  fill_lr(del_open_ket,"&lt;");
    //  fill_lr(del_close_ket,"&gt;");
    fill_lr(del_dot, "");
    if (global_state.compatibility) fill_lr(del_dot, ".");
    fill_lr(del_slash, "/");
    fill_lr(del_backslash, "&#x2216;");
    fill_lr(del_lmoustache, "&lmoustache;", "&#x023B0;");
    fill_lr(del_rmoustache, "&rmoustache;", "&#x023B1;");
    fill_lr(del_arrowvert, "");
    fill_lr(del_Arrowvert, "");
    fill_lr(del_uparrow, "&uparrow;", "&#x02191;");
    fill_lr(del_downarrow, "&downarrow;", "&#x02193;");
    fill_lr(del_updownarrow, "&updownarrow;", "&#x02195;");
    fill_lr(del_Uparrow, "&Uparrow;", "&#x021D1;");
    fill_lr(del_Downarrow, "&Downarrow;", "&#x021D3;");
    fill_lr(del_Updownarrow, "&Updownarrow;", "&#x021D5;");
    fill_lr(del_lgroup, "&#x3014;");
    fill_lr(del_rgroup, "&#x3015;");
    //  fill_lr(del_lgroup,"&#x2997;");
    // fill_lr(del_rgroup,"&#x2998;");
    fill_lr(del_bracevert, "");

    mc_table[0]  = mk_mo("error unknown control sequence");
    mc_table[1]  = mk_mo(no_ent_names ? "&#xAF;" : "&OverBar;"); // \todo big table of entities and codepoints
    mc_table[2]  = mk_mo(no_ent_names ? "&#x0FE37;" : "&OverBrace;");
    mc_table[3]  = mk_mo(no_ent_names ? "&#x332;" : "&UnderBar;");
    mc_table[4]  = mk_mo(no_ent_names ? "&#x0FE38;" : "&UnderBrace;");
    mc_table[5]  = mk_mo(no_ent_names ? "&#x022EF;" : "&ctdot;");
    mc_table[6]  = mk_mo("...");
    mc_table[7]  = mk_space("1.em");
    mc_table[8]  = mk_space("2.em");
    mc_table[9]  = mk_space("4pt");
    mc_table[10] = new Xml(the_names["mi"], new Xml(std::string("$")));
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
    mc_table[21] = mk_mo("_");
    mc_table[22] = mk_mo("#");
    mc_table[23] = math_ns::mk_mi(char32_t('j'));
    mc_table[24] = mk_mo(no_ent_names ? "&#x02020;" : "&dagger;");
    mc_table[25] = mk_mo(no_ent_names ? "&#x02021;" : "&ddagger;");
    mc_table[26] = mk_mo(no_ent_names ? "&#x2DC;" : "&tilde;");
}

auto math_ns::make_math_char(uchar c, size_t n) -> Xml * {
    Buffer B;
    if (n <= 1)
        B.push_back(static_cast<char>(c));
    else
        B.append(global_state.math_chars[c][n]);
    Xml *v   = new Xml(B);
    Xml *res = new Xml(the_names["mi"], v);
    if (n == 1) res->add_att(the_names["mathvariant"], the_names["normal"]);
    return res;
}

void MathDataP::boot_chars() {
    for (unsigned i = 0; i <= 9; i++) {
        std::string K = the_names[std::to_string(i)];
        init_builtin(i + math_dig_loc, new Xml(the_names["mn"], new Xml(K))); // \todo useless?
    }

    for (uchar i = 'A'; i <= 'Z'; i++) init_builtin(i + math_char_normal_loc, make_math_char(i, 0));
    for (uchar i = 'a'; i <= 'z'; i++) init_builtin(i + math_char_normal_loc, make_math_char(i, 0));
    for (uchar i = 'A'; i <= 'Z'; i++) init_builtin(i + math_char_upright_loc, make_math_char(i, 1));
    for (uchar i = 'a'; i <= 'z'; i++) init_builtin(i + math_char_upright_loc, make_math_char(i, 1));

    init_builtin(xml_e_loc, new Xml(the_names["e"]));
    init_builtin(xml_o_loc, new Xml(the_names["o"]));
    // nb_simplemath should be 128
    for (uchar i = 0; i < nb_simplemath; i++) {
        Buffer B;
        B.push_back(static_cast<char>(i));
        Xml *res = new Xml(the_names["simplemath"], new Xml(B));
        Xml *X   = new Xml(the_names["formula"], res);
        X->add_att(the_names["type"], the_names["inline"]);
        simplemath_table[i] = X;
    }
}

// Creates complicated objects
void MathDataP::boot2() {
    // Define \colon
    Xml *colon = mk_mo(":");
    colon->add_att(std::string("lspace"), std::string("0pt"));
    init_builtin("colon", colon_code, colon, mathord_cmd);
    // Constructs varlim etc

    auto lim_op = mk_mo("lim");
    lim_op->add_att(the_names["movablelimits"], the_names["false"]);

    init_builtin("varlimsup", varlimsup_code, new Xml(xml2sons(the_names["mover"], lim_op, get_mc_table(1))), mathop_cmd);
    init_builtin("varliminf", varliminf_code, new Xml(xml2sons(the_names["munder"], lim_op, get_mc_table(3))), mathop_cmd);

    auto *x = new Xml(xml2sons(the_names["munder"], lim_op, get_builtin(underrightarrow_code)));
    x->add_att(the_names["accentunder"], the_names["true"]);
    init_builtin("varinjlim", varinjlim_code, x, mathop_cmd);

    x = new Xml(xml2sons(the_names["munder"], lim_op, get_builtin(underleftarrow_code)));
    x->add_att(the_names["accentunder"], the_names["true"]);
    init_builtin("varprojlim", varprojlim_code, x, mathop_cmd);

    x = mk_mo("(");
    x = new Xml(the_names["mpadded"], x);
    x->add_att(the_names["np_cst_width"], the_names["0pt"]);
    x = new Xml(the_names["mphantom"], x);
    init_builtin("strut", strut_code, x, mathord_cmd);
    init_builtin("mathstrut", strut_code, x, mathord_cmd);

    Xml *y = new Xml(the_names["mpadded"], get_builtin(int_code));
    y->add_att(the_names["np_cst_width"], std::string("-3pt"));
    Xml *z = get_builtin(xml_thickmu_space_loc);
    x      = new Xml(the_names["mrow"], nullptr);
    x->push_back_unless_nullptr(z);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(z);
    init_builtin("iint", iint_code, x, mathop_cmd);
    x = new Xml(the_names["mrow"], nullptr);
    x->push_back_unless_nullptr(z);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(z);
    init_builtin("iiint", iiint_code, x, mathop_cmd);
    x = new Xml(the_names["mrow"], nullptr);
    x->push_back_unless_nullptr(z);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(y);
    x->push_back_unless_nullptr(z);
    init_builtin("iiiint", iiiint_code, x, mathop_cmd);
    x = new Xml(the_names["mrow"], nullptr);
    x->push_back_unless_nullptr(get_builtin(int_code));
    x->push_back_unless_nullptr(math_data.get_mc_table(6));
    x->push_back_unless_nullptr(get_builtin(int_code));
    init_builtin("idotsint", idotsint_code, x, mathop_cmd);
    x                              = new Xml(std::string("none"), nullptr);
    hash_table.mmlnone_token       = init_builtin("mmlnone", mml_none_code, x, mathord_cmd);
    x                              = new Xml(std::string("mprescripts"), nullptr);
    hash_table.mmlprescripts_token = init_builtin("mmlprescripts", mml_prescripts_code, x, mathord_cmd);
}

void MathDataP::boot() {
    no_ent_names = the_main.no_entnames;
    boot_table();
    fill_single_char();
    fill_math_char_slots();
    if (!no_ent_names) fill_math_char_slots_ent();
    boot_chars();

    Dispatcher::boot(); // \todo move to a more reasonable and robust place

    mk_icb("alpha", "&alpha;", "&#x3B1;", alpha_code);
    mk_icb("beta", "&beta;", "&#x3B2;", beta_code);
    mk_icb("gamma", "&gamma;", "&#x3B3;", gamma_code);
    mk_icb("delta", "&delta;", "&#x3B4;", delta_code);
    mk_icb("epsilon", "&straightepsilon;", "&#x3F5;", epsilon_code);
    mk_icb("varepsilon", "&varepsilon;", "&#x3B5;", varepsilon_code);
    mk_icb("zeta", "&zeta;", "&#x3B6;", zeta_code);
    mk_icb("eta", "&eta;", "&#x3B7;", eta_code);
    mk_icb("theta", "&theta;", "&#x3B8;", theta_code);
    mk_icb("iota", "&iota;", "&#x3B9;", iota_code);
    mk_icb("kappa", "&kappa;", "&#x3BA;", kappa_code);
    mk_icb("lambda", "&lambda;", "&#x3BB;", lambda_code);
    mk_icb("mu", "&mu;", "&#x3BC;", mu_code);
    mk_icb("nu", "&nu;", "&#x3BD;", nu_code);
    mk_icb("xi", "&xi;", "&#x3BE;", xi_code);
    mk_icb("pi", "&pi;", "&#x3C0;", pi_code);
    mk_icb("rho", "&rho;", "&#x3C1;", rho_code);
    mk_icb("sigma", "&sigma;", "&#x3C3;", sigma_code);
    mk_icb("tau", "&tau;", "&#x3C4;", tau_code);
    mk_icb("upsilon", "&upsi;", "&#x3C5;", upsilon_code);
    mk_icb("phi", "&phi;", "&#x3C6;", phi_code);
    mk_icb("varphi", "&phiv;", "&#x3D5;", varphi_code);
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
    mk_ic("hbar", "&#x127;", "&#x127;", hbar_code);
    mk_ic("hslash", "&hbar;", "&#x0210F;", hbar_code);
    mk_ic("ell", "&ell;", "&#x02113;", ell_code); // pb
    mk_ic("wp", "&wp;", "&#x02118;", wp_code);
    mk_ic("Re", "&Re;", "&#x0211C;", Re_code);
    mk_ic("Im", "&Im;", "&#x02111;", Im_code);

    // Other mi alphabetic order
    mk_ic("aleph", "&aleph;", "&#x02135;", aleph_code);
    mk_ic("angle", "&ang;", "&#x02220;", angle_code);
    mk_ic("backprime", "&backprime;", "&#x2035;", backprime_code);
    mk_ic("beth", "&beth;", "&#x02136;", beth_code);
    mk_ic("bigstar", "&bigstar;", "&#x2605;", bigstar_code);
    mk_ic("blacklozenge", "&blacklozenge;", "&#x29EB;", blacklozenge_code);
    mk_ic("blacksquare", "&blacksquare;", "&#x25AA;", blacksquare_code);
    mk_ic("blacktriangledown", "&blacktriangledown;", "&#x25BE;", blacktriangledown_code);
    mk_ic("blacktriangle", "&blacktriangle;", "&#x25B4;", blacktriangle_code);
    mk_ic("bot", "&bottom;", "&#x022A5;", bot_code);
    mk_ic("bottom", "&bottom;", "&#x022A5;", bottom_code); // eval_let ?????
    mk_ic("circledS", "&circledS;", "&#x24C8;", circledS_code);
    mk_ic("complement", "&complement;", "&#x02201;", complement_code);
    mk_ic("daleth", "&daleth;", "&#x2138;", daleth_code);
    mk_ic("diagdown", "&setminus;", "&#x2216;", diagdown_code);
    mk_ic("diagup", "&#x2215;", "&#x2215;", diagup_code);
    mk_ic("emptyset", "&emptyset;", "&#x2205;", emptyset_code);
    mk_ic("eth", "&eth;", "&#xF0;", eth_code);
    mk_ic("ExponentialE", "&ExponentialE;", "&#x2147;", exponentiale_code);
    mk_ic("Finv", "&#x2132;", "&#x2132;", Finv_code);
    mk_ic("Game", "&#x2141;", "&#x2141;", Game_code);
    mk_ic("gimel", "&gimel;", "&#x2137;", gimel_code);
    mk_ic("ImaginaryI", "&ImaginaryI;", "&#x2148;", imaginaryi_code);
    mk_ic("infty", "&infin;", "&#x0221E;", infty_code);
    mk_ic("lightning", "&#x2607;", "&#x2607;", lightning_code);
    mk_ic("lozenge", "&lozenge;", "&#x25CA;", lozenge_code);
    mk_ic("nabla", "&nabla;", "&#x02207;", nabla_code);
    mk_ic("measuredangle", "&measuredangle;", "&#x2221;", measuredangle_code);
    mk_ic("mathparagraph", "&para;", "&#xB6;", mathparagraph_code);
    mk_ic("mathsection", "&sect;", "&#xA7;", mathsection_code);
    mk_ic("partial", "&part;", "&#x02202;", partial_code);
    mk_ic("nexists", "&nexists;", "&#x02204;", nexists_code);
    mk_ic("surd", "&radic;", "&#x0221A;", surd_code);
    mk_ic("sphericalangle", "&angsph;", "&#x2222;", sphericalangle_code);
    mk_ic("top", "&top;", "&#x022A4;", top_code);
    mk_ic("triangle", "&triangle;", "&#x025B5;", triangle_code);
    mk_ic("varnothing", "&#x2300;", "&#x2300;", varnothing_code);

    // mathop rm.. alphabetic order
    mk_moo("arccos", "arccos", arccos_code);
    mk_moo("arcsin", "arcsin", arcsin_code);
    mk_moo("arctan", "arctan", arctan_code);
    mk_moo("arg", "arg", arg_code);
    mk_moo("cos", "cos", cos_code);
    mk_moo("cosh", "cosh", cosh_code);
    mk_moo("cot", "cot", cot_code);
    mk_moo("coth", "coth", coth_code);
    mk_moo("csc", "csc", csc_code);
    mk_moo("deg", "deg", deg_code);
    mk_moo("det", "det", det_code);
    mk_moo("dim", "dim", dim_code);
    mk_moo("exp", "exp", exp_code);
    mk_moo("gcd", "gcd", gcd_code);
    mk_moo("hom", "hom", hom_code);
    mk_moo("inf", "inf", inf_code);
    mk_moo("injlim", "inj lim", injlim_code);
    mk_moo("ker", "ker", ker_code);
    mk_moo("lg", "lg", lg_code);
    mk_moo("lim", "lim", lim_code);
    mk_moo("liminf", "lim inf", liminf_code);
    mk_moo("limsup", "lim sup", limsup_code);
    mk_moo("ln", "ln", ln_code);
    mk_moo("log", "log", log_code);
    mk_moo("max", "max", max_code);
    mk_moo("min", "min", min_code);
    mk_moo("Pr", "Pr", Pr_code);
    mk_moo("projlim", "proj lim", projlim_code);
    mk_moo("sec", "sec", sec_code);
    mk_moo("sin", "sin", sin_code);
    mk_moo("@mod", "mod", mod_code);
    mk_moo("sinh", "sinh", sinh_code);
    mk_moo("sup", "sup", sup_code);
    mk_moo("tan", "tan", tan_code);
    mk_moo("tanh", "tanh", tanh_code);

    mk_oc("forall", "&forall;", "&#x02200;", forall_code);
    mk_oc("exists", "&exist;", "&#x02203;", exists_code);

    // type large
    mk_ocol("coprod", "&coprod;", "&#x02210;", coprod_code);
    mk_ocol("bigvee", "&bigvee;", "&#x022C1;", bigvee_code);       // pb
    mk_ocol("bigwedge", "&bigwedge;", "&#x022C0;", bigwedge_code); // pb
    mk_ocol("biguplus", "&uplus;", "&#x0228E;", biguplus_code);
    mk_ocol("bigcap", "&bigcap;", "&#x022C2;", bigcap_code);
    mk_ocol("bigcup", "&bigcup;", "&#x022C3;", bigcup_code);
    mk_oco("int", "&int;", "&#x0222B;", int_code);
    mk_oco("xiint", "&#x0222C;", "&#x0222C;", xiint_code);
    mk_oco("xiiint", "&iiint;", "&#x0222D;", xiiint_code);
    mk_oco("xiiiint", "&iiiint;", "&#x02A0C;", xiiiint_code);
    mk_ocol("sum", "&sum;", "&#x02211;", sum_code);
    mk_ocol("prod", "&prod;", "&#x0220F;", prod_code);
    mk_ocol("bigotimes", "&bigotimes;", "&#x02A02;", bigotimes_code);
    mk_ocol("bigoplus", "&bigoplus;", "&#x02A01;", bigoplus_code);
    mk_ocol("bigodot", "&bigodot;", "&#x02A00;", bigodot_code);
    mk_oco("oint", "&oint;", "&#x0222E;", oint_code);
    mk_ocol("bigsqcup", "&bigsqcup;", "&#x02A06;", bigsqcup_code);
    mk_oco("smallint", "&int;", "&#x0222B;", smallint_code); // pas de small...

    // type bin
    mk_ocb("triangleq", "&triangleq;", "&#x0225C;", triangleq_code);
    mk_ocb("triangleleft", "&triangleleft;", "&#x025C3;", triangleleft_code);
    mk_ocb("triangleright", "&triangleright;", "&#x025B9;", triangleright_code);
    mk_ocb("bigtriangleup", "&bigtriangleup;", "&#x025B3;", bigtriangleup_code);
    mk_ocb("bigtriangledown", "&bigtriangledown;", "&#x025BD;", bigtriangledown_code);
    mk_ocb("wedge", "&wedge;", "&#x02227;", wedge_code);
    mk_ocb("land", "&wedge;", "&#x02227;", wedge_code);
    mk_ocb("vee", "&vee;", "&#x02228;", vee_code);
    mk_ocb("lor", "&vee;", "&#x02228;", vee_code);
    mk_ocb("cap", "&cap;", "&#x02229;", cap_code);
    mk_ocb("cup", "&cup;", "&#x0222A;", cup_code);
    mk_ocb("Cap", "&Cap;", "&#x022D2;", Cap_code);
    mk_ocb("Cup", "&Cup;", "&#x022D3;", Cup_code);
    mk_ocb("multimap", "&multimap;", "&#x022B8;", multimap_code);
    mk_ocb("dagger", "&dagger;", "&#x02020;", dagger_code);
    mk_ocb("ddagger", "&ddagger;", "&#x02021;", ddagger_code);
    mk_ocb("sqcap", "&sqcap;", "&#x02293;", sqcap_code);
    mk_ocb("sqcup", "&sqcup;", "&#x02294;", sqcup_code);
    mk_ocb("amalg", "&amalg;", "&#x02A3F;", almalg_code);
    mk_ocb("diamond", "&diamond;", "&#x022C4;", diamand_code);
    mk_ocb("Diamond", "&diamond;", "&#x022C4;", Diamond_code);
    mk_ocb("bullet", "&bullet;", "&#x02022;", bullet_code);
    mk_ocb("wr", "&wr;", "&#x02240;", wr_code);
    mk_ocb("div", "&div;", "&#xF7;", div_code);
    mk_ocb("odot", "&odot;", "&#x02299;", odot_code);
    mk_ocb("oslash", "&osol;", "&#x02298;", oslash_code);
    mk_ocb("otimes", "&otimes;", "&#x02297;", otimes_code);
    mk_ocb("ominus", "&ominus;", "&#x02296;", ominus_code);
    mk_ocb("oplus", "&oplus;", "&#x02295;", oplus_code);
    mk_ocb("uplus", "&uplus;", "&#x0228E;", uplus_code);
    mk_ocb("mp", "&mp;", "&#x02213;", mp_code);
    mk_ocb("pm", "&pm;", "&#xB1;", pm_code);
    mk_ocb("circ", "&SmallCircle;", "&#x02218;", circ_code);   // pb
    mk_ocb("bigcirc", "&bigcirc;", "&#x025EF;", bigcirc_code); // pb
    mk_ocb("setminus", "&setminus;", "&#x02216;", setminus_code);
    mk_ocb("cdot", "&middot;", "&#xB7;", cdot_code); // pb
    mk_ocb("ast", "&ast;", "&#x2A;", ast_code);
    mk_ocb("times", "&times;", "&#xD7;", times_code);
    mk_ocb("star", "&star;", "&#x02606;", star_code);
    mk_ocb("*", "&InvisibleTimes;", "&#x02062;", starchar_code);
    mk_ocb("InvisibleTimes", "&InvisibleTimes;", "&#x2062;", starchar_code);
    mk_ocb("ApplyFunction", "&ApplyFunction;", "&#x2061;", applyfct_code);
    mk_ocb("InvisibleComma", "&InvisibleComma;", "&#x2063;", invisiblecomma_code);
    mk_ocb("baro", "&ovbar;", "&#x233D;", baro_code);
    mk_ocb("barwedge", "&#x22BC;", "&#x22BC;", barwedge_code);
    mk_ocb("curlyvee", "&curlyvee;", "&#x22CE;", curlyvee_code);
    mk_ocb("curlywedge", "&curlywedge;", "&#x22CF;", curlywedge_code);
    mk_ocb("divideontimes", "&divideontimes;", "&#x22C7;", divideontimes_code);
    mk_ocb("dotplus", "&dotplus;", "&#x2214;", dotplus_code);
    mk_ocb("gtrdot", "&gtrdot;", "&#x22D7;", gtrdot_code);
    mk_ocb("intercal", "&intercal;", "&#x22BA;", intercal_code);
    mk_ocb("interleave", "&#x2AF4;", "&#x2AF4;", interleave_code);
    mk_ocb("leftslice", "&#x2AA6;", "&#x2AA6;", leftslice_code);
    mk_ocb("rightslice", "&#x2AA7;", "&#x2AA7;", rightslice_code);
    mk_ocb("leftthreetimes", "&leftthreetimes;", "&#x22CB;", leftthreetimes_code);
    mk_ocb("rightthreetimes", "&rightthreetimes;", "&#x22CC;", rightthreetimes_code);
    mk_ocb("lessdot", "&lessdot;", "&#x22D6;", lessdot_code);
    mk_ocb("ltimes", "&ltimes;", "&#x22C9;", ltimes_code);
    mk_ocb("rtimes", "&rtimes;", "&#x22CA;", rtimes_code);
    mk_ocb("merge", "&#x2A07;", "&#x2A07;", merge_code);
    mk_ocb("sslash", "&#x2AFD;", "&#x2AFD;", sslash_code);
    mk_ocb("talloblong", "&#x2AFF;", "&#x2AFF;", talloblong_code);
    mk_ocb("boxast", "&#x29C6;", "&#x29C6;", boxast_code);
    mk_ocb("boxbox", "&#x29C8;", "&#x29C8;", boxbox_code);
    mk_ocb("boxbslash", "&bsolb;", "&#x29C5;", boxbslash_code);
    mk_ocb("boxcircle", "&#x29C7;", "&#x29C7;", boxcircle_code);
    mk_ocb("boxdot", "&dotsquare;", "&#x22A1;", boxdot_code);
    mk_ocb("boxminus", "&boxminus;", "&#x229F;", boxminus_code);
    mk_ocb("boxplus", "&boxplus;", "&#x229E;", boxplus_code);
    mk_ocb("boxslash", "&solb;", "&#x29C4;", boxslash_code);
    mk_ocb("boxtimes", "&boxtimes;", "&#x22A0;", boxtimes_code);
    mk_ocb("centerdot", "&sdot;", "&#x22C5;", centerdot_code);
    mk_ocb("circledast", "&circledast;", "&#x229B;", circledast_code);
    mk_ocb("circledcirc", "&circledcirc;", "&#x229A;", circledcirc_code);
    mk_ocb("circleddash", "&circleddash;", "&#x229D;", circleddash_code);
    mk_ocb("obar", "&omid;", "&#x29B6;", obar_code);
    mk_ocb("ogreaterthan", "&ogt;", "&#x29C1;", ogreaterthan_code);
    mk_ocb("olessthan", "&olt;", "&#x29C0;", olessthan_code);
    mk_ocb("varocircle", "&#x29BE;", "&#x29BE;", varocircle_code);

    // Rel
    mk_ocr("propto", "&propto;", "&#x0221D;", propto_code);
    mk_ocr("sqsubseteq", "&sqsubseteq;", "&#x02291;", sqsubseteq_code);
    mk_ocr("sqsupseteq", "&sqsupseteq;", "&#x02292;", sqsupseteq_code);
    mk_ocr("sqsubset", "&sqsubset;", "&#x0228F;", sqsubset_code);
    mk_ocr("sqsupset", "&sqsupset;", "&#x02290;", sqsupset_code);
    mk_ocr("parallel", "&parallel;", "&#x02225;", parallel_code);
    mk_ocr("mid", "&mid;", "&#x02223;", mid_code);
    mk_ocr("dashv", "&dashv;", "&#x022A3;", dashv_code);
    mk_ocr("vdash", "&vdash;", "&#x022A2;", vdash_code);
    mk_ocr("Vdash", "&Vdash;", "&#x022A9;", Vdash_code);
    mk_ocr("vDash", "&vDash;", "&#x22A7;", vDash_code);
    mk_ocr("VDash", "&VDash;", "&#x22AB;", VDash_code);
    mk_ocr("Vvdash", "&Vvdash;", "&#x22AA;", Vvdash_code);
    mk_ocr("models", "&models;", "&#x022A7;", models_code);
    mk_ocr("nearrow", "&nearrow;", "&#x02197;", nearrow_code);
    mk_ocr("searrow", "&searrow;", "&#x02198;", searrow_code);
    mk_ocr("nwarrow", "&nwarrow;", "&#x02196;", nwarrow_code);
    mk_ocr("swarrow", "&swarrow;", "&#x02199;", swarrow_code);
    mk_ocr("Leftrightarrow", "&Leftrightarrow;", "&#x021D4;", Leftrightarrow_code);
    mk_ocr("Leftarrow", "&Leftarrow;", "&#x021D0;", Leftarrow_code);
    mk_ocr("Rightarrow", "&Rightarrow;", "&#x021D2;", Rightarrow_code);
    mk_ocr("implies", "&Rightarrow;", "&#x021D2;", Rightarrow_code);
    mk_ocr("ne", "&ne;", "&#x02260;", ne_code);
    mk_ocr("neq", "&ne;", "&#x02260;", ne_code);
    mk_ocr("le", "&le;", "&#x02264;", le_code);
    mk_ocr("leq", "&le;", "&#x02264;", le_code);
    mk_ocr("ge", "&ge;", "&#x2265;", ge_code);
    mk_ocr("geq", "&ge;", "&#x2265;", ge_code);
    mk_ocr("succ", "&succ;", "&#x0227B;", succ_code);
    mk_ocr("approx", "&approx;", "&#x02248;", approx_code);
    mk_ocr("succeq", "&succeq;", "&#x02AB0;", succeq_code);
    mk_ocr("preceq", "&preceq;", "&#x02AAF;", preceq_code);
    mk_ocr("prec", "&prec;", "&#x0227A;", prec_code);
    mk_ocr("doteq", "&doteq;", "&#x02250;", doteq_code);
    mk_ocr("supset", "&supset;", "&#x02283;", supset_code);
    mk_ocr("subset", "&subset;", "&#x02282;", subset_code);
    mk_ocr("supseteq", "&supseteq;", "&#x02287;", supseteq_code);
    mk_ocr("subseteq", "&subseteq;", "&#x02286;", subseteq_code);
    mk_ocr("bindnasrepma", "&#x214B;", "&#x214B;", bindnasrepma_code);
    mk_ocr("ni", "&ni;", "&#x0220B;", ni_code);
    mk_ocr("owns", "&ni;", "&#x0220B;", ni_code);
    mk_ocr("gg", "&gg;", "&#x0226B;", gg_code);
    mk_ocr("ll", "&ll;", "&#x0226A;", ll_code);
    mk_ocr("gtrless", "&gl;", "&#x02277;", gtrless_code);
    mk_ocr("geqslant", "&geqslant;", "&#x02A7E;", geqslant_code);
    mk_ocr("leqslant", "&leqslant;", "&#x02A7D;", leqslant_code);
    mk_ocr("not", "&not;", "&#xAC;", not_code); // pb
    mk_ocr("notin", "&notin;", "&#x02209;", notin_code);
    mk_ocr("leftrightarrow", "&leftrightarrow;", "&#x02194;", leftrightarrow_code);
    mk_ocr("leftarrow", "&leftarrow;", "&#x02190;", leftarrow_code);
    mk_ocr("gets", "&leftarrow;", "&#x02190;", leftarrow_code);
    mk_ocr("rightarrow", "&rightarrow;", "&#x02192;", rightarrow_code);
    mk_ocr("to", "&rightarrow;", "&#x02192;", rightarrow_code);
    mk_ocr("mapsto", "&mapsto;", "&#x021A6;", mapsto_code);
    mk_ocr("sim", "&sim;", "&#x0223C;", sim_code);
    mk_ocr("simeq", "&simeq;", "&#x02243;", simeq_code);
    mk_ocr("perp", "&perp;", "&#x022A5;", perp_code);
    mk_ocr("equiv", "&equiv;", "&#x02261;", equiv_code);
    //  mk_ocr("asymp","&asymp;","&#x02248;",asymp_code);
    mk_ocr("asymp", "&asympeq;", "&#x0224D;", asymp_code);
    mk_ocr("smile", "&smile;", "&#x02323;", smile_code);
    mk_ocr("frown", "&frown;", "&#x02322;", frown_code);
    mk_ocr("leftharpoonup", "&leftharpoonup;", "&#x021BC;", leftharpoonup_code);
    mk_ocr("leftharpoondown", "&leftharpoondown;", "&#x021BD;", leftharpoondown_code);
    mk_ocr("rightharpoonup", "&rightharpoonup;", "&#x021C0;", rightharpoonup_code);
    mk_ocr("rightharpoondown", "&rightharpoondown;", "&#x021C1;", rightharpoondown_code);
    mk_ocr("hookrightarrow", "&hookrightarrow;", "&#x021AA;", hookrightarrow_code);
    mk_ocr("hookleftarrow", "&hookleftarrow;", "&#x021A9;", hookleftarrow_code);
    mk_ocr("bowtie", "&bowtie;", "&#x022C8;", bowtie_code);
    eval_let("Join", "bowtie");
    mk_ocr("Longrightarrow", "&Longrightarrow;", "&#x027F9;", Longrightarrow_code);
    mk_ocr("longrightarrow", "&longrightarrow;", "&#x027F6;", longrightarrow_code);
    mk_ocr("longleftarrow", "&longleftarrow;", "&#x027F5;", longleftarrow_code);
    mk_ocr("Longleftarrow", "&Longleftarrow;", "&#x027F8;", Longleftarrow_code);
    // mk_ocr("longmapsto","&longmapsto;","&#x027FC;", longmapsto_code);
    // variant
    // mk_ocr("longmapsto","&longmapsto;","&#x21a4;", longmapsto_code);
    mk_ocr("longleftrightarrow", "&longleftrightarrow;", "&#x027F7;", longleftrightarrow_code);
    mk_ocr("Longleftrightarrow", "&Longleftrightarrow;", "&#x027FA;", Longleftrightarrow_code);
    //  mk_ocr("iff","&iff;");   # merde
    // mk_ocr("iff","&Longleftrightarrow;","&#x027FA;", iff_code);
    mk_ocr("iff", "&Leftrightarrow;", "&#x021D4;", iff_code);
    mk_ocr("backepsilon", "&backepsilon;", "&#x3F6;", backepsilon_code);
    mk_ocr("because", "&because;", "&#x2235;", because_code);
    mk_ocr("between", "&between;", "&#x226C;", between_code);
    mk_ocr("nmid", "&NotVerticalBar;", "&#x2224;", nmid_code);
    mk_ocr("nparallel", "&nparallel;", "&#x2226;", nparallel_code);
    mk_ocr("nVDash", "&nVDash;", "&#x22AF;", nVDash_code);
    mk_ocr("nvDash", "&nvDash;", "&#x22AD;", nvDash_code);
    mk_ocr("nVdash", "&nVdash;", "&#x22AE;", nVdash_code);
    mk_ocr("nvdash", "&nvdash;", "&#x22AC;", nvdash_code);
    mk_ocr("pitchfork", "&pitchfork;", "&#x22D4;", pitchfork_code);
    mk_ocr("therefore", "&therefore;", "&#x2234;", therefore_code);
    mk_ocr("nLeftarrow", "&nLeftarrow;", "&#x21CD;", nLeftarrow_code);
    mk_ocr("nLeftrightarrow", "&nLeftrightarrow;", "&#x21CE;", nLeftrightarrow_code);
    mk_ocr("nRightarrow", "&nRightarrow;", "&#x21CF;", nRightarrow_code);
    mk_ocr("nleftarrow", "&nleftarrow;", "&#x219A;", nleftarrow_code);
    mk_ocr("nleftrightarrow", "&nleftrightarrow;", "&#x21AE;", nleftrightarrow_code);
    mk_ocr("nrightarrow", "&nrightarrow;", "&#x219B;", nrightarrow_code);
    mk_ocr("approxeq", "&approxeq;", "&#x224A;", approxeq_code);
    mk_ocr("Subset", "&Subset;", "&#x22D0;", Subset_code);
    mk_ocr("Supset", "&Supset;", "&#x22D1;", Supset_code);
    mk_ocr("blacktriangleleft", "&blacktriangleleft;", "&#x25C2;", blacktriangleleft_code);
    mk_ocr("blacktriangleright", "&blacktriangleright;", "&#x25B8;", blacktriangleright_code);
    mk_ocr("triangledown", "&triangledown;", "&#x25BF;", triangledown_code);
    mk_ocr("circlearrowleft", "&#x2940;", "&#x2940;", circlearrowleft_code);
    mk_ocr("circlearrowright", "&#x2941;", "&#x2941;", circlearrowright_code);
    mk_ocr("curvearrowleft", "&#x293A;", "&#x293A;", curvearrowleft_code);
    mk_ocr("curvearrowright", "&#x293B;", "&#x293b;", curvearrowright_code);
    mk_ocr("dasharrow", "&rBarr;", "&#x290F;", dasharrow_code);
    mk_ocr("dashleftarrow", "&lBarr;", "&#x290E;", dashleftarrow_code);
    mk_ocr("dashrightarrow", "&rBarr;", "&#x290F;", dashrightarrow_code);
    mk_ocr("fatsemi", "&#x2A1F;", "&#x2A1F;", fatsemi_code);
    mk_ocr("downdownarrows", "&downdownarrows;", "&#x21CA;", downdownarrows_code);
    mk_ocr("downharpoonright", "&downharpoonright;", "&#x21C2;", downharpoonright_code);
    mk_ocr("downharpoonleft", "&downharpoonleft;", "&#x21C3;", downharpoonleft_code);
    mk_ocr("leftarrowtail", "&leftarrowtail;", "&#x21A2;", leftarrowtail_code);
    mk_ocr("rightarrowtail", "&rightarrowtail;", "&#x21A3;", rightarrowtail_code);
    mk_ocr("leftarrowtriangle", "&loarr;", "&#x21FD;", leftarrowtriangle_code);
    mk_ocr("rightarrowtriangle", "&roarr;", "&#x21FE;", rightarrowtriangle_code);
    mk_ocr("leftrightarrowtriangle", "&hoarr;", "&#x21FF;", leftrightarrowtriangle_code);
    mk_ocr("leftleftarrows", "&leftleftarrows;", "&#x21C7;", leftleftarrows_code);
    mk_ocr("upuparrows", "&upuparrows;", "&#x21C8;", upuparrows_code);
    mk_ocr("rightrightarrows", "&rightrightarrows;", "&#x21C9;", rightrightarrows_code);
    mk_ocr("leftrightarrows", "&leftrightarrows;", "&#x21C6;", leftrightarrows_code);
    mk_ocr("leftrightharpoons", "&leftrightharpoons;", "&#x21CB;", leftrightharpoons_code);
    mk_ocr("leftrightsquigarrow", "&leftrightsquigarrow;", "&#x21AD;", leftrightsquigarrow_code);
    mk_ocr("Lleftarrow", "&Lleftarrow;", "&#x21DA;", Lleftarrow_code);
    mk_ocr("Rrightarrow", "&Rrightarrow;", "&#x21DB;", Rrightarrow_code);
    mk_ocr("Longmapsfrom", "&#x27FD;", "&#x27FD;", Longmapsfrom_code);
    mk_ocr("longmapsfrom", "&#x27FB;", "&#x27FB;", longmapsfrom_code);
    mk_ocr("Mapsfrom", "&#x2906;", "&#x2906;", Mapsfrom_code);
    mk_ocr("Longmapsto", "&#x27FE;", "&#x27FE;", Longmapsto_code);
    mk_ocr("Mapsto", "&#x2907;", "&#x2907;", Mapsto_code);
    mk_ocr("mapsfrom", "&mapstoleft;", "&#x21A4;", mapsfrom_code);
    mk_ocr("longmapsto", "&longmapsto;", "&#x27FC;", longmapsto_code);
    mk_ocr("looparrowleft", "&looparrowleft;", "&#x21AB;", looparrowleft_code);
    mk_ocr("looparrowright", "&looparrowright;", "&#x21AC;", looparrowright_code);
    mk_ocr("Lsh", "&Lsh;", "&#x21B0;", Lsh_code);
    mk_ocr("Rsh", "&Rsh;", "&#x21B1;", Rsh_code);
    mk_ocr("rightleftarrows", "&rightleftarrows;", "&#x21C4;", rightleftarrows_code);
    mk_ocr("rightleftharpoons", "&rightleftharpoons;", "&#x21CC;", rightleftharpoons_code);
    mk_ocr("rightsquigarrow", "&rightsquigarrow;", "&#x21DD;", rightsquigarrow_code);
    mk_ocr("twoheadleftarrow", "&twoheadleftarrow;", "&#x219E;", twoheadleftarrow_code);
    mk_ocr("twoheadrightarrow", "&twoheadrightarrow;", "&#x21A0;", twoheadrightarrow_code);
    mk_ocr("upharpoonleft", "&upharpoonleft;", "&#x21BF;", upharpoonleft_code);
    mk_ocr("upharpoonright", "&upharpoonright;", "&#x21BE;", upharpoonright_code);
    mk_ocr("varprime", "&prime;", "&#x2032;", varprime_code); // vprime in isoamso,
    mk_ocr("Prime", "&Prime;", "&#x2033;", Prime_code);
    mk_ocr("tprime", "&tprime;", "&#x2034;", tprime_code);
    mk_ocr("bprime", "&backprime;", "&#x2035;", bprime_code);
    mk_ocr("qprime", "&qprime;", "&#x2057;", qprime_code);
    mk_ocr("veebar", "&veebar;", "&#x22BB;", veebar_code);
    mk_ocr("backsimeq", "&backsimeq;", "&#x22CD;", backsimeq_code);
    mk_ocr("backsim", "&backsim;", "&#x223D;", backsim_code);
    mk_ocr("Bumpeq", "&Bumpeq;", "&#x224E;", Bumpeq_code);
    mk_ocr("bumpeq", "&bumpeq;", "&#x224F;", bumpeq_code);
    mk_ocr("circeq", "&circeq;", "&#x2257;", circeq_code);
    mk_ocr("curlyeqprec", "&curlyeqprec;", "&#x22DE;", curlyeqprec_code);
    mk_ocr("curlyeqsucc", "&curlyeqsucc;", "&#x22DF;", curlyeqsucc_code);
    mk_ocr("Doteq", "&doteqdot;", "&#x2251;", Doteq_code);
    mk_ocr("doteqdot", "&doteqdot;", "&#x2251;", doteqdot_code);
    mk_ocr("eqcirc", "&eqcirc;", "&#x2256;", eqcirc_code);
    mk_ocr("eqsim", "&eqsim;", "&#x2242;", eqsim_code);
    mk_ocr("eqslantgtr", "&eqslantgtr;", "&#x2A96;", eqslantgtr_code);
    mk_ocr("eqslantless", "&eqslantless;", "&#x2A95;", eqslantless_code);
    mk_ocr("fallingdotseq", "&fallingdotseq;", "&#x2252;", fallingdotseq_code);
    mk_ocr("risingdotseq", "&risingdotseq;", "&#x2253;", risingdotseq_code);
    mk_ocr("geqq", "&geqq;", "&#x2267;", geqq_code);
    mk_ocr("leqq", "&leqq;", "&#x2266;", leqq_code);
    mk_ocr("gggtr", "&ggg;", "&#x22D9;", gggtr_code);
    mk_ocr("ggg", "&ggg;", "&#x22D9;", ggg_code);
    mk_ocr("lll", "&Ll;", "&#x22D8;", lll_code);
    mk_ocr("llless", "&Ll;", "&#x22D8;", llless_code);
    mk_ocr("gtrapprox", "&gtrapprox;", "&#x2A86;", gtrapprox_code);
    mk_ocr("lessapprox", "&lessapprox;", "&#x2A85;", lessapprox_code);
    mk_ocr("gtreqless", "&gtreqless;", "&#x22DB;", gtreqless_code);
    mk_ocr("gtreqqless", "&gtreqqless;", "&#x2A8C;", gtreqqless_code);
    mk_ocr("lesseqgtr", "&lesseqgtr;", "&#x22DA;", lesseqgtr_code);
    mk_ocr("lesseqqgtr", "&lesseqqgtr;", "&#x2A8B;", lesseqqgtr_code);
    mk_ocr("lessgtr", "&lessgtr;", "&#x2276;", lessgtr_code);
    mk_ocr("lesssim", "&lesssim;", "&#x2272;", lesssim_code);
    mk_ocr("gtrsim", "&gtrsim;", "&#x2273;", gtrsim_code);
    mk_ocr("precapprox", "&precapprox;", "&#x2AB7;", precapprox_code);
    mk_ocr("succapprox", "&succapprox;", "&#x2AB8;", succapprox_code);
    mk_ocr("succcurlyeq", "&succcurlyeq;", "&#x227D;", succcurlyeq_code);
    mk_ocr("succsim", "&succsim;", "&#x227F;", succsim_code);
    mk_ocr("preccurlyeq", "&preccurlyeq;", "&#x227C;", preccurlyeq_code);
    // mk_ocr("leftrightarroweq","&gtrsim;", "&#x2273;",leftrightarroweq_code);
    mk_ocr("gnapprox", "&gnapprox;", "&#x2A8A;", gnapprox_code);
    mk_ocr("lnapprox", "&lnapprox;", "&#x2A89;", lnapprox_code);
    mk_ocr("gneqq", "&gneqq;", "&#x2269;", gneqq_code);
    mk_ocr("lneqq", "&lneqq;", "&#x2268;", lneqq_code);
    mk_ocr("gneq", "&gneq;", "&#x2A88;", gneq_code);
    mk_ocr("lneq", "&lneq;", "&#x2A87;", lneq_code);
    mk_ocr("gnsim", "&gnsim;", "&#x22E7;", gnsim_code);
    mk_ocr("lnsim", "&lnsim;", "&#x22E6;", lnsim_code);
    mk_ocr("ngeq", "&ngeq;", "&#x2271;", ngeq_code);
    mk_ocr("ngtr", "&ngtr;", "&#x226F;", ngtr_code);
    mk_ocr("nleq", "&nleq;", "&#x2270;", nleq_code);
    mk_ocr("nless", "&nless;", "&#x226E;", nless_code);
    mk_ocr("nprec", "&nprec;", "&#x2280;", nprec_code);
    mk_ocr("nsim", "&nsim;", "&#x2241;", nsim_code);
    mk_ocr("nsucc", "&nsucc;", "&#x2281;", nsucc_code);
    mk_ocr("precnapprox", "&precnapprox;", "&#x2AB9;", precnapprox_code);
    mk_ocr("precneqq", "&precneqq;", "&#x2AB5;", precneqq_code);
    mk_ocr("precnsim", "&precnsim;", "&#x22E8;", precnsim_code);
    mk_ocr("precsim", "&precsim;", "&#x227E;", precsim_code);
    mk_ocr("succnapprox", "&succnapprox;", "&#x2ABA;", succnapprox_code);
    mk_ocr("succneqq", "&succneqq;", "&#x2AB6;", succneqq_code);
    mk_ocr("succnsim", "&succnsim;", "&#x22E9;", succnsim_code);
    mk_ocr("inplus", "&loplus;", "&#x2A2D;", inplus_code);
    mk_ocr("niplus", "&roplus;", "&#x2A2E;", niplus_code);
    mk_ocr("ntrianglerighteqslant", "&ntrianglerighteq;", "&#x22ED;", ntrianglerighteqslant_code);
    mk_ocr("ntrianglelefteqslant", "&ntrianglelefteq;", "&#x22EC;", ntrianglelefteqslant_code);
    mk_ocr("subseteqq", "&subseteqq;", "&#x2AC5;", subseteqq_code);
    mk_ocr("supseteqq", "&supseteqq;", "&#x2AC6;", supseteqq_code);
    mk_ocr("vartriangleleft", "&vartriangleleft;", "&#x22B2;", vartriangleleft_code);
    mk_ocr("vartriangleright", "&vartriangleright;", "&#x22B3;", vartriangleright_code);
    mk_ocr("trianglerighteq", "&trianglerighteq;", "&#x22B5;", trianglerighteq_code);
    mk_ocr("trianglelefteq", "&trianglelefteq;", "&#x22B4;", trianglelefteq_code);
    mk_ocr("nsubseteq", "&nsubseteq;", "&#x2288;", nsubseteq_code);
    mk_ocr("nsupseteq", "&nsupseteq;", "&#x2289;", nsupseteq_code);
    mk_ocr("ntrianglelefteq", "&ntrianglelefteq;", "&#x22EC;", ntrianglelefteq_code);
    mk_ocr("ntrianglerighteq", "&ntrianglerighteq;", "&#x22ED;", ntrianglerighteq_code);
    mk_ocr("ntriangleleft", "&ntriangleleft;", "&#x22EA;", ntriangleleft_code);
    mk_ocr("ntriangleright", "&ntriangleright;", "&#x22EB;", ntriangleright_code);
    mk_ocr("subsetneqq", "&subsetneqq;", "&#x2ACB;", subsetneqq_code);
    mk_ocr("supsetneqq", "&supsetneqq;", "&#x2ACC;", supsetneqq_code);
    mk_ocr("subsetneq", "&subsetneq;", "&#x228A;", subsetneq_code);
    mk_ocr("supsetneq", "&supsetneq;", "&#x228B;", supsetneq_code);

    mk_oc("rrbracket", "&RightDoubleBracket;", "&#x301B;", rrbracket_code);
    mk_oc("llbracket", "&LeftDoubleBracket;", "&#x301A;", llbracket_code);
    mk_oc("binampersand", "&amp;", "&amp;", binampersand_code);
    // duplicate
    mk_oc("bindnasrepma", "&#x214B;", "&#x214B;", bindnasrepma_code);
    mk_oc("llparenthesis", "&#x2987;", "&#x2987;", llparenthesis_code);
    mk_oc("rrparenthesis", "&#x2988;", "&#x2988;", rrparenthesis_code);

    // mk_oc("ldots","...");
    mk_oc("cdots", "&ctdot;", "&#x022EF;", cdots_code, mathinner_cmd, false);
    //  mk_oc("dots","&ctdot;");
    mk_oc("hdots", "&ctdot;", "&#x022EF;", hdots_code, mathinner_cmd, false);
    mk_oc("vdots", "&vellip;", "&#x022EE;", vdots_code, mathinner_cmd, false);
    mk_oc("ddots", "&dtdot;", "&#x022F1;", ddots_code, mathinner_cmd, false);
    mk_oc("imath", "&imath;", "&#x00131;", imath_code, mathord_cmd, false);
    mk_oc("jmath", "&jmath;", "&#x0006A;", jmath_code, mathord_cmd, false);
    mk_oc("@Vert", "&Verbar;", "&#x02016;", Vertx_code, mathbetween_cmd, false);
    mk_oc("|", "&parallel;", "&#x02225;", Vert_code, mathbetween_cmd, false);
    mk_oc("Vert", "&parallel;", "&#x02225;", Vert_code, mathbetween_cmd, false);
    eval_let("Arrowvert", "Vert");
    mk_oc("lVert", "&parallel;", "&#x02225;", Vert_code, mathopen_cmd, false);
    mk_oc("rVert", "&parallel;", "&#x02225;", Vert_code, mathclose_cmd, false);
    mk_oc("vert", "&vert;", "&#x7C;", vert_code, mathbetween_cmd, false);
    eval_let("arrowvert", "vert");
    eval_let("bracevert", "vert"); // U+23AA ?
    mk_oc("lvert", "&vert;", "&#x7C;", vert_code, mathopen_cmd, false);
    mk_oc("rvert", "&vert;", "&#x7C;", vert_code, mathclose_cmd, false);
    mk_oc("uparrow", "&uparrow;", "&#x02191;", uparrow_code, mathbetween_cmd, true);
    mk_oc("downarrow", "&downarrow;", "&#x02193;", downarrow_code, mathbetween_cmd, true);
    mk_oc("Uparrow", "&Uparrow;", "&#x021D1;", Uparrow_code, mathbetween_cmd, true);
    mk_oc("Downarrow", "&Downarrow;", "&#x021D3;", Downarrow_code, mathbetween_cmd, true);
    mk_oc("Updownarrow", "&Updownarrow;", "&#x021D5;", Updownarrow_code, mathbetween_cmd, true);
    mk_oc("updownarrow", "&updownarrow;", "&#x02195;", updownarrow_code, mathbetween_cmd, true);
    mk_oc("backslash", "&Backslash;", "&#x2216;", backslash_code, mathbetween_cmd, false);
    mk_oc("mho", "&mho;", "&#x02127;", mho_code);
    mk_oc("DifferentialD", "&DifferentialD;", "&#x2146;", differentiald_code);
    mk_oc("leadsto", "&#x21DD;", "&#x21DD;", leadsto_code);
    mk_oc("lhd", "&#x22B2;", "&#x22B2;", lhd_code);
    mk_oc("rhd", "&#x22B3;", "&#x22B3;", rhd_code);
    mk_oc("unlhd", "&#x22B4;", "&#x22B4;", unlhd_code);
    mk_oc("unrhd", "&#x22B5;", "&#x22B5;", unrhd_code);
    mk_oc("clubsuit", "&clubsuit;", "&#x02663;", clubsuit_code);
    mk_oc("diamondsuit", "&diamondsuit;", "&#x02666;", diamondsuit_code);
    mk_oc("heartsuit", "&heartsuit;", "&#x02665;", heartsuit_code);
    mk_oc("spadesuit", "&spadesuit;", "&#x02660;", spadesuit_code);
    mk_oc("Box", "&#x25A1;", "&#x25A1;", Box_code);
    mk_oc("rangle", "&rangle;", RANGLE, rangle_code, mathclose_cmd, true);
    mk_oc("langle", "&langle;", LANGLE, langle_code, mathopen_cmd, true);
    mk_oc("varrangle", "&rangle;", "&#x27E9;", varrangle_code, mathclose_cmd, false);
    mk_oc("varlangle", "&langle;", "&#x27E8;", varlangle_code, mathopen_cmd, false);
    mk_oc("rmoustache", "&rmoustache;", "&#x023B1;", rmoustache_code, mathclose_cmd, false);
    mk_oc("lmoustache", "&lmoustache;", "&#x023B0;", lmoustache_code, mathopen_cmd, false);
    mk_oc("rgroup", "&#x3015;", "&#x3015;", rgroup_code, mathclose_cmd, false);
    mk_oc("lgroup", "&#x3014;", "&#x3014;", lgroup_code, mathopen_cmd, false);
    mk_oc("rbrace", "&rbrace;", "&#x0007D;", rbrace_code, mathclose_cmd, true);
    mk_oc("lbrace", "&lbrace;", "&#x0007B;", lbrace_code, mathopen_cmd, true);
    mk_oc("lceil", "&lceil;", "&#x02308;", lceil_code, mathopen_cmd, false);
    mk_oc("rceil", "&rceil;", "&#x02309;", rceil_code, mathclose_cmd, false);
    mk_oc("lfloor", "&lfloor;", "&#x0230A;", lfloor_code, mathopen_cmd, false);
    mk_oc("rfloor", "&rfloor;", "&#x0230B;", rfloor_code, mathclose_cmd, false);

    mk_oc("square", "&square;", "&#x025A1;", square_code);
    mk_oc("cong", "&cong;", "&#x02245;", cong_code);
    mk_oc("ncong", "&#x2247;", "&#x02247;", ncong_code);
    mk_oc("lnot", "&not;", "&#xAC;", not_code);
    eval_let("neg", "lnot");

    mk_ocb("in", "&Element;", "&#x02208;", in_code);
    mk_gen("math{", "{", "{", open_brace_code, "mo", mathopen_cmd, true);
    mk_gen("math}", "}", "}", close_brace_code, "mo", mathclose_cmd, true);
    mk_oc("prime", "&apos;", "&#x27;", prime_code);
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
    if (global_state.bad_minus)
        TM_mk("-", "&#x2013;", mt_flag_bin);
    else
        TM_mk("-", "-", mt_flag_bin);
    TM_mk("/", "/", mt_flag_bin);
    TM_mk("=", "=", mt_flag_rel);
    TM_mk("&", "&amp;", mt_flag_small);

    eval_let("varbigtriangledown", "bigtriangledown");
    eval_let("varbigtriangleup", "bigtriangleup");
    eval_let("bigcurlyvee", "curlyvee");
    eval_let("bigcurlywedge", "curlywedge");
    eval_let("varcurlyvee", "curlyvee");
    eval_let("varcurlywedge", "curlywedge");
    eval_let("doublecup", "Cup");
    eval_let("doublecap", "Cap");
    eval_let("smallsetminus", "setminus");
    eval_let("boxempty", "square");
    eval_let("oblong", "square");
    eval_let("oast", "circledast");
    eval_let("ocircle", "circledcirc");
    eval_let("varobar", "obar");
    eval_let("varodot", "odot");
    eval_let("varoast", "oast");
    eval_let("varominus", "ominus");
    eval_let("varoplus", "oplus");
    eval_let("varotimes", "otimes");
    eval_let("varoslash", "oslash");
    eval_let("varbigcirc", "bigcirc");
    eval_let("varobslash", "oslash");
    eval_let("varogreaterthan", "ogreaterthan");
    eval_let("varolessthan", "olessthan");
    eval_let("bigbox", "square");
    eval_let("biginterleave", "interleave");
    eval_let("bignplus", "nplus");
    eval_let("bigparallel", "parallel");
    eval_let("bigsqcap", "sqcap");
    eval_let("mathellipsis", "ldots");
    eval_let("restriction", "upharpoonright");
    eval_let("shortdownarrow", "downarrow");
    eval_let("shortleftarrow", "leftarrow");
    eval_let("shortrightarrow", "rightarrow");
    eval_let("shortuparrow", "uparrow");
    eval_let("thickapprox", "approx");
    eval_let("thicksim", "sim");
    eval_let("vartimes", "times");
    init_builtin(xml_zero_space_loc, mk_space("0.3pt"));
    init_builtin(xml_thinmu_space_loc, mk_space("0.166667em"));
    init_builtin(xml_medmu_space_loc, mk_space("0.222222em"));
    init_builtin(xml_thickmu_space_loc, mk_space("0.277778em"));
    init_builtin(xml_thinmuneg_space_loc, mk_space("-0.166667em"));
    // This is not a quad, but the space of tilde
    init_builtin(xml_quad_space_loc, mk_space("3.33333pt"));
    //  mk_space("enspace", xml_thinmu_space_loc);
    // mk_space("zerospace", xml_zero_space_loc);
    mk_space(">", xml_medmu_space_loc);
    mk_space(";", xml_thickmu_space_loc);
    mk_space(":", xml_medmu_space_loc);
    init_builtin(math_c_loc + '~', xml_quad_space_loc);
    math_char_type[uchar('~')] = mt_flag_space;
    boot_xml_lr_tables();
    boot2();
}

// 338 (big ) 337 (small) negation postfix
