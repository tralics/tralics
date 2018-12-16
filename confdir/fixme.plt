%%%  -*- latex -*-
\ProvidesPackage{fixme}[2008/12/22 v1.0 FiXme]


%% $Id: fixme.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% Latex version: Copyright (C) 2007 Didier Verna

\RequirePackage{ifthen}
\RequirePackage{verbatim}

\newcommand\fxnote{\@ifnextchar[%]
  {\@fixme{note}}{\@@fixme{note}}}
\newcommand\fxwarning{\@ifnextchar[%]
  {\@fixme{warning}}{\@@fixme{warning}}}
\newcommand\fxerror{\@ifnextchar[%]
  {\@fixme{error}}{\@@fixme{error}}}
\newcommand\fixme{\@ifnextchar[%]
  {\@fixme{fatal}}{\@@fixme{fatal}}}

% Ignore optional argument
\def\@fixme#1[#2]#3{\xbox{FiXme}{\XMLaddatt{type}{#1}\XMLaddatt{opt}{#2}#3}}
\def\@@fixme#1#2{\xbox{FiXme}{\XMLaddatt{type}{#1}#2}}



\newcommand\FXInline[2]{}
\let\FXMargin\FXInline
\let\FXMarginClue\FXInline
\let\FXFootNote\FXInline
\newcommand\FXIndex[1]{}
\newcommand\FiXmeInline[1]{}
\newcommand\FiXmeMargin[1]{}
\newcommand\FiXmeFootnote[1]{}
\newcommand\FiXmeUser[1]{}
\newcommand\FiXmeIndex[1]{}
\newcommand\FiXmeInfo[1]{}
\newcommand\FiXmeWarning[1]{}
\newcommand\FXNote[1]{}
\newcommand\FXWarning[1]{}
\newcommand\FXError[1]{}
\newcommand\FXFatal[1]{}

\DeclareOption{inline}{}
\DeclareOption{noinline}{}
\DeclareOption{margin}{}
\DeclareOption{nomargin}{}
\DeclareOption{marginclue}{}
\DeclareOption{nomarginclue}{}
\DeclareOption{footnote}{}
\DeclareOption{nofootnote}{}
\DeclareOption{user}{}
\DeclareOption{nouser}{}
\DeclareOption{index}{}
\DeclareOption{noindex}{}
\DeclareOption{info}{}
\DeclareOption{noinfo}{}
\DeclareOption{warning}{}
\DeclareOption{nowarning}{}
\DeclareOption{silent}{}
\DeclareOption{nosilent}{}


\newcommand\@fxenvbegin[2]{\begin{xmlelement}{FiXmeEnv}
\XMLaddatt{type}{#1}\XMLaddatt{note}{#2}}
\newcommand\@fxenvend{\end{xmlelement}}
\newenvironment{anfxnote}[1][]{%
  \@fxenvbegin{fxnote}{#1}}{%
  \@fxenvend}
\newenvironment{anfxwarning}[1][]{%
  \@fxenvbegin{fxwarning}{#1}}{%
  \@fxenvend}
\newenvironment{anfxerror}[1][]{%
  \@fxenvbegin{fxerror}{#1}}{%
  \@fxenvend}
\newenvironment{afixme}[1][]{%
  \@fxenvbegin{fixme}{#1}}{%
  \@fxenvend}

\DeclareOption{english}{}
\DeclareOption{french}{}
\DeclareOption{francais}{}
\DeclareOption{italian}{}
\DeclareOption{german}{}
\DeclareOption{ngerman}{}
\DeclareOption{danish}{}
\DeclareOption{croatian}{}
\DeclareOption{final}{}
\DeclareOption{draft}{}

\ExecuteOptions{english,final,margin,nosilent}
\ProcessOptions*

\endinput
