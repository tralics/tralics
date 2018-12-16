%%% -*- latex -*-
\ProvidesPackage{html}[2007/12/05 v1.0 Hypertext commands for latex2html]
% based on the html.sty package v1.38 by nd, hws and rrm

%% $Id: html.plt,v 2.3 2008/04/01 17:02:06 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


\providecommand\latextohtml{\textup{\LaTeX2\texttt{HTML}}}

\newcommand\html@nop[]{}
\newcommand\html@nopI[1]{}
\newcommand\html@nopII[2]{}
\newcommand\html@nopIII[3]{}
\newcommand\html@nopBI[1][]{}
\newcommand\html@nopBII[2][]{}
\newcommand\html@nopBIII[3][]{}


%\def\htmladdnormallinkfoot#1#2{\footnote{\href{#2}{#1}}}
%\def\htmladdnormallink#1#2{\href{#2}{#1}}

% 
\let\htmladdnormallinkfoot\@@href@foot
\let\htmladdnormallink\@@href

\newcommand\htmladdimg[2][]{\hyperimage{#2}}
\let\externallabels\html@nopBIII
\let\externalref\html@nopBII
\newcommand\externalcite[1][]{\nocite}
\newcommand\htmladdTOClink[4][]{}

\newcommand{\htmlrule}{\@ifstar\html@nopBI\html@nopBI}
%\renewcommand{\htmlrulestar}[1][all]{}
\let\htmlclear\html@nopBI
%\renewcommand{\htmlclear}[1][all]{}
\let\bodytext\html@nopI
\let\htmlbody\html@nopBI

% hyperref etc co
\newcommand\hyperref@hyper[4]{#4}
\newcommand\hyperref[1][]{\hyperref@i[#1]}

\def\hyperref@i[#1]{{\def\next{#1}%
 \def\tmp{}\ifx\next\tmp\aftergroup\hyperref@def
 \else\def\tmp{ref}\ifx\next\tmp\aftergroup\hyperref@def
 \else\def\tmp{pageref}\ifx\next\tmp\aftergroup\hyperref@pageref
 \else\def\tmp{page}\ifx\next\tmp\aftergroup\hyperref@pageref
 \else\def\tmp{noref}\ifx\next\tmp\aftergroup\hyperref@noref
 \else\def\tmp{no}\ifx\next\tmp\aftergroup\hyperref@noref
 \else\def\tmp{hyper}\ifx\next\tmp\aftergroup\hyperref@hyper
 \else\def\tmp{html}\ifx\next\tmp\aftergroup\hyperref@hyper
 \else\typeout{*** unknown option \next\space to  hyperref ***}%
 \fi\fi\fi\fi\fi\fi\fi\fi}}

\newcommand{\hyperref@def}[4]{#2\ref{#4}#3}
\newcommand{\hyperref@pageref}[4]{#2\pageref{#4}#3}
\newcommand{\hyperref@noref}[3]{#2}

\newcommand\hypercite[1][]{\hypercite@i[#1]}

\def\hypercite@i[#1]{{\def\next{#1}%
 \def\tmp{}\ifx\next\tmp\aftergroup\hypercite@def
 \else\def\tmp{int}\ifx\next\tmp\aftergroup\hypercite@def
 \else\def\tmp{cite}\ifx\next\tmp\aftergroup\hypercite@def
 \else\def\tmp{ext}\ifx\next\tmp\aftergroup\hypercite@nocite
 \else\def\tmp{nocite}\ifx\next\tmp\aftergroup\hypercite@nocite
 \else\def\tmp{no}\ifx\next\tmp\aftergroup\hypercite@nocite
 \else\typeout{*** unknown option \next\space to  hypercite ***}%
 \fi\fi\fi\fi\fi\fi}}

\newcommand\hypercite@def[4]{#2{\def\tmp{#3}\def\emptyopt{}%
 \ifx\tmp\emptyopt\cite{#4}\else\cite[#3]{#4}\fi}}
\newcommand\hypercite@nocite[2]{#2\hypercite@nocitex}
\newcommand\hypercite@nocitex[2][]{\nocite{#2}}


\newcommand\htmlref[2][]{#2{\def\tmp{#1}\ifx\tmp\@empty
  \aftergroup\html@nopI\else\aftergroup\html@nopBII\fi}}

\newcommand{\htmlcite}[2][]{#2{\def\tmp{#1}\ifx\tmp\@empty
  \aftergroup\htmlcite@def\else\aftergroup\html@nopBII\fi}}
\newcommand{\htmlcite@def}[1]{ \nocite{#1}}

\let\htmlimage\html@nopBII
\let\htmlborder\html@nopBII
\newenvironment{makeimage}{}{}
\newenvironment{tex2html_deferred}{}{}
\let\htmladdtonavigation\html@nopI

% comment

\def\htmlincludecomment
 #1{\expandafter\def\csname#1\endcsname{}%
    \expandafter\def\csname end#1\endcsname{}}
\def\htmlexcludecomment#1{%
 \expandafter\let\csname#1\endcsname\comment
 \expandafter\let\csname end#1\endcsname\endcomment}

% \htmlexcludecomment{comment}

\let\includecomment=\htmlincludecomment
\let\excludecomment=\htmlexcludecomment
\let\htmlreexcludecomment=\htmlexcludecomment

\htmlexcludecomment{rawhtml}
\htmlexcludecomment{htmlonly}
\let\html\html@nopI
\htmlexcludecomment{imagesonly}
% \newenvironment{latexonly}{}{}
\newcommand\latex[1]{#1}
\long\def\latexhtml#1#2{#1}

\let\htmltracing\html@nopI
\let\htmltracenv\html@nopI
\let\strikeout\html@nopI

\let\htmlurl=\url
\let\HTMLcode\html@nopBII
\let\HTML\html@nopBII
\let\HTMLset\html@nopBIII
\let\HTMLsetenv\html@nopBIII
\let\internal\html@nopBII
\let\htmlhead\html@nopBIII
\let\htmlnohead\html@nop
\let\htmlbase\html@nopI
\let\htmlsetstyle\html@nopBIII
\let\htmladdtostyle\html@nopBIII
\let\htmllanguagestyle\html@nopBII
\let\startdocument\html@nop
\newcommand\tableofchildlinks{\@ifstar\html@nopBI\html@nopBI}
\newcommand\htmlinfo{\@ifstar\html@nopBI\html@nopBI}

%  This redefines  \begin  to allow for an optional argument
%  which is used by LaTeX2HTML to specify `style-sheet' information

\let\realLaTeX@begin=\begin
\renewcommand{\begin}[1][]{\realLaTeX@begin}


\@ifundefined{c@part}{\newcounter{part}}{}%
\@ifundefined{c@chapter}{\newcounter{chapter}}{}
\newcounter{lpart}
\newcounter{lchapter}[part]
\newcounter{lsection}[part]
\newcounter{lsubsection}[section]
\newcounter{lsubsubsection}[subsection]
\newcounter{lparagraph}[subsubsection]
\newcounter{lsubparagraph}[paragraph]
\newcounter{lsubsubparagraph}[lsubparagraph]

\let\Hpart=\part
%\let\Hchapter=\chapter
\let\Hsection=\section
\let\Hsubsection=\subsection
\let\Hsubsubsection=\subsubsection
\let\Hparagraph=\paragraph
\let\Hsubparagraph=\subparagraph
\let\Hsubsubparagraph=\subsubparagraph

\let\resetsections\html@nop
\let\resetsubsections\html@nop
\let\resetsubsubsections\html@nop
\let\resetparagraphs\html@nop
\let\resetsubparagraphs\html@nop
\let\resetsubsubparagraphs\html@nop


\let\DumpPtr\html@nopII
%\newwrite\ptrfile
\def\DumpCounters#1#2#3#4{}


\let\dumpcitestatus\html@nop
\let\loadcitestatus\html@nop
\let\dumpcurrentcolor\html@nop
\let\loadsegmentcolors\html@nop
\let\segmentpagecolor\html@nopBI
\let\segmentcolor\html@nopBII


\newcommand\segment{\@ifstar{\html@nopBI}{\html@nopBI}}
\let\@endsegment\html@nopBI
\let\endsegment\@endsegment

\def\hyperimage#1{\includegraphics{#1}}

\endinput
