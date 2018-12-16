% -*- latex -*-
\ProvidesPackage{RR}[2007/12/15 v1.4  Inria RR for Tralics]
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% $Id: RR.plt,v 2.12 2008/09/04 12:01:00 grimm Exp $

\DeclareOption{french}{\XMLaddatt[1]{RR-lang}{french}}
\DeclareOption{english}{\XMLaddatt[1]{RR-lang}{english}}
\DeclareOption{T1}{}
\DeclareOption{OT1}{}
\DeclareOption{noinputenc}{}
\DeclareOption{utf8}{}
\ProcessOptions\relax



\ifx\RRstyisuseful\relax\endinput\fi


\newcommand\UR@[2]{\XMLaddatt[1]{UR-val}{#1}\xbox{location}{#1#2}}
\newcommand\theme@[1]{\XMLaddatt[1]{theme-val}{#1}#1}

\newcommand\RRtitle[1]{{\def\\{; } \xbox{ftitle}{#1}}}
\newcommand\RRetitle[1]{{\def\\{; } \xbox{title}{#1}}}
\newcommand\RRauthor[1]{\xbox{author}{\def\and{\unskip~and~}#1}}
\newcommand\RRprojet[1]{\xbox{inria-team}{#1}}
\newcommand\RRdate[1]{\xbox{date}{#1}}
\newcommand\RRversion[1]{\xbox{version-number}{#1}}
\newcommand\RRdater[1]{\xbox{rev-date}{#1}}
\newcommand\RRNo[1]{\xbox{rrnumber}{#1}}
\newcommand\RRtheme[1]{\xbox{theme}{#1}}
\newcommand\keyword[1]{\xbox{keyword}{#1}}
\newcommand\motcle[1]{\xbox{motcle}{#1}}
\newcommand\THNum{\theme@{THnum}}
\newcommand\THCom{\theme@{THcom}}
\newcommand\THCog{\theme@{THcog}}
\newcommand\THSym{\theme@{THsym}}
\newcommand\THBio{\theme@{THbio}}

\newcommand\RCBordeaux{\UR@{Bordeaux}{ -- Sud Ouest}}
\newcommand\RCLille{\UR@{Lille}{ -- Nord Europe}}
\newcommand\RCParis{\UR@{Paris}{ -- Rocquencourt}}
\newcommand\RCSaclay{\UR@{Saclay}{ -- \^Ile-de-France}}
\newcommand\RCGrenoble{\UR@{Grenoble}{ -- Rh\^one-Alpes}}
\newcommand\RCNancy{\UR@{Nancy}{ -- Grand Est}}
\newcommand\RCRennes{\UR@{Rennes}{ -- Bretagne Atlantique}}
\newcommand\RCSophia{\UR@{Sophia}{ Antipolis -- M\'editerran\'ee}}

\let\URSophia\RCSophia
\let\URLorraine\RCNancy
\let\URRennes\RCRennes
\let\URRhoneAlpes\RCGrenoble
\let\URRocq\RCParis
%\let\URFuturs{\UR@{Futurs}{Futurs}}


\newcommand\RRresume[1]{\begin{xmlelement*}{resume}#1\end{xmlelement*}}
\newcommand\RRabstract[1]{\begin{xmlelement*}{abstract}#1\end{xmlelement*}}

\let\RRmotcle\motcle
\let\RRkeyword\keyword
\newcommand\titlehead[1]{\xbox{titlehead}{#1}}
\newcommand\authorhead[1]{\xbox{authorhead}{#1}}
\newcommand\RRnote[1]{\xbox{RRnote}{#1}}
\newcommand\email[1]{\xbox{RRemail}{#1}}

\def\makeRT{\XMLaddatt[1]{RR-type}{RT}}
\def\makeRR{\XMLaddatt[1]{RR-type}{RR}}

