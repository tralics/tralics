% -*- latex -*-
% $Id: tipa.plt,v 2.6 2008/05/29 14:49:57 grimm Exp $
\ProvidesPackage{tipa}[2008/05/19 TIPA version 1.0]

% ------------------------------------------------------------
% Copyright 2002 FUKUI Rei
%
% This program may be distributed and/or modified under the
% conditions of the LaTeX Project Public License, either version 1.2
% of this license or (at your option) any later version.
% The latest version of this license is in
%   http://www.latex-project.org/lppl.txt
% and version 1.2 or later is part of all distributions of LaTeX 
% version 1999/12/01 or later.
%
% This program consists of all files listed in Manifest.txt.
%
% Version 1.0 1996/06/10 first LaTeX2e version
% Version 1.1 2001-2002: Encoding modified: see t3enc.def.
%
% This file is based on `tsipa.sty' for TSIPA created by
%   KOBAYASHI Hajime, FUKUI Rei and SHIRAKAWA Shun.
%
% Changed by Rafael Labossiere <rafael@icp.inpg.fr> 
% on Tue Oct 20 10:10:01 CEST 1998
% \RequirePackage is used instead of \usepackage to increase
% compatibility with Latex2e  
%
% Modified to be reloadable by Dominique Unruh.
%
% ------------------------------------------------------------

\NeedsTeXFormat{LaTeX2e}

\DeclareOption{extra}{} % built in 
\DeclareOption{tone}{}  % built in
\DeclareOption{T1}{}
\DeclareOption{noenc}{}

\newif\if@ipasafe \global\@ipasafefalse
\DeclareOption{safe}{\global\@ipasafetrue}

\ProcessOptions


%
% TSIPA, TIPA logos
%
\newcommand\tsipa{\textsf{TSIPA}\@}
\newcommand\tipa{\textsf{TIPA}\@}
\newcommand\tipx{\textsf{TIPX}\@}

\let\super\textsuperscript

\def\tipa@mkhi#1#2{\xbox{hi}{\XMLaddatt{rend}{#1}#2}}

\def\textturna{^^^^0250}
\def\textscripta{^^^^0251}
\def\textturnscripta{^^^^0252}
\def\textturnv{^^^^028c}
\def\textcrb{^^^^0180}
\def\textbarb{b^^^^0336}
\def\textsoftsign{^^^^044c}
\def\texthardsign{^^^^044a}
\def\texthtb{^^^^0253}
\def\textscb{^^^^0299}
\def\textbeta{^^^^03b2}
\def\texthtc{^^^^0188} 
\def\textctc{^^^^0255}
\def\textbarc{c^^^^0336}
\def\textstretchc{^^^^0297}
\def\textcrd{^^^^0111}
\def\textbard{d^^^^0336}
\def\texthtd{^^^^0257}
\def\textrtaild{^^^^0256}
\def\textdzlig{^^^^02a3}
\def\textdctzlig{^^^^02a5}
\def\textdyoghlig{^^^^02a4}
\def\textschwa{^^^^0259}
\def\textrhookschwa{^^^^025a}
\def\textreve{^^^^0258}
\def\textepsilon{^^^^025b}
\def\textcloseepsilon{^^^^029a}
\def\textrevepsilon{^^^^025c}
\def\textrhookrevepsilon{^^^^025d}
\def\textcloserevepsilon{^^^^025e}
\def\textscriptg{^^^^0261}
\def\textbarg{^^^^0261^^^^0336}
\def\textcrg{^^^^0261^^^^0330}
\def\texthtg{^^^^0261}
\def\textscg{^^^^0262}
\def\texthtscg{^^^^029b}
\def\textgamma{^^^^0263}
\def\textramshorns{^^^^0264}
\def\texthvlig{^^^^0195}
\def\textcrh{^^^^0127}
\def\texthth{^^^^0266}
\def\textctd{^^^^0221}
\def\texththeng{^^^^0267}
\def\textturnh{^^^^0265}
\def\textsch{^^^^029c}
\def\textbari{^^^^0268}
\def\textsci{^^^^026a}
\def\textiota{^^^^0269}
\def\textlhtlongi{\tipa@mkhi{long}{^^^^027f}}
\def\textvibyi{^^^^0285}
\def\textraisevibyi{\tipa@mkhi{raise}{^^^^0285}}
\def\textctj{^^^^029d}
\def\textbardotlessj{^^^^025f}
\def\textObardotlessj{^^^^025f}
\def\texthtbardotlessj{\tipa@mkhi{var}{^^^^0284}}
\def\texthtk{^^^^0199}
\def\textturnk{^^^^029e}
\def\textltilde{^^^^026b}
\def\textbarl{^^^^019a}
\def\textbeltl{^^^^026c}
\def\textrtaill{^^^^026d}
\def\textlyoghlig{{^^^^026e}}
\def\textscl{^^^^029f}
\def\textlambda{^^^^03bb}
\def\textcrlambda{^^^^019b}
\def\textltailm{^^^^0271}
\def\textturnm{^^^^026f}
\def\textturnmrleg{^^^^0270}
\def\textltailn{^^^^0272}
\def\textrtailn{^^^^0273}
\def\textctn{^^^^0235}
\def\textscn{^^^^0274}
\def\textbullseye{^^^^0298}
\def\textbaro{^^^^0275}
\def\textscoelig{^^^^0276}
\def\textopeno{^^^^0254}
\def\textomega{^^^^03c9}
\def\textcloseomega{^^^^0277}
\def\textphi{^^^^0278}
\def\texthtp{^^^^01a5}
\def\textwynn{^^^^01bf}
\def\textthorn{^^^^00fe} % \th
\def\texthtq{^^^^02a0}
\def\textfishhookr{^^^^027e}
\def\textlonglegr{^^^^027c}
\def\textrtailr{^^^^027d}
\def\textturnr{^^^^0279}
\def\textturnrrtail{^^^^027b}
\def\textturnlonglegr{^^^^027a}
\def\textscr{^^^^0280}
\def\textinvscr{^^^^0281}
\def\textrtails{^^^^0282}
\def\textesh{^^^^0283} 
\def\textctesh{^^^^0286} 
\def\textrtailt {^^^^0288}
\def\textturnt {^^^^0287}
\def\textctt {^^^^0236}
\def\texttctclig{^^^^02a8}
\def\texttslig{^^^^02a6}
\def\textteshlig{^^^^02a7}
\def\texttheta{^^^^03b8}
\def\textbaru{^^^^0289}
\def\textupsilon{^^^^028a}
\def\textscriptv{^^^^028b}
\def\textturnw{^^^^028d}
\def\textchi{^^^^03c7}
\def\textturny{^^^^028e}
\def\textscy{^^^^028f}
\def\textlhtlongy{^^^^02ae}
\def\textvibyy{^^^^02af}
\def\textcommatailz{^^^^0225}
\def\textctz{^^^^0291}
\def\textrtailz{^^^^0290}
\def\textcrtwo{^^^^01bb}
\def\textyogh{^^^^0292}

\def\textctyogh{^^^^0293}
\def\textrevyogh{^^^^01b9}
\def\textglotstop{^^^^0294}
\def\textbarglotstop{^^^^02a1}
\def\textinvglotstop{^^^^0296}
\def\textrevglotstop{^^^^0295}
\def\textbarrevglotstop{^^^^02a2}
\def\textpipe{^^^^01c0}
\def\textdoublebarpipe{^^^^01c2}
\def\textdoublepipe{^^^^01c1}
\def\textexclamationpoint{^^^^01c3} % retroflex click

\def\textprimstress{^^^^02c8}
\def\textsecstress{^^^^02cc}
\def\textlengthmark{^^^^02d0}
\def\texthalflength{^^^^02d1}
\def\textvertline{|}
\def\textdoublevertline{||}
\def\textbottomtiebar{^^^^203f}
\def\textdownstep{\tipa@mkhi{down}{A}}
\def\textupstep{\tipa@mkhi{up}{A}}
\def\textglobfall{$\searrow $}
\def\textglobrise{$\nearrow $}


\def\tone#1{\csname tone@#1\endcsname}
\expandafter\def\csname tone@55\endcsname{^^^^02e5}
\expandafter\def\csname tone@44\endcsname{^^^^02e6}
\expandafter\def\csname tone@33\endcsname{^^^^02e7}
\expandafter\def\csname tone@22\endcsname{^^^^02e8}
\expandafter\def\csname tone@11\endcsname{^^^^02e9}


\def\texthighrise#1{#1^^^^0304^^^^0341}
\def\textlowrise#1{#1^^^^0340^^^^0304}
\def\textrisefall#1{#1^^^^0340^^^^0341^^^^0340}
\def\textfallrise#1{#1^^^^0341^^^^0340^^^^0341}



\let\textpolhook\k
\let\textdoublegrave\k
\def\textsubgrave#1{#1^^^^0316}
\def\textsubacute#1{#1^^^^0317}
\def\textsubcircum#1{#1^^^^032d}
\def\textroundcap#1{#1^^^^0311}
\def\textacutemacron#1{#1^^^^0301^^^^0304} % ugly
\def\textacutemacron#1{#1^^^^0301^^^^0331}
\def\textgravemacron#1{#1^^^^0300^^^^0304} % ugly
\def\textgravemacron#1{#1^^^^0300^^^^0331}
\def\textvbaraccent#1{#1^^^^030d}
\def\textdoublevbaraccent#1{#1^^^^030e}
\def\textgravedot#1{#1^^^^0340^^^^0307} % ugly
\def\textgravedot#1{#1^^^^0340^^^^0323}
\def\textdotacute#1{#1^^^^0307^^^^0341} % ugly
\def\textdotacute#1{#1^^^^0301^^^^0323}
\def\textcircumdot#1{#1^^^^0302^^^^0307} % ugly
\def\textcircumdot#1{#1^^^^0302^^^^0323}
\def\texttildedot#1{#1^^^^0342^^^^0307} % ugly
\def\texttildedot#1{#1^^^^0342^^^^0323}
\def\textbrevemacron#1{#1^^^^0304^^^^0306} % ugly
\def\textbrevemacron#1{#1^^^^0320^^^^0306}
\def\textringmacron#1{#1^^^^0304^^^^030a} % ugly
\def\textringmacron#1{#1^^^^0331^^^^030a}
\def\textacutewedge#1{#1^^^^030c^^^^0301} % ugly
\def\textacutewedge#1{#1^^^^0301^^^^032c}
\def\textdotbreve#1{#1^^^^0310}
\def\textsubbridge#1{#1^^^^032a}
\def\textinvsubbridge#1{#1^^^^033a}
\def\textsubsquare#1{#1^^^^033b}
\def\textsubrhalfring#1{#1^^^^0339}
\def\textsublhalfring#1{#1^^^^031c}
\def\textsubw#1{#1^^^^032b}
\def\textoverw#1{#1\unimplemented}
\def\textseagull#1{#1^^^^033c}
\def\textovercross#1{#1^^^^033d}
\def\textsubplus#1{#1^^^^031f}
\def\textraising#1{#1^^^^031d}
\def\textlowering#1{#1^^^^031e}
\def\textadvancing#1{#1^^^^0318}
\def\textretracting#1{#1^^^^0319}
\def\textsubtilde#1{#1^^^^0330}
\def\textsubumlaut#1{#1^^^^0324}
\def\textsubring#1{#1^^^^0325}
\def\textsubwedge#1{#1^^^^032c}
\def\textsubbar#1{#1^^^^0331}
\def\textsubdot#1{#1^^^^0323}
\def\textsubarch#1{#1^^^^032f}
\def\textsyllabic#1{#1^^^^0329}
\def\textsuperimposetilde#1{#1^^^^0334}
\def\textcorner{^^^^02fa}
\def\textopencorner{^^^^02f9}
\def\textrhoticity{^^^^02de}
\def\textceltpal{^^^^02ca} % OK ?
\def\textlptr{\super{<}}
\def\textrptr{\super{>}}
\def\textsca{^^^^1d00}
\def\textsce{^^^^1d07}
\def\textscj{^^^^1d0a}
\def\textscu{^^^^1d1c}

\def\texttoptiebar#1#2{#1^^^^0361#2}





\def\spreadlips#1{#1^^^^034d}
\def\overbridge#1{#1^^^^0346}
\def\bibridge#1{#1^^^^0346^^^^032a}
\def\subdoublebar#1{#1^^^^0347}
\def\subdoublevert#1{#1^^^^0348}
\def\subcorner#1{#1^^^^0349}
\def\whistle#1{#1^^^^034e}
\def\sliding#1{#1^^^^0362}
\def\crtilde#1{#1^^^^034a}
\def\dottedtilde#1{#1\unimplemented}
\def\doubletilde#1{#1^^^^034c}
\def\partvoiceless#1{#1\unimplemented}
\def\inipartvoiceless#1{#1\unimplemented}
\def\finpartvoiceless#1{#1\unimplemented}
\def\partvoice#1{#1\unimplemented}
\def\inipartvoice#1{#1\unimplemented}
\def\finpartvoice#1{#1\unimplemented}
\def\sublptr#1{#1^^^^0354}
\def\subrptr#1{#1^^^^0355}

% Missing
\def\textdoublebarslash{=^^^^0338}
\def\textraiseglotstop{\tipa@mkhi{raise}{\textglotstop}}
\def\textlhookt{\tipa@mkhi{lhook}{t}}
\def\texthtt{^^^^01ad}

\def\textdoublebaresh{\tipa@mkhi{doublebar}{\textesh}}
\def\textOlyoghlig{\tipa@mkhi{var}{^^^^026e}}
\def\textbabygamma{\tipa@mkhi{var}{^^^^0264}}
\def\textturncelig{\tipa@mkhi{lig}{\textopeno e}}
\def\textctdctzlig{\tipa@mkhi{lig}{\textctd\textctz}}
\def\textcttctclig{\tipa@mkhi{lig}{tc}}
\def\textscomega{\tipa@mkhi{sc}{^^^^03a9}}
\def\texthtrtaild{\tipa@mkhi{ht-rtail}{d}}



% Macros for font selection and IPA environment
%% Not yet implemented
\newcommand\textipa[1]{\unimplemented}
\newenvironment{IPA}{\unimplemented}{}


\newcommand\nrsuper[1]{\leavevmode\textsuperscript{#1}}

%
% TIPA additional commands. These commands may be `dangerous' in that
% having very short names or overriding existing commands. For example:
% \* has been defined in plain TeX.
% \| has been defined in LaTeX's math mode for the symbol `||'
% \:, \; and \! have been defined in LaTeX's math mode for various spacing.
%
% Note that definition of these additional commands are suppressed if
% the option `safe' has been specified.
%
% The following three lines added on 2002/08/06
 \let\tipamedspace\:
 \let\tipathickspace\;
 \let\tipanegthinspace\!
 \let\tipasterisktmp\*
 \let\tipapipetmp\|
%% \def\s{...}
%% \def\*{...}
%% \def\|#1#2{\@omniaccent{#1}{#2}}
%% \def\!{....}

% \* is \tipa@star
% \; is \tipa@semi
% \: is \tipa@colon
% \! is \tipa@exclam
% 
% \! for implosives and clicks.

%%added JG
\newcommand{\tipaunsafemode}{%
  \let\|\@omniaccent
\let\*\tipa@star
\let\; \tipa@semi
\let\:\tipa@colon
\let\!\tipa@exclam
}

\def\@omniaccent#1#2{%
  \if #1[\textsubbridge{#2}\else
  \if #1]\textinvsubbridge{#2}\else
  \if #1(\textsublhalfring{#2}\else
  \if #1)\textsubrhalfring{#2}\else
  \if #1+\textsubplus{#2}\else
  \if #1'\textraising{#2}\else
  \if #1`\textlowering{#2}\else
  \if #1<\textadvancing{#2}\else
  \if #1>\textretracting{#2}\else
  \if #1x\textovercross{#2}\else
  \if #1w\textsubw{#2}\else
  \if #1c\textroundcap{#2}\else
  \if #1m\textseagull{#2}\else #1#2%
  \fi\fi\fi\fi\fi\fi\fi\fi\fi\fi\fi\fi\fi}


% Added on 2002/08/08
\newcommand{\tipasafemode}{%
 \let\:\tipamedspace
 \let\;\tipathickspace
 \let\!\tipanegthinspace
 \let\*\tipaasterisktmp
 \let\|\tipapipetmp}

% a command suggested by Christian Folini.

\newcommand{\sups}[2]{\textipa{\tipaUpperaccent[.2ex]{%
  \lower.8ex\hbox{\super{#2}}}{#1}}}

\def\textcrinvglotstop{\tipa@mkhi{cr}{^^^^0296}}

\def\texttoneletterstem{\unimplemented}
\def\textrevapostrophe{\unimplemented}
\def\texthooktop{\unimplemented}
\def\textrthook{\unimplemented}
\def\textpalhook{\unimplemented}
\def\textmidacute#1{\unimplemented#1}
%\DeclareTextAccentDefault\textgravemid{T3}      % Grave-Macron Ligature
%\DeclareTextAccentDefault\textgravecircum{T3}   % Grave-Wedge Ligature
%\DeclareTextAccentDefault\textcircumacute{T3}   % Circumflex-Acute Ligature

\def\t{\@ifstar{\textbottomtiebar} {\texttoptiebar}}

\endinput
