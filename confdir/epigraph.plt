%%%  -*- latex -*-
\ProvidesPackage{epigraph}[2008/11/03 v1.1 typesetting epigraphs]

%% $Id: epigraph.plt,v 2.1 2008/11/13 14:13:38 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% -----------------------------------------------------------------
%%   Author: Peter Wilson (CUA) now at peter.r.wilson@boeing.com until June 2004
%%                              (or at: pandgwilson at earthlink dot net)
%%   Copyright 2001, 2002, 2003, 2004 Peter R. Wilson

%% 

\newlength{\beforeepigraphskip}
\newlength{\afterepigraphskip}
\newlength{\epigraphwidth}
\newlength{\epigraphrule}
\newcommand{\epigraphsize}{\small}
\newcommand{\epigraphflush}{flushright}
\newcommand{\textflush}{flushleft}
\newcommand{\sourceflush}{flushright}


\newenvironment{flushepinormal}{}{}

\newcommand{\@epirule}{}
\newcommand{\@epitext}[1]{\xbox{epitext}{\XMLaddatt{rend}{\textflush}#1}}
\newcommand{\@episource}[1]{\xbox{episource}{\XMLaddatt{rend}{\sourceflush}#1}}

\newcommand{\epigraph}[2]{%
  \xbox{epigraph}{\def\\{\xbox{newline}{}}\@epitext{#1}\@episource{#2}}}

\newcommand{\qitem}[2]{\@epitext{#1}\@episource{#2}}
\newcommand{\qitemlabel}[1]{}

\newenvironment{epigraphs}{\begin{xmlelement}{epigraphs}}
  {\end{xmlelement}}

\providecommand{\cleartoevenpage}[1][\@empty]{}
\newcommand{\dropchapter}[1]{}
\newcommand{\undodrop}{}

\newcommand{\epigraphhead}[2][95]{%
  \xbox{epigraphhead}{\XMLaddatt{width}{#1}#2}}

\endinput

