%%% -*- latex -*-
\ProvidesPackage{abstract}[2007/12/09 v1.1 configurable abstracts]

%% $Id: abstract.plt,v 2.1 2007/12/12 17:13:26 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\DeclareOption{original}{}
\DeclareOption{addtotoc}{}
\DeclareOption{number}{}
\DeclareOption{runin}{}
\ProcessOptions\relax


\newcommand{\abstractnamefont}{\normalfont\small\bfseries}
\newcommand{\abstracttextfont}{\normalfont\small}

\newcommand{\absnamepos}{center}
\newlength{\abstitleskip} % unused dimensions
\newlength{\absleftindent}
\newlength{\absrightindent}
\newlength{\absparindent}
\newlength{\absparsep}

\newcommand{\abslabeldelim}[1]{}

\providecommand\abstractname{abstract}

\newenvironment{abstract}
{\begin{xmlelement*}{\abstractname}%
\AddAttToCurrent{pos}{\absnamepos}\@addnl
\abstracttextfont
}{\end{xmlelement*}\@addnl}

\newenvironment{onecolabstract}
{\begin{xmlelement*}{\abstractname}%
\AddAttToCurrent{pos}{\absnamepos}%
\AddAttToCurrent{onecolumn}{\false}\@addnl
\abstracttextfont
}{\end{xmlelement*}\@addnl}

\newcommand\saythanks{} % 

\endinput

