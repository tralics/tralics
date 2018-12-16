% -*- latex -*-
% $Id: tipx.plt,v 2.2 2008/05/29 14:49:57 grimm Exp $
\NeedsTeXFormat{LaTeX2e}
\RequirePackage{tipa}
\ProvidesPackage{tipx}[2008/05/27 TIPA extensions version 1.0]

% tipx.sty: Additional Symbols for TIPA
% Copyright 2002 FUKUI Rei
%
% This program may be distributed and/or modified under the
% conditions of the LaTeX Project Public License, either version 1.2
% of this license or (at your option) any later version.
% The latest version of this license is in
%   http://www.latex-project.org/lppl.txt
% and version 1.2 or later is part of all distributions of LaTeX 
% version 1999/12/01 or later.

\let\textrevpolhook\c % well
\def\textspleftarrow{\tipa@mkhi{left}{A}}

\def\textrthooklong{\unimplemented}
\def\textpalhooklong{\unimplemented}
\def\textpalhookvar{\unimplemented}
\def\textrectangle{\unimplemented}
\def\textretractingvar{\unimplemented}
\def\textdownfullarrow{$\downarrow $}
\def\textupfullarrow{$\uparrow $}
\def\textsubrightarrow{\tipa@mkhi{sub-right}{A}}
\def\textsubdoublearrow{\tipa@mkhi{sub-double}{A}}
\def\textrhooka{\tipa@mkhi{rhook}{a}} 
\def\textinvscripta{\tipa@mkhi{inv-script}{a}}

\def\textaolig{\tipa@mkhi{lig}{ao}}
\def\textlhookfour{\tipa@mkhi{lhook}{4}}
\def\textstretchcvar{\tipa@mkhi{var}{^^^^0297}}
\def\textctstretchc{\tipa@mkhi{var}{^^^^0297}}
\def\textctstretchcvar{\tipa@mkhi{var}{^^^^0297}}
\def\textfrhookd{\tipa@mkhi{frhook}{d}}
\def\textfrhookdvar{\tipa@mkhi{frhook-var}{d}}
\def\textdblig{\tipa@mkhi{lig}{db}}
\def\textrhooke{e^^^^0322}
\def\textrhookepsilon{^^^^025b^^^^0322}
\def\textgrgamma{^^^^03b3}
\def\textfrtailgamma{\tipa@mkhi{frtail}{^^^^03b3}}
\def\textbktailgamma{\tipa@mkhi{bktail}{^^^^03b3}}
\def\textrtailhth{\tipa@mkhi{rtailht}{h}}
\def\textlhti{^^^^027f}
\def\textlhti{^^^^027f}
\def\textctjvar{\tipa@mkhi{ct-var}{j}}
\def\texthtbardotlessjvar{^^^^0284}
\def\textheng{\tipa@mkhi{heng}{h}}
\def\texthmlig{\tipa@mkhi{lig}{hm}}
\def\textfrbarn{\tipa@mkhi{frbar}{n}}
\def\textnrleg{^^^^019e}
\def\textfemale{^^^^2640}
\def\textuncrfemale{\tipa@mkhi{uncr}{^^^^2640}}
\def\textObullseye{\tipa@mkhi{var}{^^^^0298}}
\def\textrhookopeno{^^^^0254^^^^0322}
\def\textinvomega{\tipa@mkhi{inv}{^^^^03c9}}
\def\textlhookp{\tipa@mkhi{lhook}{p}}
\def\textqplig{\tipa@mkhi{lig}{qp}}
\def\textlooptoprevesh{^^^^01aa}
\def\textfrhookt{\tipa@mkhi{frhook}{t}}
\def\textctturnt{\tipa@mkhi{ct-turn}{t}}
\def\textturntwo{\tipa@mkhi{turn}{2}}
\def\textturnthree{\tipa@mkhi{turn}{3}}
\def\textbenttailyogh{^^^^01ba}
\def\textturnglotstop{\tipa@mkhi{cr}{^^^^0296}}
\def\textctinvglotstop{\tipa@mkhi{ct}{^^^^0296}}
\def\textpipevar{\tipa@mkhi{var}{\textpipe}}
\def\textdoublepipevar{\tipa@mkhi{var}{\textdoublepipe}}
\def\textdoublebarpipevar{\tipa@mkhi{var}{\textdoublebarpipe}}
\def\textlfishhookrlig{\tipa@mkhi{lig}{l^^^^027e}}
\def\textthornvari{\tipa@mkhi{var1}{^^^^00fe}}
\def\textthornvarii{\tipa@mkhi{var2}{^^^^00fe}}
\def\textthornvariii{\tipa@mkhi{var3}{^^^^00fe}}
\def\textthornvariv{\tipa@mkhi{var4}{^^^^00fe}}
\def\textglotstopvari{\tipa@mkhi{var1}{\textglotstop}}
\def\textglotstopvarii{\tipa@mkhi{var2}{\textglotstop}}
\def\textglotstopvariii{\tipa@mkhi{var3}{\textglotstop}}
\def\textinvsca{\tipa@mkhi{inv-sc}{a}}
\def\textscaolig{\tipa@mkhi{sc-lig}{ao}}
\def\textscdelta{\tipa@mkhi{s}{^^^^0394}}
\def\textscf{\tipa@mkhi{sc}{F}}
\def\textscq{\tipa@mkhi{sc}{Q}}
\def\textsck{\tipa@mkhi{sc}{K}}
\def\textscm{^^^^1d0d}
\def\textscp{^^^^1d18}
\def\textturnsck{\tipa@mkhi{sc}{^^^^029e}}
\def\textturnscu{\tipa@mkhi{turn-sc}{u}}
\def\textrevscr{^^^^044f}
\def\textrevscl{\tipa@mkhi{rev}{L}}

\endinput
