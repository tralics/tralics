// tralics, a LaTeX to XML translator
// Copyright INRIA (Jose Grimm) 2002, 2004, 2007,2008

// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use,
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info".
// (See the file COPYING in the main directory for details)

// InterDepositDigitalNumber = IDDN.FR.001.510030.000.S.P.2002.000.31235
// for version 1.6, dated 05/12/2002.
// InterDepositDigitalNumber = IDDN.FR.001.510030.001.S.P.2002.000.31235
// for version 2.9.4, dated 18/02/2007.

// The file contains the main data structures, and code to fill the tables.

#include "tralics/globals.h"
#include "txinline.h"
#include "txparser.h"
#include <ctime>

namespace main_ns {
    HalfLogger log_or_tty;              // the logger
    int        nb_errs = 0;             // number of errors seen so far
    Buffer     path_buffer;             // Result of search_in_path is here
    bool       no_xml_error    = false; // should an error produce an XML element ?
    bool       use_quotes      = false; // How to translate quotes
    bool       bib_allow_break = true;  // <allowbreak> in bibliography
} // namespace main_ns

FullLogger                   log_and_tty;             // the logger
Logger &                     the_log = log_and_tty.L; // the log file part of the logger.
MainClass *                  the_main;                // the main class data structure
Parser                       the_parser;              // the big object
std::array<Istring, np_last> the_names;

// variables declared static in some files.
bool InUrlHandler::global_in_url   = false;
bool InLoadHandler::global_in_load = false;

// local variables
namespace {
    bool ra_ok = true; // inhibits  redefinitions
} // namespace

namespace accent_ns {
    void boot_accents();
} // namespace accent_ns

namespace tralics_ns {
    auto twodig(int n) -> String;
} // namespace tralics_ns
using namespace tralics_ns;

// This is the main function. It does the following:
// print a banner, boot the parser and the math part,
// and execute the main prog.
auto main(int argc, char **argv) -> int {
    MainClass M;
    the_main    = &M;
    M.the_stack = &the_parser.the_stack;
    M.run(argc, argv);
    return main_ns::nb_errs == 0 ? 0 : 2;
}

// Puts char k, with letter catcode, in the table T at position k.
inline void mk_letter(Token *T, uchar k) { T[k] = Token(letter_t_offset, k); }

// In verbatim mode, all chars are of catcode 12, with these exceptions.
// that are of catcode 11.
void Parser::boot_verbatim() {
    Token *T = verbatim_chars.data();
    for (uint i = 0; i < nb_characters; i++) T[i] = Token(other_t_offset, codepoint(i));
    T[uint(' ')] = hash_table.tilda_token;
    mk_letter(T, '\'');
    mk_letter(T, '`');
    mk_letter(T, '-');
    mk_letter(T, '<');
    mk_letter(T, '>');
    mk_letter(T, '~');
    mk_letter(T, '&');
    mk_letter(T, ':');
    mk_letter(T, ';');
    mk_letter(T, '?');
    mk_letter(T, '!');
    mk_letter(T, '"');
    mk_letter(T, uchar('\253'));
    mk_letter(T, uchar('\273'));
}

// The list of characters for which \xspace should not add space.
// Only 7bit characters are in the list
void Parser::boot_xspace() {
    for (bool &i : ok_for_xspace) i = false;
    ok_for_xspace[uchar('.')]  = true;
    ok_for_xspace[uchar('!')]  = true;
    ok_for_xspace[uchar(',')]  = true;
    ok_for_xspace[uchar(':')]  = true;
    ok_for_xspace[uchar(';')]  = true;
    ok_for_xspace[uchar('?')]  = true;
    ok_for_xspace[uchar('/')]  = true;
    ok_for_xspace[uchar('\'')] = true;
    ok_for_xspace[uchar(')')]  = true;
    ok_for_xspace[uchar('-')]  = true;
    ok_for_xspace[uchar('~')]  = true;
}

// used below for printing numbersin dates  with 2 digits
auto tralics_ns::twodig(int n) -> String { return n < 10 ? "0" : ""; }

// This computes the current date (first thing done after printing the banner)
// stores the date in the table of equivalents, computes the default year
// for the RA.
void Parser::boot_time() {
    time_t xx;
    time(&xx);
    struct tm *x        = localtime(&xx);
    int        sec      = x->tm_sec;
    int        min      = x->tm_min;
    int        hour     = x->tm_hour;
    int        the_time = min + hour * 60;
    int        year     = x->tm_year + 1900;
    int        month    = x->tm_mon + 1;
    int        day      = x->tm_mday;
    eqtb_int_table[time_code].set_val(the_time);
    eqtb_int_table[day_code].set_val(day);
    eqtb_int_table[month_code].set_val(month);
    eqtb_int_table[year_code].set_val(year);
    std::srand(static_cast<unsigned>(sec + 60 * (min + 60 * (hour + 24 * (day + 31 * month)))));
    Buffer b;
    b << year << '/' << twodig(month) << month << '/' << twodig(day) << day;
    std::string short_date = b.to_string();
    b << bf_reset << short_date << ' ' << twodig(hour) << hour << ':' << twodig(min) << min << ':' << twodig(sec) << sec;
    TokenList today_tokens = b.str_toks(nlt_space);
    new_prim("today", today_tokens);
    b << bf_reset << "Start compilation: " << short_date << ' ' << twodig(hour) << hour << ':' << twodig(min) << min << ':' << twodig(sec)
      << sec << "\n";
    the_main->set_start_date(b.to_string());
    the_main->set_short_date(short_date);
    // Default year for the raweb. Until April its last year
    if (month <= 4) year--;
    the_parser.set_ra_year(year);
}

// Installs the default language.
void Parser::set_default_language(int v) {
    eqtb_int_table[language_code].set_val(v);
    eqtb_int_table[language_code].set_level(1);
    set_def_language_num(v);
}

// Creates some little constants.
void Parser::make_constants() {
    hash_table.OB_token      = make_char_token('{', 1);
    hash_table.CB_token      = make_char_token('}', 2);
    hash_table.dollar_token  = make_char_token('$', 3);
    hash_table.hat_token     = Token(hat_t_offset, '^');
    hash_table.zero_token    = Token(other_t_offset, '0');
    hash_table.one_token     = Token(other_t_offset, '1');
    hash_table.comma_token   = Token(other_t_offset, ',');
    hash_table.equals_token  = Token(other_t_offset, '=');
    hash_table.dot_token     = Token(other_t_offset, '.');
    hash_table.star_token    = Token(other_t_offset, '*');
    hash_table.plus_token    = Token(other_t_offset, '+');
    hash_table.minus_token   = Token(other_t_offset, '-');
    hash_table.space_token   = Token(space_token_val);
    hash_table.newline_token = Token(newline_token_val);
}

// This is for lower-case upper-case conversions.
// Defines values for the character c
void Parser::mklcuc(size_t c, size_t lc, size_t uc) {
    eqtb_int_table[c + lc_code_offset].set_val(static_cast<int>(lc));
    eqtb_int_table[c + uc_code_offset].set_val(static_cast<int>(uc));
}

// This is for lower-case upper-case conversions.
// Defines values for the pair lc, uc
void Parser::mklcuc(size_t lc, size_t uc) {
    eqtb_int_table[lc + lc_code_offset].set_val(static_cast<int>(lc));
    eqtb_int_table[lc + uc_code_offset].set_val(static_cast<int>(uc));
    eqtb_int_table[uc + lc_code_offset].set_val(static_cast<int>(lc));
    eqtb_int_table[uc + uc_code_offset].set_val(static_cast<int>(uc));
}

// This creates the lc and uc tables.
// Only ascii characters have a non-zero value
void Parser::make_uclc_table() {
    for (unsigned i = 'a'; i <= 'z'; i++) mklcuc(i, i, i - 32);
    for (unsigned i = 224; i <= 255; i++) mklcuc(i, i, i - 32);
    for (unsigned i = 'A'; i <= 'Z'; i++) mklcuc(i, i + 32, i);
    for (unsigned i = 192; i <= 223; i++) mklcuc(i, i + 32, i);
    mklcuc(215, 0, 0);
    mklcuc(247, 0, 0);    // multiplication division (\367 \327)
    mklcuc(223, 0, 0);    // sharp s (\377 347)
    mklcuc(0xFF, 0x178);  // \"y
    mklcuc(0x153, 0x152); // oe
    mklcuc(0x133, 0x132); // ij
    mklcuc(0x142, 0x141); // \l
    mklcuc(0x14B, 0x14A); // \ng
    mklcuc(0x111, 0x110); // \dh
}

// Locations 2N and 2N+1 are lowercase/uppercase equivalent.
void Parser::boot_uclc() {
    uclc_list[0]  = hash_table.locate("oe");
    uclc_list[1]  = hash_table.locate("OE");
    uclc_list[2]  = hash_table.locate("o");
    uclc_list[3]  = hash_table.locate("O");
    uclc_list[4]  = hash_table.locate("ae");
    uclc_list[5]  = hash_table.locate("AE");
    uclc_list[6]  = hash_table.locate("dh");
    uclc_list[7]  = hash_table.locate("DH");
    uclc_list[8]  = hash_table.locate("dj");
    uclc_list[9]  = hash_table.locate("DJ");
    uclc_list[10] = hash_table.locate("l");
    uclc_list[11] = hash_table.locate("L");
    uclc_list[12] = hash_table.locate("ng");
    uclc_list[13] = hash_table.locate("NG");
    uclc_list[14] = hash_table.locate("ss");
    uclc_list[15] = hash_table.locate("SS");
    uclc_list[16] = hash_table.locate("th");
    uclc_list[17] = hash_table.locate("TH");
    uclc_list[18] = hash_table.locate("i");
    uclc_list[19] = hash_table.locate("I");
    uclc_list[20] = hash_table.locate("j");
    uclc_list[21] = hash_table.locate("J");
}

// ------------------------------------------------------------

// This function is called after all primitives are constructed.
// We add some new macros.

void Parser::load_latex() {
    make_uclc_table();
    TokenList body;
    new_prim("@empty", body);
    hash_table.empty_token = hash_table.locate("@empty");
    // Special raweb compatibility hacks
    if (the_main->in_ra()) ra_ok = false;
    // this is like \let\bgroup={, etc
    hash_table.primitive("bgroup", open_catcode, subtypes('{'));
    hash_table.primitive("egroup", close_catcode, subtypes('}'));
    hash_table.primitive("sp", hat_catcode, subtypes('^'));
    hash_table.primitive("sb", underscore_catcode, subtypes('_'));
    hash_table.primitive("@sptoken", space_catcode, subtypes(' '));
    eqtb_int_table[escapechar_code].set_val('\\');
    eqtb_dim_table[unitlength_code].set_val(ScaledInt(1 << 16));
    eqtb_dim_table[textwidth_code].set_val(ScaledInt(427 << 16));
    eqtb_dim_table[linewidth_code].set_val(ScaledInt(427 << 16));
    eqtb_dim_table[textwidth_code].set_val(ScaledInt(427 << 16));
    eqtb_dim_table[columnwidth_code].set_val(ScaledInt(427 << 16));
    eqtb_dim_table[textheight_code].set_val(ScaledInt(570 << 16));
    eqtb_int_table[pretolerance_code].set_val(100);
    eqtb_int_table[tolerance_code].set_val(200);

    hash_table.eval_let("endgraf", "par");
    hash_table.eval_let("@@par", "par");
    hash_table.eval_let("endline", "cr");
    hash_table.eval_let("epsffile", "epsfbox");
    hash_table.eval_let("epsfig", "psfig");
    hash_table.eval_let("newsavebox", "newbox");
    hash_table.eval_let("@ybegintheorem", "@begintheorem");
    hash_table.eval_let("@@href", "Href");
    hash_table.eval_let("@@italiccorr", "/");
    hash_table.eval_let("verbatimfont", "tt");
    hash_table.eval_let("verbatimnumberfont", "small");
    hash_table.eval_let("empty", "@empty");
    hash_table.eval_let("verbprefix", "@empty");
    hash_table.eval_let("verbatimprefix", "@empty");
    hash_table.eval_let("urlfont", "@empty");
    hash_table.eval_let("@currentlabel", "@empty");
    hash_table.eval_let("bye", "endinput");
    hash_table.eval_let("sortnoop", "@gobble");
    hash_table.eval_let("noopsort", "@gobble");
    hash_table.eval_let("chaptermark", "@gobble");
    hash_table.eval_let("sectionmark", "@gobble");
    hash_table.eval_let("subsectionmark", "@gobble");
    hash_table.eval_let("subsubsectionmark", "@gobble");
    hash_table.eval_let("paragraphmark", "@gobble");
    hash_table.eval_let("subparagraphmark", "@gobble");
    hash_table.eval_let("dotsc", "cdots");
    hash_table.eval_let("dotso", "cdots");
    hash_table.eval_let("dotsb", "cdots");
    hash_table.eval_let("dotsi", "cdots");
    hash_table.eval_let("dotsm", "cdots");
    hash_table.eval_let("mathbm", "mathbb");
    hash_table.eval_let("mathbbm", "mathbb");
    hash_table.eval_let("pounds", "textsterling");
    hash_table.eval_let("@itemlabel", "relax");
    hash_table.eval_let("default@ds", "OptionNotUsed");
    hash_table.eval_let("texttwelveudash", "textemdash");
    hash_table.eval_let("textthreequartersemdash", "textemdash");
    hash_table.eval_let("textminus", "textemdash");
    hash_table.eval_let("texorpdfstring", "@firstoftwo");
    hash_table.eval_let("m@th", "relax");
    hash_table.eval_let("mathdollar", "$");
    hash_table.eval_let("mathsterling", "pounds");
    hash_table.eval_let("varcopyright", "copyright");
    hash_table.eval_let("@typeset@protect", "relax");
    hash_table.eval_let("protect", "relax");
    hash_table.eval_let("topfigrule", "relax");
    hash_table.eval_let("botfigrule", "relax");
    hash_table.eval_let("dblfigrule", "relax");
    hash_table.eval_let("mathalpha", "relax");
    hash_table.eval_let("@iden", "@firstofone");
    hash_table.eval_let("reset@font", "normalfont");

    // commands like \countdef\foo=\bar
    shorthand_gdefine(count_def_code, "count@", 255);
    shorthand_gdefine(count_def_code, "c@page", 0);
    shorthand_gdefine(dimen_def_code, "dimen@", 0);
    shorthand_gdefine(dimen_def_code, "dimen@i", 1);
    shorthand_gdefine(dimen_def_code, "dimen@ii", 2);
    shorthand_gdefine(char_def_code, "@ne", 1);
    shorthand_gdefine(char_def_code, "tw@", 2);
    shorthand_gdefine(char_def_code, "thr@@", 3);
    shorthand_gdefine(char_def_code, "sixt@@n", 16);
    shorthand_gdefine(char_def_code, "@cclv", 255);
    shorthand_gdefine(math_char_def_code, "@cclvi", 256);
    shorthand_gdefine(math_char_def_code, "@m", 1000);
    shorthand_gdefine(math_char_def_code, "@M", 10000);
    shorthand_gdefine(math_char_def_code, "@Mi", 10001);
    shorthand_gdefine(math_char_def_code, "@Mii", 10002);
    shorthand_gdefine(math_char_def_code, "@Miii", 10003);
    shorthand_gdefine(math_char_def_code, "@Miv", 10004);
    shorthand_gdefine(math_char_def_code, "@MM", 20000);
    shorthand_gdefine(char_def_code, "active", 13);
    shorthand_gdefine(toks_def_code, "toks@", 0);
    shorthand_gdefine(skip_def_code, "skip@", 0);
    // commands like \newdimen\foo
    make_token("epsfxsize");
    new_constant(newdimen_code);
    make_token("epsfysize");
    new_constant(newdimen_code);
    make_token("z@");
    new_constant(newdimen_code);
    make_token("p@");
    new_constant(newdimen_code);
    make_token("oddsidemargin");
    new_constant(newdimen_code);
    make_token("evensidemargin");
    new_constant(newdimen_code);
    make_token("leftmargin");
    new_constant(newdimen_code);
    make_token("rightmargin");
    new_constant(newdimen_code);
    make_token("leftmargini");
    new_constant(newdimen_code);
    make_token("leftmarginii");
    new_constant(newdimen_code);
    make_token("leftmarginiii");
    new_constant(newdimen_code);
    make_token("leftmarginiv");
    new_constant(newdimen_code);
    make_token("leftmarginv");
    new_constant(newdimen_code);
    make_token("leftmarginvi");
    new_constant(newdimen_code);
    make_token("itemindent");
    new_constant(newdimen_code);
    make_token("labelwidth");
    new_constant(newdimen_code);
    make_token("fboxsep");
    new_constant(newdimen_code);
    make_token("fboxrule");
    new_constant(newdimen_code);
    make_token("arraycolsep");
    new_constant(newdimen_code);
    make_token("tabcolsep");
    new_constant(newdimen_code);
    make_token("arrayrulewidth");
    new_constant(newdimen_code);
    make_token("doublerulesep");
    new_constant(newdimen_code);
    make_token("@tempdima");
    new_constant(newdimen_code);
    make_token("@tempdimb");
    new_constant(newdimen_code);
    make_token("@tempdimc");
    new_constant(newdimen_code);
    make_token("footnotesep");
    new_constant(newdimen_code);
    make_token("topmargin");
    new_constant(newdimen_code);
    make_token("headheight");
    new_constant(newdimen_code);
    make_token("headsep");
    new_constant(newdimen_code);
    make_token("footskip");
    new_constant(newdimen_code);
    make_token("columnsep");
    new_constant(newdimen_code);
    make_token("columnseprule");
    new_constant(newdimen_code);
    make_token("marginparwidth");
    new_constant(newdimen_code);
    make_token("marginparsep");
    new_constant(newdimen_code);
    make_token("marginparpush");
    new_constant(newdimen_code);
    make_token("maxdimen");
    new_constant(newdimen_code);
    make_token("normallineskiplimit");
    new_constant(newdimen_code);
    make_token("jot");
    new_constant(newdimen_code);
    make_token("paperheight");
    new_constant(newdimen_code);
    make_token("paperwidth");
    new_constant(newdimen_code);
    // commands like \newskip\foo
    make_token("topsep");
    new_constant(newlength_code);
    make_token("partopsep");
    new_constant(newlength_code);
    make_token("itemsep");
    new_constant(newlength_code);
    make_token("labelsep");
    new_constant(newlength_code);
    make_token("parsep");
    new_constant(newlength_code);
    make_token("fill");
    new_constant(newlength_code);
    make_token("@tempskipa");
    new_constant(newlength_code);
    make_token("@tempskipb");
    new_constant(newlength_code);
    make_token("@flushglue");
    new_constant(newlength_code);
    make_token("listparindent");
    new_constant(newlength_code);
    make_token("hideskip");
    new_constant(newlength_code);
    make_token("z@skip");
    new_constant(newlength_code);
    make_token("normalbaselineskip");
    new_constant(newlength_code);
    make_token("normallineskip");
    new_constant(newlength_code);
    make_token("smallskipamount");
    new_constant(newlength_code);
    make_token("medskipamount");
    new_constant(newlength_code);
    make_token("bigskipamount");
    new_constant(newlength_code);
    make_token("floatsep");
    new_constant(newlength_code);
    make_token("textfloatsep");
    new_constant(newlength_code);
    make_token("intextsep");
    new_constant(newlength_code);
    make_token("dblfloatsep");
    new_constant(newlength_code);
    make_token("dbltextfloatsep");
    new_constant(newlength_code);
    // commands like \newcounter\foo
    shorthand_gdefine(count_def_code, "m@ne", 20);
    counter_boot("FancyVerbLine", ""); // hard-coded 21
    counter_boot("enumi", "");
    counter_boot("enumii", "");
    counter_boot("enumiii", "");
    counter_boot("enumiv", "");
    counter_boot("enumv", "");
    counter_boot("enumvi", "");
    counter_boot("enumvii", "");
    counter_boot("enumviii", "");
    counter_boot("enumix", "");
    counter_boot("enumx", "");
    counter_boot("Enumi", "");
    counter_boot("Enumii", "");
    counter_boot("Enumiii", "");
    counter_boot("Enumiv", "");
    counter_boot("Enumv", "");
    counter_boot("Enumvi", "");
    counter_boot("Enumvii", "");
    counter_boot("Enumviii", "");
    counter_boot("Enumix", "");
    counter_boot("Enumx", "");
    counter_boot("tocdepth", ""); // This is 42 ...
    counter_boot("footnote", "");
    counter_boot("part", "");
    counter_boot("chapter", "");
    counter_boot("section", "chapter");
    counter_boot("subsection", "section");
    counter_boot("subsubsection", "subsection");
    counter_boot("paragraph", "subsubsection");
    counter_boot("subparagraph", "paragraph");
    counter_boot("mpfootnote", "");
    counter_boot("table", "");
    counter_boot("figure", "");
    counter_boot("subfigure", "figure");
    counter_boot("equation", "");
    equation_ctr_pos = allocation_table[newcount_code] + count_reg_offset;
    counter_boot("parentequation", "");

    // \newcount
    make_token("c@bottomnumber");
    new_constant(newcount_code);
    make_token("c@topnumber");
    new_constant(newcount_code);
    make_token("@tempcnta");
    new_constant(newcount_code);
    make_token("@tempcntb");
    new_constant(newcount_code);
    make_token("c@totalnumber");
    new_constant(newcount_code);
    make_token("c@dbltopnumber");
    new_constant(newcount_code);
    make_token("interfootnotelinepenalty");
    new_constant(newcount_code);
    make_token("interdisplaylinepenalty");
    new_constant(newcount_code);
    // \newtoks
    make_token("@temptokena");
    new_constant(newtoks_code);
    // \newbox
    make_token("@tempboxa");
    new_constant(newbox_code);
    make_token("voidb@x");
    new_constant(newbox_code);
    // \newif
    make_token("ifin@");
    M_newif();
    make_token("if@tempswa");
    M_newif();

    // Define now some other macros
    new_prim("lq", "`");
    new_prim("rq", "'");
    new_prim("lbrack", "[");
    new_prim("rbrack", "]");
    new_prim("xscale", "1.0");
    new_prim("yscale", "1.0");
    new_prim("xscaley", "0.0");
    new_prim("yscalex", "0.0");
    new_prim("@height", "height");
    new_prim("@depth", "depth");
    new_prim("@width", "width");
    new_prim("@plus", "plus");
    new_prim("@minus", "minus");
    new_prim("I", "I");
    new_prim("J", "J");
    new_prim("textfraction", ".2");
    new_prim("floatpagefraction", ".5");
    new_prim("dblfloatpagefraction", ".5");
    new_prim("bottomfraction", ".3");
    new_prim("dbltopfraction", ".7");
    new_prim("topfraction", ".7");
    new_prim("space", " ");
    hash_table.eval_let("@headercr", "space");
    new_prim("cl@page", "");
    new_prim("refname", "Bibliography");
    new_prim("footcitesep", ", ");
    new_prim("citepunct", ", ");
    new_prim("encodingdefault", "T1");
    new_prim("familydefault", "cmr");
    new_prim("seriesdefault", "m");
    new_prim("shapedefault", "n");
    new_prim("fmtname", "Tralics");
    new_prim("@vpt", "5");
    new_prim("@vipt", "6");
    new_prim("@viipt", "7");
    new_prim("@viiipt", "8");
    new_prim("@ixpt", "9");
    new_prim("@xpt", "10");
    new_prim("@xipt", "10.95");
    new_prim("@xiipt", "12");
    new_prim("@xivpt", "14.4");
    new_prim("@xviipt", "17.28");
    new_prim("@xxpt", "20.74");
    new_prim("@xxvpt", "24.88");
    new_prim("baselinestretch", "1");
    new_prim("arraystretch", "1");
    new_prim("NAT@open", "(");
    new_prim("NAT@sep", ";");
    new_prim("NAT@close", ")");
    new_prim("NAT@cmt", ", ");
    {
        TokenList L;
        L.push_back(Token(letter_t_offset, codepoint('&')));
        new_prim("amp", L);
    }
    more_bootstrap();
    LinePtr L;
    L.insert("%% Begin bootstrap commands for latex");
    eqtb_int_table[uchar('@')].set_val(11); // this is \makeatletter
    // initialise counters, dimen etc
    L.insert("\\@flushglue = 0pt plus 1fil");
    L.insert("\\hideskip =-1000pt plus 1fill");
    L.insert("\\smallskipamount=3pt plus 1pt minus 1pt\n");
    L.insert("\\medskipamount=6pt plus 2pt minus 2pt\n");
    L.insert("\\bigskipamount=12pt plus 4pt minus 4pt\n");
    L.insert(R"(\z@=0pt\p@=1pt\m@ne=-1 \fboxsep = 3pt %)");
    L.insert(R"(\c@page=1 \fill = 0pt plus 1fill \setcounter{tocdepth}{2})");
    L.insert("\\paperheight=297mm\\paperwidth=210mm");
    L.insert("\\jot=3pt\\maxdimen=16383.99999pt");
    // Other definitions
    L.insert(R"(\def\newfont#1#2{\font#1=#2\relax})");
    L.insert(R"(\def\symbol#1{\char #1\relax})");
    L.insert(R"({\catcode`\&=13\global\def&{\char38 }}%)");
    L.insert(R"(\catcode`\^^L=13 \outer\def^^L{\par})");
    L.insert(R"(\def~{\nobreakspace}\def^^a0{\nobreakspace})");
    L.insert(R"(\newenvironment{cases}{\left\{\begin{array}{ll}}{\end{array}\right.}%)");
    L.insert(R"(\def\markboth#1#2{\gdef\@themark{{#1}}\mark{{#1}{#2}}})");
    L.insert(R"(\def\markright#1{\expandafter\markboth\@themark{#1}})");
    L.insert(R"(\def\stretch#1{\z@ \@plus #1fill\relax})");
    L.insert(R"(\theoremstyle{plain}\theoremheaderfont{\bfseries})");
    L.insert(R"(\def\@namedef#1{\expandafter\def\csname #1\endcsname})");
    L.insert(R"(\def\@nameuse#1{\csname #1\endcsname})");
    L.insert(R"(\def\@arabic#1{\number #1})");
    L.insert(R"(\def\@roman#1{\romannumeral#1})");
    L.insert(R"(\def\@Roman#1{\Romannumeral#1})");
    L.insert(R"(\def\@nomathswiii#1#2{\@nomathsws#1\@nomathswm #2\@nomathswe})");
    L.insert(R"(\def\@nomathswiv#1#2#3{\@nomathswiii{#1}{\noexpand#3{#2}}})");
    L.insert(R"(\def\mod#1{\@nomathswiv{~\@mod\;#1}{#1}{\mod}})");
    L.insert(R"(\def\bmod{\@nomathswiii{\;\@mod\;}{\noexpand\bmod}})");
    L.insert(R"(\def\pmod#1{\@nomathswiv{\pod{\@mod\; #1}}{#1}\pmod})");
    L.insert(R"(\def\pod#1{\ifinner\mkern8mu\else\mkern18mu\fi(#1)})");
    L.insert(R"(\def\LaTeXe{\LaTeX2$\epsilon$})");
    L.insert(R"(\def\date#1{\xmlelt{date}{#1}})");
    L.insert(R"(\def\enspace{\kern.5em })");
    L.insert(R"(\def\@makeother#1{\catcode`#1=12\relax})");
    L.insert(R"(\def\@makeactive#1{\catcode`#1=13\relax})");
    L.insert("\\def\\root#1\\of{\\@root{#1}}\n");
    L.insert(R"(\long\def\g@addto@macro#1#2{\begingroup\toks@\expandafter{#1#2}\xdef#1{\the\toks@}\endgroup})");
    L.insert(R"(\long\def\addto@hook#1#2{#1\expandafter{\the#1#2}})");
    L.insert(R"(\def\x@tag#1{\qquad\mathrm{(#1)}})");
    L.insert(R"(\def\y@tag#1{\qquad\mathrm{#1}})");
    L.insert(R"(\def\@x@tag#1{\formulaattribute{tag}{(#1)}})");
    L.insert(R"(\def\@y@tag#1{\formulaattribute{tag}{#1}})");
    L.insert(R"(\def\eqref#1{(\ref{#1})})");
    L.insert(R"(\def\@carcube#1#2#3#4\@nil{#1#2#3})");
    L.insert(R"(\long\def\@thirdofthree#1#2#3{#3})");
    L.insert(R"(\def\on@line{ on input line \the\inputlineno})");
    L.insert(R"(\def\pagenumbering#1{\xmlelt{pagestyle}{\XMLaddatt{numbering}{#1}}})");
    L.insert(R"(\def\pagestyle#1{\xmlelt{pagestyle}{\XMLaddatt{style}{#1}}})");
    L.insert(R"(\def\thispagestyle#1{\xmlelt{pagestyle}{\XMLaddatt{this-style}{#1}}})");
    L.insert(R"(\def\thesubfigure{\thefigure.\@arabic\c@subfigure})");
    L.insert(R"(\def\@filedoterr#1{\error{Wrong dots in graphic file #1}})");
    L.insert(R"(\def\theEnumi{\arabic{Enumi}})");
    L.insert(R"(\def\theEnumii{\theEnumi.\arabic{Enumii}})");
    L.insert(R"(\def\theEnumiii{\theEnumii.\arabic{Enumiii}})");
    L.insert(R"(\def\theEnumiv{\theEnumiii.\arabic{Enumiv}})");
    L.insert(R"(\def\theEnumv{\theEnumiv.\arabic{Enumiv}})");
    L.insert(R"(\def\labelenumi{(\theenumi)})");
    L.insert(R"(\def\labelenumii{(\theenumii)})");
    L.insert(R"(\def\labelenumiii{(\theenumiii)})");
    L.insert(R"(\def\labelenumiv{(\theenumiv)})");
    L.insert(R"(\def\labelenumv{(\theenumv)})");
    L.insert(R"(\def\labelenumvi{(\theenumvi)})");
    L.insert(R"(\def\labelenumvii{(\theenumvii)})");
    L.insert(R"(\def\labelenumviii{(\theenumviii)})");
    L.insert(R"(\def\labelenumix{(\theenumix)})");
    L.insert(R"(\def\labelenumx{(\theenumx)})");
    L.insert(R"(\def\bordermatrix#1{{\let\cr\\)");
    L.insert("\\begin{bordermatrix }#1\\end{bordermatrix }}}");
    L.insert(R"(\def\incr@eqnum{\refstepcounter{equation}})");
    L.insert(R"(\def\@@theequation{\theparentequation\alph{equation}})");

    if (!everyjob_string.empty()) L.insert(everyjob_string, true); // is this converted ?
    L.insert("%% End bootstrap commands for latex");
    init(L);
    translate0();

    eqtb_int_table[uchar('@')].set_val(12); // this is \makeatother
    TokenList ejob = toks_registers[everyjob_code].get_val();
    back_input(ejob);
}

void Parser::more_bootstrap() {
    initialise_font(); // Define current font
    TokenList L;
    Token     T, T1;
    T = hash_table.locate("space");
    L.push_back(T);
    L.push_back(T);
    L.push_back(T);
    L.push_back(T);
    new_prim("@spaces", L);
    {
        int  A    = ' '; // eqtb loc of active space
        auto Bval = T.eqtb_loc();
        eq_define(A, hash_table.eqtb[Bval].get_cmdchr(), true);
        A    = '#';
        Bval = hash_table.locate("#").eqtb_loc();
        eq_define(A, hash_table.eqtb[Bval].get_cmdchr(), true);
        A    = '_';
        Bval = hash_table.locate("_").eqtb_loc();
        eq_define(A, hash_table.eqtb[Bval].get_cmdchr(), true);
        A    = '\r'; // eqtbloc of active end-of-line (^^M)
        Bval = hash_table.par_token.eqtb_loc();
        eq_define(A, hash_table.eqtb[Bval].get_cmdchr(), true);
    }
    L.clear();
    L.push_back(hash_table.par_token);
    new_prim("endsloppypar", L);
    L.push_back(hash_table.locate("sloppy"));
    new_prim("sloppypar", L);
    L.clear();
    brace_me(L);
    T = hash_table.locate("hbox");
    L.push_front(T);
    new_prim("null", L);
    L.clear();
    L.push_back(T);
    L.push_back(Token(letter_t_offset, 't'));
    L.push_back(Token(letter_t_offset, 'o'));
    new_prim("hb@xt@", L);
    L.clear();
    L.push_back(hash_table.locate("@arabic"));
    L.push_back(hash_table.locate("c@page"));
    new_prim("thepage", L);
    new_primx("labelitemi", "textbullet");
    // Original code was : \normalfont\bfseries \textendash
    new_primx("labelitemii", "textendash");
    new_primx("labelitemiii", "textasteriskcentered");
    new_primx("labelitemiv", "textperiodcentered");
    new_primx("guillemets", "guillemotleft");
    new_primx("endguillemets", "guillemotright");
    new_primx("@nnil", "@nil");
    L.clear();
    T  = hash_table.locate("@makeother");
    T1 = hash_table.locate("do");
    TokenList L1;
    Token     w;

    auto ADD_TO_BOTH = [&](String s) {
        w = hash_table.locate(s);
        L.push_back(T);
        L.push_back(w);
        L1.push_back(T1);
        L1.push_back(w);
    };

    ADD_TO_BOTH(" ");
    ADD_TO_BOTH("\\");
    ADD_TO_BOTH("$");
    ADD_TO_BOTH("&");
    ADD_TO_BOTH("#");
    ADD_TO_BOTH("^");
    ADD_TO_BOTH("_");
    ADD_TO_BOTH("%");
    ADD_TO_BOTH("~");
    L1.push_back(T1);
    L1.push_back(hash_table.locate("{"));
    L1.push_back(T1);
    L1.push_back(hash_table.locate("}"));
    new_prim("@sanitize", L);
    new_prim("dospecials", L1);
    T = hash_table.locate("@makeactive");
    L.clear();
    L.push_back(T);
    L.push_back(hash_table.locate(" "));
    new_prim("obeyspaces", L);
    L.clear();
    L.push_back(T);
    L.push_back(hash_table.locate("\r")); // define ^^M
    L.push_back(hash_table.let_token);
    L.push_back(Token(eqtb_offset + '\r'));
    L.push_back(hash_table.par_token);
    new_prim("obeylines", L);
    L.clear();
    L.push_back(T);
    L.push_back(hash_table.locate(" ")); // define ^^M
    L.push_back(hash_table.let_token);
    L.push_back(Token(eqtb_offset + ' '));
    L.push_back(hash_table.locate("nobreakspace"));
    new_prim("@vobeyspaces", L);
}

// This creates the table with all the names.
void tralics_ns::make_names() {
    the_names[cst_empty]        = Istring("");
    the_names[cst_accent]       = Istring("accent");
    the_names[cst_accentunder]  = Istring("accentunder");
    the_names[cst_argument]     = Istring("argument");
    the_names[cst_biblio]       = Istring("thebibliography");
    the_names[cst_composition]  = Istring("composition");
    the_names[cst_displaystyle] = Istring("displaystyle");
    the_names[cst_document]     = Istring("document");
    the_names[cst_elt]          = Istring("ELT");
    the_names[cst_flaghi]       = Istring("'hi_flag");
    the_names[cst_fonts]        = Istring("fonts");
    the_names[cst_foot]         = Istring("foot");
    the_names[cst_hanl]         = Istring("hanl");
    the_names[cst_hbox]         = Istring("hbox");
    the_names[cst_hi]           = Istring("hi");
    the_names[cst_math]         = Istring("math");
    the_names[cst_mathml]       = Istring("http://www.w3.org/1998/Math/MathML");
    the_names[cst_mathvariant]  = Istring("mathvariant");
    the_names[cst_mfenced]      = Istring("mfenced");
    the_names[cst_mfrac]        = Istring("mfrac");
    the_names[cst_mpadded]      = Istring("mpadded");
    the_names[cst_mphantom]     = Istring("mphantom");
    the_names[cst_mi]           = Istring("mi");
    the_names[cst_mn]           = Istring("mn");
    the_names[cst_mo]           = Istring("mo");
    the_names[cst_ci]           = Istring("ci");
    the_names[cst_cn]           = Istring("cn");
    the_names[cst_csymbol]      = Istring("csymbol");
    the_names[cst_multiscripts] = Istring("mmultiscripts");
    the_names[cst_nothing]      = Istring("");
    the_names[cst_mroot]        = Istring("mroot");
    the_names[cst_mover]        = Istring("mover");
    the_names[cst_mrow]         = Istring("mrow");
    the_names[cst_mrow_cst]     = Istring("<mrow></mrow>");
    the_names[cst_mspace]       = Istring("mspace");
    the_names[cst_msqrt]        = Istring("msqrt");
    the_names[cst_msub]         = Istring("msub");
    the_names[cst_mstyle]       = Istring("mstyle");
    the_names[cst_msubsup]      = Istring("msubsup");
    the_names[cst_msup]         = Istring("msup");
    the_names[cst_mtable]       = Istring("mtable");
    the_names[cst_mtd]          = Istring("mtd");
    the_names[cst_mtext]        = Istring("mtext");
    the_names[cst_mtr]          = Istring("mtr");
    the_names[cst_munder]       = Istring("munder");
    the_names[cst_mref]         = Istring("mref");
    the_names[cst_munderover]   = Istring("munderover");
    the_names[cst_mode]         = Istring("mode");
    the_names[cst_display]      = Istring("display");
    the_names[cst_nl]           = Istring("\n");
    the_names[cst_p]            = Istring("p");
    the_names[cst_particip]     = Istring("particip");
    the_names[cst_rasection]    = Istring("RAsection");
    the_names[cst_refer]        = Istring("refer");
    the_names[cst_scriptlevel]  = Istring("scriptlevel");
    the_names[cst_separators]   = Istring("separators");
    the_names[cst_temporary]    = Istring("temporary");
    the_names[cst_theorem]      = Istring("theorem");
    the_names[cst_xmlns]        = Istring("xmlns");

    the_names[cst_dig0] = Istring("0");
    the_names[cst_dig1] = Istring("1");
    the_names[cst_dig2] = Istring("2");
    the_names[cst_dig3] = Istring("3");
    the_names[cst_dig4] = Istring("4");
    the_names[cst_dig5] = Istring("5");
    the_names[cst_dig6] = Istring("6");
    the_names[cst_dig7] = Istring("7");
    the_names[cst_dig8] = Istring("8");
    the_names[cst_dig9] = Istring("9");
    // For the bibliography
    the_names[cstb_article]       = Istring("article");
    the_names[cstb_book]          = Istring("book");
    the_names[cstb_booklet]       = Istring("booklet");
    the_names[cstb_conference]    = Istring("conference");
    the_names[cstb_coursenotes]   = Istring("coursenotes");
    the_names[cstb_comment]       = Istring("comment");
    the_names[cstb_extension]     = Istring("extension");
    the_names[cstb_inbook]        = Istring("inbook");
    the_names[cstb_incollection]  = Istring("incollection");
    the_names[cstb_inproceedings] = Istring("inproceedings");
    the_names[cstb_manual]        = Istring("manual");
    the_names[cstb_masterthesis]  = Istring("mastersthesis");
    the_names[cstb_mastersthesis] = Istring("masterthesis");
    the_names[cstb_misc]          = Istring("misc");
    the_names[cstb_phdthesis]     = Istring("phdthesis");
    the_names[cstb_proceedings]   = Istring("proceedings");
    the_names[cstb_preamble]      = Istring("preamble");
    the_names[cstb_techreport]    = Istring("techreport");
    the_names[cstb_string]        = Istring("string");
    the_names[cstb_unpublished]   = Istring("unpublished");
    the_names[cstb_unknown]       = Istring("unknown");
    the_names[cstb_address]       = Istring("address");
    the_names[cstb_author]        = Istring("author");
    the_names[cstb_booktitle]     = Istring("booktitle");
    the_names[cstb_chapter]       = Istring("chapter");
    the_names[cstb_crossref]      = Istring("crossref");
    the_names[cstb_doi]           = Istring("doi");
    the_names[cstb_edition]       = Istring("edition");
    the_names[cstb_editor]        = Istring("editor");
    the_names[cstb_howpublished]  = Istring("howpublished");
    the_names[cstb_institution]   = Istring("institution");
    the_names[cstb_isbn]          = Istring("isbn");
    the_names[cstb_issn]          = Istring("issn");
    the_names[cstb_isrn]          = Istring("isrn");
    the_names[cstb_journal]       = Istring("journal");
    the_names[cstb_key]           = Istring("key");
    the_names[cstb_month]         = Istring("month");
    the_names[cstb_language]      = Istring("language");
    the_names[cstb_langue]        = Istring("langue");
    the_names[cstb_note]          = Istring("note");
    the_names[cstb_number]        = Istring("number");
    the_names[cstb_organization]  = Istring("organization");
    the_names[cstb_pages]         = Istring("pages");
    the_names[cstb_publisher]     = Istring("publisher");
    the_names[cstb_school]        = Istring("school");
    the_names[cstb_series]        = Istring("series");
    the_names[cstb_title]         = Istring("title");
    the_names[cstb_type]          = Istring("type");
    the_names[cstb_url]           = Istring("url");
    the_names[cstb_volume]        = Istring("volume");
    the_names[cstb_year]          = Istring("year");
    the_names[cstb_equipe]        = Istring("equipe");
    the_names[cstb_subtype]       = Istring("subtype");
    the_names[cstb_unite]         = Istring("unite");
    the_names[cstb_statut]        = Istring("statut");
    the_names[cstb_section]       = Istring("section");
    the_names[cst_numalign]       = Istring("numalign");
    the_names[cst_denalign]       = Istring("denomalign");
    // math fonts
    the_names[cstf_normal]                = Istring("normal"); // no attrib
    the_names[cstf_upright]               = Istring(0);
    the_names[cstf_bold]                  = Istring("bold");
    the_names[cstf_italic]                = Istring("italic");
    the_names[cstf_bold_italic]           = Istring("bold-italic");
    the_names[cstf_script]                = Istring("script");
    the_names[cstf_bold_script]           = Istring("bold-script");
    the_names[cstf_fraktur]               = Istring("fraktur");
    the_names[cstf_doublestruck]          = Istring("double-struck");
    the_names[cstf_bold_fraktur]          = Istring("bold-fraktur");
    the_names[cstf_sansserif]             = Istring("sans-serif");
    the_names[cstf_bold_sansserif]        = Istring("bold-sans-serif");
    the_names[cstf_sansserif_italic]      = Istring("sans-serif-italic");
    the_names[cstf_sansserif_bold_italic] = Istring("sans-serif-bold-italic");
    the_names[cstf_monospace]             = Istring("monospace");

    the_names[np_a1]              = Istring("a1");
    the_names[np_a2]              = Istring("a2");
    the_names[np_abarnodeconnect] = Istring("abarnodeconnect");
    the_names[np_accueil]         = Istring("accueil");
    the_names[np_affiliation]     = Istring("affiliation");
    the_names[np_allowbreak]      = Istring("allowbreak");
    the_names[np_alt_head]        = Istring("alt_head");
    the_names[np_alt_caption]     = Istring("alt_head");
    the_names[np_alt_section]     = Istring("alt_head");
    the_names[np_anchor]          = Istring("anchor");
    the_names[np_angle]           = Istring("angle");
    the_names[np_anodeconnect]    = Istring("anodeconnect");
    the_names[np_anodecurve]      = Istring("anodecurve");
    the_names[np_arc]             = Istring("pic-arc");
    the_names[np_arg1]            = Istring("arg1");
    the_names[np_arg2]            = Istring("arg2");
    the_names[np_arg3]            = Istring("arg3");
    the_names[np_array]           = Istring("array");
    the_names[np_alternatives]    = Istring("alternatives");

    the_names[np_b1]             = Istring("b1");
    the_names[np_b2]             = Istring("b2");
    the_names[np_b_rend]         = Istring("rend");
    the_names[np_backmatter]     = Istring("backmatter");
    the_names[np_bauteurs]       = Istring("bauteurs");
    the_names[np_barnodeconnect] = Istring("barnodeconnect");
    the_names[np_bediteurs]      = Istring("bediteur");
    the_names[np_bezier]         = Istring("pic-bezier");
    the_names[np_biblio]         = Istring("biblio");
    the_names[np_bigcircle]      = Istring("pic-bigcircle");
    the_names[np_bottomborder]   = Istring("bottom-border");
    the_names[np_border_bottomw] = Istring("border-bottom-width");
    the_names[np_border_topw]    = Istring("border-top-width");
    the_names[np_border_leftw]   = Istring("border-left-width");
    the_names[np_border_rightw]  = Istring("border-right-width");

    the_names[np_box]       = Istring("pic-framebox");
    the_names[np_box_pos]   = Istring("position");
    the_names[np_boxed]     = Istring("boxed");
    the_names[np_bpers]     = Istring("bpers");
    the_names[np_box_width] = Istring("width");
    the_names[np_bibitem]   = Istring("bibitem");
    the_names[np_bibkey]    = Istring("bib-key");

    the_names[np_c1]          = Istring("c1");
    the_names[np_c2]          = Istring("c2");
    the_names[np_c_left]      = Istring("left");
    the_names[np_c_right]     = Istring("right");
    the_names[np_c_center]    = Istring("center");
    the_names[np_caption]     = Istring("caption");
    the_names[np_captions]    = Istring("head");
    the_names[np_catperso]    = Istring("catperso");
    the_names[np_cell]        = Istring("cell");
    the_names[np_center]      = Istring("center");
    the_names[np_center_etc]  = Istring("center");
    the_names[np_center_etc1] = Istring("center");
    the_names[np_center_etc2] = Istring("quoted");
    the_names[np_center_etc3] = Istring("quoted");
    the_names[np_center_etc4] = Istring("flushed-left");
    the_names[np_center_etc5] = Istring("flushed-right");
    the_names[np_center_etc6] = Istring("verse");
    the_names[np_centered]    = Istring("centered");
    the_names[np_chapters]    = Istring("chapters");
    the_names[np_circle]      = Istring("pic-circle");
    the_names[np_natcit]      = Istring("Cit");
    the_names[np_cit]         = Istring("cit");
    the_names[np_cite_type]   = Istring("type");
    the_names[np_citation]    = Istring("citation");
    the_names[np_cleaders]    = Istring("cleaders");
    the_names[np_clip]        = Istring("clip");
    the_names[np_close]       = Istring("close");
    the_names[np_closecurve]  = Istring("pic-closecurve");
    the_names[np_columnalign] = Istring("columnalign");
    the_names[np_columnspan]  = Istring("columnspan");
    the_names[np_pagecolor]   = Istring("pagecolor");
    the_names[np_color]       = Istring("color");
    the_names[np_color2]      = Istring("color2");
    the_names[np_cols]        = Istring("cols");
    the_names[np_cst_width]   = Istring("width");
    the_names[np_curve]       = Istring("pic-curve");
    the_names[np_curve_nbpts] = Istring("nbsymb");

    the_names[np_dashbox]     = Istring("pic-dashbox");
    the_names[np_dashline]    = Istring("dashline");
    the_names[np_dashdim]     = Istring("dashdim");
    the_names[np_depth]       = Istring("depth");
    the_names[np_depthA]      = Istring("depthA");
    the_names[np_depthB]      = Istring("depthB");
    the_names[np_description] = Istring("description");
    the_names[np_display]     = Istring("display");
    the_names[np_div0] = the_names[cst_div0] = Istring("div0");
    the_names[np_div1] = the_names[cst_div1] = Istring("div1");
    the_names[np_div2] = the_names[cst_div2] = Istring("div2");
    the_names[np_div3] = the_names[cst_div3] = Istring("div3");
    the_names[np_div4] = the_names[cst_div4] = Istring("div4");
    the_names[np_div5] = the_names[cst_div5] = Istring("div5");
    the_names[np_div6] = the_names[cst_div6] = Istring("div6");
    the_names[np_dottedline]                 = Istring("dottedline");
    the_names[np_drawline]                   = Istring("drawline");
    the_names[np_dx]                         = Istring("dx");
    the_names[np_dy]                         = Istring("dy");

    the_names[np_encap]   = Istring("encap");
    the_names[np_english] = Istring("english");
    the_names[np_eqnpos]  = Istring("eqnpos");
    the_names[np_error]   = Istring("error");
    the_names[np_Error]   = Istring("Error");
    the_names[np_warning] = Istring(""); // empty by default
    the_names[np_Warning] = Istring("Warning");
    the_names[np_Info]    = Istring("Info");
    the_names[np_inline]  = Istring("inline");

    the_names[np_false]             = Istring("false");
    the_names[np_fbox]              = Istring("fbox");
    the_names[np_figure]            = Istring("figure");
    the_names[np_file]              = Istring("file");
    the_names[np_fileextension]     = Istring("extension");
    the_names[np_float]             = Istring("float");
    the_names[np_float_figure]      = Istring("figure");
    the_names[np_float_table]       = Istring("table");
    the_names[np_font_bold]         = Istring("bold");
    the_names[np_font_boldextended] = Istring("boldextended");
    the_names[np_font_semibold]     = Istring("semibold");
    the_names[np_font_condensed]    = Istring("condensed");
    the_names[np_font_it]           = Istring("it");
    the_names[np_font_large]        = Istring("large");
    the_names[np_font_large1]       = Istring("large1");
    the_names[np_font_large2]       = Istring("large2");
    the_names[np_font_large3]       = Istring("large3");
    the_names[np_font_large4]       = Istring("large4");
    the_names[np_font_large5]       = Istring("large5");
    the_names[np_font_large6]       = Istring("large6");
    the_names[np_font_medium]       = Istring("medium");
    the_names[np_font_normalsize]   = Istring("normalsize");
    the_names[np_font_roman]        = Istring("roman");
    the_names[np_font_sansserif]    = Istring("sansserif");
    the_names[np_font_sc]           = Istring("sc");
    the_names[np_font_slanted]      = Istring("slanted");
    the_names[np_font_small]        = Istring("small");
    the_names[np_font_small1]       = Istring("small1");
    the_names[np_font_small2]       = Istring("small2");
    the_names[np_font_small3]       = Istring("small3");
    the_names[np_font_small4]       = Istring("small4");
    the_names[np_font_small5]       = Istring("small5");
    the_names[np_font_small6]       = Istring("small6");
    the_names[np_font_tt]           = Istring("tt");
    the_names[np_font_upright]      = Istring("upright");
    the_names[np_footnote]          = Istring("note");
    the_names[np_form]              = Istring("form");
    the_names[np_formula]           = Istring("formula");
    the_names[np_foot]              = Istring("foot");
    the_names[np_frame]             = Istring("pic-frame");
    the_names[np_framed]            = Istring("framed");
    the_names[np_french]            = Istring("french");
    the_names[np_from]              = Istring("from");
    the_names[np_frontmatter]       = Istring("frontmatter");
    the_names[np_full]              = Istring("full");
    the_names[np_full_first]        = Istring("prenomcomplet");

    the_names[np_german]   = Istring("german");
    the_names[np_glo_name] = Istring("Glossary");
    the_names[np_gloss]    = Istring("gloss");
    the_names[np_graphics] = Istring("graphics");

    the_names[np_halfem]  = Istring("4.pt"); // width of space in math
    the_names[np_halign]  = Istring("halign");
    the_names[np_hdr]     = Istring("hdr");
    the_names[np_head]    = Istring("head");
    the_names[np_header]  = Istring("headings");
    the_names[np_height]  = Istring("height");
    the_names[np_hfil]    = Istring("hfil");
    the_names[np_hfill]   = Istring("hfill");
    the_names[np_hfilneg] = Istring("hfilneg");
    the_names[np_html]    = Istring("html");
    the_names[np_hss]     = Istring("hss");

    the_names[np_id]        = Istring("id");
    the_names[np_idtext]    = Istring("id-text");
    the_names[np_index]     = Istring("index");
    the_names[np_glossary]  = Istring("glossary");
    the_names[np_inline]    = Istring("inline");
    the_names[np_isproject] = Istring("isproject");
    the_names[np_item]      = Istring("item");

    the_names[np_junior] = Istring("junior");

    the_names[np_key]      = Istring("key");
    the_names[np_keywords] = Istring("keywords");

    the_names[np_label_glo]      = Istring("label");
    the_names[np_label_item]     = Istring("label");
    the_names[np_language]       = Istring(0); //
    the_names[np_leaders]        = Istring("leaders");
    the_names[np_left]           = Istring("left");
    the_names[np_leftborder]     = Istring("left-border");
    the_names[np_leg]            = Istring("leg");
    the_names[np_level]          = Istring("level");
    the_names[np_linethickness]  = Istring("linethickness");
    the_names[np_lineC]          = Istring("line");
    the_names[np_line]           = Istring("pic-line");
    the_names[np_line_thickness] = Istring("pic-linethickness");
    the_names[np_list]           = Istring("list");
    the_names[np_llap]           = Istring("llap");
    the_names[np_rlap]           = Istring("rlap");
    the_names[np_rasection]      = Istring("");

    the_names[np_mainmatter]     = Istring("mainmatter");
    the_names[np_mbox]           = Istring("mbox");
    the_names[np_marginpar]      = Istring("marginpar");
    the_names[np_minipage]       = Istring("minipage");
    the_names[np_minipage_width] = Istring("width");
    the_names[np_module]         = Istring("module");
    the_names[np_moreinfo]       = Istring("moreinfo");
    the_names[np_movablelimits]  = Istring("movablelimits");
    the_names[np_multiput]       = Istring("pic-multiput");

    the_names[np_name]  = Istring("name");
    the_names[np_nameA] = Istring("nameA");
    the_names[np_nameB] = Istring("nameB");

    the_names[np_node]         = Istring("node");
    the_names[np_nodebox]      = Istring("nodebox");
    the_names[np_nodecircle]   = Istring("nodecircle");
    the_names[np_nodeconnect]  = Istring("nodeconnect");
    the_names[np_nodecurve]    = Istring("nodecurve");
    the_names[np_nodeoval]     = Istring("nodeoval");
    the_names[np_nodetriangle] = Istring("nodetriangle");
    the_names[np_noindent]     = Istring("noindent");
    the_names[np_nom]          = Istring("nom");
    the_names[np_nonumber]     = Istring("nonumber");

    the_names[np_open]     = Istring("open");
    the_names[np_ordered]  = Istring("ordered");
    the_names[np_oval]     = Istring("pic-oval");
    the_names[np_overline] = Istring("overline");

    the_names[np_page]            = Istring("page");
    the_names[np_part]            = Istring("part");
    the_names[np_part0]           = Istring("participant");
    the_names[np_part1]           = Istring("participante");
    the_names[np_part2]           = Istring("participantes");
    the_names[np_part3]           = Istring("participants");
    the_names[np_particle]        = Istring("part");
    the_names[np_pers]            = Istring("pers");
    the_names[np_place]           = Istring("place");
    the_names[np_picture]         = Istring("picture");
    the_names[np_point]           = Istring("point");
    the_names[np_pos]             = Istring("pos");
    the_names[np_posA]            = Istring("posA");
    the_names[np_posB]            = Istring("posB");
    the_names[np_posi]            = Istring("inner-pos");
    the_names[np_pre]             = Istring("pre");
    the_names[np_prefix]          = Istring("prefix");
    the_names[np_prenom]          = Istring("prenom");
    the_names[np_prenote]         = Istring("prenote");
    the_names[np_profession]      = Istring("profession");
    the_names[np_research_centre] = Istring("research-centre");
    the_names[np_projet]          = Istring("projet");
    the_names[np_projetdeveloppe] = Istring("projetdeveloppe");
    the_names[np_put]             = Istring("pic-put");

    the_names[np_quoted] = Istring("quoted");

    the_names[np_r_angle]     = Istring("angle");
    the_names[np_raisebox]    = Istring("raisebox");
    the_names[np_ref]         = Istring("ref");
    the_names[np_rend]        = Istring("rend");
    the_names[np_repeat]      = Istring("repeat");
    the_names[np_right]       = Istring("right");
    the_names[np_rightborder] = Istring("right-border");
    the_names[np_rotatebox]   = Istring("pic-rotatebox");
    the_names[np_row]         = Istring("row");
    the_names[np_rule]        = Istring("rule");
    the_names[np_rule_tsa]    = Istring("top_rule_space_above");
    the_names[np_rule_tsb]    = Istring("top_rule_space_below");
    the_names[np_rule_bsa]    = Istring("bottom_rule_space_above");
    the_names[np_rule_bsb]    = Istring("bottom_rule_space_below");
    the_names[np_rclist]      = Istring("UR");
    the_names[np_rcval]       = Istring("+UR");

    the_names[np_sbox]            = Istring("scalebox");
    the_names[np_sc]              = Istring("sc");
    the_names[np_scale]           = Istring("scale");
    the_names[np_scaleput]        = Istring("pic-scaleput");
    the_names[np_s_caps]          = Istring("caps");
    the_names[np_s_hl]            = Istring("hl");
    the_names[np_s_old]           = Istring("oldstyle");
    the_names[np_s_scale]         = Istring("scale");
    the_names[np_s_so]            = Istring("so");
    the_names[np_s_st]            = Istring("st");
    the_names[np_s_sub]           = Istring("sub");
    the_names[np_s_sup]           = Istring("sup");
    the_names[np_s_ul]            = Istring("ul");
    the_names[np_separator]       = Istring("");
    the_names[np_simple]          = Istring("simple");
    the_names[np_simplemath]      = Istring("simplemath");
    the_names[np_size]            = Istring("size");
    the_names[np_spaceafter]      = Istring("spaceafter");
    the_names[np_spacebefore]     = Istring("spacebefore");
    the_names[np_specs]           = Istring("specs");
    the_names[np_style]           = Istring("style");
    the_names[np_starred]         = Istring("starred");
    the_names[np_subfigure]       = Istring("subfigure");
    the_names[np_stylesheet]      = Istring("");
    the_names[np_stylesheet_type] = Istring("text/css");

    the_names[np_table]        = Istring("table");
    the_names[np_Table]        = Istring("Table");
    the_names[np_tabular]      = Istring("tabular");
    the_names[np_tabular_star] = Istring("tabular*");
    the_names[np_tab_width]    = Istring("width");
    the_names[np_tagcurve]     = Istring("pic-tagcurve");
    the_names[np_target]       = Istring("target");
    the_names[np_term]         = Istring("term");
    the_names[np_texte]        = Istring("texte");
    the_names[np_texmath]      = Istring("texmath");
    the_names[np_theindex]     = Istring("theindex");
    the_names[np_theglossary]  = Istring("theglossary");
    the_names[np_theme]        = Istring("theme");
    the_names[np_theorem]      = Istring("");
    the_names[np_thick_lines]  = Istring("pic-thicklines");
    the_names[np_thin_lines]   = Istring("pic-thinlines");
    the_names[np_toc]          = Istring("tableofcontents");
    the_names[np_toc1]         = Istring("listoftables");
    the_names[np_toc2]         = Istring("listoffigures");
    the_names[np_topborder]    = Istring("top-border");
    the_names[np_totalwidth]   = Istring("totalwidth");
    the_names[np_true]         = Istring("true");
    the_names[np_type]         = Istring("type");
    the_names[np_textype]      = Istring("textype");

    the_names[np_underline]   = Istring("underline");
    the_names[np_unknown]     = Istring("unknown");
    the_names[np_unit_length] = Istring("unit-length");
    the_names[np_url]         = Istring("url");
    the_names[np_userid]      = Istring("userid");
    the_names[np_user_list]   = Istring("description");

    the_names[np_val]     = Istring("val");
    the_names[np_vector]  = Istring("pic-vector");
    the_names[np_vfil]    = Istring("vfil");
    the_names[np_vfill]   = Istring("vfill");
    the_names[np_vfilneg] = Istring("vfilneg");
    the_names[np_vpos]    = Istring("vpos");
    the_names[np_vss]     = Istring("vss");

    the_names[np_width] = Istring("width");

    the_names[np_xscale]   = Istring("xscale");
    the_names[np_xscaley]  = Istring("xscaley");
    the_names[np_xdir]     = Istring("xdir");
    the_names[np_xleaders] = Istring("xleaders");
    the_names[np_xpos]     = Istring("xpos");
    the_names[np_xref]     = Istring("xref");

    the_names[np_yscale]  = Istring("yscale");
    the_names[np_yscalex] = Istring("yscalex");
    the_names[np_ydir]    = Istring("ydir");
    the_names[np_ypos]    = Istring("ypos");

    the_names[np_zerodim]                   = Istring("0pt");
    the_names[np_mml_normal]                = Istring("mml@font@normal");
    the_names[np_mml_upright]               = Istring("mml@font@upright");
    the_names[np_mml_bold]                  = Istring("mml@font@bold");
    the_names[np_mml_italic]                = Istring("mml@font@italic");
    the_names[np_mml_bold_italic]           = Istring("mml@font@bolditalic");
    the_names[np_mml_script]                = Istring("mml@font@script");
    the_names[np_mml_bold_script]           = Istring("mml@font@boldscript");
    the_names[np_mml_fraktur]               = Istring("mml@font@fraktur");
    the_names[np_mml_doublestruck]          = Istring("mml@font@doublestruck");
    the_names[np_mml_bold_fraktur]          = Istring("mml@font@boldfraktur");
    the_names[np_mml_sansserif]             = Istring("mml@font@sansserif");
    the_names[np_mml_bold_sansserif]        = Istring("mml@font@boldsansserif");
    the_names[np_mml_sansserif_italic]      = Istring("mml@font@sansserifitalic");
    the_names[np_mml_sansserif_bold_italic] = Istring("mml@font@sansserifbolditalic");
    the_names[np_mml_monospace]             = Istring("mml@font@monospace");

    the_names[np_3pt]        = Istring("3pt");
    the_names[np_6pt]        = Istring("6pt");
    the_names[np_12pt]       = Istring("12pt");
    the_names[np_letters_tl] = Istring("tl");
    the_names[np_letters_tr] = Istring("tr");
    the_names[np_letters_bl] = Istring("bl");
    the_names[np_letters_br] = Istring("br");
    std::array<char, 2> foo{};
    foo[1] = 0;
    for (char x = 'a'; x <= 'z'; x++) {
        foo[0]                                                = x;
        the_names[static_cast<size_t>(np_letter_a + x - 'a')] = Istring(foo.data());
    }
    for (char x = 'A'; x <= 'Z'; x++) {
        foo[0]                                                = x;
        the_names[static_cast<size_t>(np_letter_A + x - 'A')] = Istring(foo.data());
    }
}

// Function called when A=B is seen in the configuration file.
// Returns true if A is recognised
auto assign(Buffer &a, Buffer &b) -> bool {
    String A = a.c_str();
    String B = b.c_str();
    auto   n = a.size();
    if (A[0] == 'e' && A[1] == 'l' && A[2] == 't' && A[3] == '_') return config_ns::assign_name(A + 4, B);
    if (A[0] == 'x' && A[1] == 'm' && A[2] == 'l' && A[3] == '_') {
        if (A[n - 1] == 'e' && A[n - 2] == 'm' && A[n - 3] == 'a' && A[n - 4] == 'n' && A[n - 5] == '_') { a.kill_at(n - 5); }
        return config_ns::assign_name(A + 4, B);
    }
    if (A[0] == 'a' && A[1] == 't' && A[2] == 't' && A[3] == '_') return config_ns::assign_att(A + 4, B);
    if (a == "lang_fr") {
        the_names[np_french] = Istring(B);
        return true;
    }
    if (a == "lang_en") {
        the_names[np_english] = Istring(B);
        return true;
    }
    if (a == "distinguish_refer_in_rabib") {
        if ((b == "true") || (b == "yes"))
            the_main->set_distinguish(true);
        else if ((b == "false") || (b == "no"))
            the_main->set_distinguish(false);
        else
            b << bf_reset << "ignored";
        return true;
    }
    if (a == "entity_names") {
        the_main->set_ent_names(B);
        return true;
    }
    if (a == "default_class") {
        the_main->set_default_class(b.to_string());
        return true;
    }
    if (a == "alternate_item") {
        if (b == "false")
            the_parser.hash_table.eval_let("item", "@@item");
        else if (b == "true")
            the_parser.hash_table.eval_let("item", "@item");
        else
            a.reset();
        return true;
    }
    if (a == "url_font") {
        Buffer aux;
        aux << "\\def\\urlfont{" << B << "}";
        the_main->add_to_from_config(1, aux);
        return true;
    }
    if (a == "everyjob") {
        Buffer aux;
        aux << "\\everyjob={" << B << "}";
        everyjob_string = aux.to_string();
        return true;
    }
    if (a == "no_footnote_hack") {
        if (b == "true") the_main->set_foot_hack(false);
        if (b == "false") the_main->set_foot_hack(true);
        return true;
    }
    if (a == "use_font_elt") {
        if (b == "true") the_main->set_use_font(true);
        if (b == "false") the_main->set_use_font(false);
        return true;
    }
    if (a == "use_all_sizes") {
        if (b == "true") the_main->set_use_sizes(true);
        if (b == "false") the_main->set_use_sizes(false);
        return true;
    }
    if (a == "bibtex_fields") {
        b.interpret_bibtex_list();
        return false;
    }
    if (a == "bibtex_extensions") {
        b.interpret_bibtex_extension_list();
        return false;
    }
    if (a == "mfenced_separator_val") {
        if (strcmp(B, "NONE") == 0)
            the_names[np_separator] = Istring(0);
        else
            the_names[np_separator] = Istring(B);
        return true;
    }
    if (n > 5 && A[n - 5] == '_' && A[n - 4] == 'v' && A[n - 3] == 'a' && A[n - 2] == 'l' && A[n - 1] == 's') {
        a.kill_at(n - 5);
        config_ns::interpret_list(a.to_string(), b);
        a.reset();
        return true;
    }
    if (a == "mml_font_normal") {
        the_names[np_mml_normal] = Istring(B);
        return true;
    }
    if (a == "mml_font_upright") {
        the_names[np_mml_upright] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold") {
        the_names[np_mml_bold] = Istring(B);
        return true;
    }
    if (a == "mml_font_italic") {
        the_names[np_mml_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_italic") {
        the_names[np_mml_bold_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_script") {
        the_names[np_mml_script] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_script") {
        the_names[np_mml_bold_script] = Istring(B);
        return true;
    }
    if (a == "mml_font_fraktur") {
        the_names[np_mml_fraktur] = Istring(B);
        return true;
    }
    if (a == "mml_font_doublestruck") {
        the_names[np_mml_doublestruck] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_fraktur") {
        the_names[np_mml_bold_fraktur] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif") {
        the_names[np_mml_sansserif] = Istring(B);
        return true;
    }
    if (a == "mml_font_bold_sansserif") {
        the_names[np_mml_bold_sansserif] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif_italic") {
        the_names[np_mml_sansserif_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_sansserif_bold_italic") {
        the_names[np_mml_sansserif_bold_italic] = Istring(B);
        return true;
    }
    if (a == "mml_font_monospace") {
        the_names[np_mml_monospace] = Istring(B);
        return true;
    }
    return false;
}

// Handles names starting with xml_
auto config_ns::assign_name(String A, String B) -> bool {
    switch (A[0]) {
    case 'a':
        if (strcmp(A, "accueil") == 0) {
            the_names[np_accueil] = Istring(B);
            return true;
        }
        if (strcmp(A, "arc") == 0) {
            the_names[np_arc] = Istring(B);
            return true;
        }
        if (strcmp(A, "allowbreak") == 0) {
            the_names[np_allowbreak] = Istring(B);
            return true;
        }
        if (strcmp(A, "anchor") == 0) {
            the_names[np_anchor] = Istring(B);
            return true;
        }
        if (strcmp(A, "anodeconnect") == 0) {
            the_names[np_anodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "abarnodeconnect") == 0) {
            the_names[np_abarnodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "anodecurve") == 0) {
            the_names[np_anodecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "alt_caption") == 0) {
            the_names[np_alt_caption] = Istring(B);
            return true;
        }
        if (strcmp(A, "alt_section") == 0) {
            the_names[np_alt_section] = Istring(B);
            return true;
        }
        if (strcmp(A, "alternatives") == 0) {
            the_names[np_alternatives] = Istring(B);
            return true;
        }
        return false;
    case 'b':
        if (strcmp(A, "box") == 0) {
            the_names[np_box] = Istring(B);
            return true;
        }
        if (strcmp(A, "bezier") == 0) {
            the_names[np_bezier] = Istring(B);
            return true;
        }
        if (strcmp(A, "bigcircle") == 0) {
            the_names[np_bigcircle] = Istring(B);
            return true;
        }
        if (strcmp(A, "backmatter") == 0) {
            the_names[np_backmatter] = Istring(B);
            return true;
        }
        if (strcmp(A, "biblio") == 0) {
            the_names[np_biblio] = Istring(B);
            return true;
        }
        if (strcmp(A, "bpers") == 0) {
            the_names[np_bpers] = Istring(B);
            return true;
        }
        if (strcmp(A, "barnodeconnect") == 0) {
            the_names[np_barnodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibitem") == 0) {
            the_names[np_bibitem] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibkey") == 0) {
            the_names[np_bibkey] = Istring(B);
            return true;
        }
        return false;
    case 'c':
        if (strcmp(A, "cell") == 0) {
            the_names[np_cell] = Istring(B);
            return true;
        }
        if (strcmp(A, "caption") == 0) {
            the_names[np_captions] = Istring(B);
            return true;
        }
        if (strcmp(A, "circle") == 0) {
            the_names[np_circle] = Istring(B);
            return true;
        }
        if (strcmp(A, "closecurve") == 0) {
            the_names[np_closecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "curve") == 0) {
            the_names[np_curve] = Istring(B);
            return true;
        }
        if (strcmp(A, "catperso") == 0) {
            the_names[np_catperso] = Istring(B);
            return true;
        }
        if (strcmp(A, "composition_ra") == 0) {
            the_names[cst_composition] = Istring(B);
            return true;
        }
        if (strcmp(A, "cleaders") == 0) {
            the_names[np_cleaders] = Istring(B);
            return true;
        }
        if (strcmp(A, "caps") == 0) {
            the_names[np_s_caps] = Istring(B);
            return true;
        }
        if (strcmp(A, "cit") == 0) {
            the_names[np_cit] = Istring(B);
            return true;
        }
        if (strcmp(A, "citation") == 0) {
            the_names[np_citation] = Istring(B);
            return true;
        }
        if (strcmp(A, "citetype") == 0) {
            the_names[np_cite_type] = Istring(B);
            return true;
        }
        return false;
    case 'd':
        if (strcmp(A, "dashline") == 0) {
            the_names[np_dashline] = Istring(B);
            return true;
        }
        if (strcmp(A, "div0") == 0) {
            the_names[np_div0] = Istring(B);
            return true;
        }
        if (strcmp(A, "div1") == 0) {
            the_names[np_div1] = Istring(B);
            return true;
        }
        if (strcmp(A, "div2") == 0) {
            the_names[np_div2] = Istring(B);
            return true;
        }
        if (strcmp(A, "div3") == 0) {
            the_names[np_div3] = Istring(B);
            return true;
        }
        if (strcmp(A, "div4") == 0) {
            the_names[np_div4] = Istring(B);
            return true;
        }
        if (strcmp(A, "div5") == 0) {
            the_names[np_div5] = Istring(B);
            return true;
        }
        if (strcmp(A, "div6") == 0) {
            the_names[np_div6] = Istring(B);
            return true;
        }
        if (strcmp(A, "drawline") == 0) {
            the_names[np_drawline] = Istring(B);
            return true;
        }
        if (strcmp(A, "dottedline") == 0) {
            the_names[np_dottedline] = Istring(B);
            return true;
        }
        return false;
    case 'e':
        if (strcmp(A, "eqnpos") == 0) {
            the_names[np_eqnpos] = Istring(B);
            return true;
        }
        return false;
    case 'f':
        if (strcmp(A, "footnote") == 0) {
            the_names[np_footnote] = Istring(B);
            return true;
        }
        if (strcmp(A, "formula") == 0) {
            the_names[np_formula] = Istring(B);
            return true;
        }
        if (strcmp(A, "fbox") == 0) {
            the_names[np_fbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "figure") == 0) {
            the_names[np_figure] = Istring(B);
            return true;
        }
        if (strcmp(A, "figure_env") == 0) {
            the_names[np_float_figure] = Istring(B);
            return true;
        }
        if (strcmp(A, "frontmatter") == 0) {
            the_names[np_frontmatter] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small") == 0) {
            the_names[np_font_small] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small1") == 0) {
            the_names[np_font_small1] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small2") == 0) {
            the_names[np_font_small2] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small3") == 0) {
            the_names[np_font_small3] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small4") == 0) {
            the_names[np_font_small4] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small5") == 0) {
            the_names[np_font_small5] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_small6") == 0) {
            the_names[np_font_small6] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large") == 0) {
            the_names[np_font_large] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large1") == 0) {
            the_names[np_font_large1] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large2") == 0) {
            the_names[np_font_large2] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large3") == 0) {
            the_names[np_font_large3] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large4") == 0) {
            the_names[np_font_large4] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large5") == 0) {
            the_names[np_font_large5] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_large6") == 0) {
            the_names[np_font_large6] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_normalsize") == 0) {
            the_names[np_font_normalsize] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_upright") == 0) {
            the_names[np_font_upright] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_medium") == 0) {
            the_names[np_font_medium] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_roman") == 0) {
            the_names[np_font_roman] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_it") == 0) {
            the_names[np_font_it] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_slanted") == 0) {
            the_names[np_font_slanted] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_sc") == 0) {
            the_names[np_font_sc] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_tt") == 0) {
            the_names[np_font_tt] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_sansserif") == 0) {
            the_names[np_font_sansserif] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_bold") == 0) {
            the_names[np_font_bold] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_boldextended") == 0) {
            the_names[np_font_boldextended] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_semibold") == 0) {
            the_names[np_font_semibold] = Istring(B);
            return true;
        }
        if (strcmp(A, "font_condensed") == 0) {
            the_names[np_font_condensed] = Istring(B);
            return true;
        }
        return false;
    case 'g':
        if (strcmp(A, "gloitem") == 0) {
            the_names[np_label_glo] = Istring(B);
            return true;
        }
        if (strcmp(A, "graphics") == 0) {
            the_names[np_graphics] = Istring(B);
            return true;
        }
        if (strcmp(A, "glo") == 0) {
            the_names[np_glo_name] = Istring(B);
            return true;
        }
        if (strcmp(A, "glossary") == 0) {
            the_names[np_glossary] = Istring(B);
            return true;
        }
        return false;
    case 'h':
        if (strcmp(A, "head") == 0) {
            the_names[np_head] = Istring(B);
            return true;
        }
        if (strcmp(A, "hl") == 0) {
            the_names[np_s_hl] = Istring(B);
            return true;
        }
        return false;
    case 'i':
        if (strcmp(A, "item") == 0) {
            the_names[np_item] = Istring(B);
            return true;
        }
        if (strcmp(A, "index") == 0) {
            the_names[np_index] = Istring(B);
            return true;
        }
        return false;
    case 'k':
        if (strcmp(A, "keywords") == 0) {
            the_names[np_keywords] = Istring(B);
            return true;
        }
        return false;
    case 'l':
        if (strcmp(A, "labelitem") == 0) {
            the_names[np_label_item] = Istring(B);
            return true;
        }
        if (strcmp(A, "lineC") == 0) {
            the_names[np_lineC] = Istring(B);
            return true;
        }
        if (strcmp(A, "line") == 0) {
            the_names[np_line] = Istring(B);
            return true;
        }
        if (strcmp(A, "listoffigures") == 0) {
            the_names[np_toc2] = Istring(B);
            return true;
        }
        if (strcmp(A, "listoftables") == 0) {
            the_names[np_toc1] = Istring(B);
            return true;
        }
        if (strcmp(A, "llap") == 0) {
            the_names[np_llap] = Istring(B);
            return true;
        }
        if (strcmp(A, "linethickness") == 0) {
            the_names[np_line_thickness] = Istring(B);
            return true;
        }
        if (strcmp(A, "list") == 0) {
            the_names[np_list] = Istring(B);
            return true;
        }
        if (strcmp(A, "leaders") == 0) {
            the_names[np_leaders] = Istring(B);
            return true;
        }
        if (strcmp(A, "leg") == 0) {
            the_names[np_leg] = Istring(B);
            return true;
        }
        return false;
    case 'm':
        if (strcmp(A, "mbox") == 0) {
            the_names[np_mbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "math") == 0) {
            the_names[cst_math] = Istring(B);
            return true;
        }
        if (strcmp(A, "multiput") == 0) {
            the_names[np_multiput] = Istring(B);
            return true;
        }
        if (strcmp(A, "mainmatter") == 0) {
            the_names[np_mainmatter] = Istring(B);
            return true;
        }
        return false;
    case 'n':
        if (strcmp(A, "node") == 0) {
            the_names[np_node] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodeconnect") == 0) {
            the_names[np_nodeconnect] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodecurve") == 0) {
            the_names[np_nodecurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodetriangle") == 0) {
            the_names[np_nodetriangle] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodecircle") == 0) {
            the_names[np_nodecircle] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodebox") == 0) {
            the_names[np_nodebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "nodeoval") == 0) {
            the_names[np_nodeoval] = Istring(B);
            return true;
        }
        if (strcmp(A, "natcit") == 0) {
            the_names[np_natcit] = Istring(B);
            return true;
        }
        return false;
    case 'o':
        if (strcmp(A, "oval") == 0) {
            the_names[np_oval] = Istring(B);
            return true;
        }
        if (strcmp(A, "oldstyle") == 0) {
            the_names[np_s_old] = Istring(B);
            return true;
        }
        if (strcmp(A, "overline") == 0) {
            the_names[np_overline] = Istring(B);
            return true;
        }
        return false;
    case 'p':
        if (strcmp(A, "picture") == 0) {
            the_names[np_picture] = Istring(B);
            return true;
        }
        if (strcmp(A, "put") == 0) {
            the_names[np_put] = Istring(B);
            return true;
        }
        if (strcmp(A, "project") == 0) {
            the_names[np_projet] = Istring(B);
            return true;
        }
        if (strcmp(A, "pers") == 0) {
            the_names[np_pers] = Istring(B);
            return true;
        }
        if (strcmp(A, "prenote") == 0) {
            the_names[np_prenote] = Istring(B);
            return true;
        }
        if (strcmp(A, "pack_font_att") == 0) {
            if (strcmp(B, "true") == 0) the_main->set_pack_font(true);
            if (strcmp(B, "false") == 0) the_main->set_pack_font(false);
            return true;
        }
        return false;
    case 'r':
        if (strcmp(A, "row") == 0) {
            the_names[np_row] = Istring(B);
            return true;
        }
        if (strcmp(A, "raisebox") == 0) {
            the_names[np_raisebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "rlap") == 0) {
            the_names[np_rlap] = Istring(B);
            return true;
        }
        if (strcmp(A, "rotatebox") == 0) {
            the_names[np_rotatebox] = Istring(B);
            return true;
        }
        if (strcmp(A, "ref") == 0) {
            the_names[np_ref] = Istring(B);
            return true;
        }
        if (strcmp(A, "rclist") == 0 && ra_ok) {
            the_names[np_rclist] = Istring(B);
            return true;
        }
        if (strcmp(A, "rcval") == 0 && ra_ok) {
            the_names[np_rcval] = Istring(B);
            return true;
        }
        if (strcmp(A, "rasection") == 0 && ra_ok) {
            the_names[np_rasection] = Istring(B);
            return true;
        }
        return false;
    case 's':
        if (strcmp(A, "subfigure") == 0) {
            the_names[np_subfigure] = Istring(B);
            return true;
        }
        if (strcmp(A, "scaleput") == 0) {
            the_names[np_scaleput] = Istring(B);
            return true;
        }
        if (strcmp(A, "scalebox") == 0) {
            the_names[np_sbox] = Istring(B);
            return true;
        }
        if (strcmp(A, "scaption") == 0) {
            the_names[np_caption] = Istring(B);
            return true;
        }
        if (strcmp(A, "sup") == 0) {
            the_names[np_s_sup] = Istring(B);
            return true;
        }
        if (strcmp(A, "sub") == 0) {
            the_names[np_s_sub] = Istring(B);
            return true;
        }
        if (strcmp(A, "so") == 0) {
            the_names[np_s_so] = Istring(B);
            return true;
        }
        if (strcmp(A, "st") == 0) {
            the_names[np_s_st] = Istring(B);
            return true;
        }
        if (strcmp(A, "stylesheet") == 0) {
            the_names[np_stylesheet] = Istring(B);
            return true;
        }
        if (strcmp(A, "stylesheettype") == 0) {
            the_names[np_stylesheet_type] = Istring(B);
            return true;
        }
        return false;
    case 't':
        if (strcmp(A, "term") == 0) {
            the_names[np_term] = Istring(B);
            return true;
        }
        if (strcmp(A, "texmath") == 0) {
            the_names[np_texmath] = Istring(B);
            return true;
        }
        if (strcmp(A, "table") == 0) {
            the_names[np_table] = Istring(B);
            return true;
        }
        if (strcmp(A, "table_env") == 0) {
            the_names[np_float_table] = Istring(B);
            return true;
        }
        if (strcmp(A, "Table") == 0) {
            the_names[np_Table] = Istring(B);
            return true;
        }
        if (strcmp(A, "tagcurve") == 0) {
            the_names[np_tagcurve] = Istring(B);
            return true;
        }
        if (strcmp(A, "thicklines") == 0) {
            the_names[np_thick_lines] = Istring(B);
            return true;
        }
        if (strcmp(A, "thinlines") == 0) {
            the_names[np_thin_lines] = Istring(B);
            return true;
        }
        if (strcmp(A, "theorem_head") == 0) {
            the_names[np_alt_head] = Istring(B);
            return true;
        }
        if (strcmp(A, "theorem") == 0) {
            if (B[0] == 0) { // reverst to old behavior
                the_names[np_theorem] = Istring(B);
                the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
            } else if (B[0] == ' ') { // special case
                the_names[np_theorem] = Istring(B + 1);
                the_parser.hash_table.eval_let("@begintheorem", "@ybegintheorem");
            } else {
                the_names[np_theorem] = Istring(B);
                the_parser.hash_table.eval_let("@begintheorem", "@xbegintheorem");
            }
            return true;
        }
        if (strcmp(A, "theindex") == 0) {
            the_names[np_theindex] = Istring(B);
            return true;
        }
        if (strcmp(A, "theglossary") == 0) {
            the_names[np_theglossary] = Istring(B);
            return true;
        }
        if (strcmp(A, "tableofcontents") == 0) {
            the_names[np_toc] = Istring(B);
            return true;
        }
        if (strcmp(A, "texte") == 0) {
            the_names[np_texte] = Istring(B);
            return true;
        }
        return false;
    case 'u':
        if (strcmp(A, "ul") == 0) {
            the_names[np_s_ul] = Istring(B);
            return true;
        }
        if (strcmp(A, "underline") == 0) {
            the_names[np_underline] = Istring(B);
            return true;
        }
        if (strcmp(A, "use_font_elt") == 0) {
            if (strcmp(B, "true") == 0) the_main->set_use_font(true);
            if (strcmp(B, "false") == 0) the_main->set_use_font(false);
            return true;
        }
        return false;
    case 'v':
        if (strcmp(A, "vector") == 0) {
            the_names[np_vector] = Istring(B);
            return true;
        }
        return false;
    case 'w':
        if (strcmp(A, "warning") == 0) {
            the_names[np_warning] = Istring(B);
            return true;
        }
        return false;
    case 'x':
        if (strcmp(A, "xref") == 0) {
            the_names[np_xref] = Istring(B);
            return true;
        }
        if (strcmp(A, "xtheorem") == 0) {
            the_names[np_theorem] = Istring(B);
            return true;
        }
        if (strcmp(A, "xleaders") == 0) {
            the_names[np_xleaders] = Istring(B);
            return true;
        }
        return false;
    default: return false;
    }
}

// Handles names starting with att_
auto config_ns::assign_att(String A, String B) -> bool {
    switch (A[0]) {
    case 'a':
        if (strcmp(A, "angle") == 0) {
            the_names[np_angle] = Istring(B);
            return true;
        }
        if (strcmp(A, "affiliation") == 0) {
            the_names[np_affiliation] = Istring(B);
            return true;
        }
        return false;
    case 'b':
        if (strcmp(A, "box_pos") == 0) {
            the_names[np_box_pos] = Istring(B);
            return true;
        }
        if (strcmp(A, "box_scale") == 0) {
            the_names[np_s_scale] = Istring(B);
            return true;
        }
        if (strcmp(A, "box_width") == 0) {
            the_names[np_box_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "boxed") == 0) {
            the_names[np_boxed] = Istring(B);
            return true;
        }
        if (strcmp(A, "bibkey") == 0) {
            the_names[np_bibkey] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_bottom_width") == 0) {
            the_names[np_border_bottomw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_top_width") == 0) {
            the_names[np_border_topw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_left_width") == 0) {
            the_names[np_border_leftw] = Istring(B);
            return true;
        }
        if (strcmp(A, "border_right_width") == 0) {
            the_names[np_border_rightw] = Istring(B);
            return true;
        }
        return false;
    case 'c':
        if (strcmp(A, "centering") == 0) {
            the_names[np_center_etc1] = Istring(B);
            return true;
        }
        if (strcmp(A, "clip") == 0) {
            the_names[np_clip] = Istring(B);
            return true;
        }
        if (strcmp(A, "cols") == 0) {
            the_names[np_cols] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_left") == 0) {
            the_names[np_c_left] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_right") == 0) {
            the_names[np_c_right] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_center") == 0) {
            the_names[np_c_center] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_leftborder") == 0) {
            the_names[np_leftborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_rightborder") == 0) {
            the_names[np_rightborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_topborder") == 0) {
            the_names[np_topborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "cell_bottomborder") == 0) {
            the_names[np_bottomborder] = Istring(B);
            return true;
        }
        if (strcmp(A, "curve_nbpts") == 0) {
            the_names[np_curve_nbpts] = Istring(B);
            return true;
        }
        return false;
    case 'd':
        if (strcmp(A, "display") == 0) {
            the_names[np_display] = Istring(B);
            return true;
        }
        if (strcmp(A, "dx") == 0) {
            the_names[np_dx] = Istring(B);
            return true;
        }
        if (strcmp(A, "dy") == 0) {
            the_names[np_dy] = Istring(B);
            return true;
        }
        if (strcmp(A, "depthA") == 0) {
            the_names[np_depthA] = Istring(B);
            return true;
        }
        if (strcmp(A, "depthB") == 0) {
            the_names[np_depthB] = Istring(B);
            return true;
        }
        if (strcmp(A, "depth") == 0) {
            the_names[np_depth] = Istring(B);
            return true;
        }
        return false;
    case 'e':
        if (strcmp(A, "encap") == 0) {
            the_names[np_encap] = Istring(B);
            return true;
        }
        return false;
    case 'f':
        if (strcmp(A, "framed") == 0) {
            the_names[np_framed] = Istring(B);
            return true;
        }
        if (strcmp(A, "full") == 0) {
            the_names[np_full] = Istring(B);
            return true;
        }
        if (strcmp(A, "file") == 0) {
            the_names[np_file] = Istring(B);
            return true;
        }
        if (strcmp(A, "file_extension") == 0) {
            the_names[np_fileextension] = Istring(B);
            return true;
        }
        if (strcmp(A, "fbox_rend") == 0) {
            the_names[np_b_rend] = Istring(B);
            return true;
        }
        if (strcmp(A, "flush_left") == 0) {
            the_names[np_center_etc4] = Istring(B);
            return true;
        }
        if (strcmp(A, "flush_right") == 0) {
            the_names[np_center_etc5] = Istring(B);
            return true;
        }
        if (strcmp(A, "foot_position") == 0) {
            the_names[np_foot] = Istring(B);
            return true;
        }
        if (strcmp(A, "full_first") == 0) {
            the_names[np_full_first] = Istring(B);
            return true;
        }
        return false;
    case 'g':
        if (strcmp(A, "gloss_type") == 0) {
            the_names[np_gloss] = Istring(B);
            return true;
        }
        return false;
    case 'h':
        if (strcmp(A, "height") == 0) {
            the_names[np_height] = Istring(B);
            return true;
        }
        if (strcmp(A, "halign") == 0) {
            the_names[np_halign] = Istring(B);
            return true;
        }
        if (strcmp(A, "hdr") == 0) {
            the_names[np_hdr] = Istring(B);
            return true;
        }
        return false;
    case 'i':
        if (strcmp(A, "inner_pos") == 0) {
            the_names[np_posi] = Istring(B);
            return true;
        }
        if (strcmp(A, "inline") == 0) {
            the_names[np_inline] = Istring(B);
            return true;
        }
        return false;
    case 'j':
        if (strcmp(A, "junior") == 0) {
            the_names[np_junior] = Istring(B);
            return true;
        }
        return false;
    case 'l':
        if (strcmp(A, "language") == 0) {
            the_names[np_language] = Istring(B);
            return true;
        }
        if (strcmp(A, "level") == 0) {
            the_names[np_level] = Istring(B);
            return true;
        }
        return false;
    case 'm':
        if (strcmp(A, "minipage_width") == 0) {
            the_names[np_minipage_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "marginpar") == 0) {
            the_names[np_marginpar] = Istring(B);
            return true;
        }
        if (strcmp(A, "mathmlns") == 0) {
            the_names[cst_mathml] = Istring(B);
            return true;
        }
        if (strcmp(A, "marginpar") == 0) {
            the_names[np_marginpar] = Istring(B);
            return true;
        }
        if (strcmp(A, "mode") == 0) {
            the_names[cst_mode] = Istring(B);
            return true;
        }
        return false;
    case 'n':
        if (strcmp(A, "noindent") == 0) {
            the_names[np_noindent] = Istring(B);
            return true;
        }
        if (strcmp(A, "nonumber") == 0) {
            the_names[np_nonumber] = Istring(B);
            return true;
        }
        if (strcmp(A, "nom") == 0) {
            the_names[np_nom] = Istring(B);
            return true;
        }
        if (strcmp(A, "name") == 0) {
            the_names[np_name] = Istring(B);
            return true;
        }
        if (strcmp(A, "nameA") == 0) {
            the_names[np_nameA] = Istring(B);
            return true;
        }
        if (strcmp(A, "nameB") == 0) {
            the_names[np_nameB] = Istring(B);
            return true;
        }
        return false;
    case 'p':
        if (strcmp(A, "place") == 0) {
            the_names[np_place] = Istring(B);
            return true;
        }
        if (strcmp(A, "prenom") == 0) {
            the_names[np_prenom] = Istring(B);
            return true;
        }
        if (strcmp(A, "pre") == 0) {
            the_names[np_pre] = Istring(B);
            return true;
        }
        if (strcmp(A, "particule") == 0) {
            the_names[np_particle] = Istring(B);
            return true;
        }
        if (strcmp(A, "page") == 0) {
            the_names[np_page] = Istring(B);
            return true;
        }
        if (strcmp(A, "profession") == 0) {
            the_names[np_profession] = Istring(B);
            return true;
        }
        if (strcmp(A, "posA") == 0) {
            the_names[np_posA] = Istring(B);
            return true;
        }
        if (strcmp(A, "posB") == 0) {
            the_names[np_posB] = Istring(B);
            return true;
        }
        if (strcmp(A, "pos") == 0) {
            the_names[np_pos] = Istring(B);
            return true;
        }
        return false;
    case 'q':
        if (strcmp(A, "quote") == 0) {
            the_names[np_center_etc2] = Istring(B);
            return true;
        }
        if (strcmp(A, "quotation") == 0) {
            the_names[np_center_etc3] = Istring(B);
            return true;
        }
        return false;
    case 'r':
        if (strcmp(A, "rotate_angle") == 0) {
            the_names[np_r_angle] = Istring(B);
            return true;
        }
        if (strcmp(A, "rend") == 0) {
            the_names[np_rend] = Istring(B);
            return true;
        }
        if (strcmp(A, "row_spaceafter") == 0) {
            the_names[np_spaceafter] = Istring(B);
            return true;
        }
        if (strcmp(A, "repeat") == 0) {
            the_names[np_repeat] = Istring(B);
            return true;
        }
        return false;
    case 's':
        if (strcmp(A, "scale") == 0) {
            the_names[np_scale] = Istring(B);
            return true;
        }
        if (strcmp(A, "space_before") == 0) {
            the_names[np_spacebefore] = Istring(B);
            return true;
        }
        if (strcmp(A, "size") == 0) {
            the_names[np_size] = Istring(B);
            return true;
        }
        if (strcmp(A, "starred") == 0) {
            the_names[np_starred] = Istring(B);
            return true;
        }
        return false;
    case 't':
        if (strcmp(A, "table_width") == 0) {
            the_names[np_tab_width] = Istring(B);
            return true;
        }
        if (strcmp(A, "type") == 0) {
            the_names[np_type] = Istring(B);
            return true;
        }
        if (strcmp(A, "textype") == 0) {
            the_names[np_textype] = Istring(B);
            return true;
        }
        return false;
    case 'u':
        if (strcmp(A, "unit_length") == 0) {
            the_names[np_unit_length] = Istring(B);
            return true;
        }
        if (strcmp(A, "user_list") == 0) {
            the_names[np_user_list] = Istring(B);
            return true;
        }
        return false;
    case 'v':
        if (strcmp(A, "vpos") == 0) {
            the_names[np_vpos] = Istring(B);
            return true;
        }
        if (strcmp(A, "verse") == 0) {
            the_names[np_center_etc6] = Istring(B);
            return true;
        }
        return false;
    case 'w':
        if (strcmp(A, "width") == 0) {
            the_names[np_width] = Istring(B);
            return true;
        }
        return false;
    case 'x':
        if (strcmp(A, "xscale") == 0) {
            the_names[np_xscale] = Istring(B);
            return true;
        }
        if (strcmp(A, "xscaley") == 0) {
            the_names[np_xscaley] = Istring(B);
            return true;
        }
        if (strcmp(A, "xpos") == 0) {
            the_names[np_xpos] = Istring(B);
            return true;
        }
        if (strcmp(A, "xdir") == 0) {
            the_names[np_xdir] = Istring(B);
            return true;
        }
        return false;
    case 'y':
        if (strcmp(A, "yscale") == 0) {
            the_names[np_yscale] = Istring(B);
            return true;
        }
        if (strcmp(A, "yscalex") == 0) {
            the_names[np_yscalex] = Istring(B);
            return true;
        }
        if (strcmp(A, "ydir") == 0) {
            the_names[np_ydir] = Istring(B);
            return true;
        }
        if (strcmp(A, "ypos") == 0) {
            the_names[np_ypos] = Istring(B);
            return true;
        }
        return false;
    default: return false;
    }
}

// Fills the catcode table. Catcodes are at the start of eqtb_int_table
void Parser::make_catcodes() {
    for (unsigned i = 0; i < nb_characters; i++) {
        set_cat(i, other_catcode);
        eqtb_int_table[i].set_level(1);
    }
    for (unsigned i = 'a'; i <= 'z'; i++) set_cat(i, letter_catcode);
    for (unsigned i = 'A'; i <= 'Z'; i++) set_cat(i, letter_catcode);
    set_cat('\\', 0);
    set_cat('{', 1);
    set_cat('}', 2);
    set_cat('$', 3);
    set_cat('&', 4);
    set_cat('\r', 5);
    set_cat('\n', 12);
    set_cat('#', 6);
    set_cat('^', 7);
    set_cat('_', 8);
    set_cat('\t', 10);
    set_cat(' ', 10);
    set_cat(160, 13); // non breaking space
    set_cat('~', 13);
    set_cat('%', 14);
    for (uint i = 0; i < nb_shortverb_values; i++) old_catcode[i] = eqtb_int_table[i].get_val();
    set_cat('@', 11);
}

// This is the main bootstrap code
void Parser::boot() {
    boot_time();
    accent_ns::boot_accents();
    boot_verbatim();
    boot_xspace();
    boot_uclc();
    make_catcodes();
    tralics_ns::make_names();
    boot_NAO();
    make_constants();
}

// This is done only if fancy_hdr is loaded (via \usepackage).
void Hashtab::boot_fancyhdr() {
    primitive("lhead", fancy_cmd, fancy_lhead_code);
    primitive("chead", fancy_cmd, fancy_chead_code);
    primitive("rhead", fancy_cmd, fancy_rhead_code);
    primitive("lfoot", fancy_cmd, fancy_lfoot_code);
    primitive("cfoot", fancy_cmd, fancy_cfoot_code);
    primitive("rfoot", fancy_cmd, fancy_rfoot_code);
    primitive("fancyhead", fancy_cmd, fancy_head_code);
    primitive("fancyfoot", fancy_cmd, fancy_foot_code);
    primitive("fancyhf", fancy_cmd, fancy_hf_code);
    primitive("fancyinternal", xfancy_cmd);
    primitive("inert@thepage", xthepage_cmd);
}

// Todo Bouche
// \def\Q{\mathbb{Q}} $\bar \Q$
