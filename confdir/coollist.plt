%%% -*- latex -*-
\ProvidesPackage{coollist}[2008/04/24 v1.0  COntent Oriented LaTeX Lists]
%% based on a package by nsetzer
%% This file may be distributed and/or modified under the
%% conditions of the Limited General Product License


%% $Id: coollist.plt,v 2.1 2008/04/01 17:02:06 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\newboolean{COOL@found}

\newcommand{\setlistStop}[1]{}
\newcommand{\setlistEnd}[1]{}


\def\COOL@listgobble#1,{%
  \def\@tempa{#1}%
  \ifx\@tempa\@nnil
  \else
  \advance\@tempcntb by 1
  \ifnum\@tempcnta=\@tempcntb\COOL@foundtrue#1\fi
  \expandafter\COOL@listgobble\fi}

\newcommand{\listval}[2]{%
  \@tempcntb=0
  \@tempcnta=#2
  \COOL@foundfalse
  \COOL@listgobble#1,\@nil,}

\newcommand{\listlen}[1]{%
  \listval{#1}{0}%
   \number\@tempcntb}

\newcommand{\listlenstore}[2]{%
  \listval{#2}{0}%
  \setcounter{#1}{\@tempcntb}}

\def\COOL@liststore@gobbler#1,{%
  \def\@tmpa{#1}%
  \ifx\@tmpa\@nnil
  \else
  \advance\@tempcntb by 1
  \expandafter\gdef\csname \@tmpb\romannumeral\@tempcntb\endcsname{#1}%
  \expandafter\COOL@liststore@gobbler\fi}

\newcommand{\liststore}[2]{%
  \@tempcntb=0\def\@tmpb{#2}%
  \COOL@liststore@gobbler#1,\@nil,}

\endinput
